/******************************************************************************
**	This program is designed for  founction  module			 **
**                                                                           **
**	History																	 **
**																			 **
**	Date		author		comments                                         **
**	2004.12		Yuan/Hong	First version
**  2010.04		V3.0a-->V4.0a  by  zwj
**
******************************************************************************/
#include	"../inc/dl476_std.h"
#include	"../inc/dl476_data.h"
//#include	"../inc/lan_common.h"

#define		NO				0
#define		YES				1
#define		TIMEOUT			3	
int		timeout;
char	infobuf[1024];

//adde by wayne
char zLogText[200];
//long zSendBytes;
fd_set fset;
struct timeval zTout;

void	PrintSendData(unsigned int nbsend);
void PrintSockInfo(int sockfd);

void Timer(int timerno)
{
	timeout=YES;
	printf("\n******#$%@#$%@********in Timer@@@@@@@@@@@@@@@@@@\n");
}

void DatapduIntial()
{ 
#ifdef NARI
	datapdu.head.nari_head1=(short)GB_HEAD;
	datapdu.head.nari_head2=(short)GB_HEAD;
#endif

	datapdu.head.ctr=A_DATA;
	datapdu.head.ns=0;
	datapdu.head.nr=0;
	datapdu.head.priority=HIGH_PRIORITY;
	datapdu.head.length=0;
	
	datablock.bid=0;
	datablock.tableno=0;
	datablock.length=0;
	memset(datablock.data,0,MAX_BLOCK_SIZE);
}

void SendSingleDataBlock(void)
{
	unsigned short	apdu_len;

	apdu_len=datablock.length+BLOCK_HEAD_SIZE;/*基本数据块块头*/
//addey by wayne for debug
	if(apdu_len>2030){
		sprintf(zLogText,"\n big apdu_len%d,line at %d",apdu_len,__LINE__);
		TraceLog(strTemp1,zLogText);
	}

	Swap2Byte(&datablock.length);
	memcpy((char *)&datapdu.block, (char *)&datablock, apdu_len);

	datapdu.head.length=apdu_len;
	Swap2Byte(&datapdu.head.length);
	
	/************************Send single datablock**************************/
	if(apdu_len > BLOCK_HEAD_SIZE)
	{
		clientsock=TcpSend(clientsock,datapdu);
		if(clientsock!=INVALID_SOCKET) 
		{
			datapdu.head.ns++; 
		}
	//	usleep(1000 * 30);	/* MODIFIED WHEN ERR OCCUR YYP 08.02.21 */
		usleep(1000 * 150);	//modified by wayne
	}
}

void AllPackage(short tableno)
{
	unsigned short	j,pi_no,point_len;

	pi_no=0;
	
	if(table[tableno].type==ANA_TABLE)
	{
		datablock.bid=B_ALL_REAL;
		point_len=5;
	}
	else if(table[tableno].type==SW_TABLE)
	{	datablock.bid=B_ALL_SW;
		point_len=1;
	}
	else
		return;
	
	if(table[tableno].ptnum==0)	
		return;
	
	datablock.tableno=(char)tableno;
	datablock.length=2;	  /* 全送起始地址占两字节 */	
	memset(datablock.data,0,MAX_BLOCK_SIZE);
	memcpy(datablock.data,&pi_no,2);
	
	for(j=table[tableno].baseaddr;j<table[tableno].ptnum+table[tableno].baseaddr;j++)
	{	
		if(table[tableno].type==ANA_TABLE)
		{
			memcpy((datablock.data+datablock.length),&dataindex[j].data.real.value,4);
			Swap4Byte(&datablock.data[datablock.length]);
			memcpy((datablock.data+datablock.length+4),&dataindex[j].data.real.quality,1);
			dataindex[j].data.real.oldvalue=dataindex[j].data.real.value;
			datablock.length=datablock.length+5;
		}
		else if(table[tableno].type==SW_TABLE)
		{
			datablock.data[datablock.length]=dataindex[j].data.sw.value;
			datablock.length=datablock.length+1;
		}
		else
			continue;

		if( (datablock.length+point_len) > MAX_BLOCK_SIZE )
		{
			printf("dl476: send table%d all_send data, more data in table\n", tableno);
			SendSingleDataBlock();

			/***************************************************************************/
			datablock.length=2;
			memset(datablock.data,0,MAX_BLOCK_SIZE);/**/
			pi_no=j-table[tableno].baseaddr+1;
			memcpy(datablock.data,&pi_no,2);
		}
		
	}
	
	if(datablock.length>2)	
	{	
		printf("dl476: send table%d all_send data, end data in table\n", tableno);
		SendSingleDataBlock();
	}
}

