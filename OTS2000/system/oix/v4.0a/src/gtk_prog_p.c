
/*
	2006.3 updated acorrding to v4.0db by zyp 
	UNIX  have SYN  TO  Windows   by chi.hailong 2007.05.09
	2007.06.13 updated by wzg
*/


#include	<stdio.h>

	

#define	PROG_DEF
#define	DEBUG

#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

extern char            cmd_str[100];/* added by zyp on 2007.7.31*/

extern void GetStationInfo();
extern void GetSgzyHistory(int crt_n);
extern void AlmBlinkReset (int crt_n,POINTER *pt );

extern void	ManageSubDspWnd (int crt_n , DMS_COMMON	*dms_cmm);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void ProgIoCtrlBlock(int crt_n,GBRCD   *rcd);
extern short   GetCurWSCtrlPrvg();
extern void ProgIoGroupCtrlBlock(int crt_n,DMS_COMMON dms_cmm);
extern void	ProgSetValue (int crt_n, GtkWidget *parent,DMS_COMMON	dms_cmm);
extern void ProgManStatSet(int crt_n,GBRCD   *rcd);
extern void ProgManCurveSet(int crt_n,GBRCD   *rcd);
extern void	ProgDevCtrl (int crt_n, GBRCD	*rcd );
extern void	ProgBrkCtrl (int crt_n, GBRCD	*rcd );
extern void	ProgSwchCtrl (int crt_n,GBRCD	* rcd );
extern void ProgUnitCtrl (int crt_n, GBRCD	*rcd );
extern void	ProgGateCtrl (int crt_n, GBRCD	*rcd );
extern void ProgCTrans(int crt_n,GBRCD   *rcd);
extern void	ProgDevModeSet (int crt_n, GBRCD   *rcd );
extern void	ProgSignRtn(int crt_n, GBRCD   *rcd );
extern void	ProgPlcSwitch (int crt_n, GBRCD	*rcd );
extern void ProgCLcuParam(int crt_n, GBRCD	*rcd );
extern void ProgTimeSync(int crt_n,GBRCD   *rcd);
extern void	ProgNetCtrl (int crt_n, GBRCD   *rcd );
extern void ProgActPwrAdj(int crt_n,GBRCD	*rcd );
extern void	ProgReActPwrAdj(int crt_n, GBRCD	*rcd );
extern void	ProgVoltStepAdj (int crt_n, GBRCD	*rcd );
extern void	ProgSpeedStepAdj (int crt_n, GBRCD	*rcd );
extern void	ProgModifyParameter (int crt_n, GBRCD   *rcd );
extern void ProgIostSet(int crt_n,GBRCD   *rcd);
extern void ProgIoattrSet(int crt_n,GBRCD   *rcd);
extern void	ProgDevOnOffLine (int crt_n, GBRCD   *rcd );
extern void ProgCOnoffForce(int crt_n,GBRCD   *rcd);
extern void ProgCAnaForce(int crt_n,GBRCD   *rcd);
extern void	ProgHostCtrl (int crt_n, GBRCD	*rcd );
extern void ProgCurveInput(int crt_n,GBRCD   *rcd);
extern void	ProgSysModeSet (int crt_n,GBRCD	*rcd );
extern void	ProgOpValueSet (int crt_n, GBRCD   *rcd );
extern int    GetSrc_Id();
extern void PopupDynCtrlMenu (int crt_n,GtkWidget	*parent,  GBRCD 	*rec);
extern int    GetHost_Cmd();
extern void ProgDataPolling(int crt_n, GBRCD	*rcd );
extern void 	ProgDbTab (GtkWidget *parent, int crt_n );
extern void	ChangePointTable (int crt_n,int cmd_type );
extern void GetPtrTable (int crt_n ,char dbname[POINT_NAME_SIZE]);
extern void 	ProgAlarmTab (GtkWidget *parent,	int crt_n  ,int TabMode  );
extern void ProgSgzySet (int crt_n );

void	ProgDoAckCel (int crt_n );
 
extern	POINTER		pnt_tab_ptr[MAX_CRT_NUMBER];
extern int   ana_sbo_id[MAX_CRT_NUMBER];
extern int	SearchMenuId ( char	menu_name[MENU_NAME_LEN]);
extern void FetchSoftCopy (int crt_n );
extern void ProgTrendSet (int  crt_n );
extern void GetTrendHistory (int crt_n );
extern void ProgManSetTimeInput(int crt_n,GBRCD  *rcd);

extern void PopupTimeManSetBox (GtkWidget *parent ,int crt_n);

