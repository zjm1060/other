/************************************************************************************************************************
FILENAME:	ots_hdb_table.cpp

FUNCTION:	
	
	some functions to fill or get table info of ots hdb library,include:

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2010.4.21	create the file for qt hdb	on oracle	yyp

*************************************************************************************************************************/

//#include "/home/ems/system/lan/v4.0a/inc/lan.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
//#include <math.h>

int		cvt_userinfo(DB_FMT *p_data,int mode)
{
	int i=0,k=1;	
	if(p_data->flag.flag_userinfo.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_userinfo.user_id);
		}
		else
		{
			p_data->seg.seg_userinfo.user_id=atoi(field_value[i]);
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.name);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.name,field_value[i]);
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.longname == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"longname");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.longname);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.longname,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.password == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"password");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.password);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.password,field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.role == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"role");
			sprintf(field_value[i],"%d",p_data->seg.seg_userinfo.role);
		}
		else
		{
			p_data->seg.seg_userinfo.role=atoi(field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.sex == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"sex");
			sprintf(field_value[i],"%d",p_data->seg.seg_userinfo.sex);
		}
		else
		{
			p_data->seg.seg_userinfo.sex=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.edu == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"edu");
			sprintf(field_value[i],"%d",p_data->seg.seg_userinfo.edu);
		}
		else
		{
			p_data->seg.seg_userinfo.edu=atoi(field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.company == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"company");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.company);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.company,field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.department == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"department");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.department);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.department,field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.duty == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"duty");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.duty);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.duty,field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.email == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"email");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.email);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.email,field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.mtel == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"mtel");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.mtel);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.mtel,field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.tel == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"tel");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.tel);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.tel,field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.idcard == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"idcard");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.idcard);
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.idcard,field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.privilege == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"privilege");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_userinfo.privilege);
		}
		else
		{
			//p_data->seg.seg_userinfo.privilege=atoi(field_value[i]);	
			//sscanf(field_value[i],"%u",&p_data->seg.seg_userinfo.privilege);
			strcpy(p_data->seg.seg_userinfo.privilege,field_value[i]);
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.groups_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"groups_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_userinfo.groups_id);
		}
		else
		{
			p_data->seg.seg_userinfo.groups_id=atoi(field_value[i]);
		}
		i++;
	}
	if(p_data->flag.flag_userinfo.groups_role == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"groups_role");
			sprintf(field_value[i],"%d",p_data->seg.seg_userinfo.groups_role);
		}
		else
		{
			p_data->seg.seg_userinfo.groups_role=atoi(field_value[i]);
		}
		i++;
	}
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			
			strcpy(field_name[i],"(select name from groups where groups_id=userinfo.groups_id) as groups_name");
		}
		else
		{
			strcpy(p_data->seg.seg_userinfo.groups_name,field_value[i]);					
		}
		i++;//k++;			
	}
	data_userinfo[row_id] = p_data->seg.seg_userinfo;
	//*p_data->rec.p_userinfo[row_id] = p_data->seg.seg_userinfo;
		
	field_num= i;
	table_num=k;
	return TRUE;
}

