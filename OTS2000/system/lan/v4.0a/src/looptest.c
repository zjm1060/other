/***************************************************************************************
FILENAME:	lanshm_test.c

FUNCTION:	
	
	source code of test tool all the time ,include:
	loop:
	(1) send one record 
	(2) send  records
	(3) send  all data

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2006.3.24	create the file for V4.0		yh

***************************************************************************************/
#include "../inc/lanhead_all.h"
  
#define PRTBUFSIZE 30

void MultiPoint(unsigned char ,unsigned char ,unsigned char ,unsigned char,unsigned short ,unsigned short ,unsigned short,int );
void AllPoint(unsigned char ,unsigned char ,unsigned char,unsigned short ,unsigned short ,unsigned short,int );
void CurveAllPoint(unsigned char ,unsigned char,unsigned short ,unsigned short,unsigned short ,float);

void LMSPack(unsigned char ,unsigned short );
void SMSPack(char* );

void main()
{	
		unsigned char prtbuf[PRTBUFSIZE];
	int k;
	int	sendb_ptr,recvb_ptr;
	int mode;

	if(-1==MountDB(DB_NAME))
		exit(0);
	
	if(lan_init_all()==FALSE)
	{
		printf("\ntest.c:: get_lan_shm error\n");
		exit(0);
	}
	
	k=-1;
	
	for(;;)
	{
	 k++;
	   if(k==10000)	exit(0);
/*	 MultiPoint(1,2,1,IMP_TYPE,2,4,0,123); 
	MultiPoint(4,1,2,SOE_TYPE,8,15,8,0); 
	MultiPoint(4,1,2,SYSMSG_TYPE,8,15,1,0);
	MultiPoint(10,1,2,CURVE_TYPE,0,15,1,111);
	LMSPack(1,1);  
	MultiPoint(10,1,1,CURVE_TYPE,1,8,1,1111);
	AllPoint(1,2,ANA_TYPE,0,1,0,999);
	AllPoint(1,2,POL_TYPE,0,1000,2,0);
	CurveAllPoint(1,2,20,9,10,5678);
	
	
	MultiPoint(1,1,2,ANA_TYPE,0,15,0,999);
	MultiPoint(4,1,2,SOE_TYPE,0,15,1,0);
	AllPoint(1,3,ANA_TYPE,0,1000,5,9);
	AllPoint(1,3,POL_TYPE,0,1000,1,0);
	MySleep(20);
	
	MultiPoint(1,1,2,ANA_TYPE,0,15,0,111);
	MultiPoint(4,1,2,SOE_TYPE,0,15,0,0);   
	AllPoint(1,3,ANA_TYPE,0,1000,5,12568);
	AllPoint(1,3,POL_TYPE,0,1000,0,0);
*/	
	MultiPoint(4,1,2,SOE_TYPE,0,150,1,0);
	MySleep(20);
	  
	} 
	
	
	SMSPack("okokokokokokok");
}	


void MultiPoint(unsigned char typeid,unsigned char stn,unsigned char dev,unsigned char data_type,unsigned short first,unsigned short end,unsigned short status,int data)
{
	LAN_HEAD	head;
	COMMON_MSG  	msg;
	short		i,j,pt;
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	char 		buf[MAX_BUF_SIZE]; 
	float		fdata;
	
	head.dest_id=0;
	GetLocalHostId((UCHAR	*)&head.src_id);
	head.dp_type=BJ_DB_TIME;

	
	memset(buf,0,MAX_BUF_SIZE);
	
	msg.type_id 	= typeid;
	msg.stn_id = stn;	
	msg.dev_id = dev;
	msg.data_type=data_type;
	
	GetLocalHostId((UCHAR	*)&msg.host_id);
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.ms  =0;
	msg.sec  =devtime->tm_sec;
	msg.min  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
        msg.day  =devtime->tm_mday;
        msg.month  =devtime->tm_mon+1;
        msg.year  =devtime->tm_year+1900; 
               

	head.length=6;
	for(i=first;i<=end;i++)
	{
		if(head.length>=(MAX_BUF_SIZE-18))
		{
			printf(" length >MAX_BUF_SIZE\n");
			 break;
		}
		
		
		memcpy(buf+head.length,&i,2);
		memcpy(buf+head.length+2,&status,2);
#ifdef _BIG_ENDIAN_
		Swap2Byte(&buf[head.length]);
		Swap2Byte(&buf[head.length+2]);	
#endif		
		if(data_type != ANA_TYPE &&data_type != IMP_TYPE  &&data_type != CURVE_TYPE)
		{
			head.length=head.length+4;
		}
		else
		{	
			if(data_type ==CURVE_TYPE)
			{
				fdata=(float)data;
				memcpy(buf+head.length+4,&fdata,4);
			}else
			{
				memcpy(buf+head.length+4,&data,4);
			}
			
#ifdef _BIG_ENDIAN_
			Swap4Byte(&buf[head.length+4]);
#endif			
			head.length=head.length+8;
		}
	}
	
	
	msg.number=i-first;
	printf("msg.number=%d\n",msg.number);
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&msg.number);	
	Swap2Byte(&msg.ms);
	Swap2Byte(&msg.year);
