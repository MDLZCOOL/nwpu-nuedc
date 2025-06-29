#if !defined(__INCLUDE_CONTROL_MC_CONFIG_H__)
#define __INCLUDE_CONTROL_MC_CONFIG_H__
#include <float.h>

//! 运算的值类型
typedef float control_value_t;

//! 控制周期
#define CONTROL_T (1.0f / 12800.0f)

//! 常量值表
// 在C语言中，我们通常将这些常量定义为全局的const变量或宏定义
// 这里我们定义为全局const变量，并加上前缀以避免命名冲突
extern const control_value_t control_constant_value_ZERO;
extern const control_value_t control_constant_value_PI;
extern const control_value_t control_constant_value_HALF_PI;
extern const control_value_t control_constant_value_TAU;
extern const control_value_t control_constant_value_MAX;
extern const control_value_t control_constant_value_MIN;

#endif // __INCLUDE_CONTROL_MC_CONFIG_H__