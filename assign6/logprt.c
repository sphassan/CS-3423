#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "logprt.h"

int readfileheader(int fdin, pcap_file_header *fileheader);
int readpacketheader(int fdin, pkthdr *packetheader);
int readpacket(int fdin, char *packet, int pcktsz);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage %s <infile>\n", argv[0]);
		exit(1);
	}
	
	int fdin = open(argv[1], O_RDONLY);
	
	if (fdin == -1)
	{
		perror(argv[1]);
		exit(3);
	}
	
	int filereturn;
	pcap_file_header *fileheader = malloc(sizeof(pcap_file_header));
	
	filereturn = readfileheader(fdin, fileheader);
	if (filereturn == -1)
	{
		perror(argv[1]);
		close(fdin);
		exit(4);
	}
	
	if (fileheader -> magic == PCAP_MAGIC)
		printf("PCAP_MAGIC\n");
	else if (fileheader -> magic == PCAP_SWAPPED_MAGIC)
		printf("PCAP_SWAPPED_MAGIC\n");
	else if (fileheader -> magic == PCAP_MODIFIED_MAGIC)
		printf("PCAP_MODIFIED_MAGIC\n");
	else if (fileheader -> magic == PCAP_SWAPPED_MODIFIED_MAGIC)
		printf("PCAP_SWAPPED_MODIFIED_MAGIC\n");
	printf("Version major number = %d\n", fileheader -> version_major);
	printf("Version minor number = %d\n", fileheader -> version_minor);
	printf("GMT to local correction = %d\n", fileheader -> thiszone);
	printf("Timestamp accuracy = %d\n", fileheader -> sigfigs);
	printf("Snaplen = %d\n", fileheader -> snaplen);
	printf("Linktype = %d\n\n", fileheader -> linktype);
	
	fflush(stdout);
	free(fileheader);
	
	int firsttime = 1;
	int packetnum = 0;
	unsigned int b_sec, c_sec;
	int b_usec, c_usec;
	
	while (filereturn != 0)
	{
		pkthdr *packetheader = malloc(sizeof(pkthdr));
		
		filereturn = readpacketheader(fdin, packetheader);
		if (filereturn == -1)
		{
			perror(argv[1]);
			close(fdin);
			exit(4);
		}
		else if (!firsttime && filereturn != 0)
			printf("\n");
		else if (filereturn == 0)
		{
			free(packetheader);
			break;
		}
		
		printf("Packet %d\n", packetnum++);
		
		fflush(stdout);
		
		if (firsttime)
		{
			firsttime = 0;
			b_sec = packetheader -> ts.tv_sec;
			b_usec = packetheader -> ts.tv_usec;
		}
		c_sec = (unsigned)packetheader -> ts.tv_sec - b_sec;
		c_usec = (unsigned)packetheader -> ts.tv_usec - b_usec;
		while (c_sec < 0)
		{
			c_usec += 1000000;
			c_sec--;
		}
		printf("%05u.%06u\n", (unsigned)c_sec, (unsigned)c_usec);
		
		printf("Captured Packet Length = %d\n", packetheader -> caplen);
		printf("Actual Packet Length = %d\n", packetheader -> len);
		
		fflush(stdout);
		
		char *packet = malloc(packetheader -> caplen);
		
		filereturn = readpacket(fdin, packet, packetheader -> caplen);
		if (filereturn == -1)
		{
			perror(argv[1]);
			close(fdin);
			exit(4);
		}
		free(packetheader);
		free(packet);
	}
	
	close(fdin);
	exit(0);
}

int readfileheader(int fdin, pcap_file_header *fileheader)
{
	ssize_t reader;
	
	reader = read(fdin, fileheader, sizeof(pcap_file_header));
	
	if (reader == -1)
		return -1;
	else if (reader == 0)
		return 0;
	
	return reader;
}

int readpacketheader(int fdin, pkthdr *packetheader)
{
	ssize_t reader;
	
	reader = read(fdin, packetheader, sizeof(pkthdr));
	
	if (reader == -1)
		return -1;
	else if (reader == 0)
		return 0;
	
	return reader;
}

int readpacket(int fdin, char *packet, int pcktsz)
{
	ssize_t reader;
	
	reader = read(fdin, packet, pcktsz);
	
	if (reader == -1)
		return -1;
	else if (reader == 0)
		return 0;
	
	return reader;
}
