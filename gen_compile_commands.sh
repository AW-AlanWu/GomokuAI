#!/bin/bash
set -e

rm -f compile_commands.json
rm -rf .cache

make clean
bear -- make -j$(nproc)