BOOL	GetPlantCtrlRight (int crt_n,DMS_COMMON dms,int IsCascade,char *errormsg )
{
	char    ctrl_sbo_str[POINT_NAME_SIZE+12];
	char    stn_name[STATION_NAME_SIZE];
	char    dev_name[GROUP_NAME_SIZE];
	UCHAR	sys_lcu_id;
        
	strcpy(errormsg , "");
	GetSysLcuId(dms.point.stn_id,&sys_lcu_id);
	if(GetStnNameById(dms.point.stn_id,stn_name)!=-1)       
		strcpy(ctrl_sbo_str,stn_name);       
	else
	{
		sprintf(errormsg,"no this station:%d\n",dms.point.stn_id);
		return FALSE;
	}
        
	strcat(ctrl_sbo_str,".");
	if(GetGroupNameById(dms.point.stn_id,sys_lcu_id,dev_name)!=-1)        
		strcat(ctrl_sbo_str,dev_name);       
	else
	{
		sprintf(errormsg,"no this group:%d.%d\n",dms.point.stn_id,sys_lcu_id);
		return FALSE;
	}        
        
	strcat(ctrl_sbo_str,".SYS"); 

	if(IsCascade==TRUE)
		strcat(ctrl_sbo_str,".CAS_DISP_OIX..");
	else
		strcat(ctrl_sbo_str,".RC_LC_OIX..");        
        
	if( InterLock_Judge(ctrl_sbo_str)!=0)
		return TRUE;
	else
	{
		if(ana_sbo_id[crt_n]!=1)
		{
			strcpy(sbo_pnt_name[crt_n],ctrl_sbo_str);  
			ManageSubDspWnd ( crt_n, &dms );
		}
		return FALSE;
	}	
}

BOOL	GetLcuRegRight (int crt_n,DMS_COMMON dms,int IsCascade, char *errormsg)
{
	char    reg_sbo_str[POINT_NAME_SIZE+12];
	char    stn_name[STATION_NAME_SIZE];
	char    dev_name[GROUP_NAME_SIZE];
	UCHAR	sys_lcu_id;
        
	strcpy(errormsg , "") ;
	GetSysLcuId(dms.point.stn_id,&sys_lcu_id);
	if(GetStnNameById(dms.point.stn_id,stn_name)!=-1)       
		strcpy(reg_sbo_str,stn_name);        
	else
	{
		sprintf(errormsg,"no this station:%d\n",dms.point.stn_id );
		return FALSE;
	}

	strcat(reg_sbo_str,".");
	if(GetGroupNameById(dms.point.stn_id,sys_lcu_id,dev_name)!=-1)        
		strcat(reg_sbo_str,dev_name);        
	else
	{
		sprintf(errormsg,"no this group:%d.%d\n",dms.point.stn_id,sys_lcu_id ); 
		return FALSE;
	}

	GetGroupNameById(dms.point.stn_id,dms.point.dev_id,dev_name);
	strcat(reg_sbo_str,".SYS.");                
	strcat(reg_sbo_str,dev_name);
	if(IsCascade==TRUE)
		strcat(reg_sbo_str,"_P_CAS_OIX..");
	else
		strcat(reg_sbo_str,"_P_RC_LC_OIX..");
        
	if( InterLock_Judge(reg_sbo_str)!=0)
		return TRUE;
	else
	{
		if(ana_sbo_id[crt_n]!=1)
		{
			strcpy(sbo_pnt_name[crt_n],reg_sbo_str);
			ManageSubDspWnd ( crt_n, &dms );
			strcpy(errormsg,"");
		}
		return FALSE;
	}
}

BOOL	GetLcuCtrlRight (int crt_n,DMS_COMMON dms,int IsCascade, char *errormsg)
{
	char    ctrl_sbo_str[POINT_NAME_SIZE+12];
	char    stn_name[STATION_NAME_SIZE];
	char    dev_name[GROUP_NAME_SIZE];
	UCHAR	sys_lcu_id;
        
	strcpy(errormsg , "");
	GetSysLcuId(dms.point.stn_id,&sys_lcu_id);
	if(GetStnNameById(dms.point.stn_id,stn_name)!=-1)       
		strcpy(ctrl_sbo_str,stn_name);       
	else
	{
		sprintf(errormsg, "no this station:%d\n",dms.point.stn_id);
		return FALSE;
	}
        
	strcat(ctrl_sbo_str,".");
	if(GetGroupNameById(dms.point.stn_id,sys_lcu_id,dev_name)!=-1)        
		strcat(ctrl_sbo_str,dev_name);       
	else
	{
		sprintf(errormsg, "no this group:%d.%d\n",dms.point.stn_id,sys_lcu_id);
		return FALSE;
	}
        
	GetGroupNameById(dms.point.stn_id,dms.point.dev_id,dev_name);
	strcat(ctrl_sbo_str,".SYS.");                
	strcat(ctrl_sbo_str,dev_name);
	if(IsCascade==TRUE)
		strcat(ctrl_sbo_str,"_CAS_OIX..");       
	else           
		strcat(ctrl_sbo_str,"_RC_LC_OIX..");
       
	if( InterLock_Judge(ctrl_sbo_str)!=0)
		return TRUE;
	else
	{
		if(ana_sbo_id[crt_n]!=1)
		{
			strcpy(sbo_pnt_name[crt_n],ctrl_sbo_str);
			ManageSubDspWnd ( crt_n, &dms );
			strcpy(errormsg,"") ;
		}
		return FALSE;
	}	
}

