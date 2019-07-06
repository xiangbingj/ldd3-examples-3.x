#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stropts.h>
#include <linux/poll.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "scull.h"

int main()
{
	char buffer1[20]={0};
	int pipetest0, pipetest1;
	int code=21, i=0;
	struct pollfd poll_list[2];
	int retval;



	if ((pipetest0 = open("/dev/scullpipe0",O_RDONLY)) < 0)	{
		printf("open scullpipe0 error! \n"); 
		exit(1);
	}
	printf("open scullpipe0 ! \n"); 

	if ((pipetest1 = open("/dev/scullpipe1",O_RDONLY)) < 0)	{
		printf("open scullpipe1 error! \n"); 
		exit(1);
	}
	printf("open scullpipe1 ! \n"); 

	if ( ioctl(pipetest0 ,  SCULL_P_IOCTSIZE , code ) < 0) 	{
		printf("pipetest0 ioctl  SCULL_P_IOCTSIZE error! \n"); 
		exit(1);
	}

	printf(" SCULL_P_IOCTSIZE : scull_p_buffer0=%d !\n" ,ioctl( pipetest0 , SCULL_P_IOCQSIZE , NULL ) );


	if ( ioctl(pipetest1 ,  SCULL_P_IOCTSIZE , code ) < 0) 	{
		printf("pipetest1 ioctl  SCULL_P_IOCTSIZE error! \n"); 
		exit(1);
	}

	printf(" SCULL_P_IOCTSIZE : scull_p_buffer1=%d !\n" ,ioctl( pipetest1 , SCULL_P_IOCQSIZE , NULL ) );

	close(pipetest0);
	printf("close pipetest0 ! \n"); 
	close(pipetest1);
	printf("close pipetest1 ! \n"); 
	if ((pipetest0 = open("/dev/scullpipe0",O_RDONLY)) < 0)	{
		printf("reopen scullpipe0 error! \n"); 
		exit(1);
	}
	printf("reopen scullpipe0 ! \n"); 

	if ((pipetest1 = open("/dev/scullpipe1",O_RDONLY)) < 0)	{
		printf("reopen scullpipe1 error! \n"); 
		exit(1);
	}
	printf("reopen scullpipe1 ! \n"); 


	poll_list[0].fd = pipetest0;
	poll_list[1].fd = pipetest1;
	poll_list[0].events = POLLIN|POLLRDNORM;
	poll_list[1].events = POLLIN|POLLRDNORM;

	while(1)
	{
		retval = poll(poll_list,(unsigned long)2,-1);

		if(retval < 0)
		{
			fprintf(stderr,"poll閿欒: %s\n",strerror(errno));
			return -1;
		}

		if(poll_list[0].revents&(POLLIN|POLLRDNORM)) 
		{
			if ((code=read(pipetest0 , buffer1 , 20)) != 20)
				printf("read from pipetest0 error! code=%d\n",code);
			else
				printf("read from pipetest0 ok! code=%d \n",code);

			for(i=0;i<20;i+=5) 
				printf("[%d]=%c [%d]=%c [%d]=%c [%d]=%c [%d]=%c\n",i,buffer1[i],i+1,buffer1[i+1],i+2,buffer1[i+2],i+3,buffer1[i+3],i+4,buffer1[i+4]);
		}

		if(poll_list[1].revents&(POLLIN|POLLRDNORM)) 
		{
			if ((code=read(pipetest1 , buffer1 , 20)) != 20)
				printf("read from pipetest1 error! code=%d \n",code);
			else
				printf("read from pipetest1 ok! code=%d \n",code);

			for(i=0;i<20;i+=5) 
				printf("[%d]=%c [%d]=%c [%d]=%c [%d]=%c [%d]=%c\n",i,buffer1[i],i+1,buffer1[i+1],i+2,buffer1[i+2],i+3,buffer1[i+3],i+4,buffer1[i+4]);
		}
	}

	close(pipetest0 );
	printf("close pipetest0  ! \n"); 
	close(pipetest1 );
	printf("close pipetest1 ! \n"); 

	printf("\n"); 
	exit(0);
}