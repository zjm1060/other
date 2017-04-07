/************************************************************************************************************************
FILENAME:	ots_hdb_con.cpp

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

int		cvt_userinfo_cond(DB_FMT *p_data,int mode)
 
{
	int j=0;	
	if(p_data->cond_flag.flag_userinfo.user_id == 1)
	{
		strcpy(cond_name[j],"user_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_userinfo.user_id);
		j++;
	}
	if(p_data->cond_flag.flag_userinfo.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_userinfo.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_userinfo.name);
		j++;
	}
	if(p_data->cond_flag.flag_userinfo.longname == 1)
	{
		strcpy(cond_name[j],"longname");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_userinfo.longname);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_userinfo.longname);
		j++;
	}
	if(p_data->cond_flag.flag_userinfo.password == 1)
	{
		strcpy(cond_name[j],"password");
		sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_userinfo.password);
		j++;
	}
	if(p_data->cond_flag.flag_userinfo.company == 1)
	{
		strcpy(cond_name[j],"company");
		sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_userinfo.company);
		j++;
	}
	if(p_data->cond_flag.flag_userinfo.idcard == 1)
	{
		strcpy(cond_name[j],"idcard");
		sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_userinfo.idcard);
		j++;
	}
	if(p_data->cond_flag.flag_userinfo.groups_id == 1)
	{
		strcpy(cond_name[j],"groups_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_userinfo.groups_id);
		j++;
	}
	if(p_data->cond_flag.flag_userinfo.groups_role == 1)
	{
		strcpy(cond_name[j],"groups_role");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_userinfo.groups_role);
		j++;
	}
	if(p_data->cond_flag.flag_userinfo.role == 1)
	{
		strcpy(cond_name[j],"role");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_userinfo.role);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by user_id");
	}
	return TRUE;
}

int		cvt_train_info_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_train_info.user_id == 1)
	{
		strcpy(cond_name[j],"user_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_train_info.user_id);
		j++;
	}
	if(p_data->cond_flag.flag_train_info.host_id== 1)
	{
		strcpy(cond_name[j],"host_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_train_info.host_id);
		j++;
	}
	if(p_data->cond_flag.flag_train_info.send_task_id== 1)
	{
		strcpy(cond_name[j],"send_task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_train_info.send_task_id);
		j++;
	}
	if(p_data->cond_flag.flag_train_info.login_status== 1)
	{
		strcpy(cond_name[j],"login_status");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_train_info.login_status);
		j++;
	}
	if(p_data->cond_flag.flag_train_info.scene_status== 1)
	{
		strcpy(cond_name[j],"scene_status");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_train_info.scene_status);
		j++;
	}
	if(p_data->cond_flag.flag_train_info.fault_status== 1)
	{
		strcpy(cond_name[j],"fault_status");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_train_info.fault_status);
		j++;
	}
	if(p_data->cond_flag.flag_train_info.is_record== 1)
	{
		strcpy(cond_name[j],"is_record");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_train_info.is_record);
		j++;
	}
	if(p_data->cond_flag.flag_train_info.simu_mode== 1)
	{
		strcpy(cond_name[j],"simu_mode");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_train_info.simu_mode);
		j++;
	}
	/*
	if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"user_id");
		strcpy(cond_value[j],"userinfo.user_id");
		j++;
		
		strcpy(cond_name[j],"host_id");
		strcpy(cond_value[j],"host.host_id");
		j++;
		
		strcpy(cond_name[j],"task_id");
		strcpy(cond_value[j],"task.task_id");
		j++;			
		
		strcpy(cond_name[j],"scene_id");
		strcpy(cond_value[j],"scene.scene_id");
		j++;	
		
		strcpy(cond_name[j],"fault_id");
		strcpy(cond_value[j],"fault.fault_id");
		j++;	
										
	}
	*/
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by train_info.user_id");
	}
	return TRUE;
}

