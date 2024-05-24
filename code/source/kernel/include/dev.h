#ifndef DEV_H
#define DEV_H

#include "comm/types.h"

#define PORT_KBD_DAT				0x0060
#define PORT_KBD_STA				0x0064
#define PORT_KBD_CMD				0x0064
#define KBD_NOT_READY	            0x02
#define KBD_WRITE_MODE		        0x60
#define KBC_MODE				    0x47
#define KBD_TO_MOUSE		0xd4
#define MOUSE_ENABLE			0xf4

#define PIT_CTRL	0x43
#define PIT_CNT0	0x40

#define MOUSE_BTN_L (1<<0)
#define MOUSE_BTN_M (1<<1)
#define MOUSE_BTN_R (1<<2)

typedef struct mouse{
    int8_t state;
    uint8_t buff[3];
    // btn只有低3位有效 L M R 按钮是否按下
    uint8_t btn;
    int offX,offY;
    int x,y;
}mouse_t;

extern mouse_t mouse;

void init_kbd();
void init_mouse();
void init_pit();
bool_t mouse_decode(uint8_t data);
// 20000 ~ 20000000 都是人类可以听到的声音
void beep(uint32_t hz);

#endif