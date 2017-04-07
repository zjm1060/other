
/*	get_coded_cmd.c

	This program is created by wdk 2003.4.16.

	This program is to interface OTS2000 to H9000 with command codes.
	That is, commands from master station to LCU in H9000 is sended to 
	OTS2000, then interpreted so OTS2000 can accept and perform the related
	command simulation. The program checks repeatly the command buffer
	to see if there is any command from master.

*/


#include "/home/ems/system/ots/ots_hdb/inc/ots.h"
#include "/home/ems/system/ots/SimuEngine/inc/SimuEngine_fun.h"
#include "/home/ems/system/cmd/v4.0a/inc/cmd_def.h"

#define MAX_RECV_DATA_SEG_NUM	800		//wdk: defined in lan_common.h. This two need change.
#define MAX_SEND_DATA_SEG_NUM	600


void	GetRtuNameByID ( POINTER *point, char *rtu_name );
void	ActivateSequenceByName ( );
void	ActivateModelByName ( );
void	WriteAnalogDB ();
extern int	ReadCMD_FromNetcmdBuf ( short *ptr, char *buff );
int		OtsCmdProc(struct net_cmd_seg *cmd);

extern	short	ots_netcmd_ptr;
static	POINTER	io_ptr;
static	char	dbnam_str[DBNAME_SIZE];
CURVE_SHM_DATA	*pCurve;		//yyp debug

/*----------------------------------------------------------------------------------------------*/
/*																								*/
/*----------------------------------------------------------------------------------------------*/

double	GetFloatFromCommand ( COMMAND_MSG *cmd )
{
	double	flt;
	flt	= (double) cmd->data.fValue;
	return flt;
}


void	GetIO_NameString ( char *dbname, char *dev, char *str )
{
	strcpy ( dbname, dev );
	strcat (dbname, str );
}

POINTER *GetIO_PtrByName ( dbname )
char	*dbname;
{
	DMS_COMMON	dms_cmm;


	if ( ReadEntryByNameStr ( dbname, &dms_cmm) == -1)
	{
		printf ( " %s is not found in database.", dbname);
		return (POINTER *) NULL;
	}
	io_ptr	= dms_cmm.point;

	return	( &io_ptr );
}

//yyp 2008.3.25 added 
void	GetRtuNameByID ( POINTER *point, char *rtu_name )
{
	char stnname[30];
	char devname[30];
	GetStnNameById( point->stn_id, stnname );	
	GetGroupNameById( point->stn_id,point->dev_id, devname );
	sprintf(rtu_name,"%s.%s",stnname,devname);
}

char	*GetDBNameByID ( POINTER *ptr )
{
	POINT_NAME		pt_name;
//added by yyp 2008.3.25
	GetPtNameById( ptr, dbnam_str, &pt_name );
	return ( dbnam_str );
}


BOOL	GetInfoFromCMD ( struct net_cmd_seg *cmd, OTS_LINK_DEF *lnk )
{
	int			cmdret;
	char	log_str[256];
	struct plc_cmd_seg plccmdseg;
	lnk->ptr.stn_id  = cmd->net_cmd_buf.netdata.stn_id;
	lnk->ptr.dev_id  = cmd->net_cmd_buf.netdata.dev_id;
	lnk->ptr.data_type   = cmd->net_cmd_buf.netdata.data_type;
	lnk->ptr.pt_id    = cmd->net_cmd_buf.netdata.point_id;
	lnk->cmd_code.src = cmd->net_cmd_buf.netdata.status.src;//zwj
	memset(&plccmdseg,0,sizeof(plccmdseg));	
	if((cmdret=CmdProcess(cmd,&plccmdseg)) == 1 )
	{
		BdCmdSendResult(*cmd, CMD_SUCCESS);	
		lnk->cmd_code.stn_id = plccmdseg.cmd_buf.cmd.stn_id;
		lnk->cmd_code.dev_id = plccmdseg.cmd_buf.cmd.lcu_id;
		lnk->cmd_code.type_id = plccmdseg.cmd_buf.cmd.type_id;
		lnk->cmd_code.obj_id = plccmdseg.cmd_buf.cmd.cmd_w[0];
		lnk->cmd_code.opr = plccmdseg.cmd_buf.cmd.cmd_w[1];

	}
	else if(cmdret == 0)
	{
		BdCmdSendResult(*cmd, CMD_SUCCESS);		
		memset(&plccmdseg,0,sizeof(plccmdseg));
		sprintf(log_str,"get_coded_cmd.c  CmdProcess return %d OK\n",cmdret);
		log_write(H9000_LOG_INFO,2,log_str);
		return FALSE;
	}
	else
	{
		sprintf(log_str,"get_coded_cmd.c CmdProcess return %d Error\n",cmdret);
		err_write(log_str);
		return FALSE;
	}
	return TRUE;
}