void ChangedRealPackage(short k)
{
	unsigned short	j,pt;
	float		diff;		

	if(table[k].type!=ANA_TABLE)	
		return;
	
	datablock.bid=B_CHANG_REAL; 
	
	datablock.tableno=(char)k;

	datablock.length=0;	
	memset(datablock.data,0,MAX_BLOCK_SIZE);

	for(j=table[k].baseaddr;j<table[k].ptnum+table[k].baseaddr;j++)
	{	 
		diff=(float)fabs((double)(dataindex[j].data.real.value - dataindex[j].data.real.oldvalue));
		
		if( diff> dataindex[j].data.real.band)
		{
			pt=j-table[k].baseaddr;

#ifdef PRTLOG
			if(!strcmp(dataindex[j].name, "WJD.WJD_SYS.ANA.AVC_PRE")||!strcmp(dataindex[j].name, "WJD.WJD_SYS.ANA.STEP_LMT")||!strcmp(dataindex[j].name, "WJD.WJD_SYS.ANA.2_BUS_DLMT"))
			{
				FILE	*fp;

				if((fp=fopen(strTemp1,"a+"))!=NULL)
				{
					fprintf(fp, "\npt %s in table%d offset %d send change data: value= %f\n",
						dataindex[j].name, k, pt, dataindex[j].data.real.value);
					fclose(fp);
				}
			}
#endif

			memcpy(datablock.data+datablock.length,&pt,2);
			Swap2Byte(datablock.data+datablock.length);
			memcpy(datablock.data+datablock.length+2,&dataindex[j].data.real.value,4);
			Swap4Byte(datablock.data+datablock.length+2);
			memcpy(datablock.data+datablock.length+6,&dataindex[j].data.real.quality,1);
			datablock.length=datablock.length+ONE_CHANG_REAL;
			dataindex[j].data.real.oldvalue=dataindex[j].data.real.value;
		}
		
		if( (datablock.length+ONE_CHANG_REAL) > MAX_BLOCK_SIZE )	
		{
			printf("dl476: send table%d change data, more data in table\n", k);
			SendSingleDataBlock();

			/***************************************************************************/
			datablock.length=0;
			memset(datablock.data,0,MAX_BLOCK_SIZE);/**/
		}
	}
	
	if(datablock.length>0)	
	{	
		printf("dl476: send table%d change data, end data in table\n", k);
		SendSingleDataBlock();
	}
} 

void StoreRealValue()
{
	unsigned short	j;

	for(j=0;j<index_num;j++)
	{
		if(dataindex[j].tabletype==ANA_TABLE)
		{	
			dataindex[j].data.real.oldvalue=dataindex[j].data.real.value;
			/*printf("index %d set oldvalue %f\n",j,index[j].data.real.oldvalue);*/
		}
	}
	
	/*printf("store real value ok \n");*/
}

void StoreSWValue()
{
	unsigned short	j;

	/*	for(j=0;j<index_num;j++)
	{*/
	for(j=0;j<index_num;j++)
	{
		if(dataindex[j].tabletype==SW_TABLE)
		
		dataindex[j].data.real.oldvalue=dataindex[j].data.sw.value;
	}			
	/*	}*/
}

void Request( int  sock,char ctr_code)
{ 
	PROTOCOL_APDU ask;

	ask.head.ctr			= ctr_code | NO_PARA_SECTION;
	ask.head.run_mode		= RULE_0 | SINGLE_TRAN;
	ask.head.status_flag		= HOST_ONLINE ;
	ask.head.length			= 0;

}


void Response ( int sock,char ack_code,char reason)
{ 
	PROTOCOL_APDU 	res;

	res.head.ctr			= ack_code | NO_PARA_SECTION ;
	res.head.run_mode    		= RULE_0 | SINGLE_TRAN;
	res.head.status_flag  		= HOST_ONLINE ;
	res.head.reason			= reason;
	res.head.length			= 0;


}


