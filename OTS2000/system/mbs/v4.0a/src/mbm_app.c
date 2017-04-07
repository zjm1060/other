#include "..\inc\mbm_com.h"

extern int	SlaveNum;
extern SLAVE_REC	sSlaveRec[MAX_SLAVE_NUM+1];
extern MBDATA mbdata[MAX_SLAVE_NUM+1];


void * ThreadCommM( void * arg )
{
	int	ret;
	int loop_count=0;
	int id;

	SOCKET sock;
	id = * ((int *) arg);
	if(id<0 || id>=SlaveNum)
	{
		sprintf(log_str,"ThreadComm id=%d error",id);
		log_write(ERR,1,log_str);
		exit(0);
	}
	sock = INVALID_SOCKET;
	for(;;)
	{
		if( mbdata[id].err_code == 1)
		{
			mbdata[id].err_code =0;
			CloseSock( &sock );
			MySleep(2000);
		}
		MySleep(2000);
		if(sock == INVALID_SOCKET)
		{
			sock =CreateSock(id, sSlaveRec[id].Ip, MBPORT);
			if(sock ==INVALID_SOCKET)
			{
				MySleep(200);
				continue;
			}
		}

		if( mbdata[id].anar.number > 0 || mbdata[id].polr.number > 0 )
		{
			ret=GetRemoteData(sock,id);
			if( ret != TRUE )
			{
				sprintf(log_str,"GetRemoteData( %02d ) error",id);
				err_write(log_str);	
				mbdata[id].err_code =1;
				continue;
			}
		}
		if( mbdata[id].anas.number > 0 || mbdata[id].pols.number > 0 )
		{
			ret=SendDataToRemote(sock,id);
			if( ret != TRUE )
			{
				sprintf(log_str,"SendDataToRemote( %02d ) error",id);
				err_write(log_str);	
				mbdata[id].err_code =1;
				continue;
			}
		}
	}
	return NULL;
}

BOOL GetRemoteData(SOCKET sock, int id)
{
	int 	seg;
	int 	ref;
	int 	result;
	int  	scanlen;
	unsigned short ananum, polnum;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	if (sock==INVALID_SOCKET)   return FALSE;
	
	/***************** Read ana Data From PLC **************************************************/
	if(mbdata[id].anar.number > 0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		//get ana data
		seg=REGSEG;
		ref=ANA_SEND_ADDR;
		scanlen=mbdata[id].anar.number * 2 + 1 ;
		ref = ref -1;
		result = ReadPLC_MB(sock, (unsigned char)seg, ref, scanlen, databuf); 
		if(result==FALSE)
		{
			sprintf(log_str,"mbmapp.c:  GetRemoteData( %d ): Ana ReadPLC_MB error \n",id);
			log_write(ERR,1,log_str);	
			return(FALSE);
		}
		else
		{
			sprintf(log_str,"mbmapp.c:  GetRemoteData( %d ): Ana ReadPLC_MB success. \n",id);
			log_write(INF,1,log_str);			
		}
		memcpy(&ananum,databuf,2);
		Swap2Str((char *)&ananum,1);
		if( ananum != mbdata[id].anar.number)
		{
			sprintf(log_str,"mbmapp.c:  Remote Ana Recv = %d differ from local =%d \n",(unsigned int)ananum, mbdata[id].anar.number);
			log_write(ERR,1,log_str);
			return(FALSE);			
		}
		/*****************decode original ana data **************************************************/
		memcpy( mbdata[id].anar.value , databuf+2 , mbdata[id].anar.number * 4);
		Swap4Str((unsigned char *)mbdata[id].anar.value, mbdata[id].anar.number);
	}
	
	/***************** Read pol Data From PLC **************************************************/
	if(mbdata[id].polr.number > 0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		//get pol data
		seg=REGSEG;
		ref=POL_SEND_ADDR;
		scanlen= (mbdata[id].polr.number+15)/16 + 1;
		ref= ref -1;
		result = ReadPLC_MB(sock, (unsigned char)seg, ref, scanlen, databuf); 
		if(result==FALSE)
		{
			sprintf(log_str,"mbmapp.c:  GetRemoteData( %d ): pol ReadPLC_MB error \n",id);
			log_write(ERR,1,log_str);	
			return(FALSE);
		}
		else
		{
			sprintf(log_str,"mbmapp.c:  GetRemoteData( %d ): pol ReadPLC_MB success. \n",id);
			log_write(INF,1,log_str);
		}
		memcpy(&polnum,databuf,2);
		Swap2Str((char *)&polnum,1);
		if( polnum != mbdata[id].polr.number)
		{
			sprintf(log_str,"mbmapp.c:  Remote pol Recv = %d differ from local =%d \n",(unsigned int)polnum, mbdata[id].polr.number);
			log_write(ERR,1,log_str);
			return(FALSE);			
		}
		/*****************decode original pol data **************************************************/
		Swap2Str(databuf+2, mbdata[id].polr.number);		
		memcpy( mbdata[id].polr.status  , databuf+2 , mbdata[id].polr.number * 2);
	}

	BdRecvData(id);

	return TRUE;
}

