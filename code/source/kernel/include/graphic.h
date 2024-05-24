#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "comm/types.h"

#define COL_000000		0
#define COL_FF0000		1
#define COL_00FF00		2
#define COL_FFFF00		3
#define COL_0000FF		4
#define COL_FF00FF		5
#define COL_00FFFF		6
#define COL_FFFFFF		7
#define COL_C6C6C6		8
#define COL_840000		9
#define COL_008400		10
#define COL_848400		11
#define COL_000084		12
#define COL_840084		13
#define COL_008484		14
#define COL_848484		15
// 透明色标记
#define COL_ALPHA       0xFF

void fill_alpha(uint8_t *vram, uint16_t w, uint16_t h);
void fill_rect(uint8_t *vram, uint16_t vram_w, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t col);
void draw_char(uint8_t *vram, uint16_t vram_w, uint16_t x, uint16_t y, char c, uint8_t col);
void draw_str(uint8_t *vram, uint16_t vram_w, uint16_t x, uint16_t y, char *s, uint8_t col);
void draw_block(uint8_t *vram, uint16_t vram_w,uint16_t vram_h, int x, int y, uint16_t w, uint16_t h, uint8_t *block);
void draw_block_alpha(uint8_t *vram, uint16_t vram_w,uint16_t vram_h, int x, int y, uint16_t w, uint16_t h, const uint8_t *block);
void draw_block_sub(uint8_t *vram, uint16_t vram_w, int minX, int minY, int maxX, int maxY,
                    int x, int y, uint16_t w, uint16_t h, uint8_t *block);
void draw_block_sub_alpha(uint8_t *vram, uint16_t vram_w, int minX, int minY, int maxX, int maxY,
                          int x, int y, uint16_t w, uint16_t h, const uint8_t *block);
void init_graphic();
void set_palette(uint8_t index, const uint8_t *rgbs,uint8_t size);
void draw_point(uint8_t *vram, uint16_t vram_w, uint16_t x, uint16_t y,uint8_t col);
void draw_line(uint8_t *vram, uint16_t vram_w,uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t col);

#endif