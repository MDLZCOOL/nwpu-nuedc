#if !defined(__INCLUDE_CONTROL_MC_SPLL_H__)
#define __INCLUDE_CONTROL_MC_SPLL_H__
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h> // for bool type
#include "mc_pid.h"
#include "mc_config.h"

// SPLL工作周期
#define CONTROL_SPLL_TI CONTROL_T

// SPLL期望频率(Hz)
#define CONTROL_SPLL_TARGET_FREQ 50.0f

// 计算1个周期有多少个点
#define CONTROL_SPLL_N_SAMPLE ((size_t)((1.0f / CONTROL_SPLL_TI) / CONTROL_SPLL_TARGET_FREQ))

/**
 * @brief 数字锁相环结构体
 *
 */
struct control_SPLL
{
    //! PI控制器
    struct control_PID pid;

    //! 是否启动环路
    bool launch_loop;

    //! 采样点索引, 最大到N_SAMPLE / 4
    uint16_t sample_index;

    //! 角速度
    control_value_t omega;

    //! 角度积分值, 作为相位wt
    control_value_t cur_phase;

    //! 自动归零: 最大值和最小值
    control_value_t auto_offset_min;
    control_value_t auto_offset_max;

    //! SOGI的积分值
    control_value_t sogi_s1;
    control_value_t sogi_s2;

    //! 上次的环路error
    control_value_t last_error;
};

/**
 * @brief SPLL结构体初始化函数
 *
 * @param [out] spll: 指向SPLL结构体的指针
 */
void control_SPLL_init(struct control_SPLL *spll);

/**
 * @brief 复位SPLL状态
 *
 * @param [out] spll: 指向SPLL结构体的指针
 */
void control_SPLL_reset(struct control_SPLL *spll);

/**
 * @brief 进行一次计算
 *
 * @note  按照Ti等间隔调用本方法, 实现PLL
 * @param [out] spll: 指向SPLL结构体的指针
 * @param [in] val: 输入值
 */
void control_SPLL_transfer_1phase(struct control_SPLL *spll, control_value_t val);

/**
 * @brief 判断PLL是否已经锁定
 *
 * @param [in] spll: 指向SPLL结构体的常指针
 * @param [in] th: 比较阈值
 * @return true: 已锁定
 * @return false: 未锁定
 */
bool control_SPLL_is_lock(const struct control_SPLL *spll, control_value_t th);

/**
 * @brief 取得当前信号的频率
 *
 * @param [in] spll: 指向SPLL结构体的常指针
 * @return control_value_t: 频率
 *
 */
inline control_value_t control_SPLL_freq(const struct control_SPLL *spll)
{
    return spll->omega / control_constant_value_TAU;
}

/**
 * @brief 取得当前信号的相位
 *
 * @param [in] spll: 指向SPLL结构体的常指针
 * @return control_value_t: 相位
 *
 */
inline control_value_t control_SPLL_phase(const struct control_SPLL *spll)
{
    return spll->cur_phase;
}

/**
 * @brief 按照历史最大值和最小值进行归中
 * @param [out] spll: 指向SPLL结构体的指针
 * @param [in] inp: 输入
 *
 * @return control_value_t: 归中后的值
 *
 */
control_value_t control_SPLL_auto_offset(struct control_SPLL *spll, control_value_t inp);

#endif // __INCLUDE_CONTROL_MC_SPLL_H__