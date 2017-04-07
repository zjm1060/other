/*******************************************************************************************************
FILENAME:	plclib.c
FUNCTION:	
	
	  the source code file of plc communication module,whether the plc type
	is "GE" or "QUANTUM".include:

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2005.11.1	start to modify the code for H9000 V4.0		yyp
********************************************************************************************************/

#include "../inc/plchead_all.h"

/********************************************************************************************************
 get config info of each plc from hosts.def
*********************************************************************************************************/

int GetPlcConf(int type)
{
	unsigned char i,devid,idx,k;
	int sys_plc_type;
	char log_str[256];

	sys_plc_type = -1;
	for(i=1;i<=g_stn_num;i++)
	{
		UCHAR stnid;
		GetStnIdByIndex(i, &stnid);
		idx = i;
		for(devid=1;devid<=g_dev_num[idx];devid++)
		{
			plconf[idx][devid].stnid=stnid;
			plconf[idx][devid].devid=devid;
			GetGroupTypeNameById( stnid,devid, plconf[idx][devid].typestr );
			if(MyStrCmp((char *)plconf[idx][devid].typestr,"QUANTUM" )!=0
				&& MyStrCmp((char *)plconf[idx][devid].typestr,"PREMIUM" )!=0
				&& MyStrCmp((char *)plconf[idx][devid].typestr,"GE" )!=0 
				&& MyStrCmp((char *)plconf[idx][devid].typestr,"AB" )!=0 
				&& MyStrCmp((char *)plconf[idx][devid].typestr,"SIEMENS" )!=0 
				 )
			{
				continue;
			}
			if( g_cascade != YES )
			{
				plconf[idx][devid].ipnum=g_plc_ip_num[idx][devid];
				if(plconf[idx][devid].ipnum<=0 || plconf[idx][devid].ipnum>4)
				{
					sprintf(log_str,"plc %d.%d ipnum =%d error",stnid,devid,plconf[idx][devid].ipnum);
					log_printf(H9000_LOG_INFO,2,0,log_str);				
					return FALSE;
				}
				if(type == 0)
				{
					if(plconf[idx][devid].ipnum>2) plconf[idx][devid].ipnum=2;	
				}

				GetGroupNameById( stnid, devid, plconf[idx][devid].name );
				strcpy(plconf[idx][devid].ipaddr[0],g_plc_ip[idx][devid][0]);
				strcpy(plconf[idx][devid].ipaddr[1],g_plc_ip[idx][devid][1]);
				strcpy(plconf[idx][devid].ipaddr[2],g_plc_ip[idx][devid][2]);
				strcpy(plconf[idx][devid].ipaddr[3],g_plc_ip[idx][devid][3]);
				
				plconf[idx][devid].scan_host_num=g_plcscan_host_num[idx][devid];
				for(k=0;k<=plconf[idx][devid].scan_host_num;k++)
				{
					strcpy(plconf[idx][devid].scan_host[k],g_plcscan_host[idx][devid][k]);
				}
			}
			if(MyStrCmp((char *)plconf[idx][devid].typestr,"GE")==0)
			{
				plconf[idx][devid].port=18245;
			}
			else if(MyStrCmp((char *)plconf[idx][devid].typestr,"QUANTUM" )==0
				|| MyStrCmp((char *)plconf[idx][devid].typestr,"PREMIUM" )==0 )
			{
				plconf[idx][devid].port=502;
			}
			else if(MyStrCmp((char *)plconf[idx][devid].typestr,"AB" )==0)
			{
				plconf[idx][devid].port=44818;
			}
			if(MyStrCmp(plconf[idx][devid].typestr,"QUANTUM")==0) 	
			{
				plconf[idx][devid].plctype=MBPLC;
			}
			else if(MyStrCmp(plconf[idx][devid].typestr,"GE")==0)
			{
				plconf[idx][devid].plctype=GEPLC;
			}
			else if(MyStrCmp(plconf[idx][devid].typestr,"PREMIUM")==0)
			{
				plconf[idx][devid].plctype=PREPLC;
			}
			else if(MyStrCmp(plconf[idx][devid].typestr,"SIEMENS")==0)
			{
				plconf[idx][devid].plctype=SIEPLC;
			}
			else if(MyStrCmp(plconf[idx][devid].typestr,"AB")==0)
			{
				plconf[idx][devid].plctype=ABPLC;
			}			
			else
			{
				plconf[idx][devid].plctype=UNKNOWPLC;
			}
			if(sys_plc_type == -1)
			{
				sys_plc_type = plconf[idx][devid].plctype;
			}
			if(sys_plc_type != plconf[idx][devid].plctype && sys_plc_type != MIXPLC)
			{
				sys_plc_type = MIXPLC;
			}
			//modified by yyp 09.02.13
			plconf[idx][devid].cpu_net_mode = (unsigned char)g_cpu_net_mode[idx][devid];
			if(plconf[idx][devid].cpu_net_mode == CPU_D_NET_D || plconf[idx][devid].cpu_net_mode == CPU_D_NET_D_2
			 	|| plconf[idx][devid].cpu_net_mode == CPU_D_NET_S )
			{
				plconf[idx][devid].isdualcpu = TRUE;
			}
			else
			{
				plconf[idx][devid].isdualcpu = FALSE;
			}
			plconf[idx][devid].valid=1;
		}
	}
	g_plc_mode = sys_plc_type;
	return(TRUE);
}

