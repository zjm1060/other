#include    "../../../telalm/v4.0a/inc/tele.h"
#define _LeadByte 0x80  
#define _isleadbyte( _c)  ( (unsigned char)(_c) & _LeadByte) 

 
HANDLE   _SMS_SENDRECEIVE_COMS[MAX_sms_dev_num];
int aotu_alm_send_locked=0,manu_alm_send_locked=0,manu_send_locked=0,timing_send_locked=0;
int strINstr(const unsigned char *src, int len  )
{   
	int i,pos ; 
	for (i =0 ; i <len ; i++)
	{
		if ((int )src[i] == 10) pos = i ;
	}
	return pos ; 
}

int _lstrlen(  LPCTSTR lpString  )
{
    int n =0;	
    while (*lpString)
	{   
		if( _isleadbyte(*lpString))
			lpString +=2 ;
        else
            lpString ++ ;
		n++ ;
	}
	return n ;
}

LRESULT WINAPI SMS_SENDRECEIVE(LONG lparam)
{	
	aotu_alm_send_locked=0;
	manu_alm_send_locked=0;
	manu_send_locked=0;
	timing_send_locked=0;
	while (1)
	{
		int i=0;
		threadstat_SMS_SENDRECEIVE++;
		if(readedxml==1)
		{
			if(oncall_sms_autoalmsendbuf.send_Ptr!=oncall_sms_autoalmsendbuf.add_Ptr && aotu_alm_send_locked==0)
			{	
				for(i=0;i<MAX_sms_dev_num;i++)
				{
					threadstat_SMS_SENDRECEIVE++;
					if(_SMS_SENDRECEIVE_COM_STATS[i]==0 && _ONCALLCOM_PORTS[i][0]!=0)
					{
						if(SMS_SENDRECEIVE_COM_PortInit(i)!=SUCCESS)
						{
							char temp_strs[400];
							sprintf(temp_strs,"open %s error",_ONCALLCOM_PORTS[i]);	
							insert_sms_logs(i,temp_strs);

							//printf("\nOnCall:%s",temp_strs);
						}
						Sleep(2000);
					}
					else
					{ 
						smsusexmlbufover=0;//不能读取配置文件
						if(_oncallL_sms_send_tempbufs[i].use_over==1  && _ONCALLCOM_PORTS[i][0]!=0)
						{
							_oncallL_sms_send_tempbufs[i].use_over=0;
							aotu_alm_send_locked=1;
							if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sendONCALL_SMS_AUTOALMSENDBUF,(LPVOID)i,0,&Oncall_sms_reeive_HANDLE_IDThreadS[i])==NULL) 					
							{
								char temp_strs[400];
								sprintf(temp_strs,"Create sendONCALL_SMS_AUTOALMSENDBUF[%d] Thread() error",i);	
								write_main_logs_now(temp_strs,1);
								 
								//printf("\nOnCall:%s",temp_strs);
							 
								exit(-1);
							}
							
							break;
						}
					}
				}
				Sleep(1500);
			}
			
			if(oncall_sms_timingsendbuf.send_Ptr!=oncall_sms_timingsendbuf.add_Ptr && timing_send_locked==0 && aotu_alm_send_locked==0)
			{
				for(i=0;i<MAX_sms_dev_num;i++)
				{
					threadstat_SMS_SENDRECEIVE++;
					if(_SMS_SENDRECEIVE_COM_STATS[i]==0 && _ONCALLCOM_PORTS[i][0]!=0)
					{
						if(SMS_SENDRECEIVE_COM_PortInit(i)!=SUCCESS)
						{
							char temp_strs[400];
							sprintf(temp_strs,"open %s error",_ONCALLCOM_PORTS[i]);	
							insert_sms_logs(i,temp_strs);
							
							//printf("\nOnCall:%s",temp_strs);
						}
						Sleep(2000);
					}
					else
					{ 
						smsusexmlbufover=0;//不能读取配置文件
						if(_oncallL_sms_send_tempbufs[i].use_over==1  && _ONCALLCOM_PORTS[i][0]!=0)
						{
							_oncallL_sms_send_tempbufs[i].use_over=0;
							timing_send_locked=1;
							if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sendONCALL_SMS_TIMINGSENDBUF,(LPVOID)i,0,&Oncall_sms_reeive_HANDLE_IDThreadS[i])==NULL) 					
							{
								char temp_strs[400];
								sprintf(temp_strs,"Create sendONCALL_SMS_TIMINGSENDBUF[%d] Thread() error",i);	
								write_main_logs_now(temp_strs,1);
								
								//printf("\nOnCall:%s",temp_strs);
								
								exit(-1);
							}
							
							break;
						}
					}
				}
				Sleep(1500);
			}
			//
			if(oncall_sms_manualmsendbuf.send_Ptr!=oncall_sms_manualmsendbuf.add_Ptr && manu_alm_send_locked==0  && aotu_alm_send_locked==0)
			{
				
				for(i=0;i<MAX_sms_dev_num;i++)
				{
					threadstat_SMS_SENDRECEIVE++;
					if(_SMS_SENDRECEIVE_COM_STATS[i]==0 && _ONCALLCOM_PORTS[i][0]!=0)
					{
						if(SMS_SENDRECEIVE_COM_PortInit(i)!=SUCCESS)
						{
							char temp_strs[400];
							sprintf(temp_strs,"open %s error",_ONCALLCOM_PORTS[i]);	
							insert_sms_logs(i,temp_strs);
							
							//printf("\nOnCall:%s",temp_strs);
						}
						Sleep(2000);
					}
					else
					{ 
						smsusexmlbufover=0;//不能读取配置文件
						if(_oncallL_sms_send_tempbufs[i].use_over==1  && _ONCALLCOM_PORTS[i][0]!=0)
						{
							_oncallL_sms_send_tempbufs[i].use_over=0;
							manu_alm_send_locked=1;
							if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sendONCALL_SMS_MANUALMSENDBUF,(LPVOID)i,0,&Oncall_sms_reeive_HANDLE_IDThreadS[i])==NULL) 					
							{
								char temp_strs[400];
								sprintf(temp_strs,"Create sendONCALL_SMS_MANUALMSENDBUF[%d] Thread() error",i);	
								write_main_logs_now(temp_strs,1);
								
								//printf("\nOnCall:%s",temp_strs);
								
								exit(-1);
							}
							
							break;
						}
					}
				}
				Sleep(1500);
			}
			//
			if(oncall_sms_manusendbuf.send_Ptr!=oncall_sms_manusendbuf.add_Ptr && manu_send_locked==0  && aotu_alm_send_locked==0 && manu_alm_send_locked==0 && timing_send_locked==0)
			{
				for(i=0;i<MAX_sms_dev_num;i++)
				{
					threadstat_SMS_SENDRECEIVE++;
					if(_SMS_SENDRECEIVE_COM_STATS[i]==0 && _ONCALLCOM_PORTS[i][0]!=0)
					{
						if(SMS_SENDRECEIVE_COM_PortInit(i)!=SUCCESS)
						{
							char temp_strs[400];
							sprintf(temp_strs,"open %s error",_ONCALLCOM_PORTS[i]);	
							insert_sms_logs(i,temp_strs);
							
							//printf("\nOnCall:%s",temp_strs);
						}
						Sleep(2000);
					}
					else
					{ 
						smsusexmlbufover=0;//不能读取配置文件
						if(_oncallL_sms_send_tempbufs[i].use_over==1  && _ONCALLCOM_PORTS[i][0]!=0)
						{
							_oncallL_sms_send_tempbufs[i].use_over=0;
							manu_send_locked=1;
							if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sendONCALL_SMS_MANUSENDBUF,(LPVOID)i,0,&Oncall_sms_reeive_HANDLE_IDThreadS[i])==NULL) 					
							{
								char temp_strs[400];
								sprintf(temp_strs,"Create sendONCALL_SMS_MANUSENDBUF[%d] Thread() error",i);	
								write_main_logs_now(temp_strs,1);
								
								//printf("\nOnCall:%s",temp_strs);
								
								exit(-1);
							}
							
							break;
						}
					}
				}
				Sleep(1500);
			}
			
			if(manu_send_locked==0  && aotu_alm_send_locked==0 && manu_alm_send_locked==0 && timing_send_locked==0)
			{
				
				for(i=0;i<MAX_sms_dev_num;i++)
				{
					threadstat_SMS_SENDRECEIVE++;
					if(_SMS_SENDRECEIVE_COM_STATS[i]==0 && _ONCALLCOM_PORTS[i][0]!=0)
					{
						if(SMS_SENDRECEIVE_COM_PortInit(i)!=SUCCESS)
						{
							char temp_strs[400];
							sprintf(temp_strs,"open %s error",_ONCALLCOM_PORTS[i]);	
							insert_sms_logs(i,temp_strs);
							
							//printf("\nOnCall:%s",temp_strs);
						}
						Sleep(2000);
					}
					else
					{ 
						smsusexmlbufover=0;//不能读取配置文件
						if(_oncallL_sms_send_tempbufs[i].use_over==1  && _ONCALLCOM_PORTS[i][0]!=0)
						{
							_oncallL_sms_send_tempbufs[i].use_over=0;
							if(_ONCALL_SMS_DEV_TYPES[i]==2)	
							{
								if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)receiveONCALL_SMS_CDMA,(LPVOID)i,0,&Oncall_sms_reeive_HANDLE_IDThreadS[i])==NULL) 					
								{
									char temp_strs[400];
									sprintf(temp_strs,"Create receiveONCALL_SMS_CDMA[%d] Thread() error",i);	
									write_main_logs_now(temp_strs,1);
									
									//printf("\nOnCall:%s",temp_strs);
									
									exit(-1);
								}
							}
							else
							{
								if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)receiveONCALL_SMS_GSM,(LPVOID)i,0,&Oncall_sms_reeive_HANDLE_IDThreadS[i])==NULL) 					
								{
									char temp_strs[400];
									sprintf(temp_strs,"Create receiveONCALL_SMS_GSM[%d] Thread() error",i);	
									write_main_logs_now(temp_strs,1);
									
									//printf("\nOnCall:%s",temp_strs);
									
									exit(-1);
								}
							}
							
						}
					}
				}
			}
		}
		else
		{
			insert_sms_logs(0,"暂停短信处理，等待配置文件读取完成......");
			printf("\n暂停短信处理，等待配置文件读取完成......\n");
		}

		smsusexmlbufover=1;
		Sleep(2000); 
	}
	return 0;
}

int AsciiToUnicode( unsigned char *str )
{	
	int				i,	n,	m,	len;
	unsigned char	strTmp[BUFSIZE/2], ch[1];
	n=0;//
	len = strlen(str); 
	strcpy(strTmp,str);
	for(i=0;i<(BUFSIZE-len-1);i++) strcat(strTmp," ");
	if( (m = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strTmp, -1, NULL,NULL ))==0 ) return(-1);
	if( MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strTmp, BUFSIZE/2, str, m )==0 ) return(-1);
	for(i=0;i<BUFSIZE;i++)
	{ 
		if( str[i+0]==0x20 && str[i+2]==0x20 && str[i+4]==0x20 && str[i+6]==0x20){ n=i; break; }
	}	
	//for(i=0;i<n+6;i++){ printf("%02x ",str[i]); } printf("\n");
	for(i=0;i<(n/2);i++)
	{  ch[0]=str[i*2+0]; str[i*2+0]=str[i*2+1]; str[i*2+1]=ch[0]; }
	//for(i=0;i<n+6;i++){ printf("%02x ",str[i]); } printf("\n");
	strcpy(strTmp,""); 
	for(i=0;i<n;i++)
	{ 
		sprintf(ch,"%02x",str[i]); 
		strcat(strTmp,ch); 
	}
	strcpy(str,strTmp);
	//printf("%s (len=%d)\n",str,strlen(str));
	return( n );
}

//手机号码和时间部分的编码转换
int CreateMobileNum(  char *str )
{	int				i,	len;
unsigned char	ch;
len = strlen(str);
//printf("old: %s (len=%d)\n",str,len);
for(i=0;i<( (int)(len/2)+1);i++)
{ ch=str[i*2+0]; str[i*2+0]=str[i*2+1]; str[i*2+1]=ch;}
if(len%2==0)//cxs
{
	str[len] = '\0';
}
else
{
	str[len-1] = 'F';
	str[len+1] = '\0';
}

//	printf("new: %s (len=%d)\n",str,strlen(str));
return( strlen(str) );
}

// inter cross between double char,手机号码和时间部分的解码转换
void DeCodeMobileNuber( unsigned char *str ,int len ) 
{
    char tmp;
	int i ;
	for (i=0 ; i< len/2; i++)
	{   
		tmp  =  *(str+i*2);
		*(str+i*2) = *(str+2*i+1);
        *(str+2*i+1) = tmp;
	}
}

int SMS_SENDRECEIVE_COM_PortInit(int idx)	   
{ 
	DCB            dcb;
	COMMTIMEOUTS   timeout;
	BOOL           fSuccess;
	CloseHandle(_SMS_SENDRECEIVE_COMS[idx]);
	_SMS_SENDRECEIVE_COMS[idx]=NULL;
	_SMS_SENDRECEIVE_COMS[idx]=CreateFile
		(  _ONCALLCOM_PORTS[idx],GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,    /* comm device must be opened with exclusive access */
		NULL, /* no security attrs */
		OPEN_EXISTING, /* comm device must use OPEN_EXISTING */
		0,    /* not overlapped I/O */
		NULL  /* hTemplate must be NULL  for comm device */
		);
	if(_SMS_SENDRECEIVE_COMS[idx]==INVALID_HANDLE_VALUE)
	{ 
		char temp_strs[400];
		sprintf(temp_strs,"%s can't open!The reason is :%d",_ONCALLCOM_PORTS[idx],GetLastError());	
		insert_sms_logs(idx,temp_strs);
		
		//printf("\nOnCall:%s",temp_strs);
 
		CloseHandle(_SMS_SENDRECEIVE_COMS[idx]);
		_SMS_SENDRECEIVE_COMS[idx]=NULL;
		return( FAILURE ); 
	}
	
	fSuccess=GetCommState(_SMS_SENDRECEIVE_COMS[idx],&dcb);
	if( !fSuccess )
	{ 
		char temp_strs[400];
		sprintf(temp_strs,"can't get %s state",_ONCALLCOM_PORTS[idx]);	
		insert_sms_logs(idx,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
 
		CloseHandle(_SMS_SENDRECEIVE_COMS[idx]);
		return( FAILURE ); 
	}
	
	dcb.BaudRate=_ONCALLBAUD_RATES[idx];
	dcb.ByteSize=8;
	dcb.Parity  =_ONCALLPARITY_TYPES[idx];
	dcb.StopBits=ONESTOPBIT;
	
	
	fSuccess=SetCommState(_SMS_SENDRECEIVE_COMS[idx], &dcb);
	if( !fSuccess )
	{
		char temp_strs[400];
		sprintf(temp_strs,"can't get %s state",_ONCALLCOM_PORTS[idx]);	
		insert_sms_logs(idx,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
	 
		CloseHandle(_SMS_SENDRECEIVE_COMS[idx]);
		return( FAILURE ); 
	}
	
	//SetupComm(hCom,5120,5120);
	
	fSuccess=GetCommTimeouts(_SMS_SENDRECEIVE_COMS[idx],&timeout);
	if(!fSuccess)
	{
	 	insert_sms_logs(idx,"GetCommTimeouts() error");
	 
		printf("GetCommTimeouts() error !\n");
		CloseHandle(_SMS_SENDRECEIVE_COMS[idx]);
		return( FAILURE );
	}
	timeout.ReadIntervalTimeout       = 200;//2000;      /* ms, max time between two bytes received */
	timeout.ReadTotalTimeoutMultiplier= TIMEOUT_SECONDS;         
	timeout.ReadTotalTimeoutConstant  = 1000;//1000*60*1; /* ms */
	
	fSuccess=SetCommTimeouts(_SMS_SENDRECEIVE_COMS[idx],&timeout);
	if(!fSuccess)
	{

		insert_sms_logs(idx,"SetCommTimeouts() error");
		printf("SetCommTimeouts() error !\n");
		CloseHandle(_SMS_SENDRECEIVE_COMS[idx]);
		return( FAILURE );
	}
	if(_ONCALL_SMS_DEV_TYPES[idx]==2)
	{
		char temp_strs[400];
		sprintf(temp_strs,">>>> CDMA :% s INIT OK.",_ONCALLCOM_PORTS[idx]);	
		insert_sms_logs(idx,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
	 
	}
	else
	{
		char temp_strs[400];
		sprintf(temp_strs,">>>> GSM :% s INIT OK.",_ONCALLCOM_PORTS[idx]);	
		insert_sms_logs(idx,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
	}
	PurgeComm(_SMS_SENDRECEIVE_COMS[idx],PURGE_RXCLEAR);
	
	_SMS_SENDRECEIVE_COM_STATS[idx]=1;
	return( SUCCESS );
}


void ClearRecvFault(int idx)
{
	DWORD	dwErrors;
	COMSTAT	cstat;
	if(!ClearCommError(_SMS_SENDRECEIVE_COMS[idx],&dwErrors,&cstat))
	{
		char temp_strs[400];
		sprintf(temp_strs,"ReadFile:%s ClearComm ERRNO= %d",_ONCALLCOM_PORTS[idx],GetLastError());	
		insert_sms_logs(idx,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
	 
	}
	if((dwErrors !=0)||(cstat.cbInQue !=0))
	{
		if(PurgeComm(_SMS_SENDRECEIVE_COMS[idx],PURGE_RXCLEAR)==FALSE)
		{
			char temp_strs[400];
			sprintf(temp_strs,"ReadFile:%s PurgeComm ERRNO= %d",_ONCALLCOM_PORTS[idx],GetLastError());	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);
		}
	}
}


void ClearSendCmdFault(int idx)
{
	DWORD	dwErrors;
	COMSTAT	cstat;
	if(ClearCommError(_SMS_SENDRECEIVE_COMS[idx],&dwErrors,&cstat)==FALSE)
	{
		char temp_strs[400];
		sprintf(temp_strs,"ClearCommError:%s ClearComm ERRNO= %d",_ONCALLCOM_PORTS[idx],GetLastError());	
		insert_sms_logs(idx,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
	 
	}
	if((dwErrors !=0)||(cstat.cbOutQue !=0))
	{
		if(PurgeComm(_SMS_SENDRECEIVE_COMS[idx],PURGE_TXCLEAR)==FALSE)
		{
			char temp_strs[400];
			sprintf(temp_strs,"ClearSendCmdFault:%s PurgeComm ERRNO= %d",_ONCALLCOM_PORTS[idx],GetLastError());	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);
		}
	}
}

// UCS2编码// pSrc: 源字符串指针// pDst: 目标编码串指针// nSrcLength: 源字符串长度// 返回:目标编码串长度
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int        nDstLength; // UNICODE宽字符数目
    WCHAR      wchar[128]; // UNICODE串缓冲区
    int i ; 
    //字符串-->UNICODE串
    nDstLength = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, pSrc, nSrcLength, wchar, 128);
	
    // 高低字节对调，输出
    for (  i = 0; i < nDstLength; i++)
    {
        // 先输出高位字节
        *pDst++ = wchar[i] >> 8;
		
        // 后输出低位字节
        *pDst++ = wchar[i] & 0xff;
    }
	
    // 返回目标编码串长度
    return nDstLength * 2;
}
// UCS2解码// pSrc: 源编码串指针// pDst: 目标字符串指针// nSrcLength: 源编码串长度// 返回: 目标字符串长度
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;        // UNICODE宽字符数目
    WCHAR wchar[512];      // UNICODE串缓冲区
    int i;
	
    // 高低字节对调，拼成UNICODE
    for (i = 0; i <nSrcLength/2; i++)
    {
        // 先高位字节
        wchar[i] = *pSrc++  << 8;
		
        // 后低位字节
        wchar[i] |= *pSrc++;	
    }
    
    //UNICODE串-->字符串
    nDstLength = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wchar, nSrcLength/2, pDst, 1024, NULL, NULL);
	
    // 返回目标字符串长度
    return nDstLength;
}


