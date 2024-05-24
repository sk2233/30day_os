#ifndef TOOL_H
#define TOOL_H
#include "comm/types.h"

int itoa(char *buff, uint32_t num, uint8_t base);
int append(char *buff,char *sub);
void fmt(char * buff, char * fmt, ...);
void mem_set(void *ptr,uint8_t val,uint32_t size);
void mem_copy(void *desc,void *src,uint32_t size);
bool_t mem_eq(void *ptr1,void *ptr2);
bool_t str_pfx(char *str, char *pfx);
uint32_t str_len(char *buff);

#endif