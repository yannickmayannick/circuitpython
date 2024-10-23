// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Cooper Dalrymple
//
// SPDX-License-Identifier: MIT

#include "py/enum.h"

#include "shared-bindings/audiofilters/DistortionMode.h"

MAKE_ENUM_VALUE(audiofilters_distortion_mode_type, distortion_mode, CLIP, DISTORTION_MODE_CLIP);
MAKE_ENUM_VALUE(audiofilters_distortion_mode_type, distortion_mode, ATAN, DISTORTION_MODE_ATAN);
MAKE_ENUM_VALUE(audiofilters_distortion_mode_type, distortion_mode, LOFI, DISTORTION_MODE_LOFI);
MAKE_ENUM_VALUE(audiofilters_distortion_mode_type, distortion_mode, OVERDRIVE, DISTORTION_MODE_OVERDRIVE);
MAKE_ENUM_VALUE(audiofilters_distortion_mode_type, distortion_mode, WAVESHAPE, DISTORTION_MODE_WAVESHAPE);

//| class DistortionMode:
//|     """The method of distortion used by the `audiofilters.Distortion` effect."""
//|
//|     CLIP: DistortionMode
//|     """Digital distortion effect which cuts off peaks at the top and bottom of the waveform."""
//|
//|     ATAN: DistortionMode
//|     """"""
//|
//|     LOFI: DistortionMode
//|     """Low-resolution digital distortion effect (bit depth reduction). You can use it to emulate the sound of early digital audio devices."""
//|
//|     OVERDRIVE: DistortionMode
//|     """Emulates the warm distortion produced by a field effect transistor, which is commonly used in solid-state musical instrument amplifiers. The `audiofilters.Distortion.drive` property has no effect in this mode."""
//|
//|     WAVESHAPE: DistortionMode
//|     """Waveshaper distortions are used mainly by electronic musicians to achieve an extra-abrasive sound."""
//|
MAKE_ENUM_MAP(audiofilters_distortion_mode) {
    MAKE_ENUM_MAP_ENTRY(distortion_mode, CLIP),
    MAKE_ENUM_MAP_ENTRY(distortion_mode, ATAN),
    MAKE_ENUM_MAP_ENTRY(distortion_mode, LOFI),
    MAKE_ENUM_MAP_ENTRY(distortion_mode, OVERDRIVE),
    MAKE_ENUM_MAP_ENTRY(distortion_mode, WAVESHAPE),
};
static MP_DEFINE_CONST_DICT(audiofilters_distortion_mode_locals_dict, audiofilters_distortion_mode_locals_table);

MAKE_PRINTER(audiofilters, audiofilters_distortion_mode);

MAKE_ENUM_TYPE(audiofilters, DistortionMode, audiofilters_distortion_mode);

audiofilters_distortion_mode_t validate_distortion_mode(mp_obj_t obj, qstr arg_name) {
    return cp_enum_value(&audiofilters_distortion_mode_type, obj, arg_name);
}
