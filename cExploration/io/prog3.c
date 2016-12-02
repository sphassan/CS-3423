#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define BUF_SZ 1024

int fileread(int fdin, char *buf);
int filewrite(int fdout, char *buf, int len);
void errortrap(int errnum, int fdin, char *argv[]);

int main(int argc, char *argv[])
{
	int in;
	int out = STDOUT_FILENO;
	
	in = open(argv[1], O_RDONLY);
	if (in == -1)
	{
		perror(argv[1]);
		exit(3);
	}
	
	int reader, writer;
	char buf[1024];
	char writebuf[1024];
	
	reader = fileread(in, buf);
	if (reader < 0)
		errortrap(4, in, argv);
	int loc = 0;
	int newline = 0;
	int x, wsz;
	
	while (reader != 0)
	{
		for (x = 0; x < reader; x++)
		{
			if (x+loc == reader)
				break;
			writebuf[x] = buf[x+loc];
			if (writebuf[x] == '\n')
			{
				writebuf[x+1] = '\n';
				newline = 1;
				break;
			}
		}
		if (x + loc == reader)
			wsz = x;
		else
			wsz = x+1;
		loc += x+1;
		if (newline)
		{
			wsz++;
			newline = 0;
		}
		
		writer = filewrite(out, writebuf, wsz);
		if (writer < 0)
			errortrap(6, in, argv);
		memset(writebuf, '\0', 1024);

		if (loc >= reader)
		{
			reader = fileread(in, buf);
			loc = 0;
			newline = 0;
			if (reader < 0)
				errortrap(4, in, argv);
		}
	}
	
	close(in);
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

void errortrap(int errnum, int fdin, char *argv[])
{
	perror(argv[1]);
	close(fdin);
	if (errnum == 5 || errnum == 6)
		exit(errnum-1);
	exit(errnum);
}
