// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Cooper Dalrymple
//
// SPDX-License-Identifier: MIT

#pragma once

#include "py/enum.h"

typedef enum audiofilters_distortion_mode_e {
    DISTORTION_MODE_CLIP,
    DISTORTION_MODE_ATAN,
    DISTORTION_MODE_LOFI,
    DISTORTION_MODE_OVERDRIVE,
    DISTORTION_MODE_WAVESHAPE,
} audiofilters_distortion_mode_t;

extern const cp_enum_obj_t distortion_mode_CLIP_obj;
extern const mp_obj_type_t audiofilters_distortion_mode_type;

extern audiofilters_distortion_mode_t validate_distortion_mode(mp_obj_t obj, qstr arg_name);
