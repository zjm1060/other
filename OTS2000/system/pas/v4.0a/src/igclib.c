#include	"../../../pas/v4.0a/inc/igclib.h"
 

int  IGC_GetCurentHostCtrlPrvg(int Itype)
{	
	char            host_name[15];
	USHORT          status;
	UCHAR           host_id;
	char    	entry_name[60]={""} ;
	short		db_err;
	
	GetLocalHostId(&host_id);
	if(Itype==1)
		printf("\nIGC:this host_id=%d\n",host_id);
	GetHostNameById(host_id,host_name);
	if(Itype==1)
		printf("\nIGC:this host_name=%s\n",host_name);
	GetHostStatePtNameStr(host_name, entry_name);
 
	if(Itype==1)
		printf("\nIGC:GetCurWSCtrlPrvg::entry_name=%s\n",entry_name);
 
	db_err = GetIndPtStateByNameStr( entry_name, &status );
	if(db_err==-1)
	{
		printf("\nread db err!!! entry_name=%s\n",entry_name);
		return db_err;
	}
	else
		return  status;
}


int IGC_SEND_H9000_SMG(char * smg)
{
	return	BdSms(smg);
}

int  igc_InitStrTextDefine()
{
	return InitStrTextDefine();
}

int IGC_Init_RECORD_IPC()
{	
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		igc_Err_log("IGC:Fail to link record shm");
		return -1;
	}



	shm_fault_addr= (FAULT_RECORD_TABLE*) LinktoShareMem( SHM_FAULT_TABLE_KEY_CHAR );
	if( shm_fault_addr == (FAULT_RECORD_TABLE*)-1 )
	{
		igc_Err_log("IGC:Fail to link fault record shm");
		return -1;
	}
	return 0;
}
 
H9000_IGC_alarm_record igc_search_one_record_by_logicalname(char * logicalname,int Forward_num,int Forward_index)
{
	int	        	rec_ptr;
	POINTER	        	point;
	int                 alarm_type;
	char	           	namestr[POINT_NAME_SIZE];
	int                 temp=0;
    ALM_STR            igc_alm_rec;
	H9000_IGC_alarm_record R_igc_alm_rec;
    int i;
	int Fi;
	IGC_datetime        temp_igctm;
 
	memset(&igc_alm_rec,0,sizeof(ALM_STR));
	memset(&R_igc_alm_rec,0,sizeof(H9000_IGC_alarm_record));
	if(Forward_num<=0) 
		Forward_num=shm_rec_addr->head.buf_size;
	if(Forward_index<1)
		Forward_index=1;
	if(Forward_index>=shm_rec_addr->head.buf_size)
		Forward_index=shm_rec_addr->head.buf_size;
	R_igc_alm_rec.ok_flag=-1;

	for(i=0,Fi=0,rec_ptr=shm_rec_addr->head.header_ptr+1;i<Forward_num;i++)
	{	
		rec_ptr--;
		if(rec_ptr<0)
			rec_ptr= shm_rec_addr->head.buf_size;

		temp=igc_GetPointIdFromRec(&shm_rec_addr->rec[rec_ptr], &point,&alarm_type);
		R_igc_alm_rec.ok_flag=-1;
	 
	    if(1==temp)
		{
			//R_igc_alm_rec.ok_flag=-1;
			continue;
		}
		else if(-1!=temp)
		{
			if(GetPtNameById(&point, namestr, NULL)==-1)
			{
				//R_igc_alm_rec.ok_flag=-1;
				continue;
			}
			else if(strcmp(logicalname,namestr)==0)
			{
				if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_SM_ALM || shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_SMS 
					|| shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_SMS)
				{	
					temp_igctm=	get_datetime_by_time_t(shm_rec_addr->rec[rec_ptr].sms_rec.time);
				}
				else if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_ANA_ALM || shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_IMP_ALM)
				{
					temp_igctm.year= shm_rec_addr->rec[rec_ptr].ana_rec.year;
					temp_igctm.month= shm_rec_addr->rec[rec_ptr].ana_rec.month;
					temp_igctm.day= shm_rec_addr->rec[rec_ptr].ana_rec.day;
					temp_igctm.hour= shm_rec_addr->rec[rec_ptr].ana_rec.hour;
					temp_igctm.minute= shm_rec_addr->rec[rec_ptr].ana_rec.minute;
					temp_igctm.second= shm_rec_addr->rec[rec_ptr].ana_rec.second;
				}
				else if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_IND_ALM)
				{
					temp_igctm.year= shm_rec_addr->rec[rec_ptr].ind_rec.year;
					temp_igctm.month= shm_rec_addr->rec[rec_ptr].ind_rec.month;
					temp_igctm.day= shm_rec_addr->rec[rec_ptr].ind_rec.day;
					temp_igctm.hour= shm_rec_addr->rec[rec_ptr].ind_rec.hour;
					temp_igctm.minute= shm_rec_addr->rec[rec_ptr].ind_rec.minute;
					temp_igctm.second= shm_rec_addr->rec[rec_ptr].ind_rec.second;
				}
				else continue;

				R_igc_alm_rec.ok_flag=1;
			}
			else
				continue;

			if(-1==GetAlmStrings(&shm_rec_addr->rec[rec_ptr], &igc_alm_rec, NULL, NULL))
			{
				R_igc_alm_rec.ok_flag=-1;
			}
			else
			{
				R_igc_alm_rec.ok_flag=1;
			 
				strcpy(R_igc_alm_rec.hostname,igc_alm_rec.hostname);
				strcpy(R_igc_alm_rec.logicname,igc_alm_rec.logicname);
				strcpy(R_igc_alm_rec.longname,igc_alm_rec.longname);
				strcpy(R_igc_alm_rec.message,igc_alm_rec.message);
				strcpy(R_igc_alm_rec.repeatAlm,igc_alm_rec.repeatAlm);
				strcpy(R_igc_alm_rec.sn_str,igc_alm_rec.sn_str);
				strcpy(R_igc_alm_rec.state_str,igc_alm_rec.state_str);
				strcpy(R_igc_alm_rec.time_str,igc_alm_rec.time_str);
				strcpy(R_igc_alm_rec.usrname,igc_alm_rec.usrname);
				
				
				R_igc_alm_rec.igc_datetime.year= temp_igctm.year;
				R_igc_alm_rec.igc_datetime.month= temp_igctm.month;
				R_igc_alm_rec.igc_datetime.day= temp_igctm.day;
				R_igc_alm_rec.igc_datetime.hour= temp_igctm.hour;
				R_igc_alm_rec.igc_datetime.minute= temp_igctm.minute;
				R_igc_alm_rec.igc_datetime.second= temp_igctm.second;

				break;
			}
		}
	}
	return R_igc_alm_rec;
}

H9000_IGC_alarm_record igc_usetime_search_one_record_by_logicalname(char * logicalname,int Forward_num,int Forward_index,IGC_datetime start_time)
{
	int	        	rec_ptr;
	POINTER	        	point;
	int                 alarm_type;
	char	           	namestr[POINT_NAME_SIZE];
	int                 temp=0;
    ALM_STR            igc_alm_rec;
	H9000_IGC_alarm_record R_igc_alm_rec;
    int i;
	int Fi;
	IGC_datetime        temp_igctm;
	long tmp_tsec;
	
	memset(&igc_alm_rec,0,sizeof(ALM_STR));
	memset(&R_igc_alm_rec,0,sizeof(H9000_IGC_alarm_record));
	if(Forward_num<=0) 
		Forward_num=shm_rec_addr->head.buf_size;
	if(Forward_index<1)
		Forward_index=1;
	if(Forward_index>=shm_rec_addr->head.buf_size)
		Forward_index=shm_rec_addr->head.buf_size;
	R_igc_alm_rec.ok_flag=-1;

	for(i=0,Fi=0,rec_ptr=shm_rec_addr->head.header_ptr+1;i<Forward_num;i++)
	{	
		rec_ptr--;
		if(rec_ptr<0)
			rec_ptr= shm_rec_addr->head.buf_size;

		temp=igc_GetPointIdFromRec(&shm_rec_addr->rec[rec_ptr], &point,&alarm_type);
		R_igc_alm_rec.ok_flag=-1;
	 
	    if(1==temp)
		{
			//R_igc_alm_rec.ok_flag=-1;
			continue;
		}
		else if(-1!=temp)
		{
			if(GetPtNameById(&point, namestr, NULL)==-1)
			{
				//R_igc_alm_rec.ok_flag=-1;
				continue;
			}
			else if(strcmp(logicalname,namestr)==0)
			{
				if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_SM_ALM || shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_SMS 
					|| shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_SMS)
				{	
					temp_igctm=	get_datetime_by_time_t(shm_rec_addr->rec[rec_ptr].sms_rec.time);
				}
				else
				if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_ANA_ALM || shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_IMP_ALM)
				{
					temp_igctm.year= shm_rec_addr->rec[rec_ptr].ana_rec.year;
					temp_igctm.month= shm_rec_addr->rec[rec_ptr].ana_rec.month;
					temp_igctm.day= shm_rec_addr->rec[rec_ptr].ana_rec.day;
					temp_igctm.hour= shm_rec_addr->rec[rec_ptr].ana_rec.hour;
					temp_igctm.minute= shm_rec_addr->rec[rec_ptr].ana_rec.minute;
					temp_igctm.second= shm_rec_addr->rec[rec_ptr].ana_rec.second;
				}
				else if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_IND_ALM)
				{
					temp_igctm.year= shm_rec_addr->rec[rec_ptr].ind_rec.year;
					temp_igctm.month= shm_rec_addr->rec[rec_ptr].ind_rec.month;
					temp_igctm.day= shm_rec_addr->rec[rec_ptr].ind_rec.day;
					temp_igctm.hour= shm_rec_addr->rec[rec_ptr].ind_rec.hour;
					temp_igctm.minute= shm_rec_addr->rec[rec_ptr].ind_rec.minute;
					temp_igctm.second= shm_rec_addr->rec[rec_ptr].ind_rec.second;
				}
				else continue;

			
				tmp_tsec=total_time_seconds(start_time,temp_igctm);

			/*	printf("\n<<<<<<<<<<<< \n  %4d-%2d-%2d %2d:%2d:%2d \n%4d-%2d-%2d %2d:%2d:%2d  \ntmp_tsec=%d",
					temp_igctm.year,temp_igctm.month,temp_igctm.day,
					temp_igctm.hour,temp_igctm.minute,temp_igctm.second,
					start_time.year,start_time.month,start_time.day,
					start_time.hour,start_time.minute,start_time.second,tmp_tsec);

*/
				if(tmp_tsec>1200)
					break;
				if(tmp_tsec>0)
						continue;
				Fi++;
				if(Fi<Forward_index)
					continue;
				R_igc_alm_rec.ok_flag=1;
			}
			else
				continue;

			if(-1==GetAlmStrings(&shm_rec_addr->rec[rec_ptr], &igc_alm_rec, NULL, NULL))
			{
				R_igc_alm_rec.ok_flag=-1;
			}
			else
			{
				R_igc_alm_rec.ok_flag=1;
			 
				strcpy(R_igc_alm_rec.hostname,igc_alm_rec.hostname);
				strcpy(R_igc_alm_rec.logicname,igc_alm_rec.logicname);
				strcpy(R_igc_alm_rec.longname,igc_alm_rec.longname);
				strcpy(R_igc_alm_rec.message,igc_alm_rec.message);
				strcpy(R_igc_alm_rec.repeatAlm,igc_alm_rec.repeatAlm);
				strcpy(R_igc_alm_rec.sn_str,igc_alm_rec.sn_str);
				strcpy(R_igc_alm_rec.state_str,igc_alm_rec.state_str);
				strcpy(R_igc_alm_rec.time_str,igc_alm_rec.time_str);
				strcpy(R_igc_alm_rec.usrname,igc_alm_rec.usrname);
				
				
				R_igc_alm_rec.igc_datetime.year= temp_igctm.year;
				R_igc_alm_rec.igc_datetime.month= temp_igctm.month;
				R_igc_alm_rec.igc_datetime.day= temp_igctm.day;
				R_igc_alm_rec.igc_datetime.hour= temp_igctm.hour;
				R_igc_alm_rec.igc_datetime.minute= temp_igctm.minute;
				R_igc_alm_rec.igc_datetime.second= temp_igctm.second;

				break;
			}
		}
	}
	return R_igc_alm_rec;
}


