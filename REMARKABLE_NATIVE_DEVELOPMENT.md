# reMarkable Paper Pro Native Development Guide

This application is **exclusively** developed for the reMarkable Paper Pro tablet using Qt Quick/QML and C++, following official reMarkable development guidelines.

## ✅ Compliance with reMarkable Guidelines

### Architecture
- ✅ **Pure Qt Quick** - No Qt Widgets (as required)
- ✅ **QML for UI** - All UI in QML files
- ✅ **C++ Backend** - Performance-critical logic in C++
- ✅ **E-paper Optimized** - Uses `-platform epaper` backend
- ✅ **Screen Dimensions** - Uses `Screen.width/height` (1872x1404)
- ✅ **Touch & Stylus** - Handles both touch and marker input

### Project Structure
```
Remarkable_app/
├── CMakeLists.txt          # Build configuration
├── src/                    # C++ source files
│   ├── main.cpp
│   ├── drawingengine.h/cpp
│   ├── tools.h/cpp
│   ├── layers.h/cpp
│   ├── draftingtools.h/cpp
│   └── stylusinput.h/cpp
├── qml/                    # QML UI files
│   ├── main.qml
│   ├── DrawingCanvas.qml
│   ├── Toolbar.qml
│   ├── Sidebar.qml
│   ├── LayersPanel.qml
│   └── GridOverlay.qml
└── resources/              # Resources (icons, etc.)
```

## Building for reMarkable Paper Pro

### Prerequisites

1. **Linux Development Machine** (required)
2. **reMarkable SDK** - Download from https://developer.remarkable.com/documentation/sdk
3. **CMake** - Build system
4. **Qt6** (or Qt5 from SDK)

### Setup

1. **Install SDK**:
   ```bash
   chmod u+x meta-toolchain-remarkable-[version]-ferrari-public-x86_64-toolchain.sh
   ./meta-toolchain-remarkable-[version]-ferrari-public-x86_64-toolchain.sh -d ~/remarkable-sdk
   ```

2. **Source SDK Environment**:
   ```bash
   source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
   ```

3. **Verify Setup**:
   ```bash
   echo $CC  # Should show arm-remarkable-linux-gnueabihf-gcc
   qmake --version
   ```

### Build

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)
```

### Deploy

```bash
# Use deployment script
./deploy-remarkable.sh

# Or manually:
scp build/recadpro root@10.11.99.1:/opt/recadpro/
```

## Running on reMarkable Paper Pro

### Steps

1. **SSH into device**:
   ```bash
   ssh root@10.11.99.1
   ```

2. **Stop xochitl** (main interface):
   ```bash
   systemctl stop xochitl
   ```

3. **Run application**:
   ```bash
   cd /opt/recadpro
   QT_QUICK_BACKEND=epaper ./recadpro -platform epaper
   ```

4. **After testing, restart xochitl**:
   ```bash
   systemctl start xochitl
   ```

### Important Notes

- **E-paper Backend**: Must use `-platform epaper` flag
- **QT_QUICK_BACKEND**: Set to `epaper` for optimal display
- **Full Screen**: Application uses full screen (1872x1404)
- **Stylus Input**: Handled automatically via touch events with pressure

## reMarkable-Specific Features

### E-Paper Display Optimization

The application is optimized for e-paper displays:

1. **Minimal Refresh**: Uses partial updates where possible
2. **Grayscale**: Optimized color palette for grayscale display
3. **Performance**: Efficient rendering for e-paper refresh cycles
4. **Backend**: Uses Qt e-paper backend (`-platform epaper`)

### Stylus Input Handling

The reMarkable marker (stylus) is handled through:

1. **Touch Events**: QTouchEvent with pressure information
2. **Pressure Sensitivity**: Pressure values from 0.0 to 1.0
3. **Automatic Detection**: Qt handles stylus vs finger distinction

### Screen Dimensions

- **Width**: 1872px
- **Height**: 1404px
- **Aspect Ratio**: 4:3
- **DPI**: Optimized for e-paper display

## Code Organization

### C++ Classes

- **DrawingEngine**: Core drawing with pressure sensitivity
- **Tools**: Tool management and properties
- **Layers**: Layer system management
- **DraftingTools**: Grid, snap-to-grid, measurements
- **StylusInput**: Stylus/marker input handling

### QML Components

- **main.qml**: Main application window
- **DrawingCanvas.qml**: Drawing surface
- **Toolbar.qml**: Top toolbar
- **Sidebar.qml**: Left sidebar with tools
- **LayersPanel.qml**: Right sidebar with layers
- **GridOverlay.qml**: Grid overlay canvas

## Development Guidelines

### Following reMarkable Standards

1. **Pure Qt Quick**: No Qt Widgets
2. **QML First**: UI in QML, logic in C++
3. **E-paper Aware**: Optimize for e-paper refresh
4. **Touch Optimized**: Large touch targets (minimum 44x44px)
5. **Grayscale**: Design for grayscale display
6. **Performance**: Optimize for ARM Cortex-A53

### Best Practices

1. **Register C++ Types**: Use `qmlRegisterType` in main.cpp
2. **Properties**: Use Q_PROPERTY for QML bindings
3. **Signals/Slots**: Use Qt's signal/slot system
4. **Memory**: Be mindful of limited RAM
5. **Battery**: Optimize for battery life

## Troubleshooting

### Build Issues

```bash
# Check SDK environment
echo $CC
echo $CXX
echo $QTDIR

# Verify Qt installation
qmake --version
```

### Runtime Issues

```bash
# Check device logs
ssh root@10.11.99.1 "journalctl -f"

# Check available memory
ssh root@10.11.99.1 "free -h"

# Check disk space
ssh root@10.11.99.1 "df -h"
```

### Display Issues

- Ensure `-platform epaper` flag is used
- Set `QT_QUICK_BACKEND=epaper` environment variable
- Check e-paper refresh settings

## Resources

### Official Documentation
- **reMarkable Developer Portal**: https://developer.remarkable.com
- **Qt Quick E-Paper**: https://developer.remarkable.com/documentation/qt_epaper
- **SDK Documentation**: https://developer.remarkable.com/documentation/sdk

### Qt Documentation
- **Qt Quick**: https://doc.qt.io/qt-6/qtquick-index.html
- **QML Reference**: https://doc.qt.io/qt-6/qtqml-index.html
- **C++ Integration**: https://doc.qt.io/qt-6/qtqml-cppintegration-topic.html

## Notes

- This application is **exclusively** for reMarkable Paper Pro
- No cross-platform code or web-based fallbacks
- All features optimized for Paper Pro hardware
- Follows official reMarkable development guidelines strictly

