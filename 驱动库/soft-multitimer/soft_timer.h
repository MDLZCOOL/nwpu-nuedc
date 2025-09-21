/*
 * SPDX-FileCopyrightText: 2025 MDLZCOOL <mdlzcool@foxmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#define TMR_COUNT 5 // 最大软件定时器数量，不要太大(0~N-1)
#include <stdint.h>

typedef enum
{
    TMR_ONCE_MODE = 0,
    TMR_AUTO_MODE = 1
} TMR_MODE_E;

typedef struct
{
    volatile uint8_t Mode;
    volatile uint8_t Flag;
    volatile uint32_t Count;
    volatile uint32_t PreLoad;
} SOFT_TMR;

extern SOFT_TMR s_tTmr[TMR_COUNT];

void InitSoftTimer(void);

void StartSoftTimer(uint8_t _id, uint32_t _period); // 单次

void StartSoftAutoTimer(uint8_t _id, uint32_t _period); // 自动

void StopSoftTimer(uint8_t _id);

uint8_t CheckSoftTimer(uint8_t _id);

void SoftTimerDec(SOFT_TMR *_tmr);
