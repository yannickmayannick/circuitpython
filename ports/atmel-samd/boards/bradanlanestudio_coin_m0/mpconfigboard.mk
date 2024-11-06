# This file is part of the CircuitPython project: https://circuitpython.org
#
# SPDX-FileCopyrightText: Copyright (c) 2024 Bradán Lane STUDIO
#
# SPDX-License-Identifier: MIT
# TODO new VID:PID not yet approved via pidcodes.github.com

USB_VID = 0x1209
USB_PID = 0x5687
USB_PRODUCT = "M0 Coin"
USB_MANUFACTURER = "Bradán Lane STUDIO"

CHIP_VARIANT = SAMD21G18A
CHIP_FAMILY = samd21

CIRCUITPY_BUILD_EXTENSIONS = bin,uf2

#INTERNAL_FLASH_FILESYSTEM = 1
#LONGINT_IMPL = NONE
#CIRCUITPY_FULL_BUILD = 0

SPI_FLASH_FILESYSTEM = 1
EXTERNAL_FLASH_DEVICES = "W25Q32JVxQ"
LONGINT_IMPL = MPZ

CIRCUITPY_PULSEIO = 0
CIRCUITPY_AUDIOIO = 0
CIRCUITPY_AUDIOBUSIO = 0


# Include these Python libraries in firmware.
#FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_NeoPixel
#FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_HID
