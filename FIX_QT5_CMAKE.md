# 🔧 Fix Qt5 CMake Error

## Problem
CMake can't find Qt5 even though SDK is sourced:
```
CMake Error: Could not find a package configuration file provided by "Qt5"
```

## Solution

### Step 1: Find Qt5 Location

After sourcing SDK, find where Qt5Config.cmake is:

```bash
# Source SDK first
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# Check QTDIR
echo $QTDIR
# Should show: /opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr

# Find Qt5Config.cmake
find /opt/codex/ferrari/5.2.96-dirty -name "Qt5Config.cmake" 2>/dev/null
# Should show: /opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr/lib/cmake/Qt5/Qt5Config.cmake
```

### Step 2: Run CMake with CMAKE_PREFIX_PATH

```bash
cd ~/Remarkable_app/build

# Option A: Use QTDIR (if set by SDK)
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$QTDIR"

# Option B: Use explicit path (if QTDIR not set)
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="/opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr"

# Option C: Find and use automatically
QT5_PATH=$(find /opt/codex/ferrari/5.2.96-dirty -name "Qt5Config.cmake" 2>/dev/null | head -1 | xargs dirname | xargs dirname | xargs dirname)
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$QT5_PATH"
```

### Step 3: Verify It Works

CMake should now find Qt5:
```
-- Found Qt5: /opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr (found version "5.x.x")
-- Configuring done
-- Generating done
```

## Quick Fix Script

Create a file `build.sh`:

```bash
#!/bin/bash
# Build script for ReCADPro

# Source SDK
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# Set Qt5 path
export CMAKE_PREFIX_PATH="$QTDIR"

# Build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$QTDIR"
make -j$(nproc)
```

Make it executable:
```bash
chmod +x build.sh
./build.sh
```

## Alternative: Update CMakeLists.txt

The CMakeLists.txt has been updated to automatically use QTDIR if available. But you still need to source SDK before running cmake.

