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
	if (argc < 3 || argc > 4)
	{
		printf("Usage %s infile1 infile2 [outfile]\n", argv[0]);
		exit(1);
	}
	else if ((*argv[1] == '-' && *argv[2] == '-') || (argc == 4 && *argv[3] == '-'))
	{
		printf("- may only be used in place of an input file, and only once\n");
		exit(2);
	}
	
	int in1,in2,out;
	
	if (*argv[1] == '-')
		in1 = STDIN_FILENO;
	else
	{
		in1 = open(argv[1], O_RDONLY);
		
		if (in1 == -1)
		{
			perror(argv[1]);
			exit(3);
		}
	}
	
	if (*argv[2] == '-')
		in2 = STDIN_FILENO;
	else
	{
		in2 = open(argv[2], O_RDONLY);
		
		if (in2 == -1)
		{
			perror(argv[2]);
			close(in1);
			exit(3);
		}
	}
	
	if (argc == 3)
		out = STDOUT_FILENO;
	else
	{
		out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600);
		
		if (out == -1)
		{
			perror(argv[3]);
			close(in1);
			close(in2);
			exit(3);
		}
	}
	
	int filereturn;
	
	filereturn = fileread(in1, out);
	if (filereturn == 4)
	{
		perror(argv[1]);
		close(in1);
		close(in2);
		close(out);
		exit(4);
	}	
	else if (filereturn == 5)
	{
		perror(argv[3]);
		close(in1);
		close(in2);
		close(out);
		exit(5);
	}
	
	filereturn = fileread(in2, out);
	if (filereturn == 4)
	{
		perror(argv[1]);
		close(in1);
		close(in2);
		close(out);
		exit(4);
	}	
	else if (filereturn == 5)
	{
		perror(argv[3]);
		close(in1);
		close(in2);
		close(out);
		exit(5);
	}

	close(in1);
	close(in2);
	close(out);
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
	
	return 0;
}
