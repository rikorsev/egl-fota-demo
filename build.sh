SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=${SCRIPT_DIR}
TARGET_PLATFORM="unknown"
TARGET_APP="main"
TARGET_SLOT="a"
VERBOSE=""
BUILD_TYPE="release"

# Script command parameters
usage() {
    echo "Usage: $0 
    [ -p TARGET_PLATFORM ]
    [ -a APPLICATION ]
    [ -s SLOT ]
    [ -v VERBOSE ]"
}

while getopts "p:t:a:s:v" opt; do
    case "$opt" in
        p)
            TARGET_PLATFORM=${OPTARG}
            ;;
        t)
            BUILD_TYPE=${OPTARG}
            ;;
        a)
            TARGET_APP=${OPTARG}
            ;;
        s)
            TARGET_SLOT=${OPTARG}
            ;;
        v)
            echo "Verbose mode enabled"
            VERBOSE="-v"
            ;;
        *)
            usage
            exit 1
            ;;
    esac
done

if [[ $TARGET_APP == "boot" ]]; then
    echo "Force TARGET_SLOT to boot"
    TARGET_SLOT="boot"
fi

OUT_DIR=${ROOT_DIR}/build/${TARGET_APP}
PROJ_DIR=${ROOT_DIR}/apps/${TARGET_APP}

# Configure
cmake -DCMAKE_TOOLCHAIN_FILE="${ROOT_DIR}/plat/${TARGET_PLATFORM}/toolchain.cmake" \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DROOT_DIR=${ROOT_DIR} \
      -DTARGET_PLATFORM=${TARGET_PLATFORM} \
      -DTARGET_SLOT=${TARGET_SLOT} \
      -B ${OUT_DIR} \
      -S ${PROJ_DIR}

# Build
cmake --build ${OUT_DIR} -j 4 ${VERBOSE}
BUILD_STATUS=$?

if [ ${BUILD_STATUS} != 0 ]; then
    echo "Build failed, return status ${BUILD_STATUS}"
fi
exit $BUILD_STATUS
