// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Brandon Hurst, Analog Devices, Inc.
//
// SPDX-License-Identifier: MIT

#ifndef MAX32_PORT_H
#define MAX32_PORT_H

#include <stdint.h>

#include "mxc_assert.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "mxc_pins.h"
#include "mxc_sys.h"

#include "gpio.h"

#ifdef MAX32690
#include "system_max32690.h"
#include "max32690.h"
#endif

/** Linker variables defined....
 *  _estack:    end of the stack
 * _ebss:       end of BSS section
 * _ezero:      same as ebss (acc. to main.c)
 */
extern uint32_t _ezero;
extern uint32_t _estack;
extern uint32_t _ebss; // Stored at the end of the bss section (which includes the heap).
extern uint32_t __stack, __heap;

extern uint32_t SystemCoreClock;

// Tick timer should be 1/1024 s. RTC Oscillator is usually 32.768 kHz ERTCO.
#define TICKS_PER_SEC   1024

#ifdef MAX32690
// 12-bit ssec register, ticks @ 4096 Hz
#define SUBSEC_PER_TICK 4
#endif

#endif //MAX32_PORT_H
