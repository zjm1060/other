
/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 		2005.9.28 
	Last Upate :	2005.9.29
        2006.3 updated by zyp for v4.0
	All the command format is modified according to 4.0
	Add the following:: 
	ProgTimeSync ProgCOnoffForce ProgCAnaForce ProgIostSet ProgIoattrSet ProgManStatSet 
	ProgManCurveSet ProgCTrans ProgPlcSwitch
	Unix Have  SYN TO  Windows   by chi  07.06.10 ; 

---------------------------------------------------------------------*/


#include <time.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

//#include	"../../../dps/v4.0a/inc/aud_dev.h"

extern   short		interlock_pt_num;
extern   short		interlock_gt_num;
extern   short		interlock_et_num;
extern   InterLock	*interlock;
extern   I_Group	*group;
extern   I_Entry	*entry;
#define  MSG_OPER_CLASS_UNBLOCK   "ÏÂ´ïÀà½âËøÁî £¬ ÇëÈ·ÈÏ \n ×¢Òâ¸Ã²Ù×÷¿ÉÄÜÒª»¨·ÑºÜ³¤Ê±¼ä£¡ !" 
#define	DEBUG	
 
POINTER	oper_cond_ptr[MAX_CRT_NUMBER];
extern	POINTER		pnt_tab_ptr[MAX_CRT_NUMBER];
extern	char	menu_title_str[50];
char            cmd_str[200];
char			cmd_type_str[20];
static	GBRCD	rectmp[MAX_CRT_NUMBER];
extern gint	oper_cond_timeout_id[MAX_CRT_NUMBER];

static	char	string[100]= {""};



#ifdef SUN_OS
//#define AUD_ALM_BEFORE_CMD              /*·¢ÁîÖ®Ç°ÊÇ·ñÒªÓïÒôÌáÊ¾*/
char    aud_cmd_str[100]="play -v 10 ";
char    end[10]=".wav";
char    send_out[50]="play -v 10 /AUD/CMD_SENDOUT.wav";
#else
char    aud_cmd_str[100]="/MME/audioplay -filename  ";
char    end[10]=".aud";
char    send_out[50]="play -v 10 /AUD/CMD_SENDOUT.aud";
#endif

void	PopupEnterTimeDlg(GtkWidget *parent,int crt_n);

void	UpdateOperCondCanvas(int crt_n); 
int		OpenSubDspFile();
void	ManageSwitchCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd );
void	ManageBrkCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd );
void	ManageDevCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd );
void	ManageHostSwitchCommand (int crt_n,DMS_COMMON *dms_cmm, GBRCD *rcd );
void	ManageNetSwitchCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd );
void 	ManageSysModeSetCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd );
void	ManageOnOffCommand (int crt_n,DMS_COMMON *dms_cmm, GBRCD	*rcd );
void	ManageSubDspWnd (int crt_n , DMS_COMMON	*dms_cmm);
void	ManageGateCommand (int crt_n,DMS_COMMON	*int_data, GBRCD	*rcd );
void	ManageGenCommand (int crt_n ,DMS_COMMON	*int_data,GBRCD	*rcd ) ;
void    ManageCTransCommand(int crt_n,DMS_COMMON	*int_data, GBRCD	*rcd);
void	ManageApowerCommand (int crt_n,DMS_COMMON	*dms_cmm, GBRCD	*rcd );
void	ManageDevModeCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd );
void	ManageRpowerCommand (int crt_n,DMS_COMMON	*dms_cmm, GBRCD	*rcd );
void 	ManageCOnoffForceCommand (int crt_n, DMS_COMMON	*int_data, GBRCD   *rcd );
void	GetCMDTypeStr(int type_id,char *cmd_type_str);

//extern static GtkWidget*CreateValueEntryBox (GtkWidget *parent ,int crt_n ,POINTER *pt);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void 	CmdBufInit ( int crt_n );
extern PopupValueEntryBox (GtkWidget *parent ,int crt_n ,POINTER *pt,int entry_data_class);
extern void	ProgCallAckCelMenu (int  crt_n,GBRCD	*rcd );
extern void DbSelectTypeInit (int crt_n );
extern void	FormCSignRtnPacket (int crt_n,DMS_COMMON *int_data, char	*oper_status );
extern void FormIoCtlrBlockPacket (int crt_n, DMS_COMMON *int_data, char	*oper_status );
extern int 	PopupMessageBoxRet (GtkWidget *parent,char msg[] );
extern void	ProgDoAckCel ( int crt_n );
extern void FormIostSetPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern void FormIoattrSetPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status );
extern void FormManStatSetPacket(int crt_n,DMS_COMMON *int_data, char		*oper_status );
extern void FormManCurveSetPacket(int crt_n,DMS_COMMON *dms_cmm, GBRCD   *rcd);
extern void	FormPlcSwitchPacket (int crt_n,DMS_COMMON *int_data, char	*oper_status );
extern void FormCTimeSyncPacket(int crt_n,DMS_COMMON *int_data, char		*oper_status);
extern void FormCAnaForcePacket (int crt_n, DMS_COMMON *int_data, char	*oper_status );
extern void PopupCurveInputBox(GtkWidget *parent, int crt_n ,POINTER pt);
extern void FormCOnOffForcePacket (int crt_n, DMS_COMMON *int_data, char	*oper_status );
extern void	PopupOperCondWnd (int crt_n );
extern void	SubRedraw (GtkWidget* w );
extern void FormApowerPacket(int crt_n,DMS_COMMON *dms_cmm, char	*oper_status);
extern void FormRpowerPacket(int crt_n,DMS_COMMON *dms_cmm, char	*oper_status);
extern void FormCUnitPacket (int crt_n,DMS_COMMON *int_data, char	*oper_status );
extern void	FormCGatePacket (int crt_n, DMS_COMMON *int_data, char		*oper_status );
extern void	FormCOnOffLinePacket (int crt_n,DMS_COMMON *int_data, char		*oper_status );
extern void	FormCDevModePacket (int crt_n,DMS_COMMON *int_data, char	*oper_status );
extern void	FormHostSwitchPacket (int crt_n,DMS_COMMON *int_data, char	*oper_status );
extern void FormNetSwitchPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status );
extern void FormCDevPacket (int crt_n,DMS_COMMON *int_data, char *oper_status );
extern void	FormCSwchPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status );
extern void FormCTransPacket(int crt_n,DMS_COMMON *dms_cmm, char		*oper_status );
extern void FormSysModePacket (int crt_n,DMS_COMMON *int_data, char		*oper_status );
extern void	FormCBrkPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status );
extern void PopTimeInputDlg(GtkWidget *parent,int crt_n);
extern void PopManSetTimeInputDlg(GtkWidget *parent ,int crt_n ,GBRCD  *rcd);
void	AlmBellProhib ( int crt_n)
{
	DMS_COMMON	dms_cmm;
	POINTER		init_ptr;

	init_ptr.stn_id =1;
	init_ptr.dev_id =1;
	init_ptr.data_type =1;
	init_ptr.pt_id = 1;
	ReadEntryById  ( &init_ptr, &dms_cmm );
        
	 PopupValueEntryBox ( main_w[crt_n], crt_n , &init_ptr,-1);  /* gtk */ 
}

void ProgActPwrAdj(int crt_n,GBRCD	*rcd )
{
	
	DMS_COMMON	dms_cmm;
	short		sbo_type;
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{		
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	if(dms_cmm.point.data_type!=ANA_TYPE && dms_cmm.point.data_type!=PARAM_TYPE
            &&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
	pnt_tab_ptr[crt_n]	= dms_cmm.point;	

	sbo_type = dms_cmm.fixed.ioattr.ctrl;		

/*   	Active Power adjust is based on the limit value. Use this , the enter_value_high_lmt[crt_n] 
	and enter_value_low_lmt[crt_n] is not used */ 	

printf("status=%s\n",rcd->lnk.act.fmt_def.prog.status);

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Power" ) !=0 
		&&strcmp ( rcd->lnk.act.fmt_def.prog.status, "Opening" ) !=0 
		&&strcmp ( rcd->lnk.act.fmt_def.prog.status, "Flux" ) !=0)
	{		
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );
		return;
	}
	if ( sbo_type == SBO_YES)
	{			
		sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_APOWER",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
		sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );  
		if ( sbo_result[crt_n] == 0)	
		{
			ManageSubDspWnd ( crt_n, &dms_cmm );
			PopupMessageBox ( main_w[crt_n], MSG_PWR_REG_CONDTN_UNSATISF );
			return;
		}
		else
			ManageApowerCommand(crt_n, &dms_cmm, rcd);
	} 
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;		
	}
}

void	ProgReActPwrAdj(int crt_n, GBRCD	*rcd )
{	
	DMS_COMMON	dms_cmm;
	short		sbo_type;
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{		
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	if(dms_cmm.point.data_type!=ANA_TYPE && dms_cmm.point.data_type!=PARAM_TYPE
            &&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
	pnt_tab_ptr[crt_n]	= dms_cmm.point;

	sbo_type = dms_cmm.fixed.ioattr.ctrl;	

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Power" ) !=0 
		&&strcmp ( rcd->lnk.act.fmt_def.prog.status, "Volt" ) !=0 
		&&strcmp ( rcd->lnk.act.fmt_def.prog.status, "Cos" ) !=0 /*added on 2008.4.8*/
	   )
	{		
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );
		return;
	}
	if ( sbo_type == SBO_YES)
	{		
		sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_RPOWER",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
		sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );  
		if ( sbo_result[crt_n] == 0)	
		{
			ManageSubDspWnd ( crt_n, &dms_cmm );
			PopupMessageBox ( main_w[crt_n], MSG_RPWR_REG_CONDTN_UNSATISF );
			return;
		}
		else
			ManageRpowerCommand(crt_n, &dms_cmm, rcd);
	}
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;
	}

