/***************************************************************************************
FILENAME:	stest.c

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

#include "../inc/lanhead_all.h"
#include "../../../iccp/v4.0a/inc/iccp_data.h"

void MultiPointPack(unsigned char ,unsigned char ,unsigned char ,unsigned char,unsigned short ,unsigned short ,unsigned short,int );
int  SendPoints();
void SendCmd();
void ReadSendShm();
void ReadRecvShm();
void ReadNetCmdShm();
void ReadCommCmdShm();
void ReadIccpDataShm();
void ReadPLCCmdShm();
void ReadPLCDataShm();
void ReadLogShm();

void ReadRealLogShm();
void WriteLog();

#define PRTBUFSIZE 30
unsigned char 	prtbuf[SHM_DATA_SEG_SIZE+8];
short	length;
int	sendb_ptr,recvb_ptr;
int 	mode;
char passwd[30];
int i,j,k, type,len,ptnum,ms,devnum;
int stnid,idx,devid;
int pol_seg_ptr;
unsigned char *pt;
int 	soeptr,objptr,start;


void  SetObjForceTest()
{
	char name[100]="PBG.F1.OUS.LCU_JTZS_DO58";
	//char name[100]="PBG.F1.OBJ.DLKP_DL1_OBJ";
	
	POINTER pt;
	GetPtIdByNameStr(name, &pt ); 
	//BdObjForceByID(pt,0);
	//BdcastQltObj(pt,0);
	BdcastQltWithSta(pt,1,1);
	printf("SetObjForceTest ok!");
}

void main(int argc, char *argv[])
{	
	int	iscont=0;
	char	InStr[100];
	POINTER point;
	wsa_startup();
	/* SetLogLevel(argc, argv); */
	/*********************# 初始化lan共享区及plc共享区 ************************#*/
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

//	get_iccp_data_shm()	;
	
	iscont = 0;
	do
	{
		if(iscont ==0 )
		{
			printf("\n\n");
			printf("\n========= 请选择 =========\n\n");
			printf("     01.模拟报警\n");
			printf("     02.模拟命令\n");
			printf("     03.send 区\n");
			printf("     04.recv 区\n");
			printf("     05.Net命令区\n");
			printf("     06.PLC命令区\n");
			printf("     07.原始数据区\n");
			printf("     08.查看实时日志\n");
			printf("     09.Comm命令区\n");			
			printf("     00.退出\n");

			printf("\n===== 请输入选择项( 0--9 ):");
			scanf("%d", &mode );
		}
		
		switch(mode)
		{
		
		case 1:
			/******************* 模拟报警 **********************/
			SendPoints();
			break;

		case 2:
			/******************* 模拟命令 *********************/
			SendCmd();
			break;
		
		case 3:
			/****************** 查看 send 区 ******************/
			ReadSendShm();
			break;		
		case 4:
			/****************** 查看 recv 区 ******************/
			ReadRecvShm();
			break;
		case 5:
			/****************** 查看 Net命令区 ******************/
			ReadNetCmdShm();
			break;
		case 6:
			/****************** 查看 PLC命令区 ******************/
			ReadPLCCmdShm();
			break;
		case 7:
			/****************** 查看 原始数据区 ******************/
			ReadPLCDataShm();
			break;
		case 8:
			/****************** 实时查看全部日志 ******************/
			ReadRealLogShm();
			break;
		case 9:
			/******************* 查看 通讯 命令区 **********************/
			ReadCommCmdShm();
			
			/* removed by yyp 2011.03.07
			j=0;
			printf("\n===== 请输入间隔(毫秒):");
			scanf("%d", &ms );	
			printf("\n===== 请输入设备数目:");
			scanf("%d", &devnum );
			printf("loop always to change all ana data....");
			for(;;)
			{
				MySleep(ms);
				j++;
				if(j>10) j=0;
				for(i=1;i<=devnum;i++)
				{
					GetEntryNumById( 1, i, 1, (unsigned int *)&ptnum );
					for(k=0;k<(ptnum/100+1);k++)
					{
					len = 99;
					if(k==ptnum/100) len = ptnum-k*100;	
					MultiPointPack((unsigned char)1,(unsigned char)1,(unsigned char)i,(unsigned char) 1, 
					(unsigned short)(k*100), (unsigned short)(k*100+len),(unsigned short)0, j * 100.111*1000);
					}	

				}
			}
			*/
			
			
			break;
		case 11:
			SetObjForceTest();	
			break;
		case 0:
			exit(0);			
		default:
			printf("\n输入错误！%d\n\n");
			
		} /***** end of switch *****/
		if(mode >=1 && mode<=9)
		{
			printf("\n\n===== 返回总菜单 or 重复? (0--返回 1--重复): ");
			scanf("%d",&iscont);
			gets(InStr);
		}

		printf("\n\n");
		
		
	}while(mode!=0);
	
	UnMountDB();
	lan_finish(0);
	exit(0);
}

