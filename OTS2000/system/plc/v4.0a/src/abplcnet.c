/****************************************************************************************
FILENAME:	abplcnet.c
FUNCTION:	
	some sub functions of plc comm  for "AB"	

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2008.4.29	add this file for ab plc		yyp
****************************************************************************************/

#include    "../inc/plchead_all.h"


/***************************************************************************************
read data from socket under stream schema

****************************************************************************************/
int RecvPLC_AB(MYSOCKET sockplc, char *buffer, int count)
{
	int result;
	int error = 0, len = sizeof(int);	
	int i;
	int timeout_s,timeout_ms;
	char log_str[256];
	if(sockplc.sock == INVALID_SOCKET)
	{
		sprintf(log_str,"RecvPLC_AB():error sockplc=%d ",sockplc.sock);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return FALSE;
	}
	for (i = 0; i < count; i += result) 
	{
		if(sockplc.sock == INVALID_SOCKET)
		{
			sprintf(log_str,"RecvPLC_AB():error sockplc=%d ",sockplc.sock);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);		
			return FALSE;
		}		
		getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
		if (error != 0)
		{
			sprintf(log_str,"RecvPLC_AB(): getsock error=%d ",error);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return FALSE;		
		}
		result = SOCKET_ERROR;
		timeout_s = SOCK_TIMEOUT_S;
		timeout_ms = SOCK_TIMEOUT_MS;
		if(xnet_select(sockplc.sock, timeout_s, timeout_ms, READ_STATUS)>0)
		{
			result = recv(sockplc.sock, (char *)(buffer + i), count - i, RECV_FLAGS);
		}
		else
		{
			
		}
		error = 0;
		getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
  		if (error != 0)
		{
			sprintf(log_str,"RecvPLC_AB(): getsock after recv error=%d ",error);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return FALSE;
		}
		
		if ((result == SOCKET_ERROR) ||( result == 0))
		{
			sprintf(log_str,"RecvPLC_AB():error recv() timeout or result invalid. res=%d \n",result);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return (FALSE);
		}
	}
	if (i != count)
	{
		sprintf(log_str,"RecvPLC_AB(): error return FALSE because recvcount!= required\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return (FALSE);
	}
#ifdef PLCDEBUG
	printf("\n\nRecvPLC_AB >>\n");
	for(i=0;i<count; i++)
	{
		printf("%02x=",(unsigned char)buffer[i]);

		if( (i+1)%20 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
#endif 
	return (count);
}
/***************************************************************************************
write a socket under stream schema

****************************************************************************************/

int SendPLC_AB(MYSOCKET sockplc, char *buffer, int count)
{
	int result;
#ifdef PLCDEBUG
	int i;
#endif
	char log_str[256];
	int error = 0, len = sizeof(int);
	result = SOCKET_ERROR;
	if(sockplc.sock == INVALID_SOCKET)
	{
		sprintf(log_str,"SendPLC_AB():error sockplc=%d ",sockplc.sock);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;
	}
	if(count <= 0 )
	{
		sprintf(log_str,"SendPLC_AB(): error,count=%d\n",count);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		return SOCKET_ERROR;
	}

	getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if (error != 0)
	{
		sprintf(log_str,"SendPLC_AB(): getsock error=%d ",error);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;
	}

	if(xnet_select(sockplc.sock, SOCK_TIMEOUT_S, SOCK_TIMEOUT_MS, WRITE_STATUS)>0)
	{
		result = send(sockplc.sock, (char *)buffer, count, 0);
	}
	else
	{
		
	}
	error = 0;
	getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if (error != 0)
	{
		sprintf(log_str,"SendPLC_MB(): getsock after send error=%d ",error);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;		
	}
	
	if ( result == SOCKET_ERROR )
	{	
		sprintf(log_str,"SendPLC_AB():error send() timeout ");	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}	
#ifdef PLCDEBUG
	printf("\n\nSend_AB >>\n");
	for(i=0;i<count; i++)
	{
		printf("%02x=",(unsigned char)buffer[i]);

		if( (i+1)%20 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
#endif 		
	return result;
}

/***************************************************************************************
read data from a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
sockplc:	socket link to plc
tagnum:     tag number,e.g. 1
ref:		starting address of	plc shared memory to be read
size:		word size of data to be read
buffer:		pointer of data  to be stored
****************************************************************************************/
int ReadPLC_AB(MYSOCKET sockplc, unsigned char tagid, int ref, int size, unsigned char *buffer)
{
	int i;
	int result;
	int j;
	int wordlen,bytelen_recv;
	int count;
	int offset,pos;
	short offset_cip,intTags;
	int base_addr;
	int bytesend,byterecv;
	CIPRequest ciprq;
	EncapsulationCIPHead * HeadRec;
	recCIP *ciphead;
	char log_str[256];
	unsigned char dummy_send[MAX_SCAN_BUF];
	unsigned char dummy_recv[MAX_SCAN_BUF];
	memcpy(&ciprq,&g_ciprq,sizeof(CIPRequest));
	ciprq.eciphead.intSessionHandle = sockplc.session;  
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"ReadPLC_AB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return FALSE;
	}
	sprintf(log_str,"==== Run ReadPLC_AB, tagnum=%d, ref=%d, size=%d \n",tagid,ref,size);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	base_addr = ref;
	count =(size-1) / ( MAX_QUERY_SIZE_AB ) + 1;
	offset=0;
	for(j=0;j<count;j++)
	{
		ref=base_addr + j * MAX_QUERY_SIZE_AB;
		if (j != count-1) 
		{
			wordlen = MAX_QUERY_SIZE_AB;
		} 
		else 
		{
			wordlen = size -j * MAX_QUERY_SIZE_AB; 
		}
		ciprq.tagnum.usintNumberOfServices = 1;
		ciprq.offset[0] = ciprq.tagnum.usintNumberOfServices*2 +2;
		for(i=0;i<ciprq.tagnum.usintNumberOfServices;i++)
		{
			 ciprq.srvdata[i].bService=0x4c;  //读数
			 ciprq.srvdata[i].bRequirePathWords=(char)((strlen(TAGNAME)+3)/2); //后面字段的16bit数
			 ciprq.srvdata[i].bExtendedSymbolSegment=(char)0x91;  //为IOS字串,即标签名称
			 ciprq.srvdata[i].bDataSize=strlen(TAGNAME);  //标签名称长度s
			 strcpy(ciprq.srvdata[i].TagName,TAGNAME);  //复制标签名称
			 //计算除了第一标签以外的标签偏移量
			 if(i>=1)
				 ciprq.offset[i]=ciprq.srvdata[i-1].bRequirePathWords*2+4+ciprq.offset[i-1]; //?? yyp 暂时不用
			 if(ciprq.srvdata[i].bDataSize%2) //标签名称的长度为奇数时，应有填充一个字节
				 ciprq.srvdata[i].TagName[strlen(ciprq.srvdata[i].TagName)]=0x00;
			 ciprq.srvdata[i].bRequirePathWords += 2;		//for reserve
			 ciprq.srvdata[i].IOIType = 0x0029; 
			 ciprq.srvdata[i].StartPos = ref;
			 ciprq.srvdata[i].ElementNum = wordlen;			//yyp ????
		}
		/* removed by yyp 2009.2.4
		ciprq.path.bRoutePathSizeWord=0x01;  //路径地址16bit数
		ciprq.path.BResever=0x00;	//保留
		ciprq.path.bPath[0]=0x01;	//是否扩展地址
		ciprq.path.bPath[1]=CPUSLOT;	//地址(CPU槽号)
		*/
		CvtReadToBuf((char *)dummy_send,&bytesend,ciprq,sockplc.readslave);
		result = SendPLC_AB(sockplc, (char *)dummy_send, bytesend);
		if (result == SOCKET_ERROR || result !=bytesend)
		{
			sprintf(log_str,"error SendPLC_AB() timeout or result=%d\n",result);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return (FALSE);
		}
		byterecv = sizeof(EncapsulationCIPHead);		//先读ECIPHEAD
		result = RecvPLC_AB(sockplc, (char *)dummy_recv, byterecv);
		if (result != byterecv )
		{
			sprintf(log_str,"ReadPLC_AB: call RecvPLC_AB(ECIPHEAD) error, res = %d\n",result);
			log_printf(H9000_LOG_ERROR,1,0,log_str);				
			return(FALSE);
		}
		HeadRec=(EncapsulationCIPHead *)dummy_recv;
#ifdef _BIG_ENDIAN_
		Swap2Byte(&HeadRec->sintCommand);
		Swap2Byte(&HeadRec->sintLength);
		Swap4Byte(&HeadRec->intSessionHandle);
		Swap4Byte(&HeadRec->intStatus);
		Swap4Byte(&HeadRec->intOption);
#endif
		//判断合法性
		if(HeadRec->sintCommand != 0x006f || HeadRec->intStatus!=0x0  )
		{
			sprintf(log_str,"RecvPLC_AB: check error, cmd=%d,status=%d\n",HeadRec->sintCommand,HeadRec->intStatus);
			err_write(log_str);				
			return(FALSE);			
		}
		else
		{
	
		}
		byterecv = HeadRec->sintLength;	//得到CIP长度,然后读CIP 数据区	
		result = RecvPLC_AB(sockplc, (char *)dummy_recv, byterecv);
		if (result != byterecv )
		{
			sprintf(log_str,"ReadPLC_AB: call RecvPLC_AB(ECIPHEAD) error, res = %d\n",result);
			log_printf(H9000_LOG_ERROR,1,0,log_str);				
			return(FALSE);
		}
		pos = sizeof(CommandSpecificData);
		ciphead=(recCIP *)&dummy_recv[pos]; 
		if((ciphead->bService==0x8a)&&(ciphead->bGeneralStatus==0x00))
		{
			pos+=4;
			memcpy(&intTags,&(dummy_recv[pos]),2);
			memcpy(&offset_cip,&(dummy_recv[pos+2]),2);	
#ifdef _BIG_ENDIAN_
			Swap2Byte(&intTags);
			Swap2Byte(&offset_cip);
#endif
			bytelen_recv = byterecv-pos-6-offset_cip;
			if(bytelen_recv == wordlen * SEGBYTE)
			{
				memcpy(buffer+offset,dummy_recv+pos+6+offset_cip,wordlen * SEGBYTE);		//yyp ???
				offset += wordlen * SEGBYTE;
			}
			else
			{
				sprintf(log_str,"ReadPLC_AB: wordlen %d !=  bytelen_recv %d err\n",wordlen,bytelen_recv);
				log_printf(H9000_LOG_ERROR,1,0,log_str);				
				return(FALSE);
			}
		}
		else
		{
			sprintf(log_str,"ReadPLC_AB: recIP  err, bService =%d, bStatus=%d\n",ciphead->bService,ciphead->bGeneralStatus);
			log_printf(H9000_LOG_ERROR,1,0,log_str);				
			return(FALSE);
		}
	}
/*
#ifdef 	PLCDEBUG
		printf("\nRecvPLC_AB:\n");
		for(i=0;i<offset;i++)
		{
			printf("=%02x",(unsigned char) buffer[i]);
			if((i+1)%20 == 0)
			{
				printf("\n");
			}		
		}
		printf("\n");
#endif
*/
	return TRUE;
}

int WritePLC_AB(MYSOCKET sockplc, unsigned char tagid, int ref, int size, unsigned char *buffer)
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
	char log_str[256];
	memcpy(&cipwt,&g_cipwt,sizeof(CIPWrite));
	cipwt.eciphead.intSessionHandle = sockplc.session;  
	if( ref<0 || size <=0 || size >= MAX_QUERY_SIZE_AB )
	{
		sprintf(log_str,"WritePLC_AB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return FALSE;
	}
	sprintf(log_str,"==== Run WritePLC_AB, seg=%d, ref=%d, size=%d \n",tagid,ref,size);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	offset=0;
	wordlen = size;
	cipwt.tagnum.usintNumberOfServices = 1;
	cipwt.offset[0] = cipwt.tagnum.usintNumberOfServices*2 +2;
	for(i=0;i<cipwt.tagnum.usintNumberOfServices;i++)
	{
		 cipwt.srvdata[i].bService=0x4d;  //写数
		 cipwt.srvdata[i].bRequirePathWords=(char)(strlen(TAGNAME)+3)/2; //后面字段的16bit数
		 cipwt.srvdata[i].bExtendedSymbolSegment=(char)0x91;  //为IOS字串,即标签名称
		 cipwt.srvdata[i].bDataSize=strlen(TAGNAME);  //标签名称长度
		 strcpy(cipwt.srvdata[i].TagName,TAGNAME);  //复制标签名称
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
		 memcpy(cipwt.srvdata[i].data,&(buffer[0]), wordlen * SEGBYTE);
	}
	CvtWriteToBuf((char *)dummy_send,&bytesend,cipwt, sockplc.readslave);	
	result = SendPLC_AB(sockplc, (char *)dummy_send, bytesend);
	if (result == SOCKET_ERROR || result !=bytesend)
	{
		sprintf(log_str,"error SendPLC_AB() timeout or result=%d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return (FALSE);
	}
#ifdef 	PLCDEBUG
	printf("\nSendPLC_AB:\n");
	for(i=0;i<bytesend;i++)
	{
		printf("=%02x",(unsigned char) dummy_send[i]);
		if((i+1)%20 == 0)
		{
			printf("\n");
		}		
	}
	printf("\n");
#endif
	byterecv = sizeof(EncapsulationCIPHead) +sizeof(CommandSpecificData) + sizeof(recCIP) + 2 
		+ 2 * cipwt.tagnum.usintNumberOfServices + 4 * cipwt.tagnum.usintNumberOfServices ;
	result = RecvPLC_AB(sockplc, (char *)dummy_recv, byterecv);
	if (result != byterecv )
	{
		sprintf(log_str,"WritePLC_AB: call RecvPLC_AB() error, res = %d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		return(FALSE);
	}
	HeadRec=(EncapsulationCIPHead *)dummy_recv;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&HeadRec->sintCommand);
	Swap2Byte(&HeadRec->sintLength);
	Swap4Byte(&HeadRec->intSessionHandle);
	Swap4Byte(&HeadRec->intStatus);
	Swap4Byte(&HeadRec->intOption);
#endif
	//判断合法性
	if(HeadRec->sintCommand != 0x006f || HeadRec->intStatus!=0x0  )
	{
		sprintf(log_str,"RecvPLC_AB: check error, cmd=%d,status=%d\n",HeadRec->sintCommand,HeadRec->intStatus);
		err_write(log_str);				
		return(FALSE);			
	}
	else
	{
		 //yyp needed to be added ?
	}
	return (TRUE);	
}

int RegisterSession(LCUID plcid,MYSOCKET *p_sockplc)
{
	char buffer[28];
	int result;
	int i;
	EncapsulationCIPHead * HeadRec;
	EncapsulationCIPHead CIPHead;
	char *bRSCommonData;
	char log_str[256];
	bRSCommonData=(char *)&buffer[24];
	CIPHead.sintCommand=0x0065;
	CIPHead.sintLength=0x0004;
	CIPHead.intSessionHandle=0;
	CIPHead.intStatus=0; 
	for(i=0;i<8;i++)
		CIPHead.bSenderContext[i]=0;
	CIPHead.intOption=0;
#ifdef _BIG_ENDIAN_
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
	result = SendPLC_AB(*p_sockplc, (char *)buffer, 28);
	if (result == SOCKET_ERROR || result !=28)
	{
		sprintf(log_str,"error SendPLC_AB() timeout or result=%d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		DisconnectPLC(p_sockplc,plcid);	
		return (FALSE);
	}
	result = RecvPLC_AB(*p_sockplc, (char *)buffer, 28);
	if (result != 28 )
	{
		sprintf(log_str,"RegisterSession: call RecvPLC_AB() error, res = %d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		DisconnectPLC(p_sockplc,plcid);
		return(FALSE);
	}
	HeadRec=(EncapsulationCIPHead *)buffer;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&HeadRec->sintCommand);
	Swap2Byte(&HeadRec->sintLength);
	Swap4Byte(&HeadRec->intStatus);
	Swap4Byte(&HeadRec->intOption);
	Swap4Byte(&HeadRec->intSessionHandle);
#endif
	if(HeadRec->sintCommand != 0x0065 || HeadRec->intStatus!=0x0  )
	{
		sprintf(log_str,"RegisterSession: check error, cmd=%d,status\n",HeadRec->sintCommand,HeadRec->intStatus);
		err_write(log_str);				
		return(FALSE);			
	}
	else
	{
		p_sockplc->session = HeadRec->intSessionHandle;
	}
	return TRUE;
}

//填冲CIP Request发送时的数据
void InitCIPRequest()
{
	int len;
	int i;
	g_ciprq.eciphead.sintLength=0x0004;
	g_ciprq.eciphead.intSessionHandle=0;
	g_ciprq.eciphead.intStatus=0; 
	for(i=0;i<8;i++)
		g_ciprq.eciphead.bSenderContext[i]=0;
	g_ciprq.eciphead.intOption=0;	  
	g_ciprq.eciphead.sintCommand = 0X6F;
	g_ciprq.cpfhead.intInterfaceHandle=0x0;             //下面六个值，一般来讲是固定为以下值
	g_ciprq.cpfhead.sintTimeout=0x5;
	g_ciprq.cpfhead.sintItemCount=0x2;
	g_ciprq.cpfhead.sintAddressTypeID=0x0;
	g_ciprq.cpfhead.sintAddressItemLength=0x0;
	g_ciprq.cpfhead.sintDataTypeID=0xb2;
	g_ciprq.ciphead.bService=0x52;            //服务类型,此六个值一般来讲也是不变的
	g_ciprq.ciphead.bRequestPath=0x2;        //以下占用两个字     
	g_ciprq.ciphead.bLogicalClassSegment =0x20; //8位逻辑类
	g_ciprq.ciphead.bClass =0x06;               //连接管理器类
	g_ciprq.ciphead.bLogicalInstanceSegment =0x24;  //8位逻辑类对象
	g_ciprq.ciphead.bInstance =0x1;             //实例1
	g_ciprq.sendhead.bPriorityAndTime_tick=0x5; //此两个值可改为其它值
	g_ciprq.sendhead.bTime_out_ticks=(char)0x99;
	g_ciprq.sendhead.bService=0x0a;  //服务类型为多个服务请求
	g_ciprq.sendhead.bRequestPathSize=0x02;  //后五个值一般也是不变的
	g_ciprq.sendhead.bRequestPathClassSegment=0x20;
	g_ciprq.sendhead.bClass=0x02;
	g_ciprq.sendhead.bRequestPathInstacneSegment=0x24;
	g_ciprq.sendhead.bInstance=0x01;
										//Message Route 的长度
	g_ciprq.sendhead.sintMessageRequestSize = 0; //??? yyp  需要在最后添加
	len = sizeof(g_ciprq.eciphead) + sizeof(g_ciprq.cpfhead) + sizeof(g_ciprq.ciphead) + sizeof(g_ciprq.sendhead);	
	memcpy(&g_cipwt,&g_ciprq,len);

}

int UnRegisterSession(LCUID plcid,MYSOCKET sockplc)
{
	char buffer[28];
	int result;
	int i;
	char log_str[256];
	EncapsulationCIPHead * HeadRec;
	EncapsulationCIPHead CIPHead;
	char *bRSCommonData;
	bRSCommonData=(char *)&buffer[24];
	CIPHead.sintCommand=0x0066;
	CIPHead.sintLength=0x0000;
	CIPHead.intStatus=0;
	for(i=0;i<8;i++)
		CIPHead.bSenderContext[i]=0;
#ifdef _BIG_ENDIAN_
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
	result = SendPLC_AB(sockplc, (char *)buffer, 28);
	if (result == SOCKET_ERROR || result !=28)
	{
		sprintf(log_str,"error SendPLC_AB() timeout or result=%d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		DisconnectPLC(&sockplc,plcid);	
		return (FALSE);
	}
	result = RecvPLC_AB(sockplc, (char *)buffer, 28);
	if (result != 28 )
	{
		sprintf(log_str,"UnRegisterSession: call RecvPLC_AB() error, res = %d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		DisconnectPLC(&sockplc,plcid);
		return(FALSE);
	}
	HeadRec=(EncapsulationCIPHead *)buffer;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&HeadRec->sintCommand);
	Swap2Byte(&HeadRec->sintLength);
	Swap4Byte(&HeadRec->intSessionHandle);
	Swap4Byte(&HeadRec->intStatus);
	Swap4Byte(&HeadRec->intOption);
#endif
	if(HeadRec->sintCommand != 0x0066 )
	{
		sprintf(log_str,"UnRegisterSession: check error, cmd=%d\n",HeadRec->sintCommand);
		err_write(log_str);				
		return(FALSE);			
	}
	return TRUE;
}

int CvtReadToBuf(char *buffer,int *p_bytesend,CIPRequest ciprq,BOOL readslave)
{
	int pos,len;	
	int i;
	int srvdatalen=0;
	unsigned short usintNumberOfServices;
	CIPRequest ciprq_tmp;
	ciprq_tmp = ciprq;
	if(readslave == TRUE)
	{
		ciprq.path.bRoutePathSizeWord=(ROUTE_SIZE_EXT-2)/2;  //路径地址16bit数
		ciprq.path.BResever=0x00;		//保留
		ciprq.path.bPath[0]=0x01;		//本地
		ciprq.path.bPath[1]=CNETSLOT;	//地址(ControlNet槽号)
		ciprq.path.bPath[2]=0x02;		//网络,非本地
		ciprq.path.bPath[3]=CNETIDM;	//主ControlNet ID
		ciprq.path.bPath[4]=0x01;		//主ControlNet ID
		ciprq.path.bPath[5]=CPUSLOT;		//主ControlNet ID
	}
	else
	{
		ciprq.path.bRoutePathSizeWord=(ROUTE_SIZE-2)/2;  //路径地址16bit数
		ciprq.path.BResever=0x00;		//保留
		ciprq.path.bPath[0]=0x01;		//本地
		ciprq.path.bPath[1]=CPUSLOT;	//地址(CPU槽号)
	}
	usintNumberOfServices = ciprq.tagnum.usintNumberOfServices;
	for(i=0;i<usintNumberOfServices;i++)
	{
		srvdatalen+=4;
		srvdatalen+=ciprq.srvdata[i].bDataSize;
		if(ciprq.srvdata[i].bDataSize%2)
			srvdatalen++;
		srvdatalen+=6;
	}
	//从sendhead第五字节到path之前的总长
	ciprq.sendhead.sintMessageRequestSize =	sizeof(ciprq.sendhead)-4 + 
		sizeof(ciprq.tagnum) +  ciprq.tagnum.usintNumberOfServices * sizeof(ciprq.offset[i]) +
		srvdatalen;
	//CIP的长度,从ciphead第一字节到path之后的总长
	ciprq.cpfhead.sintDataItemLength =   sizeof(ciprq.ciphead) + sizeof(ciprq.sendhead) + 
		sizeof(ciprq.tagnum) +  ciprq.tagnum.usintNumberOfServices * sizeof(ciprq.offset[i]) +
		srvdatalen;
	if(readslave == TRUE)
	{
		ciprq.cpfhead.sintDataItemLength += ROUTE_SIZE_EXT;
	}
	else
	{
		ciprq.cpfhead.sintDataItemLength += ROUTE_SIZE;
	}
	ciprq.eciphead.sintLength =  ciprq.cpfhead.sintDataItemLength + sizeof(ciprq.cpfhead);
	DwordToByteArray(dwSenderContext++,0x1111,ciprq.eciphead.bSenderContext);
#ifdef _BIG_ENDIAN_
	Swap2Byte(&ciprq.eciphead.sintCommand);
	Swap2Byte(&ciprq.eciphead.sintLength);
	Swap4Byte(&ciprq.eciphead.intSessionHandle);
	Swap4Byte(&ciprq.eciphead.intStatus);
	Swap4Byte(&ciprq.eciphead.intOption);

	Swap4Byte(&ciprq.cpfhead.intInterfaceHandle);
	Swap2Byte(&ciprq.cpfhead.sintTimeout);
	Swap2Byte(&ciprq.cpfhead.sintItemCount);
	Swap2Byte(&ciprq.cpfhead.sintAddressTypeID);
	Swap2Byte(&ciprq.cpfhead.sintAddressItemLength);
	Swap2Byte(&ciprq.cpfhead.sintDataTypeID);
	Swap2Byte(&ciprq.cpfhead.sintDataItemLength);

	Swap2Byte(&ciprq.sendhead.sintMessageRequestSize);
	Swap2Byte(&ciprq.tagnum.usintNumberOfServices);

	for(i=0;i<MAX_TAGS;i++)
	{
		Swap2Byte(&(ciprq.offset[i]));
	}
	for(i=0;i<MAX_TAGS;i++)
	{
		Swap2Byte(&(ciprq.srvdata[i].IOIType));
		Swap2Byte(&(ciprq.srvdata[i].StartPos));
		Swap2Byte(&(ciprq.srvdata[i].ElementNum));
	}

#endif	
	pos = 0;
	len= sizeof(ciprq.eciphead);
	memcpy(buffer+pos,&(ciprq.eciphead),len);
	pos+=len;
	len= sizeof(ciprq.cpfhead);
	memcpy(buffer+pos,&(ciprq.cpfhead),len);
	pos+=len;
	len= sizeof(ciprq.ciphead) ;
	memcpy(buffer+pos,&(ciprq.ciphead),len);
	pos+=len;
	len= sizeof(ciprq.sendhead) ;
	memcpy(buffer+pos,&(ciprq.sendhead),len);
	pos+=len;
	len= sizeof(ciprq.tagnum) ;
	memcpy(buffer+pos,&(ciprq.tagnum),len);
	pos+=len;
	for(i=0;i<usintNumberOfServices;i++)
	{
		len= sizeof(ciprq.offset[i]) ;
		memcpy(buffer+pos,&(ciprq.offset[i]),len);
		pos+=len;
	}
	for(i=0;i<usintNumberOfServices;i++)
	{
		len= 4 ;
		memcpy(buffer+pos,&(ciprq.srvdata[i].bService),len);
		pos+=len;
		len= ciprq_tmp.srvdata[i].bDataSize;
		memcpy(buffer+pos,ciprq.srvdata[i].TagName,len);
		pos+=len;
		if(ciprq_tmp.srvdata[i].bDataSize%2) //标签名称的长度为奇数时，应有填充一个字节
			 buffer[pos++]=0x00;
		len = 2;
		memcpy(buffer+pos,&(ciprq.srvdata[i].IOIType),len);
		pos+=len;
		len = 4;
		memcpy(buffer+pos,&(ciprq.srvdata[i].StartPos),len);
		pos+=len;
	}
	if(readslave == TRUE)
	{
		len= ROUTE_SIZE_EXT ;
	}
	else
	{
		len= ROUTE_SIZE ;
	}
	memcpy(buffer+pos,&(ciprq.path),len);
	pos+=len;
	*p_bytesend = pos;
	return TRUE;
}

int CvtWriteToBuf(char *buffer,int * p_bytesend,CIPWrite cipwt, BOOL readslave)
{
	int pos,len;
	int i;
	int srvdatalen=0;
	CIPWrite cipwt_tmp;
	cipwt_tmp = cipwt;
	if(readslave == TRUE)
	{
		cipwt.path.bRoutePathSizeWord=(ROUTE_SIZE_EXT-2)/2;  //路径地址16bit数
		cipwt.path.BResever=0x00;		//保留
		cipwt.path.bPath[0]=0x01;		//本地
		cipwt.path.bPath[1]=CNETSLOT;	//地址(ControlNet槽号)
		cipwt.path.bPath[2]=0x02;		//网络,非本地
		cipwt.path.bPath[3]=CNETIDM;	//主ControlNet ID
		cipwt.path.bPath[4]=0x01;		//主ControlNet ID
		cipwt.path.bPath[5]=CPUSLOT;		//主ControlNet ID
	}
	else
	{
		cipwt.path.bRoutePathSizeWord=(ROUTE_SIZE-2)/2;  //路径地址16bit数
		cipwt.path.BResever=0x00;		//保留
		cipwt.path.bPath[0]=0x01;		//本地
		cipwt.path.bPath[1]=CPUSLOT;	//地址(CPU槽号)
	}
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
	cipwt.sendhead.sintMessageRequestSize =	sizeof(cipwt.sendhead)-4 + 
		sizeof(cipwt.tagnum) +  cipwt.tagnum.usintNumberOfServices * sizeof(cipwt.offset[i]) +
		srvdatalen;
	//CIP的长度,从ciphead第一字节到path之后的总长
	cipwt.cpfhead.sintDataItemLength =   sizeof(cipwt.ciphead) + sizeof(cipwt.sendhead) + 	
		sizeof(cipwt.tagnum) +  cipwt.tagnum.usintNumberOfServices * sizeof(cipwt.offset[i]) + srvdatalen;
	if(readslave == TRUE)
	{
		cipwt.cpfhead.sintDataItemLength += ROUTE_SIZE_EXT;
	}
	else
	{
		cipwt.cpfhead.sintDataItemLength += ROUTE_SIZE;
	}
	cipwt.eciphead.sintLength =  cipwt.cpfhead.sintDataItemLength + sizeof(cipwt.cpfhead);
	DwordToByteArray(dwSenderContext++,0x2222,cipwt.eciphead.bSenderContext);

#ifdef _BIG_ENDIAN_
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
	len= sizeof(cipwt.ciphead) ;
	memcpy(buffer+pos,&(cipwt.ciphead),len);
	pos+=len;
	len= sizeof(cipwt.sendhead) ;
	memcpy(buffer+pos,&(cipwt.sendhead),len);
	pos+=len;
	len= sizeof(cipwt.tagnum) ;
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
	if(readslave == TRUE)
	{
		len= ROUTE_SIZE_EXT ;
	}
	else
	{
		len= ROUTE_SIZE ;
	}
	memcpy(buffer+pos,&(cipwt.path),len);
	pos+=len;
	*p_bytesend = pos;
	return TRUE;
}


void ByteArrayToDword(char src[8],unsigned int *dest1,unsigned int *dest2)
{
    *dest1=*(int *)&src[0];
	*dest2=*(int *)&src[4];
}

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

