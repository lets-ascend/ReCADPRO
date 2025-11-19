#!/bin/bash
# Automatic Qt5 fix script - runs deep search and tells you what to do

echo "=========================================="
echo "Automatic Qt5 Fix Script"
echo "=========================================="
echo ""

SDK_BASE="/opt/codex/ferrari/5.2.96-dirty"
BUILD_DIR="$HOME/remarkable-sdk/recadpro/build"

# Step 1: Find Qt5Config.cmake
echo "Step 1: Searching for Qt5Config.cmake..."
QT5_CONFIG=$(find "$SDK_BASE" -name Qt5Config.cmake 2>/dev/null | head -1)

if [ ! -z "$QT5_CONFIG" ]; then
    echo "✅ FOUND: $QT5_CONFIG"
    QT5_PREFIX=$(dirname "$QT5_CONFIG" | xargs dirname | xargs dirname)
    echo ""
    echo "=========================================="
    echo "SOLUTION: Run cmake with this command:"
    echo "=========================================="
    echo ""
    echo "cd $BUILD_DIR"
    echo "cmake .. \\"
    echo "    -DCMAKE_BUILD_TYPE=Release \\"
    echo "    -DCMAKE_PREFIX_PATH=\"$QT5_PREFIX\""
    echo ""
    exit 0
fi

echo "❌ Qt5Config.cmake NOT FOUND"
echo ""

# Step 2: Check if qmake exists
echo "Step 2: Checking for qmake..."
if [ -f "$SDK_BASE/environment-setup-cortexa53-crypto-remarkable-linux" ]; then
    source "$SDK_BASE/environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null
    
    if command -v qmake &> /dev/null; then
        echo "✅ qmake found: $(which qmake)"
        QT5_PREFIX=$(qmake -query QT_INSTALL_PREFIX 2>/dev/null)
        
        if [ ! -z "$QT5_PREFIX" ]; then
            echo "✅ Qt5 prefix: $QT5_PREFIX"
            echo ""
            echo "=========================================="
            echo "SOLUTION: Run cmake with this command:"
            echo "=========================================="
            echo ""
            echo "source $SDK_BASE/environment-setup-cortexa53-crypto-remarkable-linux"
            echo "cd $BUILD_DIR"
            echo "cmake .. \\"
            echo "    -DCMAKE_BUILD_TYPE=Release \\"
            echo "    -DCMAKE_PREFIX_PATH=\"$QT5_PREFIX\""
            echo ""
            exit 0
        fi
    fi
fi

echo "❌ qmake also NOT FOUND"
echo ""

# Step 3: Check sysroots
echo "Step 3: Checking sysroots..."
if [ -d "$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux/usr" ]; then
    echo "✅ Sysroot exists: cortexa53-crypto-remarkable-linux"
    SYSROOT_USR="$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux/usr"
    
    # Check if Qt5 libraries exist
    if [ -d "$SYSROOT_USR/lib" ] && ls "$SYSROOT_USR/lib"/libQt5*.so* 2>/dev/null | head -1 > /dev/null; then
        echo "✅ Qt5 libraries found in sysroot"
        echo ""
        echo "=========================================="
        echo "SOLUTION: Try cmake with sysroot path:"
        echo "=========================================="
        echo ""
        echo "cd $BUILD_DIR"
        echo "cmake .. \\"
        echo "    -DCMAKE_BUILD_TYPE=Release \\"
        echo "    -DCMAKE_PREFIX_PATH=\"$SYSROOT_USR\""
        echo ""
        exit 0
    fi
fi

# Step 4: Nothing found - but check if Qt5 libraries exist
echo "Step 4: Checking for Qt5 libraries..."
if [ -d "$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux/usr/lib" ]; then
    QT5_LIBS=$(ls "$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux/usr/lib"/libQt5*.so* 2>/dev/null | head -3)
    if [ ! -z "$QT5_LIBS" ]; then
        echo "✅ Qt5 libraries found but Qt5Config.cmake is missing!"
        echo ""
        echo "=========================================="
        echo "SOLUTION: Create Qt5Config.cmake"
        echo "=========================================="
        echo ""
        echo "Qt5 is installed but missing CMake config file."
        echo "Run this script to create it:"
        echo ""
        echo "  chmod +x create-qt5-config.sh"
        echo "  ./create-qt5-config.sh"
        echo ""
        echo "Then run cmake with:"
        echo "  cd $BUILD_DIR"
        echo "  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=\"$SYSROOT_USR\""
        echo ""
        exit 0
    fi
fi

# Step 5: Nothing found at all
echo "=========================================="
echo "❌ PROBLEM: Qt5 Not Found in SDK"
echo "=========================================="
echo ""
echo "Your SDK doesn't appear to have Qt5 installed."
echo ""
echo "POSSIBLE SOLUTIONS:"
echo "1. Check reMarkable SDK documentation for Qt5 installation"
echo "2. Verify SDK installation completed successfully"
echo "3. Check if Qt5 needs to be installed separately"
echo "4. Try a different SDK version that includes Qt5"
echo ""
echo "Run './find-qt5-deep.sh' for detailed information."
echo ""

