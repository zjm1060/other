
#include        <stdio.h>

#include	"/home/ems/system/dms/v4.0a/inc/os_type_def.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_com.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_functions.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_util_def.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_structure_def.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_text_def.h"
#include	"/home/ems/system/dms/v4.0a/inc/dms_std.h"
#include    "../../../telalm/v4.0a/inc/tele.h"
#define	    DBNAME_LEN			48
#define		TELPHONE_NUM		1000    // yujji 0902
#define		ALARM_NUM			1024    // yujji 090327
#define		TIMING_NUM			20      // yujji 090331
#define		TELINQUIRY_NUM		100      // yujji 090331
#define		SMSINQUIRY_NUM		100      // yujji 090331

#define		SUCCESS				1
#define		FAILURE				-1
#define		TIMEOUT_SECONDS		2
#define		BUFSIZE				8000
#define		DIAL_OK				"success"
#define		DIAL_ERR			"fail"
#define		DIAL_RECORD_PATH	"\\home\\ems\\h9000\\his\\del\\"
//#define		DIAL_RECORD_SAVEDAY	20  //拨号记录保留的天数

#define BUFF_SIZE 1024

char    *ioattr_name[16] ={ "RELAY","BREAKER","VALVE","DEVICE","SWITCH","","",
							"GENSTA","DEVMSG","STREAM","NETMSG","HOSTMSG","LOGMSG","ONOFFMSG","AGCCMD",""};
char	manualsms[1024];  //yujji 090331
HANDLE  hMutex;
HANDLE 	hCom;
unsigned char	buf[BUFSIZE];
#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(lib, "../../../lan/v4.0a/lib/lan.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")

// yujji 090331
int		PortInit();	
void	ClearRecvFault(),	ClearSendCmdFault();
//int		ReadSmsDefineXml(void); chenxs 20101112 取消
//int		ReadAlmDefXml(void); chenxs 20101112 取消
//int		ReadTimSendXml(void); chenxs 20101112 取消
//int		ReadTelInquiryXml(void); chenxs 20101112 取消
//int		ReadSmsInquiryXml(void); chenxs 20101112 取消
//int		ReadSmsManualXml(void); chenxs 20101112 取消
int     UnicodeToAscii( const unsigned char* pSrc, char* pDst, int nSrcLength );// unicode-ascii
int		UTF8ToUnicode ( const unsigned char* str , char* destr );   // utf8-unicode
void	DialRecord( char *textstr, char *PhoneNumber, char *Status );

void	BlankFillCopy (dst,src,len)
char    *dst;
char    *src;
int     len;
{
	while ( *src && len-- > 1 )
		*(dst++) = *(src++);

	while (len-- > 1)
		*(dst++) = ' ';
	*dst = '\0';
}

void DellBlank (dst,src,len)
char    *dst;
char    *src;
int     len;
{
	while ( *src && len-- > 1 )
	{
		if( *src != ' ' && *src != '\t' ) { *(dst++) = *src; }
		src++;
	}
	*dst = '\0';
}

