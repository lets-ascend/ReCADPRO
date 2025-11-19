# 🐧 How to Run Scripts in Linux (Beginner Guide)

## Step 1: Open Terminal

In your Ubuntu VM:
- Press `Ctrl + Alt + T` to open terminal
- OR click the terminal icon in the taskbar
- OR go to Applications → Terminal

## Step 2: Navigate to Your Project Folder

```bash
# Go to your project directory
cd ~/Remarkable_app

# OR if your project is in a different location:
cd ~/remarkable-sdk/recadpro
```

**Tip**: Type `pwd` to see where you are currently.

## Step 3: Make Script Executable

Before running a script, you need to make it executable:

```bash
# Make the script executable
chmod +x check-qt5.sh

# OR make multiple scripts executable at once:
chmod +x check-qt5.sh find-qt5.sh
```

**What this does**: `chmod +x` gives the script permission to run.

## Step 4: Run the Script

```bash
# Run the script
./check-qt5.sh
```

**Important**: The `./` means "run this file in the current directory"

## Complete Example

Here's the full process from start to finish:

```bash
# 1. Open terminal (Ctrl+Alt+T)

# 2. Go to project folder
cd ~/Remarkable_app

# 3. Make script executable (only need to do this once)
chmod +x check-qt5.sh

# 4. Run the script
./check-qt5.sh
```

## Alternative: Run Without Making Executable

You can also run scripts directly with bash:

```bash
bash check-qt5.sh
```

This doesn't require `chmod +x` first.

## Common Issues

**"Permission denied" error:**
```bash
# Solution: Make it executable first
chmod +x check-qt5.sh
./check-qt5.sh
```

**"No such file or directory" error:**
```bash
# Check if you're in the right folder
pwd
ls -la
# Make sure check-qt5.sh is listed

# If not, navigate to where the script is:
cd ~/Remarkable_app
```

**"Command not found" error:**
```bash
# Make sure you use ./ before the script name
./check-qt5.sh
# NOT just: check-qt5.sh
```

## Quick Reference

| What you want to do | Command |
|---------------------|---------|
| See current directory | `pwd` |
| List files | `ls` |
| List files with details | `ls -la` |
| Go to home folder | `cd ~` |
| Go to project folder | `cd ~/Remarkable_app` |
| Make script executable | `chmod +x script.sh` |
| Run script | `./script.sh` |
| Run script with bash | `bash script.sh` |
| Cancel command | `Ctrl + C` |
| Clear terminal | `clear` |

## For Your Specific Case

To check for Qt5:

```bash
# 1. Go to your project
cd ~/Remarkable_app

# 2. Make script executable
chmod +x check-qt5.sh

# 3. Run it
./check-qt5.sh
```

That's it! The script will run and show you the results.

