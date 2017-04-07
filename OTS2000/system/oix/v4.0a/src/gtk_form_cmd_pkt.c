
/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Create Date : 2005.9.28 
	Last Update : 2005.9.29
    Updated acorrding to v4.0db zyp :2006.3
    Unix  have SYN to Windows  by chichi  2006.06.10;
	last SYNC hcl 2007.11.28

---------------------------------------------------------------------*/
#include	"../inc/gtk_widgets.h"

#include	"../inc/oix.h"
#define   DEBUG

extern void 	CmdBufInit ( int crt_n );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void	ProgDoAckCel ( int crt_n );
extern int    GetSrc_Id();
extern int    GetHost_Cmd();
void FormCUnitPacket (int crt_n,DMS_COMMON *int_data, char	*oper_status )	
{	
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
        cmd_buf[crt_n].type_id	= C_UNIT;
        cmd_buf[crt_n].point_id		= int_data->point.pt_id;  
	cmd_buf[crt_n].ctrl_addr	= int_data->fixed.ctrl_addr;
#ifdef	DEBUG
	printf ("OIX:: FormUnitPacket:: oper_status=%s\n", oper_status ); 
#endif

	if ( strcmp ( oper_status,  "Start" ) ==0)	
        	cmd_buf[crt_n].status.opr = START_UNIT; 
	else if ( strcmp ( oper_status,  "Stop" ) ==0)
		cmd_buf[crt_n].status.opr = STOP_UNIT;
	else if ( strcmp ( oper_status,  "Reset" ) == 0)
        	cmd_buf[crt_n].status.opr = RESET;
        else if ( strcmp ( oper_status,  "Step" ) == 0)
        	cmd_buf[crt_n].status.opr = STEP_UNIT;
        else if ( strcmp ( oper_status,  "Jump" ) == 0)
        	cmd_buf[crt_n].status.opr = JUMP_UNIT;
	else if ( strcmp ( oper_status,  "NoVoltRun" ) == 0)
        	cmd_buf[crt_n].status.opr = NO_VOLT_RUN; 
	else if ( strcmp ( oper_status,  "NoLoadRun" ) == 0)
        	cmd_buf[crt_n].status.opr = NO_LOAD_RUN; 
	else if ( strcmp ( oper_status,  "Open" ) == 0)
        	cmd_buf[crt_n].status.opr = UNIT_OPEN; 
	else if ( strcmp ( oper_status,  "BlackStart" ) == 0)
        	cmd_buf[crt_n].status.opr = BLACK_START; 
	else if ( strcmp ( oper_status, "CmpStart" ) == 0)
		cmd_buf[crt_n].status.opr	= CMP_START;
	else if ( strcmp ( oper_status, "EmergStop" ) == 0)
		cmd_buf[crt_n].status.opr	= EMERG_STOP;
	else if ( strcmp ( oper_status, "ReadyStart" ) == 0)
		cmd_buf[crt_n].status.opr	= READY_START;
	else if ( strcmp ( oper_status, "TransNoLoad" ) == 0)
		cmd_buf[crt_n].status.opr	= TRANS_NOLOAD;
	else if ( strcmp ( oper_status, "ZeroVRun" ) == 0)
		cmd_buf[crt_n].status.opr	= ZERO_V_RUN;
	else if ( strcmp ( oper_status, "TranZVRun" ) == 0)
		cmd_buf[crt_n].status.opr	= TRANS_ZV_RUN;
        else if ( strcmp ( oper_status, "ZeroVStop" ) == 0)
                cmd_buf[crt_n].status.opr   = ZERO_V_STOP;
        else if ( strcmp ( oper_status, "StartToCool" ) == 0)
                cmd_buf[crt_n].status.opr   = START_TO_COOL;
        else if ( strcmp ( oper_status, "StartToSpin" ) == 0)
                cmd_buf[crt_n].status.opr   = START_TO_SPIN;
        else if ( strcmp ( oper_status, "StartToNLoad" ) == 0)
                cmd_buf[crt_n].status.opr   = START_TO_NLOAD;
        else if ( strcmp ( oper_status, "StartToOnLin" ) == 0)
                cmd_buf[crt_n].status.opr   = START_TO_ONLIN;
        else if ( strcmp ( oper_status, "StopToNLoad" ) == 0)
                cmd_buf[crt_n].status.opr   = STOP_TO_NLOAD;
        else if ( strcmp ( oper_status, "StopToSpin" ) == 0)
                cmd_buf[crt_n].status.opr   = STOP_TO_SPIN;
        else if ( strcmp ( oper_status, "StopToCool" ) == 0)
                cmd_buf[crt_n].status.opr   = STOP_TO_COOL;
        else if ( strcmp ( oper_status, "StopToStand" ) == 0)
                cmd_buf[crt_n].status.opr   = STOP_TO_STAND;
        else if ( strcmp ( oper_status, "StartStep" ) == 0)
                cmd_buf[crt_n].status.opr   = START_STEP;
        else if ( strcmp ( oper_status, "StopStep" ) == 0)
                cmd_buf[crt_n].status.opr   = STOP_STEP;
        else if ( strcmp ( oper_status, "EmergStep" ) == 0)
                cmd_buf[crt_n].status.opr   = EMERG_STEP;
        else if ( strcmp ( oper_status, "FastStopStep" ) == 0)
                cmd_buf[crt_n].status.opr   = FAST_STOPSTEP;
        else if ( strcmp ( oper_status, "PumpStart" ) == 0)
                cmd_buf[crt_n].status.opr   = PUMP_START;
        else if ( strcmp ( oper_status, "PumpStop" ) == 0)
                cmd_buf[crt_n].status.opr   = PUMP_STOP;
        else if ( strcmp ( oper_status, "DragStart" ) == 0)
                cmd_buf[crt_n].status.opr   = DRAG_START;
	else if ( strcmp ( oper_status, "SFCHSStart" ) == 0)
                cmd_buf[crt_n].status.opr   = SFCHS_START;
	else if ( strcmp ( oper_status, "SFCLSStart" ) == 0)
                cmd_buf[crt_n].status.opr   = SFCLS_START;
	else if ( strcmp ( oper_status, "SFCHSCmp" ) == 0)
                cmd_buf[crt_n].status.opr   = SFCHS_CMP;
	else if ( strcmp ( oper_status, "SFCLSCmp" ) == 0)
                cmd_buf[crt_n].status.opr   = SFCLS_CMP;
	else if ( strcmp ( oper_status, "DragHSStart" ) == 0)
                cmd_buf[crt_n].status.opr   = DRAG_HS_START;
	else if ( strcmp ( oper_status, "DragLSStart" ) == 0)
                cmd_buf[crt_n].status.opr   = DRAG_LS_START;
	else if ( strcmp ( oper_status, "DragHSCmp" ) == 0)
                cmd_buf[crt_n].status.opr   = DRAG_HS_CMP;
	else if ( strcmp ( oper_status, "DragLSCmp" ) == 0)
                cmd_buf[crt_n].status.opr   = DRAG_LS_CMP;
	else if ( strcmp ( oper_status, "PumpToGe" ) == 0)
                cmd_buf[crt_n].status.opr   = PUMP_TO_GEN;
	else if ( strcmp ( oper_status, "GenToPump" ) == 0)
                cmd_buf[crt_n].status.opr   = GEN_TO_PUMP;

        cmd_buf[crt_n].data.iValue		= 0;
	cmd_buf[crt_n].data_type 		= int_data->point.data_type;
}



