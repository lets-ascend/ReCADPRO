#!/bin/bash
# Script to find Qt5 location in reMarkable SDK

echo "Finding Qt5 in reMarkable SDK..."
echo ""

# Source SDK if path provided
if [ ! -z "$1" ]; then
    echo "Sourcing SDK from: $1"
    source "$1"
    echo ""
fi

echo "Environment variables:"
echo "  QTDIR: ${QTDIR:-NOT SET}"
echo "  OECORE_TARGET_SYSROOT: ${OECORE_TARGET_SYSROOT:-NOT SET}"
echo ""

echo "Searching for Qt5Config.cmake..."
echo ""

# Search common locations
SEARCH_PATHS=(
    "/opt/codex/ferrari"
    "$HOME/remarkable-sdk"
    "/usr/local/remarkable-sdk"
)

for BASE_PATH in "${SEARCH_PATHS[@]}"; do
    if [ -d "$BASE_PATH" ]; then
        echo "Searching in: $BASE_PATH"
        QT5_CONFIG=$(find "$BASE_PATH" -name "Qt5Config.cmake" 2>/dev/null | head -1)
        if [ ! -z "$QT5_CONFIG" ]; then
            echo "  ✓ Found: $QT5_CONFIG"
            # Get the prefix path (parent of lib/cmake/Qt5)
            QT5_PREFIX=$(dirname "$QT5_CONFIG" | xargs dirname | xargs dirname)
            echo "  → Use CMAKE_PREFIX_PATH: $QT5_PREFIX"
            echo ""
            echo "Run cmake with:"
            echo "  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=\"$QT5_PREFIX\""
            exit 0
        fi
    fi
done

echo "❌ Qt5Config.cmake not found!"
echo ""
echo "Try:"
echo "  1. Make sure SDK is installed"
echo "  2. Source SDK: source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux"
echo "  3. Check if Qt5 is installed in SDK"
exit 1

