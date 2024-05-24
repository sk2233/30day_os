#include "app/line.h"
#include "frame.h"
#include "comm/config.h"
#include "graphic.h"

static frame_t *frame;

bool_t line_click(int x,int y){
    if(window_close(frame, x, y)){
        frame_free(frame);
        frame_refresh_sub(frame->x, frame->y, frame->w, frame->h);
        return TRUE;
    }
    return FALSE;
}

void line_init() {
    draw_window(frame->vram, frame->w, frame->h, "LINE");
    draw_line(frame->vram,frame->w,8,28,8+64,28,COL_FF0000);
    draw_line(frame->vram,frame->w,8,28,8+64,28+32,COL_FF00FF);
    draw_line(frame->vram,frame->w,8,28,8+64,28+64,COL_00FF00);
    draw_line(frame->vram,frame->w,8,28,8+32,28+64,COL_FFFF00);
    draw_line(frame->vram,frame->w,8,28,8,28+64,COL_0000FF);
    frame_refresh_sub(frame->x,frame->y,frame->w,frame->h);
}

void init_line(){
    if(frame&&frame->used){// 防止多次开启
        return;
    }
    // 暂时文件里面没有记录图片的长宽信息，先直接写死
    frame= frame_alloc((SCREEN_W - 256) / 2, (SCREEN_H - 165) / 2, 64+8*2, 64+9+28);
    frame->flag|=FRAME_CLICK;
    frame->click=line_click;
    line_init();
}