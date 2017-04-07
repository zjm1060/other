/****************************************************************************************
    File name: AB LOGIX PLC.c
 
    Description:

    History:    
    修改：	 邓小刚 	Date	05.9	Comment 
	modified	WENZHENGGUO 2009
****************************************************************************************/
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <memory.h>
#include        <malloc.h>
#include        <fcntl.h>
#include        <errno.h>
#include        <termios.h>
#include        <time.h>
#include        <signal.h>
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <sys/ioctl.h>
#include        <linux/if.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>
#include		<moxadevice.h>
#include	    <stddef.h>
#include	    <unistd.h>
#include        <pthread.h>



#include "main.h"   
#include "abplc.h"



int SendPLC_AB(MYSOCKET *sockplc, char *buffer, int count);



void DwordToByteArray(unsigned int src1,unsigned int src2,char dest1[8])
{
	int dw[2];
	char *temp;
	int i;
	dw[0]=src2;
	dw[1]=src1;
	temp=(char *)&dw[0];
	for(i=0;i<8;i++)
	{
		dest1[i]=temp[i];
	}
}

int dwSenderContext;

//填冲CIP Request发送时的数据
void InitCIPRequest(CIPWrite *ciprq)
{
	CIPRequest g_ciprq;
	int		len;
	int		i;

	ciprq->eciphead.sintLength=0x0004;
	ciprq->eciphead.intSessionHandle=0;
	ciprq->eciphead.intStatus=0; 
	for(i=0;i<8;i++)
		ciprq->eciphead.bSenderContext[i]=0;
	ciprq->eciphead.intOption=0;	  
	ciprq->eciphead.sintCommand = 0X6F;
	ciprq->cpfhead.intInterfaceHandle=0x0;             //下面六个值，一般来讲是固定为以下值
	ciprq->cpfhead.sintTimeout=0x5;
	ciprq->cpfhead.sintItemCount=0x2;
	ciprq->cpfhead.sintAddressTypeID=0x0;
	ciprq->cpfhead.sintAddressItemLength=0x0;
	ciprq->cpfhead.sintDataTypeID=0xb2;
	ciprq->ciphead.bService=0x52;            //服务类型,此六个值一般来讲也是不变的
	ciprq->ciphead.bRequestPath=0x2;        //以下占用两个字     
	ciprq->ciphead.bLogicalClassSegment =0x20; //8位逻辑类
	ciprq->ciphead.bClass =0x06;               //连接管理器类
	ciprq->ciphead.bLogicalInstanceSegment =0x24;  //8位逻辑类对象
	ciprq->ciphead.bInstance =0x1;             //实例1
	ciprq->sendhead.bPriorityAndTime_tick=0x5; //此两个值可改为其它值
	ciprq->sendhead.bTime_out_ticks=(char)0x99;
	ciprq->sendhead.bService=0x0a;  //服务类型为多个服务请求
	ciprq->sendhead.bRequestPathSize=0x02;  //后五个值一般也是不变的
	ciprq->sendhead.bRequestPathClassSegment=0x20;
	ciprq->sendhead.bClass=0x02;
	ciprq->sendhead.bRequestPathInstacneSegment=0x24;
	ciprq->sendhead.bInstance=0x01;
	//Message Route 的长度
	ciprq->sendhead.sintMessageRequestSize = 0; //??? yyp  需要在最后添加
}

