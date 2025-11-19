# 🔧 Fix Missing Qt5Config.cmake

## Problem
Qt5 is installed but Qt5Config.cmake is missing. CMake needs this file to find Qt5.

## Solution: Create Qt5Config.cmake

### Step 1: Run a Creation Script

**Option A – autodetect paths:**  
```bash
chmod +x create-qt5-config.sh
./create-qt5-config.sh
```

**Option B – Yocto-aware (preferred for SDK builds):**  
```bash
chmod +x create-qt5-config-yocto.sh
./create-qt5-config-yocto.sh
```

Both scripts will:
1. Detect the Qt5 install prefix inside the Yocto SDK
2. Create a full `Qt5Config.cmake` under `lib/cmake/Qt5`
3. Print the exact `cmake` command to run next

### Step 2: Run CMake

After the script runs, it will give you the exact cmake command. It will look like:

```bash
cd ~/remarkable-sdk/recadpro/build
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="/opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr"
```

### Step 3: If Script Fails

If the script can't find Qt5, you have two options:

**Option A: Qt5 Not Installed**
- Check reMarkable SDK documentation
- Qt5 might need separate installation
- Some SDK versions don't include Qt5

**Option B: Manual Creation**
If Qt5 libraries exist but config is missing, you can manually create it:

```bash
# Find Qt5 lib directory
QT5_LIB=$(find /opt/codex/ferrari/5.2.96-dirty -name libQt5Core.so* 2>/dev/null | head -1)
QT5_PREFIX=$(dirname "$QT5_LIB" | xargs dirname)

# Create cmake directory
sudo mkdir -p "$QT5_PREFIX/lib/cmake/Qt5"

# Then run the create-qt5-config.sh script
```

## What Qt5Config.cmake Does

This file tells CMake:
- Where Qt5 is installed
- What components are available
- How to link against Qt5 libraries

Without it, CMake can't find Qt5 even if it's installed.

## After Creating the Config File

Once Qt5Config.cmake is created, CMake should be able to find Qt5. Run:

```bash
cd ~/remarkable-sdk/recadpro/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="[path from script]"
```

You should see:
```
-- Found Qt5: [path] (found version "5.x.x")
-- Configuring done
```

## Troubleshooting

**"Permission denied" when creating file:**
```bash
# Run with sudo
sudo ./create-qt5-config.sh
```

**"Qt5 not found" after creating config:**
- Make sure you use the CMAKE_PREFIX_PATH from the script
- Verify Qt5 libraries actually exist
- Check that the path is correct