void DataResponse ( int sockfd,char ack_code,char recv_num,char reason)
{ 
	DATACK_APDU	 data_ack; 

	data_ack.ctr   	= ack_code;
	data_ack.nr  	= recv_num;
	data_ack.reason = reason;

}

int SetSocket(int sockfd)
{
	int		time_on, timeout;
	int		sockopt_on = 1; /* value for turning on boolean socket option */
	
#ifdef MSWIN32	
	timeout=2000;
#else
	timeout=2;
#endif
    //    BOOL bDontLinger = FALSE;
	struct linger zLinger;



	setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout)); 
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));

	//added by wayne 
	zLinger.l_onoff = 0;
	zLinger.l_linger =2;
	if(setsockopt(sockfd,SOL_SOCKET,SO_LINGER,(char *)&zLinger,sizeof(zLinger))){
		sprintf(zLogText,"\n setsockopt SO_DONTLINGER failed %d",__LINE__);
		TraceLog(strTemp1,zLogText);
	}
	
	int nSendBuf = 32*1024;//32K
	if(setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,(char *)&nSendBuf,sizeof(nSendBuf))){
		sprintf(zLogText,"\n setsockopt SO_SNDBUF failed %d",__LINE__);
		TraceLog(strTemp1,zLogText);
	}


	time_on=1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,(char *)&time_on,sizeof (time_on)))
	{
		printf("\nERROR %d setting SO_KEEPALIVE socket option\n", errno);
		return -1;
	}
	
#ifdef TCP_NODELAY
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (const char *) &sockopt_on, sizeof (int) ) )
	{
		printf("\nERROR %d setting NODELAY socket option\n", errno);
		return -1;
	}
#endif
	return 0;
}


SOCKET	Do_Connect(void)
{
	SOCKET			sockfd;

	struct sockaddr_in   	serveraddr;

	int 			ret;
	FILE			*fp;

	//sockfd = INVALID_SOCKET;

	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET) 
	{
		printf("Do_Connect:: creat socket() failed\n");

	//	close(sockfd);

		return(INVALID_SOCKET);
	}

	SetSocket(sockfd);

	serveraddr.sin_family          = AF_INET;
	serveraddr.sin_port            = htons(WAN_PORT);

	if(hot_rnode >= rnode.number)	hot_rnode=0;  /*added 2005.12.28*/

	printf("dl476_fun::    hot_rnode =%d    ip=%s ----------\n",hot_rnode,rnode.ip[hot_rnode]);

	serveraddr.sin_addr.s_addr	=inet_addr(rnode.ip[hot_rnode]);

#ifndef MSWIN32
	timeout=NO;
	signal(SIGALRM,Timer);
	alarm(2);

	ret=connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));	

	if ((ret == SOCKET_ERROR) || (timeout == YES))
	{
		printf("connect  ip%d = %s failed: errno= %d\n",hot_rnode,rnode.ip[hot_rnode], errno);
		alarm(0);

		sprintf(zLogText,"\nbefore close at line:%d",__LINE__);
	TraceLog(strTemp1,zLogText);
		PrintSockInfo(sockfd);
		close(sockfd);
		sockfd = INVALID_SOCKET;

		hot_rnode++;
		if(hot_rnode>=rnode.number)		hot_rnode=0;

	}
	else
	{
		printf("\n\n --------------connect  ip%d=%s ok \n ",hot_rnode,rnode.ip[hot_rnode]);
		alarm(0);
		fp=fopen("/home/ems/h9000/tmp/HotRnode.dat","wb");
		if(fp==NULL)
		{
			printf("can't fopen(wb) /home/ems/h9000/tmp/HotRnode.dat\n");
		}
		else
		{
			printf("hot_rnode %d write in file-----------------\n",hot_rnode);
			fwrite(&hot_rnode,sizeof(short),1,fp);
			fclose(fp);
		}
		sprintf(zLogText,"\n after connect at line:%d",__LINE__);
	TraceLog(strTemp1,zLogText);
		PrintSockInfo(sockfd);
	}