int		cvt_train_info(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_train_info.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(user_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(user_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_train_info.user_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_train_info.user_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_train_info.user_id=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_train_info.host_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"host_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_train_info.host_id);
		}
		else
		{
				p_data->seg.seg_train_info.host_id=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_train_info.send_task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"send_task_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(send_task_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(send_task_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_train_info.send_task_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_train_info.send_task_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_train_info.send_task_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"task_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(task_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(task_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_train_info.task_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_train_info.task_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_train_info.task_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.scene_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"scene_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(scene_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(scene_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_train_info.scene_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_train_info.scene_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_train_info.scene_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.fault_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"fault_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(fault_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(fault_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_train_info.fault_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_train_info.fault_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_train_info.fault_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.login_status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"login_status");
			sprintf(field_value[i],"%d",p_data->seg.seg_train_info.login_status);
		}
		else
		{
			p_data->seg.seg_train_info.login_status=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"status");
			sprintf(field_value[i],"%d",p_data->seg.seg_train_info.status);
		}
		else
		{
			p_data->seg.seg_train_info.status=atoi(field_value[i]);						
		}
		i++;
	}
	if(p_data->flag.flag_train_info.speed == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"speed");
			sprintf(field_value[i],"%d", p_data->seg.seg_train_info.speed );
		}
		else
		{
			p_data->seg.seg_train_info.speed=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.dbin_file == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dbin_file");
			sprintf(field_value[i],"'%s'", p_data->seg.seg_train_info.dbin_file );
		}
		else
		{
			strcpy(p_data->seg.seg_train_info.dbin_file,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.task_status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"task_status");
			sprintf(field_value[i],"%d", p_data->seg.seg_train_info.task_status );
		}
		else
		{
			p_data->seg.seg_train_info.task_status=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.scene_status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"scene_status");
			sprintf(field_value[i],"%d", p_data->seg.seg_train_info.scene_status );
		}
		else
		{
			p_data->seg.seg_train_info.scene_status=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.fault_status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"fault_status");
			sprintf(field_value[i],"%d", p_data->seg.seg_train_info.fault_status );
		}
		else
		{
			p_data->seg.seg_train_info.fault_status=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.is_record == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"is_record");
			sprintf(field_value[i],"%d", p_data->seg.seg_train_info.is_record );
		}
		else
		{
			p_data->seg.seg_train_info.is_record=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_train_info.simu_mode == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"simu_mode");
			sprintf(field_value[i],"%d", p_data->seg.seg_train_info.simu_mode );
		}
		else
		{
			p_data->seg.seg_train_info.simu_mode=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			
			strcpy(field_name[i],"(select name from userinfo where user_id=train_info.user_id) as userinfo_name");
			//strcpy(field_name[i],"name");
			//strcpy(field_table[i],"userinfo");
			//strcpy(union_table[k],field_table[i]);		
			//sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.userinfo_name);
			

		}
		else
		{
			strcpy(p_data->seg.seg_train_info.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{

			strcpy(field_name[i],"(select longname from userinfo where user_id=train_info.user_id) as userinfo_longname");
			//strcpy(field_name[i],"name");
			//strcpy(field_table[i],"userinfo");
			//strcpy(union_table[k],field_table[i]);		
			//sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.userinfo_longname);


		}
		else
		{
			strcpy(p_data->seg.seg_train_info.userinfo_longname,field_value[i]);					
		}
		i++;//k++;
					
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from host where host_id=train_info.host_id) as host_name");
			/*
			strcpy(field_name[i],"name");
			strcpy(field_table[i],"host");
			strcpy(union_table[k],field_table[i]);						
			sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.host_name);
			*/
		}
		else
		{
			strcpy(p_data->seg.seg_train_info.host_name,field_value[i]);					
		}
		i++;//k++;
					
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from task where task_id=train_info.task_id) as task_name");
			/*
			strcpy(field_name[i],"name");
			strcpy(field_table[i],"task");
			strcpy(union_table[k],field_table[i]);						
			sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.task_name);
			*/
		}
		else
		{
			strcpy(p_data->seg.seg_train_info.task_name,field_value[i]);					
		}
		i++;//k++;
					
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from scene where scene_id=train_info.scene_id) as scene_name");
			/*
			strcpy(field_name[i],"name");
			strcpy(field_table[i],"scene");
			strcpy(union_table[k],field_table[i]);						
			sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.scene_name);
			*/
		}
		else
		{
			strcpy(p_data->seg.seg_train_info.scene_name,field_value[i]);					
		}
		i++;//k++;	
					
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from fault where fault_id=train_info.fault_id) as fault_name");			
			/*
			strcpy(field_name[i],"name");
			strcpy(field_table[i],"fault");
			strcpy(union_table[k],field_table[i]);						
			sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.fault_name);
			*/
		}
		else
		{
			strcpy(p_data->seg.seg_train_info.fault_name,field_value[i]);					
		}
		i++;	//k++;	
		
							
	}
	
	data_train_info[row_id] = p_data->seg.seg_train_info;	
	//*p_data->rec.p_train_info[row_id] = p_data->seg.seg_train_info;

	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_login_info(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_login_info.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_login_info.user_id);
		}
		else
		{
			p_data->seg.seg_login_info.user_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_login_info.login_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"login_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_login_info.login_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_login_info.login_time,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_login_info.logout_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"logout_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_login_info.logout_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_login_info.logout_time,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_login_info.host_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"host_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_login_info.host_id);
		}
		else
		{
			p_data->seg.seg_login_info.host_id=atoi(field_value[i]);				
		}
		i++;
	}

	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from userinfo where user_id=login_info.user_id) as userinfo_name");
		}
		else
		{
			strcpy(p_data->seg.seg_login_info.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select longname from userinfo where user_id=login_info.user_id) as userinfo_longname");
		}
		else
		{
			strcpy(p_data->seg.seg_login_info.userinfo_longname,field_value[i]);					
		}
		i++;//k++;
		
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from host where host_id=login_info.host_id) as host_name");
		}
		else
		{
			strcpy(p_data->seg.seg_login_info.host_name,field_value[i]);					
		}
		i++;//k++;		
	}		
	
	data_login_info[row_id] = p_data->seg.seg_login_info;
	//*p_data->rec.p_login_info[row_id] = p_data->seg.seg_login_info;
		
	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_record(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_record.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.user_id);
		}
		else
		{
			p_data->seg.seg_record.user_id=atoi(field_value[i]);
		}
		i++;
	}
	if(p_data->flag.flag_record.host_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"host_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.host_id);
		}
		else
		{
			p_data->seg.seg_record.host_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_record.groups_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"groups_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.groups_id);
		}
		else
		{
			p_data->seg.seg_record.groups_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_record.datetime == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"datetime");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_record.datetime);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_record.datetime,field_value[i])	;			
		}
		i++;
	}
	if(p_data->flag.flag_record.ms == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"ms");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.ms);
		}
		else
		{
			p_data->seg.seg_record.ms=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_record.type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"type");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.type);
		}
		else
		{
			p_data->seg.seg_record.type=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_record.content == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"content");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_record.content);
		}
		else
		{
			strcpy(p_data->seg.seg_record.content,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_record.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.station_id);
		}
		else
		{
			p_data->seg.seg_record.station_id=atoi(field_value[i]);	
		}
		i++;
	}
	if(p_data->flag.flag_record.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.dev_id);
		}
		else
		{
			p_data->seg.seg_record.dev_id=atoi(field_value[i]);			
		}
		i++;
	}	
	if(p_data->flag.flag_record.task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"task_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.task_id);
		}
		else
		{
			p_data->seg.seg_record.task_id=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_record.send_task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"send_task_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.send_task_id);
		}
		else
		{
			p_data->seg.seg_record.send_task_id=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_record.logicalname == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"logicalname");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_record.logicalname);
		}
		else
		{
			strcpy(p_data->seg.seg_record.logicalname,field_value[i])	;	
		}
		i++;
	}
	if(p_data->flag.flag_record.type_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"type_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.type_id);
		}
		else
		{
			p_data->seg.seg_record.type_id=atoi(field_value[i]);			
		}
		i++;
	}	
	if(p_data->flag.flag_record.status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"status");
			sprintf(field_value[i],"%d",p_data->seg.seg_record.status);
		}
		else
		{
			p_data->seg.seg_record.status=atoi(field_value[i]);			
		}
		i++;
	}	
	if(p_data->flag.flag_record.fvalue == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"fvalue");
			sprintf(field_value[i],"%f",p_data->seg.seg_record.fvalue);
		}
		else
		{
			p_data->seg.seg_record.fvalue=atof(field_value[i]);			
		}
		i++;
	}

	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from host where host_id=record.host_id) as host_name");
		}
		else
		{
			strcpy(p_data->seg.seg_record.host_name,field_value[i]);					
		}
		i++;//k++;
	}				
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from userinfo where user_id=record.user_id) as userinfo_name");
		}
		else
		{
			strcpy(p_data->seg.seg_record.userinfo_name,field_value[i]);					
		}
		i++;//k++;
	}	
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select longname from userinfo where user_id=record.user_id) as userinfo_longname");
		}
		else
		{
			strcpy(p_data->seg.seg_record.userinfo_longname,field_value[i]);					
		}
		i++;//k++;
	}	
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from groups where groups_id=record.groups_id) as groups_name");
		}
		else
		{
			strcpy(p_data->seg.seg_record.groups_name,field_value[i]);					
		}
		i++;//k++;
	}	

	data_record[row_id] = p_data->seg.seg_record;	
	//*p_data->rec.p_record[row_id] = p_data->seg.seg_record;
		
	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_scene(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	i=0;
	if(p_data->flag.flag_scene.scene_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"scene_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_scene.scene_id);
		}
		else
		{
				p_data->seg.seg_scene.scene_id=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_scene.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_scene.name);
		}
		else
		{
			strcpy(p_data->seg.seg_scene.name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_scene.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_scene.user_id);
		}
		else
		{
			p_data->seg.seg_scene.user_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_scene.src == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"src");
			sprintf(field_value[i],"%d",p_data->seg.seg_scene.src);
		}
		else
		{
			p_data->seg.seg_scene.src=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_scene.datetime == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"datetime");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_scene.datetime);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_scene.datetime,field_value[i])	;			
		}
		i++;
	}
	if(p_data->flag.flag_scene.remark == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"remark");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_scene.remark);
		}
		else
		{
			strcpy(p_data->seg.seg_scene.remark,field_value[i])	;			
		}
		i++;
	}
	if(p_data->flag.flag_scene.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_scene.station_id);
		}
		else
		{
			p_data->seg.seg_scene.station_id=atoi(field_value[i]);	

		}
		i++;
	}
	if(p_data->flag.flag_scene.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_scene.dev_id);
		}
		else
		{
			p_data->seg.seg_scene.dev_id=atoi(field_value[i]);	
		}
		i++;
	}		
	if(p_data->flag.flag_scene.filename == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"filename");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_scene.filename);
		}
		else
		{
			strcpy(p_data->seg.seg_scene.filename,field_value[i])	;			
		}
		i++;
	}

	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from userinfo where user_id=scene.user_id) as userinfo_name");
		}
		else
		{
			strcpy(p_data->seg.seg_scene.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select longname from userinfo where user_id=scene.user_id) as userinfo_longname");
		}
		else
		{
			strcpy(p_data->seg.seg_scene.userinfo_longname,field_value[i]);					
		}
		i++;//k++;
	}				
	
	data_scene[row_id] = p_data->seg.seg_scene;		
	//*p_data->rec.p_scene[row_id] = p_data->seg.seg_scene;

	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_scene_data(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	i=0;
	if(p_data->flag.flag_scene_data.scene_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"scene_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_scene_data.scene_id);
		}
		else
		{
			p_data->seg.seg_scene_data.scene_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_scene_data.logicalname == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"logicalname");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_scene_data.logicalname);
		}
		else
		{
			strcpy(p_data->seg.seg_scene_data.logicalname,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_scene_data.status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"status");
			sprintf(field_value[i],"%d",p_data->seg.seg_scene_data.status);
		}
		else
		{
				p_data->seg.seg_scene_data.status=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_scene_data.data == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"data");
			sprintf(field_value[i],"%.3f",p_data->seg.seg_scene_data.data);
		}
		else
		{
			p_data->seg.seg_scene_data.data= atof(field_value[i]);			
		}
		i++;
	}
	data_scene_data[row_id] = p_data->seg.seg_scene_data;			
	//*p_data->rec.p_scene_data[row_id] = p_data->seg.seg_scene_data;
	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_revert(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_revert.revert_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"revert_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_revert.revert_id);
		}
		else
		{
			p_data->seg.seg_revert.revert_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_revert.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_revert.name);
		}
		else
		{
			strcpy(p_data->seg.seg_revert.name,field_value[i])	;			
		}
		i++;
	}
	if(p_data->flag.flag_revert.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_revert.user_id);
		}
		else
		{
			p_data->seg.seg_revert.user_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_revert.start_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"start_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_revert.start_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
				strcpy(p_data->seg.seg_revert.start_time,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_revert.end_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"end_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_revert.end_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_revert.end_time,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_revert.filename == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"filename");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_revert.filename);
		}
		else
		{
				strcpy(p_data->seg.seg_revert.filename,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_revert.remark == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"remark");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_revert.remark);
		}
		else
		{
			strcpy(p_data->seg.seg_revert.remark,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_revert.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_revert.station_id);
		}
		else
		{
			p_data->seg.seg_revert.station_id=atoi(field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_revert.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_revert.dev_id);
		}
		else
		{
			p_data->seg.seg_revert.dev_id=atoi(field_value[i]);					
		}
		i++;
	}	
	if(p_data->flag.flag_revert.src == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"src");
			sprintf(field_value[i],"%d",p_data->seg.seg_revert.src);
		}
		else
		{
			p_data->seg.seg_revert.src=atoi(field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_revert.type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"type");
			sprintf(field_value[i],"%d",p_data->seg.seg_revert.type);
		}
		else
		{
			p_data->seg.seg_revert.type=atoi(field_value[i]);					
		}
		i++;
	}
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from userinfo where user_id=revert.user_id) as userinfo_name");
		}
		else
		{
			strcpy(p_data->seg.seg_revert.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select longname from userinfo where user_id=revert.user_id) as userinfo_longname");
		}
		else
		{
			strcpy(p_data->seg.seg_revert.userinfo_longname,field_value[i]);					
		}
		i++;//k++;
	}					
	
	data_revert[row_id] = p_data->seg.seg_revert;			
	//*p_data->rec.p_revert[row_id] = p_data->seg.seg_revert;
	
	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_fault(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_fault.fault_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"fault_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault.fault_id);
		}
		else
		{
			p_data->seg.seg_fault.fault_id=atoi(field_value[i]);		
		}
		i++;
	}
	if(p_data->flag.flag_fault.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_fault.name);
		}
		else
		{
				strcpy(p_data->seg.seg_fault.name,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_fault.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault.user_id);
		}
		else
		{
			p_data->seg.seg_fault.user_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_fault.type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"type");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault.type);
		}
		else
		{
			p_data->seg.seg_fault.type=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_fault.datetime == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"datetime");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_fault.datetime);
			field_type[i] = TYPE_TIME;
		}
		else
		{
				strcpy(p_data->seg.seg_fault.datetime,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_fault.remark == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"remark");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_fault.remark);
		}
		else
		{
			strcpy(p_data->seg.seg_fault.remark,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_fault.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault.station_id);
		}
		else
		{
			p_data->seg.seg_fault.station_id=atoi(field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_fault.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault.dev_id);
		}
		else
		{
			p_data->seg.seg_fault.dev_id=atoi(field_value[i]);				
		}
		i++;
	}			

	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from userinfo where user_id=fault.user_id) as userinfo_name");
		}
		else
		{
			strcpy(p_data->seg.seg_fault.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select longname from userinfo where user_id=fault.user_id) as userinfo_longname");
		}
		else
		{
			strcpy(p_data->seg.seg_fault.userinfo_longname,field_value[i]);					
		}
		i++;//k++;
	}			

	data_fault[row_id] = p_data->seg.seg_fault;		
	//*p_data->rec.p_fault[row_id] = p_data->seg.seg_fault;

	field_num= i;
	table_num=k;		
	return TRUE;
}

