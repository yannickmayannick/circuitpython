// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2025 snkYmkrct
//
// based on implementation from https://github.com/micropython/micropython/blob/master/ports/stm32/sdram.c
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include STM32_HAL_H
#include "common-hal/microcontroller/Pin.h"
#include "peripherals/periph.h"
#if defined(STM32H7)
#include "stm32h7xx_ll_fmc.h"
#include "stm32h7xx_hal_sdram.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_cortex.h"
#endif
#ifdef STM32H750xx
#include "stm32h7/stm32h750xx/periph.h"
#endif


#include "sdram.h"


#if MICROPY_DEBUG_VERBOSE // print debugging info
#define DEBUG_PRINT (1)
#define DEBUG_printf DEBUG_printf
#define DEBUG_OP_printf(...) DEBUG_printf(__VA_ARGS__)
#else // don't print debugging info
#define DEBUG_printf(...) (void)0
#define DEBUG_OP_printf(...) (void)0
#endif

#define SDRAM_TIMEOUT                            ((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

#if defined(CIRCUITPY_HW_FMC_SDCKE0) && defined(CIRCUITPY_HW_FMC_SDNE0)
#define FMC_SDRAM_BANK FMC_SDRAM_BANK1
#define FMC_SDRAM_CMD_TARGET_BANK FMC_SDRAM_CMD_TARGET_BANK1
#if CIRCUITPY_HW_FMC_SWAP_BANKS
#define SDRAM_START_ADDRESS 0x60000000
#else
#define SDRAM_START_ADDRESS 0xC0000000
#endif
#elif defined(CIRCUITPY_HW_FMC_SDCKE1) && defined(CIRCUITPY_HW_FMC_SDNE1)
#define FMC_SDRAM_BANK FMC_SDRAM_BANK2
#define FMC_SDRAM_CMD_TARGET_BANK FMC_SDRAM_CMD_TARGET_BANK2
#if CIRCUITPY_HW_FMC_SWAP_BANKS
#define SDRAM_START_ADDRESS 0x70000000
#else
#define SDRAM_START_ADDRESS 0xD0000000
#endif
#endif

#ifdef FMC_SDRAM_BANK

static uint8_t FMC_Initialized = 0;
static SDRAM_HandleTypeDef hsdram = {0};


static void sdram_init_seq(void);

bool sdram_init(void) {
    FMC_SDRAM_TimingTypeDef SDRAM_Timing = {0};

    if (!FMC_Initialized) {

        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

        /** Initializes the peripherals clock
        */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
        PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            DEBUG_printf("sdram: %s", "periph init clock error");
        }
        /* Peripheral clock enable */
        __HAL_RCC_FMC_CLK_ENABLE();
        FMC_Initialized = 1;
    }

    #if CIRCUITPY_HW_FMC_SWAP_BANKS
    HAL_SetFMCMemorySwappingConfig(FMC_SWAPBMAP_SDRAM_SRAM);
    #endif

    for (uint i = 0; i < MP_ARRAY_SIZE(sdram_pin_list); i++) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = pin_mask(sdram_pin_list[i].pin->number);
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
        HAL_GPIO_Init(pin_port(sdram_pin_list[i].pin->port), &GPIO_InitStruct);
        never_reset_pin_number(sdram_pin_list[i].pin->port, sdram_pin_list[i].pin->number);
    }
    /* SDRAM device configuration */
    hsdram.Instance = FMC_SDRAM_DEVICE;
    /* Timing configuration for 90 Mhz of SD clock frequency (180Mhz/2) */
    /* TMRD: 2 Clock cycles */
    SDRAM_Timing.LoadToActiveDelay = CIRCUITPY_HW_SDRAM_TIMING_TMRD;
    /* TXSR: min=70ns (6x11.90ns) */
    SDRAM_Timing.ExitSelfRefreshDelay = CIRCUITPY_HW_SDRAM_TIMING_TXSR;
    /* TRAS */
    SDRAM_Timing.SelfRefreshTime = CIRCUITPY_HW_SDRAM_TIMING_TRAS;
    /* TRC */
    SDRAM_Timing.RowCycleDelay = CIRCUITPY_HW_SDRAM_TIMING_TRC;
    /* TWR */
    SDRAM_Timing.WriteRecoveryTime = CIRCUITPY_HW_SDRAM_TIMING_TWR;
    /* TRP */
    SDRAM_Timing.RPDelay = CIRCUITPY_HW_SDRAM_TIMING_TRP;
    /* TRCD */
    SDRAM_Timing.RCDDelay = CIRCUITPY_HW_SDRAM_TIMING_TRCD;

    #define _FMC_INIT(x, n) x##_##n
    #define FMC_INIT(x, n) _FMC_INIT(x,  n)

    hsdram.Init.SDBank = FMC_SDRAM_BANK;
    hsdram.Init.ColumnBitsNumber = FMC_INIT(FMC_SDRAM_COLUMN_BITS_NUM, CIRCUITPY_HW_SDRAM_COLUMN_BITS_NUM);
    hsdram.Init.RowBitsNumber = FMC_INIT(FMC_SDRAM_ROW_BITS_NUM, CIRCUITPY_HW_SDRAM_ROW_BITS_NUM);
    hsdram.Init.MemoryDataWidth = FMC_INIT(FMC_SDRAM_MEM_BUS_WIDTH, CIRCUITPY_HW_SDRAM_MEM_BUS_WIDTH);
    hsdram.Init.InternalBankNumber = FMC_INIT(FMC_SDRAM_INTERN_BANKS_NUM, CIRCUITPY_HW_SDRAM_INTERN_BANKS_NUM);
    hsdram.Init.CASLatency = FMC_INIT(FMC_SDRAM_CAS_LATENCY, CIRCUITPY_HW_SDRAM_CAS_LATENCY);
    hsdram.Init.SDClockPeriod = FMC_INIT(FMC_SDRAM_CLOCK_PERIOD, CIRCUITPY_HW_SDRAM_CLOCK_PERIOD);
    hsdram.Init.ReadPipeDelay = FMC_INIT(FMC_SDRAM_RPIPE_DELAY, CIRCUITPY_HW_SDRAM_RPIPE_DELAY);
    hsdram.Init.ReadBurst = (CIRCUITPY_HW_SDRAM_RBURST) ? FMC_SDRAM_RBURST_ENABLE : FMC_SDRAM_RBURST_DISABLE;
    hsdram.Init.WriteProtection = (CIRCUITPY_HW_SDRAM_WRITE_PROTECTION) ? FMC_SDRAM_WRITE_PROTECTION_ENABLE : FMC_SDRAM_WRITE_PROTECTION_DISABLE;

    /* Initialize the SDRAM controller */
    if (HAL_SDRAM_Init(&hsdram, &SDRAM_Timing) != HAL_OK) {
        DEBUG_printf("sdram: %s", "init error");
        return false;
    }

    sdram_init_seq();

    return true;
}