H9000_IGC_alarm_record igc_search_one_alarm_by_logicalname(char * logicalname,int Forward_num)
{
	int	            	rec_ptr;
	POINTER	        	point;
	int                 alarm_type;
	char	           	namestr[POINT_NAME_SIZE];
	int                 temp=0;
    ALM_STR             igc_alm_rec;
	H9000_IGC_alarm_record R_igc_alm_rec;
    int i;
	int Fi;
	int Forward_index=1;
	IGC_datetime        temp_igctm;
 
	
	memset(&igc_alm_rec,0,sizeof(ALM_STR));
	memset(&R_igc_alm_rec,0,sizeof(H9000_IGC_alarm_record));
	if(Forward_num<=0) 
		Forward_num=shm_fault_addr->head.buf_size;

	if(Forward_num>=shm_fault_addr->head.count) 
		Forward_num=shm_fault_addr->head.count;

	if(Forward_num>=shm_fault_addr->head.buf_size)
		Forward_num=shm_fault_addr->head.buf_size;

	/*if(Forward_index<1)
		Forward_index=1;
	if(Forward_index>=shm_fault_addr->head.buf_size)
		Forward_index=shm_fault_addr->head.buf_size;*/
	R_igc_alm_rec.ok_flag=-1;
	
	for(i=0,Fi=0,rec_ptr=shm_fault_addr->head.header_ptr;i<Forward_num;i++)
	{	
		rec_ptr--;
	    //	if(rec_ptr<0)
		//	rec_ptr= shm_fault_addr->head.buf_size;

			if(rec_ptr<0)
			break;
		
		temp=igc_GetPointIdFromRec(&shm_fault_addr->rec[rec_ptr], &point,&alarm_type);
		R_igc_alm_rec.ok_flag=-1;
		
		if(1==temp)
		{
			//R_igc_alm_rec.ok_flag=-1;
			continue;
		}
		else if(-1!=temp)
		{
			if(GetPtNameById(&point, namestr, NULL)==-1)
			{
				//R_igc_alm_rec.ok_flag=-1;
				continue;
			}
			else if(strcmp(logicalname,namestr)==0)
			{
				Fi++;
				if(Fi<Forward_index)
					continue;

				if(shm_fault_addr->rec[rec_ptr].ana_rec.type_id==DPS_SM_ALM || shm_fault_addr->rec[rec_ptr].ana_rec.type_id==DPS_SMS 
					|| shm_fault_addr->rec[rec_ptr].ana_rec.type_id==DPS_SMS)
				{	
					temp_igctm=	get_datetime_by_time_t(shm_fault_addr->rec[rec_ptr].sms_rec.time);
				}
				else if(shm_fault_addr->rec[rec_ptr].ana_rec.type_id==DPS_ANA_ALM || shm_fault_addr->rec[rec_ptr].ana_rec.type_id==DPS_IMP_ALM)
				{
					temp_igctm.year= shm_fault_addr->rec[rec_ptr].ana_rec.year;
					temp_igctm.month= shm_fault_addr->rec[rec_ptr].ana_rec.month;
					temp_igctm.day= shm_fault_addr->rec[rec_ptr].ana_rec.day;
					temp_igctm.hour= shm_fault_addr->rec[rec_ptr].ana_rec.hour;
					temp_igctm.minute= shm_fault_addr->rec[rec_ptr].ana_rec.minute;
					temp_igctm.second= shm_fault_addr->rec[rec_ptr].ana_rec.second;
				}
				else if(shm_fault_addr->rec[rec_ptr].ana_rec.type_id==DPS_IND_ALM)
				{
					temp_igctm.year= shm_fault_addr->rec[rec_ptr].ind_rec.year;
					temp_igctm.month= shm_fault_addr->rec[rec_ptr].ind_rec.month;
					temp_igctm.day= shm_fault_addr->rec[rec_ptr].ind_rec.day;
					temp_igctm.hour= shm_fault_addr->rec[rec_ptr].ind_rec.hour;
					temp_igctm.minute= shm_fault_addr->rec[rec_ptr].ind_rec.minute;
					temp_igctm.second= shm_fault_addr->rec[rec_ptr].ind_rec.second;
				}
				else continue;

				R_igc_alm_rec.ok_flag=1;
			}
			else
				continue;
			
			if(-1==GetAlmStrings(&shm_fault_addr->rec[rec_ptr], &igc_alm_rec, NULL, NULL))
			{
				R_igc_alm_rec.ok_flag=-1;
			}
			else
			{
				R_igc_alm_rec.ok_flag=1;
				
				strcpy(R_igc_alm_rec.hostname,igc_alm_rec.hostname);
				strcpy(R_igc_alm_rec.logicname,igc_alm_rec.logicname);
				strcpy(R_igc_alm_rec.longname,igc_alm_rec.longname);
				strcpy(R_igc_alm_rec.message,igc_alm_rec.message);
				strcpy(R_igc_alm_rec.repeatAlm,igc_alm_rec.repeatAlm);
				strcpy(R_igc_alm_rec.sn_str,igc_alm_rec.sn_str);
				strcpy(R_igc_alm_rec.state_str,igc_alm_rec.state_str);
				strcpy(R_igc_alm_rec.time_str,igc_alm_rec.time_str);
				strcpy(R_igc_alm_rec.usrname,igc_alm_rec.usrname);


				
				R_igc_alm_rec.igc_datetime.year= temp_igctm.year;
				R_igc_alm_rec.igc_datetime.month= temp_igctm.month;
				R_igc_alm_rec.igc_datetime.day= temp_igctm.day;
				R_igc_alm_rec.igc_datetime.hour= temp_igctm.hour;
				R_igc_alm_rec.igc_datetime.minute= temp_igctm.minute;
				R_igc_alm_rec.igc_datetime.second= temp_igctm.second;
				
				break;
			}
		}
	}
	return R_igc_alm_rec;
}


int igc_GetPointIdFromRec(REC_ENTRY_UNION *rec, POINTER *pt, int *alm_type)
{   
    switch(rec->ana_rec.type_id)
	{
	case	DPS_ANA_ALM:
		pt->stn_id =  rec->ana_rec.stn_id ;
		pt->dev_id =  rec->ana_rec.dev_id ;
		pt->data_type =  rec->ana_rec.data_type ;
		pt->pt_id =  rec->ana_rec.pt_id ;
		*alm_type=rec->ana_rec.status.ana_status.alm_type;
		return 0;
		
	case	DPS_IMP_ALM:
		pt->stn_id =  rec->ana_rec.stn_id ;
		pt->dev_id =  rec->ana_rec.dev_id ;
		pt->data_type =  rec->ana_rec.data_type ;
		pt->pt_id =  rec->ana_rec.pt_id ;
		*alm_type= rec->ana_rec.status.imp_status.alm_type;
		return 0;

	case	DPS_IND_ALM:
		pt->stn_id =  rec->ind_rec.stn_id ;
		pt->dev_id =  rec->ind_rec.dev_id ;
		pt->data_type =  rec->ind_rec.data_type ;
		pt->pt_id =  rec->ind_rec.pt_id ;
		*alm_type= rec->ind_rec.ind_status.alm_type;
		return 0;
		
/*	case	DPS_ANA_PARAM_SET:
	case	DPS_IMP_PARAM_SET:
	case	DPS_C_ONOFF_SET:
		pt->stn_id =  rec->param_set_rec.stn_id ;
		pt->dev_id =  rec->param_set_rec.dev_id ;
		pt->data_type =  rec->param_set_rec.data_type ;
		pt->pt_id =  rec->param_set_rec.pt_id ;
		return 0;
	*/	
	case	DPS_LMS:
	case	DPS_SMS:
	case	DPS_SM_ALM:
		return 1;
		
	default:
		return -1;
	}
}
int igc_userlogin(char * user,char * pass)
{
	
	char igc_buf[50];
	memset(igc_user_login_name,0,20);
	if(CheckUserLogin(user,pass))
	{
        strcpy(igc_user_login_name,user);
		sprintf(igc_buf,"IGC_login %s",igc_user_login_name);  
		BdSms(igc_buf);
		printf("\nIGC_user login OK ,%s\n",igc_user_login_name);
		return 1;
	}
	 
	sprintf(igc_buf,"IGC login fault,User Fault");  
	BdSms(igc_buf);
	printf("\nIGC_user login fault ,%s\n",igc_user_login_name);
	return -1;
}

int igc_userunlogin()
{
	char igc_buf[50];
	memset(igc_user_login_name,0,sizeof(igc_user_login_name));
	memset(igc_buf,0,50);
	sprintf(igc_buf,"IGC_unlogin %s",igc_user_login_name);  
	BdSms(igc_buf);
	return 1;
}
int  set_igc_cmd_control_point(int cmd_type,char * logicalname)
{
	if(cmd_type<0 || cmd_type>99)
	{
		printf("\nigc_cmd_control_point set ERR,cmd_type over range");
		return -1;
	}
	memset(igc_cmd_control_point[cmd_type],0,POINT_NAME_SIZE+12);
	strcpy(igc_cmd_control_point[cmd_type],logicalname);
    if(igc_cmd_control_point[cmd_type][POINT_NAME_SIZE+11]!=0)
	{
		printf("\nigc_cmd_control_point set ERR,%s\n",igc_cmd_control_point[cmd_type]);
		memset(igc_cmd_control_point[cmd_type],0,POINT_NAME_SIZE+12);
		return -1;
	}
 
	printf("\nigc_cmd_control_point set:%s\n",igc_cmd_control_point[cmd_type]);
	return 1;
}

