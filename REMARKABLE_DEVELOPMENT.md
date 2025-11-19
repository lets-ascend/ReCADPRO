# reMarkable Paper Pro Development Guide

This document provides comprehensive information on developing **native Qt Quick/QML + C++ applications** specifically for the reMarkable Paper Pro tablet.

## Official Resources

### Primary Documentation
- **Developer Portal**: https://developer.remarkable.com
- **Developer Mode**: https://developer.remarkable.com/documentation/developer-mode
- **SDK Documentation**: https://developer.remarkable.com/documentation/sdk
- **Qt Quick E-Paper**: https://developer.remarkable.com/documentation/qt_epaper
- **Software Stack Overview**: https://developer.remarkable.com/documentation/software-stack
- **Recovery Mode**: https://developer.remarkable.com/documentation/recovery-mode

### Device Information
- **Product Codename**: `ferrari` (reMarkable Paper Pro)
- **Architecture**: ARM Cortex-A53
- **OS**: Custom Linux distribution (Yocto Project)
- **UI Framework**: Qt Quick (QML) - **Pure Qt Quick only, no Widgets**
- **Display**: E-paper display (1872x1404) optimized for drawing

## Getting Started

### Step 1: Enable Developer Mode

⚠️ **WARNING**: Enabling Developer Mode will perform a factory reset and erase all data on your device!

1. On your reMarkable Paper Pro:
   - Navigate to: `Settings` > `General` > `Paper Tablet` > `Software` > `Advanced` > `Developer Mode`
   - Follow the on-screen instructions
   - **Back up all your data first!**

2. After enabling, find SSH credentials:
   - Go to: `Settings` > `General` > `Help` > `About` > `Copyrights and Licenses`
   - Note the username (`root`) and randomly generated password

### Step 2: Set Up SSH Access

1. Connect your reMarkable Paper Pro to your computer via USB cable
2. An Ethernet-over-USB connection will appear
3. SSH into the device:
   ```bash
   ssh root@10.11.99.1
   # Use the password from Step 1
   ```

### Step 3: Download and Install the SDK

1. **Find your device's OS version**:
   ```bash
   ssh root@10.11.99.1 "cat /etc/version"
   ```

2. **Download the appropriate SDK**:
   - Visit: https://developer.remarkable.com/documentation/sdk
   - Download SDK matching your OS version
   - SDK filename pattern: `meta-toolchain-remarkable-[version]-ferrari-public-x86_64-toolchain.sh`

3. **Install the SDK**:
   ```bash
   chmod u+x meta-toolchain-remarkable-[version]-ferrari-public-x86_64-toolchain.sh
   ./meta-toolchain-remarkable-[version]-ferrari-public-x86_64-toolchain.sh -d ~/remarkable-sdk
   ```

4. **Set up the environment**:
   ```bash
   source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
   ```

   Add this to your `~/.bashrc` or `~/.zshrc` for persistence:
   ```bash
   echo "source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux" >> ~/.zshrc
   ```

### Step 4: Develop Native Qt Quick Application

The reMarkable Paper Pro uses Qt Quick for its UI framework. **Only pure Qt Quick applications are supported** (Qt Widgets are NOT supported).

**Requirements**:
- **Linux development machine** (required)
- Qt 5.x or Qt 6 (included in SDK)
- QML knowledge
- C++ for backend logic
- CMake build system

**Project Structure**:
```
remarkable-recadpro/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── drawingengine.h/cpp
│   └── other C++ classes
├── qml/
│   ├── main.qml
│   └── other QML files
└── resources/
```

## Development Workflow

### 1. Build Application

```bash
# Source SDK environment
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Build your application
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 2. Deploy to Device

```bash
# Copy binary to device
scp build/recadpro root@10.11.99.1:/opt/recadpro/

# Or use deployment script
./deploy-remarkable.sh
```

### 3. Run on Device

```bash
# SSH into device
ssh root@10.11.99.1

# Stop xochitl (main interface)
systemctl stop xochitl

