# 🔧 Qt5 Fix - Step by Step Guide

## Step 1: Run the Deep Search Script

```bash
# Make it executable
chmod +x find-qt5-deep.sh

# Run it
./find-qt5-deep.sh
```

## Step 2: Look at the Output

The script will show you several things. Here's what to look for:

### ✅ GOOD Signs (Qt5 is there):
- **"Found Qt5Config.cmake"** → Qt5 is installed!
- **"qmake found"** → Qt5 tools are available
- **"libQt5*.so" files found** → Qt5 libraries exist
- **QTDIR is set** → SDK knows where Qt5 is

### ❌ BAD Signs (Qt5 missing):
- **"Qt5Config.cmake NOT FOUND"** → Qt5 CMake files missing
- **"qmake NOT FOUND"** → Qt5 not installed
- **No Qt5 libraries found** → Qt5 not in SDK

## Step 3: What to Do Based on Results

### Scenario A: Qt5Config.cmake FOUND ✅

If the script finds Qt5Config.cmake, it will show you a path like:
```
Found: /opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr/lib/cmake/Qt5/Qt5Config.cmake
```

**Then run cmake with:**
```bash
cd ~/remarkable-sdk/recadpro/build

# Extract the prefix (remove /lib/cmake/Qt5/Qt5Config.cmake from the path)
# Example: If found at /opt/.../usr/lib/cmake/Qt5/Qt5Config.cmake
# Use: /opt/.../usr

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="/opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr"
```

### Scenario B: qmake Found but Qt5Config.cmake NOT Found ⚠️

If qmake exists but Qt5Config.cmake doesn't:

**1. Find where qmake points to:**
```bash
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
which qmake
qmake -query QT_INSTALL_PREFIX
```

**2. Use that path for CMAKE_PREFIX_PATH:**
```bash
cd ~/remarkable-sdk/recadpro/build
QT5_PREFIX=$(qmake -query QT_INSTALL_PREFIX)
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$QT5_PREFIX"
```

### Scenario C: Nothing Found ❌

If Qt5 is completely missing:

**Option 1: Check SDK Documentation**
- Your SDK might require separate Qt5 installation
- Check reMarkable SDK docs for Qt5 setup

**Option 2: Install Qt5 Development Packages**
```bash
# This might work if SDK has package manager
# (Ask in reMarkable developer forums)
```

**Option 3: Use Different SDK Version**
- Some SDK versions include Qt5, others don't
- Check SDK release notes

## Step 4: After Running CMake

### ✅ SUCCESS Looks Like:
```
-- Found Qt5: /opt/codex/ferrari/5.2.96-dirty/sysroots/.../usr (found version "5.x.x")
-- Configuring done
-- Generating done
```

### ❌ FAILURE Looks Like:
```
CMake Error: Could not find a package configuration file provided by "Qt5"
```

If you still get errors, try:
```bash
# Clean build directory
cd ~/remarkable-sdk/recadpro/build
rm -rf *

# Source SDK
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# Try cmake with explicit path
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="/opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr"
```

## Quick Reference: Copy-Paste Commands

**1. Run deep search:**
```bash
chmod +x find-qt5-deep.sh && ./find-qt5-deep.sh
```

**2. If Qt5Config.cmake found, use its path:**
```bash
cd ~/remarkable-sdk/recadpro/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/path/from/script"
```

**3. If only qmake found:**
```bash
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
cd ~/remarkable-sdk/recadpro/build
QT5_PREFIX=$(qmake -query QT_INSTALL_PREFIX)
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$QT5_PREFIX"
```

## Need Help?

Share the output of `./find-qt5-deep.sh` and I'll tell you exactly what to do next!