void FormApowerPacket(int crt_n,DMS_COMMON *dms_cmm, char	*oper_status)
{
      	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= dms_cmm->point.stn_id;
	cmd_buf[crt_n].dev_id		= dms_cmm->point.dev_id;
	cmd_buf[crt_n].point_id		= dms_cmm->point.pt_id;  

	cmd_buf[crt_n].type_id		= C_APOWER;

	if ( strcmp ( oper_status, "Power" ) == 0)
                cmd_buf[crt_n].status.opr   = POWER_ADJ;
	else if ( strcmp ( oper_status, "Opening" ) == 0)
                cmd_buf[crt_n].status.opr   = OPENNING_ADJ;
	else if ( strcmp ( oper_status, "Flux" ) == 0)
                cmd_buf[crt_n].status.opr   = FLUX_ADJ;
	
	cmd_buf[crt_n].data.fValue	= dms_cmm->var.anlg.fValue;
	cmd_buf[crt_n].data_type 	= dms_cmm->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= dms_cmm->fixed.ctrl_addr;	
}

void FormRpowerPacket(int crt_n,DMS_COMMON *dms_cmm, char	*oper_status)
{
      	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= dms_cmm->point.stn_id;
	cmd_buf[crt_n].dev_id		= dms_cmm->point.dev_id;
	cmd_buf[crt_n].point_id		= dms_cmm->point.pt_id;  

	cmd_buf[crt_n].type_id		= C_RPOWER;

	if ( strcmp ( oper_status, "Power" ) == 0)
                cmd_buf[crt_n].status.opr   = POWER_ADJ;
	else if ( strcmp ( oper_status, "Volt" ) == 0)
                cmd_buf[crt_n].status.opr   = VOLT_ADJ;
	else if ( strcmp ( oper_status, "Cos" ) == 0)
                cmd_buf[crt_n].status.opr   = COS_ADJ;
	
	cmd_buf[crt_n].data.fValue	= dms_cmm->var.anlg.fValue;
	cmd_buf[crt_n].data_type 	= dms_cmm->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= dms_cmm->fixed.ctrl_addr;	
}

