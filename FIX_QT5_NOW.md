# 🔧 Fix Qt5 Error - Run These Commands NOW

## Step 1: Find Qt5 Location

Run these commands in your terminal:

```bash
# Source SDK
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# Find Qt5Config.cmake
find /opt/codex/ferrari/5.2.96-dirty -name "Qt5Config.cmake" 2>/dev/null
```

**Copy the full path** that gets printed (it will look like):
```
/opt/codex/ferrari/5.2.96-dirty/sysroots/[something]/usr/lib/cmake/Qt5/Qt5Config.cmake
```

## Step 2: Extract the Prefix Path

From the path above, extract the prefix (remove `/lib/cmake/Qt5/Qt5Config.cmake`):

**Example:**
- If found: `/opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr/lib/cmake/Qt5/Qt5Config.cmake`
- Use prefix: `/opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr`

## Step 3: Run CMake with Correct Path

```bash
cd ~/remarkable-sdk/recadpro/build

# Replace QT5_PREFIX_PATH with the path from Step 2
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="/opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr"
```

## OR: Use This One-Liner

```bash
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux && \
QT5_PATH=$(find /opt/codex/ferrari/5.2.96-dirty -name "Qt5Config.cmake" 2>/dev/null | head -1) && \
QT5_PREFIX=$(dirname "$QT5_PATH" | xargs dirname | xargs dirname) && \
cd ~/remarkable-sdk/recadpro/build && \
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$QT5_PREFIX"
```

## If Qt5 Still Not Found

Check if Qt5 is actually installed in SDK:

```bash
# Check what sysroots exist
ls -la /opt/codex/ferrari/5.2.96-dirty/sysroots/

# Check if Qt5 libraries exist
find /opt/codex/ferrari/5.2.96-dirty -name "libQt5*.so*" 2>/dev/null | head -5

# Check if Qt5 cmake files exist
find /opt/codex/ferrari/5.2.96-dirty -name "*Qt5*.cmake" 2>/dev/null | head -5
```

If nothing is found, Qt5 might not be installed in your SDK. You may need to:
1. Reinstall SDK
2. Install Qt5 separately
3. Check SDK documentation for Qt5 installation