int  clear_igc_cmd_control_point(int cmd_type)
{
	if(cmd_type<0 || cmd_type>99)
	{
		printf("\nigc_cmd_control_point clear ERR,cmd_type over range");
		return -1;
	}
	memset(igc_cmd_control_point[cmd_type],0,POINT_NAME_SIZE+12);
	printf("\nigc_cmd_control_point clear:%s\n",igc_cmd_control_point[cmd_type]);
	return 1;
}

int trans_cmdtype_to_priv(int cmdtype)
{
	int R=-1;
	/*switch(cmdtype)
	{
	case IGC_CMDTYPE_C_UNIT:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_BREAKER:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_SWITCH:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_APOWER:
		
		break;
	case IGC_CMDTYPE_C_RPOWER:
		
		break;
	case IGC_CMDTYPE_C_DEVICE:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_GATE:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_TRANS:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_VOLT_STEP:
		
		break;
	case IGC_CMDTYPE_C_SPEED_STEP:
		
		break;
	case IGC_CMDTYPE_C_ONOFF_FORCE:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_ANA_FORCE:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_IO_CTRL_BLOCK:
		
		break;
	case IGC_CMDTYPE_C_DEV_MODE:
		
		break;
	case IGC_CMDTYPE_C_SIGNAL_RESET:
		
		break;
	case IGC_CMDTYPE_C_LCU_ONOFF:
		
		break;
	case IGC_CMDTYPE_TIME_SYNC:
		R=PRIV_LCU_CTRL;
		break;
	case IGC_CMDTYPE_C_HOST_SYS:
		
		break;
	case IGC_CMDTYPE_C_PLC_SWITCH:
		
		break;
	case IGC_CMDTYPE_C_NET_SWITCH:
		
		break;
	case IGC_CMDTYPE_C_OP_VALUE_SET:
		
		break;
	case IGC_CMDTYPE_C_SYS_MODE_SET:
		
		break;
	case IGC_CMDTYPE_C_LCU_PARAM:
		
		break;
	}
	*/
	R=PRIV_LCU_CTRL;//LS
	return R;
}