/* 用以上编码和解码模块，还不能将短消息字符串编码为PDU串需要的格式，也不能直接将PDU串中 的用户信息解码为短消息字符串，因为还差一个在可打印字符串和字节数据之间相互转换的环节。可以循环调用sscanf和sprintf函数实现这种变换。下面提供不用这些函数的算法，它们也适用于单片机、DSP编程环境。*/
// 可打印字符串转换为字节数据// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}// pSrc: 源字符串指针// pDst: 目标数据指针// nSrcLength: 源字符串长度// 返回: 目标数据长度
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength ,int DEV2)//chenxs add dev2
{    
	int i ; 
    for ( i = 0; i < nSrcLength; i++)
    {
        // 输出高4位
        if (*pSrc >= '0' && *pSrc <= '9')
        {
            *pDst = (*pSrc - '0') << 4;
        }
        else
        {
            *pDst = (*pSrc - 'A' + 10) << 4;
        }
		
        pSrc++;
		
        // 输出低4位
        if (*pSrc >= '0' && *pSrc <= '9')
        {
            *pDst |= *pSrc - '0';
        }
        else
        {
            *pDst |= *pSrc - 'A' + 10;
        }
        pSrc++;
        pDst++;
    }
	if(DEV2==1) nSrcLength =nSrcLength / 2;//chenxs // 返回目标数据长度
    return nSrcLength;
}

// 字节数据转换为可打印字符串// 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"
// pSrc: 源数据指针// pDst: 目标字符串指针// nSrcLength: 源数据长度// 返回: 目标字符串长度
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf的字符查找表
    int i;
    for (  i = 0; i < nSrcLength; i++)
    {
        // 输出高4位
        *pDst++ = tab[*pSrc >> 4];// 输出低4位
        *pDst++ = tab[*pSrc & 0x0f];
        pSrc++;
    }
    // 输出字符串加个结束符
    *pDst = '\0'; // 返回目标字符串长度
    return nSrcLength * 2;
}
// 7-bit编码// pSrc: 源字符串指针// pDst: 目标编码串指针
// nSrcLength: 源字符串长度// 返回: 目标编码串长度
int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int nSrc;        // 源字符串的计数值
    int nDst;        // 目标编码串的计数值
    int nChar;       // 当前正在处理的组内字符字节的序号，范围是0-7
    unsigned char nLeft;    // 上一字节残余的数据
	
    // 计数值初始化
    nSrc = 0;nDst = 0;
    // 将源串每8个字节分为一组，压缩成7个字节// 循环该处理过程，直至源串被处理完// 如果分组不到8字节，也能正确处理
    while (nSrc < nSrcLength)
    {
        // 取源字符串的计数值的最低3位
        nChar = nSrc & 7;// 处理源串的每个字节
        if (nChar == 0)
        {
            // 组内第一个字节，只是保存起来，待处理下一个字节时使用
            nLeft = *pSrc;
        }
        else
        {
            // 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
            *pDst = (*pSrc << (8 - nChar)) | nLeft;// 将该字节剩下的左边部分，作为残余数据保存起来
            nLeft = *pSrc >> nChar;// 修改目标串的指针和计数值
            pDst++;nDst++;
        }
        // 修改源串的指针和计数值
        pSrc++;nSrc++;
    }
    // 返回目标串长度
    return nDst;
} 
// 7-bit解// pSrc: 源编码串指针// pDst: 目标字符串指针// nSrcLength: 源编码串长度// 返回: 目标字符串长度
int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    int nSrc;        // 源字符串的计数值
    int nDst;        // 目标解码串的计数值
    int nByte;       // 当前正在处理的组内字节的序号，范围是0-6
    unsigned char nLeft;    // 上一字节残余的数据
    // 计数值初始化
    nSrc = 0;nDst = 0;
    // 组内字节序号和残余数据初始化
    nByte = 0;nLeft = 0;
    // 将源数据每7个字节分为一组，解压缩成8个字节// 循环该处理过程，直至源数据被处理完// 如果分组不到7字节，也能正确处理
    while (nSrc < nSrcLength)
    {
        // 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
        *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;// 将该字节剩下的左边部分，作为残余数据保存起来
        nLeft = *pSrc >> (7 - nByte);// 修改目标串的指针和计数值
        pDst++; nDst++;// 修改字节计数值
        nByte++;// 到了一组的最后一个字节
        if (nByte == 7)
        {
            // 额外得到一个目标解码字节
            *pDst = nLeft;
            // 修改目标串的指针和计数值
            pDst++;nDst++;// 组内字节序号和残余数据初始化
            nByte = 0;nLeft = 0;
        }// 修改源串的指针和计数值
        pSrc++;nSrc++;
    }
    *pDst = 0;// 返回目标串长度
    return nDst;
}
void Getarray(array,common_aa,point)
char array[20];
DMS_COMMON  common_aa;
POINTER		point;
{   	
	short	    sta_ioattr,status;
	float       fvalue;
	int         almsel,data;
	char        tmp[50];
    FILE        *fp;
	
	if( (fp=fopen(MSG_DEFFILE,"r"))==NULL )
	{
		printf("\nONCALL: can't open %s\n",MSG_DEFFILE); Sleep(3*1000); 
	}
	if( (point.data_type ==SOE_TYPE)|(point.data_type ==POL_TYPE)|(point.data_type ==OUTS_TYPE)|(point.data_type ==SYSMSG_TYPE))
	{
		sta_ioattr = common_aa.fixed.ioattr.procType ;
		status = common_aa.var.ind.status.state ;
		almsel = common_aa.fixed.msg_sel.alm ;
		switch (sta_ioattr)			                /*	判ioattr0_3bit(15)	 */	
		{
		case 	DEV_TYPE_RELAY:					/*	继电器	    */	
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"RELAY") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case 	DEV_TYPE_BREAKER:				/*	断路器	    */
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"BREAKER") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;				
			
		case 	DEV_TYPE_DEVICE:				/*	设备    	*/
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"DEVICE") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case 	DEV_TYPE_SWITCH:				/*	隔离开关	*/
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"SWITCH") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case 	DEV_TYPE_GENSTA:				/*	机组状态	*/
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"GENSTA") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case    DEV_TYPE_DEVMSG:					    /*	设备信息	*/
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"DEVMSG") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;	
			
		case    DEV_TYPE_STREAM:					    /*	 水	        */	
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STREAM") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case 	DEV_TYPE_NETMSG:					/*	网络信息	*/
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"NETMSG") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case 	DEV_TYPE_HOSTMSG:			        /*	主控站信息	*/
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"HOSTMSG") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case    DEV_TYPE_ONOFFMSG:			        /* 在线/离线信息*/
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"ONOFFMSG") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case    DEV_TYPE_LOGMSG:					    /*	LOG信息	    */
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"LOGMSG") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
			
		case    DEV_TYPE_AGCCMD:					    /*	AGC命令	    */
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"AGCCMD") )
				break;
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"=") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == almsel )
					break;
				else 
					continue;
			}
			}
			for(;;)
			{  fscanf(fp,"%s",tmp);
			if( strstr(tmp,"STATE") )				   
			{
				fscanf(fp,"%s",tmp);
				if( atoi(tmp) == status )
					break;
				else 
					continue;
			}
			}
			fscanf(fp,"%s",array);
			break;
		default	:	break;
			
	}//end switch (sta_ioattr);
	fclose(fp);	
	}//end if( (db.data_type ==4)|(db.data_type ==5)|(db.data_type ==6));
	else if( (point.data_type ==ANA_TYPE)|(point.data_type ==OUTA_TYPE)|(point.data_type ==PARAM_TYPE) )
    {
		fvalue = common_aa.var.anlg.fValue;
		sprintf(array,"%.3f",fvalue);
	}
	else if (point.data_type ==IMP_TYPE)
	{
		data = (int)common_aa.var.imp.entry.d_accum;
		sprintf(array,"%d",data);
	}
	else if (point.data_type ==CHARA_TYPE)
	{
		fvalue = common_aa.var.chara.chara_data.avg_value;
		sprintf(array,"%.2f",fvalue);
	}
	else if (point.data_type ==CALC_TYPE)
	{
		fvalue = common_aa.var.anlg.fValue;
		sprintf(array,"%.2f",fvalue);
	}
	else strcpy (array, "数据库中无此点");
}
//void CDMA_shortmsg()

int  CDMA_AsciiToUnicode(unsigned char *str)    //此函数返回字符串str变为unicode码后的字节数。即n的值。
{
	int				i,	n,	m,	len;
	unsigned char	strTmp[BUFSIZE/2];
	n=0;
	len = strlen(str); 
	strcpy(strTmp,str);
	for(i=0;i<(BUFSIZE-len-1);i++) 
		strcat(strTmp," ");
	if( (m = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strTmp, -1, NULL,NULL ))==0 ) 
		return(-1);
	if( MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strTmp, BUFSIZE/2, str, m )==0 )
		return(-1);
	for(i=0;i<BUFSIZE;i++)
	{ 
		if( str[i+0]==0x20 && str[i+2]==0x20 && str[i+4]==0x20 && str[i+6]==0x20)
		{
			n=i; 
			break;
		}
	}	
	return n;
}

BOOL GetIntValueByIndexStr(char *str, char *index, int *value)
{
	char *ptr;
	ptr= strstr(str, index);
	if(ptr)
	{
		ptr += strlen(index)+2;
		if(sscanf(ptr, "%d", value)==1)
			return TRUE;
	}
	return FALSE;
}

BOOL GetStrValueFromCellStr(char *str, char *strValue)
{
	char *ptr, *end_ptr, buff[256];
	char *index= "<Data ss:Type=\"String\">";
	char *index1= "<Data ss:Type=\"Number\">";
	char *index10= "<ss:Data ss:Type=\"String\""; //chenxs
	char *index11= "<ss:Data ss:Type=\"Number\""; //chenxs
	char *index2= "ss:Index=";			//add by yujji 2010.06.29
	if(!strstr(str, "<Cell"))
		return FALSE;
	strcpy(buff, str);
	ptr= strstr(buff, index);
	if(!ptr)
		ptr= strstr(buff, index1);
	if(!ptr)
		ptr= strstr(buff, index10);
	if(!ptr)
		ptr= strstr(buff, index11);
	if(ptr)
	{
		ptr += strlen(index);
		end_ptr= strstr(ptr, "</Data>");
		if(end_ptr)
			*end_ptr= 0;
		if(sscanf(ptr, "%s", strValue)==1)
			return TRUE;
	}
	ptr= strstr(buff, index2);		//add by yujji 2010.06.29
	if(ptr)         				//add by yujji 2010.06.29
		return TRUE;				//add by yujji 2010.06.29
	return FALSE;
	
}
BOOL XmlCellcheckadgetvalue(char *str, char *strValue,char * ckstartchar,char * ckendchar,char * ckendchar2,int cgtype,int * ended) 
{
	char *ptr, *end_ptr;
		*ended=0;
	if(!strstr(str, ckstartchar))
		return FALSE;
	ptr= strstr(str, ckstartchar);

	if(cgtype==0)//check
	{
		if(ptr)return TRUE;
		else return FALSE;
	}
	else//get
	{
		if(ptr)
		{
			ptr += strlen(ckstartchar);
			end_ptr= strstr(ptr, ckendchar);
			if(end_ptr)
			{
				*end_ptr= 0;
				*ended=1;
			}
			else
			{
				end_ptr= strstr(ptr, ckendchar2);
				if(end_ptr)
				{
					*end_ptr= 0;
					*ended=1;
				}
			}
			strcpy(strValue,ptr);
			//if(sscanf(ptr, "%s", strValue)==1)
				return TRUE;
		}
		else return FALSE;		 	 
	}
}
int UnicodeToAscii(const unsigned char* pSrc, char* pDst, int nSrcLength) // unicode-ascii
{
    int nDstLength;        // UNICODE宽字符数目//UNICODE串-->字符串//nDstLength = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pSrc, nSrcLength/2, pDst, 160, NULL, NULL);
    nDstLength = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (const unsigned short *)pSrc, nSrcLength/2, pDst, 1024, NULL, NULL);// 返回目标字符串长度
    return nDstLength;
}

int UTF8ToUnicode( const unsigned char* str ,char*  destr )   // utf8-unicode
{
	int  unicodeLen , len ;
	WCHAR pUnicode[BUFSIZE];
	unicodeLen = MultiByteToWideChar( CP_UTF8, 0, str, -1, NULL , 0);      
	memset(pUnicode,0,1024);  
	MultiByteToWideChar( CP_UTF8, 0, str ,-1, (LPWSTR)pUnicode, unicodeLen ); 
	len = ( unicodeLen-1 )*2;
	memcpy (destr , pUnicode, len);  
	return len;
}
void UTF8ToAscii(char *str)
{
	unsigned char   temp[BUFSIZE],temp1[BUFSIZE];
	int	len;
	memset(temp, 0, sizeof(temp));
	memset(temp1, 0, sizeof(temp1));
	len = UTF8ToUnicode(str,temp);
	UnicodeToAscii(temp, temp1, len);
	strcpy(str, temp1);
}
int set_Send_No(int n,char * str,int ptr)
{
	char a,b,c,k;
	a=n/100;
	b=(n%100)/10;
	c=n%10;
	
	k=0;
	if(a>0)
	{
		str[k+ptr]=a+48;
		k++;
	}
	if(b>0 || a>0)
	{
		str[k+ptr]=b+48;
		k++;
	}
	if(c>0 || b>0 || a>0)
	{
		str[k+ptr]=c+48;
		k++;
	}
	return k;
}
//int SendMessagetoCDMA(int len, int len1, char NumStr[20], unsigned char strText[BUFSIZE], unsigned char send_msg[BUFSIZE],unsigned char rTONNPI0,unsigned char rTONNPI1)
int SendMessagetoCDMA(int idx)
{
	int		x,numlen,n,m,i,j,ts;
	unsigned char	buf[BUFSIZE];
	unsigned char	strDesTel[100];
	
	if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
			_oncallL_sms_send_tempbufs[idx].use_over=0;
 
	for(x=0;x<20;x++)
	{
		if(_oncallL_sms_send_tempbufs[idx].NumStr[x]==0)
			break;
		numlen=x;
	}
	numlen++;
	if(numlen>19)
	{
		if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
			_oncallL_sms_send_tempbufs[idx].use_over=1;
		return -1;
	}
	strcpy(_oncallL_sms_send_tempbufs[idx].strText,_oncallL_sms_send_tempbufs[idx].send_msg);

	if((_oncallL_sms_send_tempbufs[idx].len=CDMA_AsciiToUnicode(_oncallL_sms_send_tempbufs[idx].strText))!=-1)
	{
		n=_oncallL_sms_send_tempbufs[idx].len/136;
		m=_oncallL_sms_send_tempbufs[idx].len%136;
		if(m!=0)n++;
		
		ts=0;
		for(i=0;i<n;i++)
		{	
			strcpy(buf,"AT+CMGF=1");
			buf[9]=13; 
			buf[10]=10; 
			buf[11]=0;
			if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],buf,11,&_oncallL_sms_send_tempbufs[idx].len1,NULL) )
			{
				char temp_strs[400];
				sprintf(temp_strs,"SendMessagetoCDMA() WriteFile 'AT+CMGF=1' error! index=%d",idx);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);
				if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
				ClearSendCmdFault(idx); 
				Sleep(100); 
			}
			memset(buf,0,BUFSIZE);
			Sleep(100);
			ReadFile(_SMS_SENDRECEIVE_COMS[idx],buf,BUFSIZE,&_oncallL_sms_send_tempbufs[idx].len1,NULL);
			
			strcpy(strDesTel,_oncallL_sms_send_tempbufs[idx].NumStr); 
			
			strcpy(buf,"AT+WSCL=6,4");
			buf[11]=13; 
			buf[12]=10; 
			buf[13]='\0';
			if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],buf,13,&_oncallL_sms_send_tempbufs[idx].len1,NULL) )
			{
				char temp_strs[400];
				sprintf(temp_strs,"SendMessagetoCDMA() WriteFile AT+CMGS=<length> error! index=%d",idx);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);

				if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			 
				ClearSendCmdFault(idx); 
				goto REC_ERR;
			} 
			Sleep(100);
			
			memset(buf,0,100);

			strcpy(buf,"AT+CMGS=");
			buf[8]=34;
			for(j=9;j<9+numlen;j++)
				buf[j]=_oncallL_sms_send_tempbufs[idx].NumStr[j-9];
			buf[9+numlen]=34;
			buf[10+numlen]=39;

			for(x=0,j=14+numlen;x<136;)
			{
				if(i>=n-1)
				{
					if(x>m-1)
						break;
				}
				j++;
				x++;
			}
			_oncallL_sms_send_tempbufs[idx].len=x;
			x=set_Send_No(_oncallL_sms_send_tempbufs[idx].len,buf,11+numlen);

			buf[11+numlen+x]=13;
			buf[12+numlen+x]=0;
			x=12+numlen+x;

			if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],buf,x,&_oncallL_sms_send_tempbufs[idx].len1,NULL))
			{	
				char temp_strs[400];
				sprintf(temp_strs,"CDMA_shortmsg() WriteFile AT+CMGS=<length> error! index=%d",idx);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);

				if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			 
				ClearSendCmdFault(idx); 
				goto REC_ERR;
			} 	
			printf("send len=%d\n",_oncallL_sms_send_tempbufs[idx].len1);
			Sleep(200);
			buf[0]=0;
			ReadFile(_SMS_SENDRECEIVE_COMS[idx],buf,BUFSIZE,&_oncallL_sms_send_tempbufs[idx].len1,NULL); 
		
		 	for(x=1,j=0;j<136;)
			{
				if(i>=n-1)
				{
					if(j>m-1)
						break;
				}
				if(x==0)
					x=1;
				else 
					x=0;

				if(x==0)
					buf[j]=_oncallL_sms_send_tempbufs[idx].strText[i*136+j+1];
				else
					buf[j]=_oncallL_sms_send_tempbufs[idx].strText[i*136+j-1];
				j++;
			}
			buf[j]=0;
			buf[j+1]=26;
			buf[j+2]=0;
			if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],buf,j+2,&_oncallL_sms_send_tempbufs[idx].len1,NULL))//write command to the com port
			{	
				char temp_strs[400];
				sprintf(temp_strs,"CDMA_shortmsg() WriteFile AT+CMGS=<length> error! index=%d",idx);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);

				if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			 
				ClearSendCmdFault(idx); 
				goto REC_ERR;
			} 	
			Sleep(8000);
			printf("send len1=%d\n",_oncallL_sms_send_tempbufs[idx].len1);		
		
			memset(buf,0,BUFSIZE);
			ReadFile(_SMS_SENDRECEIVE_COMS[idx],buf,BUFSIZE,&_oncallL_sms_send_tempbufs[idx].len1,NULL); 
		 
			if(strstr(buf,"ERROR") || buf[0]==0)	
			{
				if(ts<4)
				{
					ts++;
					i--;
					Sleep(5000);
					continue;
				}
				else
					goto REC_ERR;
			}		
			if(strstr(buf,"OK") && strstr(buf,"CMGS:"))
			{
				char temp_strs[400];
				sprintf(temp_strs,"Send Message to %s ok ! index=%d Flag=%d",_oncallL_sms_send_tempbufs[idx].NumStr,idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);
				if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]=0;//20120327

			 	if(i>=n-1)
				{
					Sleep(5000);
					if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
						_oncallL_sms_send_tempbufs[idx].use_over=1;
					return 1;
				}
				else
				{
					ts=0;
					Sleep(3000);
					continue;
				}
			}
			else
			{
				if(ts<3)
				{
					char temp_strs[400];
					sprintf(temp_strs,"Send Message to %s ERR, try times %d  index=%d  Flag=%d!",_oncallL_sms_send_tempbufs[idx].NumStr,ts,idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
					insert_sms_logs(idx,temp_strs);
					//printf("\nOnCall:%s",temp_strs);

					if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
					ts++;
					i--;
				
					Sleep(5000);
					continue;
				}
				else
				{

					int _i=-99;
					int tms_k=200;
					int allsmsnum=0;
					for(_i=0;_i<MAX_sms_dev_num;_i++)
					{
						if(_ONCALLCOM_PORTS[_i][0]!=0)
						{
							allsmsnum++;
						}
					}
					if(allsmsnum>1 &&  _oncallL_sms_send_tempbufs[idx].use_sc!=-1)
					{
						
						while(1)
						{
							for(_i=0;_i<MAX_sms_dev_num;_i++)
							{
								if(_oncallL_sms_send_tempbufs[_i].use_over==1  && _ONCALLCOM_PORTS[_i][0]!=0  && _i!=idx)
								{
									_oncallL_sms_send_tempbufs[_i].use_over=0;
									_oncallL_sms_send_tempbufs[_i].use_sc=-1;
									_oncallL_sms_send_tempbufs[_i].len=_oncallL_sms_send_tempbufs[idx].len;
									_oncallL_sms_send_tempbufs[_i].len1=_oncallL_sms_send_tempbufs[idx].len1;
									strcpy(_oncallL_sms_send_tempbufs[_i].NumStr,_oncallL_sms_send_tempbufs[idx].NumStr);
									strcpy(_oncallL_sms_send_tempbufs[_i].strText,_oncallL_sms_send_tempbufs[idx].strText);
									strcpy(_oncallL_sms_send_tempbufs[_i].send_msg,_oncallL_sms_send_tempbufs[idx].send_msg);
									_oncallL_sms_send_tempbufs[_i].rTONNPI0=_oncallL_sms_send_tempbufs[idx].rTONNPI0;
									_oncallL_sms_send_tempbufs[_i].rTONNPI1=_oncallL_sms_send_tempbufs[idx].rTONNPI1;
									
									
									if(SendMessagetoCDMA(_i)!=-1)
									{
										_i=-2;
										
									}
									_oncallL_sms_send_tempbufs[_i].use_sc=0;
									if(_i<0)
										break;
								}
							}
							if(_i>=0)
								tms_k-=20;
							else
								tms_k-=100;
							
							if(_i<0)
								break;
							
							if(tms_k<1)
							{
								_i=-99;
								break;
							}
							Sleep(500);
						}
					}
					
				//	if(_i==-99)
						goto REC_ERR;

				}
			}		
REC_ERR:		
		 
		 	if(1==1)
			{
				//int Nhour,Nmin,NDay,NMon,Nyear,Nsecond;
				//struct tm *tp;
				//time_t timep;
				char temp_strs[400];

				//timep=time(NULL);  
				//tp=localtime(&timep);	


				sprintf(temp_strs,"Send Message to %s fail ! index=%d Flag=%d",_oncallL_sms_send_tempbufs[idx].NumStr,idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);
				
				//Nhour=tp->tm_hour;
				//Nmin=tp->tm_min;
				//NDay=tp->tm_mday;
			//	NMon=tp->tm_mon+1;
				//Nyear=tp->tm_year+1900;
			//	Nsecond=tp->tm_sec;

			//	sprintf(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"%d-%d-%d %d:%d:%d",Nyear,NMon,NDay,Nhour,Nmin,Nsecond);
			/*	strcpy(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nyear);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],NMon);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],NDay);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr]," ");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nhour);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nmin);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nsecond);
				*/
				//strcpy(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," >>>> Send Message to ");
				//strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],_oncallL_sms_send_tempbufs[idx].NumStr);
				
				//strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," fail !!!");
				//oncallL_smsquiry_writebuf.add_Ptr++;
			//	if(oncallL_smsquiry_writebuf.add_Ptr>MAX_WRITEFILEBUFNUM-1)
				//	oncallL_smsquiry_writebuf.add_Ptr=0;
			}
			Sleep(3000); 
			if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
			_oncallL_sms_send_tempbufs[idx].use_over=1;
			return -1;
		}
	}
	else
	{
	//	printf(">>>> Send Message to %s fail \nlen=CDMA_AsciiToUnicode(strText))==-1\n",_oncallL_sms_send_tempbufs[idx].NumStr);
		if(1==1)
		{
			char temp_strs[400];
			sprintf(temp_strs,"Send Message to %s fail len=CDMA_AsciiToUnicode(strText))==-1! index=%d Flag=%d",_oncallL_sms_send_tempbufs[idx].NumStr,idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);
			//int Nhour,Nmin,NDay,NMon,Nyear,Nsecond;
			//struct tm *tp;
		//	time_t timep;
		//	timep=time(NULL);  
		//	tp=localtime(&timep);	
			
		//	Nhour=tp->tm_hour;
		//	Nmin=tp->tm_min;
		//	NDay=tp->tm_mday;
		//	NMon=tp->tm_mon+1;
		//	Nyear=tp->tm_year+1900;
		//	Nsecond=tp->tm_sec;

		//	sprintf(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"%d-%d-%d %d:%d:%d",Nyear,NMon,NDay,Nhour,Nmin,Nsecond);
		/*
			strcpy(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nyear);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],NMon);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],NDay);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr]," ");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nhour);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nmin);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nsecond);
			*/
			//strcpy(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," >>>> Send Message to ");
			//strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],_oncallL_sms_send_tempbufs[idx].NumStr);
			
		//	strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," fail \nlen=CDMA_AsciiToUnicode(strText))==-1");
		//	oncallL_smsquiry_writebuf.add_Ptr++;
		//	if(oncallL_smsquiry_writebuf.add_Ptr>MAX_WRITEFILEBUFNUM-1)
		//		oncallL_smsquiry_writebuf.add_Ptr=0;
			}

		if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
			_oncallL_sms_send_tempbufs[idx].use_over=1;
		return -1;
	}
	if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
			_oncallL_sms_send_tempbufs[idx].use_over=1;
	return 1;
}

