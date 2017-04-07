#include "..\inc\mbs_com.h"

extern int	MasterNum;
extern MASTER_REC	sMasterRec[MAX_MASTER_NUM+1];
extern MBDATA mbdata[MAX_MASTER_NUM+1];


void * ThreadCommS( void * arg )
{
	int	result;
	int loop_count=0;
	int id;
	unsigned char recvbuf[1024];
	unsigned char sendbuf[1024];
	SOCKET *p_sock;
	id = * ((int *) arg);
	if(id<0 || id>=MasterNum)
	{
		sprintf(log_str,"ThreadComm id=%d error",id);
		log_write(ERR,1,log_str);
		exit(0);
	}
	p_sock = &mbdata[id].newsock;
	for(;;)
	{
	
		if( mbdata[id].err_code == 1)
		{
			mbdata[id].err_code =0;
			CloseSock( p_sock );
		}
		MySleep(200);
		if(*p_sock == INVALID_SOCKET)
		{
#ifndef MULTITHREAD
			return FALSE;
#endif			
			continue;
		}
		memset(sendbuf,0,1024);
		memset(recvbuf,0,1024);
		sprintf(log_str,"\n%2d: Wait For New Request...",id);
		log_write(ERR,1,log_str);
		result = RecvPLC_MB(*p_sock, recvbuf, PLC_SCAN_LEN, 2);
		//if read error
		if(result == FALSE)
		{
			mbdata[id].err_code = 1;
			continue;
		}
		
		switch (recvbuf[7])		/* function code */
		{
		case 0x04:
		case 0x03:
		case 0x33:
			if(MbFunc04(recvbuf,id)!=TRUE)
			{
				break;
			}
			break;
			
		case 0x10:
		case 0x34:
			if(MbFunc16(recvbuf,id)!=TRUE)
			{
				break;
			}
			break;
		default:
			sprintf(log_str,"%2d: function code = %d error, it must be 03,04,or 16 ",id,recvbuf[7]);
			log_write(ERR,1,log_str);	
			mbdata[id].err_code = 1;
			break;
		}//switch(recvbuf[7])
		
	}
	return NULL;
}

BOOL MbFunc04(char *recvbuf,int id)
{
	int	byte_to_send;
	unsigned short Address;
	unsigned short WordNum;
	int i;
	int funcode;
	unsigned char sendbuf[1024];
	int result;
	SOCKET *p_sock;
	p_sock = &mbdata[id].newsock;
	funcode = recvbuf[7];
	sprintf(log_str,"\nGet One Request, in MbFunc04(%2d):  ",id);
	log_write(INF,1,log_str);		
	if( FillMbArea(id) != TRUE )
	{
		return FALSE;
	}
	memcpy(&Address, recvbuf+2+6,2);
	memcpy(&WordNum, recvbuf+4+6,2);
	Swap2Str((unsigned char *)&Address,2);
	Swap2Str((unsigned char *)&WordNum,2);
	if(Address < (ANA_SEND_ADDR-1) || WordNum<1 || Address+WordNum > MAX_MB_AREA)
	{
		sprintf(log_str,"MbFunc04(%2d):: Address exceed error",id);
		log_printf(ERR,1,PLC_APP,log_str);	
		mbdata[id].err_code = 1;
		return FALSE;
	}
	if(WordNum > MAX_QUERY_SIZE )
	{
		sprintf(log_str,"MbFunc04(%2d):: error, request num must less than %d",id, MAX_QUERY_SIZE);
		log_printf(ERR,1,PLC_APP,log_str);	
		mbdata[id].err_code = 1;
	}
	memcpy(sendbuf,recvbuf,8);
	if( funcode == 51 )
	{
		byte_to_send = RECVPDUHEAD  + WordNum*4 +2; //WZG
		sendbuf[2+6] = WordNum * 4 +2; //WZG
		memcpy(sendbuf+3+6,mbdata[id].mbarea+Address,sendbuf[2+6]);		
	}
	else
	{
		byte_to_send = RECVPDUHEAD + WordNum*2 +2; //WZG
		sendbuf[2+6] = WordNum * 2 +2; //WZG
		memcpy(sendbuf+3+6,mbdata[id].mbarea +Address,sendbuf[2+6]);
	}
	sendbuf[5] = 1 + 2 + sendbuf[2+6];
	if( byte_to_send > 512 )
	{
		sprintf(log_str,"MbFunc04(%2d):: send buf num exceed error",id);
		log_printf(ERR,1,PLC_APP,log_str);	
		mbdata[id].err_code = 1;
		return FALSE;
	}
	result = SOCKET_ERROR;
	result = SendPLC_MB(*p_sock, (char *)sendbuf, byte_to_send);
	if (result == SOCKET_ERROR )
	{
		sprintf(log_str,"MbFunc04(%2d)::error send() timeout or result=%d",id,result);
		log_printf(ERR,1,PLC_APP,log_str);	
		mbdata[id].err_code = 1;
		return (FALSE);
	}
	else
	{
		sprintf(log_str,"MbFunc04(%2d)::response success Addr=%d, Num=%d",id,Address+1,WordNum);
		log_printf(INF,1,PLC_APP,log_str);	
	}
#ifdef PLCDEBUG
	printf("\n==== MbFunc04(%2d): Send data >>\n",id);
	for(i=0;i<result;i++)
	{
		printf("=%02x",sendbuf[i]);
		if((i+1)%20 == 0)
		{
			printf("\n");
		}		
	}
	printf("\n");
#endif
	return TRUE;
}

