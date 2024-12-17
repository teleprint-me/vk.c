#!/usr/bin/env bash

# ---------------------------------------------------------
# Script: validate.sh
# Purpose:
#   - Validate memory safety using Valgrind.
#   - Optionally bypass AddressSanitizer (ASAN) link order verification.
#   - Run Valgrind in isolation without modifying other scripts.
#
# Why:
#   - ASAN sometimes reports false positives due to external libraries
#     (e.g., Vulkan validation layers, ICU).
#   - Valgrind acts as a backup to verify true memory safety.
#
# Usage:
#   ./validate.sh <program> [args...]
#
# Options:
#   - Set ASAN_OPTIONS to bypass link verification if needed.
#   - Set LSAN_OPTIONS to suppress known harmless leaks.
#
# ---------------------------------------------------------

# Suppression file for known false positives
SUPPRESSION_FILE="$(pwd)/asan.supp"

# Enable or disable ASAN link order bypass (set to 1 to enable)
BYPASS_ASAN_LINK_ORDER=0

# Configure ASAN options
if [[ $BYPASS_ASAN_LINK_ORDER -eq 1 ]]; then
    ASAN_OPTIONS="verify_asan_link_order=0"
else
    ASAN_OPTIONS=""
fi

# Configure LSAN options (quietly suppress known false positives)
LSAN_OPTIONS="suppressions=$SUPPRESSION_FILE"

# Preload AddressSanitizer runtime library
LD_PRELOAD=$(gcc -print-file-name=libasan.so)

# Export options to environment
export LD_PRELOAD ASAN_OPTIONS LSAN_OPTIONS

# Check for input arguments
if [[ $# -lt 1 ]]; then
    echo "Usage: $0 <program> [args...]"
    exit 1
fi

# Run the program with Valgrind for memory leak checks
valgrind --leak-check=full --show-leak-kinds=all "$@"
