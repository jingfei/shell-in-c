#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 
#include <ctype.h>
#include <ncurses.h>

char *internal[]={
};

char *external[]={
};

void cd(int myargc, char *myargv[]){
	if(!myargv[1])
		printf("Error, no arguments\n");
	else{
		int Return = chdir(myargv[1]);
		if(Return==-1)
			printf("directory not found\n");
		else{
			char buf[80];
			getcwd(buf, sizeof(buf));
			printf("%s\n",buf);
		}
	}
}

void Mkdir(int myargc, char *myargv[]){
	if(!myargv[1]) printf("Error, no arguments\n");
	else
		if (mkdir(myargv[1],0755) == -1) {
			perror(myargv[0]);
			exit(EXIT_FAILURE);
		}
}

void du(int myargc, char *myargv[]){
	struct stat mystat;
	stat(myargv[1], &mystat);
	int Size = mystat.st_size;
	if(Size>=0)
		printf("%d\n",mystat.st_size);
	else
		printf("file not found\n");
}

void date(){
	time_t current_time;
	char* c_time_string;

	/* Obtain current time as seconds elapsed since the Epoch. */
	current_time = time(NULL);

	if (current_time == ((time_t)-1))
	{
		fprintf(stderr, "Failure to compute the current time.");
		return;
	}

	/* Convert to local time format. */
	c_time_string = ctime(&current_time);

	if (c_time_string == NULL)
	{
		fprintf(stderr, "Failure to convert the current time.");
		return;
	}

	/* Print to stdout. */
	printf("%s", c_time_string);
}

void Echo(int myargc, char *myargv[]){
	int i;
	for(i=1; i<myargc; ++i)
		printf("%s ",myargv[i]);
	printf("\n");
}

void pwd(){
	char buf[80];
	getcwd(buf, sizeof(buf));
	printf("%s\n",buf);
}

void cat(int myargc, char *myargv[]){
	FILE *fPtr;
	char buf[1000];
	fPtr = fopen(myargv[1], "r");
	if(fPtr)
		while(fgets(buf,1000, fPtr)!=NULL)
			printf("%s",buf);
	else
		printf("File open failed\n");
}

void ps(){
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
}

void ls(){
	DIR *mydir;
	struct dirent *myfile;
	char buf[80];
	getcwd(buf, sizeof(buf));

	mydir = opendir(buf);
	while((myfile = readdir(mydir)) != NULL)
		printf(" %s\n", myfile->d_name);
	closedir(mydir);
}

int main(){
	while(1){
		printf(" $$ ");
		char cmd[1000], *myargv[100];
		int myargc=0;
		/* get cmd */
		fgets(cmd, sizeof(cmd), stdin);
		cmd[strlen(cmd)-1]='\0';
		/* sparse cmd */
		myargv[myargc++]=strtok(cmd, " ");
		while(myargv[myargc-1]!=NULL)
			myargv[myargc++]=strtok(NULL, " ");
		--myargc;
		/* judge cmd */
		if(myargc==0) 
			continue;
		else if(!strcmp(myargv[0], "df")){
			if(fork()==0)
				execv("/bin/df", myargv);
			else
				wait(NULL);
		}
		else if(!strcmp(myargv[0], "tar")){
			if(fork()==0)
				execv("/bin/tar", myargv);
			else
				wait(NULL);
		}
		else if(!strcmp(myargv[0], "ps")){
			ps();
		}
		else if(!strcmp(myargv[0], "ls")){
			ls();
		}
		else if(!strcmp(myargv[0], "cat")){
			cat(myargc, myargv);
		}
		else if(!strcmp(myargv[0], "pwd")){
			pwd();
		}
		else if(!strcmp(myargv[0], "echo")){
			Echo(myargc, myargv);
		}
		else if(!strcmp(myargv[0], "date")){
			date();
		}
		else if(!strcmp(myargv[0], "du")){
			du(myargc, myargv);
		}
		else if(!strcmp(myargv[0], "mkdir")){
			Mkdir(myargc, myargv);
		}
		else if(!strcmp(myargv[0], "cd")){
			cd(myargc, myargv);
		}
	//		case "": break;
	//		case "time",
	//		case "cd",
	//		case "hw4"
	//		case "tar": execv("/bin/tar", myargv); break;
	//		case "df": execv("/bin/df", myargv); break;
	//	}
		else if(!strcmp(myargv[0], "exit"))
			break;
		else
			printf("Command not found\n");
	}
	return 0;
}
