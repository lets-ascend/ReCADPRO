# Application vs OS Modification

## ✅ What We Built: **Standalone Application**

This is a **separate application** that runs on your reMarkable Paper Pro, similar to installing an app on your phone or computer.

### Characteristics:

1. **Runs as Separate Process**
   - Installed in `/opt/drafting-pro/` (user space)
   - Runs independently from the main OS interface
   - Doesn't modify system files

2. **Temporary Execution**
   - You stop `xochitl` (the main interface) to run it
   - After testing, you restart `xochitl` to return to normal
   - The OS remains unchanged

3. **Developer Mode Required**
   - Developer Mode allows installing custom applications
   - Doesn't change the OS itself
   - Can be disabled (though it requires factory reset)

4. **Non-Destructive**
   - Doesn't modify system files
   - Doesn't change OS behavior
   - Can be uninstalled by deleting the folder
   - Your device works normally when not running the app

## ❌ What We Did NOT Build: **OS Modification**

An OS modification would:
- Change system files permanently
- Modify `xochitl` or other system components
- Alter how the device boots or behaves
- Require system-level changes
- Be harder to remove/revert

## How It Works

### Normal Operation:
```
reMarkable OS → xochitl (main interface) → Your notebooks/documents
```

### When Running Our App:
```
reMarkable OS → drafting-pro (our app) → Drawing interface
                (xochitl is stopped)
```

### After Testing:
```
reMarkable OS → xochitl (restarted) → Back to normal
```

## Installation Location

```
/opt/drafting-pro/          ← Our app lives here
├── drafting-pro            ← Executable
└── resources/              ← App resources

/usr/bin/                   ← System binaries (unchanged)
/etc/                       ← System config (unchanged)
/usr/lib/                   ← System libraries (unchanged)
```

## Comparison

| Aspect | Standalone App (Ours) | OS Modification |
|--------|----------------------|-----------------|
| **Location** | `/opt/` (user space) | System directories |
| **Persistence** | Only when running | Always active |
| **Reversibility** | Delete folder | Requires reflash |
| **Risk Level** | Low | High |
| **OS Changes** | None | Permanent |
| **Warranty** | May void if issues | Definitely voids |
| **Removal** | Easy | Difficult |

## Running the Application

### Step 1: Stop Main Interface
```bash
ssh root@10.11.99.1
systemctl stop xochitl
```
This temporarily stops the main interface so our app can use the screen.

### Step 2: Run Our App
```bash
cd /opt/drafting-pro
QT_QUICK_BACKEND=epaper ./drafting-pro -platform epaper
```
Our application takes over the display.

### Step 3: Return to Normal
```bash
# Press Ctrl+C to exit our app
systemctl start xochitl
```
The main interface returns, OS unchanged.

## Why This Approach?

### Advantages:
- ✅ **Safe**: Doesn't modify system
- ✅ **Reversible**: Easy to remove
- ✅ **Isolated**: Doesn't affect other functions
- ✅ **Testable**: Can test without risk
- ✅ **Standard**: Follows reMarkable's recommended approach

### Limitations:
- ⚠️ Can't run simultaneously with main interface
- ⚠️ Requires Developer Mode
- ⚠️ Must stop/start xochitl manually

## Future Possibilities

### If You Want Permanent Integration:
You could potentially:
1. **Create a launcher** in the main interface (requires more advanced modification)
2. **Add to system menu** (requires OS-level changes)
3. **Run as background service** (complex, not recommended)

But for now, our approach is:
- ✅ **Safer**
- ✅ **Easier to develop**
- ✅ **Easier to maintain**
- ✅ **Follows reMarkable guidelines**

## Summary

**This is a standalone application**, not an OS modification:
- Installs as a separate app
- Runs independently
- Doesn't change the OS
- Can be easily removed
- Safe and reversible

Think of it like installing an app on your phone - it's separate from the operating system itself.