#ifdef	DEBUG
	printf ( "OIX: in q:: enter_value_high_lmt[crt_n] =%f, enter_value_low_lmt[crt_n]=%f\n", 
		enter_value_high_lmt[crt_n], enter_value_low_lmt[crt_n]);
#endif 
}

void ProgUnitCtrl (int crt_n, GBRCD	*rcd )
{	

	DMS_COMMON	dms_cmm;
	short		sbo_type;	

#ifdef	DEBUG
	printf ( " OIX: in ProgUnitCtrl:::::\n" );
#endif	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{		
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
        pnt_tab_ptr[crt_n]      = dms_cmm.point;
	sbo_type = dms_cmm.fixed.ioattr.ctrl;	
	if(dms_cmm.point.data_type!=SOE_TYPE && dms_cmm.point.data_type!=POL_TYPE
		&& dms_cmm.point.data_type!=SYSMSG_TYPE&&dms_cmm.point.data_type!=OUTS_TYPE 
                &&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}	

	if ( sbo_type == SBO_YES)
	{            
		sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_UNIT",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
	       
                sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );                 
                
                    
		if ( sbo_result[crt_n] != 0)	
			ManageGenCommand ( crt_n, &dms_cmm, rcd );
		else
		{
			ManageSubDspWnd ( crt_n, &dms_cmm );
			return;
		}
	}
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;		
	}
}


void	ProgGateCtrl (int crt_n, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	short		sbo_type;
        short           lcu_sbo_result[MAX_CRT_NUMBER];

#ifdef	DEBUG
	printf ( "OIX:  in ProgGateCtrl::::::\n" );
#endif
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm ) == -1)
	{		
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	if(dms_cmm.point.data_type!=SOE_TYPE && dms_cmm.point.data_type!=POL_TYPE
		&& dms_cmm.point.data_type!=SYSMSG_TYPE&&dms_cmm.point.data_type!=ANA_TYPE 
                && dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}		 
	pnt_tab_ptr[crt_n]      = dms_cmm.point;
	
	sbo_type = dms_cmm.fixed.ioattr.ctrl;
	lcu_sbo_result[crt_n]= 0 ;	
	if ( sbo_type == SBO_YES)
	{		
		sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_GATE",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
		/*if(dms_cmm.point.data_type!=DEVOBJ_TYPE)*/
                    {
                    sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );
                    printf("####sbo_result=%d\n",sbo_result[crt_n]);
                }
              /*  else*/
                if(dms_cmm.point.data_type==DEVOBJ_TYPE)
                    {
                        if(strcmp(rcd->lnk.act.fmt_def.prog.status,"Open")==0)
                            lcu_sbo_result[crt_n]=  dms_cmm.var.devobj.fb_status1.condc0;
                        else if(strcmp(rcd->lnk.act.fmt_def.prog.status,"Close")==0)
                            lcu_sbo_result[crt_n]=  dms_cmm.var.devobj.fb_status1.condc1; 
						else /*(strcmp(rcd->lnk.act.fmt_def.prog.status,"Reset")==0)*/
							lcu_sbo_result[crt_n]= 1 ;

                        if ( sbo_result[crt_n] != 0&&lcu_sbo_result[crt_n]!=0)
                            ManageGateCommand ( crt_n, &dms_cmm, rcd );
                        else
                            {
                                if(sbo_result[crt_n]==0)
                                    ManageSubDspWnd ( crt_n, &dms_cmm);
                                else if(lcu_sbo_result[crt_n]==0)
                                    PopupMessageBox ( main_w[crt_n], MSG_GATE_CONDTN_UNSATISF );
                        }
                            
                }
                else
                    {
                       if ( sbo_result[crt_n] != 0)
                            ManageGateCommand ( crt_n, &dms_cmm, rcd );		
                        else
                        {                    
                            ManageSubDspWnd ( crt_n, &dms_cmm);		  
                            return;
                        }
		}
	}
	else		
	{
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;
	}
}


void	ProgSpeedStepAdj (int crt_n, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	short		sbo_type;
	char  LongNamerStr1[128];

#ifdef	DEBUG
	printf ( "OIX:  in ProgSpeedCtrl::::::\n" );
#endif
	rectmp[crt_n]   = *rcd;
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm ) == -1)
	{		
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	pnt_tab_ptr[crt_n]	= dms_cmm.point;

	sbo_type = dms_cmm.fixed.ioattr.ctrl;			

	if ( sbo_type == SBO_YES)
	{		
		sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_SPEED_STEP",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
		sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );  
		if ( sbo_result[crt_n] == 0)	
		{
			ManageSubDspWnd ( crt_n, &dms_cmm );
		    	PopupMessageBox ( main_w[crt_n], MSG_STEP_PWR_REG_CONDTN_UNSATISF );
			return;
		}
	}
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;
	}
    	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= dms_cmm.point.stn_id;
	cmd_buf[crt_n].dev_id		= dms_cmm.point.dev_id;
	cmd_buf[crt_n].point_id		= dms_cmm.point.pt_id;
	cmd_buf[crt_n].data_type	= dms_cmm.point.data_type;  
	cmd_buf[crt_n].type_id		= C_SPEED_STEP;	
	cmd_buf[crt_n].data.iValue	= 0;
	cmd_buf[crt_n].ctrl_addr	= dms_cmm.fixed.ctrl_addr;
	
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Increase" ) ==0 )	
		cmd_buf[crt_n].status.opr= STEP_INCREASE;
	
	else if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Decrease" ) ==0 )	
		cmd_buf[crt_n].status.opr=  STEP_DECREASE;	
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
	
       /* strcpy ( cmd_str, dms_cmm.fixed.entryLongname );  */
        if(-1!=GetPtCmdMsgStrById(&dms_cmm.point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))   
		{
			GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
			sprintf(cmd_str,"%s:\n\t %s \0",LongNamerStr1 ,string );
		}/* strcat(cmd_str,string);  */

	ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
}

void	ProgVoltStepAdj (int crt_n, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	short		sbo_type;
	char  LongNamerStr1[128];
	
#ifdef	DEBUG
	printf ( "OIX:  in ProgVoltCtrl::::::\n" );
#endif
	rectmp[crt_n]   = *rcd;
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	pnt_tab_ptr[crt_n]	= dms_cmm.point;

	sbo_type = dms_cmm.fixed.ioattr.ctrl;	

	if ( sbo_type == SBO_YES)
	{			
		sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_VOLT_STEP",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
		sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );  
		if ( sbo_result[crt_n] == 0)	
		{
			ManageSubDspWnd ( crt_n, &dms_cmm );
			return;
		}
	}
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;
	}
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= dms_cmm.point.stn_id;
	cmd_buf[crt_n].dev_id		= dms_cmm.point.dev_id;
	cmd_buf[crt_n].point_id		= dms_cmm.point.pt_id;
	cmd_buf[crt_n].data_type	= dms_cmm.point.data_type;
	cmd_buf[crt_n].type_id		= C_VOLT_STEP; 
	cmd_buf[crt_n].data.iValue	= 0;        
	cmd_buf[crt_n].ctrl_addr	= dms_cmm.fixed.ctrl_addr;
	
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Increase" ) ==0 )	
		cmd_buf[crt_n].status.opr = STEP_INCREASE;	
	else if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Decrease" ) ==0 )	
		cmd_buf[crt_n].status.opr = STEP_DECREASE;		
	else	
	{
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );
		return;
	}	
	
        /*strcpy ( cmd_str, dms_cmm.fixed.entryLongname );  */
	
        if(-1!=GetPtCmdMsgStrById(&dms_cmm.point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
			GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
			sprintf(cmd_str,"%s:\n\t %s\0",LongNamerStr1  ,string );
		}
        ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );
}