//int SendMessagetoGSM(int len, int len1, char NumStr[20], unsigned char  strText[BUFSIZE], unsigned char   send_msg[BUFSIZE],unsigned char rTONNPI0,unsigned char rTONNPI1)
 int SendMessagetoGSM(int idx)
{
	int		i,x, j,y, numlen,ts;
	unsigned char texttmp[512];
	unsigned char	buf[BUFSIZE];
	unsigned char	strSmsTel[100];
	
	unsigned char	strDesTel[100];
	unsigned char	strMsgBuf[BUFSIZE];

	if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
	_oncallL_sms_send_tempbufs[idx].use_over=0;
	for(x=0;x<20;x++)
	{
		if(_oncallL_sms_send_tempbufs[idx].NumStr[x]==0)
			break;
		numlen=x;
	}
	numlen++;
	if(numlen>19)
	{
		if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
			_oncallL_sms_send_tempbufs[idx].use_over=1;
		return -1;
	}
	if(_oncallL_sms_send_tempbufs[idx].len>0)
	{
		x = _oncallL_sms_send_tempbufs[idx].len1/276;
		y = _oncallL_sms_send_tempbufs[idx].len1%276;
		if(y!=0)
			x++;
		
		strcpy(strSmsTel,_strSmsCenterS[idx]); 
		strcpy(strDesTel,_oncallL_sms_send_tempbufs[idx].NumStr); 
		CreateMobileNum(strSmsTel);
		CreateMobileNum(strDesTel);
		
		ts=0;
		i=0;
		for(i=0;i<x;i++)
		{

			memset(texttmp,0,sizeof(texttmp));

			for(j=0;j<276;)
			{
				if(i>=x-1)
				{
					if(j>y-1)
						break;
				}
				texttmp[j]=_oncallL_sms_send_tempbufs[idx].strText[276*i+j];
				j++;
			}
			_oncallL_sms_send_tempbufs[idx].len=j/2;
 	
			memset(strMsgBuf,0,BUFSIZE);
    		sprintf(strMsgBuf,"0891%s1100%02x81%s0008A7%02x%s",strSmsTel,numlen,strDesTel,_oncallL_sms_send_tempbufs[idx].len,texttmp);  
			if(numlen%2!=0)
				sprintf(buf,"AT+CMGS=%03d",8+_oncallL_sms_send_tempbufs[idx].len+numlen/2+1);
			else
				sprintf(buf,"AT+CMGS=%03d",8+_oncallL_sms_send_tempbufs[idx].len+numlen/2);
			buf[11]=13; 
			buf[12]=10; 
			
			buf[13]=0;
			if(_ONCALL_SMS_DEV_TYPES[idx]==0)
			{
				if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],buf,13,&_oncallL_sms_send_tempbufs[idx].len,NULL) )
				{
					char temp_strs[400];
					sprintf(temp_strs,"SendMessagetoGSM() WriteFile AT+CMGS=<length> error index=%d",idx);	
					insert_sms_logs(idx,temp_strs);
					//printf("\nOnCall:%s",temp_strs);

					if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
				 	 
					ClearSendCmdFault(idx); 
					goto gsmREC_ERR;
				} 
			}
			else
			{
				if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],buf,12,&_oncallL_sms_send_tempbufs[idx].len,NULL) )
				{
					char temp_strs[400];
					sprintf(temp_strs,"SendMessagetoGSM() WriteFile AT+CMGS=<length> error index=%d",idx);	
					insert_sms_logs(idx,temp_strs);
					//printf("\nOnCall:%s",temp_strs);

					if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
					ClearSendCmdFault(idx); 
					goto gsmREC_ERR;
				} 
			}
			Sleep(100);
			ReadFile(_SMS_SENDRECEIVE_COMS[idx],buf,BUFSIZE,&_oncallL_sms_send_tempbufs[idx].len,NULL);
		
			j=strlen(strMsgBuf);
			strcpy(buf,strMsgBuf);
			buf[j]   = 26;
			buf[j+1]   = 0;
			if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],buf,j+1,&_oncallL_sms_send_tempbufs[idx].len,NULL) )
			{	
				char temp_strs[400];
				sprintf(temp_strs,"SendAUTOMANUSMS() WriteFile  > <text> error index=%d",idx);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);

				if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			 	ClearSendCmdFault(idx); 
				goto gsmREC_ERR;
			}
			memset(buf,0,BUFSIZE);
			Sleep(8000);
			ReadFile(_SMS_SENDRECEIVE_COMS[idx],buf,BUFSIZE,&_oncallL_sms_send_tempbufs[idx].len,NULL);
	
			if(strstr(buf,"ERROR") || buf[0]==0)	
			{
				if(ts<3)
				{
					char temp_strs[400];
					sprintf(temp_strs,"Send Message to %s ERR try times %d  index=%d  Flag=%d",_oncallL_sms_send_tempbufs[idx].NumStr,ts,idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
					insert_sms_logs(idx,temp_strs);
					//printf("\nOnCall:%s",temp_strs);

					ts++;
					i--;
					if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
					Sleep(4000);
					continue;
				}
				else
				{	
					int _i=-99;
					int tms_k=200;
					int allsmsnum=0;
				
					
					for(_i=0;_i<MAX_sms_dev_num;_i++)
					{
						if(_ONCALLCOM_PORTS[_i][0]!=0)
						{
							allsmsnum++;
						}
					}
					if(allsmsnum>1 &&  _oncallL_sms_send_tempbufs[idx].use_sc!=-1)
					{
						
						while(1)
						{
							for(_i=0;_i<MAX_sms_dev_num;_i++)
							{
								if(_oncallL_sms_send_tempbufs[_i].use_over==1  && _ONCALLCOM_PORTS[_i][0]!=0 && _i!=idx)
								{
									_oncallL_sms_send_tempbufs[_i].use_over=0;
									_oncallL_sms_send_tempbufs[_i].use_sc=-1;
									_oncallL_sms_send_tempbufs[_i].len=_oncallL_sms_send_tempbufs[idx].len;
									_oncallL_sms_send_tempbufs[_i].len1=_oncallL_sms_send_tempbufs[idx].len1;
									strcpy(_oncallL_sms_send_tempbufs[_i].NumStr,_oncallL_sms_send_tempbufs[idx].NumStr);
									strcpy(_oncallL_sms_send_tempbufs[_i].strText,_oncallL_sms_send_tempbufs[idx].strText);
									strcpy(_oncallL_sms_send_tempbufs[_i].send_msg,_oncallL_sms_send_tempbufs[idx].send_msg);
									_oncallL_sms_send_tempbufs[_i].rTONNPI0=_oncallL_sms_send_tempbufs[idx].rTONNPI0;
									_oncallL_sms_send_tempbufs[_i].rTONNPI1=_oncallL_sms_send_tempbufs[idx].rTONNPI1;
									if(SendMessagetoGSM(_i)!=-1)
									{
										_i=-2;
										
									}
									_oncallL_sms_send_tempbufs[_i].use_sc=0;
									
									if(_i<0)
										break;
								}
							}


							if(_i>=0)
								tms_k-=20;
							else
								tms_k-=100;
						 
							
							if(_i<0)
								break;
							
							if(tms_k<1)
							{
								_i=-99;
								break;
							}
							Sleep(500);
						}
					}

				//	if(_i==-99)
						goto gsmREC_ERR;
				}
			}
			else// if(strstr(buf,"OK"))	
			{
				char temp_strs[400];
				sprintf(temp_strs,"Send Message to %s ok! index=%d Flag=%d",_oncallL_sms_send_tempbufs[idx].NumStr,idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);

				if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]=0;//20120327
				//printf(">>>> Send Message to %s ok !!! index=%d Flag=%d\n",_oncallL_sms_send_tempbufs[idx].NumStr,idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
				if(i>=x-1)
				{
					Sleep(8000);
					if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
						_oncallL_sms_send_tempbufs[idx].use_over=1;
					return 1;
				}
				else
				{
					ts=0;
					Sleep(8000);
					continue;
				}
			}
			/*else
			{
				if(ts<4)
				{
					ts++;
					i--;
					printf(">>>> Send Message to %s ERR try times %d!!!\n",NumStr,ts);	
					Sleep(3000);
					continue;
				}
				else
					goto gsmREC_ERR;
			}*/
