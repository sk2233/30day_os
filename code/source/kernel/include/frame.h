#ifndef FRAME_H
#define FRAME_H

#include "comm/types.h"
#include "fifo.h"

#define FRAME_COUNT 32

#define FRAME_HIDE (1<<0)
//#define FRAME_TOP  (1<<1)
#define FRAME_CLICK  (1<<2)
#define FRAME_ALPHA  (1<<3)

typedef struct frame{
    uint8_t *vram;
    int x,y;
    uint16_t w,h;
    uint16_t order;
    uint8_t flag;
    bool_t used;
    bool_t (*click)(int x,int y);// 右键点击事件暂时只处理这个 因为事件不是那么频繁暂时直接处理不再借助队列 可选的
    fifo_t kdb_fifo; // 由主队列把事件中转过来使用 都有的
}frame_t;

extern frame_t *bg_frame;
extern frame_t *cursor_frame;
extern frame_t *window_frame;

void init_frame();
void frame_free(frame_t *frame);
frame_t *frame_alloc(int x,int y,uint16_t w,uint16_t h);
void frame_top(frame_t *frame);
void frame_sort();
void frame_move(frame_t *frame,int x,int y);
void frame_refresh();
void frame_refresh_sub(int x,int y,uint16_t w,uint16_t h);

void draw_window(uint8_t *vram, uint16_t w, uint16_t h, char *title);
bool_t window_close(frame_t *frame,int x,int y);
void draw_textbox(uint8_t *vram,uint16_t w,int minX, int minY, int maxX, int maxY, uint8_t col);

frame_t *click_frame(int x,int y);

#endif