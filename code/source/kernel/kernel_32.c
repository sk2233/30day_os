#include "comm/cpu_instr.h"
#include "graphic.h"
#include "desc.h"
#include "int.h"
#include "dev.h"
#include "fifo.h"
#include "tool.h"
#include "mem.h"
#include "timer.h"
#include "frame.h"
#include "task.h"
#include "file.h"
#include "app/console.h"

void test_task() {
    uint32_t count = 0;
    while (TRUE) {
        count++;
        char buff[16];
        mem_set(buff, 0, 16);
        fmt(buff, "TEST %d", count);
        fill_rect(window_frame->vram, window_frame->w, 0, 16, 16 * 8, 16, COL_000000);
        draw_str(window_frame->vram, window_frame->w, 0, 16, buff, COL_FFFFFF);
        frame_refresh_sub(window_frame->x, window_frame->y + 16, 16 * 8, 16);
        task_switch();
    }
}

// move_frame一定是active_frame的，但是active_frame不一定是move_frame的
static frame_t *active_frame; // 可以在切换时添加回掉用于修改部分属性，例如进程优先级(很多不激活的窗口不分配时间片都行)，是否激活标识等东西
static frame_t *move_frame;
static int offsetX;
static int offsetY;

/**
 * 32位main
 */
void kernel_main32() {
    init_mem();
    init_file();
    init_gdt_idt();
    init_pic();
    init_task();
    sti(); //打开中断
    fifo_init(&kdb_fifo, 128);
    fifo_init(&mouse_fifo, 128);
    fifo_init(&timer_fifo, 128);
    init_kbd();
    init_mouse();
    init_pit();
    init_timer();

    init_graphic();
    init_frame();

//    uint32_t count =0;
    while (TRUE) {
        // task_sleep(timer+300);
        cli();
        if (fifo_can_read(&mouse_fifo)) {
            uint8_t data = fifo_read(&mouse_fifo);
            sti();
            if (mouse_decode(data)) {
                frame_move(cursor_frame, mouse.x, mouse.y);
                // 没有数据时才尝试移动窗口，减少无谓的移动
                if(move_frame&&!fifo_can_read(&mouse_fifo)){
                    frame_move(move_frame,mouse.x+offsetX,mouse.y+offsetY);
                }
                if (mouse.btn & MOUSE_BTN_L) {
                    frame_t *frame=click_frame(mouse.x,mouse.y);
                    if(!frame){// 取消所有窗口
                        active_frame=0;
                        move_frame=0;
                        // 点击到开始按钮创建命令行窗口
                        if(mouse.x>=2&&mouse.x<=60&&mouse.y>=SCREEN_H-24&&mouse.y<=SCREEN_H-4){
                            init_console();
                        }
                        continue;
                    }
                    if(frame!=active_frame){// 激活窗口
                        active_frame=frame;
                        frame_top(active_frame);
                        frame_refresh_sub(active_frame->x,active_frame->y,active_frame->w,active_frame->h);
                        continue;
                    }
                    if(frame->click&&frame->click(mouse.x,mouse.y)){// 事件被内部处理了
                        if(!active_frame->used){// 可能触发的是关闭事件
                            active_frame=0;
                        }
                        continue;
                    }
                    if(move_frame){// 移动状态切换
                        move_frame=0;
                    } else{
                        move_frame= frame;
                        offsetX=move_frame->x-mouse.x;
                        offsetY=move_frame->y-mouse.y;
                    }
                }
            }
        } else if(fifo_can_read(&kdb_fifo)){
            // 还有直接分发，否则丢失按钮事件
            uint8_t data= fifo_read(&kdb_fifo);
            sti();
            if(active_frame&& fifo_can_write(&active_frame->kdb_fifo)){
                fifo_write(&active_frame->kdb_fifo,data);
            }
        } else {
            sti();
            hlt();
        }
    }
}
