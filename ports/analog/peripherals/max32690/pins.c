// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Brandon Hurst, Analog Devices, Inc.
//
// SPDX-License-Identifier: MIT

#include "py/obj.h"
#include "py/mphal.h"
#include "peripherals/pins.h"

#include "gpio.h"
#include "gpio_regs.h"

const mcu_pin_obj_t pin_P0_00 = PIN(MXC_GPIO_PORT_0, 0);
const mcu_pin_obj_t pin_P0_01 = PIN(MXC_GPIO_PORT_0, 1);
const mcu_pin_obj_t pin_P0_02 = PIN(MXC_GPIO_PORT_0, 2);
const mcu_pin_obj_t pin_P0_03 = PIN(MXC_GPIO_PORT_0, 3);
const mcu_pin_obj_t pin_P0_04 = PIN(MXC_GPIO_PORT_0, 4);
const mcu_pin_obj_t pin_P0_05 = PIN(MXC_GPIO_PORT_0, 5);
const mcu_pin_obj_t pin_P0_06 = PIN(MXC_GPIO_PORT_0, 6);
const mcu_pin_obj_t pin_P0_07 = PIN(MXC_GPIO_PORT_0, 7);
const mcu_pin_obj_t pin_P0_08 = PIN(MXC_GPIO_PORT_0, 8);
const mcu_pin_obj_t pin_P0_09 = PIN(MXC_GPIO_PORT_0, 9);
const mcu_pin_obj_t pin_P0_10 = PIN(MXC_GPIO_PORT_0, 10);
const mcu_pin_obj_t pin_P0_11 = PIN(MXC_GPIO_PORT_0, 11);
const mcu_pin_obj_t pin_P0_12 = PIN(MXC_GPIO_PORT_0, 12);
const mcu_pin_obj_t pin_P0_13 = PIN(MXC_GPIO_PORT_0, 13);
const mcu_pin_obj_t pin_P0_14 = PIN(MXC_GPIO_PORT_0, 14);
const mcu_pin_obj_t pin_P0_15 = PIN(MXC_GPIO_PORT_0, 15);
const mcu_pin_obj_t pin_P0_16 = PIN(MXC_GPIO_PORT_0, 16);
const mcu_pin_obj_t pin_P0_17 = PIN(MXC_GPIO_PORT_0, 17);
const mcu_pin_obj_t pin_P0_18 = PIN(MXC_GPIO_PORT_0, 18);
const mcu_pin_obj_t pin_P0_19 = PIN(MXC_GPIO_PORT_0, 19);
const mcu_pin_obj_t pin_P0_20 = PIN(MXC_GPIO_PORT_0, 20);
const mcu_pin_obj_t pin_P0_21 = PIN(MXC_GPIO_PORT_0, 21);
const mcu_pin_obj_t pin_P0_22 = PIN(MXC_GPIO_PORT_0, 22);
const mcu_pin_obj_t pin_P0_23 = PIN(MXC_GPIO_PORT_0, 23);
const mcu_pin_obj_t pin_P0_24 = PIN(MXC_GPIO_PORT_0, 24);
const mcu_pin_obj_t pin_P0_25 = PIN(MXC_GPIO_PORT_0, 25);
const mcu_pin_obj_t pin_P0_26 = PIN(MXC_GPIO_PORT_0, 26);
const mcu_pin_obj_t pin_P0_27 = PIN(MXC_GPIO_PORT_0, 27);
const mcu_pin_obj_t pin_P0_28 = PIN(MXC_GPIO_PORT_0, 28);
const mcu_pin_obj_t pin_P0_29 = PIN(MXC_GPIO_PORT_0, 29);
const mcu_pin_obj_t pin_P0_30 = PIN(MXC_GPIO_PORT_0, 30);
const mcu_pin_obj_t pin_P0_31 = PIN(MXC_GPIO_PORT_0, 31);

