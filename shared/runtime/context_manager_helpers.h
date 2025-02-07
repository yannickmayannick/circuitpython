// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2017 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#pragma once

#include "py/obj.h"

#define default___enter___obj (mp_identity_obj)
MP_DECLARE_CONST_FUN_OBJ_VAR_BETWEEN(default___exit___obj);
