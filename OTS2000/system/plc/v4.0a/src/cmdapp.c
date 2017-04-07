/*******************************************************************************************************
FILENAME:	plcapp.c
FUNCTION:	
	
	  the source code file of plc communication module,whether the plc type
	is "GE" or "QUANTUM".include:
	(1) int main();
	(2)int ThreadCmd(void *arg);
	(3)int WriteCmd(LCUID *p_plcid);	

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2008.6.27  upgraded for AB								yyp
 2007.7.30  upgraded for Siemens						yyp		
 2005.11.1	start to modify the code for H9000 V4.0		yyp

********************************************************************************************************/
#include "../inc/plchead_all.h"

/********************************************************************************************************
 Write Command to plc
 main function of WriteCmd thread 
*********************************************************************************************************/
struct plc_data_seg_head	head;
void * ThreadCmd(void *arg)
{
	char log_str[256];
	MYSOCKET	sockplc;	
	LCUID		plcid;
	short		err_count;			/* error count of reading data function */
	short		loop_count=0;			/* for loop count */
	short		ret;				/* return value of  */
	unsigned char	stnid,idx,devid,cmd_stnid,cmd_devid;
	short		CurCmdPrvg=CUR_CMD_VICE,OldCmdPrvg=CUR_CMD_VICE;	/* PLC Cmd Send Prvg of last loop,current loop */
	short		OldLinkStat= LCU_OFF,CurLinkStat= LCU_OFF;	
	int		waitms=0;		
	sockplc.sock=INVALID_SOCKET;
	plcid.stnid = ((THRARG *) arg)->stnid ;
	plcid.devid = ((THRARG *) arg)->devid ;
	stnid=plcid.stnid;
	devid=plcid.devid;	
	idx = index_stn[stnid];	
	sprintf(log_str,"==== cmdapp.c:  Start ThreadCmd Function! plcid=%d.%d!\n",stnid,devid);
	log_printf(H9000_LOG_INFO,1,0,log_str);
	err_count=0;

	runvar[idx][devid].hotcpu=rdb_cpu(stnid,devid);
	runvar[idx][devid].hotnet=rdb_net(stnid,devid);
	runvar[idx][devid].FirstCmd = TRUE;
	
	/* end ptr equal to head ptr when running first */

	runvar[idx][devid].netcmdptr=shm_net_cmd->head;	//yyp
	runvar[idx][devid].FirstCmd = 1;
	for(;;)
	{
		MySleep(100);
		/******* if current net/cpu is not equal to last hotnet /hotcpu *******/
		/* added by yyp 07.05.17 */
		//modifed by yyp 2012.02.17, cmd net card could differ from data scan net card
		if( runvar[idx][devid].hotcpu !=rdb_cpu(stnid,devid) )
		{
			DisconnectPLC(&sockplc,plcid);
			runvar[idx][devid].hotcpu = rdb_cpu(stnid,devid);
			MySleep(200);
		}
		if( runvar[idx][devid].netcmdptr == shm_net_cmd->head )
		{
			loop_count++;
			/****** disconnect plc when loop count>=200 ******/
			if(loop_count>=MAX_CMD_LOOP)
			{
				DisconnectPLC(&sockplc,plcid);
				loop_count=0;
				sprintf(log_str,"==== cmdapp.c: stnid=%d, devid=%d, curptr=%d, headptr=%d \n",stnid,devid,(unsigned short) runvar[idx][devid].netcmdptr,(unsigned short)shm_net_cmd->head);
				log_printf(H9000_LOG_INFO,3,0,log_str);
			}
			MySleep(200);
			continue;
		}

		/*******  if cur cmd  ptr doesn't belong to cur stnid and devid , then next cmd *******/
		cmd_stnid=shm_net_cmd->seg[runvar[idx][devid].netcmdptr].net_cmd_buf.netdata.stn_id;
		cmd_devid=shm_net_cmd->seg[runvar[idx][devid].netcmdptr].net_cmd_buf.netdata.dev_id;
		if(  cmd_stnid != stnid ||  cmd_devid  != devid )	
		{
			if(stnid==1 && devid==1)
			{
				if(cmd_stnid<1 && cmd_stnid > g_stn_num)
				{	
					sprintf(log_str,"==== cmdapp.c: net_cmd_buf[%d].netdata.stn_id =%d invalid",runvar[idx][devid].netcmdptr, cmd_stnid);
					log_printf(H9000_LOG_ERROR,1,0,log_str);				
				}
				else if(cmd_devid<1 && cmd_devid > g_dev_num[cmd_stnid])
				{
					sprintf(log_str,"==== cmdapp.c: net_cmd_buf[%d].netdata.dev_id =%d invalid",runvar[idx][devid].netcmdptr, cmd_devid);
					log_printf(H9000_LOG_ERROR,1,0,log_str);				
				}
			}
			loop_count++;
			/****** disconnect plc when loop count>=200 ******/
			if(loop_count>=MAX_CMD_LOOP)
			{
				DisconnectPLC(&sockplc,plcid);
				loop_count=0;
			}
			runvar[idx][devid].netcmdptr++;
			if(runvar[idx][devid].netcmdptr>=MAX_NETCMD_SEG_NUM) 
			{
				runvar[idx][devid].netcmdptr=0;
			}
							
			MySleep(200);
			continue ;
		}
		
		/******* if cur cmd  ptr belong to cur stnid and devid  then go ahead	*******************************/
		
		/******* read the plc link stat ,if linkoff and  not LCU_ONOFF cmd, then BdCmdErr and next loop *******/
		
		CurLinkStat=GetDevONOFFStat(plcid);
		if(CurLinkStat==LCU_OFF && shm_net_cmd->seg[runvar[idx][devid].netcmdptr].net_cmd_buf.netdata.type_id != C_LCU_ONOFF)
		{
			if(sockplc.sock != INVALID_SOCKET)
			{
				DisconnectPLC(&sockplc,plcid);
			}
			OldLinkStat =LCU_ERR; 
			sprintf(log_str,"==== cmdapp.c: CurLinstat =%d stnid=%d,devid=%d next loop\n",CurLinkStat,stnid,devid);	
			log_printf(H9000_LOG_INFO,1,0,log_str);
			sprintf(log_str,"LCU %d.%d 离线, 禁止操作!",stnid,devid);
			BdSms(log_str);
			
			/****** Bd cmd error and next cmd *****/
			BdCurCmdErr(plcid);						
			MySleep(2000);
			continue ;
		}
		
		loop_count=0;
		/******* connect plc while not connected *******/
		/* modifed by yyp 07.08.1 */
		if( sockplc.sock == INVALID_SOCKET && IsCmdSendPlc(plcid,shm_net_cmd->seg[runvar[idx][devid].netcmdptr].net_cmd_buf.netdata.type_id) == TRUE ) 
		/* if( sockplc.sock == INVALID_SOCKET ) */
		{
			sockplc=ConnectPLC(plcid,THR_CMD);
			if(sockplc.sock==INVALID_SOCKET)
			{
				sprintf(log_str,"%s连接LCU%d.%d失败!",hostname,stnid,devid);
				BdSms(log_str);				
				/* BdPlcLinkStat(plcid,LCU_ERR); */
				BdAllCmdErr(0,plcid);
				runvar[idx][devid].netcmdptr=shm_net_cmd->head;
				MySleep(2000);
				continue;
			}
		}
		
		/************* call WriteCmd()  *****************/				
		ret=WriteCmd(sockplc,plcid);
		if(ret!=TRUE)
		{
			printf("cmdapp::  call WriteCmd error ret=%d, stnid=%d,devid=%d \n",ret,stnid,devid);
			DisconnectPLC(&sockplc,plcid);
			BdAllCmdErr(1,plcid);
			runvar[idx][devid].netcmdptr=shm_net_cmd->head;
	
			err_count++;
			if(err_count>=MAXRETRY_CMD)
			{
				/* BdPlcLinkStat(plcid,LCU_ERR); */
				err_count=0;
			}
			runvar[idx][devid].FirstCmd=1;			
		}
		else
		{
			err_count=0;	
		}
	}
	return NULL;
}


