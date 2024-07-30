TARGET_MCU=stm32l412rbtx
TARGET_SLOT="a"

while getopts "p:a:s:" opt; do
    case "$opt" in
        a)
            TARGET_APP=${OPTARG}
            ;;
        s)
            TARGET_SLOT=${OTPARG}
            ;;
    esac
done

if [[ $TARGET_APP == "boot" ]]; then
    TARGET_SLOT="boot"
fi

ELF=build/${TARGET_APP}/afd_${TARGET_APP}.${TARGET_SLOT}.elf

pyocd gdbserver --target ${TARGET_MCU} &
gdb-multiarch ${ELF} -ex "target remote localhost:3333"
pkill pyocd