#!/bin/bash
g++ build.cpp -o build/build && \
./build/build "$@"