void ModifyDataClass(int crt_n,GtkWidget *parent, DMS_COMMON	*dms_cmm, POINTER	*entry_ptr,int entry_data_class)
{
	float		value;
	char		modify_type;		
	short		num;

#ifdef DEBUG
	printf ( " in ModifyDataClass entry_data_class=%d,stn_id=%d\n", entry_data_class,entry_ptr->stn_id);
#endif
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= entry_ptr->stn_id;
	cmd_buf[crt_n].dev_id		= entry_ptr->dev_id;
	cmd_buf[crt_n].point_id		= entry_ptr->pt_id; 
	cmd_buf[crt_n].data_type 	= entry_ptr->data_type ;			
	cmd_buf[crt_n].ctrl_addr	= 0;
	switch( entry_ptr->data_type )
	{
	case	ANA_TYPE:
	case	PARAM_TYPE:
            case CURVE_TYPE:
		switch  ( entry_data_class)
		{
		case	DEAD_BAND:
			value	= dms_cmm->var.anlg.eng_para.D_BAND;
			modify_type=ANA_PARAM_SET_D_BAND;
			break;

		case	H_LMT:
			value	= dms_cmm->var.anlg.lmt_para.HI_WARN;
			modify_type=ANA_PARAM_SET_HI_WARN;
			break;

		case	L_LMT:
			value	= dms_cmm->var.anlg.lmt_para.LO_WARN;
			modify_type=ANA_PARAM_SET_LO_WARN;
			break;

		case	HH_LMT:
			value	= dms_cmm->var.anlg.lmt_para.HI_ALARM;
			modify_type=ANA_PARAM_SET_HI_ALARM;
			break;

		case	LL_LMT:
			value	= dms_cmm->var.anlg.lmt_para.LO_ALARM;
			modify_type=ANA_PARAM_SET_LO_ALARM;
			break;

		case	HRTN_LMT:
			value	= dms_cmm->var.anlg.lmt_para.HI_RECOVER;
			modify_type=ANA_PARAM_SET_HI_RECOVER;
			break;

		case	LRTN_LMT:
			value	= dms_cmm->var.anlg.lmt_para.LO_RECOVER;
			modify_type=ANA_PARAM_SET_LO_RECOVER;
			break;

		case	ENG_CVTER:
			value	= dms_cmm->var.anlg.eng_para.ENG_CVT;
			modify_type=ANA_PARAM_SET_ENG_CVT;
			break;

		case	HIRANGE:
			value	= dms_cmm->var.anlg.eng_para.HI_RANGE;
			modify_type=ANA_PARAM_SET_HI_RANGE;
			break;

		case	LORANGE:
			value	= dms_cmm->var.anlg.eng_para.LO_RANGE;
			modify_type=ANA_PARAM_SET_LO_RANGE;
			break;

		case	BASE:
			value	= dms_cmm->var.anlg.eng_para.ZERO;
			modify_type=ANA_PARAM_SET_ZERO;
			break;

		case	COMPENSATE:
			value	= dms_cmm->var.anlg.eng_para.COMPSATE;
			modify_type=ANA_PARAM_SET_COMPSATE;
			break;

		case	Tr_WARN:
			value	= dms_cmm->var.anlg.lmt_para.TR_WARN;
			modify_type=ANA_PARAM_SET_TR_WARN;
			break;

		case	Tr_RECOVER:
			value 	= dms_cmm->var.anlg.lmt_para.TR_RECOVER;
			modify_type=ANA_PARAM_SET_TR_RECOVER;
			break;

		default:
			return;
		}	

		enter_value_high_lmt[crt_n]	= 9999;
		enter_value_low_lmt[crt_n]	= -9999;
		
		cmd_buf[crt_n].type_id		= DPS_ANA_PARAM_SET;		
		
		cmd_buf[crt_n].status.opr	= modify_type;
		cmd_buf[crt_n].data.fValue	= value;
printf("cmd_buf.fValue=%.2f\n",	cmd_buf[crt_n].data.fValue);
		break;

	case	IMP_TYPE:
#ifdef DEBUG
		printf ( " in Modify_data_class____-IMP_TYPE: entry_data_class=%d\n", entry_data_class );
#endif		
                
		switch  ( entry_data_class)
		{
                    case    IMP_CONSTANT:
                        value =(float)( dms_cmm->var.imp.entry.eng_para.CONSTANT);
			modify_type = IMP_PARAM_SET_CONSTANT; 					
			break;
                        
                   case    ACCUM:
                        value = dms_cmm->var.imp.entry.d_accum;
			modify_type = IMP_PARAM_SET_D_ACCUM; 					
			break;
                             
		case	PEAK_DAY:
			value = dms_cmm->var.imp.peak.day;
			modify_type = IMP_PARAM_SET_PEAK_DAY; 					
			break;

		case	PEAK_MONTH:
			value = dms_cmm->var.imp.peak.month;
			modify_type = IMP_PARAM_SET_PEAK_MONTH;                         
			break;

		case	PEAK_YEAR:
			value = dms_cmm->var.imp.peak.year;
			modify_type = IMP_PARAM_SET_PEAK_YEAR; 					
			break;

		case	VALLEY_DAY:
			value = dms_cmm->var.imp.valley.day;
			modify_type = IMP_PARAM_SET_VALLEY_DAY; 					
			break;

		case	VALLEY_MONTH:
			value = dms_cmm->var.imp.valley.month;
			modify_type = IMP_PARAM_SET_VALLEY_MONTH; 					
			break;

		case	VALLEY_YEAR:
			value = dms_cmm->var.imp.valley.year;
			modify_type = IMP_PARAM_SET_VALLEY_YEAR;
			break;

		case	NORM_DAY:
			value = dms_cmm->var.imp.normal.day;
			modify_type = IMP_PARAM_SET_NORM_DAY; 					
			break;

		case	NORM_MONTH:
			value = dms_cmm->var.imp.normal.month;
			modify_type = IMP_PARAM_SET_NORM_MONTH; 					
			break;

		case	NORM_YEAR:
			value = dms_cmm->var.imp.normal.year;
			modify_type = IMP_PARAM_SET_NORM_YEAR;
			break;

		case	TOTAL_DAY:
			value = dms_cmm->var.imp.sum.day;
			modify_type = IMP_PARAM_SET_SUM_DAY; 					
			break;

		case	TOTAL_MONTH:
			value = dms_cmm->var.imp.sum.month;
			modify_type = IMP_PARAM_SET_SUM_MONTH; 					
			break;

		case	TOTAL_YEAR:
			value = dms_cmm->var.imp.sum.year;
			modify_type = IMP_PARAM_SET_SUM_YEAR;
			break;

		case	CVT_F:
			value = dms_cmm->var.imp.entry.eng_para.ENG_CVT;
			modify_type = IMP_PARAM_SET_CVT_F; 					
			break;

		case	PPEAK_DAY:
			value = dms_cmm->var.imp.ppeak.day;
			modify_type = IMP_PARAM_SET_PPEAK_DAY; 					
			break;

		case	PPEAK_MONTH:
			value = dms_cmm->var.imp.ppeak.month;
			modify_type = IMP_PARAM_SET_PPEAK_MONTH; 					
			break;

		case	PPEAK_YEAR:
			value = dms_cmm->var.imp.ppeak.year;
			modify_type = IMP_PARAM_SET_PPEAK_YEAR;
			break;

                case	DAY_SUM_INI:
			value =(float) (dms_cmm->var.imp.entry.init.day);
			modify_type = IMP_PARAM_SET_INIT_DAY;
			break;
                        
               case	MONTH_SUM_INI:
			value = (float)(dms_cmm->var.imp.entry.init.month);
			modify_type = IMP_PARAM_SET_INIT_MONTH;
			break;
                        
               case	YEAR_SUM_INI:
			value =(float) (dms_cmm->var.imp.entry.init.year);
			modify_type = IMP_PARAM_SET_INIT_YEAR;
			break;
                        
               case	PT_CVT:
			value =  dms_cmm->var.imp.entry.eng_para.PT;
			modify_type = IMP_PARAM_SET_PT;
			break;
                        
               case	CT_CVT:
			value =  dms_cmm->var.imp.entry.eng_para.CT;
			modify_type = IMP_PARAM_SET_CT;
			break;
		default:
			return;
		}
	
		enter_value_high_lmt[crt_n]    = IMP_HIGH_LMT;		
		enter_value_low_lmt[crt_n]     = IMP_LOW_LMT;		  
		
		cmd_buf[crt_n].type_id		= DPS_IMP_PARAM_SET;		
		cmd_buf[crt_n].status.opr	= modify_type;
                if(cmd_buf[crt_n].status.opr==IMP_PARAM_SET_CONSTANT) 
                    cmd_buf[crt_n].data.iValue		=(int) value;
                else
                    cmd_buf[crt_n].data.fValue		= value;		
		break;

	case	SOE_TYPE:
	case	POL_TYPE:
#ifdef DEBUG
		printf ( " in DPS_INT or POL: \n" );
#endif		
		switch  ( entry_data_class)
		{
		case	Open_Num:
			modify_type	= IND_PARAM_SET_NORM_OPEN_NUM;
			num = dms_cmm->var.ind.stc.norm_open;
			break;

		case	Close_Num:
			modify_type	= IND_PARAM_SET_NORM_CLOSE_NUM;
			num = dms_cmm->var.ind.stc.norm_close;
			break;

		case	SgOpnNum:
			modify_type	= IND_PARAM_SET_FAULT_OPEN_NUM;
			num = dms_cmm->var.ind.stc.fault_open;
			break;

		case	SgClsNum:
			modify_type	= IND_PARAM_SET_FAULT_CLOSE_NUM;
			num = dms_cmm->var.ind.stc.fault_close;
			break;

		default:
			break;
		}

		cmd_buf[crt_n].type_id		= DPS_C_ONOFF_SET;
		cmd_buf[crt_n].status.opr   = modify_type;
		cmd_buf[crt_n].data.iValue	= num; 
		break;

	default:
		printf("data_type not correct\n");
		return;
		break;
	}
		DbSelectTypeInit( crt_n );               
		PopupValueEntryBox (  parent  , crt_n, entry_ptr,entry_data_class);
}

