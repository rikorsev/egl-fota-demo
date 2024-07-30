TARGET=stm32l412rbtx
ELF=build/main/afs_main.elf

pyocd gdbserver --target ${TARGET} &
gdb-multiarch ${ELF} -ex "target remote localhost:3333"
pkill pyocd