BOOL SendDataToRemote(SOCKET sock, int id)
{
	int 	seg;
	int 	ref;
	int  	scanlen;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	if (sock==INVALID_SOCKET)   return FALSE;
	

	sprintf(log_str,"\nSendDataToRemote = %d:  ",id);
	log_write(INF,1,log_str);	
	if( FillMbArea(id) != TRUE )
	{
		return FALSE;
	}
	/***************** Read ana Data From PLC **************************************************/
	if(mbdata[id].anas.number > 0 )
	{	
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg = REGSEG;
		ref = ANA_RECV_ADDR;
		scanlen=mbdata[id].anas.number * 2 + 1 ;
		if(scanlen > MAX_SCAN_BUF)
		{
			sprintf(log_str,"\nSendDataToRemote (%d) scanlen =%d error:  ",id, scanlen);
			err_write(log_str);	
			return FALSE;
		}
		ref= ref -1;
		memcpy(databuf,mbdata[id].mbarea + ANA_SEND_ADDR -1, scanlen*2 );

		if(WritePLC_MB(sock, (unsigned char)seg, ref, scanlen, databuf) == FALSE)
		{
			sprintf(log_str,"\nWritePLC_MB (%d) error:  ",id);
			err_write(log_str);	
			return FALSE;
		}
		else
		{
			sprintf(log_str,"mbmapp.c:  SendDataToRemote (%d)  Ana success. \n",id);
			log_write(INF,1,log_str);
		}
	}	
	
	/***************** Read ana Data From PLC **************************************************/
	if(mbdata[id].pols.number > 0 )
	{	
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=POL_RECV_ADDR;
		scanlen= 1 + Mod16( mbdata[id].pols.number ) ;
		ref= ref -1;
		memcpy(databuf,mbdata[id].mbarea + POL_SEND_ADDR -1, scanlen * 2 );
		if(WritePLC_MB(sock, (unsigned char)seg, ref, scanlen, databuf) == FALSE)
		{
			sprintf(log_str,"\nWritePLC_MB (%d) error:  ",id);
			err_write(log_str);	
			return FALSE;
		}
		else
		{
			sprintf(log_str,"mbmapp.c:  SendDataToRemote (%d)  POL success. \n",id);
			log_write(INF,1,log_str);
		}		
	}
	return TRUE;
}