gsmREC_ERR:

			//printf(">>>> Send Message to %s fail !!! index=%d Flag=%d\n",_oncallL_sms_send_tempbufs[idx].NumStr,idx,_oncallL_sms_send_tempbufs[idx].use_sc);
			if(1==1)
			{
				char temp_strs[400];
				sprintf(temp_strs,"Send Message to %s fail ! index=%d Flag=%d",_oncallL_sms_send_tempbufs[idx].NumStr,idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);

				//int Nhour,Nmin,NDay,NMon,Nyear,Nsecond;
				//struct tm *tp;
				//time_t timep;
				//timep=time(NULL);  
				//tp=localtime(&timep);	
				
				//Nhour=tp->tm_hour;
				//Nmin=tp->tm_min;
				//NDay=tp->tm_mday;
				//NMon=tp->tm_mon+1;
				//Nyear=tp->tm_year+1900;
				//Nsecond=tp->tm_sec;

				//sprintf(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"%d-%d-%d %d:%d:%d",Nyear,NMon,NDay,Nhour,Nmin,Nsecond);
		/*
				strcpy(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nyear);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],NMon);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],NDay);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr]," ");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nhour);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nmin);
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
				strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],Nsecond);
				*/
				//strcpy(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," >>>> Send Message to ");
				//strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],_oncallL_sms_send_tempbufs[idx].NumStr);
				
				//strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," fail !!!");
				//oncallL_smsquiry_writebuf.add_Ptr++;
				//if(oncallL_smsquiry_writebuf.add_Ptr>MAX_WRITEFILEBUFNUM-1)
				//	oncallL_smsquiry_writebuf.add_Ptr=0;
			}


			Sleep(3000);
			if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
			_oncallL_sms_send_tempbufs[idx].use_over=1;
			return -1;
		}
	}
	Sleep(3000);
	if(_oncallL_sms_send_tempbufs[idx].use_over!=-1)
			_oncallL_sms_send_tempbufs[idx].use_over=1;
	return 1;
}
void Oncall_sms_timingsend(int idx)
{
	int i,j,k,kf,Jx,nci;
	int Nhour,Nmin,NDay,NMon,Nyear;
	time_t timep;
	struct tm *tp;
	POINTER  point0;
	DMS_COMMON  oncalldms_common0;	//test
	char  array0[256];	//test
	char    *dvlC,*dvl; 
	char templongname[100];
	int add_telnum_idx;//20120329
	char temp_strs[800];//20120326 add
	int Ix,kx,Lx;
	struct tm 	*today;
	time_t		ltime;	
	ltime=time(NULL);
	today=localtime(&ltime);
 		Ix=0,Jx=0,kx=0,Lx=0;

	dvlC=";";
	nci=0;
	
	memset(&oncall_sms_timingsendbuf,0,sizeof(oncall_sms_timingsendbuf));
	while(1)//定时短信
	{
		Sleep(3000);
		threadstat_Oncall_sms_timingsend++;
		if(readedxml!=1)//20120330
			continue;
		Sleep(3000);
		threadstat_Oncall_sms_timingsend++;
		Sleep(3000);
		threadstat_Oncall_sms_timingsend++;
		Sleep(3000);
		threadstat_Oncall_sms_timingsend++;
		Sleep(3000);
		threadstat_Oncall_sms_timingsend++;
		
		timep=time(NULL);  
		tp=localtime(&timep);	
		
		Nhour=tp->tm_hour;
		Nmin=tp->tm_min;
		NDay=tp->tm_mday;
		NMon=tp->tm_mon+1;
		Nyear=tp->tm_year+1900;
		for(i=0;i<timingsend_def.real_timingsend_def_num;i++)
		{
			for(j=0;j<48;j++)
			{
				if(timingsend_def.hour[i][j]*60+timingsend_def.min[i][j] > Nhour*60+Nmin)
					timingsend_def.sended[i][j]=0;
				if(timingsend_def.hour[i][j]*60+timingsend_def.min[i][j] < Nhour*60+Nmin)
					timingsend_def.sended[i][j]=1;
			}
		}
		for(i=0;i<timingsend_def.real_timingsend_def_num;i++)
		{
			for(j=0;j<48;j++)
			{
				if(timingsend_def.valid[i][j]==0)continue;
				if(timingsend_def.sended[i][j]==1)continue;
				if(timingsend_def.hour[i][j]*60+timingsend_def.min[i][j] != Nhour*60+Nmin)continue;
				
				kf=-1;
				memset(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],0,sizeof(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr]));


				
				for(Jx=0;Jx<20;Jx++)//定义
					memset(oncall_sms_timingsendbuf.sendlongnames[oncall_sms_timingsendbuf.add_Ptr][Jx],0,sizeof(oncall_sms_timingsendbuf.sendlongnames[oncall_sms_timingsendbuf.add_Ptr][Jx]));
				
				nci=0;
				memset(templongname,0,sizeof(templongname));
				strcpy(templongname,timingsend_def.longname[i]);
				dvl=strtok(templongname,dvlC);
				while(dvl)
				{
					strcpy(oncall_sms_timingsendbuf.sendlongnames[oncall_sms_timingsendbuf.add_Ptr][nci], dvl);
					dvl=strtok(NULL,dvlC);
					nci++;
					if(nci>18)break;
				}

				for(k=0;k<20;k++)
				{
					if(oncall_sms_timingsendbuf.sendlongnames[oncall_sms_timingsendbuf.add_Ptr][k][0]!=0)
					{
						if(k>0)	
							strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],",");
						strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],oncall_sms_timingsendbuf.sendlongnames[oncall_sms_timingsendbuf.add_Ptr][k]);
						strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],":");
					}
					if(timingsend_def.logicnames[i][k][0]==0)continue;
					if(-1==GetPtId(timingsend_def.logicnames[i][k], &point0))
					{
						char temp_strs[400];
						sprintf(temp_strs,"定时逻辑名错误 %s!",timingsend_def.logicnames[i][k]);	
						insert_timingsend_logs(temp_strs);
						//printf("\nOnCall:%s",temp_strs);

						//printf("\n>>>ONCALL 定时短信 查询逻辑名定义错误" );
						strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],"定时逻辑名错误!");
						strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],"(");
						strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],timingsend_def.logicnames[i][k]);
						strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],")");


						kf=1;
						//break;
					}
					else
					{  
						if(-1==ReadEntryById( &point0, &oncalldms_common0))
						{
							char temp_strs[400];
							sprintf(temp_strs,"定时逻辑名错误 %s!",timingsend_def.logicnames[i][k]);	
							insert_timingsend_logs(temp_strs);
							//printf("\nOnCall:%s",temp_strs);


							//printf("\n>>>ONCALL 定时短信 No this point in database!" ); 
							strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],"定时逻辑名错误!");
							strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],"(");
							strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],timingsend_def.logicnames[i][k]);
							strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],")");

						 

							kf=2;
							//break;
						} 
						else
						{
							Getarray(array0,oncalldms_common0,point0);	
							//strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr],"  ");
							strcat(oncall_sms_timingsendbuf.message[oncall_sms_timingsendbuf.add_Ptr], array0);
							//strcpy(oncall_sms_timingsendbuf.logicnames[oncall_sms_timingsendbuf.add_Ptr][k], timingsend_def.logicnames[i][k]);
							kf=3;
						}
					}
				}

				if(kf<0 && timingsend_def.logicnames[i][0][0]!=0)
					continue;
				
				//strcpy(oncall_sms_timingsendbuf.logicname[oncall_sms_timingsendbuf.add_Ptr], timingsend_def.logicname[i]);
				//strcpy(oncall_sms_timingsendbuf.longname[oncall_sms_timingsendbuf.add_Ptr],  timingsend_def.longname[i]);
				
				memset(oncall_sms_timingsendbuf.time_str[oncall_sms_timingsendbuf.add_Ptr],'\0',sizeof(oncall_sms_timingsendbuf.time_str));
				sprintf(oncall_sms_timingsendbuf.time_str[oncall_sms_timingsendbuf.add_Ptr],"%d-%d-%d %d:%d",Nyear,NMon,NDay,Nhour,Nmin);
			 
				/*for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)//定义
				{
					memset(oncall_sms_timingsendbuf.telnum[oncall_sms_timingsendbuf.add_Ptr][Jx],'\0',20);
					
					if(timingsend_def.namecode[i][Jx][0]=='\0') continue;
					for(Jy=0;Jy<telphone_def.real_telphone_def_num;Jy++)//电话
					{
						if(strcmp(telphone_def.namecode[Jy],timingsend_def.namecode[i][Jx])==0)//查询namecode 对应号码
						{
							strcpy(oncall_sms_timingsendbuf.telnum[oncall_sms_timingsendbuf.add_Ptr][Jx],telphone_def.telphone[Jy]);
						}
					}
				}*/
				add_telnum_idx=0;
				for(Jx=0;Jx<TELPHONE_NUM;Jx++)
					oncall_sms_timingsendbuf.telnum[oncall_sms_timingsendbuf.add_Ptr][Jx][0]='\0';
				for(Ix=0;Ix<TELPHONE_NUM;Ix++)
				{
					if(add_telnum_idx>=TELPHONE_NUM)
						break;
					memset(oncall_sms_timingsendbuf.telnum[oncall_sms_timingsendbuf.add_Ptr][Ix],'\0',20);
					if(timingsend_def.namecode[i][Ix][0]!='\0')
					{
						for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
						{
							if(strcmp(telphone_def.namecode[Jx],timingsend_def.namecode[i][Ix])==0)
							{
								strcpy(oncall_sms_timingsendbuf.telnum[oncall_sms_timingsendbuf.add_Ptr][add_telnum_idx],telphone_def.telphone[Jx]);
								
								//20120329 add srart
								sprintf(temp_strs,"will timingsend telnum:%s",telphone_def.telphone[Jx]);	
								insert_timingsend_logs(temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//20120329 add end
								
								add_telnum_idx++;
								if(add_telnum_idx>=TELPHONE_NUM)//可重复定义号码
									break;
							}
						}
					}
				}
				
				for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
				{
					if(add_telnum_idx>=TELPHONE_NUM)
						break;
					if(timingsend_def.groupnamecode[i][Ix][0]==0)
						continue;
					for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
					{
						if(add_telnum_idx>=TELPHONE_NUM)
							break;
						if(telphonegroup_def.grpnamecode[kx][0]==0)
							continue;
						if(strcmp(timingsend_def.groupnamecode[i][Ix],telphonegroup_def.grpnamecode[kx])==0)
						{
							if(telphonegroup_def.months[kx][today->tm_mon]!=1)
								continue;
							if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
								continue;
							if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
								continue;
							if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
							{
								if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
								{
									for(Jx=0;Jx<TELPHONE_NUM;Jx++)
									{
										if(add_telnum_idx>=TELPHONE_NUM)
											break;
										if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
											continue;
										
										strcpy(oncall_sms_timingsendbuf.telnum[oncall_sms_timingsendbuf.add_Ptr][add_telnum_idx],telphonegroup_def.telphonenums[kx][Jx]);
										//20120329 add srart
										sprintf(temp_strs,"will timingsend telnum_G:%s",telphonegroup_def.telphonenums[kx][Jx]);	
										insert_timingsend_logs(temp_strs);
										//printf("\nOnCall:%s",temp_strs);
										//20120329 add end
										add_telnum_idx++;
									}
								}
							}
						}
					}
				}

				/*
				for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
				{
					if(timingsend_def.groupnamecode[i][Ix][0]==0)
						continue;
					for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
					{
						if(telphonegroup_def.grpnamecode[kx][0]==0)
							continue;
						if(strcmp(timingsend_def.groupnamecode[i][Ix],telphonegroup_def.grpnamecode[kx])==0)
						{
							if(telphonegroup_def.months[kx][today->tm_mon]!=1)
								continue;
							if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
								continue;
							if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
								continue;
							if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
							{
								if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
								{
									for(Jx=0;Jx<TELPHONE_NUM;Jx++)
									{
										if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
											continue;
										for(Lx=0;Lx<TELPHONE_NUM;Lx++)
										{
											if(oncall_sms_timingsendbuf.telnum[oncall_sms_timingsendbuf.add_Ptr][Lx][0]==0)
											{
												strcpy(oncall_sms_timingsendbuf.telnum[oncall_sms_timingsendbuf.add_Ptr][Lx],telphonegroup_def.telphonenums[kx][Jx]);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			 */
				timingsend_def.sended[i][j]=1;
				oncall_sms_timingsendbuf.add_Ptr++;
				if(oncall_sms_timingsendbuf.add_Ptr>ONCALL_SMS_TIMINGSENDBUF_SIZE-1)
					oncall_sms_timingsendbuf.add_Ptr=0;
			}
		}
	}
}
void receiveONCALL_SMS_CDMA(int idx)
{
	 
	int     indexofrbuf,len,len1, method, result1,svcNumLen;//,svcNumFormat,rsuNumFormat;//chenxs 20101109 add (svcNumLen,rsuNumLen)短消息中心号码长度，回复号码长度,服务中心号码格式,回复号码格式,回复号码补F
	int     NSrcLength ,i , j , k ,Kx,ky,kz,kl,kn,km, tnum,tnum_old,nci,si;
	char    * pdest1;
	char    *dvl,*dvlC,*dvl_N; 
	POINTER		point;
	char  array[256];
	char  rTONNPI[2];
	DMS_COMMON  oncalldms_common;
	unsigned char   year[3],month[3],day[3],hour[3],minute[3],second[3],
		RsTbf[BUFSIZE],RS_Smsg[BUFSIZE],RsTr[BUFSIZE],RsTr1[BUFSIZE],ShortMassege[BUFSIZE],SendMassege[BUFSIZE],Inquirenumber[20];
	
	int Ix,Jx,kx,kkx,Lx;
	struct tm 	*today;
	time_t		ltime;	
	char templongname[100];//20111008
	char tempsendlongnames[20][40];//20111008

 
	char search_chars[20][20];//20111115
	
	while (manu_send_locked==0  && aotu_alm_send_locked==0 
		&& manu_alm_send_locked==0 && timing_send_locked==0
		&& oncall_sms_timingsendbuf.send_Ptr==oncall_sms_timingsendbuf.add_Ptr
		&& oncall_sms_manualmsendbuf.send_Ptr==oncall_sms_manualmsendbuf.add_Ptr
		&& oncall_sms_manusendbuf.send_Ptr==oncall_sms_manusendbuf.add_Ptr
		&& oncall_sms_autoalmsendbuf.send_Ptr==oncall_sms_autoalmsendbuf.add_Ptr)//20120322
	{ 
		si=999;
		
		ltime=time(NULL);
		today=localtime(&ltime);
		Ix=0,Jx=0,kx=0,kkx=0,Lx=0;
		
		dvlC=";";
		rTONNPI[0]=0;rTONNPI[1]=0;
		if(printflevel>1) 
			printf("\nReceiveShortMsg() :  === for receive quirying messages ===\n");
		if(printflevel>1) 
			printf("\n=======================\n");

		strcpy(RsTbf,"AT");  RsTbf[2]=13; RsTbf[3]=10; RsTbf[4]='\0';
		if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,5,&len,NULL) )
		{	
			char temp_strs[400];
			sprintf(temp_strs,"ReceiveShortMsg() WriteFile 'AT' error! index=%d Flag=%d",idx,_oncallL_sms_send_tempbufs[idx].use_sc);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			//printf("\nONCALL: ReceiveShortMsg() WriteFile 'AT' error !!!\n"); 
			ClearSendCmdFault(idx);
			Sleep(100); 
			_SMS_SENDRECEIVE_COM_STATS[idx]=0;////20101118
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		
		Sleep(150); 
		ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,400,&len,NULL);
		//if(printflevel>0) printf("\n>>>>>>开始查询短信\n"); 
		if(len<1)
		{
			char temp_strs[400];
			sprintf(temp_strs,"查询短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
		
			Sleep(8000);
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		strcpy(RsTbf,"AT+CMGF=1"); RsTbf[9]=13; RsTbf[10]=10; RsTbf[11]='\0';//Set text mode 
		if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,11,&len,NULL) )
		{	
			char temp_strs[400];
			sprintf(temp_strs,"ReceiveShortMsg() WriteFile 'AT+CMGF=1' error index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			//printf("\nONCALL: ReceiveShortMsg() WriteFile 'AT+CMGF=1' error !!!\n"); 
			ClearSendCmdFault(idx); Sleep(100); 
		}
		
		ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,20,&len,NULL);
		if(len>=20)
			ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,BUFSIZE,&len,NULL);
		memset(RsTbf,'\0',BUFSIZE);
		strcpy(RsTbf,"AT+CMGL=\"ALL\""); RsTbf[13]=13; RsTbf[14]=0; RsTbf[15]=0;
		
		if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,14,&len,NULL) )
		{	
			char temp_strs[400];
			sprintf(temp_strs,"ReceiveShortMsg() WriteFile 'AT+CMGL=4' error index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			//printf("\nONCALL: ReceiveShortMsg() WriteFile 'AT+CMGL=4' error !!!\n"); 
			ClearSendCmdFault(idx); 
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		Sleep(500);
		memset(RsTbf,'\0',BUFSIZE);
		ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,BUFSIZE,&len,NULL);
		Sleep(1000);
		if(len<6)
		{
			char temp_strs[400];
			sprintf(temp_strs,"查询短信返回异常[数据过少], index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
			//if(printflevel>0) printf("\n查询短信返回异常[数据过少] index=%d!!!\n",idx); 
			Sleep(8000);
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		if(strstr(RsTbf,"ERROR") && strstr(RsTbf,"AT+CMGL=\"ALL\"")) 
		{
			char temp_strs[400];
			sprintf(temp_strs,"查询短信返回异常，请检查SIM卡是否正确插入及是否损坏, index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);


			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
			//if(printflevel>0)
				//printf("\n查询短信返回异常，请检查SIM卡是否正确插入及是否损坏index=%d!!!\n",idx); 
			Sleep(8000);
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		if(!strstr(RsTbf,"+CMGL:")) 
		{
			if(printflevel>0)
				printf("\n无查询短信 index=%d\n",idx); 
			Sleep(500);
			//if(si>=0)//20120322
			//	_oncallL_sms_send_tempbufs[idx].use_over=1;
			//return;
			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]=0;//20120327
			continue;
		}
		if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]=0;//20120327
		
		
		indexofrbuf=0;
		tnum_old=-99999;
		
		while(indexofrbuf>-1)//多条短信处理
		{ 
			if(indexofrbuf<0)
			{
				//if(si>=0)
				//	_oncallL_sms_send_tempbufs[idx].use_over=1;   
				//return;
				break;
			}
			if(indexofrbuf>BUFSIZE-1000) 
			{
				//if(si>=0)
				//	_oncallL_sms_send_tempbufs[idx].use_over=1;
				//return;
				break;
			}
			for(k=0;k<BUFSIZE-indexofrbuf;k++)
			{
				RsTbf[k]=RsTbf[k+indexofrbuf];
				if(RsTbf[k]==0)
					if(RsTbf[k+1]==0)
						break;
			}
			for(;k<BUFSIZE;k++)//清除后部数据，以免重复解析
			{
				RsTbf[k]=RsTbf[k+indexofrbuf];
				if(RsTbf[k]==0)
					if(RsTbf[k+1]==0)
						break;
			}
			//pdest=strstr(RsTbf,"0891");
			//if(!pdest)return;
			//result=pdest-RsTbf+3;
			pdest1=strstr(RsTbf,"+CMGL:");
			if(!pdest1)
			{
				//if(si>=0)
					//_oncallL_sms_send_tempbufs[idx].use_over=1;
				//return;
				break;
				
			}
			result1=pdest1-RsTbf+6;
			
			if(result1>BUFSIZE-1000)
			{
				//if(si>=0)
					//_oncallL_sms_send_tempbufs[idx].use_over=1;
				//return;
				break;
			}
			RsTr[0] = RsTbf[result1];
			RsTr[1] = RsTbf[result1+1];
			RsTr[2] = '\0' ;
			RsTr1[0] = RsTr[0];
			RsTr1[1] = '\0';           
			if(!strstr(RsTr,","))
				sscanf (RsTr , "%d" , &tnum );
			else sscanf (RsTr1 , "%d" , &tnum );
			if(tnum_old==tnum)
			{
				indexofrbuf+=20;
				continue;
			}
			
			tnum_old=tnum;
			
			pdest1=strstr(RsTbf,"\",\"");
			result1=pdest1-RsTbf+3;
			//memset(strTexty,'\0',BUFSIZE);
			memset(Inquirenumber,'\0',20);
			memset(RsTr,'\0',20);
			for(k=result1,j=0;;k++,j++)  
			{
				if(j>18) break;
				if (RsTbf[k]<48 || RsTbf[k]>57) //if (RsTbf[k] == '\"')
					break;
				RsTr[j]=RsTbf[k];
			}
			strcpy(Inquirenumber,RsTr);
			
			
			svcNumLen = strlen(Inquirenumber);
			k=result1+svcNumLen+4;
			RsTr[0] = RsTbf[k];
			RsTr[1] = '\0' ;    		
			method = atoi(RsTr);
			
			k=result1+svcNumLen+6; 
			
			memset(RsTr,'\0',20);
			for(j=k;j<BUFSIZE-10;j++)    
			{
				if (RsTbf[j]<48 || RsTbf[j]>57) //if (RsTbf[k] == '\"')
					break;
				if (RsTbf[j] == 13)
					break;
				RsTr[j-k]=RsTbf[j];
				RsTr[j-k+1]=0;
			}
			RsTr[j+1] = 0; 
			NSrcLength=-1;
			sscanf(RsTr, "%d",&NSrcLength);
			
			if(NSrcLength<0)NSrcLength=0;
			k=strlen(RsTr);
			k+=result1+svcNumLen+8;  
			indexofrbuf+=k-1;
			
			memset(RsTr,'\0',20);
			for(j=k; j<NSrcLength+k;j++)    
			{
				RsTr[j-k]=RsTbf[j];
				RsTr[j-k+1]=0;
			}
			
			memset(ShortMassege,0,BUFSIZE);
			memset(SendMassege,0,BUFSIZE);
			
			if (method == 4)
				gsmDecodeUcs2(RsTr, ShortMassege, NSrcLength);
			if (method == 2)
				strcpy(ShortMassege,RsTr);
			
			if (tnum<10)
			{ 
				sprintf(RsTr1,"AT+CMGR=%d",tnum); 
				RsTr1[9]=13; RsTr1[10]=10; RsTr1[11]='\0';
				len=11;
			}
			else
			{ 
				sprintf(RsTr1,"AT+CMGR=%d",tnum); 
				RsTr1[10]=13; RsTr1[11]=10; RsTr1[12]='\0';
				len=12;
			}		
			if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],RsTr1,len,&len,NULL))
			{  
				char temp_strs[400];
				sprintf(temp_strs,"CDMA_ReceiveShortMsg() WriteFile 'AT+CMGR' error, index=%d",idx);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);


				if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			 
				ClearSendCmdFault(idx); 
				Sleep(100);
			}
			memset(RsTr1,'\0',BUFSIZE);
			Sleep(1000);
			ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTr1,BUFSIZE,&len,NULL);
			
			pdest1 = strstr(RsTr1, Inquirenumber ); 
			result1 = pdest1-RsTr1;
			k=result1+svcNumLen+3;
			
			if(result1<0) break;
			
			for(j=0;k<BUFSIZE-100;k++)
			{
				if(RsTr1[k]>=48 && RsTr1[k]<=57)
				{
					year[j]=RsTr1[k];
					j++;
				}
				if(j>1)
					break;
			}
			year[2]=0;k++;
			for(j=0;k<BUFSIZE-100;k++)
			{
				if(RsTr1[k]>=48 && RsTr1[k]<=57)
				{
					month[j]=RsTr1[k];
					j++;
				}
				if(j>1)
					break;
			}
			month[2]=0;k++;
			for(j=0;k<BUFSIZE-100;k++)
			{
				if(RsTr1[k]>=48 && RsTr1[k]<=57)
				{
					day[j]=RsTr1[k];
					j++;
				}
				if(j>1)
					break;
			}
			day[2]=0;k++;
			
			for(j=0;k<BUFSIZE-100;k++)
			{
				if(RsTr1[k]>=48 && RsTr1[k]<=57)
				{
					hour[j]=RsTr1[k];
					j++;
				}
				if(j>1)
					break;
			}
			hour[2]=0;k++;
			
			for(j=0;k<BUFSIZE-100;k++)
			{
				if(RsTr1[k]>=48 && RsTr1[k]<=57)
				{
					minute[j]=RsTr1[k];
					j++;
				}
				if(j>1)
					break;
			}
			minute[2]=0;k++;
			
			for(j=0;k<BUFSIZE-100;k++)
			{
				if(RsTr1[k]>=48 && RsTr1[k]<=57)
				{
					second[j]=RsTr1[k];
					j++;
				}
				if(j>1)
					break;
			}
			second[2]=0;k++;
			
			//if(printflevel>0) 
			//	printf("\n\n>>>>%s-%s-%s %s:%s:%s Inquirenumber=%s \n method=%x \n recv message:%s\n", year, month, day, hour, minute, second, Inquirenumber,method, ShortMassege);
			if(1==1)
			{
				char temp_strs[400];
				sprintf(temp_strs,"Inquirenumber=%s  method=%x  recv message:%s, index=%d",Inquirenumber,method, ShortMassege,idx);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);
			}
			for(Kx=0;Kx<12;Kx++)
			{
				if (tnum<10)
				{ 
					sprintf(RsTr1,"AT+CMGD=%d",tnum); 
					RsTr1[9]=13; RsTr1[10]=10; RsTr1[11]='\0';
					len=11;
				}
				else
				{ 
					sprintf(RsTr1,"AT+CMGD=%d",tnum); 
					RsTr1[10]=13; RsTr1[11]=10; RsTr1[12]='\0';
					len=12;
				}		
				if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],RsTr1,len,&len,NULL))
				{  
					char temp_strs[400];
					sprintf(temp_strs,"CDMA_ReceiveShortMsg() WriteFile 'AT+CMGD' error, index=%d",idx);	
					insert_sms_logs(idx,temp_strs);
					//printf("\nOnCall:%s",temp_strs);

					if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
					//	printf("\nTELALM.EXE: CDMA_ReceiveShortMsg() WriteFile 'AT+CMGD' error !!!\n"); 
					ClearSendCmdFault(idx); 
					Sleep(100);
					if(si>=0)
						_oncallL_sms_send_tempbufs[idx].use_over=1;
					return;
				}
				Sleep(150);
				ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTr1,BUFSIZE,&len,NULL);
				if(strstr(RsTr1,"ERROR"))
				{
					Sleep(200); 
					continue;
				}
				else
					break;
			}
			
			kz=-1;
			for(Kx=0;Kx<telphone_def.real_telphone_def_num;Kx++)
			{
				if (strcmp(telphone_def.telphone[Kx], Inquirenumber) ==0) 
				{
					kz=Kx;
					break;
				}
				
				for(ky=0;ky<5;ky++)
				{
					if (strcmp(telphone_def.Other_telphones[Kx][ky], Inquirenumber) ==0) 
						kz=Kx;break;
				}
				if(kz>=0)break;
			}
			if(kz<0)
			{
				kz=0;
				for(ky=0;ky<10;ky++)
				{
					if(telphone_def.countrynum[ky]==0)
						break;
					kz=ky+1;
				}
				for(ky=0;ky<(int)(sizeof(Inquirenumber)-kz);ky++)
				{
					Inquirenumber[ky]=Inquirenumber[ky+kz];
				}
				for(;ky<sizeof(Inquirenumber);ky++)
				{
					Inquirenumber[ky]=0;
				}
				kz=-2;
				for(Kx=0;Kx<telphone_def.real_telphone_def_num;Kx++)
				{
					if (strcmp(telphone_def.telphone[Kx], Inquirenumber) ==0) 
					{
						kz=Kx;
						break;
					}
					
					for(ky=0;ky<5;ky++)
					{
						if (strcmp(telphone_def.Other_telphones[Kx][ky], Inquirenumber) ==0) 
							kz=Kx;break;
					}
					if(kz>=0)break;
				}
			}
			
			
			if(kz<0)//无此查询人员定义
			{
				
			}
			else
			{
				nci=-1;
				for(i=0;i<BUFSIZE;i++)
				{
					if(ShortMassege[i]==0)break;
					if(ShortMassege[i]==':')
					{
						nci=i;
						break;
					}
				}
				
				if(nci>0)
				{
					for(i=0;i<BUFSIZE-1-i-nci;i++)
					{
						ShortMassege[i]=ShortMassege[i+nci+1];
					}
					if(i<BUFSIZE)
						ShortMassege[i+1]='\0';
				}
				
				
				
				//20111115 add start
				for(nci=0;nci<20;nci++)
					memset(search_chars[nci],0,20);
				dvl=strtok(ShortMassege,dvlC);
				nci=0;
				while(dvl)
				{
					strcpy(search_chars[nci],dvl);
					dvl=strtok(NULL,dvlC);
					nci++;
					if(nci>9)
						break;
				}
				//20111115 add end
				
				
				
				nci=0;
				kl=0;
				//dvl=strtok(ShortMassege,dvlC);//20111115 del
				Kx=-1;
				km=-1;
				
				//while(dvl)
				while(search_chars[nci][0]!=0)//20111115 chg
				{
					if(nci>0 && kl>0)
						strcat(SendMassege,";");
					kn=-1;
					for(i=0;i<oncallinquiry_def.real_oncallinquiry_def_num;i++)
					{			
						if (strcmp(oncallinquiry_def.SMS_code[i], search_chars[nci]) ==0) 
						{
							Kx=-1;
							kn++;
							for(ky=0;ky<telphone_def.real_telphone_def_num;ky++)
							{
								if(strcmp(oncallinquiry_def.namecode[i][ky],telphone_def.namecode[kz])==0)
								{
									Kx++;
									break;
								}
							}
							if(Kx<0)//20110307
							{
								for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
								{
									if(Kx>0) break;
									if(oncallinquiry_def.groupnamecode[i][Ix][0]==0)
										continue;
									for(kkx=0;kkx<TELPHONEGROUP_NUM;kkx++)
									{
										if(Kx>0) break;
										if(telphonegroup_def.grpnamecode[kkx][0]==0)
											continue;
										if(strcmp(oncallinquiry_def.groupnamecode[i][Ix],telphonegroup_def.grpnamecode[kkx])==0)
										{
											if(telphonegroup_def.months[kkx][today->tm_mon]!=1)
												continue;
											if(telphonegroup_def.days[kkx][today->tm_mday-1]!=1)
												continue;
											if(telphonegroup_def.weeklimit[kkx][today->tm_wday]!=1)
												continue;
											if(telphonegroup_def.shours[kkx]*60+telphonegroup_def.smins[kkx]<=today->tm_hour*60+today->tm_min)
											{
												if(telphonegroup_def.ehours[kkx]*60+telphonegroup_def.emins[kkx]>=today->tm_hour*60+today->tm_min)
												{
													for(Jx=0;Jx<TELPHONE_NUM;Jx++)
													{
														if(Kx>0) break;
														if(telphonegroup_def.telphonenums[kkx][Jx][0]==0)
															continue;
														if(strcmp(Inquirenumber,telphonegroup_def.telphonenums[kkx][Jx])==0)
														{
															Kx++;
															break;  
														}
													}
												}
											}
										}
									}
								}
							}
							if(Kx<0)
							{
								strcat(SendMassege,"您无权查询该点 ");
								strcat(SendMassege,search_chars[nci]);
								strcat(SendMassege," ,请联系管理员");
								kl++;
								//i=-8;
								continue;
							}
							if(oncallinquiry_def.logicnames[i][0][0]==0)
							{
								strcat(SendMassege,oncallinquiry_def.longname[i]);
								break;
							}
							
							//20111009 add start
							
							for(j=0;j<20;j++)
								memset(tempsendlongnames[j],0,sizeof(tempsendlongnames[j]));
							
							memset(templongname,0,sizeof(templongname));
							strcpy(templongname,oncallinquiry_def.longname[i]);
							dvl_N=strtok(templongname,dvlC);
							j=0;
							while(dvl_N)
							{
								strcpy(tempsendlongnames[j], dvl_N);
								dvl_N=strtok(NULL,dvlC);
								j++;
								if(j>18)break;
							}
							//20111009 add end
							for(j=0;j<20;j++)
							{
								if(oncallinquiry_def.logicnames[i][j][0]==0)
								{
									i=-99;
									break;
								}
								
								if(tempsendlongnames[j][0]!=0)//20111009 add
								{
									if(j>0)	
										strcat(SendMassege,",");
									strcat(SendMassege,tempsendlongnames[j]);
									strcat(SendMassege,":");
								}
								/* if(j==0)//20111009 can
								{
								strcat(SendMassege,oncallinquiry_def.longname[i]);
								strcat(SendMassege," ");
							}*/
								
								if(-1==GetPtId(oncallinquiry_def.logicnames[i][j], &point))
								{
									char temp_strs[400];
									sprintf(temp_strs,"查询逻辑名定义错误 %s!",oncallinquiry_def.logicnames[i][j]);	
									insert_sms_logs(idx,temp_strs);
									//printf("\nOnCall:%s",temp_strs);
								
									//printf("\n>>>ONCALL查询逻辑名定义错误");
									strcat(SendMassege,"逻辑名定义错误，请联系管理员[");
									strcat(SendMassege,search_chars[nci]);
									strcat(SendMassege,"  ");
									strcat(SendMassege,oncallinquiry_def.logicnames[i][j]);
									strcat(SendMassege,"];");

								 

									i=-2;
									kl++;
									break;
								}
								else
								{  
									if(-1==ReadEntryById(&point, &oncalldms_common))
									{
										char temp_strs[400];
										sprintf(temp_strs,"ReadEntryById查询错误 No this point in database %s!",oncallinquiry_def.logicnames[i][j]);	
										insert_sms_logs(idx,temp_strs);
										//printf("\nOnCall:%s",temp_strs);

										//printf("\n>>>ONCALL ReadEntryById查询错误 No this point in database!");
										strcat(SendMassege,"ReadEntryById查询错误,请联系管理员[");
										strcat(SendMassege,search_chars[nci]);
										strcat(SendMassege,"  ");
										strcat(SendMassege,oncallinquiry_def.logicnames[i][j]);
										strcat(SendMassege,"];");
										i=-3;
										kl++;
										break;
									} 
									else
									{
										//if((point.data_type ==SOE_TYPE)|(point.data_type ==POL_TYPE)|(point.data_type ==OUTS_TYPE)|(point.data_type ==SYSMSG_TYPE))   
										Getarray(array,oncalldms_common,point);
										//GetPtLongnameById(&oncalldms_common.point, tmp2);
										if(j>0)
											strcat(SendMassege,", ");
										strcat(SendMassege,array);
										kl++;
									}
								}
							}
							break;
					   }
					   if(i<0) break;
				   }
				   if(kn<0)
				   {
					   strcat(SendMassege,"查询码 ");
					   strcat(SendMassege,search_chars[nci]);
					   strcat(SendMassege," ,不存在，请联系管理员 ");
					   km++;
				   }
				   // dvl=strtok(NULL,dvlC);//20111115 del
				   nci++;
				   if(nci>9)break;
			   }
			   if(kl==0) 
			   {
				   strcat(SendMassege,oncallinquiry_def.smserrcodere);
			   }
			   else if(km>=0)//部分查询码不存在
			   {
				   strcat(SendMassege," ");
				   strcat(SendMassege,oncallinquiry_def.smserrcodere);
			   }
			}
			
			/*strcpy(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],year);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],month);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],day);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr]," ");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],hour);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],minute);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],second);
			
			strcpy(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," NUM:");
			strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],Inquirenumber);
			
			strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," MSG:");
			strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],ShortMassege);
			oncallL_smsquiry_writebuf.add_Ptr++;
			if(oncallL_smsquiry_writebuf.add_Ptr>MAX_WRITEFILEBUFNUM-1)
				oncallL_smsquiry_writebuf.add_Ptr=0;
			*/
			if(kz>=0)
			{ 
				memset(RS_Smsg,0,sizeof(RS_Smsg));
				strcpy(RS_Smsg,SendMassege);
				//strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],SendMassege);
				len=AsciiToUnicode(SendMassege);
				len1=strlen(SendMassege);
				if(len>=0)
				{	
					Sleep(200);
					
					//real =
					
					
					//while(1)
					//{ 
					si=8;
					for(si=0;si<MAX_sms_dev_num;si++)
					{
						if(_oncallL_sms_send_tempbufs[si].use_over==1 && _ONCALLCOM_PORTS[si][0]!=0 && si!=idx)
						{
							_oncallL_sms_send_tempbufs[si].use_over=0;
							_oncallL_sms_send_tempbufs[si].len=len;
							_oncallL_sms_send_tempbufs[si].len1=len1;
							strcpy(_oncallL_sms_send_tempbufs[si].NumStr,Inquirenumber);
							strcpy(_oncallL_sms_send_tempbufs[si].strText,SendMassege);
							strcpy(_oncallL_sms_send_tempbufs[si].send_msg,RS_Smsg);
							_oncallL_sms_send_tempbufs[si].rTONNPI0=defaultTONNPI[0];
							_oncallL_sms_send_tempbufs[si].rTONNPI1=defaultTONNPI[1];
							_oncallL_sms_send_tempbufs[si].use_sc=0;
							//creatthread......... 
							if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoCDMA,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							{
								char temp_strs[400];
								sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								write_main_logs_now(temp_strs,1);
								
								//printf("\nOnCall:%s",temp_strs);
								
								exit(-1);
							}
							////SendMessagetoCDMA(si);
							
							si=-1;
							
							break;
						}
					}
					if(si<0)
					{
						si=9;
						//break;
					}
					else
					{
						_oncallL_sms_send_tempbufs[idx].use_over=-1;
						_oncallL_sms_send_tempbufs[idx].use_sc=0;
						_oncallL_sms_send_tempbufs[idx].len=len;
						_oncallL_sms_send_tempbufs[idx].len1=len1;
						strcpy(_oncallL_sms_send_tempbufs[idx].NumStr,Inquirenumber);
						strcpy(_oncallL_sms_send_tempbufs[idx].strText,SendMassege);
						strcpy(_oncallL_sms_send_tempbufs[idx].send_msg,RS_Smsg);
						_oncallL_sms_send_tempbufs[idx].rTONNPI0=defaultTONNPI[0];
						_oncallL_sms_send_tempbufs[idx].rTONNPI1=defaultTONNPI[1];
						SendMessagetoCDMA(idx);
						//break;
					}
					// break;
					//Sleep(1000);
					//}
					
					
					// real =SendMessagetoCDMA(len, len1,Inquirenumber , SendMassege, RS_Smsg,rTONNPI[0],rTONNPI[1]);
					//if(real>0) 
					//	   strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," _OK");
					//else 
					//	   strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," _ERR");
					//oncallL_smsquiry_writebuf.add_Ptr++;
					//if(oncallL_smsquiry_writebuf.add_Ptr>MAX_WRITEFILEBUFNUM-1)
					//	oncallL_smsquiry_writebuf.add_Ptr=0;
				}
				
			}
			
			
   }

   }
   if(si>=0)
	   _oncallL_sms_send_tempbufs[idx].use_over=1;
}
void receiveONCALL_SMS_GSM(int idx)
{
	
	int     indexofrbuf,lenn, lenn1,len,len1, method, result, result1,svcNumLen,rsuNumLen,rsuNumLenF;//,svcNumFormat,rsuNumFormat;//chenxs 20101109 add (svcNumLen,rsuNumLen)短消息中心号码长度，回复号码长度,服务中心号码格式,回复号码格式,回复号码补F
	int     NSrcLength ,i , j , k ,Kx,ky,kz,kl,kn,km, tnum,tnum_old,nci,si;
	char   * pdest, * pdest1;
	char    *dvl,*dvlC; 
	POINTER		point;
	char  array[256];
	char  rTONNPI[2];
	DMS_COMMON  oncalldms_common;
	unsigned char   year[2],month[2],day[2],hour[2],minute[2],second[2],TimeZone[2],RsTbf[BUFSIZE],RS_Smsg[BUFSIZE],RsTr[BUFSIZE],tempsmssendbuf[BUFSIZE],RsTr1[BUFSIZE],ShortMassege[BUFSIZE],SendMassege[BUFSIZE],Inquirenumber[20];
	int Ix,Jx,kx,Lx,kkx;
	struct tm 	*today;
	time_t		ltime;	
	char templongname[100];//20111008
	char tempsendlongnames[20][40];//20111008
	char    *dvl_N;
	
	char search_chars[20][20];//20111115
	char Sendnumber[20];
	
	while (manu_send_locked==0  && aotu_alm_send_locked==0 
		&& manu_alm_send_locked==0 && timing_send_locked==0
		&& oncall_sms_timingsendbuf.send_Ptr==oncall_sms_timingsendbuf.add_Ptr
		&& oncall_sms_manualmsendbuf.send_Ptr==oncall_sms_manualmsendbuf.add_Ptr
		&& oncall_sms_manusendbuf.send_Ptr==oncall_sms_manusendbuf.add_Ptr
		&& oncall_sms_autoalmsendbuf.send_Ptr==oncall_sms_autoalmsendbuf.add_Ptr)//20120322
	{ 
		
		si=999;
		Sleep(1000);
		ltime=time(NULL);
		today=localtime(&ltime);
		Ix=0,Jx=0,kx=0,kkx=0,Lx=0;
		dvlC=";";
		if(printflevel>1) 
			printf("\nReceiveShortMsg() :  === for receive quirying messages ===\n");
		if(printflevel>1)
			printf("\n=======================\n");
		strcpy(RsTbf,"AT");  RsTbf[2]=13; RsTbf[3]=10; RsTbf[4]='\0';
		if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,5,&len,NULL) )
		{	
			char temp_strs[400];
			sprintf(temp_strs,"ReceiveShortMsg() WriteFile 'AT' error! index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			//printf("\nONCALL: ReceiveShortMsg() WriteFile 'AT' error !!!\n"); 
			ClearSendCmdFault(idx);
			Sleep(100); 
			_SMS_SENDRECEIVE_COM_STATS[idx]=0;////20101118
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		
		Sleep(150); 
		ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,400,&len,NULL);
		//if(printflevel>0) printf("\n>>>>>>开始查询短信\n"); 
		if(len<1)
		{
			char temp_strs[400];
			sprintf(temp_strs,"查询短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
			//if(printflevel>0) 
			//	printf("\n查询短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常!!!\n",idx); 
			Sleep(8000);

			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		strcpy(RsTbf,"AT+CMGF=0"); RsTbf[9]=13; RsTbf[10]=10; RsTbf[11]='\0';//Set PDU mode 
		if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,11,&len,NULL) )
		{	
			char temp_strs[400];
			sprintf(temp_strs,"ReceiveShortMsg() WriteFile 'AT+CMGF=0' error index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			//printf("\nONCALL: ReceiveShortMsg() WriteFile 'AT+CMGF=0' error !!!\n"); 
			ClearSendCmdFault(idx); 
			Sleep(100); 
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		
		ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,20,&len,NULL);
		if(len>=20)
			ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,BUFSIZE,&len,NULL);
		memset(RsTbf,'\0',BUFSIZE);
		strcpy(RsTbf,"AT+CMGL=4"); RsTbf[9]=13; RsTbf[10]=10; RsTbf[11]='\0';
		
		if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,11,&len,NULL) )
		{	
			char temp_strs[400];
			sprintf(temp_strs,"ReceiveShortMsg() WriteFile 'AT+CMGF=4' error index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
			//printf("\nONCALL: ReceiveShortMsg() WriteFile 'AT+CMGL=4' error !!!\n"); 
			ClearSendCmdFault(idx); 
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		Sleep(1000);
		memset(RsTbf,'\0',BUFSIZE);
		ReadFile(_SMS_SENDRECEIVE_COMS[idx],RsTbf,BUFSIZE,&len,NULL);
		if(len<6)
		{
			char temp_strs[400];
			sprintf(temp_strs,"查询短信返回异常[数据过少] index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
			//if(printflevel>0) printf("\n查询短信返回异常[数据过少] index=%d!!!\n",idx); 
			Sleep(8000);
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		if(strstr(RsTbf,"ERROR") && strstr(RsTbf,"AT+CMGL=4")) 
		{
			char temp_strs[400];
			sprintf(temp_strs,"查询短信返回异常，请检查SIM卡是否正确插入及是否损坏 index=%d",idx);	
			insert_sms_logs(idx,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
			//if(printflevel>0) printf("\n查询短信返回异常，请检查SIM卡是否正确插入及是否损坏 index=%d!!!\n",idx); 
			Sleep(8000);
			if(si>=0)
				_oncallL_sms_send_tempbufs[idx].use_over=1;
			return;
		}
		if(!strstr(RsTbf,"+CMGL:")) 
		{
			if(printflevel>0)
				printf("\n无查询短信 index=%d!!!\n",idx); 
			Sleep(500);
			//if(si>=0)//20120322
			//	_oncallL_sms_send_tempbufs[idx].use_over=1;
			//return;
			if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]=0;//20120327
			continue;
		}
		if(lock_oncall_sms_err_times==0)
			oncall_sms_err_times[idx]=0;//20120327
		
		
		indexofrbuf=0;
		tnum_old=-99999;
		
		while(indexofrbuf>-1)//多条短信处理
		{ 
			if(indexofrbuf<0)
			{
				//if(si>=0)
				//	_oncallL_sms_send_tempbufs[idx].use_over=1;
				//return;
				break;
			}
			
			if(indexofrbuf>BUFSIZE-1000)
			{
				//if(si>=0)
				//	_oncallL_sms_send_tempbufs[idx].use_over=1;
			//	return;
					break;
			}
			for(k=0;k<BUFSIZE-indexofrbuf;k++)
			{
				RsTbf[k]=RsTbf[k+indexofrbuf];
				if(RsTbf[k]==0)
					if(RsTbf[k+1]==0)
						break;
			}
			for(;k<BUFSIZE;k++)//清除后部数据，以免重复解析
			{
				RsTbf[k]=RsTbf[k+indexofrbuf];
				if(RsTbf[k]==0)
					if(RsTbf[k+1]==0)
						break;
			}
			pdest=strstr(RsTbf,"0891");
			if(!pdest)
			{
				//if(si>=0)
				//	_oncallL_sms_send_tempbufs[idx].use_over=1;
				//return;
					break;
			}
			result=pdest-RsTbf+3;
			pdest1=strstr(RsTbf,"+CMGL:");
			if(!pdest1)
			{
				//if(si>=0)
				//	_oncallL_sms_send_tempbufs[idx].use_over=1;
				//return;
				break;
			}
			result1=pdest1-RsTbf+6;
			
			if(result1>BUFSIZE-1000)
			{
				//if(si>=0)
				//	_oncallL_sms_send_tempbufs[idx].use_over=1;
				//return;
					break;
			}
			RsTr[0] = RsTbf[result1+1];RsTr[1] = RsTbf[result1+2];RsTr[2] = '\0' ;
			RsTr1[0] = RsTr[0];RsTr1[1] = '\0';           
			if(!strstr(RsTr,","))
				sscanf (RsTr , "%d" , &tnum );
			else sscanf (RsTr1 , "%d" , &tnum );
			if(tnum_old==tnum)
			{
				indexofrbuf+=20;continue;
			}
			
			tnum_old=tnum;
			
			k=result-3;
			RsTr[0] = RsTbf[k];RsTr[1] = RsTbf[k+1];RsTr[2] = '\0' ;    
			sscanf (RsTr , "%x" , &svcNumLen );
			svcNumLen=svcNumLen*2;//字节长度
			
			k=result+svcNumLen+1;//(-3+2+2)
			RsTr[0] = RsTbf[k];RsTr[1] = RsTbf[k+1];RsTr[2] = '\0' ;    
			sscanf (RsTr , "%x" , &rsuNumLen);
			if(rsuNumLen%2==0) rsuNumLenF=0;//是否补F
			else rsuNumLenF=1;
			
			k=result+svcNumLen+3;
			rTONNPI[0]=RsTbf[k];
			rTONNPI[1]=RsTbf[k+1];
			
			for(k=result+svcNumLen+5,j=0;j<rsuNumLen+rsuNumLenF;k++,j++)//取查询手机号码并解码         
				Sendnumber[j]=RsTbf[k];
			strcpy(RsTr,Sendnumber);
			DeCodeMobileNuber(RsTr ,rsuNumLen+rsuNumLenF); 
			RsTr[rsuNumLen] = '\0' ; 
			strcpy(Inquirenumber,RsTr);
			
			k=result+svcNumLen+rsuNumLen+rsuNumLenF+8;
			RsTr[0]=RsTbf[k];RsTr[1] = '\0' ; method=atoi(RsTr);
			
			for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+9,j=0;j<2;k++,j++) RsTr[j]=RsTbf[k]; //取查询时间年、月、日、时、分、秒	      
			DeCodeMobileNuber(RsTr ,2 );RsTr[2] = '\0' ; strcpy(year,RsTr); 
			
			for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+11,j=0;j<2;k++,j++) RsTr[j]=RsTbf[k];
			DeCodeMobileNuber(RsTr ,2 );RsTr[2] = '\0' ; strcpy(month,RsTr);   
			
			for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+13,j=0;j<2;k++,j++) RsTr[j]=RsTbf[k];
			DeCodeMobileNuber(RsTr ,2 );RsTr[2] = '\0' ; strcpy(day,RsTr);  
			
			for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+15,j=0;j<2;k++,j++) RsTr[j]=RsTbf[k];
			DeCodeMobileNuber(RsTr ,2 );RsTr[2] = '\0' ; strcpy(hour,RsTr);
			
			for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+17,j=0;j<2;k++,j++) RsTr[j]=RsTbf[k];
			DeCodeMobileNuber(RsTr ,2 );RsTr[2] = '\0' ; strcpy(minute,RsTr);  
			
			for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+19,j=0;j<2;k++,j++) RsTr[j]=RsTbf[k];
			DeCodeMobileNuber(RsTr ,2 );RsTr[2] = '\0' ; strcpy(second,RsTr);  
			
			for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+21,j=0;j<2;k++,j++) RsTr[j]=RsTbf[k];
			DeCodeMobileNuber(RsTr ,2 );RsTr[2] = '\0' ; strcpy(TimeZone,RsTr);  
			
			for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+23,j=0;j<2;k++,j++) RsTr[j]=RsTbf[k];   //取短信息长度  
			RsTr[2] = '\0' ; sscanf (RsTr , "%x" , &NSrcLength);	
			
			indexofrbuf+=result+svcNumLen+rsuNumLen+rsuNumLenF+25;
			
			
			memset(ShortMassege,'\0',BUFSIZE);
			memset(SendMassege,'\0',BUFSIZE);
			switch(method)
			{
			case 0:	  //取短信息内容并按照7-bit解码
				for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+25,j=0;RsTbf[k]!=13 && j<NSrcLength*2;k++,j++)    
					RsTr[j]=RsTbf[k];
				lenn  = gsmString2Bytes(RsTr, RsTr1, NSrcLength,0);
				lenn1 = gsmDecode7bit(RsTr1, ShortMassege, NSrcLength);
				ShortMassege[NSrcLength] = '\0' ; 
				break;
			case 8:   //取短信息内容并按照UCS2解码
				for(k=result+svcNumLen+rsuNumLen+rsuNumLenF+25,j=0;j<NSrcLength*2;k++,j++)  
					RsTr[j]=RsTbf[k];
				RsTr[NSrcLength*2] = '\0' ; 
				lenn  = gsmString2Bytes(RsTr, RsTr1, NSrcLength,1);
				lenn1 = gsmDecodeUcs2(RsTr1, ShortMassege, NSrcLength);
				break;
			default : printf("\noncall method error\n");
			}
			
			//if(printflevel>0) printf("\n\n>>>>%s-%s-%s %s:%s:%s Inquirenumber=%s \n method=%x \n recv message:%s\n", year, month, day, hour, minute, second, Inquirenumber,method, ShortMassege);
			
			if(1==1)
			{
				char temp_strs[400];
				sprintf(temp_strs,"Inquirenumber=%s  method=%x  recv message:%s, index=%d",Inquirenumber,method, ShortMassege,idx);	
				insert_sms_logs(idx,temp_strs);
				//printf("\nOnCall:%s",temp_strs);
			}

			for(Kx=0;Kx<12;Kx++)
			{
				memset(tempsmssendbuf,'\0',BUFSIZE);//短信多时常常无法正常删除，多执行几次OK
				sprintf(tempsmssendbuf,"AT+CMGD=%d",tnum);
				if (tnum<10){len=11; tempsmssendbuf[9]=13; tempsmssendbuf[10]=10; tempsmssendbuf[11]='\0';}//删除短信
				else{len=12;tempsmssendbuf[10]=13; tempsmssendbuf[11]=10; tempsmssendbuf[12]='\0';}	
				
				if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,len,&len,NULL) )
				{
					char temp_strs[400];
					sprintf(temp_strs,"receiveONCALL_SMS_GSM() WriteFile 'AT+CMGD' error index=%d",idx);	
					insert_sms_logs(idx,temp_strs);
					//printf("\nOnCall:%s",temp_strs);


					//printf("\nONCALL: receiveONCALL_SMS_GSM() WriteFile 'AT+CMGD' error !!!\n");
					Sleep(10);
					if(si>=0)
						_oncallL_sms_send_tempbufs[idx].use_over=1;
					return;
				}
				Sleep(150);
				ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,BUFSIZE,&len,NULL);
				if(strstr(tempsmssendbuf,"ERROR"))
				{
					Sleep(200); 
					continue;
				}
				else
					break;
				
			}
			
			kz=-1;
			
			for(Kx=0;Kx<telphone_def.real_telphone_def_num;Kx++)
			{
				if (strcmp(telphone_def.telphone[Kx], Inquirenumber) ==0) 
				{
					kz=Kx;
					break;
				}
				
				for(ky=0;ky<5;ky++)
				{
					if (strcmp(telphone_def.Other_telphones[Kx][ky], Inquirenumber) ==0) 
						kz=Kx;break;
				}
				if(kz>=0)break;
			}
			if(kz<0)
			{
				kz=0;
				for(ky=0;ky<10;ky++)
				{
					if(telphone_def.countrynum[ky]==0)
						break;
					kz=ky+1;
				}
				for(ky=0;ky<(int)(sizeof(Inquirenumber)-kz);ky++)
				{
					Inquirenumber[ky]=Inquirenumber[ky+kz];
				}
				for(;ky<sizeof(Inquirenumber);ky++)
				{
					Inquirenumber[ky]=0;
				}
				kz=-2;
				for(Kx=0;Kx<telphone_def.real_telphone_def_num;Kx++)
				{
					if (strcmp(telphone_def.telphone[Kx], Inquirenumber) ==0) 
					{
						kz=Kx;
						break;
					}
					
					for(ky=0;ky<5;ky++)
					{
						if (strcmp(telphone_def.Other_telphones[Kx][ky], Inquirenumber) ==0) 
							kz=Kx;break;
					}
					if(kz>=0)break;
				}
			}
			
			if(kz<0)//无此查询人员定义
			{
				
			}
			else
			{
				nci=-1;
				for(i=0;i<BUFSIZE;i++)
				{
					if(ShortMassege[i]==0)break;
					if(ShortMassege[i]==':')
					{
						nci=i;
						break;
					}
				}
				
				if(nci>0)
				{
					for(i=0;i<BUFSIZE-1-i-nci;i++)
					{
						ShortMassege[i]=ShortMassege[i+nci+1];
					}
					if(i<BUFSIZE)
						ShortMassege[i+1]='\0';
				}
				
				
				
				//20111115 add start
				for(nci=0;nci<20;nci++)
					memset(search_chars[nci],0,20);
				dvl=strtok(ShortMassege,dvlC);
				nci=0;
				while(dvl)
				{
					strcpy(search_chars[nci],dvl);
					dvl=strtok(NULL,dvlC);
					nci++;
					if(nci>9)
						break;
				}
				//20111115 add end
				
				nci=0;
				kl=0;
				//dvl=strtok(ShortMassege,dvlC);
				Kx=-1;
				km=-1;
				//while(dvl)
				while(search_chars[nci][0]!=0)//20111115 chg
				{
					if(nci>0 && kl>0)
						strcat(SendMassege,";");
					kn=-1;
					for(i=0;i<oncallinquiry_def.real_oncallinquiry_def_num;i++)
					{			
						if (strcmp(oncallinquiry_def.SMS_code[i], search_chars[nci]) ==0) 
						{
							Kx=-1;
							kn++;
							for(ky=0;ky<telphone_def.real_telphone_def_num;ky++)
							{
								if(strcmp(oncallinquiry_def.namecode[i][ky],telphone_def.namecode[kz])==0)
								{
									Kx++;
									break;  
								}
							}
							if(Kx<0)//20110307
							{
								for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
								{
									if(Kx>0) break;
									if(oncallinquiry_def.groupnamecode[i][Ix][0]==0)
										continue;
									for(kkx=0;kkx<TELPHONEGROUP_NUM;kkx++)
									{
										if(Kx>0) break;
										if(telphonegroup_def.grpnamecode[kkx][0]==0)
											continue;
										if(strcmp(oncallinquiry_def.groupnamecode[i][Ix],telphonegroup_def.grpnamecode[kkx])==0)
										{
											if(telphonegroup_def.months[kkx][today->tm_mon]!=1)
												continue;
											if(telphonegroup_def.days[kkx][today->tm_mday-1]!=1)
												continue;
											if(telphonegroup_def.weeklimit[kkx][today->tm_wday]!=1)
												continue;
											if(telphonegroup_def.shours[kkx]*60+telphonegroup_def.smins[kkx]<=today->tm_hour*60+today->tm_min)
											{
												if(telphonegroup_def.ehours[kkx]*60+telphonegroup_def.emins[kkx]>=today->tm_hour*60+today->tm_min)
												{
													for(Jx=0;Jx<TELPHONE_NUM;Jx++)
													{
														if(Kx>0) break;
														if(telphonegroup_def.telphonenums[kkx][Jx][0]==0)
															continue;
														if(strcmp(Inquirenumber,telphonegroup_def.telphonenums[kkx][Jx])==0)
														{
															Kx++;
															break;  
														}
													}
												} 
											}
										}
									}
								}
							}
							if(Kx<0)
							{
								strcat(SendMassege,"您无权查询该点 ");
								strcat(SendMassege,search_chars[nci]);
								strcat(SendMassege," ,请联系管理员");
								kl++;
								//i=-8;
								continue;
							}
							if(oncallinquiry_def.logicnames[i][0][0]==0)
							{
								strcat(SendMassege,oncallinquiry_def.longname[i]);
								break;
							}
							
							
							
							//20111009 add start
							
							for(j=0;j<20;j++)
								memset(tempsendlongnames[j],0,sizeof(tempsendlongnames[j]));
							
							memset(templongname,0,sizeof(templongname));
							strcpy(templongname,oncallinquiry_def.longname[i]);
							dvl_N=strtok(templongname,dvlC);
							j=0;
							while(dvl_N)
							{
								strcpy(tempsendlongnames[j], dvl_N);
								dvl_N=strtok(NULL,dvlC);
								j++;
								if(j>18)break;
							}
							//20111009 add end
							
							for(j=0;j<20;j++)
							{
								if(oncallinquiry_def.logicnames[i][j][0]==0)
								{
									i=-99;
									break;
								}
								if(tempsendlongnames[j][0]!=0)//20111009 add
								{
									if(j>0)	
										strcat(SendMassege,",");
									strcat(SendMassege,tempsendlongnames[j]);
									strcat(SendMassege,":");
								}
								
								/*if(j==0)
								{
								strcat(SendMassege,oncallinquiry_def.longname[i]);
								strcat(SendMassege," ");
							}*/
								
								if(-1==GetPtId(oncallinquiry_def.logicnames[i][j], &point))
								{
									char temp_strs[400];
									sprintf(temp_strs,"查询逻辑名定义错误 %s!",oncallinquiry_def.logicnames[i][j]);	
									insert_sms_logs(idx,temp_strs);
									//printf("\nOnCall:%s",temp_strs);

									//printf("\n>>>ONCALL查询逻辑名定义错误");
									strcat(SendMassege,"逻辑名定义错误，请联系管理员[");
									strcat(SendMassege,search_chars[nci]);
									strcat(SendMassege,"  ");
									strcat(SendMassege,oncallinquiry_def.logicnames[i][j]);
									strcat(SendMassege,"];");
									i=-2;
									kl++;
									break;
								}
								else
								{  
									if(-1==ReadEntryById(&point, &oncalldms_common))
									{
										char temp_strs[400];
										sprintf(temp_strs,"ReadEntryById查询错误 No this point in database %s!",oncallinquiry_def.logicnames[i][j]);	
										insert_sms_logs(idx,temp_strs);
										//printf("\nOnCall:%s",temp_strs);
										//printf("\n>>>ONCALL ReadEntryById查询错误 No this point in database!");
										strcat(SendMassege,"ReadEntryById查询错误,请联系管理员[");
										strcat(SendMassege,search_chars[nci]);
										strcat(SendMassege,"  ");
										strcat(SendMassege,oncallinquiry_def.logicnames[i][j]);
										strcat(SendMassege,"];");
										i=-3;
										kl++;
										break;
									} 
									else
									{
										//if((point.data_type ==SOE_TYPE)|(point.data_type ==POL_TYPE)|(point.data_type ==OUTS_TYPE)|(point.data_type ==SYSMSG_TYPE))   
										Getarray(array,oncalldms_common,point);
										//GetPtLongnameById(&oncalldms_common.point, tmp2);
										if(j>0)
											strcat(SendMassege,", ");
										strcat(SendMassege,array);
										kl++;
									}
								}
							}
							break;
					   }
					   if(i<0) break;
				   }
				   if(kn<0)
				   {
					   strcat(SendMassege,"查询码 ");
					   strcat(SendMassege,search_chars[nci]);
					   strcat(SendMassege," ,不存在，请联系管理员 ");
					   km++;
				   }
				   //dvl=strtok(NULL,dvlC);
				   nci++;
				   if(nci>9)break;
			   }
			   if(kl==0) 
				   strcat(SendMassege,oncallinquiry_def.smserrcodere);
			   else if(km>=0)//部分查询码不存在
			   {strcat(SendMassege," ");strcat(SendMassege,oncallinquiry_def.smserrcodere);}
			}
			
		/*	strcpy(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],year);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],month);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],"-");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],day);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr]," ");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],hour);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],minute);
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],":");
			strcat(oncallL_smsquiry_writebuf.time_str[oncallL_smsquiry_writebuf.add_Ptr],second);
			
			strcpy(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," NUM:");
			strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],Inquirenumber);
			
			strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," MSG:");
			strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],ShortMassege);
			oncallL_smsquiry_writebuf.add_Ptr++;
			if(oncallL_smsquiry_writebuf.add_Ptr>MAX_WRITEFILEBUFNUM-1)
				oncallL_smsquiry_writebuf.add_Ptr=0;
			*/
			if(kz>=0)
			{ 
				memset(RS_Smsg,0,sizeof(RS_Smsg));
				strcpy(RS_Smsg,SendMassege);
				//strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr],SendMassege);
				len=AsciiToUnicode(SendMassege);
				len1=strlen(SendMassege);
				if(len>=0)
				{	
					Sleep(200);
					
					//while(1)
					//{ 
					si=8;
					for(si=0;si<MAX_sms_dev_num;si++)
					{
						if(_oncallL_sms_send_tempbufs[si].use_over==1 && _ONCALLCOM_PORTS[si][0]!=0 && si!=idx)
						{
							_oncallL_sms_send_tempbufs[si].use_over=0;
							_oncallL_sms_send_tempbufs[si].len=len;
							_oncallL_sms_send_tempbufs[si].len1=len1;
							strcpy(_oncallL_sms_send_tempbufs[si].NumStr,Inquirenumber);
							strcpy(_oncallL_sms_send_tempbufs[si].strText,SendMassege);
							strcpy(_oncallL_sms_send_tempbufs[si].send_msg,RS_Smsg);
							_oncallL_sms_send_tempbufs[si].rTONNPI0=defaultTONNPI[0];
							_oncallL_sms_send_tempbufs[si].rTONNPI1=defaultTONNPI[1];
							_oncallL_sms_send_tempbufs[si].use_sc=0;
							//creatthread......... 
							if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoGSM,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							{
								char temp_strs[400];
								sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								write_main_logs_now(temp_strs,1);
								
								//printf("\nOnCall:%s",temp_strs);
								
								exit(-1);

							 
							}
							////SendMessagetoGSM(si);
							
							si=-1;
							
							break;
						}
					}
					if(si<0)
					{
						si=9;
						//break;
					}
					else
					{
						_oncallL_sms_send_tempbufs[idx].use_sc=0;
						_oncallL_sms_send_tempbufs[idx].use_over=-1;
						_oncallL_sms_send_tempbufs[idx].len=len;
						_oncallL_sms_send_tempbufs[idx].len1=len1;
						strcpy(_oncallL_sms_send_tempbufs[idx].NumStr,Inquirenumber);
						strcpy(_oncallL_sms_send_tempbufs[idx].strText,SendMassege);
						strcpy(_oncallL_sms_send_tempbufs[idx].send_msg,RS_Smsg);
						_oncallL_sms_send_tempbufs[idx].rTONNPI0=defaultTONNPI[0];
						_oncallL_sms_send_tempbufs[idx].rTONNPI1=defaultTONNPI[1];
						SendMessagetoGSM(idx);
						//break;
					}
					// break;
					//Sleep(1000);
					//}
					
					
					/*if(real>0) 
					strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," _OK");
					else 
					strcat(oncallL_smsquiry_writebuf.message[oncallL_smsquiry_writebuf.add_Ptr]," _ERR");
					*/
				//	oncallL_smsquiry_writebuf.add_Ptr++;
				//	if(oncallL_smsquiry_writebuf.add_Ptr>MAX_WRITEFILEBUFNUM-1)
				//		oncallL_smsquiry_writebuf.add_Ptr=0;
				}
			}
   }
  
   }
   if(si>=0)
	   _oncallL_sms_send_tempbufs[idx].use_over=1;
}
void sendONCALL_SMS_AUTOALMSENDBUF(int idx)
{
	   int		i, j, len,len1,si;
	   char	NumStr[20];
	   unsigned char strText[BUFSIZE], tempsmssendbuf[BUFSIZE],send_msg[BUFSIZE];
	   si=99;
	   if(oncall_sms_autoalmsendbuf.send_Ptr==oncall_sms_autoalmsendbuf.add_Ptr)
	   { 
		   //printf("\nONCALL: 无自动报警短信!\n");
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   aotu_alm_send_locked=0;
		   return;
	   }
	   
	   strcpy(tempsmssendbuf,"AT");  tempsmssendbuf[2]=13; tempsmssendbuf[3]=10; tempsmssendbuf[4]='\0';
	   if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,5,&len,NULL) )
	   {	
		   char temp_strs[400];
		   sprintf(temp_strs,"sendONCALL_SMS_AUTOALMSENDBUF() WriteFile 'AT' error  index=%d!",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);

		   //printf("\nONCALL: sendONCALL_SMS_AUTOALMSENDBUF() WriteFile 'AT' error !!!\n"); 
		   ClearSendCmdFault(idx); 

		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327

		   _SMS_SENDRECEIVE_COM_STATS[idx]=0;////20101118
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   aotu_alm_send_locked=0;
		   return;
	   }
	   Sleep(200); 
	   ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,400,&len,NULL);
	   if(len<1)
	   {
		   char temp_strs[400];
		   sprintf(temp_strs,"处理自动报警短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常!",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);


		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
		   //if(printflevel>0) 
		//	   printf("\n处理自动报警短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常!!!\n",idx); 
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   aotu_alm_send_locked=0;
		   return;
	   }
	   strcpy(tempsmssendbuf,"AT+CMGF=0"); tempsmssendbuf[9]=13; tempsmssendbuf[10]=10; tempsmssendbuf[11]='\0';//Set PDU mode 
	   if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,11,&len,NULL) )
	   {	
		   char temp_strs[400];
		   sprintf(temp_strs,"sendONCALL_SMS_AUTOALMSENDBUF() WriteFile 'AT+CMGF=0' error!  index=%d",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);


		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
		   //printf("\nONCALL: sendONCALL_SMS_AUTOALMSENDBUF() WriteFile 'AT+CMGF=0' error !!!\n"); 
		   ClearSendCmdFault(idx); 
		   Sleep(100); 
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   aotu_alm_send_locked=0;
		   return;
	   }
	   ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,BUFSIZE,&len,NULL);
