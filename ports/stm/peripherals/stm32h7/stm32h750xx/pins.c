// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 snkYmkrct
//
// SPDX-License-Identifier: MIT

#include "py/obj.h"
#include "py/mphal.h"
#include "peripherals/pins.h"

// ADC added only to pins exposed on Daisy Seed board

const mcu_pin_obj_t pin_PA00 = PIN(0, 0, ADC_INPUT(ADC_1, 16));
const mcu_pin_obj_t pin_PA01 = PIN(0, 1, ADC_INPUT(ADC_1, 17));
const mcu_pin_obj_t pin_PA02 = PIN(0, 2, ADC_INPUT(ADC_12, 14));
const mcu_pin_obj_t pin_PA03 = PIN(0, 3, ADC_INPUT(ADC_12, 15));
const mcu_pin_obj_t pin_PA04 = PIN(0, 4, ADC_INPUT(ADC_12, 18));
const mcu_pin_obj_t pin_PA05 = PIN(0, 5, ADC_INPUT(ADC_12, 19));
const mcu_pin_obj_t pin_PA06 = PIN(0, 6, ADC_INPUT(ADC_12, 3));
const mcu_pin_obj_t pin_PA07 = PIN(0, 7, ADC_INPUT(ADC_12, 7));
const mcu_pin_obj_t pin_PA08 = PIN(0, 8, NO_ADC);
const mcu_pin_obj_t pin_PA09 = PIN(0, 9, NO_ADC);
const mcu_pin_obj_t pin_PA10 = PIN(0, 10, NO_ADC);
const mcu_pin_obj_t pin_PA11 = PIN(0, 11, NO_ADC);
const mcu_pin_obj_t pin_PA12 = PIN(0, 12, NO_ADC);
const mcu_pin_obj_t pin_PA13 = PIN(0, 13, NO_ADC);
const mcu_pin_obj_t pin_PA14 = PIN(0, 14, NO_ADC);
const mcu_pin_obj_t pin_PA15 = PIN(0, 15, NO_ADC);

const mcu_pin_obj_t pin_PB00 = PIN(1, 0, NO_ADC);
const mcu_pin_obj_t pin_PB01 = PIN(1, 1, ADC_INPUT(ADC_12, 5));
const mcu_pin_obj_t pin_PB02 = PIN(1, 2, NO_ADC);
const mcu_pin_obj_t pin_PB03 = PIN(1, 3, NO_ADC);
const mcu_pin_obj_t pin_PB04 = PIN(1, 4, NO_ADC);
const mcu_pin_obj_t pin_PB05 = PIN(1, 5, NO_ADC);
const mcu_pin_obj_t pin_PB06 = PIN(1, 6, NO_ADC);
const mcu_pin_obj_t pin_PB07 = PIN(1, 7, NO_ADC);
const mcu_pin_obj_t pin_PB08 = PIN(1, 8, NO_ADC);
const mcu_pin_obj_t pin_PB09 = PIN(1, 9, NO_ADC);
const mcu_pin_obj_t pin_PB10 = PIN(1, 10, NO_ADC);
const mcu_pin_obj_t pin_PB11 = PIN(1, 11, NO_ADC);
const mcu_pin_obj_t pin_PB12 = PIN(1, 12, NO_ADC);
const mcu_pin_obj_t pin_PB13 = PIN(1, 13, NO_ADC);
const mcu_pin_obj_t pin_PB14 = PIN(1, 14, NO_ADC);
const mcu_pin_obj_t pin_PB15 = PIN(1, 15, NO_ADC);

const mcu_pin_obj_t pin_PC00 = PIN(2, 0, ADC_INPUT(ADC_12, 10));
const mcu_pin_obj_t pin_PC01 = PIN(2, 1, ADC_INPUT(ADC_123, 11));
const mcu_pin_obj_t pin_PC02 = PIN(2, 2, NO_ADC);
const mcu_pin_obj_t pin_PC03 = PIN(2, 3, NO_ADC);
const mcu_pin_obj_t pin_PC04 = PIN(2, 4, ADC_INPUT(ADC_12, 4));
const mcu_pin_obj_t pin_PC05 = PIN(2, 5, NO_ADC);
const mcu_pin_obj_t pin_PC06 = PIN(2, 6, NO_ADC);
const mcu_pin_obj_t pin_PC07 = PIN(2, 7, NO_ADC);
const mcu_pin_obj_t pin_PC08 = PIN(2, 8, NO_ADC);
const mcu_pin_obj_t pin_PC09 = PIN(2, 9, NO_ADC);
const mcu_pin_obj_t pin_PC10 = PIN(2, 10, NO_ADC);
const mcu_pin_obj_t pin_PC11 = PIN(2, 11, NO_ADC);
const mcu_pin_obj_t pin_PC12 = PIN(2, 12, NO_ADC);
const mcu_pin_obj_t pin_PC13 = PIN(2, 13, NO_ADC);
const mcu_pin_obj_t pin_PC14 = PIN(2, 14, NO_ADC);
const mcu_pin_obj_t pin_PC15 = PIN(2, 15, NO_ADC);

