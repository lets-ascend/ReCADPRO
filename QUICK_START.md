# Quick Start Guide - reMarkable Paper Pro Native Application

## 🚀 Fast Track Setup

### 1. Enable Developer Mode (⚠️ Factory Reset!)

On your reMarkable Paper Pro:
```
Settings → General → Paper Tablet → Software → Advanced → Developer Mode
```

**⚠️ BACKUP YOUR DATA FIRST!**

### 2. Get SSH Credentials

```
Settings → General → Help → About → Copyrights and Licenses
```
Note: Username is `root`, password is randomly generated

### 3. Download SDK

1. Visit: https://developer.remarkable.com/documentation/sdk
2. Find your device OS version:
   ```bash
   ssh root@10.11.99.1 "cat /etc/version"
   ```
3. Download matching SDK: `meta-toolchain-remarkable-[version]-ferrari-public-x86_64-toolchain.sh`

### 4. Install SDK

```bash
chmod u+x meta-toolchain-remarkable-*.sh
./meta-toolchain-remarkable-*.sh -d ~/remarkable-sdk
```

### 5. Run Setup Script

```bash
./setup-remarkable.sh ~/remarkable-sdk
```

### 6. Source Environment

```bash
source env-setup.sh
```

### 7. Test Connection

```bash
ssh root@10.11.99.1
# Enter password from step 2
```

## 📦 Build and Deploy Native Qt Application

### Build

```bash
# Source SDK environment
source env-setup.sh

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Deploy

```bash
# Use deployment script
./deploy-remarkable.sh

# Or manually:
scp build/recadpro root@10.11.99.1:/opt/recadpro/
```

### Run on Device

```bash
# SSH into device
ssh root@10.11.99.1

# Stop xochitl (main interface)
systemctl stop xochitl

# Run application
cd /opt/recadpro
QT_QUICK_BACKEND=epaper ./recadpro -platform epaper

# After testing, restart xochitl
systemctl start xochitl
```

## 🔑 Key Commands

```bash
# SSH into device
ssh root@10.11.99.1

# Check device info
ssh root@10.11.99.1 "cat /etc/version"
ssh root@10.11.99.1 "free -h"
ssh root@10.11.99.1 "df -h"

# View logs
ssh root@10.11.99.1 "journalctl -f"

# Copy files
scp file.txt root@10.11.99.1:/home/root/
```

## 📚 Documentation

- **Native Development Guide**: [REMARKABLE_NATIVE_DEVELOPMENT.md](./REMARKABLE_NATIVE_DEVELOPMENT.md)
- **General Development Info**: [REMARKABLE_DEVELOPMENT.md](./REMARKABLE_DEVELOPMENT.md)
- **Resources**: [RESOURCES.md](./RESOURCES.md)
- **Official Docs**: https://developer.remarkable.com

## ⚠️ Important Notes

- **Native Qt Application**: This is a Qt Quick/QML + C++ application, NOT a web app
- **Linux Required**: Development must be done on Linux
- **Developer Mode**: Enabling Developer Mode performs a factory reset (backup first!)
- **Warranty**: May void warranty
- **Testing**: Test thoroughly before deploying

## 🆘 Troubleshooting

**Can't SSH?**
- Check USB connection
- Try different USB port
- Restart device

**Build fails?**
- Verify SDK environment: `echo $CC`
- Check Qt installation: `qmake --version`
- Source environment: `source env-setup.sh`
- Ensure you're on Linux (required for reMarkable development)

**App won't run?**
- Check logs: `journalctl -f`
- Verify permissions: `chmod +x /opt/recadpro/recadpro`
- Ensure xochitl is stopped: `systemctl stop xochitl`
- Use e-paper backend: `QT_QUICK_BACKEND=epaper ./recadpro -platform epaper`