void	FormCEnterValueAdjItem (int crt_n,float ent_value,LIMIT_VAL *lmtVal, int useLmt)
{	

printf("!!!! in FormCEnterValueAdjItem::: ent_value=%f\n",ent_value);
//	CmdBufInit ( crt_n );
	switch(cmd_buf[crt_n].type_id)
	{		
		case C_APOWER:
		case C_RPOWER:	
        case C_ANA_FORCE:
        case C_GATE:
        case C_UNIT:
        case C_TRANS:
		case DPS_MAN_VALUE_SET:
		case C_OP_VALUE_SET:
		case DPS_MAN_CURVE_SET:
              		cmd_buf[crt_n].data.iValue =( gint) (ent_value *1000);
             		break ;
                        
                case DPS_ANA_PARAM_SET:			
			cmd_buf[crt_n].data.fValue = ent_value ;
                        break;
                        
		case DPS_IMP_PARAM_SET:
                        if(cmd_buf[crt_n].status.opr==IMP_PARAM_SET_CONSTANT 
                            ||cmd_buf[crt_n].status.opr==IMP_PARAM_SET_IMP_ACCUM
							||cmd_buf[crt_n].status.opr==IMP_PARAM_SET_BASE_ACCUM)
                        cmd_buf[crt_n].data.iValue		=(int) ent_value;
                        else
                           cmd_buf[crt_n].data.fValue = ent_value ; 
			break;

		case DPS_C_ONOFF_SET :
			cmd_buf[crt_n].data.iValue = (unsigned short) ent_value;
			break;

		default:
			cmd_buf[crt_n].data.fValue = ent_value;
			break;
	}		
#ifdef	DEBUG
	printf ("OIX: obj_power =%f\n", cmd_buf[crt_n].data.fValue);
#endif
	
	if ( cmd_buf[crt_n].type_id == DPS_IMP_PARAM_SET ||cmd_buf[crt_n].type_id ==C_OP_VALUE_SET || cmd_buf[crt_n].type_id == DPS_C_ONOFF_SET
		||cmd_buf[crt_n].type_id == DPS_ANA_PARAM_SET||cmd_buf[crt_n].type_id == DPS_MAN_CURVE_SET||cmd_buf[crt_n].type_id == DPS_MAN_VALUE_SET) 
	{
            if (useLmt)
                {
                    if ( (ent_value <= lmtVal->up_lmt) 
                                &&  (ent_value >= lmtVal->dn_lmt))
                        ProgDoAckCelBrdcst ( crt_n );
                    else
                        PopupMessageBox ( main_w[crt_n], MSG_LMT_OVER_RANGE );
            }else
               ProgDoAckCelBrdcst ( crt_n ); 
        }
	else
	{
		if ( (ent_value <= lmtVal->up_lmt) 
			&&  (ent_value >= lmtVal->dn_lmt))		
			ProgDoAckCel ( crt_n );	
		else
			PopupMessageBox ( main_w[crt_n], MSG_VALUE_OVER_LMT );		
	}
}

 



void	FormCGatePacket (int crt_n, DMS_COMMON *int_data, char		*oper_status )
{	
#ifdef	DEBUG
	printf ("OIX:: FormGatePacket:: oper_status=%s\n", oper_status ); 
#endif

        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
        cmd_buf[crt_n].type_id		= C_GATE;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

	cmd_buf[crt_n].ctrl_addr	= int_data->fixed.ctrl_addr;     	
	if ( strcmp ( oper_status, "Reset" ) ==0 )
		cmd_buf[crt_n].status.opr = 0;    
        else if( strcmp( oper_status, "Close" ) ==0 )
		cmd_buf[crt_n].status.opr = GATE_CLOSE;        	
	else if ( strcmp ( oper_status, "Open" ) == 0 )
		cmd_buf[crt_n].status.opr = GATE_OPEN;
	else if ( strcmp( oper_status, "Stop") == 0 )
		cmd_buf[crt_n].status.opr =GATE_STOP;
	else if ( strcmp( oper_status, "Up") == 0 )
		cmd_buf[crt_n].status.opr =GATE_UP;
	else if ( strcmp( oper_status, "Down") == 0 )
		cmd_buf[crt_n].status.opr =GATE_DOWN;
	else if ( strcmp( oper_status, "FastOpen") == 0 )
		cmd_buf[crt_n].status.opr =GATE_FASTOPEN;
	else if ( strcmp( oper_status, "FastClose") == 0 )
		cmd_buf[crt_n].status.opr =GATE_FASTCLOSE;
	else if ( strcmp( oper_status, "StepUp") == 0 )
		cmd_buf[crt_n].status.opr =GATE_STEPUP;
	else if ( strcmp( oper_status, "StepDown") == 0 )
		cmd_buf[crt_n].status.opr =GATE_STEPDOWN;
	else if ( strcmp( oper_status, "Forward") == 0 )
		cmd_buf[crt_n].status.opr =GATE_FORWARD;
	else if ( strcmp( oper_status, "Backward") == 0 )
		cmd_buf[crt_n].status.opr =GATE_BACKFORD;
	else if ( strcmp( oper_status, "SetZero") == 0 )
		cmd_buf[crt_n].status.opr =GATE_SETZERO;
	else if ( strcmp( oper_status, "SetFlux") == 0 )
		cmd_buf[crt_n].status.opr =GATE_SETFLUX;        
        else if ( strcmp ( oper_status, "EqualPress" ) == 0)
                cmd_buf[crt_n].status.opr   = GATE_EQUALPRESS;
	cmd_buf[crt_n].data.iValue	= 0;
	cmd_buf[crt_n].data_type = int_data->point.data_type;
}

void FormCTransPacket(int crt_n,DMS_COMMON *dms_cmm, char		*oper_status )
{	
#ifdef	DEBUG
	printf ("OIX:: FormGatePacket:: oper_status=%s\n", oper_status ); 
#endif

        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= dms_cmm->point.stn_id;
        cmd_buf[crt_n].dev_id		= dms_cmm->point.dev_id;
        cmd_buf[crt_n].type_id		= C_TRANS;
	cmd_buf[crt_n].point_id		= dms_cmm->point.pt_id;

	cmd_buf[crt_n].ctrl_addr	= dms_cmm->fixed.ctrl_addr;     	
	 
        if ( strcmp ( oper_status, "Reset" ) ==0 )
		cmd_buf[crt_n].status.opr = 0; 
        else if( strcmp( oper_status, "Raise" ) ==0 )
		cmd_buf[crt_n].status.opr = TRANS_RAISE;        	
	else if ( strcmp ( oper_status, "Down" ) == 0 )
		cmd_buf[crt_n].status.opr = TRANS_DOWN;
	else if ( strcmp( oper_status, "Stop") == 0 )
		cmd_buf[crt_n].status.opr =TRANS_STOP;
	else if ( strcmp( oper_status, "RaiseTo") == 0 )
		cmd_buf[crt_n].status.opr =TRANS_RAISE_TO;
	else if ( strcmp( oper_status, "DownTo") == 0 )
		cmd_buf[crt_n].status.opr =TRANS_DOWN_TO;

	cmd_buf[crt_n].data.iValue	= 0;
	cmd_buf[crt_n].data_type = dms_cmm->point.data_type;
}

