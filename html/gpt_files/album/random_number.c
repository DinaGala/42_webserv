#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define H1 "<h1>Random Number</h1>"
#define ENV "<p>No input found.\n Generating a number between 0 and 100</p>"
#define QUERY "<p>If you want to set a maximum, add a query at the end of the url: \"?input=number\"</p>"
#define RESULT "<p>Your random number is</p>"

int main(int argc, char **argv, char **env)
{
	printf("Content-Type: text/html\n\n");
	printf("<html>%s", H1);
	if (!env || !env[0])
	{
		printf("<p>What is going on?! No environment passed</p></html>");
		return (0);
	}
	int i = 0;
	while (env[i] && strncmp("QUERY_STRING=", env[i], strlen("QUERY_STRING=")) != 0)
		i++;
	if (!env[i] || !env[i][0])
		printf(ENV QUERY);
	printf(RESULT);
	srand(time(0));
	int	num = rand() % 100;
	printf("<p>%d<p></html>", num);
}
