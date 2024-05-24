  	// 16位代码，务必加上
  	.code16
 	.text
main:
	MOV		$0,%ax			; 初始化寄存器
	MOV		%ax,%ss
	MOV		$0x7c00,%sp
	MOV		%ax,%ds
	MOV		%ax,%es

	MOV		msg,%si
putloop:
	MOV		[%si],%al
	ADD		$1,%si			; 给SI加1
	CMP		$0,%al
	JE		fin
	MOV		$0x0e,%ah			; 显示一个文字
	MOV		$15,%bx			; 指定字符颜色
	int     $0x10			; 调用显卡BIOS
	JMP		putloop
fin:
	HLT						; 让CPU停止，等待指令
	JMP		fin				; 无限循环
msg:
	.byte   'L',0
	
	// 引导结束段 
	.section boot_end
 	.byte 0x55, 0xaa
