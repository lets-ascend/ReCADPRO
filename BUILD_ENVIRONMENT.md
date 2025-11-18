# 💻 Build Environment Requirements

## ❌ You Cannot Build on Mac or Windows Directly

The reMarkable SDK requires **Linux** for cross-compilation. You have several options:

## ✅ Option 1: Linux Virtual Machine (Recommended)

### Using VirtualBox (Free)

1. **Download VirtualBox**: https://www.virtualbox.org/
2. **Download Ubuntu 22.04 LTS**: https://ubuntu.com/download/desktop
3. **Create VM**:
   - Allocate 4GB+ RAM
   - Allocate 50GB+ disk space
   - Enable USB passthrough (for device connection)
4. **Install Ubuntu** in VM
5. **Install build tools**:
   ```bash
   sudo apt update
   sudo apt install build-essential cmake git
   ```
6. **Transfer project** to VM (shared folder or git clone)
7. **Build in VM** using Linux environment

### Using VMware Fusion (Mac, Paid)

Similar to VirtualBox but better performance on Mac.

### Using Parallels (Mac, Paid)

Best performance on Mac, easiest setup.

## ✅ Option 2: Docker Container (Advanced)

### Create Dockerfile
```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    ssh \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
```

### Build in Docker
```bash
# Build container
docker build -t remarkable-build .

# Run container with project mounted
docker run -it -v $(pwd):/workspace remarkable-build

# Inside container, build project
cd /workspace
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
mkdir build && cd build
cmake ..
make
```

## ✅ Option 3: Cloud Linux Instance

### AWS EC2 / Google Cloud / Azure

1. **Launch Linux instance** (Ubuntu 22.04)
2. **SSH into instance**
3. **Clone project**:
   ```bash
   git clone <your-repo>
   ```
4. **Build remotely**
5. **Download binary** via SCP

**Note**: You'll need USB-over-IP or VPN for device connection.

## ✅ Option 4: Dual Boot Linux (Permanent Solution)

1. **Partition Mac disk** (or use separate drive)
2. **Install Ubuntu** alongside macOS
3. **Boot into Linux** when building
4. **Best performance** - native Linux

## ❌ Option 5: Windows WSL2 (Not Recommended)

**Problem**: WSL2 has USB passthrough limitations, making device connection difficult.

**Possible but complex**: Requires USB-over-IP or network bridge setup.

## 🎯 Recommended Setup for Mac Users

### **Best Option: VirtualBox + Ubuntu**

**Why:**
- ✅ Free
- ✅ Easy to set up
- ✅ USB passthrough works
- ✅ Can connect device directly
- ✅ Full Linux environment
- ✅ Can delete VM when done

**Steps:**

1. **Install VirtualBox**:
   ```bash
   brew install --cask virtualbox
   ```

2. **Download Ubuntu ISO**:
   - Visit: https://ubuntu.com/download/desktop
   - Download Ubuntu 22.04 LTS

3. **Create VM**:
   - Open VirtualBox
   - New → Name: "Remarkable Dev"
   - Type: Linux, Version: Ubuntu (64-bit)
   - Memory: 4096 MB
   - Hard disk: 50 GB (VDI, dynamically allocated)

4. **Configure USB**:
   - Settings → USB → Enable USB Controller
   - Add USB filter for reMarkable device

5. **Install Ubuntu**:
   - Start VM
   - Select Ubuntu ISO
   - Follow installation wizard

6. **Install build tools**:
   ```bash
   sudo apt update
   sudo apt install build-essential cmake git ssh
   ```

7. **Transfer project**:
   ```bash
   # Option A: Shared folder
   # Settings → Shared Folders → Add Mac folder
   
   # Option B: Git clone
   git clone <your-repo-url>
   
   # Option C: SCP from Mac
   scp -r /path/to/Remarkable_app user@vm-ip:/home/user/
   ```

8. **Build in VM**:
   ```bash
   cd Remarkable_app
   source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
   mkdir build && cd build
   cmake ..
   make -j$(nproc)
   ```

9. **Deploy from VM**:
   ```bash
   ./deploy-remarkable.sh
   ```

## 📋 System Requirements

### Minimum VM Requirements
- **RAM**: 4GB (8GB recommended)
- **Disk**: 50GB free space
- **CPU**: 2 cores (4 recommended)
- **USB**: USB 2.0+ support

### Mac Requirements
- **macOS**: 10.14+ (for VirtualBox)
- **RAM**: 8GB+ total (4GB for VM)
- **Disk**: 100GB+ free (for VM + project)

## 🔌 USB Device Connection

### VirtualBox USB Setup

1. **Install VirtualBox Extension Pack**:
   - Download: https://www.virtualbox.org/wiki/Downloads
   - Install: `File → Preferences → Extensions → Add`

2. **Enable USB in VM**:
   - VM Settings → USB
   - Enable USB Controller
   - Select USB 2.0 or 3.0

3. **Add USB Filter**:
   - When device connected, VM → USB → Select device
   - Or: Settings → USB → Add filter (automatic)

4. **Verify Connection**:
   ```bash
   # In VM
   lsusb
   # Should show reMarkable device
   ```

## 🚀 Quick Start (VM Method)

```bash
# 1. Install VirtualBox and Ubuntu (see above)

# 2. In Ubuntu VM, install tools
sudo apt update
sudo apt install build-essential cmake git ssh

# 3. Transfer project to VM
# (Use shared folder or git clone)

# 4. Download SDK in VM
# Visit: https://developer.remarkable.com/documentation/sdk
# Download SDK matching your device version

# 5. Install SDK
chmod u+x meta-toolchain-remarkable-*.sh
./meta-toolchain-remarkable-*.sh -d ~/remarkable-sdk

# 6. Source environment
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux

# 7. Build project
cd Remarkable_app
mkdir build && cd build
cmake ..
make -j$(nproc)

# 8. Deploy
./deploy-remarkable.sh
```

## 📝 Summary

**For Mac Users:**
- ✅ **Use VirtualBox + Ubuntu** (easiest, free)
- ✅ **Or Docker** (if comfortable with containers)
- ✅ **Or Cloud Linux** (if no local setup wanted)
- ❌ **Cannot build directly on Mac** (SDK requires Linux)
- ❌ **Windows alone won't work** (need Linux)

**Bottom Line:**
You need a **Linux environment** (VM, Docker, or cloud) to build for reMarkable. VirtualBox + Ubuntu is the **simplest solution** for Mac users.

