// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2017 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

// Use the MP_WEAK supervisor/shared/board.c versions of routines not defined here.
// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2016 Glenn Ruben Bakke
// SPDX-FileCopyrightText: Copyright (c) 2018 Dan Halbert for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#pragma once

#define MICROPY_HW_BOARD_NAME       "APARD32690"
#define MICROPY_HW_MCU_NAME         "max32690"

#define FLASH_SIZE                  (0x300000) // 3MiB
#define FLASH_PAGE_SIZE             (0x4000)   // 16384 byte pages (16 KiB)

#define BOARD_HAS_CRYSTAL 1

#define NUM_GPIO_PORTS 4

#if INTERNAL_FLASH_FILESYSTEM
#define CIRCUITPY_INTERNAL_FLASH_FILESYSTEM_START_ADDR (0x102FC000) // for MAX32690
#define CIRCUITPY_INTERNAL_FLASH_FILESYSTEM_SIZE (64 * 1024)        // 64K

#define MAX32_FLASH_SIZE  0x300000 // 3 MiB
#define INTERNAL_FLASH_FILESYSTEM_SIZE 0x10000 // 64KiB
#define INTERNAL_FLASH_FILESYSTEM_START_ADDR 0x102FC000 // Load into the last MiB of code/data storage

#else
#define CIRCUITPY_INTERNAL_FLASH_FILESYSTEM_SIZE (0)
#endif
