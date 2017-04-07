#include 	<signal.h>

#include 	"../../../dms/v4.0a/inc/dms_com.h"
#include 	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include 	"../../../dps/v4.0a/inc/dps_type_def.h"
#include 	"../../../dps/v4.0a/inc/ipc_common.h"
#include    "../../../pas/v4.0a/inc/pdc_def.h"

#ifdef		SUN_OS
#include 	<sys/socket.h>  /* for AF_INET        */
#include 	<netinet/in.h>  /* for struct in_addr */
#include 	<arpa/inet.h>   /* for inet_ntoa()    */
#include 	<sys/types.h>
#include 	<sys/ioctl.h>
#include	<fcntl.h>
#include 	<netdb.h>       /* for struct hostent */
#include	<errno.h>
#include 	<pthread.h>

#endif

#if defined(SUN) && defined(_AIX)
#include	<sys/wait.h>
#elif defined(SUN)
#include 	<sys/filio.h>
#include	<wait.h>
#endif

#ifdef	WINDOWS_OS
#define		EINPROGRESS			WSAEWOULDBLOCK     
#define 	bzero(addr, size)		memset(addr, 0, size)

#endif


#define SYS_RUN_SERV_TCP_PORT	44444  

char   	dayfilename[60],monthfilename[60];


int	CreateSockNoBlock(void)        
{
	int		ret, sockfd;

	if(-1==(sockfd=socket(AF_INET, SOCK_STREAM, 0)))
	{ 
		Err_log("Fail to create stream socket");
		return -1;
	}

	ret= 1;
#ifdef	WINDOWS_OS
	if( ioctlsocket(sockfd, FIONBIO, &ret) )
#else
	if( -1==ioctl(sockfd, FIONBIO, &ret) )
#endif
	{ 
		Err_log("copy_rpt_data::Fail to set socket NONBLOCK");
		closesocket(sockfd);
		return -1;
	}
	return sockfd;
}


void GetRptFileName()
{
        time_t 	       t2;
	struct tm      t_tm;
	
	t2 = time(NULL);
	t2 = t2 - 2*3600;
	t_tm = *localtime (&t2); 

#ifdef ENGLISH_VER
	sprintf(dayfilename, "HD%02d-%s-%04d.DATA", t_tm.tm_mday, mthstr[t_tm.tm_mon],t_tm.tm_year+1900);
	sprintf(monthfilename, "HM%s-%04d.DATA", mthstr[t_tm.tm_mon],t_tm.tm_year+1900);
#else                
	sprintf(dayfilename, "HD%04d%02d%02d.DATA", t_tm.tm_year+1900,t_tm.tm_mon+1,t_tm.tm_mday);
	sprintf(monthfilename, "HM%04d%02d.DATA", t_tm.tm_year+1900,t_tm.tm_mon+1);
#endif
}


int	RestoreFiles(char *ipaddr, BOOL remote_unix, char local_disk, char remote_disk)
/*int	RestoreFiles(char *rcp_hostname, BOOL remote_unix, char local_disk, char remote_disk)*/

