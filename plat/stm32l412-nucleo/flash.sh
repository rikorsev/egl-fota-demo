
BINARY=build/main/afs_main.bin
TARGET=stm32l412r8tx

pyocd load --target ${TARGET} ${BINARY}
