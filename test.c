/* execve.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	execv("/usr/bin/which", argv);
	perror("execve");   /* execve() only returns on error */
	exit(EXIT_FAILURE);
}

