/*
##########################################
	sniffer local network adapter
	? 显示帮助
	-p 打印本机的网卡名称
	配置文件SnifferConfig.def
	#define MultiSniffer 若定义将在配置文件依次读取要监听的网卡，否则只监听配置中的第一项
##########################################
*/
#ifndef WIN32
#include <arpa/inet.h>
#endif

#include	"pcap.h"
#include	"../inc/sniffer_inner.h"

#include    "../../../dps/v4.0a/inc/ipc_common.h"
#include 	"../../../dms/v4.0a/inc/dms_com.h"
#include 	"../../../dms/v4.0a/inc/dms_util_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"

int init(void);

int	err_output_mode;
char hostname[256];

//#define DEBUG
#define MultiSniffer

#ifndef WIN32
#define Sleep(x)	usleep(x*1000)
#endif

int checkChildPid;
void CheckSnifferActive(void);


int main(int argc,char *argv[])
{
	int i=0;
	char tmp[200];
	int onlyPrintDeviceList;

	onlyPrintDeviceList=0;	

	chdir(SYS_DEFINE_PATH);

	SetModuleName("SNIFFER");
	SetErrLogModeEx(ERROR_LOG_OUTPUT);

	/******help message****************/
	if(argc == 2)
	{
		if(strcmp(argv[1],"?")==0)
		{
			printHelpMessage();
			exit(0);
		}
		if(strcmp(argv[1],"-p")==0)
			onlyPrintDeviceList=1;
		
		if(onlyPrintDeviceList)
		{
			listAllDevice();
			printf("\nPress any key to terminate program");
			getchar();
			exit(0);
		}
	}
	/******end help message****************/


	if(-1==MountDB(NULL))
	{
		SendTraceMsg(0,0,"\nFail to MountDB!");
		exit(-1);
	}
	
	lan_out_init();
	
	/******set environment*****************/
	if(-1==init())
	{
		printf("初始化失败! 按任意键结束程序!",CONFIGFILE);
		exit(1);
	}
	if (protocolCount==0)
	{
		printf("配置文件：%s 中未定义需要检测的规约!按任意键结束程序!",CONFIGFILE);
		exit(1);
	}
	if(argc == 2)
	{
		protocolNo=atoi(argv[1]);
		if(protocolNo==-1)
			exit(0);
	}
	else
	{
		protocolNo=0;
	}

	gethostname(hostname, sizeof(hostname));

#ifdef MultiSniffer
	#ifndef WIN32
		signal(SIGTERM,Endproc);
		signal(SIGINT,Endproc);
		strcpy( protocolRec[protocolCount].snifferName, "WATCH_DOG" );
		protocolRec[protocolCount].Pid = getpid();
		CreateSnifferProcess();
	#else
		OpenNewTermalWindow();
		snifferOneNetwork(protocolRec[protocolNo].device,protocolRec[protocolNo].filter);
		SetConsoleTitle (tmp );			
		hMutex= OpenMutex(MUTEX_ALL_ACCESS|SYNCHRONIZE, FALSE, MuteOfSnifferChild);
		if(hMutex == NULL )
		{
			SendTraceMsg(0,0,"\nFailed to open mutex %s",MuteOfSnifferChild);
			exit(-1);
		}
	#endif
#else
	snifferOneNetwork(protocolRec[protocolNo].device,protocolRec[protocolNo].filter);
#endif
	/***********end set envionment**************************/
	
}
void printHelpMessage()
{
	printf("Usage:\n");
	printf("Sniffer -p: only list all device on local machine\n");
	printf("Sniffer: get configuration from file:%s\n",CONFIGFILE);
}