/********************************************************************************************************
 find whether cur cmd needs to be sent to plc or not
*********************************************************************************************************/

int IsCmdSendPlc(LCUID plcid,unsigned char typeid)
{
	switch(typeid)
	{
		case C_LCU_ONOFF: 
		
			return FALSE;
			break;
		/*			
		case C_NET_SWITCH:
		
			return FALSE;
			break;
			
		case C_DATA_POLLING:
		
			return FALSE;
			break;
			
		case C_OP_VALUE_SET:
		
			return FALSE;
			break;

		case DPS_IOATTR_SET:
		
			return FALSE;
			break;

		case DPS_IOST_SET:
		
			return FALSE;
			break;

		case DPS_MAN_VALUE_SET:
		
			return FALSE;
			break;

		case DPS_MAN_STAT_SET:
		
			return FALSE;
			break;

		case DPS_MAN_CURVE_SET:
		
			return FALSE;
			break;

		case C_SYS_MODE_SET:
		
			return FALSE;
			break;

		case DPS_ANA_PARAM_SET:
		
			return FALSE;
			break;

		case DPS_IMP_PARAM_SET:
		
			return FALSE;
			break;

		case DPS_C_ONOFF_SET:
		
			return FALSE;
			break;
		*/	
		default:
			break;
	}
	return(TRUE);
}

/********************************************************************************************************
 broadcast current plc cpu run state
*********************************************************************************************************/
short BdPlcCpuStat(LCUID plcid,unsigned char cpuid, short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	unsigned short	status;
	char log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	if(GetStnNameById(stnid,tmp)==-1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
	GetGroupNameById(stnid,devid,tmp);
	strcat(buf,tmp);
	strcat(buf,".SYS.");
	if(cpuid != CPUB)
	{
		strcat(buf,"CPUA_STA");
	}
	else
	{
		strcat(buf,"CPUB_STA");		
	}		
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	sprintf(log_str,"BdPlcCpuStat(): state =%d,status =%d, %s",state,status,buf);
	log_printf(H9000_LOG_INFO,3,0,log_str);
	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	return(TRUE);
}

/********************************************************************************************************
 broadcast current plc cpu run state
*********************************************************************************************************/
short BdPlcCpuStatQlt(LCUID plcid,unsigned char cpuid, short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	POINTER		pt;
	stnid=plcid.stnid;
	devid=plcid.devid;
	if(GetStnNameById(stnid,tmp)==-1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
	GetGroupNameById(stnid,devid,tmp);
	strcat(buf,tmp);
	strcat(buf,".SYS.");
	if(cpuid != CPUB)
	{
		strcat(buf,"CPUA_STA");
	}
	else
	{
		strcat(buf,"CPUB_STA");		
	}		
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	/* needed to be modified by yyp 07.04.19 */
	BdcastQlt(pt,state);
	return(TRUE);
}

/********************************************************************************************************
 broadcast current plc net card state
*********************************************************************************************************/

short BdPlcNetStat(LCUID plcid,unsigned char cpuid, unsigned char netid, short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,idx,devid;
	POINTER		pt;
	unsigned short	status;
	char log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];
	if(GetStnNameById(stnid,tmp)==-1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
	GetGroupNameById(stnid,devid,tmp);
	strcat(buf,tmp);
	strcat(buf,".SYS.");
	if(plconf[idx][devid].cpu_net_mode != CPU_D_NET_D)
	{
		if(netid!=NETB)
			strcat(buf,"CPU_NETA");
		else
			strcat(buf,"CPU_NETB");		
	}
	else
	{
		if(cpuid != CPUB)
		{
			if(netid != NETB)
				strcat(buf,"CPUA_NETA");
			else
				strcat(buf,"CPUA_NETB");
		}
		else
		{
			if(netid != NETB)
				strcat(buf,"CPUB_NETA");
			else
				strcat(buf,"CPUB_NETB");		
		}
	}

	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);

	sprintf(log_str,"BdPlcNetStat(): state =%d,status =%d, %s",state,status,buf);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	return(TRUE);
}

/********************************************************************************************************
 broadcast current plc communication state  of localhost
*********************************************************************************************************/

short BdCommStat(LCUID plcid,short state)
{
	char		tmp[100],buf[100];
	unsigned char stnid,idx,devid;
	POINTER		pt;
	unsigned short	status,ctrlstatus0,ctrlstatus1;
	char log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx=index_stn[stnid];
	/* added by yyp 07.04.22 */
	GetHostCtrlNetState(g_hostname, 0, &ctrlstatus0);
	GetHostCtrlNetState(g_hostname, 1, &ctrlstatus1);		
	if(ctrlstatus0 !=0 || ctrlstatus1!=0)
	{
		return(FALSE);
	}
	if(GetStnNameById(stnid,tmp)==-1) return(FALSE);
	strcpy(buf,tmp);
	strcat(buf,".");
	GetGroupNameById(stnid,devid,tmp);
	strcat(buf,tmp);
	strcat(buf,".SYS.");
	if(plconf[idx][devid].cpu_net_mode != CPU_D_NET_D)
	{
		
		if(runvar[idx][devid].hotnet!=NETB)
			
			strcat(buf,"CPU_NETA");
		
		else
			
			strcat(buf,"CPU_NETB");		
		
	}
	else
	{
		if(runvar[idx][devid].hotcpu!=CPUB)
		{
			if(runvar[idx][devid].hotnet!=NETB)
				strcat(buf,"CPUA_NETA");
			else
				strcat(buf,"CPUA_NETB");
		}
		else
		{
			if(runvar[idx][devid].hotnet!=NETB)
				strcat(buf,"CPUB_NETA");
			else
				strcat(buf,"CPUB_NETB");		
		}	
	}
	if(GetPtIdByNameStr(buf,&pt)==-1) return(FALSE);
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(FALSE);
	sprintf(log_str,"BdCommStat(): state =%d,status =%d, %s",state,status,buf);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	if( status != state)
	{
		BdcastMsg(stnid,devid,SYSMSG_TYPE,pt.pt_id,state);
	}
	return(TRUE);
}

/********************************************************************************************************
 broadcast each cmd sending error in shm_net_cmd
*********************************************************************************************************/
int BdAllCmdErr(int type,LCUID plcid)
{
	unsigned char stnid,idx,devid;
	short	curptr;	
	struct net_cmd_seg netcmdseg;
	stnid = plcid.stnid;
	devid = plcid.devid;
	idx=index_stn[stnid];
	curptr =  runvar[idx][devid].netcmdptr;	
	while( curptr != shm_net_cmd->head )
	{	
		netcmdseg = shm_net_cmd->seg[curptr];
		if(netcmdseg.net_cmd_buf.netdata.stn_id==stnid
			&& netcmdseg.net_cmd_buf.netdata.dev_id==devid)
		{
#ifdef _BIG_ENDIAN_
			Swap2Byte( &(netcmdseg.lan_head.length));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.point_id));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.status));
			Swap4Byte( &(netcmdseg.net_cmd_buf.netdata.data));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.ctrl_addr));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.year));
