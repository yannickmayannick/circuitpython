// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2019 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include "shared-module/terminalio/Terminal.h"

#include "shared-module/fontio/BuiltinFont.h"
#include "shared-bindings/displayio/TileGrid.h"
#include "shared-bindings/displayio/Palette.h"
#include "shared-bindings/terminalio/Terminal.h"

#if CIRCUITPY_STATUS_BAR
#include "shared-bindings/supervisor/__init__.h"
#include "shared-bindings/supervisor/StatusBar.h"
#endif

void terminalio_terminal_clear_status_bar(terminalio_terminal_obj_t *self) {
    if (self->status_bar) {
        common_hal_displayio_tilegrid_set_all_tiles(self->status_bar, 0);
    }
}

void common_hal_terminalio_terminal_construct(terminalio_terminal_obj_t *self,
    displayio_tilegrid_t *scroll_area, const fontio_builtinfont_t *font,
    displayio_tilegrid_t *status_bar) {
    self->cursor_x = 0;
    self->cursor_y = 0;
    self->font = font;
    self->scroll_area = scroll_area;
    self->status_bar = status_bar;
    self->status_x = 0;
    self->status_y = 0;
    self->first_row = 0;
    common_hal_displayio_tilegrid_set_all_tiles(self->scroll_area, 0);
    if (self->status_bar) {
        common_hal_displayio_tilegrid_set_all_tiles(self->status_bar, 0);
    }

    common_hal_displayio_tilegrid_set_top_left(self->scroll_area, 0, 1);
}

