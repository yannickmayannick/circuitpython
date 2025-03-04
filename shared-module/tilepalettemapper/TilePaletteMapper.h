// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2025 Tim Cocks for Adafruit Industries
//
// SPDX-License-Identifier: MIT


#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "py/obj.h"

typedef struct {
  mp_obj_base_t base;
  mp_obj_t palette;
  uint16_t width_in_tiles;
  uint16_t height_in_tiles;
  uint16_t **tile_mappings;
} tilepalettemapper_tilepalettemapper_t;