int		cvt_login_info_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_login_info.user_id == 1)
	{
		strcpy(cond_name[j],"user_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_login_info.user_id);
		j++;
	}
	if(p_data->cond_flag.flag_login_info.host_id == 1)
	{
		strcpy(cond_name[j],"host_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_login_info.host_id);
		j++;
	}
	if(p_data->cond_flag.flag_login_info.login_time == 1)
	{
		strcpy(cond_name[j],"login_time");
		sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_login_info.login_time);
		j++;
	}
	if(p_data->cond_flag.flag_login_info.datatime == 1)
	{
		strcpy(cond_name[j],"login_time");		
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_login_info.datatime);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_login_info.datatime);
		strcpy(cond_opr[j],p_data->cond_flag.flag_login_info.datetime_opr);
		j++;
	}
	if(p_data->cond_flag.flag_login_info.datatime_end == 1)
	{
		strcpy(cond_name[j],"login_time");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_login_info.datatime_end);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_login_info.datatime_end);
		strcpy(cond_opr[j],p_data->cond_flag.flag_login_info.datetime_end_opr);
		j++;
	}
	/*if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"user_id");
		strcpy(cond_value[j],"userinfo.user_id");
		j++;
		
		strcpy(cond_name[j],"host_id");
		strcpy(cond_value[j],"host.host_id");
		j++;
										
	}*/	
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by login_time");
	}
	return TRUE;
}

int		cvt_record_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_record.user_id == 1)
	{
		strcpy(cond_name[j],"user_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.user_id);
		j++;
	}
	if(p_data->cond_flag.flag_record.host_id == 1)
	{
		strcpy(cond_name[j],"host_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.host_id);
		j++;
	}
	if(p_data->cond_flag.flag_record.groups_id == 1)
	{
		strcpy(cond_name[j],"groups_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.groups_id);
		j++;
	}
	if(p_data->cond_flag.flag_record.datetime == 1)
	{
		strcpy(cond_name[j],"datetime");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_record.datetime);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_record.datetime);
		strcpy(cond_opr[j],p_data->cond_flag.flag_record.datetime_opr);
		j++;
	}
	if(p_data->cond_flag.flag_record.datetime_end == 1)
	{
		strcpy(cond_name[j],"datetime");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_record.datetime_end);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_record.datetime_end);
		strcpy(cond_opr[j],p_data->cond_flag.flag_record.datetime_end_opr);
		j++;
	}
	if(p_data->cond_flag.flag_record.type == 1)
	{
		strcpy(cond_name[j],"type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.type);
		j++;
	}
	if(p_data->cond_flag.flag_record.ms == 1)
	{
		strcpy(cond_name[j],"ms");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.ms);
		j++;
	}
	if(p_data->cond_flag.flag_record.station_id == 1)
	{
		strcpy(cond_name[j],"station_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.station_id);
		j++;
	}
	if(p_data->cond_flag.flag_record.dev_id == 1)
	{
		strcpy(cond_name[j],"dev_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.dev_id);
		j++;
	}
	if(p_data->cond_flag.flag_record.task_id == 1)
	{
		strcpy(cond_name[j],"task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.task_id);
		j++;
	}
	if(p_data->cond_flag.flag_record.send_task_id == 1)
	{
		strcpy(cond_name[j],"send_task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_record.send_task_id);
		j++;
	}
	if(p_data->cond_flag.flag_record.logicalname == 1)
	{
		strcpy(cond_name[j],"logicalname");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_record.logicalname);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_record.logicalname);
		j++;
	}
	/*if(p_data->opr_mode == DB_SELECT)
	{

		strcpy(cond_name[j],"host_id");
		strcpy(cond_value[j],"host.host_id");
		j++;
										
	}*/
	
	cond_num=j;

	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by record.datetime desc,record.ms desc");
	}	

	return TRUE;
}

