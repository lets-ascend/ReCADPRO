# 📋 Copy-Paste Scripts Directly

If you can't transfer files, copy-paste these scripts directly into your VM.

## Script 1: create-minimal-qt5-cmake.sh

**In Ubuntu VM terminal:**
```bash
cd ~/remarkable-sdk/recadpro
nano create-minimal-qt5-cmake.sh
```

**Then paste this entire content** (right-click → Paste or Shift+Insert):

```bash
#!/bin/bash
# Create minimal Qt5 CMake configuration for reMarkable SDK

SDK_BASE="/opt/codex/ferrari/5.2.96-dirty"
SYSROOT="$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux"
TARGET_USR="$SYSROOT/usr"

if [ ! -d "$TARGET_USR" ]; then
    echo "❌ ERROR: Sysroot not found"
    exit 1
fi

QT5_CMAKE_DIR="$TARGET_USR/lib/cmake/Qt5"
sudo mkdir -p "$QT5_CMAKE_DIR" || mkdir -p "$QT5_CMAKE_DIR"

cat > "$QT5_CMAKE_DIR/Qt5Config.cmake" << 'EOF'
get_filename_component(_qt5_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
set(QT_VERSION_MAJOR 5)
set(QT_VERSION_MINOR 15)
set(QT_VERSION_PATCH 0)
set(QT_VERSION_STRING "5.15.0")
set(Qt5_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(Qt5_PREFIX_PATH "${_qt5_install_prefix}")
set(Qt5Core_DIR "${Qt5_DIR}/Qt5Core")
set(Qt5Gui_DIR "${Qt5_DIR}/Qt5Gui")
set(Qt5Qml_DIR "${Qt5_DIR}/Qt5Qml")
set(Qt5Quick_DIR "${Qt5_DIR}/Qt5Quick")
set(Qt5QuickControls2_DIR "${Qt5_DIR}/Qt5QuickControls2")
macro(_qt5_find_library _name _lib)
    find_library(${_name} NAMES ${_lib} PATHS "${Qt5_PREFIX_PATH}/lib" NO_DEFAULT_PATH)
endmacro()
_qt5_find_library(Qt5Core_LIBRARY Qt5Core)
_qt5_find_library(Qt5Gui_LIBRARY Qt5Gui)
_qt5_find_library(Qt5Qml_LIBRARY Qt5Qml)
_qt5_find_library(Qt5Quick_LIBRARY Qt5Quick)
if(Qt5Core_LIBRARY)
    set(Qt5_FOUND TRUE)
endif()
EOF

mkdir -p "$QT5_CMAKE_DIR/Qt5Core"
cat > "$QT5_CMAKE_DIR/Qt5Core/Qt5CoreConfig.cmake" << EOF
set(Qt5Core_FOUND TRUE)
set(Qt5Core_VERSION "5.15.0")
find_library(Qt5Core_LIBRARY NAMES Qt5Core PATHS "\${Qt5_PREFIX_PATH}/lib" NO_DEFAULT_PATH)
if(Qt5Core_LIBRARY)
    add_library(Qt5::Core SHARED IMPORTED)
    set_target_properties(Qt5::Core PROPERTIES IMPORTED_LOCATION "\${Qt5Core_LIBRARY}")
endif()
EOF

echo "✅ Created Qt5Config.cmake at: $QT5_CMAKE_DIR"
echo ""
echo "Now run:"
echo "cd ~/remarkable-sdk/recadpro/build"
echo "cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=\"$TARGET_USR\""
```

**Save**: Press `Ctrl+X`, then `Y`, then `Enter`

**Make executable:**
```bash
chmod +x create-minimal-qt5-cmake.sh
```

**Run it:**
```bash
sudo ./create-minimal-qt5-cmake.sh
```