void	ProgModifyParameter (int crt_n, GBRCD   *rcd )
{
	DMS_COMMON	dms_cmm;
	char		info[50];
	if ( ReadEntryById ( &pnt_tab_ptr[crt_n], &dms_cmm ) == -1)
	{
		printf ( "OIX: This point is not Found in the database file !!!\n" );
		
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	DbSelectTypeInit( crt_n );	
	ModifyDataClass( crt_n,main_w[crt_n], &dms_cmm, &pnt_tab_ptr[crt_n], rcd->lnk.dyn.data_class);
}


void	ProgDevOnOffLine (int crt_n, GBRCD   *rcd )
{
	DMS_COMMON	dms_cmm;
	
#ifdef	DEBUG
	printf ( "OIX: in ProgOnLine ::: \n" );
#endif	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Online" ) ==0 ||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Offline" ) ==0)
		ManageOnOffCommand ( crt_n, &dms_cmm, rcd );		
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
}	

void	ProgDevModeSet (int crt_n, GBRCD   *rcd )
{
	DMS_COMMON	dms_cmm;
	
#ifdef	DEBUG
	printf ( "OIX: in ProgDevModeSet ::: \n" );
#endif	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Off" ) ==0 ||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "On" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "On2" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "On3" ) ==0)
		ManageDevModeCommand ( crt_n, &dms_cmm, rcd );		
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
}	

void	ProgSetValue (int crt_n, GtkWidget *parent,DMS_COMMON	dms_cmm)
{
	int		tmp_limit;

#ifdef	DEBUG
	printf ( "OIX : in ProgSetValue \n" );
#endif

	pnt_tab_ptr[crt_n]	= dms_cmm.point;

	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id	= dms_cmm.point.stn_id;
	cmd_buf[crt_n].dev_id	= dms_cmm.point.dev_id;
	cmd_buf[crt_n].point_id	= dms_cmm.point.pt_id;  
	cmd_buf[crt_n].data_type= dms_cmm.point.data_type; 
	cmd_buf[crt_n].ctrl_addr= 0;
	switch( dms_cmm.point.data_type )
	{
	case	ANA_TYPE:
	case	PARAM_TYPE:
		enter_value_high_lmt[crt_n]	=  dms_cmm.var.anlg.lmt_para.HI_ALARM ;	
		enter_value_low_lmt[crt_n] 	=  dms_cmm.var.anlg.lmt_para.LO_ALARM ;	
		if ( enter_value_high_lmt[crt_n] == enter_value_low_lmt[crt_n] )
		{
			enter_value_high_lmt[crt_n] = ANA_HIGH_LMT;
			enter_value_low_lmt[crt_n]  = ANA_LOW_LMT;
		} 
		if ( enter_value_high_lmt[crt_n] < enter_value_low_lmt[crt_n] )
		{
			tmp_limit =(int) (enter_value_high_lmt[crt_n]);
			enter_value_high_lmt[crt_n] = enter_value_low_lmt[crt_n];
			enter_value_low_lmt[crt_n]  =(float) tmp_limit;
		}		
		
		cmd_buf[crt_n].type_id	= DPS_MAN_VALUE_SET;
		cmd_buf[crt_n].status.opr =0;/* Real_value*/			
		cmd_buf[crt_n].data.fValue	=dms_cmm.var.anlg.fValue;
		break;		

	default:
		break;
	}

	DbSelectTypeInit( crt_n );
	PopupValueEntryBox (parent, crt_n ,&dms_cmm.point,-1);  
}


void	ProgDevCtrl (int crt_n, GBRCD	*rcd )
{	

	DMS_COMMON      dms_cmm;
	short           sbo_type;
        short           lcu_sbo_result[MAX_CRT_NUMBER];
#ifdef	DEBUG
	printf ( "OIX: in ProgDevCtrl::::::\n" );
#endif
	printf("####rcd->lnk.dbname=%s\n",rcd->lnk.dbname);
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{		
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	if(dms_cmm.point.data_type!=SOE_TYPE && dms_cmm.point.data_type!=POL_TYPE
		&& dms_cmm.point.data_type!=SYSMSG_TYPE &&dms_cmm.point.data_type!=OUTS_TYPE
                &&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
	pnt_tab_ptr[crt_n]      = dms_cmm.point;
	
	sbo_type = dms_cmm.fixed.ioattr.ctrl;
	
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Off" ) == 0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "On" ) == 0||
                strcmp ( rcd->lnk.act.fmt_def.prog.status, "Reset" ) == 0)
	{ 
		if ( sbo_type == SBO_YES)
		{					
			sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_DEVICE",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
			                                                  
                        sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] ); 
                      
                        if(dms_cmm.point.data_type==DEVOBJ_TYPE)
                        {
                            if(strcmp(rcd->lnk.act.fmt_def.prog.status,"Off")==0)
                                lcu_sbo_result[crt_n]= dms_cmm.var.devobj.fb_status1.condc0;
                            else if(strcmp(rcd->lnk.act.fmt_def.prog.status,"On")==0)
                                lcu_sbo_result[crt_n]= dms_cmm.var.devobj.fb_status1.condc1;  
							else if(strcmp(rcd->lnk.act.fmt_def.prog.status,"Reset")==0)
                                lcu_sbo_result[crt_n]=  1;  



                            if ( sbo_result[crt_n] != 0&&lcu_sbo_result[crt_n]!=0)
                                ManageDevCommand ( crt_n, &dms_cmm, rcd );
                            else
                            {
                                if(sbo_result[crt_n]==0)
                                    ManageSubDspWnd ( crt_n, &dms_cmm);
                                else if(lcu_sbo_result[crt_n]==0)
                                    PopupMessageBox ( main_w[crt_n], MSG_CONDTN_UNSATISF );
                            }
                        }
                        else
                        {
                            if ( sbo_result[crt_n] != 0)
				ManageDevCommand ( crt_n, &dms_cmm, rcd );
                            else
                            {                                                                       
                                ManageSubDspWnd ( crt_n, &dms_cmm);
				return;
                            }
                        }
		}
		else
		{
			PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
			return;
		}	
	}
	else	
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
}

void	ProgSwchCtrl (int crt_n,GBRCD	* rcd )
{	
	short		cur_status;
	DMS_COMMON      dms_cmm;
	short           sbo_type;
	short           lcu_sbo_result[MAX_CRT_NUMBER];
#ifdef	DEBUG
	printf ( "OIX:  in ProgOpenSwchCtrl::::::\n" );
#endif
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm ) == -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	if(dms_cmm.point.data_type!=SOE_TYPE && dms_cmm.point.data_type!=POL_TYPE
		&& dms_cmm.point.data_type!=SYSMSG_TYPE &&dms_cmm.point.data_type!=OUTS_TYPE
                &&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
	pnt_tab_ptr[crt_n]      = dms_cmm.point;
	
	sbo_type = dms_cmm.fixed.ioattr.ctrl;
		
	cur_status = dms_cmm.var.ind.status.state;
      
       
		if ( sbo_type == SBO_YES)
		{
				sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_SWITCH",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
								
                                sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );
                                
                              
                                if(dms_cmm.point.data_type==DEVOBJ_TYPE)
                                {
                                    if(strcmp(rcd->lnk.act.fmt_def.prog.status,"Open")==0 || strcmp(rcd->lnk.act.fmt_def.prog.status,"TestOpen")==0)
                                        lcu_sbo_result[crt_n]= dms_cmm.var.devobj.fb_status1.condc0; 
                                    else if(strcmp(rcd->lnk.act.fmt_def.prog.status,"Close")==0 || strcmp(rcd->lnk.act.fmt_def.prog.status,"TestClose")==0 )
                                        lcu_sbo_result[crt_n]=  dms_cmm.var.devobj.fb_status1.condc1; 
									else /* if(strcmp(rcd->lnk.act.fmt_def.prog.status,"Reset")==0 )*/
                                        lcu_sbo_result[crt_n]=   1;



                                    if ( sbo_result[crt_n] != 0&&lcu_sbo_result[crt_n]!=0)
                                            ManageSwitchCommand ( crt_n, &dms_cmm, rcd );
                                    else
                                    {
                                            if(sbo_result[crt_n]==0)
                                                ManageSubDspWnd ( crt_n, &dms_cmm);
                                            else if(lcu_sbo_result[crt_n]==0)
                                                    PopupMessageBox ( main_w[crt_n], MSG_CONDTN_UNSATISF );
                                    }
                                }
                                else
                                {
                                    if ( sbo_result[crt_n] !=0)
					ManageSwitchCommand ( crt_n, &dms_cmm, rcd );
                                    else
                                    {                                        
                                            ManageSubDspWnd ( crt_n, &dms_cmm);                                        
                                            return;
                                    }
                                }
                         }
		
		else
		{
				PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
				return;
		}		
	
					
}

void	ProgBrkCtrl (int crt_n, GBRCD	*rcd )
{
	short		cur_status;
	DMS_COMMON      dms_cmm;
	short           sbo_type;
	short           lcu_sbo_result[MAX_CRT_NUMBER];
        
#ifdef	DEBUG
	printf ( "OIX: in ProgBrkCtrl::::::\n" );
#endif
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm ) == -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	
	pnt_tab_ptr[crt_n]      = dms_cmm.point;
	
	sbo_type = dms_cmm.fixed.ioattr.ctrl;
	
	cur_status = dms_cmm.var.ind.status.state;

	if(dms_cmm.point.data_type!= SOE_TYPE && dms_cmm.point.data_type !=POL_TYPE
		&& dms_cmm.point.data_type !=SYSMSG_TYPE&&dms_cmm.point.data_type!=OUTS_TYPE
                &&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR );
		return;
	}		
			
            if ( sbo_type == SBO_YES)
            {							
                    sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_BREAKER",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/

                    sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );

                    if(dms_cmm.point.data_type==DEVOBJ_TYPE) 
                    {
                        if(strcmp(rcd->lnk.act.fmt_def.prog.status,"Open")==0  || strcmp(rcd->lnk.act.fmt_def.prog.status,"TestOpen")==0)
                            lcu_sbo_result[crt_n]= sbo_result[crt_n] & dms_cmm.var.devobj.fb_status1.condc0;  
                        else if(strcmp(rcd->lnk.act.fmt_def.prog.status, "Close")==0 || strcmp(rcd->lnk.act.fmt_def.prog.status,"IIM_Tran")==0 
										      || strcmp(rcd->lnk.act.fmt_def.prog.status,"TestClose")==0 || strcmp(rcd->lnk.act.fmt_def.prog.status,"IM_Line")==0 
											  || strcmp(rcd->lnk.act.fmt_def.prog.status,"IIM_Line")==0 || strcmp(rcd->lnk.act.fmt_def.prog.status,"Line_IM")==0 
											  || strcmp(rcd->lnk.act.fmt_def.prog.status,"Line_IIM")==0 || strcmp(rcd->lnk.act.fmt_def.prog.status,"Tran_IM")==0  
											  || strcmp(rcd->lnk.act.fmt_def.prog.status,"Tran_IIM")==0 || strcmp(rcd->lnk.act.fmt_def.prog.status,"IM_Tran")==0  
											  || strcmp(rcd->lnk.act.fmt_def.prog.status,"LoopClose")==0 || strcmp(rcd->lnk.act.fmt_def.prog.status,"ChgVltCls")==0  
											  || strcmp(rcd->lnk.act.fmt_def.prog.status,"ZroVltCls")==0)
                            lcu_sbo_result[crt_n]= sbo_result[crt_n] & dms_cmm.var.devobj.fb_status1.condc1; 
						else /* if  (strcmp(rcd->lnk.act.fmt_def.prog.status, "Reset")==0)   */ 
							lcu_sbo_result[crt_n]  =   1 ;


                        if ( sbo_result[crt_n] != 0&&lcu_sbo_result[crt_n]!=0)
                            ManageBrkCommand ( crt_n, &dms_cmm, rcd );
                        else
                        {
                            if(sbo_result[crt_n]==0)
                                ManageSubDspWnd ( crt_n, &dms_cmm);
                            else if(lcu_sbo_result[crt_n]==0)
                                PopupMessageBox ( main_w[crt_n], MSG_CONDTN_UNSATISF );
                        }
                    }
                    else
                    {
                        if ( sbo_result[crt_n] !=0 )
                                ManageBrkCommand ( crt_n, &dms_cmm, rcd );
                        else
                        {                            
                                ManageSubDspWnd ( crt_n, &dms_cmm);                               
                                return;
                        }
                    }
            }
            else
            {
                    PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
                    return;
            }				
	
}

