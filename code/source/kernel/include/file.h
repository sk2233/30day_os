#ifndef FILE_H

#include "comm/types.h"
#include "comm/config.h"

#define FILE_H

#define DISK_CMD_READ  0x24
#define DISK_CMD_WRITE 0x34

#define FILE_BASE_SEL 256
#define FILE_HEADER_SEL (FILE_BASE_SEL+0)
#define FILE_LINK_SEL  (FILE_BASE_SEL+1)

#define FILE_ITEM_COUNT 32

#define FILE_LINK_EMPTY 0
#define FILE_LINK_END   1

// 这里使用的是 fat16文件系统与书中的 fat 文件系统不同，这里模拟一个内存文件系统
// 实现类似fat16文件系统
// 第256个扇区存储文件表 每个文件表项使用16byte 11byte名称 1byte相对扇区号() 4byte文件大小 最多存储32个文件
// 第257个扇区存储扇区链表，只有前256byte有效存储相对偏移(从256开始的相对偏移，0指向文件表无效，1指向文件链接表无效)
// 258～512扇区储存文件内容
#pragma pack(1)
typedef struct file_item{ // 磁盘上的一个表项
    char name[11];
    uint8_t selector;
    uint32_t size;
}file_item_t;
#pragma pack()

typedef struct file{ // 实际内存中使用的
    char name[11];
    uint8_t base_sel;
    uint8_t curr_sel;
    uint8_t buff[SECTOR_SIZE];
    uint32_t pos;
    uint32_t size;
    int item_index;
}file_t;

void init_file();
file_t *file_open(char *name,bool_t create);// 打开文件并指定是否需要在文件不存在时进行创建
uint32_t file_read(file_t *file,uint32_t size,uint8_t *buff);
bool_t file_seek(file_t *file,uint32_t pos);
void file_write(file_t *file,uint32_t size,uint8_t *buff);
void file_flush(file_t *file); // 主要是针对写操作，默认写操作不会立即写入，调用这个方法使其立即写入，file_close也会写入
void file_close(file_t *file); // 主要是释放资源
void file_delete(file_t *file); // 标记删除

void disk_read(uint32_t sector, uint32_t count, void *buff);
void disk_write(uint32_t sector, uint32_t count, void *buff);

#endif