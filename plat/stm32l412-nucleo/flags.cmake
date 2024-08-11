# Assembler flags 
set(CMAKE_ASM_FLAGS "\
    -mcpu=cortex-m4 \
    -mthumb \
    -x \
    assembler-with-cpp \
")

# C++ flags
set(CMAKE_C_FLAGS " \
    -mcpu=cortex-m4 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -fno-strict-aliasing \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -fno-exceptions \
    -mapcs \
    -std=gnu99 \
")

set(CMAKE_EXE_LINKER_FLAGS "\
    -mcpu=cortex-m4 \
    -Wall \
    -specs=nosys.specs \
    -specs=nano.specs \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -fno-exceptions \
    -mthumb \
    -mapcs \
    -Xlinker --gc-sections \
    -Xlinker -static \
    -Xlinker -z \
    -Xlinker muldefs \
    -Xlinker -Map=output.map \
    -Wl,--print-memory-usage \
    -T${ROOT_DIR}/plat/${TARGET_PLATFORM}/stm32l412rbtx_${TARGET_SLOT}.ld \
    -T${ROOT_DIR}/plat/${TARGET_PLATFORM}/stm32l412rbtx_base.ld \
")

add_definitions(-DSTM32L412xx)