void	ProgHostCtrl (int crt_n, GBRCD	*rcd )
{	
	DMS_COMMON      dms_cmm;
	/*short	cur_status;*/

#ifdef	DEBUG
	printf ( "OIX: in ProgHostCtrl::::::\n" );
#endif	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}

		ManageHostSwitchCommand ( crt_n, &dms_cmm, rcd );
}

void	ProgSysModeSet (int crt_n,GBRCD	*rcd )
{
	DMS_COMMON      dms_cmm;
	short		sbo_type;

#ifdef	DEBUG
	printf ( "OIX: in ProgSysMsgSet::::::\n" );
#endif	
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}

	    sbo_type = dms_cmm.fixed.ioattr.ctrl;
        sbo_type = SBO_YES ;
        if ( sbo_type == SBO_YES)
        {              
                sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_SYS_MODE_SET",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
		              
               sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );
               if ( sbo_result[crt_n] !=0 )
		       	ManageSysModeSetCommand ( crt_n, &dms_cmm, rcd );
               else
               {
                       ManageSubDspWnd ( crt_n, &dms_cmm );                       
                       return;
               }
         }
         else
        {
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;
	}
}

void	ProgOpValueSet (int crt_n, GBRCD   *rcd )
{	
	DMS_COMMON      dms_cmm;
	float	tmp_limit;
    /*    short		sbo_type;*/
#ifdef	DEBUG
	printf ( "OIX: in ProgOpValueSet::::::\n" );
#endif
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}

	if ( dms_cmm.point.data_type != ANA_TYPE &&dms_cmm.point.data_type != PARAM_TYPE 
		&&dms_cmm.point.data_type !=CURVE_TYPE &&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{		
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
    /*    sbo_type = dms_cmm.fixed.ioattr.ctrl;
        if ( sbo_type != SBO_YES)
        {
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;
	}
	rm by chi  */
        sprintf(sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_OP_VALUE_SET",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
        sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );
        if ( sbo_result[crt_n] == 0)
	{	       	
                       ManageSubDspWnd ( crt_n, &dms_cmm );
                       return;
        }
        
	pnt_tab_ptr[crt_n]      = dms_cmm.point;

	enter_value_high_lmt[crt_n]	=  dms_cmm.var.anlg.lmt_para.HI_ALARM ;	
	enter_value_low_lmt[crt_n] 	=  dms_cmm.var.anlg.lmt_para.LO_ALARM ;	
	if ( enter_value_high_lmt[crt_n] == enter_value_low_lmt[crt_n] )
	{
		enter_value_high_lmt[crt_n] = ANA_HIGH_LMT;
		enter_value_low_lmt[crt_n]  = ANA_LOW_LMT;
	} 
	if ( enter_value_high_lmt[crt_n] < enter_value_low_lmt[crt_n] )
	{
		tmp_limit = enter_value_high_lmt[crt_n];
		enter_value_high_lmt[crt_n] = enter_value_low_lmt[crt_n];
		enter_value_low_lmt[crt_n]  = tmp_limit;
	}	
	
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id	= dms_cmm.point.stn_id;
	cmd_buf[crt_n].dev_id	= dms_cmm.point.dev_id;
	cmd_buf[crt_n].point_id	= dms_cmm.point.pt_id;  
	cmd_buf[crt_n].data_type= dms_cmm.point.data_type; 
	cmd_buf[crt_n].type_id	= C_OP_VALUE_SET;	
	cmd_buf[crt_n].data.fValue	=dms_cmm.var.anlg.fValue;

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "RealValue" ) ==0)	
		cmd_buf[crt_n].status.opr = REAL_VALUE;	
	else if( strcmp ( rcd->lnk.act.fmt_def.prog.status, "ParamValue" ) ==0)
		cmd_buf[crt_n].status.opr = PARAM_VALUE;

	switch( dms_cmm.point.data_type )
	{
	case	ANA_TYPE:
	case	PARAM_TYPE:		
		cmd_buf[crt_n].ctrl_addr= 0;
		break;	

	case 	CURVE_TYPE:
		/*cmd_buf[crt_n].ctrl_addr = rcd->lnk.act.fmt_def.curve.seg_num;*/
		break;	

	default:
		break;
	}

	DbSelectTypeInit( crt_n );
        pnt_tab_ptr[crt_n]	= dms_cmm.point;
 	PopupValueEntryBox (main_w[crt_n], crt_n ,&dms_cmm.point ,-1); 
}
    
void	ProgSignRtn(int crt_n, GBRCD   *rcd )
{	
	DMS_COMMON      dms_cmm;	
	char  LongNamerStr1[128];

#ifdef	DEBUG
	printf ( "OIX: in ProgSignRtn::::::\n" );
#endif
    rectmp[crt_n]   = *rcd;  //added by chichi 080107
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
        }
	
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "FaultReset" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "FailReset" ) == 0  )
	{
		FormCSignRtnPacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
		
                /* strcpy ( cmd_str, dms_cmm.fixed.entryLongname );  */
                if(-1!=GetPtCmdMsgStrById(&dms_cmm.point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
				{
					GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
					sprintf(cmd_str,"%s:\n\t %s\0",LongNamerStr1 ,string );
                }
				ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
	}		
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
}

void	ProgNetCtrl (int crt_n, GBRCD   *rcd )
{	
	DMS_COMMON      dms_cmm;

#ifdef	DEBUG
	printf ( "OIX: in ProgNetCtrl::::::\n" );
#endif	
        if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
                printf ( "OIX :in ProgNetCtrl %s is not found in database.\n", rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], MSG_NET_NOT_EXIST );               
        }
	else	
  		ManageNetSwitchCommand ( crt_n, &dms_cmm, rcd );
}
 
/***** following is added by zyp for v4.0 ****/

void ProgIoCtrlBlock(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	char  LongNamerStr1[128];
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
                printf ( "OIX :in ProgIOCtrlBlock %s is not found in database.\n", rcd->lnk.dbname );
		return;              
        }
	rectmp[crt_n] = *rcd;
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "UnBlock" ) ==0 ||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Block" ) ==0)
            {
		FormIoCtlrBlockPacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
                
                /*strcpy ( cmd_str, dms_cmm.fixed.entryLongname );  */
                if(-1!=GetPtCmdMsgStrById(&dms_cmm.point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
				{
					GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
					sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1 ,string );
				}
                ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
        }
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );
}

void ProgIoGroupCtrlBlock(int crt_n,DMS_COMMON dms_cmm)
{
	DMS_COMMON      tmp_dms_cmm;
        POINTER         tmp_point;
        int     io_group,i,entry_num,send_cnt = 0;
	
		io_group = dms_cmm.fixed.io_group;
        GetEntryNumById(dms_cmm.point.stn_id,dms_cmm.point.dev_id,dms_cmm.point.data_type,&entry_num);
        tmp_point = dms_cmm.point;
        if (PopupMessageBoxRet(main_w[crt_n],MSG_OPER_CLASS_UNBLOCK)== GTK_RESPONSE_CANCEL ) 
			return ;

        for(i=0;i<entry_num;i++)
        {

                tmp_point.pt_id = i;
                if(ReadEntryById(&tmp_point,&tmp_dms_cmm)!=-1)
                {
                        if( tmp_dms_cmm.fixed.io_group==io_group &&tmp_dms_cmm.var.anlg.value.status.ctrl_block==LOCK_OFF) 
                        {
                            CmdBufInit ( crt_n );
                            cmd_buf[crt_n].stn_id	= tmp_dms_cmm.point.stn_id;
                            cmd_buf[crt_n].dev_id	= tmp_dms_cmm.point.dev_id;
                            cmd_buf[crt_n].point_id	= tmp_dms_cmm.point.pt_id;
                            cmd_buf[crt_n].data_type= tmp_dms_cmm.point.data_type;
                            cmd_buf[crt_n].type_id	= C_IO_CTRL_BLOCK;  
                    
                            cmd_buf[crt_n].status.opr  	= LOCK_ON ;
                            cmd_buf[crt_n].ctrl_addr	=	tmp_dms_cmm.point.pt_id;
printf ("######### OIX:: send ProgIoGroupCtrlBlock Num(%d/%d)   \n"  ,i ,entry_num  ) ;
                            ProgDoAckCel ( crt_n ); 
                            MySleep(100) ;
                            send_cnt++;
                            if ( send_cnt > MAX_NETCMD_SEG_NUM )
                            {
                                MySleep ( 300 );
                                send_cnt = 0;
                            }
                        }
                }
        }              
}

void ProgIostSet(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
                printf ( "OIX :in ProgNetCtrl %s is not found in database.\n", rcd->lnk.dbname );
		return;              
        }

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Scan_state_Enable" ) ==0 ||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Scan_state_Disable" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Alm_state_Enable" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Alm_state_Disable" ) ==0)
	{
		FormIostSetPacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
		
		ProgDoAckCelBrdcst ( crt_n );
	}
	else if(strcmp ( rcd->lnk.act.fmt_def.prog.status, "Alm_ack_state" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Tag_state_Enable" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Tag_state_Disable" ) ==0)
	{
		FormIostSetPacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
		
		ProgDoAckCelBrdcst ( crt_n );
	}		
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );
}

