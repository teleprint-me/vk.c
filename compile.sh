#!/usr/bin/env bash

rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Debug 
cmake --build build --config Debug -j "$(nproc)"