BOOL BdRecvData(int id)
{
	int i;
	unsigned short status;
	for(i=0;i<mbdata[id].anar.number;i++)
	{
		if(BdANAByName(mbdata[id].anar.name[i], (float)(mbdata[id].anar.value[i])/1000 )!=TRUE )
		{
			sprintf(log_str,"BdAnaByName %s error\n",mbdata[id].anar.name[i]);
			err_write(log_str);	
		}
	}
	for(i=0;i<mbdata[id].polr.number;i++)
	{
		status  = GetCharBits((unsigned short)mbdata[id].polr.status[i/16],(unsigned short)(i%16),1); 
		if(BdINDByName(mbdata[id].polr.name[i],status)!=TRUE )
		{
			sprintf(log_str,"BdIndByName %s error\n",mbdata[id].polr.name[i]);
			err_write(log_str);	
		}
	}
	sprintf(log_str,"BdRecvData ( %02d ) success \n",id);
	log_write(INF,1,log_str);
	return  TRUE;
}




int FillMbArea(int id)
{
	int value,i;
	BIT_SHORT data_bit;
	unsigned short tmp;
	memset( mbdata[id].mbarea, 0, 2 * MAX_MB_AREA );
	if(GetSendData(id)!=TRUE)
	{
		err_write("GetSendData error");
		return FALSE;
	}
	mbdata[id].mbarea[ANA_SEND_ADDR-1] = mbdata[id].anas.number;
	for(i=0;i<mbdata[id].anas.number;i++)
	{
		value = mbdata[id].anas.value[i];
		memcpy(&tmp,(unsigned char *)&value + 2, 2);
		Swap2Str((unsigned char *)&tmp,1);
		memcpy(mbdata[id].mbarea+1+2*i, &tmp,2);

		memcpy(&tmp,(unsigned char *)&value, 2);
		Swap2Str((unsigned char *)&tmp,1);
		memcpy(mbdata[id].mbarea+1+2*i+1, &tmp,2);
	}
	mbdata[id].mbarea[POL_SEND_ADDR-1] = mbdata[id].pols.number;
	memset(&data_bit,0,2);
	for(i=0;i<mbdata[id].pols.number;i++)
	{
		switch ( i % 16 )
		{
		case 0:	
			data_bit.bit0 = mbdata[id].pols.status[i]; 
			break;
		case 1:
			data_bit.bit1 = mbdata[id].pols.status[i];
			break;
		case 2:
			data_bit.bit2 = mbdata[id].pols.status[i];
			break;
		case 3:
			data_bit.bit3 = mbdata[id].pols.status[i];
			break;
		case 4:
			data_bit.bit4 = mbdata[id].pols.status[i];
			break;
		case 5:
			data_bit.bit5 = mbdata[id].pols.status[i];
			break;
		case 6:
			data_bit.bit6 = mbdata[id].pols.status[i];
			break;
		case 7:
			data_bit.bit7 = mbdata[id].pols.status[i];
			break;
		case 8:
			data_bit.bit8 = mbdata[id].pols.status[i];
			break;
		case 9:
			data_bit.bit9 = mbdata[id].pols.status[i];
			break;
		case 10:
			data_bit.bit10 = mbdata[id].pols.status[i];
			break;
		case 11:
			data_bit.bit11 = mbdata[id].pols.status[i];
			break;
		case 12:
			data_bit.bit12 = mbdata[id].pols.status[i];
			break;
		case 13:
			data_bit.bit13 = mbdata[id].pols.status[i];
			break;
		case 14:
			data_bit.bit14 = mbdata[id].pols.status[i];
			break;
		case 15:
			data_bit.bit15 = mbdata[id].pols.status[i];
			break;
		}
		if((i%16 == 0 && i>0) || (i==mbdata[id].pols.number-1))
		{
			Swap2Str((unsigned char *)&data_bit,1);
			if( i%16 == 0 && i>0 )
			{
				memcpy(mbdata[id].mbarea+POL_SEND_ADDR-1 + i/16,&data_bit,2); 
			}
			else
			{
				memcpy(mbdata[id].mbarea+POL_SEND_ADDR-1 + (int)(i/16) +1 ,&data_bit,2); 
			}
			memset(&data_bit,0,2);
		}
	}	
	return TRUE;
}