{
	char	command[256];
	char	err_str[256];
	char	ftpfile[128];
	FILE	*fp;
	int		ret,rcp1OK,rcp2OK;
	
	
	GetRptFileName();	
        rcp1OK = rcp2OK = 0;
#ifndef	WINDOWS_OS
	if(remote_unix) /*unix->unix*/
	{
		sprintf(command, "rcp -p %s:%s%s %s", ipaddr, RPT_DATA_PATH_UNIX, dayfilename, RPT_DATA_PATH_UNIX);
		/*sprintf(command, "rcp -p %s:%s%s %s",rcp_hostname, RPT_DATA_PATH_UNIX, dayfilename, RPT_DATA_PATH_UNIX);*/
		printf("\n ---> %s\n", command);		
		ret= system(command);		
		if(ret)  { rcp1OK = 1;  printf("unix->unix: rcp rpt_day_data fail!!\n"); }
		else     printf("unix->unix: rcp rpt_day_data OK!!\n");
		
		sprintf(command, "rcp -p %s:%s%s %s", ipaddr, RPT_DATA_PATH_UNIX, monthfilename, RPT_DATA_PATH_UNIX);
		/*sprintf(command, "rcp -p %s:%s%s %s", rcp_hostname, RPT_DATA_PATH_UNIX, monthfilename, RPT_DATA_PATH_UNIX);*/
		printf("\n ---> %s\n", command);		
		ret= system(command);		
		if(ret) { rcp2OK = 1;  printf("unix->unix: rcp rpt_month_data fail!!\n");}
		else     printf("unix->unix: rcp rpt_month_data OK!!\n");
		
		if(( rcp1OK ==1) || ( rcp2OK ==1)) return -1;            
               
					
	}
	else /*win->unix*/
		return -1;
#else /*WINDOWS_OS*/
	if(remote_unix) /*unix->win*/
	{
		sprintf(ftpfile, "%srptdata.txt", SYS_TMP_PATH);
		if((fp=fopen(ftpfile, "w" )) == NULL )
		{
			sprintf(err_str, "copy_rpt_data::Failed to create DAY temp %s", ftpfile);
			Err_log(err_str);
			return -1;
		}
		fprintf(fp, "open  %s\n", ipaddr);
		fprintf(fp, "ems\n");
		fprintf(fp, "H9000ems\n");
		fprintf(fp, "bin\n");
		fprintf(fp, "get %s%s %s%s\n", RPT_DATA_PATH_UNIX, 
			dayfilename, RPT_DATA_PATH_WIN, dayfilename);
		fprintf(fp, "quit\n");
		fclose(fp); 
		sprintf(command, "ftp -s:%s", ftpfile);
		printf("\n ---> %s\n", command);		
		ret= system(command);		
		if(ret)
			{   printf("copy_rpt_data::unix->win: rcp rpt_day_data fail!!\n");  return -1;  }
			
			
		sprintf(ftpfile, "%srptdata.txt", SYS_TMP_PATH);
		if((fp=fopen(ftpfile, "w" )) == NULL )
		{
			sprintf(err_str, "copy_rpt_data::Failed to create MONTH temp %s", ftpfile);
			Err_log(err_str);
			return -1;
		}
		fprintf(fp, "open  %s\n", ipaddr);
		fprintf(fp, "ems\n");
		fprintf(fp, "H9000ems\n");
		fprintf(fp, "bin\n");
		fprintf(fp, "get %s%s %s%s\n", RPT_DATA_PATH_UNIX, 
			monthfilename, RPT_DATA_PATH_WIN, monthfilename);
		fprintf(fp, "quit\n");
		fclose(fp); 
		sprintf(command, "ftp -s:%s", ftpfile);
		printf("\n ---> %s\n", command);		
		ret= system(command);		
		if(ret)
			{   printf("copy_rpt_data::unix->win: rcp rpt_month_data fail!!\n");  return -1;  }

		
	}
	else /*win->win*/
	{
		sprintf(command, "copy \\\\%s\\%c$%s%s %s", ipaddr, remote_disk, RPT_DATA_PATH_WIN, dayfilename, RPT_DATA_PATH_WIN);
		printf("\n ---> %s\n", command);		
		ret= system(command);		
		if(ret)
			{   printf("copy_rpt_data::win->win: rcp rpt_day_data fail!!\n");  return -1;  }
               
                sprintf(command, "copy \\\\%s\\%c$%s%s %s", ipaddr, remote_disk, RPT_DATA_PATH_WIN, monthfilename, RPT_DATA_PATH_WIN);
		printf("\n ---> %s\n", command);		
		ret= system(command);		
		if(ret)
			{   printf("copy_rpt_data::win->win: rcp rpt_month_data fail!!\n");  return -1;  }
		
	}
#endif
	return 0;
}


