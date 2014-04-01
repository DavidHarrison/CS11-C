#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    char name[100];
    int rand_int;
    int i;
    srand(time(0));
    rand_int = (int) (((double) rand() / (double) RAND_MAX) * 10) + 1;
    printf("Enter name:\n");
    scanf("%s", name);
    for (i = 0; i < rand_int; i++)
    {
        if (rand_int % 2 == 0)
        {
            printf("Hello, %s\n", name);
        } else {
            printf("Hi there, %s\n", name);
        }
    }
    return 1;
}
