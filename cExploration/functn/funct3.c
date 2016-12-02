/*
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
*/
int funct3(char *prog1, char *pargv[], char flag)
{
	int fd[2];
	if (pipe(fd) == -1)
	{
		perror("pipe error");
		exit(1);
	}
	
	switch (fork())
	{
		case -1 :
			perror("fork error");
			exit(2);
		case 0 : //child code
			if (flag == 'r')
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
			}
			else
			{
				close(fd[1]);
				dup2(fd[0], STDIN_FILENO);
			}
			execvp(prog1,pargv);
			perror("exec error");
			exit(2);
		default : //parent code
			if (flag == 'r')
			{
				close(fd[1]);
				dup2(fd[0], STDIN_FILENO);
			}
			else
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
			}
	}
	
	if (flag == 'r')
		return fd[0];
	return fd[1];
}
/*
int main()
{
	char *argv[] = {"ls", NULL};
	int fd = funct3("ls",argv,'w');
	close(fd);
	return 0;
}
*/
