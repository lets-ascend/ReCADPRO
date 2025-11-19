#!/bin/bash
# Create Qt5Config.cmake if Qt5 is installed but config file is missing

echo "=========================================="
echo "Creating Qt5 CMake Configuration"
echo "=========================================="
echo ""

SDK_BASE="/opt/codex/ferrari/5.2.96-dirty"
SYSROOT="$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux"

# Source SDK to get environment
if [ -f "$SDK_BASE/environment-setup-cortexa53-crypto-remarkable-linux" ]; then
    source "$SDK_BASE/environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null
fi

# Find Qt5 installation
QT5_PREFIX=""
QT5_LIB_DIR=""

# Method 1: Check QTDIR
if [ ! -z "$QTDIR" ]; then
    QT5_PREFIX="$QTDIR"
    echo "Found QTDIR: $QTDIR"
fi

# Method 2: Check sysroot
if [ -z "$QT5_PREFIX" ] && [ -d "$SYSROOT/usr/lib" ]; then
    # Check if Qt5 libraries exist
    if ls "$SYSROOT/usr/lib"/libQt5*.so* 2>/dev/null | head -1 > /dev/null; then
        QT5_PREFIX="$SYSROOT/usr"
        echo "Found Qt5 libraries in sysroot: $QT5_PREFIX"
    fi
fi

# Method 3: Use qmake to find Qt5
if [ -z "$QT5_PREFIX" ] && command -v qmake &> /dev/null; then
    QT5_PREFIX=$(qmake -query QT_INSTALL_PREFIX 2>/dev/null)
    if [ ! -z "$QT5_PREFIX" ]; then
        echo "Found Qt5 via qmake: $QT5_PREFIX"
    fi
fi

if [ -z "$QT5_PREFIX" ]; then
    echo "❌ ERROR: Cannot find Qt5 installation!"
    echo ""
    echo "Qt5 doesn't appear to be installed in your SDK."
    echo ""
    echo "SOLUTIONS:"
    echo "1. Check reMarkable SDK documentation for Qt5 installation"
    echo "2. Verify SDK installation completed successfully"
    echo "3. Qt5 might need to be installed separately"
    echo ""
    exit 1
fi

QT5_LIB_DIR="$QT5_PREFIX/lib"
QT5_CMAKE_DIR="$QT5_PREFIX/lib/cmake/Qt5"

echo ""
echo "Qt5 Prefix: $QT5_PREFIX"
echo "Qt5 Lib Dir: $QT5_LIB_DIR"
echo ""

# Check if cmake directory exists
if [ ! -d "$QT5_CMAKE_DIR" ]; then
    echo "Creating Qt5 cmake directory..."
    sudo mkdir -p "$QT5_CMAKE_DIR" 2>/dev/null || mkdir -p "$QT5_CMAKE_DIR"
fi

# Create Qt5Config.cmake
echo "Creating Qt5Config.cmake..."

QT5_VERSION=$(qmake -query QT_VERSION 2>/dev/null || echo "5.15.0")

cat > "$QT5_CMAKE_DIR/Qt5Config.cmake" << EOF
# Qt5 CMake Configuration
# Auto-generated for reMarkable SDK

set(QT_VERSION_MAJOR 5)
set(QT_VERSION_MINOR 15)
set(QT_VERSION_PATCH 0)
set(QT_VERSION_STRING "$QT5_VERSION")

set(Qt5_DIR "\${CMAKE_CURRENT_LIST_DIR}")
set(Qt5Core_DIR "\${Qt5_DIR}/Qt5Core")
set(Qt5Gui_DIR "\${Qt5_DIR}/Qt5Gui")
set(Qt5Qml_DIR "\${Qt5_DIR}/Qt5Qml")
set(Qt5Quick_DIR "\${Qt5_DIR}/Qt5Quick")
set(Qt5QuickControls2_DIR "\${Qt5_DIR}/Qt5QuickControls2")

# Set installation prefix
set(Qt5_PREFIX_PATH "$QT5_PREFIX")

# Include component configs if they exist
if(EXISTS "\${Qt5Core_DIR}/Qt5CoreConfig.cmake")
    include("\${Qt5Core_DIR}/Qt5CoreConfig.cmake")
endif()
if(EXISTS "\${Qt5Gui_DIR}/Qt5GuiConfig.cmake")
    include("\${Qt5Gui_DIR}/Qt5GuiConfig.cmake")
endif()
if(EXISTS "\${Qt5Qml_DIR}/Qt5QmlConfig.cmake")
    include("\${Qt5Qml_DIR}/Qt5QmlConfig.cmake")
endif()
if(EXISTS "\${Qt5Quick_DIR}/Qt5QuickConfig.cmake")
    include("\${Qt5Quick_DIR}/Qt5QuickConfig.cmake")
endif()
if(EXISTS "\${Qt5QuickControls2_DIR}/Qt5QuickControls2Config.cmake")
    include("\${Qt5QuickControls2_DIR}/Qt5QuickControls2Config.cmake")
endif()

# Find Qt5 libraries
find_library(Qt5Core_LIBRARY
    NAMES Qt5Core
    PATHS "\${Qt5_PREFIX_PATH}/lib"
    NO_DEFAULT_PATH
)

find_library(Qt5Gui_LIBRARY
    NAMES Qt5Gui
    PATHS "\${Qt5_PREFIX_PATH}/lib"
    NO_DEFAULT_PATH
)

find_library(Qt5Qml_LIBRARY
    NAMES Qt5Qml
    PATHS "\${Qt5_PREFIX_PATH}/lib"
    NO_DEFAULT_PATH
)

find_library(Qt5Quick_LIBRARY
    NAMES Qt5Quick
    PATHS "\${Qt5_PREFIX_PATH}/lib"
    NO_DEFAULT_PATH
)

find_library(Qt5QuickControls2_LIBRARY
    NAMES Qt5QuickControls2
    PATHS "\${Qt5_PREFIX_PATH}/lib"
    NO_DEFAULT_PATH
)

# Set found flag
set(Qt5_FOUND TRUE)
EOF

if [ $? -eq 0 ]; then
    echo "✅ Created: $QT5_CMAKE_DIR/Qt5Config.cmake"
    echo ""
    echo "=========================================="
    echo "NEXT STEP: Run cmake with:"
    echo "=========================================="
    echo ""
    echo "cd ~/remarkable-sdk/recadpro/build"
    echo "cmake .. \\"
    echo "    -DCMAKE_BUILD_TYPE=Release \\"
    echo "    -DCMAKE_PREFIX_PATH=\"$QT5_PREFIX\""
    echo ""
else
    echo "❌ ERROR: Failed to create Qt5Config.cmake"
    echo "You may need to run with sudo or check permissions"
    exit 1
fi

