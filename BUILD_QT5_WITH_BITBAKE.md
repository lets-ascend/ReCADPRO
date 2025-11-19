# 🔨 Build Qt5 with Bitbake (Yocto)

## Prerequisites

Your SDK needs to have:
- Bitbake installed
- Qt5 recipes (meta-qt5 layer)
- Build environment set up

## Step 1: Check if Bitbake is Available

```bash
# Source SDK
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# Check for bitbake
which bitbake
bitbake --version
```

## Step 2: Find Qt5 Recipes

```bash
# Search for Qt5 recipes in SDK
find /opt/codex/ferrari/5.2.96-dirty -name "*qt5*.bb" 2>/dev/null
find /opt/codex/ferrari/5.2.96-dirty -name "*qtbase*.bb" 2>/dev/null

# Check for meta-qt5 layer
find /opt/codex/ferrari/5.2.96-dirty -name "meta-qt5" -type d 2>/dev/null
```

## Step 3: Build Qt5

If recipes exist:

```bash
# Initialize build environment (if available)
# This depends on your SDK structure
source /opt/codex/ferrari/5.2.96-dirty/oe-init-build-env build

# Build Qt5 base
bitbake qtbase

# Build Qt5 Quick
bitbake qtdeclarative

# Build Qt5 Quick Controls
bitbake qtquickcontrols2
```

## Step 4: Install Qt5 to SDK

After building:

```bash
# Install to SDK sysroot
bitbake qtbase -c populate_sdk
```

## Alternative: Use Pre-built Qt5

If reMarkable provides Qt5 packages:

```bash
# Check if opkg or similar package manager exists
which opkg

# Install Qt5 packages (if available)
opkg update
opkg install qtbase qtdeclarative qtquickcontrols2
```

## If Bitbake Not Available

If your SDK doesn't include bitbake/build tools, you'll need to:

1. **Use the minimal CMake config** (see create-minimal-qt5-cmake.sh)
2. **Install Qt5 from source** (complex, not recommended)
3. **Use a different SDK version** that includes Qt5
4. **Contact reMarkable support** for Qt5 installation instructions

## Recommended Approach

Since you're using a pre-built SDK, the easiest solution is:

1. **Create minimal CMake config** (run create-minimal-qt5-cmake.sh)
2. **Check if Qt5 libraries can be installed separately**
3. **Use the CMake config** - it will work if Qt5 libraries exist

