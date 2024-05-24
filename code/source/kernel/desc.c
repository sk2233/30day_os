#include "desc.h"
#include "comm/cpu_instr.h"
#include "int.h"
#include "mem.h"

static seg_desc_t *gdt;
static gate_desc_t *idt;

void init_gdt_idt(){
    gdt= mem_alloc(sizeof(seg_desc_t)*GDT_COUNT);
    // 第一个必须是空表项目 gdt初始化
    set_seg_desc(gdt, 0, 0, 0);// 若需要添加段注意总数量 注意这里是先代码段再数据段
    set_seg_desc(gdt + 1, 0xffffffff, 0x00000000, 0x409a);// 仅可执行的段 代码段
    set_seg_desc(gdt + 2, 0xffffffff, 0x00000000, 0x4092);// 读写段 数据段
    lgdt((uint32_t)gdt, sizeof(seg_desc_t)*GDT_COUNT);
    idt= mem_alloc(sizeof(gate_desc_t)*IDT_COUNT);
    /* IDT初始化 */
    for (int i = 0; i < 64; ++i) {
        set_gate_desc(idt + i, 0, 0, 0);
    }
    set_gate_desc(idt + 0x21, (uint32_t )int_handler_kbd, 1 * 8, ATTR_INT_GATE);
    set_gate_desc(idt + 0x2c, (uint32_t )int_handler_mouse, 1 * 8, ATTR_INT_GATE);
    set_gate_desc(idt + 0x20, (uint32_t )int_handler_pit, 1 * 8, ATTR_INT_GATE);
    set_gate_desc(idt + 0x27, (uint32_t )int_handler_noise, 1 * 8, ATTR_INT_GATE);
    lidt((uint32_t)idt, sizeof(gate_desc_t)*IDT_COUNT);
}

void create_seg_desc(uint16_t index, uint32_t limit, uint32_t base, uint16_t attr){
    set_seg_desc(gdt + index, limit, base, attr);
}

void set_seg_desc(seg_desc_t *seg, uint32_t limit, uint32_t base, uint16_t attr){
    if (limit > 0xfffff) {
        attr |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }
    seg->limit_low    = limit & 0xffff;
    seg->base_low     = base & 0xffff;
    seg->base_mid     = (base >> 16) & 0xff;
    seg->access_right = attr & 0xff;
    seg->limit_high   = ((limit >> 16) & 0x0f) | ((attr >> 8) & 0xf0);
    seg->base_high    = (base >> 24) & 0xff;
}

void set_gate_desc(gate_desc_t *gate, uint32_t offset, uint16_t selector, uint16_t attr){
    gate->offset_low   = offset & 0xffff;
    gate->selector     = selector;
    gate->dw_count     = (attr >> 8) & 0xff;
    gate->access_right = attr & 0xff;
    gate->offset_high  = (offset >> 16) & 0xffff;
}