#endif	
	memcpy(buf,&msg,6);
	if(typeid !=  DPS_IND_ALM )
	{
		memcpy(buf+head.length,&msg.host_id, 8);	
		head.length=head.length+8;
	}
	else
	{	memcpy(buf+head.length,&msg.ms, 10);
		head.length=head.length+10;
	}

	printf("head.length=%d dp_type=%d\n",head.length ,head.dp_type);
	for(j=0;j<head.length;j++)
	{
		printf("%d",(unsigned char) buf[j]);
		printf("=");
	}	
	printf("\n");
	
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
#endif			
	lan_out((char *)buf,head,0);

	return;
}

void AllPoint(unsigned char stn,unsigned char dev,unsigned char type,unsigned short offset,unsigned short num,unsigned short status,int data)
{
	LAN_HEAD	head;
	COMMON_MSG  	msg;
	short		i,j;
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	char 		buf[MAX_BUF_SIZE]; 
	
	head.dest_id=0;
	GetLocalHostId((UCHAR	*)&head.src_id);
	head.dp_type=ALL_DB_TIME;

	
	memset(buf,0,MAX_BUF_SIZE);
	
	if(type == ANA_TYPE)	msg.type_id 	= DPS_ANA_ALL;
	else	 if(type == IMP_TYPE)		msg.type_id 	= DPS_IMP_ALL;
	else			msg.type_id 	= DPS_STATUS_ALL;
	
	msg.stn_id = stn;	
	msg.dev_id = dev;
	msg.data_type=type;
	msg.point_id = offset;
	
	GetLocalHostId((UCHAR	*)&msg.host_id);
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.ms  =0;
	msg.sec  =devtime->tm_sec;
	msg.min  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
        msg.day  =devtime->tm_mday;
        msg.month  =devtime->tm_mon+1;
        msg.year  =devtime->tm_year+1900; 
               
	head.length=8;
	
	for(i=0;i<=num;i++)
	{	
		if(head.length>=(MAX_BUF_SIZE-14))
		{
			printf("head.length=%d,  length >MAX_BUF_SIZE\n",head.length);
			 break;
		}
		
		memcpy(buf+head.length,&status,2);
#ifdef _BIG_ENDIAN_
		Swap2Byte(&buf[head.length]);
#endif		
		if(type != ANA_TYPE&& type != IMP_TYPE)
		{
			head.length=head.length+2;
		}
		else
		{	

			memcpy(buf+head.length+2,&data,4);
#ifdef _BIG_ENDIAN_
			Swap4Byte(&buf[head.length+2]);
#endif			
			head.length=head.length+6;
		}
		
		
	}
	
	msg.number=i;	printf("msg.number=%d\n",msg.number);
#ifdef _BIG_ENDIAN_
	Swap2Byte(&msg.number);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.ms);
	Swap2Byte(&msg.year);
#endif	
	memcpy(buf,&msg,8);
	
	memcpy(buf+head.length,&msg.host_id, 8);
	
	head.length=head.length+8;
	
	for(j=0;j<head.length;j++)
	{
		printf("%d",(unsigned char) buf[j]);
		printf("=");
	}	
	printf("\n");
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
#endif			
	lan_out((char *)buf,head,0);


	return;
}

