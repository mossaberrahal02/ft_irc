#include <stdio.h>
#include <stdlib.h>


int main()
{
	char *t = malloc(1024);
	printf("%ld\n", sizeof(t));
	free(t);
	return 0;
}