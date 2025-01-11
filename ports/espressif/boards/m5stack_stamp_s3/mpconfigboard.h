// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: 2025 Stella Schwankl for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#pragma once

// Micropython setup

#define MICROPY_HW_BOARD_NAME       "M5Stack Stamp-S3"
#define MICROPY_HW_MCU_NAME         "ESP32-S3FN8"

#define MICROPY_HW_NEOPIXEL     (&pin_GPIO21)

#define DEFAULT_I2C_BUS_SCL     (&pin_GPIO15)
#define DEFAULT_I2C_BUS_SDA     (&pin_GPIO13)

#define DEFAULT_UART_BUS_RX     (&pin_GPIO44)
#define DEFAULT_UART_BUS_TX     (&pin_GPIO43)

// Below section commented out due to LCD not being included by default with StampS3
// Can be re-enabled to add LCD functionality included by Cardputer replacement kit

/*
#define CIRCUITPY_BOARD_SPI         (2)
#define CIRCUITPY_BOARD_SPI_PIN     {{.clock = &pin_GPIO36, .mosi = &pin_GPIO35}, \
                                     {.clock = &pin_GPIO40, .mosi = &pin_GPIO14, .miso = &pin_GPIO39}}
*/
