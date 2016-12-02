#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct product
{
	int type;
	int number;
};

int binread(int fdin, struct product* prd);

int main(int argc, char *argv[])
{
	int in = open(argv[1], O_RDONLY);
	if (in == -1)
	{
		perror(argv[1]);
		exit(1);
	}
	
	struct product prd;
	
	int bytesread;
	while ((bytesread = binread(in, &prd)) > 0)
	{
		printf("Product\t%d\tQuantity\t%d\n",prd.type,prd.number);
		fflush(stdout);
		unsigned serial;
		int x;
		for (x = 0; x < prd.number; x++)
			read(in, &serial, sizeof(unsigned));
	}
	
	close(in);
	return 0;
}

int binread(int fdin, struct product* prd)
{
	int reader = read(fdin, prd, sizeof(struct product));
	return reader;
}	
