/***************************************************************************************
FILENAME:	test.c

FUNCTION:	
	
	source code of lan test tool,include:
	(1) read lan send and recv shared memory
	(2) write one record to send shared memory
	(3) write batch records to send shared memory

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	create the file for V4.0		yyp

***************************************************************************************/
#include "/LANINC/lanhead_all.h"

void SendCmd(int type);
void SendRanCmd();
void MultiPointPack(unsigned char typeid,unsigned char stn,unsigned char dev,unsigned char type,unsigned short first,unsigned short end,unsigned short status,int data);
int SendPoints();
#define PRTBUFSIZE 30
unsigned char 	prtbuf[SHM_DATA_SEG_SIZE+8];
short	length;
int	sendb_ptr,recvb_ptr;
int 	mode;
int	cmdnum;
int i,j,k, type,len;
int stnid,devid;
int pol_seg_ptr;
unsigned char *pt;
int 	soeptr,start;
LCUID	plcid;

void main()
{	
	int	iscont;
	stnid =1;
	devid=1;
	/*##################### 初始化lan共享区及plc共享区 #########################*/

	if(MountDB(DB_NAME) == -1)
	{
		printf("test.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("test.c: call lan_init_all() error\n");		
		exit(0);
	}
	if( InitPlcDataShm(0) == FALSE) 
	{
		printf("test.c: call initPlcDataShm(0) error\n");
		exit(0);
	}
	
	iscont = 0;
	do
	{
		if(iscont == 0)
		{
			printf("\n\n\n\n\n\n\n\n\n\n========= 请选择 ============================\n\n");
			printf("	01.发送开机令\n");
			printf("	02.发送合断路器令\n");
			printf("	03.发送短信息 \n");
			printf("	04.发送网络切换令\n");
			printf("	05.发送格式错开机令\n");
			printf("	06.修改上位机plc共享区\n");
			printf("	07.循环发送开机令\n");
			printf("	08.发送任意格式命令\n");
			printf("	09.模拟任意格式报警\n");
			printf("	00.exit\n");

			printf("\n======= 请输入选择项( 0-9 ):");
			scanf("%d", &mode );
		}
		switch(mode)
		{
		case 1:
			printf("\n======= 请输入命令个数:");
			scanf("%d", &cmdnum );		
			for(i=0;i<cmdnum;i++)
			{			
				printf("i=%d: ",i);
				SendCmd(1);
				MySleep(40);
			}	
			break;
		case 8:
			SendRanCmd();
			break;
		case 9:
			SendPoints();
			break;			
		case 7:
	
			for(;;)
			{			
				SendCmd(1);
				MySleep(300);
			}	
			break;
		case 2:
			printf("\n======= 请输入命令个数:");
			scanf("%d", &cmdnum );		
			for(i=0;i<cmdnum;i++)
			{			
				printf("i=%d: ",i);
				SendCmd(2);
				MySleep(30);
			}	
			break;
		
		case 3:
			printf("\n======= 请输入命令个数:");
			scanf("%d", &cmdnum );		
			for(i=0;i<cmdnum;i++)
			{			
				BdSms("this is a test msg");
				MySleep(30);
			}	
			break;	
		case 4:
			printf("\n======= 请输入命令个数:");
			scanf("%d", &cmdnum );		
			for(i=0;i<cmdnum;i++)
			{			
				plcid.stnid=1;
				plcid.devid=1;
				NetSwitchCmd(plcid);
			}	
			break;
		case 5:
			printf("\n======= 请输入命令个数:");
			scanf("%d", &cmdnum );		
			for(i=0;i<cmdnum;i++)
			{			
				printf("i=%d: ",i);
				SendCmd(5);
				MySleep(30);
			}	
			break;
		case 6:
			/*shm_plc_send->seg[1][1].send_update_flag = 0;*/
			shm_plc_send->seg[1][1].send_update_flag = SENDFLAGSCADA;
			shm_plc_send->seg[1][1].pol_base = 0;
			shm_plc_send->seg[1][1].ai_size =0;
			shm_plc_send->seg[1][1].pol_size =0;
			shm_plc_send->seg[1][1].ana_data[0] = 1;
			shm_plc_send->seg[1][1].ana_data[2] = 2;
			shm_plc_send->seg[1][1].ana_data[199] = 99;
			shm_plc_send->seg[1][1].pol_data[0] = 101;
			shm_plc_send->seg[1][1].pol_data[1] = 202;
			shm_plc_send->seg[1][1].pol_data[31] = 909;
			

			printf("\nthe plc shm have been modified !\n");				
			break;			
		case 0:
			break;			
		default:
			printf("\n输入错误！%d\n\n");
			
		} /***** end of switch *****/
		
		if(mode >=1 && mode<=9)
		{
			printf("\n\n======= 是否重复上一次选择项？(1--是 0--否): ");
			scanf("%d",&iscont);
		}
		
	}while(mode!=0);
	
	UnMountDB();
	lan_finish(0);
	exit(0);
}

/*##################### 模拟命令 #################################*/
void SendCmd(int type)
{
	LAN_HEAD        head;
	COMMAND_MSG		msg;
	int				val,length,i,j;
	char			buf[COMMAND_SIZE];
	time_t     		re_time,newtime;
	struct  tm      *devtime;
	POINTER			pt;
	printf("\n\n");
	if(type ==1)
	{
		msg.type_id = C_UNIT;
		msg.stn_id = 1;
		msg.dev_id = 1;
		msg.data_type = 6;
		msg.point_id = 1;	
		msg.status.opr = 1;
		msg.ctrl_addr = 1;
	}
	else if(type ==5)
	{
		msg.type_id = C_UNIT;
		msg.stn_id = 1;
		msg.dev_id = 1;
		msg.data_type = 6;
		msg.point_id = 1;	
		msg.status.opr = 1;
		msg.ctrl_addr = 0;
	}
	else if(type ==2)
	{
		msg.type_id = C_BREAKER;
		msg.stn_id = 1;
		msg.dev_id = 1;
		msg.data_type = 5;
		msg.point_id = 1;	
		msg.status.opr = 1;	
		msg.ctrl_addr = 65;
	}

	else
	{
		return;
	}		
	msg.status.result = 0;
	msg.status.state = 0;
	msg.status.src = 0;
	msg.data.iValue = val;

	GetSrcId(&msg.host_id);
	msg.usr_id=0;
	strcpy(msg.usr_login_name,"test");
	pt.stn_id = msg.stn_id;
	pt.dev_id = msg.dev_id;
	pt.pt_id = msg.point_id;
	pt.data_type =msg.data_type;
/*
	if(GetPtCtrlAddrById( &pt, &msg.ctrl_addr ) == -1)
	{
		printf("GetPtCtrlAddrById error\n");
		return ;
	}
*/
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.second  =devtime->tm_sec;
	msg.minute  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
	msg.day  =devtime->tm_mday;
 	msg.month  =devtime->tm_mon+1;
	msg.year  =devtime->tm_year+1900; 

	head.dest_id=msg.dev_id;
	head.src_id = msg.host_id;
	head.dp_type=COMM_TIME;
	head.length = COMMAND_SIZE;
	head.dest_stn = msg.stn_id;
	length = head.length;
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);
#endif
	memcpy(buf,&msg,COMMAND_SIZE);

	for(i=0;i<length;i++)
	{
		printf("%d",(unsigned char) buf[i]);
		printf("=");		
	}
	printf("\n");

	lan_out((char *)&msg,head,0);
	return;
}


