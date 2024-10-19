#!/usr/bin/env bash

set -e

BUILD_PATH="build"
BUILD_TYPE="${1:-Debug}"

echo "Cleaning previous build..."
rm -rf "${BUILD_PATH}"

echo "Configuring the project with build type: $BUILD_TYPE"
cmake -B "${BUILD_PATH}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

echo "Building the project..."
cmake --build "${BUILD_PATH}" --config "${BUILD_TYPE}" -j "$(nproc)"

echo "Build completed successfully."
