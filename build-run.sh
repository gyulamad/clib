#!/bin/bash


# --suppress=cstyleCast \
# --suppress=unusedFunction \
# --suppress=noCopyConstructor \
# --suppress=noOperatorEq \

unset GTK_PATH
cppcheck . \
    --enable=all \
    --suppress=missingInclude \
    --suppress=useStlAlgorithm \
    --error-exitcode=1 && \
mkdir -p build && \
g++ -c build.cpp -o build/build.o && \
g++ -o build/build build/build.o && \
./build/build "$@"