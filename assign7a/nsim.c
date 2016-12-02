#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

int main()
{
	int fd[9][2];
	
	int x,ndx;
	for (x = 0; x < 9; x++)
	{
		if (pipe(fd[x]) == -1)
		{
			perror("pipe error");
			exit(1);
		}
	}
	for (x = 1; x < 9; x++)
	{
		int fstatus = fork();
		if (fstatus == -1)
		{
			perror("fork error");
			exit(2);
		}
		else if (fstatus == 0)
		{
			close(fd[x][1]);
			ndx = x;
			for (x = 1; x < 9; x++)
				if (x != ndx)
					close(fd[x][0]);
			break;
		}
		else
		{
			close(fd[x][0]);
			ndx = 0;
		}
	}
	if (ndx == 0)
		close(fd[0][1]);
	
	struct pollfd pipefd;
	pipefd.fd = fd[ndx][0];
	pipefd.events = POLLHUP;
	
	int rng, num, j;
	
	srand(1313317*ndx);
	rng = RAND_MAX/9;
	rng *= 9;
	for (x = 0; x < 13; x++)
	{
		do
		{
			num = rand();
			j = num%9;
		}
		while (num >= rng || ndx == j);
		char msg[] = {ndx+'0','\n'};
		write(fd[j][1], msg, sizeof(msg));
	}
	for (x = 0; x < 9; x++)
		close(fd[x][1]);
	
	poll(&pipefd, 1, 1000);
	char buf[1024];
	int r = read(fd[ndx][0], buf, 1024);
	close(fd[ndx][0]);
	char tokbuf[1024];
	char *token;// = tokbuf;
	if (r > 0)
	{
		token = strtok(buf,"\n");
		while (token != NULL && token[0] >= '0' && token[0] <= '9')
		{
			printf("process%d has received a message from process%s\n",ndx,token);
			token = strtok(NULL,"\n");
		}
	}
	
	return 0;
}
