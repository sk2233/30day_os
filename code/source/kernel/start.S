  	// 16位代码，务必加上
  	.code16
 	.text
 	.extern kernel_main16
main:
	// 栈和段等沿用之前的设置,也可以重新设置
	// 这里简单起见，就不做任何设置了
	// VGA显卡，800x600x8bit 设置显示模式
	// 显示模式
    //	0x101 :  640 x  480 x 8bit 彩色
    //	0x103 :  800 x  600 x 8bit 彩色
    //	0x105 : 1024 x  768 x 8bit 彩色
    //	0x107 : 1280 x 1024 x 8bit 彩色
	mov	$0x13,%al
	mov	$0x00,%ah
    int	$0x10

    // 校验可以使用高分辨率
	// mov        $0x9000,%ax
    // MOV        %ax,%es
    // MOV        $0,%di
    // MOV        $0x4f00,%ax
    // int        $0x10
     //mov        $0x103,%cx    
    //mov        $0x4f01,%ax
     //int        $0x10

	// 你可能会想，直接跳到kernel_main16，但这样需要先知识kernel_main16在哪儿
	// kernel没有这个能力做到，所以直接用汇编，以便main处于整个bin文件开头，这样boot直接跳到开头就可以
	jmp kernel_main16

	// 32位保护模式下的代码
	.code32
	.text
	.global protect_mode_main
	.extern kernel_main32
protect_mode_main:
	// 重新加载所有的数据段描述符
	mov $16, %ax		// 16为数据段选择子
	mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    // 长跳转进入到32位内核加载模式中
    jmp $8, $kernel_main32

// 键盘中断处理函数 保护上下文，为调用函数初始化环境，iret返回中断 注意返回的中断还是返回到对应的段选择子，若是期间重新设置的gdt需要保证对应的选项与原来兼容(cpu会保存选择子)
	.text
	.global int_handler_kbd
	.extern handler_kbd
int_handler_kbd:
    pushal
    push %ds
    push %es
    push %fs
    push %gs
    push %esp
    call handler_kbd
    add $(1*4), %esp
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popal
    iret

// 鼠标中断处理函数
	.text
	.global int_handler_mouse
	.extern handler_mouse
int_handler_mouse:
    pushal
    push %ds
    push %es
    push %fs
    push %gs
    push %esp
    call handler_mouse
    add $(1*4), %esp
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popal
    iret

	.text
	.global int_handler_pit
	.extern handler_pit
int_handler_pit:
    pushal
    push %ds
    push %es
    push %fs
    push %gs
    push %esp
    call handler_pit
    add $(1*4), %esp
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popal
    iret

// 噪声中断处理函数 详情参考 handler_noise
	.text
	.global int_handler_noise
	.extern handler_noise
int_handler_noise:
    pushal
    push %ds
    push %es
    push %fs
    push %gs
    push %esp
    call	handler_noise
    add $(1*4), %esp
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popal
    iret

