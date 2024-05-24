#include "int.h"
#include "comm/cpu_instr.h"
#include "dev.h"
#include "fifo.h"
#include "timer.h"

fifo_t kdb_fifo;
fifo_t mouse_fifo;

/* PIC初始化 */
void init_pic(){
    // 设置pic0
    outb(PIC0_ICW1, 0x11  ); /* 边缘触发模式（edge trigger mode） */
    outb(PIC0_ICW2, 0x20  ); /* IRQ 0~7由INT 20~27接收 其他中断已被占用*/
    outb(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2相连 */
    outb(PIC0_ICW4, 0x01  ); /* 无缓冲区模式 */
    // 设置pic1
    outb(PIC1_ICW1, 0x11  ); /* 边缘触发模式（edge trigger mode） */
    outb(PIC1_ICW2, 0x28  ); /* IRQ 8~15由INT 28~2f接收 */
    outb(PIC1_ICW3, 2     ); /* PIC1由IRQ2连接 */
    outb(PIC1_ICW4, 0x01  ); /* 无缓冲区模式 */
    // 设置屏蔽那些中断,放开部分中断
    outb(PIC0_IMR, 0xf8); /* PIT和PIC1和键盘设置为许可(11111000) */
    outb(PIC1_IMR, 0xef); /* 开放鼠标中断(11101111) */
}

/* 来自PS/2键盘的中断 中断号21 */
void handler_kbd(uint32_t *esp){
    // 通知处理完毕了
    outb(PIC0_OCW2, PIC_ICW_ECHO);
    if(fifo_can_write(&kdb_fifo)){
        uint8_t data= inb(PORT_KBD_DAT);
        fifo_write(&kdb_fifo,data);
    }
}

/* 来自计数器的中断 中断号20 */
void handler_pit(uint32_t *esp){
    // 通知处理完毕了
    outb(PIC0_OCW2, PIC_ICW_ECHO);
    tick_timer();
}

/* 来自PS/2鼠标的中断,中断号2c */
void handler_mouse(uint32_t *esp){
    outb(PIC1_OCW2, PIC_ICW_ECHO);	/* 通知PIC IRQ-12 已经受理完毕 */
    outb(PIC0_OCW2, PIC_ICW_ECHO);	/* 通知PIC IRQ-02 已经受理完毕 */
    if(fifo_can_write(&mouse_fifo)){
        uint8_t data = inb(PORT_KBD_DAT);
        fifo_write(&mouse_fifo,data);
    }
}

/* PIC0中断的不完整策略  中断号27 */
/* 这个中断在Athlon64X2上通过芯片组提供的便利，只需执行一次 */
/* 这个中断只是接收，不执行任何操作 */
/* 为什么不处理？
	→  因为这个中断可能是电气噪声引发的、只是处理一些重要的情况。*/
void handler_noise(uint32_t *esp){
    // 直接标记处理完毕
    outb(PIC0_OCW2, PIC_ICW_ECHO);
}