
#include	"../inc/dl476_std.h" 
#include	"../inc/dl476_data.h" 

#ifdef MSWIN32
WORD   	wvr;
WSADATA WSAData; 
HANDLE	hMapObject;
#else 
void 	sig_dl476(int); 
int	childflag;
pid_t	pid;
#endif  

SOCKET 	sock,newsock;

short		almptr;
SOCKET		clientsock,serversock;
char		old_day;

void main()															
{
#ifdef MSWIN32
	wvr=MAKEWORD(2,2);
	
	if (WSAStartup(wvr, &WSAData) != 0)
	{
		printf("dl476-92 client exit\n"); 
		return;
	}
#else
	signal(SIGTERM,sig_dl476);
	signal(SIGINT,sig_dl476);
#endif	

	/*数据库共享*/
	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}



	/*报警共享区初始化*/
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		printf("DL476_client:: can't attach  rec shared memory");
		return;
	}
			 
	/*Lan_init();*/
	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}
	
	if(-1==InterLockFileLoad())
	{
		printf("Fail to Load InterLock File!\n");
		exit(-1);
	}
	
	/*数据文件名*/ 
	if(GenTableConf()==FALSE)	return; 

	/*基本数据表*/
	if(GenDATAIndex()==FALSE)	return;
	 
	/*开关量控制表*/ 
	if(GenSWCTRLIndex()==FALSE)	return;
   	
	/*生成节点定义表*/
	if(GenNodeTable()==FALSE)	return;


	for(;;)
	 
	{
		ClientRecv();
		
		sleep(2);
	}
		
		
} 
 
 

#ifdef MSWIN32

int ClientRecv()
{

	SOCKADDR_IN    	clientaddr,serveraddr;

	int				clientaddrlen,timeout,on;

	DWORD			threadID;

	HANDLE			hthreadid;	   

     	sock = socket(AF_INET, SOCK_STREAM,0);
                
	if (sock == INVALID_SOCKET) 
	{ 
		printf("\ntcp.c:: creat socket() failed\n");
            
		close(sock);
			
		return(-1);
    	}
    	
    	memset((char *) &serveraddr,0,sizeof (struct sockaddr_in));
		serveraddr.sin_family      = AF_INET;
    	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	serveraddr.sin_port        = htons(WAN_PORT);  


    	if ( bind(sock,(struct sockaddr FAR *)&serveraddr,sizeof(serveraddr))==SOCKET_ERROR) 
	{
		printf("\nDL476-92: bind  is the error.\n ");

		close(sock);
		
		return(-1);		

	}
    
	timeout=5000;

	setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout)); 
	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
	
	on=1;            
    	setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&on,sizeof (on));   
	
	if(listen(sock,6)<0){

		 printf("\nbind error:  is the error.\n " );

		 return(-1);

	}

	clientaddrlen=sizeof(clientaddr);

	printf("\nDL476-92:: waiting for connection\n");
		
	for(;;)
	{
		newsock=accept(sock,(struct sockaddr *) &clientaddr,(int FAR *)&clientaddrlen);
		
		if(newsock==INVALID_SOCKET) 
		{
				printf("accept  is the error.\n " );

				return(-1);
		}
		 
		hthreadid=CreateThread(NULL,
					0,
					(LPTHREAD_START_ROUTINE)CmdRecv(newsock),
					NULL,
					CREATE_SUSPENDED,
					(LPDWORD)&threadID);
		 
		 if(hthreadid==NULL)
		 {
		 		printf("DL476-92::creat thread CmdRecv() failed    \n" );

				continue;
		} 	

		printf("-----waiting for connection-----\n");     
          
		close(newsock);
	}

	return(0);
}

#else

int ClientRecv()
{

	struct sockaddr_in    	clientaddr,serveraddr;

	int			clientaddrlen,timeout; 

	pid_t			pid;
 
	 
    	sock = socket(AF_INET, SOCK_STREAM,0);
                
	if (sock == INVALID_SOCKET) {
     
            	printf("\nClientRecv:: creat socket() failed\n");
            
            	close(sock);
			
		return(-1);
    	}
    	
    	memset((char *) &serveraddr,0,sizeof (struct sockaddr_in));
		serveraddr.sin_family      = AF_INET;
    	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    	serveraddr.sin_port        = htons(WAN_PORT);  


    	if ( bind(sock,(struct sockaddr *)&serveraddr,sizeof(serveraddr))==SOCKET_ERROR) 
	{
		printf("\nDL476-92: bind   is  error.\n " );

		close(sock);
		
		return(-1);
	}
	
 
	timeout=5;   
 
 	setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout)); 
	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
	
	if(listen(sock,6)<0){

		 printf("\nDL476-92: listen   is  error.\n " );

		 return(-1);

	}

	clientaddrlen=sizeof(clientaddr);

	childflag=0;
		
	for(;;)
	{
		printf("\nDL476-92:: waiting for connection\n");
		
		newsock=accept(sock,(struct sockaddr *) &clientaddr,&clientaddrlen);
		
		if(newsock==INVALID_SOCKET) 
		{
				printf("\nDL476_client_recv: accept   is   error.\n " );

				continue;
		}
		
		printf("dl476_client_recv: recv new connection\n");

		pid=fork(); 

		if(0 == pid) //child
		{
			printf ("dl476_client_recv: child comm task start...\n");
			close(sock);
			childflag=1;
			CmdRecv(newsock);
			close(newsock);
			printf ("dl476_client_recv: child comm task exit\n");
			exit(0);
		}
		else
		{
			printf("dl476_client_recv: success to start child comm task: pid= %d\n", pid);
			close(newsock);
		}

	}
	return(0);
}


#endif
int CmdRecv(SOCKET sockfd)
{
	short len;
	
		for(;;)
		{
			/*printf("waiting for cmd  ....\n"); */

			/* CMD 包头*/ 
			
			if (Do_Recv(sockfd,(char *)&cmdapdu.head,sizeof (BASEDATA_HEAD))==0)
			{
				Do_Disconnect(sockfd);
				return(-1);  
			}

 			len=cmdapdu.head.length	;
 #ifdef SUN
			Swap2Byte(&len);
#endif
			if(len==0)		continue;		/* heart beat block*/

			if(len>MAX_APDU_SIZE)
			{
				Do_Disconnect(sockfd);
				return(-1);  
			}

			/* CMD 内容 */ 

			if (Do_Recv(sockfd,(char *)&cmdapdu.block,len)==0)
			{
				Do_Disconnect(sockfd);
				return(-1);  
			}

			/* CMD 分析 */

			if(ClientCmdConvert(sockfd,cmdapdu)==0)		/*回送命令失败*/
			{
				Do_Disconnect(sockfd);
				return(-1);  
			}
	
		}
        
		return(0);
}  


#ifndef MSWIN32
void sig_dl476(int signo)
{ 	
	if(childflag==0)
	{	  
 		kill(pid,SIGINT);
			
		printf("dl476_client_recv.dxe  main exit...\n");

	}else{
		/*shutdown(newsock,2);*/
		close(newsock);
	/*	shutdown(sock,2);*/
		close(sock);
		printf(" dl476_client_recv:: child  %d exit...\n",getpid());
	}
	
	exit(0); 
}
#endif
