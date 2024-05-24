#include "app/anim.h"
#include "comm/types.h"
#include "frame.h"
#include "file.h"
#include "mem.h"
#include "graphic.h"
#include "timer.h"

static frame_t *frame;
static uint8_t *imgs;
static uint8_t index;

void anim_step(){
    add_timer(timer+10,anim_step);
    index=(index+1)%ANIM_COUNT;
    draw_block(frame->vram,frame->w,frame->h,8,28,ANIM_W,ANIM_H,imgs+index*ANIM_W*ANIM_H);
    frame_refresh_sub(frame->x+8,frame->y+28,ANIM_W,ANIM_H);
}

bool_t anim_click(int x,int y){
    if(window_close(frame, x, y)){
        remove_timer(anim_step);
        mem_free(imgs,ANIM_SIZE);
        frame_free(frame);
        frame_refresh_sub(frame->x, frame->y, frame->w, frame->h);
        return TRUE;
    }
    return FALSE;
}

void anim_init() {
    index=0;
    add_timer(timer+10,anim_step);
    draw_window(frame->vram, frame->w, frame->h, "ANIM");
    frame_refresh_sub(frame->x,frame->y,frame->w,frame->h);
    imgs= mem_alloc(ANIM_SIZE);
    disk_read(ANIM_SEL,ANIM_SEL_SIZE,imgs);
}

void init_anim(){ // 暂时不考虑透明
    if(frame&&frame->used){// 防止多次开启
        return;
    }
    frame= frame_alloc((SCREEN_W - ANIM_W) / 2, (SCREEN_H - ANIM_H) / 2, ANIM_W+8*2, ANIM_H+9+28);
    frame->flag|=FRAME_CLICK;
    frame->click=anim_click;
    anim_init();
}