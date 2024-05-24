#include "mem.h"
#include "tool.h"

ram_info_t ram_info;			// 启动参数信息 因为必须在16位模式下获取内存大小，初始化在 kernel_16.c 中完成
static mem_t mems[MEM_SIZE];

void init_mem(){
    mem_set(mems,0, sizeof(mems));
    // 忽略第一块可用内存，那个已经被当前代码占用了
    for (int i = 1; i < ram_info.ram_count; ++i) {
        mem_t *mem=mems+i-1;
        mem->used=TRUE;
        mem->start=ram_info.rams[i].start;
        mem->size= ram_info.rams[i].size;
    }
}

uint32_t mem_total(){
    uint32_t sum=0;
    for (int i = 0; i < MEM_SIZE; ++i) {
        if(mems[i].used){
            sum+=mems[i].size;
        }
    }
    return sum;
}

void *mem_alloc(uint32_t size){
    for (int i = 0; i < MEM_SIZE; ++i) {
        if(mems[i].used&&mems[i].size>size){
            mems[i].size-=size;// 从后面移除分配的并返回分配地址
            if(mems[i].size==0){ // 正好分配完啦
                mems[i].used=FALSE;
            }
            // 默认进行初始化
            void *ptr=(void *)(mems[i].start+mems[i].size);
            mem_set(ptr,0,size);
            return ptr;
        }
    }
    return 0;// 没有内存了，不太可能出现
}

void mem_free(void *ptr,uint32_t size){
    mem_t *mem=0; // 先找到一块空的用于记录
    for (int i = 0; i < MEM_SIZE; ++i) {
        if(!mems[i].used){
            mem=&mems[i];
            mem->used=TRUE;
            mem->start=(uint32_t)ptr;
            mem->size=size;
            break;
        }
    }
    if(mem==0){// 没有表项可以使用了，直接丢弃内存
        return;
    }

    // 试图合并前后相连的内存
    for (int i = 0; i < MEM_SIZE; ++i) {
        if(!mems[i].used){
            continue;
        }
        uint32_t start=mems[i].start;
        uint32_t end=start+ mems[i].size;
        if(end==mem->start){
            mem->start=start;
            mem->size+=mems[i].size;
            mems[i].used=FALSE;
        } else if(start==mem->start+mem->size){
            mem->size+=mems[i].size;
            mems[i].used=FALSE;
        }
    }
}
