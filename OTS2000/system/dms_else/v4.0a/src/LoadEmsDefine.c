


#include 	"../../../dms/v4.0a/inc/dms_com.h"
#include 	"../../../dms/v4.0a/inc/ems_init_def.h"
#include 	<string.h>

#ifndef	WINDOWS_OS
#include 	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#endif

typedef struct	
{
	struct 
	{
		char	name[MAX_HOST_NAME_SIZE];
		char	longname[MAX_HOST_LONGNAME_SIZE];
		char	type[MAX_HOST_TYPE_SIZE];
		char	ipaddr[MAX_HOST_IPADDR_NUM][MAX_IPADDR_SIZE];
		char	process[MAX_PROCESS_NUM][MAX_PROCESS_NAME_SIZE];
		char	domain[MAX_DOMAIN_NAME_SIZE];
		int		ipaddr_num;
		int		process_num;
		BOOL	state_manage_flag;
		UCHAR	stn_id;
		char	disk;
	} host[MAX_HOST_NUM];
	int			host_num;

	char		base_process[MAX_BASE_PROCESS_NUM][MAX_PROCESS_NAME_SIZE];
	int			base_process_num;

	char		broadcast[MAX_HOST_IPADDR_NUM][MAX_IPADDR_SIZE];
	int			broadcast_num;

	struct
	{
		char	role[MAX_ROLE_NAME_SIZE];
		char	host[MAX_HOST_NUM_IN_ROLE][MAX_HOST_NAME_SIZE];
		int		host_num;
	} host_role[MAX_HOST_ROLE_NUM];
	int			host_role_num;

	char		rcp_host[MAX_RCP_HOST_NUM][MAX_HOST_NAME_SIZE];
	int			rcp_host_num;

	struct
	{
		char	host[2][MAX_HOST_NAME_SIZE];
	} host_pair[MAX_HOST_PAIR_NUM];
	int			host_pair_num;

	struct
	{
		char	host[MAX_HOST_NAME_SIZE];
		char	port[MAX_PORT_NAME_SIZE];
		char	gps_type[MAX_GPS_TYPE_NAME_SIZE];
	} time_server[MAX_TIME_SERVER_NUM];
	int			time_server_num;

	struct
	{
		UCHAR	stn_id;
		UCHAR	dev_id;
		char	host[MAX_PLCSCAN_HOST_NUM][MAX_HOST_NAME_SIZE];
		int		host_num;
	} plc_scan[MAX_ALL_LCU_NUM];
	int			plc_scan_num;

	struct
	{
		UCHAR	stn_id;
		UCHAR	dev_id;
		char	ipaddr[MAX_PLC_IPADDR_NUM][MAX_IPADDR_SIZE];
		int		ipaddr_num;
	} lcu_ipaddr[MAX_ALL_LCU_NUM];
	int			lcu_ipaddr_num;

	struct
	{
		UCHAR	stn_id;
		UCHAR	dev_id;
	} plc_dcpu[MAX_ALL_LCU_NUM];
	int			plc_dcpu_num;

	struct
	{
		UCHAR	host_id;
		UCHAR	cmd_id;
	} cmd_host[MAX_CMD_HOST_NUM];
	int			cmd_host_num;

	int			sys_net_num;
	int			ctrl_net_num;
	BOOL		ctrl_net_independ;
	BOOL		event_file_log;
	BOOL		power_adj_log;
	BOOL		io_force_disable;
	BOOL		dev_obj_disable;
	BOOL		his_rdb_disable;
	char		dispatch_name[STATION_NAME_SIZE];

	struct
	{
		char	name[MAX_VARIABLE_NAME_SIZE];
		float	value;
	} variable[MAX_VARIABLE_NUM];
	int			var_num;

	struct
	{
		char	name[MAX_STRING_NAME_SIZE];
		char	value[MAX_STRING_VALUE_SIZE];
	} string[MAX_STRING_NUM];
	int			string_num;

}	EMS_CONFIG_INFO;


static EMS_CONFIG_INFO	config_info;



int	LocateKeyword( FILE *fp, char *keyword )
{
	char	str[MAX_CHAR_NUM_IN_LINE];
	char	key[64];

	sprintf(key, "[%s]", keyword);
	rewind( fp );
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( strstr(str, key) )
			return 0;
	}

	return -1;
}