int init(void)
{
	char tmp[100];
	char item[100],content[100];
	pcap_if_t *device;
	int err;
	char protocolPosition;
	char snifferName[30];
	if((configFile=fopen(CONFIGFILE,"r"))==NULL)
	{
		SendTraceMsg(0,0,"Failed to read config file:%s\n",CONFIGFILE);
		#ifdef WIN32
			err=GetLastError();
		#else
			printf("Please lookup the error no:%d in man page of fopen\n",errno);
			switch(errno)
			{
			case    EACCES:printf("EACCES\n");break;
			case	EINTR:printf("EINTR\n");break;
			case    EISDIR:printf("EISDIR\n");break;
			case	ELOOP:printf("ELOOP\n");break;
			case    EMFILE:printf("EMFILE\n");break;
			case	ENAMETOOLONG:printf("ENAMETOOLONG\n");break;
			case    ENFILE:printf("ENFILE\n");break;
			case	ENOENT:printf("ENOENT\n");break;
			case    ENOSPC:printf("ENOSPC\n");break;
			}
		#endif
		exit(1);
	}
	while(fgets(tmp,sizeof(tmp),configFile)!=NULL)
	{
		if(getGroupName(tmp,sizeof(tmp),snifferName)==0)
		{
			if(protocolCount>=MAX_PROTOCOL_NUM)
			{
				printf("max defined protocol num is %d", MAX_PROTOCOL_NUM);
				break;
			}
			protocolCount++;
			protocolPosition=protocolCount-1;
			protocolRec[protocolPosition].printToLogFile=0;
#ifndef WIN32
			protocolRec[protocolPosition].Pid=-1;
#endif
			strcpy(protocolRec[protocolPosition].snifferName,snifferName);
			continue;
		}
		if (protocolCount>0)
		{
			if(getItemAndContent(tmp,sizeof(tmp),item,content)==-1) continue;
			if (strcmp(item,"Protocol")==0)
			{
				strcpy(protocolRec[protocolPosition].protocolName,content);
				if(strcmp(content,"GCOM")==0)
				{
					protocolRec[protocolPosition].shareMemory=initShareMemory(SHM_SNIFFER_GCOM_KEY,&protocolRec[protocolPosition]);
					protocolRec[protocolPosition].isGCOM=TRUE;
				}
				else
				{
					protocolRec[protocolPosition].shareMemory=initShareMemory(SHM_SNIFFER_VIP_KEY,&protocolRec[protocolPosition]);
					protocolRec[protocolPosition].isGCOM=FALSE;
				}
			}
			else if (strcmp(item,"Device")==0)
			{
				device=goToDevice(content);
				if(device==NULL || device==(pcap_if_t *)-1)
				{
					SendTraceMsg(0,0,"Can't found device:%s on this machine,please check config file:%s",content,CONFIGFILE);
					fclose(configFile);
					return -1;
				}
				protocolRec[protocolPosition].device=device;
			}
			else if (strcmp(item,"Filter")==0)
			{
				strcpy(protocolRec[protocolPosition].filter,content);
			}
			else if(strcmp(item,"PrintToLogFile")==0)
			{
				if(strcmp(content,"YES")==0)
				{
					protocolRec[protocolPosition].printToLogFile=1;
					sprintf(protocolRec[protocolPosition].logFileName,"%s%s_%s",ERROR_LOG_PATH,snifferName,LOGFILENAME);
				}

			}
		}
	}
	fclose(configFile);
	return 0;
}
int getGroupName(char *theString,unsigned short stringLength,char *groupName)
{
	int i;
	if(theString[0]=='[')
	{
		for (i=0;i<stringLength-1;i++)
		{
			if (theString[i+1]==']')
			{
				groupName[i]='\0';
				return 0;
			}
			groupName[i]=theString[i+1];
		}
	}
	return -1;


}
int getItemAndContent(char *theString,unsigned short stringLength,char *item,char *content)
{
	int i,j,equalPos=-1;
	int frontBlankPassed=FALSE; //前置空格已处理
	if (theString[0]=='#')
	{
		return -1;
	}
	equalPos=foundEqualCharPosition(theString,stringLength);
	if (equalPos==-1) return -1;
	for (i=0;i<equalPos;i++)
	{
		if (!frontBlankPassed)
		{
			if (theString[i]==' ') continue;
			else frontBlankPassed=TRUE;
		}
		 if (theString[i]!=' ')
			 item[i]=theString[i];
	}
	item[i]='\0';
	j=0;frontBlankPassed=FALSE;
	for (i=equalPos+1;i<stringLength;i++)
	{
		if (!frontBlankPassed)
		{
			if (theString[i]==' ') continue;
			else frontBlankPassed=TRUE;
		}
		
		if(theString[i]=='\n') //读到回车
		{
			content[j++]='\0';
			return 0;
		}
		else
			content[j++]=theString[i];
	}
	return 0;
}
int foundEqualCharPosition(char *theString,unsigned short stringLength)
{
	int i;
	for(i=0;i<stringLength;i++)
	{
		if (theString[i]=='=')
			return i;
		if (theString[i]=='\n')
			return -1;
	}
	return -1;
	
}
pcap_if_t *getAllDevice()
{
	pcap_if_t *alldevs;
	char errbuf[PCAP_ERRBUF_SIZE];
	/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}
	return alldevs;
}
pcap_if_t *goToDevice(char *deviceName)
{
	pcap_if_t *d;
	pcap_if_t *alldevs=getAllDevice();
	char emptyString[1];
	for(d=alldevs;;d=d->next)
	{
		if (d==NULL)
		{
			SendTraceMsg(0,0,"未找到配置的网卡:%s,程序退出!\n",deviceName);
			return (pcap_if_t *)-1;
		}
		if(strcmp(d->name,deviceName)==0)
		{
			if (d->description==NULL)
			{
				d->description=emptyString;
			}
			return d;
		}
	}
	return NULL;
}
void listAllDevice()
{
	pcap_if_t *d;
	int i=0;
	pcap_if_t *alldevs=getAllDevice();
	
	/* Print the list */
	for(d=alldevs; d; d=d->next)
	{
		printf("\n%d.Device Name: %s\n", ++i, d->name);
		if (d->description)
			printf("Decription(%s)\n", d->description);
		else
			printf("(No description available)\n");

		if(d->addresses)
		{
			printf("Address:%s\n", inet_ntoa((((struct sockaddr_in*)d->addresses->addr)->sin_addr)));
			printf("Netmask %s\n", inet_ntoa((((struct sockaddr_in*)d->addresses->netmask)->sin_addr)));
		}
		else
			printf("(No addresses available)\n");
	}
	pcap_freealldevs(alldevs);

}
int snifferOneNetwork(pcap_if_t *d,char * packet_filter)
{
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_u_int32 netmask;
	struct bpf_program fp;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	int res;
	char snifferName[100];
	
	sprintf(snifferName,"sniffer:  %s",protocolRec[protocolNo].snifferName );
	SendTraceMsg(0,0,"%s start\n",snifferName);
	
	/* Open the adapter */
	if ((adhandle= pcap_open_live(d->name,	// name of the device
			65536,			// portion of the packet to capture. 
						 // 65536 grants that the whole packet will be captured on all the MACs.
			 1,				// promiscuous mode (nonzero means promiscuous)
			1000,			// read timeout
			errbuf			// error buffer
			)) == NULL)
	{
		//SendTraceMsg(stderr,"\nUnable to open the adapter: %s\n, error message is:%s\n", d->name,errbuf);
		return -1;
	}
	SendTraceMsg(0,0,"\n%s:listening on %s...\n",snifferName,inet_ntoa((((struct sockaddr_in*)d->addresses->addr)->sin_addr)));
	
	if(d->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask=0xffffff; 
	
	/* compile the filter */
	printf("%s:filter:%s\n",snifferName,packet_filter);
	if (strcmp(packet_filter,"")!=0)
	{
		if(pcap_compile(adhandle,&fp,packet_filter,1,netmask ) == -1)
		{ 
			SendTraceMsg(0,0,"%s:Error calling pcap_compile with packet_filter:%s\n",snifferName,packet_filter); 		
			exit(1); 
		}
		/* set the compiled program as the filter */
		if(pcap_setfilter(adhandle,&fp) == -1)
	{ SendTraceMsg(0,0,"%s:Error setting filter\n",snifferName); exit(1); }
	}
	
	
	pcap_freealldevs(d);
	
	/* start the capture asyn*/
	/*pcap_loop(adhandle, 0, packet_handler, NULL);
	pcap_close(adhandle);
	*/
	while((res = pcap_next_ex( adhandle, &header, &pkt_data)) >= 0)
	{
        
        if(res == 0)
            /* Timeout elapsed */
           continue;
		
        analysePacket(header,pkt_data);
    }
    if(res == -1)
	{
        SendTraceMsg(0,0,"%s:Error reading the packets: %s\n",snifferName, pcap_geterr(adhandle));
        return -1;
    }
    

	return 0;


}
/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	
	analysePacket(header,pkt_data);
}