void LMSPack(unsigned char stn,unsigned short msg_ptr)
{
	LAN_HEAD	head;
	COMMON_MSG  	msg;
	short		j;
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	char 		buf[MAX_BUF_SIZE]; 
	
	head.dest_id=0;
	GetLocalHostId((UCHAR	*)&head.src_id);
	head.dp_type=BJ_DB_TIME;
	head.length=12;
	
	memset(buf,0,MAX_BUF_SIZE);
	
	GetLocalHostId((UCHAR	*)&msg.host_id);
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.sec  =devtime->tm_sec;
	msg.min  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
        msg.day  =devtime->tm_mday;
        msg.month  =devtime->tm_mon+1;
        msg.year  =devtime->tm_year+1900; 

	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&msg.ms);
	Swap2Byte(&msg.year);
#endif	

	buf[0]=5;
	buf[1]=stn;
	
	memcpy(buf+2,&msg_ptr, 2);
	memcpy(buf+4,&msg.host_id, 8);

	printf("head.length=%d dp_type=%d\n",head.length ,head.dp_type);
	for(j=0;j<head.length;j++)
	{
		printf("%d",(unsigned char) buf[j]);
		printf("=");
	}	
	printf("\n");
	
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
#endif			
	lan_out((char *)buf,head,0);

	return;
}

void SMSPack(char* sms)
{
	LAN_HEAD	head;
	short		j;
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	char 		buf[MAX_BUF_SIZE]; 
	
	head.dest_id=0;
	GetLocalHostId((UCHAR	*)&head.src_id);
	head.dp_type=BJ_DB_TIME;
	head.length=32;
	
	memset(buf,0,MAX_BUF_SIZE);
	
	re_time=time(&newtime);
	
#ifdef _BIG_ENDIAN_
	Swap4Byte(&re_time);
	
#endif	

	buf[0]=6;
	buf[1]=head.src_id;
	

	memcpy(buf+2,sms,12);
	
	memcpy(buf+28,&re_time,4);
	
	printf("head.length=%d dp_type=%d\n",head.length ,head.dp_type);
	for(j=0;j<head.length;j++)
	{
		printf("%d",(unsigned char) buf[j]);
		printf("=");
	}	
	printf("\n");
	
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
#endif			
	lan_out((char *)buf,head,0);

	return;
}

void CurveAllPoint(unsigned char stn,unsigned char dev,unsigned short num,unsigned short pt,unsigned short offset,float fdata)
{
	LAN_HEAD	head;
	COMMON_MSG  	msg;
	short		i,j;
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	char 		buf[MAX_BUF_SIZE]; 
	
	head.dest_id=0;
	GetLocalHostId((UCHAR	*)&head.src_id);
	head.dp_type=ALL_DB_TIME;

	
	memset(buf,0,MAX_BUF_SIZE);
	msg.type_id 	= DPS_CURVE_ALL;
	
	msg.stn_id = stn;	
	msg.dev_id = dev;
	msg.data_type=CURVE_TYPE;
	msg.point_id = pt;
	
	GetLocalHostId((UCHAR	*)&msg.host_id);
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.ms  =0;
	msg.sec  =devtime->tm_sec;
	msg.min  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
        msg.day  =devtime->tm_mday;
        msg.month  =devtime->tm_mon+1;
        msg.year  =devtime->tm_year+1900; 
               
	head.length=8;
	
	memcpy(buf+head.length,&offset,2);
#ifdef _BIG_ENDIAN_
	Swap2Byte(&buf[head.length]);
#endif	
	head.length=head.length+2;
	
	for(i=0;i<=num;i++)
	{	
		if(head.length>=(MAX_BUF_SIZE-18))
		{
			printf(" length >MAX_BUF_SIZE\n");
			 break;
		}
		
		
		memcpy(buf+head.length,&fdata,4);
#ifdef _BIG_ENDIAN_
		Swap4Byte(&buf[head.length]);
#endif			
		head.length=head.length+4;
	}
	
	msg.number=i;printf("msg.number=%d\n",msg.number);
#ifdef _BIG_ENDIAN_
	Swap2Byte(&msg.number);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.ms);
	Swap2Byte(&msg.year);
#endif	
	memcpy(buf,&msg,8);
	
	memcpy(buf+head.length,&msg.host_id, 8);
	
	head.length=head.length+8;
	
	for(j=0;j<head.length;j++)
	{
		printf("%d",(unsigned char) buf[j]);
		printf("=");
	}	
	printf("\n");
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
#endif			
	lan_out((char *)buf,head,0);


	return;
}