/*##################### 模拟任意命令 #################################*/
void SendRanCmd()
{
	LAN_HEAD        head;
	COMMAND_MSG		msg;
	int				val,length,i,j;
	char			buf[COMMAND_SIZE];
	time_t     		re_time,newtime;
	struct  tm      *devtime;
	POINTER			pt;
	printf("\n\n");
	printf("输入用户数据长度(32):" );
	scanf("%d", &val);
	head.length  = val;
	printf("输入包类型:" );
	scanf("%d", &val);
	head.dp_type  = val;
	printf("输入目的设备号:" );
	scanf("%d", &val);
	head.dest_id  = val;
	printf("输入源设备号:" );
	scanf("%d", &val);
	head.src_id  = val;	
	printf("输入目的厂站号:" );
	scanf("%d", &val);
	head.dest_stn  = val;
		
	printf("\n\n输入类型标识:" );
	scanf("%d", &val);
	msg.type_id = val;
	printf("输入厂站号:");
	scanf("%d", &val);
	msg.stn_id = val;
	printf("输入设备号:");
	scanf("%d", &val);
	msg.dev_id = val;
	printf("输入数据类型：(1.模拟 3.脉冲 4.中断 5.扫查 6.综合信息 ......）:");
	scanf("%d", &val);
	msg.data_type = val;
	printf("输入点号:");
	scanf("%d", &val);
	msg.point_id = val;	
	printf("输入操作状态:");
	scanf("%d", &val);
	msg.status.opr = val;
	msg.status.result = 0;
	msg.status.state = 0;
	msg.status.src = 0;
	printf("输入数据:");
	scanf("%d", &val);
	msg.data.iValue = val*1000;
	printf("输入控制地址:");
	scanf("%d", &val);
	msg.ctrl_addr = val;	
	printf("输入主机标识:");
	scanf("%d", &val);
	msg.host_id = val;	
	msg.usr_id=0;
	strcpy(msg.usr_login_name,"test");
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.second  =devtime->tm_sec;
	msg.minute  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
	msg.day  =devtime->tm_mday;
 	msg.month  =devtime->tm_mon+1;
	msg.year  =devtime->tm_year+1900; 

	length = head.length;
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);
#endif
	memcpy(buf,&msg,COMMAND_SIZE);
	printf("head.length=%d type_id=%d\n",length ,msg.type_id);
	for(i=0;i<length;i++)
	{
		printf("%d",(unsigned char) buf[i]);
		printf("=");		
	}
	printf("\n");

	lan_out((char *)&msg,head,0);
	return;
}


