#include "app/nano.h"
#include "task.h"
#include "frame.h"
#include "comm/config.h"
#include "graphic.h"
#include "tool.h"
#include "file.h"
#include "mem.h"
#include "comm/cpu_instr.h"
#include "comm/math.h"

static frame_t *frame;
static task_t *task;
static int offset;
static bool_t has_more;
static char name[16]; // 用于保存文件名称

bool_t nano_click(int x,int y){// 暂时只判断关闭事件
    if(window_close(frame, x, y)){
        frame_free(frame);
        frame_refresh_sub(frame->x, frame->y, frame->w, frame->h);
        task_exit(task);
        return TRUE;
    }
    return FALSE;
}

// 30 * 8
void show_content() {
    // 读取
    file_t *file =file_open(name,FALSE);
    if(!file){
        return;
    }
    has_more=file->size>30*8+offset;
    file_seek(file,offset);
    char *buff= mem_alloc(30*8);
    file_read(file,30*8,(uint8_t *)buff);
    // 绘制
    fill_rect(frame->vram,frame->w,8,28,240,128,COL_FFFFFF);
    for (int i = 7; i >=0; --i) {
        draw_str(frame->vram,frame->w,8,28+i*16,buff+i*30,COL_000000);
        buff[i*30]='\0';// 显示完就就行截断
    }
    frame_refresh_sub(frame->x+8,frame->y+28,240,128);
    // 回收
    file_close(file);
    mem_free(buff,30*8);
}

void nano_task(){
    while (TRUE){
        cli();
        if(fifo_can_read(&frame->kdb_fifo)){
            uint8_t data= fifo_read(&frame->kdb_fifo);
            sti();
            switch (data) {
                case 17: // W
                    if(offset>0){// 向前1行
                        offset=max(offset-30,0);
                        show_content();
                    }
                    break;
                case 31: // S
                    if(has_more){// 向后一行
                        offset+=30;
                        show_content();
                    }
                    break;
                case 30: // A
                    if(offset>0){// 向前1页
                        offset=max(offset-30*8,0);
                        show_content();
                    }
                    break;
                case 32: // D
                    if(has_more){// 向后1页
                        offset+=30*8;
                        show_content();
                    }
                    break;
            }
        } else{
            sti();
            hlt();
        }
    }
}

void nano_init() {
    draw_window(frame->vram, frame->w, frame->h, "NANO");
    draw_textbox(frame->vram, frame->w, 8, 28, 248, 156, COL_FFFFFF);
    frame_refresh_sub(frame->x, frame->y, frame->w, frame->h);
}

void init_nano(char *path){
    if(frame&&frame->used){// 防止多次开启
        return;
    }
    frame= frame_alloc((SCREEN_W - 256) / 2, (SCREEN_H - 165) / 2, 256, 165);
    frame->flag|=FRAME_CLICK;
    frame->click=nano_click;
    nano_init();
    task=create_task(nano_task,3);
    offset=0;
    mem_copy(name,path,16);
    show_content();
//
//    char *temp ="I want to teach my students how to live this life on earth,To face its struggle and its strife and improve their worth.Not just the lesson in a book or how the rivers flow,But h ow to choose the proper path wherever they may go.To understand eternal truth and kn ow the right from wrong,And gather all the beauty of a flower and a song.For if I help th e world to grow in wisdom and in grace,Then,I shall feel that I have won and I have fill ed my place.And so I ask my guidance,God,that I may do my part,For character and confidence and happiness of heart.";
//    file_t *file= file_open("poem",TRUE);
//    file_write(file, str_len(temp),(uint8_t *)temp);
//    file_close(file);
}