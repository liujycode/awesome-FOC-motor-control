/**
 * @file    svpwm.h
 * @brief   SVPWM 调制模块（扇区法七段式，浮点版）
 *
 * 适用：三相逆变器的占空比生成（FOC 电流环输出 αβ 电压 → 三相占空比）。
 * 设计约定：
 *  - 输入 αβ 电压（Clarke 变换后的定子静止坐标系），单位与 vbus 一致（V）；
 *  - 输出三相反逻辑/共逻辑均可用的占空比 0.0~1.0（高电平有效，中心对齐 PWM）；
 *  - 线性调制区上限 m = |Vref|/Vbus ≤ 1/√3 ≈ 0.577（对应线电压=母线电压）；
 *  - 超出线性区按比例收缩 t1/t2（保相角、保对称），不进入方波过调制——
 *    量产代码里"可预期的电压跌落"好过"不可预期的失真"；
 *  - 依赖 libm 的 sqrtf/atan2f/sinf；若主频紧张可换查表（注释见 .c）。
 *
 * 使用示例：
 * @code
 *      float duty[3];
 *      // 电流环输出 v_alpha / v_beta（V），母线采样 vbus（V）：
 *      svpwm_update(v_alpha, v_beta, vbus, duty);
 *      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(duty[0] * (TIM1->ARR + 1)));
 *      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)(duty[1] * (TIM1->ARR + 1)));
 *      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint32_t)(duty[2] * (TIM1->ARR + 1)));
 * @endcode
 */
#ifndef SVPWM_H
#define SVPWM_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief 母线电压低于此值（V）视为异常，输出安全态 50/50/50 */
#define SVPWM_VBUS_MIN   2.0f

/**
 * @brief  SVPWM 单步调制
 * @param  alpha     α 轴电压（V）
 * @param  beta      β 轴电压（V）
 * @param  vbus      母线电压（V），须 > SVPWM_VBUS_MIN
 * @param  out_duty  输出数组 [3]：A/B/C 相占空比 0.0~1.0
 * @note   参数异常时输出三相 0.5（上下管对称，无有效线电压的安全态）
 */
void svpwm_update(float alpha, float beta, float vbus, float out_duty[3]);

#ifdef __cplusplus
}
#endif

#endif /* SVPWM_H */
