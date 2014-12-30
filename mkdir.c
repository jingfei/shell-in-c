#include <stdlib.h>
#include <sys/stat.h> // mkdir
#include <stdio.h> // perror

int main(int argc, const char *argv[])
{
	if(!argv[1]) printf("Error, no arguments\n");
	else
		if (mkdir(argv[1],0755) == -1) {
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	return 0;
}