void ProgIoattrSet(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	if (  strcmp(rcd->lnk.dbname,"")!=0 && (ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1))
        {
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
        }
	else
	{
		if(ReadEntryById ( &pnt_tab_ptr[crt_n], &dms_cmm )==-1)
		{
			char		info[50];
			strcpy(info,OIX_STR_DB_NO_POINT);
			strcat(info,rcd->lnk.dbname );
			PopupMessageBox ( main_w[crt_n], info);
			return;
		}
	}

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Statistic_Yes" ) ==0 ||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Statistic_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Failpnt_Yes" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Failpnt_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Faultpnt_Yes" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Faultpnt_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Trendalm_Yes" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Trendalm_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Keysignal_Yes" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Keysignal_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Audioalm_Yes" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Audioalm_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Cascadepnt_Yes" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Cascadepnt_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdbrec_Yes" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdbrec_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdbcurve_Yes" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdbcurve_No" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdb_period0" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdb_period1" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdb_period2" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdb_period3" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdb_period4" ) ==0||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Hdb_period5" ) ==0
)
	{
		FormIoattrSetPacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
		
		ProgDoAckCelBrdcst ( crt_n );
	}		
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );
}

void ProgManStatSet(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
        }
	if(dms_cmm.point.data_type !=SYSMSG_TYPE&&dms_cmm.point.data_type !=SOE_TYPE
		&&dms_cmm.point.data_type !=POL_TYPE&&dms_cmm.point.data_type!=OUTS_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Open" ) ==0 ||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Close" ) ==0 ||
		strcmp ( rcd->lnk.act.fmt_def.prog.status, "Transit" ) ==0)
	{
		FormManStatSetPacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
		ProgDoAckCelBrdcst ( crt_n ); 
	}				
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
}

void ProgManCurveSet(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
        }
	if(dms_cmm.point.data_type !=CURVE_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Real_Value" ) ==0 )
	{
                FormManCurveSetPacket ( crt_n, &dms_cmm, rcd/*->lnk.act.fmt_def.prog.status */);		
		DbSelectTypeInit( crt_n );
                pnt_tab_ptr[crt_n]	= dms_cmm.point;
	 	PopupValueEntryBox (main_w[crt_n], crt_n  ,&dms_cmm.point,-1);  
	}				
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
}

void ProgCTrans(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	int sbo_type;

	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
                char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
        }
        sbo_type = dms_cmm.fixed.ioattr.ctrl;
	if ( sbo_type == SBO_YES)
	{		
		sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_TRANS",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
		sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );  
		if ( sbo_result[crt_n] !=0 )
			ManageCTransCommand ( crt_n, &dms_cmm, rcd );		
		else
		{
			ManageSubDspWnd ( crt_n, &dms_cmm);
		    	PopupMessageBox ( main_w[crt_n], MSG_GATE_CONDTN_UNSATISF );
			return;
		}
	}
	else
	{
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;
	}
}

void	ProgPlcSwitch (int crt_n, GBRCD	*rcd )
{	
	DMS_COMMON      dms_cmm;
	char  LongNamerStr1[128];
	
#ifdef	DEBUG
	printf ( "OIX: in ProgPlcSwitch::::::\n" );
#endif	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
                char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
	FormPlcSwitchPacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n] = *rcd;
		
       /* strcpy ( cmd_str, dms_cmm.fixed.entryLongname );  */
        if(-1!=GetPtCmdMsgStrById(&dms_cmm.point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{ 
			GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
			sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1   ,string );
		}
	ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
}

void ProgDataPolling(int crt_n, GBRCD	*rcd )
{
	DMS_COMMON      dms_cmm;
        rectmp[crt_n] = *rcd;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
                char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
        }
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id	= dms_cmm.point.stn_id;
	cmd_buf[crt_n].dev_id	= dms_cmm.point.dev_id;
	cmd_buf[crt_n].point_id	= dms_cmm.point.pt_id;  
	cmd_buf[crt_n].data_type= dms_cmm.point.data_type; 
	cmd_buf[crt_n].type_id	= C_DATA_POLLING;	
	cmd_buf[crt_n].data.iValue	=0;
	cmd_buf[crt_n].ctrl_addr	=0;

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "DataPolling" ) ==0)	
		cmd_buf[crt_n].status.opr = DATA_POLLING;	
	
        ProgDoAckCelBrdcst ( crt_n );
}

void ProgCLcuParam(int crt_n, GBRCD	*rcd )
{
	DMS_COMMON      dms_cmm;
	char  LongNamerStr1[128];
        rectmp[crt_n] = *rcd;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
                char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
        }
	if(dms_cmm.point.data_type !=ANA_TYPE&&dms_cmm.point.data_type !=PARAM_TYPE&&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id	= dms_cmm.point.stn_id;
	cmd_buf[crt_n].dev_id	= dms_cmm.point.dev_id;
	cmd_buf[crt_n].point_id	= dms_cmm.point.pt_id;  
	cmd_buf[crt_n].data_type= dms_cmm.point.data_type; 
	cmd_buf[crt_n].type_id	= C_LCU_PARAM;	
	cmd_buf[crt_n].data.iValue	=0;
	cmd_buf[crt_n].ctrl_addr	=0;

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "ParamPolling" ) ==0)	
	{	
		cmd_buf[crt_n].status.opr = PARAM_POLLING;
	}
	else if(strcmp ( rcd->lnk.act.fmt_def.prog.status, "ParamDownLoad" ) ==0)
		cmd_buf[crt_n].status.opr = PARAM_DOWNLOAD;
	
       /* strcpy ( cmd_str, dms_cmm.fixed.entryLongname );  */
        if(-1!=GetPtCmdMsgStrById(&dms_cmm.point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
			GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
			sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1   ,string );
		}
	ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
}


void ProgTimeSync(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	char  LongNamerStr1[128];
        rectmp[crt_n] = *rcd;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm )== -1)
        {
                char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;              
        }
	if(dms_cmm.point.data_type !=SYSMSG_TYPE&&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "TimeSync" ) ==0 )
	{
		FormCTimeSyncPacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
		
                /* strcpy ( cmd_str, dms_cmm.fixed.entryLongname );  */
                if(-1!=GetPtCmdMsgStrById(&dms_cmm.point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
                {
					GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
					sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1,string );
				}
                ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
	}		
	else
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
}

void ProgCOnoffForce(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	short		sbo_type;
	
#ifdef	DEBUG
	printf ( "OIX: in ProgCOnoffForce::::::\n" );
#endif	
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
        char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}

	sbo_type = dms_cmm.fixed.ioattr.ctrl;

	if(dms_cmm.point.data_type!=OUTS_TYPE&&dms_cmm.point.data_type!=DEVOBJ_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
 
        if ( sbo_type == SBO_YES)
        {               
                sprintf( sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,"C_ONOFF_FORCE",rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
		
                sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );
               if ( sbo_result[crt_n] !=0)
		       ManageCOnoffForceCommand ( crt_n, &dms_cmm, rcd );
               else
               {
                       ManageSubDspWnd ( crt_n, &dms_cmm );
                       PopupMessageBox ( main_w[crt_n], MSG_SYSMSG_SETCONDTN_UNSATISF );
                       return;
               }
         }
        else
        {
		PopupMessageBox(main_w[crt_n],MSG_NOT_CTRL);
		return;
	}
}

void ProgCAnaForce(int crt_n,GBRCD   *rcd)
{
	DMS_COMMON      dms_cmm;
	short		sbo_type;
	float		tmp_limit;
    char		LongName[100];
#ifdef	DEBUG
	printf ( "OIX: in ProgCOnoffForce::::::\n" );
#endif	
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
                char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}
    GetPtLongnameByNameStr ( rcd->lnk.dbname, LongName );
	sbo_type = dms_cmm.fixed.ioattr.ctrl;

	if(dms_cmm.point.data_type!=OUTA_TYPE&&dms_cmm.point.data_type!=DEVOBJ_TYPE&&dms_cmm.point.data_type!=ANA_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
 	enter_value_high_lmt[crt_n]	=  dms_cmm.var.anlg.lmt_para.HI_ALARM ;	
	enter_value_low_lmt[crt_n] 	=  dms_cmm.var.anlg.lmt_para.LO_ALARM ;	
	if ( enter_value_high_lmt[crt_n] == enter_value_low_lmt[crt_n] )
	{
		enter_value_high_lmt[crt_n] = ANA_HIGH_LMT;
		enter_value_low_lmt[crt_n]  = ANA_LOW_LMT;
	} 
	if ( enter_value_high_lmt[crt_n] < enter_value_low_lmt[crt_n] )
	{
		tmp_limit = enter_value_high_lmt[crt_n];
		enter_value_high_lmt[crt_n] = enter_value_low_lmt[crt_n];
		enter_value_low_lmt[crt_n]  = tmp_limit;
	}	
        if ( sbo_type == SBO_YES)
        {
               sprintf ( sbo_pnt_name[crt_n],"%s.%s.%s", rcd->lnk.dbname,"C_ANA_FORCE",rcd->lnk.act.fmt_def.prog.status );
               sbo_result[crt_n] = InterLock_Judge( sbo_pnt_name[crt_n] );
               if ( sbo_result[crt_n] !=0)
				{
				    if (PopupMessageBoxRet ( main_w[crt_n], strcat(LongName ,_CS_(MSG_FORCE_INFO) ) )==GTK_RESPONSE_CANCEL)
						return ;
				   FormCAnaForcePacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
				   DbSelectTypeInit( crt_n );
				   pnt_tab_ptr[crt_n]	= dms_cmm.point;                        
				   ProgDoAckCel ( crt_n );	
				}
               else
               {
                   ManageSubDspWnd ( crt_n, &dms_cmm );
                   PopupMessageBox ( main_w[crt_n], MSG_SYSMSG_SETCONDTN_UNSATISF );
                   return;
               }
         }
         else
		 {
			if (PopupMessageBoxRet ( main_w[crt_n], strcat(LongName ,_CS_(MSG_FORCE_INFO)) )==GTK_RESPONSE_CANCEL)
						return ;
  			FormCAnaForcePacket ( crt_n, &dms_cmm, rcd->lnk.act.fmt_def.prog.status );
			DbSelectTypeInit( crt_n );
            pnt_tab_ptr[crt_n]	= dms_cmm.point;                
            ProgDoAckCel ( crt_n );	
		 }	
}

