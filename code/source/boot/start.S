	// 16位代码，务必加上
  	.code16
 	.text
	.extern boot_main
main:
	// 重置数据段寄存器
	mov $0, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	// 根据https://wiki.osdev.org/Memory_Map_(x86)
	// 使用0x7c00之前的空间作栈，大约有30KB的RAM，足够boot和loader使用
	mov $main,%sp

	// 加载loader，只支持磁盘1
	// https://wiki.osdev.org/Disk_access_using_the_BIOS_(INT_13h)
read_kernel:
	mov $0x8000, %bx	// 读取到的内存地址
	mov $0x2, %cx		// ch:磁道号，cl起始扇区号
	mov $0x2, %ah		// ah: 0x2读磁盘命令
	mov $128, %al		// al: 读取的扇区数量, 必须不大于128，这里使用64k作为kernel，若是kernel太大会出问题，可以尝试多读几次
	mov $0x0080, %dx	// dh: 磁头号，dl驱动器号0x80(磁盘1)
	int $0x13
	jc read_kernel      // 发生失败重复读取

	// 跳转至c部分执行，再由c部分做一些处理
	jmp boot_main

	// 引导结束段
	.section boot_end
boot_sig: .byte 0x55, 0xaa
