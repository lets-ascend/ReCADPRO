# 🚀 VM Build Steps - Complete Guide

## ✅ You've Cloned the Repository - What's Next?

Follow these steps **in your Linux VM** to build and deploy the application.

## Step 1: Install Build Tools

```bash
# Update package list
sudo apt update

# Install essential build tools
sudo apt install -y build-essential cmake git ssh

# Verify installations
cmake --version
gcc --version
```

## Step 2: Download reMarkable SDK

### Find Your Device Version

First, connect your reMarkable Paper Pro to your Mac via USB, then:

**Option A: From Mac Terminal (EASIEST)**
```bash
# On Mac, SSH to device
ssh root@10.11.99.1 "cat /etc/version"
# Note the version number (e.g., "3.10.5")
```

**Option B: From VM (if USB passthrough works)**
```bash
# In VM, check if device is accessible
ssh root@10.11.99.1 "cat /etc/version"
```

### Download SDK - UTM VM Solutions

**🎯 EASIEST METHOD: Download directly in VM (NO file transfer needed!)**

1. **In your UTM VM**, open a terminal and run:
   ```bash
   # First, get your device version (from Mac terminal)
   # Then download SDK directly in VM:
   cd ~
   
   # Install wget if needed
   sudo apt update
   sudo apt install -y wget
   
   # Download SDK (replace VERSION with your version, e.g., 3.10.5)
   # Find the exact URL at: https://developer.remarkable.com/documentation/sdk
   wget https://developer.remarkable.com/sdk/meta-toolchain-remarkable-VERSION-ferrari-public-x86_64-toolchain.sh
   ```

**Alternative: Network Transfer from Mac to VM**

If you downloaded the SDK on your Mac, transfer it via network:

1. **Find VM IP address** (in VM terminal):
   ```bash
   ip addr show | grep "inet " | grep -v 127.0.0.1
   # Note the IP (e.g., 192.168.64.2)
   ```

2. **Enable SSH in VM** (if not already):
   ```bash
   sudo apt install -y openssh-server
   sudo systemctl start ssh
   sudo systemctl enable ssh
   ```

3. **From Mac terminal**, transfer the file:
   ```bash
   # Replace VM_IP with your VM's IP address
   # Replace USERNAME with your VM username
   scp ~/Downloads/meta-toolchain-remarkable-*.sh USERNAME@VM_IP:~/Downloads/
   ```

**Alternative: HTTP Server Method**

1. **On Mac**, start a simple HTTP server:
   ```bash
   cd ~/Downloads
   python3 -m http.server 8000
   ```

2. **In VM**, download from Mac:
   ```bash
   # Find your Mac's IP address (in Mac terminal):
   # ifconfig | grep "inet " | grep -v 127.0.0.1
   
   # In VM, download:
   wget http://MAC_IP_ADDRESS:8000/meta-toolchain-remarkable-*.sh
   ```

**Alternative: UTM Shared Folder**

1. **In UTM**, configure shared folder:
   - VM Settings → Sharing → Directory Sharing
   - Add your Mac folder (e.g., ~/Downloads)
   - Mount point: `/mnt/shared`

2. **In VM**, access shared folder:
   ```bash
   sudo apt install -y spice-vdagent  # May help with shared folders
   ls /mnt/shared  # Check if folder is accessible
   cp /mnt/shared/meta-toolchain-*.sh ~/
   ```

## Step 3: Install SDK

```bash
# Navigate to where you downloaded the SDK
cd ~

# Make SDK installer executable
chmod u+x meta-toolchain-remarkable-*.sh

# Install SDK (this will take a few minutes)
./meta-toolchain-remarkable-*.sh -d ~/remarkable-sdk

# Wait for installation to complete...
```

## Step 4: Set Up SDK Environment

```bash
# Source the SDK environment
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Verify SDK is loaded
echo $CC
# Should show: arm-remarkable-linux-gnueabihf-gcc

# Make environment persistent (add to ~/.bashrc)
echo "source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux" >> ~/.bashrc
```

