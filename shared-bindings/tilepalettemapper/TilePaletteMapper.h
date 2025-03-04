//
// Created by timc on 3/4/25.
//
#include "shared-module/tilepalettemapper/TilePaletteMapper.h"

extern const mp_obj_type_t tilepalettemapper_tilepalettemapper_type;

void common_hal_tilepalettemapper_tilepalettemapper_construct(tilepalettemapper_tilepalettemapper_t *self,
    mp_obj_t paltte, uint16_t bitmap_width_in_tiles, uint16_t bitmap_height_in_tiles);


uint16_t common_hal_tilepalettemapper_tilepalettemapper_get_width(tilepalettemapper_tilepalettemapper_t *self);
uint16_t common_hal_tilepalettemapper_tilepalettemapper_get_height(tilepalettemapper_tilepalettemapper_t *self);
mp_obj_t common_hal_tilepalettemapper_tilepalettemapper_get_palette(tilepalettemapper_tilepalettemapper_t *self);
mp_obj_t common_hal_tilepalettemapper_tilepalettemapper_get_mapping(tilepalettemapper_tilepalettemapper_t *self, uint16_t x, uint16_t y);
void common_hal_tilepalettemapper_tilepalettemapper_set_mapping(tilepalettemapper_tilepalettemapper_t *self, uint16_t x, uint16_t y, size_t len, mp_obj_t *items);

uint32_t common_hal_tilepalettemapper_tilepalettemapper_get_color(tilepalettemapper_tilepalettemapper_t *self, uint16_t tile_index, uint32_t palette_index);