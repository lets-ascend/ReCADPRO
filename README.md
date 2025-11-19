# ReCADPro

**Native Qt Quick/QML + C++ standalone application for reMarkable Paper Pro**

This is a **standalone application** (not an OS modification) that runs on your reMarkable Paper Pro tablet. It's built specifically for the device, following official reMarkable development guidelines. It uses pure Qt Quick (no Widgets) with C++ backend, optimized for e-paper display.

> **Note**: This is an **application**, not an OS modification. It runs separately from the main interface and doesn't modify system files. See [APP_VS_OS_MODIFICATION.md](./APP_VS_OS_MODIFICATION.md) for details.

## 🎯 reMarkable Paper Pro Exclusive

- ✅ Pure Qt Quick/QML + C++
- ✅ E-paper display optimized
- ✅ Pressure-sensitive stylus support
- ✅ Follows official reMarkable guidelines
- ✅ Native performance
- ✅ No cross-platform code

## Features

### Drawing Tools
- **Pen Tool**: Pressure-sensitive pen strokes
- **Pencil Tool**: Natural pencil-like drawing
- **Brush Tool**: Variable-width brush strokes
- **Eraser**: Precise erasing
- **Shapes**: Line, Rectangle, Circle, Arrow

### Drafting Features
- **Grid System**: Lines, dots, isometric grids
- **Snap to Grid**: Precise alignment
- **Ruler Tool**: Distance measurements
- **Protractor**: Angle measurements
- **Templates**: Pre-configured paper types

### Prototyping
- **Layer System**: Multiple layers
- **Layer Management**: Show/hide, rename, delete
- **Export**: PNG, SVG, PDF formats

## Quick Start

### Prerequisites

1. **Linux development machine** (required)
2. **reMarkable SDK** - Download from https://developer.remarkable.com/documentation/sdk
3. **CMake** and build tools

### Build

```bash
# Source SDK environment
source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Deploy

```bash
# Deploy to device
./deploy-remarkable.sh

# Or manually:
scp build/recadpro root@10.11.99.1:/opt/recadpro/
```

### Run on Device

```bash
# SSH into device
ssh root@10.11.99.1

# Stop xochitl
systemctl stop xochitl

# Run application
cd /opt/recadpro
QT_QUICK_BACKEND=epaper ./recadpro -platform epaper

# After testing, restart xochitl
systemctl start xochitl
```

## Project Structure

```
Remarkable_app/
├── CMakeLists.txt              # Build configuration
├── src/                        # C++ source files
│   ├── main.cpp
│   ├── drawingengine.h/cpp
│   ├── tools.h/cpp
│   ├── layers.h/cpp
│   ├── draftingtools.h/cpp
│   └── stylusinput.h/cpp
├── qml/                        # QML UI files
│   ├── main.qml
│   ├── DrawingCanvas.qml
│   ├── Toolbar.qml
│   ├── Sidebar.qml
│   ├── LayersPanel.qml
│   └── GridOverlay.qml
├── resources/                  # Resources
├── deploy-remarkable.sh        # Deployment script
└── REMARKABLE_NATIVE_DEVELOPMENT.md  # Full development guide
```

## 📚 Documentation

### 🚀 Getting Started
- **[IMPLEMENTATION_GUIDE.md](./IMPLEMENTATION_GUIDE.md)** ⭐ **START HERE** - Complete step-by-step guide for Ubuntu VM setup and building
- **[TESTING_GUIDE.md](./TESTING_GUIDE.md)** - Comprehensive testing checklist for final testing
- **[QUICK_START.md](./QUICK_START.md)** - Quick reference guide

### 📊 Project Status
- **[IMPLEMENTATION_STATUS.md](./IMPLEMENTATION_STATUS.md)** - Complete implementation status (what's done vs. future ideas)
- **[HIGH_PRIORITY_STATUS.md](./HIGH_PRIORITY_STATUS.md)** - High priority features completion status

### 🔧 Development
- **[REMARKABLE_NATIVE_DEVELOPMENT.md](./REMARKABLE_NATIVE_DEVELOPMENT.md)** - Complete development guide
- **[BUILD_ENVIRONMENT.md](./BUILD_ENVIRONMENT.md)** - Build environment requirements
- **[VM_BUILD_STEPS.md](./VM_BUILD_STEPS.md)** - VM-specific build instructions
- **[GIT_SETUP.md](./GIT_SETUP.md)** - Git repository setup guide

### 📖 Reference
- **[CAD_COMPARISON.md](./CAD_COMPARISON.md)** - Feature comparison with AutoCAD/Fusion 360
- **[COLOR_CAPABILITIES.md](./COLOR_CAPABILITIES.md)** - Canvas Color display capabilities
- **[ADVANCED_LINE_TOOLS.md](./ADVANCED_LINE_TOOLS.md)** - Advanced line tools documentation
- **[RESOURCES.md](./RESOURCES.md)** - Development resources and links
- **[SAFETY_REVIEW.md](./SAFETY_REVIEW.md)** - Code safety and integrity review
- **[APP_VS_OS_MODIFICATION.md](./APP_VS_OS_MODIFICATION.md)** - Application vs OS modification explanation

## Important Notes

⚠️ **This application is exclusively for reMarkable Paper Pro**
- No web-based code
- No cross-platform support
- Pure native Qt Quick application
- Follows official reMarkable guidelines strictly

## License

MIT License