void	PrintRecvData(unsigned char *data, int length)
{
	int d;
	
	SendTraceMsg(0,0,"%s:data length= %d",protocolRec[protocolNo].snifferName, length);
	printf("\n");
	for(d=1;d<=length;d++)
	{
		printf("%02x ",data[d-1]);
		if((d%10)==0)
			printf(" ");
		if((d%20)==0)
			printf("\n");
	}
	printf("\n");
}

void analysePacket(const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	struct tm *ltime;
	char timestr[16];
	time_t local_tv_sec;
	ethernet_head *eth_head;
	ip_header *ip_head;
	unsigned int ipHeader_len;
	unsigned int eth_type;
	int ipPackage=0;
	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime=localtime(&local_tv_sec);
	strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
	
	eth_head=(ethernet_head *)pkt_data; //假设是以太网封装
	eth_type=ntohs(eth_head->type);
	
	/* retireve the position of the ip header */
	
	/* print timestamp and length of the packet */

	SendTraceMsg(0,0,"%s:%s.%.6d recv package len:%d",protocolRec[protocolNo].snifferName, timestr, header->ts.tv_usec, header->len);

	if (protocolRec[protocolNo].isGCOM==TRUE)
	{//802.3
		if (IsGCOMPackage(pkt_data, header->len)==TRUE)
		{
			SendTraceMsg(0,0,"recv GCOM package size= %d", header->len);
// 			if (isGCOMMulticasePackage(pkt_data, header->len)==FALSE && isGCOMAckPackage(pkt_data, header->len)==FALSE)
// 			{
// 				SendTraceMsg(0,0,"recv GCOM event package");
// 				handleGCOM(pkt_data,header->len);
// 			}
// 			else
// 			{
// 				SendTraceMsg(0,0,"recv GCOM multicase or ack package");
// 				PrintRecvData(pkt_data, header->len);
// 			}
			handleGCOM(pkt_data,header->len);

		}
	}
	else
	{
		switch(eth_type)
		{
		case ARPPackage:
#ifdef DEBUG
			SendTraceMsg(0,0,"%s:ethernet ARP package\n",protocolRec[protocolNo].snifferName);
#endif
			break;
		case RARPPackage:
#ifdef DEBUG
			SendTraceMsg(0,0,"%s:ethernet RARP package\n",protocolRec[protocolNo].snifferName);
#endif
			break;
		case IPPackage:
			ipPackage=1;
			ip_head = (ip_header *) (pkt_data +
				sizeof(ethernet_head)); //length of ethernet header
			break;
		default:	
			break;
		}
		if (ipPackage==1)
		{
			ipHeader_len = (ip_head->ver_ihl & 0xf) * 4;
			//treatment depend on protocol type
			switch (ip_head->proto)
			{
			case ICMP:break;
			case IGMP:break;			
			case TCP:
#ifdef DEBUG
				SendTraceMsg(0,0,"%s:ethernet TCP protocol\n",protocolRec[protocolNo].snifferName);
#endif
				handlerTCP(ip_head,ipHeader_len); //VIP-R use TCP to send command
				break;
			case UDP:
#ifdef DEBUG
				SendTraceMsg(0,0,"%s:ethernet UDP protocol\n",protocolRec[protocolNo].snifferName);
#endif
				handlerUDP(ip_head,ipHeader_len);
			} 
		}
	}
}
int IsGCOMPackage(const u_char *pkt_data, int length)//sa:00, 00, 23, 00, nn, 01
{
	//if (pkt_data[0]==0x00&&pkt_data[1]==0x00&&pkt_data[2]==0x23&&pkt_data[3]==0x00&&pkt_data[5]==0x01
	if(length>=18 && pkt_data[6]==0x00&&pkt_data[7]==0x00&&pkt_data[8]==0x23&&pkt_data[9]==0x00&&pkt_data[11]==0x01)
	{
		return TRUE;
	}
	else
		return FALSE;
}
int isGCOMMulticasePackage(const u_char *pkt_data, int length)//multicast destination address< 01, 00, 23, 00, 00, 01 >.
{
	if (length==26 && pkt_data[0]==0x01&&pkt_data[1]==0x00&&pkt_data[2]==0x23&&pkt_data[3]==0x00&&pkt_data[4]==0x00&&pkt_data[5]==0x01
		&& (pkt_data[17] & 0x0F)==0x04) //type=I am here
		return TRUE;
	else
		return FALSE;
}

