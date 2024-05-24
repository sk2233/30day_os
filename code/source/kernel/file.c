#include "file.h"
#include "comm/cpu_instr.h"
#include "comm/config.h"
#include "mem.h"
#include "tool.h"

void disk_cmd(uint32_t sector,uint32_t count,uint8_t cmd){
    outb(0x1F6, (uint8_t) 0xE0);

    outb(0x1F2, (uint8_t) (count >> 8));
    outb(0x1F3, (uint8_t) (sector >> 24));		// LBA参数的24~31位
    outb(0x1F4, (uint8_t) (0));					// LBA参数的32~39位
    outb(0x1F5, (uint8_t) (0));					// LBA参数的40~47位

    outb(0x1F2, (uint8_t) count);
    outb(0x1F3, (uint8_t) sector);			// LBA参数的0~7位
    outb(0x1F4, (uint8_t) (sector >> 8));		// LBA参数的8~15位
    outb(0x1F5, (uint8_t) (sector >> 16));		// LBA参数的16~23位

    outb(0x1F7, cmd);
}

// 读取磁盘
void disk_read(uint32_t sector, uint32_t count, void *buff) {
    disk_cmd(sector,count,DISK_CMD_READ);
    // 读取数据
    uint16_t *temp = buff;
    while (count-- > 0) {
        // 读取时先检察再读取
        while (inb(0x1F7) & 0x80) {}
        // 读取并将数据写入到缓存中
        for (int i = 0; i < SECTOR_SIZE / 2; i++) {
            *temp++ = inw(0x1F0);
        }
    }
}

// 写入磁盘
void disk_write(uint32_t sector, uint32_t count, void *buff) {
    disk_cmd(sector,count,DISK_CMD_WRITE);
    // 写入数据
    uint16_t *temp = (uint16_t*) buff;
    while (count-- > 0) {
        // 每次写入一个扇区的数据，然后等待数据的写入
        for (int i = 0; i < SECTOR_SIZE / 2; i++) {
            outw(0x1F0,*temp++);
        }
        // 先写入再检察
        while (inb(0x1F7) & 0x80) {}
    }
}

void init_file(){

}

file_t *file_item2file(file_item_t *item,int index) {
    file_t *file= mem_alloc(sizeof(file_t));
    mem_copy(file->name, item->name, 11);
    file->size= item->size;
    file->base_sel=item->selector;
    file->curr_sel=FILE_LINK_EMPTY; // 默认执行空表示没有缓存
    file->pos=0;
    file->item_index=index;
    return file;
}

uint8_t find_empty_sel() {
    uint8_t *sel= mem_alloc(SECTOR_SIZE);
    disk_read(FILE_LINK_SEL,1,sel);
    uint8_t res=FILE_LINK_END; // 一般不会使用这个默认值
    for (int i = 2; i < 256; ++i) {// 0 1 都是无效的
        if(sel[i]==FILE_LINK_EMPTY){
            res=i;
            sel[i]=FILE_LINK_END;
            disk_write(FILE_LINK_SEL,1,sel);
            break;
        }
    }
    mem_free(sel,SECTOR_SIZE);
    return res;
}

file_t *file_open(char *name, bool_t create){
    file_item_t *base_item= mem_alloc(SECTOR_SIZE);
    disk_read(FILE_HEADER_SEL, 1, base_item);
    file_t *file=0;
    for (int i = 0; i < FILE_ITEM_COUNT; ++i) {
        file_item_t *item=base_item+i;
        if(mem_eq(item->name, name)){// 找到了
            file= file_item2file(item, i);
            break;
        }
    }

    if(!create||file){
        mem_free(base_item,SECTOR_SIZE);
        return file;
    }

    for (int i = 0; i < FILE_ITEM_COUNT; ++i) {
        file_item_t *item=base_item+i;
        if(item->name[0]=='\0'){// 空表项，所有文件名称都不能为空
            mem_copy(item->name,name, str_len(name));
            item->size=0;
            item->selector=find_empty_sel();// 就算为空也分配一个有效扇区，防止后续写入再修改表头
            disk_write(FILE_HEADER_SEL,1,base_item);
            file= file_item2file(item, i);
            break;
        }
    }
    mem_free(base_item,SECTOR_SIZE);
    return file;
}

void make_sure_buff(file_t *file, const uint8_t *sel) {
    uint8_t count=file->pos/SECTOR_SIZE;
    uint8_t curr_sel=file->base_sel;
    while (count>0){
        curr_sel=sel[curr_sel];
        count--;
    }
    if(curr_sel != file->curr_sel){
        file->curr_sel=curr_sel;
        disk_read(FILE_BASE_SEL+(uint32_t)curr_sel,1,file->buff);
    }
}

