#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmpstr(const void *vs1, const void *vs2)
{
	char * const *s1 = vs1;
	char * const *s2 = vs2;
	return strcmp(*s1, *s2);
}

int rjhchdir(char *args)
{
	if(args != NULL)
	{
		printf("You changed directory to %s.\n", args);
	}
	else
	{
		printf("You changed to your home directory.\n");
	}
	return 0;
}

int rjhclear(void)
{
	printf("You cleared the display.\n");
	return 0;
}

int rjhdf(void)
{
	printf("You asked for disk usage info.\n");
	return 0;
}

int rjhdu(char *args)
{
	printf("You asked for directory usage info.\n");
	if(args != NULL)
	{
		printf("Specifically, you wanted the option %s\n", args);
	}
	return 0;
}

int rjhls(char *args)
{
	printf("You asked for a directory listing.\n");
	if(args != NULL)
	{
		printf("Options: %s\n", args);
	}
	return 0;
}

int rjhmkdir(char *args)
{
	if(args != NULL)
	{
		printf("You created a directory, %s\n", args);
	}
	else
	{
		printf("You have to say what "
		"directory you want to create.\n");
	}
	return 0;
}

int rjhsu(char *args)
{
	if(args != NULL)
	{
		printf("You switched user to %s\n", args);
	}
	else
	{
		printf("You are now root.\n");
	}
	return 0;
}

int main(void)
{
	/* ensure this list is sorted! */
	char *command[] =
	{
		"cd",
		"clear",
		"df",
		"du",
		"exit",
		"ls",
		"mkdir",
		"su"
	};
	size_t numcommands = sizeof command / sizeof command[0];
	char line[4096] = {0};
	char safeline[4096] = {0};
	int done = 0;
	int i = 0;
	char *s = line;
	char **t = NULL;
	char *prompt = ">";
	char *args = NULL;
	char *nl = NULL;

	while(!done)
	{
		printf("%s", prompt);
		fflush(stdout);
		if(NULL == fgets(line, sizeof line, stdin))
		{
			t = NULL;
			done = 1;
		}
		else
		{
			nl = strchr(line, '\n');
			if(nl != NULL)
			{
				*nl = '\0';
				strcpy(safeline, line);
			}
			else
			{
				int ch;
				printf("Line too long! Ignored.\n");
				while((ch = getchar()) != '\n' && ch != EOF)
				{
					continue;
				}
				if(ch == EOF)
				{
					done = 1;
				}
			}
			args = strchr(line, ' ');
			if(args != NULL)
			{
				*args++ = '\0';
			}
			t = bsearch(&s,
			command,
			numcommands,
			sizeof command[0],
			cmpstr);
		}

		if(!done && t != NULL)
		{
			i = (int)(t - command);
			switch(i)
			{
				case 0: rjhchdir(args); break;
				case 1: rjhclear(); break;
				case 2: rjhdf(); break;
				case 3: rjhdu(args); break;
				case 4: done = 1; break;
				case 5: rjhls(args); break;
				case 6: rjhmkdir(args); break;
				case 7: rjhsu(args); break;
				default: break;
			}
		}
		else
		{
			printf("%s is not an internal command. "
			"Trying to execute it instead.\n", line);
			system(safeline);
		}
	}

	return 0;
}