int isGCOMAckPackage(const u_char *pkt_data, int length)
{
	if (length>=18 && (pkt_data[17] & 0x0F)==0x01 ) //type=ACK
		return TRUE;
	else
		return FALSE;
}

void handleGCOM(const u_char *pkt_data,unsigned int packageLength)
{
	SaveRecvData(protocolRec[protocolNo].shareMemory,(u_char *)pkt_data,0,packageLength,
		protocolRec[protocolNo].printToLogFile,protocolRec[protocolNo].snifferName);
	
}
void handlerTCP(ip_header *ip_head,	unsigned int ipHeader_len)
{
	tcp_header *tcpHeader;
	unsigned int tcpHeader_len;
	unsigned int dataPosition;
	unsigned int dataLength=0;
	
	unsigned short sport,dport;
	tcpHeader = (tcp_header *) ((char*)ip_head + ipHeader_len);
	/* calculate data position */
	tcpHeader_len=TCP_HEAD_LENGTH(tcpHeader);
	dataPosition= ipHeader_len + tcpHeader_len;
	dataLength=ntohs(ip_head->tlen)-ipHeader_len-tcpHeader_len;
	sport = ntohs( tcpHeader->sport );
	dport = ntohs( tcpHeader->dport );
	
	/* print ip addresses and udp ports */
	SendTraceMsg(0,0,"%s:TCP Packet:ip total length:%d %d.%d.%d.%d:%d -> %d.%d.%d.%d:%d\n",
		protocolRec[protocolNo].snifferName,
		ntohs(ip_head->tlen),
		ip_head->saddr.byte1,
		ip_head->saddr.byte2,
		ip_head->saddr.byte3,
		ip_head->saddr.byte4,
		sport,
		ip_head->daddr.byte1,
		ip_head->daddr.byte2, 
		ip_head->daddr.byte3,
		ip_head->daddr.byte4,
		dport);
	SaveRecvData(protocolRec[protocolNo].shareMemory,(u_char *)ip_head,dataPosition,dataLength,protocolRec[protocolNo].printToLogFile,protocolRec[protocolNo].snifferName);
}
void handlerUDP(ip_header *ip_head,unsigned int ipHeader_len)
{
	udp_header *udpHeader;
	unsigned short sport,dport;
	unsigned int dataPosition;
	unsigned int dataLength=0;
	/* retireve the position of the udp header */
	udpHeader = (udp_header *) ((char*)ip_head + ipHeader_len);
	/* calculate data position */
	dataPosition= ipHeader_len + UDP_HEADER_LENGTH;
	dataLength=ntohs(ip_head->tlen)-ipHeader_len-UDP_HEADER_LENGTH;
	
	/* convert from network byte order to host byte order */
	sport = ntohs( udpHeader->sport );
	dport = ntohs( udpHeader->dport );
	
	/* print ip addresses and udp ports */
	SendTraceMsg(0,0,"%s:UDP Packet:ip total length:%d %d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
		protocolRec[protocolNo].snifferName,
		ntohs(ip_head->tlen),
		ip_head->saddr.byte1,
		ip_head->saddr.byte2,
		ip_head->saddr.byte3,
		ip_head->saddr.byte4,
		sport,
		ip_head->daddr.byte1,
		ip_head->daddr.byte2, 
		ip_head->daddr.byte3,
		ip_head->daddr.byte4,
		dport);
	SaveRecvData(protocolRec[protocolNo].shareMemory,(u_char *)ip_head,dataPosition,
		dataLength,protocolRec[protocolNo].printToLogFile,protocolRec[protocolNo].snifferName);
}
//initialize share memory of sniffer
SHM_SNIFFER *initShareMemory(char SHMSnifferKey,PROTOCOL_REC *protocolRec )
{
	SHM_SNIFFER *shm_sniff_addr;
	char memoryName[100];
	
#ifndef WIN32
	setreuid(-1,getuid());
#endif
	protocolRec->writePoint=0;
	strcpy(memoryName,"sniffer_share_meory");
	strcat(memoryName,&SHMSnifferKey);
	printf("begin link to %s !\n",memoryName);
	shm_sniff_addr= (SHM_SNIFFER*) LinktoShareMem( SHMSnifferKey );
	
	if( shm_sniff_addr == (SHM_SNIFFER*)-1 )
	{
		printf("begin create %s !\n",memoryName);
		shm_sniff_addr=CreateShareMemory(SHMSnifferKey,sizeof(SHM_SNIFFER),memoryName);
		
		if( shm_sniff_addr == (SHM_SNIFFER*)-1 )
		{
			
#ifndef WIN32
			SendTraceMsg(0,0,"\nSniffer: Fail to shmat shm_sniff_addr: errno=%d\n",errno);
#else
			SendTraceMsg(0,0,"\nSniffer: Fail to shmat shm_sniff_addr: errno=%d\n",GetLastError());
#endif
			exit(-1);
		}
		else
		{
		printf("create %s success!\n",memoryName);
			shm_sniff_addr->header.writePoint=shm_sniff_addr->header.readPoint=0;
		}
	}
	else
	{
		printf("link to %s success!\n",memoryName);
		protocolRec->writePoint=shm_sniff_addr->header.writePoint;
	}
#ifndef WIN32
	setreuid(-1,0);
#endif
	return shm_sniff_addr;
}