void	FormCOnOffLinePacket (int crt_n,DMS_COMMON *int_data, char		*oper_status )
{
#ifdef	DEBUG
	printf (" oix:: FormCOnOffLinePacket: oper_status =%s\n", oper_status ); 
	printf (" oix:: FormCOnOffLinePacket: dev_id=%d\n", int_data->point.dev_id); 
#endif
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

        cmd_buf[crt_n].type_id	= C_LCU_ONOFF;
        
	if ( strcmp ( oper_status, "Online" ) ==0 )
        	cmd_buf[crt_n].status.opr	= LCU_ON_LINE;	
	else if ( strcmp ( oper_status, "Offline" ) ==0 )	
		cmd_buf[crt_n].status.opr	= LCU_OFF_LINE;

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].data.iValue	= 0;
}

void FormCtrlLockPacketAndBrdcst(int crt_n,DMS_COMMON *int_data,int flag)
{
    	time_t	newtime;
	struct	tm	devtime;
	char		*command;
	LAN_HEAD	cmd_head;
        
//	CmdBufInit ( crt_n );

	newtime = time(NULL);
	devtime = *localtime( &newtime);

#ifdef SUN_OS
	Swap4Byte(&newtime);
#endif
	ctrl_lock.stn_id		= int_data->point.stn_id;
	ctrl_lock.dev_id 		= int_data->point.dev_id;
	ctrl_lock.data_type 	= int_data->point.data_type;
	ctrl_lock.type_id 		= DPS_IOST_SET;       
	ctrl_lock.point_id		= int_data->point.pt_id;	
	ctrl_lock.status.opr	= IOST_SET_CTRL_LOCK;
	ctrl_lock.status.state  = flag;	
    ctrl_lock.status.result	= 0; 
	ctrl_lock.status.src    = GetSrc_Id();
	ctrl_lock.data.iValue	= 0;
	ctrl_lock.ctrl_addr		= 0;
	ctrl_lock.host_id  		= GetHost_Cmd();
	ctrl_lock.usr_id 		= ( char ) cur_usr_id;
	ctrl_lock.second 		= devtime.tm_sec;
	ctrl_lock.minute 		= devtime.tm_min;
	ctrl_lock.hour 		= devtime.tm_hour;
	ctrl_lock.day 		= devtime.tm_mday;
	ctrl_lock.month		= devtime.tm_mon+1;
	ctrl_lock.year 		= devtime.tm_year+1900;
 
	strcpy(ctrl_lock.usr_login_name, cur_usr_name);
#ifdef SUN_OS	
	Swap2Byte(&ctrl_lock.year);
#endif   	
	command 	= ( char *)&ctrl_lock;	
	cmd_head.length 	= sizeof ( ctrl_lock );	
	cmd_head.src_id 	= GetHost_Cmd(); 
	cmd_head.dest_id 	= 0;
	cmd_head.dest_stn 	= 0;
	cmd_head.dp_type 	= COMM_RECORD_TIME;
	cmd_head.packet_num 	= 0;
#ifdef DEBUG
printf("******in FormCtrlLockPacketAndBrdcst\n");
	printf("cmd_head::length=%d,dest_id=%d,dest_stn=%d,dp_type=%d,src_id=%d\n",cmd_head.length,cmd_head.dest_id,cmd_head.dest_stn,cmd_head.dp_type,cmd_head.src_id);
	printf("cmd_buf::type_id=%d,stn_id=%d,dev_id=%d,data_type=%d,pt_id=%d\n",ctrl_lock.type_id,ctrl_lock.stn_id,ctrl_lock.dev_id,ctrl_lock.data_type,ctrl_lock.point_id);
	printf("cmd_buf::cmd_status.opr=%d,cmd_status.state=%d,idata=%d,fdata=%.2f,ctrl_addr=%d,host_id=%d,usr_id=%d\n",ctrl_lock.status.opr,ctrl_lock.status.state,ctrl_lock.data.iValue,ctrl_lock.data.fValue,ctrl_lock.ctrl_addr,ctrl_lock.host_id,ctrl_lock.usr_id);
	printf("cmd_buf::usr_name=%s,%d-%d %d:%d:%d\n",ctrl_lock.usr_login_name,ctrl_lock.month,ctrl_lock.day,ctrl_lock.hour,ctrl_lock.minute,ctrl_lock.second);
printf("******end\n");
#endif

#ifdef SUN_OS
	Swap2Byte(&ctrl_lock.point_id);
	Swap2Byte(&ctrl_lock.ctrl_addr);
	Swap2Byte(&ctrl_lock.status);	
	Swap4Byte(&ctrl_lock.data);
	Swap2Byte(&cmd_head.length);
#endif
	lan_out ( command, cmd_head,0 );	
		/*ProgDoAckCelBrdcst ( crt_n );*/
}

void FormIostSetPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status )
{
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

        cmd_buf[crt_n].type_id		= DPS_IOST_SET;        
	 
	if (	strcmp ( oper_status, "Scan_state_Enable" ) ==0 ||
		strcmp ( oper_status, "Scan_state_Disable" ) ==0)
	{
		cmd_buf[crt_n].status.opr	= IOST_SET_SCAN_STATE;
		cmd_buf[crt_n].status.state = ENABLE;
		if(strcmp ( oper_status, "Scan_state_Disable" ) ==0)
			cmd_buf[crt_n].status.state = DISABLE;
	}
	else if( strcmp ( oper_status, "Alm_state_Enable" ) ==0 ||
		 strcmp ( oper_status, "Alm_state_Disable" ) ==0)
	{
		cmd_buf[crt_n].status.opr	= IOST_SET_ALM_STATE;
		cmd_buf[crt_n].status.state = ENABLE;
		if(strcmp ( oper_status, "Alm_state_Disable" ) ==0)
			cmd_buf[crt_n].status.state = DISABLE;
	}
	else if( strcmp ( oper_status, "Alm_ack_state" ) ==0)
	{
		cmd_buf[crt_n].status.opr	= IOST_SET_ALM_NOACK;
		cmd_buf[crt_n].status.state = NO;
	}		
	else if( strcmp ( oper_status, "Tag_state_On" ) ==0 ||
		 strcmp ( oper_status, "Tag_state_Off" ) ==0||
		strcmp ( oper_status, "Tag_state_On2" ) ==0 ||
		strcmp ( oper_status, "Tag_state_On3" ) ==0)
	{
		cmd_buf[crt_n].status.opr	= IOST_SET_TAG_STATE;
		cmd_buf[crt_n].status.state = IOST_TAG_OFF;
		if(strcmp ( oper_status, "Tag_state_On" ) ==0)
			cmd_buf[crt_n].status.state = IOST_TAG_ON; 
		else if(strcmp ( oper_status, "Tag_state_On2" ) ==0)
			cmd_buf[crt_n].status.state = IOST_TAG_ON2; 
		else if(strcmp ( oper_status, "Tag_state_On3" ) ==0)
			cmd_buf[crt_n].status.state = IOST_TAG_ON3;      
	}    	

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].data.iValue	= 0;
}

void FormIoattrSetPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status )
{
      CmdBufInit ( crt_n );
	 cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
      cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	 cmd_buf[crt_n].point_id	= int_data->point.pt_id;

     cmd_buf[crt_n].type_id		= DPS_IOATTR_SET; 
     cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].data.iValue	= 0; 
  
	if(strstr(oper_status,"_Yes")!=(char *)NULL)
		cmd_buf[crt_n].status.state = YES;
	else if(strstr(oper_status,"_No")!=NULL)
		cmd_buf[crt_n].status.state = NO;

	if( strstr ( oper_status, "Statistic" ) !=NULL )	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_STATISTIC;		
	
	else if( strstr ( oper_status, "Failpnt" ) !=NULL)	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_FAIL_PNT;

	else if( strstr ( oper_status, "Faultpnt" ) !=NULL)	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_FAULT_PNT;

	else if( strstr ( oper_status, "Trendalm" ) !=NULL)	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_TREND_ALM;

	else if( strstr ( oper_status, "Keysignal" ) !=NULL)	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_KEY_SIGNAL;

	else if( strstr ( oper_status, "Audioalm" ) !=NULL)	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_AUD_ALM;

	else if( strstr ( oper_status, "Cascadepnt" ) !=NULL)	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_CASCADE_PNT;
	
	else if(strstr ( oper_status, "Hdbrec" ) !=NULL)	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_HDB_REC;	
	
	else if(strstr ( oper_status, "Hdbcurve" ) !=NULL)	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_HDB_CURVE;

	else if(strstr ( oper_status, "Hdb_period" ) !=NULL)
	{	
		cmd_buf[crt_n].status.opr	= IOATTR_SET_HDB_PERIOD;  
		if(strcmp(oper_status,"Hdb_period0")==0)
			cmd_buf[crt_n].data.iValue	= 0; 
		else if(strcmp(oper_status,"Hdb_period1")==0)
			cmd_buf[crt_n].data.iValue = 1;
		else if(strcmp(oper_status,"Hdb_period2")==0)
			cmd_buf[crt_n].data.iValue = 2;
		else if(strcmp(oper_status,"Hdb_period3")==0)
			cmd_buf[crt_n].data.iValue = 3;
		else if(strcmp(oper_status,"Hdb_period4")==0)
			cmd_buf[crt_n].data.iValue = 4;
		else if(strcmp(oper_status,"Hdb_period5")==0)
			cmd_buf[crt_n].data.iValue = 5;
		else
			printf("oper_status error\n");
	} 	
}

void FormManStatSetPacket(int crt_n,DMS_COMMON *int_data, char		*oper_status )
{
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

        cmd_buf[crt_n].type_id		= DPS_MAN_STAT_SET; 
       
	if( strcmp ( oper_status, "Open" ) ==0 )	
		cmd_buf[crt_n].status.opr	= MAN_SET_OPEN;		
	
	else if( strcmp ( oper_status, "Close" ) ==0)	
		cmd_buf[crt_n].status.opr	= MAN_SET_CLOSE;

	else if( strcmp ( oper_status, "Transit" ) ==0)	
		cmd_buf[crt_n].status.opr	= MAN_SET_TRANSIT;

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].data.iValue	= 0; 
}

