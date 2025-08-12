//
// Created by MDLZ on 25-2-24.
//

#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

#define TMR_COUNT 10 // 最大软件定时器数量，不要太大
#include <stdint.h>

typedef enum {
    TMR_ONCE_MODE = 0,
    TMR_AUTO_MODE = 1
} TMR_MODE_E;

typedef struct {
    volatile uint8_t Mode;
    volatile uint8_t Flag;
    volatile uint32_t Count;
    volatile uint32_t PreLoad;
} SOFT_TMR;

extern SOFT_TMR s_tTmr[TMR_COUNT];

void InitSoftTimer(void);

void StartSoftTimer(uint8_t _id, uint32_t _period);

void StartSoftAutoTimer(uint8_t _id, uint32_t _period);

void StopSoftTimer(uint8_t _id);

uint8_t CheckSoftTimer(uint8_t _id);

void SoftTimerDec(SOFT_TMR *_tmr);

#endif //SOFT_TIMER_H