int main(int argc, char *argv[])
{
	char   	temp[128];
	char	SmsCenter[20], strText[1024], strDesTel[20], NumStr[20], strSmsTel[20], strMsgBuf[BUFSIZE], strTexty[BUFSIZE];
	char    str[4096];
	int     x, y, m, n, q, s;
	FILE	*fp1;

	int		        i, run_type=0, yh_num, len, len1;
	int             cc=0;
	char	strFlag[80];

	argc=1;
    if( argc>1 ) 
		run_type=1; 
	else run_type=0;

    if(-1==MountDB(NULL))
	{
	    printf("\nFail to MountDB!");
		exit(0);
	}
//	yh_num = ReadSmsDefineXml();		//电话号码配置表  chenxs 20101112 取消
//	ReadSmsManualXml();     //手动发送短消息配置表  chenxs 20101112 取消
	memset(manualsms,'\0',1024);
	if( _access(MANUAL_FILE,0) ==-1 )
	{ 
		printf("\nSendBytimeShortMsg() can't open %s!\n",MANUAL_FILE);exit(0);		
	}  
	fp1=fopen(MANUAL_FILE,"r");
	for(;;)
	{
		fscanf(fp1,"%s",strFlag);
		strcat(manualsms, " ");
		strcat(manualsms, strFlag);
		if(feof(fp1))
		{
			printf("Config file %s End!\n",MANUAL_FILE);
			fclose(fp1);
			break;
		}
	}
	memset(temp,'\0',128);
	for(i=0; i < yh_num; i++)
	{
		if( strcmp(telphone_def.manrecv[i],"NO") == 0 ) 
			continue;
		else
		{
			strcat(temp, telphone_def.name[i]);
			strcat(temp, ",");
		}
	}

	if( run_type!=1 )
	{
		printf("\n\n\n是否向 %s 发送短消息?短信内容是:\n<%s> \n\n\n", temp, manualsms);
		printf("\n\n\n按回车键确认发送。。。\n\n\n"); gets(str);
	}
	if( PortInit()!=SUCCESS )
	{
		printf("\nAlmClassGen.exe: open %s error!串口被占用，请稍后再试!\n",COM_PORT);
		return 0;
	}
	else	
		printf("\nAlmClassGen.exe: open %s OK!串口被成功打开，等待发送短消息!\n",COM_PORT);

	strcpy(buf,"AT");  buf[2]=13; buf[3]=10; buf[4]='\0';
	if( !WriteFile(hCom,buf,5,&len,NULL) )
	{	printf("\nAlmClassGen.exe: ManualSendShortMsg() WriteFile 'AT' error !!!\n"); 
		ClearSendCmdFault(); 
		Sleep(100); 
	}
	Sleep(100); 
	ReadFile(hCom,buf,BUFSIZE,&len,NULL);

	strcpy(buf,"AT+CMGF=0"); buf[9]=13; buf[10]=10; buf[11]='\0';//Set PDU mode 
	if( !WriteFile(hCom,buf,11,&len,NULL) )
	{	
		printf("\nAlmClassGen.exe: ManualSendShortMsg() WriteFile 'AT+CMGF=0' error !!!\n"); 
		ClearSendCmdFault(); 
		Sleep(100); 
	}
	ReadFile(hCom,buf,BUFSIZE,&len,NULL);
	
//	strcpy(SmsCenter,"8613800835500"); 
//	strcpy(strSmsTel,"8613800835500"); 
	strcpy(SmsCenter,"8613800100500"); 
	strcpy(strSmsTel,"8613800100500"); 
	CreateMobileNum( strSmsTel );

		for(i=0; i< yh_num; i++)
		{ 	  			   
			if( strcmp(telphone_def.manrecv[i],"YES")==0 )
			{				
				strcpy(strDesTel,telphone_def.telphone[i]);
				strcpy(NumStr,telphone_def.telphone[i]);
				strcat(NumStr,"_MANSMS");
				CreateMobileNum( strDesTel );  //手机号码
			
				strcpy(strText, manualsms); //短信内容
				len=AsciiToUnicode( strText );
				len1=strlen( strText );
				if((len !=-1))
				{
					x = len1/280;
					y = len1%280;
					if (len1==280||len1<280)
					{
						sprintf(strMsgBuf,"0891%s11000B81%s0008A7%02x%s",strSmsTel,strDesTel,len,strText);                       
						sprintf(buf,"AT+CMGS=%03d",8+6+len); 
						buf[11]=13; 
						buf[12]=10; 
						buf[13]='\0';                            // 如果是西门子模块此处需要这样定义,WAVECOM则不需要   
						if( !WriteFile(hCom,buf,13,&len,NULL) )  // 如果是西门子模块此处需要这样定义,WAVECOM则不需要
//						if( !WriteFile(hCom,buf,12,&len,NULL) )
						{
							printf("\nTELALM.EXE: ManualSendShortMsg() WriteFile 'AT+CMGS=<length>' error !!!\n"); 
							ClearSendCmdFault(); 
						} 
						Sleep(100);
						ReadFile(hCom,buf,BUFSIZE,&len,NULL);						
						strcpy(buf,strMsgBuf),
						buf[strlen(strMsgBuf)]   = 26;   //26=ctrl^Z  
						buf[strlen(strMsgBuf)+1] = '\0';					

						if( !WriteFile(hCom,buf,strlen(buf),&len,NULL) )
						{	
							printf("\nTELALM.EXE: ManualSendShortMsg() WriteFile '> <text>' error !!!\n"); 
							ClearSendCmdFault(); 
						}
						Sleep(1000); 					
						ReadFile(hCom,buf,BUFSIZE,&len,NULL);				        											
						memset(buf,0,BUFSIZE);
						ReadFile(hCom,buf,BUFSIZE,&len,NULL);	

//						if( strstr(buf,"ERROR")==NULL )
						if( strstr(buf,"+CMGS:")!=NULL )
						{
							DialRecord(manualsms,NumStr,DIAL_OK); 
							Sleep(100);
							printf("\n发送短消息至<%s>成功!\n",telphone_def.name[i]);												
						}

						else 
						{
							printf(">>>> Send Manual Message to  %s fail !!!\n",NumStr);
							DialRecord(manualsms,NumStr,DIAL_ERR); 
							printf("\n发送短消息至<%s>失败!\n",telphone_def.name[i]);	
							Sleep(100); 
						}
					} // end if (len1==280||len1<280)
					else
					{
						for (m=0;m<x;m++)
						{ 
							for (n=m*280,q=0;n < m*280+280;n++,q++)
							{   
								strTexty[q] = strText[n];
								strTexty[q+1] = '\0' ; 
							}
							len=strlen(strTexty)/2;
							
							sprintf(strMsgBuf,"0891%s11000B81%s0008A7%02x%s",strSmsTel,strDesTel,len,strTexty);                       
							sprintf(buf,"AT+CMGS=%03d",8+6+len); 
							buf[11]=13; 
							buf[12]=10;
							buf[13]='\0';                            // 如果是西门子模块此处需要这样定义,WAVECOM则不需要   
							if( !WriteFile(hCom,buf,13,&len,NULL) )  // 如果是西门子模块此处需要这样定义,WAVECOM则不需要
//							if( !WriteFile(hCom,buf,12,&len,NULL) )
							{	
								printf("\nTELALM.EXE: ReSendShortMsg() WriteFile 'AT+CMGS=<length>' error !!!\n"); 
								ClearSendCmdFault(); 
							} 
							Sleep(1000);
							ReadFile(hCom,buf,BUFSIZE,&len,NULL);						
							strcpy(buf,strMsgBuf),
							buf[strlen(strMsgBuf)]   = 26;   //26=ctrl^Z  
							buf[strlen(strMsgBuf)+1] = '\0';					

							if( !WriteFile(hCom,buf,strlen(buf),&len,NULL) )
							{	
								printf("\nTELALM.EXE: ReSendShortMsg() WriteFile '> <text>' error !!!\n"); 
								ClearSendCmdFault(); 
							}
							Sleep(1000); 					
							ReadFile(hCom,buf,BUFSIZE,&len,NULL);				        											
							memset(buf,0,BUFSIZE);
							ReadFile(hCom,buf,BUFSIZE,&len,NULL);			 
							Sleep(500); 
							if( strstr(buf,"ERROR")==NULL )
							{
								DialRecord(manualsms,NumStr,DIAL_OK); 
								Sleep(100); 
								printf("\n发送多条短消息至<%s>成功!\n",telphone_def.name[i]);		
							}
							else 
							{
								printf(">>>> Send Manual Message to %s fail !!!\n",NumStr);
							
								DialRecord(manualsms,NumStr,DIAL_ERR);
								printf("\n发送多条短消息至<%s>失败!\n",telphone_def.name[i]);	
								Sleep(100); 
							}
						}  // end for (m=0;m<x;m++)
						for (n=x*280,q=0;n<len1;n++,q++)
						{   
							strTexty[q] = strText[n];
							strTexty[q+1] = '\0' ; 
						}
						len=(len1-x*280)/2;
						sprintf(strMsgBuf,"0891%s11000B81%s0008A7%02x%s",strSmsTel,strDesTel,len,strTexty);                       
						sprintf(buf,"AT+CMGS=%03d",8+6+len); 
						buf[11]=13; 
						buf[12]=10; 
						buf[13]='\0'; /* Start PDU send  */		  
						if( !WriteFile(hCom,buf,13,&len,NULL) )
//						if( !WriteFile(hCom,buf,12,&len,NULL) )
						{	
							printf("\nTELALM.EXE: ReSendShortMsg() WriteFile 'AT+CMGS=<length>' error !!!\n"); 
							ClearSendCmdFault(); 
						} 
						Sleep(1000);
						ReadFile(hCom,buf,BUFSIZE,&len,NULL);						
						strcpy(buf,strMsgBuf),
						buf[strlen(strMsgBuf)]   = 26;   //26=ctrl^Z  
						buf[strlen(strMsgBuf)+1] = '\0';					

						if( !WriteFile(hCom,buf,strlen(buf),&len,NULL) )
						{	
							printf("\nTELALM.EXE: ReSendShortMsg() WriteFile '> <text>' error !!!\n"); 
							ClearSendCmdFault(); 
						}
						Sleep(500); 					
						ReadFile(hCom,buf,BUFSIZE,&len,NULL);				        											
						memset(buf,0,BUFSIZE);
						ReadFile(hCom,buf,BUFSIZE,&len,NULL);			 
						if( strstr(buf,"ERROR")==NULL )
						{
							DialRecord(manualsms,NumStr,DIAL_OK); 
							Sleep(100);
							printf("\n发送多条短消息至<%s>成功!\n",telphone_def.name[i]);	
						}
						else
						{
							printf(">>>> Send Manual Message to %s fail !!!\n",NumStr);
							DialRecord(manualsms,NumStr,DIAL_ERR);
							printf("\n发送多条短消息至<%s>失败!\n",telphone_def.name[i]);	
							Sleep(100); 
						}
					} //end else len>280;
				} //end if(len=AsciiToUnicode( strText )!=-1);
			}//end if( strcmp(telphone_def.manrecv[i],"YES")==0 )
		} // end for(i=0; i< yh_num; i++)
	
	printf("\n    *** 短消息发送完毕 ***\n\n");
	s = CloseHandle( hCom );	//added by yujji 2010.06.24
	if (s == 0)
		printf("\nTELALM.EXE: close %s error:%d !\n",COM_PORT,s);
	else
		printf("\nTELALM.EXE: close %s ok:%d  !\n",COM_PORT,s);
	return 0;
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
	char *index2= "ss:Index=";			//add by yujji 2010.06.29

	if(!strstr(str, "<Cell"))
		return FALSE;

	strcpy(buff, str);
	ptr= strstr(buff, index);
	if(!ptr)
		ptr= strstr(buff, index1);
	if(ptr)//ptr!=0
	{
		ptr += strlen(index);
		end_ptr= strstr(ptr, "</Data>");
		if(end_ptr)
			*end_ptr= 0;
		if(sscanf(ptr, "%s", strValue)==1)
			return TRUE;
	}
	ptr= strstr(buff, index2);		//add by yujji 2010.06.29
	if(ptr)//ptr!=0					//add by yujji 2010.06.29
		return TRUE;				//add by yujji 2010.06.29
	return FALSE;
	
}


