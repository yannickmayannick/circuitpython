// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Bradán Lane STUDIO
//
// SPDX-License-Identifier: MIT

#pragma once

#define MICROPY_HW_BOARD_NAME "Bradán Lane STUDIO M0 Coin"
#define MICROPY_HW_MCU_NAME "samd21g18"

#define MICROPY_HW_LED_STATUS   (&pin_PA17)

#define SPI_FLASH_MOSI_PIN          &pin_PB22
#define SPI_FLASH_MISO_PIN          &pin_PB03
#define SPI_FLASH_SCK_PIN           &pin_PB23
#define SPI_FLASH_CS_PIN            &pin_PA27

// USB is always used internally so skip the pin objects for it.

// Not connected
#define IGNORE_PIN_PA13     1
#define IGNORE_PIN_PA28     1


#define IGNORE_PIN_PA00     1
#define IGNORE_PIN_PA01     1
// PIN_PA02 = A0
#define IGNORE_PIN_PA03     1
#define IGNORE_PIN_PA04     1
#define IGNORE_PIN_PA05     1
#define IGNORE_PIN_PA06     1
#define IGNORE_PIN_PA07     1
#define IGNORE_PIN_PA08     1
#define IGNORE_PIN_PA09     1
#define IGNORE_PIN_PA10     1
#define IGNORE_PIN_PA11     1
#define IGNORE_PIN_PA12     1
#define IGNORE_PIN_PA13     1
#define IGNORE_PIN_PA14     1
#define IGNORE_PIN_PA15     1
#define IGNORE_PIN_PA20     1
#define IGNORE_PIN_PA21     1
#define IGNORE_PIN_PA22     1
#define IGNORE_PIN_PA23     1
#define IGNORE_PIN_PA24     1   // USB_D+
#define IGNORE_PIN_PA25     1   // USB_D-
// PIN_PA27 = SPI_FLASH_CS
#define IGNORE_PIN_PA28     1
#define IGNORE_PIN_PA30     1   // SWCLK
#define IGNORE_PIN_PA31     1   // SWDIO

// PIN_PB02 = A5
// PIN_PB03 = SPI_FLASH_MISO
// PIN_PB08 = A1
// PIN_PB09 = A2
#define IGNORE_PIN_PB10     1   // MOSI
#define IGNORE_PIN_PB11     1   // SCK
// PIN_PB22 = SPI_FLASH_MOSI
// PIN_PB23 = SPI_FLASH_SCK
