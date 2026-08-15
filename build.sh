#!/bin/bash

set -Eeuo pipefail

DEBUG_MODE=false

while getopts :"d" option; do
    case ${option} in
        "d")
            DEBUG_MODE=true
        ;;
    esac
done

mkdir -p build
mkdir -p games

if [[ $DEBUG_MODE = true ]]; then
    echo Building app - Debug
    mkdir -p build/debug
    cmake --preset debug
    cmake --build build/debug --preset debug --target dice_counter --parallel
    cp build/debug/dice_counter .
else
    echo Building app - Release
    mkdir -p build/release
    cmake --preset release
    cmake --build build/release --preset release --target dice_counter --parallel
    cp build/release/dice_counter .
fi