const mcu_pin_obj_t pin_P1_00 = PIN(MXC_GPIO_PORT_1, 0);
const mcu_pin_obj_t pin_P1_01 = PIN(MXC_GPIO_PORT_1, 1);
const mcu_pin_obj_t pin_P1_02 = PIN(MXC_GPIO_PORT_1, 2);
const mcu_pin_obj_t pin_P1_03 = PIN(MXC_GPIO_PORT_1, 3);
const mcu_pin_obj_t pin_P1_04 = PIN(MXC_GPIO_PORT_1, 4);
const mcu_pin_obj_t pin_P1_05 = PIN(MXC_GPIO_PORT_1, 5);
const mcu_pin_obj_t pin_P1_06 = PIN(MXC_GPIO_PORT_1, 6);
const mcu_pin_obj_t pin_P1_07 = PIN(MXC_GPIO_PORT_1, 7);
const mcu_pin_obj_t pin_P1_08 = PIN(MXC_GPIO_PORT_1, 8);
const mcu_pin_obj_t pin_P1_09 = PIN(MXC_GPIO_PORT_1, 9);
const mcu_pin_obj_t pin_P1_10 = PIN(MXC_GPIO_PORT_1, 10);
const mcu_pin_obj_t pin_P1_11 = PIN(MXC_GPIO_PORT_1, 11);
const mcu_pin_obj_t pin_P1_12 = PIN(MXC_GPIO_PORT_1, 12);
const mcu_pin_obj_t pin_P1_13 = PIN(MXC_GPIO_PORT_1, 13);
const mcu_pin_obj_t pin_P1_14 = PIN(MXC_GPIO_PORT_1, 14);
const mcu_pin_obj_t pin_P1_15 = PIN(MXC_GPIO_PORT_1, 15);
const mcu_pin_obj_t pin_P1_16 = PIN(MXC_GPIO_PORT_1, 16);
const mcu_pin_obj_t pin_P1_17 = PIN(MXC_GPIO_PORT_1, 17);
const mcu_pin_obj_t pin_P1_18 = PIN(MXC_GPIO_PORT_1, 18);
const mcu_pin_obj_t pin_P1_19 = PIN(MXC_GPIO_PORT_1, 19);
const mcu_pin_obj_t pin_P1_20 = PIN(MXC_GPIO_PORT_1, 20);
const mcu_pin_obj_t pin_P1_21 = PIN(MXC_GPIO_PORT_1, 21);
const mcu_pin_obj_t pin_P1_22 = PIN(MXC_GPIO_PORT_1, 22);
const mcu_pin_obj_t pin_P1_23 = PIN(MXC_GPIO_PORT_1, 23);
const mcu_pin_obj_t pin_P1_24 = PIN(MXC_GPIO_PORT_1, 24);
const mcu_pin_obj_t pin_P1_25 = PIN(MXC_GPIO_PORT_1, 25);
const mcu_pin_obj_t pin_P1_26 = PIN(MXC_GPIO_PORT_1, 26);
const mcu_pin_obj_t pin_P1_27 = PIN(MXC_GPIO_PORT_1, 27);
const mcu_pin_obj_t pin_P1_28 = PIN(MXC_GPIO_PORT_1, 28);
const mcu_pin_obj_t pin_P1_29 = PIN(MXC_GPIO_PORT_1, 29);
const mcu_pin_obj_t pin_P1_30 = PIN(MXC_GPIO_PORT_1, 30);
const mcu_pin_obj_t pin_P1_31 = PIN(MXC_GPIO_PORT_1, 31);

