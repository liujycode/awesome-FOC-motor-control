/**
 * @file    svpwm.c
 * @brief   SVPWM 实现（扇区法七段式 + 比例式过调制限制）
 */
#include "svpwm.h"
#include <math.h>

void svpwm_update(float alpha, float beta, float vbus, float out_duty[3])
{
    if ((out_duty == NULL) || (vbus < SVPWM_VBUS_MIN)) {
        if (out_duty != NULL) {
            out_duty[0] = 0.5f;
            out_duty[1] = 0.5f;
            out_duty[2] = 0.5f;
        }
        return;
    }

    static const float SQRT3 = 1.7320508f;
    static const float PI_3  = 1.0471976f;   /* π/3 = 60° */
    static const float PI_2  = 6.2831853f;   /* 2π */

    /* 调制比 m = |Vref| / Vbus（相电压幅值口径），线性区 m ≤ 1/√3 */
    const float mag   = sqrtf(alpha * alpha + beta * beta) / vbus;
    float       theta = atan2f(beta, alpha);
    if (theta < 0.0f) {
        theta += PI_2;
    }

    int sector = (int)(theta / PI_3);        /* 0..5，浮点边缘可能到 6 */
    if (sector > 5) {
        sector = 5;
    }
    const float theta_s = theta - (float)sector * PI_3;   /* 扇区内角度 0..60° */

    /* 主/辅矢量作用时间（归一化到 PWM 周期）：t = √3·m·sin(...) */
    float t1 = SQRT3 * mag * sinf(PI_3 - theta_s);        /* 主矢量（靠近 θ） */
    float t2 = SQRT3 * mag * sinf(theta_s);               /* 辅矢量 */

    /* 过调制（t1+t2 > 1）：等比例收缩，保相角；等效于限幅在最大线性调制 */
    float sum = t1 + t2;
    if (sum > 1.0f) {
        const float scale = 1.0f / sum;
        t1  *= scale;
        t2  *= scale;
        sum  = 1.0f;
    }

    /* 零矢量均分（七段式）：t0 取一半分到首尾 */
    const float t0 = (1.0f - sum) * 0.5f;

    /* 各扇区三相占空比（中心对齐，高电平有效） */
    float ta, tb, tc;
    switch (sector) {
    case 0: ta = t0 + t1 + t2; tb = t0 + t2;       tc = t0;           break; /* V1(100) V2(110) */
    case 1: ta = t0 + t1;      tb = t0 + t1 + t2;  tc = t0;           break; /* V2(110) V3(010) */
    case 2: ta = t0;           tb = t0 + t1 + t2;  tc = t0 + t2;       break; /* V3(010) V4(011) */
    case 3: ta = t0;           tb = t0 + t1;       tc = t0 + t1 + t2;  break; /* V4(011) V5(001) */
    case 4: ta = t0 + t2;      tb = t0;            tc = t0 + t1 + t2;  break; /* V5(001) V6(101) */
    default:ta = t0 + t1 + t2; tb = t0;            tc = t0 + t1;       break; /* V6(101) V1(100) */
    }

    out_duty[0] = ta;
    out_duty[1] = tb;
    out_duty[2] = tc;
}

/* @note 性能提示：sqrtf/atan2f/sinf 在 M4/M7 上无 FPU 加速（除 sqrtf）。
 *       高频电流环（>20kHz）建议：
 *       1) atan2f+sinf 换 CORDIC 或 64 点查表（6 扇区 sin 表仅需 3 个值/扇区）；
 *       2) mag 用 mag² 比较线性区上限（(1/√3)² = 1/3），免开方。 */
