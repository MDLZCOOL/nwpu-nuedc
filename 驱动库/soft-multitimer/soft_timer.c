//
// Created by MDLZ on 25-2-24.
//

#include "soft_timer.h"

#include "cmsis_gcc.h"

SOFT_TMR s_tTmr[TMR_COUNT];

#define ENABLE_INT() __set_PRIMASK(0)  /* 使能全局中断 */
#define DISABLE_INT() __set_PRIMASK(1) /* 禁止全局中断 */

void InitSoftTimer(void)
{
    uint8_t i;
    /* 清零所有的软件定时器 */
    for (i = 0; i < TMR_COUNT; i++)
    {
        s_tTmr[i].Count = 0;
        s_tTmr[i].PreLoad = 0;
        s_tTmr[i].Flag = 0;
        s_tTmr[i].Mode = TMR_ONCE_MODE;
    }
}

void StartSoftTimer(uint8_t _id, uint32_t _period)
{
    if (_id >= TMR_COUNT)
    {
        while (1)
            ; /* 参数异常，死机 */
    }
    DISABLE_INT();
    s_tTmr[_id].Count = _period;
    s_tTmr[_id].PreLoad = _period;
    s_tTmr[_id].Flag = 0;
    s_tTmr[_id].Mode = TMR_ONCE_MODE;
    ENABLE_INT();
}

void StartSoftAutoTimer(uint8_t _id, uint32_t _period)
{
    if (_id >= TMR_COUNT)
    {
        while (1)
            ; /* 参数异常，死机 */
    }
    DISABLE_INT();
    s_tTmr[_id].Count = _period;
    s_tTmr[_id].PreLoad = _period;
    s_tTmr[_id].Flag = 0;
    s_tTmr[_id].Mode = TMR_AUTO_MODE;
    ENABLE_INT();
}

void StopSoftTimer(uint8_t _id)
{
    if (_id >= TMR_COUNT)
    {
        while (1)
            ; /* 参数异常，死机 */
    }
    DISABLE_INT();
    s_tTmr[_id].Count = 0;
    s_tTmr[_id].Flag = 0;
    s_tTmr[_id].Mode = TMR_ONCE_MODE;
    ENABLE_INT();
}

uint8_t CheckSoftTimer(uint8_t _id)
{
    if (_id >= TMR_COUNT)
    {
        while (1)
            ; /* 参数异常，死机 */
    }
    if (s_tTmr[_id].Flag == 1)
    {
        s_tTmr[_id].Flag = 0;
        return 1;
    }
    return 0;
}

void SoftTimerDec(SOFT_TMR *_tmr)
{
    if (_tmr->Count > 0)
    {
        if (--_tmr->Count == 0)
        {
            _tmr->Flag = 1;
            if (_tmr->Mode == TMR_AUTO_MODE)
            {
                _tmr->Count = _tmr->PreLoad;
            }
        }
    }
}
