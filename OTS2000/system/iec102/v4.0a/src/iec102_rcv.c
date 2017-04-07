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
/*程序接收遥测中的正向总有功，反向总有功，正向总无功，反向总无功
  (积分周期/负荷曲线周期=15分钟)*/

#include "\home\ems\system\iec102\v4.0a\inc\iec102_com.h"

HANDLE	   hCom;
SYSTEMTIME t;
 YC				yc[MAX_YC_NUM];
 extern unsigned char	RcvBuf[BUFSIZE],	SndBuf[BUFSIZE];
 int			last_year=0, last_month=0, last_day=0, last_hour=0, last_minute=0, last_second=0;
 int			firstscan[SlaveTotal];
 int			runlog[SlaveTotal];		//已读写标志
 char			Addr[SlaveTotal];
 char			Addrforgather[SlaveTotal];
 int			ControlLog;			    //for HostSendAskCmd           
 int			inttmp;

  char   quest_cmd[6]	={ 0x10, 0x49, 0x02, 0x00, 0x4B, 0x16 };
  char   reset_cmd[6]	={ 0x10, 0x40, 0x02, 0x00, 0x42, 0x16 };
  char   quest_answer[6]={ 0x10, 0x0B, 0x02, 0x00, 0x0D, 0x16 };
  char   reset_answer[6]={ 0x10, 0x20, 0x02, 0x00, 0x22, 0x16 };
  char   quest_answer2[6]={ 0x10, 0x2B, 0x02, 0x00, 0x2D, 0x16 };

