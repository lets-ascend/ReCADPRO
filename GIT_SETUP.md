# 📦 Git Setup Guide - Transfer Project to Linux VM

## Option 1: Create New Git Repository (Recommended)

If your project isn't in git yet, here's how to set it up:

### On Your Mac

1. **Initialize git repository** (if not already done):
   ```bash
   cd /Users/drewtruesdell/Remarkable_app
   git init
   ```

2. **Create .gitignore** (to exclude build files):
   ```bash
   cat > .gitignore << 'EOF'
   # Build directories
   build/
   *.o
   *.a
   *.so
   
   # CMake
   CMakeCache.txt
   CMakeFiles/
   cmake_install.cmake
   Makefile
   
   # IDE
   .idea/
   .vscode/
   *.swp
   *.swo
   *~
   
   # OS
   .DS_Store
   Thumbs.db
   
   # SDK (don't commit SDK)
   remarkable-sdk/
   env-setup.sh
   EOF
   ```

3. **Add all files**:
   ```bash
   git add .
   git commit -m "Initial commit - reMarkable Drafting Pro app"
   ```

4. **Create GitHub repository** (or use GitLab/Bitbucket):
   - Go to: https://github.com/new
   - Create new repository (name: `Remarkable_app` or similar)
   - **Don't** initialize with README (you already have files)
   - Copy the repository URL

5. **Push to GitHub**:
   ```bash
   git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
   git branch -M main
   git push -u origin main
   ```

### In Linux VM

1. **Install git** (if not already installed):
   ```bash
   sudo apt update
   sudo apt install git
   ```

2. **Clone repository**:
   ```bash
   cd ~
   git clone https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
   cd YOUR_REPO_NAME
   ```

3. **Verify files**:
   ```bash
   ls -la
   # Should see: CMakeLists.txt, src/, qml/, etc.
   ```

## Option 2: Use Existing Git Repository

If you already have a git repository:

### On Your Mac

1. **Check remote URL**:
   ```bash
   cd /Users/drewtruesdell/Remarkable_app
   git remote -v
   ```

2. **Push latest changes**:
   ```bash
   git add .
   git commit -m "Update before VM transfer"
   git push
   ```

### In Linux VM

1. **Clone repository**:
   ```bash
   cd ~
   git clone <YOUR_REPO_URL>
   cd Remarkable_app  # or whatever your repo is named
   ```

## Option 3: Create Local Git Server (Advanced)

If you don't want to use GitHub/GitLab:

### On Your Mac

1. **Create bare repository**:
   ```bash
   cd ~
   git clone --bare /Users/drewtruesdell/Remarkable_app Remarkable_app.git
   ```

2. **Share via network** (if VM can access Mac):
   - Use shared folder, or
   - Use `git daemon` (advanced)

### In Linux VM

1. **Clone from Mac**:
   ```bash
   # If using shared folder
   git clone /path/to/shared/folder/Remarkable_app.git
   ```

## 🚀 Complete Workflow Example

### Step-by-Step: Mac → GitHub → VM

**On Mac:**

```bash
# 1. Navigate to project
cd /Users/drewtruesdell/Remarkable_app

# 2. Initialize git (if needed)
git init

# 3. Create .gitignore (see above)

# 4. Add and commit
git add .
git commit -m "Initial commit"

# 5. Create GitHub repo (via web browser)
# Visit: https://github.com/new
# Name: Remarkable_app
# Don't initialize with README

# 6. Add remote and push
git remote add origin https://github.com/YOUR_USERNAME/Remarkable_app.git
git branch -M main
git push -u origin main
```

**In Linux VM:**

```bash
# 1. Install git
sudo apt update
sudo apt install git

# 2. Clone repository
cd ~
git clone https://github.com/YOUR_USERNAME/Remarkable_app.git
cd Remarkable_app

# 3. Verify project structure
ls -la
# Should see: CMakeLists.txt, src/, qml/, etc.

# 4. Continue with SDK setup and build
# (Follow BUILD_ENVIRONMENT.md from here)
```

## 🔄 Updating Project (After Changes)

### On Mac (make changes)

```bash
cd /Users/drewtruesdell/Remarkable_app
# Make your changes...
git add .
git commit -m "Description of changes"
git push
```

### In Linux VM (get updates)

```bash
cd ~/Remarkable_app
git pull
# Rebuild if needed
cd build
cmake ..
make -j$(nproc)
```

## 📋 Quick Reference

### Common Git Commands

```bash
# Check status
git status

# Add files
git add .

# Commit changes
git commit -m "Your message"

# Push to remote
git push

# Pull updates
git pull

# View commit history
git log --oneline

# Create new branch
git checkout -b feature-name

# Switch branches
git checkout main
```

## ⚠️ Important Notes

1. **Don't commit SDK** - Add `remarkable-sdk/` to `.gitignore`
2. **Don't commit build files** - Add `build/` to `.gitignore`
3. **Don't commit sensitive data** - No passwords or API keys
4. **Use .gitignore** - Keeps repository clean

## 🎯 Recommended Approach

**For beginners**: Use **GitHub** (free, easy, reliable)
- ✅ Free public/private repos
- ✅ Easy to set up
- ✅ Works from anywhere
- ✅ Backup of your code

**Steps:**
1. Create GitHub account (if needed)
2. Create new repository
3. Push from Mac
4. Clone in VM

That's it! Your code is now accessible from both Mac and VM.