const mcu_pin_obj_t pin_PD00 = PIN(3, 0, NO_ADC);
const mcu_pin_obj_t pin_PD01 = PIN(3, 1, NO_ADC);
const mcu_pin_obj_t pin_PD02 = PIN(3, 2, NO_ADC);
const mcu_pin_obj_t pin_PD03 = PIN(3, 3, NO_ADC);
const mcu_pin_obj_t pin_PD04 = PIN(3, 4, NO_ADC);
const mcu_pin_obj_t pin_PD05 = PIN(3, 5, NO_ADC);
const mcu_pin_obj_t pin_PD06 = PIN(3, 6, NO_ADC);
const mcu_pin_obj_t pin_PD07 = PIN(3, 7, NO_ADC);
const mcu_pin_obj_t pin_PD08 = PIN(3, 8, NO_ADC);
const mcu_pin_obj_t pin_PD09 = PIN(3, 9, NO_ADC);
const mcu_pin_obj_t pin_PD10 = PIN(3, 10, NO_ADC);
const mcu_pin_obj_t pin_PD11 = PIN(3, 11, NO_ADC);
const mcu_pin_obj_t pin_PD12 = PIN(3, 12, NO_ADC);
const mcu_pin_obj_t pin_PD13 = PIN(3, 13, NO_ADC);
const mcu_pin_obj_t pin_PD14 = PIN(3, 14, NO_ADC);
const mcu_pin_obj_t pin_PD15 = PIN(3, 15, NO_ADC);

const mcu_pin_obj_t pin_PE00 = PIN(4, 0, NO_ADC);
const mcu_pin_obj_t pin_PE01 = PIN(4, 1, NO_ADC);
const mcu_pin_obj_t pin_PE02 = PIN(4, 2, NO_ADC);
const mcu_pin_obj_t pin_PE03 = PIN(4, 3, NO_ADC);
const mcu_pin_obj_t pin_PE04 = PIN(4, 4, NO_ADC);
const mcu_pin_obj_t pin_PE05 = PIN(4, 5, NO_ADC);
const mcu_pin_obj_t pin_PE06 = PIN(4, 6, NO_ADC);
const mcu_pin_obj_t pin_PE07 = PIN(4, 7, NO_ADC);
const mcu_pin_obj_t pin_PE08 = PIN(4, 8, NO_ADC);
const mcu_pin_obj_t pin_PE09 = PIN(4, 9, NO_ADC);
const mcu_pin_obj_t pin_PE10 = PIN(4, 10, NO_ADC);
const mcu_pin_obj_t pin_PE11 = PIN(4, 11, NO_ADC);
const mcu_pin_obj_t pin_PE12 = PIN(4, 12, NO_ADC);
const mcu_pin_obj_t pin_PE13 = PIN(4, 13, NO_ADC);
const mcu_pin_obj_t pin_PE14 = PIN(4, 14, NO_ADC);
const mcu_pin_obj_t pin_PE15 = PIN(4, 15, NO_ADC);

const mcu_pin_obj_t pin_PF00 = PIN(5, 0, NO_ADC);
const mcu_pin_obj_t pin_PF01 = PIN(5, 1, NO_ADC);
const mcu_pin_obj_t pin_PF02 = PIN(5, 2, NO_ADC);
const mcu_pin_obj_t pin_PF03 = PIN(5, 3, NO_ADC);
const mcu_pin_obj_t pin_PF04 = PIN(5, 4, NO_ADC);
const mcu_pin_obj_t pin_PF05 = PIN(5, 5, NO_ADC);
const mcu_pin_obj_t pin_PF06 = PIN(5, 6, NO_ADC);
const mcu_pin_obj_t pin_PF07 = PIN(5, 7, NO_ADC);
const mcu_pin_obj_t pin_PF08 = PIN(5, 8, NO_ADC);
const mcu_pin_obj_t pin_PF09 = PIN(5, 9, NO_ADC);
const mcu_pin_obj_t pin_PF10 = PIN(5, 10, NO_ADC);
const mcu_pin_obj_t pin_PF11 = PIN(5, 11, NO_ADC);
const mcu_pin_obj_t pin_PF12 = PIN(5, 12, NO_ADC);
const mcu_pin_obj_t pin_PF13 = PIN(5, 13, NO_ADC);
const mcu_pin_obj_t pin_PF14 = PIN(5, 14, NO_ADC);
const mcu_pin_obj_t pin_PF15 = PIN(5, 15, NO_ADC);