#endif
			BdCmdSendResult(netcmdseg, CMD_SENDERR);
		}
		curptr++;
		if(curptr>=MAX_NETCMD_SEG_NUM) 
		{
			curptr=0;
		}
		runvar[idx][devid].netcmdptr = curptr;
	}
	return TRUE;
}

/********************************************************************************************************
 broadcast each cmd sending error in shm_net_cmd
*********************************************************************************************************/

int BdCurCmdErr(LCUID plcid)
{
	unsigned char stnid,idx,devid;
	short	curptr;	
	struct net_cmd_seg netcmdseg;
	stnid = plcid.stnid;
	devid = plcid.devid;
	idx=index_stn[stnid];
	curptr =  runvar[idx][devid].netcmdptr;	
	
		netcmdseg = shm_net_cmd->seg[curptr];
		if(netcmdseg.net_cmd_buf.netdata.stn_id==stnid
			&& netcmdseg.net_cmd_buf.netdata.dev_id==devid)
		{
#ifdef _BIG_ENDIAN_
			Swap2Byte( &(netcmdseg.lan_head.length));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.point_id));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.status));
			Swap4Byte( &(netcmdseg.net_cmd_buf.netdata.data));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.ctrl_addr));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.year));
#endif
			BdCmdSendResult(netcmdseg, CMD_SENDERR);
		}
		curptr++;
		if(curptr>=MAX_NETCMD_SEG_NUM) 
		{
			curptr=0;
		}
		runvar[idx][devid].netcmdptr = curptr;	
		return TRUE;
}


