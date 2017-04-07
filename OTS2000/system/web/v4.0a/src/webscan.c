#include "../inc/webscan.h";

int SockId,errCount=0;
int	main(int argc, char *argv[])

{
    int i,n,maxi = -1,retNum;
    int nready,len=0;
    int sockfd,maxfd=-1,connectfd;
	char c;
    
    unsigned int myport,lisnum;

    struct sockaddr_in  my_addr,addr;
    struct timeval tv;

    fd_set rset,allset;    
    
    char buf[BUFSIZE],ret[BUFSIZE],log[200];
    CLIENT client[FD_SETSIZE];
    
	CreateLockFile(".webscan.lock");
	if(-1==InitSysLog(H9000_MODULE_WEBSCAN, "WebScan"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== hdbwrite.c: call Lan_init_all() error\n");		
		exit(0);
	}
	sprintf(log,"**********Start WebScan!***********");
	log_write(1,1,log);
    myport = SERV_PORT;
    lisnum = SERV_IP;
	
    if((SockId = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        sprintf(log,"create socted err");
		log_write(1,1,log);
        exit(1);
    }

    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(SockId, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        sprintf(log,"bind socted err");
		log_write(1,1,log);
            exit(1);
        }

    if (listen(SockId, lisnum) == -1) {
        sprintf(log,"listen socted err");
		log_write(1,1,log);
            exit(1);
        }

    for(i=0;i<FD_SETSIZE;i++)
    {
        client[i].fd = -1;
    }

    FD_ZERO(&allset);           
        FD_SET(SockId, &allset);  
    maxfd = SockId;
    
	sprintf(log,"Waiting for connections and data...");
	log_write(1,1,log);
    while (1)
    {        
		rset = allset;            

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		nready = select(maxfd + 1, &rset, NULL, NULL, &tv);

        if(nready == 0)
            continue;
        else if(nready < 0)
        {
			sprintf(log,"select failed!nready=%d",nready);
			log_write(1,1,log);
            break;
        }    
        else
        {
            if(FD_ISSET(SockId,&rset)) // new connection
            {            
                len = sizeof(struct sockaddr);
                if((connectfd = accept(SockId,
                    (struct sockaddr*)&addr,&len)) == -1)
                {
					sprintf(log,"accept() error");
					log_write(1,2,log);
                    continue;
                }
                for(i=0;i<FD_SETSIZE;i++)
                {
                    if(client[i].fd < 0)
                    {
                        client[i].fd = connectfd;
                        client[i].addr = addr;   
					 	sprintf(log,"Yout got a connection from %s",inet_ntoa(client[i].addr.sin_addr));
						log_write(1,1,log);	
                        break;
                    }
                }
                if(i == FD_SETSIZE)            
				{
					sprintf(log,"too many connections");
					log_write(1,2,log);	
				}
                FD_SET(connectfd,&allset);
                if(connectfd > maxfd)
                    maxfd = connectfd;
                if(i > maxi)
                    maxi = i;
            }
            else
            {            
                for(i=0;i<=maxi;i++)
                {            
                    if((sockfd = client[i].fd)<0)
                        continue;                
                    if(FD_ISSET(sockfd,&rset))
                    {           
                        bzero(buf,BUFSIZE);
						if((n = recv(sockfd,buf,BUFSIZE-1,0)) > 0)
                        {
							sprintf(log,"received data: from %s\n",inet_ntoa(client[i].addr.sin_addr));
							log_write(1,2,log);
							if(n>52) 
							{
								RcvDataProc(n,buf,&retNum,ret);
							 	sprintf(log,"readData Complete!n=%d,retNum=%d\n",n,retNum);
								log_write(1,2,log);
							 
								if(retNum>0)
								{
									n=send(sockfd,ret,retNum,0);
							 		sprintf(log,"send=%d\n",n);
									log_write(1,2,log);
									if( n<=0  )
									{
										closesocket( sockfd ); 
										errCount++;
										sleep(2); 
										break;						
									}
								}
							}
                        }                
                        else
                        {
						 	sprintf(log,"disconnected by client!");
							log_write(1,2,log); 
                            close(sockfd);
                            FD_CLR(sockfd,&allset);
                            client[i].fd = -1;
                        }
                    }
                }
            }
        }    
    }
    close(SockId);
}
void EndProc()
{ 	
	closesocket( SockId );
	sprintf(log,"!!!!!! the webscan.dxe stop !!!!!!");
	log_write(1,2,log);
 	exit(-1);
}
void RcvDataProc(int num,char buf[],int *retNum,char ret[])
{
	int i=0,j=0,x=0,y=0;
	UCHAR type,en=1,typeNum=0,typeSize=0;
	char		pt_name[POINT_NAME_SIZE],typeResult[MAXDATASIZE],log[200];
	char	p0,p1,pointCount;
	DMS_COMMON	common_aa;
	POINTER		point;
	float	ana_value;

	y=0;
	*retNum=0;

	while(i<num)
	{
		while(buf[i]!=(char)255)
		{
			i++;
			if(i>=num) return;
		}
		while(buf[i]==(char)255)
		{
			i++;
			if(i>=num) return;
		}
		while(buf[i]==(char)0)
		{
			i++;
			if(i>=num) return;
		}
		if((i+POINT_NAME_SIZE)>num) break;
		for(j=0;j<(POINT_NAME_SIZE);j++)
		{
			pt_name[j]=buf[i++];
		}
		y++;
		if(y==65)
		{
			y=y;
		}
		en=1;
		if(-1==GetPtIdFromStr(pt_name, &point))
		{
			sprintf(log,"\nNo this point in database,PT_NAME=%s",pt_name );
			log_write(1,2,log);
			errCount++;
			en=0;
		}
		else
		{
			if(-1==ReadEntryById( &point, &common_aa ))
			{
				sprintf(log,"\nNo this point in database,PT_NAME=%s",pt_name );
				log_write(1,2,log);
				errCount++;
				en=0;
			}
		}
		typeNum=buf[i++];
		if(0==en)
		{
			i=i+typeNum;
			continue;
		}
		memcpy(&ret[x],&pt_name[0],POINT_NAME_SIZE);
		x=x+POINT_NAME_SIZE;
		ret[x++]=typeNum;
		for(j=0;j<typeNum;j++)
		{
			type=buf[i++];
			memset(typeResult,0,MAXDATASIZE);
			switch(type)
			{
				case GET_PT_ID:break;
				case GET_LOGICALNAME:
					typeSize=POINT_NAME_SIZE;
					GetPtNameById(&common_aa.point,typeResult,NULL);
					break;
				case GET_LONGNAME:
					typeSize=POINT_LONGNAME_SIZE;
					GetPtLongnameById(&common_aa.point, typeResult);
					break;
				case GET_FVALUE:
					typeSize=sizeof(common_aa.var.anlg.fValue);
					memcpy(&ana_value,&common_aa.var.anlg.fValue,typeSize);
					#ifdef 	SUN_OS
 						Swap4Byte(&ana_value);
					#endif
					memcpy(&typeResult[0],&ana_value,typeSize);
					break;
				case GET_IND_STATE:
					typeSize=1;
					typeResult[0]=common_aa.var.ind.status.state;
					break;
				case GET_OBJ_STATE:
					typeSize=1;
					p0=common_aa.var.devobj.fb_status1.p0;
					p1=common_aa.var.devobj.fb_status1.p1;
					if(0==p0 && 0==p1) typeResult[0]=2;
					if(0==p0 && 1==p1) typeResult[0]=1;
					if(1==p0 && 0==p1) typeResult[0]=0;
					if(1==p0 && 1==p1) typeResult[0]=3;
					break;
				default:
					typeSize=0;
					break;
			}
			ret[x++]=type;
			ret[x++]=typeSize;
			memcpy(&ret[x],&typeResult[0],typeSize);
			x=x+typeSize;
			errCount=0;
		}
	}
	*retNum=x;
}
