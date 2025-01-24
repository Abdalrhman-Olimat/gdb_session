#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

int main ()
{
    int num = 0;
    int *ptr = malloc (sizeof(int));
    int *badptr = NULL;

    srand(time(NULL));

    while (true)
    {
        num = rand () % 255;
        if (num == 13 )
            *badptr = num;
        else
            *ptr = num;
        printf("random : %d\n",num);
        sleep(1);
    }
}