void WritetoLogFile(unsigned char *buf, unsigned int length, char * moduleName)
{
	FILE	*fp;
	unsigned int	d;
	struct tm ltime;
	time_t long_time;	
	char	logFileName[256];
	struct stat		statbuf;
	
	time( &long_time );   
	ltime=*localtime(&long_time);

	sprintf(logFileName,"%s%s_Log%02d.txt",ERROR_LOG_PATH,moduleName,ltime.tm_mday);

	if(-1==(stat(logFileName, &statbuf)))
		fp= fopen(logFileName, "w");
	else if((long_time-statbuf.st_mtime)>(3600*24))
		fp= fopen(logFileName, "w");
	else
		fp=fopen(logFileName, "a+");

	if(fp==NULL)
	{
		SendTraceMsg(0,0,"Fail to open %s", logFileName);
		return;
	}

	fprintf(fp, "\n%04d-%02d-%02d %02d:%02d:%02d data_length= %d\n", ltime.tm_year+1900,
		ltime.tm_mon+1, ltime.tm_mday, ltime.tm_hour, ltime.tm_min, ltime.tm_sec, length);

	for(d=1;d<=length;d++)
	{
		fprintf(fp,"%02x ",buf[d-1]);
		if((d%10)==0)
			fprintf(fp," ");
		if((d%20)==0)
			fprintf(fp,"\n");
	}

	fclose(fp);
}

