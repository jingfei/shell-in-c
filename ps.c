#include <dirent.h> 
#include <sys/procfs.h> 
int fdproc; 
DIR *dirp; 
struct prpsinfo pinfo; 
int main(){
/* 
 * open the /proc directory for reading process statuses 
 */ 
if ((dirp = opendir("/proc")) == (DIR *)NULL) { 
	perror("/proc"); 
	exit(1); 
} 
/* 
 * loop for each process in the system 
 */ 
while(dirent = readdir(dirp)) { 
	if (dirent->d_name[0] != '.') { 
		strcpy(procbuf, "/proc/"); 
		strcat(procbuf, dirent->d_name); 
		if ((fdproc = open(procbuf, O_RDONLY)) < 0) { 
			continue; 
		} 
		/* 
		 * get the ps status for the process 
		 */ 
		if (ioctl(fdproc, PIOCPSINFO, &pinfo) < 0) { 
			close(fdproc); 
			continue; 
		} 
		/* process the pinfo stuff here, see 
		   /usr/include/sys/procfs.h for details */ 
		close(fdproc); 
	} 
} 
return 0;
}
