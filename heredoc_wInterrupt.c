#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>
#include <stdlib.h>

int g_sig;

void msh_sigint_handler(int sig) {
	g_sig = sig;
}

int msh_eventhook (void)
{
	if (g_sig == SIGINT)
		rl_done = 1;
	return (0);
}

int main() {
    struct sigaction sa = {0};
    sa.sa_handler = msh_sigint_handler;
    sigaction(SIGINT, &sa, NULL);   // For Ctrl-C, for example

	rl_event_hook = msh_eventhook;

	char *line = "";
	while (1)
	{
		g_sig = 0;
    	line = readline("prompt> ");
    	if (!line)
        	printf("Got EOF/interrupt\n");
		else 
		{
			if (!strcmp(line, "unsethook"))
				rl_event_hook = NULL;
			else if (!strcmp(line, "stop"))
				break;
			else
		    	printf("You typed: '%s'\n", line);
	    	free(line);
		}
	}
	return 0;
}