void sdram_deinit(void) {
    FMC_SDRAM_CommandTypeDef command = {0};
    if (FMC_Initialized) {
        /* Send the module into powerdown mode */
        command.CommandMode = FMC_SDRAM_CMD_POWERDOWN_MODE;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 1;
        command.ModeRegisterDefinition = 0;

        /* Send the command */
        if (HAL_SDRAM_SendCommand(&hsdram, &command, HAL_MAX_DELAY) != HAL_OK) {
            DEBUG_printf("sdram power off error:%s:%d", __func__, __LINE__);
        }
    }
}

void *sdram_start(void) {
    return (void *)SDRAM_START_ADDRESS;
}

void *sdram_end(void) {
    return (void *)(SDRAM_START_ADDRESS + CIRCUITPY_HW_SDRAM_SIZE);
}

uint32_t sdram_size(void) {
    return CIRCUITPY_HW_SDRAM_SIZE;
}

static void sdram_init_seq(void) {
    FMC_SDRAM_CommandTypeDef command = {0};
    /* Program the SDRAM external device */
    __IO uint32_t tmpmrd = 0;

    /* Configure a clock configuration enable command */
    command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK;
    command.AutoRefreshNumber = 1;
    command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&hsdram, &command, HAL_MAX_DELAY);

    /* Insert 100 ms delay */
    HAL_Delay(100);

    /* Configure a PALL (precharge all) command */
    command.CommandMode = FMC_SDRAM_CMD_PALL;
    command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK;
    command.AutoRefreshNumber = 1;
    command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&hsdram, &command, HAL_MAX_DELAY);

    /* Configure a Auto-Refresh command */
    command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK;
    command.AutoRefreshNumber = CIRCUITPY_HW_SDRAM_AUTOREFRESH_NUM;
    command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(&hsdram, &command, HAL_MAX_DELAY);

    /* Program the external memory mode register */
    tmpmrd = (uint32_t)0x0 | FMC_INIT(SDRAM_MODEREG_BURST_LENGTH, CIRCUITPY_HW_SDRAM_BURST_LENGTH) |
        SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
        FMC_INIT(SDRAM_MODEREG_CAS_LATENCY, CIRCUITPY_HW_SDRAM_CAS_LATENCY) |
        SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK;
    command.AutoRefreshNumber = 1;
    command.ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    HAL_SDRAM_SendCommand(&hsdram, &command, HAL_MAX_DELAY);

    /* Set the refresh rate counter.
       Assuming 100MHz frequency, 8192 refresh cycles and 64ms refresh rate:
       RefreshRate = 64 ms / 8192 cyc = 7.8125 us/cyc
       RefreshCycles = 7.8125 us * 100 MHz = 782
       According to the formula on p.1665 of the reference manual,
       we also need to subtract 20 from the value, so the target
       refresh rate is 782 - 20 = 762
     */

    #define REFRESH_COUNT (CIRCUITPY_HW_SDRAM_REFRESH_RATE * CIRCUITPY_HW_SDRAM_FREQUENCY_KHZ / CIRCUITPY_HW_SDRAM_REFRESH_CYCLES - 20)

    HAL_SDRAM_ProgramRefreshRate(&hsdram, REFRESH_COUNT);

    #if defined(STM32F7) || defined(STM32H7)
    __disable_irq();
    MPU_Region_InitTypeDef MPU_InitStruct = {0};
    /** Enable caching for SDRAM to support non-alligned access.
    */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = CPY_SDRAM_REGION;
    MPU_InitStruct.BaseAddress = SDRAM_START_ADDRESS;
    MPU_InitStruct.Size = CPY_SDRAM_REGION_SIZE;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    __ISB();
    __DSB();
    __DMB();
    __enable_irq();

    #endif

}

