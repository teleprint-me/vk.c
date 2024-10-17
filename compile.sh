#!/usr/bin/env bash

mkdir -p build/examples
cmake -B build -DCMAKE_BUILD_TYPE=Debug 
cmake --build build --config Debug -j "$(nproc)"
