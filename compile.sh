#!/usr/bin/env bash

set -e

BUILD_PATH="build"
BUILD_TYPE="${1:-Debug}"
BUILD_DOCS="${2:-ON}" # Default to ON, can be set to OFF

echo "Cleaning previous build..."
rm -rf "${BUILD_PATH}"

echo "Configuring the project with build type: $BUILD_TYPE and BUILD_DOCS=${BUILD_DOCS}"
cmake -B "${BUILD_PATH}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" -DBUILD_DOCS=${BUILD_DOCS}

echo "Building the project..."
cmake --build "${BUILD_PATH}" --config "${BUILD_TYPE}" -j "$(nproc)"

if [ "${BUILD_DOCS}" = "ON" ] && cmake -LAH | grep -q 'DOXYGEN_FOUND:BOOL=TRUE'; then
    echo "Building documentation with Doxygen..."
    cmake --build "${BUILD_PATH}" --target doc_doxygen
    echo "Documentation generated at ${BUILD_PATH}/docs/html/index.html"
else
    echo "Documentation build skipped."
fi

echo "Build completed successfully."
