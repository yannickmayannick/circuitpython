/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Jeff Epler for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <string.h>

#include "py/runtime.h"
#include "py/mperrno.h"

#include "common-hal/canio/CAN.h"
#include "shared-bindings/microcontroller/Pin.h"
#include "shared-bindings/util.h"
#include "supervisor/port.h"
#include "supervisor/shared/tick.h"

#include "sdk/drivers/flexcan/fsl_flexcan.h"

static CAN_Type *const flexcan_bases[] = CAN_BASE_PTRS; // e.g.: { (CAN_Type *)0u, CAN1, CAN2, CAN3 }
static canio_can_obj_t *can_objs[MP_ARRAY_SIZE(mcu_can_banks)];

// Get frequency of flexcan clock.
#define MIMXRT10XX_FLEXCAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (CLOCK_GetDiv(kCLOCK_CanDiv) + 1))

static void config_periph_pin(const mcu_periph_obj_t *periph) {
    if (!periph) {
        return;
    }
    IOMUXC_SetPinMux(
        periph->pin->mux_reg, periph->mux_mode,
        periph->input_reg, periph->input_idx,
        0,
        0);

    IOMUXC_SetPinConfig(0, 0, 0, 0,
        periph->pin->cfg_reg,
        IOMUXC_SW_PAD_CTL_PAD_PUS(0) // Pull Up/Down Config. Field: 100K Ohm Pull Down
        #if IMXRT10XX
        | IOMUXC_SW_PAD_CTL_PAD_HYS(0) // Hyst. Enable Field: Hysteresis Disabled
        | IOMUXC_SW_PAD_CTL_PAD_PKE(1) // Pull/Keep Enable Field: Pull/Keeper Enabled
        | IOMUXC_SW_PAD_CTL_PAD_SPEED(2) // Speed Field: medium (100MHz)
        #endif
        | IOMUXC_SW_PAD_CTL_PAD_PUE(0) // Pull/Keep Select Field: Keeper
        | IOMUXC_SW_PAD_CTL_PAD_ODE(0) // Open Drain Enable Field: Open Drain Disabled
        | IOMUXC_SW_PAD_CTL_PAD_DSE(6) // Drive Strength Field: R0/6
        | IOMUXC_SW_PAD_CTL_PAD_SRE(0)); // Slew Rate Field: Slow Slew Rate
}

static uint8_t mimxrt10xx_flexcan_get_free_tx_mbid(canio_can_obj_t *self) {
    // Get the next free tx message buffer atomically so that an interrupt can't occur while
    // doing bit search.
    //
    // The idea here is that in an integer each bit acts as a boolean telling us if a
    // tx message buffer is in use or not.
    // If a free message buffer is found then we set it as used and return it's index.
    bool found_free_tx_mb = false;
    int8_t tx_array_id = 0;
    mp_uint_t atomic_state = MICROPY_BEGIN_ATOMIC_SECTION();
    for ( ; tx_array_id < MIMXRT10XX_FLEXCAN_TX_MB_NUM ; ++tx_array_id)
    {
        uint64_t tx_array_id_bit = (1UL << tx_array_id);
        if (!(self->data->tx_state & tx_array_id_bit))
        {
            // Found a free tx array id. Mark it as used.
            self->data->tx_state |= tx_array_id_bit;
            found_free_tx_mb = true;
            break;
        }
    }
    MICROPY_END_ATOMIC_SECTION(atomic_state);

    if (!found_free_tx_mb) {
        mp_raise_ValueError(MP_ERROR_TEXT("Unable to send CAN Message: all tx message buffer is busy"));
    }

    return MIMXRT10XX_FLEXCAN_TX_ARRID_TO_MBID(tx_array_id);
}

static void mimxrt10xx_flexcan_set_tx_mb_free_by_mbid(canio_can_obj_t *self, uint8_t mb_idx)
{
    // We simply set the Nth bit zero. This means that that message buffer is free to use.
    uint64_t tx_array_id_bit = (1UL << MIMXRT10XX_FLEXCAN_TX_MBID_TO_ARRID(mb_idx));
    self->data->tx_state &= ~(tx_array_id_bit);
}

