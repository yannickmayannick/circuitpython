/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 snkYmkrct
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

// Micropython setup

#define MICROPY_HW_BOARD_NAME       "DAISY_SEED"
#define MICROPY_HW_MCU_NAME         "STM32H750xx"

#define MICROPY_HW_LED_STATUS (&pin_PC07)

// H7 and F7 MPU definitions
#define CPY_FLASH_REGION_SIZE   ARM_MPU_REGION_SIZE_8MB
#define CPY_ITCM_REGION_SIZE    ARM_MPU_REGION_SIZE_64KB
#define CPY_DTCM_REGION_SIZE    ARM_MPU_REGION_SIZE_128KB
#define CPY_SRAM_REGION_SIZE    ARM_MPU_REGION_SIZE_512KB
#define CPY_SRAM_SUBMASK        0x00
#define CPY_SRAM_START_ADDR     0x24000000

#define HSE_VALUE ((uint32_t)16000000)
#define BOARD_HSE_SOURCE (RCC_HSE_ON) // use external oscillator
#define BOARD_HAS_LOW_SPEED_CRYSTAL (0)

#define CIRCUITPY_CONSOLE_UART_TX (&pin_PB09)
#define CIRCUITPY_CONSOLE_UART_RX (&pin_PB08)

// USB
#define BOARD_NO_USB_OTG_ID_SENSE (1)

// for RNG not audio
#define CPY_CLK_USB_USES_AUDIOPLL (1)
