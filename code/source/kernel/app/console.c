#include "app/console.h"
#include "frame.h"
#include "timer.h"
#include "graphic.h"
#include "tool.h"
#include "mem.h"
#include "file.h"
#include "comm/cpu_instr.h"
#include "comm/math.h"
#include "task.h"
#include "app/image.h"
#include "app/line.h"
#include "app/anim.h"
#include "app/nano.h"

// 1 esc  0 no shift  1 yes shift
static char *code2char[]= {"??12345678"
                           "90-=?\tqwer"
                           "tyuiop[]??"
                           "asdfghjkl;"
                           "\'???zxcvbn"
                           "m,./??? ",
                           "??!@#$%^&*"
                           "()_+?\tQWER"
                           "TYUIOP{}??"
                           "ASDFGHJKL:"
                           "\"???ZXCVBN"
                           "M<>???? "};
static int cursor_x;
static int cursor_y;
static bool_t cursor_show;
static bool_t shift_press;
static frame_t *frame;
static task_t *task;

void cursor_blink(){
    add_timer(timer + 50, cursor_blink);
    uint8_t col=COL_000000;
    if(cursor_show){
        col=COL_FFFFFF;
    }
    fill_rect(frame->vram, frame->w, cursor_x, cursor_y, 8, 16, col);
    frame_refresh_sub(frame->x + cursor_x, frame->y + cursor_y, 8, 16);
    cursor_show=!cursor_show;
}

void add_char(char ch) {
    fill_rect(frame->vram, frame->w, cursor_x, cursor_y, 8, 16, COL_000000);
    draw_char(frame->vram, frame->w, cursor_x, cursor_y, ch, COL_FFFFFF);
    frame_refresh_sub(frame->x + cursor_x, frame->y + cursor_y, 8, 16);
    cursor_x+=8;
    if(cursor_x>240){
        cursor_x=8;
        cursor_y+=16;
    }
}

void clear_cursor() {
    fill_rect(frame->vram, frame->w, cursor_x, cursor_y, 8, 16, COL_000000);
    frame_refresh_sub(frame->x + cursor_x, frame->y + cursor_y, 8, 16);
}

// 参数通过空格分割，可以通过这个获取多个参数
char *get_param(char *cmd) {
    while (*cmd!=' '){
        cmd++;
    }
    while (*cmd==' '){
        cmd++;
    }
    return cmd;
}

// 多个参数前面的参数后面会有空格，使用改方法裁剪，先重复调用get_param获取所有参数，最后一个参数无需裁剪
void trim_param(char *param){
    while (*param&&*param!=' '){
        param++;
    }
    *param='\0';
}

void console_exit() {
    remove_timer(cursor_blink);// 移除回掉
    frame_free(frame);
    // frame_refresh();
    frame_refresh_sub(frame->x, frame->y, frame->w, frame->h);
    task_exit(task);// 可能是外部调用导致的必须指定当前进程
}

uint8_t console_exe(char cmd[32], char *res[]){
    if(str_len(cmd)==0){
        return 0;
    }
    if(mem_eq(cmd,"mem")){
        mem_set(res[0],0,32);
        fmt(res[0],"Mem %dM",mem_total()/1024/1024);
        return 1;
    } else if(mem_eq(cmd,"ls")){
        file_item_t *base_item= mem_alloc(SECTOR_SIZE);
        disk_read(FILE_HEADER_SEL, 1, base_item);
        uint8_t count=0;
        for (int i = 0; i < FILE_ITEM_COUNT; ++i) {
            file_item_t *item=base_item+i;
            if(item->name[0]!='\0'){
                mem_set(res[count],0,32);
                fmt(res[count++],"%s %dbyte",item->name,item->size);
                if(count>=8){// 最多显示8个文件 命令行显示的内容太少了
                    break;
                }
            }
        }
        mem_free(base_item,SECTOR_SIZE);
        return count;
    }else if(mem_eq(cmd,"exit")){
        console_exit();// 直接通过这个退出没有解除激活窗口，可能有bug 推荐点击关闭
        return 0;
    }else if(mem_eq(cmd,"line")){
        init_line();
        return 0;
    }else if(mem_eq(cmd,"anim")){
        init_anim();
        return 0;
    } else if(str_pfx(cmd,"cat")){
        char *name=get_param(cmd);
        file_t *file=file_open(name,FALSE);
        if(file){
            uint8_t count =0;
            while (count<8&&file->pos<file->size){ // 最多读取8行
                mem_set(res[count],0,32);
                file_read(file,32,(uint8_t *)res[count++]);
            }
            file_close(file);
            return count;
        } else{
            fmt(res[0],"file not find %s",name);
            return 1;
        }
    }else if(str_pfx(cmd,"write")){// write 文件名(没有自动创建) 内容
        char *name=get_param(cmd);
        char *content= get_param(name);
        trim_param(name);
        file_t *file=file_open(name,TRUE);
        file_write(file, str_len(content),(uint8_t *)content);
        file_close(file);
        return 0;
    } else if(str_pfx(cmd,"rm")){// write 文件名(没有自动创建) 内容
        char *name=get_param(cmd);
        file_t *file=file_open(name,FALSE);
        if(file){
            file_delete(file);
            return 0;
        } else{
            mem_set(res[0],0,32);
            fmt(res[0],"file not find %s",name);
            return 1;
        }
    } else if(str_pfx(cmd,"img")){
        char *path= get_param(cmd);
        init_image(path);
        return 0;
    }else if(str_pfx(cmd,"nano")){
        char *path= get_param(cmd);
        init_nano(path);
        return 0;
    }
    fmt(res[0],"No Cmd Of %s",cmd);
    return 1;
}

