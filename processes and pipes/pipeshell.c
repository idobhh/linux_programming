#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CHAR_LENGTH 100

/*  This function prints a prompt, reads in a command
	line, and parses it, placing pointers to the
	individual tokens (substrings) prior to a "|" token 
	into the upstream array (u), and tokens following 
	a "|" token (if any) into the downstream array (d). 
	Both arrays are terminated by a NULL pointer
*/
int prompt_and_parse(char **u, char **d)
{
	static char line[MAX_CHAR_LENGTH];

	d[0] = NULL;
	printf("basicshell> ");
	if (fgets(line, MAX_CHAR_LENGTH,stdin) == NULL)
		return -1;
	/*
	int len=strlen(line);
   	line[len-1]='\0';
	*/
	*(strchr(line, '\n')) = '\0'; // remove '\n' at the end of line

	*u++ = strtok(line, " \t"); // delim is ' ' and '\t'
	// strtok(NULL, " \t") continue to the next substring untill reach null
	while (*u = strtok(NULL, " \t")) {
		if (strcmp(*u, "|") == 0) {
			*u = NULL;
			while (*d++ = strtok(NULL, " \t"))
				/* Empty body */;
			return 1;
		}
		u++;
	}
	return 1;
}

int main()
{
	int p[2];
	char *upstream[20], *downstream[20];

	/* Main command loop */
	while (prompt_and_parse(upstream, downstream) > 0) {
		/*  If downstream[0] is NULL, there is no pipeline 
			so we just execute the upstream command.
		*/
		if (downstream[0] == NULL) {
			if (fork() == 0) { /* Child */
				execvp(upstream[0], upstream);
				printf("%s: not found\n",upstream[0]);
				exit(1);
			}
			else { /* Parent */
				wait(0);
			}
		}
		else {
			/*  There is a pipeline so we need to fork
				twice to run the downstream and upstream
				commands concurrently with a pipe between 
				them
			*/
			pipe(p);
			if (fork() == 0) { /* Upstream child */
				/* Connect stdout to upstream end of pipe */
				dup2(p[1], 1);
				/* Close downstream end of pipe */
				close(p[0]);
				/* Execute the upstream command */
				execvp(upstream[0], upstream);
				printf("%s: not found\n",upstream[0]);
				exit(1);
			}
			if (fork() == 0) { /* Downstream child */
				/* Connect stdin to downstream end of pipe */
				dup2(p[0], 0);
				/* Close upstream end of pipe */
				close(p[1]);
				/* Execute the upstream command */
				execvp(downstream[0], downstream);
				printf("%s: not found\n",downstream[0]);
				exit(1);
			}
			else { /* Parent */
				/* Close both ends of pipe */
				close(p[0]);
				close(p[1]);
				/* Wait for both children to finish */
				wait(0);
				wait(0);
			}
		}
	}
	printf("\r\nBye!\n");	
}
