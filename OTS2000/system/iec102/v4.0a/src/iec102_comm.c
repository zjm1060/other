/*******************************************************************************************************************************
FILENAME:	
FUNCTION:		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, XP

HISTORY:
2008.4.10	start to modify the code for H9000 V4.0		zyf
********************************************************************************************************************************/
/*********iec102 by zyf for ankong to connect SYBASE**********/
/************************change for all************************/
/*程序只编写接收遥测中的正向总有功，反向总有功，正向总无功，反向总无功。
  (积分周期/负荷曲线周期=15分钟)*/
#include "\home\ems\system\iec102\v4.0a\inc\iec102_com.h"

HANDLE			hCom;
unsigned char	RcvBuf[BUFSIZE],	SndBuf[BUFSIZE];
YC				yc[MAX_YC_NUM];
int             YcNum;

void main()
{	
	WSADATA WSAData;	

    SetConsoleTitle ( "IEC102_COM" );             
	if(WSAStartup(MAKEWORD(1,1),&WSAData)!=0)
	{ printf("\n iec102.exe:: WSAStart() err!\n");exit(0); }


	if(-1==MountDB(NULL))
	{
		printf("\n Fail to MountDB! ");
		exit(-1);
	}

	if(lan_out_init() == FALSE)
	{
		printf("\n Fail to lan_out_init()!");
		exit(-1);
	}

	while( ReadDefineFile()!=SUCCESS )
	{
		printf("iec102.EXE: ReadDefineFile ERROR!\n"); exit(-1);
	}
	/*
    while( DatabaseConnect(Ex_dbname, Ex_username, Ex_password, &henv, &hdbc)!=SQL_SUCCESS )
	{ printf("\nAKSD_RCV_RTU: 连接SYBASE库错!!!\n"); Sleep(10000);  } 
	printf("\nAKSD_RCV_RTU: 连接SYBASE库成功!!!"); 
	*/

	hCom=ComInit(COMM_PORT,BAUD_RATE,8,PARITY_TYPE,ONESTOPBIT);
        printf("\n %s init compelete !!!\n\n",COMM_PORT);	
		
	if(PurgeComm(hCom,PURGE_TXABORT|PURGE_RXABORT|
					PURGE_RXCLEAR|PURGE_TXCLEAR)==FALSE)
		printf("INIT: PurgeComm ERRNO= %d \n",GetLastError());

	iecHostStation();

	//DatabaseDisconnect(&henv, &hdbc);
	//UnMountDB(); 
	//Lan_finish();
	exit(0);
}
      


unsigned char CrcCheckforval( char *CrcBuf,int len )
{
	int				i;
	unsigned char	ch=0;

	if (len==1) return( ch&0xff );
	else
	{  
       printf("ch=%0x \n",ch);
	for(i=4;i<len;i++)
	{	

		ch= ch+(*(CrcBuf+i)) ;	

	}
	return( ch&0xff );
	}
}
unsigned char CrcCheckforfix( char *CrcBuf,int len )
{
	int				i;
	unsigned char	ch=0;

	if (len==1) return( ch&0xff );
	else
	{
	for(i=1;i<len;i++)
		ch= ch+(*(CrcBuf+i)) ;		
	return( ch&0xff );
	}
}


int ReadDefineFile()
{	int		i,j,id;
	char		instrument_id;
	char		gather_id;
	FILE	*fp;
	char	c_name[200],	l_name[50];
	DMS_COMMON common_aa; 


	for(i=0;i<MAX_YC_NUM;i++)
	{		strcpy(yc[i].c_name,"");	strcpy(yc[i].l_name,"");	yc[i].instrument_id	= 0;	}	
	
	while((fp=fopen(YC_TABLE,"r"))==NULL)
	{ printf("DL_CDT.EXE: %s can not open\n",YC_TABLE); Sleep(10*1000 ); }
	
	while( fscanf(fp,"%s",c_name)!=EOF )
	{	if( strstr(c_name,"*/")!=NULL ) break;	}
	
	fscanf(fp,"%d",&j);
	{ Slavenum = j; printf("\n Slavenum== %d\n",Slavenum); }

	for(id=0;;id++)
	{
		if( fscanf(fp,"%d %s %s %d %d",&i,c_name,l_name,&instrument_id,&gather_id)==EOF)
		{ YcNum = id; printf("READ %s OK! (YcNum=%d)\n",YC_TABLE,YcNum); fclose( fp ); break; }

		strcpy( yc[id].c_name, c_name ); strcpy( yc[id].l_name, l_name ); 
		yc[id].instrument_id = instrument_id;
		yc[id].gather_id	 = gather_id;   
		
        while(ReadEntryByNameStr(yc[id].l_name,&common_aa)==-1)
		{	printf("No %s in RDB\n",yc[id].l_name); Sleep(5*1000); }

		yc[id].dev_id	=	common_aa.point.dev_id;
		yc[id].data_type=	common_aa.point.data_type;
		yc[id].point_id	=	common_aa.point.pt_id;

		printf("%3d %20s %20s  %d  %d ( %d_%d_%d )\n",id,yc[id].c_name,yc[id].l_name,yc[id].instrument_id,yc[id].gather_id,
					yc[id].dev_id, yc[id].data_type, yc[id].point_id);
	}
	return( SUCCESS );
}

//=========================================================================//