BOOL MbFunc16(char *recvbuf, int id)
{
	int ByteNum;
	int	byte_to_send;
	int byte_to_recv;
	short Address;
	short WordNum;
	short StartPos;
	short offset;
	int i;
	int result;
	int type,ptnum;
	unsigned char sendbuf[1024];
	unsigned char databuf[1024];
	SOCKET *p_sock;
	p_sock = &mbdata[id].newsock;
	sprintf(log_str,"Get One Request, in MbFunc16(%2d):  ",id);
	log_write(INF,1,log_str);	
	if( recvbuf[7] == 52 )
	{
		ByteNum = 4;
	}
	else
	{
		ByteNum = 2;		
	}
	memcpy(&Address, recvbuf+2+6,2);
	memcpy(&WordNum, recvbuf+4+6,2);
	Swap2Str((unsigned char *)&Address,2);
	Swap2Str((unsigned char *)&WordNum,2);
	if(Address < ANA_RECV_ADDR-1 || WordNum<1 || Address+WordNum* ByteNum / 2 > MAX_MB_AREA)
	{
		sprintf(log_str,"MbFunc16(%2d):: Address exceed error",id);
		log_printf(ERR,1,PLC_APP,log_str);	
		mbdata[id].err_code = 1;
		return FALSE;
	}
	if(WordNum * ByteNum / 2 > MAX_QUERY_SIZE )
	{
		sprintf(log_str,"MbFunc16():: error, request num must less than %d", MAX_QUERY_SIZE);
		log_printf(ERR,1,PLC_APP,log_str);	
		mbdata[id].err_code = 1;
	}

	byte_to_recv = 1 + WordNum * ByteNum;
	result = RecvPLC_MB(*p_sock, recvbuf+PLC_SCAN_LEN, byte_to_recv, 2);
	memcpy(databuf,recvbuf,PLC_SCAN_LEN+ byte_to_recv);
	//if read error
	if(result == FALSE)
	{
		mbdata[id].err_code = 1;
		return FALSE;
	}
	if(recvbuf[PLC_SCAN_LEN] != WordNum * ByteNum)
	{
		sprintf(log_str,"MbFunc16(%2d):: byte num error",id);
		log_printf(ERR,1,PLC_APP,log_str);	
		mbdata[id].err_code = 1;	
		return FALSE;	
	}
	memcpy(sendbuf,recvbuf,WR_PLC_ACK_LEN);
	byte_to_send = WR_PLC_ACK_LEN;
	result = SOCKET_ERROR;
	result = SendPLC_MB(*p_sock, (char *)sendbuf, byte_to_send);
	if (result == SOCKET_ERROR )
	{
		sprintf(log_str,"MbFunc16(%2d)::error send() timeout or result=%d",id,result);
		log_printf(ERR,1,PLC_APP,log_str);	
		mbdata[id].err_code = 1;
		return (FALSE);
	}
	else
	{
		sprintf(log_str,"MbFunc16(%2d)::response success Addr=%d, Num=%d",id,Address,WordNum);
		log_printf(INF,1,PLC_APP,log_str);	
	}
	if( Address < POL_RECV_ADDR -1 )
	{
		type = 1; //ana
		if(Address == ANA_RECV_ADDR-1)
		{
			StartPos = 0;
			ptnum = (WordNum-1) * ByteNum / 4;
			offset = 2;
		}
		else
		{
			StartPos = (Address - ANA_RECV_ADDR ) /2;
			ptnum = WordNum * ByteNum / 4;
			offset = 0;
		}

	}
	else
	{
		type = 4; //pol
		if(Address == POL_RECV_ADDR-1)
		{		
			StartPos = 0 ;
			ptnum = (WordNum-1) * 8 * ByteNum;
			offset = 2;
		}
		else
		{
			StartPos = (Address - POL_RECV_ADDR ) * 16 ;
			ptnum = WordNum * 8 * ByteNum;
			offset = 0;
		}
	}

#ifdef PLCDEBUG
	printf("\n==== MbFunc16(%2d): Send data >>\n",id);
	for(i=0;i<result;i++)
	{
		printf("=%02x",sendbuf[i]);
		if((i+1)%20 == 0)
		{
			printf("\n");
		}		
	}
	printf("\n");
#endif
	
	//need to be added by yyp 2009.12.4
	if(BdRecvData(type,ptnum,StartPos,databuf+PLC_SCAN_LEN+1+offset,id) == FALSE)
	{
		sprintf(log_str,"BdRecvData(%d)  error",id);
		err_write(log_str);
		return FALSE;
	}
	return TRUE;
}


