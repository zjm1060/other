/************************************************************************
*  FILE NAME:           iccp_app_fun.c      				*
*  FUNCTION:            Controlled Station				*
*			Application Layer Function			*
*  's NAME : 	              						*
*  DATE:                Dec,2006                			*
*									*
* MODIFICATION LOG :							*
*  Date     Who   Rev                     Comments			*
* --------  ---  ------   ----------------------------------------------*
* 05/17/07  zy    3       Remote Client data and ds function added      *
* 03/28/07  zy    2       updated					*
* 12/06/06  zy    1       Created for SUN ICCP TASE.2 application	*
************************************************************************/

#include <time.h>
#include "../inc/mit_main.h"
#include "str_util.h"

#include "cppinc.h"

#include "../inc/iccp_main.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"
#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"

/*
#define	DEBUG
*/
//#define BY_RECORD		/* use shared memory record area to catch SOE */
#define CMD_TO_LCU

#define GW_DEVID	20

SHM_RECORD *shm_rec_addr;

extern	char	infobuf[1024];
extern	SHM_BUF *shm_buf_addr;

extern	int	soe_overflow_flag,soe_overflow_Sendflag;
extern	unsigned char	cmd_src;

extern	SOEBUF	soebuf;
short	rcv_end;
extern	ST_INT	nAssocNum[MAX_DC_NUM];		/* Added for save association number for remotes */
extern	sGWAssoc  sAssocDat[MAX_DC_NUM][2];	/* Changed to two dimension for 2 association in one remote 2003.7.30 zy */

unsigned char	orgdpsoebuf[MAX_DP_NUM];

#ifdef REC_YF_AGC_CMD
	unsigned short	daysaved;
	char		cmdrecfile[80];
	FILE		*fagc;
#endif

extern		SPDEF		spdef;
extern		DPDEF		dpdef;
extern		MEDEF		medef;
extern		DPDEF		dpdef;
extern		MEDEF		medef;
extern		SEDEF		sedef;
extern		CMDDEF		scdef;
extern	short	cmd_end;
extern	unsigned char cur_stn_id;		//added by yyp 2011.03.09

static	unsigned char	me_buf[1440];
static	SW_MSG		sw_buf[70];
static	short		me_num = 0;
static	short		sw_num = 0;
static	int		ana_all_flag = 0;

char    src_id;
long GetTimefromCmd(struct net_cmd_data *p_rec);			//added by yyp 2011.03.09
extern  ST_VOID *findDevice (MIT_DEVICE_REF *pstDeviceSet, ST_CHAR *devName);
extern ST_VOID    DisplayDeviceYYP (ST_VOID);


//######################  yyp