**Important**: You need to source this environment **every time** you open a new terminal, or add it to `~/.bashrc` for persistence.

## Step 5: Navigate to Project

```bash
# Go to cloned project
cd ~/Remarkable_app

# Verify project structure
ls -la
# Should see: CMakeLists.txt, src/, qml/, etc.
```

## Step 6: Build the Application

```bash
# Make sure SDK environment is sourced
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build (this will take several minutes)
make -j$(nproc)

# Wait for build to complete...
```

### Build Output

You should see:
```
[100%] Built target recadpro
```

The binary will be at: `build/recadpro`

## Step 7: Verify Build

```bash
# Check binary exists
ls -lh build/recadpro

# Check it's ARM binary (should show ARM architecture)
file build/recadpro
# Should show: ELF 32-bit LSB executable, ARM, ...

# Check binary size (should be reasonable, not huge)
du -h build/recadpro
```

## Step 8: Set Up SSH Access to Device

### Get Device Credentials

1. **On reMarkable device**:
   - Settings → General → Help → About → Copyrights and Licenses
   - Note the **password** (username is always `root`)

2. **Test connection** (from VM):
   ```bash
   # Connect device via USB to Mac
   # Ensure USB passthrough is enabled in VirtualBox
   
   # Test SSH connection
   ssh root@10.11.99.1
   # Enter password when prompted
   # Type 'exit' to disconnect
   ```

### Set Up SSH Key (Optional, but recommended)

```bash
# Generate SSH key (if you don't have one)
ssh-keygen -t rsa -b 4096 -C "remarkable-dev"

# Copy key to device
ssh-copy-id root@10.11.99.1

# Test passwordless login
ssh root@10.11.99.1
```

## Step 9: Deploy to Device

### Option A: Use Deployment Script

```bash
# Make sure you're in project root
cd ~/Remarkable_app

# Make deployment script executable
chmod +x deploy-remarkable.sh

# Run deployment script
./deploy-remarkable.sh
```

### Option B: Manual Deployment

```bash
# Create directory on device
ssh root@10.11.99.1 "mkdir -p /opt/recadpro"

# Copy binary
scp build/recadpro root@10.11.99.1:/opt/recadpro/

# Make executable
ssh root@10.11.99.1 "chmod +x /opt/recadpro/recadpro"

# Copy resources (if any)
scp -r resources root@10.11.99.1:/opt/recadpro/ 2>/dev/null || echo "No resources folder"
```

## Step 10: Run on Device

```bash
# SSH into device
ssh root@10.11.99.1

# Stop main interface (xochitl)
systemctl stop xochitl

# Navigate to app directory
cd /opt/recadpro

# Run application
QT_QUICK_BACKEND=epaper ./recadpro -platform epaper
```

### If App Runs Successfully

You should see the application interface on your reMarkable screen!

### To Exit Application

- Press `Ctrl+C` in the SSH terminal
- Or close the application window

### To Return to Normal

```bash
# Restart main interface
systemctl start xochitl

# Exit SSH
exit
```

## 🔧 Troubleshooting

### UTM VM Connection Issues

**Problem: Can't transfer files between Mac and UTM VM**

**Solution 1: Download SDK directly in VM (RECOMMENDED - No transfer needed!)**
```bash
# In UTM VM terminal:
cd ~
sudo apt update
sudo apt install -y wget curl

# Get your device version first (from Mac terminal):
# ssh root@10.11.99.1 "cat /etc/version"

# Then download SDK directly (replace VERSION):
wget https://developer.remarkable.com/sdk/meta-toolchain-remarkable-VERSION-ferrari-public-x86_64-toolchain.sh
```

