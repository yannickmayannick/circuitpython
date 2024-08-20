/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. All Rights Reserved.
 * (now owned by Analog Devices, Inc.),
 * Copyright (C) 2023 Analog Devices, Inc. All Rights Reserved. This software
 * is proprietary to Analog Devices, Inc. and its licensors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/**
 * @file port.c
 * @author Brandon Hurst @ Analog Devices, Inc.
 * @brief Functions required for a basic CircuitPython port
 * @date 2024-07-30
 *
 * @copyright Copyright (c) 2024
 */

#include <stdint.h>
#include <stdio.h>
#include "supervisor/background_callback.h"
#include "supervisor/board.h"
#include "supervisor/port.h"

#include "common-hal/microcontroller/Pin.h"
#include "shared-bindings/microcontroller/__init__.h"

//todo: pack the below definitions into their own module
//todo: under peripherals/gpio, peripherals/clocks, etc.

// Sys includes
#include "max32_port.h"

// Timers
#include "mxc_delay.h"
#include "rtc.h"

// Externs defined by linker .ld file
extern uint32_t _stack, _heap, _estack, _eheap;
extern uint32_t _ebss;

// From boards/$(BOARD)/board.c
extern const mxc_gpio_cfg_t pb_pin[];
extern const int num_pbs;
extern const mxc_gpio_cfg_t led_pin[];
extern const int num_leds;

//todo: define an LED HAL
// #include "peripherals/led.h"

// For caching rtc data for ticks
static uint32_t subsec, sec = 0;

// defined by cmsis core files
extern void NVIC_SystemReset(void) NORETURN;

safe_mode_t port_init(void) {
    int err = E_NO_ERROR;

    // Enable GPIO (enables clocks + common init for ports)
    for (int i = 0; i < MXC_CFG_GPIO_INSTANCES; i++){
        err = MXC_GPIO_Init(0x1 << i);
        if (err) {
            return SAFE_MODE_PROGRAMMATIC;
        }
    }

    // Init Board LEDs
    /* setup GPIO for the LED */
    for (int i = 0; i < num_leds; i++) {
        // Set the output value
        MXC_GPIO_OutClr(led_pin[i].port, led_pin[i].mask);

        if (MXC_GPIO_Config(&led_pin[i]) != E_NO_ERROR) {
            return SAFE_MODE_PROGRAMMATIC;
        }
    }

    // Turn on one LED to indicate Sign of Life
    MXC_GPIO_OutSet(led_pin[2].port, led_pin[2].mask);

    // Init RTC w/ 0sec, 0subsec
    // Driven by 32.768 kHz ERTCO, with ssec= 1/4096 s
    err = MXC_RTC_Init(0, 0);
    if (err) {
        return SAFE_MODE_SDK_FATAL_ERROR;
    }
    NVIC_EnableIRQ(RTC_IRQn);

    // todo: init periph clocks / console here when ready

    MXC_RTC_Start();
    return SAFE_MODE_NONE;
}

// Reset the MCU completely
void reset_cpu(void) {
    // includes MCU reset request + awaits on memory bus
    NVIC_SystemReset();
}

// Reset MCU state
void reset_port(void) {
    reset_all_pins();

    // todo: may need rtc-related resets here later
}

// Reset to the bootloader
// note: not implemented since max32 requires external stim ignals to
//       activate bootloaders
// todo: check if there's a method to jump to it
void reset_to_bootloader(void) {
    NVIC_SystemReset();
    while (true) {
        asm ("nop;");
    }
}

/** Acquire values of stack & heap starts & limits
 *  Return variables defined by linkerscript.
 */
uint32_t *port_stack_get_limit(void) {
    // ignore array bounds GCC warnings
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Warray-bounds"

    // NOTE: Only return how much stack we have alloted for CircuitPython
    return port_stack_get_top() - (CIRCUITPY_DEFAULT_STACK_SIZE + CIRCUITPY_EXCEPTION_STACK_SIZE) / sizeof(uint32_t);
    #pragma GCC diagnostic pop
}
uint32_t *port_stack_get_top(void) {
    return &_stack;
}
uint32_t *port_heap_get_bottom(void) {
    return &_heap;
}
uint32_t *port_heap_get_top(void) {
    return port_stack_get_limit();
}

/** Save & retrieve a word from memory over a reset cycle.
 *  Used for safe mode
 */
void port_set_saved_word(uint32_t value) {
    _ebss = value;
}
uint32_t port_get_saved_word(void) {
    return _ebss;
}