uint32_t file_read(file_t *file, uint32_t size, uint8_t *buff){
    if(file->pos+size>file->size){
        size=file->size-file->pos;
    }
    uint32_t read_size=size;

    uint8_t *sel= mem_alloc(SECTOR_SIZE);
    disk_read(FILE_LINK_SEL,1,sel);
    while (size){
        make_sure_buff(file,sel);
        uint32_t read = file->pos%SECTOR_SIZE;
        uint32_t has=SECTOR_SIZE-read;
        if(has>=size){// 读取完毕了
            mem_copy(buff,file->buff+read,size);
            file->pos+=size;
            size=0;
        } else{// 还需要继续读取,且读完整个扇区
            mem_copy(buff,file->buff+read,has);
            buff+=has;
            file->pos+=has;
            size-=has;
        }
    }
    mem_free(sel,SECTOR_SIZE);
    return read_size;
}

uint8_t file_next_sel(uint8_t curr, uint8_t *sel, uint32_t size) {
    if(size<=0){
        return FILE_LINK_END;
    }
    if(curr!=FILE_LINK_END){
        sel[curr]= file_next_sel(sel[curr],sel,size>SECTOR_SIZE?size-SECTOR_SIZE:0);
        return curr;
    }
    for (int i = 2; i < 256; ++i) {
        if(sel[i]==FILE_LINK_EMPTY){
            sel[i]=FILE_LINK_END;
            sel[i]= file_next_sel(sel[i],sel,size>SECTOR_SIZE?size-SECTOR_SIZE:0);
            return i;
        }
    }
    return 0; // 有问题了
}

void extend_file(file_t *file, uint32_t size, uint8_t *sel) {
    file->size=size;
    file->base_sel=file_next_sel(file->base_sel,sel,size);
    disk_write(FILE_LINK_SEL,1,sel); // 写入链接关系
    // 写入文件修改
    file_item_t *base_item= mem_alloc(SECTOR_SIZE);
    disk_read(FILE_HEADER_SEL, 1, base_item);
    file_item_t *item=base_item+file->item_index;
    item->selector=file->base_sel;
    item->size=file->size;
    disk_write(FILE_HEADER_SEL,1,base_item);
    mem_free(base_item,SECTOR_SIZE);
}

void file_write(file_t *file, uint32_t size, uint8_t *buff){
    uint8_t *sel= mem_alloc(SECTOR_SIZE);
    disk_read(FILE_LINK_SEL,1,sel);
    if(file->pos+size>file->size){
        extend_file(file,file->pos+size,sel);
    }

    while (size){
        make_sure_buff(file,sel);
        uint32_t write = file->pos%SECTOR_SIZE;
        uint32_t has=SECTOR_SIZE-write;
        if(has>=size){
            mem_copy(file->buff+write,buff,size);
            file->pos+=size;
            size=0;
        } else{
            mem_copy(file->buff+write,buff,has);
            disk_write(FILE_BASE_SEL+(uint32_t)file->curr_sel,1,file->buff);
            buff+=has;
            file->pos+=has;
            size-=has;
        }
    }
    mem_free(sel,SECTOR_SIZE);
}

void file_flush(file_t *file){
    if(file->curr_sel!=FILE_LINK_END&&file->curr_sel!=FILE_LINK_EMPTY){
        disk_write(FILE_BASE_SEL+(uint32_t)file->curr_sel,1,file->buff);
    }
}

void file_close(file_t *file){
    file_flush(file);
    mem_free(file, sizeof(file_t));
}

bool_t file_seek(file_t *file,uint32_t pos){
    if(pos>file->size){
        return FALSE;
    }
    file->pos=pos;
    return TRUE;
}

void free_file_link(file_t *file) {
    uint8_t *sel= mem_alloc(SECTOR_SIZE);
    disk_read(FILE_LINK_SEL,1,sel);
    uint8_t curr= file->base_sel;
    while (curr!=FILE_LINK_END){
        uint8_t next=sel[curr];// 全部释放
        sel[curr]=FILE_LINK_EMPTY;
        curr=next;
    }
    disk_write(FILE_LINK_SEL,1,sel);
    mem_free(sel,SECTOR_SIZE);
}

// 主要标记文件头，与文件链接，同时也会释放文件
void file_delete(file_t *file){
    // 写文件头
    file_item_t *base_item= mem_alloc(SECTOR_SIZE);
    disk_read(FILE_HEADER_SEL, 1, base_item);
    file_item_t *item=base_item+file->item_index;
    item->name[0]='\0';
    disk_write(FILE_HEADER_SEL,1,base_item);
    mem_free(base_item,SECTOR_SIZE);
    // 清零链接内容
    free_file_link(file);
    // 释放文件对象
    mem_free(file, sizeof(file_t));
}