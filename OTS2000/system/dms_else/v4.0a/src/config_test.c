


#include 	"../inc/dms_com.h"
#include	"../inc/ems_init_def.h"



int	GetHostIdByName(char *hostname, int *host_id)
{
	int		i;
	char	err_str[128];

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
		{
			*host_id= i+1;
			return 0;
		}
	}
	sprintf(err_str, "No such host: %s", hostname);
	Err_log(DMS_MODULE, err_str);
	return -1;
}


int	GetHostNameById(int host_id, char *hostname)
{
	char	err_str[128];

	if(host_id<1 || host_id>config_info.host_num)
	{
		sprintf(err_str, "No such host id: %d", host_id);
		Err_log(DMS_MODULE, err_str);
		return -1;
	}
	strcpy(hostname, config_info.host[host_id-1].name);
	return 0;
}


int	GetBroadcastIpaddr(int netNo, char *ipaddr)
{
	char	err_str[128];
	
	if(netNo<0 || netNo>3)
	{
		sprintf(err_str, "netNo overrange: %d", netNo);
		Err_log(DMS_MODULE, err_str);
		return -1;
	}

	if(netNo>=config_info.broadcast_num)
	{
		sprintf(err_str, "No such netNo define in BROADCAST: %d", netNo);
		Err_log(DMS_MODULE, err_str);
		return -1;
	}
	strcpy(ipaddr, config_info.broadcast[netNo]);
	return 0;
}



int	GetHostIpaddr(char *hostname, int netNo, char *ipaddr)
{
	int		i;
	char	err_str[128];
	
	if(netNo<0 || netNo>3)
	{
		sprintf(err_str, "netNo overrange: %d", netNo);
		Err_log(DMS_MODULE, err_str);
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
		Err_log(DMS_MODULE, err_str);
		return -1;
	}

	if(netNo>=config_info.host[i].ipaddr_num)
	{
		sprintf(err_str, "No such netNo define in %s: %d", hostname, netNo);
		Err_log(DMS_MODULE, err_str);
		return -1;
	}
	strcpy(ipaddr, config_info.host[i].ipaddr[netNo]);
	return 0;
}



BOOL IsServerStation(void)
{
	int		i, j;
	char	hostname[MAX_HOST_NAME_SIZE];

#ifdef	WINDOWS_OS
	WSADATA		WSAData;

	if(WSAStartup(MAKEWORD(2,2), &WSAData))
	{
		Err_log(DMS_MODULE, "Fail to initialize net");
		return FALSE;
	}
#endif

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log(DMS_MODULE, "Fail to get host name");
		return FALSE;
	}

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

#ifdef	WINDOWS_OS
	WSADATA		WSAData;

	if(WSAStartup(MAKEWORD(2,2), &WSAData))
	{
		Err_log(DMS_MODULE, "Fail to initialize net");
		return FALSE;
	}
#endif
	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log(DMS_MODULE, "Fail to get host name");
		return FALSE;
	}

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


int	GetRunProcessNum(void)
{
	int		i;
	char	err_str[128];
	char	hostname[MAX_HOST_NAME_SIZE];

#ifdef	WINDOWS_OS
	WSADATA		WSAData;

	if(WSAStartup(MAKEWORD(2,2), &WSAData))
	{
		Err_log(DMS_MODULE, "Fail to initialize net");
		return -1;
	}
#endif

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log(DMS_MODULE, "Fail to get host name");
		return -1;
	}

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
			break;
	}
	if(i>=config_info.host_num)
	{
		sprintf(err_str, "No such host processes define: %s", hostname);
		Err_log(DMS_MODULE, err_str);
		return -1;
	}

	return config_info.host[i].process_num;
}