int UnicodeToAscii(const unsigned char* pSrc, char* pDst, int nSrcLength) // unicode-ascii
{
    int nDstLength;        // UNICODE宽字符数目

    //UNICODE串-->字符串
//    nDstLength = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pSrc, nSrcLength/2, pDst, 160, NULL, NULL);
    nDstLength = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pSrc, nSrcLength/2, pDst, 1024, NULL, NULL);
   // 返回目标字符串长度
    return nDstLength;
}

int UTF8ToUnicode( const unsigned char* str ,char*  destr )   // utf8-unicode

{
 int  unicodeLen , len ;
 WCHAR pUnicode[1024];

 unicodeLen = MultiByteToWideChar( CP_UTF8, 0, str, -1, NULL , 0);      
 memset(pUnicode,0,1024);  
 MultiByteToWideChar( CP_UTF8, 0, str ,-1, (LPWSTR)pUnicode, unicodeLen ); 
 len = ( unicodeLen-1 )*2;
 memcpy (destr , pUnicode, len);  
 return len;
}

void UTF8ToAscii(char *str)
{
	unsigned char   temp[256],temp1[256];
	int	len;

	memset(temp, 0, sizeof(temp));
	memset(temp1, 0, sizeof(temp1));
	len = UTF8ToUnicode(str,temp);
	UnicodeToAscii(temp, temp1, len);
	strcpy(str, temp1);
}

