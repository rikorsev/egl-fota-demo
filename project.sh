CMD=$1
TARGET_PLATFORM=$3

shift 1
case "$CMD" in
    ("build") ./build.sh $@ ;;
    ("flash") ./plat/${TARGET_PLATFORM}/flash.sh $@ ;;
    ("debug") ./plat/${TARGET_PLATFORM}/debug.sh $@ ;;
esac
