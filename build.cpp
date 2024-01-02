#include <stdio.h>

int main(int argc, const char* argv[]) {
    printf("argc = %d:\n", argc);
    int i = -1;
    while (argv[++i]) printf("argv[%d]: %s\n", i, argv[i]);
    return 0;
}