#!/bin/bash

# Deployment script for reMarkable Paper Pro
# Follows official reMarkable development guidelines

set -e

DEVICE_IP="${1:-10.11.99.1}"
APP_NAME="drafting-pro"
INSTALL_PATH="/opt/drafting-pro"
BUILD_DIR="build"

echo "=========================================="
echo "Deploying to reMarkable Paper Pro"
echo "=========================================="
echo ""

# Check if SDK is sourced
if [ -z "$CC" ]; then
    echo "ERROR: SDK environment not sourced!"
    echo "Please run: source env-setup.sh"
    exit 1
fi

echo "✓ SDK environment detected"
echo "  Cross-compiler: $CC"
echo ""

# Build the application
echo "Building application..."
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"
cmake ..
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo "✓ Build successful"
echo ""

# Check if binary exists
if [ ! -f "$APP_NAME" ]; then
    echo "ERROR: Binary not found: $BUILD_DIR/$APP_NAME"
    exit 1
fi

# Create deployment directory on device
echo "Creating deployment directory on device..."
ssh root@$DEVICE_IP "mkdir -p $INSTALL_PATH"

# Copy application
echo "Copying application to device..."
scp $APP_NAME root@$DEVICE_IP:$INSTALL_PATH/

# Copy any resources if needed
if [ -d "../resources" ]; then
    echo "Copying resources..."
    scp -r ../resources root@$DEVICE_IP:$INSTALL_PATH/
fi

echo ""
echo "=========================================="
echo "Deployment complete!"
echo "=========================================="
echo ""
echo "To run the application on your reMarkable Paper Pro:"
echo ""
echo "1. SSH into device:"
echo "   ssh root@$DEVICE_IP"
echo ""
echo "2. Stop xochitl (main interface):"
echo "   systemctl stop xochitl"
echo ""
echo "3. Run the application:"
echo "   cd $INSTALL_PATH"
echo "   QT_QUICK_BACKEND=epaper ./$APP_NAME -platform epaper"
echo ""
echo "4. After testing, restart xochitl:"
echo "   systemctl start xochitl"
echo ""
echo "Note: The application uses the e-paper backend for optimal"
echo "      display performance on reMarkable Paper Pro."
echo ""

