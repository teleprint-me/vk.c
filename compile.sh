#!/usr/bin/env bash

set -e  # Exit immediately if a command exits with a non-zero status.

rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Debug 
cmake --build build --config Debug -j "$(nproc)"