**Solution 2: Set up network connection**
```bash
# In VM, check network:
ip addr show

# Enable SSH in VM:
sudo apt install -y openssh-server
sudo systemctl start ssh
sudo systemctl enable ssh

# Find VM IP:
hostname -I

# From Mac, test connection:
ping VM_IP_ADDRESS

# Transfer file from Mac:
scp ~/Downloads/meta-toolchain-*.sh USERNAME@VM_IP:~/
```

**Solution 3: Use HTTP server (works even if SSH doesn't)**
```bash
# On Mac terminal:
cd ~/Downloads
python3 -m http.server 8000

# Find Mac IP:
ifconfig | grep "inet " | grep -v 127.0.0.1

# In VM terminal:
wget http://MAC_IP:8000/meta-toolchain-remarkable-*.sh
```

**Solution 4: UTM Shared Folders**
1. UTM → VM Settings → Sharing → Directory Sharing
2. Add folder (e.g., ~/Downloads)
3. In VM: `sudo apt install -y spice-vdagent`
4. Check: `ls /mnt/shared` or `/Volumes/UTM`

### Build Errors

**Error: "CMake not found"**
```bash
sudo apt install cmake
```

**Error: "Qt6 not found"**
- SDK should include Qt, but check:
```bash
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
qmake --version
```

**Error: "Cross-compiler not found"**
```bash
# Make sure SDK is sourced
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
echo $CC
```

### Deployment Errors

**Error: "Permission denied"**
```bash
# Make sure binary is executable
chmod +x build/recadpro
```

**Error: "Connection refused"**
- Check device is connected via USB
- Check USB passthrough is enabled in VirtualBox
- Try: `ping 10.11.99.1`

**Error: "Device not found"**
- Ensure reMarkable is connected via USB
- Check VirtualBox USB settings
- Try disconnecting/reconnecting USB

### Runtime Errors

**App doesn't start**
```bash
# Check logs
ssh root@10.11.99.1 "journalctl -f"

# Check binary permissions
ssh root@10.11.99.1 "ls -la /opt/recadpro/recadpro"

# Try running with verbose output
ssh root@10.11.99.1 "cd /opt/recadpro && QT_QUICK_BACKEND=epaper ./recadpro -platform epaper 2>&1"
```

## 📋 Quick Reference Checklist

- [ ] Install build tools (`build-essential`, `cmake`, `git`)
- [ ] Download reMarkable SDK
- [ ] Install SDK (`./meta-toolchain-*.sh -d ~/remarkable-sdk`)
- [ ] Source SDK environment (`source ~/remarkable-sdk/environment-setup-*`)
- [ ] Navigate to project (`cd ~/Remarkable_app`)
- [ ] Build (`mkdir build && cd build && cmake .. && make`)
- [ ] Verify binary (`file build/recadpro`)
- [ ] Set up SSH access to device
- [ ] Deploy (`./deploy-remarkable.sh` or manual `scp`)
- [ ] Stop xochitl (`systemctl stop xochitl`)
- [ ] Run app (`cd /opt/recadpro && ./recadpro -platform epaper`)
- [ ] Test application
- [ ] Restart xochitl (`systemctl start xochitl`)

## 🎯 Next Steps After First Build

### Making Changes

1. **Edit code** on Mac (or in VM)
2. **Commit changes**:
   ```bash
   git add .
   git commit -m "Description"
   git push
   ```
3. **Pull in VM**:
   ```bash
   cd ~/Remarkable_app
   git pull
   ```
4. **Rebuild**:
   ```bash
   cd build
   make -j$(nproc)
   ```
5. **Redeploy**:
   ```bash
   ./deploy-remarkable.sh
   ```

## ✅ Success Indicators

You'll know everything worked when:
- ✅ Build completes without errors
- ✅ Binary is ARM architecture (`file` command shows ARM)
- ✅ Binary deploys to device successfully
- ✅ Application launches on reMarkable screen
- ✅ You can interact with the drawing interface

## 🎉 You're Ready!

Follow these steps and you'll have your app running on your reMarkable Paper Pro!

