
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "py/mpconfig.h"

/** Sections defined in linker files under "linking" */
#ifdef MAX32690
#define MAX32_FLASH_SIZE  0x300000 // 3 MB
#define INTERNAL_FLASH_FILESYSTEM_SIZE 0x10000 // 64K
#define INTERNAL_FLASH_FILESYSTEM_START_ADDR 0x10032000 // Load into the last MB of code/data storage???
#endif

#define INTERNAL_FLASH_FILESYSTEM_NUM_BLOCKS (INTERNAL_FLASH_FILESYSTEM_SIZE / FILESYSTEM_BLOCK_SIZE)