#else	
	ret=connect(sockfd, (PSOCKADDR)&serveraddr, sizeof(serveraddr));

	if(ret==SOCKET_ERROR)
	{
		printf("connect  ip %d = %s failed \n",hot_rnode,rnode.ip[hot_rnode]);
		close(sockfd);
		sockfd=INVALID_SOCKET;
		
		hot_rnode++;
		if(hot_rnode>=rnode.number)		hot_rnode=0;

	}else
	{
		printf("connect  ip %d = %s ok \n ",hot_rnode,rnode.ip[hot_rnode]);
		fp=fopen("/home/ems/h9000/tmp/HotRnode.dat","wb");
		if(fp==NULL)
		{
			printf("can't fopen(wb) /home/ems/h9000/tmp/HotRnode.dat\n");
		}
		else
		{
			printf("hot_rnode %d write in file-----------------\n",hot_rnode);
			fwrite(&hot_rnode,sizeof(short),1,fp);
			fclose(fp);
		}
	}
#endif
	
	clientsock= sockfd;

	return(sockfd);
}

void	Do_Disconnect(SOCKET sockfd)
{
	int retval;
	if(INVALID_SOCKET!=sockfd){
		
		PrintSockInfo(sockfd);
	//	shutdown(sockfd,2);
		retval = close(sockfd);

		sprintf(zLogText,"\in Do_Disconnect at line:%d, close return:%d, errno:%d",__LINE__,retval,errno);
		TraceLog(strTemp1,zLogText);

	//	close(sockfd);//added for test by wayne
	}
	//	shutdown(clientsock,2);
	clientsock = INVALID_SOCKET;
}

unsigned short Do_Send(SOCKET sockfd,DATA_APDU apdu)
{
	int	rc;
	int ret;
	unsigned short	dcount;

	if(sockfd==INVALID_SOCKET)
	{
		printf("\ndl476_send: send socket invalid");
		return 0;
	}

	rc=0;                           /*add 050927*/
	dcount=apdu.head.length;
#ifdef SUN
	Swap2Byte(&dcount);
#endif
#ifndef MSWIN32
	FD_ZERO(&fset);
	FD_SET(sockfd,&fset);
	zTout.tv_sec =2;
	zTout.tv_usec = 0;
	alarm(3);
	ret = select(sockfd+1,NULL,&fset,NULL,&zTout);
	
#endif
	
	dcount=sizeof(BASEDATA_HEAD)+dcount;
	if(ret != -1){
		rc=send(sockfd,(char *)&apdu,dcount,0);    
	}else{
		sprintf(zLogText, "dl476_fun: select ip%d=%s error,ret=%d,data_length=%d,errno=%d\n",hot_rnode,rnode.ip[hot_rnode],ret,dcount, errno);
		TraceLog(strTemp1,zLogText);
		return(0);
	}


	if(rc!=dcount)
	{
#ifndef MSWIN32
alarm(0);
#endif
		printf(" dl476_fun: send ip%d=%s error, timeout=%d,ret=%d,data_length=%d,errno=%d\n",hot_rnode,rnode.ip[hot_rnode],timeout,rc,dcount, errno);
		sprintf(zLogText, "dl476_fun: send ip%d=%s error, timeout=%d,ret=%d,data_length=%d,errno=%d\n",hot_rnode,rnode.ip[hot_rnode],timeout,rc,dcount, errno);
		TraceLog(strTemp1,zLogText);
		return(0);
	}/*else{//debug for statistic of send bytes by wayne
		if(zSendBytes>1048576)
			zSendBytes = 0;
		zSendBytes = zSendBytes + rc;
		sprintf(zLogText, "dl476_fun: send bytes%d, total %d\n",rc,zSendBytes);
		TraceLog(strTemp1,zLogText);
	}
*/
	#ifndef MSWIN32 
		alarm(0);
	#endif


	printf("send: ctr=%d  nr=%d  ns=%d  priority=%d  length=%d\n",
		apdu.head.ctr, apdu.head.nr,apdu.head.ns,apdu.head.priority,dcount);

	PrintSendData(rc-sizeof(BASEDATA_HEAD));  /*打印记录发送数据块报文*/

	return(rc); 
}

