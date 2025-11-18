#!/bin/bash

# Quick script to set up git repository for reMarkable Drafting Pro
# Run this on your Mac before cloning in VM

echo "=========================================="
echo "Setting up Git Repository"
echo "=========================================="
echo ""

# Check if already a git repo
if [ -d ".git" ]; then
    echo "✓ Git repository already exists"
    echo "  Run: git remote -v  to see remotes"
    exit 0
fi

# Initialize git repository
echo "1. Initializing git repository..."
git init

# Check if .gitignore exists and is good
if [ -f ".gitignore" ]; then
    echo "✓ .gitignore exists"
else
    echo "Creating .gitignore..."
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
    echo "✓ Created .gitignore"
fi

# Add all files
echo ""
echo "2. Adding files to git..."
git add .

# Initial commit
echo ""
echo "3. Creating initial commit..."
git commit -m "Initial commit - reMarkable Drafting Pro application"

echo ""
echo "=========================================="
echo "Git repository ready!"
echo "=========================================="
echo ""
echo "Next steps:"
echo ""
echo "Option A - Push to GitHub:"
echo "  1. Create repository at: https://github.com/new"
echo "  2. Run: git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO.git"
echo "  3. Run: git push -u origin main"
echo ""
echo "Option B - Use in VM:"
echo "  1. Copy this folder to VM (shared folder or USB)"
echo "  2. In VM: cd /path/to/Remarkable_app"
echo "  3. Continue with SDK setup"
echo ""
echo "To see repository status:"
echo "  git status"
echo ""

