# reMarkable Paper Pro Development Resources

## Official Documentation

### Primary Resources
- **Developer Portal**: https://developer.remarkable.com
- **Developer Mode Guide**: https://developer.remarkable.com/documentation/developer-mode
- **SDK Documentation**: https://developer.remarkable.com/documentation/sdk
- **Qt Quick Applications**: https://developer.remarkable.com/documentation
- **Software Stack**: https://developer.remarkable.com/documentation/software-stack
- **Recovery Mode**: https://developer.remarkable.com/documentation/recovery-mode

### Device Specifications
- **Product Codename**: `ferrari`
- **Architecture**: ARM Cortex-A53
- **Display**: 1872x1404 e-paper
- **OS**: Custom Linux (Yocto Project)
- **UI Framework**: Qt Quick (QML)
- **SDK Pattern**: `meta-toolchain-remarkable-[version]-ferrari-public-x86_64-toolchain.sh`

## Community Resources

### GitHub Repositories
- **rmkit**: https://github.com/rmkit-dev/rmkit
  - Community toolkit for reMarkable development
  - Utilities, examples, and helper libraries

- **reMarkable Apps**: Search GitHub for "remarkable" apps
  - Various community-developed applications
  - Examples and templates

### Forums & Communities
- **Reddit**: r/RemarkableTablet
- **reMarkable Support**: https://support.remarkable.com
- **Discord**: Search for reMarkable communities

## Development Tools

### Required Software
- **SDK**: Download from developer.remarkable.com
- **SSH Client**: For device access
- **Qt 5.x**: Included in SDK
- **CMake**: For building Qt applications
- **Cross-compiler**: ARM toolchain (included in SDK)

### Useful Commands

#### SSH Access
```bash
# Connect to device (default IP)
ssh root@10.11.99.1

# Check device version
ssh root@10.11.99.1 "cat /etc/version"

# Check available space
ssh root@10.11.99.1 "df -h"
```

#### SDK Setup
```bash
# Install SDK
chmod u+x meta-toolchain-remarkable-*.sh
./meta-toolchain-remarkable-*.sh -d ~/remarkable-sdk

# Source environment
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Verify setup
echo $CC  # Should show arm-remarkable-linux-gnueabihf-gcc
```

#### Building Applications
```bash
# Source SDK environment
source env-setup.sh

# Build Qt application
mkdir build && cd build
cmake ..
make

# Deploy
./deploy.sh
```

## Key Technologies

### Qt Quick (QML)
- **Documentation**: https://doc.qt.io/qt-5/qtqml-index.html
- **Examples**: https://doc.qt.io/qt-5/qtquick-qml-referenceexamples.html
- **Best Practices**: Optimize for e-paper display

### Yocto Project
- **Documentation**: https://www.yoctoproject.org/docs/
- Used for building reMarkable OS

### Linux ARM Development
- **Cross-compilation**: Use SDK toolchain
- **Architecture**: ARMv8 (Cortex-A53)

## Development Workflow

1. **Development**: Write Qt Quick/QML + C++ code on Linux
2. **Cross-compile**: Build for ARM using SDK
3. **Deploy**: Transfer binary to device via SSH
4. **Test**: Run on actual reMarkable Paper Pro device
5. **Iterate**: Refine based on device testing

## Important Links Summary

| Resource | URL |
|----------|-----|
| Developer Portal | https://developer.remarkable.com |
| Developer Mode | https://developer.remarkable.com/documentation/developer-mode |
| SDK Download | https://developer.remarkable.com/documentation/sdk |
| Qt Documentation | https://doc.qt.io/ |
| QML Reference | https://doc.qt.io/qt-5/qtqml-index.html |
| Support | https://support.remarkable.com |
| rmkit GitHub | https://github.com/rmkit-dev/rmkit |

## Getting Help

1. **Official Documentation**: Start with developer.remarkable.com
2. **Community Forums**: Reddit, Discord, GitHub discussions
3. **Examples**: Check rmkit and other GitHub repositories
4. **Device Logs**: Use `journalctl -f` on device for debugging

## Next Steps

1. Read [REMARKABLE_DEVELOPMENT.md](./REMARKABLE_DEVELOPMENT.md)
2. Enable Developer Mode on your device
3. Download and install the SDK
4. Run `./setup-remarkable.sh` to configure your environment
5. Start developing!

