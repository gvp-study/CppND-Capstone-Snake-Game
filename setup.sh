#!/bin/bash

set -e  # Exit immediately if a command fails

# -----------------------------
echo "\n= Setting up the SnakeGame project..."

# Update package lists
echo "\n= Updating system packages..."
#sudo apt-get update

# Install dependencies
echo "\n=æ Installing required libraries..."
#sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev
sudo apt-get install libsdl2-ttf-dev

# Create build directory if not exists
echo "\n<× Creating build directory..."
mkdir -p build
cd build

# Run CMake and build
echo "\n Running CMake and building project..."
cmake ..
make -j$(nproc)

# -----------------------------
echo "\n Setup complete!"
echo "\nTo run the game:"
echo "  ./build/SnakeGame\n"
echo "=
 Have fun playing SnakeGame!"
