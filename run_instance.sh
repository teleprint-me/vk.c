#!/bin/bash

# ---------------------------------------------------------
# Wrapper Script: run_instance.sh
# Purpose:
#   - Suppress false positive memory leaks reported by ASAN.
#   - Preload ASAN library and set LSAN options to handle
#     known Vulkan validation layer quirks.
#
# Why?
#   AddressSanitizer reports a small memory leak (128 bytes)
#   during program exit caused by pthread_once initialization
#   in Vulkan Validation Layers or ICU libraries.
#
#   This leak is harmless but unavoidable for now due to:
#   - External libraries not being fully instrumented by ASAN.
#   - ASAN failing to handle certain multi-threaded library
#     initializations safely.
#
# How It Works:
#   - LSAN suppresses specific leak patterns using a suppression file.
#   - LD_PRELOAD ensures ASAN runtime is linked before execution.
#
# Reference:
#   - Vulkan SDK Validation Layers: https://vulkan.lunarg.com/
#   - ASAN Docs: https://clang.llvm.org/docs/AddressSanitizer.html
# ---------------------------------------------------------

# Suppression File (created to ignore false positive leaks)
SUPPRESSION_FILE="$(pwd)/asan.supp"

# Debug Mode (verbose logs for troubleshooting)
# Uncomment the following line if you need detailed LSAN output:
# LSAN_OPTIONS="suppressions=$SUPPRESSION_FILE:verbosity=1:log_threads=1"

# Normal Mode (suppresses known false positives)
LSAN_OPTIONS="suppressions=$SUPPRESSION_FILE"

# Link the AddressSanitizer runtime
LD_PRELOAD=$(gcc -print-file-name=libasan.so)

# Export environment variables
export LSAN_OPTIONS LD_PRELOAD

# Notes for Future Maintenance:
#   - If new leaks appear, add their patterns to asan.supp.
#   - Use 'LSAN_OPTIONS=verbosity=1' for detailed analysis.
#   - Valgrind can be used as a backup tool if needed.

# Run the Vulkan example binary
./build/examples/instance
