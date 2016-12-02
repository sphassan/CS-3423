#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int funct2(char *prog1, char *prog2)
{
	int fd[2];
	if (pipe(fd) == -1)
	{
		perror("pipe error");
		exit(3);
	}
	
	switch (fork())
	{
		case -1 :
			perror("fork error");
			exit(1);
		case 0 : //child code
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			execlp(prog1,prog1,NULL);
			perror("exec error");
			exit(2);
		default : //parent code
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
	}
	
	execlp(prog2,prog2,NULL);
	perror("exec error");
	exit(2);
}