# Run application with e-paper backend
cd /opt/recadpro
QT_QUICK_BACKEND=epaper ./recadpro -platform epaper

# After testing, restart xochitl
systemctl start xochitl
```

## reMarkable-Specific Considerations

### Display Optimization

- **E-paper refresh**: Minimize full-screen refreshes
- **Partial updates**: Use region-based updates when possible
- **Color**: Grayscale only (no color support)
- **Resolution**: Optimize for device resolution (1872x1404 for Paper Pro)
- **Backend**: Must use `-platform epaper` flag

### Input Handling

- **Stylus**: Pressure-sensitive input supported via QTouchEvent
- **Touch**: Multi-touch gestures supported
- **Buttons**: Physical buttons available

### Performance

- **Battery**: Optimize for low power consumption
- **Memory**: Limited RAM, optimize memory usage
- **CPU**: ARM Cortex-A53, optimize for efficiency
- **E-paper**: Use Qt e-paper backend for optimal refresh

### File System

- **Storage**: Limited internal storage
- **SD Card**: External storage support
- **Installation**: Use `/opt/` for applications

## Application Architecture

### Pure Qt Quick (Required)

- ✅ Use QML for all UI
- ✅ Use C++ for backend logic
- ✅ Register C++ types with `qmlRegisterType`
- ✅ Use Q_PROPERTY for QML bindings
- ❌ **Do NOT use Qt Widgets** (not supported)

### E-Paper Backend

Always run with:
```bash
QT_QUICK_BACKEND=epaper ./your-app -platform epaper
```

### Screen Dimensions

Use Qt's Screen object:
```qml
ApplicationWindow {
    width: Screen.width   // 1872 for Paper Pro
    height: Screen.height // 1404 for Paper Pro
}
```

## Community Resources

### GitHub Repositories

- **rmkit**: Community toolkit for reMarkable development
  - GitHub: https://github.com/rmkit-dev/rmkit
  - Provides utilities and examples

- **reMarkable Apps**: Collection of community apps
  - Search GitHub for "remarkable" apps

### Forums and Communities

- Reddit: r/RemarkableTablet
- reMarkable Community Forums
- Discord servers (search for reMarkable)

## Testing Checklist

- [ ] Test on actual reMarkable Paper Pro device
- [ ] Verify stylus pressure sensitivity
- [ ] Test touch gestures
- [ ] Check battery consumption
- [ ] Verify e-paper refresh behavior
- [ ] Test with e-paper backend (`-platform epaper`)
- [ ] Verify full screen usage
- [ ] Test file save/load functionality

## Troubleshooting

### SSH Connection Issues

```bash
# Check if device is connected
ping 10.11.99.1

# Try different USB port
# Restart device
```

### Build Issues

```bash
# Verify SDK environment is sourced
echo $CC
# Should show arm-remarkable-linux-gnueabihf-gcc

# Check Qt version
qmake --version

# Verify you're on Linux (required)
uname -a
```

### Runtime Issues

```bash
# Check logs on device
journalctl -f

# Check available memory
free -h

# Check disk space
df -h

# Verify xochitl is stopped
systemctl status xochitl

# Ensure e-paper backend is used
QT_QUICK_BACKEND=epaper ./app -platform epaper
```

## Important Notes

- ⚠️ **Warranty**: Modifying device software may void warranty
- ⚠️ **Data Loss**: Developer Mode performs factory reset
- ⚠️ **Recovery**: Keep recovery instructions handy
- ✅ **Backup**: Always backup before enabling Developer Mode
- ✅ **Testing**: Test thoroughly before deploying
- ✅ **Linux Required**: Development must be done on Linux
- ✅ **Pure Qt Quick**: Only Qt Quick is supported, no Widgets

## Additional Resources

- reMarkable Support: https://support.remarkable.com
- Yocto Project Documentation: https://www.yoctoproject.org/docs/
- Qt Documentation: https://doc.qt.io/
- QML Documentation: https://doc.qt.io/qt-5/qtqml-index.html
- Qt Quick E-Paper Guide: https://developer.remarkable.com/documentation/qt_epaper