/*##################### 模拟变位报警 #################################*/
int SendPoints()
{
	LAN_HEAD        head;
	int	        val,type,stn,dev,data_type,data,status,first,end;
	DMS_COMMON      common_aa;
	COMMON_MSG  	msg;
	short		i,j,pt;
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	char 		buf[MAX_BUF_SIZE]; 	

	printf("\n\n");
	printf("输入类型标识:\n(1.ANA 3.IMP 4.IND 5.LMS 6.SMS 13.ANA_ALM 15.ANA_QLT_ALM 17.IND_QLT_ALM):" );
	scanf("%d", &type);
		printf("输入厂站号:");
	        scanf("%d", &stn);
		printf("输入设备号:");
	        scanf("%d", &dev);
	        printf("输入数据类型：(1.模拟 3.脉冲 4.中断 5.扫查 6.综合信息 ......）:");
	        scanf("%d", &data_type);
		printf("输入起始Ｉ／Ｏ输入地址(含):");
		scanf("%d", &first);
		printf("输入中止Ｉ／Ｏ输入地址(含):");
		scanf("%d", &end);

	
	if(data_type==4 || data_type ==5 || data_type==6)
	{
		
		printf("输入状态标志值 (0.复归 1.动作):");
		scanf("%d", &status);
	}
	else if(data_type==1 || data_type ==2 || data_type==3)
	{
		printf("输入状态标志值 (0.复归 1.越上上限 2.越上限 3.越下限 4.越下下限....):");
		scanf("%d", &status);
		printf("输入数据值:");
		scanf("%d", &data);
	}
	else
	{
		scanf("%d", &status);
		printf("\n数据类型输入错误，退出! \n");
		return TRUE;
	}

	MultiPointPack((unsigned char)type,(unsigned char)stn,(unsigned char)dev,(unsigned char) data_type, 
	(unsigned short)first, (unsigned short)end,(unsigned short)status, data);

	
	return TRUE;
}

void MultiPointPack(unsigned char typeid,unsigned char stn,unsigned char dev,unsigned char type,unsigned short first,unsigned short end,unsigned short status,int data)
{
	LAN_HEAD	head;
	COMMON_MSG  	msg;
	short		i,j,pt;
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	char 		buf[MAX_BUF_SIZE]; 
	int	        val,tmplen;	
	printf("\n\n");
	printf("输入用户数据长度（一个模拟量-22;一个开关量-20）：" );
	scanf("%d", &val);
	/*head.length  = val;*/
	tmplen =val;
	printf("输入包类型:" );
	scanf("%d", &val);
	head.dp_type  = val;
	printf("输入目的设备号:" );
	scanf("%d", &val);
	head.dest_id  = val;
	printf("输入源设备号:" );
	scanf("%d", &val);
	head.src_id  = val;	
	printf("输入目的厂站号:" );
	scanf("%d", &val);
	head.dest_stn  = val;
	
	printf("输入主机标识:");
	scanf("%d", &val);
	msg.host_id = val;	
	
	memset(buf,0,MAX_BUF_SIZE);
	
	msg.type_id 	= typeid;
	msg.stn_id = stn;	
	msg.dev_id = dev;
	msg.data_type=type;

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
		if(msg.type_id != DPS_ANALOG)
		{
			head.length=head.length+4;
		}
		else
		{	
			memcpy(buf+head.length+4,&data,4);
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
	if(typeid !=  DPS_IND_ALM  )
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
	
	head.length =tmplen;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
#endif			
	lan_out((char *)buf,head,0);

	return;
}