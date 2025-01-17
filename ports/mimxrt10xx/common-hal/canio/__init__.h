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

#pragma once

#include "shared-module/canio/Message.h"
#include "sdk/drivers/flexcan/fsl_flexcan.h"

typedef struct canio_listener canio_listener_t;
typedef struct canio_can canio_can_t;
typedef uint32_t canio_can_filter_t;

// There are 64 message buffers in each mimxrt10xx chip.
// Rx fifo will use the message buffers at the front (from index zero).
// As far as I can see rx fifo uses message buffer 0 to 5.
// Also id filter table might occupy message buffer memory area 6 to 37.
//
// Let's use the last few message buffers for sending.
//
// We use 8 (tx) message buffers in total.
// This makes tracking the state (free/busy) of each (tx) message buffer easy.
// We can use a single byte, where each bit represent the state.
// Zero means the message buffer can be used for sending.
// One means the message buffer is busy sending.
// If you make this larger then you have to change the type of 'tx_state' in struct 'mimxrt10xx_flexcan_data_t'.
#define MIMXRT10XX_FLEXCAN_TX_MB_NUM (8)

// For safety we use SDK provided macro to get the last message buffer index instead of hard coding it.
#define MIMXRT10XX_FLEXCAN_TX_MBID_MAX (FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(0))
#define MIMXRT10XX_FLEXCAN_TX_MBID_MIN (MIMXRT10XX_FLEXCAN_TX_MBID_MAX - MIMXRT10XX_FLEXCAN_TX_MB_NUM)

// Convert from tx message buffer index to frame array index.
#define MIMXRT10XX_FLEXCAN_TX_MBID_TO_ARRID(x) (x - MIMXRT10XX_FLEXCAN_TX_MBID_MIN)

// Convert from frame array index to tx message buffer index.
#define MIMXRT10XX_FLEXCAN_TX_ARRID_TO_MBID(x) (x + MIMXRT10XX_FLEXCAN_TX_MBID_MIN)

// We limit the amount of filter+mask pairs to 8 because above that the filters
// are impacted by the global mask rather than individual masks alone, which is
// not compatible with the current canio implementation.
//
// See Table 44-22 of the i.MX RT1060 Processor Reference Manual, Rev. 3
// for more details.
#define MIMXRT10XX_FLEXCAN_RX_FILTER_COUNT (8)

// Enables/disables SDK calculated "improved" timing configuration.
#define MIMXRT10XX_FLEXCAN_USE_IMPROVED_TIMING_CONFIG (1)

typedef struct {
    CAN_Type *base; // FlexCAN peripheral base address.
    flexcan_handle_t handle; // FlexCAN handle which can be used for FlexCAN transactional APIs.
    uint8_t tx_state;
    uint32_t rx_fifo_filter[MIMXRT10XX_FLEXCAN_RX_FILTER_COUNT];
} mimxrt10xx_flexcan_data_t;

bool mimxrt_canio_message_obj_to_flexcan_frame(canio_message_obj_t *src, flexcan_frame_t *dst);
bool mimxrt_flexcan_frame_to_canio_message_obj(flexcan_frame_t *src, canio_message_obj_t *dst);