int		cvt_scene_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_scene.scene_id == 1)
	{
		strcpy(cond_name[j],"scene_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_scene.scene_id);
		j++;
	}
	if(p_data->cond_flag.flag_scene.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_scene.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_scene.name);
		j++;
	}
	if(p_data->cond_flag.flag_scene.user_id == 1)
	{
		strcpy(cond_name[j],"user_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_scene.user_id);
		j++;
	}
	if(p_data->cond_flag.flag_scene.src == 1)
	{
		strcpy(cond_name[j],"src");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_scene.src);
		j++;
	}
	if(p_data->cond_flag.flag_scene.station_id == 1)
	{
		strcpy(cond_name[j],"station_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_scene.station_id);
		j++;
	}
	if(p_data->cond_flag.flag_scene.dev_id == 1)
	{
		strcpy(cond_name[j],"dev_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_scene.dev_id);
		j++;
	}
	/*if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"user_id");
		strcpy(cond_value[j],"userinfo.user_id");
		j++;
										
	}*/
	cond_num=j;	

	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by scene_id");
	}	

	return TRUE;
}

int		cvt_scene_data_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_scene_data.scene_id == 1)
	{
		strcpy(cond_name[j],"scene_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_scene_data.scene_id);
		j++;
	}
	if(p_data->cond_flag.flag_scene_data.logicalname == 1)
	{
		strcpy(cond_name[j],"logicalname");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_scene_data.logicalname);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_scene_data.logicalname);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by scene_id");
	}	
	return TRUE;
}

int		cvt_revert_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_revert.revert_id == 1)
	{
		strcpy(cond_name[j],"revert_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_revert.revert_id);
		j++;
	}
	if(p_data->cond_flag.flag_revert.src == 1)
	{
		strcpy(cond_name[j],"src");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_revert.src);
		j++;
	}
	if(p_data->cond_flag.flag_revert.type == 1)
	{
		strcpy(cond_name[j],"type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_revert.type);
		j++;
	}
	if(p_data->cond_flag.flag_revert.station_id == 1)
	{
		strcpy(cond_name[j],"station_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_revert.station_id);
		j++;
	}
	if(p_data->cond_flag.flag_revert.dev_id == 1)
	{
		strcpy(cond_name[j],"dev_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_revert.dev_id);
		j++;
	}
	/*if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"user_id");
		strcpy(cond_value[j],"userinfo.user_id");
		j++;
									
	}*/	
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by revert_id");
	}
	return TRUE;
}

int		cvt_fault_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	

	if(p_data->cond_flag.flag_fault.fault_id == 1)
	{
		strcpy(cond_name[j],"fault_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_fault.fault_id);
		j++;
	}
	if(p_data->cond_flag.flag_fault.type == 1)
	{
		strcpy(cond_name[j],"type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_fault.type);
		j++;
	}
	if(p_data->cond_flag.flag_fault.station_id == 1)
	{
		strcpy(cond_name[j],"station_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_fault.station_id);
		j++;
	}
	if(p_data->cond_flag.flag_fault.dev_id == 1)
	{
		strcpy(cond_name[j],"dev_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_fault.dev_id);
		j++;
	}
	cond_num=j;
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by fault_id");
	}	
	return TRUE;
}

int		cvt_fault_data_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_fault_data.fault_id == 1)
	{
		strcpy(cond_name[j],"fault_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_fault_data.fault_id);
		j++;
	}
	if(p_data->cond_flag.flag_fault_data.logicalname == 1)
	{
		strcpy(cond_name[j],"logicalname");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_fault_data.logicalname);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_fault_data.logicalname);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by fault_id");
	}	
	return TRUE;
}

int		cvt_task_grp_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_task_grp.task_grp_id == 1)
	{
		strcpy(cond_name[j],"task_grp_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_task_grp.task_grp_id);
		j++;
	}
	if(p_data->cond_flag.flag_task_grp.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_task_grp.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_task_grp.name);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by task_grp_id");
	}	
	return TRUE;
}

