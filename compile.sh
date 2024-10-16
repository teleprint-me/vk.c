#!/usr/bin/env bash

mkdir -p build/examples
gcc -o build/examples/simple examples/simple.c -lvulkan