int	GetRunProcess(int procNo, char *processName)
{
	int		i;
	char	err_str[128];
	char	hostname[MAX_HOST_NAME_SIZE];

#ifdef	WINDOWS_OS
	WSADATA		WSAData;

	if(WSAStartup(MAKEWORD(2,2), &WSAData))
	{
		Err_log(DMS_MODULE, "Fail to initialize net");
		return -1;
	}
#endif

	if(gethostname(hostname, sizeof(hostname)))
	{
		Err_log(DMS_MODULE, "Fail to get host name");
		return -1;
	}

	for(i=0; i<config_info.host_num; i++)
	{
		if(!strcmp(hostname, config_info.host[i].name))
			break;
	}
	if(i>=config_info.host_num)
	{
		sprintf(err_str, "No such host processes define: %s", hostname);
		Err_log(DMS_MODULE, err_str);
		return -1;
	}

	if(procNo<0 || procNo>=config_info.host[i].process_num)
	{
		sprintf(err_str, "No such process: procNo %d", procNo);
		Err_log(DMS_MODULE, err_str);
		return -1;
	}

	strcpy(processName, config_info.host[i].process[procNo]);
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
		Err_log(DMS_MODULE, err_str);
		return -1;
	}
	strcpy(hostname, config_info.rcp_host[hostNo]);
	return 0;
}


int	GetDhswHostName(char *hostname)
{
	int		i;
	char	local_hostname[MAX_HOST_NAME_SIZE];

#ifdef	WINDOWS_OS
	WSADATA		WSAData;

	if(WSAStartup(MAKEWORD(2,2), &WSAData))
	{
		Err_log(DMS_MODULE, "Fail to initialize net");
		return -1;
	}
#endif

	if(gethostname(local_hostname, sizeof(local_hostname)))
	{
		Err_log(DMS_MODULE, "Fail to get local host name");
		return -1;
	}

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


int	GetTimeServer(int hostNo, char *hostname, char *port)
{
	char	err_str[128];

	if(hostNo<0 || hostNo>=config_info.time_server_num)
	{
		sprintf(err_str, "No such time server: hostNo %d", hostNo);
		Err_log(DMS_MODULE, err_str);
		return -1;
	}
	strcpy(hostname, config_info.time_server[hostNo].host);
	strcpy(port, config_info.time_server[hostNo].port);
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
		Err_log(DMS_MODULE, err_str);
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
		Err_log(DMS_MODULE, err_str);
		return -1;
	}

	if(hostNo<0 || hostNo>=config_info.plc_scan[i].host_num)
	{
		sprintf(err_str, "No such plc scan hostNo: %d-%d %d", stn_id, dev_id, hostNo);
		Err_log(DMS_MODULE, err_str);
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
		Err_log(DMS_MODULE, err_str);
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
		Err_log(DMS_MODULE, err_str);
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
		Err_log(DMS_MODULE, err_str);
		return -1;
	}

	if(netNo>=config_info.lcu_ipaddr[i].ipaddr_num)
	{
		sprintf(err_str, "No such netNo define in lcu %d-%d: %d", stn_id, dev_id, netNo);
		Err_log(DMS_MODULE, err_str);
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
	return config_info.net_num;
}


int GetAlmPrinter(char *almPrinter)
{
	strcpy(almPrinter, config_info.alm_printer);
	return 0;
}



int GetRptPrinter(char *rptPrinter)
{
	strcpy(rptPrinter, config_info.rpt_printer);
	return 0;
}


int GetHcopyPrinter(char *hcopyPrinter)
{
	strcpy(hcopyPrinter, config_info.hcopy_printer);
	return 0;
}


BOOL	IsEventFileLog(void)
{
	return config_info.event_file_log;
}


BOOL	IsPowerAdjLog(void)
{
	return config_info.power_adj_log;
}





int	main(void)
{
	char	err_str[128];
	int		i, j, k, num;
	UCHAR	stn_id, dev_id;
	char	ipaddr[MAX_IPADDR_SIZE];
	int		host_id;
	char	hostname[MAX_HOST_NAME_SIZE];
	char	port[MAX_PORT_NAME_SIZE];

	if(-1==MountDB(DATABASE_NAME))
	{
		Err_log(DPS_MODULE, "Fail to MountDB!\n");
		exit(-1);
	}
	
	if(-1==(num=GetLcuIpaddrNum(1, 1)))
		printf("\nError");
	else
		printf("\n%d", num);

//	printf("\n%d", GetRcpHostNum());
GetRptPrinter(hostname);
	printf("\n%d", IsPowerAdjLog());
	return 0;
}


