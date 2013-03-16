#include <stdio.h>

int
main(int argc, char *argv[])
{
    char s[1024];
    fgets(s, 1024, stdin);
    printf("%s\n", s);
    return 0;
}

