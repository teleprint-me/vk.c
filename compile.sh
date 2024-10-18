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

# Check if Doxygen is available and build documentation
if cmake -LAH | grep -q 'DOXYGEN_FOUND:BOOL=TRUE'; then
    echo "Building documentation with Doxygen..."
    cmake --build "${BUILD_PATH}" --target doc_doxygen
    echo "Documentation generated at ${BUILD_PATH}/docs/html/index.html"
else
    echo "Doxygen not found. Skipping documentation generation."
fi

echo "Build completed successfully."
