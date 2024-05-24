#include <stdarg.h>
#include "tool.h"

static char num2char[]="0123456789ABCDEF";

int itoa(char *buff, uint32_t num, uint8_t base) {
    char temp[32];//暂定 32位 再多就不行了
    int index =0;
    while (num){
        temp[index]=num2char[num%base];
        index++;
        num/=base;
    }
    int res = index;
    while (index>0){
        *buff++=temp[index-1];
        index--;
    }
    return res;
}

int append(char *buff,char *sub){
    int res =0;
    while (*sub){
        *buff++=*sub++;
        res++;
    }
    return res;
}

void fmt(char * buff, char * fmt, ...) {
    va_list args;
    va_start(args, fmt);

    bool_t in_fmt=FALSE;
    char ch;
    int val;
    while (*fmt){
        ch=*fmt;
        fmt++;
        if(in_fmt){// 格式化
            switch (ch) {
                case 'd':
                    val = va_arg(args, int);
                    buff += itoa(buff, val, 10);
                    break;
                case 'x':
                    val = va_arg(args, int);
                    buff += itoa(buff, val, 16);
                    break;
                case 'b':
                    val = va_arg(args, int);
                    buff += itoa(buff, val, 2);
                    break;
                case 'c':
                    val = va_arg(args, int);
                    *buff++=(char )val;
                    break;
                case 's':
                    val = va_arg(args, int );
                    buff+= append(buff,(char *)val);
                    break;
                default:
                    *buff++='?';
                    break;
            }
            in_fmt=FALSE;
        } else if(ch=='%'){// 进入格式化模式
            in_fmt=TRUE;
        } else{// 普通字符串
            *buff++=ch;
        }
    }
    *buff='\0'; // 直接截断
    va_end(args);
}

void mem_set(void *ptr,uint8_t val,uint32_t size){
    uint8_t *temp=ptr;
    for (int i = 0; i < size; ++i) {
        *temp++=val;
    }
}

bool_t mem_eq(void *ptr1,void *ptr2){
    uint8_t *p1=ptr1;
    uint8_t *p2=ptr2;
    while (*p1&&*p2&&*p1==*p2){
        p1++;
        p2++;
    }
    return (!*p1)&&(!*p2);
}

void mem_copy(void *desc,void *src,uint32_t size){
    uint32_t count=size/4;
    uint32_t *d32=desc;
    uint32_t *s32=src; // 先4个一批拷贝，再单个拷贝
    for (int i = 0; i < count; ++i) {
        *d32++=*s32++;
    }
    count=size%4;
    uint8_t *d8=(uint8_t*)d32;
    uint8_t *s8=(uint8_t*)s32;
    for (int i = 0; i < count; ++i) {
        *d8++=*s8++;
    }
}

bool_t str_pfx(char *str, char *pfx){
    while (*str&&*pfx&&*str==*pfx){
        str++;
        pfx++;
    }
    return !*pfx;
}

uint32_t str_len(char *buff){
    uint32_t res=0;
    while (*buff){
        buff++;
        res++;
    }
    return res;
}