size_t common_hal_terminalio_terminal_write(terminalio_terminal_obj_t *self, const byte *data, size_t len, int *errcode) {
    // Make sure the terminal is initialized before we do anything with it.
    if (self->scroll_area == NULL) {
        return len;
    }

    uint32_t _select_color(uint16_t ascii_color) {
        uint32_t color_value = 0;
        if ((ascii_color & 1) > 0) {
            color_value += 0xff0000;
        }
        if ((ascii_color & 2) > 0) {
            color_value += 0x00ff00;
        }
        if ((ascii_color & 4) > 0) {
            color_value += 0x0000ff;
        }

        return color_value;
    }

    displayio_palette_t *terminal_palette = self->scroll_area->pixel_shader;
    const byte *i = data;
    uint16_t start_y = self->cursor_y;
    while (i < data + len) {
        unichar c = utf8_get_char(i);
        i = utf8_next_char(i);
        if (self->in_osc_command) {
            if (c == 0x1b && i[0] == '\\') {
                self->in_osc_command = false;
                self->status_x = 0;
                self->status_y = 0;
                i += 1;
            } else if (
                self->osc_command == 0 &&
                self->status_bar != NULL &&
                self->status_y < self->status_bar->height_in_tiles) {
                uint8_t tile_index = fontio_builtinfont_get_glyph_index(self->font, c);
                if (tile_index != 0xff) {
                    // Clear the tile grid before we start putting new info.
                    if (self->status_x == 0 && self->status_y == 0) {
                        common_hal_displayio_tilegrid_set_all_tiles(self->status_bar, 0);
                    }
                    common_hal_displayio_tilegrid_set_tile(self->status_bar, self->status_x, self->status_y, tile_index);
                    self->status_x++;
                    if (self->status_x >= self->status_bar->width_in_tiles) {
                        self->status_y++;
                        self->status_x %= self->status_bar->width_in_tiles;
                    }
                }
            }
            continue;
        }
        // Always handle ASCII.
        if (c < 128) {
            if (c >= 0x20 && c <= 0x7e) {
                uint8_t tile_index = fontio_builtinfont_get_glyph_index(self->font, c);
                common_hal_displayio_tilegrid_set_tile(self->scroll_area, self->cursor_x, self->cursor_y, tile_index);
                self->cursor_x++;
            } else if (c == '\r') {
                self->cursor_x = 0;
            } else if (c == '\n') {
                self->cursor_y++;
                // Commands below are used by MicroPython in the REPL
            } else if (c == '\b') {
                if (self->cursor_x > 0) {
                    self->cursor_x--;
                }
            } else if (c == 0x1b) {
                // Handle commands of the form [ESC].<digits><command-char> where . is not yet known.
                uint16_t n = 0;
                uint8_t j = 1;
                uint8_t j2 = 0;
                for (; j < 6; j++) {
                    if ('0' <= i[j] && i[j] <= '9') {
                        n = n * 10 + (i[j] - '0');
                    } else {
                        c = i[j];
                        break;
                    }
                }
                if (i[0] == '[') {
                    if (i[1] == 'K') {
                        // Clear the rest of the line.
                        for (uint16_t k = self->cursor_x; k < self->scroll_area->width_in_tiles; k++) {
                            common_hal_displayio_tilegrid_set_tile(self->scroll_area, k, self->cursor_y, 0);
                        }
                        i += 2;
                    } else if (i[1] == '?') {
                        if (i[2] == '2' && i[3] == '5') {
                            // cursor visibility commands
                            if (i[4] == 'h') {
                                // make cursor visible
                                // not implemented yet
                            } else if (i[4] == 'l') {
                                // make cursor invisible
                                // not implemented yet
                            }
                        }
                        i += 5;
                    } else {
                        if (c == 'D') {
                            if (n > self->cursor_x) {
                                self->cursor_x = 0;
                            } else {
                                self->cursor_x -= n;
                            }
                        }
                        if (c == 'J') {
                            if (n == 2) {
                                common_hal_displayio_tilegrid_set_top_left(self->scroll_area, 0, 0);
                                self->cursor_x = self->cursor_y = start_y = 0;
                                n = 0;
                                common_hal_displayio_tilegrid_set_all_tiles(self->scroll_area, 0);
                            }
                        }
                        if (c == 'm') {
                            if ((n >= 40 && n <= 47) || (n >= 30 && n <= 37)) {
                                common_hal_displayio_palette_set_color(terminal_palette, 1 - (n / 40), _select_color(n % 10));
                            }
                            if (n == 0) {
                                common_hal_displayio_palette_set_color(terminal_palette, 0, 0x000000);
                                common_hal_displayio_palette_set_color(terminal_palette, 1, 0xffffff);
                            }
                        }
                        if (c == 'H') {
                            self->cursor_x = self->cursor_y = start_y = 0;
                        }
                        if (c == ';') {
                            uint16_t m = 0;
                            uint8_t m2 = 0;
                            for (++j; j < 9; j++) {
                                if ('0' <= i[j] && i[j] <= '9') {
                                    m = m * 10 + (i[j] - '0');
                                } else {
                                    c = i[j];
                                    break;
                                }
                            }
                            if (c == ';') {
                                for (++j2; j2 < 9; j2++) {
                                    if ('0' <= i[j2] && i[j2] <= '9') {
                                        m2 = m2 * 10 + (i[j2] - '0');
                                    } else {
                                        c = i[j2];
                                        break;
                                    }
                                }
                            }
                            if (c == 'H') {
                                if (n > 0) {
                                    n--;
                                }
                                if (m > 0) {
                                    m--;
                                }
                                if (n >= self->scroll_area->height_in_tiles) {
                                    n = self->scroll_area->height_in_tiles - 1;
                                }
                                if (m >= self->scroll_area->width_in_tiles) {
                                    m = self->scroll_area->width_in_tiles - 1;
                                }
                                n = (n + self->scroll_area->top_left_y) % self->scroll_area->height_in_tiles;
                                self->cursor_x = m;
                                self->cursor_y = n;
                                start_y = self->cursor_y;
                            }
                            if (c == 'm') {
                                if ((n >= 40 && n <= 47) || (n >= 30 && n <= 37)) {
                                    common_hal_displayio_palette_set_color(terminal_palette, 1 - (n / 40), _select_color(n % 10));
                                }
                                if (n == 0) {
                                    common_hal_displayio_palette_set_color(terminal_palette, 0, 0x000000);
                                    common_hal_displayio_palette_set_color(terminal_palette, 1, 0xffffff);
                                }
                                if ((m >= 40 && m <= 47) || (m >= 30 && m <= 37)) {
                                    common_hal_displayio_palette_set_color(terminal_palette, 1 - (m / 40), _select_color(m % 10));
                                }
                                if (m == 0) {
                                    common_hal_displayio_palette_set_color(terminal_palette, 0, 0x000000);
                                    common_hal_displayio_palette_set_color(terminal_palette, 1, 0xffffff);
                                }
                                if ((m2 >= 40 && m2 <= 47) || (m2 >= 30 && m2 <= 37)) {
                                    common_hal_displayio_palette_set_color(terminal_palette, 1 - (m2 / 40), _select_color(m2 % 10));
                                }
                                if (m2 == 0) {
                                    common_hal_displayio_palette_set_color(terminal_palette, 0, 0x000000);
                                    common_hal_displayio_palette_set_color(terminal_palette, 1, 0xffffff);
                                }
                            }
                        }
                        i += j + j2 + 1;
                        continue;
                    }
                } else if (i[0] == 'M') {
                    if (self->cursor_y != self->scroll_area->top_left_y) {
                        if (self->cursor_y > 0) {
                            self->cursor_y = self->cursor_y - 1;
                        } else {
                            self->cursor_y = self->scroll_area->height_in_tiles - 1;
                        }
                    } else {
                        if (self->cursor_y > 0) {
                            common_hal_displayio_tilegrid_set_top_left(self->scroll_area, 0, self->cursor_y - 1);
                        } else {
                            common_hal_displayio_tilegrid_set_top_left(self->scroll_area, 0, self->scroll_area->height_in_tiles - 1);
                        }
                        for (uint8_t icol = 0; icol < self->scroll_area->width_in_tiles; icol++) {
                            common_hal_displayio_tilegrid_set_tile(self->scroll_area, icol, self->scroll_area->top_left_y, 0);
                        }

                        self->cursor_x = 0;
                        self->cursor_y = self->scroll_area->top_left_y;
                    }
                    start_y = self->cursor_y;
                    i++;
                } else if (i[0] == 'D') {
                    self->cursor_y = (self->cursor_y + 1) % self->scroll_area->height_in_tiles;
                    if (self->cursor_y == self->scroll_area->top_left_y) {
                        common_hal_displayio_tilegrid_set_top_left(self->scroll_area, 0, (self->cursor_y + 1) % self->scroll_area->height_in_tiles);
                        for (uint8_t icol = 0; icol < self->scroll_area->width_in_tiles; icol++) {
                            common_hal_displayio_tilegrid_set_tile(self->scroll_area, icol, self->cursor_y, 0);
                        }
                        self->cursor_x = 0;
                    }
                    start_y = self->cursor_y;
                    i++;
                } else if (i[0] == ']' && c == ';') {
                    self->in_osc_command = true;
                    self->osc_command = n;
                    i += j + 1;
                }
            }
        } else {
            uint8_t tile_index = fontio_builtinfont_get_glyph_index(self->font, c);
            if (tile_index != 0xff) {
                common_hal_displayio_tilegrid_set_tile(self->scroll_area, self->cursor_x, self->cursor_y, tile_index);
                self->cursor_x++;

            }
        }
        if (self->cursor_x >= self->scroll_area->width_in_tiles) {
            self->cursor_y++;
            self->cursor_x %= self->scroll_area->width_in_tiles;
        }
        if (self->cursor_y >= self->scroll_area->height_in_tiles) {
            self->cursor_y %= self->scroll_area->height_in_tiles;
        }
        if (self->cursor_y != start_y) {
            // clear the new row in case of scroll up
            if (self->cursor_y == self->scroll_area->top_left_y) {
                for (uint16_t j = 0; j < self->scroll_area->width_in_tiles; j++) {
                    common_hal_displayio_tilegrid_set_tile(self->scroll_area, j, self->cursor_y, 0);
                }
                common_hal_displayio_tilegrid_set_top_left(self->scroll_area, 0, (self->cursor_y + self->scroll_area->height_in_tiles + 1) % self->scroll_area->height_in_tiles);
            }
            start_y = self->cursor_y;
        }
    }
    return i - data;
}

bool common_hal_terminalio_terminal_ready_to_tx(terminalio_terminal_obj_t *self) {
    return self->scroll_area != NULL;
}
