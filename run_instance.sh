#!/bin/bash

# Debug: Increase verbosity for deeper inspection
#LSAN_OPTIONS="suppressions=$(pwd)/asan.supp:verbosity=1:log_threads=1"

# Normal: Allow program to run with ignore rules enabled.
LSAN_OPTIONS="suppressions=$(pwd)/asan.supp"

# Link libasan to the binary.
LD_PRELOAD=$(gcc -print-file-name=libasan.so)

# Export the CLI options
export LSAN_OPTIONS LD_PRELOAD

# Run the binary
./build/examples/instance