/*
	print to console,log file and share memory
	shm_sniff_addr 写入的共享内存地址
	recvbuf 接收数据包的缓冲区
	begin 将写入共享内存的数据在recvbuf中的开始地址
	count 写入的字节数
*/
void	SaveRecvData(SHM_SNIFFER *shm_sniff_addr, unsigned char *recvbuf,unsigned int begin,unsigned int count,int printToLogFile,char * moduleName)
{
	WritetoShare(shm_sniff_addr,recvbuf+begin,count);	

	if(printToLogFile)
		WritetoLogFile(recvbuf+begin, count, moduleName);

#ifdef DEBUG
	PrintRecvData(recvbuf+begin, count);
#endif
}

//write to share memory
void WritetoShare(SHM_SNIFFER * shm_sniff_addr,u_char * buf,int length)
{
	unsigned int 	writePoint;
	if(length==0) return;
	if (shm_sniff_addr==NULL)
		return;
	writePoint = shm_sniff_addr->header.writePoint;
	shm_sniff_addr->packageLength[writePoint]=length;
	memcpy(&shm_sniff_addr->buf[writePoint][0],buf,length);
	writePoint++;
	if(writePoint>=BufferRecordCount) 
		writePoint=0;
	shm_sniff_addr->header.writePoint=writePoint;
}
#ifdef WIN32
void OpenNewTermalWindow ()
{
	STARTUPINFO				sui;
	int i;
	char	cmdline[128];
	DWORD ret;

	hMutex= CreateMutex( NULL, TRUE, MuteOfSnifferChild );
	if ( GetLastError () == ERROR_ALREADY_EXISTS )
		return;

	if ( SetConsoleCtrlHandler ((void *)ExitRoutine,TRUE ) == 0 )
	{	
		SendTraceMsg(0,0,"\n !!! sniffer: SetConsoleCtrlHandler ERROR %d!!! \n",GetLastError());
		exit ( -1 );
	}
	
	GetStartupInfo(&sui);

	SetConsoleTitle ( "sniffer主进程" );

	for(i=0;i<protocolCount;i++)
	{
		sprintf(cmdline,"%s %d",EXE_FILE,i);
		
		sui.cb       		 = sizeof (STARTUPINFO);
		sui.lpReserved       = NULL;
		sui.cbReserved2      = 0;
		sui.lpReserved2      = NULL;
		sui.lpDesktop 		 = NULL;
		sui.dwFlags          = 0;
		sui.lpDesktop        = NULL;
		sui.lpTitle          = NULL;
		sui.dwX              = 0;
		sui.dwY              = 0;
		sui.dwXSize          = 0;
		sui.dwYSize          = 0;
		sui.dwXCountChars    = 0;
		sui.dwYCountChars    = 0;
		sui.dwFillAttribute  = 0;
		sui.wShowWindow      = 0;

		if( CreateProcess ( NULL,cmdline, NULL, NULL, FALSE,
							 CREATE_NEW_CONSOLE, NULL, NULL, &sui, &protocolRec[i].Pid)==FALSE )

		{	
			SendTraceMsg(0,0,"\n !!!CREATE CHILD WINDOW %s %d ERROR %d!!! \n",EXE_FILE,i,GetLastError());
		}
		Sleep(3);
	}

	for ( ;; )//watch dog
	{ 
		for(i=0;i<protocolCount;i++)
		{
			ret= WaitForSingleObject ( protocolRec[i].Pid.hProcess, 0 );
			if( ret == WAIT_OBJECT_0 )
			{
				CloseHandle(protocolRec[i].Pid.hThread);
				CloseHandle(protocolRec[i].Pid.hProcess);
				/*GetTimeStr(err_buf);*/
				SendTraceMsg(0,0,"\n !!!Reboot %s %d CHILD WINDOW Now!!! \n",EXE_FILE,i);

				sprintf(cmdline,"%s %d",EXE_FILE,i);
				if( CreateProcess ( NULL,cmdline, NULL, NULL, FALSE,
									 CREATE_NEW_CONSOLE, NULL, NULL, &sui, & protocolRec[i].Pid)==FALSE )
				{	
						SendTraceMsg(0,0,"\n !!!Reboot CHILD WINDOW %s %d ERROR %d!!!\n",EXE_FILE,i,GetLastError());
				}
			}	
			Sleep ( 2 );	

		}
	}
}
#else
void CreateSnifferProcess()
{
	int i,wpid,pid;
	wpid=-1;
	for(;;)
	{
		for(i=0;i<protocolCount;i++)
		{
			if((wpid == protocolRec[i].Pid)||(-1 == protocolRec[i].Pid))
			{
				switch (pid=fork())
				{
				case -1:
					printf("fork() for %s error.\n",protocolRec[i].snifferName);
					sleep(5);
					continue;
				case 0: //child
					protocolNo=i;
					snifferOneNetwork(protocolRec[i].device,protocolRec[i].filter);
					break;
				default://father
					protocolRec[i].Pid=pid;
					break;
				}
			}
		}
		
		pid= fork();
		if(pid==0)
		{
			CheckSnifferActive();
			exit(0);
		}
		else
			checkChildPid= pid;
			
		for (;;)
		{
			wpid=wait(NULL); //如果有错误发生则返回-1
			printf("wpid:%d\n",wpid);
			if(wpid!=-1)
			{
				kill(checkChildPid, SIGKILL); 
				break;
			}
		}
	}
}
#endif
#ifdef WIN32
void ExitRoutine ( DWORD signal_type )
{
	int		i;
	UINT	uExitCode= 0;
	

	switch ( signal_type )
	{
	case CTRL_C_EVENT:
		printf ( "\n!!! CTRL_C_EVENT received !!! \n" ); 
		break;

	case CTRL_BREAK_EVENT:
		printf ( "\n!!! CTRL_BREAK_EVENT received !!! \n" ); 
		break;
	
	case CTRL_CLOSE_EVENT:
		printf ( "\n!!! CTRL_CLOSE_EVENT received !!! \n" ); 
		break;
	
	case CTRL_LOGOFF_EVENT:
		printf ( "\n!!! CTRL_LOGOFF_EVENT received !!! \n" ); 
		break;
	
	case CTRL_SHUTDOWN_EVENT:
		printf ( "\n!!! CTRL_SHUTDOWN_EVENT received !!! \n" ); 
		break;
	
	default:
		break;
	}
	UnMountDB();
#ifdef MultiSniffer
	for(i=0;i<protocolCount;i++)
	{
		if ( TerminateProcess ( protocolRec[i].Pid.hProcess, uExitCode ) == 0 )
		{
			printf ( "ERROR in TerminateProcess %d ( num=%d ), Err=%d %d\n", protocolRec[i].Pid.hProcess,i,uExitCode,GetLastError());
		}

		if ( TerminateThread ( protocolRec[i].Pid.hThread, uExitCode ) == 0 )
		{
			printf ( "ERROR in TerminateProcess (hThread ) %d ( num=%d ), Err=%d %d\n",protocolRec[i].Pid.hThread,i,uExitCode,GetLastError());
		}
		SendTraceMsg(0,0,"\n !!!Sniffer %d Terminate Now!!! \n",i);
		
	}
	
#endif
	exit (0 );
}
#else
void Endproc(int sig_no)
{
	int i;
	
	printf("  ---- Master exit signal %d received!  ----\n",sig_no);
	if( getpid()== protocolRec[protocolCount].Pid )
	{
		for(i=0;i<protocolCount;i++)
		{
			printf("\n---- %s Sniffer Master stop! ( pid=%05d ) ----\n",protocolRec[i].snifferName,protocolRec[i].Pid);
			kill( protocolRec[i].Pid,SIGKILL );
		}
		UnMountDB();
	}
	exit(0);
} 
#endif