int		cvt_task_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_task.task_id == 1)
	{
		strcpy(cond_name[j],"task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_task.task_id);
		j++;
	}
	if(p_data->cond_flag.flag_task.task_grp_id == 1)
	{
		strcpy(cond_name[j],"task_grp_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_task.task_grp_id);
		j++;
	}
	if(p_data->cond_flag.flag_task.type == 1)
	{
		strcpy(cond_name[j],"type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_task.type);
		j++;
	}
	if(p_data->cond_flag.flag_task.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_task.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_task.name);
		j++;
	}
	if(p_data->cond_flag.flag_task.station_id == 1)
	{
		strcpy(cond_name[j],"station_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_task.station_id);
		j++;
	}
	if(p_data->cond_flag.flag_task.dev_id == 1)
	{
		strcpy(cond_name[j],"dev_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_task.dev_id);
		j++;
	}
	/*if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"task_grp_id");
		strcpy(cond_value[j],"task_grp.task_grp_id");
		j++;								
	}*/	
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by task_id");
	}
	return TRUE;
}

int		cvt_send_task_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_send_task.send_task_id == 1)
	{
		strcpy(cond_name[j],"send_task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_send_task.send_task_id);
		j++;
	}
	if(p_data->cond_flag.flag_send_task.task_id == 1)
	{
		strcpy(cond_name[j],"task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_send_task.task_id);
		j++;
	}
	if(p_data->cond_flag.flag_send_task.user_id == 1)
	{
		strcpy(cond_name[j],"user_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_send_task.user_id);
		j++;
	}
	if(p_data->cond_flag.flag_send_task.status == 1)
	{
		strcpy(cond_name[j],"status");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_send_task.status);
		j++;
	}
	if(p_data->cond_flag.flag_send_task.start_time_start == 1)
	{
		strcpy(cond_name[j],"start_time");		
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_send_task.start_time_start);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_send_task.start_time_start);
		strcpy(cond_opr[j],p_data->cond_flag.flag_send_task.start_time_start_opr);
		j++;
	}
	if(p_data->cond_flag.flag_send_task.start_time_end == 1)
	{
		strcpy(cond_name[j],"start_time");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_send_task.start_time_end);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_send_task.start_time_end);
		strcpy(cond_opr[j],p_data->cond_flag.flag_send_task.start_time_end_opr);
		j++;
	}
	if(p_data->cond_flag.flag_send_task.end_time_start == 1)
	{
		strcpy(cond_name[j],"end_time");		
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_send_task.end_time_start);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_send_task.end_time_start);
		strcpy(cond_opr[j],p_data->cond_flag.flag_send_task.end_time_start_opr);
		j++;
	}
	if(p_data->cond_flag.flag_send_task.end_time_end == 1)
	{
		strcpy(cond_name[j],"end_time");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_send_task.end_time_end);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_send_task.end_time_end);
		strcpy(cond_opr[j],p_data->cond_flag.flag_send_task.end_time_end_opr);
		j++;
	}
	/*if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"task_id");
		strcpy(cond_value[j],"task.task_id");
		j++;
		strcpy(cond_name[j],"user_id");
		strcpy(cond_value[j],"userinfo.user_id");
		j++;
	}	*/
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by send_task_id");
	}
	return TRUE;
}

int		cvt_host_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_host.host_id == 1)
	{
		strcpy(cond_name[j],"host_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_host.host_id);
		j++;
	}
	if(p_data->cond_flag.flag_host.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_host.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_host.name);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by host_id");
	}
	return TRUE;
}


int		cvt_opr_type_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_opr_type.opr_type == 1)
	{
		strcpy(cond_name[j],"opr_type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_opr_type.opr_type);
		j++;
	}
	if(p_data->cond_flag.flag_opr_type.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_opr_type.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_opr_type.name);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by opr_type");
	}
	return TRUE;
}