/*********************# 模拟命令 ********************************#*/
void SendCmd()
{
	LAN_HEAD        head;
	COMMAND_MSG		msg;
	int				val,length,i,j;
	char			buf[COMMAND_SIZE];
	time_t     		re_time,newtime;
	struct  tm      *devtime;
	POINTER			pt;
	printf("\n\n");
	if(strcmp(passwd,"Eeeeeee")!=0)
	{
		printf("请输入密码:" );
		scanf("%s", passwd);
	}
	if(strcmp(passwd,"Eeeeeee")!=0)
	{
		printf("你没有模拟命令的权限!\n" );
		return;
	}
	system("cls"); 
	printf("\n请选择:" );
	printf("\n 1. 实际命令" );
	printf("\n 2. 仿真命令\n" );

	scanf("%d", &val);
	if(val == 1)
	{
		head.dp_type=COMM_TIME;
	}
	else if(val == 2)
	{
		head.dp_type=COMM_SIM;
	}
	else
	{
		printf("\n输入错误" );
		return;
	}
		
	printf("\n输入类型标识(不支持广播命令):" );
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
	GetSrcId(&msg.host_id);
	msg.usr_id=0;
	strcpy(msg.usr_login_name,"test");
	pt.stn_id = msg.stn_id;
	pt.dev_id = msg.dev_id;
	pt.pt_id = msg.point_id;
	pt.data_type =msg.data_type;
	if(GetPtCtrlAddrById( &pt, &msg.ctrl_addr ) == -1)
	{
		printf("GetPtCtrlAddrById eror\n");
		return ;
	}
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


/*********************# 模拟变位报警 ********************************#*/
int SendPoints()
{
	LAN_HEAD        head;
	int	        type,type_id,stn,dev,data_type,data,status,first,end;
	DMS_COMMON      common_aa;
	int 		i,j,count,pos_start,pos_end; 
	char		logicname[80];
	
	printf("\n\n");
	printf("输入类型标识:\n\n" );
        printf(" 01.ANA\n");
        printf(" 03.IMP\n");
        printf(" 04.IND\n");
        printf(" 05.LMS\n");
        printf(" 06.SMS\n");
        printf(" 13.ANA_ALM\n");
        printf(" 15.ANA_QLT_ALM\n");
        printf(" 17.IND_QLT_ALM\n");
	printf("\n请选择( 1--17 ):");
        scanf("%d", &type_id);
	if(type_id==6)
	{
		BdSms("做人要厚道!");
		printf("\nBdSms success!!");
		return TRUE;
	}	
	if(type_id==5)
	{
		printf("输入长信编号:");
        	scanf("%d", &stn);
		BdLms(stn);
		printf("\nBdLms success!!");
		return TRUE;
	}

	printf("\n\n");
	printf("选择模拟发送操作参数（1.piont_id发送 2.by_name 发送):");
	scanf("%d", &type);
	/*
	type =1;
	*/
	if(type==1)
	{
	
		printf("输入厂站号:");
		scanf("%d", &stn);
		printf("输入设备号:");
		scanf("%d", &dev);
		printf("输入数据类型：(1.模拟 3.脉冲 4.中断 5.扫查 6.综合信息 ......）:");
		scanf("%d", &data_type);
		printf("输入起始I/O输入地址(含该点):");
		scanf("%d", &first);
		printf("输入中止I/O输入地址(含该点):");
		scanf("%d", &end);
	}
	if(type==2)
	{
		printf("输入逻辑名: \n");
	        scanf("%s", &logicname[0]);
		if(ReadEntryByNameStr(logicname,&common_aa)==-1)
		{
			printf("****  %s No this point in the database file!!!\n",logicname);
			return TRUE;
		}
		else
		{
			stn = common_aa.point.stn_id;
			dev = common_aa.point.dev_id;
			data_type = common_aa.point.data_type;
			first = end = common_aa.point.pt_id;
		}

	}
/*
	printf("输入厂站号:");
	scanf("%d", &stn);
	printf("输入设备号:");
	scanf("%d", &dev);
	printf("输入数据类型：(1.模拟 3.脉冲 4.中断 5.扫查 6.综合信息 ......）:");
	scanf("%d", &data_type);
	printf("输入起始Ｉ／Ｏ输入地址(含该点):");
	scanf("%d", &first);
	printf("输入中止Ｉ／Ｏ输入地址(含该点):");
	scanf("%d", &end);
*/		
	if(data_type==4 || data_type ==5 || data_type==6 || data_type==14)
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
	status = status + 256;
	count = (end-first+1)/200 + 1;
	for(i=0;i<count;i++)
	{
		pos_start= first+i*200;
		if(i != count-1)
		{
			pos_end = pos_start + 199;
		}
		else
		{
			pos_end = end;
		}
		if( pos_start>pos_end )
		{
			break;
		}
		MultiPointPack((unsigned char)type_id,(unsigned char)stn,(unsigned char)dev,(unsigned char) data_type, 
		(unsigned short)pos_start, (unsigned short)pos_end,(unsigned short)status, data);
	}

	
	return TRUE;
}

short ii;
char 		buf[MAX_BUF_SIZE]; 
//modified by wzg 2011
void MultiPointPack(unsigned char typeid,unsigned char stn,unsigned char dev,unsigned char type,unsigned short first,unsigned short end,unsigned short status,int data)
{
	LAN_HEAD	head;
	COMMON_MSG  	msg;
	short		j,pt,num;
	DATE_TIME	devtime;
	
	head.dest_id=0;
	GetSrcId(&head.src_id);
	head.dp_type=BJ_DB_TIME;

	memset(buf,0,MAX_BUF_SIZE);
	
	GetSysTime(&devtime);

	ii= first;
	for(;ii<=end;)
	{
		msg.type_id = typeid;
		msg.stn_id = stn;	
		msg.dev_id = dev;
		msg.data_type=type;
		GetSrcId(&msg.host_id);
		
		msg.ms  =devtime.ms;
		msg.sec  =devtime.second;
		msg.min  =devtime.minute;
		msg.hour  =devtime.hour;
		msg.day  =devtime.day;
		msg.month  =devtime.month;
		msg.year  =devtime.year; 

		head.length=6;
		num=0;
		for(;ii<=end;ii++,num++)
		{
			if(head.length>=(MAX_BUF_SIZE-18))
			{
				printf(" length >MAX_BUF_SIZE\n");
				break;
			}		
			
			memcpy(buf+head.length,&ii,2);
			memcpy(buf+head.length+2,&status,2);
			Swap2Byte(&buf[head.length]);
			Swap2Byte(&buf[head.length+2]);
			
			if(msg.type_id == DPS_IND_ALM)
			{
				head.length=head.length+4;
			}
			else
			{	
				memcpy(buf+head.length+4,&data,4);
				Swap4Byte(&buf[head.length+4]);
				head.length=head.length+8;
			}
		}	
		
		msg.number=num;
		printf("\nmsg.number=%d\n",msg.number);
		
		Swap2Byte(&msg.number);	
		Swap2Byte(&msg.ms);
		Swap2Byte(&msg.year);
		
		memcpy(buf,&msg,6);
		if(typeid !=  DPS_IND_ALM  )
		{
			memcpy(buf+head.length,&msg.host_id, 8);	
			head.length=head.length+8;
		}
		else
		{	
			memcpy(buf+head.length,&msg.ms, 10);
			head.length=head.length+10;
		}
		printf("head.length=%d dp_type=%d\n",head.length ,head.dp_type);
		
		for(j=0;j<head.length;j++)
		{
			printf("%d",(unsigned char) buf[j]);
			printf("=");
		}	
		printf("\n");
		
		Swap2Byte(&head.length);
		lan_out((char *)buf,head,0);
		
	}
	return;
}

void ReadSendShm()
{
		
			sendb_ptr=0;			
			while(sendb_ptr!=shm_send_addr->head)
			{
				memcpy(prtbuf,&shm_send_addr->shm_data_seg[sendb_ptr],SHM_DATA_SEG_SIZE+8);

				printf("%2d= ",sendb_ptr);
				for(k=0;k<PRTBUFSIZE;k++)
				{
					printf("%02X ",prtbuf[k]);

					if(k%8 ==7 )
					{
						printf(" ");				
					}
				}
				length = shm_send_addr->shm_data_seg[sendb_ptr].lan_head.length;
#ifdef _BIG_ENDIAN_
				Swap2Byte(&length);				
#endif
				printf("... ");
				for(k=0;k<8;k++)
				{
					if(shm_send_addr->shm_data_seg[sendb_ptr].lan_head.dp_type == COMM_TIME || 
						shm_send_addr->shm_data_seg[sendb_ptr].lan_head.dp_type == COMM_RECORD_TIME)
					{
						printf("%02X ",prtbuf[length-8+k]);
					}
					else
					{
						printf("%02X ",prtbuf[length+k]);
					}	
					if(k%8 ==7 )
					{
						printf(" ");				
					}
				}
				printf("\n");

				sendb_ptr++;
				if(sendb_ptr>=MAX_SEND_DATA_SEG_NUM)	sendb_ptr=0;
	
			}
			printf("\nshm_send_head=%d\n\n",shm_send_addr->head);
			printf("\n\n");	
}


void ReadRecvShm()
{
			recvb_ptr=0;
			while(recvb_ptr!=shm_recv_addr->head)
			{
				memcpy(prtbuf,&shm_recv_addr->shm_data_seg[recvb_ptr],PRTBUFSIZE+8);
				
				printf("%2d= ",recvb_ptr);	
				
				for(k=0;k<PRTBUFSIZE;k++)
				{
					printf("%02X ",prtbuf[k]);
					if(k%8 ==7 )
					{
						printf(" ");				
					}
				}
				length = shm_recv_addr->shm_data_seg[recvb_ptr].lan_head.length;
#ifdef _BIG_ENDIAN_
				Swap2Byte(&length);				
#endif
				printf("... ");
				for(k=0;k<8;k++)
				{
					if(shm_recv_addr->shm_data_seg[recvb_ptr].lan_head.dp_type == COMM_TIME || 
						shm_recv_addr->shm_data_seg[recvb_ptr].lan_head.dp_type == COMM_RECORD_TIME)
					{
						printf("%02X ",prtbuf[length-8+k]);
					}
					else
					{
						printf("%02X ",prtbuf[length+k]);
					}
					if(k%8 ==7 )
					{
						printf(" ");				
					}					
				}
				printf("\n");	
				recvb_ptr++;
				if(recvb_ptr>=MAX_RECV_DATA_SEG_NUM)	recvb_ptr=0;

			}
			printf("\nshm_recv_head=%d\n\n",shm_recv_addr->head);			
			
			printf("\n\n");	
}

void ReadNetCmdShm()
{
			for(i=0;i<MAX_NETCMD_SEG_NUM;i++)
			{
			printf("i=%02d  ",i);
			for(j=0;j<SHM_NETCMD_SEG_SIZE;j++)
			{
				printf("%02X",shm_net_cmd->seg[i].net_cmd_buf.data[j]);
				if(j%5 == 4)
				{
					printf("  ");
				}
				else
				{
					printf("=");
				}
			}
			
			printf("\n");
			}
			printf("\n\nshm_net_cmd->head=%d\n",shm_net_cmd->head);
}

void ReadCommCmdShm()
{
	unsigned int stn_id;
	printf("\n\n请输入厂站号( 1--28 ):");
        scanf("%d", &stn_id);
	for(i=0;i<MAX_NETCMD_SEG_NUM;i++)
	{
		printf("i=%02d  ",i);
		for(j=0;j<SHM_NETCMD_SEG_SIZE;j++)
		{
			printf("%02X",shm_comm_cmd->stn_data[idx].seg[i].net_cmd_buf.data[j]);
			if(j%5 == 4)
			{
				printf("  ");
			}
			else
			{
				printf("=");
			}
		}
		
		printf("\n");
	}
	printf("\n\nshm_comm_cmd->head=%d\n",shm_comm_cmd->stn_data[idx].head);
}

void ReadIccpDataShm()
{
#ifdef YYP
	unsigned int idx;
	if(shm_iccp_data_addr == NULL)
	{
		printf("没有iccp_data共享区\n");
		return;
	}
	printf("\n\n请输入远端编号( 0--10 ):");
    scanf("%d", &idx);

	printf("\n\nidx%d\n",	shm_iccp_data_addr->iccp_data[idx].stn_id);
	printf("\nstn_name%s\n",	shm_iccp_data_addr->iccp_data[idx].stn_name);
	printf("\ndp_num%d\n",	shm_iccp_data_addr->iccp_data[idx].dp_num);
	printf("\nme_num%d\n",	shm_iccp_data_addr->iccp_data[idx].me_num);
#endif
	return;
}


void ReadPLCCmdShm()
{

			for(i=0;i<MAX_PLCCMD_SEG_NUM;i++)
			{
				printf("buf num =%02d   ",i);
				for(j=0;j<PLCCMD_BUF_SIZE;j++)
				{
					printf("=%04x=",shm_plc_cmd->seg[i].cmd_buf.data[j]);
				}	
		
				 printf("\n");
			}
			printf("\nhm_plc_cmd->head=%d\n\n",shm_plc_cmd->head);	
}

void ReadPLCDataShm()
{
	struct plc_data_seg_head head;
	int type;
	int iscont=0;

	char	InStr[100];
	printf("\n请输入厂站号:");
	scanf("%d", &stnid );
	idx = index_stn[stnid];
	printf("请输入设备号:");
	scanf("%d", &devid );

	do
	{	
		if(iscont ==0 )
		{

			printf("\n请输入数据区号：\n\n");
			printf(" 01.模拟量\n");
			printf(" 02.温度脉冲量\n");
			printf(" 03.扫查开关量\n");
			printf(" 04.综合信息与中断\n");
			printf(" 05.中断报警\n");
			printf(" 06.head头信息\n");
			printf(" 07.更新标志\n");
			printf(" 08.下行数据区\n");
			printf(" 09.定值上送区\n");
			printf(" 10.控制对象数据\n");
			printf(" 11.控制对象变化数据\n");
			printf(" 00.返回\n");
			printf("\n请输入选择项( 0--11 ):");
			scanf("%d", &type );
		}
		
		head = shm_plc_data->seg[ segid[idx][devid] ].head;
		switch(type)
		{				
		case 1:
			len = head.ai_size *2;
			printf("\n\nai_data: size =%d, start_addr =mw%d \n",len,head.ai_start_addr );
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].ai_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			len = ((head.ai_size-1)/16+1) *2;
			printf("\n\nai_data qlt  : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].ai_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			len = ((head.ai_size-1)/16+1) *2;
			printf("\n\nai_data en  : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].ai_en);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			printf("\n");

			len = head.ao_size *2;
			printf("\n\nao_data: size =%d  start_addr =mw%d \n",len,head.ao_start_addr );
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].ao_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			
			len = ((head.ao_size-1)/16+1) *2;
			printf("\n\nao_data qlt   : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].ao_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			
			len = ((head.ao_size-1)/16+1) *2;
			printf("\n\nao_data en  : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].ao_en);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			printf("\n");

			len = head.aic_size *2;
			printf("aic_data: size =%d start_addr =mw%d  \n",len,head.aic_start_addr );
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].aic_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			printf("\n");

			len = head.aicomm_size *2;
			printf("aicomm: size =%d start_addr =mw%d  \n",len,head.aicomm_start_addr );
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].aicomm_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			len = ((head.aicomm_size-1)/16+1) *2;
			printf("\naicomm_data qlt  : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].aicomm_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			printf("\n");
			
			len = ((head.aicomm_size-1)/16+1) *2;
			printf("\naicomm_data en  : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].aicomm_en);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			printf("\n");
			
			break;
		case 2:
			len = head.rtd_size *2;
			printf("\nrtd_data: size =%d start_addr =mw%d \n",len,head.rtd_start_addr );
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].rtd_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			len = head.rtd_size/16 *2;
			printf("\nrtd_data qlt  : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].rtd_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
        		
        		
			len = head.rtd_size/16 *2;
			printf("\nrtd_data en  : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].rtd_en);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			len = head.imp_size * 4;
			printf("\nimp_data: size =%d start_addr =mw%d  \n",len,head.imp_start_addr );
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pi_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			len = ((head.imp_size-1)/16+1) *2;
			printf("\nimp_data qlt  : size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pi_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
			}
			break;
		case 3:

			pol_seg_ptr = shm_plc_data->seg[ segid[idx][devid] ].pol_seg_ptr;  			
			printf("\n\npol_ptr  =%d \n",pol_seg_ptr);

			len = 14;
	
			printf("\npol_head buf: size =%d \n",len);
			pt = (unsigned char *)(&(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].backup));
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}			
			
			len = head.pol_size/16*2;
			pol_seg_ptr = shm_plc_data->seg[ segid[idx][devid] ].pol_seg_ptr;  			
			printf("\n\npol_data buf: size =%d  start_addr =m%d  \n",len, head.pol_start_addr+7*16 );
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].pol_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
 
 			len = head.pol_size/16*2;
			
 			printf("\npol_qlt buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].pol_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
			len = head.pol_size/16*2;
			printf("\npol_en buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].pol_en);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
  
  			len = head.dout_size/16*2;
			printf("\ndout_data buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].dout_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
			}
 
 			len = head.dout_size/16*2;
			printf("\ndout_qlt buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].dout_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
        		
			len = head.dout_size/16*2;
			printf("\ndout_en buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].dout_en);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}

			len = head.polc_size/16*2;
			printf("\npolc_data buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].polc_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
        		
			len = head.polcomm_size/16*2;
			printf("\npolcomm_data buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].polcomm_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}        		
 
 			len = head.polcomm_size/16*2;
			
 			printf("\npolcomm_qlt buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].polcomm_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
			len = head.polcomm_size/16*2;
			printf("\npolcomm_en buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_seg_ptr].polcomm_en);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
        		
        		break;

        	case 4:	        		       		
			len = head.msg_size/16*2;
			printf("\nmsg data buf: size =%d start_addr =mw%d  \n",len,head.msg_start_addr);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].msg_data);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
        		
			len = head.soe_size/16*2;
			printf("\nsoe data buf: size =%d  start_addr =mw%d \n",len,head.soe_start_addr );
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].soe_data);
			for(j=0;j<len;j++)
			{
					printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
			}   
        		
 			len = head.soe_size/16*2;
			printf("\nsoe_qlt buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].soe_qlt);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
        		}
        		
			len = head.soe_size/16*2;
			printf("\nsoe_en buf: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].soe_en);
			for(j=0;j<len;j++)
			{
					printf("=%02x=",( unsigned char ) *( pt + j));
 		           	if( ((j+1)%20)==0 )       printf("\n");
			}        		     		
        		break;
        			
		case 5:
			len = 140;
			printf("\n\nSoeAlm data: head =%d start_addr =mw%d \n",shm_plc_data->seg[ segid[idx][devid] ].soealm_ptr,
				head.soealm_start_addr );
			soeptr = shm_plc_data->seg[ segid[idx][devid] ].soealm_ptr;
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].soealm_buf);
			if(soeptr>10)
			{
				start = 14 * (soeptr-10);
				for(j=start;j<start+len;j++)
				{
 		               		printf("=%02x=",( unsigned char ) *( pt + j));
 		           		if( ((j+1)%14)==0 )       printf("\n");
				}
			}
			else
			{
				start = (MAX_SOEALM_NUM	-10 + soeptr) * 14 ;
				for(j=start;j< MAX_SOEALM_NUM  * 14;j++)
				{
					printf("=%02x=",( unsigned char ) *( pt + j));
 		           		if( ((j+1)%14)==0 )       printf("\n");
				}
        			
				start = 0;
				for(j=start;j< start + soeptr * 14;j++)
				{
						printf("=%02x=",( unsigned char ) *( pt + j));
 		           		if( ((j+1)%14)==0 )       printf("\n");
				}				
			}
			
			break;
			
		case 6:			
			/* printf("\nhead info: size =%d start_addr =mw%d \n",PLCHEADLEN, HEADSCANBASE); */
			len = PLCHEADLEN ;
			pt =(unsigned char *) & (shm_plc_data->seg[ segid[idx][devid] ].head);
			for(j=0;j<len;j++)
			{
				printf("=%02x=",  *( pt +j));
				if( ((j+1)%20)==0 )       printf("\n");
			}
			break;
        		
		case 7:

			len = 22 ;
			printf("\nupdate flag: size =%d\n",len);

			pt =(unsigned char *) & (shm_plc_data->seg[ segid[idx][devid] ].param_update_flag);
			for(j=0;j<len;j++)
			{
 		               	printf("=%02x=",  *( pt +j));
 		           	if( ((j+1)%30)==0 )       printf("\n");
        		}

			printf("\npol_seg_ptr=%d\n",shm_plc_data->seg[ segid[idx][devid] ].pol_seg_ptr);
			printf("head_thr_valid=%d\n",shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid);
			printf("endpp=%d\n",shm_plc_data->seg[ segid[idx][devid] ].endpp);
			printf("plc_data: valid =%d \n",shm_plc_data->valid);
			break;
		case 8:
			/*
			shm_plc_send->seg[1][1].send_update_flag = SENDFLAGSCADA;
			shm_plc_send->seg[1][1].pol_base = 51;
			shm_plc_send->seg[1][1].pol_size = 80;
			shm_plc_send->seg[1][1].ai_size=5;
			for(i=0;i<shm_plc_send->seg[1][1].ai_size;i++)
			{
				shm_plc_send->seg[1][1].ana_data[i*2]=0;
				shm_plc_send->seg[1][1].ana_data[i*2+1]=i+1;	
			}
			for(i=0;i<shm_plc_send->seg[1][1].pol_size/16;i++)
			{
				shm_plc_send->seg[1][1].pol_data[i*2]=0;				
				shm_plc_send->seg[1][1].pol_data[i*2+1]=i+1;	
			}
						
			printf("请输入厂站号:");
			scanf("%d", &stnid );
			printf("请输入设备号:");
			scanf("%d", &devid );
			*/
			printf("send start_addr =mw%d\n",head.send_start_addr );
			printf("send_update_flag=%d\n",shm_plc_send->seg[idx][devid].send_update_flag);
			printf("ai_size=%d\n",shm_plc_send->seg[idx][devid].ai_size);
			printf("pol_size=%d\n",shm_plc_send->seg[idx][devid].pol_size);
			printf("aiqlt_base=%d\n",shm_plc_send->seg[idx][devid].aiqlt_base);
			printf("pol_base=%d\n",shm_plc_send->seg[idx][devid].pol_base);	
			printf("\nana data >>\n\n");
			for(i=0;i<SEND_ANA_SIZE*2;i++)
			{
				printf("%02X=",(unsigned char) shm_plc_send->seg[idx][devid].ana_data[i]);
				if(i%20 ==19 )
				{
					printf("\n");				
				}
			}

			printf("\nana qlt data >>\n\n");
			for(i=0;i<SEND_ANA_QLT_WORD_SIZE*2;i++)
			{
				printf("%02X=",(unsigned char) shm_plc_send->seg[idx][devid].ana_qlt[i]);
				if(i%20 ==19 )
				{
					printf("\n");				
				}
			}

			printf("\npol data >>\n\n");
			for(i=0;i<SEND_POL_SIZE/16*2;i++)
			{
				printf("%02X=",(unsigned char)shm_plc_send->seg[idx][devid].pol_data[i]);
				if(i%20 ==19 )
				{
					printf("\n");				
				}
			}			
			break;

		case 9:
			/*
			printf("请输入厂站号:");
			scanf("%d", &stnid );
			printf("请输入设备号:");
			scanf("%d", &devid );
			*/
			printf("param_size=%d start_addr =mw%d  \n",shm_plc_data->seg[ segid[idx][devid] ].head.param_size,head.param_start_addr);
	
			printf("\nparam data >>\n\n");
			for(i=0;i<shm_plc_data->seg[ segid[idx][devid] ].head.param_size*2;i++)
			{
				printf("%02X=",shm_plc_data->seg[ segid[idx][devid] ].param_data[i]);
				if(i%10 ==9 )
				{
					printf("\n");				
				}
			}			
			break;
		case 10:
			len = head.obj_size *6;
			printf("\n\nobj_data: size =%d \n",len);
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].obj_data);
			for(j=0;j<len;j++)
			{
				printf("=%02x=",( unsigned char ) *( pt + j));
				if( ((j+1)%20)==0 )       printf("\n");
				if( ((j+1)%300)==0 )       printf("\n");
			}
			break;

		case 11:
			len = OBJ_ALM_LEN * 2 *10;
			printf("\n\nObjAlm data: head =%d, start_addr =mw%d \n",shm_plc_data->seg[ segid[idx][devid] ].objalm_ptr,head.objalm_start_addr);
			objptr = shm_plc_data->seg[ segid[idx][devid] ].objalm_ptr;
			pt = (unsigned char *)(shm_plc_data->seg[ segid[idx][devid] ].objalm_buf);
			if(objptr>10)
			{
				start = (OBJ_ALM_LEN * 2) * (objptr-10);
				for(j=start;j<start+len;j++)
				{
 		               		printf("=%02x=",( unsigned char ) *( pt + j));
 		           		if( ((j+1)%(OBJ_ALM_LEN * 2))==0 )       printf("\n");
        			}
        		}
			else
			{
				start = (MAX_OBJALM_NUM	-10 + objptr) * (OBJ_ALM_LEN * 2) ;
				for(j=start;j< MAX_OBJALM_NUM  * (OBJ_ALM_LEN * 2);j++)
				{
 		               		printf("=%02x=",( unsigned char ) *( pt + j));
 		           		if( ((j+1)%(OBJ_ALM_LEN * 2))==0 )       printf("\n");
        			}
        			
				start = 0;
				for(j=start;j< start + objptr * (OBJ_ALM_LEN * 2);j++)
				{
 		               		printf("=%02x=",( unsigned char ) *( pt + j));
 		           		if( ((j+1)%(OBJ_ALM_LEN * 2))==0 )       printf("\n");
        			}				
			}
			
			break;	
		default:
			return;		
		}/* end of switch */	
		
		if(type >=1 && type<=12)
		{
			printf("\n\n======= 重复查看该数据区？(0--否 1--是): ");
			scanf("%d",&iscont);
			gets(InStr);
		}		
		printf("\n\n");

	}while(type!=0);
				
}

void ReadLogShm()
{
	int lognum;
	char logstr[MAX_LOG_INFO_SIZE+100];			/*one log record string*/
	int	startnum;
	printf("under construction\n");
	/* needed to be modified by yyp 2011.09.19
	printf("\nthe data in log shared memory is:\n");
	printf("\nlockflag=%d",shm_log_addr->flag);
	printf("\nhead=%d\n\n",shm_log_addr->head);
	if(shm_log_addr->head >=100 )
	{
		startnum = shm_log_addr->head -100;
	}
	else
	{
		startnum = 0;
	}
	for(i=startnum;i<shm_log_addr->head;i++)
	{
		printf("i=%d	",i);
		LogRecToStr(shm_log_addr->shm_log_seg[i],logstr);
		printf("%s\n",logstr);			
	}
	*/
}

void ReadRealLogShm()
{
	int lognum;
	char logstr[MAX_LOG_INFO_SIZE+100];			/*one log record string*/
	int type;
	/*
	printf("\nthe data in log shared memory is:\n");
	printf("\nlockflag=%d",shm_log_addr->flag);
	printf("\nhead=%d\n\n",shm_log_addr->head);
	*/

	printf("\n\n");
	printf("\n========= 请选择 =========\n\n");
	printf("     00.All\n");
	printf("     01.PLCSCAN\n");
	printf("     02.PLCCMD\n");
	printf("     03.PLC\n");
	printf("     04.PLCLIB\n");
	printf("     05.PLCMON\n");
	printf("     06.MBXSCAN\n");
	printf("     08.LAN_INB\n");
	printf("     09.LAN_OUTB\n");
	printf("     10.LANLIB\n");
	printf("     28.HDB\n");

	printf("\n===== 请输入选择项( 0--100 ):");	
	scanf("%d", &type );
	type = 0;
	i=0;
	printf("under construction\n");
	/* needed to be modified by yyp 2011.09.19 
	for(;;)
	{
		while(i!=shm_log_addr->head)
		{
			if(shm_log_addr->shm_log_seg[i].appid == type || type ==0 )
			{
				printf("i=%d	",i);
				LogRecToStr(shm_log_addr->shm_log_seg[i],logstr);
				printf("%s\n",logstr);
			}
			i++;
			if(i==MAX_LOG_NUM)
			{
				i=0;
			}
		}
		MySleep(1000);
	}
	*/ 
}

void WriteLog()
{
	int lognum;
	char logstr[MAX_LOG_INFO_SIZE+100];			/*one log record string*/
	printf("\n请输入您要写入的日志总数: ");
	scanf("%d",&lognum);
	printf("\n");
	for(i=0;i<lognum;i++)
	{
		log_printf(H9000_LOG_ERROR,1,0,"hangs up! money or life?");	
		MySleep(100);
	}		
}
