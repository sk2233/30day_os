#ifndef QUEUE_H
#define QUEUE_H
#include "comm/types.h"

typedef struct fifo{
    uint32_t *data;
    uint16_t read;
    uint16_t write;
    uint16_t count;
    uint16_t size;
}fifo_t;

void fifo_init(fifo_t *fifo,uint16_t size);
void fifo_free(fifo_t *fifo);
bool_t fifo_can_read(fifo_t *fifo);
bool_t fifo_can_write(fifo_t *fifo);
uint32_t fifo_read(fifo_t *fifo);
void fifo_write(fifo_t *fifo, uint32_t val);

#endif