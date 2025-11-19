# ⚡ Quick Build Steps - Copy & Paste

## One-Time Setup (Do This First)

```bash
# 1. Source SDK environment (REQUIRED!)
# Replace with YOUR actual SDK path!
# Example: source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
# OR: source ~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux

# 2. Verify SDK is active
echo $CC
# Should show: arm-remarkable-linux-gnueabi-gcc

# 3. Navigate to project
cd ~/Remarkable_app
mkdir -p build
cd build
```

## Build Commands (Copy & Paste)

```bash
# Configure build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build application
make -j$(nproc)
```

## That's It!

If you get errors, check:
1. Did you source the SDK? (Run `echo $CC` - should show arm-remarkable...)
2. Is SDK installed? (Check `ls ~/remarkable-sdk/`)

---

## If You Open a New Terminal Later

You'll need to source SDK again in that new terminal (use YOUR actual SDK path):

```bash
# Replace with YOUR actual SDK path!
source /path/to/your/sdk/environment-setup-cortexa53-crypto-remarkable-linux
cd ~/Remarkable_app/build
make -j$(nproc)  # or cmake .. if you need to reconfigure
```