void	FormCmdTest ( cmd )
COMMAND_MSG	*cmd;
{
    short status =0;
	cmd->dev_id		= 1;
    cmd->data_type	= C_APOWER;
	cmd->point_id	= 12;
    /* yyp 2008.3.24 */
	/*
	cmd->status		= 0;
	cmd->status = SetCharBits ( cmd->status, START_UNIT, BitS_CMD_TYPE, BitN_CMD_TYPE);
	*/
	memcpy(&cmd->status,&status,2);
	//wdk 2008.3.22	cmd->data_1		= 112;
	cmd->data.iValue		= 112;

}

void	CheckCmdBuffer ()
{
	struct net_cmd_seg netcmdseg;
	OTS_LINK_DEF	lnk;
	int result;
	for ( ;; )
	{
		if ( ReadCMD_FromNetcmdBuf ( &ots_netcmd_ptr, (char *)&netcmdseg) == TRUE )
		{
			if(GetInfoFromCMD ( &netcmdseg, &lnk ) != TRUE)
			{
				return;
			}
			if(GetSeqNameFromID(&lnk) != TRUE)
			{
				if( g_ots_ver == OTS_VER_OPR)
				{
					result = OtsCmdProc( &netcmdseg );
				}
				return; 
			}
			else
			{
				if(shm_ots_addr->runflag != 1)
				{
					BdSms("仿真停止，命令无法下达");
					return;
				}
			}
			//yyp 2012.08.07
			StopSimulog();
			MySleep(100);
			//shm_ots_addr->chg_any = 1;
			printf ( "in CheckCmdBuffer: cmd_name=%s.\n", lnk.cmd_name );

			if(GetSequenceIdByName(lnk.cmd_name) != -1)
			{
				cur_seq_id = GetSequenceIdByName(lnk.cmd_name);
				localtime ( &cur_seq_time );
			}


			switch ( lnk.cmd_code.type_id )
			{
			case	C_APOWER:
				//needed to be modified by yyp 2009.10.21
				MySleep(50);
				ActivateSequenceByName ( lnk.cmd_name, -1 );
				BdYGParam(lnk.ptr.stn_id, lnk.ptr.dev_id, (float)(netcmdseg.net_cmd_buf.netdata.data/1000) );
				
				break;

			case	C_RPOWER:
				//needed to be modified by yyp 2009.10.21
				StopModelByName("BS.BS_F1.ANA.WG_MVAR");
				MySleep(50);
				ActivateSequenceByName ( lnk.cmd_name,-1 );
				BdWGParam(lnk.ptr.stn_id, lnk.ptr.dev_id, (float)(netcmdseg.net_cmd_buf.netdata.data/1000) );

				break;
			default:
				ActivateSequenceByName ( lnk.cmd_name,-1 );
				break;
			}

			//ActiveAutoSeq();
		}
		break;
	}
}

int GetSeqNameFromID(OTS_LINK_DEF *lnk)
{
	char buf_id[30],buf_id_pt_any[30],buf_id_type_any[30];
	int i,src;
	if(lnk->cmd_code.src==4)  //借用命令来源国调作现地
		src=0;//命令来源为现地（现地仿真盘柜/3D）
	else
		src=1;//命令来源非现地（监控/AGC/AVC）
	sprintf(buf_id,"%d.%d.%d.%d.%d.%d.%d",lnk->cmd_code.stn_id,lnk->cmd_code.dev_id,lnk->ptr.data_type,lnk->ptr.pt_id,
		lnk->cmd_code.type_id,lnk->cmd_code.opr,src);//zwj
	sprintf(buf_id_type_any,"%d.%d.%d.%d.%d.%d.%d",lnk->cmd_code.stn_id,lnk->cmd_code.dev_id,lnk->ptr.data_type,lnk->ptr.pt_id,
		0,lnk->cmd_code.opr,src);//zwj
	for(i=0;i<seqid.num;i++)
	{
		if(strcmp(seqid.seg[i].seqid,buf_id) == 0 )
		{
			strcpy(lnk->cmd_name,seqid.seg[i].seqname);
			printf("Err: recved  %s cmd\n",seqid.seg[i].seqname);
			return TRUE;
		}
	}

	for(i=0;i<seqid.num;i++)
	{
		if(strcmp(seqid.seg[i].seqid,buf_id_type_any) == 0)
		{
			strcpy(lnk->cmd_name,seqid.seg[i].seqname);
			printf("Err: recved  %s cmd\n",seqid.seg[i].seqname);
			return TRUE;
		}
	}
	printf("Err: can not find %s in sequence id def\n",buf_id);
	return FALSE;
}


