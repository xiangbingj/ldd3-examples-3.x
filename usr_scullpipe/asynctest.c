#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

int gotdata=0;
void sighandler(int signo)
{
	if (signo==SIGIO)
		gotdata++;
	return;
}

char buffer[21];

int main(int argc, char **argv)
{
	int pipetest0;
	int count;
	struct sigaction action;

	if ((pipetest0 = open("/dev/scullpipe0",O_RDONLY)) < 0)	{
		printf("open scullpipe0 error! \n"); 
		exit(1);
	}

	memset(&action, 0, sizeof(action));
	action.sa_handler = sighandler;
	action.sa_flags = 0;

	sigaction(SIGIO, &action, NULL);

	fcntl(pipetest0, F_SETOWN, getpid());
	fcntl(pipetest0, F_SETFL, fcntl(pipetest0, F_GETFL) | FASYNC);

	while(1) {
		/* this only returns if a signal arrives */
		sleep(86400); /* one day */
		if (!gotdata)
			continue;
		count=read(pipetest0, buffer, 21);
		/* buggy: if avail data is more than 4kbytes... */
		write(1,buffer,count);
		gotdata=0;
		break;
	}

	close(pipetest0 );
	printf("close pipetest0  ! \n"); 

	printf("exit !\n"); 
	exit(0);
}