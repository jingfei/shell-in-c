#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
	DIR *mydir;
	struct dirent *myfile;
	char buf[80];
	getcwd(buf, sizeof(buf));

	mydir = opendir(buf);
	while((myfile = readdir(mydir)) != NULL)
		printf(" %s\n", myfile->d_name);
	closedir(mydir);
	return 0;
}
