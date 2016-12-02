#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


struct vector
{
	float x;
	float y;
	float z;
};

struct particle
{
	float mass;
	struct vector pos;
	struct vector vel;
};

int binread(int fdin, struct particle* par);

int main(int argc, char *argv[])
{
	int in = open(argv[1], O_RDONLY);
	if (in == -1)
	{
		perror(argv[1]);
		exit(1);
	}
	
	struct particle par;
	
	int pcount = 1;
	int bytesread;
	while ((bytesread = binread(in, &par)) > 0)
	{
		if (bytesread < sizeof(struct particle))
			printf("Error: Incomplete Record\n");
		else
		{
			printf("Particle\t%d\n",pcount++);
			printf("\tmass\t%f\n",par.mass);
			printf("\tpos\t(%f,%f,%f)\n",par.pos.x,par.pos.y,par.pos.z);
			printf("\tvel\t(%f,%f,%f)\n",par.vel.x,par.vel.y,par.vel.z);
		}
		fflush(stdout);
	}
	
	close(in);
	return 0;
}

int binread(int fdin, struct particle* par)
{
	int reader = read(fdin, par, sizeof(struct particle));
	return reader;
}	
