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

#include <math.h>
#include <string.h>

#include "py/obj.h"
#include "py/runtime.h"
#include "py/mperrno.h"

#include "shared/runtime/interrupt_char.h"

#include "common-hal/canio/__init__.h"
#include "common-hal/canio/Listener.h"
#include "shared-bindings/canio/Listener.h"
#include "shared-bindings/util.h"
#include "supervisor/shared/tick.h"
#include "sdk/drivers/flexcan/fsl_flexcan.h"

/*
typedef struct {
    mp_obj_base_t base;
    int id;
    int mask;
    bool extended;
} canio_match_obj_t;
*/

void common_hal_canio_listener_construct(canio_listener_obj_t *self, canio_can_obj_t *can, size_t nmatch, canio_match_obj_t **matches, float timeout) {

    common_hal_canio_listener_set_timeout(self, timeout);

    if (nmatch > MIMXRT10XX_FLEXCAN_RX_FILTER_COUNT) {
        mp_raise_ValueError(MP_ERROR_TEXT("Filters too complex"));
    }

    self->can = can;

    for (size_t i = 0; i < nmatch; i++) {
        if (matches[i]->extended) {
            self->can->data->rx_fifo_filter[i] = FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(matches[i]->id, 0, 1);
        } else {
            self->can->data->rx_fifo_filter[i] = FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(matches[i]->id, 0, 0);
        }
    }

    flexcan_rx_fifo_config_t fifo_config;
    fifo_config.idFilterNum = nmatch;
    fifo_config.idFilterTable = self->can->data->rx_fifo_filter;
    fifo_config.idFilterType = kFLEXCAN_RxFifoFilterTypeA;
    fifo_config.priority = kFLEXCAN_RxFifoPrioHigh;
    FLEXCAN_SetRxFifoConfig(self->can->data->base, &fifo_config, true);
}

void common_hal_canio_listener_set_timeout(canio_listener_obj_t *self, float timeout) {
    self->timeout_ms = (int)MICROPY_FLOAT_C_FUN(ceil)(timeout * 1000);
}

float common_hal_canio_listener_get_timeout(canio_listener_obj_t *self) {
    return self->timeout_ms / 1000.0f;
}

void common_hal_canio_listener_check_for_deinit(canio_listener_obj_t *self) {
    if (!self->can) {
        raise_deinited_error();
    }
    common_hal_canio_can_check_for_deinit(self->can);
}

int common_hal_canio_listener_in_waiting(canio_listener_obj_t *self) {
    if(FLEXCAN_GetMbStatusFlags(self->can->data->base, kFLEXCAN_RxFifoFrameAvlFlag)) return 1;
    return 0;
}

mp_obj_t common_hal_canio_listener_receive(canio_listener_obj_t *self) {
    if (!common_hal_canio_listener_in_waiting(self)) {
        uint64_t deadline = supervisor_ticks_ms64() + self->timeout_ms;
        do {
            if (supervisor_ticks_ms64() > deadline) {
                return NULL;
            }
            RUN_BACKGROUND_TASKS;
            // Allow user to break out of a timeout with a KeyboardInterrupt.
            if (mp_hal_is_interrupted()) {
                return NULL;
            }
        } while (!common_hal_canio_listener_in_waiting(self));
    }

    flexcan_frame_t rx_frame;
    if(FLEXCAN_ReadRxFifo(self->can->data->base, &rx_frame) != kStatus_Success) {
        mp_raise_OSError(MP_EIO);
    }

    // We've read from the FIFO, clear the "frame available" flag, which
    // allows the CPU to serve the next FIFO entry
    FLEXCAN_ClearMbStatusFlags(self->can->data->base, (uint32_t)kFLEXCAN_RxFifoFrameAvlFlag);

    canio_message_obj_t *message = m_new_obj(canio_message_obj_t);
    if (!mimxrt_flexcan_frame_to_canio_message_obj(&rx_frame, message)) {
        mp_raise_ValueError(MP_ERROR_TEXT("Unable to receive CAN Message: missing or malformed flexcan frame"));
    }

    return message;
}

void common_hal_canio_listener_deinit(canio_listener_obj_t *self) {
    if (self->can) {
        // Clear all filters.
        flexcan_rx_fifo_config_t fifo_config;
        fifo_config.idFilterNum = 0;
        fifo_config.idFilterTable = self->can->data->rx_fifo_filter;
        fifo_config.idFilterType = kFLEXCAN_RxFifoFilterTypeA;
        fifo_config.priority = kFLEXCAN_RxFifoPrioHigh;
        FLEXCAN_SetRxFifoConfig(self->can->data->base, &fifo_config, true);
    }
    self->can = NULL;
}