void console_task(){
    cursor_x=8;
    cursor_y=28;
    cursor_show=TRUE;
    shift_press=FALSE;
    add_timer(timer + 50, cursor_blink);
    char cmd_line[32];
    mem_set(cmd_line,0, sizeof(cmd_line));
    uint8_t cmd_index=0;
    char *res[8]; // 最多展示8行,每行最多32个字符
    for (int i = 0; i < 8; ++i) {
        res[i]= mem_alloc(32);
    }
    // 输出默认提示符
    add_char('>');
    while (TRUE){
        cli();
        if(fifo_can_read(&frame->kdb_fifo)){
            uint8_t data= fifo_read(&frame->kdb_fifo);
            sti();
            if(data==42){// shift 按下 暂时只处理 左shift
                shift_press=TRUE;
            } else if(data==170){// shift 松开
                shift_press=FALSE;
            } else if(data==14){ // 删除键
                // 清楚原来的防止留白
                clear_cursor();
                if(cursor_x>8){
                    cursor_x-=8;
                } else if(cursor_y>28){
                    cursor_y-=16;
                    cursor_x=240;
                }
                if(cmd_index>0){
                    cmd_line[--cmd_index]='\0';
                }
            } else if(data==15){ // 退格
                // 清楚原来的防止留白
                clear_cursor();
                cursor_x= clump(cursor_x+4*8,240,8);
                cmd_line[cmd_index++]=' ';
            } else if(data==28){ // 回车键
                if(mem_eq(cmd_line,"cls")){// 比较特别不输出任何内容,清屏幕
                    fill_rect(frame->vram, frame->w, 8, 28, 240, 128, COL_000000);
                    frame_refresh_sub(frame->x + 8, frame->y + 28, 240, 128);
                    cursor_x=8;
                    cursor_y=28;
                } else{
                    uint8_t count= console_exe(cmd_line,res);
                    if(cursor_y+count*16<140){// 可以直接展示
                        // 清楚原来的防止留白
                        clear_cursor();
                        cursor_y+=16;
                        cursor_x=8;
                        for (int i = 0; i < count; ++i) {
                            draw_str(frame->vram, frame->w, cursor_x, cursor_y, res[i], COL_FFFFFF);
                            cursor_y+=16;
                        }
                        frame_refresh_sub(frame->x + 8, frame->y + cursor_y - count * 16, 240, 16 * count);
                    } else{// 需要滚屏了
                        // 依旧需要把原来的移除
                        fill_rect(frame->vram, frame->w, cursor_x, cursor_y, 8, 16, COL_000000);
                        for (int y = 28; y < 140-16*count; ++y) {
                            mem_copy(frame->vram + y * 256 + 8, frame->vram + (y + 16 * count + 16) * 256 + 8, 240);
                        }
                        cursor_x=8;
                        cursor_y=140;
                        fill_rect(frame->vram, 256, 8, 140 - count * 16, 240, 16 + count * 16, COL_000000);
                        for (int i = 0; i < count; ++i) {
                            draw_str(frame->vram, frame->w, cursor_x, cursor_y - (count - i) * 16,
                                     res[i], COL_FFFFFF);
                        }
                        frame_refresh_sub(frame->x + 8, frame->y + 28, 240, 128);
                    }
                }
                // 输出默认提示符
                add_char('>');
                mem_set(cmd_line,0, sizeof(cmd_line));
                cmd_index=0;
            } else if(data<58){// 普通有按钮映射的按键
                char ch =code2char[shift_press][data];
                cmd_line[cmd_index++]=ch;
                add_char(ch);
            }
        } else{
            sti();
            hlt();
        }
    }
}

void console_init() {
    draw_window(frame->vram, frame->w, frame->h, "CONSOLE");
    draw_textbox(frame->vram, frame->w, 8, 28, 248, 156, COL_000000);
    frame_refresh_sub(frame->x, frame->y, frame->w, frame->h);
}

bool_t console_click(int x,int y){// 暂时只判断关闭事件
    if(window_close(frame, x, y)){
        console_exit();
        return TRUE;
    }
    return FALSE;
}

void init_console(){
    if(frame&&frame->used){// 防止多次开启
        return;
    }
    frame= frame_alloc((SCREEN_W - 256) / 2, (SCREEN_H - 165) / 2, 256, 165);
    frame->flag|=FRAME_CLICK;
    frame->click=console_click;
    console_init();
    task=create_task(console_task,3);
}