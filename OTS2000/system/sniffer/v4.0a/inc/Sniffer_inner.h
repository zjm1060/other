#include "sniffer.h"

int		network_no;
typedef void *HANDLE;
HANDLE	hMutex;

#define CONFIGFILE "../def/SnifferConfig.def"
#define EXE_FILE "Sniffer.exe"
#define MuteOfSnifferChild "SNIFFER_CHILD"
#define LOGFILENAME "Log.txt"
FILE * configFile;
short protocolNo=-1;
//unsigned char recvbuf[1024];

#define EHTERNET_HEAD_LENGTH 14
#define ETHER_ADDR_LEN  6

#define IPPackage 2048
#define ARPPackage 2054
#define RARPPackage 32821

typedef struct csma_cd_head
{
	char sourceMac[6];
	char destinationMac[6];
	unsigned short  length; 
	char DSAP;
	char SSAP;
	char cntl;
	char org[3];
	unsigned short  type; 
}csma_cd_head;

typedef struct ethernet_head
{
	char sourceMac[6];
	char destinationMac[6];
	unsigned short  type; 
}ethernet_head;

/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header
{
	char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	char	tos;			// Type of service 
	unsigned short tlen;			// Total length 
	unsigned short identification; // Identification
	unsigned short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
#define IP_RF 0x8000            /* reserved fragment flag */
#define IP_DF 0x4000            /* don't fragment flag */
#define IP_MF 0x2000            /* more fragments flag */
#define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
	char	ttl;			// Time to live
	char	proto;			// Protocol
#define ICMP 1
#define IGMP 2
#define TCP  6
#define UDP  17
	unsigned short crc;			// Header checksum
	ip_address	saddr;		// Source address
	ip_address	daddr;		// Destination address
	unsigned int	op_pad;			// Option + Padding
}ip_header;

 /*IPv4 header
struct ip_header {
	char	ip_vhl;		// header length, version 
#define IP_V(ip)	(((ip)->ip_vhl & 0xf0) >> 4)   // version 
#define IP_HL(ip)	((ip)->ip_vhl & 0x0f)          // header length
	char	ip_tos;		// type of service 
	unsigned short	ip_len;		// total length 
	unsigned short	ip_id;		// identification 
	unsigned short	ip_off;		// fragment offset field 
#define	IP_DF 0x4000			// dont fragment flag 
#define	IP_MF 0x2000			// more fragments flag 
#define	IP_OFFMASK 0x1fff		// mask for fragmenting bits 
	char	ip_ttl;		// time to live 
	char	ip_p;		// protocol 
	unsigned short	ip_sum;		// checksum 
	struct	in_addr ip_src,ip_dst;	// source and dest address 
}ip_header;
 */

/* Structure of an tcp header. */
typedef	unsigned int	tcp_seq;
typedef struct 
{
	unsigned short	sport;		/* source port */
	unsigned short	dport;		/* destination port */
	tcp_seq		seq;			/* 32 bytes sequence number */
	tcp_seq		ack;			/* 32 bytes acknowledgement number */
	char	offx2;		/* 4位首部长度 data offset, rsvd 之后保留6位*/
#define TCP_HEAD_LENGTH(th)	(((th)->offx2 & 0xf0) >> 4)*4
	char	flags;
#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20
#define TH_ECNECHO	0x40	/* ECN Echo */
#define TH_CWR		0x80	/* ECN Cwnd Reduced */
	unsigned short	winSize;			/* window size*/
	unsigned short	sum;			/* checksum */
	unsigned short	urp;			/* urgent pointer */
}tcp_header;
#define UDP_HEADER_LENGTH 8
/* UDP header*/
typedef struct udp_header
{
	unsigned short sport;			// Source port
	unsigned short dport;			// Destination port
	unsigned short len;			// Datagram length = 全长-ip首部长度
	unsigned short crc;			// Checksum
}udp_header;

typedef struct PROTOCOL_REC
{
	char	snifferName[20];
	char	protocolName[10];
	pcap_if_t *	device;
	char	filter[50]; 
	int	printToLogFile;//是否记录到文件
	int isGCOM;
	char logFileName[200];
	SHM_SNIFFER * shareMemory;
#ifndef WIN32 
	int Pid;
#else
	PROCESS_INFORMATION 	Pid;
#endif
	unsigned int  writePoint;
	unsigned int  gcom_tn_node_id, gcom_dsv_node_id;
}PROTOCOL_REC;

#define MAX_PROTOCOL_NUM	3
int protocolCount;
PROTOCOL_REC protocolRec[MAX_PROTOCOL_NUM];

extern void* CreateShareMemory(char key_char, int size, char *shm_name);
extern int GetLocalHostState(unsigned short *state);
#ifdef WIN32
void OpenNewTermalWindow ();
#else
void CreateSnifferProcess();
#endif
SHM_SNIFFER *initShareMemory(char SHMSnifferKey,PROTOCOL_REC *protocolRec );
void printHelpMessage();
/* prototype of the packet handler */
void packet_handler(char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

void WritetoShare(SHM_SNIFFER * shm_sniff_addr,u_char * buf,int length);

int snifferOneNetwork(pcap_if_t *d,char * packet_filter);
void handlerUDP(ip_header *ih,unsigned int ipHeader_len);
void handlerTCP(ip_header *ih,	unsigned int ipHeader_len);

void listAllDevice();
pcap_if_t *goToDevice(char *deviceName);
pcap_if_t *getAllDevice();
void	SaveRecvData(SHM_SNIFFER *shm_sniff_addr, unsigned char *recvbuf,unsigned int begin,unsigned int count,int printToLogFile,char * moduleName);
#ifdef WIN32
	void ExitRoutine ( DWORD signal_type );
#else
	void Endproc(int sig_no);
#endif
int getItemAndContent(char *theString,unsigned short stringLength,char *item,char *content);
int foundEqualCharPosition(char *theString,unsigned short stringLength);
int getGroupName(char *theString,unsigned short stringLength,char *groupName);
void analysePacket(const struct pcap_pkthdr *header, const u_char *pkt_data);
void handleGCOM(const u_char *pkt_data,unsigned int packageLength);
int isGCOMMulticasePackage(const u_char *pkt_data, int length);
int IsGCOMPackage(const u_char *pkt_data, int length);
int isGCOMAckPackage(const u_char *pkt_data, int length);