static void mimxrt10xx_flexcan_set_tx_mb_busy_by_mbid(canio_can_obj_t *self, uint8_t mb_idx)
{
    // We simply set the Nth bit 1. This means that that message buffer is busy and cannot be used.
    uint64_t tx_array_id_bit = (1UL << MIMXRT10XX_FLEXCAN_TX_MBID_TO_ARRID(mb_idx));
    self->data->tx_state |= tx_array_id_bit;
}

static void mimxrt10xx_flexcan_abort_tx_frames(canio_can_obj_t *self)
{
    for (uint8_t tx_array_id = 0 ; tx_array_id < MIMXRT10XX_FLEXCAN_TX_MB_NUM ; ++tx_array_id)
    {
        uint64_t tx_array_id_bit = (1UL << tx_array_id);
        if (self->data->tx_state & tx_array_id_bit)
        {
            // Found a used/busy tx message buffer. Abort it.
            FLEXCAN_TransferAbortSend(self->data->base, &self->data->handle, MIMXRT10XX_FLEXCAN_TX_ARRID_TO_MBID(tx_array_id));

            self->data->tx_state &= ~(tx_array_id_bit); // Mark tx message buffer as free.
        }
    }
}

static void mimxrt10xx_flexcan_handle_error(canio_can_obj_t *self) {
    canio_bus_state_t state = common_hal_canio_can_state_get(self);
    if (state == BUS_STATE_OFF)
    {
        // Abort any pending tx and rx in case of bus-off.
        mimxrt10xx_flexcan_abort_tx_frames(self);
        FLEXCAN_TransferAbortReceiveFifo(self->data->base, &self->data->handle);
    }
}

static void mimxrt10xx_flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *selfPtr)
{
    (void) base; // unused variable
    (void) handle; // unused variable
    // The result field can either be a message buffer index or a status flags value.

    canio_can_obj_t *self = (canio_can_obj_t*) selfPtr;

    switch (status) {

        // Process rx message buffer is idle event.
        case kStatus_FLEXCAN_RxIdle:
            // We don't control any rx message buffers 'manually'. The rx fifo has control.
            break;

        // Process tx message buffer is idle event.
        case kStatus_FLEXCAN_TxIdle:
            mimxrt10xx_flexcan_set_tx_mb_free_by_mbid(self, result);
            break;

        // Process tx message buffer is busy event.
        case kStatus_FLEXCAN_TxBusy:
            mimxrt10xx_flexcan_set_tx_mb_busy_by_mbid(self, result);
            break;

        // Process remote message is send out and message buffer changed to receive one event.
        case kStatus_FLEXCAN_TxSwitchToRx:
            mimxrt10xx_flexcan_set_tx_mb_free_by_mbid(self, result);
            break;

        // Process rx message buffer is busy event.
        case kStatus_FLEXCAN_RxBusy:
            break;

        // Process rx message buffer is overflowed event.
        case kStatus_FLEXCAN_RxOverflow:
            break;

        // Process rx message fifo is busy event.
        case kStatus_FLEXCAN_RxFifoBusy:
            break;

        // Process rx message fifo is idle event.
        case kStatus_FLEXCAN_RxFifoIdle:
            break;

        // Process rx message fifo is overflowed event.
        case kStatus_FLEXCAN_RxFifoOverflow:
            break;

        // Process rx message fifo is almost overflowed event.
        case kStatus_FLEXCAN_RxFifoWarning:
            break;

        // Process rx message fifo is disabled during reading event.
        case kStatus_FLEXCAN_RxFifoDisabled:
            break;

        // Process FlexCAN is waken up from stop mode event.
        case kStatus_FLEXCAN_WakeUp:
            break;

        // Process unhandled interrupt asserted event.
        case kStatus_FLEXCAN_UnHandled:
        // Process FlexCAN module error and status event.
        case kStatus_FLEXCAN_ErrorStatus:
            // We could do some fancy statistics update, but canio does not have.
            mimxrt10xx_flexcan_handle_error(self);
            break;
    }
}

