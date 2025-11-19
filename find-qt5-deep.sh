#!/bin/bash
# Deep search for Qt5 in SDK

echo "=========================================="
echo "Deep Qt5 Search in reMarkable SDK"
echo "=========================================="
echo ""

SDK_BASE="/opt/codex/ferrari/5.2.96-dirty"

echo "1. Checking sysroots that exist:"
ls -la "$SDK_BASE/sysroots/"
echo ""

echo "2. Searching for Qt5Config.cmake:"
find "$SDK_BASE" -name Qt5Config.cmake 2>/dev/null
echo ""

echo "3. Searching for any Qt5 cmake files:"
find "$SDK_BASE" -name "*Qt5*.cmake" 2>/dev/null | head -10
echo ""

echo "4. Searching for Qt5 libraries:"
find "$SDK_BASE" -name "libQt5*.so*" 2>/dev/null | head -10
echo ""

echo "5. Checking cortexa53 sysroot for Qt:"
if [ -d "$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux" ]; then
    echo "   Checking usr/lib:"
    ls "$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux/usr/lib/" | grep -i qt | head -10
    echo ""
    echo "   Checking usr/lib/cmake:"
    ls "$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux/usr/lib/cmake/" 2>/dev/null | head -10
    echo ""
    echo "   Checking usr/include:"
    ls "$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux/usr/include/" | grep -i qt | head -10
fi
echo ""

echo "6. Checking if qmake exists after sourcing SDK:"
if [ -f "$SDK_BASE/environment-setup-cortexa53-crypto-remarkable-linux" ]; then
    source "$SDK_BASE/environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null
    echo "   QTDIR: ${QTDIR:-NOT SET}"
    echo "   qmake location: $(which qmake 2>/dev/null || echo 'NOT FOUND')"
    if command -v qmake &> /dev/null; then
        qmake --version
        echo ""
        echo "   Qt5 Install Prefix:"
        qmake -query QT_INSTALL_PREFIX 2>/dev/null || echo "   (Cannot query - try using QTDIR or sysroot path)"
    fi
fi
echo ""
echo "=========================================="
echo "WHAT TO DO NEXT:"
echo "=========================================="
echo ""
echo "Run this script to get automatic fix instructions:"
echo "  chmod +x AUTO_FIX_QT5.sh"
echo "  ./AUTO_FIX_QT5.sh"
echo ""
echo "OR see QT5_FIX_STEP_BY_STEP.md for manual instructions."
echo ""

