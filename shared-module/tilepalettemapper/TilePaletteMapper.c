// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2025 Tim Cocks for Adafruit Industries
//
// SPDX-License-Identifier: MIT
#include <stdlib.h>
#include "py/runtime.h"
#include "shared-bindings/tilepalettemapper/TilePaletteMapper.h"
#include "shared-bindings/displayio/Palette.h"

void common_hal_tilepalettemapper_tilepalettemapper_construct(tilepalettemapper_tilepalettemapper_t *self,
    mp_obj_t palette, uint16_t width, uint16_t height) {

    self->palette = palette;
    self->width_in_tiles = width;
    self->height_in_tiles = height;
    self->needs_refresh = false;
    int mappings_len = width * height;
    self->tile_mappings = (uint16_t **)m_malloc(mappings_len * sizeof(uint16_t *));
    uint32_t palette_len = common_hal_displayio_palette_get_len(self->palette);
    for (int i = 0; i < mappings_len; i++) {
        self->tile_mappings[i] = (uint16_t *)m_malloc(palette_len * sizeof(uint16_t));
        for (uint16_t j = 0; j < palette_len; j++) {
            self->tile_mappings[i][j] = j;
        }
    }
}

uint16_t common_hal_tilepalettemapper_tilepalettemapper_get_width(tilepalettemapper_tilepalettemapper_t *self) {
    return self->width_in_tiles;
}

uint16_t common_hal_tilepalettemapper_tilepalettemapper_get_height(tilepalettemapper_tilepalettemapper_t *self) {
    return self->height_in_tiles;
}

mp_obj_t common_hal_tilepalettemapper_tilepalettemapper_get_palette(tilepalettemapper_tilepalettemapper_t *self) {
    return self->palette;
}

mp_obj_t common_hal_tilepalettemapper_tilepalettemapper_get_mapping(tilepalettemapper_tilepalettemapper_t *self, uint16_t x, uint16_t y) {
    mp_obj_list_t *list = MP_OBJ_TO_PTR(mp_obj_new_list(0, NULL));
    uint32_t palette_len = common_hal_displayio_palette_get_len(self->palette);
    int index = x + y * self->width_in_tiles;
    for (uint32_t i = 0; i < palette_len; i++) {
        mp_obj_list_append(list, mp_obj_new_int(self->tile_mappings[index][i]));
    }
    return MP_OBJ_FROM_PTR(list);
}

void common_hal_tilepalettemapper_tilepalettemapper_set_mapping(tilepalettemapper_tilepalettemapper_t *self, uint16_t x, uint16_t y, size_t len, mp_obj_t *items) {
//  size_t len = 0;
//  mp_obj_t *items;
//  mp_obj_list_get(mapping, &len, &items);
    uint32_t palette_len = common_hal_displayio_palette_get_len(self->palette);
    for (uint16_t i = 0; i < len; i++) {
        int mapping_val = mp_arg_validate_type_int(items[i], MP_QSTR_mapping_value);
        mp_arg_validate_int_range(mapping_val, 0, palette_len - 1, MP_QSTR_mapping_value);
        self->tile_mappings[y * self->width_in_tiles + x][i] = mapping_val;
    }
    self->needs_refresh = true;
}

uint32_t tilepalettemapper_tilepalettemapper_get_color(tilepalettemapper_tilepalettemapper_t *self, uint16_t tile_index, uint32_t palette_index) {
    uint32_t mapped_index = self->tile_mappings[tile_index][palette_index];
    return common_hal_displayio_palette_get_color(self->palette, mapped_index);
}

bool tilepalettemapper_tilepalettemapper_needs_refresh(tilepalettemapper_tilepalettemapper_t *self) {
    return self->needs_refresh;
}

void tilepalettemapper_tilepalettemapper_finish_refresh(tilepalettemapper_tilepalettemapper_t *self) {
    self->needs_refresh = false;
}
