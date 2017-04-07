#include	<time.h>
#include	<stdlib.h>
#include	<stdio.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"
#include	"../inc/sniffer.h"
#include	"../inc/vip_cmd_def.h"


#ifndef  WINDOWS_OS
#define	Sleep(x)	usleep(1000*x)
#endif



void*	InitIPC(char key_char);
int		InitPtTable(void);
void	PrintRecvData(unsigned char *data, int length);
int		SEND_VIP(SOCKET sock, VIP_CONFIG *vip_config);
SOCKET	InitSocket(VIP_CONFIG *vip_config);
int		GetVIPConf(VIP_CONFIG *vip_config);



#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/dpslib.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(lib, "../../../lan/v4.0a/lib/lan.lib")


#define CONFIG_DEFINE_FILE	"/home/ems/h9000/def/vip_cmd_config.def"


T_VIP_R	VIP_Send;


int	main(int argc, char *argv[])
{
	SOCKET	send_sock= INVALID_SOCKET;
	VIP_CONFIG vip_config;
	int		i=0;


	SetModuleName("VIP_CMD");
	SetErrLogMode(argc, argv);
	CreateLockFile(".vip_cmd.lock");

	if(-1==MountDB(NULL))
	{
		Err_log("Fail to MountDB!\n");
		exit(-1);
	}
	
	if(-1==lan_out_init())
	{
		Err_log("Fail to lan_out_init!\n");
		exit(-1);
	}
	
// 	if((SHM_SNIFFER*)-1==(vip_data_seg=(SHM_SNIFFER*)LinktoShareMem(SHM_SNIFFER_VIP_KEY)))
// 	{
// 		Err_log("Fail to get VIP recv data seg shm!\n");
// 		exit(-1);
// 	}
// 	else
// 		Err_log("Success to get VIP recv data seg shm!\n");
	
	
	if(-1==GetVIPConf(&vip_config))
	{
		Err_log("Fail to init vip config!\n");
		exit(-1);
	}
	else
		Err_log("Success to init vip config!\n");

	Err_log("Start to cmd process\n");

	for(;;)
	{
		//add cmd monitor code


		//VIP_Send.yt_flag= i++;
		//VIP_Send.R_VALUE[0]= i+0.50;
		
		if(INVALID_SOCKET==send_sock)
		{
			if(INVALID_SOCKET==(send_sock=InitSocket(&vip_config)))
			{
				Err_log("Fail to create socket!\n");
				Sleep(1000);
				continue;
			}
		}

		if(INVALID_SOCKET!=send_sock)
		{
			if(-1==SEND_VIP(send_sock, &vip_config))
			{
				closesocket(send_sock);
				send_sock= INVALID_SOCKET;
			}
		}

		Sleep(100);

	} //for(;;)
}


void	PrintRecvData(unsigned char *data, int length)
{
	int d;

	printf("\n");
	for(d=1;d<=length;d++)
	{
		printf("%02x ",data[d-1]);
		if((d%10)==0)
			printf("\n");
	}
	printf("\n");
}

int InitPtTable(void)
{

	return 0;
}

SOCKET InitSocket(VIP_CONFIG *vip_config)
{
	int on, ret, len;
	SOCKET	sock;
	SOCKADDR_IN	vip_dest_addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);  //SOCK_DGRAM
	if (sock == INVALID_SOCKET) 
		return INVALID_SOCKET;

	on=1;
	if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(int)))
	{
		SendTraceMsg(0,0,"Fail to set socket SO_KEEPALIVE");
		closesocket(sock);
		return INVALID_SOCKET;
	}

	len= 0;
	if(setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(int)))
	{
		SendTraceMsg(0,0,"Fail to set socket SNDBUF size 0");
		closesocket(sock);
		return INVALID_SOCKET;
	}

	vip_dest_addr.sin_addr.s_addr	=inet_addr(vip_config->ipaddr);
	vip_dest_addr.sin_family		= AF_INET;
	vip_dest_addr.sin_port			= htons(vip_config->port);
	
	ret=connect(sock, (struct sockaddr*)&(vip_dest_addr), sizeof(vip_dest_addr)); 
	if (ret == SOCKET_ERROR)
	{
		SendTraceMsg(0,0,"Fail to connect to %s %d", vip_config->ipaddr, vip_config->port);
		closesocket(sock);
		return INVALID_SOCKET;
	}

	return sock;
}