/********************************************************************************************************
 write cmd to plc
*********************************************************************************************************/

int WriteCmd(MYSOCKET sockplc,LCUID plcid)
{
	short		err_count;				/* error count of reading data function */
	int 		seg;
	int			cmdret;
	int 		ref;
	int 		result;
	int  		scanlen;
	char		host;
#ifdef _BIG_ENDIAN_
	int			i;
#endif
	int			j;
	unsigned char * pt;
	char		host_firstrun[2];
	unsigned	char databuf[MAX_SCAN_BUF*2]; 	
	unsigned char	stnid,idx,devid,srcid,cmdbufid;
	unsigned short cmd_start_addr,cmd_size;
	unsigned short cmd_base_def,cmd_size_def;
	unsigned short		curptr,cmdprt;
	struct net_cmd_seg netcmdseg;
	struct plc_cmd_seg plccmdseg;
	char log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];
	err_count=0;
	
	sprintf(log_str,"==== cmdapp.c:  Start WriteCmd Function! plcid=%d.%d!\n",stnid,devid);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	if (sockplc.sock==INVALID_SOCKET && IsCmdSendPlc(plcid,
		shm_net_cmd->seg[runvar[idx][devid].netcmdptr].net_cmd_buf.netdata.type_id) == TRUE )   
	{	
		printf("writecmd err: maybe sockplc=-1 ");
		return FALSE;
	}

	if(runvar[idx][devid].netcmdptr == shm_net_cmd->head)
	{
		return TRUE;
	}

	if(plconf[idx][devid].plctype==SIEPLC)
	{	
		cmd_start_addr = CMDBASE_SIE;
		cmd_size = CMDSIZE_SIE;
		cmd_base_def = CMDBASE_SIE;
		cmd_size_def = CMDSIZE_SIE;
	}		
	else if(plconf[idx][devid].plctype==MBPLC
		|| plconf[idx][devid].plctype==PREPLC)
	{
		cmd_start_addr = CMDBASE_MB;
		cmd_size = CMDSIZE_MB;
		cmd_base_def = CMDBASE_MB;
		cmd_size_def = CMDSIZE_MB;
	}
	else if(plconf[idx][devid].plctype==GEPLC)
	{
		cmd_start_addr = CMDBASE_GE;
		cmd_size = CMDSIZE_GE;
		cmd_base_def = CMDBASE_GE;
		cmd_size_def = CMDSIZE_GE;
	}
	else if(plconf[idx][devid].plctype==ABPLC)
	{
		cmd_start_addr = CMDBASE_AB;
		cmd_size = CMDSIZE_AB;
		cmd_base_def = CMDBASE_AB;
		cmd_size_def = CMDSIZE_AB;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[idx][devid].plctype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return(FALSE);
	}		
	/***************** Read Head Data From PLC **************************************************/
	if(runvar[idx][devid].FirstCmd==1 && IsCmdSendPlc(plcid,
		shm_net_cmd->seg[runvar[idx][devid].netcmdptr].net_cmd_buf.netdata.type_id) == TRUE )
	{
		if(plconf[idx][devid].plctype == MBPLC
			|| plconf[idx][devid].plctype==PREPLC)
		{			
			memset(databuf,0,MAX_SCAN_BUF*2);
			if(plconf[idx][devid].plctype==SIEPLC)
			{	
				seg = HEADDB_NO;
				ref = HEADBASE_SIE/2;
			}		
			else if(plconf[idx][devid].plctype==MBPLC
				|| plconf[idx][devid].plctype==PREPLC)
			{
				seg=REGSEG;
				ref=HEADBASE_MB;
			}
			else if(plconf[idx][devid].plctype==GEPLC)
			{
				seg=REGSEG;
				ref=HEADBASE_GE;
			}
			else
			{
				sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[idx][devid].plctype);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				return(FALSE);
			}			

			scanlen=(PLCHEADLEN+1)/2;
			result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf,plcid);
			if(result==FALSE)
			{
				sprintf(log_str,"==== cmdapp.c: stnid=%d,devid=%d WriteCmd(): ReadPLC error \n",stnid,devid);	
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				return(FALSE);
			}
			memcpy(&head,databuf,PLCHEADLEN);
