/*
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
*/
int funct4(char *prog1, char *pargv[], int *fdread, int *fdwrite)
{
	int fd[2][2];
	if (pipe(fd[0]) == -1)
	{
		perror("pipe error");
		exit(1);
	}
	if (pipe(fd[1]) == -1)
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
			close(fd[0][0]);
			dup2(fd[1][1], STDOUT_FILENO);
			close(fd[0][1]);
			dup2(fd[1][0], STDIN_FILENO);
			execvp(prog1,pargv);
			perror("exec error");
			exit(2);
		default : //parent code
			close(fd[1][1]);
			dup2(fd[0][0], STDIN_FILENO);
			close(fd[1][0]);
			dup2(fd[0][1], STDOUT_FILENO);
	}
	
	fdread = &fd[0][0];
	fdwrite = &fd[0][1];
	
	return 0;
}
/*
int main()
{
	int fdread, fdwrite;
	char *argv[] = {"ls", NULL};
	funct4("ls",argv,&fdread,&fdwrite);
	sleep(10);
	return 0;
}
*/