int		cvt_fault_data(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_fault_data.fault_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"fault_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault_data.fault_id);
		}
		else
		{
			p_data->seg.seg_fault_data.fault_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_fault_data.logicalname == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"logicalname");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_fault_data.logicalname);
		}
		else
		{
			strcpy(p_data->seg.seg_fault_data.logicalname, field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_fault_data.type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"type");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault_data.type);
		}
		else
		{
			p_data->seg.seg_fault_data.type=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_fault_data.init_value == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"init_value");
			sprintf(field_value[i],"'%.3f'",p_data->seg.seg_fault_data.init_value);
		}
		else
		{
			p_data->seg.seg_fault_data.init_value =atof(field_value[i]);						
		}
		i++;
	}
	if(p_data->flag.flag_fault_data.dst_value == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dst_value");
			sprintf(field_value[i],"'%.3f'",p_data->seg.seg_fault_data.dst_value);
		}
		else
		{
			p_data->seg.seg_fault_data.dst_value = atof(field_value[i]);						
		}
		i++;
	}
	if(p_data->flag.flag_fault_data.norm_value == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"norm_value");
			sprintf(field_value[i],"'%.3f'",p_data->seg.seg_fault_data.norm_value);
		}
		else
		{
			p_data->seg.seg_fault_data.norm_value =atof(field_value[i]);						
		}
		i++;
	}
	if(p_data->flag.flag_fault_data.chg_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"chg_time");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault_data.chg_time);
		}
		else
		{
			p_data->seg.seg_fault_data.chg_time=atoi(field_value[i]);
		}
		i++;
	}
	if(p_data->flag.flag_fault_data.chg_mode == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"chg_mode");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault_data.chg_mode);
		}
		else
		{
			p_data->seg.seg_fault_data.chg_mode=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_fault_data.delay == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"delay");
			sprintf(field_value[i],"%d",p_data->seg.seg_fault_data.delay);
		}
		else
		{
			p_data->seg.seg_fault_data.delay=atoi(field_value[i]);				
		}
		i++;
	}
	data_fault_data[row_id] = p_data->seg.seg_fault_data;	
	//*p_data->rec.p_fault_data[row_id] = p_data->seg.seg_fault_data;

	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_task_grp(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_task_grp.task_grp_id == 1)
	{
		
		if(mode == 0)
		{
			strcpy(field_name[i],"task_grp_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_task_grp.task_grp_id);
		}
		else
		{
			p_data->seg.seg_task_grp.task_grp_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task_grp.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_task_grp.name);
		}
		else
		{
				strcpy(p_data->seg.seg_task_grp.name,field_value[i]);
		}
		i++;
	}
	if(p_data->flag.flag_task_grp.remark == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"remark");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_task_grp.remark);
		}
		else
		{
			strcpy(p_data->seg.seg_task_grp.remark,field_value[i]);	
		}
		i++;
	}
	
	data_task_grp[row_id] = p_data->seg.seg_task_grp;	
	//*p_data->rec.p_task_grp[row_id] = p_data->seg.seg_task_grp;

	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_task(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_task.task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"task_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_task.task_id);
		}
		else
		{
			p_data->seg.seg_task.task_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_task.name);
		}
		else
		{
			strcpy(p_data->seg.seg_task.name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.task_grp_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"task_grp_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_task.task_grp_id);
		}
		else
		{
			p_data->seg.seg_task.task_grp_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"type");
			sprintf(field_value[i],"%d",p_data->seg.seg_task.type);
		}
		else
		{
			p_data->seg.seg_task.type=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.datetime == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"datetime");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_task.datetime);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_task.datetime,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_task.user_id);
		}
		else
		{
			p_data->seg.seg_task.user_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.remark == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"remark");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_task.remark);
		}
		else
		{
			strcpy(p_data->seg.seg_task.remark,field_value[i]);				
		}
		i++;
	}

	if(p_data->flag.flag_task.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_task.station_id);
		}
		else
		{
			p_data->seg.seg_task.station_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_task.dev_id);
		}
		else
		{
			p_data->seg.seg_task.dev_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.scene_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"scene_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(scene_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(scene_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_task.scene_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_task.scene_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_task.scene_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_task.fault_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"fault_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(fault_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(fault_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_task.fault_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_task.fault_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_task.fault_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from userinfo where user_id=task.user_id) as userinfo_name");
		}
		else
		{
			strcpy(p_data->seg.seg_task.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select longname from userinfo where user_id=task.user_id) as userinfo_longname");
		}
		else
		{
			strcpy(p_data->seg.seg_task.userinfo_longname,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from task_grp where task_grp_id=task.task_grp_id) as task_grp_name");
		}
		else
		{
			strcpy(p_data->seg.seg_task.task_grp_name,field_value[i]);					
		}
		i++;//k++;
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from scene where scene_id=task.scene_id) as scene_name");
		}
		else
		{
			strcpy(p_data->seg.seg_task.scene_name,field_value[i]);					
		}
		i++;//k++;
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from fault where fault_id=task.fault_id) as fault_name");
		}
		else
		{
			strcpy(p_data->seg.seg_task.fault_name,field_value[i]);					
		}
		i++;//k++;
	}			
	
	data_task[row_id] = p_data->seg.seg_task;		
	//*p_data->rec.p_task[row_id] = p_data->seg.seg_task;
		
	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_send_task(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_send_task.send_task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"send_task_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_send_task.send_task_id);
		}
		else
		{
			p_data->seg.seg_send_task.send_task_id=atoi(field_value[i]);					
		}
		i++;
	}
	if(p_data->flag.flag_send_task.task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"task_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_send_task.task_id);
		}
		else
		{
			p_data->seg.seg_send_task.task_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_send_task.send_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"send_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_send_task.send_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_send_task.send_time,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_send_task.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_send_task.user_id);
		}
		else
		{
			p_data->seg.seg_send_task.user_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_send_task.start_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"start_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_send_task.start_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_send_task.start_time,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_send_task.status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"status");
			sprintf(field_value[i],"%d",p_data->seg.seg_send_task.status);
		}
		else
		{
			p_data->seg.seg_send_task.status=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_send_task.end_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"end_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_send_task.end_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_send_task.end_time,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_send_task.score == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"score");
			sprintf(field_value[i],"'%.3f'",p_data->seg.seg_send_task.score);
		}
		else
		{
			p_data->seg.seg_send_task.score=atof(field_value[i]);						
		}
		i++;
	}
	if(p_data->flag.flag_send_task.time_len== 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"time_len");
			sprintf(field_value[i],"%d",p_data->seg.seg_send_task.time_len);
		}
		else
		{
			p_data->seg.seg_send_task.time_len=atoi(field_value[i]);						
		}
		i++;
	}

	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from userinfo where user_id=send_task.user_id) as userinfo_name");
		}
		else
		{
			strcpy(p_data->seg.seg_send_task.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select longname from userinfo where user_id=send_task.user_id) as userinfo_longname");
		}
		else
		{
			strcpy(p_data->seg.seg_send_task.userinfo_longname,field_value[i]);					
		}
		i++;//k++;
					
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from task where task_id=send_task.task_id) as task_name");		
		}
		else
		{
			strcpy(p_data->seg.seg_send_task.task_name,field_value[i]);					
		}
		i++;//k++;
	}
		
	data_send_task[row_id] = p_data->seg.seg_send_task;		
	//*p_data->rec.p_send_task[row_id] = p_data->seg.seg_send_task;

	field_num= i;
	table_num=k;
	return TRUE;
}
int		cvt_host(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_host.host_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"host_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_host.host_id);
		}
		else
		{
				p_data->seg.seg_host.host_id=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_host.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_host.name);
		}
		else
		{
			strcpy(p_data->seg.seg_host.name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_host.oth_name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"oth_name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_host.oth_name);
		}
		else
		{
			strcpy(p_data->seg.seg_host.oth_name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_host.ip_a == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"ip_a");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_host.ip_a);
		}
		else
		{
			strcpy(p_data->seg.seg_host.ip_a,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_host.ip_b == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"ip_b");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_host.ip_b);
		}
		else
		{
			strcpy(p_data->seg.seg_host.ip_b,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_host.is_student == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"is_student");
			sprintf(field_value[i],"%d",p_data->seg.seg_host.is_student);
		}
		else
		{
			p_data->seg.seg_host.is_student=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_host.is_teacher == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"is_teacher");
			sprintf(field_value[i],"%d",p_data->seg.seg_host.is_teacher);
		}
		else
		{
			p_data->seg.seg_host.is_teacher=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_host.is_server == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"is_server");
			sprintf(field_value[i],"%d",p_data->seg.seg_host.is_server);
		}
		else
		{
			p_data->seg.seg_host.is_server=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_host.server_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"server_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_host.server_id);
		}
		else
		{
			p_data->seg.seg_host.server_id=atoi(field_value[i]);				
		}
		i++;
	}		
	data_host[row_id] = p_data->seg.seg_host;	
	//*p_data->rec.p_host[row_id] = p_data->seg.seg_host;

	field_num= i;
	table_num=k;	
	return TRUE;
}
int		cvt_opr_type(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_opr_type.opr_type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"opr_type");
			sprintf(field_value[i],"%d",p_data->seg.seg_opr_type.opr_type);
		}
		else
		{
			p_data->seg.seg_opr_type.opr_type=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_opr_type.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_opr_type.name);
		}
		else
		{
				strcpy(p_data->seg.seg_opr_type.name,field_value[i]);			
		}
		i++;
	}
	data_opr_type[row_id] = p_data->seg.seg_opr_type;	
	//*p_data->rec.p_opr_type[row_id] = p_data->seg.seg_opr_type;

	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_local_opr(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_local_opr.opr_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"opr_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_local_opr.opr_id);
		}
		else
		{
			p_data->seg.seg_local_opr.opr_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_local_opr.opr_type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"opr_type");
			sprintf(field_value[i],"%d",p_data->seg.seg_local_opr.opr_type);
		}
		else
		{
			p_data->seg.seg_local_opr.opr_type=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_local_opr.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_local_opr.name);
		}
		else
		{
			strcpy(p_data->seg.seg_local_opr.name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_local_opr.opr_mode == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"opr_mode");
			sprintf(field_value[i],"%d",p_data->seg.seg_local_opr.opr_mode);
		}
		else
		{
			p_data->seg.seg_local_opr.opr_mode=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_local_opr.msg == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"msg");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_local_opr.msg);
		}
		else
		{
			strcpy(p_data->seg.seg_local_opr.msg,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_local_opr.show_mode == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"show_mode");
			sprintf(field_value[i],"%d",p_data->seg.seg_local_opr.show_mode);
		}
		else
		{
			p_data->seg.seg_local_opr.show_mode =atoi(field_value[i]);	
		}
		i++;
	}

	if(p_data->flag.flag_local_opr.node_type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"node_type");
			sprintf(field_value[i],"%d",p_data->seg.seg_local_opr.node_type);
		}
		else
		{
			p_data->seg.seg_local_opr.node_type =atoi(field_value[i]);	
		}
		i++;
	}

	if(p_data->flag.flag_local_opr.parent_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"parent_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_local_opr.parent_id);
		}
		else
		{
			p_data->seg.seg_local_opr.parent_id =atoi(field_value[i]);	
		}
		i++;
	}
	
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from opr_type where opr_type=local_opr.opr_type) as opr_type_name");
		}
		else
		{
			strcpy(p_data->seg.seg_local_opr.opr_type_name,field_value[i]);					
		}
		i++;//k++;
	}				
	
	data_local_opr[row_id] = p_data->seg.seg_local_opr;			
	//*p_data->rec.p_local_opr[row_id] = p_data->seg.seg_local_opr;

	field_num= i;
	table_num=k;	
	return TRUE;
}
int		cvt_run_model(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_run_model.model_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"model_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_run_model.model_id);
		}
		else
		{
			p_data->seg.seg_run_model.model_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_run_model.host_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"host_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_run_model.host_id);
		}
		else
		{
			p_data->seg.seg_run_model.host_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_run_model.status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"status");
			sprintf(field_value[i],"%d",p_data->seg.seg_run_model.status);
		}
		else
		{
				p_data->seg.seg_run_model.status=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_run_model.start_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"start_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_run_model.start_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_run_model.start_time,field_value[i]);				
		}
		i++;
	}
	
	if(p_data->flag.flag_model.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_model.station_id);
		}
		else
		{
			p_data->seg.seg_model.station_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_model.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_model.dev_id);
		}
		else
		{
			p_data->seg.seg_model.dev_id=atoi(field_value[i]);				
		}
		i++;
	}		

	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from model where model_id=run_model.model_id) as model_name");
		}
		else
		{
			strcpy(p_data->seg.seg_run_model.model_name,field_value[i]);					
		}
		i++;//k++;
	}			
	
	data_run_model[row_id] = p_data->seg.seg_run_model;		
	//*p_data->rec.p_run_model[row_id] = p_data->seg.seg_run_model;
	
	field_num= i;
	table_num=k;	
	return TRUE;
}
int		cvt_run_sequence(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_run_sequence.seq_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"seq_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_run_sequence.seq_id);
		}
		else
		{
			p_data->seg.seg_run_sequence.seq_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_run_sequence.host_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"host_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_run_sequence.host_id);
		}
		else
		{
			p_data->seg.seg_run_sequence.host_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_run_sequence.status == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"status");
			sprintf(field_value[i],"%d",p_data->seg.seg_run_sequence.status);
		}
		else
		{
			p_data->seg.seg_run_sequence.status=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_run_sequence.start_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"start_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_run_sequence.start_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_run_sequence.start_time,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_sequence.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_sequence.station_id);
		}
		else
		{
			p_data->seg.seg_sequence.station_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_sequence.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_sequence.dev_id);
		}
		else
		{
			p_data->seg.seg_sequence.dev_id=atoi(field_value[i]);				
		}
		i++;
	}				
	
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from sequence where seq_id=run_sequence.seq_id) as sequence_name");
		}
		else
		{
			strcpy(p_data->seg.seg_run_sequence.sequence_name,field_value[i]);					
		}
		i++;//k++;
	}				
	
	data_run_sequence[row_id] = p_data->seg.seg_run_sequence;	
	//*p_data->rec.p_run_sequence[row_id] = p_data->seg.seg_run_sequence;
		
	field_num= i;
	table_num=k;	
	return TRUE;
}
int		cvt_model(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_model.model_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"model_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_model.model_id);
		}
		else
		{
			p_data->seg.seg_model.model_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_model.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_model.name);
		}
		else
		{
			strcpy(p_data->seg.seg_model.name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_model.long_name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"long_name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_model.long_name);
		}
		else
		{
			strcpy(p_data->seg.seg_model.long_name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_model.datetime == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"datetime");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_model.datetime);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_model.datetime,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_model.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_model.station_id);
		}
		else
		{
			p_data->seg.seg_model.station_id=atoi(field_value[i]);	

		}
		i++;
	}
	if(p_data->flag.flag_model.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_model.dev_id);
		}
		else
		{
			p_data->seg.seg_model.dev_id=atoi(field_value[i]);	
		}
		i++;
	}	
	data_model[row_id] = p_data->seg.seg_model;	
	//*p_data->rec.p_model[row_id] = p_data->seg.seg_model;	

	field_num= i;
	table_num=k;	
	return TRUE;
}
int		cvt_sequence(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_sequence.seq_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"seq_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_sequence.seq_id);
		}
		else
		{
			p_data->seg.seg_sequence.seq_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_sequence.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_sequence.name);
		}
		else
		{
			strcpy(p_data->seg.seg_sequence.name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_sequence.long_name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"long_name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_sequence.long_name);
		}
		else
		{
			strcpy(p_data->seg.seg_sequence.long_name,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_sequence.datetime == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"datetime");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_sequence.datetime);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_sequence.datetime,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_sequence.station_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"station_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_sequence.station_id);
		}
		else
		{
			p_data->seg.seg_sequence.station_id=atoi(field_value[i]);	

		}
		i++;
	}
	if(p_data->flag.flag_sequence.dev_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"dev_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_sequence.dev_id);
		}
		else
		{
			p_data->seg.seg_sequence.dev_id=atoi(field_value[i]);	
		}
		i++;
	}	
	data_sequence[row_id] = p_data->seg.seg_sequence;	
	//*p_data->rec.p_sequence[row_id] = p_data->seg.seg_sequence;	

	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_var_relation(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_var_relation.model_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"model_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_var_relation.model_id);
		}
		else
		{
			p_data->seg.seg_var_relation.model_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_var_relation.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_var_relation.name);
		}
		else
		{
				strcpy(p_data->seg.seg_var_relation.name,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_var_relation.type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"type");
			sprintf(field_value[i],"%d",p_data->seg.seg_var_relation.type);
		}
		else
		{
			p_data->seg.seg_var_relation.type=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_var_relation.logicalname == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"logicalname");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_var_relation.logicalname);
		}
		else
		{
			strcpy(p_data->seg.seg_var_relation.logicalname,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_var_relation.remark == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"remark");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_var_relation.remark);
		}
		else
		{
			strcpy(p_data->seg.seg_var_relation.remark,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_var_relation.data_type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"data_type");
			sprintf(field_value[i],"%d",p_data->seg.seg_var_relation.data_type);
		}
		else
		{
			p_data->seg.seg_var_relation.data_type=atoi(field_value[i]);				
		}
		i++;
	}
	data_var_relation[row_id] = p_data->seg.seg_var_relation;	
	//*p_data->rec.p_var_relation[row_id] = p_data->seg.seg_var_relation;	

	field_num= i;
	table_num=k;
	return TRUE;
}

