# 🔍 Simple Qt5 Check Commands

## Run These One at a Time
    
**1. Check what sysroots exist:**
```bash
ls /opt/codex/ferrari/5.2.96-dirty/sysroots/
```

**2. Find Qt5Config.cmake:**
```bash
find /opt/codex/ferrari/5.2.96-dirty -name Qt5Config.cmake
```

**3. If found, get the prefix path:**
```bash
find /opt/codex/ferrari/5.2.96-dirty -name Qt5Config.cmake | head -1 | xargs dirname | xargs dirname | xargs dirname
```

**4. Check for Qt libraries:**
```bash
find /opt/codex/ferrari/5.2.96-dirty -name libQt5*.so | head -5
```

## OR Use the Helper Script

```bash
chmod +x check-qt5.sh
./check-qt5.sh
```

This will check everything and tell you what to use for CMAKE_PREFIX_PATH.

