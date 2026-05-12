#!/usr/bin/env bash

set -euo pipefail

readonly DB_HOST="${DB_HOST:-127.0.0.1}"
readonly DB_PORT="${DB_PORT:-5432}"
readonly DB_USER="${DB_USER:-postgres}"
readonly DB_NAME="${DB_NAME:-los_css_drogon}"
readonly DB_PASS="${DB_PASS:-lsj}"

readonly PGPASSWORD="${DB_PASS}"

# PGPASSWORD 这里 导出？
export PGPASSWORD

# 连接 命令
PSQL_CMD="psql -h ${DB_HOST} -p ${DB_PORT} -U ${DB_USER} -d ${DB_NAME} -v ON_ERROR_STOP=1"

# BASH_SOURCE 就是 这个 脚本的 全路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly SCRIPT_DIR

# 声明一个数组
# SQL_FILES
declare -a SQL_FILES=()

# 如果 未指定 SQL 文件
# 就包含 当前目录下所有 .sql 文件
if [ $# -eq 0 ]; then
    echo "[INFO] 未指定 SQL 文件，将包含当前目录下所有 .sql 文件"

    for file in "${SCRIPT_DIR}"/*.sql; do
        if [ -f "${file}" ]; then
            SQL_FILES+=("${file}")
        fi
    done
else
    echo "[INFO] 已指定 SQL 文件，将只执行指定文件"
    for arg in "$@"; do
        if [ -f "${SCRIPT_DIR}/${arg}" ]; then
            SQL_FILES+=("${SCRIPT_DIR}/${arg}")
        elif [ -f "${arg}" ]; then
            SQL_FILES+=("${arg}")
        else
            echo "[ERROR] 文件不存在: ${arg}"
            exit 1
        fi
    done
fi
