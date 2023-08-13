#!/bin/bash

# Exit on any error
set -e

# Create build directory if it doesn't exist
mkdir -p build

cd build

# Run CMake to generate build files and build the project

cmake ..
make

echo "Build completed successfully!"