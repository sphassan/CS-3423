/* magic constants for various pcap file types */
#define PCAP_MAGIC 0xa1b2c3d4
#define PCAP_SWAPPED_MAGIC 0xd4c3b2a1
#define PCAP_MODIFIED_MAGIC 0xa1b2cd34
#define PCAP_SWAPPED_MODIFIED_MAGIC 0x34cdb2a1

/* pcap data stored once at the beginning of the file */
struct timev {
	unsigned int tv_sec;
	unsigned int tv_usec;
};
typedef struct timev timev;

/* data prefixing each packet */
struct my_pkthdr {
	struct timev ts;	/* timestamp */
	int caplen;	/* captured length of the packet */
	int len;		/* actual length of the packet*/
};
typedef struct my_pkthdr pkthdr;
