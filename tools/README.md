# tools/ 嵌入式 C 代码模板

量产级嵌入式 C 模块，直接开源、直接抄用。每个模块带 Doxygen 注释、边界检查和使用示例，风格按量产固件的要求来写（无动态内存、无递归、ISR 友好）。

## 已开源

| 模块 | 文件 | 一句话 |
|---|---|---|
| **PI 控制器** | [pi_controller.c](pi_controller.c) / [.h](pi_controller.h) | clamping 抗积分饱和 + 输出限幅，电流环/速度环通用，ISR 安全 |
| **环形缓冲区** | [ring_buffer.c](ring_buffer.c) / [.h](ring_buffer.h) | 2 的幂容量、单生产者/单消费者无锁，ISR ↔ 主循环安全共用 |
| **SVPWM** | [svpwm.c](svpwm.c) / [.h](svpwm.h) | 扇区法七段式，线性区自动限幅，过调制按比例收缩保相角 |

## 计划中

- RS485 收发切换（含方向控制时序）
- 一阶低通滤波（带 Nyquist 频率检查）
- CRC 校验（含 Modbus 变体）

想优先看到哪个，欢迎提 issue。
