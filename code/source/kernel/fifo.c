#include "fifo.h"
#include "mem.h"

void fifo_init(fifo_t *fifo,uint16_t size){
    fifo->count=0;
    fifo->read=0;
    fifo->write=0;
    fifo->size=size;
    fifo->data= mem_alloc(size* sizeof(uint32_t));
}

void fifo_free(fifo_t *fifo){
    mem_free(fifo->data,fifo->size* sizeof(uint32_t));
}

bool_t fifo_can_read(fifo_t *fifo){
    return fifo->count>0;
}

bool_t fifo_can_write(fifo_t *fifo){
    return fifo->count<fifo->size;
}

uint32_t fifo_read(fifo_t *fifo){
    fifo->count--;
    uint32_t res = fifo->data[fifo->read];
    fifo->read=(fifo->read+1)%fifo->size;
    return res;
}

void fifo_write(fifo_t *fifo, uint32_t val){
    fifo->count++;
    fifo->data[fifo->write]=val;
    fifo->write=(fifo->write+1)%fifo->size;
}