int	LoadEmsDefine(char *filename)
{
	FILE	*fp;
	char	err_str[256];
	char	str[MAX_CHAR_NUM_IN_LINE];
	char	strbuf[MAX_STR_NUM_IN_LINE][MAX_CHAR_NUM_IN_LINE];
	char	keyword[64];
	int		i, j, k, proc_num, str_num;
	int		value;
	UCHAR	stn_id, dev_id;
	BOOL	flag;
#ifdef	WINDOWS_OS
	WSADATA		WSAData;

	if(WSAStartup(MAKEWORD(2,2), &WSAData))
	{
		Err_log("Fail to initialize net");
		return -1;
	}
#endif

/*	printf("\nconfig_info size: %d", sizeof(config_info));*/

	if( (fp=fopen(filename, "r"))==NULL )
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		return -1;
	}


	/* KEYWORD=="HOST_NAME" */
	strcpy(keyword, "HOST_NAME");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}

	config_info.host_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num!=6 && str_num!=7)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(strlen(strbuf[0])>=MAX_HOST_NAME_SIZE)
		{
			sprintf(err_str, "Host name too long in %s: %s", keyword, strbuf[0]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		if(strlen(strbuf[2])>=MAX_DOMAIN_NAME_SIZE)
		{
			sprintf(err_str, "Host domain name too long in %s: %s", keyword, strbuf[2]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(strlen(strbuf[4])>=MAX_HOST_LONGNAME_SIZE)
		{
			sprintf(err_str, "Host longname too long in %s: %s", keyword, strbuf[4]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(-1==GetStnIdByName(strbuf[1], &stn_id))
		{
			sprintf(err_str, "No such station in %s: %s", keyword, strbuf[1]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		config_info.host[config_info.host_num].stn_id= stn_id;

		if( !strcmp(strbuf[3], "YES") )
			config_info.host[config_info.host_num].state_manage_flag= TRUE;
		else if( !strcmp(strbuf[3], "NO") )
			config_info.host[config_info.host_num].state_manage_flag= FALSE;
		else
		{
			sprintf(err_str, "Must be YES or NO in %s: %s", keyword, strbuf[3]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(strcmp(strbuf[5], "UNIX") && strcmp(strbuf[5], "WINDOWS")
			&& strcmp(strbuf[5], "PRINTER") && strcmp(strbuf[5], "SWITCH")
			&& strcmp(strbuf[5], "ROUTER") && strcmp(strbuf[5], "WIN_ISLAND")
			&& strcmp(strbuf[5], "COLOR_PRT"))
		{
			sprintf(err_str, "Unknown host type in %s: %s", keyword, strbuf[5]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		strcpy(config_info.host[config_info.host_num].name, strbuf[0]);
		strcpy(config_info.host[config_info.host_num].domain, strbuf[2]);
		strcpy(config_info.host[config_info.host_num].longname, strbuf[4]);
		strcpy(config_info.host[config_info.host_num].type, strbuf[5]);
		if(!strcmp(strbuf[5], "WINDOWS"))
		{
			if(str_num!=7)
			{
				Err_log("Windows host need disk define");
				return -1;
			}
			else
				config_info.host[config_info.host_num].disk= *strbuf[6];
		}

		config_info.host_num++;
	}


	/* KEYWORD=="HOST_ROLE" */
	strcpy(keyword, "HOST_ROLE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}

	config_info.host_role_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0 || str_num==1)
			continue;

		if(config_info.host_role_num>=MAX_HOST_ROLE_NUM)
		{
			sprintf(err_str, "Too much role in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if((str_num-1)>MAX_HOST_NUM_IN_ROLE)
		{
			sprintf(err_str, "Too much host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(strlen(strbuf[0])>=MAX_ROLE_NAME_SIZE)
		{
			sprintf(err_str, "Role name too long in %s: %s", keyword, strbuf[0]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		strcpy(config_info.host_role[config_info.host_role_num].role, strbuf[0]);

		k= 0;
		for(i=1; i<str_num; i++)
		{
			for(j=0; j<config_info.host_num; j++)
			{
				if(!strcmp(strbuf[i], config_info.host[j].name))
					break;
			}
			if(j>=config_info.host_num)
			{
				sprintf(err_str, "No such host in %s: %s", keyword, strbuf[i]);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
			strcpy(config_info.host_role[config_info.host_role_num].host[k++], strbuf[i]);
		}
		config_info.host_role[config_info.host_role_num].host_num= k;
		config_info.host_role_num++;
	}



	/* KEYWORD=="BROADCAST" */
	strcpy(keyword, "BROADCAST");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	i=0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		for(j=0; j<str_num; j++)
		{
			if(strlen(strbuf[j])>=MAX_IPADDR_SIZE)
			{
				sprintf(err_str, "IP addr too long in %s: %s", keyword, strbuf[j]);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}

			if(i>=MAX_HOST_IPADDR_NUM)
			{
				sprintf(err_str, "Too much broadcast ipaddr in %s: %s", keyword, str);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}

			if(-1==inet_addr(strbuf[j]))
			{ 
				sprintf(err_str, "broadcast ipaddr %s error in %s", strbuf[j], keyword);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}

			strcpy(config_info.broadcast[i], strbuf[j]);
			config_info.broadcast_num= i+1;
			i++;
		}

	}

	


	/* KEYWORD=="HOST_IPADDR" */
	strcpy(keyword, "HOST_IPADDR");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0 || str_num==1)
			continue;

		if((str_num-1)>MAX_HOST_IPADDR_NUM)
		{
			sprintf(err_str, "Too much host ipaddr in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(i=0; i<config_info.host_num; i++)
		{
			if(!strcmp(strbuf[0], config_info.host[i].name))
				break;
		}
		if(i>=config_info.host_num)
		{
			sprintf(err_str, "No such host in %s: %s", keyword, strbuf[0]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(j=1; j<str_num; j++)
		{
			if(strlen(strbuf[j])>=MAX_IPADDR_SIZE)
			{
				sprintf(err_str, "IP addr too long in %s: %s", keyword, strbuf[j]);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
			if(-1==inet_addr(strbuf[j]))
			{ 
				sprintf(err_str, "host ipaddr %s error in %s", strbuf[j], keyword);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}

			strcpy(config_info.host[i].ipaddr[j-1], strbuf[j]);
		}
		config_info.host[i].ipaddr_num= str_num-1;
	}

	
	/* KEYWORD=="BASE_PROCESS" */
	strcpy(keyword, "BASE_PROCESS");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	config_info.base_process_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num!=1)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(strbuf[0][0]=='!' || strbuf[0][0]=='#')
			continue;

		if(strlen(strbuf[0])>=MAX_PROCESS_NAME_SIZE)
		{
			sprintf(err_str, "Base process name too long in %s: %s", keyword, strbuf[0]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(config_info.base_process_num>=MAX_BASE_PROCESS_NUM)
		{
			sprintf(err_str, "Too much base processes in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(j=0; j<config_info.base_process_num; j++)
		{
			if(!strcmp(config_info.base_process[j], strbuf[0]))
			{
				sprintf(err_str, "Base process name repeat in %s: %s", keyword, strbuf[0]);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
		}
		strcpy(config_info.base_process[config_info.base_process_num], strbuf[0]);
		config_info.base_process_num++;
	}

	


	/* KEYWORD=="HOST_PROCESS" */
	strcpy(keyword, "HOST_PROCESS");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0 || str_num==1)
			continue;

		if((str_num-1)>MAX_PROCESS_NUM)
		{
			sprintf(err_str, "Too much processes in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(i=0; i<config_info.host_num; i++)
		{
			if(!strcmp(strbuf[0], config_info.host[i].name))
				break;
		}

		if(i>=config_info.host_num)
		{
			sprintf(err_str, "No such host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		proc_num= 0;
		for(j=0; j<(str_num-1); j++)
		{
			if(strlen(strbuf[j+1])>=MAX_PROCESS_NAME_SIZE)
			{
				sprintf(err_str, "Process name too long in %s: %s", keyword, strbuf[j+1]);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
			if(strbuf[j+1][0]=='!' || strbuf[j+1][0]=='#')
				continue;

			for(k=0; k<config_info.base_process_num; k++)
			{
				if(!strcmp(strbuf[j+1], config_info.base_process[k]))
				{
					sprintf(err_str, "Process name already is defined in base processes in %s: %s", keyword, strbuf[j+1]);
					Err_log(err_str);
					fclose(fp);
					return -1;
				}
			}
			strcpy(config_info.host[i].process[proc_num++], strbuf[j+1]);
		}
		config_info.host[i].process_num= proc_num;
	}

	

	/* KEYWORD=="REMOTE_COPY" */
	strcpy(keyword, "REMOTE_COPY");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	config_info.rcp_host_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num!=1)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(config_info.rcp_host_num>=MAX_RCP_HOST_NUM)
		{
			sprintf(err_str, "Too much rcp host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(i=0; i<config_info.host_num; i++)
		{
			if(!strcmp(strbuf[0], config_info.host[i].name))
				break;
		}

		if(i>=config_info.host_num)
		{
			sprintf(err_str, "No such host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
	
		if(strcmp(config_info.host[i].type, "UNIX") && strcmp(config_info.host[i].type, "WINDOWS"))
		{
			sprintf(err_str, "Host type must be UNIX or WINDOWS in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		strcpy(config_info.rcp_host[config_info.rcp_host_num], strbuf[0]);
		config_info.rcp_host_num++;
	}

	

	/* KEYWORD=="HOST_SWITCH" */
	strcpy(keyword, "HOST_SWITCH");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	config_info.host_pair_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num!=2)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(config_info.host_pair_num>=MAX_HOST_PAIR_NUM)
		{
			sprintf(err_str, "Too much host pair in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(i=0; i<config_info.host_num; i++)
		{
			if(!strcmp(strbuf[0], config_info.host[i].name))
				break;
		}
		if(i>=config_info.host_num)
		{
			sprintf(err_str, "No such host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		stn_id= config_info.host[i].stn_id;

		for(i=0; i<config_info.host_num; i++)
		{
			if(!strcmp(strbuf[1], config_info.host[i].name))
				break;
		}
		if(i>=config_info.host_num)
		{
			sprintf(err_str, "No such host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(stn_id!=config_info.host[i].stn_id)
		{
			sprintf(err_str, "Pair hosts must be in same station: %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		strcpy(config_info.host_pair[config_info.host_pair_num].host[0], strbuf[0]);
		strcpy(config_info.host_pair[config_info.host_pair_num].host[1], strbuf[1]);
		config_info.host_pair_num++;
	}

	

	/* KEYWORD=="TIME_SERVER" */
	strcpy(keyword, "TIME_SERVER");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	config_info.time_server_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num!=3)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(config_info.time_server_num>=MAX_TIME_SERVER_NUM)
		{
			sprintf(err_str, "Too much time server in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(i=0; i<config_info.host_num; i++)
		{
			if(!strcmp(strbuf[0], config_info.host[i].name))
				break;
		}
		if(i>=config_info.host_num)
		{
			sprintf(err_str, "No such host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(strlen(strbuf[1])>=MAX_PORT_NAME_SIZE)
		{
			sprintf(err_str, "Port name too long in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		if(strlen(strbuf[2])>=MAX_GPS_TYPE_NAME_SIZE)
		{
			sprintf(err_str, "Gps type name too long in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		strcpy(config_info.time_server[config_info.time_server_num].host, strbuf[0]);
		strcpy(config_info.time_server[config_info.time_server_num].port, strbuf[1]);
		strcpy(config_info.time_server[config_info.time_server_num].gps_type, strbuf[2]);
		config_info.time_server_num++;
	}


	
	/* KEYWORD=="PLC_SCAN" */
	strcpy(keyword, "PLC_SCAN");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}

	config_info.plc_scan_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num<=2)
			continue;

		if(config_info.plc_scan_num>=MAX_ALL_LCU_NUM)
		{
			sprintf(err_str, "Too much plc in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if((str_num-2)>MAX_PLCSCAN_HOST_NUM)
		{
			sprintf(err_str, "Too much host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(-1==GetStnIdByName(strbuf[0], &stn_id))
		{
			sprintf(err_str, "No such station in %s: %s", keyword, strbuf[0]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		if(-1==GetGroupIdByName(stn_id, strbuf[1], &dev_id))
		{
			sprintf(err_str, "No such group in %s: %s.%s", keyword, strbuf[0], strbuf[1]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		k= 0;
		for(i=2; i<str_num; i++)
		{
			for(j=0; j<config_info.host_num; j++)
			{
				if(!strcmp(strbuf[i], config_info.host[j].name))
					break;
			}
			if(j>=config_info.host_num)
			{
				sprintf(err_str, "No such host in %s: %s", keyword, strbuf[i]);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
			strcpy(config_info.plc_scan[config_info.plc_scan_num].host[k++], strbuf[i]);
		}
		config_info.plc_scan[config_info.plc_scan_num].stn_id= stn_id;
		config_info.plc_scan[config_info.plc_scan_num].dev_id= dev_id;
		config_info.plc_scan[config_info.plc_scan_num].host_num= k;
		config_info.plc_scan_num++;
	}

	
	/* KEYWORD=="LCU_IPADDR" */
	strcpy(keyword, "LCU_IPADDR");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}

	config_info.lcu_ipaddr_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num<=2)
			continue;

		if(config_info.lcu_ipaddr_num>=MAX_ALL_LCU_NUM)
		{
			sprintf(err_str, "Too much lcu in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if((str_num-2)>MAX_PLC_IPADDR_NUM)
		{
			sprintf(err_str, "Too much lcu ipaddr in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(-1==GetStnIdByName(strbuf[0], &stn_id))
		{
			sprintf(err_str, "No such station in %s: %s", keyword, strbuf[0]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		if(-1==GetGroupIdByName(stn_id, strbuf[1], &dev_id))
		{
			sprintf(err_str, "No such group in %s: %s.%s", keyword, strbuf[0], strbuf[1]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(j=2; j<str_num; j++)
		{
			if(strlen(strbuf[j])>=MAX_IPADDR_SIZE)
			{
				sprintf(err_str, "IP addr too long in %s: %s", keyword, strbuf[j]);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
			if(-1==inet_addr(strbuf[j]))
			{ 
				sprintf(err_str, "lcu ipaddr %s error in %s", strbuf[j], keyword);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}

			strcpy(config_info.lcu_ipaddr[config_info.lcu_ipaddr_num].ipaddr[j-2], strbuf[j]);
		}
		config_info.lcu_ipaddr[config_info.lcu_ipaddr_num].stn_id= stn_id;
		config_info.lcu_ipaddr[config_info.lcu_ipaddr_num].dev_id= dev_id;
		config_info.lcu_ipaddr[config_info.lcu_ipaddr_num].ipaddr_num= str_num-2;
		config_info.lcu_ipaddr_num++;
	}

	
	/* KEYWORD=="PLC_DOUBLECPU" */
	strcpy(keyword, "PLC_DOUBLECPU");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	config_info.plc_dcpu_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num!=2)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(config_info.plc_dcpu_num>=MAX_ALL_LCU_NUM)
		{
			sprintf(err_str, "Too much plc in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(-1==GetStnIdByName(strbuf[0], &stn_id))
		{
			sprintf(err_str, "No such station in %s: %s", keyword, strbuf[0]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		if(-1==GetGroupIdByName(stn_id, strbuf[1], &dev_id))
		{
			sprintf(err_str, "No such group in %s: %s.%s", keyword, strbuf[0], strbuf[1]);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		config_info.plc_dcpu[config_info.plc_dcpu_num].stn_id= stn_id;
		config_info.plc_dcpu[config_info.plc_dcpu_num].dev_id= dev_id;
		config_info.plc_dcpu_num++;

	}


	/* KEYWORD=="CMD_HOST" */
	strcpy(keyword, "CMD_HOST");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	config_info.cmd_host_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num!=2)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(config_info.cmd_host_num>=MAX_CMD_HOST_NUM)
		{
			sprintf(err_str, "Too much cmd host define in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		for(i=0; i<config_info.host_num; i++)
		{
			if(!strcmp(strbuf[0], config_info.host[i].name))
				break;
		}
		if(i>=config_info.host_num)
		{
			sprintf(err_str, "No such host in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		value= 0;
		sscanf(strbuf[1], "%d", &value);
		if(value<1 || value>MAX_CMD_HOST_NUM)
		{
			sprintf(err_str, "Command host id error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		config_info.cmd_host[config_info.cmd_host_num].host_id= (UCHAR)(i+1);
		config_info.cmd_host[config_info.cmd_host_num].cmd_id= (UCHAR)value;
		config_info.cmd_host_num++;
	}


	/* KEYWORD=="CASCADE" */
	strcpy(keyword, "CASCADE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(-1==GetStnIdByName(strbuf[0], &stn_id))
		{
			config_info.dispatch_name[0]= 0;
			sprintf(err_str, "cascade station name error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		strcpy(config_info.dispatch_name, strbuf[0]);
		break;
	}


	/* KEYWORD=="EMS_PARAMETER" */
	strcpy(keyword, "EMS_PARAMETER");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num<2)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(!strcmp(strbuf[0], "SYS_NET_NUM"))
		{
			sscanf(strbuf[1], "%d", &config_info.sys_net_num);
			if(config_info.sys_net_num!=1 && config_info.sys_net_num!=2)
			{
				sprintf(err_str, "Sys net num error in %s: %s", keyword, str);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
		}
		else if(!strcmp(strbuf[0], "CTRL_NET_NUM"))
		{
			sscanf(strbuf[1], "%d", &config_info.ctrl_net_num);
			if(config_info.ctrl_net_num!=1 && config_info.ctrl_net_num!=2)
			{
				sprintf(err_str, "Ctrl net num error in %s: %s", keyword, str);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
		}
		else
		{
			sprintf(err_str, "Unknown define in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
	}


	/* KEYWORD=="EMS_FLAG" */
	config_info.event_file_log= TRUE;
	config_info.power_adj_log= TRUE;
	config_info.ctrl_net_independ= FALSE;
	config_info.io_force_disable= TRUE;
	config_info.dev_obj_disable= TRUE;
	config_info.his_rdb_disable= TRUE;

	strcpy(keyword, "EMS_FLAG");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")))	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num==1)
			flag= TRUE;
		else if(str_num==2)
		{
			if(!strcmp(strbuf[1], "YES"))
				flag= TRUE;
			else
				flag= FALSE;
		}
		else
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(!strcmp(strbuf[0], "EVENT_FILE_LOG"))
			config_info.event_file_log= flag;
		else if(!strcmp(strbuf[0], "POWER_ADJ_LOG"))
			config_info.power_adj_log= flag;
		else if(!strcmp(strbuf[0], "CTRL_NET_INDEPEND"))
			config_info.ctrl_net_independ= flag;
		else if(!strcmp(strbuf[0], "IO_FORCE_DISLABLE"))
			config_info.io_force_disable= flag;
		else if(!strcmp(strbuf[0], "DEV_OBJ_DISABLE"))
			config_info.dev_obj_disable= flag;
		else if(!strcmp(strbuf[0], "HIS_RDB_DISABLE"))
			config_info.his_rdb_disable= flag;
		else
		{
			sprintf(err_str, "Unknown define in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

	}


	/* KEYWORD=="EMS_VARIABLE" */
	strcpy(keyword, "EMS_VARIABLE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	config_info.var_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num==1)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(config_info.var_num>=MAX_VARIABLE_NUM)
		{
			sprintf(err_str, "Too much variable define in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(strlen(strbuf[0])>=MAX_VARIABLE_NAME_SIZE)
		{
			sprintf(err_str, "variable name too long in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		strcpy(config_info.variable[config_info.var_num].name, strbuf[0]);
		if(1!=sscanf(strbuf[1], "%f", &config_info.variable[config_info.var_num].value))
		{
			sprintf(err_str, "variable value error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		config_info.var_num++;
	}

	/* KEYWORD=="EMS_STRING" */
	strcpy(keyword, "EMS_STRING");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	config_info.string_num= 0;
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( (strstr(str, "[") && strstr(str, "]")) || strstr(str, "END") )	
			break;
		
		if(-1==(str_num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(str_num==0)
			continue;

		if(str_num==1)
		{
			sprintf(err_str, "Format error in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(config_info.string_num>=MAX_STRING_NUM)
		{
			sprintf(err_str, "Too much string define in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}

		if(strlen(strbuf[0])>=MAX_STRING_NAME_SIZE)
		{
			sprintf(err_str, "string name too long in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		if(strlen(strbuf[1])>=MAX_STRING_VALUE_SIZE)
		{
			sprintf(err_str, "string value too long in %s: %s", keyword, str);
			Err_log(err_str);
			fclose(fp);
			return -1;
		}
		strcpy(config_info.string[config_info.string_num].name, strbuf[0]);
		strcpy(config_info.string[config_info.string_num].value, strbuf[1]);
		config_info.string_num++;
	}

	fclose(fp);
	return 0;
}


int GetSysHostNum(void)
{
	return config_info.host_num;
}


int	GetHostStnId(char *hostname, UCHAR *stn_id)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
		{
			*stn_id= config_info.host[i].stn_id;
			return 0;
		}
	}
	sprintf(err_str, "No host define: %s", hostname);
	Err_log(err_str);
	return -1;
}


int	GetLocalHostStnId(UCHAR	*stn_id)
{
	char	hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return -1;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	return GetHostStnId(hostname, stn_id);
}


int	GetHostId(char *hostname, UCHAR *host_id)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
		{
			*host_id= (UCHAR)(i+1);
			return 0;
		}
	}
	sprintf(err_str, "No host define: %s", hostname);
	Err_log(err_str);
	return -1;
}


int	GetHostType(char *hostname, char *type)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
		{
			strcpy(type, config_info.host[i].type);
			return 0;
		}
	}
	sprintf(err_str, "No host define: %s", hostname);
	Err_log(err_str);
	return -1;
}


int	GetHostDomain(char *hostname, char *domain)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
		{
			strcpy(domain, config_info.host[i].domain);
			return 0;
		}
	}
	sprintf(err_str, "No host define: %s", hostname);
	Err_log(err_str);
	return -1;
}


BOOL	IsColorPrinter(char *hostname)
{
	char	type[MAX_HOST_TYPE_SIZE];

	if(-1==GetHostType(hostname, type))
		return FALSE;  

	if(strcmp(type, "COLOR_PRT"))
		return FALSE;
	return TRUE;
}


int	GetHostDisk(char *hostname, char *disk)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
		{
			*disk= config_info.host[i].disk;
			return 0;
		}
	}
	sprintf(err_str, "No host define: %s", hostname);
	Err_log(err_str);
	return -1;
}


int	GetLocalHostId(UCHAR *host_id)
{
	char	hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return -1;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	return GetHostId(hostname, host_id);
}


int	GetHostNameById(int host_id, char *hostname)
{
	char	err_str[128];

	if(host_id<1 || host_id>config_info.host_num)
	{
		sprintf(err_str, "No such host id: %d", host_id);
		Err_log(err_str);
		return -1;
	}
	strcpy(hostname, config_info.host[host_id-1].name);
	return 0;
}


int	GetBroadcastIpaddr(int netNo, char *ipaddr)
{
	char	err_str[128];
	
	if(netNo<0 || netNo>=MAX_HOST_IPADDR_NUM)
	{
		sprintf(err_str, "BROADCAST netNo overrange: %d", netNo);
		Err_log(err_str);
		return -1;
	}

	if(netNo>=config_info.broadcast_num)
	{
		sprintf(err_str, "No such netNo define in BROADCAST: %d", netNo);
		Err_log(err_str);
		return -1;
	}
	strcpy(ipaddr, config_info.broadcast[netNo]);
	return 0;
}


int	GetHostIpaddrNum(char *hostname, int *ipaddrNum)
{
	int		i;
	char	err_str[128];
	
	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
			break;
	}
	if(i>=config_info.host_num)
	{
		sprintf(err_str, "No such host: %s", hostname);
		Err_log(err_str);
		return -1;
	}

	*ipaddrNum= config_info.host[i].ipaddr_num;
	return 0;
}




int	GetHostIpaddr(char *hostname, int netNo, char *ipaddr)
{
	int		i;
	char	err_str[128];
	
	if(netNo<0 || netNo>=MAX_HOST_IPADDR_NUM)
	{
		sprintf(err_str, "netNo overrange: %d", netNo);
		Err_log(err_str);
		return -1;
	}

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
			break;
	}
	if(i>=config_info.host_num)
	{
		sprintf(err_str, "No such host: %s", hostname);
		Err_log(err_str);
		return -1;
	}

	if(netNo>=config_info.host[i].ipaddr_num)
	{
		sprintf(err_str, "No such netNo define in %s: %d", hostname, netNo);
		Err_log(err_str);
		return -1;
	}
	strcpy(ipaddr, config_info.host[i].ipaddr[netNo]);
	return 0;
}



BOOL IsServerStation(void)
{
	int		i, j;
	char	hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return FALSE;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, "SERVER"))
		{
			for(j=0; j<config_info.host_role[i].host_num; j++)
			{
				if(!strcmp(hostname, config_info.host_role[i].host[j]))
					return TRUE;
			}
		}
	}
	return FALSE;
}


BOOL IsOperatorStation(void)
{
	int		i, j;
	char	hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return FALSE;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, "OPERATOR"))
		{
			for(j=0; j<config_info.host_role[i].host_num; j++)
			{
				if(!strcmp(hostname, config_info.host_role[i].host[j]))
					return TRUE;
			}
		}
	}
	return FALSE;
}



int GetLocalHostPrvg(char *prvg, BOOL *own)
{
	int		i, j;
	char	hostname[MAX_HOST_NAME_SIZE];
	char	err_str[128];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return -1;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, prvg))
		{
			for(j=0; j<config_info.host_role[i].host_num; j++)
			{
				if(!strcmp(hostname, config_info.host_role[i].host[j]))
				{
					*own= TRUE;
					return 0;
				}
			}
			*own= FALSE;
			return 0;
		}
	}
	sprintf(err_str, "Fail to find prvg %s", prvg);
	Err_log(err_str);
	return -1;
}


int	GetAlmPrinterNum(void)
{
	int		i;

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, "ALM_PRINTER"))
			return config_info.host_role[i].host_num;
	}
	return 0;
}


int	GetAlmPrinter(int printerNo, char *printer)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, "ALM_PRINTER"))
		{
			if(printerNo<0||printerNo>=config_info.host_role[i].host_num)
			{
				sprintf(err_str, "No such printerNo define in ALM_PRINTER: %d", printerNo);
				Err_log(err_str);
				return -1;
			}
			strcpy(printer, config_info.host_role[i].host[printerNo]);
			return 0;
		}
	}
	return -1;
}


int	GetHcopyPrinterNum(void)
{
	int		i;

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, "HCOPY_PRINTER"))
			return config_info.host_role[i].host_num;
	}
	return 0;
}


int	GetHcopyPrinter(int printerNo, char *printer)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, "HCOPY_PRINTER"))
		{
			if(printerNo<0||printerNo>=config_info.host_role[i].host_num)
			{
				sprintf(err_str, "No such printerNo define in HCOPY_PRINTER: %d", printerNo);
				Err_log(err_str);
				return -1;
			}
			strcpy(printer, config_info.host_role[i].host[printerNo]);
			return 0;
		}
	}
	return -1;
}


int	GetRptPrinterNum(void)
{
	int		i;

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, "RPT_PRINTER"))
			return config_info.host_role[i].host_num;
	}
	return 0;
}


int	GetRptPrinter(int printerNo, char *printer)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_role_num; i++)
	{
		if(!strcmp(config_info.host_role[i].role, "RPT_PRINTER"))
		{
			if(printerNo<0||printerNo>=config_info.host_role[i].host_num)
			{
				sprintf(err_str, "No such printerNo define in RPT_PRINTER: %d", printerNo);
				Err_log(err_str);
				return -1;
			}
			strcpy(printer, config_info.host_role[i].host[printerNo]);
			return 0;
		}
	}
	return -1;
}



int	GetRunProcessNum(void)
{
	int		i;
	char	err_str[128];
	char	hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return -1;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
			break;
	}
	if(i>=config_info.host_num)
	{
		sprintf(err_str, "No such host define: %s", hostname);
		Err_log(err_str);
		return -1;
	}

	return config_info.host[i].process_num+config_info.base_process_num;
}


int	GetRunProcess(int procNo, char *process)
{
	int		i;
	char	err_str[128];
	char	hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return -1;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
			break;
	}
	if(i>=config_info.host_num)
	{
		sprintf(err_str, "No such host define: %s", hostname);
		Err_log(err_str);
		return -1;
	}

	if(procNo<0 || procNo>=(config_info.host[i].process_num+config_info.base_process_num))
	{
		sprintf(err_str, "No such process in %s: procNo %d", hostname, procNo);
		Err_log(err_str);
		return -1;
	}
	if(procNo<config_info.base_process_num)
		strcpy(process, config_info.base_process[procNo]);
	else
		strcpy(process, config_info.host[i].process[procNo-config_info.base_process_num]);
	return 0;
}


int	GetRcpHostNum(void)
{
	return config_info.rcp_host_num;
}


int	GetRcpHostName(int hostNo, char *hostname)
{
	char	err_str[128];

	if(hostNo<0 || hostNo>=config_info.rcp_host_num)
	{
		sprintf(err_str, "No such rcp host: hostNo %d", hostNo);
		Err_log(err_str);
		return -1;
	}
	strcpy(hostname, config_info.rcp_host[hostNo]);
	return 0;
}


int	GetDhswHostName(char *hostname)
{
	int		i;
	char	local_hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(local_hostname, sizeof(local_hostname)))
	{
		Err_log("Fail to get local host name");
		return -1;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	for(i=0; i<config_info.host_pair_num; i++)
	{
		if(!strcmp(local_hostname, config_info.host_pair[i].host[0]))
		{
			strcpy(hostname, config_info.host_pair[i].host[1]);
			return 0;
		}
		else if(!strcmp(local_hostname, config_info.host_pair[i].host[1]))
		{
			strcpy(hostname, config_info.host_pair[i].host[0]);
			return 0;
		}
	}
	return -1;
}



int	GetTimeServerNum(void)
{
	return config_info.time_server_num;
}


int	GetTimeServer(int hostNo, char *hostname, char *port, char *gps_type)
{
	char	err_str[128];

	if(hostNo<0 || hostNo>=config_info.time_server_num)
	{
		sprintf(err_str, "No such time server: hostNo %d", hostNo);
		Err_log(err_str);
		return -1;
	}
	strcpy(hostname, config_info.time_server[hostNo].host);
	strcpy(port, config_info.time_server[hostNo].port);
	strcpy(gps_type, config_info.time_server[hostNo].gps_type);
	return 0;
}


int	GetPlcScanHostNum(UCHAR stn_id, UCHAR dev_id)
{
	int		i;
	char	err_str[128];


	for(i=0; i<config_info.plc_scan_num; i++)
	{
		if(config_info.plc_scan[i].stn_id==stn_id
			&& config_info.plc_scan[i].dev_id==dev_id)
			break;
	}
	if(i>=config_info.plc_scan_num)
	{
		sprintf(err_str, "No such plc scan define: %d-%d", stn_id, dev_id);
		Err_log(err_str);
		return -1;
	}

	return config_info.plc_scan[i].host_num;
}



int	GetPlcScanHost(UCHAR stn_id, UCHAR dev_id, int hostNo, char *hostname)
{
	int		i;
	char	err_str[128];


	for(i=0; i<config_info.plc_scan_num; i++)
	{
		if(config_info.plc_scan[i].stn_id==stn_id
			&& config_info.plc_scan[i].dev_id==dev_id)
			break;
	}
	if(i>=config_info.plc_scan_num)
	{
		sprintf(err_str, "No such plc scan define: %d-%d", stn_id, dev_id);
		Err_log(err_str);
		return -1;
	}

	if(hostNo<0 || hostNo>=config_info.plc_scan[i].host_num)
	{
		sprintf(err_str, "No such plc scan hostNo: %d-%d %d", stn_id, dev_id, hostNo);
		Err_log(err_str);
		return -1;
	}
	
	strcpy(hostname, config_info.plc_scan[i].host[hostNo]);
	return 0;
}


int	GetLcuIpaddrNum(UCHAR stn_id, UCHAR dev_id)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.lcu_ipaddr_num; i++)
	{
		if(config_info.lcu_ipaddr[i].stn_id==stn_id
			&& config_info.lcu_ipaddr[i].dev_id==dev_id)
			break;
	}
	if(i>=config_info.lcu_ipaddr_num)
	{
		sprintf(err_str, "No such lcu ipaddr define: %d-%d", stn_id, dev_id);
		Err_log(err_str);
		return -1;
	}

	return config_info.lcu_ipaddr[i].ipaddr_num;
}



int	GetLcuIpaddr(UCHAR stn_id, UCHAR dev_id, int netNo, char *ipaddr)
{
	int		i;
	char	err_str[128];
	
	if(netNo<0 || netNo>3)
	{
		sprintf(err_str, "netNo overrange: %d", netNo);
		Err_log(err_str);
		return -1;
	}

	for(i=0; i<config_info.lcu_ipaddr_num; i++)
	{
		if(stn_id==config_info.lcu_ipaddr[i].stn_id
			&& dev_id==config_info.lcu_ipaddr[i].dev_id)
			break;
	}
	if(i>=config_info.lcu_ipaddr_num)
	{
		sprintf(err_str, "No such lcu: %d-%d", stn_id, dev_id);
		Err_log(err_str);
		return -1;
	}

	if(netNo>=config_info.lcu_ipaddr[i].ipaddr_num)
	{
		sprintf(err_str, "No such netNo define in lcu %d-%d: %d", stn_id, dev_id, netNo);
		Err_log(err_str);
		return -1;
	}
	strcpy(ipaddr, config_info.lcu_ipaddr[i].ipaddr[netNo]);
	return 0;
}



BOOL	IsPlcDoubleCpu(UCHAR stn_id, UCHAR dev_id)
{
	int		i;
	
	for(i=0; i<config_info.plc_dcpu_num; i++)
	{
		if(stn_id==config_info.plc_dcpu[i].stn_id
			&& dev_id==config_info.plc_dcpu[i].dev_id)
			return TRUE;
	}
	return FALSE;
}


int	GetSysNetNum(void)
{
	return config_info.sys_net_num;
}


int	GetCtrlNetNum(void)
{
	return config_info.ctrl_net_num;
}


BOOL	IsCtrlNetIndepend(void)
{
	return config_info.ctrl_net_independ;
}


BOOL	IsEventFileLog(void)
{
	return config_info.event_file_log;
}


BOOL	IsPowerAdjLog(void)
{
	return config_info.power_adj_log;
}


BOOL	IsIoForceDisable(void)
{
	return config_info.io_force_disable;
}


BOOL	IsDevObjDisable(void)
{
	return config_info.dev_obj_disable;
}


BOOL	IsHisRdbDisable(void)
{
	return config_info.his_rdb_disable;
}


BOOL	IsHostNoHardDisk(void)
{
	char	hostname[MAX_HOST_NAME_SIZE];
	char	type[MAX_HOST_TYPE_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return FALSE;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	if(-1==GetHostType(hostname, type))
		return FALSE;

	if(strcmp(type, "WINDOWS_NHD"))
		return FALSE;
	return TRUE;
}


BOOL	IsHostIsland(void)
{
	char	hostname[MAX_HOST_NAME_SIZE];
	char	type[MAX_HOST_TYPE_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return FALSE;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	if(-1==GetHostType(hostname, type))
		return FALSE;

	if(strcmp(type, "WIN_ISLAND"))
		return FALSE;
	return TRUE;
}


int	GetEmsVariable(char *variable, float *value)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.var_num; i++)
	{
		if(!strcmp(variable, config_info.variable[i].name))
		{
			*value= config_info.variable[i].value;
			return 0;
		}
	}
	sprintf(err_str, "Fail to find variable %s", variable);
	Err_log(err_str);
	return -1;
}



int	GetEmsString(char *strName, char *strValue)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.string_num; i++)
	{
		if(!strcmp(strName, config_info.string[i].name))
		{
			strcpy(strValue, config_info.string[i].value);
			return 0;
		}
	}
	sprintf(err_str, "Fail to find ems string %s", strName);
	Err_log(err_str);
	return -1;
}


int	GetSystemPassword(BOOL isUnix, char *password)
{
	int		i;
	char	pdName[256];

	if(isUnix)
		strcpy(pdName, "UNIX_PASSWORD");
	else
		strcpy(pdName, "WIN_PASSWORD");

	for(i=0; i<config_info.string_num; i++)
	{
		if(!strcmp(pdName, config_info.string[i].name))
		{
			strcpy(password, config_info.string[i].value);
			return 0;
		}
	}
	strcpy(password, "H9000ems");
	return 0;
}

int GetSysLcuId(UCHAR stn_id, UCHAR *dev_id)
{
	return GetStnSysLcuIdById(stn_id, dev_id);
}


int	GetHostSysLcuId(char *hostname, UCHAR *dev_id)
{
	UCHAR		stn_id;

	if(-1==GetHostStnId(hostname, &stn_id))
		return -1;

	return GetSysLcuId(stn_id, dev_id);
}




int	GetLocalSysLcuId(UCHAR *dev_id)
{
	char	hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return -1;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	return GetHostSysLcuId(hostname, dev_id);
}



int	GetHostCmdId(UCHAR host_id, UCHAR *cmd_id)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.cmd_host_num; i++)
	{
		if(config_info.cmd_host[i].host_id==host_id)
		{
			*cmd_id= config_info.cmd_host[i].cmd_id;
			return 0;
		}
	}
	sprintf(err_str, "No host %d cmd id define", host_id);
	Err_log(err_str);
	return -1;
}



int GetHostStatePtId(char *hostname, POINTER *point)
{
	char		err_str[128];
	char		entryName[ENTRY_NAME_SIZE];

	if((strlen(hostname)+strlen(HOST_STATE_NAME_DEFINE))>=ENTRY_NAME_SIZE)
	{
		sprintf(err_str, "Host name too long: %s", hostname);
		Err_log(err_str);
		return -1;
	}

	if(-1==GetHostStnId(hostname, &point->stn_id))
		return -1;

	if(-1==GetHostSysLcuId(hostname, &point->dev_id))
		return -1;

	point->data_type= SYSMSG_TYPE;
	sprintf(entryName, "%s%s", hostname, HOST_STATE_NAME_DEFINE);

	if(-1==GetEntryIdByName(entryName, point))
		return -1;

	return 0;
}


int GetHostStatePtNameStr(char *hostname, char *ptNameStr)
{
	POINTER		point;

	if(-1==GetHostStatePtId(hostname, &point))
		return -1;

	return GetPtNameById(&point, ptNameStr, NULL);
}


int GetHostState(char *hostname, USHORT *state)
{
	POINTER		point;

	if(-1==GetHostStatePtId(hostname, &point))
		return -1;
	
	return GetIndPtStateById(&point, state);
}



int GetLocalHostState(USHORT *state)
{
	POINTER		point;
	char		hostname[MAX_HOST_NAME_SIZE];

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log("Fail to get host name");
		return -1;
	}
#ifdef WINDOWS_OS
	strupr(hostname);
#endif

	if(-1==GetHostStatePtId(hostname, &point))
		return -1;
	
	return GetIndPtStateById(&point, state);
}



int GetHostSysNetStatePtId(char *hostname, USHORT net_no, POINTER *point)
{
	char		err_str[128];
	char		entryName[ENTRY_NAME_SIZE];

	if(net_no>1)
	{
		sprintf(err_str, "Sys net id error: %d", net_no);
		Err_log(err_str);
		return -1;
	}

	if((strlen(hostname)+strlen(SYSNET_STATE_NAME_DEFINE))>=ENTRY_NAME_SIZE)
	{
		sprintf(err_str, "Host name too long: %s", hostname);
		Err_log(err_str);
		return -1;
	}

	if(-1==GetHostStnId(hostname, &point->stn_id))
		return -1;

	if(-1==GetHostSysLcuId(hostname, &point->dev_id))
		return -1;

	point->data_type= SYSMSG_TYPE;
	sprintf(entryName, "%s%s%c", hostname, SYSNET_STATE_NAME_DEFINE, net_no+'A');

	if(-1==GetEntryIdByName(entryName, point))
		return -1;

	return 0;
}


int GetHostSysNetStatePtNameStr(char *hostname, USHORT net_no, char *ptNameStr)
{
	POINTER		point;

	if(-1==GetHostSysNetStatePtId(hostname, net_no, &point))
		return -1;

	return GetPtNameById(&point, ptNameStr, NULL);
}


int GetHostSysNetState(char *hostname, USHORT net_no, USHORT *state)
{
	POINTER		point;

	if(-1==GetHostSysNetStatePtId(hostname, net_no, &point))
		return -1;
	
	return GetIndPtStateById(&point, state);
}



int GetHostCtrlNetStatePtId(char *hostname, USHORT net_no, POINTER *point)
{
	char		err_str[128];
	char		entryName[ENTRY_NAME_SIZE];

	if(!config_info.ctrl_net_independ)
		return GetHostSysNetStatePtId(hostname, net_no, point);

	if(net_no>1)
	{
		sprintf(err_str, "Ctrl net id error: %d", net_no);
		Err_log(err_str);
		return -1;
	}

	if((strlen(hostname)+strlen(CTRLNET_STATE_NAME_DEFINE))>=ENTRY_NAME_SIZE)
	{
		sprintf(err_str, "Host name too long: %s", hostname);
		Err_log(err_str);
		return -1;
	}

	if(-1==GetHostStnId(hostname, &point->stn_id))
		return -1;

	if(-1==GetHostSysLcuId(hostname, &point->dev_id))
		return -1;

	point->data_type= SYSMSG_TYPE;
	sprintf(entryName, "%s%s%c", hostname, CTRLNET_STATE_NAME_DEFINE, net_no+'A');

	if(-1==GetEntryIdByName(entryName, point))
		return -1;

	return 0;
}


int GetHostCtrlNetStatePtNameStr(char *hostname, USHORT net_no, char *ptNameStr)
{
	POINTER		point;

	if(-1==GetHostCtrlNetStatePtId(hostname, net_no, &point))
		return -1;

	return GetPtNameById(&point, ptNameStr, NULL);
}


int GetHostCtrlNetState(char *hostname, USHORT net_no, USHORT *state)
{
	POINTER		point;

	if(-1==GetHostCtrlNetStatePtId(hostname, net_no, &point))
		return -1;
	
	return GetIndPtStateById(&point, state);
}


int	GetDispatchStnName(char *StnName)
{
	char	err_str[128];

	if(config_info.dispatch_name[0])
	{
		strcpy(StnName, config_info.dispatch_name);
		return 0;
	}
	sprintf(err_str, "No dispatch station define");
	Err_log(err_str);
	StnName[0]= 0;
	return -1;
}


BOOL IsLocalHostDispatch(void)
{
	UCHAR	stn_id, stnId;
	char	stnName[STATION_NAME_SIZE];

	if(-1==GetLocalHostStnId(&stn_id))
		return FALSE;
	
	if(-1==GetDispatchStnName(stnName))
		return FALSE;

	if(-1==GetStnIdByName(stnName, &stnId))
		return FALSE;

	if(stn_id==stnId)
		return TRUE;
	else
		return FALSE;
}


BOOL	IsHostStateManage(char *hostname)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
			return config_info.host[i].state_manage_flag;
	}
	sprintf(err_str, "No host define: %s", hostname);
	Err_log(err_str);
	return FALSE;
}


int	GetHostLongname(char *hostname, char *longname)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
		{
			strcpy(longname, config_info.host[i].longname);
			return 0;
		}
	}
	sprintf(err_str, "No host define: %s", hostname);
	Err_log(err_str);
	return -1;
}








