#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSZ 1024

struct cmd
{
	int redirect_in;     /* Any stdin redirection?         */
	int redirect_out;    /* Any stdout redirection?        */
	int redirect_append; /* Append stdout redirection?     */
	int background;      /* Put process in background?     */
	int piping;          /* Pipe prog1 into prog2?         */
	char *infile;        /* Name of stdin redirect file    */
	char *outfile;       /* Name of stdout redirect file   */
	char *argv1[10];     /* First program to execute       */
	char *argv2[10];     /* Second program in pipe         */
};

int main()
{
	char buf[BUFSZ];
	struct cmd cmds;
	int fd[2];
	
	printf(">> ");
	fgets(buf, BUFSZ, stdin);
	buf[strcspn(buf, "\r\n")] = 0;
	
	while (strcmp(buf,"exit") != 0)
	{
		if (cmdscan(&buf, &cmds) == -1) //should probably not kill the shell just because of this
		{
			printf("parse error\n");
			exit(1);
		}
		if (cmds.piping)
		{
			if (pipe(fd) == -1) //ditto
			{
				perror("pipe error");
				exit(2);
			}
		}
		switch (fork())
		{
			case -1:
				perror("fork error");
				exit(3);
			case 0: //child code
				if (cmds.redirect_in)
				{
					int fdin = open(cmds.infile, O_RDONLY);
					dup2(fdin,STDIN_FILENO);
				}
				if (cmds.redirect_out && cmds.redirect_append)
				{
					int fdout = open(cmds.outfile, O_WRONLY | O_CREAT | O_APPEND, 0600);
					dup2(fdout,STDOUT_FILENO);
				}
				else if (cmds.redirect_out)
				{
					int fdout = open(cmds.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
					dup2(fdout,STDOUT_FILENO);
				}
				if (!cmds.background)
				{
					if (!cmds.piping)
					{
						execvp(cmds.argv1[0], cmds.argv1);
						perror("exec error");
						exit(4);
					}
					else
					{
						switch (fork())
						{
							case -1:
								perror("fork error");
								exit(3);
							case 0: //grandchild code
								close(fd[0]);
								dup2(fd[1], STDOUT_FILENO);
								execvp(cmds.argv1[0], cmds.argv1);
								perror("exec error");
								exit(4);
							default: //child code
								close(fd[1]);
								dup2(fd[0], STDIN_FILENO);
								execvp(cmds.argv2[0], cmds.argv2);
								perror("exec error");
								exit(4);
						}
					}
				}
				else
				{
					switch (fork())
					{
						case -1:
							perror("fork error");
							exit(3);
						case 0: //granchild code
							if (!cmds.piping)
							{
								execvp(cmds.argv1[0], cmds.argv1);
								perror("exec error");
								exit(4);
							}
							else
							{
								switch (fork())
								{
									case -1:
										perror("fork error");
										exit(3);
									case 0: //greatgrandchild code
										close(fd[0]);
										dup2(fd[1], STDOUT_FILENO);
										execvp(cmds.argv1[0], cmds.argv1);
										perror("exec error");
										exit(4);
									default: //grandchild code
										close(fd[1]);
										dup2(fd[0], STDIN_FILENO);
										execvp(cmds.argv2[0], cmds.argv2);
										perror("exec error");
										exit(4);
								}
							}
						default: //child code
							exit(0);
					}
				}
			default: //parent code
				if (cmds.piping)
				{
					close(fd[0]);
					close(fd[1]);
				}
				wait(NULL);
		}
		printf(">> ");
		fgets(buf, BUFSZ, stdin);
		buf[strcspn(buf, "\r\n")] = 0;
	}
	
	return 0;
}
