#include "timer.h"
#include "mem.h"
#include "fifo.h"
#include "task.h"
#include "comm/cpu_instr.h"

static event_t *curr_event;

uint32_t timer;
fifo_t timer_fifo;

void init_timer(){
    curr_event=0;
    timer=0;
}

event_t *insert_event(event_t *curr, event_t *next) {
    if(!curr){
        return next;
    }
    if(curr->time>next->time){
        next->next=curr;
        return next;
    }
    curr->next=insert_event(curr->next,next);
    return curr;
}

void add_timer(uint32_t time, void (*callback)()){
    event_t *event= mem_alloc(sizeof(event_t));
    if(!event){
        return;
    }
    event->time=time;
    event->callback=callback;
    curr_event=insert_event(curr_event,event);
}

event_t *remove_event(event_t *curr, void (*target)()) {
    if(curr==0){
        return curr;
    }
    if(curr->callback==target){// 移除成功
        return curr->next;
    }
    curr->next= remove_event(curr->next,target);
    return curr;
}

void remove_timer(void (*callback)()){
    curr_event=remove_event(curr_event,callback);
}

// 每10ms调用一次
void tick_timer(){
    timer++;
    // 其他异步任务回掉还是使用fifo队列
    while (curr_event&&curr_event->time<=timer&& fifo_can_write(&timer_fifo)){
        fifo_write(&timer_fifo,(uint32_t)curr_event->callback);
        mem_free(curr_event, sizeof(event_t));
        curr_event=curr_event->next;
    }
    if(timer%2==0){
        // 定时切换task若是需要监听 fifo 实现，要保证每个进程都去查看fifo队列太麻烦了，直接在这里处理尽可能快速解决
        task_switch();
    }
}