BOOL GenAnaSendTable(int id)		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[38];
	char			iStr[5];
	char			tmpstr[200];
	short			j;
	char	filenamestr[200];
	
	sprintf(filenamestr,"%s%s_mbm_%s",HOMEPATH,sSlaveRec[id].NameID,ANA_SEND_FILE);
 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		sprintf(log_str,"can't open file %s",filenamestr);
		err_write(log_str);
 		mbdata[id].anas.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
	while ( fscanf(fp,"%s",tmpstr)!=EOF )
	{
		if(strstr(tmpstr,"*/")!=0) break;
	}  
	j=0;
	while( fscanf(fp,"%s	%s	%s",iStr,tmpstr, mbdata[id].anas.name[j])!=EOF )
	{
		if(strcmp(mbdata[id].anas.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("%02d: anas.name %d pnt: %s\n",id, j,mbdata[id].anas.name[j]);
#endif
			strcpy(name_buf2,mbdata[id].anas.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				sprintf(log_str,"#### ANA_SEND %d pnt: %s No this point in the database file!!!",j,name_buf2);
				err_write(log_str);
				return(FALSE); 
			}
			j++;
		}
	}
 	mbdata[id].anas.number = j;
	sprintf(log_str,"%02d: GenAnaSendTable: point number = %d",id,j);
	err_write(log_str);
	fclose(fp);

	return(TRUE);
}


BOOL GenAnaRecvTable(int id)		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[38];
	char			iStr[5];
	char			tmpstr[50];
	short			j;
	char	filenamestr[200];
	
	sprintf(filenamestr,"%s%s_mbm_%s",HOMEPATH,sSlaveRec[id].NameID,ANA_RECV_FILE);
 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
		sprintf(log_str,"can't open file %s",filenamestr);
		err_write(log_str);
		mbdata[id].anar.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
	while ( fscanf(fp,"%s",tmpstr)!=EOF )
	{
		if(strstr(tmpstr,"*/")!=0) break;
	}  
	j=0;
	while( fscanf(fp,"%s	%s	%s",iStr,tmpstr, mbdata[id].anar.name[j])!=EOF )
	{
		if(strcmp(mbdata[id].anar.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("%02d: anar.name %d pnt: %s\n",id,j,mbdata[id].anar.name[j]);
#endif
			strcpy(name_buf2,mbdata[id].anar.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### ANA_RECV %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				return(FALSE); 
			}
			j++;
		}
	}
 	mbdata[id].anar.number = j;
	sprintf(log_str,"%02d: GenAnaRecvTable: point number = %d",id,j);
	err_write(log_str);
	fclose(fp);

	return(TRUE);
}


BOOL GenPolSendTable(int id)		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[38];
	char			iStr[5];
	char			tmpstr[50];
	short			j;
	char	filenamestr[200];
	
	sprintf(filenamestr,"%s%s_mbm_%s",HOMEPATH,sSlaveRec[id].NameID,POL_SEND_FILE);
 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
		sprintf(log_str,"can't open file %s",filenamestr);
		err_write(log_str);
		mbdata[id].pols.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
	while ( fscanf(fp,"%s",tmpstr)!=EOF )
	{
		if(strstr(tmpstr,"*/")!=0) break;
	}  
	j=0;
	while( fscanf(fp,"%s	%s	%s",iStr,tmpstr, mbdata[id].pols.name[j])!=EOF )
	{
		if(strcmp(mbdata[id].pols.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("%02d: pols.name %d pnt: %s\n",id,j,mbdata[id].pols.name[j]);
#endif
			strcpy(name_buf2,mbdata[id].pols.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### POL_SEND %d pnt: %s No this point in the database file!!!\n",j,name_buf2);

				return(FALSE); 
			}
			j++;
		}
	}
 	mbdata[id].pols.number = j;
	sprintf(log_str,"%02d: GenPolSendTable: point number = %d",id,j);
	err_write(log_str);
	fclose(fp);

	return(TRUE);
}


