#!/bin/bash
# End-to-end helper for reMarkable Qt builds inside Yocto SDK

set -euo pipefail

SDK_ROOT="/opt/codex/ferrari/5.2.96-dirty"
YOCTO_TRIPLET="cortexa53-crypto-remarkable-linux"
SYSROOT="${SDK_ROOT}/sysroots/${YOCTO_TRIPLET}"
QT_PREFIX="${SYSROOT}/usr"

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${REPO_ROOT}/build"

ENV_FILE="${SDK_ROOT}/environment-setup-${YOCTO_TRIPLET}"

echo "========== reMarkable Qt helper =========="
echo "SDK root        : ${SDK_ROOT}"
echo "Sysroot (Qt)    : ${QT_PREFIX}"
echo "Repo root       : ${REPO_ROOT}"
echo "Build directory : ${BUILD_DIR}"
echo "=========================================="

if [ ! -f "${ENV_FILE}" ]; then
    echo "ERROR: Yocto env file not found at ${ENV_FILE}"
    exit 1
fi

echo "[1/5] Sourcing Yocto environment..."
# shellcheck disable=SC1090
source "${ENV_FILE}"

echo "[2/5] Creating Qt5 CMake config (Yocto)..."
chmod +x "${REPO_ROOT}/create-qt5-config-yocto.sh"
"${REPO_ROOT}/create-qt5-config-yocto.sh"

echo "[3/5] Preparing build directory..."
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

echo "[4/5] Running CMake configure..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="${QT_PREFIX}" \
    -DQT_MOC_EXECUTABLE="${QT_PREFIX}/lib/qt5/bin/moc" \
    -DQT_UIC_EXECUTABLE="${QT_PREFIX}/lib/qt5/bin/uic"

echo "[5/5] Building target..."
cmake --build . -- -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"

echo "=========================================="
echo "Build complete. Binaries in ${BUILD_DIR}"
echo "=========================================="

