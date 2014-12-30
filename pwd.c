#include <stdio.h>

int main(int argc, char *argv[]){
	char buf[80];
	getcwd(buf, sizeof(buf));
	printf("%s\n",buf);
	return 0;
}