void ProgCurveInput(int crt_n,GBRCD  *rcd)
{
    DMS_COMMON      dms_cmm;
	char		info[50];
    if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{ 
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,rcd->lnk.dbname );
		PopupMessageBox ( main_w[crt_n], info);
		return;
	}	

	if(dms_cmm.point.data_type!=CURVE_TYPE)
	{
		PopupMessageBox ( main_w[crt_n], MSG_DATA_TYPE_ERROR);
		return;
	}
    PopupCurveInputBox(main_w[crt_n], crt_n ,dms_cmm.point);
}
void ProgManSetTimeInput(int crt_n,GBRCD  *rcd)
{
	


	PopManSetTimeInputDlg(main_w[crt_n], crt_n,rcd);


}

void ManageCOnoffForceCommand (int crt_n, DMS_COMMON	*int_data, GBRCD   *rcd )
{
	char  LongNamerStr1[128];
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif
	int dev_num;
	FormCOnOffForcePacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n] = *rcd;	

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;

	if ( int_data->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}	
        
       /* if(strcmp ( rcd->lnk.act.fmt_def.prog.status, "UnForce" )!=0)
            PopTimeInputDlg(main_w[crt_n],crt_n);*/	
	
	/*  strcpy ( cmd_str, int_data->fixed.entryLongname );   */
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1  ,string );
		}
	ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
        
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif
}


void	ManageSubDspWnd (int crt_n , DMS_COMMON	*dms_cmm)
{
	int     max_num=0;
	int     j, g_end,numb;
	char    tmpstr[100];
        
	oper_cond_ptr[crt_n] = dms_cmm->point;
	printf("interlock_pt_num=%d\n",interlock_pt_num);
	
	for(numb=0;numb<interlock_pt_num;numb++)
	{	            
		if (strcmp(sbo_pnt_name[crt_n],interlock[numb].o_name)==0)
			break;
	}
	
	if (numb>=interlock_pt_num)
	{
		sprintf (tmpstr,"%s:%s",MSG_SBO_POINT_NOT_EXIST ,sbo_pnt_name[crt_n]);
		PopupMessageBox ( main_w[crt_n],tmpstr  );
		return ;
	}

	g_end=interlock[numb].offset+interlock[numb].g_num;
	for(j=interlock[numb].offset;j<=g_end;j++)
		max_num += group[j].e_num ; //max_num= MAX(max_num, group[j].e_num);
		
	sub_dsp_box_width = (interlock[numb].g_num+1)*250 + 60;
	sub_dsp_box_height= max_num*20 + 50;
	
	if( sub_dsp_box_width < 400 )
		sub_dsp_box_width = 400;
        if(sub_dsp_box_width>600)
            sub_dsp_box_width=600;
	if( sub_dsp_box_height < 150 )
		sub_dsp_box_height = 150;

	PopupOperCondWnd ( crt_n);	

	SubRedraw( sub_dsp_canvas[crt_n] ); 	
}

void	ManageApowerCommand (int crt_n,DMS_COMMON	*dms_cmm, GBRCD	*rcd )
{	
	char  LongNamerStr1[128];
	int dev_num;
	char tmp_str[50];
	FormApowerPacket ( crt_n, dms_cmm, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n]           = *rcd;
	
	if(-1==GetGroupNumById(dms_cmm->point.stn_id, &dev_num))
		return ;

	if ( dms_cmm->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}
	GetEntryLongnameById (&dms_cmm->point,LongNamerStr1 );
	strcpy ( string, LongNamerStr1 );

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Power" ) == 0 )
		strcat ( string, _CS_(OIX_STR_APOWER_POWER) );
	else if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Opening" ) ==0 )
		strcat ( string, _CS_(OIX_STR_APOWER_OPENNING) );
	else if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Flux" ) ==0 )
		strcat ( string, _CS_(OIX_STR_APOWER_FLUX ));
	else
	{
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
		return;
	}
	enter_value_high_lmt[crt_n] = dms_cmm->var.anlg.lmt_para.HI_ALARM ;
	enter_value_low_lmt[crt_n] =  dms_cmm->var.anlg.lmt_para.LO_ALARM  ;
	
	cmd_buf[crt_n].data.fValue     = dms_cmm->var.anlg.fValue; 
	
	strcpy(menu_title_str,string);
        strcpy(cmd_str,string);
	if(-1!=GetPtCmdMsgStrById(&dms_cmm->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string)) 
	{
		GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
		sprintf(tmp_str,"\t%s  ÃüÁî×´Ì¬=%d \0"  ,cmd_type_str,cmd_buf[crt_n].status.opr);
		strcat(cmd_str,tmp_str);
	}

	DbSelectTypeInit( crt_n);
        PopupValueEntryBox ( main_w[crt_n],crt_n , &dms_cmm->point,-1);  	
}

void	ManageRpowerCommand (int crt_n,DMS_COMMON	*dms_cmm, GBRCD	*rcd )
{	
	char  LongNamerStr1[128];
	int dev_num;
	char tmp_str[50];
	FormRpowerPacket ( crt_n, dms_cmm, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n]           = *rcd;

	if(-1==GetGroupNumById(dms_cmm->point.stn_id, &dev_num))
		return ;

	if ( dms_cmm->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}
	GetEntryLongnameById (&dms_cmm->point,LongNamerStr1 );
	strcpy ( string, LongNamerStr1 );

	if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Power" ) == 0 )
		strcat ( string, _CS_(OIX_STR_RPOWER_POWER) );
	else if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Volt" ) ==0 )
		strcat ( string, _CS_(OIX_STR_RPOWER_VOLT) );
	else if ( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Cos" ) ==0 )/* added on 2008.4.8*/
		strcat ( string,_CS_( OIX_STR_RPOWER_COS) );
	else
	{
		PopupMessageBox ( main_w[crt_n], MSG_OPER_STATE_ERR );	
		return;
	}
	enter_value_high_lmt[crt_n] = dms_cmm->var.anlg.lmt_para.HI_ALARM ;
	enter_value_low_lmt[crt_n] =  dms_cmm->var.anlg.lmt_para.LO_ALARM ;
	
	cmd_buf[crt_n].data.fValue     = dms_cmm->var.anlg.fValue; 
	
	strcpy(menu_title_str,string); 
	strcpy(cmd_str,string);
	if(-1!=GetPtCmdMsgStrById(&dms_cmm->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string)) 
	{
		GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
		sprintf(tmp_str,"\t%s  ÃüÁî×´Ì¬=%d \0"  ,cmd_type_str,cmd_buf[crt_n].status.opr);
        strcat(cmd_str,tmp_str);
	}
	DbSelectTypeInit( crt_n);
	PopupValueEntryBox ( main_w[crt_n],crt_n,&dms_cmm->point,-1);  	
}

void	ManageGenCommand (int crt_n ,DMS_COMMON  *int_data,GBRCD	*rcd ) 
{
	int dev_num;
	char  LongNamerStr1[128];
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif
	FormCUnitPacket ( crt_n, int_data ,rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n] 	= *rcd;

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;

	if ( int_data->point.dev_id > dev_num  )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}
	
		if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))   
		{
			GetGroupCommentById(int_data->point.stn_id, int_data->point.dev_id, LongNamerStr1);
			strcpy(menu_title_str,LongNamerStr1);	
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
		}
		
        if(cmd_buf[crt_n].status.opr == JUMP_UNIT)
            PopupValueEntryBox ( main_w[crt_n],crt_n,&int_data->point,-1); 
        else
	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );	
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif
}

void ManageCTransCommand(int crt_n,DMS_COMMON	*int_data, GBRCD	*rcd)
{	
	char  LongNamerStr1[128];
	int dev_num;
	char tmp_str[50];
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif	
	FormCTransPacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n]           = *rcd;

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;

	if ( int_data->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}
	
	if(	strcmp (rcd->lnk.act.fmt_def.prog.status,"RaiseTo")==0
		||strcmp ( rcd->lnk.act.fmt_def.prog.status, "DownTo" ) ==0)
	{
		enter_value_high_lmt[crt_n] =(float) (int_data->var.anlg.lmt_para.HI_ALARM *10);
		enter_value_low_lmt[crt_n] = (float) ( int_data->var.anlg.lmt_para.LO_ALARM  *0.1);
	
		cmd_buf[crt_n].data.fValue     = int_data->var.anlg.fValue; 

		PopupValueEntryBox ( main_w[crt_n], crt_n ,&(int_data->point),-1); 
		return;
	}

		//GetEntryLongnameById (&int_data->point,LongNamerStr1 );
        //strcpy ( cmd_str, LongNamerStr1);
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))   
		{
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
		}
	ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif
}

