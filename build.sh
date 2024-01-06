unset GTK_PATH
cppcheck . -ibuild \
    --enable=all \
    --suppress=missingInclude \
    --suppress=useStlAlgorithm \
    --error-exitcode=1 && \
mkdir -p build && \
g++ -c builder.cpp -o build/builder.o && \
g++ -o build/builder build/builder.o