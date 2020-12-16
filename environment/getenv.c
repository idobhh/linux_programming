/* Show value of specified evriroment variables */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int i;
	char *val;

	for(i=1; i<argc; i++)
	{
		if((val = getenv(argv[i])) != NULL) 
			printf("%s: %s\n", argv[i], val);
		else
			printf("%s: not in the environment\n",argv[i]);
	}
}
