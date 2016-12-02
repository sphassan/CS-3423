#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "pcap.h"
#include "dnet.h"
#include "prtlog.h"

int readfileheader(int fdin, struct pcap_file_header *fileheader);
int readpacketheader(int fdin, pkthdr *packetheader);
int readethernetheader(int fdin, struct eth_hdr *ethernetheader);
int readarpheader(int fdin, struct arp_hdr *arphdr);
int readipheader(int fdin, struct ip_hdr *iphdr);
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
	struct pcap_file_header *fileheader = malloc(sizeof(struct pcap_file_header));
	
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
		while (c_usec < 0)
		{
			c_usec += 1000000;
			c_sec--;
		}
		printf("%05u.%06u\n", (unsigned)c_sec, (unsigned)c_usec);
		
		printf("Captured Packet Length = %d\n", packetheader -> caplen);
		printf("Actual Packet Length = %d\n", packetheader -> len);
		fflush(stdout);
		
		struct eth_hdr *ethernetheader = malloc(sizeof(struct eth_hdr));
		int offset = sizeof(struct eth_hdr);
		
		filereturn = readethernetheader(fdin, ethernetheader);
		if (filereturn == -1)
		{
			perror(argv[1]);
			close(fdin);
			exit(4);
		}
		
		printf("Eternet Header\n");
		
		switch (ntohs(ethernetheader -> eth_type))
		{
			case ETH_TYPE_PUP : printf("    PUP\n"); break;
			case ETH_TYPE_REVARP : printf("    REVARP\n"); break;
			case ETH_TYPE_8021Q : printf("    8021Q\n"); break;
			case ETH_TYPE_IPV6 : printf("    IPv6\n"); break;
			case ETH_TYPE_MPLS : printf("    MPLS\n"); break;
			case ETH_TYPE_MPLS_MCAST : printf("    MPLS MCAST\n"); break;
			case ETH_TYPE_PPPOEDISC : printf("    PPPOEDISC\n"); break;
			case ETH_TYPE_PPPOE : printf("    PPPOE\n"); break;
			case ETH_TYPE_LOOPBACK : printf("    LOOPBACK\n"); break;
			case ETH_TYPE_ARP : break;
			case ETH_TYPE_IP : break;
			default : printf("    UNRECOGNIZED\n");
		}
		fflush(stdout);
		
		if (ntohs(ethernetheader -> eth_type) == ETH_TYPE_ARP)
		{
			printf("    ARP\n");
			struct arp_hdr *arphdr = malloc(sizeof(struct arp_hdr));
			offset += sizeof(struct arp_hdr);
			
			filereturn = readarpheader(fdin, arphdr);
			if (filereturn == -1)
			{
				perror(argv[1]);
				close(fdin);
				exit(4);
			}
			
			printf("        arp operation = ");
			
			switch (ntohs(arphdr -> ar_op))
			{
				case ARP_OP_REQUEST : printf("Arp Request\n"); break;
				case ARP_OP_REPLY : printf("Arp Reply\n"); break;
				case ARP_OP_REVREQUEST : printf("Arp Rev Request\n"); break;
				case ARP_OP_REVREPLY : printf("Arp Rev Reply\n"); break;
			}
			fflush(stdout);
			free(arphdr);
		}
		else if (ntohs(ethernetheader -> eth_type) == ETH_TYPE_IP)
		{
			printf("    IP\n");
			struct ip_hdr *iphdr = malloc(sizeof(struct ip_hdr));
			offset += sizeof(struct ip_hdr);
			
			filereturn = readipheader(fdin, iphdr);
			if (filereturn == -1)
			{
				perror(argv[1]);
				close(fdin);
				exit(4);
			}
			
			switch (iphdr -> ip_p)
			{
				case IP_PROTO_ICMP : printf("        ICMP\n"); break;
				case IP_PROTO_IGMP : printf("        IGMP\n"); break;
				case IP_PROTO_TCP : printf("        TCP\n"); break;
				case IP_PROTO_UDP : printf("        UDP\n"); break;
				default : printf("        UNRECOGNIZED\n"); break;
			}
			fflush(stdout);
			free(iphdr);
		}
		
		char *packet = malloc(packetheader -> caplen - offset);
		
		filereturn = readpacket(fdin, packet, packetheader -> caplen - offset);
		if (filereturn == -1)
		{
			perror(argv[1]);
			close(fdin);
			exit(4);
		}
		free(packetheader);
		free(ethernetheader);
		free(packet);
	}
	
	close(fdin);
	return 0;
}

int readfileheader(int fdin, struct pcap_file_header *fileheader)
{
	ssize_t reader;
	
	reader = read(fdin, fileheader, sizeof(struct pcap_file_header));
	
	return reader;
}

int readpacketheader(int fdin, pkthdr *packetheader)
{
	ssize_t reader;
	
	reader = read(fdin, packetheader, sizeof(pkthdr));
	
	return reader;
}

int readethernetheader(int fdin, struct eth_hdr *ethhdr)
{
	ssize_t reader;
	
	reader = read(fdin, ethhdr, sizeof(struct eth_hdr));
	
	return reader;
}

int readarpheader(int fdin, struct arp_hdr *arphdr)
{
	ssize_t reader;
	
	reader = read(fdin, arphdr, sizeof(struct arp_hdr));
	
	return reader;
}

int readipheader(int fdin, struct ip_hdr *iphdr)
{
	ssize_t reader;
	
	reader = read(fdin, iphdr, sizeof(struct ip_hdr));
	
	return reader;
}

int readpacket(int fdin, char *packet, int pcktsz)
{
	ssize_t reader;
	
	reader = read(fdin, packet, pcktsz);
	
	return reader;
}
