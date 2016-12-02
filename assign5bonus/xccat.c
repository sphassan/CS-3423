#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
	
	ssize_t reader1 = 1;
	ssize_t reader2;
	ssize_t writer;
	char *readbuf1[1024];
	char *readbuf2[1024];
	char *writebuf;
	int loc1,loc2,x;
	
	while (reader1 != 0)
	{
		reader1 = read(in1, readbuf1, 1024);
		reader2 = read(in2, readbuf2, 1024);

		if (reader1 == -1)
		{
			perror(argv[1]);
			close(in1);
			close(in2);
			close(out);
			exit(4);
		}
		if (reader2 == -1)
		{
			perror(argv[2]);
			close(in1);
			close(in2);
			close(out);
			exit(4);
		}
		
		loc1 = 0;
		loc2 = 0;
		
		while (loc1 < 1024 && loc2 < 1024)
		{
			for (x = loc1; x < reader1; x++)
			{
				writebuf[x] = *readbuf1[x];
				if (*readbuf1[x] == '\n')
				{
					loc1 = x+1;
					break;
				}
			}
			
			writer = write(out, writebuf, reader1);
			
			if (writer == -1)
			{
				perror(argv[3]);
				close(in1);
				close(in2);
				close(out);
				exit(5);
			}
			
			for (x = loc2; x < reader2; x++)
			{
				writebuf[x] = *readbuf2[x];
				if (*readbuf2[x] == '\n')
				{
					loc2 = x+1;
					break;
				}
			}
			
			writer = write(out, writebuf, reader1);
			
			if (writer == -1)
			{
				perror(argv[3]);
				close(in1);
				close(in2);
				close(out);
				exit(5);
			}
		}
	}
	
	reader2 = 1;
	
	while (reader2 != 0)
	{
		reader2 = read(in2, readbuf2, 1024);
		
		if (reader2 == -1)
		{
			perror(argv[2]);
			close(in1);
			close(in2);
			close(out);
			exit(4);
		}
		
		writer = write(out, readbuf2, reader2);
		
		if (writer == -1)
		{
			perror(argv[3]);
			close(in1);
			close(in2);
			close(out);
			exit(5);
		}
	}
	
	close(in1);
	close(in2);
	close(out);
	return 0;
}