void FormManCurveSetPacket(int crt_n,DMS_COMMON *dms_cmm, GBRCD   *rcd)
{
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= dms_cmm->point.stn_id;
        cmd_buf[crt_n].dev_id		= dms_cmm->point.dev_id;
	cmd_buf[crt_n].point_id		= dms_cmm->point.pt_id;

        cmd_buf[crt_n].type_id		= DPS_MAN_CURVE_SET; 
       
	if( strcmp ( rcd->lnk.act.fmt_def.prog.status, "Real_Value" ) !=0 )	
		cmd_buf[crt_n].status.opr	= CRUVE_REAL_VALUE;

	cmd_buf[crt_n].data_type = dms_cmm->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= rcd->lnk.dyn.fmt_def.curve_point.point_id;
	cmd_buf[crt_n].data.iValue	= 0; 
}

void FormCTimeSyncPacket(int crt_n,DMS_COMMON *int_data, char		*oper_status)
{
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

        cmd_buf[crt_n].type_id		= TIME_SYNC;
        
	if ( strcmp ( oper_status, "TimeSync" ) ==0 )
        	cmd_buf[crt_n].status.opr	= TIME_SYNC_ON;

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].data.iValue	= 0;
}

void FormCOnOffForcePacket (int crt_n, DMS_COMMON *int_data, char	*oper_status )
{
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

        cmd_buf[crt_n].type_id		= C_ONOFF_FORCE;
        
	if ( strcmp ( oper_status, "UnForce" ) ==0 )
        	cmd_buf[crt_n].status.opr	= UNFORCE;
	else if ( strcmp ( oper_status, "ForceOff" ) ==0 )
        	cmd_buf[crt_n].status.opr	= FORCEOFF;
        else if ( strcmp ( oper_status, "ForceOn" ) ==0 )
        	cmd_buf[crt_n].status.opr	= FORCEON;

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr = int_data->fixed.ctrl_addr;
	cmd_buf[crt_n].data.iValue	= 0;
}

void FormCAnaForcePacket (int crt_n, DMS_COMMON *int_data, char	*oper_status )
{
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

        cmd_buf[crt_n].type_id		= C_ANA_FORCE;
        
	if ( strcmp ( oper_status, "UnForce" ) ==0 )
        {
        	cmd_buf[crt_n].status.opr	= UNFORCE;
                cmd_buf[crt_n].data.iValue      = 0;
        }
	else if ( strcmp ( oper_status, "Force" ) ==0 )
        	cmd_buf[crt_n].status.opr	= ANA_FORCE;

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr = int_data->fixed.ctrl_addr;
	cmd_buf[crt_n].data.fValue	= 0;
}

void FormIoCtlrBlockPacket (int crt_n, DMS_COMMON *int_data, char	*oper_status )
{
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

        cmd_buf[crt_n].type_id		= C_IO_CTRL_BLOCK;
        
	if ( strcmp ( oper_status, "UnBlock" ) ==0 )
        	cmd_buf[crt_n].status.opr	= LOCK_ON ;
	else if ( strcmp ( oper_status, "Block" ) ==0 )
        	cmd_buf[crt_n].status.opr	= LOCK_OFF;

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr = int_data->point.pt_id;
	cmd_buf[crt_n].data.fValue	= 0;
}

void	FormCDevModePacket (int crt_n,DMS_COMMON *int_data, char	*oper_status )
{
#ifdef	DEBUG
	printf (" oix:: FormCOnOffLinePacket: oper_status =%s\n", oper_status ); 
	printf (" oix:: FormCOnOffLinePacket: dev_id=%d\n", int_data->point.dev_id); 
#endif
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;

        cmd_buf[crt_n].type_id	= C_DEV_MODE;
        
	if ( strcmp ( oper_status, "Off" ) ==0 )
        	cmd_buf[crt_n].status.opr	= SET_TRIP_OFF;	
	else if ( strcmp ( oper_status, "On" ) ==0 )	
		cmd_buf[crt_n].status.opr	= SET_TRIP_ON;
	else if (strcmp ( oper_status, "On2" ) ==0 )	
		cmd_buf[crt_n].status.opr	= SET_TRIP_ON2;
	else if (strcmp ( oper_status, "On3" ) ==0 )	
		cmd_buf[crt_n].status.opr	= SET_TRIP_ON3;

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr	= int_data->fixed.ctrl_addr;
	cmd_buf[crt_n].data.iValue	= 0;
}

void	FormHostSwitchPacket (int crt_n,DMS_COMMON *int_data, char	*oper_status )
{
#ifdef	DEBUG
	printf (" oix:: FormHostSwitchPacket: oper_status =%s\n", oper_status ); 
#endif
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;
        cmd_buf[crt_n].type_id		= C_HOST_SYS;
        
	if ( strcmp ( oper_status, "MainHost" ) ==0 )
        	cmd_buf[crt_n].status.opr	= BE_HOST;
	else if ( strcmp ( oper_status, "ViceHost" ) ==0 )
        	cmd_buf[crt_n].status.opr	= BE_VICE;
	cmd_buf[crt_n].data_type = int_data->point.data_type;

	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].data.iValue	= 0;
}

void	FormPlcSwitchPacket (int crt_n,DMS_COMMON *int_data, char	*oper_status )
{
#ifdef	DEBUG
	printf (" oix:: FormHostSwitchPacket: oper_status =%s\n", oper_status ); 
#endif
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;
        cmd_buf[crt_n].type_id		= C_PLC_SWITCH;
        
	if ( strcmp ( oper_status, "MainCPU" ) ==0 )
        	cmd_buf[crt_n].status.opr	= BE_HOST;
	else if ( strcmp ( oper_status, "ViceCPU" ) ==0 )
        	cmd_buf[crt_n].status.opr	= BE_VICE;
	cmd_buf[crt_n].data_type = int_data->point.data_type;

	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].data.iValue	= 0;
}

