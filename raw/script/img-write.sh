export DISK_NAME=disk.dmg
# 每个块都是 512 字节
# 写boot区，定位到磁盘开头，写1个块，由系统自动加载
dd if=boot.bin of=$DISK_NAME bs=512 conv=notrunc count=1

# 写loader区，定位到磁盘第2个块，需要自己加载
dd if=loader.bin of=$DISK_NAME bs=512 conv=notrunc seek=1

# 写kernel区，定位到磁盘第100个块，需要自己加载
# dd if=kernel.elf of=$DISK_NAME bs=512 conv=notrunc seek=100
