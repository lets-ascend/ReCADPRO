# 📍 Finding Your SDK Installation Path

## Your SDK Path

Based on your installation, your SDK path is:
```
/opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
```

## Quick Reference

**To source your SDK, use:**
```bash
source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux
```

## Finding SDK Path (If You Forget)

If you forget your SDK path, find it with:

```bash
# Search in home directory
find ~ -name "environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null

# Search in /opt (common location)
find /opt -name "environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null

# Search everywhere (slower)
find / -name "environment-setup-cortexa53-crypto-remarkable-linux" 2>/dev/null | head -5
```

## Common SDK Locations

- `~/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux` (default)
- `/opt/codex/ferrari/[VERSION]/environment-setup-cortexa53-crypto-remarkable-linux` (your location)
- `/usr/local/remarkable-sdk/environment-setup-cortexa53-crypto-remarkable-linux`

## Make It Easier: Create an Alias

Add this to your `~/.bashrc` or `~/.zshrc`:

```bash
# Add SDK alias
alias source-sdk='source /opt/codex/ferrari/5.2.96-dirty/environment-setup-cortexa53-crypto-remarkable-linux'

# Then reload shell config
source ~/.bashrc  # or source ~/.zshrc
```

**Now you can just type:**
```bash
source-sdk
```

Instead of the full path!