int		cvt_subject(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_subject.subject_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"subject_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_subject.subject_id);
		}
		else
		{
			p_data->seg.seg_subject.subject_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_subject.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_subject.name);
		}
		else
		{
			strcpy(p_data->seg.seg_subject.name,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_subject.task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"task_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_subject.task_id);
		}
		else
		{
			p_data->seg.seg_subject.task_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_subject.step == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"step");
			sprintf(field_value[i],"%d",p_data->seg.seg_subject.step);
		}
		else
		{
			p_data->seg.seg_subject.step=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_subject.type == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"type");
			sprintf(field_value[i],"%d",p_data->seg.seg_subject.type);
		}
		else
		{
			p_data->seg.seg_subject.type=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_subject.datetime == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"datetime");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_subject.datetime);
		}
		else
		{
			strcpy(p_data->seg.seg_subject.datetime,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_subject.judge_logic == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"judge_logic");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_subject.judge_logic);
		}
		else
		{
			strcpy(p_data->seg.seg_subject.judge_logic,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_subject.pre_condition == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"pre_condition");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_subject.pre_condition);
		}
		else
		{
			strcpy(p_data->seg.seg_subject.pre_condition,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_subject.interlock_name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"interlock_name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_subject.interlock_name);
		}
		else
		{
			strcpy(p_data->seg.seg_subject.interlock_name,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_subject.subject_tip == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"subject_tip");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_subject.subject_tip);
		}
		else
		{
			strcpy(p_data->seg.seg_subject.subject_tip,field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_subject.time_limit == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"time_limit");
			sprintf(field_value[i],"%d",p_data->seg.seg_subject.time_limit);
		}
		else
		{
			p_data->seg.seg_subject.time_limit=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_subject.score == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"score");
			sprintf(field_value[i],"%f",p_data->seg.seg_subject.score);
			field_type[i] = TYPE_FLOAT;
		}
		else
		{
			p_data->seg.seg_subject.score=atof(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_subject.is_keystep == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"is_keystep");
			sprintf(field_value[i],"%d",p_data->seg.seg_subject.is_keystep);
		}
		else
		{
			p_data->seg.seg_subject.is_keystep=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{

			strcpy(field_name[i],"(select name from task where task_id=subject.task_id) as task_name");
			//strcpy(field_name[i],"name");
			//strcpy(field_table[i],"userinfo");
			//strcpy(union_table[k],field_table[i]);		
			//sprintf(field_value[i],"'%s'",p_data->seg.seg_score.userinfo_name);
		}
		else
		{
			strcpy(p_data->seg.seg_subject.task_name,field_value[i]);					
		}
		i++;//k++;
	}
	data_subject[row_id] = p_data->seg.seg_subject;	
	//*p_data->rec.p_subject[row_id] = p_data->seg.seg_subject;	

	field_num= i;
	table_num=k;
	return TRUE;
}