#ifdef _BIG_ENDIAN_
			Swap2Head( &head );
#endif
			memcpy(&host_firstrun,&head.host_firstrun,2);
#ifdef _BIG_ENDIAN_
			host = host_firstrun[0];
#else
			host = host_firstrun[1];
#endif
			if((host == 1) || (host == 3) ) 
			{
				host =1;
			}
			else
			{
				host=0;
			}
			cmd_start_addr=head.cmd_start_addr;
			cmd_size = head.cmd_size;
			if( cmd_start_addr != cmd_base_def || cmd_size !=cmd_size_def)
			{
				sprintf(log_str,"==== cmdapp.c: stnid=%d devid=%d cmd_start_addr =%d or cmd_size =%d invalid\n",
					stnid,devid, head.cmd_start_addr, head.cmd_size);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				return(FALSE);
			}
			if(runvar[idx][devid].hotcpu!=host)
			{
				sprintf(log_str,"==== cmdapp.c: stnid=%d,devid=%d error, WriteCmd():hotcpu =%d differ from host =%d in plc!\n",
					stnid,devid,runvar[idx][devid].hotcpu,host);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				runvar[idx][devid].hotcpu=host;
				BdCpuHost(plcid, runvar[idx][devid].hotcpu);
				
				return(FALSE);
			}
		}
		runvar[idx][devid].FirstCmd=0;
	}

	/************ write  command ******************************************/
	curptr =  runvar[idx][devid].netcmdptr;

	while( curptr != shm_net_cmd->head )
	{	
		netcmdseg = shm_net_cmd->seg[curptr];
		if(netcmdseg.net_cmd_buf.netdata.stn_id==stnid 
			&& netcmdseg.net_cmd_buf.netdata.dev_id==devid && netcmdseg.lan_head.dp_type == COMM_TIME)
		{

#ifdef _BIG_ENDIAN_
			Swap2Byte( &(netcmdseg.lan_head.length));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.point_id));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.status));
			Swap4Byte( &(netcmdseg.net_cmd_buf.netdata.data));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.ctrl_addr));
			Swap2Byte( &(netcmdseg.net_cmd_buf.netdata.year));
