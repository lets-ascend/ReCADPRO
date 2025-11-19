#!/bin/bash
# Helper script to download reMarkable SDK
# Usage: ./download-sdk.sh [VERSION]
# Example: ./download-sdk.sh 3.10.5

set -e

VERSION="${1:-}"

if [ -z "$VERSION" ]; then
    echo "Usage: $0 [VERSION]"
    echo ""
    echo "To find your version, run on Mac:"
    echo "  ssh root@10.11.99.1 \"cat /etc/version\""
    echo ""
    echo "Then run:"
    echo "  $0 3.10.5"
    exit 1
fi

FILENAME="meta-toolchain-remarkable-${VERSION}-ferrari-public-x86_64-toolchain.sh"
URL="https://developer.remarkable.com/sdk/${FILENAME}"

echo "Downloading SDK version ${VERSION}..."
echo "URL: ${URL}"
echo ""

# Check if wget is installed
if ! command -v wget &> /dev/null; then
    echo "Installing wget..."
    sudo apt update
    sudo apt install -y wget
fi

# Download
cd ~
wget "${URL}"

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Download successful!"
    echo "File: ~/${FILENAME}"
    echo ""
    echo "Next steps:"
    echo "  chmod u+x ${FILENAME}"
    echo "  ./${FILENAME} -d ~/remarkable-sdk"
else
    echo ""
    echo "❌ Download failed. Check:"
    echo "  1. Version number is correct"
    echo "  2. Visit https://developer.remarkable.com/documentation/sdk to verify URL"
    echo "  3. Check your internet connection"
fi

