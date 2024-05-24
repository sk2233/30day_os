#include "app/image.h"
#include "frame.h"
#include "comm/config.h"
#include "file.h"
#include "mem.h"
#include "graphic.h"

static frame_t *frame;

bool_t image_click(int x,int y){
    if(window_close(frame, x, y)){
        frame_free(frame);
        frame_refresh_sub(frame->x, frame->y, frame->w, frame->h);
        return TRUE;
    }
    return FALSE;
}

void image_init(char *path) {
    draw_window(frame->vram, frame->w, frame->h, "IMG");
    file_t *file=file_open(path,FALSE);
    if(file){
        // 图像大小暂时固定
        uint8_t *buff= mem_alloc(32*32);
        file_read(file,32*32,buff);
        file_close(file);
        draw_block(frame->vram,frame->w,frame->h,8,28,32,32,buff);
        mem_free(buff,32*32);
    }
    frame_refresh_sub(frame->x,frame->y,frame->w,frame->h);
}

void init_image(char *path){
    if(frame&&frame->used){// 防止多次开启
        return;
    }
    // 暂时文件里面没有记录图片的长宽信息，先直接写死
    frame= frame_alloc((SCREEN_W - 256) / 2, (SCREEN_H - 165) / 2, 32+8*2, 32+9+28);
    frame->flag|=FRAME_CLICK;
    frame->click=image_click;
    image_init(path);
}