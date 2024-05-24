#include "task.h"
#include "mem.h"
#include "desc.h"
#include "comm/cpu_instr.h"
#include "timer.h"

static uint16_t tss_index;
static task_t *curr_task;
static bool_t task_die;

// 永远不会进行休眠的进程，用于为其他进程提供基础服务，例如定时任务
void idle_task(){
    while (TRUE){
        // 只有这一个地方使用 无需关中断
        if(fifo_can_read(&timer_fifo)){// 管理回掉
            uint32_t data= fifo_read(&timer_fifo);
            ((void (*)())data)();
        } else if(task_die){
            task_die=FALSE;
            task_t *task=curr_task->next;// 当前进程就是idle_task无需检察
            task_t *pre_task=curr_task;
            while (task!=curr_task){
                if(task->die){
                    // 移除任务
                    pre_task->next=task->next;
                    // 回收资源
                    mem_free(task->tss, sizeof(tss_t));
                    mem_free(task, sizeof(task_t));
                    // 继续循环
                    task=task->next;
                } else{
                    // 继续循环
                    pre_task=pre_task->next;
                    task=task->next;
                }
            }
        } else{
            hlt();
        }
    }
    // 这里若是直接return 会导致cpu不知道返回到哪里执行完毕切换就行了
}

void task_switch(){
    curr_task->last_slice--;
    if(curr_task->last_slice>0){
        return;
    }
    task_t *task=curr_task->next;
    while (task->time>timer||task->die){
        task=task->next;
    }
    if(curr_task==task){
        return;
    }
    curr_task=task;
    curr_task->last_slice=curr_task->slice;
    far_jump(curr_task->selector,0); // 任务再切换回来还会从这里回来
}

// 睡眠到指定时间
void task_sleep(uint32_t time){
    if(time<=timer){
        return;
    }
    // 保证立即切换
    curr_task->time=time;
    curr_task->last_slice=0;
    task_switch();
}

// 操作当前进程退出 tss 描述符无法复用了，一直递增使用的，再回头使用不方便先直接抛弃
void task_exit(task_t *task){
    if(!task){// 传空使用当前进程
        task=curr_task;
    }
    // 标记死亡，依赖idle_task回收资源 不会回收tss描述符
    task->die=TRUE;
    task_die=TRUE;
    // 切换进程
    task->last_slice=0;
    task_switch();
}

task_t *create_task(void (*func)(),int8_t slice){
    tss_t *tss= mem_alloc(sizeof(tss_t));
    tss->ldtr=0;
    tss->iomap=0x40000000;
    tss->eip=(uint32_t)func;
    tss->esp=(uint32_t)(mem_alloc(1024) + 1024); // 新分配的栈空间
    tss->eflags=0x00000202;
    tss->es = 2 * 8;
    tss->cs = 1 * 8;
    tss->ss = 2 * 8;
    tss->ds = 2 * 8;
    tss->fs = 2 * 8;
    tss->gs = 2 * 8;
    create_seg_desc(tss_index, 103, (uint32_t)tss, ATTR_TSS);

    task_t *task= mem_alloc(sizeof(task_t));
    task->selector=tss_index*8;
    task->tss=tss;
    task->slice=slice;
    // 构建环形单链表
    task->next=curr_task->next;
    curr_task->next=task;
    tss_index++;
    return task;
}

void init_task(){
    // 初始化主进程
    tss_t *main= mem_alloc(sizeof(tss_t));
    main->ldtr=0;
    main->iomap=0x40000000;
    create_seg_desc(3, 103, (uint32_t)main, ATTR_TSS);
    curr_task= mem_alloc(sizeof(task_t));
    curr_task->selector=3*8;
    curr_task->tss=main;
    curr_task->next=curr_task;
    curr_task->slice=5;
    curr_task->last_slice=5;
    write_tr(3*8);// 为cpu设置当前任务默认值
    // 为创建其他进程做处理
    tss_index=4;// 1,2是代码段，3要留给main进程使用，其他进程从4开始
    task_die=FALSE;
    create_task(idle_task,1);
}
