/*
#include<unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
*/
int funct1(int n)
{
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
	
	printf("%d\t%d\n",ndx,getpid());
	if (ndx != 0)
	{
		sleep(10);
		exit(0);
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
