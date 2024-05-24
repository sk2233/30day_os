#ifndef TASK_H
#define TASK_H

#include "comm/types.h"

typedef struct tss {
    uint32_t backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldtr, iomap;
}tss_t;

typedef struct task task_t;

struct task{
    uint32_t time; // 可以开始执行的时间
    uint32_t selector; // tss选择子
    tss_t *tss;
    task_t *next; // 下一个
    int8_t slice;
    int8_t last_slice;
    bool_t die;
};

void init_task();
void task_switch();
void task_sleep(uint32_t time);
task_t *create_task(void (*func)(),int8_t slice);
void task_exit(task_t *task);

#endif