void iecHostStation()
{ int    num,k;
  char   bufrcvfirst[BUFSIZE],bufrcvend[BUFSIZE],TmpBuf1[BUFSIZE];
  BOOL   fSuccess;
  int     i,j,m, now_year, now_month, now_day, now_hour, now_minute, now_second;
  struct  tm *timeptr;
  time_t  time_value;
    
  printf("\n\n@@@@@@@ iec102_com.exe SERVER START @@@@@@@\n\n");
  for(i=0;i<Slavenum;i++)
  {
	firstscan[i]		=0;
	runlog[i]			=0;
	Addr[i]				=yc[0+4*i].instrument_id;
	Addrforgather[i]	=yc[0+4*i].gather_id;
  }

  time_value=time(NULL);  
  timeptr=localtime( &time_value );  
  now_year  = timeptr->tm_year+1900-2000;   
  now_month = timeptr->tm_mon+1;            
  now_day   = timeptr->tm_mday;
  now_hour  = timeptr->tm_hour;
  now_minute= timeptr->tm_min;
  now_second= timeptr->tm_sec;
  printf("\n **firstscan** \n");
			if (now_hour !=0)
			{
		     last_year=now_year;
	         last_month=now_month;
             last_day=now_day;
	         last_hour=now_hour-1;
			}
			else if(now_hour==0)
			{
				if(now_day !=1)
				{
             last_year=now_year;
	         last_month=now_month;
             last_day=now_day-1;
	         last_hour=23;           
				}
				else
				{
                 last_year=now_year;
	             last_month=now_month-1;
                 if((now_month==3)||(now_month==5)||(now_month==7)||(now_month==8)||(now_month==10)||(now_month==12))  last_day=31;
				 else if((now_month==4)||(now_month==6)||(now_month==9)||(now_month==11))  last_day=30;
                 else if(now_month==2)  {if(((last_year%4==0)&&(last_year%100!=0))||(last_year%400==0)) last_day=29; else last_day=28;}
                 else if(now_month==1)  {last_year=now_year-1;last_month=12;last_day=31;}
	             last_hour=23;          
				}
			}		   
  

for(;;)
{
  time_value=time(NULL);  
  timeptr=localtime( &time_value );  
  now_year  = timeptr->tm_year+1900-2000;   
  now_month = timeptr->tm_mon+1;            
  now_day   = timeptr->tm_mday;
  now_hour  = timeptr->tm_hour;
  now_minute= timeptr->tm_min;
  now_second= timeptr->tm_sec;

  for(j=0;j<Slavenum;j++)      //对应采集站个数
  {
	if(( (firstscan[j]==0)||((now_minute > 10)&&(now_minute < 20)&&(last_hour!=now_hour)))&&(runlog[j]==0))  //每小时只计算一次
	{	
		printf("\n now_time:=%d==%d==%d==%d==%d==%d=\n",now_year,now_month,now_day,now_hour,now_minute,now_second);  
		printf("\n last_time:=%d==%d==%d==%d=\n",last_year,last_month,last_day,last_hour);  
		runlog[j]=1;

		
	    for(;;)
		{
			ControlLog=0x40;
			HostSendAskCmd(j,ControlLog);
			fSuccess=ReadFile(hCom,RcvBuf,BUFSIZE,&num,NULL);
			if( !fSuccess ){ printf("ReadFie() com error.\n"); Sleep(5000); continue; }
			printf("\n>>> "); 
		  	for (i=0;i<num;i++)
			{
				printf("%02x ",RcvBuf[i]); 					
			}
			if (RcvBuf[1]==0x20 || num > 0) break;
			printf("\n 复位链路失败..."); 
		}
        
    memset(SndBuf,0,BUFSIZE); 
    memset(TmpBuf1,0,BUFSIZE); 
    HostSendReadCmd(j,now_year,now_month,now_day,now_hour,last_year,last_month,last_day,last_hour);
    for(k=0;k<27;k++) TmpBuf1[k]=SndBuf[k];
    Sleep(2000);   

	memset(RcvBuf,0,BUFSIZE); 
	fSuccess=ReadFile(hCom,RcvBuf,BUFSIZE,&num,NULL);
	if( !fSuccess )
	{ printf("ReadFie() com1 error.\n"); Sleep(5000); continue; }
		printf("\n>>>");
		for (i=0;i<num;i++)    printf("%02x  ",RcvBuf[i]); 
	if(RcvBuf[0] !=0xE5)  { printf("从站确定 error.\n"); Sleep(5000); continue; }

	ControlLog=0x5A;
	memset(SndBuf,0,BUFSIZE); 
	HostSendAskCmd(j,ControlLog);
	Sleep(2000); 

	memset(RcvBuf,0,BUFSIZE);   
	fSuccess=ReadFile(hCom,RcvBuf,BUFSIZE,&num,NULL);
	if( !fSuccess )
	{ printf("从站镜像帧确认 error.\n"); Sleep(5000); continue; }
		printf("\n>>>");
 		for (i=0;i<num;i++)     printf("%02x  ",RcvBuf[i]);
		Sleep(2000); 
    fSuccess=	HostReadInfoBack((char *)&RcvBuf,(char *)&TmpBuf1);
    if(!fSuccess)  
    { printf("ReadFile() error.\n"); Sleep(5000); continue; }

	if(ControlLog==0x5A)  ControlLog=0x7A;
	else if (ControlLog==0x7A)  ControlLog=0x5A;
	memset(SndBuf,0,BUFSIZE); 
	HostSendAskCmd(j,ControlLog);
	Sleep(2000);

	memset(bufrcvfirst,0,BUFSIZE);     
	fSuccess=ReadFile(hCom,bufrcvfirst,BUFSIZE,&num,NULL);
	if( !fSuccess ){ printf("从站上送第一时间段数据 error.\n"); Sleep(5000); continue; }
        printf("\n >>>"); 
    	for (i=0;i<num;i++)   printf("%02x  ",bufrcvfirst[i]); 
	fSuccess=HostRcvDataAnalyse(j,(char *)&bufrcvfirst);
	if(!fSuccess)  { printf("HostRcvDataAnalyse error.\n"); Sleep(5000); continue; }

	for(m=2;m<20;m++) //负荷曲线周期=15分钟
	{
		if( ControlLog == 0x5A)  ControlLog=0x7A;
		else  ControlLog=0x5A;

		memset(SndBuf,0,BUFSIZE);      
		HostSendAskCmd(j,ControlLog);
		Sleep(2000); 
		memset(RcvBuf,0,BUFSIZE);    
		fSuccess=ReadFile(hCom,RcvBuf,BUFSIZE,&num,NULL);
		if( !fSuccess )
		{ printf("第%d时间段数据 error.\n",m); Sleep(5000); continue; }
			printf("\n >>>");
			for (i=0;i<num;i++)        printf("%02x  ",RcvBuf[i]); 
		if(RcvBuf[1]==0x15) { printf("\n镜像帧结束\n");  break; }

		memset(SndBuf,0,BUFSIZE);  
		for (i=0;i<num;i++)        bufrcvend[i]=RcvBuf[i];     

		fSuccess=HostRcvDataAnalyse(j,bufrcvend);
		if(!fSuccess)  { printf("HostRcvDataAnalyse error.\n"); Sleep(5000); continue; }
	}  //end for(m=2;m<20;m++)

	fSuccess=HostReadInfoEnd(j,(char *)&RcvBuf,now_year,now_month,now_day,now_hour);
	if(!fSuccess)  { printf("HostReadInfoEnd error.\n"); Sleep(5000); continue; }

	if( ControlLog == 0x5A)  ControlLog=0x7A;
	else  ControlLog=0x5A;
	memset(SndBuf,0,BUFSIZE);      
	HostSendAskCmd(j,ControlLog);
	Sleep(2000); 
	memset(RcvBuf,0,BUFSIZE);   
	fSuccess=ReadFile(hCom,RcvBuf,BUFSIZE,&num,NULL);
	if( !fSuccess ){ printf("从站确定 error.\n"); Sleep(5000); continue; }
		printf("\n>>>",RcvBuf[i]); 
  		for (i=0;i<num;i++)       printf("%02x  ",RcvBuf[i]); 
		if(RcvBuf[0] !=0xE5)  { printf("从站确定 error.\n"); Sleep(5000); continue; }

	HostRcvDataCul(j,(char *)&bufrcvfirst,(char *)&bufrcvend);   
  
	} //end   if
   firstscan[j]=1;
   runlog[j]=1;
   last_year=now_year;
   last_month=now_month; 
   last_day=now_day; 
   last_hour=now_hour; 
   last_minute=now_minute; 
   last_second=now_second;
  }  //end  for(j=0;j<Slavenum;j++)

//  DatabaseDisconnect(&henv, &hdbc);
  Sleep(50000);
 }  //end for(;;)
//  goto LOOP;
}