void FormSysModePacket (int crt_n,DMS_COMMON *int_data, char		*oper_status )
{
#ifdef	DEBUG
	printf (" oix:: FormSysMsgPacket: oper_status =%s\n", oper_status ); 
#endif
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
        cmd_buf[crt_n].point_id		= int_data->point.pt_id;
        cmd_buf[crt_n].type_id		= C_SYS_MODE_SET;
	if ( strcmp ( oper_status, "Off" ) ==0 )
        	cmd_buf[crt_n].status.opr	= SET_TRIP_OFF;
	else if ( strcmp ( oper_status, "On" ) ==0 )
        	cmd_buf[crt_n].status.opr	= SET_TRIP_ON;
	else if ( strcmp ( oper_status, "On2" ) ==0 )
        	cmd_buf[crt_n].status.opr	= SET_TRIP_ON2;
	else if ( strcmp ( oper_status, "On3" ) ==0 )
        	cmd_buf[crt_n].status.opr	= SET_TRIP_ON3;
	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr = 0;
	cmd_buf[crt_n].data.iValue=0;
}

void FormNetSwitchPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status )
{
#ifdef	DEBUG
	printf (" oix:: FormHostSwitchPacket: oper_status =%s\n", oper_status ); 
#endif
        CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= int_data->point.stn_id;
        cmd_buf[crt_n].dev_id		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;
        cmd_buf[crt_n].type_id		= C_NET_SWITCH;
        
	if ( strcmp ( oper_status, "NetA" ) ==0 )
        	cmd_buf[crt_n].status.opr	= BE_NETA;
	else if ( strcmp ( oper_status, "NetB" ) ==0 )
        	cmd_buf[crt_n].status.opr	= BE_NETB;
	else if ( strcmp ( oper_status, "NetC" ) ==0 )
        	cmd_buf[crt_n].status.opr	= BE_NETC; 
	else if ( strcmp ( oper_status, "NetD" ) ==0 )
        	cmd_buf[crt_n].status.opr	= BE_NETD;

	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].data.iValue = 0;
	cmd_buf[crt_n].ctrl_addr =0;
}

void	FormCSwchPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status )
{
	
#ifdef	DEBUG
	printf (" oix:: FormCSwitchPacket: oper_status =%s\n", oper_status ); 
#endif
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id 		= int_data->point.stn_id;
	cmd_buf[crt_n].dev_id 		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id; 
	cmd_buf[crt_n].type_id 		= C_SWITCH;
	
	cmd_buf[crt_n].ctrl_addr	= int_data->fixed.ctrl_addr;		
			
	if ( strcmp ( oper_status, "Close" ) == 0)		 
		cmd_buf[crt_n].status.opr	=  SWITCH_CLOSE;
	else if ( strcmp ( oper_status, "Open" ) ==0 ) 
		cmd_buf[crt_n].status.opr	= SWITCH_OPEN;
	if ( strcmp ( oper_status, "TestOpen" ) == 0 ) 
		cmd_buf[crt_n].status.opr 	= SWITCH_TEST_OPEN;	
	else if ( strcmp ( oper_status, "TestClose" ) == 0) 
		cmd_buf[crt_n].status.opr	= SWITCH_TEST_CLOSE;
        else if(strcmp ( oper_status, "Reset" ) == 0) 
		cmd_buf[crt_n].status.opr	= 0;

	cmd_buf[crt_n].data.iValue	= 0;
	cmd_buf[crt_n].data_type 	= int_data->point.data_type;
}

void	FormCBrkPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status )
{	
#ifdef	DEBUG
	printf (" oix:: FormCBrkPacket: oper_status =%s\n", oper_status ); 
#endif
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id 		= int_data->point.stn_id;
	cmd_buf[crt_n].dev_id 		= int_data->point.dev_id;
	cmd_buf[crt_n].point_id		= int_data->point.pt_id;
	cmd_buf[crt_n].type_id 		= C_BREAKER;	
	
	cmd_buf[crt_n].ctrl_addr	= int_data->fixed.ctrl_addr;  
	
	if ( strcmp ( oper_status, "Reset" ) == 0 ) 
		cmd_buf[crt_n].status.opr 	= 0;
	else if ( strcmp ( oper_status, "Open" ) ==0)  
		cmd_buf[crt_n].status.opr 	= TRIP_OFF;	
	else if ( strcmp ( oper_status, "Close" ) == 0)
		cmd_buf[crt_n].status.opr 	= TRIP_ON; 
	else if ( strcmp ( oper_status, "TestClose" ) == 0) 
		cmd_buf[crt_n].status.opr 	= TEST_TRIP_ON; 
	else if ( strcmp ( oper_status, "IM_Line" ) == 0)
		cmd_buf[crt_n].status.opr 	= IM_Line_ON; 
	else if ( strcmp ( oper_status, "IIM_Line" ) == 0)
		cmd_buf[crt_n].status.opr 	= IIM_Line_ON; 
	else if ( strcmp ( oper_status, "Line_IM" ) == 0)
		cmd_buf[crt_n].status.opr 	= Line_IM_ON; 
	else if ( strcmp ( oper_status, "Line_IIM" ) == 0)
		cmd_buf[crt_n].status.opr 	= Line_IIM_ON; 
	else if ( strcmp ( oper_status, "Tran_IM" ) == 0)
		cmd_buf[crt_n].status.opr 	= Tran_IM_ON; 
	else if ( strcmp ( oper_status, "Tran_IIM" ) == 0)
		cmd_buf[crt_n].status.opr 	= Tran_IIM_ON;
	else if ( strcmp ( oper_status, "IM_Tran" ) == 0)
		cmd_buf[crt_n].status.opr 	= IM_Tran_ON; 
	else if ( strcmp ( oper_status, "IIM_Tran" ) == 0)
		cmd_buf[crt_n].status.opr 	= IIM_Tran_ON; 
	else if ( strcmp ( oper_status, "CancelSyn" ) == 0)
		cmd_buf[crt_n].status.opr 	= CANCEL_SYN; 
	else if ( strcmp ( oper_status, "LoopClose" ) == 0)
		cmd_buf[crt_n].status.opr 	= LOOP_CLOSE; 
	else if ( strcmp ( oper_status, "ChgVltCls" ) == 0) 
		cmd_buf[crt_n].status.opr 	= CHG_VLT_CLOSE; 
	else if ( strcmp ( oper_status, "ZroVltCls" ) == 0 )
		cmd_buf[crt_n].status.opr 	= ZRO_VLT_CLOSE; 	
        else if ( strcmp ( oper_status, "TestOpen" ) ==0)  
		cmd_buf[crt_n].status.opr 	= TEST_OPEN;
        
	cmd_buf[crt_n].data.iValue	= 0;
	cmd_buf[crt_n].data_type = int_data->point.data_type;	
}

