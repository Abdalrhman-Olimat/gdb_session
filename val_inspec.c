#include <stdio.h>

int main ()
{
	int x = 0;
	int y = 0;
	int z = 0;

	x+=1;
	printf("\n");
	printf("\n");
	printf("\n");
	
	for(int i = 0; i < 10; i++)
	{
		if(i%2 == 0)
			y++;
		else
			z++;
		x+2;
	}
	return 0;
}
