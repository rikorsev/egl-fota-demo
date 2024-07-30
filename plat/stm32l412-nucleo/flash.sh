
TARGET_APP="main"
TARGET_SLOT="a"
TARGET_ADDR=0x08010000

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

# Choose SLOT addr
if [[ $TARGET_SLOT == "a" ]]; then
    TARGET_ADDR=0x08010000
elif [[ $TARGET_SLOT == "b" ]]; then
    TARGET_ADDR=0x08020000
elif [[ $TARGET_SLOT == "boot" ]]; then
    TARGET_ADDR=0x08000000
else
    echo "Incorrect slot"
    exit 1
fi

TARGET_BINARY=build/${TARGET_APP}/afd_${TARGET_APP}.${TARGET_SLOT}.bin
TARGET_MCU=stm32l412rbtx

pyocd load --target ${TARGET_MCU} -a ${TARGET_ADDR} --format bin ${TARGET_BINARY}
