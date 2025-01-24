#include <stdio.h>
int sum (int g, int y)
{
    int z;

    z = g + y;
    return (z);
}
int main ()
{
    int g,y,z;
    g = 10;
    y = 20;
    
    z=sum(g,y);

    printf("the sum of x + y = %d",z);
    return 0;
}