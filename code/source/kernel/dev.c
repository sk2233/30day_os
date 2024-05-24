#include "dev.h"
#include "comm/cpu_instr.h"
#include "comm/types.h"
#include "comm/config.h"
#include "comm/math.h"

mouse_t mouse;

/* 等待键盘控制电路准备完毕 */
void wait_kbd_ready(){
    for (;;) {
        if (!(inb(PORT_KBD_STA) & KBD_NOT_READY)) {
            break;
        }
    }
}

/* 初始化键盘控制电路 */
void init_kbd() {
    wait_kbd_ready();
    outb(PORT_KBD_CMD, KBD_WRITE_MODE);
    wait_kbd_ready();
    outb(PORT_KBD_DAT, KBC_MODE);
}

/* 激活鼠标 */
void init_mouse(){
    wait_kbd_ready();
    outb(PORT_KBD_CMD, KBD_TO_MOUSE);
    wait_kbd_ready();
    // 准备好后会立即触发一个中断  0xfa
    outb(PORT_KBD_DAT, MOUSE_ENABLE);
    mouse.state=-1;// 默认设置为没有准备完毕
    mouse.x= SCREEN_W / 2;
    mouse.y= SCREEN_H / 2;
}

bool_t mouse_decode(uint8_t data){
    if(mouse.state==-1){// 没有激活等待激活
        if(data==0xfa){
            mouse.state=0;
        }
    } else{// 激活了收集数据 3个一组
        if(mouse.state==0&&(data&0xc8)!=0x08){
            return FALSE; // 第一个字节主要记录键位信息 需要保证其合法性  鼠标信息可能断连导致数据错位使用这个校验
        }
        mouse.buff[mouse.state++]=data;
        if(mouse.state>=3){// 开始解析数据
            mouse.state=0;
            mouse.btn = mouse.buff[0] & 0x07; // 只放在了低3位
            mouse.offX = mouse.buff[1];
            mouse.offY = mouse.buff[2];
            // 处理负数
            if ((mouse.buff[0] & 0x10) != 0) {
                mouse.offX |= 0xffffff00;
            }
            if ((mouse.buff[0] & 0x20) != 0) {
                mouse.offY |= 0xffffff00;
            }
            /* 鼠标的y方向与画面符号相反 */
            mouse.offY = - mouse.offY;
            mouse.x= clump(mouse.x+mouse.offX, SCREEN_W, 0);
            mouse.y=clump(mouse.y+mouse.offY, SCREEN_H, 0);
            return TRUE;
        }
    }
    return FALSE;
}

void init_pit(){
    // 先准备设置中断间隔，然后依次写入中断间隔
    outb(PIT_CTRL,0x34);
    // 设置为 1秒100次中断
    outb(PIT_CNT0,0x9c);
    outb(PIT_CNT0,0x2e);
}

// qemu 没有蜂鸣器
void beep(uint32_t hz){
    if(hz<20000||hz>20000000){
        // 关闭设备
       uint8_t sign= inb(0x61);
        outb(0x61,sign&0x0d);
    } else{
        hz=1193180000/hz;
        outb(0x43,0xb6); // 开始设置音高
        // 先设置低8位再设置高8位
        outb(0x42,hz&0xff);
        outb(0x42,(hz>>8)&0xff);
        // 打开设备
        uint8_t sign= inb(0x61);
        outb(0x61,(sign|0x03)&0x0f);
    }
}