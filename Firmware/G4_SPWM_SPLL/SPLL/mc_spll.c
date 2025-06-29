#include "mc_spll.h"
#include "mc_pid.h"     // 确保包含PID头文件
#include "mc_config.h"  // 确保包含配置头文件
#include "arm_math.h"   // ARM CMSIS DSP库，C语言兼容

// SPLL结构体初始化函数
void control_SPLL_init(struct control_SPLL *spll)
{
    // 初始化PID控制器
    control_PID_init(&spll->pid);

    spll->pid.param.kp = 750;
    spll->pid.param.ki = 15;
    spll->pid.param.kd = 0;
    spll->pid.param.kc = 10;
    // 输出限幅
    spll->pid.param.i_min = -(CONTROL_SPLL_TARGET_FREQ + 15) * control_constant_value_TAU;
    spll->pid.param.i_max = +(CONTROL_SPLL_TARGET_FREQ + 15) * control_constant_value_TAU;
    // 复位程序
    control_SPLL_reset(spll);
}

void control_SPLL_reset(struct control_SPLL *spll)
{
    spll->cur_phase = 0;
    spll->last_error = 0;
    spll->launch_loop = false;
    spll->sample_index = 0;
    spll->sogi_s1 = 0;
    spll->sogi_s2 = 0;
    spll->auto_offset_min = control_constant_value_MAX;
    spll->auto_offset_max = control_constant_value_MIN;
    control_PID_reset(&spll->pid);
}

void control_SPLL_transfer_1phase(struct control_SPLL *spll, control_value_t val)
{
    control_value_t v_org = control_SPLL_auto_offset(spll, val);
    //
    if (spll->sample_index < CONTROL_SPLL_N_SAMPLE)
    {
        spll->sample_index += 1;
        spll->launch_loop = false;
        return;
    }
    else {
        spll->launch_loop = true;
    }
    //
    // 缩放
    //
    control_value_t v = v_org / (spll->auto_offset_max - spll->auto_offset_min);
    //
    // SOGI
    //
    // SOGI: k
    const control_value_t k = 1.414f;
    // SOGI: w
    const control_value_t w = CONTROL_SPLL_TARGET_FREQ * control_constant_value_TAU;
    // 2pi -> 360°的系数
    const control_value_t angle_k = 360.0f / control_constant_value_TAU;
    //
    control_value_t sogi_u = (k * (v - spll->sogi_s1) - spll->sogi_s2) * w;
    spll->sogi_s1 += CONTROL_SPLL_TI * sogi_u;
    spll->sogi_s2 += CONTROL_SPLL_TI * w * spll->sogi_s1;
    //
    // VCO(park)
    //
    control_value_t ua = spll->sogi_s1;
    control_value_t ub = spll->sogi_s2;
    control_value_t theta = angle_k * spll->cur_phase; // 这里需要注意，theta应是弧度制给arm_sin_cos_f32
                                                       // arm_sin_cos_f32期望角度在0到360度之间，所以theta保持360度制是正确的
    control_value_t st, ct;
    arm_sin_cos_f32(theta, &st, &ct);
    control_value_t uq = ct * ub - st * ua;
    //
    // LPF (PI)
    //
    control_value_t e = 0 - uq;
    control_value_t u = control_PID_pi_transfer(&spll->pid, e);
    //
    // 鉴相器
    //
    control_value_t i = spll->cur_phase;
    i += CONTROL_SPLL_TI * u;
    if (i > control_constant_value_TAU)
    {
        i -= control_constant_value_TAU;
    }
    else if (i < -control_constant_value_TAU)
    {
        i += control_constant_value_TAU;
    }
    //
    // 输出
    //
    spll->omega = u;
    spll->cur_phase = i;
    spll->last_error = e;
    // -- loop --
}

bool control_SPLL_is_lock(const struct control_SPLL *spll, control_value_t th)
{
    return spll->launch_loop && spll->last_error < th;
}

control_value_t control_SPLL_auto_offset(struct control_SPLL *spll, control_value_t inp)
{
    if (inp > spll->auto_offset_max)
    {
        spll->auto_offset_max = inp;
    }
    else if (inp < spll->auto_offset_min)
    {
        spll->auto_offset_min = inp;
    }
    control_value_t mid = (spll->auto_offset_min + spll->auto_offset_max) * 0.5f;
    return inp - mid;
}