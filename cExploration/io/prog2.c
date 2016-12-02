#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SZ 1024

int fileread(int fdin, int fdout);

int main(int argc, char *argv[])
{
	
	int in;
	int out = STDOUT_FILENO;
	
	int x;
	for (x = 1; x < argc; x++)
	{
		in = open(argv[x], O_RDONLY);
		
		if (in == -1)
		{
			perror(argv[x]);
			exit(3);
		}
		
		int filereturn;
		
		filereturn = fileread(in, out);
		if (filereturn == 4)
		{
			perror(argv[x]);
			close(in);
			exit(4);
		}	
		else if (filereturn == 5)
		{
			perror("write error");
			close(in);
			exit(5);
		}
		
		close(in);
	}
	
	return 0;
}

int fileread(int fdin, int fdout)
{
	ssize_t reader = 1;
	ssize_t writer;
	char *buf = malloc(BUF_SZ);
	
	while (reader != 0)
	{
		reader = read(fdin, buf, BUF_SZ);
		
		if (reader == -1)
			return 4;
		
		writer = write(fdout, buf, reader);
		
		if (writer == -1)
			return 5;
	}
	
	free(buf);
	return 0;
}