BOOL BdRecvData(int type, int ptnum,int start,char * buf, int id)
{

	int i;
	int ivalue;
	float fvalue;
	short tmp,status;
	int end;
	if(type == 1)
	{
		if((start+ptnum) > mbdata[id].anar.number)
		{
			sprintf(log_str,"BdRecvData ANA start=%d or ptnum=%d  error",start,ptnum);
			err_write(log_str);
			return FALSE;
		}
		for(i=start;i<start+ptnum;i++)
		{
			memcpy(&ivalue, buf + (i - start) * 4,4);	
			Swap4Str((char *)&ivalue, 1);
			fvalue = ((float)ivalue)/1000;
			if(BdANAByName(mbdata[id].anar.name[i],fvalue )!=TRUE )
			{
				sprintf(log_str,"BdAnaByName %s error",mbdata[id].anar.name[i]);
				err_write(log_str);	
			}
		}
	}
	else if( type == 4 )
	{
		if((start+ptnum) > mbdata[id].polr.number+16)
		{
			sprintf(log_str,"BdRecvData POL start=%d or ptnum=%d  error",start,ptnum);
			err_write(log_str);
			return FALSE;
		}
		end=start+ptnum;
		if( end>=mbdata[id].polr.number )
		{
			end = mbdata[id].polr.number;
		}
		for(i=start;i<end;i++)
		{
			memcpy(&tmp,buf + (int)((i-start)/16),2);
			Swap2Str((unsigned char *)&tmp,1);
			status  = GetCharBits(tmp,(unsigned short)(i%16),1); 
			if(BdINDByName(mbdata[id].polr.name[i],status )!=TRUE )
			{
				sprintf(log_str,"BdIndByName %s error",mbdata[id].polr.name[i]);
				err_write(log_str);	
			}
		}
	}
	else
	{
		return FALSE;
	}
	sprintf(log_str,"Get Function 16...");
	err_write(log_str);
	return  TRUE;
}



