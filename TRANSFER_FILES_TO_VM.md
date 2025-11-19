# 📁 How to Transfer Files from Mac to Ubuntu VM

## Method 1: Shared Folder (Easiest - If Available)

### For VirtualBox:
1. **On Mac**: Copy files to a folder (e.g., `~/VM_Share`)
2. **In VirtualBox**: 
   - Select your VM → Settings → Shared Folders
   - Add shared folder pointing to `~/VM_Share`
   - Check "Auto-mount" and "Make Permanent"
3. **In Ubuntu VM**: Files appear at `/media/sf_VM_Share/`
   - You may need to add your user to `vboxsf` group:
     ```bash
     sudo usermod -aG vboxsf $USER
     # Then logout and login again
     ```

### For VMware/UTM:
- Similar process - check VM settings for shared folders

## Method 2: Git (Best for Code Files)

If your project is in a Git repository:

**On Mac:**
```bash
cd ~/Remarkable_app
git add .
git commit -m "Add Qt5 scripts"
git push  # Push to GitHub/GitLab/etc
```

**In Ubuntu VM:**
```bash
# Clone or pull your repository
cd ~
git clone <YOUR_REPO_URL> Remarkable_app
# OR if already cloned:
cd ~/Remarkable_app
git pull
```

## Method 3: SCP (Secure Copy) - Direct Transfer

**On Mac Terminal:**
```bash
# First, find your VM's IP address
# In Ubuntu VM, run: ip addr show | grep "inet " | grep -v 127.0.0.1
# Note the IP (e.g., 192.168.64.2)

# Transfer files
cd ~/Remarkable_app
scp create-minimal-qt5-cmake.sh create-qt5-config.sh check-qt5.sh find-qt5-deep.sh AUTO_FIX_QT5.sh YOUR_USERNAME@VM_IP_ADDRESS:~/recadpro/

# Example:
# scp create-minimal-qt5-cmake.sh user@192.168.64.2:~/recadpro/
```

**In Ubuntu VM**, make sure SSH server is running:
```bash
sudo apt install openssh-server
sudo systemctl start ssh
sudo systemctl enable ssh
```

## Method 4: Copy-Paste (For Small Files)

### For Small Scripts:
1. **On Mac**: Open the script file, select all (Cmd+A), copy (Cmd+C)
2. **In Ubuntu VM**: 
   - Open terminal
   - Type: `nano scriptname.sh`
   - Paste (right-click or Shift+Insert)
   - Save: Ctrl+X, then Y, then Enter

## Method 5: USB Drive

1. Copy files to USB drive on Mac
2. Plug USB into Mac
3. In VM, attach USB device (VM menu → USB)
4. Mount USB in Ubuntu:
   ```bash
   lsblk  # Find USB device (usually /dev/sdb1)
   sudo mkdir -p /mnt/usb
   sudo mount /dev/sdb1 /mnt/usb
   cp /mnt/usb/*.sh ~/recadpro/
   ```

## Method 6: HTTP Server (Quick & Easy)

**On Mac Terminal:**
```bash
cd ~/Remarkable_app
python3 -m http.server 8000
```

**In Ubuntu VM:**
```bash
# Find your Mac's IP address
# On Mac, run: ifconfig | grep "inet " | grep -v 127.0.0.1
# Note the IP (e.g., 192.168.1.100)

# Download files
cd ~/recadpro
wget http://MAC_IP_ADDRESS:8000/create-minimal-qt5-cmake.sh
wget http://MAC_IP_ADDRESS:8000/create-qt5-config.sh
wget http://MAC_IP_ADDRESS:8000/check-qt5.sh
wget http://MAC_IP_ADDRESS:8000/find-qt5-deep.sh
wget http://MAC_IP_ADDRESS:8000/AUTO_FIX_QT5.sh

# Make executable
chmod +x *.sh
```

**Then stop the server on Mac**: Press `Ctrl+C`

## Recommended: Git Method

Since you're working with code, Git is the best option:

**On Mac:**
```bash
cd ~/Remarkable_app
git add create-minimal-qt5-cmake.sh create-qt5-config.sh check-qt5.sh find-qt5-deep.sh AUTO_FIX_QT5.sh
git commit -m "Add Qt5 CMake configuration scripts"
git push
```

**In Ubuntu VM:**
```bash
cd ~/remarkable-sdk/recadpro
git pull
chmod +x *.sh
```

## Quick Start: Copy-Paste Method

For the most important script (`create-minimal-qt5-cmake.sh`), you can copy-paste:

1. **On Mac**: Open `create-minimal-qt5-cmake.sh`, copy all content
2. **In Ubuntu VM**:
   ```bash
   cd ~/remarkable-sdk/recadpro
   nano create-minimal-qt5-cmake.sh
   # Paste content (right-click or Shift+Insert)
   # Save: Ctrl+X, Y, Enter
   chmod +x create-minimal-qt5-cmake.sh
   ```

## Which Method Should You Use?

- **Git**: Best for code files, version control
- **SCP**: Fast, direct transfer
- **HTTP Server**: Quick, no setup needed
- **Shared Folder**: Easiest if already configured
- **Copy-Paste**: Works for small files

Choose the method that's easiest for you!

