#!/bin/bash

# Check if a preset argument was provided
if [ $# -eq 0 ]; then
    echo "No preset specified. Usage: ./build-clean.sh <preset-name>"
    exit 1
fi

PRESET=$1
BUILD_DIR="build"

# Remove existing build directory
echo "Cleaning build directory..."
rm -rf $BUILD_DIR

# Create a new build directory
mkdir $BUILD_DIR

# Run CMake with the specified preset
echo "Configuring project with preset: $PRESET..."
cmake --preset $PRESET -B $BUILD_DIR

# Build the project
echo "Building project..."
cmake --build $BUILD_DIR

echo "Build process completed."