unsigned	short	Do_Recv(SOCKET sockfd,char * buff,short maxlen)  
{
	unsigned short	dcount;
	int				nbytes; 

	int				SockErrorNo;

	dcount=0;

	for(;;)
	{
#ifndef MSWIN32
		timeout=NO;
		signal(SIGALRM,Timer);
		alarm(5);
		nbytes=recv(sockfd, buff+dcount,maxlen-dcount, 0);
		if(nbytes <= 0 || nbytes >maxlen || timeout==YES )               
		{ 	
			printf("\ndl476_fun::recv error or timeout: timeout=%d errno=%d\n",timeout, errno); 
			alarm(0); 
			return(0);
		}
		else
		{
			alarm(0);
			dcount=dcount+nbytes;			
			if(dcount==maxlen)	
				break;
		}

#else
		nbytes=recv(sockfd, buff+dcount,maxlen-dcount, 0);

		if(nbytes <= 0 || nbytes >maxlen  )               
		{ 
			SockErrorNo =WSAGetLastError();
			printf("\ndl476_fun::recv error  %d\n" ,SockErrorNo);
			return(0);
		}
		else
		{ 
			dcount=dcount+nbytes;			
			if(dcount==maxlen)	
				break;
		} 
#endif
		
	}
	
	printf("Do_recv::  nbytes=%d maxlen=%d\n",nbytes,maxlen);
	return(dcount);
}

void	WriteMainInfo(char *buf)
{
#ifdef	PRTLOG
	FILE               *fp;
	
	if((fp=fopen(strTemp1,"a+"))==NULL)
	{
		printf("can't open file %s\n",strTemp1);                                
	}
	fprintf(fp, "%s",buf);
	fclose(fp);
#else
	return;
#endif
}

void	PrintSendData(unsigned int nbsend)
{
	unsigned int d;
#ifdef PRTLOG
//	FILE               *fp;
#endif
    time_t	tim;
	struct tm *timeptr;

	tim=time(&tim);
	timeptr=localtime(&tim);

	printf ("\nSendDatatime %02d:%02d:%02d!",timeptr->tm_hour,timeptr->tm_min,timeptr->tm_sec);
	for(d=0;d<nbsend;d++)
	{
		if((d%20)==0)
			printf("\n<< ");
		else if((d%10)==0)
			printf(" ");
		printf(" %02X",datapdu.block[d]);
	}
	printf("\n");

#ifdef PRTLOG
	/*
	if((fp=fopen(strTemp1,"a+"))!=NULL)
 	{
		fprintf (fp,"\nSendDatatime %02d:%02d:%02d!",timeptr->tm_hour,timeptr->tm_min,timeptr->tm_sec);
		
		for(d=0;d<nbsend;d++)
		{
			if((d%20)==0)
				fprintf(fp, "\n<< ");
			else if((d%10)==0)
				fprintf(fp, " ");
			fprintf(fp, " %02X",datapdu.block[d]);
		}
		fprintf(fp, "\n");
		fclose(fp);
	}
*/
#endif
}


int	TraceLog(char *traceFile,char     *text)
{
	FILE    *fp;
	char    filename[100];
	time_t  tt;
	struct tm ltm;
	char    timestr[19];
	char    all[200];
	int   offset;
	char    new[100];

	//close log funtion
	return(TRUE);

	time(&tt);
	GetTimeString(tt,all);
	strcat(all,"   ");
	strcat(all,text);
	//printf("all=%s\n",all);

	strcpy(filename,traceFile);
	strcat(filename,".log");

	//sleep(1);
	//printf("\n%s..%s\n",traceFile,text);

	fp=fopen(filename,"a+");
	if(fp==NULL)
	{
		printf("can not open file %s\n",filename);
		return(FALSE);
	}
	fseek(fp,0,2);
	offset=ftell(fp);

	if(offset>102400)
	{
		fclose(fp);
		sprintf(new,"%s.bak",filename);
		rename(filename,new);

		fp=fopen(filename,"a+");
		if(fp==NULL)
		{
			printf("can not open file %s\n",filename);
			return(FALSE);
		}
	}

	fprintf(fp,"%s\n",all);

	fclose(fp);
	return(TRUE);
}

//added by wayne for debug
void PrintSockInfo(int sockfd){	
	struct sockaddr_in   clientaddr;
	int len= sizeof(clientaddr);

	if(sockfd!=-1)
	{	
		getsockname(sockfd, (struct sockaddr *)&clientaddr, &len);
		sprintf(zLogText,"\nfilename:  ,line:%d: local port= %d",__LINE__, clientaddr.sin_port);
		TraceLog(strTemp1,zLogText);
	}
}