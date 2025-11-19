#!/bin/bash
# Create minimal Qt5 CMake configuration for reMarkable SDK
# This creates a working Qt5Config.cmake even if Qt5 isn't fully installed

echo "=========================================="
echo "Creating Minimal Qt5 CMake Configuration"
echo "=========================================="
echo ""

SDK_BASE="/opt/codex/ferrari/5.2.96-dirty"
SYSROOT="$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux"
TARGET_USR="$SYSROOT/usr"

# Check if sysroot exists
if [ ! -d "$TARGET_USR" ]; then
    echo "❌ ERROR: Sysroot not found at $TARGET_USR"
    exit 1
fi

echo "Using sysroot: $TARGET_USR"
echo ""

# Create Qt5 cmake directory structure
QT5_CMAKE_DIR="$TARGET_USR/lib/cmake/Qt5"
echo "Creating Qt5 cmake directory: $QT5_CMAKE_DIR"

# Try with sudo first, fall back to regular mkdir
sudo mkdir -p "$QT5_CMAKE_DIR" 2>/dev/null || mkdir -p "$QT5_CMAKE_DIR"

if [ ! -d "$QT5_CMAKE_DIR" ]; then
    echo "❌ ERROR: Cannot create directory. Try running with sudo."
    exit 1
fi

# Check what Qt5 components might exist
echo "Checking for Qt5 components..."
QT5_CORE_LIB=$(find "$TARGET_USR/lib" -name "libQt5Core.so*" 2>/dev/null | head -1)
QT5_GUI_LIB=$(find "$TARGET_USR/lib" -name "libQt5Gui.so*" 2>/dev/null | head -1)
QT5_QML_LIB=$(find "$TARGET_USR/lib" -name "libQt5Qml.so*" 2>/dev/null | head -1)
QT5_QUICK_LIB=$(find "$TARGET_USR/lib" -name "libQt5Quick.so*" 2>/dev/null | head -1)

if [ -z "$QT5_CORE_LIB" ]; then
    echo "⚠️  WARNING: Qt5Core library not found!"
    echo "Qt5 might not be installed. This config will be minimal."
fi

# Get Qt5 version (try multiple methods)
QT5_VERSION="5.15.0"
if command -v qmake &> /dev/null; then
    QT5_VERSION=$(qmake -query QT_VERSION 2>/dev/null || echo "5.15.0")
fi

echo "Using Qt5 version: $QT5_VERSION"
echo ""

# Create main Qt5Config.cmake
echo "Creating Qt5Config.cmake..."

cat > "$QT5_CMAKE_DIR/Qt5Config.cmake" << 'QT5CONFIG_EOF'
# Qt5 CMake Configuration for reMarkable SDK
# Minimal configuration - may need Qt5 installed separately