void HostSendReadCmd(int j,int now_year,int now_month,int now_day,int now_hour,int last_year,int last_month,int last_day,int last_hour)
{
		int		i, SndLen;
	

		SndBuf[0] = 0x68;	SndBuf[1] = 0x15;
		SndBuf[2] = 0x15;	SndBuf[3] = 0x68;
		SndBuf[4] = 0x73;	SndBuf[5] = Addr[j];          
		SndBuf[6] = 0x00;	SndBuf[7] = 0x78;   //累计量=0x78，增量=0x79 
        SndBuf[8] = 0x01;   SndBuf[9] = 0x06;
		SndBuf[10] =Addr[j];	SndBuf[11] = 0x00;
		SndBuf[12] = 0x0B; 
		/****************对应需改的地址*************************/
		SndBuf[13] = Addrforgather[j];       SndBuf[14] = Addrforgather[j]+3; 	
		SndBuf[15] = 0x00;
		SndBuf[16] = DtoH(last_hour);   SndBuf[17] = DtoH(last_day);	
		SndBuf[18] = DtoH(last_month);  SndBuf[19] =DtoH(last_year);		
        SndBuf[20] = 0x00;
		SndBuf[21] = DtoH(now_hour);   SndBuf[22] = DtoH(now_day);	
		SndBuf[23] = DtoH(now_month);  SndBuf[24] = DtoH(now_year);
				
		SndBuf[25] = CrcCheckforval(SndBuf,25);
		SndBuf[26] = 0x16;
		
		SndLen = -1;
		if( !WriteFile(hCom,SndBuf,27,&SndLen,NULL) ) printf("\n HostSendReadCmd WriteFile error.\n");
		printf("<<<  ");		
		for (i=0;i<SndLen;i++)
			printf("%02x  ",SndBuf[i]);		
		Sleep(2*1000);		
	
}
void HostSendAskCmd(int j,int ControlLog)
{
	int		i,SndLen;
	


		SndBuf[0] = 0x10;	SndBuf[1] = ControlLog;
		SndBuf[2] = Addr[j];	SndBuf[3] = 0x00;
		SndBuf[4] = CrcCheckforfix(SndBuf,4);
		SndBuf[5] = 0x16;
		SndLen = -1;
		if( !WriteFile(hCom,SndBuf,6,&SndLen,NULL) ) printf("\n Host ReadCmd error.\n");
		printf(" <<< ");
        for (i=0;i<SndLen;i++)
		printf("%02x  ",SndBuf[i]);
		Sleep(2*1000);		
        


}

