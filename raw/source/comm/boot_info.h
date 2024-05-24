#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include "types.h"

#define BOOT_INFO_RAM_COUNT			10		// 暂时认为只有10块RAM

/**
 * 启动信息参数
 */
typedef struct boot_info {
    // RAM区信息
    struct {
        uint32_t start;
        uint32_t size;
    }rams[BOOT_INFO_RAM_COUNT];
    int ram_count;
}boot_info_t;

#define SECTOR_SIZE		512			// 磁盘扇区大小
#define KERNEL_ADDR		0x100000		// 内核加载的起始地址

#endif