// Raw monotonic tick count since startup.
// NOTE (rollover):
// seconds reg is 32 bits, can hold up to 2^32-1
// theref. rolls over after ~136 years
uint64_t port_get_raw_ticks(uint8_t *subticks) {
    // Ensure we can read from ssec register as soon as we can
    // MXC function does cross-tick / busy checking of RTC controller
    __disable_irq();
    MXC_RTC_GetTime(&sec, &subsec);
    __enable_irq();

    // Return ticks given total subseconds
    // ticks = TICKS/s * s + subsec/ subs/tick
    uint64_t raw_ticks = ((uint64_t)TICKS_PER_SEC) * sec + (subsec / SUBSEC_PER_TICK);

    if (subticks) {
        // subticks may only be filled to a resn of 1/4096 in some cases
        // e.g. multiply by 32 / 8 = 4 to get true 1/32768 subticks
        *subticks = (32 / (SUBSEC_PER_TICK)) * (subsec - (subsec / SUBSEC_PER_TICK));
    }

    return raw_ticks;
}

// Enable 1/1024 second tick.
void port_enable_tick(void) {
    MXC_RTC_Start();
}

// Disable 1/1024 second tick.
void port_disable_tick(void) {
    MXC_RTC_Stop();
}

// Wake the CPU after a given # of ticks or sooner
void port_interrupt_after_ticks(uint32_t ticks) {
    // Stop RTC & store current time & ticks
    port_disable_tick();
    port_get_raw_ticks(NULL);

    uint32_t target_sec = (ticks / TICKS_PER_SEC);
    uint32_t target_ssec = (ticks - (target_sec * TICKS_PER_SEC)) * SUBSEC_PER_TICK;

    // Set up alarm configuration
    // if alarm is greater than 1 s,
    //     use the ToD alarm --> resol. to closest second
    // else
    //     use Ssec alarm --> resn. to 1/1024 s. (down to a full tick)
    if (target_sec > 0) {
        if (MXC_RTC_DisableInt(MXC_F_RTC_CTRL_SSEC_ALARM_IE |
                                MXC_F_RTC_CTRL_TOD_ALARM_IE) == E_BUSY) {
            // todo: signal some RTC error!
        }

        if (MXC_RTC_SetTimeofdayAlarm(target_sec) != E_NO_ERROR) {
            // todo: signal some RTC error!
        }
        if (MXC_RTC_EnableInt(MXC_F_RTC_CTRL_TOD_ALARM_IE) == E_BUSY) {
            // todo: signal some RTC error!
        }
    }
    else {
        if (MXC_RTC_DisableInt(MXC_F_RTC_CTRL_SSEC_ALARM_IE |
                                MXC_F_RTC_CTRL_TOD_ALARM_IE) == E_BUSY) {
            // todo: signal some RTC error!
        }

        if (MXC_RTC_SetSubsecondAlarm(target_ssec) != E_NO_ERROR) {
            // todo: signal some RTC error!
        }

        if (MXC_RTC_EnableInt(MXC_F_RTC_CTRL_SSEC_ALARM_IE) == E_BUSY) {
            // todo: signal some RTC error!
        }
    }
    port_enable_tick();
}

void RTC_IRQHandler(void) {
    // Read flags to clear
    int flags = MXC_RTC_GetFlags();

    if (flags & MXC_F_RTC_CTRL_TOD_ALARM) {
        MXC_RTC_ClearFlags(MXC_F_RTC_CTRL_TOD_ALARM);
        while (MXC_RTC_DisableInt(MXC_F_RTC_CTRL_TOD_ALARM_IE) == E_BUSY) {}
    }

    if (flags & MXC_F_RTC_CTRL_SSEC_ALARM) {
        MXC_RTC_ClearFlags(MXC_F_RTC_CTRL_SSEC_ALARM);
        while (MXC_RTC_DisableInt(MXC_F_RTC_CTRL_SSEC_ALARM_IE) == E_BUSY) {}
    }
}

void port_idle_until_interrupt(void) {
    // Check if alarm triggers before we even got here
    if (MXC_RTC_GetFlags() == (MXC_F_RTC_CTRL_TOD_ALARM | MXC_F_RTC_CTRL_SSEC_ALARM)) {
        return;
    }

    common_hal_mcu_disable_interrupts();
    if (!background_callback_pending()) {
        __WFI();
    }
    common_hal_mcu_enable_interrupts();
}

__attribute__((used)) void MemManage_Handler(void) {
    reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
    while (true) {
        asm ("nop;");
    }
}

__attribute__((used)) void BusFault_Handler(void) {
    reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
    while (true) {
        asm ("nop;");
    }
}

__attribute__((used)) void UsageFault_Handler(void) {
    reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
    while (true) {
        asm ("nop;");
    }
}

__attribute__((used)) void HardFault_Handler(void) {
    reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
    while (true) {
        asm ("nop;");
    }
}

// Required by libc _init_array loops in startup code
// if we are compiling using "-nostdlib/-nostartfiles"
void _init(void) {
}
