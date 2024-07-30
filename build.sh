SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=${SCRIPT_DIR}
OUT_DIR=${ROOT_DIR}/build/main
PROJ_DIR=${ROOT_DIR}/apps/main
TARGET_PLATFORM="unknown"
VERBOSE=""

# Script command parameters
usage() {
    echo "Usage: $0 
    [ -p TARGET_PLATFORM ]
    [ -v VERBOSE ]"
}

while getopts ":p:v" opt; do
    case "$opt" in
        p)
            echo "Target Platform ${OPTARG}"
            TARGET_PLATFORM=${OPTARG}
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

# Configure
cmake -DCMAKE_TOOLCHAIN_FILE="${ROOT_DIR}/plat/${TARGET_PLATFORM}/toolchain.cmake" \
      -DCMAKE_BUILD_TYPE="debug" \
      -DROOT_DIR=${ROOT_DIR} \
      -DTARGET_PLATFORM=${TARGET_PLATFORM} \
      -B ${OUT_DIR} \
      -S ${PROJ_DIR}

# Build
cmake --build ${OUT_DIR} -j 4 ${VERBOSE}
BUILD_STATUS=$?

if [ ${BUILD_STATUS} != 0 ]; then
    echo "Build failed, return status ${BUILD_STATUS}"
fi
exit $BUILD_STATUS