/*int ReadSmsDefineXml(void)			//modified by wzg 2010.04.13
{	
	FILE	*fp;
	char	err_str[256];	
	char	keyword[64];
	int		k, q;
	int		i, bm, mz, dh, d1, d2, d3, d4, s1, s2, s3, s4, cx, ds, sd, row;
	char	str[256];
	int		index;


	for(i=0;i<TELPHONE_NUM;i++)    
	{
		strcpy(telphone_def.department[i], "NO");
		strcpy(telphone_def.name[i], "NO");
		strcpy(telphone_def.telphone[i], "NO");
		strcpy(telphone_def.telalmcls1[i], "NO");			
		strcpy(telphone_def.telalmcls2[i], "NO");
		strcpy(telphone_def.telalmcls3[i], "NO");
		strcpy(telphone_def.telalmcls4[i], "NO");
		strcpy(telphone_def.smsalmcls1[i], "NO");			
		strcpy(telphone_def.smsalmcls2[i], "NO");
		strcpy(telphone_def.smsalmcls3[i], "NO");			
		strcpy(telphone_def.smsalmcls4[i], "NO");
		strcpy(telphone_def.smsquery[i], "NO");			
		strcpy(telphone_def.smstiming[i], "NO");
		strcpy(telphone_def.manrecv[i], "NO");
	}

	if( (fp=fopen(SMS_XML_FILE, "r"))==NULL )
	{
		sprintf(err_str, "Fail to open %s", SMS_XML_FILE);
		Err_log(err_str);
		return -1;
	}

	strcpy(keyword, "电话定义");
//	strcpy(keyword, _CS_("TELALM_STR_QUIRY"));
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		UTF8ToAscii(str);
		if( strstr(str, "<Worksheet") && strstr(str, keyword)  )	
			break;
	}


	i=0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		UTF8ToAscii(str);

		if( strstr(str, "</Row>")  )	
			break;

		if(strstr(str, "<Row"))
		{
			i=0;
			continue;
		}

		strcpy(keyword, ">部门<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_DEPARTMENT"))  )	
			bm = i;
		strcpy(keyword, ">姓名<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_NAME")  )  )
			mz = i;
		strcpy(keyword, ">电话号码<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_PHONENUMBER")  )  )
			dh = i;
		strcpy(keyword, ">电话报警1类<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_TELALM1")  )  )
			d1 = i;
		strcpy(keyword, ">电话报警2类<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_TELALM2")  )  )
			d2 = i;
		strcpy(keyword, ">电话报警3类<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_TELALM3")  )  )
			d3 = i;
		strcpy(keyword, ">电话报警4类<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_TELALM4")  )  )
			d4 = i;
		strcpy(keyword, ">短信报警1类<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_SMSALM1")  )  )
			s1 = i;
		strcpy(keyword, ">短信报警2类<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_SMSALM2")  )  )
			s2 = i;
		strcpy(keyword, ">短信报警3类<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_SMSALM3")  )  )
			s3 = i;
		strcpy(keyword, ">短信报警4类<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_SMSALM4")  )  )
			s4 = i;
		strcpy(keyword, ">短信查询权限<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_SMSINQUIER")  )  )
			cx = i;
		strcpy(keyword, ">定时发送权限<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_SMSTIMING")  )  )
			ds = i;
		strcpy(keyword, ">手动发送接收<");
		if( strstr(str, keyword)  )	
//		if( strstr(str, _CS_("TELALM_STR_SMSTIMING")  )  )
			sd = i;
		i++;
	}

	row=0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		char	strValue[256];

		UTF8ToAscii(str);

		if( strstr(str, "</Table>")  )
			break;

		if(strstr(str, "</Row>"))
		{
			row++;
			continue;
		}

		if(strstr(str, "<Row"))
		{
			k=0;
			continue;
		}

		if(GetStrValueFromCellStr(str, strValue))
		{
			q = (strstr(str, "<Data ss:Type=\"String\">")!=0) || (strstr(str, "<Data ss:Type=\"Number\">")!=0);//add by yujji 2010.06.29

			if(GetIntValueByIndexStr(str, "ss:Index", &index))
				k= index-1;

			if( (k==bm) && (q!=0) )
				strcpy(telphone_def.department[row], strValue);
			else if( (k==mz) && (q!=0) )
				strcpy(telphone_def.name[row], strValue);
			else if( (k==dh) && (q!=0) )
				strcpy(telphone_def.telphone[row], strValue);
			else if( (k==d1) && (q!=0) )
				strcpy(telphone_def.telalmcls1[row], strValue);
			else if( (k==d2) && (q!=0) )
				strcpy(telphone_def.telalmcls2[row], strValue);
			else if( (k==d3) && (q!=0) )
				strcpy(telphone_def.telalmcls3[row], strValue);
			else if( (k==d4) && (q!=0) )
				strcpy(telphone_def.telalmcls4[row], strValue);
			else if( (k==s1) && (q!=0) )
				strcpy(telphone_def.smsalmcls1[row], strValue);
			else if( (k==s2) && (q!=0) )
				strcpy(telphone_def.smsalmcls2[row], strValue);
			else if( (k==s3) && (q!=0) )
				strcpy(telphone_def.smsalmcls3[row], strValue);
			else if( (k==s4) && (q!=0) )
				strcpy(telphone_def.smsalmcls4[row], strValue);
			else if( (k==cx) && (q!=0) )
				strcpy(telphone_def.smsquery[row], strValue);
			else if( (k==ds) && (q!=0) )
				strcpy(telphone_def.smstiming[row], strValue);
			else if( (k==sd) && (q!=0) )
				strcpy(telphone_def.manrecv[row], strValue);

		} //if

		k++;		

	} //while( NULL != fgets(str, sizeof(str), fp) )

	for(i=0;i<row;i++)    
	{
		printf("\n%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s", 
		telphone_def.department[i],
		telphone_def.name[i], 
		telphone_def.telphone[i],
		telphone_def.telalmcls1[i],
		telphone_def.telalmcls2[i],
		telphone_def.telalmcls3[i],
		telphone_def.telalmcls4[i],
		telphone_def.smsalmcls1[i],
		telphone_def.smsalmcls2[i],
		telphone_def.smsalmcls3[i],
		telphone_def.smsalmcls4[i],
		telphone_def.smsquery[i],
		telphone_def.smstiming[i],
		telphone_def.manrecv[i]);
	}

	printf("\n电话定义表: total num= %d\n", row);
	fclose(fp);
	return row;
}

int ReadSmsManualXml(void)
{	
	FILE	*fp;
	char	err_str[256];	
	char	keyword[64], keyword1[64];
	int		m, n, len;
	int		i, j, result, result1, result2, column, row;
	unsigned char	str[1024];
	char	*ColumnCount, *RowCount, *strnum;
	char	strbur[1024];
	unsigned char   temp[1024],temp1[1024];

	if( (fp=fopen("\\home\\ems\\h9000\\def\\oncall_define.xml", "r"))==NULL )
	{
		sprintf(err_str, "Fail to open %s", "\\home\\ems\\h9000\\def\\oncall_define.xml");
		Err_log(err_str);
		return -1;
	}
	strcpy(keyword, "手动发送短消息");
//	strcpy(keyword, _CS_("TELALM_STR_QUIRY"));
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		memset(temp,'\0',1024);
		memset(temp1,'\0',1024);
		len = UTF8ToUnicode(str,temp);
		j = UnicodeToAscii(temp, temp1, len);
		strcpy(str, temp1);

		if( strstr(str, keyword)  )	
			break;
	}

	strcpy(keyword, "ExpandedColumnCount");
	strcpy(keyword1, "ExpandedRowCount");
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( ColumnCount = strstr(str, keyword)  )
		{
			result1=ColumnCount-str+21;
			RowCount = strstr(str, keyword1) ;
			result2=RowCount-str+18;
			for(i=result1,j=0;str[i]!='"';i++,j++)    
		       strbur[j]=str[i];
			column = atoi( strbur );

			for(i=result2,j=0;str[i]!='"';i++,j++)    
		       strbur[j]=str[i];
			row = atoi( strbur );
			strcpy(manualsms, "NO");
			break;
		}
	}

	while( NULL != fgets(str, sizeof(str), fp) )
	{

		strcpy(keyword, "<Row ss:AutoFitHeight=\"0\">");
		if( strstr(str, keyword)  )	
			break;
	}
	memset(manualsms,'\0',1024);
	memset(str,'\0',1024);
	for(i=0; i< row;i++)    
	{
		while( NULL != fgets(str, sizeof(str), fp) )
		{
			memset(temp,'\0',1024);
			memset(temp1,'\0',1024);
			len = UTF8ToUnicode(str,temp);
			UnicodeToAscii(temp, temp1, len);
			strcpy(str, temp1);

			if( strstr(str, "</Row>")  )
				break;
			if( strstr(str, "<Row ss:AutoFitHeight=\"0\">")  )
				continue;
			if( strnum = strstr(str, "\"String\">")  )
			{
				memset(strbur,'\0',1024);
				result=strnum-str+9;
				for(m=result,n=0;str[m]!='<';m++,n++)  
					strbur[n]=str[m];
				strbur[n+1]='\0';
				strcat(manualsms, strbur);
			} // end if( strstr(str, "<Cell><Data ss:")  )
		}
		continue ;
		
	} // end for (i=0,j=0; i<row ;i++,j++) 
	
	fclose(fp);
	return row;
}
*/
int PortInit()	   
{ DCB            dcb;
  COMMTIMEOUTS   timeout;
  BOOL           fSuccess;
  
  hCom=CreateFile
  (  COM_PORT,GENERIC_READ | GENERIC_WRITE,
     FILE_SHARE_WRITE | FILE_SHARE_READ,    /* comm device must be opened with exclusive access */
     NULL, /* no security attrs */
     OPEN_EXISTING, /* comm device must use OPEN_EXISTING */
     0,    /* not overlapped I/O */
     NULL  /* hTemplate must be NULL  for comm device */
  );

  if(hCom==INVALID_HANDLE_VALUE)
  { 
	  printf("%s can't open!The reason is :%d!\n",COM_PORT,GetLastError() ); 
	  CloseHandle(hCom);
	  return( FAILURE ); 
  }

  fSuccess=GetCommState(hCom,&dcb);
  if( !fSuccess )
  { printf("can't get %s state!\n",COM_PORT); return( FAILURE ); }
  
  dcb.BaudRate=BAUD_RATE;
  dcb.ByteSize=8;
  dcb.Parity  =ONCALLPARITY_TYPE;
  dcb.StopBits=ONESTOPBIT;
  
  fSuccess=SetCommState(hCom, &dcb);
  if( !fSuccess )
  { printf("can't set %s state!\n",COM_PORT); return( FAILURE ); }
  
  fSuccess=GetCommTimeouts(hCom,&timeout);
  if(!fSuccess)
  { printf("GetCommTimeouts() error !\n"); return( FAILURE ); }
  timeout.ReadIntervalTimeout       = 500;//2000;      /* ms, max time between two bytes received */
  timeout.ReadTotalTimeoutMultiplier= TIMEOUT_SECONDS;         
  timeout.ReadTotalTimeoutConstant  = 1000;//1000*60*1; /* ms */
  
  fSuccess=SetCommTimeouts(hCom,&timeout);
  if(!fSuccess)
  { printf("SetCommTimeouts() error !\n"); return( FAILURE ); }
  printf(">>>> GSM :% s INIT OK.\n",COM_PORT); 
  PurgeComm(hCom,PURGE_RXCLEAR);
  return( SUCCESS );
}

