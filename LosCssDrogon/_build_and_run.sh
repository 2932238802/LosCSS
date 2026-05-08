#!/usr/bin/env bash
set -euo pipefail

readonly PROJECT_NAME="LosCssDrogon"
readonly BUILD_DIR="_build"
readonly VCPKG_ROOT="/home/losangelous/LosAngelous/dev/vcpkg"

cd "$(dirname "$(readlink -f "$0")")"

readonly C_GREEN='\033[0;32m'
readonly C_YELLOW='\033[0;33m'
readonly C_RED='\033[0;31m'
readonly C_BLUE='\033[0;36m'
readonly C_RESET='\033[0m'

log()  { echo -e "${C_GREEN}[INFO]${C_RESET} $*"; }
warn() { echo -e "${C_YELLOW}[WARN]${C_RESET} $*"; }
err()  { echo -e "${C_RED}[ERR ]${C_RESET} $*" >&2; }
step() { echo -e "\n${C_BLUE}==>${C_RESET} $*"; }

readonly START_TS=$(date +%s)
CONFIG_END_TS=0
BUILD_END_TS=0

on_exit() {
    local code=$?
    local now=$(date +%s)
    echo
    if [[ $code -eq 0 ]]; then
        local total=$(( now - START_TS ))
        log "total duration: ${total}s"
    else
        err "aborted (exit=$code)"
    fi
}
trap on_exit EXIT

readonly TOOLCHAIN="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"

if [[ ! -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
    step "configure (first time, cmake ..)"

    if [[ ! -f "$TOOLCHAIN" ]]; then
        err "vcpkg toolchain not found: $TOOLCHAIN"
        exit 1
    fi

    cmake -S . -B "$BUILD_DIR" \
        -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
        -DCMAKE_BUILD_TYPE=Debug
else
    log "cmake cache exists, skip configure"
fi

CONFIG_END_TS=$(date +%s)
log "configure took $(( CONFIG_END_TS - START_TS ))s"

step "build (cmake --build, $(nproc) jobs)"
cmake --build "$BUILD_DIR" -j"$(nproc)"

BUILD_END_TS=$(date +%s)
log "build took $(( BUILD_END_TS - CONFIG_END_TS ))s"

readonly BIN="${BUILD_DIR}/${PROJECT_NAME}"
if [[ ! -x "$BIN" ]]; then
    err "build succeeded but binary missing: $BIN"
    exit 1
fi

step "run ${PROJECT_NAME}"
log "binary : $(readlink -f "$BIN")"
echo

cd "$BUILD_DIR"
exec "./${PROJECT_NAME}"