BOOL HostReadInfoBack(unsigned char *RcvBuf,unsigned char *TmpBuf1)
{

	


	if((RcvBuf[0]  == TmpBuf1[0]) &&(	RcvBuf[1] == TmpBuf1[1])
	&&(	RcvBuf[2]  == TmpBuf1[2]) &&(	RcvBuf[3] == TmpBuf1[3])
	&&(	RcvBuf[4]  == 0x28)       &&(	RcvBuf[5] == TmpBuf1[5]) 
	&&(	RcvBuf[6]  == TmpBuf1[6]) &&(	RcvBuf[7] == TmpBuf1[7])
    &&( RcvBuf[8]  == TmpBuf1[8]) &&(    RcvBuf[9] == 0x07)
	&&(	RcvBuf[10] == TmpBuf1[10])&&(	RcvBuf[11] == TmpBuf1[11])
	&&(	RcvBuf[12] == TmpBuf1[12])&&(	RcvBuf[13] == TmpBuf1[13])
	&&(	RcvBuf[14] == TmpBuf1[14])&&(    RcvBuf[15] == TmpBuf1[15])	
	&&(	RcvBuf[16] == TmpBuf1[16])&&(    RcvBuf[17] == TmpBuf1[17])
	&&(	RcvBuf[18] == TmpBuf1[18])&&(	RcvBuf[19] == TmpBuf1[19])
	&&( RcvBuf[20] == TmpBuf1[20])&&(	RcvBuf[21] == TmpBuf1[21])
    &&( RcvBuf[22] == TmpBuf1[22])&&(	RcvBuf[23] == TmpBuf1[23])
	&&( RcvBuf[24] == TmpBuf1[24])&&(	RcvBuf[25] == CrcCheckforval(RcvBuf,25))
	&&(	RcvBuf[26] == TmpBuf1[26]))
     return(TRUE);
	else   return(FALSE);
		Sleep(2*1000);		
        


}
int HostReadInfoEnd(int j,unsigned char *RcvBuf,int now_year,int now_month,int now_day,int now_hour)
{   char tmp[100];

		    tmp[0] = 0x00;
			tmp[1] = DtoH(last_hour);
	        tmp[2] = DtoH(last_day);
		    tmp[3] = DtoH(last_month);
		    tmp[4] = DtoH(last_year);		

            tmp[5] = 0x00;
			tmp[6] = DtoH(now_hour);
	        tmp[7] = DtoH(now_day);
		    tmp[8] = DtoH(now_month);
		    tmp[9] = DtoH(now_year);

	if((RcvBuf[0]  == 0x68) &&(	RcvBuf[1] == 0x15)
	&&(	RcvBuf[2]  == 0x15) &&(	RcvBuf[3] == 0x68)
	&&(	RcvBuf[4]  == 0x28) &&(	RcvBuf[5] == Addr[j]) 
	&&(	RcvBuf[6]  == 0x00) &&(	RcvBuf[7] == 0xAA)
    &&( RcvBuf[8]  == 0x01) &&( RcvBuf[9] == 0x0A)
	&&(	RcvBuf[10] == Addr[j]) &&(	RcvBuf[11] == 0x00)
	&&(	RcvBuf[12] == 0x0B) &&(	RcvBuf[13] == 0x01)
	&&(	RcvBuf[14] == 0x04) &&( RcvBuf[15] == tmp[0])	
	&&(	RcvBuf[16] == tmp[1])&&(RcvBuf[17] == tmp[2])
	&&(	RcvBuf[18] == tmp[3])&&(RcvBuf[19] == tmp[4])
	&&( RcvBuf[20] == tmp[5])&&(RcvBuf[21] ==tmp[6])
	&&( RcvBuf[22] == tmp[7])&&(RcvBuf[23] == tmp[8])
	&&( RcvBuf[24] == tmp[9])&&(RcvBuf[25] == CrcCheckforval(RcvBuf,25))
	&&(	RcvBuf[26] == 0x28))
     return(1);
	else   return(-1);
		Sleep(2*1000);		


}