int FillMbArea(int id)
{
	int value,i,j,num,ptr;
	USHORT data_bit;
	memset( mbdata[id].mbarea, 0, 2 * MAX_MB_AREA );
	if(GetSendData(id)!=TRUE)
	{
		err_write("GetSendData error");
		return FALSE;
	}
	mbdata[id].mbarea[ANA_SEND_ADDR-1] = mbdata[id].anas.number;
	Swap2Str((char *)&(mbdata[id].mbarea[ANA_SEND_ADDR-1]),1);
	for(i=0;i<mbdata[id].anas.number;i++)
	{
		value = mbdata[id].anas.value[i];
		Swap4Str((char *)&value, 1);
		memcpy(mbdata[id].mbarea+1+2*i, &value,4);
	}
	mbdata[id].mbarea[POL_SEND_ADDR-1] = mbdata[id].pols.number;
	Swap2Str((char *)&(mbdata[id].mbarea[POL_SEND_ADDR-1]),1);
	memset(&data_bit,0,2);

	ptr=0;
	num= (mbdata[id].pols.number+15)/16;
	for(i=0;i<num;i++)
	{
		memset(&data_bit,0,2);
		for(j=0; j<16; j++)
		{
			if(ptr>=mbdata[id].pols.number)
				break;
			data_bit |= (mbdata[id].pols.status[ptr]&0x01)<<j;
			ptr++;
		}
		Swap2Str((unsigned char *)&data_bit,1);
		memcpy(mbdata[id].mbarea+POL_SEND_ADDR+i,&data_bit,2); 
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
	
	sprintf(filenamestr,"%s%s_mbs_%s",HOMEPATH,sMasterRec[id].NameID,ANA_SEND_FILE);
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
			printf("%2d: anas.name %d pnt: %s\n",id, j,mbdata[id].anas.name[j]);
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
	sprintf(log_str,"%2d: GenAnaSendTable: point number = %d",id,j);
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
	
	sprintf(filenamestr,"%s%s_mbs_%s",HOMEPATH,sMasterRec[id].NameID,ANA_RECV_FILE);
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
	while( fscanf(fp,"%s	%s	%s",iStr,tmpstr, mbdata[id].anar.name[j])!=EOF )
	{
		if(strcmp(mbdata[id].anar.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("%2d: anar.name %d pnt: %s\n",id,j,mbdata[id].anar.name[j]);
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
	sprintf(log_str,"%2d: GenAnaRecvTable: point number = %d",id,j);
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
	
	sprintf(filenamestr,"%s%s_mbs_%s",HOMEPATH,sMasterRec[id].NameID,POL_SEND_FILE);
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
	while( fscanf(fp,"%s	%s	%s",iStr,tmpstr, mbdata[id].pols.name[j])!=EOF )
	{
		if(strcmp(mbdata[id].pols.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("%2d: pols.name %d pnt: %s\n",id,j,mbdata[id].pols.name[j]);
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
	sprintf(log_str,"%2d: GenPolSendTable: point number = %d",id,j);
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
	
	sprintf(filenamestr,"%s%s_mbs_%s",HOMEPATH,sMasterRec[id].NameID,POL_RECV_FILE);
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
			printf("%2d: polr.name %d pnt: %s\n",id,j,mbdata[id].polr.name[j]);
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
	sprintf(log_str,"%2d: GenPolRecvTable: point number = %d",id,j);
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
			mbdata[id].anas.value[i] = (int)(db_data.fvalue * 1000) ;
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


int ReadMbsStnDef(char * pStnDefFile)
{	
	FILE	*fp;
	char	NameID[20], Ip[40];
	char	filebuf[LINE_BUF_CHAR];
	int		r;

	r=0;
	MasterNum = 0;
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
				printf("\nRead Station define file %s: master num = %d!!!",pStnDefFile,MasterNum);
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
		strcpy( sMasterRec[MasterNum].NameID, NameID );
		strcpy( sMasterRec[MasterNum].Ip, Ip );
		sMasterRec[MasterNum].stnid = MasterNum;
		if(sMasterRec[MasterNum].NameID[0] == '!')
		{
			//sprintf(log_str,"\nDisable Station name %s: IP = %s",sMasterRec[MasterNum].NameID,
			//	sMasterRec[MasterNum].Ip);
			//err_write(log_str);
			continue;
		}
		sprintf(log_str,"\nStation %2d name %s: IP = %s \n",MasterNum,sMasterRec[MasterNum].NameID,
			sMasterRec[MasterNum].Ip);
		log_write(INF,1,log_str);
		MasterNum++;
		if(MasterNum>=MAX_MASTER_NUM)
		{
			fclose( fp );
#ifdef DEBUG
			printf("\nRead Station define file %s: master num = %d!!!\n\n",pStnDefFile,MasterNum);
#endif
			return( TRUE );
		}
	}
}