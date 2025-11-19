# 📁 Transfer Files Without Copy-Paste

## Method 1: HTTP Server (Easiest - No Copy-Paste Needed!)

### Step 1: On Your Mac

Open Terminal and run:
```bash
cd ~/Remarkable_app
python3 -m http.server 8000
```

You'll see: `Serving HTTP on 0.0.0.0 port 8000 ...`

**Keep this terminal open!**

### Step 2: Find Your Mac's IP Address

**On Mac**, in a NEW terminal window:
```bash
ifconfig | grep "inet " | grep -v 127.0.0.1
```

Look for something like: `inet 192.168.1.100` or `inet 192.168.64.2`

**Write down this IP address!**

### Step 3: In Your Ubuntu VM

Open terminal and run:
```bash
# Replace MAC_IP with the IP from Step 2
MAC_IP="192.168.1.100"  # CHANGE THIS!

cd ~/remarkable-sdk/recadpro

# Download all scripts
wget http://$MAC_IP:8000/create-minimal-qt5-cmake.sh
wget http://$MAC_IP:8000/create-qt5-config.sh
wget http://$MAC_IP:8000/check-qt5.sh
wget http://$MAC_IP:8000/find-qt5-deep.sh
wget http://$MAC_IP:8000/AUTO_FIX_QT5.sh

# Make executable
chmod +x *.sh
```

### Step 4: Stop Server on Mac

Go back to Mac terminal and press `Ctrl+C`

**Done!** Files are now in your VM.

---

## Method 2: SCP (Direct File Transfer)

### Step 1: Enable SSH in Ubuntu VM

**In Ubuntu VM:**
```bash
sudo apt install openssh-server
sudo systemctl start ssh
sudo systemctl enable ssh

# Find VM's IP address
ip addr show | grep "inet " | grep -v 127.0.0.1
# Write down the IP (e.g., 192.168.64.3)
```

### Step 2: Transfer from Mac

**On Mac Terminal:**
```bash
cd ~/Remarkable_app

# Replace VM_IP and USERNAME with your VM's IP and username
scp create-minimal-qt5-cmake.sh USERNAME@VM_IP:~/recadpro/
scp create-qt5-config.sh USERNAME@VM_IP:~/recadpro/
scp check-qt5.sh USERNAME@VM_IP:~/recadpro/
scp find-qt5-deep.sh USERNAME@VM_IP:~/recadpro/
scp AUTO_FIX_QT5.sh USERNAME@VM_IP:~/recadpro/

# Example:
# scp create-minimal-qt5-cmake.sh pickledick@192.168.64.3:~/recadpro/
```

**In Ubuntu VM:**
```bash
cd ~/recadpro
chmod +x *.sh
```

---

## Method 3: Create Script Directly in VM (No Transfer!)

**In Ubuntu VM terminal**, create the script directly:

```bash
cd ~/remarkable-sdk/recadpro

cat > create-minimal-qt5-cmake.sh << 'SCRIPT_END'
#!/bin/bash
SDK_BASE="/opt/codex/ferrari/5.2.96-dirty"
SYSROOT="$SDK_BASE/sysroots/cortexa53-crypto-remarkable-linux"
TARGET_USR="$SYSROOT/usr"
QT5_CMAKE_DIR="$TARGET_USR/lib/cmake/Qt5"
sudo mkdir -p "$QT5_CMAKE_DIR" || mkdir -p "$QT5_CMAKE_DIR"
cat > "$QT5_CMAKE_DIR/Qt5Config.cmake" << 'EOF'
get_filename_component(_qt5_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
set(QT_VERSION_MAJOR 5)
set(QT_VERSION_MINOR 15)
set(QT_VERSION_PATCH 0)
set(QT_VERSION_STRING "5.15.0")
set(Qt5_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(Qt5_PREFIX_PATH "${_qt5_install_prefix}")
set(Qt5Core_DIR "${Qt5_DIR}/Qt5Core")
set(Qt5Gui_DIR "${Qt5_DIR}/Qt5Gui")
set(Qt5Qml_DIR "${Qt5_DIR}/Qt5Qml")
set(Qt5Quick_DIR "${Qt5_DIR}/Qt5Quick")
set(Qt5QuickControls2_DIR "${Qt5_DIR}/Qt5QuickControls2")
macro(_qt5_find_library _name _lib)
    find_library(${_name} NAMES ${_lib} PATHS "${Qt5_PREFIX_PATH}/lib" NO_DEFAULT_PATH)
endmacro()
_qt5_find_library(Qt5Core_LIBRARY Qt5Core)
_qt5_find_library(Qt5Gui_LIBRARY Qt5Gui)
_qt5_find_library(Qt5Qml_LIBRARY Qt5Qml)
_qt5_find_library(Qt5Quick_LIBRARY Qt5Quick)
if(Qt5Core_LIBRARY)
    set(Qt5_FOUND TRUE)
endif()
EOF
mkdir -p "$QT5_CMAKE_DIR/Qt5Core"
cat > "$QT5_CMAKE_DIR/Qt5Core/Qt5CoreConfig.cmake" << 'EOF2'
set(Qt5Core_FOUND TRUE)
set(Qt5Core_VERSION "5.15.0")
find_library(Qt5Core_LIBRARY NAMES Qt5Core PATHS "${Qt5_PREFIX_PATH}/lib" NO_DEFAULT_PATH)
if(Qt5Core_LIBRARY)
    add_library(Qt5::Core SHARED IMPORTED)
    set_target_properties(Qt5::Core PROPERTIES IMPORTED_LOCATION "${Qt5Core_LIBRARY}")
endif()
EOF2
echo "✅ Created Qt5Config.cmake"
echo "Run: cd ~/remarkable-sdk/recadpro/build && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=\"$TARGET_USR\""
SCRIPT_END

chmod +x create-minimal-qt5-cmake.sh
```

**This creates the script directly in your VM - no transfer needed!**

---

## Method 4: Git (If Repository is Set Up)

**On Mac:**
```bash
cd ~/Remarkable_app
git add *.sh
git commit -m "Add Qt5 scripts"
git push
```

**In Ubuntu VM:**
```bash
cd ~/remarkable-sdk/recadpro
git pull
chmod +x *.sh
```

---

## Method 5: Shared Folder (If Configured)

**For VirtualBox:**
1. VM Settings → Shared Folders → Add shared folder
2. Point to `~/Remarkable_app` on Mac
3. In VM: Files appear at `/media/sf_Remarkable_app/`
4. Copy files:
   ```bash
   sudo cp /media/sf_Remarkable_app/*.sh ~/recadpro/
   sudo chmod +x ~/recadpro/*.sh
   ```

---

## ⭐ RECOMMENDED: Method 1 (HTTP Server)

It's the easiest and works immediately:

1. **Mac**: `cd ~/Remarkable_app && python3 -m http.server 8000`
2. **VM**: `wget http://[MAC_IP]:8000/create-minimal-qt5-cmake.sh`
3. **Done!**

No copy-paste, no complex setup, just works!

---

## Quick Test

After transferring, test in VM:
```bash
cd ~/remarkable-sdk/recadpro
ls -la *.sh
# Should show your scripts
chmod +x *.sh
./create-minimal-qt5-cmake.sh
```

