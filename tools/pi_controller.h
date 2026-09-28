/**
 * @file    pi_controller.h
 * @brief   通用 PI 控制器（浮点版）：抗积分饱和 + 输出限幅
 *
 * 适用：FOC 电流环 / 速度环内环等需要抗饱和的 PI 场景。
 * 设计约定：
 *  - 无动态内存、无递归，可直接在 ISR 中调用（pi_update 为纯计算）；
 *  - 抗饱和采用 clamping 法：输出已限幅且误差继续同向时冻结积分，
 *    实现简单、参数直观，量产中最常用；
 *  - 结构体由调用方分配，本模块零依赖（仅需 stdint/stdbool）。
 *
 * 使用示例：
 * @code
 *      pi_t pi_iq;
 *      pi_init(&pi_iq, 0.35f, 0.02f, -8.0f, 8.0f);   // kp, ki, 输出限幅 ±8V
 *
 *      // 电流环中断里（Ts = 控制周期，秒）：
 *      float vq = pi_update(&pi_iq, iq_ref, iq_fb, TS);
 * @endcode
 */
#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief PI 控制器运行时状态 */
typedef struct {
    float kp;            /**< 比例增益 */
    float ki;            /**< 积分增益（输出 = kp*e + ki*∫e） */
    float out_min;       /**< 输出下限 */
    float out_max;       /**< 输出上限 */
    float integral;      /**< 积分累加值 ∫e·dt */
} pi_t;

/**
 * @brief  初始化 PI 控制器
 * @param  pi       控制器实例（调用方分配）
 * @param  kp       比例增益
 * @param  ki       积分增益
 * @param  out_min  输出下限（如电压/占空比下限）
 * @param  out_max  输出上限
 * @note   out_min 必须小于 out_max，否则断言失败后返回 false
 * @return true=成功 / false=参数非法
 */
bool pi_init(pi_t *pi, float kp, float ki, float out_min, float out_max);

/**
 * @brief  单步 PI 运算（在每个控制周期调用一次）
 * @param  pi   控制器实例
 * @param  ref  参考值（给定）
 * @param  fb   反馈值（测量）
 * @param  dt   本次与上次调用的时间间隔（秒），必须 > 0
 * @return 限幅后的控制器输出
 * @note   dt 异常（<= 0）时跳过积分项，仅返回比例项，避免脏时间戳污染积分
 */
float pi_update(pi_t *pi, float ref, float fb, float dt);

/** @brief 复位积分项（启停机、切环时调用，防止旧积分顶输出） */
void pi_reset(pi_t *pi);

#ifdef __cplusplus
}
#endif

#endif /* PI_CONTROLLER_H */
