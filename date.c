#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	time_t current_time;
	char* c_time_string;

	/* Obtain current time as seconds elapsed since the Epoch. */
	current_time = time(NULL);

	if (current_time == ((time_t)-1))
	{
		fprintf(stderr, "Failure to compute the current time.");
		return 0;
	}

	/* Convert to local time format. */
	c_time_string = ctime(&current_time);

	if (c_time_string == NULL)
	{
		fprintf(stderr, "Failure to convert the current time.");
		return 0;
	}

	/* Print to stdout. */
	printf("%s", c_time_string);
	return 0;
}
