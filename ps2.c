#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <regex.h>
#include <unistd.h>

//prototypes
int matchDetails(char* filename,char* path);
char *get_regerror(int errcode,regex_t *compiled);

int main(void){
	int result,count,index;
	char path[] = "/cygdrive/c/PROGRA~1/RATIONAL/RATION~1/NUTCROOT/usr/temp";
	struct dirent **direntp;
	struct stat sbuf;

	if (stat(path,&sbuf) != -1){
		if(S_ISDIR(sbuf.st_mode)){
			count = scandir(path,&direntp,NULL,alphasort);
			if(count){
				char temp[FILENAME_MAX];
				for(index=0; index<count; ++index){
					strcpy(temp,path);
					result = matchDetails(direntp[index]->d_name,temp);
					printf("%d\n",result);
					free(direntp[index]);
				}
			}else{
				fprintf(stderr,"Can't open %s\n",path);
				exit(EXIT_FAILURE);
			}
		}else{
			fprintf(stderr,"Can't find process directories\n");
			return;
		}
	}else{
		fprintf(stderr,"Can't stat %s\n",path);
		return;
	} 
	return EXIT_SUCCESS;
}

int matchDetails(char* filename,char* path){
	int status,regstat;
	char *pattern = "[0-9]"; //Match digits only
	char *result;
	struct stat sbuf;

	//Get the full pathname
	strcat(path,"/");
	strcat(path,filename);

	//Stat the full path name
	if(stat(path,&sbuf) != -1){
		if(S_ISDIR(sbuf.st_mode)){ //If a directory
			regex_t re;
			if((regstat = regcomp(&re,pattern,REG_EXTENDED)) != 0){
				result = get_regerror(regstat,&re);
				fprintf(stderr,"%s\n",result);
				return(0);
			}
			if((status = regexec(&re,filename,re.re_nsub,NULL,0)) != 0){
				result = get_regerror(status,&re);
				fprintf(stderr,"%s\n",result);
				return(0);
			}
			regfree(&re);
		}else{
			fprintf(stderr,"Can't find process directories\n");
			return;
		}
	}else{
		fprintf(stderr,"Can't stat %s\n",filename);
		return;
	}
	return(1);

}

char *get_regerror(int errcode,regex_t *compiled){
	size_t length = regerror(errcode,compiled,NULL,0);
	char *buffer = malloc(length);
	(void) regerror(errcode,compiled,buffer,length);
	return buffer;
}