BOOL GenPolRecvTable(int id)		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[38];
	char			iStr[5];
	char			tmpstr[50];
	short			j;
	char	filenamestr[200];
	
	sprintf(filenamestr,"%s%s_mbm_%s",HOMEPATH,sSlaveRec[id].NameID,POL_RECV_FILE);
 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
		sprintf(log_str,"can't open file %s",filenamestr);
		err_write(log_str);
		mbdata[id].anas.number = 0;
		return(FALSE);          
	}
	
	rewind(fp);	
	
	while ( fscanf(fp,"%s",tmpstr)!=EOF )
	{
		if(strstr(tmpstr,"*/")!=0) break;
	}  
	j=0;
	while( fscanf(fp,"%s	%s	%s",iStr,tmpstr, mbdata[id].polr.name[j])!=EOF )
	{
		if(strcmp(mbdata[id].polr.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("%02d: polr.name %d pnt: %s\n",id,j,mbdata[id].polr.name[j]);
#endif
			strcpy(name_buf2,mbdata[id].polr.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### POL_RECV %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				return(FALSE); 
			}
			j++;
		}
	}
 	mbdata[id].polr.number = j;
	sprintf(log_str,"%02d: GenPolRecvTable: point number = %d",id,j);
	err_write(log_str);
	fclose(fp);

	return(TRUE);


}

BOOL GetSendData(int id)
{
	int i;
	DB_DATA		db_data;
	IND_DATA    status;
	for(i=0;i<mbdata[id].anas.number;i++ )
	{
		if(DBread_by_name( mbdata[id].anas.name[i], &db_data )!=FALSE)
		{		
			mbdata[id].anas.value[i] = (int)(db_data.fvalue * 1000);
		}
		else
		{
			printf("DBread_by_name error %s\n",mbdata[id].anas.name[i]);
			return FALSE;
		}
	}

	for(i=0;i<mbdata[id].pols.number;i++ )
	{
		if(DBread_by_name( mbdata[id].pols.name[i], &db_data )!=FALSE)
		{		
			memcpy(&status,&(db_data.status),sizeof(status));
			mbdata[id].pols.status[i] = status.state;
		}
		else
		{
			printf("DBread_by_name error %s\n",mbdata[id].pols.name[i]);
			return FALSE;
		}
	}
	return  TRUE;
}


int ReadMbmStnDef(char * pStnDefFile)
{	
	FILE	*fp;
	char	NameID[20], Ip[40];
	char	filebuf[LINE_BUF_CHAR];
	int		r;

	r=0;
	SlaveNum = 0;
	while( (fp=fopen(pStnDefFile,"r"))==NULL )
	{
		sprintf(log_str,"\n### ALARM !!! IEC104: Can't open Station config file %s !!! ",pStnDefFile);
		err_write(log_str);
		Sleep(5000);
	}

	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
	#ifdef DEBUG
				printf("\nRead Station define file %s: master num = %d!!!",pStnDefFile,SlaveNum);
	#endif
				fclose(fp);
				return(TRUE);
			}
			else
			{
				printf("Error!%3d line: is a null line.",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '='))
		{
			sprintf(log_str,"%s",filebuf);
			err_write(log_str);
			continue;
		}
		if(sscanf(filebuf,"%s %s",NameID,Ip)<=0)
		{
			sprintf(log_str,"Error!%3d line: %s",r,filebuf);
			err_write(log_str);
			continue;
		}
		strcpy( sSlaveRec[SlaveNum].NameID, NameID );
		strcpy( sSlaveRec[SlaveNum].Ip, Ip );
		sSlaveRec[SlaveNum].stnid = SlaveNum;
		if(sSlaveRec[SlaveNum].NameID[0] == '!')
		{
			//sprintf(log_str,"\nDisable Station name %s: IP = %s",sSlaveRec[SlaveNum].NameID,
			//	sSlaveRec[SlaveNum].Ip);
			//err_write(log_str);
			continue;
		}
		sprintf(log_str,"\nStation %02d name %s: IP = %s \n",SlaveNum,sSlaveRec[SlaveNum].NameID,
			sSlaveRec[SlaveNum].Ip);
		log_write(INF,1,log_str);
		SlaveNum++;
		if(SlaveNum>=MAX_SLAVE_NUM)
		{
			fclose( fp );
#ifdef DEBUG
			printf("\nRead Station define file %s: master num = %d!!!\n\n",pStnDefFile,SlaveNum);
#endif
			return( TRUE );
		}
	}
}

