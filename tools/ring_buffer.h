/**
 * @file    ring_buffer.h
 * @brief   环形缓冲区（单生产者 / 单消费者无锁设计）
 *
 * 适用：串口 RX 缓存、ADC 数据流缓冲、日志缓冲等"一端写一端读"场景。
 * 设计约定：
 *  - 容量必须为 2 的幂（用掩码取模，省掉除法）；
 *  - head 仅写方修改，tail 仅读方修改，均用 volatile，
 *    单生产者+单消费者下无锁安全，可在 ISR(写) 与主循环(读) 间共用；
 *  - 存储区由调用方提供（数组或静态区），本模块零动态内存；
 *  - 满时丢弃新数据并返回 false——宁可丢新数据也不覆盖未读旧数据，
 *    调试类场景（波形/日志）不允许静默错位。
 *
 * 使用示例：
 * @code
 *      static ringbuf_t    g_rx_rb;
 *      static uint8_t      g_rx_mem[256];          // 2 的幂
 *
 *      rb_init(&g_rx_rb, g_rx_mem, sizeof(g_rx_mem));
 *
 *      // UART ISR（生产者）：
 *      rb_push(&g_rx_rb, (uint8_t)USART1->DR);
 *
 *      // 主循环（消费者）：
 *      uint8_t byte;
 *      while (rb_pop(&g_rx_rb, &byte)) { parse(byte); }
 * @endcode
 */
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief 环形缓冲区控制块 */
typedef struct {
    volatile uint32_t head;  /**< 写指针（仅生产者修改） */
    volatile uint32_t tail;  /**< 读指针（仅消费者修改） */
    uint32_t          mask;  /**< 容量掩码：size-1 */
    uint8_t          *mem;   /**< 存储区（调用方提供） */
} ringbuf_t;

/**
 * @brief  初始化环形缓冲区
 * @param  rb      控制块
 * @param  mem     存储区首地址（生命周期须覆盖整个使用期）
 * @param  size    存储区字节数，必须为 2 的幂
 * @return true=成功 / false=参数非法（含 size 非 2 的幂）
 */
bool rb_init(ringbuf_t *rb, uint8_t *mem, uint32_t size);

/** @brief 已缓存字节数（读方/写方均可安全调用） */
static inline uint32_t rb_count(const ringbuf_t *rb)
{
    return (rb->head - rb->tail) & rb->mask;
}

/** @brief 是否为空 */
static inline bool rb_empty(const ringbuf_t *rb)
{
    return rb->head == rb->tail;
}

/**
 * @brief  压入 1 字节（生产者调用）
 * @return true=成功 / false=缓冲已满（数据被丢弃）
 * @note   可在 ISR 中调用
 */
bool rb_push(ringbuf_t *rb, uint8_t byte);

/**
 * @brief  弹出 1 字节（消费者调用）
 * @return true=取到数据 / false=缓冲为空
 */
bool rb_pop(ringbuf_t *rb, uint8_t *byte);

/** @brief 清空缓冲（读写双方同时空闲时才可调用，如初始化/复位阶段） */
void rb_flush(ringbuf_t *rb);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_H */
