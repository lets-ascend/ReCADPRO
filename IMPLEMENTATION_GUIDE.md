# 🚀 ReCADPro - Complete Implementation Guide

## 📋 Quick Overview

This guide will get you from a fresh Ubuntu VM to a running ReCADPro app on your reMarkable Paper Pro.

**Estimated Time**: 2-3 hours (first time)

---

## ✅ Prerequisites

- ✅ Ubuntu 22.04 LTS VM installed (VirtualBox, VMware, UTM, etc.)
- ✅ reMarkable Paper Pro device
- ✅ USB cable to connect device
- ✅ Internet connection in VM

---

## Step 1: Initial Ubuntu Setup (15 minutes)

### 1.1 Update System

```bash
sudo apt updat
```

### 1.2 Install Essential Build Tools

```bash
sudo apt install -y \
    build-essential \
    cmake \
    git \
    ssh \
    wget \
    curl \
    file \
    python3 \
    python3-pip \
    libqt5quick5 \
    qtbase5-dev \
    qtdeclarative5-dev \
    qttools5-dev \
    qttools5-dev-tools \
    qt5-qmake \
    libqt5svg5-dev
```

### 1.3 Verify Installations

```bash
cmake --version    # Should show 3.16+
gcc --version      # Should show 9.0+
git --version      # Should show 2.25+
qmake --version    # Should show Qt 5.x
```

---

## Step 2: Get Your Device Version (5 minutes)

### 2.1 Connect Device to Mac

1. Connect reMarkable Paper Pro to Mac via USB
2. Enable Developer Mode on device (Settings → General → Developer Mode)
3. **Note**: Developer Mode will reset your device!

### 2.2 Get Version Number

**On Mac Terminal:**
```bash
ssh root@10.11.99.1 "cat /etc/version"
```

**Output example**: `3.10.5` or `3.11.2`

**Write down this version number!**

---

## Step 3: Download reMarkable SDK (10 minutes)

### 3.1 Find SDK Download URL

1. Visit: https://developer.remarkable.com/sdk
2. Find SDK for your device version
3. Copy the download URL (looks like):
   ```
   https://developer.remarkable.com/sdk/meta-toolchain-remarkable-3.10.5-ferrari-public-x86_64-toolchain.sh
   ```

### 3.2 Download SDK in VM

**In Ubuntu VM terminal:**

```bash
cd ~
mkdir -p remarkable-sdk
cd remarkable-sdk

# Replace VERSION with your version (e.g., 3.10.5)
# Replace URL with actual URL from reMarkable website
wget https://developer.remarkable.com/sdk/meta-toolchain-remarkable-VERSION-ferrari-public-x86_64-toolchain.sh

# Make executable
chmod +x meta-toolchain-remarkable-*.sh

# Install SDK (this takes 5-10 minutes)
./meta-toolchain-remarkable-*.sh
```

**Follow prompts:**
- The installer will ask where to install the SDK
- **Default is usually `~/remarkable-sdk`** but it may install elsewhere (like `/opt/codex/ferrari/...`)
- **Note the installation path** - you'll need it later!
- Wait for installation to complete

### 3.3 Find Your SDK Installation Path

After installation, find where the SDK was installed:

```bash
# The SDK installer usually tells you the path
# Look for output like:
# "SDK installed to: /opt/codex/ferrari/5.2.96-dirty"
# or
# "SDK installed to: /home/USER/remarkable-sdk"

# Find the environment setup file:
find ~ -name "environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null
# or
find /opt -name "environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null

# Common locations:
# - ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
# - /opt/codex/ferrari/[VERSION]/environment-setup-cortexa53-crypto-remarkable-linux
```

**Write down your SDK path!** You'll need it for every build.

### 3.4 Verify SDK Installation

```bash
# Source the SDK environment (use YOUR actual path!)
# Example paths:
# source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
# OR
# source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# Replace with YOUR actual path:
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Verify cross-compiler
$CC --version
# Should show ARM cross-compiler version

# Verify Qt version
qmake --version
# Should show Qt 5.x for ARM
```

---

## Step 4: Clone Repository (5 minutes)

### 4.1 Clone Project

```bash
cd ~
git clone <YOUR_REPO_URL> Remarkable_app
cd Remarkable_app

# Verify structure
ls -la
# Should see: src/, qml/, CMakeLists.txt
```

**If you don't have a repo yet:**
```bash
cd ~
mkdir Remarkable_app
cd Remarkable_app
git init
# Then copy your files here or pull from remote
```

---

## Step 5: Build Application (20-30 minutes)

### 5.1 Source SDK Environment

**⚠️ CRITICAL FIRST STEP**: You MUST source the SDK environment before running cmake!

