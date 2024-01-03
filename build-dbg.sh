#!/bin/bash
unset GTK_PATH
cppcheck . \
    --enable=all \
    --suppress=missingInclude \
    --suppress=useStlAlgorithm \
    --error-exitcode=1 && \
mkdir -p build && \
g++ -g -c build.cpp -o build/build.o && \
g++ -g -o build/build build/build.o
