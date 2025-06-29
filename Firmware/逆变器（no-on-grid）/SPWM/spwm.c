#include "spwm.h"
#include <math.h>

#define SPWM_POINTS 300

static uint16_t spwm_table[SPWM_POINTS];
static volatile uint16_t spwm_index = 0;

extern HRTIM_HandleTypeDef hhrtim1;
#define SPWM_TIMER HRTIM_TIMERINDEX_TIMER_B
#define SPWM_CMP   HRTIM_COMPAREUNIT_1

void spwm_init(uint16_t period)
{
    for (int i = 0; i < SPWM_POINTS; ++i)
    {
        float angle = 2.0f * 3.1415926f * i / SPWM_POINTS;
        float sine_val = (sinf(angle) + 1.0f) / 2.0f;
        spwm_table[i] = (uint16_t)(sine_val * period);
    }
}

void spwm_update(void)
{
    __HAL_HRTIM_SetCompare(&hhrtim1, SPWM_TIMER, SPWM_CMP, spwm_table[spwm_index]);
    spwm_index = (spwm_index + 1) % SPWM_POINTS;
}

void spwm_start(void)
{
    HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TB1);
    HAL_HRTIM_WaveformCounterStart(&hhrtim1, SPWM_TIMER);
}