//	   strcpy(SmsCenter,_strSmsCenterS[idx]); 
	  
	   for(i=oncall_sms_autoalmsendbuf.send_Ptr;i!=oncall_sms_autoalmsendbuf.add_Ptr;)
	   {    		 
		   if(oncall_sms_autoalmsendbuf.canceled==1)
		   {
			   oncall_sms_autoalmsendbuf.canceled=0;
			   break;
		   }
		   memset(strText,0,sizeof(strText));
		   memset(send_msg,0,sizeof(send_msg));
		   strcpy(send_msg,oncall_sms_autoalmsendbuf.time_str[i]);
		   strcat(send_msg," ");
		   strcat(send_msg,oncall_sms_autoalmsendbuf.longname[i]);
		   strcat(send_msg," ");
		   strcat(send_msg,oncall_sms_autoalmsendbuf.message[i]);
		   strcpy(strText,send_msg);
		   len=AsciiToUnicode( strText );
		   len1=strlen(strText );
		   if(len>=0)
		   {	
			   for(j=0;j<TELPHONE_NUM;j++)
			   { 	  
				   if(oncall_sms_autoalmsendbuf.canceled==1)
				   {
					   oncall_sms_autoalmsendbuf.canceled=0;
					   break;
				   }
				   if(oncall_sms_autoalmsendbuf.telnum[i][j][0]=='\0')
					   continue;
				   strcpy(NumStr,oncall_sms_autoalmsendbuf.telnum[i][j]);


				   si=8;
				   for(si=0;si<MAX_sms_dev_num;si++)
				   {
					   if(_oncallL_sms_send_tempbufs[si].use_over==1 && _ONCALLCOM_PORTS[si][0]!=0 && si!=idx)
					   {
						   char temp_strs[400];
						   sprintf(temp_strs,"SMS_AUTOALMSENDBUF thread index=%d",si);	
						   insert_sms_logs(idx,temp_strs);
						   //printf("\nOnCall:%s",temp_strs);

						//   printf("\nSMS_AUTOALMSENDBUF thread index=%d\n",idx);
						   _oncallL_sms_send_tempbufs[si].use_over=0;
						   _oncallL_sms_send_tempbufs[si].len=len;
						   _oncallL_sms_send_tempbufs[si].len1=len1;
						   strcpy(_oncallL_sms_send_tempbufs[si].NumStr,NumStr);
						   strcpy(_oncallL_sms_send_tempbufs[si].strText,strText);
						   strcpy(_oncallL_sms_send_tempbufs[si].send_msg,send_msg);
						   _oncallL_sms_send_tempbufs[si].rTONNPI0=defaultTONNPI[0];
						   _oncallL_sms_send_tempbufs[si].rTONNPI1=defaultTONNPI[1];
						   _oncallL_sms_send_tempbufs[si].use_sc=0;
						   //creatthread......... 
						 
						  
						   if(_ONCALL_SMS_DEV_TYPES[si]==2)
						   {  
							   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoCDMA,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							   {
								   char temp_strs[400];
								   sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								   write_main_logs_now(temp_strs,1);
								   
								   //printf("\nOnCall:%s",temp_strs);
								   
								exit(-1);
							   }
						   }
						   else
						   { 
							   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoGSM,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							   {
								   char temp_strs[400];
								   sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								   write_main_logs_now(temp_strs,1);
								   
								   //printf("\nOnCall:%s",temp_strs);
								   
								exit(-1);
							   }
						   }
 
						   
						   si=-1;
						   
						   break;
					   }
				   }
				   if(si<0)
				   {
					   si=9;
					   //break;
				   }
				   else
				   {
					   char temp_strs[400];
					   sprintf(temp_strs,"SMS_AUTOALMSENDBUF self index=%d",idx);	
					   insert_sms_logs(idx,temp_strs);
					   //printf("\nOnCall:%s",temp_strs);

					   //printf("\nSMS_AUTOALMSENDBUF self index=%d\n",idx);
					   _oncallL_sms_send_tempbufs[idx].use_sc=0;
					   _oncallL_sms_send_tempbufs[idx].use_over=-1;
					   _oncallL_sms_send_tempbufs[idx].len=len;
					   _oncallL_sms_send_tempbufs[idx].len1=len1;
					   strcpy(_oncallL_sms_send_tempbufs[idx].NumStr,NumStr);
					   strcpy(_oncallL_sms_send_tempbufs[idx].strText,strText);
					   strcpy(_oncallL_sms_send_tempbufs[idx].send_msg,send_msg);
					   _oncallL_sms_send_tempbufs[idx].rTONNPI0=defaultTONNPI[0];
						   _oncallL_sms_send_tempbufs[idx].rTONNPI1=defaultTONNPI[1];
					   if(_ONCALL_SMS_DEV_TYPES[idx]==2)
					   { 
						   SendMessagetoCDMA(idx);
					   }
					   else
					   {
						   SendMessagetoGSM(idx);
					   }
				   }


			   }
			   /*strcpy(NumStr,telphone_def.telphone[j]);
			   len=AsciiToUnicode( strText );
			   len1=strlen( strText );
			   CreateMobileNum( strDesTel );  
			   //real = SendMessagetoGSM(len, len1, NumStr, strText, send_msg,defaultTONNPI[0],defaultTONNPI[1]);*/
		   }

		   if(1==1)
		   {
			   char temp_strs[400];
			   sprintf(temp_strs,"自动报警短信[%d]处理完成,sms dev index=%d",i,idx);	
			   insert_sms_logs(idx,temp_strs);
			   //printf("\nOnCall:%s",temp_strs);
		   }
		   //printf("\nONCALL: 自动报警短信[%d]处理完成!\n",i);
		   i++;
		   if(i>ONCALL_SMS_AUTOALMSENDBUF_SIZE-1) i=0;
		   oncall_sms_autoalmsendbuf.send_Ptr++;
		   if(oncall_sms_autoalmsendbuf.send_Ptr>=ONCALL_SMS_AUTOALMSENDBUF_SIZE)
		   {
			   oncall_sms_autoalmsendbuf.send_Ptr=0;
			   Sleep(1000);//防止死循环
			   break;
		   }
		   
	   } 

	   if(si>=0)
			  _oncallL_sms_send_tempbufs[idx].use_over=1;
	   aotu_alm_send_locked=0;
}

