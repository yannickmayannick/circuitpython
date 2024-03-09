
/*
 * This file is part of the Micro Python project, http://micropython.org/
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

#include "stm32h7xx_hal.h"

// Chip:                STM32H750
// Line Type:           Single-Core
// Speed:               480MHz (MAX)

// Defaults:
#ifndef CPY_CLK_VSCALE
#define CPY_CLK_VSCALE (PWR_REGULATOR_VOLTAGE_SCALE0)
#endif
#ifndef CPY_CLK_PLLM
#define CPY_CLK_PLLM (1)
#endif
#ifndef CPY_CLK_PLLN
#define CPY_CLK_PLLN (50)
#endif
#ifndef CPY_CLK_PLLP
#define CPY_CLK_PLLP (2)
#endif
#ifndef CPY_CLK_PLLQ
#define CPY_CLK_PLLQ (4)
#endif
#ifndef CPY_CLK_PLLR
#define CPY_CLK_PLLR (2)
#endif
#ifndef CPY_CLK_PLLRGE
#define CPY_CLK_PLLRGE (RCC_PLL1VCIRANGE_3)
#endif
#ifndef CPY_CLK_PLLVCOSEL
#define CPY_CLK_PLLVCOSEL (RCC_PLL1VCOWIDE)
#endif
#ifndef CPY_CLK_PLLFRACN
#define CPY_CLK_PLLFRACN (0)
#endif
#ifndef CPY_CLK_AHBDIV
#define CPY_CLK_AHBDIV (RCC_HCLK_DIV2)
#endif
#ifndef CPY_CLK_APB1DIV
#define CPY_CLK_APB1DIV (RCC_APB1_DIV2)
#endif
#ifndef CPY_CLK_APB2DIV
#define CPY_CLK_APB2DIV (RCC_APB2_DIV2)
#endif
#ifndef CPY_CLK_APB3DIV
#define CPY_CLK_APB3DIV (RCC_APB3_DIV2)
#endif
#ifndef CPY_CLK_APB4DIV
#define CPY_CLK_APB4DIV (RCC_APB4_DIV2)
#endif
#ifndef CPY_CLK_FLASH_LATENCY
#define CPY_CLK_FLASH_LATENCY (FLASH_LATENCY_2)
#endif
#ifndef CPY_CLK_USB_USES_AUDIOPLL
#define CPY_CLK_USB_USES_AUDIOPLL (0)
#endif
#ifndef BOARD_HSE_SOURCE
#define BOARD_HSE_SOURCE (RCC_HSE_ON)
#endif
#ifndef BOARD_PLL_STATE
#define BOARD_PLL_STATE (RCC_PLL_ON)
#endif
#ifndef BOARD_PLL_SOURCE
#define BOARD_PLL_SOURCE (RCC_PLLSOURCE_HSE)
#endif