int		cvt_score(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_score.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(user_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(user_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_train_info.user_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_score.user_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_score.user_id=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_score.host_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"host_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_score.host_id);
		}
		else
		{
				p_data->seg.seg_score.host_id=atoi(field_value[i]);			
		}
		i++;
	}
	if(p_data->flag.flag_score.send_task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"send_task_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(send_task_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(send_task_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_score.send_task_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_score.send_task_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_score.send_task_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_score.task_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"task_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(task_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(task_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_score.task_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_score.task_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_score.task_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_score.subject_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"subject_id");
			if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
			{
				if (strcmp(g_ots_hdb_type,"ORACLE")==0)
				{
					sprintf(field_name[i],"NVL(subject_id,%d)",CONST_NEG);
				}
				else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
				{
					sprintf(field_name[i],"IFNULL(subject_id,%d)",CONST_NEG);
				}
			}
			if(p_data->seg.seg_score.subject_id != CONST_NEG )
			{
				sprintf(field_value[i],"%d",p_data->seg.seg_score.subject_id);
			}
			else
			{
				strcpy(field_value[i],"null");
			}
		}
		else
		{
			p_data->seg.seg_score.subject_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_score.score == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"score");
			sprintf(field_value[i],"%f",p_data->seg.seg_score.score);
			field_type[i] = TYPE_FLOAT;
		}
		else
		{
			p_data->seg.seg_score.score=atof(field_value[i]);				
		}
		i++;
	}
	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			
			strcpy(field_name[i],"(select name from userinfo where user_id=score.user_id) as userinfo_name");
			//strcpy(field_name[i],"name");
			//strcpy(field_table[i],"userinfo");
			//strcpy(union_table[k],field_table[i]);		
			//sprintf(field_value[i],"'%s'",p_data->seg.seg_score.userinfo_name);
		}
		else
		{
			strcpy(p_data->seg.seg_score.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{

			strcpy(field_name[i],"(select longname from userinfo where user_id=score.user_id) as userinfo_longname");
			//strcpy(field_name[i],"name");
			//strcpy(field_table[i],"userinfo");
			//strcpy(union_table[k],field_table[i]);		
			//sprintf(field_value[i],"'%s'",p_data->seg.seg_score.userinfo_name);
		}
		else
		{
			strcpy(p_data->seg.seg_score.userinfo_longname,field_value[i]);					
		}
		i++;//k++;
					
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from host where host_id=score.host_id) as host_name");
			/*
			strcpy(field_name[i],"name");
			strcpy(field_table[i],"host");
			strcpy(union_table[k],field_table[i]);						
			sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.host_name);
			*/
		}
		else
		{
			strcpy(p_data->seg.seg_score.host_name,field_value[i]);					
		}
		i++;//k++;
					
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from task where task_id=score.task_id) as task_name");
			/*
			strcpy(field_name[i],"name");
			strcpy(field_table[i],"task");
			strcpy(union_table[k],field_table[i]);						
			sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.task_name);
			*/
		}
		else
		{
			strcpy(p_data->seg.seg_score.task_name,field_value[i]);					
		}
		i++;//k++;
					
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from subject where scene_id=score.subject_id) as subject_name");
			/*
			strcpy(field_name[i],"name");
			strcpy(field_table[i],"scene");
			strcpy(union_table[k],field_table[i]);						
			sprintf(field_value[i],"'%s'",p_data->seg.seg_train_info.scene_name);
			*/
		}
		else
		{
			strcpy(p_data->seg.seg_score.subject_name,field_value[i]);					
		}
		i++;//k++;					
	}
	
	data_score[row_id] = p_data->seg.seg_score;	
	//*p_data->rec.p_score[row_id] = p_data->seg.seg_score;	

	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_groups(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_groups.groups_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"groups_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_groups.groups_id);
		}
		else
		{
			p_data->seg.seg_groups.groups_id=atoi(field_value[i]);
		}
		i++;
	}
	if(p_data->flag.flag_groups.name == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"name");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_groups.name);
		}
		else
		{
			strcpy(p_data->seg.seg_groups.name,field_value[i]);
		}
		i++;
	}
	
	data_groups[row_id] = p_data->seg.seg_groups;	
	//*p_data->rec.p_groups[row_id] = p_data->seg.seg_groups;

	field_num= i;
	table_num=k;	
	return TRUE;
}

