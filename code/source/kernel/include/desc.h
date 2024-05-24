#ifndef DESC_H
#define DESC_H

#include "comm/types.h"

#define ATTR_INT_GATE	0x008e
#define ATTR_TSS	0x0089

#define GDT_COUNT 1024
#define IDT_COUNT 64

#pragma pack(1)
// cpu因为设计为兼容历史逻辑，所以很多数字很分散
typedef struct seg_desc {
    // base 32bit  limit 20bit (limit_high能使用的只有低4位) 通过标志位中的粒度标记标明单位是byte还是4kb 其他的让给控制位
    uint16_t limit_low, base_low;
    uint8_t base_mid, access_right;
    uint8_t limit_high, base_high;
}seg_desc_t;

typedef struct gate_desc {
    uint16_t offset_low, selector;
    uint8_t dw_count, access_right;
    uint16_t offset_high;
}gate_desc_t;
#pragma pack()

void set_seg_desc(seg_desc_t *seg, uint32_t limit, uint32_t base, uint16_t attr);
void create_seg_desc(uint16_t index, uint32_t limit, uint32_t base, uint16_t attr);
void set_gate_desc(gate_desc_t *gate, uint32_t offset, uint16_t selector, uint16_t attr);
void init_gdt_idt();

#endif