void ClearRecvFault()
{	DWORD	dwErrors;
	COMSTAT	cstat;
	if(!ClearCommError(hCom,&dwErrors,&cstat))
		printf(">>>>>>ReadFile: ClearCommError ERRNO= %d \n",GetLastError());
	if((dwErrors !=0)||(cstat.cbInQue !=0))
		if(PurgeComm(hCom,PURGE_RXCLEAR)==FALSE)
			printf(">>>>>>ClearRecvFault: PurgeComm ERRNO= %d \n",GetLastError());	
}


void ClearSendCmdFault()
{	DWORD	dwErrors;
	COMSTAT	cstat;
	if(ClearCommError(hCom,&dwErrors,&cstat)==FALSE)
		printf("<<<<<<WriteFile: ClearCommError ERRNO= %d \n",GetLastError());	
	if((dwErrors !=0)||(cstat.cbOutQue !=0))
		if(PurgeComm(hCom,PURGE_TXCLEAR)==FALSE)
			printf("<<<<<<ClearSendCmdFault: PurgeComm ERRNO= %d \n",GetLastError());	
}

int AsciiToUnicode( unsigned char *str )
{	int				i,	n,	m,	len;
	unsigned char	strTmp[BUFSIZE], ch[1];
	len = strlen(str); 
	strcpy(strTmp,str);
	for(i=0;i<(BUFSIZE-len-1);i++) strcat(strTmp," ");
	if( (m = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strTmp,  -1, NULL,NULL ))==0 ) return(-1);
	if( MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strTmp, 400, str, m )==0 ) return(-1);
	for(i=0;i<BUFSIZE;i++)
	{ 
		if( str[i+0]==0x20 && str[i+2]==0x20 && str[i+4]==0x20 && str[i+6]==0x20){ n=i; break; }
	}	
	//for(i=0;i<n+6;i++){ printf("%02x ",str[i]); } printf("\n");
	for(i=0;i<(n/2);i++)
	{  ch[0]=str[i*2+0]; str[i*2+0]=str[i*2+1]; str[i*2+1]=ch[0]; }
	//for(i=0;i<n+6;i++){ printf("%02x ",str[i]); } printf("\n");
	strcpy(strTmp,""); 
	for(i=0;i<n;i++){ 
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
	{ ch=str[i*2+0]; str[i*2+0]=str[i*2+1]; str[i*2+1]=ch; }
	str[len-1] = 'F';
	str[len+1] = '\0';
//	printf("new: %s (len=%d)\n",str,strlen(str));
	return( strlen(str) );
}