void sendONCALL_SMS_MANUALMSENDBUF(int idx)
{
	   int		i, j, len,len1,k,si;
	   char	NumStr[20];
	   unsigned char strText[BUFSIZE], send_msg[BUFSIZE],tempsmssendbuf[BUFSIZE];

	   si=99;
	   if(oncall_sms_manualmsendbuf.send_Ptr==oncall_sms_manualmsendbuf.add_Ptr)
	   {
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   manu_alm_send_locked=0;
		   return;
	   }
	   for(k=0,i=oncall_sms_manualmsendbuf.send_Ptr;i!=oncall_sms_manualmsendbuf.add_Ptr;i++)//如果不等，检查是否有确认或取消，如果无则返回，不执行串口相关操作
	   { 
		   if(oncall_sms_manualmsendbuf.add_Ptr<0)break;
		   if(oncall_sms_manualmsendbuf.add_Ptr>ONCALL_SMS_MANUALMSENDBUF_SIZE-1)break;
		   if(i<0)break;
		   if(i>ONCALL_SMS_MANUALMSENDBUF_SIZE-1)break;
		   if(oncall_sms_manualmsendbuf.confirmed[i]>0)k++;//有确认/取消 过 需要发送的短信
	   }
	   if(k<1)
	   {
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		    manu_alm_send_locked=0;
		   return;
	   }
	   strcpy(tempsmssendbuf,"AT");  tempsmssendbuf[2]=13; tempsmssendbuf[3]=10; tempsmssendbuf[4]='\0';
	   if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,5,&len,NULL) )
	   {	
		   char temp_strs[400];
		   sprintf(temp_strs,"sendONCALL_SMS_MANUALMSENDBUF() WriteFile 'AT' error index=%d",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);

		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
		   //printf("\nONCALL: sendONCALL_SMS_MANUALMSENDBUF() WriteFile 'AT' error !!!\n"); 
		   ClearSendCmdFault(idx); 
		   _SMS_SENDRECEIVE_COM_STATS[idx]=0;////20101118
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		    manu_alm_send_locked=0;
		   return;
	   }
	   Sleep(200); 
	   ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,400,&len,NULL);
	   if(len<1)
	   {
		   
		   char temp_strs[400];
		   sprintf(temp_strs,"处理手动报警短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);


		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
		   //if(printflevel>0) printf("\n处理手动报警短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常!!!\n",idx); 
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		    manu_alm_send_locked=0;
		   return;
	   }
	   strcpy(tempsmssendbuf,"AT+CMGF=0"); tempsmssendbuf[9]=13; tempsmssendbuf[10]=10; tempsmssendbuf[11]='\0';//Set PDU mode 
	   if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,11,&len,NULL) )
	   {	
		   char temp_strs[400];
		   sprintf(temp_strs,"sendoncall_sms_manualmsendbuf() WriteFile 'AT+CMGF=0' error,index=%d",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);

		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
		   //printf("\nONCALL: sendoncall_sms_manualmsendbuf() WriteFile 'AT+CMGF=0' error !!!\n"); 
		   ClearSendCmdFault(idx); 
		   Sleep(100); 
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   manu_alm_send_locked=0;
		   return;
	   }
	   ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,BUFSIZE,&len,NULL);
	//   strcpy(SmsCenter,_strSmsCenterS[idx]); 
	   memset(send_msg,0,sizeof(send_msg));
	  
	   for(i=oncall_sms_manualmsendbuf.send_Ptr;i!=oncall_sms_manualmsendbuf.add_Ptr;i++)
	   {    
		   if(i>ONCALL_SMS_MANUALMSENDBUF_SIZE-1) i=0;
		   if(oncall_sms_manualmsendbuf.canceled==1)
		   {
			   oncall_sms_manualmsendbuf.canceled=0;
			   break;
		   }
		   if(oncall_sms_manualmsendbuf.confirmed[i]==2)
		   {
			   oncall_sms_manualmsendbuf.send_Ptr++;
			   if(oncall_sms_manualmsendbuf.send_Ptr>=ONCALL_SMS_MANUALMSENDBUF_SIZE)
			   {
				   oncall_sms_manualmsendbuf.send_Ptr=0;
				   Sleep(1000);//防止死循环
				   break;
			   }
			   oncall_sms_manualmsendbuf.confirmed[i]=0;
			   continue;
		   }
		   if(oncall_sms_manualmsendbuf.confirmed[i]!=1)break;//未确认，取消执行
		   memset(strText,0,sizeof(strText));
		   memset(send_msg,0,sizeof(send_msg));
		   strcpy(send_msg,oncall_sms_manualmsendbuf.time_str[i]);
		   strcat(send_msg," ");
		   strcat(send_msg,oncall_sms_manualmsendbuf.longname[i]);
		   strcat(send_msg," ");
		   strcat(send_msg,oncall_sms_manualmsendbuf.message[i]);
		   strcpy(strText,send_msg);
		   len=AsciiToUnicode( strText );
		   len1=strlen(strText );
		   if(len>=0)
		   {	
			   for(j=0;j<TELPHONE_NUM;j++)
			   { 	  
				   if(oncall_sms_manualmsendbuf.canceled==1)
				   {
					   oncall_sms_manualmsendbuf.canceled=0;
					   break;
				   }
				   if(oncall_sms_manualmsendbuf.telnum[i][j][0]=='\0')
					   continue;
				   strcpy(NumStr,oncall_sms_manualmsendbuf.telnum[i][j]);



				   
				    
				   

				   si=8;
				   for(si=0;si<MAX_sms_dev_num;si++)
				   {
					   if(_oncallL_sms_send_tempbufs[si].use_over==1 && _ONCALLCOM_PORTS[si][0]!=0 && si!=idx)
					   {
						   char temp_strs[400];
						   sprintf(temp_strs,"SMS_MANUALMSENDBUF thread,index=%d",si);	
						   insert_sms_logs(idx,temp_strs);
						   //printf("\nOnCall:%s",temp_strs);

						    //printf("\nSMS_MANUALMSENDBUF thread index=%d\n",si);
						   _oncallL_sms_send_tempbufs[si].use_over=0;
						   _oncallL_sms_send_tempbufs[si].len=len;
						   _oncallL_sms_send_tempbufs[si].len1=len1;
						   strcpy(_oncallL_sms_send_tempbufs[si].NumStr,NumStr);
						   strcpy(_oncallL_sms_send_tempbufs[si].strText,strText);
						   strcpy(_oncallL_sms_send_tempbufs[si].send_msg,send_msg);
						   _oncallL_sms_send_tempbufs[si].rTONNPI0=defaultTONNPI[0];
						   _oncallL_sms_send_tempbufs[si].rTONNPI1=defaultTONNPI[1];
						   _oncallL_sms_send_tempbufs[si].use_sc=0;
						   //creatthread......... 
						   
						   
						   if(_ONCALL_SMS_DEV_TYPES[si]==2)
						   {  
							   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoCDMA,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							   {
								   char temp_strs[400];
								   sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								   write_main_logs_now(temp_strs,1);
								   
								   //printf("\nOnCall:%s",temp_strs);
								   
								exit(-1);
							   }
						   }
						   else
						   { 
							   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoGSM,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							   {
								   char temp_strs[400];
								   sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								   write_main_logs_now(temp_strs,1);
								   
								   //printf("\nOnCall:%s",temp_strs);
								   
								exit(-1);
							   }
						   }
						   
						   
						   si=-1;
						   
						   break;
					   }
				   }
				   if(si<0)
				   {
					   si=9;
					   //break;
				   }
				   else
				   {
					   char temp_strs[400];
					   sprintf(temp_strs,"SMS_MANUALMSENDBUF self index=%d",idx);	
					   insert_sms_logs(idx,temp_strs);
					   //printf("\nOnCall:%s",temp_strs);


					   //printf("\nSMS_MANUALMSENDBUF self index=%d\n",idx);
					   _oncallL_sms_send_tempbufs[idx].use_over=-1;
					   _oncallL_sms_send_tempbufs[idx].use_sc=0;
					   _oncallL_sms_send_tempbufs[idx].len=len;
					   _oncallL_sms_send_tempbufs[idx].len1=len1;
					   strcpy(_oncallL_sms_send_tempbufs[idx].NumStr,NumStr);
					   strcpy(_oncallL_sms_send_tempbufs[idx].strText,strText);
					   strcpy(_oncallL_sms_send_tempbufs[idx].send_msg,send_msg);
					   _oncallL_sms_send_tempbufs[idx].rTONNPI0=defaultTONNPI[0];
						   _oncallL_sms_send_tempbufs[idx].rTONNPI1=defaultTONNPI[1];
					   if(_ONCALL_SMS_DEV_TYPES[idx]==2)
					   { 
						   SendMessagetoCDMA(idx);
					   }
					   else
					   {
						   SendMessagetoGSM(idx);
					   }
				   }
				  


				   /*if(ONCALL_SMS_DEV_TYPE==2)
					   real =SendMessagetoCDMA(len, len1, NumStr, strText, send_msg,defaultTONNPI[0],defaultTONNPI[1]);
				   else
					   real =SendMessagetoGSM(len, len1, NumStr, strText, send_msg,defaultTONNPI[0],defaultTONNPI[1]);*/
			   }
		   }
		   if(1==1)
		   {
			   char temp_strs[400];
			   sprintf(temp_strs,"手动报警短信[%d]处理完成, sms dev index=%d",i,idx);	
			   insert_sms_logs(idx,temp_strs);
			   //printf("\nOnCall:%s",temp_strs);
			   
		   }
		   //printf("\nONCALL: 手动报警短信[%d]处理完成!\n",i);
		   oncall_sms_manualmsendbuf.confirmed[i]=0;

		   oncall_sms_manualmsendbuf.send_Ptr++;
		   if(oncall_sms_manualmsendbuf.send_Ptr>=ONCALL_SMS_MANUALMSENDBUF_SIZE)
		   {
			   oncall_sms_manualmsendbuf.send_Ptr=0;
			   Sleep(1000);//防止死循环
			   break;
		   }
		  
	   } 		
	   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
	    manu_alm_send_locked=0;
}

