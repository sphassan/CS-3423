/*
#include<unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
*/
int funct5(int n)
{
	int fd[n][2];
	int i;
	for (i = 0; i < n; i++)
		pipe(fd[i]);
	
	int ndx;
	int forks = 1;
	while (forks < n+1)
	{
		switch (fork())
		{
			case -1 :
				perror("fork error");
				exit(1);
			case 0 : //child code
				ndx = forks;
				forks = n+1;
				break;
			default : //parent code
				forks++;
				ndx = 0;
		}
	}
	
	if (ndx == 0)
	{
		srand(739);
		
		int x;
		for (x = 1; x <= n; x++)
		{
			//write to processx
		}
	}
	
	return 0;
}
/*
int main()
{
	int ret = funct1(5);
	return 0;
}
*/