#endif
			/*************** analyze one net cmd ******************/
			memset(&plccmdseg,0,sizeof(plccmdseg));

			if((cmdret=CmdProcess(&netcmdseg,&plccmdseg)) == 1 )
			{
#ifdef _BIG_ENDIAN_			
				for(i=0;i<7;i++)
				{
					Swap2Byte( &(plccmdseg.cmd_buf.cmd.cmd_w[i]) );
				}	

#endif				
				/*************** send one cmd to plc ******************/
				seg=REGSEG;
				if(plconf[idx][devid].plctype==SIEPLC)
				{	
					seg=CMDDB_NO;
				}
				srcid = plccmdseg.cmd_buf.cmd.src_id;
				cmdbufid = GetCmdBufId(srcid);
				if(cmdbufid < 1 || cmdbufid > cmd_size_def )
				{
					sprintf(log_str,"==== cmdapp.c: stnid=%d devid=%d cmdbufid =%d invalid\n",stnid,devid,cmdbufid);
					log_printf(H9000_LOG_ERROR,1,0,log_str);
					sprintf(log_str,"%s 未定义命令区ID!",g_hostname);
					BdSms(log_str);
					BdCmdSendResult(netcmdseg, CMD_SENDERR);
					curptr++;
					if(curptr>=MAX_NETCMD_SEG_NUM) 
					{
						curptr=0;
					}
					runvar[idx][devid].netcmdptr = curptr;
					return(FALSE);
				}
				if(plconf[idx][devid].plctype==SIEPLC)
				{					
					ref = cmd_base_def/2 + (cmdbufid-1)  * PLCCMD_BUF_SIZE;	
				}
				else
				{
					ref = cmd_base_def + (cmdbufid-1)  * PLCCMD_BUF_SIZE;					
				}
				scanlen = PLCCMD_BUF_SIZE;
				if( sockplc.sock ==INVALID_SOCKET ) 
				{
					sprintf(log_str,"==== cmdapp.c stnid=%d,devid=%d,cmdbufid=%d  write cmd socket =%d error ====\n",
						stnid,devid,cmdbufid,sockplc.sock);
					log_printf(H9000_LOG_ERROR,1,0,log_str);
					BdCmdSendResult(netcmdseg, CMD_SENDERR);
					curptr++;
					if(curptr>=MAX_NETCMD_SEG_NUM) 
					{
						curptr=0;
					}
					runvar[idx][devid].netcmdptr = curptr;					
					return(FALSE);					
				}
				result = WritePLC(sockplc, (unsigned char)seg, ref, PLCCMD_BUF_SIZE,
						(unsigned char *)&(plccmdseg.cmd_buf.cmd), plcid);
				if(result==FALSE)
				{
					sprintf(log_str,"==== cmdapp.c stnid=%d,devid=%d,cmdbufid=%d  write cmd error ====\n",stnid,devid,cmdbufid);
					log_printf(H9000_LOG_ERROR,1,0,log_str); 
					BdCmdSendResult(netcmdseg, CMD_SENDERR);
					curptr++;
					if(curptr>=MAX_NETCMD_SEG_NUM) 
					{
						curptr=0;
					}
					runvar[idx][devid].netcmdptr = curptr;					
					return(FALSE);
				}
				else
				{
					sprintf(log_str,"====  cmdapp.c stnid=%d,devid=%d,cmdbufid=%d write cmd ok ====\n ",
						stnid,devid,cmdbufid);
					log_printf(H9000_LOG_ERROR,1,0,log_str);
					BdCmdSendResult(netcmdseg, CMD_SUCCESS);
					/* BdPlcLinkStat(plcid,LCU_ON); */
					pt = (unsigned char *) &(plccmdseg.cmd_buf.cmd);
					printf("cmd buf:  ");
					for(j=0;j<PLCCMD_BUF_SIZE*2;j++)
					{
						printf("%02x=", (unsigned char )*(pt +j));
	  
						  if(j==5)
						  {
						   printf("   ");
						  }
						  
						  if(j==11)
						  {
						   printf("   ");
						  }
						  
					}
					printf("\n\n");
					curptr++;
					if(curptr>=MAX_NETCMD_SEG_NUM) 
					{
						curptr=0;
					}
					runvar[idx][devid].netcmdptr = curptr;
					
					cmdprt = shm_plc_cmd->head;
					if(cmdprt>=MAX_PLCCMD_SEG_NUM) 
					{
						cmdprt=0;
					}	
					shm_plc_cmd->seg[cmdprt] = plccmdseg;
					cmdprt++;
					shm_plc_cmd->head =  cmdprt ;					
				}
			}
			else if(cmdret == 0)
			{
							
				memset(&plccmdseg,0,sizeof(plccmdseg));
				curptr++;
				if(curptr>=MAX_NETCMD_SEG_NUM) 
				{
					curptr=0;
				}
				runvar[idx][devid].netcmdptr = curptr;
				sprintf(log_str,"====  cmdapp.c plc%d.%d CmdProcess return %d OK\n",stnid,devid,cmdret);
				log_printf(H9000_LOG_INFO,1,0,log_str);
				return(TRUE);
			}
			else
			{
				curptr++;
				if(curptr>=MAX_NETCMD_SEG_NUM) 
				{
					curptr=0;
				}
				runvar[idx][devid].netcmdptr = curptr;
				sprintf(log_str,"====  cmdapp.c plc%d.%d CmdProcess return %d Error\n",stnid,devid,cmdret);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				return(TRUE);				
			}
			/**** end of if cmdret ****/
		}
		else
		{
			curptr++;
			if(curptr>=MAX_NETCMD_SEG_NUM) 
			{
				curptr=0;
			}
			runvar[idx][devid].netcmdptr = curptr;
		}/**** end of if netcmdseg. ****/
		
	} /*** end of while ***/
	return(TRUE);
}


