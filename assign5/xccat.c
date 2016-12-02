#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SZ 1024

int fileread(int fdin, char *buf);
int filewrite(int fdout, char *buf, int len);
void errortrap(int errnum, int fdin1, int fdin2, int fdout, char *argv[]);

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
	
	int reader1, reader2, writer;
	char *buf1 = malloc(BUF_SZ);
	char *buf2 = malloc(BUF_SZ);
	char *writebuf = malloc(BUF_SZ);
	
	reader1 = fileread(in1, buf1);
	if (reader1 < 0)
		errortrap(4, in1, in2, out, argv);
	reader2 = fileread(in2, buf2);
	if (reader2 < 0)
		errortrap(5, in1, in2, out, argv);
	int loc1 = 0;
	int loc2 = 0;
	int x;
	
	while (reader1 != 0 || reader2 != 0)
	{
		for (x = 0; x < reader1; x++)
		{
			if (x+loc1 > reader1)
				break;
			writebuf[x] = buf1[x+loc1];
			if (writebuf[x] == '\n')
				break;
			
			//I realized here that I needed to catch for lines greater than 1024
			//I considered a refactor, or using labels and GOTO
			//instead I did this
			if (x == reader1 - 1 - loc1 && writebuf[x] != '\n')
			{
				writer = filewrite(out, writebuf, x+1);
				if (writer < 0)
					errortrap(6, in1, in2, out, argv);
				reader1 = fileread(in1, buf1);
				if (reader1 < 0)
					errortrap(4, in1, in2, out, argv);
				x = -1;
				loc1 = 0;
				writebuf = calloc(BUF_SZ, 1);
			}
		}
		loc1 += x + 1;
		
		writer = filewrite(out, writebuf, x+1);
		if (writer < 0)
			errortrap(6, in1, in2, out, argv);
		
		for (x = 0; x < reader2; x++)
		{
			if (x+loc2 > reader2)
				break;
			writebuf[x] = buf2[x+loc2];
			if (writebuf[x] == '\n')
				break;

			if (x == reader2 - 1 - loc2 && writebuf[x] != '\n')
			{
				writer = filewrite(out, writebuf, x);
				if (writer < 0)
					errortrap(6, in1, in2, out, argv);
				reader2 = fileread(in2, buf2);
				if (reader2 < 0)
					errortrap(5, in1, in2, out, argv);
				x = -1;
				loc2 = 0;
				writebuf = calloc(BUF_SZ, 1);
			}
		}
		loc2 += x + 1;
		
		writer = filewrite(out, writebuf, x+1);
		if (writer < 0)
			errortrap(6, in1, in2, out, argv);
		
		if (loc1 == reader1)
		{
			reader1 = fileread(in1, buf1);
			loc1 = 0;
			if (reader1 < 0)
				errortrap(4, in1, in2, out, argv);
			writebuf = calloc(BUF_SZ, 1);
		}
		if (loc2 == reader2)
		{
			reader2 = fileread(in2, buf2);
			loc2 = 0;
			if (reader2 < 0)
				errortrap(5, in1, in2, out, argv);
			writebuf = calloc(BUF_SZ, 1);
		}
	}
	
	close(in1);
	close(in2);
	close(out);
	return 0;
}

int fileread(int fdin, char *buf)
{
	ssize_t reader;
	
	reader = read(fdin, buf, BUF_SZ);
	
	if (reader == -1)
		return -4;
	
	return reader;
}

int filewrite(int fdout, char *buf, int len)
{
	ssize_t writer = write(fdout, buf, len);
	
	if (writer == -1)
		return -5;
	
	return writer;
}

void errortrap(int errnum, int fdin1, int fdin2, int fdout, char *argv[])
{
	perror(argv[errnum-3]);
	close(fdin1);
	close(fdin2);
	close(fdout);
	if (errnum == 5 || errnum == 6)
		exit(errnum-1);
	exit(errnum);
}
