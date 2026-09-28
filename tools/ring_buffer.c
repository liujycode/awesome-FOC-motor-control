/**
 * @file    ring_buffer.c
 * @brief   环形缓冲区实现（单生产者/单消费者无锁）
 */
#include "ring_buffer.h"

bool rb_init(ringbuf_t *rb, uint8_t *mem, uint32_t size)
{
    if (rb == NULL || mem == NULL || size < 2u) {
        return false;
    }
    /* 容量必须为 2 的幂：size & (size-1) 应为 0 */
    if ((size & (size - 1u)) != 0u) {
        return false;
    }
    rb->head = 0u;
    rb->tail = 0u;
    rb->mask = size - 1u;
    rb->mem  = mem;
    return true;
}

bool rb_push(ringbuf_t *rb, uint8_t byte)
{
    if (rb == NULL) {
        return false;
    }
    const uint32_t next = (rb->head + 1u) & rb->mask;
    if (next == rb->tail) {         /* 满判据：head+1 追上 tail */
        return false;               /* 丢弃新数据，不覆盖未读数据 */
    }
    rb->mem[rb->head] = byte;
    rb->head = next;                /* 最后才发布 head，读方见到即数据有效 */
    return true;
}

bool rb_pop(ringbuf_t *rb, uint8_t *byte)
{
    if (rb == NULL || byte == NULL) {
        return false;
    }
    if (rb->head == rb->tail) {     /* 空判据 */
        return false;
    }
    *byte     = rb->mem[rb->tail];
    rb->tail = (rb->tail + 1u) & rb->mask;
    return true;
}

void rb_flush(ringbuf_t *rb)
{
    if (rb != NULL) {
        rb->head = 0u;
        rb->tail = 0u;
    }
}