int HostRcvDataAnalyse(int j,char *bufrcv0)
{

    if((bufrcv0[0]  == 0x68) &&(	bufrcv0[3] == 0x68)
	&&(	bufrcv0[4]  == 0x28) &&(	bufrcv0[5] == Addr[j]) 
	&&(	bufrcv0[6]  == 0x00) &&(	bufrcv0[7] == 0x02)
    &&(    bufrcv0[9] == 0x05)
	&&(	bufrcv0[10] == Addr[j]) &&(	bufrcv0[11] == 0x00))
//	&&(	bufrcv0[12] == 0x0B)
//	&&(	bufrcv0[125] == CrcCheckforval(bufrcv0,125))
//	&&(	bufrcv0[126] == 0x16))
     return(1);
	else   return(-1);
		Sleep(2*1000);	
}


void  HostRcvDataCul(int j,char *bufrcvfirst,char *bufrcvend)
{
  LAN_HEAD        head;
  ANA_MSG  		  msg;
  //DMS_COMMON	  common_aa;

  char        time_year[10];
  char        time_mth[10];
  char        time_day[10];
  char        gen_db_time[100];
  int         i;
  int		  valuetmp;
  long        dlNo11,dlNo12,dlNo13,dlNo14,dlNo21,dlNo22,dlNo23,dlNo24;
  float       valuenew[4];
  time_t  tim;
  struct tm   tmpt;
/* 
  //用于历史数据库
  long        time0;
  char        time_tm[20];
  struct tm   *datetime;
  time0=time(NULL); datetime=localtime(&time0);
  strcpy( gen_db_time ,"" );

  sprintf(time_mth,"%d",  datetime->tm_mon+1 );
  strcat (gen_db_time,time_mth);
  strcat (gen_db_time,"/");
  sprintf(time_day,"%d",  datetime->tm_mday );
  strcat (gen_db_time,time_day);
  strcat (gen_db_time,"/");
  sprintf(time_year,"%d",  datetime->tm_year + 1900);
  strcat (gen_db_time, time_year);
  strcat (gen_db_time," ");
  sprintf (time_tm," %d",datetime->tm_hour);
  strcat (time_tm,":00");
  strcat (gen_db_time,time_tm);
*/

    memcpy((char *)&dlNo11,(bufrcvfirst+14),4);   
    memcpy((char *)&dlNo12,(bufrcvfirst+21),4);
    memcpy((char *)&dlNo13,(bufrcvfirst+28),4);   
    memcpy((char *)&dlNo14,(bufrcvfirst+35),4);
    memcpy((char *)&dlNo21,(bufrcvend+14),4);   
    memcpy((char *)&dlNo22,(bufrcvend+21),4);
    memcpy((char *)&dlNo23,(bufrcvend+28),4);   
    memcpy((char *)&dlNo24,(bufrcvend+35),4);

/*
	if(dlNo21>=dlNo11) valuenew[0]=(float)((dlNo21-dlNo11)*0.01); else valuenew[0]=(float)((2147483647-dlNo11+dlNo21)*0.01);
	if(dlNo22>=dlNo12) valuenew[1]=(float)((dlNo22-dlNo12)*0.01); else valuenew[1]=(float)((2147483647-dlNo12+dlNo22)*0.01);
	if(dlNo23>=dlNo13) valuenew[2]=(float)((dlNo23-dlNo13)*0.01); else valuenew[2]=(float)((2147483647-dlNo13+dlNo23)*0.01);
	if(dlNo24>=dlNo14) valuenew[3]=(float)((dlNo24-dlNo14)*0.01); else valuenew[3]=(float)((2147483647-dlNo14+dlNo24)*0.01);
*/	

	printf("\n 起始电量: %f,%f, %f,%f \n",dlNo11,dlNo12,dlNo13,dlNo14);    
	printf("\n 最终电量: %f,%f, %f,%f \n",dlNo21,dlNo22,dlNo23,dlNo24);

    valuenew[0]=dlNo21;
	valuenew[1]=dlNo22;
	valuenew[2]=dlNo23;
	valuenew[3]=dlNo24;

	printf("\n instrument_id==%d \n",j+1);
	for(i=0;i<4;i++)
	{
     //四个值分别为正向有功，反向有功，正向无功，反向无功
	 //Insert_HDB(dev_id,point_id,(char *)&gen_db_time,valuenew[i]);

	msg.type_id =DPS_IMPULSE;
    msg.stn_id =1;
    msg.dev_id =yc[j*4+i].dev_id;
    msg.data_type=DPS_IMPULSE;
    msg.number=1;
    msg.point_id = yc[j*4+i].point_id;

    msg.status =(unsigned short)0;
	valuetmp = (int)(valuenew[i]);
	memcpy(msg.data,&valuetmp,4);

    GetLocalHostId(&msg.host_id);

    tim=time (NULL);
    tmpt=*localtime (&tim);
    msg.sec=tmpt.tm_sec;
    msg.min=tmpt.tm_min;
    msg.hour=tmpt.tm_hour;
    msg.day=tmpt.tm_mday;
    msg.month=tmpt.tm_mon+1;
    msg.year=tmpt.tm_year+1900;
    
    head.length=ANAMSG_SIZE;
    
    head.dp_type=BJ_DB_TIME;
    head.dest_id =0;
    GetLocalHostId(&head.src_id);
    head.dest_stn =msg.stn_id;
    head.packet_num =0; 
    printf("\n come into lanout %d \n",msg.point_id);
    lan_out ((char *)&msg,head,(unsigned char)0); 
/*****************************************/
	}
	Sleep(5000);

}