void	UpdateSnifferState(char *protocolName, USHORT state)
{
	char	entry_name[256];
	POINTER		point;
	USHORT		old_state;

	sprintf(entry_name, "%s_%s_STAT", hostname, protocolName);
	
	point.stn_id=1;
	
	if(-1==GetSysLcuId(point.stn_id, &point.dev_id))
		return;
	
	point.data_type= SYSMSG_TYPE;
	
	if(-1==GetEntryIdByName(entry_name, &point))
		return;

	GetIndPtStateById(&point, &old_state);
	if(old_state!=state)
	{
		BroadCastIndPtState(point, 0, state);
		printf("Broadcast %s state %d", entry_name, state);
	}	
}

void CheckSnifferActive(void)
{
	int	i;
	unsigned int readPoint[MAX_PROTOCOL_NUM];

	for(i=0;i<protocolCount;i++)
		readPoint[i]= protocolRec[i].writePoint;

	for(;;)
	{
		Sleep(2000);
		for(i=0;i<protocolCount;i++)
		{
			if(protocolRec[i].shareMemory->header.writePoint==readPoint[i])
				UpdateSnifferState(protocolRec[i].protocolName, 1);
			else
			{
				readPoint[i]= protocolRec[i].shareMemory->header.writePoint;
				UpdateSnifferState(protocolRec[i].protocolName, 0);
			}
		}
	}
}