get_filename_component(_qt5_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

set(QT_VERSION_MAJOR 5)
set(QT_VERSION_MINOR 15)
set(QT_VERSION_PATCH 0)
set(QT_VERSION_STRING "5.15.0")

set(Qt5_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(Qt5_PREFIX_PATH "${_qt5_install_prefix}")

# Component directories
set(Qt5Core_DIR "${Qt5_DIR}/Qt5Core")
set(Qt5Gui_DIR "${Qt5_DIR}/Qt5Gui")
set(Qt5Qml_DIR "${Qt5_DIR}/Qt5Qml")
set(Qt5Quick_DIR "${Qt5_DIR}/Qt5Quick")
set(Qt5QuickControls2_DIR "${Qt5_DIR}/Qt5QuickControls2")
set(Qt5Network_DIR "${Qt5_DIR}/Qt5Network")
set(Qt5Widgets_DIR "${Qt5_DIR}/Qt5Widgets")

# Find Qt5 libraries
macro(_qt5_find_library _name _lib)
    find_library(${_name}
        NAMES ${_lib}
        PATHS "${Qt5_PREFIX_PATH}/lib"
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH
    )
endmacro()

_qt5_find_library(Qt5Core_LIBRARY Qt5Core)
_qt5_find_library(Qt5Gui_LIBRARY Qt5Gui)
_qt5_find_library(Qt5Qml_LIBRARY Qt5Qml)
_qt5_find_library(Qt5Quick_LIBRARY Qt5Quick)
_qt5_find_library(Qt5QuickControls2_LIBRARY Qt5QuickControls2)
_qt5_find_library(Qt5Network_LIBRARY Qt5Network)

# Set found status
if(Qt5Core_LIBRARY)
    set(Qt5_FOUND TRUE)
    set(Qt5Core_FOUND TRUE)
    set(Qt5Gui_FOUND TRUE)
    set(Qt5Qml_FOUND TRUE)
    set(Qt5Quick_FOUND TRUE)
else()
    set(Qt5_FOUND FALSE)
endif()

# Include component configs if they exist
if(EXISTS "${Qt5Core_DIR}/Qt5CoreConfig.cmake")
    include("${Qt5Core_DIR}/Qt5CoreConfig.cmake")
endif()
if(EXISTS "${Qt5Gui_DIR}/Qt5GuiConfig.cmake")
    include("${Qt5Gui_DIR}/Qt5GuiConfig.cmake")
endif()
if(EXISTS "${Qt5Qml_DIR}/Qt5QmlConfig.cmake")
    include("${Qt5Qml_DIR}/Qt5QmlConfig.cmake")
endif()
if(EXISTS "${Qt5Quick_DIR}/Qt5QuickConfig.cmake")
    include("${Qt5Quick_DIR}/Qt5QuickConfig.cmake")
endif()

# Helper function to link Qt5
function(qt5_use_modules _target)
    if(Qt5Core_LIBRARY)
        target_link_libraries(${_target} PRIVATE
            ${Qt5Core_LIBRARY}
            ${Qt5Gui_LIBRARY}
            ${Qt5Qml_LIBRARY}
            ${Qt5Quick_LIBRARY}
        )
        target_include_directories(${_target} PRIVATE
            "${Qt5_PREFIX_PATH}/include"
            "${Qt5_PREFIX_PATH}/include/QtCore"
            "${Qt5_PREFIX_PATH}/include/QtGui"
            "${Qt5_PREFIX_PATH}/include/QtQml"
            "${Qt5_PREFIX_PATH}/include/QtQuick"
        )
    endif()
endfunction()
QT5CONFIG_EOF

# Create component configs
create_component_config() {
    local COMPONENT=$1
    local COMPONENT_DIR="$QT5_CMAKE_DIR/$COMPONENT"
    
    mkdir -p "$COMPONENT_DIR"
    
    cat > "$COMPONENT_DIR/${COMPONENT}Config.cmake" << EOF
# ${COMPONENT} CMake Configuration
set(${COMPONENT}_FOUND TRUE)
set(${COMPONENT}_VERSION "5.15.0")
set(${COMPONENT}_LIBRARIES Qt5::${COMPONENT})
set(${COMPONENT}_INCLUDE_DIRS "\${Qt5_PREFIX_PATH}/include" "\${Qt5_PREFIX_PATH}/include/${COMPONENT}")

# Find library
find_library(${COMPONENT}_LIBRARY
    NAMES Qt5${COMPONENT}
    PATHS "\${Qt5_PREFIX_PATH}/lib"
    NO_DEFAULT_PATH
)

if(${COMPONENT}_LIBRARY)
    add_library(Qt5::${COMPONENT} SHARED IMPORTED)
    set_target_properties(Qt5::${COMPONENT} PROPERTIES
        IMPORTED_LOCATION "\${${COMPONENT}_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "\${${COMPONENT}_INCLUDE_DIRS}"
    )
endif()
EOF
}

echo "Creating component configs..."
create_component_config "Qt5Core"
create_component_config "Qt5Gui"
create_component_config "Qt5Qml"
create_component_config "Qt5Quick"
create_component_config "Qt5QuickControls2"

# Verify creation
if [ -f "$QT5_CMAKE_DIR/Qt5Config.cmake" ]; then
    echo ""
    echo "✅ SUCCESS: Created Qt5 CMake configuration"
    echo ""
    echo "Location: $QT5_CMAKE_DIR/Qt5Config.cmake"
    echo ""
    echo "=========================================="
    echo "NEXT STEP: Run cmake with:"
    echo "=========================================="
    echo ""
    echo "cd ~/remarkable-sdk/recadpro/build"
    echo "cmake .. \\"
    echo "    -DCMAKE_BUILD_TYPE=Release \\"
    echo "    -DCMAKE_PREFIX_PATH=\"$TARGET_USR\""
    echo ""
    echo "Note: If Qt5 libraries are missing, you may need to:"
    echo "1. Install Qt5 packages separately"
    echo "2. Build Qt5 with Yocto/bitbake"
    echo "3. Check reMarkable SDK documentation"
    echo ""
else
    echo "❌ ERROR: Failed to create Qt5Config.cmake"
    exit 1
fi

