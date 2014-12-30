#include <stdio.h>

int main(int argc, char* argv[]){
	if(!argv[1])
		printf("Error, no arguments\n");
	else{
		int Return = chdir(argv[1]);
		if(Return==-1)
			printf("directory not found\n");
		else{
			char buf[80];
			getcwd(buf, sizeof(buf));
			printf("%s\n",buf);
		}
	}

	return 0;
}
