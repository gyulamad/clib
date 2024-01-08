#!/bin/bash
unset GTK_PATH
cppcheck . -ibuild -itests/shared \
    --enable=all \
    --suppress=missingInclude \
    --suppress=useStlAlgorithm \
    --error-exitcode=1 && \
cppcheck tests/shared \
    --enable=all \
    --suppress=missingInclude \
    --suppress=unusedFunction \
    --error-exitcode=1 && \
mkdir -p build && \
g++ -g -c builder.cpp -o build/builder.o && \
g++ -g -o build/builder build/builder.o