void common_hal_canio_can_construct(canio_can_obj_t *self, const mcu_pin_obj_t *tx, const mcu_pin_obj_t *rx, int baudrate, bool loopback, bool silent) {

    int instance = -1;
    const mcu_periph_obj_t *rx_periph = find_pin_function(mcu_can_rx_list, rx, &instance, MP_QSTR_rx);
    const mcu_periph_obj_t *tx_periph = find_pin_function(mcu_can_tx_list, tx, &instance, MP_QSTR_tx);

    // mp_printf(&mp_plat_print, "can instance: %d\n", instance);
    // mp_printf(&mp_plat_print, "can loopback: %d\n", loopback ? 1 : 0);
    // mp_printf(&mp_plat_print, "can silent: %d\n", silent ? 1 : 0);
    // mp_printf(&mp_plat_print, "can baudrate: %d\n", baudrate);

    self->rx_pin = rx;
    self->tx_pin = tx;
    config_periph_pin(rx_periph);
    config_periph_pin(tx_periph);

    self->loopback = loopback;
    self->silent = silent;
    self->baudrate = baudrate;

    self->data = m_new_obj(mimxrt10xx_flexcan_data_t);
    self->data->base = flexcan_bases[instance]; // 'flexcan_bases' start indexing from 1. (The first element is NULL)
    self->data->tx_state = 0;

    // Get flexcan module default configuration.
    flexcan_config_t config;
    FLEXCAN_GetDefaultConfig(&config);

    // Change default flexcan module configuration based on canio constructor parameters.
    config.clkSrc               = CLOCK_GetMux(kCLOCK_CanMux);
    config.baudRate             = baudrate;
    config.enableLoopBack       = loopback;
    config.enableListenOnlyMode = silent;
    config.maxMbNum             = 64;
    config.enableIndividMask    = true;
    // config.disableSelfReception = true; // TODO: do we want to disable this?

    #if (defined(MIMXRT10XX_FLEXCAN_USE_IMPROVED_TIMING_CONFIG) && MIMXRT10XX_FLEXCAN_USE_IMPROVED_TIMING_CONFIG)
        // If improved timing configuration is enabled then tell the SDK to calculate it.
        flexcan_timing_config_t timing_config;
        memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
        if (FLEXCAN_CalculateImprovedTimingValues(self->data->base, config.baudRate, MIMXRT10XX_FLEXCAN_CLK_FREQ, &timing_config))
        {
            // SDK could calculate the improved timing configuration. Yay!
            // Let's update our flexcan module config to use it.
            memcpy(&(config.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
        }
    #endif

    // Initialize the flexcan module with user-defined settings.
    FLEXCAN_Init(self->data->base, &config, MIMXRT10XX_FLEXCAN_CLK_FREQ);

    // Create FlexCAN handle structure and set call back function.
    // As callback data we set 'self'. In callback we can cast it back to 'canio_can_obj_t'.
    FLEXCAN_TransferCreateHandle(self->data->base, &self->data->handle, mimxrt10xx_flexcan_callback, (void*)self);

    // Set rx mask to don't care on all bits.
    FLEXCAN_SetRxMbGlobalMask(self->data->base, FLEXCAN_RX_MB_EXT_MASK(0x00, 0, 0));
    FLEXCAN_SetRxFifoGlobalMask(self->data->base, FLEXCAN_RX_MB_EXT_MASK(0x00, 0, 0));

    flexcan_rx_fifo_config_t fifo_config;
    fifo_config.idFilterNum = 0;
    fifo_config.idFilterTable = self->data->rx_fifo_filter;
    fifo_config.idFilterType = kFLEXCAN_RxFifoFilterTypeA;
    fifo_config.priority = kFLEXCAN_RxFifoPrioHigh;
    FLEXCAN_SetRxFifoConfig(self->data->base, &fifo_config, true);

    claim_pin(rx);
    claim_pin(tx);

    can_objs[instance] = self;
}

bool common_hal_canio_can_loopback_get(canio_can_obj_t *self) {
    return self->loopback;
}

int common_hal_canio_can_baudrate_get(canio_can_obj_t *self) {
    return self->baudrate;
}

int common_hal_canio_can_transmit_error_count_get(canio_can_obj_t *self) {
    uint8_t tx_err_cnt; // Transmit error counter.
    FLEXCAN_GetBusErrCount(self->data->base, &tx_err_cnt, NULL);
    return tx_err_cnt;
}

int common_hal_canio_can_receive_error_count_get(canio_can_obj_t *self) {
    uint8_t rx_err_cnt; // Transmit error counter.
    FLEXCAN_GetBusErrCount(self->data->base, NULL, &rx_err_cnt);
    return rx_err_cnt;
}

canio_bus_state_t common_hal_canio_can_state_get(canio_can_obj_t *self) {
    uint64_t status_flags = FLEXCAN_GetStatusFlags(self->data->base);
    if ((status_flags & CAN_ESR1_FLTCONF(2)) != 0U) {
        return BUS_STATE_OFF;
    }
    if ((status_flags & CAN_ESR1_FLTCONF(1)) != 0U) {
        return BUS_STATE_ERROR_PASSIVE;
    }
    if ((status_flags & (kFLEXCAN_TxErrorWarningFlag | kFLEXCAN_RxErrorWarningFlag)) != 0) {
        return BUS_STATE_ERROR_WARNING;
    }
    return BUS_STATE_ERROR_ACTIVE;
}

void common_hal_canio_can_restart(canio_can_obj_t *self) {
    // Supposedly the felxcan SDK has built in recovery.
    // But I will leave this code here just in case.

    canio_bus_state_t state = common_hal_canio_can_state_get(self);
    if (state != BUS_STATE_OFF) return;

    self->data->base->CTRL1 &= ~CAN_CTRL1_BOFFREC_MASK;

    // Hard coded wait time for bus to recover.
    uint64_t deadline = supervisor_ticks_ms64() + 100; // 100ms timeout
    do {
        if (supervisor_ticks_ms64() > deadline) {
            break;
        }
        RUN_BACKGROUND_TASKS;
    } while (common_hal_canio_can_state_get(self) == BUS_STATE_OFF);

    self->data->base->CTRL1 |= CAN_CTRL1_BOFFREC_MASK;
}

bool common_hal_canio_can_auto_restart_get(canio_can_obj_t *self) {
    return self->auto_restart;
}

void common_hal_canio_can_auto_restart_set(canio_can_obj_t *self, bool value) {
    self->auto_restart = value;
}

static void maybe_auto_restart(canio_can_obj_t *self) {
    if (self->auto_restart) {
        common_hal_canio_can_restart(self);
    }
}

void common_hal_canio_can_send(canio_can_obj_t *self, mp_obj_t message_in) {
    maybe_auto_restart(self);

    canio_bus_state_t state = common_hal_canio_can_state_get(self);
    if (state == BUS_STATE_OFF)
    {
        // Bus is off. Transmit failed.
        mp_raise_OSError(MP_ENODEV);
    }

    canio_message_obj_t *message = message_in;
    flexcan_frame_t tx_frame;
    if (!mimxrt_canio_message_obj_to_flexcan_frame(message, &tx_frame)) {
        mp_raise_ValueError(MP_ERROR_TEXT("Unable to send CAN Message: missing or malformed canio message object"));
    }

    flexcan_mb_transfer_t tx_xfer;
    tx_xfer.mbIdx = mimxrt10xx_flexcan_get_free_tx_mbid(self);
    tx_xfer.frame = &tx_frame;

    // Setup tx message buffer.
    FLEXCAN_SetTxMbConfig(self->data->base, tx_xfer.mbIdx, true);

    if (FLEXCAN_TransferSendNonBlocking(self->data->base, &self->data->handle, &tx_xfer) != kStatus_Success) {
        mp_raise_OSError(MP_EIO);
    }
}

bool common_hal_canio_can_silent_get(canio_can_obj_t *self) {
    return self->silent;
}

bool common_hal_canio_can_deinited(canio_can_obj_t *self) {
    return !self->data;
}

void common_hal_canio_can_check_for_deinit(canio_can_obj_t *self) {
    if (common_hal_canio_can_deinited(self)) {
        raise_deinited_error();
    }
}

void common_hal_canio_can_deinit(canio_can_obj_t *self) {
    if (self->data) {
        mimxrt10xx_flexcan_abort_tx_frames(self);
        FLEXCAN_TransferAbortReceiveFifo(self->data->base, &self->data->handle);

        FLEXCAN_Deinit(self->data->base);

        // Free can data by doing nothing and letting gc take care of it.
        // If the VM has finished already, this will be safe.
        self->data = NULL;
    }

    common_hal_reset_pin(self->rx_pin);
    common_hal_reset_pin(self->tx_pin);

    self->rx_pin = NULL;
    self->tx_pin = NULL;
}

void common_hal_canio_reset(void) {
    for (size_t i = 0; i < MP_ARRAY_SIZE(can_objs); i++) {
        if (can_objs[i]) {
            common_hal_canio_can_deinit(can_objs[i]);
            can_objs[i] = NULL;
        }
    }
}