BOOL	GetLcuForceRight (int crt_n,DMS_COMMON dms,int IsCascade, char *errormsg)
{
	char    ctrl_sbo_str[POINT_NAME_SIZE+12];
	char    stn_name[STATION_NAME_SIZE];
	char    dev_name[GROUP_NAME_SIZE];
	UCHAR	sys_lcu_id;
        
	strcpy(errormsg , "");
	GetSysLcuId(dms.point.stn_id,&sys_lcu_id);
	if(GetStnNameById(dms.point.stn_id,stn_name)!=-1)       
		strcpy(ctrl_sbo_str,stn_name);       
	else
	{
		sprintf(errormsg, "no this station:%d\n",dms.point.stn_id);
		return FALSE;
	}
        
	strcat(ctrl_sbo_str,".");
	if(GetGroupNameById(dms.point.stn_id,sys_lcu_id,dev_name)!=-1)        
		strcat(ctrl_sbo_str,dev_name);       
	else
	{
		sprintf(errormsg, "no this group:%d.%d\n",dms.point.stn_id,sys_lcu_id);
		return FALSE;
	}
        
	GetGroupNameById(dms.point.stn_id,dms.point.dev_id,dev_name);
	strcat(ctrl_sbo_str,".SYS.");                
	strcat(ctrl_sbo_str,dev_name);
	if(IsCascade==TRUE)
		strcat(ctrl_sbo_str,"_CAS_FORCE_OIX..");       
	else           
		strcat(ctrl_sbo_str,"_FORCE_OIX..");
       
	if( InterLock_Judge(ctrl_sbo_str)!=0)
		return TRUE;
	else
	{
		if(ana_sbo_id[crt_n]!=1)
		{
			strcpy(sbo_pnt_name[crt_n],ctrl_sbo_str);
			ManageSubDspWnd ( crt_n, &dms );
			strcpy(errormsg,"") ;
		}
		return FALSE;
	}	
}


BOOL	GetCtrlRight (int crt_n,DMS_COMMON dms_cmm,int IsCascade)
{
	char error_msg[256] ;

    if(GetPlantCtrlRight(crt_n,dms_cmm,IsCascade,error_msg)!=TRUE)
    {
		if (strcmp(error_msg,"")!=0)
            PopupMessageBox(main_w[crt_n],error_msg);
		return FALSE;
    }
    if(GetLcuCtrlRight(crt_n,dms_cmm,IsCascade,error_msg)!=TRUE)
    {
		if (strcmp(error_msg,"")!=0)
            PopupMessageBox(main_w[crt_n],error_msg); 
		return FALSE;
    }
    return TRUE;
}

BOOL	GetRegRight (int crt_n,DMS_COMMON dms_cmm,int IsCascade)
{
	char error_msg[256];

    if(GetPlantCtrlRight(crt_n,dms_cmm,IsCascade,error_msg)!=TRUE)
    {
		if (strcmp(error_msg,"")!=0)
			PopupMessageBox(main_w[crt_n],error_msg); 
		return FALSE;
    }
    if(GetLcuRegRight(crt_n,dms_cmm,IsCascade,error_msg)!=TRUE)
    {			
		if (strcmp(error_msg,"")!=0)
			PopupMessageBox(main_w[crt_n],error_msg); 
		return FALSE;
    }
    return TRUE;
}

BOOL	GetForceRight (int crt_n,DMS_COMMON dms_cmm,int IsCascade)
{
	char error_msg[256];

    if(GetPlantCtrlRight(crt_n,dms_cmm,IsCascade,error_msg)!=TRUE)
    {
		if (strcmp(error_msg,"")!=0)
			PopupMessageBox(main_w[crt_n],error_msg); 
		return FALSE;
    }
    if(GetLcuForceRight(crt_n,dms_cmm,IsCascade,error_msg)!=TRUE)
    {			
		if (strcmp(error_msg,"")!=0)
			PopupMessageBox(main_w[crt_n],error_msg); 
		return FALSE;
    }
    return TRUE;
}