const mcu_pin_obj_t pin_P2_00 = PIN(MXC_GPIO_PORT_2, 0);
const mcu_pin_obj_t pin_P2_01 = PIN(MXC_GPIO_PORT_2, 1);
const mcu_pin_obj_t pin_P2_02 = PIN(MXC_GPIO_PORT_2, 2);
const mcu_pin_obj_t pin_P2_03 = PIN(MXC_GPIO_PORT_2, 3);
const mcu_pin_obj_t pin_P2_04 = PIN(MXC_GPIO_PORT_2, 4);
const mcu_pin_obj_t pin_P2_05 = PIN(MXC_GPIO_PORT_2, 5);
const mcu_pin_obj_t pin_P2_06 = PIN(MXC_GPIO_PORT_2, 6);
const mcu_pin_obj_t pin_P2_07 = PIN(MXC_GPIO_PORT_2, 7);
const mcu_pin_obj_t pin_P2_08 = PIN(MXC_GPIO_PORT_2, 8);
const mcu_pin_obj_t pin_P2_09 = PIN(MXC_GPIO_PORT_2, 9);
const mcu_pin_obj_t pin_P2_10 = PIN(MXC_GPIO_PORT_2, 10);
const mcu_pin_obj_t pin_P2_11 = PIN(MXC_GPIO_PORT_2, 11);
const mcu_pin_obj_t pin_P2_12 = PIN(MXC_GPIO_PORT_2, 12);
const mcu_pin_obj_t pin_P2_13 = PIN(MXC_GPIO_PORT_2, 13);
const mcu_pin_obj_t pin_P2_14 = PIN(MXC_GPIO_PORT_2, 14);
const mcu_pin_obj_t pin_P2_15 = PIN(MXC_GPIO_PORT_2, 15);
const mcu_pin_obj_t pin_P2_16 = PIN(MXC_GPIO_PORT_2, 16);
const mcu_pin_obj_t pin_P2_17 = PIN(MXC_GPIO_PORT_2, 17);
const mcu_pin_obj_t pin_P2_18 = PIN(MXC_GPIO_PORT_2, 18);
const mcu_pin_obj_t pin_P2_19 = PIN(MXC_GPIO_PORT_2, 19);
const mcu_pin_obj_t pin_P2_20 = PIN(MXC_GPIO_PORT_2, 20);
const mcu_pin_obj_t pin_P2_21 = PIN(MXC_GPIO_PORT_2, 21);
const mcu_pin_obj_t pin_P2_22 = PIN(MXC_GPIO_PORT_2, 22);
const mcu_pin_obj_t pin_P2_23 = PIN(MXC_GPIO_PORT_2, 23);
const mcu_pin_obj_t pin_P2_24 = PIN(MXC_GPIO_PORT_2, 24);
const mcu_pin_obj_t pin_P2_25 = PIN(MXC_GPIO_PORT_2, 25);
const mcu_pin_obj_t pin_P2_26 = PIN(MXC_GPIO_PORT_2, 26);
const mcu_pin_obj_t pin_P2_27 = PIN(MXC_GPIO_PORT_2, 27);
const mcu_pin_obj_t pin_P2_28 = PIN(MXC_GPIO_PORT_2, 28);
const mcu_pin_obj_t pin_P2_29 = PIN(MXC_GPIO_PORT_2, 29);
const mcu_pin_obj_t pin_P2_30 = PIN(MXC_GPIO_PORT_2, 30);
const mcu_pin_obj_t pin_P2_31 = PIN(MXC_GPIO_PORT_2, 31);

const mcu_pin_obj_t pin_P3_00 = PIN(MXC_GPIO_PORT_3, 0);
const mcu_pin_obj_t pin_P3_01 = PIN(MXC_GPIO_PORT_3, 1);
const mcu_pin_obj_t pin_P3_02 = PIN(MXC_GPIO_PORT_3, 2);
const mcu_pin_obj_t pin_P3_03 = PIN(MXC_GPIO_PORT_3, 3);
const mcu_pin_obj_t pin_P3_04 = PIN(MXC_GPIO_PORT_3, 4);
const mcu_pin_obj_t pin_P3_05 = PIN(MXC_GPIO_PORT_3, 5);
const mcu_pin_obj_t pin_P3_06 = PIN(MXC_GPIO_PORT_3, 6);
const mcu_pin_obj_t pin_P3_07 = PIN(MXC_GPIO_PORT_3, 7);
const mcu_pin_obj_t pin_P3_08 = PIN(MXC_GPIO_PORT_3, 8);
const mcu_pin_obj_t pin_P3_09 = PIN(MXC_GPIO_PORT_3, 9);

const mcu_pin_obj_t pin_P4_00 = PIN(MXC_GPIO_PORT_4, 0);
const mcu_pin_obj_t pin_P4_01 = PIN(MXC_GPIO_PORT_4, 1);
