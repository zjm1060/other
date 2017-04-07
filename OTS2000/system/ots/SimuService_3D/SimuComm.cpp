#include "SimuComm.h"

//int BdCmdByNameStr(char *ptNameStr, unsigned char type_id, USHORT cmd_stat, float fvalue)
int BdCmdByNameStr(char *ptNameStr, unsigned char type_id, USHORT cmd_stat)
{
	DMS_COMMON	dms_comm;
	LAN_HEAD	lanhead;
	COMMAND_MSG	cmd_buf;
	struct	tm	tmpt;
	time_t		tim;
	char			*pt;
	char			buf[10];

	if(ReadEntryByNameStr(ptNameStr, &dms_comm) ==-1) return FALSE;

	cmd_buf.type_id = type_id;
	cmd_buf.stn_id = dms_comm.point.stn_id;
	cmd_buf.dev_id = dms_comm.point.dev_id;
	cmd_buf.data_type = dms_comm.point.data_type;
	cmd_buf.point_id = dms_comm.point.pt_id;

	if(type_id== DPS_IOST_SET)
	{	cmd_buf.status.opr=IOST_SET_TAG_STATE;
	cmd_buf.status.state= cmd_stat;
	}
	else
	{	
		cmd_buf.status.opr=cmd_stat;
		if (type_id==C_GATE &&cmd_stat==15)  /*闸门、阀开度控制命令*/
			cmd_buf.status.state=0;
		else
			cmd_buf.status.state=1;
		strcpy(pt,ptNameStr);
		strcat(pt,"_");
		sprintf(buf,"%d",cmd_buf.status.opr);
		strcat(pt,buf);
	/*	if(InterLock_Judge(pt)!=1)
		{
			printf("闭锁条件不满足\n");
			return FALSE;
		}*/
	}

	cmd_buf.status.result=0;
	cmd_buf.status.src=4;/**命令来源，如"三维系统"*/
	//if (type_id==C_GATE &&cmd_stat==15)  /*闸门、阀开度控制命令*/
	//	cmd_buf.data.iValue=(int)(value*1000);
	//else
		cmd_buf.data.iValue=0;
	cmd_buf.ctrl_addr=dms_comm.fixed.ctrl_addr;
	GetLocalHostId(&cmd_buf.host_id);
	cmd_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	cmd_buf.second=tmpt.tm_sec;
	cmd_buf.minute=tmpt.tm_min;
	cmd_buf.hour=tmpt.tm_hour;
	cmd_buf.day=tmpt.tm_mday;
	cmd_buf.month=tmpt.tm_mon+1;
	cmd_buf.year=tmpt.tm_year+1900;
	strcpy(cmd_buf.usr_login_name,"3D");
	GetSrcId(&cmd_buf.host_id);
	lanhead.length=sizeof(COMMAND_MSG);
	if( type_id==C_HOST_SYS ||type_id==C_SYS_MODE_SET||type_id==C_NET_SWITCH || type_id== DPS_IOST_SET)
	{
		lanhead.dest_id	= 0;
		lanhead.dest_stn	= 0;
		lanhead.dp_type	= COMM_RECORD_TIME;
	}
	else
	{
		lanhead.dest_id 	= cmd_buf.dev_id;
		lanhead.dest_stn	= cmd_buf.stn_id;
		lanhead.dp_type	= COMM_RECORD_TIME;//COMM_SIM;
	}	
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);

	return TRUE;
}