int GetHeadStat(struct plc_data_seg_head head,unsigned char stnid ,unsigned char devid)
{
	unsigned 	char	host,cpua_state,cpub_state;
	char		host_firstrun[2];
	LCUID		plcid;
	unsigned char idx;
	char log_str[256];
	plcid.stnid = stnid;
	plcid.devid = devid;
	idx=index_stn[stnid];
	if(head.obj_size < 0 || head.obj_size > OBJ_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: obj_size =%d",stnid,devid,head.obj_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}
	if(head.ai_size < 0 || head.ai_size > AI_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: ai_size =%d",stnid,devid,head.ai_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}
	if(head.aic_size < 0 || head.aic_size > AIC_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: aic_size =%d",stnid,devid,head.aic_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}
	if(head.aicomm_size < 0 || head.aicomm_size > AICOMM_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: aicomm_size =%d",stnid,devid,head.aicomm_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}

	if(head.rtd_size < 0 || head.rtd_size > RTD_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: rtd_size =%d",stnid,devid,head.rtd_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}

	if(head.imp_size < 0 || head.imp_size > PI_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: imp_size =%d",stnid,devid,head.imp_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}
	if(head.impcomm_size < 0 || head.impcomm_size > PICOMM_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: impcomm_size =%d",stnid,devid,head.impcomm_size);
		log_printf(H9000_LOG_ERROR,2,0,log_str);
		return NO;
	}

	if(head.ao_size < 0 || head.ao_size > AO_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: ao_size =%d",stnid,devid,head.ao_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}
	
	if(head.soe_size < 0 || head.soe_size > DII_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: soe_size =%d",stnid,devid,head.soe_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}

	if(head.msg_size < 0 || head.msg_size > MSG_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: msg_size =%d",stnid,devid,head.msg_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}

	if(head.dout_size < 0 || head.dout_size > DOUT_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: dout_size =%d",stnid,devid,head.dout_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}

	if(head.pol_size < 0 || head.pol_size > DIN_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: pol_size =%d",stnid,devid,head.pol_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}		

	if(head.polc_size < 0 || head.polc_size > DIC_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: polc_size =%d",stnid,devid,head.polc_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}
	
	if(head.polcomm_size < 0 || head.polcomm_size > DICOMM_SIZE) 
	{
		sprintf(log_str,"==== plc %d.%d err: polcomm_size =%d",stnid,devid,head.polcomm_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}	
	if(head.param_size <0 || head.param_size > PARAM_SIZE ) 
	{
		sprintf(log_str,"==== plc %d.%d err: param_size =%d",stnid,devid,head.param_size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return NO;
	}

	if(plconf[idx][devid].plctype == SIEPLC)
	{
		if(head.ai_start_addr != ANABASE_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: ai_start_addr =%d",stnid,devid,head.ai_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(head.pol_start_addr != SWBASE_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: pol_start_addr =%d",stnid,devid,head.pol_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(head.obj_start_addr != OBJBASE_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: obj_start_addr =%d",stnid,devid,head.obj_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(head.cmd_start_addr != CMDBASE_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: cmd_start_addr =%d",stnid,devid,head.cmd_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		if(head.cmd_size != CMDSIZE_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: cmd_size =%d",stnid,devid,head.cmd_size);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}			
		if(head.soealm_start_addr != SOEALMBASE_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_start_addr =%d",stnid,devid,head.soealm_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		if(head.soealm_max != SOEALMMAX_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_max =%d",stnid,devid,head.soealm_max);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		
		if(head.soealm_ptr >= SOEALMMAX_SIE  || head.soealm_ptr<0) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_ptr  =%d",stnid,devid,head.soealm_ptr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}	
		if(head.param_start_addr != PARAMBASE_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: param_start_addr =%d",stnid,devid,head.param_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);		
			return NO;
		}
		

		
		if(head.send_start_addr != SENDBASE_SIE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: send_start_addr =%d",stnid,devid,head.send_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
	}
	else if(plconf[idx][devid].plctype == MBPLC
		|| plconf[idx][devid].plctype==PREPLC)
	{
		if(head.ai_start_addr != ANABASE_MB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: ai_start_addr =%d",stnid,devid,head.ai_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(plconf[idx][devid].plctype == MBPLC)
		{
			if(head.pol_start_addr != SWBASE_MB ) 
			{
				sprintf(log_str,"==== plc %d.%d err: pol_start_addr =%d",stnid,devid,head.pol_start_addr);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				return NO;
			}
		}
		else
		{
			if(head.pol_start_addr != SWBASE_PRE ) 
			{
				sprintf(log_str,"==== plc %d.%d err: pol_start_addr =%d",stnid,devid,head.pol_start_addr);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				return NO;
			}			
		}
		if(head.obj_start_addr != OBJBASE_MB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: obj_start_addr =%d",stnid,devid,head.obj_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(head.cmd_start_addr != CMDBASE_MB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: cmd_start_addr =%d",stnid,devid,head.cmd_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		if(head.cmd_size != CMDSIZE_MB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: cmd_size =%d",stnid,devid,head.cmd_size);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}			
		if(head.soealm_start_addr != SOEALMBASE_MB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_start_addr =%d",stnid,devid,head.soealm_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		if(head.soealm_max != SOEALMMAX_MB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_max =%d",stnid,devid,head.soealm_max);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		
		if(head.soealm_ptr >= SOEALMMAX_MB  || head.soealm_ptr<0) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_ptr  =%d",stnid,devid,head.soealm_ptr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}	
		if(head.param_start_addr != PARAMBASE_MB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: param_start_addr =%d",stnid,devid,head.param_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);		
			return NO;
		}
		
		if(head.send_start_addr != SENDBASE_MB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: send_start_addr =%d",stnid,devid,head.send_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}		
	}
	else if(plconf[idx][devid].plctype == GEPLC)
	{
		if(head.ai_start_addr != ANABASE_GE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: ai_start_addr =%d",stnid,devid,head.ai_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(head.obj_start_addr != OBJBASE_GE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: obj_start_addr =%d",stnid,devid,head.obj_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(head.cmd_start_addr != CMDBASE_GE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: cmd_start_addr =%d",stnid,devid,head.cmd_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		if(head.cmd_size != CMDSIZE_GE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: cmd_size =%d",stnid,devid,head.cmd_size);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}			
		if(head.soealm_start_addr != SOEALMBASE_GE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_start_addr =%d",stnid,devid,head.soealm_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		if(head.soealm_max != SOEALMMAX_GE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_max =%d",stnid,devid,head.soealm_max);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		
		if(head.soealm_ptr >= SOEALMMAX_GE  || head.soealm_ptr<0) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_ptr  =%d",stnid,devid,head.soealm_ptr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}	
		if(head.param_start_addr != PARAMBASE_GE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: param_start_addr =%d",stnid,devid,head.param_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);		
			return NO;
		}
	
		if(head.send_start_addr != SENDBASE_GE ) 
		{
			sprintf(log_str,"==== plc %d.%d err: send_start_addr =%d",stnid,devid,head.send_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}		
	}
	else if(plconf[idx][devid].plctype == ABPLC)
	{
		if(head.ai_start_addr != ANABASE_AB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: ai_start_addr =%d",stnid,devid,head.ai_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(head.obj_start_addr != OBJBASE_AB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: obj_start_addr =%d",stnid,devid,head.obj_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return NO;
		}
		if(head.cmd_start_addr != CMDBASE_AB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: cmd_start_addr =%d",stnid,devid,head.cmd_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		if(head.cmd_size != CMDSIZE_AB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: cmd_size =%d",stnid,devid,head.cmd_size);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}			
		if(head.soealm_start_addr != SOEALMBASE_AB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_start_addr =%d",stnid,devid,head.soealm_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		if(head.soealm_max != SOEALMMAX_AB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_max =%d",stnid,devid,head.soealm_max);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}
		
		if(head.soealm_ptr >= SOEALMMAX_AB  || head.soealm_ptr<0) 
		{
			sprintf(log_str,"==== plc %d.%d err: soealm_ptr  =%d",stnid,devid,head.soealm_ptr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}	
		if(head.param_start_addr != PARAMBASE_AB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: param_start_addr =%d",stnid,devid,head.param_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);		
			return NO;
		}
	
		if(head.send_start_addr != SENDBASE_AB ) 
		{
			sprintf(log_str,"==== plc %d.%d err: send_start_addr =%d",stnid,devid,head.send_start_addr);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return NO;
		}		
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",stnid,devid,plconf[idx][devid].plctype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return NO;
	}

	memcpy(&host_firstrun,&head.host_firstrun,2);
#ifdef _BIG_ENDIAN_
	host = host_firstrun[0];
#else
	host = host_firstrun[1];
#endif
	if(plconf[idx][devid].isdualcpu==YES)
	{
		if(host ==4 )
		{
			cpua_state = CPURUN;
			cpub_state = CPUBACKUP;
		}
		else if(host ==1 )
		{
			cpub_state = CPURUN;
			cpua_state = CPUBACKUP;
		}
		else if(host ==2 )
		{
			cpua_state = CPURUN;
			cpub_state = CPUSTOP;
		}
		else if(host ==3 )
		{
			cpub_state = CPURUN;
			cpua_state = CPUSTOP;
		}
		else if(host ==0 )
		{
			cpua_state = CPUSTOP;
			cpub_state = CPUSTOP;
			sprintf(log_str,"==== plc %d.%d err: host =%d",stnid,devid,host);
			log_printf(H9000_LOG_ERROR,2,0,log_str);
		}
		else
		{
			sprintf(log_str,"==== plc %d.%d err: host =%d",stnid,devid,host);
			log_printf(H9000_LOG_ERROR,2,0,log_str);	
			return NO;	
		}
		if((host == 1) || (host == 3) ) 
		{
			host =1;
		}
		else
		{
			host=0;
		}
	}
	else
	{
		if(host ==2 )
		{
			cpua_state = CPURUN;
		}
		else
		{
			cpua_state = CPUSTOP;
		}
		host = 0;
	}
	if( runvar[idx][devid].cpua_state != cpua_state )
	{
		sprintf(log_str,"==== plclib.c: plc %d.%d cpua_state =%d changed\n",
			stnid,devid,host, cpua_state );
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		runvar[idx][devid].cpua_state = cpua_state;
	}
	/* modified by yyp 2008.12.04 */
	if(runvar[idx][devid].cpu_flag != head.cpuflag)
	{	
		BdPlcCpuStat(plcid,CPUA,runvar[idx][devid].cpua_state);	
	}
	if(plconf[idx][devid].isdualcpu==YES)
	{
		if( runvar[idx][devid].cpub_state != cpub_state )
		{
			sprintf(log_str,"==== plclib.c: plc %d.%d cpub_state =%d changed\n",
				stnid,devid,host, cpub_state );
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			runvar[idx][devid].cpub_state = cpub_state;
		}
		/* modified by yyp 2008.12.04 */
		if(runvar[idx][devid].cpu_flag != head.cpuflag)
		{
			BdPlcCpuStat(plcid,CPUB,runvar[idx][devid].cpub_state);	
		}
	}
	if( runvar[idx][devid].cpu_flag != head.cpuflag )
	{
		if( runvar[idx][devid].hotcpu!=host )
		{
			sprintf(log_str,"==== plclib.c: GetHeadStat plc %d.%d head.host =%d differ from  hotcpu =%d \n",
			stnid,devid,host, runvar[idx][devid].hotcpu );
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			runvar[idx][devid].hotcpu = host;
			BdCpuHost(plcid, host);
			return NO;
		}
		/*
		if(host == 0)
		{
			if(runvar[idx][devid].cpua_state != CPURUN)
			{
				sprintf(log_str,"==== err:  plc %d.%d the cpu A connected is not the primary ",
				stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);				
				runvar[idx][devid].hotcpu=1;
				return NO;
			}
		}
		if(host == 1)
		{
			if(runvar[idx][devid].cpub_state != CPURUN)
			{
				sprintf(log_str,"==== err:  plc %d.%d the cpu B connected is not the primary ",
				stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				runvar[idx][devid].hotcpu=0;			
				return NO;								
			}
		}
		*/

	}	

	return YES;		
}

/********************************************************************************************************
 Find whether the host is the primary scan host in the scan host list
*********************************************************************************************************/
short   GetPLCScanPrvg(LCUID plcid,int *p_err)
{
	short           i,status;
	char		hostname[40];
	unsigned char	stnid,idx,devid;
	int		scanid;
	char log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];	
	if(g_plcscan_host_num[idx][devid] <=0  || g_plcscan_host_num[idx][devid]>= MAX_SCAN_HOST_NUM )
	{
		sprintf(log_str,"==== plc %d.%d scan host num =%d err!\n",stnid,devid,g_plcscan_host_num[idx][devid]);
		log_printf(H9000_LOG_ERROR,2,0,log_str);
		return (CUR_WS_IN_VICE);
	}	
	gethostname(hostname,40);
	for(scanid=0; scanid<g_plcscan_host_num[idx][devid]; scanid++)
	{
		if ( strcmp ( hostname , g_plcscan_host[idx][devid][scanid]) ==0 )
			break;
	}
	if(scanid == g_plcscan_host_num[idx][devid])
	{
		*p_err = 1;
		return ( CUR_WS_IN_VICE); 
	}	
	
	for(i=0;i<scanid;i++)
	{
		status = GetHostLinkStat(g_plcscan_host[idx][devid][i]);
		if(status != NET_ERR)
		{
			if(status == NET_OK)
			{
				WriteScanHost(plcid,g_plcscan_host[idx][devid][i]);
			}
			*p_err = 2;
			return ( CUR_WS_IN_VICE); 	
		}
	}
	
	status = GetHostLinkStat(g_plcscan_host[idx][devid][scanid]);
	if(status != NET_OK)
	{
		*p_err = 3;		
		return ( CUR_WS_IN_VICE);
	}
	
	return ( CUR_WS_IN_HOST);
}

/********************************************************************************************************
 get the host link stat ,if linked ,return 0, else return 1
*********************************************************************************************************/

short GetHostLinkStat(char * hostname)
{
	char log_str[256];

	unsigned short sysstatus[2],ctrlstatus[2],status,hoststate;
	if(GetHostState(hostname, &hoststate) == -1 )
	{
		sprintf(log_str,"plclib.c: GetHostState %s return -1,error!\n",hostname);
		log_printf(H9000_LOG_ERROR,2,0,log_str);
		return NET_ERR;
	}
	
	sysstatus[0] = sysstatus[1] =1;
	ctrlstatus[0] = ctrlstatus[1] =1;
	
	if(GetHostSysNetState(hostname, 0, &status) != -1)
	{
		sysstatus[0] =status ;
	}
	else
	{
		return -1;
	}
	
	if(g_sys_dualnet == YES )
	{
		if(GetHostSysNetState(hostname, 1, &status) != -1)
		{
			sysstatus[1] =status ;
		}
		else
		{
			return -1;
		}		
	}
	if(g_ctrl_net_independ == TRUE)
	{	
	if(GetHostCtrlNetState(hostname, 0, &status) != -1)
	{
		ctrlstatus[0] =status ;
	}
	else
	{
		return -1;
	}
		
	if(g_ctrl_dualnet == YES )
	{
		if(GetHostCtrlNetState(hostname, 1, &status) != -1)
		{
			ctrlstatus[1] =status ;
		}
		else
		{
			return -1;
		}		
	}
	}
	if(g_ctrl_net_independ == TRUE)
	{		

		if((sysstatus[0] ==0 || sysstatus[1] ==0) && (ctrlstatus[0] ==0 || ctrlstatus[1] ==0) 
		&& (hoststate == 1 || hoststate == 2) )	
		{
			return NET_OK;
		}
	}
	else
	{
		if((sysstatus[0] ==0 || sysstatus[1] ==0) && (hoststate == 1 || hoststate == 2) )	
		{
			return NET_OK;
		}	
	}
	return NET_ERR;
}


/********************************************************************************************************
 Find whether the host is the primary mon host in the scan host list
*********************************************************************************************************/
short   GetPLCMonPrvg(unsigned char stnid)
{
	short           i,status;
	char		hostname[40];
	unsigned char	idx,devid;
	LCUID		plcid;
	int		scanid;
	char log_str[256];
	
	plcid.stnid = stnid;
	plcid.devid = 1;
	devid=plcid.devid;
	idx=index_stn[stnid];	
	if(g_plcscan_host_num[idx][devid] <=0  || g_plcscan_host_num[idx][devid]>= MAX_SCAN_HOST_NUM )
	{
		sprintf(log_str,"==== plc %d.%d scan host num =%d err!\n",stnid,devid,g_plcscan_host_num[idx][devid]);
		log_printf(H9000_LOG_ERROR,2,0,log_str);
		return (CUR_WS_IN_VICE);
	}	
	gethostname(hostname,40);
	for(scanid=0; scanid<g_plcscan_host_num[idx][devid]; scanid++)
	{
		if ( strcmp ( hostname , g_plcscan_host[idx][devid][scanid]) ==0 )
			break;
	}
	if(scanid == g_plcscan_host_num[idx][devid])
	{
		sprintf(log_str,"==== host %s is not in plc %d.%d scan host list!\n",hostname,stnid,devid);
		log_printf(H9000_LOG_ERROR,3,0,log_str);
		return ( CUR_WS_IN_VICE); 
	}	
	
	for(i=0;i<scanid;i++)
	{
		status = GetHostNetStat(g_plcscan_host[idx][devid][i]);
		if(status != NET_ERR)
		{
			return ( CUR_WS_IN_VICE); 	
		}
	}
	
	status = GetHostNetStat(g_plcscan_host[idx][devid][scanid]);
	if(status != NET_OK)
	{
		return ( CUR_WS_IN_VICE); 	
	}
	
	return ( CUR_WS_IN_HOST);
}

short GetHostNetStat(char * hostname)
{
	char log_str[256];

	unsigned short sysstatus[2],ctrlstatus[2],status,hoststate;
	if(GetHostState(hostname, &hoststate) == -1 )
	{
		sprintf(log_str,"plclib.c: GetHostState %s return -1,error!\n",hostname);
		log_printf(H9000_LOG_ERROR,2,0,log_str);
		return NET_ERR;
	}
	
	sysstatus[0] = sysstatus[1] =1;
	ctrlstatus[0] = ctrlstatus[1] =1;
	
	if(GetHostSysNetState(hostname, 0, &status) != -1)
	{
		sysstatus[0] =status ;
	}
	else
	{
		return -1;
	}
	
	if(g_sys_dualnet == YES )
	{
		if(GetHostSysNetState(hostname, 1, &status) != -1)
		{
			sysstatus[1] =status ;
		}
		else
		{
			return -1;
		}		
	}
	if(g_ctrl_net_independ == TRUE)
	{	
		if(GetHostCtrlNetState(hostname, 0, &status) != -1)
		{
			ctrlstatus[0] =status ;
		}
		else
		{
			return -1;
		}
			
		if(g_ctrl_dualnet == YES )
		{
			if(GetHostCtrlNetState(hostname, 1, &status) != -1)
			{
				ctrlstatus[1] =status ;
			}
			else
			{
				return -1;
			}		
		}
	}
	if(g_ctrl_net_independ == TRUE)
	{
		if((sysstatus[0] ==0 && sysstatus[1] ==0) && (ctrlstatus[0] ==0 && ctrlstatus[1] ==0) )	
		{
			return NET_OK;
		}
	}
	else
	{
		if(sysstatus[0] ==0 && sysstatus[1] ==0 )	
		{
			return NET_OK;
		}
	}
	return NET_ERR;
}


/********************************************************************************************************
 get plc cmd send prvg of local machine
*********************************************************************************************************/

short GetPLCCmdPrvg(LCUID plcid)
{
	return CUR_CMD_HOST;
}

short GetDevLinkStat(LCUID plcid)
{
	char		tmp[100],buf[100];
	unsigned char stnid,idx,devid;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
	idx=index_stn[stnid];
 	if(GetStnNameById(stnid,tmp)==-1) return(LCU_ERR);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(LCU_ERR);
	strcat(buf,tmp);
	strcat(buf,".SYS.LINK_STA");
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(LCU_ERR);
	if( status == LCU_ON)
	{
		return(LCU_ON);
	}
	return LCU_ERR;	
}

short GetDevONOFFStat(LCUID plcid)
{
	char		tmp[100],buf[100];
	unsigned char stnid,devid;
	unsigned short	status;
	stnid=plcid.stnid;
	devid=plcid.devid; 
 	if(GetStnNameById(stnid,tmp)==-1) return(LCU_OFF);
	strcpy(buf,tmp);
	strcat(buf,".");
 	if(GetGroupNameById(stnid,devid,tmp)==-1) return(LCU_OFF);
	strcat(buf,tmp);
	strcat(buf,".SYS.LCU_ONOFF_STA");
	if(GetIndPtStateByNameStr(buf,&status)==-1) return(LCU_OFF);
	if( status == LCU_ON)
	{
		return(LCU_ON);
	}
	return LCU_OFF;	
}

int Mod16(int len)
{ int i =0;
   if(len>0)
    {
      while(1)
       { if(len<=16) { i = i + 1 ; break ; }
         else         { len = len - 16 ; i = i + 1 ; }
       }
    }
   return(i);
}



void Timer(int ss)
{
         timeout=YES;
}

int MyStrCmp(char * str1,char * str2)
{
	if(strstr(str1,str2)!=NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int Swap2Head(struct plc_data_seg_head *p_head)
{
	int i=0;
	char buf[PLCHEADLEN];
	char tmp;
	memcpy(buf,p_head,PLCHEADLEN);
	while(i<PLCHEADLEN)
	{
		tmp=buf[i];
		buf[i] = buf[i+1];
		buf[i+1] = tmp;
		i=i+2;
	}
	memcpy(p_head,buf,PLCHEADLEN);
	return TRUE;
}

int PrintHead(struct plc_data_seg_head head)
{
	int j;
	unsigned char *pt;
	pt =(unsigned char *) & (head);
	printf("head info print: \n");
	for(j=0;j<PLCHEADLEN;j++)
	{
 		printf("=%02x=",  *( pt +j));
 		if( ((j+1)%10)==0 )       printf("\n");
        }
        printf("\n\n");
	return TRUE;
}


void GetStationInfo()
{

	unsigned char i;
	unsigned int tmp;
	for(i=1;i<MAX_STN_NUM;i++)
	{
	  g_dev_num[i]=0;
	}
	GetStationNum(&tmp);
	g_stn_num = (unsigned char) tmp;
 	for(i=1;i<=g_stn_num;i++) 
  	{  	
		UCHAR stnId;
		GetStnIdByIndex(i, &stnId);
	       if(GetGroupNumById( stnId, &tmp )==-1)
		 {
		   ;
		 }
	       g_dev_num[i] =  (unsigned char) tmp;
	}
		
}

int Swap2Str(unsigned char *buf,int wordlen)
{
	int i=0;
	char tmp;
	while(i < wordlen*2 )
	{
		tmp=buf[i];
		buf[i] = buf[i+1];
		buf[i+1] = tmp;
		i=i+2;
	}
	return TRUE;
}


SOCKET xnet_select(SOCKET s, int sec, int msec, short x)
{
 int st = errno;
 struct timeval to;
 fd_set fs;
	char log_str[256];

 if( s == INVALID_SOCKET )
 {
	sprintf(log_str,"plclib.c: xnet_select, sock = -1, error!\n"); 
	log_printf(H9000_LOG_ERROR,1,0,log_str);
	return -1;
 }
 to.tv_sec = sec;
 to.tv_usec = msec * 1000;
 FD_ZERO(&fs);
 FD_SET(s, &fs);
 switch(x)
 {
  case READ_STATUS:
  st = select(s+1, &fs, 0, 0, &to);
  break;
  case WRITE_STATUS:
  st = select(s+1, 0, &fs, 0, &to);
  break;
  case EXCPT_STATUS:
  st = select(s+1, 0, 0, &fs, &to);
  break;
 }
 return(st);
}
#ifndef MSWIN32
void FlushSock(SOCKET s)
{
	fd_set fs;
	if( s == INVALID_SOCKET )
	{
		return ;
	}
	FD_ZERO(&fs);
	FD_SET(s, &fs);
	if (ioctl(s, I_FLUSH, FLUSHR) < 0) 
	printf(" ioctl flush error\n "); 
}

#endif

