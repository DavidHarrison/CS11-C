#include <stdio.h>

int main(void)
{
    /* hopefully, the name is not longer than 100 characters */
    char name[100];
    printf("Enter your name:\n");
    scanf("%99s", name);
    printf("hello, %s\n", name);
    return 1;
}
