# 适用于mac   -s -S 是等待 vscode链接
qemu-system-i386  -m 128M -s -S -serial stdio -drive file=disk.dmg,index=0,media=disk,format=raw -d pcall,page,mmu,cpu_reset,guest_errors,page,trace:ps2_put_keycode