bool __attribute__((optimize("Os"))) sdram_test(bool exhaustive) {
    uint8_t const pattern = 0xaa;
    uint8_t const antipattern = 0x55;
    volatile uint8_t *const mem_base = (uint8_t *)sdram_start();

    char error_buffer[1024];

    DEBUG_printf("sdram: %s\n", "sdram test started");

    #if (__DCACHE_PRESENT == 1)
    bool i_cache_disabled = false;
    bool d_cache_disabled = false;

    // Disable caches for testing.
    if (SCB->CCR & (uint32_t)SCB_CCR_IC_Msk) {
        SCB_DisableICache();
        i_cache_disabled = true;
    }

    if (SCB->CCR & (uint32_t)SCB_CCR_DC_Msk) {
        SCB_DisableDCache();
        d_cache_disabled = true;
    }
    #endif

    // Test data bus
    for (uint32_t i = 0; i < CIRCUITPY_HW_SDRAM_MEM_BUS_WIDTH; i++) {
        *((volatile uint32_t *)mem_base) = (1u << i);
        __DSB();
        if (*((volatile uint32_t *)mem_base) != (1u << i)) {
            snprintf(error_buffer, sizeof(error_buffer),
                "Data bus test failed at 0x%p expected 0x%x found 0x%lx",
                &mem_base[0], (1 << i), ((volatile uint32_t *)mem_base)[0]);
            DEBUG_printf("error: %s\n", error_buffer);
            return false;
        }
    }

    // Test address bus
    for (uint32_t i = 1; i < CIRCUITPY_HW_SDRAM_SIZE; i <<= 1) {
        mem_base[i] = pattern;
        __DSB();
        if (mem_base[i] != pattern) {
            snprintf(error_buffer, sizeof(error_buffer),
                "Address bus test failed at 0x%p expected 0x%x found 0x%x",
                &mem_base[i], pattern, mem_base[i]);
            DEBUG_printf("error: %s\n", error_buffer);
            return false;
        }
    }

    // Check for aliasing (overlapping addresses)
    mem_base[0] = antipattern;
    __DSB();
    for (uint32_t i = 1; i < CIRCUITPY_HW_SDRAM_SIZE; i <<= 1) {
        if (mem_base[i] != pattern) {
            snprintf(error_buffer, sizeof(error_buffer),
                "Address bus overlap at 0x%p expected 0x%x found 0x%x",
                &mem_base[i], pattern, mem_base[i]);
            DEBUG_printf("error: %s\n", error_buffer);
            return false;
        }
    }

    // Test all RAM cells
    if (exhaustive) {
        // Write all memory first then compare, so even if the cache
        // is enabled, it's not just writing and reading from cache.
        // Note: This test should also detect refresh rate issues.
        for (uint32_t i = 0; i < CIRCUITPY_HW_SDRAM_SIZE; i++) {
            mem_base[i] = ((i % 2) ? pattern : antipattern);
        }

        for (uint32_t i = 0; i < CIRCUITPY_HW_SDRAM_SIZE; i++) {
            if (mem_base[i] != ((i % 2) ? pattern : antipattern)) {
                snprintf(error_buffer, sizeof(error_buffer),
                    "Address bus slow test failed at 0x%p expected 0x%x found 0x%x",
                    &mem_base[i], ((i % 2) ? pattern : antipattern), mem_base[i]);
                DEBUG_printf("error: %s\n", error_buffer);
                return false;
            }
        }
    }

    #if (__DCACHE_PRESENT == 1)
    // Re-enable caches if they were enabled before the test started.
    if (i_cache_disabled) {
        SCB_EnableICache();
    }

    if (d_cache_disabled) {
        SCB_EnableDCache();
    }
    #endif

    DEBUG_printf("sdram: %s\n", "sdram test successfully!");

    return true;
}


#endif // FMC_SDRAM_BANK