void	ManageGateCommand (int crt_n,DMS_COMMON	*int_data, GBRCD	*rcd )
{	
	char  LongNamerStr1[128];
	int dev_num;
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif
	FormCGatePacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n]           = *rcd;

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;

	if ( int_data->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}		

	if(	strcmp (rcd->lnk.act.fmt_def.prog.status,"Up")==0
		||strcmp ( rcd->lnk.act.fmt_def.prog.status, "Down" ) ==0
		||strcmp ( rcd->lnk.act.fmt_def.prog.status, "SetFlux" ) ==0
	  )
	{
		enter_value_high_lmt[crt_n] = int_data->var.anlg.lmt_para.HI_ALARM *10;
		enter_value_low_lmt[crt_n] = (float) (int_data->var.anlg.lmt_para.LO_ALARM  *0.1);
	
		cmd_buf[crt_n].data.fValue     = int_data->var.anlg.fValue; 

		PopupValueEntryBox ( main_w[crt_n], crt_n ,&int_data->point,-1); /*  gtk  */
		return;
	}

       
       /* strcpy ( cmd_str, int_data->fixed.entryLongname );  */
        
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
            GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1  ,string );
		}
	ProgCallAckCelMenu (crt_n, &rectmp[crt_n] );
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif
	
}

void	ManageOnOffCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd )
{
	char  LongNamerStr1[128];
	int dev_num;
	FormCOnOffLinePacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n] = *rcd;	
	GetGroupNumById(int_data->point.stn_id,&dev_num);
//	if ( int_data->point.dev_id > StnDef[int_data->point.stn_id].dev_num )
	if ( int_data->point.dev_id >dev_num) //hcl for station num change
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}
	
        /*strcpy ( cmd_str, int_data->fixed.entryLongname );  */
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1 ,string );
		}
        
	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );
}

void	ManageDevModeCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd )
{
	char  LongNamerStr1[128];
	int dev_num;
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif
	FormCDevModePacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n] = *rcd;	

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;

	if ( int_data->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}

       /* strcpy ( cmd_str, int_data->fixed.entryLongname );  */
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
            GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1,string );
		}
	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif	
}

void	ManageHostSwitchCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd )
{
	char  LongNamerStr1[128];
	FormHostSwitchPacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
      
	rectmp[crt_n] = *rcd;
	
	/* strcpy ( cmd_str, int_data->fixed.entryLongname );  */
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1 ,string );
		}

	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );	
}


void ManageSysModeSetCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd )
{
	char  LongNamerStr1[128];
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif
	FormSysModePacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif
        
        rectmp[crt_n] = *rcd;	
	
        /*strcpy ( cmd_str, int_data->fixed.entryLongname );*/
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string)) 
		{
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1 ,string );
		}
            /*  strcat(cmd_str,string);   */
	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );
}


void	ManageNetSwitchCommand (int crt_n,DMS_COMMON *int_data, GBRCD	*rcd )
{
	char  LongNamerStr1[128];
	int dev_num;
	FormNetSwitchPacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
       
	rectmp[crt_n] = *rcd;

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;
	
	if ( int_data->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}	
       /* strcpy ( cmd_str, int_data->fixed.entryLongname );  */
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
        {
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1 ,string );
		}
			/*strcat(cmd_str,string);*/
	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );	
}


void	ManageDevCommand (int crt_n,DMS_COMMON      *int_data, GBRCD	*rcd )
{	
	char  LongNamerStr1[128];
	int dev_num;
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif
	FormCDevPacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n]   = *rcd;

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;

	if ( int_data->point.dev_id >dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}

        /*strcpy ( cmd_str, int_data->fixed.entryLongname );*/
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1 ,string );
		}
			/*strcat(cmd_str,string);*/
	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif
}

void	ManageSwitchCommand (int crt_n,DMS_COMMON *int_data, GBRCD		*rcd )
{
	char  LongNamerStr1[128];
	int dev_num;
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif
	FormCSwchPacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n]	= *rcd;

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;

	if ( int_data->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}
	
        /*strcpy ( cmd_str, int_data->fixed.entryLongname );  */
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1 ,string );
		}
			/*strcat(cmd_str,string);*/
	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif
}

void	ManageBrkCommand (int crt_n,DMS_COMMON *int_data, GBRCD		*rcd )
{
	char  LongNamerStr1[128];
	int dev_num;
#ifdef AUD_ALM_BEFORE_CMD
	char            db_name[ENTRY_NAME_SIZE];
	char            tmp_cmd_str[100];
#endif
	FormCBrkPacket ( crt_n, int_data, rcd->lnk.act.fmt_def.prog.status );
	rectmp[crt_n]	= *rcd;

	if(-1==GetGroupNumById(int_data->point.stn_id, &dev_num))
		return ;

	if ( int_data->point.dev_id > dev_num )
	{
		PopupMessageBox ( main_w[crt_n], MSG_LCU_NOT_EXIST );
		return;
	}
	
        /*strcpy ( cmd_str, int_data->fixed.entryLongname );*/
        if(-1!=GetPtCmdMsgStrById(&int_data->point, cmd_buf[crt_n].type_id, cmd_buf[crt_n].status.opr, string))               
		{
			GetEntryLongnameById (&int_data->point,LongNamerStr1 );
			GetCMDTypeStr(cmd_buf[crt_n].type_id,cmd_type_str);
			sprintf(cmd_str,"\t\t\t%s:\n\t\t\t%s\t%s  ÃüÁî×´Ì¬=%d \0",LongNamerStr1 ,string ,cmd_type_str,cmd_buf[crt_n].status.opr);
			//sprintf(cmd_str,"%s:\n\t%s\0",LongNamerStr1 ,string );
		}
			/*strcat(cmd_str,string);*/
	ProgCallAckCelMenu ( crt_n, &rectmp[crt_n] );	
#ifdef AUD_ALM_BEFORE_CMD

        strcpy(tmp_cmd_str,aud_cmd_str);
        strcat(tmp_cmd_str,file_aud_dev[int_data->point.dev_id]);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        strcpy(db_name,strrchr(rcd->lnk.dbname,'.'));
        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",db_name+1,end);
        system(tmp_cmd_str);
        printf("aud_cmd_str=%s\n",tmp_cmd_str);

        sprintf(tmp_cmd_str,"%s%s%s%s",aud_cmd_str,"/AUD/",rcd->lnk.act.fmt_def.prog.status,end);
        system(tmp_cmd_str);

        system(send_out);
#endif
}
void GetCMDTypeStr(int type_id,char *cmd_type_str)
{
	switch(type_id)
	{
		case DPS_IOST_SET :
			strcpy(cmd_type_str,"DPS_IOST_SET");
			break;
		case DPS_IOATTR_SET :
			strcpy(cmd_type_str,"DPS_IOATTR_SET");
			break;
		case DPS_IO_GROUP_SET :
			strcpy(cmd_type_str,"DPS_IO_GROUP_SET");
			break;
		case DPS_MAN_VALUE_SET :
			strcpy(cmd_type_str,"DPS_MAN_VALUE_SET");
			break;
		case DPS_MAN_STAT_SET :
			strcpy(cmd_type_str,"DPS_MAN_STAT_SET");
			break;
		case DPS_MAN_CURVE_SET :
			strcpy(cmd_type_str,"DPS_MAN_CURVE_SET");
			break;
		case DPS_MAN_TIME_SET :
			strcpy(cmd_type_str,"DPS_MAN_TIME_SET");
			break;
		case DPS_ANA_PARAM_SET :
			strcpy(cmd_type_str,"DPS_ANA_PARAM_SET");
			break;
		case DPS_IMP_PARAM_SET :
			strcpy(cmd_type_str,"DPS_IMP_PARAM_SET");
			break;
		case DPS_C_ONOFF_SET :
			strcpy(cmd_type_str,"DPS_C_ONOFF_SET");
			break;
		case TIME_SYNC :
			strcpy(cmd_type_str,"TIME_SYNC");
			break;
		case C_LCU_ONOFF :
			strcpy(cmd_type_str,"C_LCU_ONOFF");
			break;
		case C_UNIT :
			strcpy(cmd_type_str,"C_UNIT");
			break;
		case C_BREAKER :
			strcpy(cmd_type_str,"C_BREAKER");
			break;
		case C_SWITCH :
			strcpy(cmd_type_str,"C_SWITCH");
			break;
		case C_APOWER :
			strcpy(cmd_type_str,"C_APOWER");
			break;
		case C_RPOWER :
			strcpy(cmd_type_str,"C_RPOWER");
			break;
		case C_DEVICE :
			strcpy(cmd_type_str,"C_DEVICE");
			break;
		case C_GATE :
			strcpy(cmd_type_str,"C_GATE");
			break;
		case C_TRANS :
			strcpy(cmd_type_str,"C_TRANS");
			break;
		case C_DEV_MODE :
			strcpy(cmd_type_str,"C_DEV_MODE");
			break;
		case C_VOLT_STEP :
			strcpy(cmd_type_str,"C_VOLT_STEP");
			break;
		case C_SPEED_STEP :
			strcpy(cmd_type_str,"C_SPEED_STEP");
			break;
		case C_ONOFF_FORCE :
			strcpy(cmd_type_str,"C_ONOFF_FORCE");
			break;
		case C_ANA_FORCE :
			strcpy(cmd_type_str,"C_ANA_FORCE");
			break;
		case C_IO_CTRL_BLOCK :
			strcpy(cmd_type_str,"C_IO_CTRL_BLOCK");
			break;
		case C_HOST_SYS :
			strcpy(cmd_type_str,"C_HOST_SYS");
			break;
		case C_NET_SWITCH :
			strcpy(cmd_type_str,"C_NET_SWITCH");
			break;
		case C_DATA_POLLING :
			strcpy(cmd_type_str,"C_DATA_POLLING");
			break;
		case C_LCU_PARAM :
			strcpy(cmd_type_str,"C_LCU_PARAM");
			break;
		case C_PLC_SWITCH :
			strcpy(cmd_type_str,"C_PLC_SWITCH");
			break;
		case C_SIGNAL_RESET :
			strcpy(cmd_type_str,"C_SIGNAL_RESET");
			break;
		case C_OP_VALUE_SET :
			strcpy(cmd_type_str,"C_OP_VALUE_SET");
			break;
		case C_SYS_MODE_SET :
			strcpy(cmd_type_str,"C_SYS_MODE_SET");
			break;
		default:
			break;
	}
}
