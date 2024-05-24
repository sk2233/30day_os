#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include "types.h"

// 屏幕大小
#define SCREEN_W 320
#define SCREEN_H 200
// 显存所在位置
#define VRAM_ADDR 0x000a0000
#define VRAM_SIZE (SCREEN_W*SCREEN_H)

#define SECTOR_SIZE		512			// 磁盘扇区大小

#endif
