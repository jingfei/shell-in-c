#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char* argv[])
{
	DIR *mydir;
	struct dirent *myfile;
	FILE *fPtr;
	char pids[1000][2][1000];
	int pid_num=0, i;
	mydir = opendir("/proc");
	while((myfile = readdir(mydir)) != NULL){
		int Digit=1;
		char pid[1000], path[1000];
		strcpy(pid, myfile->d_name);
		int Len=strlen(pid);
		for(i=0; i<Len; ++i)
			if(!isdigit(pid[i])){
				Digit=0;
				break;
			}
		strcpy(path, "/proc/");
		strcat(path, pid);
		strcat(path, "/status");
		if(!Digit) continue;
		fPtr = fopen(path, "r");
		fgets(pids[pid_num][1], 1000, fPtr);
		//		strcpy(pids[pid_num][1], pids[pid_num][1]+6);
		int Len2=strlen(pids[pid_num][1])-7;
		strncpy(pids[pid_num][1], pids[pid_num][1]+6, Len2);
		pids[pid_num][1][Len2]='\0';
		strcpy(pids[pid_num][0], pid);
		++pid_num;
	}
	closedir(mydir);
	printf("  PID\tCMD\n");
	for(i=0; i<pid_num; ++i){
		printf("%5s\t%s\n", pids[i][0], pids[i][1]);
	}
	return 0;
}

