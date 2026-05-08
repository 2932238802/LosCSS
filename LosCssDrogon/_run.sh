#!/usr/bin/env bash

# 
set -euo pipefail

readonly PROJECT_NAME="LosCssDrogon"
readonly BUILD_DIR="_build"

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

on_exit() {
    local code=$?
    local dur=$(( $(date +%s) - START_TS ))
    if [[ $code -eq 0 ]]; then
        log "done in ${dur}s"
    else
        err "failed (exit=$code) in ${dur}s"
    fi
}

trap on_exit EXIT

readonly BIN="${BUILD_DIR}/${PROJECT_NAME}"
step "check binary"
if [[ ! -x "$BIN" ]]; then
    err "executable not found: $BIN"
    warn "please run ./_build_and_run.sh first (or ./_re_build.sh)"
    exit 1
fi
step "running ${PROJECT_NAME}"
log "binary : $(readlink -f "$BIN")"
log "pwd    : $(pwd)/${BUILD_DIR}"
echo
cd "$BUILD_DIR"
exec "./${PROJECT_NAME}"