int		cvt_local_opr_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_local_opr.opr_id == 1)
	{

		strcpy(cond_name[j],"opr_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_local_opr.opr_id);
		j++;
	}
	if(p_data->cond_flag.flag_local_opr.name == 1)
	{

		strcpy(cond_name[j],"name");
		sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_local_opr.name);
		j++;
	}
	if(p_data->cond_flag.flag_local_opr.opr_type == 1)
	{

		strcpy(cond_name[j],"opr_type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_local_opr.opr_type);
		j++;
	}
	if(p_data->cond_flag.flag_local_opr.opr_mode == 1)
	{

		strcpy(cond_name[j],"opr_mode");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_local_opr.opr_mode);
		j++;
	}
	if(p_data->cond_flag.flag_local_opr.show_mode == 1)
	{

		strcpy(cond_name[j],"show_mode");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_local_opr.show_mode);
		j++;
	}
	if(p_data->cond_flag.flag_local_opr.node_type == 1)
	{

		strcpy(cond_name[j],"node_type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_local_opr.node_type);
		j++;
	}
	if(p_data->cond_flag.flag_local_opr.parent_id == 1)
	{

		strcpy(cond_name[j],"parent_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_local_opr.parent_id);
		j++;
	}
	
	/*if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"opr_type");
		strcpy(cond_value[j],"opr_type.opr_type");
		j++;
								
	}	*/
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by opr_id");
	}
	return TRUE;
}
int		cvt_run_model_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_run_model.model_id == 1)
	{
		strcpy(cond_name[j],"model_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_run_model.model_id);
		j++;
	}
	if(p_data->cond_flag.flag_run_model.host_id == 1)
	{
		strcpy(cond_name[j],"host_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_run_model.host_id);
		j++;
	}
	if(p_data->cond_flag.flag_run_model.status == 1)
	{
		strcpy(cond_name[j],"status");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_run_model.status);
		j++;
	}
	/*if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"model_id");
		strcpy(cond_value[j],"model.model_id");
		j++;
								
	}	*/		
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by start_time");
	}
	return TRUE;
}
int		cvt_run_sequence_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_run_sequence.seq_id == 1)
	{
		strcpy(cond_name[j],"seq_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_run_sequence.seq_id);
		j++;
	}
	if(p_data->cond_flag.flag_run_sequence.host_id == 1)
	{
		strcpy(cond_name[j],"host_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_run_sequence.seq_id);
		j++;
	}
	if(p_data->cond_flag.flag_run_sequence.status == 1)
	{
		strcpy(cond_name[j],"status");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_run_sequence.status);
		j++;
	}
	/*if(p_data->opr_mode == DB_SELECT)
	{
		strcpy(cond_name[j],"seq_id");
		strcpy(cond_value[j],"sequence.seq_id");
		j++;
								
	}*/			
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by start_time");
	}
	return TRUE;
}
int		cvt_model_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_model.model_id == 1)
	{
		strcpy(cond_name[j],"model_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_model.model_id);
		j++;
	}
	if(p_data->cond_flag.flag_model.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_model.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_model.name);
		j++;
	}
	if(p_data->cond_flag.flag_model.station_id == 1)
	{
		strcpy(cond_name[j],"station_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_model.station_id);
		j++;
	}
	if(p_data->cond_flag.flag_model.dev_id == 1)
	{
		strcpy(cond_name[j],"dev_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_model.dev_id);
		j++;
	}
	cond_num=j;
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by model_id");
	}
	return TRUE;
}
int		cvt_sequence_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_sequence.seq_id == 1)
	{
		strcpy(cond_name[j],"seq_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_sequence.seq_id);
		j++;
	}
	if(p_data->cond_flag.flag_sequence.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_sequence.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_sequence.name);
		j++;
	}
	if(p_data->cond_flag.flag_sequence.station_id == 1)
	{
		strcpy(cond_name[j],"station_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_sequence.station_id);
		j++;
	}
	if(p_data->cond_flag.flag_sequence.dev_id == 1)
	{
		strcpy(cond_name[j],"dev_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_sequence.dev_id);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by seq_id");
	}
	return TRUE;
}