**First, find your SDK path** (if you don't know it):
```bash
# Find the environment setup file
find ~ -name "environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null
find /opt -name "environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null
```

**Common SDK paths:**
- `~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux`
- `/opt/codex/ferrari/[VERSION]/environment-setup-cortexa53-crypto-remarkable-linux`

**Then source it** (replace with YOUR actual path):
```bash
# Example: If SDK is at /opt/codex/ferrari/5.2.96-dirty/
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# OR if SDK is at ~/remarkable-sdk/
# source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Verify you're in SDK environment
echo $CC
# Should show: arm-remarkable-linux-gnueabi-gcc

# Verify Qt5 is available
qmake --version
# Should show Qt 5.x version

# Verify Qt5 path is set
echo $QTDIR
# Should show Qt5 installation path
```

**Note**: You need to source the SDK in your current terminal session. If you open a NEW terminal later, you'll need to source it again in that new terminal.

### 5.2 Create Build Directory

```bash
cd ~/Remarkable_app
mkdir -p build
cd build
```

### 5.3 Configure Build

**⚠️ BEFORE RUNNING CMAKE**: Make sure you completed Step 5.1 (sourced SDK)!

If you haven't sourced the SDK yet, do it now (use YOUR actual SDK path):
```bash
# Replace with YOUR actual SDK path!
# Example: source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
# OR: source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux
```

Then run cmake:
```bash
# First, find Qt5 location using helper script
cd ~/Remarkable_app
chmod +x find-qt5.sh
./find-qt5.sh /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# OR find manually:
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
QT5_PATH=$(find /opt/codex/ferrari/5.2.96-dirty -name "Qt5Config.cmake" 2>/dev/null | head -1)
if [ ! -z "$QT5_PATH" ]; then
    QT5_PREFIX=$(dirname "$QT5_PATH" | xargs dirname | xargs dirname)
    echo "Found Qt5 at: $QT5_PREFIX"
    cd build
    cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_PREFIX_PATH="$QT5_PREFIX"
else
    echo "ERROR: Qt5 not found! Check SDK installation."
fi
```

**If you get "Qt5 not found" errors**, it means SDK wasn't sourced. Fix it:
```bash
# Check if SDK environment is active
echo $CC
# Should show: arm-remarkable-linux-gnueabi-gcc (if not, SDK not sourced!)

# If $CC is empty, source SDK now (use YOUR actual SDK path):
# Example: source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Verify it worked
echo $CC
qmake --version

# Now try cmake again with CMAKE_PREFIX_PATH
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$QTDIR"

**Expected output:**
```
-- The C compiler identification is GNU...
-- The CXX compiler identification is GNU...
-- Found Qt5: ...
-- Configuring done
-- Generating done
-- Build files have been written to: /home/USER/Remarkable_app/build
```

### 5.4 Build Application

```bash
# Build (this takes 10-20 minutes first time)
make -j$(nproc)

# If you get errors, try single-threaded:
make
```

**Expected output:**
```
[100%] Built target ReCADPro
```

### 5.5 Verify Build

```bash
# Check binary was created
file ReCADPro
# Should show: ELF 32-bit LSB executable, ARM, version 1

# Check it's ARM binary
readelf -h ReCADPro | grep Machine
# Should show: Machine: ARM
```

---

## Step 6: Deploy to Device (10 minutes)

### 6.1 Connect Device to VM

**Option A: USB Passthrough (Recommended)**
1. In VM settings, enable USB passthrough
2. Connect device to Mac
3. In VM, select device from USB menu
4. Device should appear in VM

**Option B: Network Connection (Easier)**
1. Connect device to Mac via USB
2. Device gets IP: `10.11.99.1`
3. VM can access via network

### 6.2 Test Device Connection

**In VM terminal:**
```bash
# Test SSH connection
ssh root@10.11.99.1 "uname -a"
# Should show device info
```

**If connection fails:**
```bash
# Add device to known hosts
ssh-keyscan -H 10.11.99.1 >> ~/.ssh/known_hosts

# Try again
ssh root@10.11.99.99.1 "uname -a"
```

### 6.3 Deploy Application

**Option A: Use Deployment Script**

```bash
cd ~/Remarkable_app
chmod +x deploy-remarkable.sh
./deploy-remarkable.sh
```

**Option B: Manual Deployment**

```bash
cd ~/Remarkable_app/build

# Copy binary to device
scp ReCADPro root@10.11.99.1:/usr/bin/ReCADPro

# Copy QML files
scp -r ../qml root@10.11.99.1:/usr/share/ReCADPro/

# Make executable
ssh root@10.11.99.1 "chmod +x /usr/bin/ReCADPro"
```

---

## Step 7: Run Application on Device (5 minutes)

### 7.1 SSH to Device

```bash
ssh root@10.11.99.1
```

### 7.2 Run Application

**On device terminal:**
```bash
# Run with e-paper optimization
QT_QPA_PLATFORM=epaper ReCADPro

# Or run normally (for testing)
ReCADPro
```

### 7.3 Create Launcher (Optional)

**On device terminal:**
```bash
# Create desktop entry
cat > /usr/share/applications/ReCADPro.desktop << EOF
[Desktop Entry]
Name=ReCADPro
Comment=Advanced CAD Drawing Application
Exec=/usr/bin/ReCADPro
Icon=applications-graphics
Type=Application
Categories=Graphics;
EOF

# Refresh launcher
killall -HUP xochitl
```

---

## Step 8: Testing Checklist

### ✅ Basic Functionality

- [ ] Application launches without errors
- [ ] Drawing canvas appears
- [ ] Pen tool draws strokes
- [ ] Pressure sensitivity works
- [ ] Undo/redo works
- [ ] Toolbar buttons respond
- [ ] Sidebar tools work

### ✅ Advanced Features

- [ ] Object snap works (enable in sidebar)
- [ ] Zoom/pan gestures work
- [ ] Shape tools (rectangle, circle, arc)
- [ ] Text tool
- [ ] Selection tool
- [ ] Layers panel
- [ ] Export functions

### ✅ Performance

- [ ] Smooth drawing (no lag)
- [ ] Responsive UI
- [ ] E-paper refresh is acceptable
- [ ] No crashes during normal use

---

## 🔧 Troubleshooting

### Build Errors

**Error: "Qt not found" or "Could not find Qt5"**
```bash
# CRITICAL: Make sure SDK is sourced FIRST! (use YOUR actual SDK path)
# Example: source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Verify Qt5 is available
qmake --version
# Should show Qt 5.x

# Verify Qt5 path
echo $QTDIR
# Should show path like: /home/USER/remarkable-sdk/sysroots/cortexa53-crypto-remarkable-linux/usr

# Verify cross-compiler
echo $CC
# Should show: arm-remarkable-linux-gnueabi-gcc

# If still not found, check SDK installation
ls ~/remarkable-sdk/sysroots/cortexa53-crypto-remarkable-linux/usr/lib/libQt5*
# Should show Qt5 libraries

# Clean build directory and re-run cmake AFTER sourcing SDK
cd ~/Remarkable_app/build
rm -rf *
# Source SDK (use YOUR actual path!)
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux
cmake .. -DCMAKE_BUILD_TYPE=Release
```

**Error: "CMake version too old"**
```bash
# Install newer CMake
sudo apt remove cmake
sudo apt install -y cmake=3.22+
```

**Error: "Missing dependencies"**
```bash
# Install all Qt dependencies
sudo apt install -y \
    qtbase5-dev \
    qtdeclarative5-dev \
    qttools5-dev \
    libqt5quick5 \
    libqt5svg5-dev
```

### Deployment Errors

**Error: "Permission denied"**
```bash
# Make sure binary is executable
chmod +x ReCADPro

# Check device permissions
ssh root@10.11.99.1 "ls -la /usr/bin/ReCADPro"
```

**Error: "Cannot connect to device"**
```bash
# Check device IP
ping 10.11.99.1

# Check SSH
ssh -v root@10.11.99.1
```

### Runtime Errors

**Error: "QML module not found"**
```bash
# Check QML files are deployed
ssh root@10.11.99.1 "ls -la /usr/share/ReCADPro/qml/"

# Check QML import paths
ssh root@10.11.99.1 "QT_QPA_PLATFORM=epaper ReCADPro 2>&1 | grep -i qml"
```

**Error: "Application crashes on launch"**
```bash
# Run with debug output
ssh root@10.11.99.1 "QT_QPA_PLATFORM=epaper QT_LOGGING_RULES='*.debug=true' ReCADPro 2>&1 | tee /tmp/recadpro.log"

# Check log
ssh root@10.11.99.1 "cat /tmp/recadpro.log"
```

---

## 📝 Quick Reference Commands

### Build Commands
```bash
# ALWAYS source SDK first in every terminal session! (use YOUR actual SDK path)
# Example: source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Clean build
cd ~/Remarkable_app/build
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Rebuild after changes (still need SDK sourced - use YOUR actual path)
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux
make -j$(nproc)
```

### Deployment Commands
```bash
# Deploy binary
scp build/ReCADPro root@10.11.99.1:/usr/bin/ReCADPro

# Deploy QML
scp -r qml root@10.11.99.1:/usr/share/ReCADPro/

# Run on device
ssh root@10.11.99.1 "QT_QPA_PLATFORM=epaper ReCADPro"
```

### Testing Commands
```bash
# Check binary type
file build/ReCADPro

# Check dependencies
arm-remarkable-linux-gnueabi-readelf -d build/ReCADPro

# View device logs
ssh root@10.11.99.1 "journalctl -f"
```

---

## 🎯 Next Steps After Successful Build

1. **Test all features** using the testing checklist above
2. **Report bugs** if you find any issues
3. **Optimize performance** if needed
4. **Add missing features** (Ortho Mode, Array Tool)

---

## 📚 Additional Resources

- **reMarkable SDK Docs**: https://developer.remarkable.com/documentation/sdk
- **Qt for Embedded**: https://doc.qt.io/qt-5/embedded-linux.html
- **Project README**: See `README.md` for project overview

---

## ✅ Success Criteria

You've successfully completed setup when:
- ✅ Application builds without errors
- ✅ Binary is ARM-compatible
- ✅ Application deploys to device
- ✅ Application launches on device
- ✅ Basic drawing works
- ✅ UI is responsive

**Congratulations! You're ready to test ReCADPro! 🎉**

