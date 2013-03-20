#include <stdio.h>
#include <stdlib.h>

/* char g[33554432]; */

int
main(int argc, char *argv[])
{
    char s[100];
    scanf("%s", s);
    printf("%s\n", s);
    malloc(33554432);
    return 0;
}
