/*######################################################################################
FILENAME:	tcp.c

FUNCTION:	
	
	source code of tcp function,include:
	!- 	Tcp_init()
	!-	Tcp_open(char *serverhost)	
	!-	Tcp_close()
	!-	Tcp_send()
	!-	Tcp_recv()

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

#######################################################################################*/

#include "../inc/lanhead_all.h"

struct sockaddr_in	serveraddr;
struct sockaddr_in	clientaddr;
#define maxpid 6
int pid,childflag,sofd;

int tcp_init(unsigned short port)
{
	int	      sockfd=-1;
	int	      sockaddrlen=sizeof (struct sockaddr_in);
	int    		newsockfd;
	int    		clientaddrlen;
	int i;
	/*signal(SIGCHLD, SIG_IGN); */
	signal(SIGCHLD, sig_child);
	signal(SIGINT,sig_termm);     
	signal(SIGTERM,sig_termm);
	signal(SIGKILL,sig_termm);
	signal(SIGPIPE,sig_termm);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket_create");
		exit(1);
	}
	memset((char *) &serveraddr,0,sizeof (struct sockaddr_in));
	serveraddr.sin_family      = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port	 = htons(port);
	if ( bind(sockfd,(struct sockaddr *)&serveraddr,sockaddrlen) < 0) 
	{
	  perror("socket_bind");
	  exit(2);
	}
	listen(sockfd, 8);
	for(;;)
	{
	 	if ((newsockfd =accept(sockfd,(struct sockaddr *) &clientaddr,
							      &clientaddrlen)) < 0) 
		{
			  if (errno == EINTR) 
			  {
				    printf (" tcp:: Bye...\n");
				    continue; 
			  } 
			  else 
			  {
				    perror("socket_accept");
				    continue;
			   }
		}
		pid = fork();
		if (pid==0)
		{
			close(sockfd);
			sofd=newsockfd;
			childflag=1;
			net_recv(newsockfd);
			printf ("child exit\n");
			shutdown(newsockfd,2);
			/*close(newsockfd); */
			exit(0);
		}
		else if (pid!=-1)
		{ 
			close(newsockfd);
			printf ("child pid=%d\n",pid);
			sofd=sockfd;
			childflag=0;
		}
		else
		{
			perror("dosession_fork");
		}
	}
	return(0);
}

void sig_termm(int signo)
{
	int		i;
	char    buf[80];
	FILE    *fp;
	pid_t   id;
	printf ("sig_termm:signal INT or TRRM or KILL --,exit...\n");
	if (childflag==0) 
	{
		if (system("ps -eo pid,comm >/DPSTMP/tmp.wan")==-1)      exit(0);
	 	if ((fp=fopen("/DPSTMP/tmp.wan","r"))==NULL)		exit(0);
	 	rewind(fp);
	 	fscanf (fp,"%s",buf);
	 	fscanf (fp,"%s",buf);
	 	while(!feof (fp))
		{
		  	fscanf (fp,"%d",&id);
  			fscanf (fp,"%s",buf);
		   	if ((strcmp(buf,"wan_recv.dxe"))==0)
			{
			   	printf ("id=%d  buf=%s",id,buf);
			   	kill(id,SIGINT);
		  	}
	 	}
	 	fclose(fp);
	}
	shutdown(sofd,2);
	exit(0);
}

/*-------------------------------------------------------------------------------
!
!	The following used by client .
!
!--------------------------------------------------------------------------------*/

int	timeout;
int tcp_open(unsigned short port,char id,short net)
{
	int			sockfd,ret,i,on;
	HOST_MSG		host;		
	FILE			*fp;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("LAN_tcp:: socket_create");
		exit(-1);	
	}
	memset((char *) &serveraddr,0,sizeof (struct sockaddr_in));
	fp=fopen("/LANDEF/hosts.def","r");
	if (fp==NULL)
	{
#ifdef DEBUG
		   printf ("can't open file /LANDEF/hosts.def \n");
#endif
		   return(-1);
	}
	rewind(fp);
	for(i=0;i<MAX_PLC_NUM;i++)
	{
		fscanf (fp,"%d",&host.devid);
		fscanf (fp,"%s",host.name);
		fscanf (fp,"%s",host.addr[0]);
		if (MAX_NET_NUM==2){
		   fscanf (fp,"%s",host.addr[1]);
		}
		if (host.devid==id)
		   break;
	}
	if (i==MAX_PLC_NUM)
	{
#ifdef DEBUG
		  printf ("dev %d not exist in file /LANDEF/hosts.def \n",id);
#endif
		  return(-1);
	 }
	fclose(fp);
	serveraddr.sin_addr.s_addr	=inet_addr(host.addr[net]);	
	serveraddr.sin_family	   	= AF_INET;
	serveraddr.sin_port	     	= htons(port);	     
	timeout=NO;
	signal(SIGALRM,Timer);
	alarm(1);
	ret=connect(sockfd, (struct sockaddr *) &serveraddr, sizeof (serveraddr));
	if ((timeout == YES) || (ret<0))
	{
		alarm(0);
		close(sockfd);
		return(-1);
	 }
	else
	{
		alarm(0);
		on=1;
		setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,&on,sizeof (on));
		return(sockfd);
	}
}

/*
 * The following used by client and server.
 */


/*-------------------------------------------------------------------------------
!
!	The following used by client and server.
!
!--------------------------------------------------------------------------------*/

void tcp_close(int sockfd)
{
	 close(sockfd);
	 sockfd=-1;
}

void end_close(int sockfd)
{
	printf ("End_close\n");
	close(sockfd);
}

int tcp_send(int sockfd, char *buff, int len)
{
	int    rc;
	short	templen;
	rc=send(sockfd,buff,len,0);
	if (rc<=0)
		perror("Tcp_send::");
	if (rc<0 ||rc!=len)
	{
		return(-1);
	}
	return(rc);
}

int tcp_recv(int sockfd, char *buff, short maxlen)
{
	register int    nbytes;
	if ((nbytes=recv(sockfd, buff,maxlen, 0))< 0) 
	{
		return(-1);
	}
	if (nbytes== 0) 
	{
		return(-1);
	}
	if (nbytes>maxlen)
	{
		return(-1);
	}
	return(nbytes);
}		  

int sig_pipe()
{
	DEVID		dev_id;
	int 		type;
	DMS_COMMON      common_aa;
	char	     name[30];
	printf ("pipe broken !!!!!!!!!!!\n");
	strcpy(name,SYS_TAB_NAME);
	strcat(name,"RC_LINK_STA");
	ReadEntryByName(name,&type,&common_aa);
	if ( common_aa.err_code == -1 )
	{
		printf (" %s  not in Database!\n",name);
	}
	else
	{
		if (common_aa.entry.ind.variable.value.status==0)
		 {		
			Wdb(name,4);
			/**** added by yyp for V4.0 05.12.12 *****/
			dev_id.devid=SYS_RTU;
			BdcastMsg(dev_id,DPS_SYSMSG,common_aa.entry.ind.variable.ptr.point_id,4);
		}
	}
	return(0);
}

void Timer(int signo)
{
	 timeout=YES;
}

void sig_child(int signo)
{
	int status;
	timeout=NO;
	alarm(2);
	signal(SIGALRM,Timer);
	wait(&status);  
	alarm(0);
	timeout=YES;
	signal(SIGCHLD,sig_child);
}