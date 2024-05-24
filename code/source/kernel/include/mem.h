#ifndef MEM_H
#define MEM_H

#include "comm/types.h"

#define MEM_SIZE 1024
// 暂时认为只有10块RAM
#define RAM_COUNT			10

// 内存检测信息结构
typedef struct smap {
    uint32_t baseL; // base address uint64_t
    uint32_t baseH;
    uint32_t lengthL; // length uint64_t
    uint32_t lengthH;
    uint32_t type; // entry type
    uint32_t acpi; // extended
}__attribute__((packed)) smap_t;

typedef struct ram_info {
    // RAM区信息
    struct {
        uint32_t start;
        uint32_t size;
    }rams[RAM_COUNT];
    uint8_t ram_count;
}ram_info_t;

typedef struct mem{
    uint32_t start,size;
    bool_t used;
}mem_t;

extern ram_info_t ram_info;

void init_mem();
uint32_t mem_total();
void *mem_alloc(uint32_t size);
void mem_free(void *ptr,uint32_t size);

#endif