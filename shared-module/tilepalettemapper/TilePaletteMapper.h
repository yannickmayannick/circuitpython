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
    bool needs_refresh;
} tilepalettemapper_tilepalettemapper_t;

bool tilepalettemapper_tilepalettemapper_needs_refresh(tilepalettemapper_tilepalettemapper_t *self);
void tilepalettemapper_tilepalettemapper_finish_refresh(tilepalettemapper_tilepalettemapper_t *self);

uint32_t tilepalettemapper_tilepalettemapper_get_color(tilepalettemapper_tilepalettemapper_t *self, uint16_t tile_index, uint32_t palette_index);
