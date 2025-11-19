# ⚠️ Qt5 Not Found - Solutions

## Problem
Qt5Config.cmake is not found in your SDK. This means Qt5 might not be included in your SDK installation.

## Solution Options

### Option 1: Check if Qt5 is in a Different Location

Run the deep search script:
```bash
chmod +x find-qt5-deep.sh
./find-qt5-deep.sh
```

This will check:
- All sysroots
- All Qt5 files
- Qt5 libraries
- qmake location

### Option 2: Install Qt5 in SDK

If Qt5 is truly missing, you may need to:

1. **Check SDK documentation** - Some SDKs require separate Qt5 installation
2. **Install Qt5 packages** - The SDK might have Qt5 packages available

### Option 3: Use System Qt5 (Not Recommended)

You could use system Qt5, but this won't cross-compile correctly for reMarkable.

### Option 4: Check SDK Version

Your SDK might be missing Qt5. Check:
- SDK version compatibility
- Whether Qt5 needs to be installed separately
- SDK release notes

## Next Steps

1. Run `./find-qt5-deep.sh` to see what we find
2. Check SDK documentation for Qt5 installation
3. Verify SDK installation completed successfully

## Quick Check Commands

```bash
# Check if qmake exists after sourcing SDK
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
which qmake
qmake --version

# Check what's in the sysroot
ls -la /opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr/lib/ | grep -i qt
```

