#ifndef TIMER_H
#define TIMER_H

#include "comm/types.h"
#include "fifo.h"

#define EVENT_COUNT 32

typedef struct event event_t;

struct event{
    uint32_t time;
    void (*callback)();
    event_t *next;
};

extern uint32_t timer;
extern fifo_t timer_fifo;

void init_timer();
void add_timer(uint32_t time, void (*callback)());
void remove_timer(void (*callback)());
void tick_timer();

#endif