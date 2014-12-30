#include <stdio.h>

int main(int argc, char* argv[]){
	FILE *fPtr;
	char buf[1000];
	fPtr = fopen(argv[1], "r");
	if(fPtr)
		while (fgets(buf,1000, fPtr)!=NULL)
			printf("%s",buf);
	else
		printf("File open failed\n");
	return 0;
}
