/**
 * @file    pi_controller.c
 * @brief   通用 PI 控制器实现（抗积分饱和：clamping 法）
 */
#include "pi_controller.h"

bool pi_init(pi_t *pi, float kp, float ki, float out_min, float out_max)
{
    if (pi == NULL || out_min >= out_max) {
        return false;
    }
    pi->kp       = kp;
    pi->ki       = ki;
    pi->out_min  = out_min;
    pi->out_max  = out_max;
    pi->integral = 0.0f;
    return true;
}

float pi_update(pi_t *pi, float ref, float fb, float dt)
{
    if (pi == NULL) {
        return 0.0f;
    }

    const float err = ref - fb;
    const float p_term = pi->kp * err;

    /* 积分候选值：先算出来，判断是否会被饱和卡住 */
    if (dt > 0.0f) {
        const float integ_new = pi->integral + err * dt;
        const float unsat     = p_term + pi->ki * integ_new;

        /* clamping 抗饱和：
         * 输出已顶到上限且误差仍为正（继续往上顶）→ 冻结积分；
         * 输出已顶到下限且误差仍为负 → 同理冻结。
         * 误差把输出往限幅内拉的时候，积分照常累加（退饱和顺畅）。 */
        const bool sat_high = (unsat > pi->out_max) && (err > 0.0f);
        const bool sat_low  = (unsat < pi->out_min) && (err < 0.0f);

        if (!sat_high && !sat_low) {
            pi->integral = integ_new;
        }
    }

    float out = p_term + pi->ki * pi->integral;

    if (out > pi->out_max) out = pi->out_max;
    if (out < pi->out_min) out = pi->out_min;
    return out;
}

void pi_reset(pi_t *pi)
{
    if (pi != NULL) {
        pi->integral = 0.0f;
    }
}