unsigned char DtoH(int change)
{
 unsigned char  Hex_table[] =   
                {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
                 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
                 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
                 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
                 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
                 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
                 0x60,0x61,0x62,0x63};
      unsigned char i;
      i=Hex_table[change];
	  return(i);
	}

void iecHostSetTime()
{
		int			SndLen;
	
	//---broadcast time to slave
		printf("\n\n: !!! Host Set Time Run!!! \n\n");

		SndBuf[0] = 0x68;	SndBuf[1] = 0x0e;
		SndBuf[2] = 0x00;	SndBuf[3] = 0x68;
		SndBuf[4] = 0x46;	SndBuf[5] = 0xff;
		SndBuf[6] = 0x67;	SndBuf[7] = 0x01;
        SndBuf[8] = 0x05;   SndBuf[9] = 0x88;
		SndBuf[10] = 0x88;	SndBuf[11] = 0x71;
		SndBuf[12] = 0x33+(t.wYear-2000);	SndBuf[13] = 0x33+t.wMonth; 
		SndBuf[14] = 0x33+t.wDay;	    	SndBuf[15] = 0x33+t.wHour;
		SndBuf[16] = 0x33+t.wMinute;		SndBuf[17] = 0x33+t.wSecond;
		SndBuf[18] = CrcCheckforval(SndBuf,18);
		SndBuf[19] = 0x16;
        
		printf("\n\n %2x  %2x  %2x  %2x  %2x  %2x ",SndBuf[12],SndBuf[13],SndBuf[14],SndBuf[15],SndBuf[16],SndBuf[17]);

		SndLen = -1;
		if( !WriteFile(hCom,SndBuf,20,&SndLen,NULL) ) printf("\nHost SetTimeCmd error.\n");
		
		Sleep(2*1000);		
	
}

