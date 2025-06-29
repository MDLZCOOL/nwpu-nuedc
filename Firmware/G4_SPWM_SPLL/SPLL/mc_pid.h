#if !defined(__INCLUDE_CONTROL_MC_PID_H__)
#define __INCLUDE_CONTROL_MC_PID_H__
#include "mc_config.h" // 包含配置头文件

/**
 * @brief PID控制器参数结构体
 *
 */
struct control_PID_Factors
{
    //! 比例系数
    control_value_t kp;

    //! 积分系数
    control_value_t ki;

    //! 微分系数
    control_value_t kd;

    //! 微分增益
    control_value_t kg;

    //! 饱和系数
    control_value_t kc;

    //! 积分器限幅(min)
    control_value_t i_min;

    //! 积分器限幅(max)
    control_value_t i_max;
};

/**
 * @brief 带退饱和反馈和积分限幅的PID控制器(float32)结构体
 *
 */
struct control_PID
{
    //! 控制器参数
    struct control_PID_Factors param;

    //! 饱和误差
    control_value_t sat_err;

    //! 积分值
    control_value_t i_sum;
};

/**
 * @brief PID结构体初始化函数
 *
 * @param [out] pid: 指向PID结构体的指针
 */
void control_PID_init(struct control_PID *pid);

/**
 * @brief 重置调节器状态
 *
 * @param [out] pid: 指向PID结构体的指针
 */
void control_PID_reset(struct control_PID *pid);

/**
 * @brief 数字P控制器计算
 *
 * @param [in] pid: 指向PID结构体的指针
 * @param [in] e: 输入, error
 *
 * @return control_value_t: 输出
 *
 */
control_value_t control_PID_p_transfer(struct control_PID *pid, control_value_t e);

/**
 * @brief 数字PI控制器计算
 *
 * @param [in] pid: 指向PID结构体的指针
 * @param [in] e: 输入, error
 *
 * @return control_value_t: 输出
 *
 */
control_value_t control_PID_pi_transfer(struct control_PID *pid, control_value_t e);

/**
 * @brief 数字PD控制器计算
 *
 * @param [in] pid: 指向PID结构体的指针
 * @param [in] e: 输入, error
 *
 * @return control_value_t: 输出
 *
 */
control_value_t control_PID_pd_transfer(struct control_PID *pid, control_value_t e);

/**
 * @brief 数字PID控制器计算
 *
 * @param [in] pid: 指向PID结构体的指针
 * @param [in] e: 输入, error
 *
 * @return control_value_t: 输出
 *
 */
control_value_t control_PID_pid_transfer(struct control_PID *pid, control_value_t e);

#endif // __INCLUDE_CONTROL_MC_PID_H__