int CvtWriteToBuf(char *buffer, int* p_bytesend, CIPWrite cipwt)
{
	int pos,len;
	int i;
	int srvdatalen=0;
	CIPWrite cipwt_tmp;
	
	cipwt_tmp = cipwt;
	for(i=0;i<cipwt.tagnum.usintNumberOfServices;i++)
	{
		srvdatalen+=4;
		srvdatalen+=cipwt.srvdata[i].bDataSize;
		if(cipwt.srvdata[i].bDataSize%2)
			srvdatalen++;
		srvdatalen+=8;
		srvdatalen+=cipwt.srvdata[i].ElementNum * SEGBYTE;  //yyp ???
		if((cipwt.srvdata[i].ElementNum  * SEGBYTE)%2)
			srvdatalen++;
	}
	//从sendhead第五字节到path之前的总长
	cipwt.sendhead.sintMessageRequestSize =	SVR_REQ_DATA_SIZE -4 + 
		SVR_AGGR_SIZE +  cipwt.tagnum.usintNumberOfServices * sizeof(cipwt.offset[i]) +
		srvdatalen;
	//CIP的长度,从ciphead第一字节到path之后的总长
	cipwt.cpfhead.sintDataItemLength =   CIP_HEAD_SIZE + SVR_REQ_DATA_SIZE + 
		SVR_AGGR_SIZE +  cipwt.tagnum.usintNumberOfServices * sizeof(cipwt.offset[i]) +
		srvdatalen + sizeof(cipwt.path);
	cipwt.eciphead.sintLength =  cipwt.cpfhead.sintDataItemLength + sizeof(cipwt.cpfhead);
	DwordToByteArray(dwSenderContext++,0x2222,cipwt.eciphead.bSenderContext);


#ifdef BIG_ENDIAN_
	Swap2Byte(&cipwt.eciphead.sintCommand);
	Swap2Byte(&cipwt.eciphead.sintLength);
	Swap4Byte(&cipwt.eciphead.intSessionHandle);
	Swap4Byte(&cipwt.eciphead.intStatus);
	Swap4Byte(&cipwt.eciphead.intOption);

	Swap4Byte(&cipwt.cpfhead.intInterfaceHandle);
	Swap2Byte(&cipwt.cpfhead.sintTimeout);
	Swap2Byte(&cipwt.cpfhead.sintItemCount);
	Swap2Byte(&cipwt.cpfhead.sintAddressTypeID);
	Swap2Byte(&cipwt.cpfhead.sintAddressItemLength);
	Swap2Byte(&cipwt.cpfhead.sintDataTypeID);
	Swap2Byte(&cipwt.cpfhead.sintDataItemLength);

	Swap2Byte(&cipwt.sendhead.sintMessageRequestSize);
	Swap2Byte(&cipwt.tagnum.usintNumberOfServices);

	for(i=0;i<MAX_TAGS;i++)
	{
		Swap2Byte(&(cipwt.offset[i]));
	}
	for(i=0;i<MAX_TAGS;i++)
	{
		Swap2Byte(&(cipwt.srvdata[i].IOIType));
		Swap2Byte(&(cipwt.srvdata[i].StartPos));
		Swap2Byte(&(cipwt.srvdata[i].ElementNum));
	}

#endif	
	
	pos = 0;

	len= sizeof(cipwt.eciphead);
	memcpy(buffer+pos,&(cipwt.eciphead),len);
	pos+=len;

	len= sizeof(cipwt.cpfhead);
	memcpy(buffer+pos,&(cipwt.cpfhead),len);
	pos+=len;

	len= CIP_HEAD_SIZE;
	memcpy(buffer+pos,&(cipwt.ciphead),len);
	pos+=len;

	len= SVR_REQ_DATA_SIZE;
	memcpy(buffer+pos,&(cipwt.sendhead),len);
	pos+=len;

	len= SVR_AGGR_SIZE;
	memcpy(buffer+pos,&(cipwt.tagnum),len);
	pos+=len;

	for(i=0;i<cipwt_tmp.tagnum.usintNumberOfServices;i++)
	{
		len= sizeof(cipwt.offset[i]) ;
		memcpy(buffer+pos,&(cipwt.offset[i]),len);
		pos+=len;
	}
	for(i=0;i<cipwt_tmp.tagnum.usintNumberOfServices;i++)
	{
		len= 4 ;
		memcpy(buffer+pos,&(cipwt.srvdata[i].bService),len);
		pos+=len;
		len= cipwt_tmp.srvdata[i].bDataSize;
		memcpy(buffer+pos,cipwt.srvdata[i].TagName,len);
		pos+=len;
		if(cipwt_tmp.srvdata[i].bDataSize%2) //标签名称的长度为奇数时，应有填充一个字节
			 buffer[pos++]=0x00;
		len = 8;
		memcpy(buffer+pos,&(cipwt.srvdata[i].IOIType),len);
		pos+=len;
		len = cipwt_tmp.srvdata[i].ElementNum  * SEGBYTE;
		memcpy(buffer+pos,&(cipwt.srvdata[i].data[0]),len);
		pos+=len;
		if((cipwt_tmp.srvdata[i].ElementNum * SEGBYTE)%2) //标签名称的长度为奇数时，应有填充一个字节
			 buffer[pos++]=0x00;
	}
	len= sizeof(cipwt.path) ;
	memcpy(buffer+pos,&(cipwt.path),len);
	pos+=len;
	*p_bytesend = pos;
	return TRUE;
}





