/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Jeff Epler for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "common-hal/canio/__init__.h"
#include "shared-bindings/canio/RemoteTransmissionRequest.h"

// Convert from back from FLEXCAN IDs to normal CAN IDs.
#define FLEXCAN_ID_TO_CAN_ID_STD(id) \
    ((uint32_t)((((uint32_t)(id)) & CAN_ID_STD_MASK) >> CAN_ID_STD_SHIFT))

#define FLEXCAN_ID_TO_CAN_ID_EXT(id) \
    ((uint32_t)((((uint32_t)(id)) & (CAN_ID_STD_MASK | CAN_ID_EXT_MASK)) \
    >> CAN_ID_EXT_SHIFT))

bool mimxrt_canio_message_obj_to_flexcan_frame(canio_message_obj_t *src, flexcan_frame_t *dst)
{
    memset(dst, 0, sizeof(*dst)); // Zero out output.
    if (src == NULL) return false; // Missing input.

    if (src->extended) { // CAN Frame Identifier (STD or EXT format).
        dst->id = FLEXCAN_ID_EXT(src->id); // CAN Frame Identifier (EXT).
        dst->format = kFLEXCAN_FrameFormatExtend;
    } else {
        dst->id = FLEXCAN_ID_STD(src->id); // CAN Frame Identifier (STD).
        dst->format = kFLEXCAN_FrameFormatStandard;
    }

    bool rtr = src->base.type == &canio_remote_transmission_request_type;
    if (rtr) { // CAN Frame Type(DATA or REMOTE).
        dst->type = kFLEXCAN_FrameTypeRemote;
    } else {
        dst->type = kFLEXCAN_FrameTypeData;
    }

    dst->length = src->size; // CAN frame data length in bytes (Range: 0~8). 

    dst->dataByte0 = src->data[0];
    dst->dataByte1 = src->data[1];
    dst->dataByte2 = src->data[2];
    dst->dataByte3 = src->data[3];
    dst->dataByte4 = src->data[4];
    dst->dataByte5 = src->data[5];
    dst->dataByte6 = src->data[6];
    dst->dataByte7 = src->data[7];

    return true;
}

bool mimxrt_flexcan_frame_to_canio_message_obj(flexcan_frame_t *src, canio_message_obj_t *dst)
{
    memset(dst, 0, sizeof(*dst)); // Zero out output.
    if (src == NULL) return false; // Missing input.

    if (src->format == kFLEXCAN_FrameFormatExtend) { // CAN Frame Identifier (STD or EXT format).
        dst->extended = true;
        dst->id = FLEXCAN_ID_TO_CAN_ID_EXT(src->id); // CAN Frame Identifier (EXT).
    } else {
        dst->extended = false;
        dst->id = FLEXCAN_ID_TO_CAN_ID_STD(src->id); // CAN Frame Identifier (STD).
    }

    if (src->type == kFLEXCAN_FrameTypeRemote) { // CAN Frame Type(DATA or REMOTE).
        dst->base.type = &canio_remote_transmission_request_type;
    } else {
        dst->base.type = &canio_message_type;
    }

    dst->size = src->length; // CAN frame data length in bytes (Range: 0~8). 

    dst->data[0] = src->dataByte0;
    dst->data[1] = src->dataByte1;
    dst->data[2] = src->dataByte2;
    dst->data[3] = src->dataByte3;
    dst->data[4] = src->dataByte4;
    dst->data[5] = src->dataByte5;
    dst->data[6] = src->dataByte6;
    dst->data[7] = src->dataByte7;

    return true;
}