SOCKET CreateSock(int id, char *address, unsigned short port)
{
	SOCKET newsock; 
	struct sockaddr_in pin;
	int bindret = TRUE;
	struct timeval tmval;
	fd_set set;
	int sel;
#ifndef MSWIN32
	int f ;
#endif
	int error = -1, len = sizeof(int);
	struct hostent *hp;
	unsigned int opt_val;
	int result,on,i;
	newsock = INVALID_SOCKET;
	pin.sin_family = AF_INET;
	pin.sin_port = htons(port);
	result= SOCKET_ERROR;
	hp=gethostbyname(address);
	sprintf(log_str,"\nTry Connect(%d): ip=%s port=%d...",id,address,port);
	log_write(INF,1,log_str);
	if (hp)
	{
		pin.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;
	}
	else
 	{
		pin.sin_addr.s_addr = inet_addr(address);
	}
	if ((newsock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		CloseSock(&newsock);
		sprintf(log_str,"CreateSock(): socket() error\n");
		log_printf(ERR,1,PLC_APP,log_str);
		newsock = SOCKET_ERROR;
		return (newsock);
	}

	opt_val = (char) NODELAY;
	setsockopt(newsock, IPPROTO_TCP, TCP_NODELAY, (char *)&opt_val, sizeof(char));
	on=1;
	setsockopt(newsock,SOL_SOCKET,SO_KEEPALIVE,(char *)&on,sizeof (on));
#ifndef MSWIN32
	f = fcntl(newsock, F_GETFL);
	fcntl(newsock, F_SETFL, f | O_NONBLOCK);	
#else
	opt_val   =   1;   
	result = ioctlsocket(newsock,   FIONBIO,   &opt_val); 
#endif
	for(i=0;i<MAXRETRY_CONN;i++)
	{
		result=connect(newsock, (struct sockaddr*)&(pin), sizeof(pin)); 
		if (result == SOCKET_ERROR)
		{
			tmval.tv_sec = 0;
			tmval.tv_usec = CONN_TIMEOUT_MS * 1000;
			FD_ZERO(&set);
			FD_SET(newsock, &set);
			if ((sel=select(newsock + 1, NULL, &set, NULL, &tmval)) > 0)
			{
					getsockopt(newsock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
					if (error == 0)
					{
						result = TRUE;
						break;
					}
			}
			MySleep(1000);		/* modified by yyp for 200 to 1000 on 2008.12.4 */
			continue; 
		}
		else
		{
			break;
		}
	} 
	if(result != SOCKET_ERROR)
	{
#ifndef MSWIN32
		fcntl(newsock,F_SETFL,O_NDELAY);
#endif		
	}
	if(result == SOCKET_ERROR)
	{
		sprintf(log_str,"\nCreateSock(%d): ip=%s port=%d error",id,address,port);
		log_write(ERR,1,log_str);		
		CloseSock(&newsock);
		newsock = result;
	}
	else
	{
		sprintf(log_str,"\nCreateSock(%d): ip=%s port=%d sock=%d success",id, address,port,newsock);
		log_write(INF,1,log_str);
	}
	return (newsock);
}