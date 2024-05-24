// 16位代码，必须加上放在开头，以便有些io指令生成为32位
__asm__(".code16gcc");

#include "kernel.h"
#include "comm/cpu_instr.h"
#include "mem.h"

// GDT表。临时用，后面内容会替换成自己的
static uint16_t gdt_table[][4] = {
    {0, 0, 0, 0},
    {0xFFFF, 0x0000, 0x9A00, 0x00CF}, // 可执行代码段  0 ~ 0xFFFF
    {0xFFFF, 0x0000, 0x9200, 0x00CF}, // 段寄存器
};

/**
 * 进入保护模式
 */
static void  enter_protect_mode() {
    // 关中断
    cli();

    // 开启A20地址线，使得可访问1M以上空间
    // 使用的是Fast A20 Gate方式，见https://wiki.osdev.org/A20#Fast_A20_Gate
    uint8_t v = inb(0x92);
    outb(0x92, v | 0x2);

    // 加载GDT。由于中断已经关掉，IDT不需要加载
    lgdt((uint32_t)gdt_table, sizeof(gdt_table));

    // 打开CR0的保护模式位，进入保持模式
    uint32_t cr0 = read_cr0();
    write_cr0(cr0 | (1 << 0));

    // 长跳转进入到保护模式
    // 使用长跳转，以便清空流水线，将里面的16位代码给清空
    far_jump(8, (uint32_t)protect_mode_main);
}

// 参考：https://wiki.osdev.org/Memory_Map_(x86)
// 1MB以下比较标准, 在1M以上会有差别
// 检测：https://wiki.osdev.org/Detecting_Memory_(x86)#BIOS_Function:_INT_0x15.2C_AH_.3D_0xC7
static void  detect_memory() {
    uint32_t contID = 0;
    smap_t temp;
    int signature, bytes;

    // 初次：EDX=0x534D4150,EAX=0xE820,ECX=24,INT 0x15, EBX=0（初次）
    // 后续：EAX=0xE820,ECX=24,
    // 结束判断：EBX=0
    ram_info.ram_count = 0;
    for (int i = 0; i < RAM_COUNT; i++) {
        smap_t * smap = &temp;

        __asm__ __volatile__("int  $0x15"
                : "=a"(signature), "=c"(bytes), "=b"(contID)
                : "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(smap));
        if (signature != 0x534D4150) {
            return;
        }

        // todo: 20字节
        if (bytes > 20 && (smap->acpi & 0x0001) == 0){
            continue;
        }

        // 保存RAM信息，只取32位，空间有限无需考虑更大容量的情况
        if (smap->type == 1) {
            ram_info.rams[ram_info.ram_count].start = smap->baseL;
            ram_info.rams[ram_info.ram_count].size = smap->lengthL;
            ram_info.ram_count++;
        }

        if (contID == 0) {
            break;
        }
    }
}

// 16位main
void kernel_main16() {
    detect_memory();
    enter_protect_mode();
}