void *copy_rpt_data(PDC_TASK *pTask)
{ 
	struct sockaddr_in   serv_addr; 
	int		hostNo, netNo,run_flag,rcpOK;
	char	local_hostname[MAX_HOST_NAME_SIZE];
	char	rcp_hostname[MAX_HOST_NAME_SIZE];
	char	host_type_name[MAX_HOST_TYPE_SIZE];
	BOOL	remote_unix_type;
	char	local_disk, remote_disk;
	char	ipaddr[MAX_IPADDR_SIZE];
	int		link_sock;
	fd_set	wset; 
	struct	timeval tv; 
	int		sys_net_num, rcp_host_num;
	char	err_str[128];
	int		ret;
	char	timestr[64];
	
	for(;;)
	{
		sleep(pTask->check_period);
     if((run_flag = PdcCheck(pTask))!=0) 
		 continue;
	 GetSysTimeStr(timestr);
	printf("\n------RCP_RPT_DATA------%s\n", timestr);
    rcpOK = 0;
	if(gethostname(local_hostname, sizeof(local_hostname)))	
  	{
		Err_log("copy_rpt_data::Fail to gethostname!\n");
		return(0);
  	} 

	if(-1==GetHostDisk(local_hostname, &local_disk))
		return(0);

	sys_net_num= GetSysNetNum();
	rcp_host_num= GetRcpHostNum();
	if(rcp_host_num==0)
	{ 
		Err_log("copy_rpt_data::No define rcp host");
		return(0);
	}

	for(hostNo=0; hostNo<rcp_host_num; hostNo++)
	{
		if(rcpOK == 1) break;
		if(-1==GetRcpHostName(hostNo, rcp_hostname))
			continue;

		if(!strcmp(local_hostname, rcp_hostname))
			continue;

		if(-1==GetHostType(rcp_hostname, host_type_name))
			continue;

		if(!strcmp(host_type_name, "UNIX"))
			remote_unix_type= TRUE;
		else if(!strcmp(host_type_name, "WINDOWS"))
		{
			remote_unix_type= FALSE;
			GetHostDisk(rcp_hostname, &remote_disk);
		}
		else
		{
			sprintf(err_str, "copy_rpt_data::Host type must be UNIX or WINDOWS: %s", host_type_name);
			Err_log(err_str);
			continue;
		}

		for(netNo=0; netNo<sys_net_num; netNo++)
		{       
		        if(rcpOK == 1) break;
		        
			if(-1==GetHostIpaddr(rcp_hostname, netNo, ipaddr))
				break;

			bzero((char *)&serv_addr,sizeof(serv_addr));
			if(-1==(serv_addr.sin_addr.s_addr=inet_addr(ipaddr)))
			{ 
				sprintf(err_str, "copy_rpt_data::Host %s ipaddr %s error", rcp_hostname, ipaddr);
				Err_log(err_str);
				continue;
			}
			serv_addr.sin_family     =AF_INET;
			serv_addr.sin_port       =htons(SYS_RUN_SERV_TCP_PORT);

			if(-1==(link_sock=CreateSockNoBlock()))
			{ 
				Err_log("Fail to create link socket");
				return (void*)-1;
			}

			ret= connect(link_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			if(ret == 0)
				goto DONE;               /* connect completed immediately */
			
			/*ret==-1*/
#ifdef	WINDOWS_OS
			if(WSAGetLastError() != WSAEWOULDBLOCK)
#else
			if(errno != EINPROGRESS)
#endif
			{
				sprintf(err_str, "fail to connect %s", ipaddr);
				Err_log(err_str);
				closesocket(link_sock);
				continue;
			}

			FD_ZERO(&wset);
			FD_SET(link_sock, &wset);
			tv.tv_sec= 1;
			tv.tv_usec= 0;
			ret= select(link_sock+1, NULL, &wset, NULL, &tv);
			if(ret == 0)
			{
				sprintf(err_str, "connect %s timeout", ipaddr);
				Err_log(err_str);
				closesocket(link_sock);
				continue;
			}
			else if(ret < 0)
			{
				sprintf(err_str, "connect %s error", ipaddr);
				Err_log(err_str);
				closesocket(link_sock);
				continue;
			}

DONE:
			closesocket(link_sock);
			if(!RestoreFiles(ipaddr, remote_unix_type, local_disk, remote_disk))
			/*if(!RestoreFiles(rcp_hostname, remote_unix_type, local_disk, remote_disk))*/
				{  rcpOK = 1;  break;  }
		}
	}	
	continue;
   } 
}