void	ProgHandler (int crt_n,GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	LCU_ID		lcu_id;
	char		info[50];
  	short	ws_ctrl_prvg;
	char    error_msg[200] ;

	int     IsCascade=IsLocalHostDispatch();

	GetStationInfo();
	ana_sbo_id[crt_n]= 0;
        
        strcpy(cmd_str,"");
        printf("IN ProgHandler\n"); 
        ws_ctrl_prvg=GetCurWSCtrlPrvg();

        switch( rcd->lnk.act.fmt_def.prog.id )
	{
	case	PrgN_IO_CTRL_BLOCK:
	case	PrgN_IO_CTRL_GROUP_BLOCK :
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
			return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_CTRL_BLOCK_SET))
		{
			PopupMessageBox ( main_w[crt_n], MSG_CTRL_BLOCK_SETTING );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(PrgN_IO_CTRL_BLOCK == rcd->lnk.act.fmt_def.prog.id)
			ProgIoCtrlBlock ( crt_n, rcd );
		else if(PrgN_IO_CTRL_GROUP_BLOCK == rcd->lnk.act.fmt_def.prog.id)
			ProgIoGroupCtrlBlock ( crt_n, dms_cmm );
		break;

		
	case	PrgN_MAN_VALUE_SET:
	case 	PrgN_MAN_STAT_SET:
	case	PrgN_MAN_CURVE_SET:                
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
		if(!GetRegRight(crt_n,dms_cmm,IsCascade))
		{		
			return;
		}

		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

	/* 	if(!CheckUserPrivilege(cur_usr_name, PRIV_RTVALUE_MAN_SET))
		 if(!CheckUserPrivilege(cur_usr_name, rcd->lnk.act.fmt_def.prog.id ==  PrgN_MAN_CURVE_SET ?PRIV_SYS_RUN_SET : PRIV_RTVALUE_MAN_SET))
		{
			PopupMessageBox ( main_w[crt_n], MSG_MANUAL_SETTING );					
			return;
		}  */
		if (rcd->lnk.act.fmt_def.prog.id ==  PrgN_MAN_CURVE_SET)
		{
			if(!CheckUserPrivilege(cur_usr_name,   PRIV_SYS_RUN_SET  ))
			{
				PopupMessageBox ( main_w[crt_n], MSG_SYS_RUN_SETTING );					
				return;
			}
		}else
		{ 
			if(!CheckUserPrivilege(cur_usr_name,    PRIV_RTVALUE_MAN_SET))
			{
				PopupMessageBox ( main_w[crt_n], MSG_MANUAL_SETTING );					
				return;
			}
			
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		if(PrgN_MAN_VALUE_SET == rcd->lnk.act.fmt_def.prog.id)
			ProgSetValue ( crt_n,main_w[crt_n], dms_cmm );
		else if(PrgN_MAN_STAT_SET == rcd->lnk.act.fmt_def.prog.id)
			ProgManStatSet ( crt_n, rcd );	
		else if(PrgN_MAN_CURVE_SET == rcd->lnk.act.fmt_def.prog.id)
			ProgManCurveSet ( crt_n, rcd );                   
		break;


	case	PrgN_C_DEVICE:
	case	PrgN_C_BREAKER:
	case	PrgN_C_SWITCH:
	case	PrgN_C_UNIT:
	case	PrgN_C_GATE:
	case	PrgN_C_TRANS:
	case	PrgN_C_DEV_MOD:
	case    PrgN_C_SIGNAL_RESET:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
			return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_LCU_CTRL))
		{
			PopupMessageBox ( main_w[crt_n], MSG_PLANT_DEV_CTRL );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(PrgN_C_DEVICE == rcd->lnk.act.fmt_def.prog.id)
			ProgDevCtrl ( crt_n, rcd );
		else if(PrgN_C_BREAKER == rcd->lnk.act.fmt_def.prog.id)
			ProgBrkCtrl ( crt_n, rcd );
		else if(PrgN_C_SWITCH == rcd->lnk.act.fmt_def.prog.id)
			ProgSwchCtrl ( crt_n, rcd );
		else if(PrgN_C_UNIT == rcd->lnk.act.fmt_def.prog.id)
			ProgUnitCtrl ( crt_n, rcd );				
		else if(PrgN_C_GATE == rcd->lnk.act.fmt_def.prog.id)
			ProgGateCtrl ( crt_n, rcd );
		else if(PrgN_C_TRANS == rcd->lnk.act.fmt_def.prog.id)
			ProgCTrans ( crt_n, rcd );
		else if(PrgN_C_DEV_MOD == rcd->lnk.act.fmt_def.prog.id)
			ProgDevModeSet ( crt_n, rcd );
		else if(PrgN_C_SIGNAL_RESET == rcd->lnk.act.fmt_def.prog.id)
			ProgSignRtn ( crt_n, rcd );
        break;


	case	PrgN_C_PLC_SWITCH:
	case	PrgN_C_LCU_PARAM:
	case    PrgN_TIME_SYNC:   
	case	PrgN_C_NET_SWITCH:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
			return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_SYS_CONF_MGR))
		{
			PopupMessageBox ( main_w[crt_n], MSG_SYS_CONFIG_MGR );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(PrgN_C_PLC_SWITCH == rcd->lnk.act.fmt_def.prog.id)
			ProgPlcSwitch ( crt_n, rcd );
		else if(PrgN_C_LCU_PARAM == rcd->lnk.act.fmt_def.prog.id)
			ProgCLcuParam ( crt_n, rcd );		
		else if(PrgN_TIME_SYNC == rcd->lnk.act.fmt_def.prog.id)
			ProgTimeSync(crt_n,rcd);
		else if(PrgN_C_NET_SWITCH == rcd->lnk.act.fmt_def.prog.id)
			ProgNetCtrl ( crt_n, rcd );					
        break;


	case	PrgN_C_APOWER:
	case	PrgN_C_RPOWER:
	case	PrgN_C_VOLT_STEP :
	case	PrgN_C_SPEED_STEP:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
			return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_POWER_ADJ))
		{
			PopupMessageBox ( main_w[crt_n], MSG_POWER_ADJ );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		if(!GetRegRight(crt_n,dms_cmm,IsCascade))
		{
			PopupMessageBox(main_w[crt_n],MSG_REG_RIGHT_NOT_HERE);
			return;
		}

		if(PrgN_C_APOWER == rcd->lnk.act.fmt_def.prog.id)
			ProgActPwrAdj ( crt_n, rcd );
		else if(PrgN_C_RPOWER == rcd->lnk.act.fmt_def.prog.id)
			ProgReActPwrAdj ( crt_n, rcd );
		else if(PrgN_C_VOLT_STEP == rcd->lnk.act.fmt_def.prog.id)
			ProgVoltStepAdj ( crt_n, rcd );
		else if(PrgN_C_SPEED_STEP == rcd->lnk.act.fmt_def.prog.id)
			ProgSpeedStepAdj ( crt_n, rcd );
		break;


	case	PrgN_MODFY_PRMT:		/*ANA_PARAM_SET*/
	case	PrgN_C_ONOFF_SET:
	case	PrgN_C_IMP_PARAM_SET:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
		{
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
			memcpy(&pnt_tab_ptr[crt_n],&dms_cmm.point ,sizeof(POINTER));
		}
		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(rcd->lnk.act.fmt_def.prog.id == PrgN_C_IMP_PARAM_SET)
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_IMP_MAN_SET))
			{
				PopupMessageBox ( main_w[crt_n], MSG_IMP_MANUAL_SETTING   );
				return;
			}
		}
		else
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_LIMIT_PARA_SET))
			{
				PopupMessageBox ( main_w[crt_n], MSG_LMT_PARAM_SETTING   );
				return;
			}
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		ProgModifyParameter ( crt_n, rcd );
		break;


	case	PrgN_IOST_SET:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );

		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Scan_state_Enable" ) ==0 ||
			strcmp ( rcd->lnk.act.fmt_def.prog.status, "Scan_state_Disable" ) ==0||
			strcmp ( rcd->lnk.act.fmt_def.prog.status, "Alm_state_Enable" ) ==0||
			strcmp ( rcd->lnk.act.fmt_def.prog.status, "Alm_state_Disable" ) ==0)
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_SCAN_ALM_SET))
			{
				PopupMessageBox ( main_w[crt_n], MSG_SCAN_ALM_ENABLE   );
				return;
			}
		}
		else if(strcmp ( rcd->lnk.act.fmt_def.prog.status, "Alm_ack_state" ) ==0)
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
			{
				PopupMessageBox ( main_w[crt_n], MSG_ALARM_ACK );
				return;
			}
		}
		else if(strcmp ( rcd->lnk.act.fmt_def.prog.status, "Tag_state_Enable" ) ==0||
			strcmp ( rcd->lnk.act.fmt_def.prog.status, "Tag_state_Disable" ) ==0)
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_TAG_OPR))
			{
				PopupMessageBox ( main_w[crt_n], MSG_TAG_OPR_SETTING   );
				return;
			}
		}
		else
			return;

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}
	  
		ProgIostSet ( crt_n, rcd );
		break;


	case	PrgN_IOATTR_SET:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_IO_ATTR_SET))
		{	
			PopupMessageBox ( main_w[crt_n], MSG_IO_ATTR_SETTING   );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}
	  
		ProgIoattrSet ( crt_n, rcd );
		break;
	

	case	PrgN_C_LCU_ONOFF:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
		//wzgif(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
		//	return;

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
			return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_SYS_CONF_MGR))
		{	
			PopupMessageBox ( main_w[crt_n], MSG_SYS_CONFIG_MGR   );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}
	  
		ProgDevOnOffLine ( crt_n, rcd );
		break;


	case	PrgN_C_ONOFF_FORCE:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
			return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(dms_cmm.point.data_type==OUTS_TYPE)
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_OUT_FORCE_SET))
			{
				PopupMessageBox ( main_w[crt_n], MSG_OUT_FORCE_SETTING );
				return;
			}
		}
		else if(dms_cmm.point.data_type==SOE_TYPE || dms_cmm.point.data_type==POL_TYPE)
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_IN_FORCE_SET))
			{
				PopupMessageBox ( main_w[crt_n], MSG_IN_FORCE_SETTING );
				return;
			}
		}
		else
		{
			PopupMessageBox ( main_w[crt_n], MSG_OPR_DATATYPE_ERROR );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		if(!GetForceRight(crt_n,dms_cmm,IsCascade))                     
			return;

		ProgCOnoffForce ( crt_n, rcd );
		break;


	case	PrgN_C_ANA_FORCE:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
			return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(dms_cmm.point.data_type==OUTA_TYPE)
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_OUT_FORCE_SET))
			{
				PopupMessageBox ( main_w[crt_n], MSG_OUT_FORCE_SETTING );
				return;
			}
		}
		else if(dms_cmm.point.data_type==ANA_TYPE)
		{
			if(!CheckUserPrivilege(cur_usr_name, PRIV_IN_FORCE_SET))
			{
				PopupMessageBox ( main_w[crt_n], MSG_IN_FORCE_SETTING );
				return;
			}
		}
		else
		{
			PopupMessageBox ( main_w[crt_n], MSG_OPR_DATATYPE_ERROR );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		if(!GetForceRight(crt_n,dms_cmm,IsCascade))                     
			return;

		ProgCAnaForce ( crt_n, rcd );
		break;


	case	PrgN_C_HOST_SYS:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_SYS_RUN_SET/*PRIV_SYS_CONF_MGR*/))
		{
			PopupMessageBox ( main_w[crt_n], MSG_SYS_RUN_SETTING /*MSG_SYS_CONFIG_MGR*/ );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		ProgHostCtrl ( crt_n, rcd );
		break;


	case PrgN_CURVE_INPUT:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
		//	return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_SYS_RUN_SET))
		{
			PopupMessageBox ( main_w[crt_n], MSG_SYS_RUN_SETTING );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		ProgCurveInput(crt_n, rcd);                  
		break;      


	case    PrgN_C_SYS_MODE_SET:
	case    PrgN_OP_VALUE_SET:
		if( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
		{
			sprintf(info,"%s%s",_CS_(OIX_STR_DB_NO_POINT),rcd->lnk.dbname);                        
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
		else
			GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
		if(!GetCtrlRight(crt_n,dms_cmm,IsCascade))                     
			return;

		if(ws_ctrl_prvg!=1)
		{
			if (ws_ctrl_prvg==-1 )
				PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
			else			
				PopupMessageBox(main_w[crt_n],MSG_HOST_NOT_ON);
			return;
		}
    
		if(	dms_cmm.fixed.iost.tagState)
		{
			PopupMessageBox ( main_w[crt_n], MSG_PROHIBIT_TO_OPERATE );
			return;
		}

		if(!CheckUserPrivilege(cur_usr_name, PRIV_SYS_RUN_SET))
		{
			PopupMessageBox ( main_w[crt_n], MSG_SYS_RUN_SETTING );
			return;
		}

		lcu_id.stn_id= dms_cmm.point.stn_id;
		lcu_id.dev_id= dms_cmm.point.dev_id;
		if(!CheckHostPrivilege(local_hostname, lcu_id))
		{
			PopupMessageBox ( main_w[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
			return;
		}

		if(!GetPlantCtrlRight(crt_n,dms_cmm,IsCascade,error_msg))
		{
			if(strcmp(error_msg,"")!=0)
				PopupMessageBox( main_w[crt_n],error_msg); 
			return ;
		}

		if(PrgN_C_SYS_MODE_SET == rcd->lnk.act.fmt_def.prog.id)
			ProgSysModeSet ( crt_n, rcd );
		else if(PrgN_OP_VALUE_SET == rcd->lnk.act.fmt_def.prog.id)
			ProgOpValueSet ( crt_n, rcd );
		break;


	case	PrgN_ALARM_TAB:
		ProgAlarmTab ( main_w[crt_n],crt_n ,FALSE);
		break;

	case	PrgN_SOFT_COPY:
/*#ifdef SUN_OS*/
		FetchSoftCopy ( crt_n );
/*#endif*/
		break;

	case	PrgN_POINT_TAB:
		GetPtrTable ( crt_n,rcd->lnk.dbname );
		break;

	case	PrgN_SGZY_SET:	
/*#ifdef  SUN_OS*/		
		if(CheckUserPrivilege(cur_usr_name, PRIV_SGZY_SET))
		{
//			if ( crt_n == CRT_1 )
				ProgSgzySet ( crt_n );	
//			else
//				gdk_beep();
		}
		else
			PopupMessageBox ( main_w[crt_n], MSG_SGZY_TREND_SETTING );

		break;
	
	case	PrgN_GET_SGZY_HISTORY:

//		if ( crt_n == CRT_1 )
			GetSgzyHistory ( crt_n );
//		else
//			gdk_beep();

		break;

	case	PrgN_TEMP_TREND_SET:
		if(CheckUserPrivilege(cur_usr_name, PRIV_SGZY_SET))
		{

//			if ( crt_n == CRT_1 )
				ProgTrendSet ( crt_n );	
//			else
//				gdk_beep();

		}
		else
			PopupMessageBox ( main_w[crt_n], MSG_SGZY_TREND_SETTING );

		break;
	
	case	PrgN_GET_TEMP_TREND:

//		if ( crt_n == CRT_1 )
		  	GetTrendHistory ( crt_n );
//		else
//			gdk_beep();

		break;
                
	case	PrgN_ALM_BLINK_RESET:
		if(CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
			AlmBlinkReset (crt_n, &dms_cmm.point);
		else
	   		PopupMessageBox ( main_w[crt_n], MSG_ALARM_ACK);
		break;
                
	case	PrgN_PREV_POINT:
	case	PrgN_NEXT_POINT:
	case	PrgN_ANALOG_TAB:
	case	PrgN_IMPULSE_TAB:
	case	PrgN_IND_POL_TAB:
	case	PrgN_IND_INT_TAB:
	case	PrgN_OUTA_TAB:
	case	PrgN_OUTS_TAB:
	case	PrgN_SYSMSG_TAB:
	case	PrgN_PARAM_TAB:
	case	PrgN_PREV_LCU:
	case	PrgN_NEXT_LCU:
	case	PrgN_PREV_STN:
	case	PrgN_NEXT_STN:
		ChangePointTable ( crt_n, rcd->lnk.act.fmt_def.prog.id );
		break;

	case	PrgN_DB_LIST_TAB:
		ProgDbTab ( main_w[crt_n], crt_n );
		break;
                
	case	PrgN_C_DATA_POLLING:
		ProgDataPolling ( crt_n, rcd );				
		break;

	case	PrgN_DO_ACKCEL:
		ProgDoAckCel ( crt_n);	
		break;

	case	PrgN_CANCEL:
		break;
	case  PrgN_SCENE_REDISP:
PopupMessageBox ( main_w[crt_n], "scene redisplay !!!1" );
		break;
	case PrgN_MAN_TIME_SET:
	ProgManSetTimeInput(crt_n, rcd);
//	PopupTimeManSetBox(main_w[crt_n],crt_n);
		break;

	default:
		sprintf (error_msg, "Unsupported program id: %d", rcd->lnk.act.fmt_def.prog.id );
		PopupMessageBox ( main_w[crt_n], error_msg );	
		break;
	}


 //  strcpy(cmd_str,"");  // ¡¤¨¤?1?t??2?¦Ì£¤??¨®D?¨¹¨¢???3?

}



void	ProgDoAckCelBrdcst (int crt_n )
{
	char		*command;
	LAN_HEAD	cmd_head;	
	time_t	newtime;
	struct	tm	devtime;

	newtime = time(NULL);
	devtime = *localtime( &newtime);

#ifdef SUN_OS
	Swap4Byte(&newtime);
#endif
	cmd_buf[crt_n].second 		= devtime.tm_sec;
	cmd_buf[crt_n].minute 		= devtime.tm_min;
	cmd_buf[crt_n].hour 		= devtime.tm_hour;
	cmd_buf[crt_n].day 		= devtime.tm_mday;
	cmd_buf[crt_n].month		= devtime.tm_mon+1;
	cmd_buf[crt_n].year 		= devtime.tm_year+1900; 
	
#ifdef SUN_OS	
	Swap2Byte(&cmd_buf[crt_n].year);
#endif
	printf ( "OIX: in PProgDoAckCelBrdcst BROADCAST :::: command_buf will be send out !!!\n" );
	command 	= ( char *)&cmd_buf[crt_n];
	cmd_buf[crt_n].usr_id = (unsigned char)cur_usr_id;
	cmd_head.length 	= sizeof ( cmd_buf[crt_n] );	
	cmd_head.src_id 	= GetHost_Cmd(); 
	cmd_head.dest_id 	= 0;
	cmd_head.dest_stn 	= 0;
	cmd_head.dp_type 	= COMM_RECORD_TIME;
	cmd_head.packet_num 	= 0;
#ifdef DEBUG
	printf("cmd_head::length=%d,dest_id=%d,dest_stn=%d,dp_type=%d,src_id=%d\n",cmd_head.length,cmd_head.dest_id,cmd_head.dest_stn,cmd_head.dp_type,cmd_head.src_id);
	printf("cmd_buf::type_id=%d,stn_id=%d,dev_id=%d,data_type=%d,pt_id=%d\n",cmd_buf[crt_n].type_id,cmd_buf[crt_n].stn_id,cmd_buf[crt_n].dev_id,cmd_buf[crt_n].data_type,cmd_buf[crt_n].point_id);
	printf("cmd_buf::cmd_status.opr=%d,cmd_status.state=%d,idata=%d,fdata=%.2f,ctrl_addr=%d,host_id=%d,usr_id=%d\n",cmd_buf[crt_n].status.opr,cmd_buf[crt_n].status.state,cmd_buf[crt_n].data.iValue,cmd_buf[crt_n].data.fValue,cmd_buf[crt_n].ctrl_addr,cmd_buf[crt_n].host_id,cmd_buf[crt_n].usr_id);
	printf("cmd_buf::usr_name=%s,%d-%d %d:%d:%d\n",cmd_buf[crt_n].usr_login_name,cmd_buf[crt_n].month,cmd_buf[crt_n].day,cmd_buf[crt_n].hour,cmd_buf[crt_n].minute,cmd_buf[crt_n].second);
#endif

#ifdef SUN_OS
	Swap2Byte(&cmd_buf[crt_n].point_id);
	Swap2Byte(&cmd_buf[crt_n].ctrl_addr);
	Swap2Byte(&cmd_buf[crt_n].status);	
	Swap4Byte(&cmd_buf[crt_n].data);
	Swap2Byte(&cmd_head.length);
#endif
	lan_out ( command, cmd_head,0 );
	printf ( "OIX: after ProgDoAckCelBrdcst: Lan_out \n" );
}

void	ProgCallAckCelMenu (int  crt_n,GBRCD	*rcd )
{
	char	menu_name[MENU_NAME_LEN];
	
	rcd->lnk.act.ctrl_type	= CTRL_REQ_MENU;
	rcd->lnk.act.ctrl_tag	= CTRL_TAG_ON;
	strcpy ( menu_name, "Ch_AckCel" );
        strcpy (rcd->lnk.act.fmt_def.menu.name, menu_name );   
	rcd->lnk.act.fmt_def.menu.id	= SearchMenuId ( menu_name );
	ctrl_rcd_parent[crt_n]	= *rcd;

	PopupDynCtrlMenu ( crt_n, main_w[crt_n], rcd );	 
/*	memset(rcd,0 , sizeof(GBRCD)) ; free temporary variant*/
}

void	ProgDoAckCel ( int crt_n )
{
	char		*command;	
	LAN_HEAD	cmd_head;

	char	sys_lcu_id;
	time_t	newtime;
	struct	tm	devtime;

	newtime = time(NULL);
	devtime = *localtime( &newtime);

#ifdef SUN_OS
	Swap4Byte(&newtime);
#endif
	cmd_buf[crt_n].second 		= devtime.tm_sec;
	cmd_buf[crt_n].minute 		= devtime.tm_min;
	cmd_buf[crt_n].hour 		= devtime.tm_hour;
	cmd_buf[crt_n].day 		= devtime.tm_mday;
	cmd_buf[crt_n].month		= devtime.tm_mon+1;
	cmd_buf[crt_n].year 		= devtime.tm_year+1900; 
	
#ifdef SUN_OS	
	Swap2Byte(&cmd_buf[crt_n].year);
#endif
	printf ( "OIX: in ProgDoAckCel :::: command_buf will be send out !!!\n" );
        
	command 	= ( char *)&cmd_buf[crt_n];	

	cmd_head.length 	= sizeof ( cmd_buf[crt_n] );	
	
	//GetLocalSysLcuId(&sys_lcu_id);

	GetSysLcuId(cmd_buf[crt_n].stn_id,&sys_lcu_id);
	//modified by yyp 2012.11.09
	//if( cmd_buf[crt_n].dev_id == sys_lcu_id||cmd_buf[crt_n].type_id==C_HOST_SYS
    //    ||cmd_buf[crt_n].type_id==C_SYS_MODE_SET||cmd_buf[crt_n].type_id==C_NET_SWITCH )
	if( cmd_buf[crt_n].type_id==C_HOST_SYS ||cmd_buf[crt_n].type_id==C_SYS_MODE_SET||cmd_buf[crt_n].type_id==C_NET_SWITCH )
	{
		cmd_head.dest_id	= 0;
		cmd_head.dest_stn	= 0;
		cmd_head.dp_type	= COMM_RECORD_TIME;
	}
	else
	{
		cmd_head.dest_id 	= cmd_buf[crt_n].dev_id;
		cmd_head.dest_stn	= cmd_buf[crt_n].stn_id;
		cmd_head.dp_type	= COMM_TIME;
	}	
	cmd_head.packet_num = 0;
	cmd_head.src_id = GetHost_Cmd (); 
#ifdef DEBUG
	printf("cmd_head::length=%d,dest_id=%d,dest_stn=%d,dp_type=%d,src_id=%d\n",cmd_head.length,cmd_head.dest_id,cmd_head.dest_stn,cmd_head.dp_type,cmd_head.src_id);
	printf("cmd_buf::type_id=%d,stn_id=%d,dev_id=%d,data_type=%d,pt_id=%d\n",cmd_buf[crt_n].type_id,cmd_buf[crt_n].stn_id,cmd_buf[crt_n].dev_id,cmd_buf[crt_n].data_type,cmd_buf[crt_n].point_id);
	printf("cmd_buf::cmd_status.opr=%d,cmd_status.state=%d,data=%d,ctrl_addr=%d,host_id=%d,usr_id=%d\n",cmd_buf[crt_n].status.opr,cmd_buf[crt_n].status.state,cmd_buf[crt_n].data.iValue,cmd_buf[crt_n].ctrl_addr,cmd_buf[crt_n].host_id,cmd_buf[crt_n].usr_id);
	printf("cmd_buf::usr_name=%s,%d-%d %d:%d:%d\n",cmd_buf[crt_n].usr_login_name,cmd_buf[crt_n].month,cmd_buf[crt_n].day,cmd_buf[crt_n].hour,cmd_buf[crt_n].minute,cmd_buf[crt_n].second);
#endif
#ifdef  SUN_OS
	Swap2Byte(&cmd_buf[crt_n].point_id);
	Swap2Byte(&cmd_buf[crt_n].ctrl_addr);
	Swap2Byte(&cmd_buf[crt_n].status);

	Swap4Byte(&cmd_buf[crt_n].data);

	Swap2Byte(&cmd_head.length);
#endif	
	lan_out ( command, cmd_head,0 );
}

void 	CmdBufInit ( int crt_n )
{	

	cmd_buf[crt_n].stn_id		= (char) NULL;
	cmd_buf[crt_n].dev_id 		= ( char ) NULL;
	cmd_buf[crt_n].data_type 	= ( char ) NULL;
	cmd_buf[crt_n].type_id 		= ( char ) NULL;
	cmd_buf[crt_n].point_id		= 0;
	
	cmd_buf[crt_n].status.opr	= 0;
	cmd_buf[crt_n].status.state	= 0;  
    cmd_buf[crt_n].status.result	= 0; 
	cmd_buf[crt_n].status.src       = GetSrc_Id();
	cmd_buf[crt_n].data.iValue	= 0;
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].host_id  	= GetHost_Cmd();
	cmd_buf[crt_n].usr_id 		= ( char ) cur_usr_id;
	strcpy(cmd_buf[crt_n].usr_login_name, cur_usr_name);
}