int		OtsCmdProc(struct net_cmd_seg *net_cmd_seg_ptr)
{
	unsigned char cmd_type,stnid,devid,local_stnid,db_type;
	
	unsigned short status,devno;
	short result,result1;
	
	int  data;
	unsigned short pnt;
	
	LCUID lcuid;
	
	float hi_range,lo_range,hi_code,lo_code,xcode;	
	char	err_str[256]; 
	
	POINTER		pointer,pointer1;
	DMS_COMMON	common_aa;
	int	doublePt;

	stnid = net_cmd_seg_ptr->net_cmd_buf.netdata.stn_id; 
	
	cmd_type = net_cmd_seg_ptr->net_cmd_buf.netdata.type_id;
	
	devid =  net_cmd_seg_ptr->net_cmd_buf.netdata.dev_id;

	pointer.stn_id  = net_cmd_seg_ptr->net_cmd_buf.netdata.stn_id;
	pointer.dev_id  = net_cmd_seg_ptr->net_cmd_buf.netdata.dev_id;
	pointer.data_type   = net_cmd_seg_ptr->net_cmd_buf.netdata.data_type;
	pointer.pt_id    = net_cmd_seg_ptr->net_cmd_buf.netdata.point_id;
	status = net_cmd_seg_ptr->net_cmd_buf.netdata.status.opr;
	data = net_cmd_seg_ptr->net_cmd_buf.netdata.data;
	result = -1;
	result1 = -1;

	ReadEntryById( &pointer , &common_aa);
	doublePt=common_aa.fixed.ioattr.doublePt;
	if(doublePt==1)
	pointer1 = common_aa.fixed.pair_pt;

	switch (cmd_type)
	{
				      						
		case C_UNIT:
		{	
			if( status == 1 || status == 23 )
			{
				result = 1;
			}
			if( status == 2 || status == 27 )
			{
				result = 2;
			}
			else if( status == 20  || status == 26 )
			{
				result = 7;
			}
			else if( status == 21 || status == 25 )
			{
				result = 4;
			}
			else if( status == 2 || status == 24 )
			{
				result = 5;
			}		
			if(result != -1)
			{
				BdINDByID(pointer,result);			
			}			
			return 1;
			break;
		}
		case C_GATE:
		{	if(status==15)   /*闸门开度给定*/
			{
				BdANAByID(pointer,data/1000.0);
			}
			else
			{
				result = status -1;
				if(result != -1)
				{
					BdINDByID(pointer,result);			
				}
			}
			return 1;
			break;
		}
		case C_BREAKER:
		{
			if(status == 1 || status == 12)
			{
				result = 0;
				result1 = 1;//zwj
			}
			else if(status == 2 ||status == 11)//zwj 试验合
			{
				result = 1;
				result1 = 0;//zwj
			}
			else if(status == 3)
			{
				result = 2;
			}
			else if(status == 4)
			{
				result = 3;
			}
			if(result != -1)
			{
				BdINDByID(pointer,result);
				if(doublePt==1)//zwj
					MySleep(50);
					BdINDByID(pointer1,result1);
			}
			return 1;
			break;
		}
		
		case C_SWITCH:
		{
			if(status == 1 || status == 3)
			{
				result = 0;
				result1 = 1;//zwj
			}
			else if(status == 0)
			{
				return 1;
			}
			else if(status == 2 || status == 4)
			{
				result = 1;
				result1 = 0;//zwj
			}
			if(result != -1)
			{
				BdINDByID(pointer,result);
				if(doublePt==1)//zwj
					MySleep(50);
					BdINDByID(pointer1,result1);
			}
			return 1;
			break;
		}	
		case C_DEVICE:
		{
			if(status == 1)
			{
				result = 0;
				result1 = 1;//zwj
			}
			else if(status == 0)
			{
				return 1;
			}
			else if(status == 2)
			{
				result = 1;
				result1 = 0;//zwj
			}
			else
			{
				result = status;
			}
			if(result != -1)
			{
				BdINDByID(pointer,result);	
				if(doublePt==1)//zwj
					MySleep(50);
					BdINDByID(pointer1,result1);
			}
			
			return 1;
			break;
		}			
		case C_DEV_MODE:
		{
		/*	result = status;*/
			if(status == 1)
			{
				result = 0;
				result1 = 1;//zwj
			}
			else if(status == 0)
			{
				return 1;
			}
			else if(status == 2)
			{
				result = 1;
				result1 = 0;//zwj
			}
			else
			{
				result = status;
			}
			if(result != -1)
			{
				BdINDByID(pointer,result);
				if(doublePt==1)//zwj
					MySleep(50);
					BdINDByID(pointer1,result1);
			}			
			return 1;
			break;
		}
		case C_ONOFF_FORCE:
		{	
			result = status -1;
			if(result != -1)
			{
				BdINDByID(pointer,result);			
			}			
			return 1;
			break;
		}
		default:
		{
			sprintf(err_str,"cmd_type=%d not processsed by simulog ",cmd_type);
			err_write(err_str);
			return -1;
		}
	}
	return -1;
}