set(CMAKE_ASM_FLAGS "\
    -mcpu=cortex-m0 \
    -mthumb \
    -x \
    assembler-with-cpp \
")

set(CMAKE_C_FLAGS " \
    -mcpu=cortex-m0 \
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
    -mcpu=cortex-m0 \
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
    -T${CMAKE_CURRENT_LIST_DIR}/linker/stm32l073rztx_${TARGET_SLOT}.ld \
    -T${CMAKE_CURRENT_LIST_DIR}/linker/stm32l073rztx_base.ld \
")
