# This file is part of the CircuitPython project: https://circuitpython.org
#
# SPDX-FileCopyrightText: Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
#
# SPDX-License-Identifier: MIT

MCU_SERIES=max32
MCU_VARIANT=max32690

INTERNAL_FLASH_FILESYSTEM=1
# FLASH: 	0x10000000 to 0x10300000 (ARM)
# SRAM: 	0x20000000 to 0x20100000

#### USB CONFIGURATION
# Use 0x0456 for Analog Devices, Inc.; 0B6A for Maxim
USB_VID=0x0456
# USB_VID=0x0B6A
USB_PID=0x003C
USB_MANUFACTURER="Analog Devices, Inc."
USB_PRODUCT="MAX32690 APARD"
USB_NUM_ENDPOINT_PAIRS=12
USB_HIGHSPEED=1
###

# define UID len for memory safety (buffer gets passed as a raw ptr)
COMMON_HAL_MCU_PROCESSOR_UID_LENGTH=30

# NOTE: Not implementing external flash for now
# CFLAGS+=-DEXT_FLASH_MX25