int		cvt_var_relation_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_var_relation.model_id == 1)
	{
		strcpy(cond_name[j],"model_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_var_relation.model_id);
		j++;
	}
	if(p_data->cond_flag.flag_var_relation.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_var_relation.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_var_relation.name);
		j++;
	}
	if(p_data->cond_flag.flag_var_relation.type == 1)
	{
		strcpy(cond_name[j],"type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_var_relation.type);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by model_id,type");
	}
	return TRUE;
}

int		cvt_subject_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_subject.subject_id == 1)
	{
		strcpy(cond_name[j],"subject_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_subject.subject_id);
		j++;
	}
	if(p_data->cond_flag.flag_subject.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_subject.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_subject.name);
		j++;
	}
	if(p_data->cond_flag.flag_subject.task_id == 1)
	{
		strcpy(cond_name[j],"task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_subject.task_id);
		j++;
	}
	if(p_data->cond_flag.flag_subject.type == 1)
	{
		strcpy(cond_name[j],"type");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_subject.type);
		j++;
	}
	if(p_data->cond_flag.flag_subject.step == 1)
	{
		strcpy(cond_name[j],"step");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_subject.step);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by step");
	}
	return TRUE;
}
int		cvt_score_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_score.user_id == 1)
	{
		strcpy(cond_name[j],"user_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_score.user_id);
		j++;
	}
	if(p_data->cond_flag.flag_score.send_task_id == 1)
	{
		strcpy(cond_name[j],"send_task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_score.send_task_id);
		j++;
	}
	if(p_data->cond_flag.flag_score.task_id == 1)
	{
		strcpy(cond_name[j],"task_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_score.task_id);
		j++;
	}
	if(p_data->cond_flag.flag_score.subject_id == 1)
	{
		strcpy(cond_name[j],"subject_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_score.subject_id);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by score.subject_id");
	}
	return TRUE;
}
int		cvt_groups_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_groups.groups_id == 1)
	{
		strcpy(cond_name[j],"groups_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_groups.groups_id);
		j++;
	}
	if(p_data->cond_flag.flag_groups.name == 1)
	{
		strcpy(cond_name[j],"name");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_groups.name);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_groups.name);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by groups.groups_id");
	}
	return TRUE;
}
int		cvt_groups_record_cond(DB_FMT *p_data,int mode) 
{
	int j=0;	
	if(p_data->cond_flag.flag_groups_record.user_id == 1)
	{
		strcpy(cond_name[j],"user_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_groups_record.user_id);
		j++;
	}
	if(p_data->cond_flag.flag_groups_record.groups_id == 1)
	{
		strcpy(cond_name[j],"groups_id");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_groups_record.groups_id);
		j++;
	}
	if(p_data->cond_flag.flag_groups_record.groups_role == 1)
	{
		strcpy(cond_name[j],"groups_role");
		sprintf(cond_value[j],"%d",p_data->cond_seg.seg_groups_record.groups_role);
		j++;
	}
	if(p_data->cond_flag.flag_groups_record.datatime == 1)
	{
		strcpy(cond_name[j],"chg_time");		
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_groups_record.datatime);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_groups_record.datatime);
		strcpy(cond_opr[j],p_data->cond_flag.flag_groups_record.datetime_opr);
		j++;
	}
	if(p_data->cond_flag.flag_groups_record.datatime_end == 1)
	{
		strcpy(cond_name[j],"chg_time");
		if (p_data->opr_mode==DB_FUZZY_SELECT)
		{
			sprintf(cond_value[j],"%s",p_data->cond_seg.seg_groups_record.datatime_end);
		}
		else
			sprintf(cond_value[j],"'%s'",p_data->cond_seg.seg_groups_record.datatime_end);
		strcpy(cond_opr[j],p_data->cond_flag.flag_groups_record.datetime_end_opr);
		j++;
	}
	cond_num=j;	
	if(strlen(p_data->addon_str) <= 1)
	{
		strcpy(p_data->addon_str,"order by chg_time");
	}
	return TRUE;
}