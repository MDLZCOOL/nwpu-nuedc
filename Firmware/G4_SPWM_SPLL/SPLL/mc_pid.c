#include "mc_pid.h"
#include "mc_config.h" // 确保包含mc_config.h以使用control_value_t

// PID结构体初始化函数
void control_PID_init(struct control_PID *pid)
{
    // 初始化参数为0，虽然在C++默认构造函数中可以这样写，但C中需要显式初始化
    // 如果这些参数在外部使用时会立即被设置，那么这里可以不做额外操作
    // 否则，建议在这里提供默认值或清零
    pid->param.kp = 0;
    pid->param.ki = 0;
    pid->param.kd = 0;
    pid->param.kg = 0;
    pid->param.kc = 0;
    pid->param.i_min = 0;
    pid->param.i_max = 0;

    // 调用reset函数重置内部状态
    control_PID_reset(pid);
}

void control_PID_reset(struct control_PID *pid)
{
    pid->i_sum = 0;
    pid->sat_err = 0;
}

control_value_t control_PID_p_transfer(struct control_PID *pid, control_value_t e)
{
    control_value_t m = pid->param.kp * e;
    control_value_t out
        = m > pid->param.i_max ? pid->param.i_max
        : m < pid->param.i_min ? pid->param.i_min
        : m;
    return out;
}

control_value_t control_PID_pi_transfer(struct control_PID *pid, control_value_t e)
{
    control_value_t sat = pid->param.kp * e + pid->i_sum;
    // PI输出
    // U(s) = kp * E(s) + ki * E(s) / s
    control_value_t out
        = sat > pid->param.i_max ? pid->param.i_max
        : sat < pid->param.i_min ? pid->param.i_min
        : sat;
    // 饱和误差
    pid->sat_err = out - sat; // 注意：这里将局部变量sat_err改为结构体成员
    // 累积积分, 加上饱和误差
    pid->i_sum += pid->param.ki * e + pid->param.kc * pid->sat_err;
    // 积分限幅, clamp
    if (pid->i_sum > pid->param.i_max)
    {
        pid->i_sum = pid->param.i_max;
    }
    else if (pid->i_sum < pid->param.i_min)
    {
        pid->i_sum = pid->param.i_min;
    }
    //
    return out;
}

control_value_t control_PID_pd_transfer(struct control_PID *pid, control_value_t e)
{
    // 原始C++代码这里返回0，保持一致
    return 0;
}

control_value_t control_PID_pid_transfer(struct control_PID *pid, control_value_t e)
{
    // 原始C++代码这里返回0，保持一致
    return 0;
}