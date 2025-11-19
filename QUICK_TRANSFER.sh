#!/bin/bash
# Quick transfer script - Run this on your MAC
# This creates a simple HTTP server to transfer files to VM

echo "=========================================="
echo "File Transfer Server for Ubuntu VM"
echo "=========================================="
echo ""

# Get Mac's IP address
MAC_IP=$(ifconfig | grep "inet " | grep -v 127.0.0.1 | head -1 | awk '{print $2}')
if [ -z "$MAC_IP" ]; then
    MAC_IP=$(ipconfig getifaddr en0 2>/dev/null || echo "192.168.1.100")
fi

echo "Your Mac's IP address: $MAC_IP"
echo ""
echo "In your Ubuntu VM, run these commands:"
echo ""
echo "cd ~/remarkable-sdk/recadpro"
echo "wget http://$MAC_IP:8000/create-minimal-qt5-cmake.sh"
echo "wget http://$MAC_IP:8000/create-qt5-config.sh"
echo "wget http://$MAC_IP:8000/check-qt5.sh"
echo "wget http://$MAC_IP:8000/find-qt5-deep.sh"
echo "wget http://$MAC_IP:8000/AUTO_FIX_QT5.sh"
echo "chmod +x *.sh"
echo ""
echo "Starting HTTP server on port 8000..."
echo "Press Ctrl+C to stop when done"
echo ""

cd ~/Remarkable_app
python3 -m http.server 8000

