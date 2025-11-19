# 🔧 Build Qt5 with Yocto for reMarkable SDK

## Overview

Your SDK is based on Yocto Project. We need to build Qt5 using bitbake recipes.

## Step 1: Check SDK Structure

```bash
# Check if SDK has bitbake/oe-init-build-env
ls /opt/codex/ferrari/5.2.96-dirty/

# Check for Yocto meta layers
find /opt/codex/ferrari/5.2.96-dirty -name "meta-qt5" -type d 2>/dev/null
find /opt/codex/ferrari/5.2.96-dirty -name "*.bb" | grep -i qt5 | head -5
```

## Step 2: Find Qt5 Recipe

```bash
# Search for Qt5 recipes
find /opt/codex/ferrari/5.2.96-dirty -name "*qt5*.bb" 2>/dev/null
find /opt/codex/ferrari/5.2.96-dirty -name "*qtbase*.bb" 2>/dev/null
```

## Step 3: Build Qt5 with Bitbake

If recipes exist, you can build Qt5:

```bash
# Source SDK environment
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux

# Check if bitbake is available
which bitbake

# If bitbake exists, build Qt5
# (This requires the full Yocto build environment)
```

## Step 4: Alternative - Create Minimal Qt5 CMake Config

If building with Yocto isn't possible, we can create a minimal CMake config that works with what's available.