int RecvPLC_AB(MYSOCKET *sockplc, char *buffer, int count)
{
	int result;
	int error = 0, len = sizeof(int);	
	int i;
	int timeout_s,timeout_ms;

	if(sockplc->sock == INVALID_SOCKET)
	{
		SendTraceMsg("RecvPLC_AB():error sockplc=%d ",sockplc->sock);	
		return FALSE;
	}
	for (i = 0; i < count; i += result) 
	{
		getsockopt(sockplc->sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
		if (error != 0)
		{
			SendTraceMsg("RecvPLC_AB(): getsock error=%d ",error);	
			return FALSE;		
		}
		result = SOCKET_ERROR;
		timeout_s = SOCK_TIMEOUT_S;
		timeout_ms = SOCK_TIMEOUT_MS;
		if(xnet_select(sockplc->sock, timeout_s, timeout_ms, READ_STATUS)<=0)
		{
			SendTraceMsg("RecvPLC_AB(): recv data from plc timeout");	
			return FALSE;		
		}

		result = recv(sockplc->sock, (char *)(buffer + i), count - i, RECV_FLAGS);

		error = 0;
		getsockopt(sockplc->sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
  		if (error != 0)
		{
			SendTraceMsg("RecvPLC_AB(): getsock after recv error=%d ",error);	
			return FALSE;
		}
		
		if ((result == SOCKET_ERROR) ||( result == 0))
		{
			SendTraceMsg("RecvPLC_AB():error recv() timeout or result invalid. res=%d \n",result);	
			return (FALSE);
		}
	}

	if (i != count)
	{
		SendTraceMsg("RecvPLC_AB(): received data num error %d need= %d", i, count);
		return (FALSE);
	}

	return TRUE;
}
/***************************************************************************************
write a socket under stream schema

****************************************************************************************/

int SendPLC_AB(MYSOCKET *sockplc, char *buffer, int count)
{
	int		result;

	int error = 0, len = sizeof(int);

	result = SOCKET_ERROR;

	if(sockplc->sock == INVALID_SOCKET)
	{
		SendTraceMsg("SendPLC_AB():error sockplc=%d ",sockplc->sock);	
		return FALSE;
	}

	if(count < 0 )
	{
		SendTraceMsg("SendPLC_AB(): error,count=%d\n",count);
		return FALSE;
	}

	getsockopt(sockplc->sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if (error != 0)
	{
		SendTraceMsg("SendPLC_AB(): getsock error=%d ",error);	
		return FALSE;
	}

	if(xnet_select(sockplc->sock, SOCK_TIMEOUT_S, SOCK_TIMEOUT_MS, WRITE_STATUS)<=0)
	{
		SendTraceMsg("SendPLC_AB():error send() timeout");
		return FALSE;
	}

	if((result=send(sockplc->sock, (char *)buffer, count, 0)) != count)
	{
		SendTraceMsg("SendPLC_AB(): Fail to send res= %d ", result);	
		return FALSE;		
	}

	error = 0;
	getsockopt(sockplc->sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if(error != 0)
	{
		SendTraceMsg("SendPLC_AB(): getsock after send error=%d ",error);	
		return FALSE;		
	}
	return TRUE;
}




int WritePLC_AB(MYSOCKET *sockplc, char *tagName, int ref, int size, unsigned char *buffer)
{
	int i;
	int result;
	int wordlen;
	int offset;
	int bytesend,byterecv;
	CIPWrite cipwt;
	EncapsulationCIPHead * HeadRec;
	unsigned char dummy_send[SEND_SIZE_AB];
	unsigned char dummy_recv[MAX_QUERY_SIZE_AB * 2];

	if( ref<0 || size <0 || size >= MAX_QUERY_SIZE_AB )
	{
		SendTraceMsg("write data error: ref= %d size= %d", ref, size);
		return FALSE;
	}
	if(size==0)
		return TRUE;
	
	InitCIPRequest(&cipwt);
	cipwt.eciphead.intSessionHandle = sockplc->session;  

	offset=0;
	wordlen = size;
	cipwt.tagnum.usintNumberOfServices = 1;
	cipwt.offset[0] = cipwt.tagnum.usintNumberOfServices*2 +2;
	for(i=0;i<cipwt.tagnum.usintNumberOfServices;i++)
	{
		cipwt.srvdata[i].bService=0x4d;  //写数
		cipwt.srvdata[i].bRequirePathWords=(char)(strlen(tagName)+3)/2; //后面字段的16bit数
		cipwt.srvdata[i].bExtendedSymbolSegment=(char)0x91;  //为IOS字串,即标签名称
		cipwt.srvdata[i].bDataSize=strlen(tagName);  //标签名称长度
		strcpy(cipwt.srvdata[i].TagName,tagName);  //复制标签名称
		//计算除了第一标签以外的标签偏移量
		if(i>=1)
			 cipwt.offset[i]=cipwt.srvdata[i-1].bRequirePathWords*2+4+cipwt.offset[i-1]; //?? yyp
		if(cipwt.srvdata[i].bDataSize%2) //标签名称的长度为奇数时，应有填充一个字节
			 cipwt.srvdata[i].TagName[strlen(cipwt.srvdata[i].TagName)]=0x00;
		cipwt.srvdata[i].bRequirePathWords += 2;		//for reserve
		cipwt.srvdata[i].IOIType = 0x0029;  
		cipwt.srvdata[i].bDataType = (char)SHORT_ID;					//yyp ????
		cipwt.srvdata[i].bReserve = 0;
		cipwt.srvdata[i].StartPos = ref;
		cipwt.srvdata[i].ElementNum  = wordlen;
		memcpy(cipwt.srvdata[i].data, buffer, wordlen * SEGBYTE);
	}
	cipwt.path.bRoutePathSizeWord=0x01;  //路径地址16bit数
	cipwt.path.BResever=0x00; //保留
	cipwt.path.bExtendLinkAddress=0x01;  //是否扩展地址
	cipwt.path.bLinkAddress=0x00;        //地址(CPU槽号)
	CvtWriteToBuf((char *)dummy_send,&bytesend,cipwt);	

	if(!SendPLC_AB(sockplc, (char *)dummy_send, bytesend))
	{
		SendTraceMsg("Fail to SendPLC_AB: byte= %d", bytesend);
		return FALSE;
	}
	SendTraceMsg("Success to SendPLC_AB: byte= %d", bytesend);
	
	byterecv = sizeof(EncapsulationCIPHead) +sizeof(CommandSpecificData) + sizeof(recCIP) + 2 
		+ 2 * cipwt.tagnum.usintNumberOfServices + 4 * cipwt.tagnum.usintNumberOfServices ;
	if(!RecvPLC_AB(sockplc, (char *)dummy_recv, byterecv))
	{
		SendTraceMsg("Fail to recv respond of write data");
		return(FALSE);
	}

	HeadRec=(EncapsulationCIPHead *)dummy_recv;
#ifdef BIG_ENDIAN_
	Swap2Byte(&HeadRec->sintCommand);
	Swap2Byte(&HeadRec->sintLength);
	Swap4Byte(&HeadRec->intSessionHandle);
	Swap4Byte(&HeadRec->intStatus);
	Swap4Byte(&HeadRec->intOption);
#endif
	//判断合法性
	if(HeadRec->sintCommand != 0x006f || HeadRec->intStatus!=0x0  )
	{
		SendTraceMsg("Fail to SendPLC_AB: respond cmd= 0x%04x status= 0x%04x", HeadRec->sintCommand, HeadRec->intStatus);
		return FALSE;
	}
	
	return (TRUE);	
}


int RegisterSession(MYSOCKET *p_sockplc)
{
	char	buffer[28];
	int		result;
	int		i;
	EncapsulationCIPHead * HeadRec;
	EncapsulationCIPHead CIPHead;
	char *bRSCommonData;
	
	SendTraceMsg("start to register session");
	
	bRSCommonData=(char *)&buffer[24];
	CIPHead.sintCommand=0x0065;
	CIPHead.sintLength=0x0004;
	CIPHead.intSessionHandle=0;
	CIPHead.intStatus=0;
	
	for(i=0;i<8;i++)
		CIPHead.bSenderContext[i]=0;
	
	CIPHead.intOption=0;
#ifdef BIG_ENDIAN_
	Swap2Byte(&CIPHead.sintCommand);
	Swap2Byte(&CIPHead.sintLength);
	Swap4Byte(&CIPHead.intSessionHandle);
	Swap4Byte(&CIPHead.intStatus);
	Swap4Byte(&CIPHead.intOption);
#endif
	memcpy(buffer,&CIPHead,24);
	bRSCommonData[0]=0x01;
	bRSCommonData[1]=0x0;
	bRSCommonData[2]=0x0;
	bRSCommonData[3]=0x0;
	
	if(!SendPLC_AB(p_sockplc, (char *)buffer, 28))
	{
		SendTraceMsg("Fail to send register request");
		return (FALSE);
	}
	SendTraceMsg("Success to send register request");
	
	if(!RecvPLC_AB(p_sockplc, (char *)buffer, 28))
	{
		SendTraceMsg("Fail to recv register respond");
		return(FALSE);
	}
	SendTraceMsg("Success to recv register respond");
	
	HeadRec=(EncapsulationCIPHead *)buffer;
#ifdef BIG_ENDIAN_
	Swap2Byte(&HeadRec->sintCommand);
	Swap2Byte(&HeadRec->sintLength);
	Swap4Byte(&HeadRec->intStatus);
	Swap4Byte(&HeadRec->intOption);
	Swap4Byte(&HeadRec->intSessionHandle);
#endif
	if(HeadRec->sintCommand != 0x0065 || HeadRec->intStatus!=0x0  )
	{
		SendTraceMsg("Received Session respond error: cmd= 0x%04x status= 0x%04x", HeadRec->sintCommand, HeadRec->intStatus);
		return(FALSE);
	}
	else
		p_sockplc->session = HeadRec->intSessionHandle;
	return TRUE;
}



int ConnectPLC(MYSOCKET *sockplc, char *address, unsigned short port)
{
	struct sockaddr_in pin;
#ifndef MSWIN32
	int f ;
#endif
	int error = -1, len = sizeof(int);
  
	unsigned int opt_val;
	int result,on,i;

	sockplc->sock = INVALID_SOCKET;

	pin.sin_family = AF_INET;
	pin.sin_port = htons(port);
	pin.sin_addr.s_addr = inet_addr(address);
	result= SOCKET_ERROR;

	if((sockplc->sock= socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		SendTraceMsg("Fail to create socket");
		return FALSE;
	}
	
	on=1;
	setsockopt(sockplc->sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&on,sizeof (on));

#ifndef MSWIN32
	f = fcntl(sockplc->sock, F_GETFL);
	result= fcntl(sockplc->sock, F_SETFL, f | O_NONBLOCK);	
#else
	opt_val   =   1;   
	result = ioctlsocket(sockplc->sock,   FIONBIO,   &opt_val); 
#endif

	if(result == SOCKET_ERROR)
	{
		SendTraceMsg("Fail to set nonblock socket");
		close(sockplc->sock);
		return FALSE;
	}

#ifndef MSWIN32
	fcntl(sockplc->sock,F_SETFL,O_NDELAY);
#endif		


	if(connect(sockplc->sock, (struct sockaddr*)&(pin), sizeof(pin)) == SOCKET_ERROR)
	{
		struct timeval tmval;
		fd_set set;
		int sel;
		
		tmval.tv_sec = 0;
		tmval.tv_usec = CONN_TIMEOUT_MS * 1000;
		FD_ZERO(&set);
		FD_SET(sockplc->sock, &set);
		if((sel=select(sockplc->sock + 1, NULL, &set, NULL, &tmval)) > 0)
		{
			getsockopt(sockplc->sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
			if (error != 0)
			{
				SendTraceMsg("Fail to connect %s", address);
				close(sockplc->sock);
				return FALSE;
			}
		}
	}
	
	if(!RegisterSession(sockplc))
	{
		SendTraceMsg("Fail to Register Session");
		close(sockplc->sock);
		return FALSE;
	}
	SendTraceMsg("Success to Register Session");
	
	return TRUE;
}


int WriteDataToPlc(MYSOCKET *plc_sock, char *tagName, int start_addr, int word_num, char *buf)
{
	int		dataSent= 0;

	SendTraceMsg("start to write data to plc: word_num= %d", word_num);
	while(dataSent < word_num) 
	{
		int		sendNum= (word_num-dataSent) < MAX_QUERY_SIZE_AB? (word_num-dataSent):MAX_QUERY_SIZE_AB;
		if(WritePLC_AB(plc_sock, tagName, start_addr+dataSent, sendNum, buf+dataSent*SEGBYTE))
			dataSent += sendNum;
		else
		{
			SendTraceMsg("Fail to WritePLC_AB: num= %d", sendNum);
			return FALSE;
		}
	}
	return TRUE;
}



AB_PLC_DEF	config_info;

int InitCommConf(void)
{
	char	*filename=CONFIG_DEFINE_FILE;
	char	*sect="AB_PLC";
	char	string[256];
	int		port;
	
	if(!GetProfInt(filename, sect, "PORT", &port))
		return FALSE;
	else
		config_info.port= (unsigned short)port;
	SendTraceMsg("plc port: %d", config_info.port);
	
	if(GetProfString(filename, sect, "IPADDR1", string))
	{
		if(-1==inet_addr(string))
		{ 
			SendTraceMsg("ipaddr %s error", string);
			return FALSE;
		}
		strcpy(config_info.ipaddr[config_info.ipaddr_num++], string);
		SendTraceMsg("plc ip addr1: %s", string);
	}
	if(GetProfString(filename, sect, "IPADDR2", string))
	{
		if(-1==inet_addr(string))
		{ 
			SendTraceMsg("ipaddr %s error", string);
			return FALSE;
		}
		strcpy(config_info.ipaddr[config_info.ipaddr_num++], string);
		SendTraceMsg("plc ip addr2: %s", string);
	}
	if(GetProfString(filename, sect, "IPADDR3", string))
	{
		if(-1==inet_addr(string))
		{ 
			SendTraceMsg("ipaddr %s error", string);
			return FALSE;
		}
		strcpy(config_info.ipaddr[config_info.ipaddr_num++], string);
		SendTraceMsg("plc ip addr3: %s", string);
	}
	if(GetProfString(filename, sect, "IPADDR4", string))
	{
		if(-1==inet_addr(string))
		{ 
			SendTraceMsg("ipaddr %s error", string);
			return FALSE;
		}
		strcpy(config_info.ipaddr[config_info.ipaddr_num++], string);
		SendTraceMsg("plc ip addr4: %s", string);
	}
	
	if(!GetProfString(filename, sect, "TAGNAME", string))
		return FALSE;
	if(strlen(string)>=sizeof(config_info.tagname))
	{
		SendTraceMsg("plc tagname too long(max %d): %s", sizeof(config_info.tagname)-1, string);
		return FALSE;
	}
	strcpy(config_info.tagname, string);
	SendTraceMsg("plc tagname: %s", config_info.tagname);
	
	return TRUE;
}


int main(int argc, char *argv[])
{
	MYSOCKET		plc_sock;
	int				task_id, i, j, curr_net=0;
	WORD			comm_state[MAX_COMM_TASK_NUM];
	
	SetModuleName("ABPLC");
	SetErrLogMode(argc, argv);
	
	SendTraceMsg("start to run ab plc comm program");
	
	if(-1==(task_id=GetTaskId(argc, argv)))
	{
		SendTraceMsg("Usage: program_name task_id, for example: acuvim.dxe 2");
		exit(-1);
	}

	if(!InitShareCommTable())
	{
		SendTraceMsg("Fail to mount share comm table");
		exit(-1);
	}	
	SendTraceMsg("Success to mount share comm table");
	
	if(!InitCommConf())
	{
		SendTraceMsg("Fail to read comm define file");
		exit(-1);
	}	
	SendTraceMsg("Success to read comm define file");
	
	
	plc_sock.sock= INVALID_SOCKET;
	memset(comm_state, 0, sizeof(comm_state));
	
	for(;;)
	{
		sleep(1);

		for(i=0; i<share_table->dev_num; i++)
		{
			int addr_error= 0;

			COMM_TASK_INFO	*dev= &share_table->dev[i];

			for(j=0; j<i; j++)
			{
				if(j==task_id) //不与本任务进行比较
					continue;

				COMM_TASK_INFO	*prev_dev= &share_table->dev[j];
				if(dev->start_pos < (prev_dev->start_pos+prev_dev->data_num*prev_dev->data_bytes/2))
				{
					SendTraceMsg("ERROR: %s addr %d is in previous task addr range", dev->sect_name, dev->start_pos);
					addr_error= 1;
					break;
				}
			}
			if(addr_error)
			{
				SendTraceMsg("WARRING: now to exit...");
				exit(0);
			}
			
			comm_state[dev->port_id]= dev->update_flag;
			Swap2Byte(&comm_state[dev->port_id]);
			
			SendTraceMsg("%s update_flag= %d, old= %d", dev->sect_name, dev->update_flag, dev->update_flag_old);
			if(dev->update_flag==dev->update_flag_old)
				continue;
			dev->update_flag_old= dev->update_flag;
			SendTraceMsg("%s have new data to send", dev->sect_name);
			
			if(plc_sock.sock==INVALID_SOCKET)
			{
				SendTraceMsg("Start to connect plc: %s", config_info.ipaddr[curr_net]);				
				if(!ConnectPLC(&plc_sock, config_info.ipaddr[curr_net], config_info.port))
				{
					closesocket(plc_sock.sock);
					plc_sock.sock= INVALID_SOCKET;
					SendTraceMsg("Fail to connect plc: %s", config_info.ipaddr[curr_net]);
					curr_net++;
					if(curr_net>=config_info.ipaddr_num)
						curr_net= 0;
				}
				else
					SendTraceMsg("Success to connect plc: %s", config_info.ipaddr[curr_net]);				
			}
			
			if(plc_sock.sock!=INVALID_SOCKET)
			{
				int		i, j;
				char	data_buf[MAX_COMM_DATA_NUM*2];

				if((dev->data_num*dev->data_bytes) > sizeof(data_buf))
				{
					SendTraceMsg("Error: max buf size %d, send size= %d", MAX_COMM_DATA_NUM*2, dev->data_num*dev->data_bytes);				
					continue;
				}
				memcpy(data_buf, dev->data_buf, dev->data_num*dev->data_bytes);

				if(dev->swap_flag)
				{
					for(i=0; i<dev->data_num; i++)
					{
						if(dev->data_bytes==2)
							Swap2Byte(&data_buf[i*2]);
						else if(dev->data_bytes==4)
							Swap4Byte(&data_buf[i*4]);
					}
				}
				
				PrintfData(dev->data_num*dev->data_bytes, data_buf, 0);

				if(!WriteDataToPlc(&plc_sock, config_info.tagname, dev->start_pos+MAX_COMM_TASK_NUM, dev->data_num*dev->data_bytes/2, data_buf))
				{
					closesocket(plc_sock.sock);
					plc_sock.sock= INVALID_SOCKET;
					SendTraceMsg("Fail to write %s data to plc: start= %d data_num= %d", dev->sect_name, 
						dev->start_pos, dev->data_num);				
					curr_net++;
					if(curr_net>=config_info.ipaddr_num)
						curr_net= 0;
				}
				else
					SendTraceMsg("Success to write %s data to plc: start= %d data_num= %d", dev->sect_name, 
						dev->start_pos, dev->data_num);				
			}
		}
		
		if(plc_sock.sock==INVALID_SOCKET)
		{
			SendTraceMsg("Start to connect plc: %s", config_info.ipaddr[curr_net]);				
			if(!ConnectPLC(&plc_sock, config_info.ipaddr[curr_net], config_info.port))
			{
				closesocket(plc_sock.sock);
				plc_sock.sock= INVALID_SOCKET;
				SendTraceMsg("Fail to connect plc: %s", config_info.ipaddr[curr_net]);
				curr_net++;
				if(curr_net>=config_info.ipaddr_num)
					curr_net= 0;
			}
			else
				SendTraceMsg("Success to connect plc: %s", config_info.ipaddr[curr_net]);				
		}
		if(plc_sock.sock!=INVALID_SOCKET)
		{
			if(!WriteDataToPlc(&plc_sock, config_info.tagname, 0, MAX_COMM_TASK_NUM, (char*)&comm_state))
			{
				closesocket(plc_sock.sock);
				plc_sock.sock= INVALID_SOCKET;
				SendTraceMsg("Fail to write comm states to plc");				
				curr_net++;
				if(curr_net>=config_info.ipaddr_num)
					curr_net= 0;
			}
			else
				SendTraceMsg("Success to write comm states to plc");				
		}
	}
}























