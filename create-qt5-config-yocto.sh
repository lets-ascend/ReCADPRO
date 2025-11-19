#!/bin/bash
# Create a Yocto-friendly Qt5Config.cmake for the reMarkable SDK
# This script inspects the Yocto SDK sysroot (OECORE_TARGET_SYSROOT) and
# writes a minimal but functional Qt5 package config so CMake can locate Qt5.

set -euo pipefail

echo "=========================================="
echo "Yocto Qt5 CMake Config Generator"
echo "=========================================="

SDK_ROOT_DEFAULT="/opt/codex/ferrari/5.2.96-dirty"
YOCTO_TRIPLET_DEFAULT="cortexa53-crypto-remarkable-linux"

SDK_ROOT="${SDK_ROOT:-$SDK_ROOT_DEFAULT}"
YOCTO_TRIPLET="${YOCTO_TRIPLET:-$YOCTO_TRIPLET_DEFAULT}"
ENV_FILE="${SDK_ROOT}/environment-setup-${YOCTO_TRIPLET}"

if [ -f "$ENV_FILE" ]; then
    echo "Sourcing Yocto environment: $ENV_FILE"
    # shellcheck disable=SC1090
    source "$ENV_FILE"
else
    echo "⚠️  Yocto environment file not found at $ENV_FILE"
    echo "    Continuing with defaults. Export SDK_ROOT/YOCTO_TRIPLET to override."
fi

if [ -z "${OECORE_TARGET_SYSROOT:-}" ]; then
    OECORE_TARGET_SYSROOT="${SDK_ROOT}/sysroots/${YOCTO_TRIPLET}"
    echo "Using default OECORE_TARGET_SYSROOT: $OECORE_TARGET_SYSROOT"
else
    echo "Detected OECORE_TARGET_SYSROOT: $OECORE_TARGET_SYSROOT"
fi

TARGET_USR="${OECORE_TARGET_SYSROOT}/usr"
if [ ! -d "$TARGET_USR" ]; then
    echo "❌ ERROR: Target sysroot not found at $TARGET_USR"
    echo "Set OECORE_TARGET_SYSROOT or SDK_ROOT/YOCTO_TRIPLET and retry."
    exit 1
fi

QMAKE_BIN="${OE_QMAKE_QMAKE:-}"
if [ -z "$QMAKE_BIN" ] && command -v qmake >/dev/null 2>&1; then
    QMAKE_BIN="$(command -v qmake)"
fi

QT_PREFIX=""
QT_VERSION=""

if [ -n "$QMAKE_BIN" ]; then
    echo "Using qmake: $QMAKE_BIN"
    QT_PREFIX="$("$QMAKE_BIN" -query QT_INSTALL_PREFIX 2>/dev/null || true)"
    QT_VERSION="$("$QMAKE_BIN" -query QT_VERSION 2>/dev/null || true)"
fi

if [ -z "$QT_PREFIX" ]; then
    QT_PREFIX="$TARGET_USR"
    echo "Falling back to sysroot prefix: $QT_PREFIX"
else
    echo "Detected Qt prefix from qmake: $QT_PREFIX"
fi

if [ -z "$QT_VERSION" ]; then
    QT_VERSION="5.15.0"
    echo "Using default Qt version: $QT_VERSION"
else
    echo "Detected Qt version: $QT_VERSION"
fi

QT_MAJOR="${QT_VERSION%%.*}"
QT_MINOR_PATCH="${QT_VERSION#*.}"
QT_MINOR="${QT_MINOR_PATCH%%.*}"
QT_PATCH="${QT_VERSION##*.}"

QT_CMAKE_DIR="${QT_PREFIX}/lib/cmake/Qt5"
echo "Writing config to: $QT_CMAKE_DIR"

if ! sudo mkdir -p "$QT_CMAKE_DIR" 2>/dev/null; then
    mkdir -p "$QT_CMAKE_DIR"
fi

COMPONENTS=(Core Gui Qml Quick QuickControls2 Network Svg Widgets)

write_main_config() {
    cat > "${QT_CMAKE_DIR}/Qt5Config.cmake" <<EOF
# Auto-generated Qt5Config.cmake for Yocto SDK
set(QT_VERSION_MAJOR ${QT_MAJOR})
set(QT_VERSION_MINOR ${QT_MINOR})
set(QT_VERSION_PATCH ${QT_PATCH})
set(QT_VERSION "${QT_VERSION}")

set(Qt5_DIR "\${CMAKE_CURRENT_LIST_DIR}")
set(Qt5_PREFIX_PATH "${QT_PREFIX}")
set(Qt5_LIB_DIR "\${Qt5_PREFIX_PATH}/lib")
set(Qt5_INCLUDE_DIR "\${Qt5_PREFIX_PATH}/include")
set(Qt5_FOUND TRUE)

foreach(_qt_component IN ITEMS ${COMPONENTS[@]})
    set(Qt5\${_qt_component}_DIR "\${Qt5_DIR}/Qt5\${_qt_component}")
    if(EXISTS "\${Qt5\${_qt_component}_DIR}/Qt5\${_qt_component}Config.cmake")
        include("\${Qt5\${_qt_component}_DIR}/Qt5\${_qt_component}Config.cmake")
    endif()
endforeach()
EOF

    cat > "${QT_CMAKE_DIR}/Qt5ConfigVersion.cmake" <<EOF
set(PACKAGE_VERSION "${QT_VERSION}")
if(PACKAGE_FIND_VERSION STREQUAL PACKAGE_VERSION)
    set(PACKAGE_VERSION_EXACT TRUE)
endif()
if(PACKAGE_FIND_VERSION_MAJOR EQUAL ${QT_MAJOR})
    set(PACKAGE_VERSION_COMPATIBLE TRUE)
endif()
EOF
}

write_component_config() {
    local component="\$1"
    local libname="libQt5\${component}.so"
    local component_dir="\${QT_CMAKE_DIR}/Qt5\${component}"

    mkdir -p "\$component_dir"

    cat > "\${component_dir}/Qt5\${component}Config.cmake" <<EOF
include_guard()

set(Qt5${component}_FOUND TRUE)
set(Qt5${component}_VERSION "${QT_VERSION}")
set(Qt5${component}_LIBRARIES Qt5::${component})
set(Qt5${component}_INCLUDE_DIRS
    "\${Qt5_INCLUDE_DIR}"
    "\${Qt5_INCLUDE_DIR}/Qt${component}"
)

find_library(Qt5${component}_LIBRARY
    NAMES Qt5${component}
    PATHS "\${Qt5_LIB_DIR}"
    NO_DEFAULT_PATH
)

if(Qt5${component}_LIBRARY AND NOT TARGET Qt5::${component})
    add_library(Qt5::${component} SHARED IMPORTED)
    set_target_properties(Qt5::${component} PROPERTIES
        IMPORTED_LOCATION "\${Qt5${component}_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "\${Qt5${component}_INCLUDE_DIRS}"
    )
endif()
EOF
}

write_main_config

for component in "${COMPONENTS[@]}"; do
    write_component_config "$component"
done

if [ -f "${QT_CMAKE_DIR}/Qt5Config.cmake" ]; then
    echo "✅ Generated Qt5 CMake config at ${QT_CMAKE_DIR}"
    echo ""
    echo "Next steps:"
    echo "  export CMAKE_PREFIX_PATH=\"${QT_PREFIX}\""
    echo "  cmake .. -DCMAKE_PREFIX_PATH=\"${QT_PREFIX}\""
else
    echo "❌ Failed to create Qt5Config.cmake"
    exit 1
fi