void sendONCALL_SMS_MANUSENDBUF(int idx)
{
	   int		i, j, len,len1 ,si;
	   char	NumStr[20];
	   unsigned char strText[BUFSIZE], send_msg[BUFSIZE],tempsmssendbuf[BUFSIZE];
	   si=99;

	   if(oncall_sms_manusendbuf.send_Ptr==oncall_sms_manusendbuf.add_Ptr)
	   { 
		   //printf("\nONCALL: 无定时短信!\n");
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   manu_send_locked=0;
		   return;
	   }
	   
	   strcpy(tempsmssendbuf,"AT");  tempsmssendbuf[2]=13; tempsmssendbuf[3]=10; tempsmssendbuf[4]='\0';
	   if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,5,&len,NULL) )
	   {
		   char temp_strs[400];
		   sprintf(temp_strs,"sendONCALL_SMS_MANUSENDBUF() WriteFile 'AT' error, index=%d",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);

		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
		   //printf("\nONCALL: sendONCALL_SMS_MANUSENDBUF() WriteFile 'AT' error !!!\n"); 
		   ClearSendCmdFault(idx); 
		   _SMS_SENDRECEIVE_COM_STATS[idx]=0;////20101118
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   manu_send_locked=0;
		   return;
	   }
	   Sleep(200); 
	   ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,400,&len,NULL);
	   if(len<1) 
	   {
		   char temp_strs[400];
		   sprintf(temp_strs,"处理手动短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);


		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
		   //if(printflevel>0) printf("\n处理手动短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常!!!\n",idx);
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   manu_send_locked=0;
		   return;
	   }
	   strcpy(tempsmssendbuf,"AT+CMGF=0"); tempsmssendbuf[9]=13; tempsmssendbuf[10]=10; tempsmssendbuf[11]='\0';//Set PDU mode 
	   if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,11,&len,NULL) )
	   {	
		   char temp_strs[400];
		   sprintf(temp_strs,"sendoncall_sms_manusendbuf() WriteFile 'AT+CMGF=0' error,index=%d",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);


		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
		   //printf("\nONCALL: sendoncall_sms_manusendbuf() WriteFile 'AT+CMGF=0' error !!!\n"); 
		   ClearSendCmdFault(idx); 
		   Sleep(100); 
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   manu_send_locked=0;
		   return;
	   }
	   ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,BUFSIZE,&len,NULL);
	 //  strcpy(SmsCenter,_strSmsCenterS[idx]); 
	  
	   for(i=oncall_sms_manusendbuf.send_Ptr;i!=oncall_sms_manusendbuf.add_Ptr;)
	   {    		 
		   if(oncall_sms_manusendbuf.canceled==1)
		   {
			   oncall_sms_manusendbuf.canceled=0;
			   break;
		   }
		   memset(strText,0,sizeof(strText));
		   memset(send_msg,0,sizeof(send_msg));
		   strcpy(send_msg,oncall_sms_manusendbuf.message[i]);
		   strcpy(strText,send_msg);
		   len=AsciiToUnicode( strText );
		   len1=strlen(strText );
		   if(len>=0)
		   {	
			   for(j=0;j<TELPHONE_NUM;j++)
			   { 	  
				   if(oncall_sms_manusendbuf.canceled==1)
				   {
					   oncall_sms_manusendbuf.canceled=0;
					   break;
				   }
				   if(oncall_sms_manusendbuf.telnum[i][j][0]=='\0')
					   continue;
				   strcpy(NumStr,oncall_sms_manusendbuf.telnum[i][j]);


				   
				   si=8;
				   for(si=0;si<MAX_sms_dev_num;si++)
				   {
					   if(_oncallL_sms_send_tempbufs[si].use_over==1 && _ONCALLCOM_PORTS[si][0]!=0 && si!=idx)
					   {
						   char temp_strs[400];
						   sprintf(temp_strs,"SMS_MANUSENDBUF thread  index=%d",si);	
						   insert_sms_logs(idx,temp_strs);
						   //printf("\nOnCall:%s",temp_strs);


						   _oncallL_sms_send_tempbufs[si].use_over=0;
						   _oncallL_sms_send_tempbufs[si].len=len;
						   _oncallL_sms_send_tempbufs[si].len1=len1;
						   strcpy(_oncallL_sms_send_tempbufs[si].NumStr,NumStr);
						   strcpy(_oncallL_sms_send_tempbufs[si].strText,strText);
						   strcpy(_oncallL_sms_send_tempbufs[si].send_msg,send_msg);
						   _oncallL_sms_send_tempbufs[si].use_sc=0;
						 
						   _oncallL_sms_send_tempbufs[si].rTONNPI0=defaultTONNPI[0];
						   _oncallL_sms_send_tempbufs[si].rTONNPI1=defaultTONNPI[1];
						   
						   //creatthread......... 
						   
						   //printf("\nSMS_MANUSENDBUF thread index=%d\n",si);
						   if(_ONCALL_SMS_DEV_TYPES[si]==2)
						   {  
							   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoCDMA,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							   {
								   char temp_strs[400];
								   sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								   write_main_logs_now(temp_strs,1);
								   
								   //printf("\nOnCall:%s",temp_strs);
								   
								exit(-1);
							   }
						   }
						   else
						   { 
							   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoGSM,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							   {
								   char temp_strs[400];
								   sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								   write_main_logs_now(temp_strs,1);
								   
								   //printf("\nOnCall:%s",temp_strs);
								   
								exit(-1);
							   }
						   }
						   
						   
						   si=-1;
						   
						   break;
					   }
				   }
				   if(si<0)
				   {
					   si=9;
					   //break;
				   }
				   else
				   {
					   char temp_strs[400];
					   sprintf(temp_strs,"nSMS_MANUSENDBUF self  index=%d",idx);	
					   insert_sms_logs(idx,temp_strs);
					   //printf("\nOnCall:%s",temp_strs);
					   


					   //printf("\nSMS_MANUSENDBUF self index=%d\n",idx);
					   _oncallL_sms_send_tempbufs[idx].use_over=-1;
					   _oncallL_sms_send_tempbufs[idx].len=len;
					   _oncallL_sms_send_tempbufs[idx].len1=len1;
					   strcpy(_oncallL_sms_send_tempbufs[idx].NumStr,NumStr);
					   strcpy(_oncallL_sms_send_tempbufs[idx].strText,strText);
					   strcpy(_oncallL_sms_send_tempbufs[idx].send_msg,send_msg);
					   _oncallL_sms_send_tempbufs[idx].use_sc=0;
					   
					   _oncallL_sms_send_tempbufs[idx].rTONNPI0=defaultTONNPI[0];
					   _oncallL_sms_send_tempbufs[idx].rTONNPI1=defaultTONNPI[1];
						   
					   if(_ONCALL_SMS_DEV_TYPES[idx]==2)
					   { 
						   SendMessagetoCDMA(idx);
					   }
					   else
					   {
						   SendMessagetoGSM(idx);
					   }
				   }

				  
				   /*
				   if(ONCALL_SMS_DEV_TYPE==2)
					   real =SendMessagetoCDMA(len, len1, NumStr, strText, send_msg,defaultTONNPI[0],defaultTONNPI[1]);
				   else
					   real =SendMessagetoGSM(len, len1, NumStr, strText, send_msg,defaultTONNPI[0],defaultTONNPI[1]);*/
			   }
		   }
		   if(1==1)
		   {
			   char temp_strs[400];
			   sprintf(temp_strs,"手动短信[%d]处理完成,  index=%d",i,idx);	
			   insert_sms_logs(idx,temp_strs);
			   //printf("\nOnCall:%s",temp_strs);
			   
	

		   }
		   //printf("\nONCALL: 手动短信[%d]处理完成!\n",i);
		   i++;
	      if(i>ONCALL_SMS_MANUSENDBUF_SIZE-1) i=0;
		    oncall_sms_manusendbuf.send_Ptr++;
		   if(oncall_sms_manusendbuf.send_Ptr>=ONCALL_SMS_MANUSENDBUF_SIZE)
		   {
			   oncall_sms_manusendbuf.send_Ptr=0;
			   Sleep(1000);//防止死循环
			   break;
		   }
		   
	   } 
	   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
	   manu_send_locked=0;
}

