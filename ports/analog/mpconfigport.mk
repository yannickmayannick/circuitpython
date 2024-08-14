# This file is part of the CircuitPython project: https://circuitpython.org
#
# SPDX-FileCopyrightText: Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
#
# SPDX-License-Identifier: MIT

CHIP_FAMILY ?= max32

# Necessary to build CircuitPython
USB_NUM_ENDPOINT_PAIRS ?= 0
LONGINT_IMPL ?= MPZ
INTERNAL_LIBM ?= 1

####################################################################################
# Suggested config for first-time porting
####################################################################################
# These modules are implemented in ports/<port>/common-hal:

# Typically the first module to create
CIRCUITPY_MICROCONTROLLER = 1
# Typically the second module to create
CIRCUITPY_DIGITALIO = 0
# Other modules:
CIRCUITPY_ANALOGIO = 0
CIRCUITPY_BUSIO = 0
CIRCUITPY_COUNTIO = 0
CIRCUITPY_NEOPIXEL_WRITE = 0
CIRCUITPY_PULSEIO = 0
CIRCUITPY_OS = 1
CIRCUITPY_NVM = 0
CIRCUITPY_AUDIOBUSIO = 0
CIRCUITPY_AUDIOIO = 0
CIRCUITPY_ROTARYIO = 0
CIRCUITPY_RTC = 0
CIRCUITPY_SDCARDIO = 0
CIRCUITPY_FRAMEBUFFERIO = 0
CIRCUITPY_FREQUENCYIO = 0
CIRCUITPY_I2CTARGET = 0
# Requires SPI, PulseIO (stub ok):
CIRCUITPY_DISPLAYIO = 0

# These modules are implemented in shared-module/ - they can be included in
# any port once their prerequisites in common-hal are complete.
# Requires DigitalIO:
CIRCUITPY_BITBANGIO = 0
# Requires neopixel_write or SPI (dotstar)
CIRCUITPY_PIXELBUF = 0
# Requires OS
CIRCUITPY_RANDOM = 0
# Requires OS, filesystem
CIRCUITPY_STORAGE = 0
# Requires Microcontroller
CIRCUITPY_TOUCHIO = 0
# Requires UART!
CIRCUITPY_CONSOLE_UART = 0
# Requires USB
CIRCUITPY_USB_DEVICE = 0
CIRCUITPY_USB_CDC = 0
CIRCUITPY_USB_HID = 0
CIRCUITPY_USB_MIDI = 0
# Does nothing without I2C
CIRCUITPY_REQUIRE_I2C_PULLUPS = 0
# No requirements, but takes extra flash
CIRCUITPY_ULAB = 0
####################################################################################
# Required for clean building (additional CircuittPython Defaults)
####################################################################################
# Enabled by default
CIRCUITPY_PWMIO = 0
# Depends on BUSIO
# CIRCUITPY_BLEIO = 0
CIRCUITPY_BLEIO_HCI = 0
CIRCUITPY_KEYPAD = 0
CIRCUITPY_BUSDEVICE = 0

# TinyUSB will be added later.
CIRCUITPY_TINYUSB = 0
CIRCUITPY_PYUSB = 0

INTERNAL_FLASH_FILESYSTEM = 1
SPI_FLASH_FILESYSTEM = 0
QSPI_FLASH_FILESYSTEM = 0

# TODO: Review flash filesystem funcs before flashing
DISABLE_FILESYSTEM = 0
