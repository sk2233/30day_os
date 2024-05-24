#ifndef INIT_H
#define INIT_H

#include "fifo.h"

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021

#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

#define PIC_ICW_ECHO (1<<5)

extern fifo_t kdb_fifo;
extern fifo_t mouse_fifo;

void init_pic();

void int_handler_kbd();
void int_handler_mouse();
void int_handler_noise();
void int_handler_pit();

#endif