int		cvt_groups_record(DB_FMT *p_data,int mode) 
{
	int i=0,k=1;	
	if(p_data->flag.flag_groups_record.user_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"user_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_groups_record.user_id);
		}
		else
		{
			p_data->seg.seg_groups_record.user_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_groups_record.groups_id == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"groups_id");
			sprintf(field_value[i],"%d",p_data->seg.seg_groups_record.groups_id);
		}
		else
		{
			p_data->seg.seg_groups_record.groups_id=atoi(field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_groups_record.chg_time == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"chg_time");
			sprintf(field_value[i],"'%s'",p_data->seg.seg_groups_record.chg_time);
			field_type[i] = TYPE_TIME;
		}
		else
		{
			strcpy(p_data->seg.seg_groups_record.chg_time,field_value[i]);				
		}
		i++;
	}
	if(p_data->flag.flag_groups_record.groups_role == 1)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"groups_role");
			sprintf(field_value[i],"%d",p_data->seg.seg_groups_record.groups_role);
		}
		else
		{
			p_data->seg.seg_groups_record.groups_role=atoi(field_value[i]);				
		}
		i++;
	}

	if(p_data->opr_mode == DB_SELECT||p_data->opr_mode==DB_FUZZY_SELECT)
	{
		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from userinfo where user_id=groups_record.user_id) as userinfo_name");
		}
		else
		{
			strcpy(p_data->seg.seg_groups_record.userinfo_name,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select longname from userinfo where user_id=groups_record.user_id) as userinfo_longname");
		}
		else
		{
			strcpy(p_data->seg.seg_groups_record.userinfo_longname,field_value[i]);					
		}
		i++;//k++;

		if(mode == 0)
		{
			strcpy(field_name[i],"(select name from groups where groups_id=groups.groups_id) as groups_name");
		}
		else
		{
			strcpy(p_data->seg.seg_groups_record.groups_name,field_value[i]);					
		}
		i++;//k++;		
	}		

	data_groups_record[row_id] = p_data->seg.seg_groups_record;	
	//*p_data->rec.p_groups_record[row_id] = p_data->seg.seg_groups_record;

	field_num= i;
	table_num=k;	
	return TRUE;
}