int SEND_VIP(SOCKET sock, VIP_CONFIG *vip_config)
{
	int	i, ret;

	VIP_Send.len= sizeof(VIP_Send);
	VIP_Send.msg_id= vip_config->msg_id;

	VIP_Send.len= htons(VIP_Send.len);
	VIP_Send.msg_id= htons(VIP_Send.msg_id);
	VIP_Send.yt_flag= htons(VIP_Send.yt_flag);
	VIP_Send.yk_flag= htons(VIP_Send.yk_flag);

	for(i=0; i<IL_DATA_NUM; i++)
		VIP_Send.IL_VALUE[i]= htonl(VIP_Send.IL_VALUE[i]);

	for(i=0; i<R_DATA_NUM; i++)
	{
		UINT	tmp;

		memcpy(&tmp, &VIP_Send.R_VALUE[i], 4);
		tmp= htonl(tmp);
		memcpy(&VIP_Send.R_VALUE[i], &tmp, 4);
	}

	PrintRecvData((unsigned char*)&VIP_Send, sizeof(VIP_Send));

	ret= send(sock, (char*)&VIP_Send, sizeof(VIP_Send), 0);
	if(ret != sizeof(VIP_Send))
	{
		SendTraceMsg(0,0,"Fail to send VIP-R message: len= %d, ret= %d", 
			sizeof(VIP_Send), ret);
		return -1;
	}

	SendTraceMsg(0,0,"Success to send VIP-R message: len= %d", sizeof(VIP_Send));

	return 0;
}



int GetProfString(char* szFileName, char* szSectionName, char* szKeyName, char* szString)
{
	FILE	*fp;
	char	str[256];
	char	strbuf[MAX_STR_NUM_IN_LINE][MAX_CHAR_NUM_IN_LINE];
	int		str_num;
	
	if((fp=fopen(szFileName, "r"))==NULL)
	{
		SendTraceMsg(0,0,"Fail to open %s", szFileName);
		return FALSE;
	}
	
	if(-1==LocateKeyword(fp, szSectionName))
	{
		SendTraceMsg(0,0,"Fail to find [%s] section", szSectionName);
		fclose(fp);
		return FALSE;
	}
	
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			SendTraceMsg(0,0,"Format error in [%s]: %s", szSectionName, str);
			break;
		}
		
		if(str_num==0)
			continue;
		
		if(str_num!=2)
		{
			SendTraceMsg(0,0,"Format error in [%s]: %s", szSectionName, str);
			break;
		}
		
		if(!strcmp(strbuf[0], szKeyName))
		{
			strcpy(szString, strbuf[1]);
			fclose(fp);
			return TRUE;
		}
	}
	
	fclose(fp);
	return FALSE;	
}


int GetProfInt(char* szFileName, char* szSectionName, char* szKeyName, int* iValue)
{
	char	buff[256];

	if(GetProfString(szFileName, szSectionName, szKeyName, buff))
	{
		if(1==sscanf(buff, "%d", iValue))
			return TRUE;
		else
			SendTraceMsg(0,0,"Format error in [%s]: %s", szSectionName, szKeyName);
	}
	return FALSE;
}


int GetVIPConf(VIP_CONFIG *vip_config)
{
	char	*filename=CONFIG_DEFINE_FILE;
	char	string[256];
	char	*sect_name= "VIP_CMD";
	int		value;
	
	if(!GetProfInt(filename, sect_name, "PORT", &value))
		return -1;
	vip_config->port= value;
	
	if(!GetProfInt(filename, sect_name, "MSG_ID", &value))
		return -1;
	vip_config->msg_id= value;
	
	if(!GetProfString(filename, sect_name, "IPADDR", string))
		return -1;

	if(-1==inet_addr(string))
	{ 
		SendTraceMsg(0,0,"vip host ipaddr %s error in %s", string);
		return -1;
	}
	strcpy(vip_config->ipaddr, string);
	return 0;
}
