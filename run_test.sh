#!/bin/bash
# run_tests.sh
# A cross-platform shell script to build and run the AVL test suite.
# This script works on Linux, macOS, and Windows environments that support Bash.

# Check if the executable "avlTree" exists and is executable.
if [ ! -x "./avlTree" ]; then
    echo "Executable 'avlTree' not found or not executable. Building using make..."
    make || { echo "Build failed!"; exit 1; }
fi

echo "Running the AVL test suite..."
./avlTree

echo "Test suite execution finished."
