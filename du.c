#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	struct stat mystat;
	stat(argv[1], &mystat);
	int Size = mystat.st_size;
	if(Size>=0)
		printf("%d\n",mystat.st_size);
	else
		printf("file not found\n");
	return 0;
}
