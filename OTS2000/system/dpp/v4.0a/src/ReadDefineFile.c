/************************************************************************************************************************************************
FILENAME:	ReadDefineFile.c
FUNCTION:	
		functions of  get definition of data sended from database to plc:ReadDefineFile()
		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:	2006.04.20		ycx
************************************************************************************************************************************************/

#include "../../../dms/v4.0a/inc/dms_com.h"

#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"

#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"


unsigned short ReadDefineFile(LCUID lcu_id, SENDATA * s_data)  //modified by wzg, 20120312
{
	char	ana_filename[256], ind_filename[256], ana_scale_filename[256];
	int		i;	
	FILE	*fp;
	float	scale;
	char	c_name[256], l_name[256];
	char    lcu_name[256], stn_name[256], grp_name[256], buff[1024];
	
	s_data->anum = 0;
	s_data->dnum = 0;
	s_data->anum_scale = 0;

	GetStnNameById(lcu_id.stnid, stn_name);
	GetGroupNameById(lcu_id.stnid, lcu_id.devid, grp_name);
	sprintf(lcu_name, "%s.%s", stn_name, grp_name);

	/*modified by wzg 20110409
	下传模拟量分成两部分：37个归一化模拟量 37个标度化模拟量 SEND_ANA_SIZE=74

	/********** read ana define file 归一化***********/
	
	sprintf(ana_filename, "%s%s", SYS_DEFINE_PATH, DOWNLOAD_ANA_DEFINE_FILE);
	if(fp = fopen(ana_filename, "r"))	
	{
		while(fgets(buff, sizeof(buff), fp))
		{
			char lcu_str[256];

			int num = sscanf(buff, "%d %s %s %s", &i, lcu_str, l_name, c_name);

			if(buff[0]=='#' || buff[0]=='!')
				continue;

			if(0==num || EOF==num)
				continue;

			if(4!=num) 
			{
				SendTraceMsg(0, 0, "read point error: %d %s %s %s", &i, lcu_str, l_name, c_name);
				continue;
			}

			if(strcmp(lcu_str, lcu_name))
				continue;

			if(strlen(l_name) >= POINT_NAME_SIZE)
			{
				strncpy(s_data->ana[s_data->anum].l_name, l_name, POINT_NAME_SIZE-1); 
				SendErrMsg(0, "read %s  %s  error, l_name too long!\n", ana_filename, l_name);
			}
			else
			{			
				strcpy(s_data->ana[s_data->anum].l_name, l_name);
			}

			if(strlen(c_name) >= POINT_NAME_SIZE)
			{
				strncpy(s_data->ana[s_data->anum].c_name, c_name, POINT_NAME_SIZE-1); 
				SendErrMsg(0, "read %s %s error, c_name too long!\n", ana_filename, c_name);
			}
			else
			{			
				strcpy(s_data->ana[s_data->anum].c_name, c_name);
			}
			s_data->anum++;
			if(s_data->anum>=SEND_ANA_SIZE/2)
				break;		
		}
		
		SendTraceMsg(0, 0, "read %s OK! ana_down_num = %d\n", ana_filename, s_data->anum);
		fclose(fp);
	}
	else
		SendTraceMsg(0, 0, "Fail to open %s", ana_filename);

	/********** read ana_scale define file 标度化***********/
	sprintf(ana_scale_filename, "%s%s", SYS_DEFINE_PATH, DOWNLOAD_ANA_SCALE_DEFINE_FILE);
	if(fp = fopen(ana_scale_filename, "r"))	
	{
		while(fgets(buff, sizeof(buff), fp))
		{
			char lcu_str[256];
			
			int num = sscanf(buff, "%d %s %s %s %f", &i, lcu_str, l_name, c_name, &scale);
			
			if(buff[0]=='#' || buff[0]=='!')
				continue;
			
			if(0==num || EOF==num)
				continue;
			
			if(5!=num) 
			{
				SendTraceMsg(0, 0, "read point error: %d %s %s %s", &i, lcu_str, l_name, c_name);
				continue;
			}
			
			if(strcmp(lcu_str, lcu_name))
				continue;
			
			if(strlen(l_name) >= POINT_NAME_SIZE)
			{
				strncpy(s_data->ana_scale[s_data->anum_scale].l_name, l_name, POINT_NAME_SIZE-1); 
				SendErrMsg(0, "read %s  %s  error, l_name too long!\n", ana_scale_filename, l_name);
			}
			else
			{			
				strcpy(s_data->ana_scale[s_data->anum_scale].l_name, l_name);
			}
			
			if(strlen(c_name) >= POINT_NAME_SIZE)
			{
				strncpy(s_data->ana_scale[s_data->anum_scale].c_name, c_name, POINT_NAME_SIZE-1); 
				SendErrMsg(0, "read %s %s error, c_name too long!\n", ana_scale_filename, c_name);
			}
			else
			{			
				strcpy(s_data->ana_scale[s_data->anum_scale].c_name, c_name);
			}

			s_data->ana_scale[s_data->anum_scale].scale= scale;

			s_data->anum_scale++;
			if(s_data->anum_scale>=SEND_ANA_SIZE/2)
				break;		
		}
		
		SendTraceMsg(0, 0, "read %s OK! ana_scale_down_num = %d\n", ana_scale_filename, s_data->anum_scale);
		fclose(fp);
	}
	else
		SendTraceMsg(0, 0, "Fail to open %s", ana_scale_filename);

	/********** read ind define file  开关量 ***********/	
	sprintf(lcu_name, "%s.%s", stn_name, grp_name);
	
	sprintf(ind_filename, "%s%s", SYS_DEFINE_PATH, DOWNLOAD_IND_DEFINE_FILE);
	if(fp = fopen(ind_filename, "r"))	
	{
		while(fgets(buff, sizeof(buff), fp))
		{
			char lcu_str[256];
			
			int num = sscanf(buff, "%d %s %s %s", &i, lcu_str, l_name, c_name);
			
			if(buff[0]=='#' || buff[0]=='!')
				continue;
			
			if(0==num || EOF==num)
				continue;
			
			if(4!=num) 
			{
				SendTraceMsg(0, 0, "read point error: %d %s %s %s", &i, lcu_str, l_name, c_name);
				continue;
			}
			
			if(strcmp(lcu_str, lcu_name))
				continue;
			
			if(strlen(l_name) >= POINT_NAME_SIZE)
			{
				strncpy(s_data->ind[s_data->dnum].l_name, l_name, POINT_NAME_SIZE-1); 
				SendErrMsg(0, "read %s  %s  error, l_name too long!\n", ind_filename, l_name);
			}
			else
			{			
				strcpy(s_data->ind[s_data->dnum].l_name, l_name);
			}
			
			if(strlen(c_name) >= POINT_NAME_SIZE)
			{
				strncpy(s_data->ind[s_data->dnum].c_name, c_name, POINT_NAME_SIZE-1); 
				SendErrMsg(0, "read %s %s error, c_name too long!\n", ind_filename, c_name);
			}
			else
			{			
				strcpy(s_data->ind[s_data->dnum].c_name, c_name);
			}
			s_data->dnum++;
			if(s_data->dnum>=SEND_POL_SIZE)
				break;		
		}
		
		SendTraceMsg(0, 0, "read %s OK! ind_down_num = %d\n", ind_filename, s_data->dnum);
		fclose(fp);
	}
	else
		SendTraceMsg(0, 0, "Fail to open %s", ind_filename);
		
	if(s_data->dnum || s_data->anum || s_data->anum_scale) 
		return 1;
	else 
		return 0;
}





