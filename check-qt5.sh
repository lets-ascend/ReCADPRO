#!/bin/bash
# Simple script to check for Qt5 in SDK
# Run this with: ./check-qt5.sh  or  bash check-qt5.sh

echo "=========================================="
echo "Checking for Qt5 in reMarkable SDK"
echo "=========================================="
echo ""

# Check what sysroots exist
echo "Available sysroots:"
ls -la /opt/codex/ferrari/5.2.96-dirty/sysroots/ 2>/dev/null || echo "sysroots directory not found"
echo ""

# Find Qt5Config.cmake
echo "Searching for Qt5Config.cmake..."
QT5_CONFIG=$(find /opt/codex/ferrari/5.2.96-dirty -name Qt5Config.cmake 2>/dev/null | head -1)
if [ ! -z "$QT5_CONFIG" ]; then
    echo "Found: $QT5_CONFIG"
    QT5_PREFIX=$(dirname "$QT5_CONFIG" | xargs dirname | xargs dirname)
    echo "Use CMAKE_PREFIX_PATH: $QT5_PREFIX"
else
    echo "❌ Qt5Config.cmake NOT FOUND"
    echo ""
    echo "Checking for any Qt files..."
    QT_FILES=$(find /opt/codex/ferrari/5.2.96-dirty -type f -name '*Qt*' 2>/dev/null | head -10)
    if [ -z "$QT_FILES" ]; then
        echo "   No Qt files found at all!"
        echo ""
        echo "Checking if qmake exists after sourcing SDK..."
        source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux 2>/dev/null
        if command -v qmake &> /dev/null; then
            echo "   ✓ qmake found: $(which qmake)"
            qmake --version
            echo ""
            echo "   QTDIR: ${QTDIR:-NOT SET}"
        else
            echo "   ❌ qmake also not found!"
        fi
    else
        echo "$QT_FILES"
    fi
    echo ""
    echo "=========================================="
    echo "NEXT STEPS:"
    echo "=========================================="
    echo "Qt5 might not be installed in your SDK."
    echo "Run: ./find-qt5-deep.sh for more details"
fi