int HostSendLLZTTCmd()
{
  int    i, num;
  BOOL   fSuccess;
/************************************/
  memset(SndBuf,0,BUFSIZE); 
  memset(RcvBuf,0,BUFSIZE); 
  fSuccess=WriteFile(hCom,quest_cmd,6,&num,NULL);
    printf("\n <<<");
    for(i=0;i<6;i++)  printf("=%0x",quest_cmd[i]);
  if( !fSuccess ){ printf("WriteFile() rtu error.\n"); Sleep(5000); return(0); }

  fSuccess=ReadFile(hCom,RcvBuf,BUFSIZE,&num,NULL);
  if( !fSuccess ){ printf("ReadFile() com1 error.\n"); Sleep(5000); return(0); }
	printf("\n >>>");
	for(i=0;i<6;i++)  printf("=%0x=",RcvBuf[i]);

  if((RcvBuf[0]==quest_answer2[0])&&(RcvBuf[1]==quest_answer2[1])
	  &&(RcvBuf[2]==quest_answer2[2])&&(RcvBuf[3]==quest_answer2[3])
	  &&(RcvBuf[4]==quest_answer2[4])&&(RcvBuf[5]==quest_answer2[5]))
  { printf("\n 二次请求链路状态成功！\n ");return(1); }

  for(i=0;i<6;i++) 
  {
	  if(RcvBuf[i]!=quest_answer[i]) return(0); 
  }
  printf("\n 请求链路状态成功！\n ");
/************************************/
  memset(SndBuf,0,BUFSIZE); 
  memset(RcvBuf,0,BUFSIZE); 
  fSuccess=WriteFile(hCom,reset_cmd,6,&num,NULL);
  if( !fSuccess ){ printf("WriteFile() rtu error.\n"); Sleep(5000); return(0); }

  fSuccess=ReadFile(hCom,RcvBuf,BUFSIZE,&num,NULL);
  if( !fSuccess ){ printf("ReadFile() com1 error.\n"); Sleep(5000); return(0); }
    printf("\n");
  for(i=0;i<6;i++)  printf("=%0x=",RcvBuf[i]);
  for(i=0;i<6;i++) 
  {
	  if(RcvBuf[i]!=reset_answer[i]) return(0); 
  }
  printf("\n 复位远方链路成功！\n ");
/************************************/
  memset(SndBuf,0,BUFSIZE); 
  memset(RcvBuf,0,BUFSIZE); 
  fSuccess=WriteFile(hCom,quest_cmd,6,&num,NULL);
  if( !fSuccess ){ printf("WriteFile() rtu error.\n"); Sleep(5000); return(0); }

  fSuccess=ReadFile(hCom,RcvBuf,BUFSIZE,&num,NULL);
  if( !fSuccess ){ printf("ReadFile() com1 error.\n"); Sleep(5000); return(0); }
    printf("\n");
  for(i=0;i<6;i++)  printf("=%0x=",RcvBuf[i]);
  for(i=0;i<6;i++) 
  {
	  if(RcvBuf[i]!=quest_answer2[i]) return(0); 
  }
  printf("\n 二次请求链路状态成功！\n ");
  return(1); 
/************************************/
}
