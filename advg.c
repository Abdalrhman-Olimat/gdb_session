#include <stdio.h>

#define log(x) printf("value of %s is %d\n",#x,x)

int main ()
{
	int p = 42;
	int* px = &p;

	p = 32;
	log(p);
	*px=57;
	printf("p is %d\n",p);

	return 0;
}