void sendONCALL_SMS_TIMINGSENDBUF(int idx)
{
	   int		i, j, len,len1,si;
	   char	NumStr[20];
	   unsigned char strText[BUFSIZE], send_msg[BUFSIZE],tempsmssendbuf[BUFSIZE];
	   si=99;
	   if(oncall_sms_timingsendbuf.send_Ptr==oncall_sms_timingsendbuf.add_Ptr)
	   { 
		   //printf("\nONCALL: 无定时短信!\n");
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		   timing_send_locked=0;
		   return;
	   }
	   
	   
	   strcpy(tempsmssendbuf,"AT");  tempsmssendbuf[2]=13; tempsmssendbuf[3]=10; tempsmssendbuf[4]='\0';
	   if(!WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,5,&len,NULL) )
	   {	
		   char temp_strs[400];
		   sprintf(temp_strs,"sendONCALL_SMS_TIMINGSENDBUF() WriteFile 'AT' error  index=%d",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);
		   

		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
		   //printf("\nONCALL: sendONCALL_SMS_TIMINGSENDBUF() WriteFile 'AT' error !!!\n"); 
		   ClearSendCmdFault(idx); 
		   _SMS_SENDRECEIVE_COM_STATS[idx]=0;////20101118
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		    timing_send_locked=0;
		   return;
	   }
	   Sleep(200); 
	   ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,400,&len,NULL);
	   if(len<1)
	   {
		   char temp_strs[400];
		   sprintf(temp_strs,"处理定时短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);
		   


		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]+=3;//20120327
		  // if(printflevel>0) 
		//	   printf("\n处理定时短信返回异常 index=%d [无返回数据]，请检查短信装置是否正常!!!\n",idx); 
		   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
		    timing_send_locked=0;
		   return;
	   }
	   strcpy(tempsmssendbuf,"AT+CMGF=0"); tempsmssendbuf[9]=13; tempsmssendbuf[10]=10; tempsmssendbuf[11]='\0';//Set PDU mode 
	   if( !WriteFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,11,&len,NULL) )
	   {	
		   char temp_strs[400];
		   sprintf(temp_strs," sendoncall_sms_timingsendbuf() WriteFile 'AT+CMGF=0' error, index=%d",idx);	
		   insert_sms_logs(idx,temp_strs);
		   //printf("\nOnCall:%s",temp_strs);
		   

		   if(lock_oncall_sms_err_times==0)oncall_sms_err_times[idx]++;//20120327
		  // printf("\nONCALL: sendoncall_sms_timingsendbuf() WriteFile 'AT+CMGF=0' error !!!\n"); 
		   ClearSendCmdFault(idx); 
		   Sleep(100); 
	   }
	   ReadFile(_SMS_SENDRECEIVE_COMS[idx],tempsmssendbuf,BUFSIZE,&len,NULL);
//	   strcpy(SmsCenter,_strSmsCenterS[idx]); 
	 
	   for(i=oncall_sms_timingsendbuf.send_Ptr;i!=oncall_sms_timingsendbuf.add_Ptr;)
	   {    		
		   if(oncall_sms_timingsendbuf.canceled==1)
		   {
			   oncall_sms_timingsendbuf.canceled=0;
			   break;
		   }
		   memset(strText,0,sizeof(strText));
		   memset(send_msg,0,sizeof(send_msg));
		   strcpy(send_msg,oncall_sms_timingsendbuf.time_str[i]);
		   strcat(send_msg," ");
		   //strcat(send_msg,oncall_sms_timingsendbuf.longname[i]);
		  //strcat(send_msg," [");
//		   strcat(send_msg,oncall_sms_timingsendbuf.logicname[i]);
		// strcat(send_msg,"] ");
		   strcat(send_msg,oncall_sms_timingsendbuf.message[i]);
		   strcpy(strText,send_msg);
		   len=AsciiToUnicode( strText );
		   len1=strlen(strText );
		   if(len>=0)
		   {	
			   for(j=0;j<TELPHONE_NUM;j++)
			   { 	  
				   if(oncall_sms_timingsendbuf.canceled==1)
				   {
					   oncall_sms_timingsendbuf.canceled=0;
					   break;
				   }
				   if(oncall_sms_timingsendbuf.telnum[i][j][0]=='\0')
					   continue;
				   strcpy(NumStr,oncall_sms_timingsendbuf.telnum[i][j]);
				   
				   si=8;
				   for(si=0;si<MAX_sms_dev_num;si++)
				   {
					   if(_oncallL_sms_send_tempbufs[si].use_over==1 && _ONCALLCOM_PORTS[si][0]!=0 && si!=idx)
					   {
						   char temp_strs[400];
						   sprintf(temp_strs,"SMS_TIMINGSENDBUF thread index=%d",si);	
						   insert_sms_logs(idx,temp_strs);
						   //printf("\nOnCall:%s",temp_strs);
		   

						   _oncallL_sms_send_tempbufs[si].use_over=0;
						   _oncallL_sms_send_tempbufs[si].len=len;
						   _oncallL_sms_send_tempbufs[si].len1=len1;
						   strcpy(_oncallL_sms_send_tempbufs[si].NumStr,NumStr);
						   strcpy(_oncallL_sms_send_tempbufs[si].strText,strText);
						   strcpy(_oncallL_sms_send_tempbufs[si].send_msg,send_msg);
						   _oncallL_sms_send_tempbufs[si].rTONNPI0=defaultTONNPI[0];
						   _oncallL_sms_send_tempbufs[si].rTONNPI1=defaultTONNPI[1];
						   _oncallL_sms_send_tempbufs[si].use_sc=0;
						   //creatthread......... 
						   
						   // printf("\nSMS_TIMINGSENDBUF thread index=%d\n",si);
						   if(_ONCALL_SMS_DEV_TYPES[si]==2)
						   {  
							   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoCDMA,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							   {
								   char temp_strs[400];
								   sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								   write_main_logs_now(temp_strs,1);
								   
								   //printf("\nOnCall:%s",temp_strs);
								   
								exit(-1);
							   }
						   }
						   else
						   { 
							   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMessagetoGSM,(LPVOID)si,0,&Oncall_sms_send_HANDLE_IDThreadS[si])==NULL) 					
							   {
								   char temp_strs[400];
								   sprintf(temp_strs,"Create Oncall_sms_send_HANDLE %d Thread() error",si);	
								   write_main_logs_now(temp_strs,1);
								   
								   //printf("\nOnCall:%s",temp_strs);
								   
								   exit(-1);
							   }
						   }
						   
						   
						   si=-1;
						   
						   break;
					   }
				   }
				   if(si<0)
				   {
					   si=9;
					   //break;
				   }
				   else
				   {
					   char temp_strs[400];
					   sprintf(temp_strs,"SMS_TIMINGSENDBUF self index=%d",idx);	
					   insert_sms_logs(idx,temp_strs);
					   //printf("\nOnCall:%s",temp_strs);
		   

					   // printf("\nSMS_TIMINGSENDBUF self index=%d\n",idx);
					   _oncallL_sms_send_tempbufs[idx].use_over=-1;
					   _oncallL_sms_send_tempbufs[idx].len=len;
					   _oncallL_sms_send_tempbufs[idx].len1=len1;
					   strcpy(_oncallL_sms_send_tempbufs[idx].NumStr,NumStr);
					   strcpy(_oncallL_sms_send_tempbufs[idx].strText,strText);
					   strcpy(_oncallL_sms_send_tempbufs[idx].send_msg,send_msg);
					   _oncallL_sms_send_tempbufs[idx].rTONNPI0=defaultTONNPI[0];
						   _oncallL_sms_send_tempbufs[idx].rTONNPI1=defaultTONNPI[1];
						   _oncallL_sms_send_tempbufs[idx].use_sc=0;
					   if(_ONCALL_SMS_DEV_TYPES[idx]==2)
					   { 
						   SendMessagetoCDMA(idx);
					   }
					   else
					   {
						   SendMessagetoGSM(idx);
					   }
				   }
				  /* if(ONCALL_SMS_DEV_TYPE==2)
					   real =SendMessagetoCDMA(len, len1, NumStr, strText, send_msg,defaultTONNPI[0],defaultTONNPI[1]);
				   else
					   real =SendMessagetoGSM(len, len1, NumStr, strText, send_msg,defaultTONNPI[0],defaultTONNPI[1]);
				   */
				   
				   //if(real>=0) 
					 //  printf("\nONCALL: 定时短信[%d,%d]处理完成!\n",i,j);
				  // else 
				//	   printf("\nONCALL: 定时短信[%d,%d]处理完成!\n",i,j);

			   }
		   }
		   
		   i++;
	  	   if(i>ONCALL_SMS_TIMINGSENDBUF_SIZE-1) i=0;
 		   oncall_sms_timingsendbuf.send_Ptr++;
		   if(oncall_sms_timingsendbuf.send_Ptr>=ONCALL_SMS_TIMINGSENDBUF_SIZE)
		   {
			   oncall_sms_timingsendbuf.send_Ptr=0;
			   Sleep(1000);//防止死循环
			   break;
		   }
		  
	   } 
	   if(si>=0)
			   _oncallL_sms_send_tempbufs[idx].use_over=1;
	    timing_send_locked=0;
}