int	ICCPSendCmd()
{
	short	sd_pnt;
	ST_CHAR buffer[MAX_IDENT_LEN +1];	
	//unsigned char cmd_type,stn_id,op_type; 
	unsigned char cmd_type,op_type; 
	unsigned char stnid;		//added by yyp 2010.03.29
	struct net_cmd_data	cmd_rec;
	char dev_iccp_name[256];
	COMMAND_STATUS cmd_stat;
	MIT_ASSOC_INFO *pstAssociationInfo;
	MIT_MIC_VCC *pstMitMicVcc;	
	MIC_DEVICE  *pDevice;	
	unsigned char	stat;	
	MI_REQ_CTRL *req;
	short	rcv_pnt;
	POINTER	rcv_ptr;
	POINTER	ptr;
	static MI_DEV_VALUE_UNION devValue;
	static MI_CONTROL_COMMAND lastCmdValue;  /* ST_INT16 */
	static MI_CONTROL_SETPOINT_REAL lastRealValue = (ST_FLOAT)4.235; /* ST_FLOAT */
	int	cmd_data/*,normv*/;
 	char	host_id;
	short	/*ob_addr,base_addr,i,*/pntnum;
	float	ftemp,setpv;
	time_t	rec_time,curr_time_tt; /*zwj for cmd overtime*/
	struct	tm	timeptr;

	sd_pnt = 0;
	cmd_type = 0;
	stnid= cur_stn_id;
	printf("@@cmd_end=%d@@ stnid=%d, head=%d@@\n",cmd_end,stnid,shm_comm_cmd->stn_data[stnid].head);
	//modified by yyp 2011.03.09
	while( cmd_end != shm_comm_cmd->stn_data[stnid].head )
	{
		rcv_pnt = cmd_end;
		cmd_end++;
		if(cmd_end >= MAX_NETCMD_SEG_NUM)
			cmd_end = 0;
		//pCmd_alm = &shm_rec_addr->rec[rcv_pnt].cmd_rec;
		cmd_rec =shm_comm_cmd->stn_data[stnid].seg[rcv_pnt].net_cmd_buf.netdata;
		ClearCommShm(stnid,rcv_pnt,TYPE_ONE_SEG);	
#ifdef _BIG_ENDIAN_
		Swap2Byte( &(cmd_rec.point_id));
		Swap2Byte( &(cmd_rec.status));
		Swap4Byte( &(cmd_rec.data));
		Swap2Byte( &(cmd_rec.ctrl_addr));
		Swap2Byte( &(cmd_rec.year));
#endif
		if(cmd_rec.type_id < C_LCU_ONOFF)
			continue;

		cmd_type = cmd_rec.type_id;
		cmd_stat = cmd_rec.status;
		cmd_data = cmd_rec.data;//?

		op_type = cmd_stat.opr;

		rcv_ptr.stn_id = cmd_rec.stn_id;
		rcv_ptr.dev_id = cmd_rec.dev_id;
		rcv_ptr.data_type = cmd_rec.data_type;
		rcv_ptr.pt_id = cmd_rec.point_id;
		host_id = cmd_rec.host_id;

#ifdef DEBUG
		printf("\nNew Cmd:cmd_type=%d stn_id=%d dev_id=%d data_type=%d cmd_stat=%d cmd_data=%f host_id=%d,op_type=%d\n",
			cmd_type, rcv_ptr.stn_id, rcv_ptr.dev_id, rcv_ptr.data_type,cmd_stat.state,cmd_data/1000,host_id,op_type);
		
#endif
		rec_time=GetTimefromCmd(&cmd_rec);
		if(-1==rec_time)
		{
			sprintf(infobuf,"\n记录时间错误：stn_id=%d dev_id=%d data_type=%d host_id=%d\n",
			cmd_rec.stn_id, cmd_rec.type_id, cmd_rec.data_type,cmd_rec.host_id);
			WriteDspInfo(infobuf);
			//WriteLogFile(infobuf);
			continue;
		}

		curr_time_tt=time(NULL);
		timeptr=*localtime(&curr_time_tt);

		if((rec_time-curr_time_tt)>(5*60))
		{
			sprintf(infobuf,"超前5分钟:记录时间%04d-%02d-%02d %d:%d:%d  本机时间%04d-%02d-%02d %d:%d:%d\n",cmd_rec.year,cmd_rec.month,cmd_rec.day,
				cmd_rec.hour,cmd_rec.minute,cmd_rec.second,timeptr.tm_year+1900,timeptr.tm_mon+1,timeptr.tm_mday,timeptr.tm_hour,timeptr.tm_min,timeptr.tm_sec);
			WriteDspInfo(infobuf);
			//WriteLogFile(infobuf);
			continue;
		}
		if((curr_time_tt-rec_time)>(5*60))
		{
			sprintf(infobuf,"5分钟以前命令，命令失效:记录时间%04d-%02d-%02d %d:%d:%d  本机时间%04d-%02d-%02d %d:%d:%d\n",cmd_rec.year,cmd_rec.month,cmd_rec.day,
				cmd_rec.hour,cmd_rec.minute,cmd_rec.second,timeptr.tm_year+1900,timeptr.tm_mon+1,timeptr.tm_mday,timeptr.tm_hour,timeptr.tm_min,timeptr.tm_sec);
			WriteDspInfo(infobuf);
			//WriteLogFile(infobuf);
			continue;
		}

		switch	( cmd_type ) 
		{
		case C_APOWER:
		case C_RPOWER:
		case DPS_MAN_VALUE_SET:
		case C_OP_VALUE_SET:

			for(pntnum=0;pntnum<sedef.number;pntnum++)
			{
				if(GetPtIdByNameStr(sedef.name[pntnum],&ptr)==-1) continue;
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(sedef.cmdtype[pntnum] == cmd_type))
					break;
			}
			if(pntnum>=sedef.number)
			{
				printf("No this C_APOWER/C_RPOWER:%d stn:%d dev:%d in set point table!\n",cmd_type,rcv_ptr.stn_id,rcv_ptr.dev_id);
				break;
			}

			setpv = (float)(cmd_data/1000.0);
			devValue.set_pt_r = setpv;
			strcpy(dev_iccp_name,sedef.iccp_name[pntnum]);
			printf("cmd_data= %d; value=%f\n",cmd_data,setpv);

			sprintf(infobuf,"\n遥调令:%s,cmd_type=%d\n",sedef.name[pntnum],cmd_type);
			WriteDspInfo(infobuf);
			//WriteLogFile(infobuf);
			break;
		case C_SWITCH:
		case C_BREAKER:
		case C_DEVICE:
		case C_GATE:
		case C_TRANS:
		case C_SYS_MODE_SET:
		case C_SIGNAL_RESET:
		case DPS_MAN_STAT_SET:
		case C_ONOFF_FORCE:
		case C_SPEED_STEP:
		case C_VOLT_STEP:
		case C_UNIT:

		default:
			for(pntnum=0;pntnum<scdef.number;pntnum++)		/* Check for DC Command */
			{
				if(GetPtIdByNameStr(scdef.name[pntnum],&ptr)==-1) continue;				
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(scdef.cmdtype[pntnum] == cmd_type))
				{
					break;
				}
			}
			if(pntnum>=scdef.number)
			{
				printf("No this cmd :%d stn:%d dev:%d in scdef table!\n",cmd_type,rcv_ptr.stn_id,rcv_ptr.dev_id);
				continue;
			}
			if(scdef.optype1[pntnum] == op_type)
			{
				stat = 0x1;
			}
			else if(scdef.optype2[pntnum] == op_type)
			{
				stat = 0x2;				
			}
			else
			{
				continue;
			}
			//needed to be modified
			devValue.cmd = stat;
			strcpy(dev_iccp_name,scdef.iccp_name[pntnum]);
			sprintf(infobuf,"\n遥控令:%s,cmd_type=%d,op_type=%d\n",scdef.name[pntnum],cmd_type,op_type);
			WriteDspInfo(infobuf);
			//WriteLogFile(infobuf);
			//return(true);
			
			break;
		}/* switch(cmd_type) */
	
		//needed to be modified  yyp 2011.09.28
		pstAssociationInfo = getAssociationName (buffer);
		pstMitMicVcc = findMitMicVcc (pstAssociationInfo->remoteName);

      if ((pDevice = (struct _mic_device *)findDevice (pstMitMicVcc->pstDeviceSet, dev_iccp_name))!=NULL)
        {
        	//strcpy (defaultDevName, buffer);
        	//firstDeviceName=SD_FALSE;
        }			   
		else
        {
       	 	printf ( "\n Device '%s' not found 221 ,stat=%d ", dev_iccp_name, pstAssociationInfo->pstAssocCtrl->state);
       	 	//DisplayDeviceYYP ();
        	//pause_msg ("\n Press any key to continue");
        	continue;
        }
      //}
      
      		
      req = mic_device_operate (pstAssociationInfo->pstAssocCtrl->mi_conn,pDevice, &devValue);
      if (req==NULL)
        {
        		printf ("Operate Device failed (0x%04x)", mi_op_err);
        }
      else
        {
			printf ("Operate Device issued");
			//need to be modified by yyp 2011.09.29
			//req->u_req_done = doOperateDone;			
			req->user_info[0] = pstAssociationInfo;
			decReqCapacity(pstAssociationInfo);
			
        }
	}/* while(shm_send_addr->head!= cmd_end) */
	return(true);
}/* IEC870_5_Send() */