int igc_SendCmd(int cmdtype,int operate_type,int sim_cmd,char * logicalname,float setvalue,char * interlockname_user)
//cmdtype 规约定义 命令类型
//sim_cmd 是滞仿真命令
{
	LAN_HEAD        head;
	COMMAND_MSG		msg;
	int				length;
	char			buf[COMMAND_SIZE];
	time_t     		re_time,newtime;
	struct  tm      *devtime;
	POINTER			point;
	LCU_ID          igc_lcu_id;
	char	        local_hostname[MAX_HOST_NAME_SIZE];
    H9000_IGC_data  can_send_cmd_flag;
	char			geted_interlockname[POINT_NAME_SIZE+12];
	int             outrangeflag;
    char            igc_sms_buf[200];
	int             igc_user_need_priv;
	memset(buf,0,sizeof(buf));
	memset(local_hostname,0,sizeof(local_hostname));
	memset(geted_interlockname,0,sizeof(geted_interlockname));

	igc_user_need_priv=trans_cmdtype_to_priv(cmdtype);
	if(igc_user_need_priv<0)
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,PRIV ERR");  
		BdSms(igc_sms_buf);
		
		printf("\nIGC:can not send cmd,please set \"user do not have priv\" first\n");
		return -1;
	}
	
	if(!CheckUserPrivilege(igc_user_login_name,igc_user_need_priv))//ls 用此权限
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,NO PRIV_LCU_CTRL");  
		BdSms(igc_sms_buf);
		
		printf("\nIGC:CMD FAULT,NO PRIV_LCU_CTRL\n");
		return -1;
	}


	if(IGC_GetCurentHostCtrlPrvg(0)!=1)
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,Host No CtrlPrvg");  
		BdSms(igc_sms_buf);	
		printf("\nIGC:CMD FAULT,Host No CtrlPrvg\n");
		return -1;
	}

	

	if(igc_cmd_control_point[cmdtype][0]==0)
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,no ctl pt");  
		BdSms(igc_sms_buf);

		printf("\nIGC:can not send cmd,please set \"igc_cmd_control_point\" first\n");
		return -1;
	}
	can_send_cmd_flag= get_data_value_by_logicalname(igc_cmd_control_point[cmdtype]);
	if(can_send_cmd_flag.ok_flag!=1)
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,CPT IS FAULT");  
		BdSms(igc_sms_buf);

		printf("\nIGC:can not send cmd,can not get \"igc_cmd_control_point\" value\n");
		return -1;
	}

	if(can_send_cmd_flag.chan_fail==1)
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,X_CHAN IS FAULT");  
		BdSms(igc_sms_buf);
		
		printf("\nIGC:can not send cmd, \"igc_cmd_control_point\" is chan_fail\n");
		return -1;
	}

	if(can_send_cmd_flag.value!=1)
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,CPT IS not 1");  
		BdSms(igc_sms_buf);

		printf("\nIGC:can not send cmd,\"igc_cmd_control_point\" value is not 1\n");
		return -1;
	}

	if(igc_Judge_interlock(interlockname_user,0)==0)
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,intlk(user) fault");  
		BdSms(igc_sms_buf);

		printf("\nIGC:can not send cmd,interlock(user) is fault\n");
		return -1;
	}


	strcpy(geted_interlockname,logicalname);
	strcat(geted_interlockname,".");
	outrangeflag=0;
	switch(cmdtype)
	{
	case    IGC_CMDTYPE_C_UNIT:
		if(operate_type<IGC_CMDTYPE_OPER_C_UNIT_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_UNIT_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_UNIT[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_UNIT[operate_type]);	
		break;
	case    IGC_CMDTYPE_C_BREAKER:
		if(operate_type<IGC_CMDTYPE_OPER_C_BREAKER_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_BREAKER_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_BREAKER[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_BREAKER[operate_type]);	
		break;
	case    	IGC_CMDTYPE_C_SWITCH:
		if(operate_type<IGC_CMDTYPE_OPER_C_SWITCH_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_SWITCH_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_SWITCH[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_SWITCH[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_APOWER:
		if(operate_type<IGC_CMDTYPE_OPER_C_APOWER_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_APOWER_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_APOWER[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_APOWER[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_RPOWER:
		if(operate_type<IGC_CMDTYPE_OPER_C_RPOWER_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_RPOWER_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_RPOWER[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_RPOWER[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_DEVICE:
		if(operate_type<IGC_CMDTYPE_OPER_C_DEVICE_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_DEVICE_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_DEVICE[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_DEVICE[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_GATE:
		if(operate_type<IGC_CMDTYPE_OPER_C_GATE_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_GATE_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_GATE[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_GATE[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_TRANS:
		if(operate_type<IGC_CMDTYPE_OPER_C_TRANS_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_TRANS_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_TRANS[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_TRANS[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_VOLT_STEP:
		if(operate_type<IGC_CMDTYPE_OPER_C_VOLT_STEP_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_VOLT_STEP_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_VOLT_STEP[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_VOLT_STEP[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_SPEED_STEP:
		if(operate_type<IGC_CMDTYPE_OPER_C_SPEED_STEP_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_SPEED_STEP_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_SPEED_STEP[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_SPEED_STEP[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_ONOFF_FORCE:
		if(operate_type<IGC_CMDTYPE_OPER_C_ONOFF_FORCE_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_ONOFF_FORCE_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_ONOFF_FORCE[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_ONOFF_FORCE[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_ANA_FORCE:
		if(operate_type<IGC_CMDTYPE_OPER_C_ANA_FORCE_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_ANA_FORCE_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_ANA_FORCE[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_ANA_FORCE[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_IO_CTRL_BLOCK:
		if(operate_type<IGC_CMDTYPE_OPER_C_IO_CTRL_BLOCK_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_IO_CTRL_BLOCK_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_IO_CTRL_BLOCK[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_IO_CTRL_BLOCK[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_DEV_MODE:
		if(operate_type<IGC_CMDTYPE_OPER_C_DEV_MODE_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_DEV_MODE_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_DEV_MODE[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_DEV_MODE[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_SIGNAL_RESET:
		if(operate_type<IGC_CMDTYPE_OPER_C_SIGNAL_RESET_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_SIGNAL_RESET_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_SIGNAL_RESET[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_SIGNAL_RESET[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_LCU_ONOFF:
		if(operate_type<IGC_CMDTYPE_OPER_C_LCU_ONOFF_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_LCU_ONOFF_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_LCU_ONOFF[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_LCU_ONOFF[operate_type]);
		break;
	case    	IGC_CMDTYPE_TIME_SYNC:
		if(operate_type<IGC_CMDTYPE_OPER_TIME_SYNC_start_NUM || operate_type>IGC_CMDTYPE_OPER_TIME_SYNC_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_TIME_SYNC[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_TIME_SYNC[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_HOST_SYS:
		if(operate_type<IGC_CMDTYPE_OPER_C_HOST_SYS_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_HOST_SYS_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_HOST_SYS[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_HOST_SYS[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_PLC_SWITCH:
		if(operate_type<IGC_CMDTYPE_OPER_C_PLC_SWITCH_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_PLC_SWITCH_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_PLC_SWITCH[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_PLC_SWITCH[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_NET_SWITCH:
		if(operate_type<IGC_CMDTYPE_OPER_C_NET_SWITCH_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_NET_SWITCH_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_NET_SWITCH[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_NET_SWITCH[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_OP_VALUE_SET:
		if(operate_type<IGC_CMDTYPE_OPER_C_OP_VALUE_SET_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_OP_VALUE_SET_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_OP_VALUE_SET[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_OP_VALUE_SET[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_SYS_MODE_SET:
		if(operate_type<IGC_CMDTYPE_OPER_C_SYS_MODE_SET_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_SYS_MODE_SET_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_SYS_MODE_SET[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_SYS_MODE_SET[operate_type]);
		break;
	case    	IGC_CMDTYPE_C_LCU_PARAM:
		if(operate_type<IGC_CMDTYPE_OPER_C_LCU_PARAM_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_LCU_PARAM_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_LCU_PARAM[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_LCU_PARAM[operate_type]);
		break;

	case    	IGC_CMDTYPE_C_DATA_POLLING:
		if(operate_type<IGC_CMDTYPE_OPER_C_DATA_POLLING_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_DATA_POLLING_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_DATA_POLLING[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_DATA_POLLING[operate_type]);
		break;


	case    	IGC_CMDTYPE_ANA_PARAM_SET:
		if(operate_type<IGC_CMDTYPE_OPER_ANA_PARAM_SET_start_NUM || operate_type>IGC_CMDTYPE_OPER_ANA_PARAM_SET_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_ANA_PARAM_SET[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_ANA_PARAM_SET[operate_type]);
		break;



	case    	IGC_CMDTYPE_IMP_PARAM_SET:
		if(operate_type<IGC_CMDTYPE_OPER_IMP_PARAM_SET_start_NUM || operate_type>IGC_CMDTYPE_OPER_IMP_PARAM_SET_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_IMP_PARAM_SET[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_IMP_PARAM_SET[operate_type]);
		break;


	case    	IGC_CMDTYPE_C_ONOFF_SET:
		if(operate_type<IGC_CMDTYPE_OPER_C_ONOFF_SET_start_NUM || operate_type>IGC_CMDTYPE_OPER_C_ONOFF_SET_stop_NUM )
		{
			outrangeflag=1;
			break;
		}
		else if(strcmp(IGC_CMDTYPE_OPER_C_ONOFF_SET[operate_type],"NULL")==0)
		{
			outrangeflag=1;
			break;
		}
		strcat(geted_interlockname,IGC_CMDTYPE_OPER_C_ONOFF_SET[operate_type]);
		break;
	default:
		printf("\nIGC:can not send cmd,cmd_type over range\n");
		return -1;
	}

	if(outrangeflag==1)
	{	 
		printf("\nIGC:can not send cmd,cmd_type:operate_type over range\n");
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,operatetype overrange");  
		BdSms(igc_sms_buf);
		return -1;
	}
 

	if(igc_Judge_interlock(geted_interlockname,0)==0)
	{
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,intlk(RDB) fault");  
		BdSms(igc_sms_buf);
		printf("\nIGC:can not send cmd,interlock(RDB) is fault\n");
		return -1;
	}

	if(sim_cmd == 1)
		head.dp_type=COMM_SIM;
	else 
		head.dp_type=COMM_TIME;

	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		return -1;
	}
		
	igc_lcu_id.stn_id= point.stn_id;
	igc_lcu_id.dev_id= point.dev_id;
	gethostname(local_hostname,sizeof(local_hostname));
	if(!CheckHostPrivilege(local_hostname, igc_lcu_id))
	{
		printf("\nIGC:can not send cmd,local_hostname do not have Privilege to sendcmd\n");
		memset(igc_sms_buf,0,200);
		sprintf(igc_sms_buf,"IGC CMD FAULT,hostname no Privilege");  
		BdSms(igc_sms_buf);
		return -1;
	}

	msg.type_id = cmdtype;
 
	msg.stn_id = point.stn_id;
	msg.dev_id = point.dev_id;
	msg.data_type = point.data_type;	
	msg.point_id = point.pt_id;
	msg.status.opr = operate_type;
	msg.status.result = 0;
	msg.status.state = 0;
	msg.status.src = 0;
 
	msg.data.iValue = (int)(setvalue*1000);
	
	GetSrcId(&msg.host_id);
	msg.usr_id=0;
	strcpy(msg.usr_login_name,"ems");
	
	switch(cmdtype)
	{
	case    IGC_CMDTYPE_TIME_SYNC:
	case    IGC_CMDTYPE_C_HOST_SYS:
	case    IGC_CMDTYPE_C_PLC_SWITCH:
	case    IGC_CMDTYPE_C_NET_SWITCH:
	case    IGC_CMDTYPE_C_OP_VALUE_SET://区分
	case    IGC_CMDTYPE_C_SYS_MODE_SET:
	case    IGC_CMDTYPE_C_DATA_POLLING:
	case    IGC_CMDTYPE_C_LCU_PARAM:
	case    IGC_CMDTYPE_ANA_PARAM_SET:
	case    IGC_CMDTYPE_IMP_PARAM_SET:
	case    IGC_CMDTYPE_C_ONOFF_SET:
		msg.ctrl_addr =0;
		break;
		
		
	default :
		if(GetPtCtrlAddrById(&point, &msg.ctrl_addr ) == -1)
		{
			printf("\nIGC:can not send cmd,get cmd CtrlAddress eror\n");
			return -1;
		}
		break;
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
	
	memset(igc_sms_buf,0,200);
    sprintf(igc_sms_buf,"IGC_CMD %d.%d.%d.%d T=%d-%d",point.stn_id,point.dev_id,point.data_type,point.pt_id,cmdtype,operate_type);  
	BdSms(igc_sms_buf);
	head.dp_type=BJ_DB_TIME; 
	lan_out((char *)&msg,head,0);
	return 1;
}


int igc_Judge_interlock(char * interlockname,int type)
{
	if(!interlockname)
		return 0;
	
	if(strlen((const char * )interlockname)==0)
		return 0;

	return InterLock_Judge(interlockname);
}

void igc_InterLockFree()
{
	InterLockFree();
}

int igc_UnMountDB()
{
	return UnMountDB();
}

int igc_InterLockFileLoad()
{
	return InterLockFileLoad();
}
int get_CLK_TCK()
{
	int igc_CLK_TCK=CLK_TCK;
	return igc_CLK_TCK;
}
int igc_MountDB()//init
{
	
	igc_user_login_name[20];
	memset(igc_user_login_name,0,sizeof(igc_user_login_name));
	memset(igc_cmd_control_point,0,sizeof(igc_cmd_control_point));
	LoadUserPrivilegeDatabase( );

	return MountDB(NULL);
}


int igc_WSAStartup()
{
#ifdef	WINDOWS_OS
	WSADATA		WSAData;
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		return -1;
	}
	else
		return 0;
#endif
	return 0;
}

int igc_lan_out_init()
{
	if(!lan_out_init())
		return  -1;
	else
		return 0;
}

/*void igc_SetModuleName(char *module)
{
	SetModuleName(module);
}*/

int	 igc_SetErrLogMode(int argc, char *argv[])
{
	CreateLockFile(".igc.lock");
	if(-1==InitSysLog(H9000_MODULE_IGC, "igc"))
	{
		Err_log("igc:Fail to InitSysLog!\n");
		exit(-1);
	}
	return -1;
}

/*void igc_CreateLockFile(char *filename )
{
	CreateLockFile(filename );
}*/

void igc_Err_log( char *info_buf )
{
	Err_log(info_buf);
}

int set_pt_to_doublePt_by_pointid(unsigned short stnid,unsigned short devid,unsigned short datatype_id,unsigned short ptid,unsigned short stnid_P,unsigned short devid_P,unsigned short datatype_id_P,unsigned short ptid_P,int isdoublept)
{
	POINTER pnt;
	POINTER pnt_P;
	IOATTR	ioattr={0};
	int rtn;
 


	ioattr.doublePt=isdoublept;
	pnt.stn_id=(UCHAR)stnid;
	pnt.dev_id=(UCHAR)devid;
	pnt.data_type=(UCHAR)datatype_id;
	pnt.pt_id=ptid;
	
	pnt_P.stn_id=(UCHAR)stnid_P;
	pnt_P.dev_id=(UCHAR)devid_P;
	pnt_P.data_type=(UCHAR)datatype_id_P;
	pnt_P.pt_id=ptid_P;


	if(isdoublept==1)
	{
		rtn=SetPairPtById(&pnt,pnt_P);
		if(rtn==-1)
			return rtn;
		rtn=SetPairPtById(&pnt_P,pnt);
		if(rtn==-1)
			return rtn;
	}

	rtn=SetPtIoattrById(&pnt, ioattr, RW_DOUBLE_PT );
	
	if(rtn==-1)
		return rtn;
	
	rtn=SetPtIoattrById(&pnt_P, ioattr, RW_DOUBLE_PT );
	
	return rtn;

}
 
int if_point_is_Doublept_By_pointid(unsigned short stnid,unsigned short devid,unsigned short datatype_id,unsigned short ptid)
{
	POINTER pnt;
	BOOL isdoublept=0;
	
	pnt.stn_id=(UCHAR)stnid;
	pnt.dev_id=(UCHAR)devid;
	pnt.data_type=(UCHAR)datatype_id;
	pnt.pt_id=ptid;
	
	if(IsPtDoublePointById(&pnt, &isdoublept)==-1)
		return 0;
	else
		return (int)isdoublept;
}
int  get_iogroup_by_pointid(unsigned short stnid,unsigned short devid,unsigned short datatype_id,unsigned short ptid)
{
	
	POINTER pnt;
	unsigned short grpid=65535;
	
	pnt.stn_id=(UCHAR)stnid;
	pnt.dev_id=(UCHAR)devid;
	pnt.data_type=(UCHAR)datatype_id;
	pnt.pt_id=ptid;
	
	if(GetPtIoGroupById(&pnt, &grpid)==-1)
		return -1;
	else
		return (int)grpid;

}

int  set_iogroup_by_pointid(unsigned short stnid,unsigned short devid,unsigned short datatype_id,unsigned short ptid,unsigned short io_group_id)
{
	
	POINTER pnt;
	
	pnt.stn_id=(UCHAR)stnid;
	pnt.dev_id=(UCHAR)devid;
	pnt.data_type=(UCHAR)datatype_id;
	pnt.pt_id=ptid;
	
	return SetPtIoGroupById(&pnt,io_group_id);	
}


void sleep_millisecond_igclib(int millisecond)
{
	if(millisecond<10)
		millisecond=10;
	if(millisecond>500)
		millisecond=500;
	Msleep(millisecond);
}

void sleep_second_igclib(int second)
{
	if(second<1)
		second=1;
	if(second>100)
		second=100;
	sleep(second);
//	Msleep(second*1000);
}


IGC_datetime get_lacal_datetime_now()
{
	struct tm 	 *today;
	time_t		 ltime;
	IGC_datetime R_IGC_datetime;

	ltime=time(NULL);
	today=localtime(&ltime);
	
	R_IGC_datetime.day=today->tm_mday;
	
	R_IGC_datetime.hour=today->tm_hour;
	
	R_IGC_datetime.minute=today->tm_min;
	
	R_IGC_datetime.month=today->tm_mon+1;
	
	R_IGC_datetime.second=today->tm_sec;
	
	R_IGC_datetime.year=today->tm_year+1900;
	
	return R_IGC_datetime;
}


IGC_datetime get_datetime_by_time_t(time_t ltime)
{
	struct tm 	 *today;
	IGC_datetime R_IGC_datetime;

	today=localtime(&ltime);
	
	R_IGC_datetime.day=today->tm_mday;
	
	R_IGC_datetime.hour=today->tm_hour;
	
	R_IGC_datetime.minute=today->tm_min;
	
	R_IGC_datetime.month=today->tm_mon+1;
	
	R_IGC_datetime.second=today->tm_sec;
	
	R_IGC_datetime.year=today->tm_year+1900;
	
	return R_IGC_datetime;
}

long total_time_seconds(IGC_datetime end_time,IGC_datetime start_time)//end_time-starttime
{
	long        R_seconds=0;
	struct tm 	  *s_time;
	time_t		  s_t_ime;

	struct tm 	  *e_time;
	time_t		  e_t_ime;
	
	s_t_ime=time(NULL);
	s_time=localtime(&s_t_ime);

	s_time->tm_hour=start_time.hour;
	s_time->tm_mday=start_time.day;
	s_time->tm_min=start_time.minute;
	s_time->tm_mon=start_time.month-1;
	s_time->tm_sec=start_time.second;

	s_time->tm_year=start_time.year-1900;
	
	s_t_ime=mktime (s_time);

	e_t_ime=time(NULL);
	e_time=localtime(&e_t_ime); 

	e_time->tm_hour=end_time.hour;
	e_time->tm_mday=end_time.day;
	e_time->tm_min=end_time.minute;
	e_time->tm_mon=end_time.month-1;
	e_time->tm_sec=end_time.second;

	e_time->tm_year=end_time.year-1900;
	
	e_t_ime=mktime (e_time);
	

	R_seconds=e_t_ime-s_t_ime;
//	if(R_seconds<0)
	//	R_seconds=0-R_seconds;

	return R_seconds;
}

H9000_IGC_data get_data_value_by_logicalname(char * logicalname)
{
	POINTER  point;
	H9000_IGC_data R_data;
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		printf("\nget_data_value_by_logicalname,logicalname ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.value=0;
		return R_data;
	}
	get_data_value_by_pointid_IN(point,&R_data);
  	return R_data;
}

H9000_IGC_data get_data_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int POINT_ID)
{
	H9000_IGC_data R_data;
	POINTER point;
	point.update_src=0;
	point.stn_id=station_ID;
	point.dev_id=DEVICE_ID;
	point.data_type=TTPE_ID;
	point.pt_id=POINT_ID;
	get_data_value_by_pointid_IN(point,&R_data);
	return R_data;
}

void get_data_value_by_pointid_IN(POINTER point,H9000_IGC_data * R_data)
{
	DMS_COMMON  common;
	short	 sta_ioattr,state;
	int      almsel;
	
	R_data->ok_flag=1;
	R_data->chan_fail=0;
	if(-1==ReadEntryById(&point, &common))
	{
		R_data->ok_flag=-1;
		R_data->chan_fail=1;
		R_data->value=0;
		R_data->int_value=(int)R_data->value;
	}
	if(R_data->ok_flag!=-1)
	{
		if(point.data_type ==SOE_TYPE ||  point.data_type ==POL_TYPE || point.data_type ==OUTS_TYPE || point.data_type ==SYSMSG_TYPE)
		{
			sta_ioattr = common.fixed.ioattr.procType ;
			state = common.var.ind.status.state;
			almsel = common.fixed.msg_sel.alm;
			R_data->chan_fail=common.var.ind.status.chan_fail;
			R_data->chan_fail=R_data->chan_fail | common.var.ind.status.res3;//20110830
	
			R_data->value=(float)state;
			R_data->int_value=(int)R_data->value;

			R_data->update_time.day=common.var.ind.update_time.day;
			R_data->update_time.hour=common.var.ind.update_time.hour;
			R_data->update_time.minute=common.var.ind.update_time.minute;
			R_data->update_time.month=common.var.ind.update_time.month;
			R_data->update_time.second=common.var.ind.update_time.second;
			R_data->update_time.year=common.var.ind.update_time.year;


			R_data->change_time.day=common.var.ind.change_time.day;
			R_data->change_time.hour=common.var.ind.change_time.hour;
			R_data->change_time.minute=common.var.ind.change_time.minute;
			R_data->change_time.month=common.var.ind.change_time.month;
			R_data->change_time.second=common.var.ind.change_time.second;
			R_data->change_time.year=common.var.ind.change_time.year;
		}
		else if( (point.data_type ==ANA_TYPE)|(point.data_type ==OUTA_TYPE)|(point.data_type ==PARAM_TYPE) )
		{
			R_data->value = common.var.anlg.fValue;	
			R_data->int_value=(int)R_data->value;

			R_data->chan_fail=common.var.anlg.value.status.chan_fail;
			R_data->chan_fail=R_data->chan_fail | common.var.anlg.value.status.overflow;//20110830

			R_data->update_time.day=common.var.anlg.update_time.day;
			R_data->update_time.hour=common.var.anlg.update_time.hour;
			R_data->update_time.minute=common.var.anlg.update_time.minute;
			R_data->update_time.month=common.var.anlg.update_time.month;
			R_data->update_time.second=common.var.anlg.update_time.second;
			R_data->update_time.year=common.var.anlg.update_time.year;
			
			
			R_data->change_time.day=common.var.anlg.update_time.day;
			R_data->change_time.hour=common.var.anlg.update_time.hour;
			R_data->change_time.minute=common.var.anlg.update_time.minute;
			R_data->change_time.month=common.var.anlg.update_time.month;
			R_data->change_time.second=common.var.anlg.update_time.second;
			R_data->change_time.year=common.var.anlg.update_time.year;
		}
		else if (point.data_type ==IMP_TYPE)
		{
			R_data->value =  common.var.imp.entry.d_accum;
			R_data->int_value=(int)R_data->value;
			R_data->chan_fail=common.var.imp.entry.status.chan_fail;
			R_data->chan_fail=R_data->chan_fail | common.var.imp.entry.status.res1;//20110830

			R_data->update_time.day=common.var.imp.entry.update_time.day;
			R_data->update_time.hour=common.var.imp.entry.update_time.hour;
			R_data->update_time.minute=common.var.imp.entry.update_time.minute;
			R_data->update_time.month=common.var.imp.entry.update_time.month;
			R_data->update_time.second=common.var.imp.entry.update_time.second;
			R_data->update_time.year=common.var.imp.entry.update_time.year;
			
			R_data->change_time.day=common.var.imp.entry.update_time.day;
			R_data->change_time.hour=common.var.imp.entry.update_time.hour;
			R_data->change_time.minute=common.var.imp.entry.update_time.minute;
			R_data->change_time.month=common.var.imp.entry.update_time.month;
			R_data->change_time.second=common.var.imp.entry.update_time.second;
			R_data->change_time.year=common.var.imp.entry.update_time.year;
		}
		else if (point.data_type ==CHARA_TYPE)
		{



			R_data->value = common.var.chara.chara_data.avg_value;
			R_data->int_value=(int)R_data->value;
			R_data->chan_fail=0;

			R_data->update_time.day=common.var.chara.update_time.day;
			R_data->update_time.hour=common.var.chara.update_time.hour;
			R_data->update_time.minute=common.var.chara.update_time.minute;
			R_data->update_time.month=common.var.chara.update_time.month;
			R_data->update_time.second=common.var.chara.update_time.second;
			R_data->update_time.year=common.var.chara.update_time.year;
			
			R_data->change_time.day=common.var.chara.update_time.day;
			R_data->change_time.hour=common.var.chara.update_time.hour;
			R_data->change_time.minute=common.var.chara.update_time.minute;
			R_data->change_time.month=common.var.chara.update_time.month;
			R_data->change_time.second=common.var.chara.update_time.second;
			R_data->change_time.year=common.var.chara.update_time.year;
		}
		else if (point.data_type ==CALC_TYPE)
		{
			R_data->value = common.var.anlg.fValue;
			R_data->int_value=(int)R_data->value;

			R_data->chan_fail=common.var.calc.value.status.chan_fail;
			R_data->chan_fail=R_data->chan_fail | common.var.calc.value.status.overflow;//20110830

			R_data->update_time.day=common.var.calc.update_time.day;
			R_data->update_time.hour=common.var.calc.update_time.hour;
			R_data->update_time.minute=common.var.calc.update_time.minute;
			R_data->update_time.month=common.var.calc.update_time.month;
			R_data->update_time.second=common.var.calc.update_time.second;
			R_data->update_time.year=common.var.calc.update_time.year;
			
			R_data->change_time.day=common.var.calc.update_time.day;
			R_data->change_time.hour=common.var.calc.update_time.hour;
			R_data->change_time.minute=common.var.calc.update_time.minute;
			R_data->change_time.month=common.var.calc.update_time.month;
			R_data->change_time.second=common.var.calc.update_time.second;
			R_data->change_time.year=common.var.calc.update_time.year;
		}
		else 
		{
			R_data->ok_flag=-1;
			R_data->value=0;
			R_data->int_value=(int)R_data->value;
			R_data->chan_fail=1;
		}
	}
}


H9000_IGC_CHARA_data  get_chara_data_value_by_logicalname(char * logicalname)
{
	
	POINTER  point;
	H9000_IGC_CHARA_data R_data;
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		printf("\nget_chara_data_value_by_logicalname,logicalname ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.avg_value=0;
		R_data.max_value=0;
		R_data.min_value=0;
		R_data.max_rate=0;
		R_data.min_rate=0;
		return R_data;
	}
	
	if(point.data_type !=CHARA_TYPE)
	{
		printf("\nget_ind_data_value_by_logicalname,type ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.avg_value=0;
		R_data.max_value=0;
		R_data.min_value=0;
		R_data.max_rate=0;
		R_data.min_rate=0;
		
		return R_data;
	}
	get_chara_data_value_by_pointid_IN(point,&R_data);
  	return R_data;
}
H9000_IGC_CHARA_data get_chara_data_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int POINT_ID)
{
	H9000_IGC_CHARA_data R_data;
	POINTER point;
	point.update_src=0;
	point.stn_id=station_ID;
	point.dev_id=DEVICE_ID;
	point.data_type=TTPE_ID;
	point.pt_id=POINT_ID;
	
	if(TTPE_ID !=CHARA_TYPE)
	{
		printf("\nget_chara_data_value_by_pointid,type ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.avg_value=0;
		R_data.max_value=0;
		R_data.min_value=0;
		R_data.max_rate=0;
		R_data.min_rate=0;
		
		return R_data;
	}
	get_chara_data_value_by_pointid_IN(point,&R_data);
	return R_data;
}
void get_chara_data_value_by_pointid_IN(POINTER point,H9000_IGC_CHARA_data * R_data)
{
	DMS_COMMON  common;

	
	R_data->ok_flag=1;
	R_data->chan_fail=0;
	if(-1==ReadEntryById(&point, &common))
	{
		R_data->ok_flag=-1;
		R_data->chan_fail=1;
		R_data->avg_value=0;
		R_data->max_value=0;
		R_data->min_value=0;
		R_data->max_rate=0;
		R_data->min_rate=0;
	}
	if(R_data->ok_flag!=-1)
	{
		if (point.data_type ==CHARA_TYPE)
		{
			R_data->chan_fail=0;
			
			R_data->update_time.day=common.var.chara.update_time.day;
			R_data->update_time.hour=common.var.chara.update_time.hour;
			R_data->update_time.minute=common.var.chara.update_time.minute;
			R_data->update_time.month=common.var.chara.update_time.month;
			R_data->update_time.second=common.var.chara.update_time.second;
			R_data->update_time.year=common.var.chara.update_time.year;
			
		
			
			R_data->max_no=common.var.chara.chara_data.max_no;
			R_data->min_no=common.var.chara.chara_data.min_no;
			R_data->max_rate_no=common.var.chara.chara_data.max_rate_no;
			R_data->min_rate_no=common.var.chara.chara_data.min_rate_no;
			R_data->max_value=common.var.chara.chara_data.max_value;
			R_data->min_value=common.var.chara.chara_data.min_value;
			R_data->avg_value=common.var.chara.chara_data.avg_value;
			R_data->std_value=common.var.chara.chara_data.std_value;
			R_data->max_rate=common.var.chara.chara_data.max_rate;
			R_data->min_rate=common.var.chara.chara_data.min_rate;

			R_data->ok_flag=1;
		 
		}
		else 
		{
			R_data->ok_flag=0;
			R_data->chan_fail=1;
			R_data->avg_value=0;
			R_data->max_value=0;
			R_data->min_value=0;
			R_data->max_rate=0;
			R_data->min_rate=0;
		}
	}
}



H9000_IGC_IND_data get_ind_data_value_by_logicalname(char * logicalname)
{
	POINTER  point;
	H9000_IGC_IND_data R_data;
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		printf("\nget_ind_data_value_by_logicalname,logicalname ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.value=0;
		return R_data;
	}
	
	if(point.data_type !=SOE_TYPE &&  point.data_type !=POL_TYPE && point.data_type !=OUTS_TYPE && point.data_type !=SYSMSG_TYPE)
	{
		printf("\nget_ind_data_value_by_logicalname,type ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.value=0;
		
		return R_data;
	}
	get_ind_data_value_by_pointid_IN(point,&R_data);
  	return R_data;
}

H9000_IGC_IND_data get_ind_data_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int POINT_ID)
{
	H9000_IGC_IND_data R_data;
	POINTER point;
	point.update_src=0;
	point.stn_id=station_ID;
	point.dev_id=DEVICE_ID;
	point.data_type=TTPE_ID;
	point.pt_id=POINT_ID;

	if(TTPE_ID !=SOE_TYPE &&  TTPE_ID !=POL_TYPE && TTPE_ID !=OUTS_TYPE && TTPE_ID !=SYSMSG_TYPE)
	{
		printf("\nget_ind_data_value_by_pointid,type ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.value=0;
		
		return R_data;
	}
	get_ind_data_value_by_pointid_IN(point,&R_data);
	return R_data;
}

void get_ind_data_value_by_pointid_IN(POINTER point,H9000_IGC_IND_data * R_data)
{
	DMS_COMMON  common;
	short	 sta_ioattr,state;
	int      almsel;
	
	R_data->ok_flag=1;
	R_data->chan_fail=0;
	if(-1==ReadEntryById(&point, &common))
	{
		R_data->ok_flag=-1;
		R_data->chan_fail=1;
		R_data->value=0;
	 
	}
	if(R_data->ok_flag!=-1)
	{
		if(point.data_type ==SOE_TYPE ||  point.data_type ==POL_TYPE || point.data_type ==OUTS_TYPE || point.data_type ==SYSMSG_TYPE)
		{
			sta_ioattr = common.fixed.ioattr.procType ;
			state = common.var.ind.status.state;
			almsel = common.fixed.msg_sel.alm;
			R_data->chan_fail=common.var.ind.status.chan_fail;
			R_data->chan_fail=R_data->chan_fail | common.var.ind.status.res3;//20110830
	
			R_data->value=(int)state;
		 
			R_data->update_time.day=common.var.ind.update_time.day;
			R_data->update_time.hour=common.var.ind.update_time.hour;
			R_data->update_time.minute=common.var.ind.update_time.minute;
			R_data->update_time.month=common.var.ind.update_time.month;
			R_data->update_time.second=common.var.ind.update_time.second;
			R_data->update_time.year=common.var.ind.update_time.year;


			R_data->change_time.day=common.var.ind.change_time.day;
			R_data->change_time.hour=common.var.ind.change_time.hour;
			R_data->change_time.minute=common.var.ind.change_time.minute;
			R_data->change_time.month=common.var.ind.change_time.month;
			R_data->change_time.second=common.var.ind.change_time.second;
			R_data->change_time.year=common.var.ind.change_time.year;
		}
		else 
		{
			R_data->ok_flag=-1;
			R_data->value=0;
	 
			R_data->chan_fail=1;
		}
	}
}

H9000_IGC_ANA_data get_ana_data_value_by_logicalname(char * logicalname)
{
	POINTER  point;
	H9000_IGC_ANA_data R_data;
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.value=0;

		return R_data;
	}
	if(point.data_type !=ANA_TYPE &&  point.data_type !=OUTA_TYPE && point.data_type !=PARAM_TYPE )
	{
		printf("\nget_ana_data_value_by_logicalname,type ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.value=0;
		
		return R_data;
	}

	get_ana_data_value_by_pointid_IN(point,&R_data);
  	return R_data;
}

H9000_IGC_ANA_data get_ana_data_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int POINT_ID)
{
	H9000_IGC_ANA_data R_data;
	POINTER point;
	point.update_src=0;
	point.stn_id=station_ID;
	point.dev_id=DEVICE_ID;
	point.data_type=TTPE_ID;
	point.pt_id=POINT_ID;

	
	if(point.data_type !=ANA_TYPE &&  point.data_type !=OUTA_TYPE && point.data_type !=PARAM_TYPE )
	{
		printf("\nget_ana_data_value_by_pointid,type ERR\n");
		R_data.ok_flag=0;
		R_data.chan_fail=1;
		R_data.value=0;

		return R_data;
	}


	get_ana_data_value_by_pointid_IN(point,&R_data);
	return R_data;
}

void get_ana_data_value_by_pointid_IN(POINTER point,H9000_IGC_ANA_data * R_data)
{
	DMS_COMMON  common;
  
	
	R_data->ok_flag=1;
	R_data->chan_fail=0;
	if(-1==ReadEntryById(&point, &common))
	{
		R_data->ok_flag=-1;
		R_data->chan_fail=1;
		R_data->value=0;
		R_data->int_value=(int)R_data->value;
	}
	if(R_data->ok_flag!=-1)
	{
	    if( (point.data_type ==ANA_TYPE)|(point.data_type ==OUTA_TYPE)|(point.data_type ==PARAM_TYPE) )
		{
			R_data->value = common.var.anlg.fValue;	
			R_data->int_value=(int)R_data->value;

			R_data->chan_fail=common.var.anlg.value.status.chan_fail;
			R_data->chan_fail=R_data->chan_fail | common.var.anlg.value.status.overflow;//20110830


			R_data->D_BAND=common.var.anlg.eng_para.D_BAND;
			R_data->HH_LMT=common.var.anlg.lmt_para.HI_ALARM;  
			R_data->H_LMT=common.var.anlg.lmt_para.HI_WARN;   		
			R_data->L_LMT=common.var.anlg.lmt_para.LO_WARN;   
			R_data->LL_LMT=common.var.anlg.lmt_para.LO_ALARM;   
			R_data->HR_LMT=common.var.anlg.lmt_para.HI_RECOVER;    
			R_data->LR_LMT=common.var.anlg.lmt_para.LO_RECOVER;   
			R_data->ENG_CVT=common.var.anlg.eng_para.ENG_CVT;	 
			R_data->HI_RANGE=common.var.anlg.eng_para.HI_RANGE;   
			R_data->LO_RANGE=common.var.anlg.eng_para.LO_RANGE;	 
			R_data->ZERO=common.var.anlg.eng_para.ZERO;     
			R_data->COMPENSATE=common.var.anlg.eng_para.COMPSATE;
			R_data->TR_LIM=common.var.anlg.lmt_para.TR_WARN;    
			R_data->RT_LIM=common.var.anlg.lmt_para.TR_RECOVER;    

			R_data->update_time.day=common.var.anlg.update_time.day;
			R_data->update_time.hour=common.var.anlg.update_time.hour;
			R_data->update_time.minute=common.var.anlg.update_time.minute;
			R_data->update_time.month=common.var.anlg.update_time.month;
			R_data->update_time.second=common.var.anlg.update_time.second;
			R_data->update_time.year=common.var.anlg.update_time.year;
			
			
			R_data->change_time.day=common.var.anlg.update_time.day;
			R_data->change_time.hour=common.var.anlg.update_time.hour;
			R_data->change_time.minute=common.var.anlg.update_time.minute;
			R_data->change_time.month=common.var.anlg.update_time.month;
			R_data->change_time.second=common.var.anlg.update_time.second;
			R_data->change_time.year=common.var.anlg.update_time.year;
		}
		else 
		{
			R_data->ok_flag=-1;
			R_data->value=0;
			R_data->int_value=(int)R_data->value;
			R_data->chan_fail=1;

			R_data->D_BAND=0;
			R_data->HH_LMT=0;
			R_data->H_LMT=0; 		
			R_data->L_LMT=0;
			R_data->LL_LMT=0;
			R_data->HR_LMT=0;
			R_data->LR_LMT=0; 
			R_data->ENG_CVT=0;
			R_data->HI_RANGE=0;
			R_data->LO_RANGE=0;
			R_data->ZERO=0; 
			R_data->COMPENSATE=0;
			R_data->TR_LIM=0;
			R_data->RT_LIM=0;
			
		}
	}
}
void MultiPointPack_set_ind(UCHAR type_id,UCHAR stn,UCHAR dev,UCHAR type,unsigned short first,unsigned short end,unsigned short status,int data,int smsfalg)
{
	LAN_HEAD	head;
	COMMON_MSG  msg;
	short		num;
	DATE_TIME	devtime;
	unsigned short ii=0;
	char 		buf[MAX_BUF_SIZE]; 
	H9000_IGC_data tempigcdt;
	POINTER temppt;
	int chgflagforsend;
  
	if(type_id !=  DPS_STATUS_ALL)
		return;
	
	chgflagforsend=0;
	
	temppt.stn_id=stn;
	temppt.dev_id=dev;
	temppt.data_type=type;
	for(ii=first;ii<=end;ii++)
	{
		temppt.pt_id=ii;
		get_data_value_by_pointid_IN(temppt,&tempigcdt);
		if(tempigcdt.ok_flag==1)
		{
			//if(tempigcdt.chan_fail==1){}
			if(tempigcdt.int_value!=data)
			{
				chgflagforsend++;
				break;
			}
		}
		else
		{
			IGC_SEND_H9000_SMG("IGC set ind dt err,cannot get dt");
			printf("\nIGC MultiPointPack_set_ind err,can not get data value");
			return ;	
		}
	}
	if(chgflagforsend==0)
	{
/*#ifdef DEBUG
		printf("\nind dt not change,vill not senddt");
#endif*/
		return;
	}


	memset(buf,0,MAX_BUF_SIZE);

	sprintf(buf,"IGC_SET %d.%d.%d.%d N=%d",stn,dev,type,first,end-first+1);  
	
	if(smsfalg==1)
		BdSms(buf);
	
	head.dest_id=0;
	GetSrcId(&head.src_id);
	head.dp_type=BJ_DB_TIME;
	
	memset(buf,0,MAX_BUF_SIZE);
	
	GetSysTime(&devtime);
	
	ii= first;
	for(;ii<=end;)
	{
		msg.type_id = type_id;
		msg.stn_id = stn;	
		msg.dev_id = dev;
		msg.data_type=type;
		GetSrcId(&msg.host_id);
		msg.point_id=ii;
		Swap2Byte(&msg.point_id);	
	 
		msg.ms  =devtime.ms;
		msg.sec  =devtime.second;
		msg.min  =devtime.minute;
		msg.hour  =devtime.hour;
		msg.day  =devtime.day;
		msg.month  =devtime.month;
		msg.year  =devtime.year; 
		
		head.length=8;
		num=0;
		for(;ii<=end;ii++,num++)
		{
			if(head.length>=(MAX_BUF_SIZE-18))
			{
				printf(" length >MAX_BUF_SIZE\n");
				break;
			}		
			
			memcpy(buf+head.length,&status,2);
			Swap2Byte(&buf[head.length]);
			
			head.length=head.length+2;
		}	
		
		msg.number=num;
		
		
		Swap2Byte(&msg.number);	
		Swap2Byte(&msg.year);
		
		memcpy(buf,&msg,8);
		
		memcpy(buf+head.length,&msg.host_id, 8);
		head.length=head.length+8;	
		
		Swap2Byte(&head.length);
	 
		head.dp_type=ALL_DB_TIME;
		lan_out((char *)buf,head,0);	
	}
	return;
}



void MultiPointPack_ALM(UCHAR type_id,UCHAR stn,UCHAR dev,UCHAR type,unsigned short first,unsigned short end,unsigned short status,int data,int almfag,int smsfalg)
{
	LAN_HEAD	head;
	COMMON_MSG  msg;
	short		num;
	DATE_TIME	devtime;
	unsigned short ii=0;
	char 		buf[MAX_BUF_SIZE]; 
	

	H9000_IGC_data tempigcdt;
	POINTER temppt;
	int chgflagforsend;


	memset(buf,0,MAX_BUF_SIZE);

	if(almfag==1)
	{
		sprintf(buf,"IGC_ALM %d.%d.%d.%d N=%d",stn,dev,type,first,end-first+1);  
	}
	else
	{
		chgflagforsend=0;	
		temppt.stn_id=stn;
		temppt.dev_id=dev;
		temppt.data_type=type;
		for(ii=first;ii<=end;ii++)
		{
			temppt.pt_id=ii;
			get_data_value_by_pointid_IN(temppt,&tempigcdt);
			if(tempigcdt.ok_flag==1)
			{
				//if(tempigcdt.chan_fail==1){}
				if(tempigcdt.value*1000-(float)data<0.0001 && tempigcdt.value*1000-(float)data>-0.0001)//转换误差
				{
				
				}
				else
				{
					chgflagforsend++;
					break;
				}
			}
			else
			{
				IGC_SEND_H9000_SMG("IGC alm dt err,cannot get dt");
				printf("\nIGC MultiPointPack_ALM err,can not get data value");
				return ;	
			}
		}
		if(chgflagforsend==0)
		{
/*#ifdef DEBUG
			printf("\n dt not change,vill not senddt");
#endif*/
			return;
		}
		sprintf(buf,"IGC_SET %d.%d.%d.%d N=%d",stn,dev,type,first,end-first+1);
	}


	if(smsfalg==1)
		BdSms(buf);

	head.dest_id=0;
	GetSrcId(&head.src_id);
	head.dp_type=BJ_DB_TIME;
	
	memset(buf,0,MAX_BUF_SIZE);
	
	GetSysTime(&devtime);
	
	ii= first;
	for(;ii<=end;)
	{
		msg.type_id = type_id;
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
	
		
		Swap2Byte(&msg.number);	
		Swap2Byte(&msg.ms);
		Swap2Byte(&msg.year);
		
		memcpy(buf,&msg,6);
		if(type_id !=  DPS_IND_ALM)
		{
			memcpy(buf+head.length,&msg.host_id, 8);	
			head.length=head.length+8;
		}
		else
		{	
			memcpy(buf+head.length,&msg.ms, 10);
			head.length=head.length+10;
		}

		Swap2Byte(&head.length);
		head.dp_type=BJ_DB_TIME; 
		lan_out((char *)buf,head,0);
		
	}
	return;
}


int set_ind_value_by_logicalname(char * logicalname,int set_value,int stand_by,int MAN_SET,int CHAN_FAIL,int sendsms_falg)
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	unsigned short status;
	char sendbuf[200];
	if(set_value<0)
		set_value=0;
	if(set_value>15)
		set_value=15;
	
	status=(unsigned short)set_value;
	
	
	
/*	if(DATA_QUALITY>=0 && DATA_QUALITY<=1)
		status=status | (DATA_QUALITY*0x2000);*/
	
	if(MAN_SET>=0 && MAN_SET<=1)
		status=status | (MAN_SET*0x4000);
	
	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);
	
	if(MAN_SET>=0 && MAN_SET<=1)
		status = status + 512;
	else if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status = status + 768;
//	else 
	//	status = status + 256;
	
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		sprintf(sendbuf,"IGCSET_ERR,NO POINT %s",logicalname);
	    BdSms(sendbuf);
		return -1;
	}
	
	istnid=(UCHAR)point.stn_id;
	idevid=(UCHAR)point.dev_id;
	itypeid=(UCHAR)point.data_type;

	if(itypeid !=SOE_TYPE &&  itypeid !=POL_TYPE && itypeid !=OUTS_TYPE && itypeid !=SYSMSG_TYPE)
	{
		printf("\nset_ind_value_by_logicalname,type ERR\n");
		return -1;
	}

	istartid=(unsigned short)point.pt_id;
	iendid=istartid;
	
	MultiPointPack_set_ind(DPS_STATUS_ALL,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)status,sendsms_falg);

	return 1;
}
int set_ana_value_by_logicalname(char * logicalname,float set_value,int stand_by,int MAN_SET,int CHAN_FAIL,int data_overflow,int sendsms_falg)
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	unsigned short status;
	UCHAR send_type_id; 
	char sendbuf[200];
	status=0;
	
	set_value=set_value*1000;
 	
	if(data_overflow>=0 && data_overflow<=1)
		status=status | (data_overflow*0x2000);
	
	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);
	
	
	status = status + 256;
	
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		sprintf(sendbuf,"IGCSET_ERR,NO POINT %s",logicalname);
	    BdSms(sendbuf);
		return -1;
	}
	
	istnid=(UCHAR)point.stn_id;
	idevid=(UCHAR)point.dev_id;
	itypeid=(UCHAR)point.data_type;
	istartid=(unsigned short)point.pt_id;
	iendid=istartid;
 
	
/*	switch(point.data_type)
	{
	case ANA_TYPE:
	case PARAM_TYPE:
	case OUTA_TYPE:
	case CALC_TYPE:
		send_type_id=20;
		break;
	case IMP_TYPE:
		send_type_id=21;
		break;
		
	case CURVE_TYPE:
		send_type_id=23;
		break;
		
	case DEVOBJ_TYPE:
		send_type_id=24;
		break;
	case TIME_TYPE:
		send_type_id=25;
		break;
	default :
		send_type_id=20;
		printf("\nset_ana_value_by_logicalname,type ERR\n");
		return -1;
	}
*/
	send_type_id=1;
	MultiPointPack_ALM(send_type_id,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)set_value,0,sendsms_falg);
	return 1;
}

int set_int_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int START_POINT_ID,unsigned int END_POINT_ID,int set_value,int stand_by,int MAN_SET,int CHAN_FAIL,int sendsms_falg)
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	unsigned short status;
	int i=0;
	int count=0;
	int pos_start=0;
	int pos_end=0;
 
	if(set_value<0)
		set_value=0;
	if(set_value>15)
		set_value=15;
	
	status=(unsigned short)set_value;
	
	
	
/*	if(DATA_QUALITY>=0 && DATA_QUALITY<=1)
		status=status | (DATA_QUALITY*0x2000);*/
	
	if(MAN_SET>=0 && MAN_SET<=1)
		status=status | (MAN_SET*0x4000);
	
	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);
	
	if(MAN_SET>=0 && MAN_SET<=1)
		status = status + 512;
	else if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status = status + 768;
//	else 
	//	status = status + 256;

	point.stn_id=station_ID;
	point.dev_id=DEVICE_ID;
	point.data_type=TTPE_ID;
	point.pt_id=START_POINT_ID;
	
	istnid=(UCHAR)point.stn_id;
	idevid=(UCHAR)point.dev_id;
	itypeid=(UCHAR)point.data_type;
	istartid=(unsigned short)point.pt_id;
	iendid=istartid;

	count = (END_POINT_ID-START_POINT_ID+1)/200 + 1;
	for(i=0;i<count;i++)
	{
		pos_start= START_POINT_ID+i*200;
		if(i != count-1)
		{
			pos_end = pos_start + 199;
		}
		else
		{
			pos_end = END_POINT_ID;
		}
		if( pos_start>pos_end )
		{
			break;
		}
	 
		itypeid=(UCHAR)point.data_type;
		istartid=(unsigned short)pos_start;
		iendid= (unsigned short)pos_end;
		
	
		
		if(itypeid !=SOE_TYPE &&  itypeid !=POL_TYPE && itypeid !=OUTS_TYPE && itypeid !=SYSMSG_TYPE)
		{
			printf("\nset_ind_value_by_logicalname,type ERR\n");
			return -1;
		}
		
		//istartid=(unsigned short)point.pt_id;
		//iendid=istartid;
		
		MultiPointPack_set_ind(DPS_STATUS_ALL,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)status,sendsms_falg);
	}
	return 1;
}
int set_ana_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int START_POINT_ID,unsigned int END_POINT_ID,float set_value,int stand_by,int MAN_SET,int CHAN_FAIL,int data_overflow,int sendsms_falg)
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	unsigned short status;
	UCHAR send_type_id; 
 
	int i=0;
	int count=0;
	int pos_start=0;
	int pos_end=0;
	status=0;
	
	set_value=set_value*1000;
 	
	if(data_overflow>=0 && data_overflow<=1)
		status=status | (data_overflow*0x2000);
	
	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);
	
	status = status + 256;


	point.stn_id=station_ID;
	point.dev_id=DEVICE_ID;
	point.data_type=TTPE_ID;
	point.pt_id=START_POINT_ID;

	istnid=(UCHAR)point.stn_id;
	idevid=(UCHAR)point.dev_id;
	itypeid=(UCHAR)point.data_type;
	istartid=(unsigned short)point.pt_id;
	iendid=istartid;
 
/*	
	switch(point.data_type)
	{
	case ANA_TYPE:
	case PARAM_TYPE:
	case OUTA_TYPE:
	case CALC_TYPE:
		send_type_id=20;
		break;
	case IMP_TYPE:
		send_type_id=21;
		break;
		
	case CURVE_TYPE:
		send_type_id=23;
		break;
		
	case DEVOBJ_TYPE:
		send_type_id=24;
		break;
	case TIME_TYPE:
		send_type_id=25;
		break;
	default :
		send_type_id=20;
		printf("\nset_ana_value_by_logicalname,type ERR\n");
		return -1;
	}
*/
	count = (END_POINT_ID-START_POINT_ID+1)/200 + 1;
	send_type_id=1;
	for(i=0;i<count;i++)
	{
		pos_start= START_POINT_ID+i*200;
		if(i != count-1)
		{
			pos_end = pos_start + 199;
		}
		else
		{
			pos_end = END_POINT_ID;
		}
		if( pos_start>pos_end )
		{
			break;
		}
		istnid=(UCHAR)point.stn_id;
		idevid=(UCHAR)point.dev_id;
		itypeid=(UCHAR)point.data_type;
		istartid=(unsigned short)pos_start;
		iendid= (unsigned short)pos_end;
		
		istnid=(UCHAR)point.stn_id;
		idevid=(UCHAR)point.dev_id;
		itypeid=(UCHAR)point.data_type;
		
	
		
		//istartid=(unsigned short)point.pt_id;
		//iendid=istartid;
		MultiPointPack_ALM(send_type_id,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)set_value,0,sendsms_falg);
	}
	return 1;
}

int ind_alm_by_logicalname(char * logicalname,int alm_status,int stand_by,int MAN_SET,int CHAN_FAIL,int alarm_type,int sendsms_falg)
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	unsigned short status;
	char sendbuf[200];
	if(alm_status<0)
		alm_status=0;
	if(alm_status>15)
		alm_status=15;
	
	status=(unsigned short)alm_status;
	
	
	
/*	if(DATA_QUALITY>=0 && DATA_QUALITY<=1)
		status=status | (DATA_QUALITY*0x2000);*/
	
	if(MAN_SET>=0 && MAN_SET<=1)
		status=status | (MAN_SET*0x4000);
	
	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);


	if(alarm_type==2)
		status = status + 512;
	else if(alarm_type==3)
		status = status + 768;
	else
		status = status + 256;
	
/*	if(MAN_SET>=0 && MAN_SET<=1)
		status = status + 512;
	else if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status = status + 768;
	else 
		status = status + 256;
*/
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{	
		sprintf(sendbuf,"IGCALM_ERR,NO POINT %s",logicalname);
	    BdSms(sendbuf);
		return -1;
	}

	istnid=(UCHAR)point.stn_id;
	idevid=(UCHAR)point.dev_id;
	itypeid=(UCHAR)point.data_type;
	istartid=(unsigned short)point.pt_id;
	iendid=istartid;

	if(itypeid !=SOE_TYPE &&  itypeid !=POL_TYPE && itypeid !=OUTS_TYPE && itypeid !=SYSMSG_TYPE)
	{
		printf("\nind_alm_by_logicalname,type ERR\n");
		return -1;
		
	}

	MultiPointPack_ALM((UCHAR)4,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)status,1,sendsms_falg);
	return 1;
}

int ana_alm_by_logicalname(char * logicalname,float set_value,int alm_status,int trend_alarm_status,int data_overflow,int CHAN_FAIL,int alarm_type,int sendsms_falg)
//status (0.复归 1.越上上限 2.越上限 3.越下限 4.越下下限 5.复上限 6.复下限 7.复上上限 8.复下下限....)
//trend_alarm_status 0 正常 1 越限  2复限,其它无效  
//data_overflow  0. 正常 1.溢出 
//CHAN_FAIL 1.故障 0.正常 其它无效
//alarm_type 1.越复限报警 2.趋势越复限报警 3.通道故障报警 4.退出控制，5.溢出报警，其它默认为越复限报警 
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	unsigned short status;
	char sendbuf[200];
	if(alm_status<0)
		alm_status=0;
	if(alm_status>15)
		alm_status=15;

	status=(unsigned short)alm_status;


	set_value=set_value*1000;

	if(trend_alarm_status>=0 && trend_alarm_status<=2)
		status=status | (trend_alarm_status*0x10);

	if(data_overflow>=0 && data_overflow<=1)
		status=status | (data_overflow*0x2000);

	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);

	
	if(alarm_type==2)
		status = status + 512;
	else if(alarm_type==3)
		status = status + 768;
	else if(alarm_type==4)
		status = status + 1024;
	else if(alarm_type==5)
		status = status + 1280;
	else
		status = status + 256;

	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		sprintf(sendbuf,"IGCALM_ERR,NO POINT %s",logicalname);
	    BdSms(sendbuf);
		return -1;
	}

	istnid=(UCHAR)point.stn_id;
	idevid=(UCHAR)point.dev_id;
	itypeid=(UCHAR)point.data_type;
	istartid=(unsigned short)point.pt_id;
	iendid=istartid;

	if(point.data_type ==IMP_TYPE)
		MultiPointPack_ALM((UCHAR)16,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)set_value,1,sendsms_falg);
	else
		MultiPointPack_ALM((UCHAR)13,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)set_value,1,sendsms_falg);
	return 1;
}

int ind_alm_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,
					   int alm_status,int stand_by,int MAN_SET,int CHAN_FAIL,int alarm_type,unsigned int START_POINT_ID,unsigned int END_POINT_ID,int sendsms_falg)//status (0.复归 1.越上上限 2.越上限 3.越下限 4.越下下限 5.复上限 6.复下限 7.复上上限 8.复下下限....)
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	int i=0;
	int count=0;
	int pos_start=0;
	int pos_end=0;
	unsigned short status;
	if(alm_status<0)
		alm_status=0;
	if(alm_status>15)
		alm_status=15;
	
	status=(unsigned short)alm_status;
	
	
	/*if(DATA_QUALITY>=0 && DATA_QUALITY<=1)
		status=status | (DATA_QUALITY*0x2000);*/
	
	if(MAN_SET>=0 && MAN_SET<=1)
		status=status | (MAN_SET*0x4000);
	
	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);

	if(alarm_type==2)
		status = status + 512;
	else if(alarm_type==3)
		status = status + 768;
	else
		status = status + 256;


	if(START_POINT_ID<0)
		START_POINT_ID=0;
	
	if(END_POINT_ID>99999)
		END_POINT_ID=99999;

	point.update_src=0;
	point.stn_id=station_ID;
	point.dev_id=DEVICE_ID;
	point.data_type=TTPE_ID;
 
	
	count = (END_POINT_ID-START_POINT_ID+1)/200 + 1;

 
	for(i=0;i<count;i++)
	{
		pos_start= START_POINT_ID+i*200;
		if(i != count-1)
		{
			pos_end = pos_start + 199;
		}
		else
		{
			pos_end = END_POINT_ID;
		}
		if( pos_start>pos_end )
		{
			break;
		}
		istnid=(UCHAR)point.stn_id;
		idevid=(UCHAR)point.dev_id;
		itypeid=(UCHAR)point.data_type;
		istartid=(unsigned short)pos_start;
    	iendid= (unsigned short)pos_end;

		if(itypeid !=SOE_TYPE &&  itypeid !=POL_TYPE && itypeid !=OUTS_TYPE && itypeid !=SYSMSG_TYPE)
		{
			printf("\nind_alm_by_pointid,type ERR\n");
			continue;
		}

		MultiPointPack_ALM((UCHAR)4,istnid,idevid,itypeid, 
			istartid, iendid,(unsigned short)status, (int)status,1,sendsms_falg);
	}
	return 1;
}
int ana_alm_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,float set_value,int alm_status,int trend_alarm_status,int data_overflow,int CHAN_FAIL,int alarm_type,unsigned int START_POINT_ID,unsigned int END_POINT_ID,int sendsms_falg)
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	int i=0;
	int count=0;
	int pos_start=0;
	int pos_end=0;
	unsigned short status;

	memset(&point,0,sizeof(POINTER));
	if(alm_status<0)
		alm_status=0;
	if(alm_status>15)
		alm_status=15;
 
	status=(unsigned short)alm_status;
	
	set_value=set_value*1000;
	
	if(trend_alarm_status>=0 && trend_alarm_status<=2)
		status=status | (trend_alarm_status*0x10);
	
	if(data_overflow>=0 && data_overflow<=1)
		status=status | (data_overflow*0x2000);
	
	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);

	
	if(alarm_type==2)
		status = status + 512;
	else if(alarm_type==3)
		status = status + 768;
	else if(alarm_type==4)
		status = status + 1024;
	else if(alarm_type==5)
		status = status + 1280;
	else
		status = status + 256;



	if(START_POINT_ID<0)
		START_POINT_ID=0;
	
	if(END_POINT_ID>99999)
		END_POINT_ID=99999;

	point.update_src=0;
	point.stn_id=station_ID;
	point.dev_id=DEVICE_ID;
	point.data_type=TTPE_ID;
	
	count = (END_POINT_ID-START_POINT_ID+1)/200 + 1;
	for(i=0;i<count;i++)
	{
		pos_start= START_POINT_ID+i*200;
		if(i != count-1)
		{
			pos_end = pos_start + 199;
		}
		else
		{
			pos_end = END_POINT_ID;
		}
		if( pos_start>pos_end )
		{
			break;
		}

		istnid=(UCHAR)point.stn_id;
		idevid=(UCHAR)point.dev_id;
		itypeid=(UCHAR)point.data_type;
		istartid=(unsigned short)pos_start;
    	iendid= (unsigned short)pos_end;

		if(point.data_type ==IMP_TYPE)
			MultiPointPack_ALM((UCHAR)16,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)set_value,1,sendsms_falg);
		else
			MultiPointPack_ALM((UCHAR)13,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)set_value,1,sendsms_falg);
	}
return 1;	
}