const mcu_pin_obj_t pin_PG00 = PIN(6, 0, NO_ADC);
const mcu_pin_obj_t pin_PG01 = PIN(6, 1, NO_ADC);
const mcu_pin_obj_t pin_PG02 = PIN(6, 2, NO_ADC);
const mcu_pin_obj_t pin_PG03 = PIN(6, 3, NO_ADC);
const mcu_pin_obj_t pin_PG04 = PIN(6, 4, NO_ADC);
const mcu_pin_obj_t pin_PG05 = PIN(6, 5, NO_ADC);
const mcu_pin_obj_t pin_PG06 = PIN(6, 6, NO_ADC);
const mcu_pin_obj_t pin_PG07 = PIN(6, 7, NO_ADC);
const mcu_pin_obj_t pin_PG08 = PIN(6, 8, NO_ADC);
const mcu_pin_obj_t pin_PG09 = PIN(6, 9, NO_ADC);
const mcu_pin_obj_t pin_PG10 = PIN(6, 10, NO_ADC);
const mcu_pin_obj_t pin_PG11 = PIN(6, 11, NO_ADC);
const mcu_pin_obj_t pin_PG12 = PIN(6, 12, NO_ADC);
const mcu_pin_obj_t pin_PG13 = PIN(6, 13, NO_ADC);
const mcu_pin_obj_t pin_PG14 = PIN(6, 14, NO_ADC);
const mcu_pin_obj_t pin_PG15 = PIN(6, 15, NO_ADC);

const mcu_pin_obj_t pin_PH00 = PIN(7, 0, NO_ADC);
const mcu_pin_obj_t pin_PH01 = PIN(7, 1, NO_ADC);
const mcu_pin_obj_t pin_PH02 = PIN(7, 2, NO_ADC);
const mcu_pin_obj_t pin_PH03 = PIN(7, 3, NO_ADC);
const mcu_pin_obj_t pin_PH04 = PIN(7, 4, NO_ADC);
const mcu_pin_obj_t pin_PH05 = PIN(7, 5, NO_ADC);
const mcu_pin_obj_t pin_PH06 = PIN(7, 6, NO_ADC);
const mcu_pin_obj_t pin_PH07 = PIN(7, 7, NO_ADC);
const mcu_pin_obj_t pin_PH08 = PIN(7, 8, NO_ADC);
const mcu_pin_obj_t pin_PH09 = PIN(7, 9, NO_ADC);
const mcu_pin_obj_t pin_PH10 = PIN(7, 10, NO_ADC);
const mcu_pin_obj_t pin_PH11 = PIN(7, 11, NO_ADC);
const mcu_pin_obj_t pin_PH12 = PIN(7, 12, NO_ADC);
const mcu_pin_obj_t pin_PH13 = PIN(7, 13, NO_ADC);
const mcu_pin_obj_t pin_PH14 = PIN(7, 14, NO_ADC);
const mcu_pin_obj_t pin_PH15 = PIN(7, 15, NO_ADC);

const mcu_pin_obj_t pin_PI00 = PIN(8, 0, NO_ADC);
const mcu_pin_obj_t pin_PI01 = PIN(8, 1, NO_ADC);
const mcu_pin_obj_t pin_PI02 = PIN(8, 2, NO_ADC);
const mcu_pin_obj_t pin_PI03 = PIN(8, 3, NO_ADC);
const mcu_pin_obj_t pin_PI04 = PIN(8, 4, NO_ADC);
const mcu_pin_obj_t pin_PI05 = PIN(8, 5, NO_ADC);
const mcu_pin_obj_t pin_PI06 = PIN(8, 6, NO_ADC);
const mcu_pin_obj_t pin_PI07 = PIN(8, 7, NO_ADC);
const mcu_pin_obj_t pin_PI08 = PIN(8, 8, NO_ADC);
const mcu_pin_obj_t pin_PI09 = PIN(8, 9, NO_ADC);
const mcu_pin_obj_t pin_PI10 = PIN(8, 10, NO_ADC);
const mcu_pin_obj_t pin_PI11 = PIN(8, 11, NO_ADC);
const mcu_pin_obj_t pin_PI12 = PIN(8, 12, NO_ADC);
const mcu_pin_obj_t pin_PI13 = PIN(8, 13, NO_ADC);
const mcu_pin_obj_t pin_PI14 = PIN(8, 14, NO_ADC);
const mcu_pin_obj_t pin_PI15 = PIN(8, 15, NO_ADC);
