// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Brandon Hurst, Analog Devices, Inc.
//
// SPDX-License-Identifier: MIT

#pragma once

// STD includes
#include <stdint.h>
#include <stdbool.h>

// CktPy includes
#include "py/obj.h"

// HAL includes
// #include "gpio.h"

typedef struct {
    mp_obj_base_t base;
    const uint8_t port;
    const uint8_t pad;
    // const uint8_t level : 4; // FIXME: Find how to include the VDDIO/VDDIOH level
} mcu_pin_obj_t;

extern const mp_obj_type_t mcu_pin_type;

#define NO_PIN (0xFF) // for non-connected pins

#define PIN(pin_port, pin_pad) \
    { \
        { &mcu_pin_type }, \
        .port = pin_port, \
        .pad = pin_pad, \
    }

// TODO: Create macros for detecting MCU VARIANT
#include "max32690/pins.h"