void DialRecord( char *textstr, char *PhoneNumber, char *Status )
{ 
  FILE          *recfp;
  char 	    	FileName[200];
  struct tm 	*today;
  time_t		ltime;

  ltime=time(NULL);    today=localtime(&ltime);	

  // 删除历史拨号记录文件 (保留"DIAL_RECORD_SAVEDAY"天)
  ltime = ltime - (60*60)*24*DIAL_RECORD_SAVEDAY;
  today=localtime(&ltime);

  sprintf(FileName,"del %s%04d-%02d-%02d.txt", DIAL_RECORD_PATH, 
	    	(*today).tm_year+1900,	(*today).tm_mon+1,	(*today).tm_mday  ); 
	
  if( _access(FileName,0)!=-1 )
  { 
	  printf("DIALALM.EXE: %s\n",FileName); system(FileName);   
  }	
		
  ltime=time(NULL);    today=localtime(&ltime);	
  sprintf( FileName, "%s%04d-%02d-%02d.txt",DIAL_RECORD_PATH,
	     (*today).tm_year+1900, (*today).tm_mon+1, (*today).tm_mday );		
	
  if( (recfp=fopen(FileName,"a+"))==NULL )
  { 
	  printf("\nDIALALM.EXE: can't open(w) the file %s!\n",FileName); return;
  }  
  else
  {   
     fprintf( recfp,"%04d-%02d-%02d %02d:%02d:%02d %25s  dialout===>>> %18s : %6s\n",
	             (*today).tm_year+1900, (*today).tm_mon+1, 				 
				 (*today).tm_mday,      (*today).tm_hour, 
				 (*today).tm_min,       (*today).tm_sec,
				 textstr, PhoneNumber, Status						);	
  }
  
  fclose(recfp);
  return;
}