void FormCDevPacket (int crt_n,DMS_COMMON *int_data, char *oper_status )
{	
#ifdef	DEBUG
	printf (" oix:: FormCSwitchPacket: oper_status =%s\n", oper_status ); 
#endif
	CmdBufInit ( crt_n );

	cmd_buf[crt_n].type_id 	= C_DEVICE;
	cmd_buf[crt_n].stn_id 	= int_data->point.stn_id;
	cmd_buf[crt_n].dev_id 	= int_data->point.dev_id;
	cmd_buf[crt_n].point_id	= int_data->point.pt_id;
	cmd_buf[crt_n].data_type= int_data->point.data_type;
	
	cmd_buf[crt_n].ctrl_addr= int_data->fixed.ctrl_addr;
  	
				
	if(strcmp ( oper_status, "On" ) ==0)
		cmd_buf[crt_n].status.opr = TRIP_ON;
	else if(strcmp ( oper_status, "Off" ) ==0)
		cmd_buf[crt_n].status.opr = TRIP_OFF;
        else if(strcmp ( oper_status, "Reset" ) ==0)
		cmd_buf[crt_n].status.opr = 0;
		
	cmd_buf[crt_n].data.iValue	= 0;
}


void	FormCSignRtnPacket (int crt_n,DMS_COMMON *int_data, char		*oper_status )
{
#ifdef	DEBUG
	printf (" oix:: FormCSignRtnPacket: oper_status =%s\n", oper_status ); 
#endif
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id 	= int_data->point.stn_id;
	cmd_buf[crt_n].dev_id 	= int_data->point.dev_id;
	cmd_buf[crt_n].point_id	= int_data->point.pt_id; 
	cmd_buf[crt_n].type_id  = C_SIGNAL_RESET;
	
	cmd_buf[crt_n].data.iValue	= 0; 
	
	if ( strcmp ( oper_status, "FaultReset" ) ==0)
		cmd_buf[crt_n].status.opr = FAULT_RESET;
	else if (strcmp ( oper_status, "FailReset" ) ==0  )		
		cmd_buf[crt_n].status.opr = FAIL_RESET;
	cmd_buf[crt_n].data_type = int_data->point.data_type;
	cmd_buf[crt_n].ctrl_addr = int_data->fixed.ctrl_addr;
}	
/////added by chihl.  10.06.07 ;  

void FormManTimeSetPacket(int crt_n,DMS_COMMON *dms_cmm, char	*oper_status)
{
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= dms_cmm->point.stn_id;
	cmd_buf[crt_n].dev_id		= dms_cmm->point.dev_id;
	cmd_buf[crt_n].point_id		= dms_cmm->point.pt_id; 
	cmd_buf[crt_n].data_type 	= dms_cmm->point.data_type;
	
	cmd_buf[crt_n].type_id		= DPS_MAN_TIME_SET   ;
	
    ////////// to be continued!!
	
	cmd_buf[crt_n].data.fValue	= dms_cmm->var.anlg.fValue;
	
	cmd_buf[crt_n].ctrl_addr	=0;	
}

void AlmBlinkReset (int crt_n,POINTER *pt )
{
#ifdef	DEBUG
	printf (" oix:: FormAlckPacket: oper_status %d.%d.%d.%d \n", pt->stn_id ,pt->dev_id ,pt->data_type ,pt->pt_id ); 
#endif
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id 	= pt->stn_id;
	cmd_buf[crt_n].dev_id 	= pt->dev_id;
	cmd_buf[crt_n].point_id	= pt->pt_id;
        cmd_buf[crt_n].data_type = pt->data_type;
	cmd_buf[crt_n].type_id  = DPS_IOST_SET;
        cmd_buf[crt_n].status.opr  =  3  ;       /* ALM_ACK_STATE */
        cmd_buf[crt_n].status.state=  0  ;    /* ack */
        cmd_buf[crt_n].status.src  =  0  ;      /* ack */
        memset(&cmd_buf[crt_n].data ,0, sizeof (int));   
        ProgDoAckCelBrdcst (  crt_n );
 	 
}



