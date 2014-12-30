#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
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

void SetColor(){
	start_color();
	init_color(COLOR_BLACK,128,128,128);
	init_pair(1,COLOR_RED,COLOR_RED); //wall
	init_pair(2,COLOR_BLUE,COLOR_BLUE);
	init_pair(3,COLOR_GREEN,COLOR_GREEN);
	init_pair(4,COLOR_YELLOW,COLOR_YELLOW);
	init_pair(5,COLOR_MAGENTA,COLOR_MAGENTA);
	init_pair(6,COLOR_CYAN,COLOR_CYAN);
	init_pair(7,COLOR_WHITE,COLOR_WHITE);
	init_pair(8,COLOR_BLACK,COLOR_WHITE);
}

void Init(){
	initscr();
//	SetColor();
	echo();
	keypad(stdscr,TRUE);
	idlok(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	wsetscrreg(stdscr,9, 29);
//	nodelay(stdscr,TRUE);
}

void cd(int myargc, char *myargv[]){
	if(!myargv[1])
		printw("Error, no arguments\n");
	else{
		int Return = chdir(myargv[1]);
		if(Return==-1)
			printw("directory not found\n");
		else{
			char buf[80];
			getcwd(buf, sizeof(buf));
			printw("%s\n",buf);
		}
	}
}

void Mkdir(int myargc, char *myargv[]){
	if(!myargv[1]) printw("Error, no arguments\n");
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
		printw("%d\n",mystat.st_size);
	else
		printw("file not found\n");
}

void date(){
	time_t current_time;
	struct tm *timeinfo;
	time(&current_time);
	timeinfo = localtime(&current_time);
	printw("%s\n",asctime(timeinfo));
}

void Echo(int myargc, char *myargv[]){
	int i;
	for(i=1; i<myargc; ++i)
		printw("%s ",myargv[i]);
	printw("\n");
}

void pwd(){
	char buf[80];
	getcwd(buf, sizeof(buf));
	printw("%s\n",buf);
}

void cat(int myargc, char *myargv[]){
	FILE *fPtr;
	char buf[1000];
	fPtr = fopen(myargv[1], "r");
	if(fPtr)
		while(fgets(buf,1000, fPtr)!=NULL)
			printw("%s",buf);
	else
		printw("File open failed\n");
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
	printw("  PID\tCMD\n");
	for(i=0; i<pid_num; ++i){
		printw("%5s\t%s\n", pids[i][0], pids[i][1]);
	}
}

void ls(){
	DIR *mydir;
	struct dirent *myfile;
	char buf[80];
	getcwd(buf, sizeof(buf));

	mydir = opendir(buf);
	while((myfile = readdir(mydir)) != NULL)
		printw(" %s\n", myfile->d_name);
	closedir(mydir);
}

int main(){
	Init();
	char cmds[1000][1000], cmdn=0;
	while(1){
		cbreak();
		printw(" $$ ");
		char cmd[1000], *myargv[100];
		int ch, up=0;
		int myargc=0;
		/* get cmd */
		int cn=0;
		while( (ch = getch()) != '\n'){
			if(ch == KEY_UP){
				int y,x;
				getyx(stdscr,y,x);
				move(y,0);
				clrtoeol();
				printw(" $$ ");
				if(cmdn-up!=0) ++up;
				strcpy(cmd, cmds[cmdn-up]);
				printw("%s",cmd);
				cn = strlen(cmd);
			}
			else if(ch == KEY_DOWN){
				int y,x;
				getyx(stdscr,y,x);
				move(y,0);
				clrtoeol();
				printw(" $$ ");
				if(up!=0) --up;
				strcpy(cmd, cmds[cmdn-up]);
				printw("%s",cmd);
				cn = strlen(cmd);
			}
			else if(ch == KEY_BACKSPACE){
				delch();
				--cn;
			}
			else cmd[cn++]=ch;
		}
		cmd[cn]='\0';
		strcpy(cmds[cmdn++],cmd);
		nocbreak();
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
	//		case "time",
	//		case "cd",
	//		case "hw4"
		else if(!strcmp(myargv[0], "exit"))
			break;
		else
			printw("Command not found\n");
	}
	endwin();
	return 0;
}
