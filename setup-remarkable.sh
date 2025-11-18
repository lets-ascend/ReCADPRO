#!/bin/bash

# reMarkable Paper Pro Development Setup Script
# This script helps set up the development environment for reMarkable Paper Pro

set -e

echo "=========================================="
echo "reMarkable Paper Pro Development Setup"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if SDK directory is provided
if [ -z "$1" ]; then
    echo -e "${YELLOW}Usage: $0 <sdk-install-path>${NC}"
    echo "Example: $0 ~/remarkable-sdk"
    echo ""
    echo "First, download the SDK from:"
    echo "https://developer.remarkable.com/documentation/sdk"
    echo ""
    echo "Then install it:"
    echo "chmod u+x meta-toolchain-remarkable-*.sh"
    echo "./meta-toolchain-remarkable-*.sh -d ~/remarkable-sdk"
    exit 1
fi

SDK_PATH="$1"
ENV_SETUP="$SDK_PATH/environment-setup-cortexa53-crypto-remarkable-linux"

# Check if SDK is installed
if [ ! -f "$ENV_SETUP" ]; then
    echo -e "${RED}Error: SDK not found at $SDK_PATH${NC}"
    echo "Please install the SDK first."
    exit 1
fi

echo -e "${GREEN}✓ SDK found at $SDK_PATH${NC}"

# Source the SDK environment
echo ""
echo "Sourcing SDK environment..."
source "$ENV_SETUP"

# Verify environment
echo ""
echo "Verifying environment setup..."
if command -v $CC &> /dev/null; then
    echo -e "${GREEN}✓ Cross-compiler found: $CC${NC}"
else
    echo -e "${RED}✗ Cross-compiler not found${NC}"
    exit 1
fi

# Check Qt
if command -v qmake &> /dev/null; then
    QT_VERSION=$(qmake --version | head -n 1)
    echo -e "${GREEN}✓ Qt found: $QT_VERSION${NC}"
else
    echo -e "${YELLOW}⚠ Qt not found in PATH${NC}"
fi

# Note: Project structure already exists
echo ""
echo "Project structure already exists:"
echo "  - src/     (C++ source files)"
echo "  - qml/     (QML UI files)"
echo "  - CMakeLists.txt (build configuration)"

# Note: CMakeLists.txt already exists in project root
# This script is for SDK environment setup only
echo -e "${YELLOW}Note: CMakeLists.txt already exists in project root${NC}"
echo -e "${YELLOW}This script sets up the SDK environment for building${NC}"

# Note: Source files already exist in src/ and qml/ directories
# Note: Deployment script already exists as deploy-remarkable.sh
echo -e "${YELLOW}Note: Source files already exist in src/ and qml/ directories${NC}"
echo -e "${YELLOW}Note: Use deploy-remarkable.sh for deployment${NC}"

# Create environment setup script
cat > env-setup.sh << EOF
#!/bin/bash
# Source this file to set up the reMarkable SDK environment
# Usage: source env-setup.sh

source "$ENV_SETUP"
echo "reMarkable SDK environment loaded"
echo "Cross-compiler: \$CC"
EOF

chmod +x env-setup.sh
echo -e "${GREEN}✓ Created env-setup.sh${NC}"

# Update shell config
SHELL_RC="$HOME/.zshrc"
if [ -f "$HOME/.bashrc" ] && [ ! -f "$HOME/.zshrc" ]; then
    SHELL_RC="$HOME/.bashrc"
fi

echo ""
echo -e "${YELLOW}Add this to your $SHELL_RC to auto-load SDK environment:${NC}"
echo "source $(pwd)/env-setup.sh"
echo ""
read -p "Add it now? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "" >> "$SHELL_RC"
    echo "# reMarkable SDK Environment" >> "$SHELL_RC"
    echo "source $(pwd)/env-setup.sh" >> "$SHELL_RC"
    echo -e "${GREEN}✓ Added to $SHELL_RC${NC}"
fi

echo ""
echo "=========================================="
echo -e "${GREEN}Setup complete!${NC}"
echo "=========================================="
echo ""
echo "Next steps:"
echo "1. Source the environment: source env-setup.sh"
echo "2. Build Qt app: mkdir build && cd build && cmake .. && make"
echo "3. Deploy: ./deploy-remarkable.sh"
echo ""
echo "Note: This is a native Qt Quick/QML + C++ application"
echo "      Build using CMake, not npm"
echo ""

