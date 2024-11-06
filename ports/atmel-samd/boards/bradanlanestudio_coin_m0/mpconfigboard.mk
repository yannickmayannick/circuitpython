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

#CIRCUITPY_BUILD_EXTENSIONS = bin,uf2

SPI_FLASH_FILESYSTEM = 1
EXTERNAL_FLASH_DEVICES = "W25Q32JVxQ"
LONGINT_IMPL = MPZ

CIRCUITPY_PULSEIO = 0

#CIRCUITPY_FULL_BUILD = 0

# A number of modules are removed
# Many I/O functions are not available or not used in a keyboard
#CIRCUITPY_ANALOGIO = 0 # Needed for potentiometer input (mouse)
#CIRCUITPY_AUDIOCORE = 0
#CIRCUITPY_AUDIOIO = 0
#CIRCUITPY_AUDIOBUSIO = 0
# Needed for I2C, SPI and UART - removed that for keyboards...
#CIRCUITPY_BUSIO = 1
#CIRCUITPY_PULSEIO = 1
# only needed for speaker or LED PWM functions. Takes 2314 bytes.
#CIRCUITPY_PWMIO = 1
#CIRCUITPY_RTC = 0
#CIRCUITPY_MATH = 0
#CIRCUITPY_RANDOM = 0
#CIRCUITPY_ONEWIREIO = 0
# Needed for RGB LEDs
#CIRCUITPY_NEOPIXEL_WRITE = 1
# Needed for RGB LEDs
#CIRCUITPY_RAINBOWIO = 1
# These are used in a keyboard or computer input device.
#CIRCUITPY_ROTARYIO = 0
#CIRCUITPY_KEYPAD = 1
#CIRCUITPY_USB_HID = 1
#CIRCUITPY_USB_MIDI = 0

# Include these Python libraries in firmware.
#FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_HID
#FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_NeoPixel
