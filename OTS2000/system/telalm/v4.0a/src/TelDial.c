/*
#include "/users/ems/system/telalm/v3.0a/inc/tele.h"

char		TelNumber[MAX_ALM_CLASS][MAX_TEL_NUM][50];  
char		BpNumber[MAX_ALM_CLASS][MAX_BP_NUM][50];  
extern int	LockFlag;
extern	char *voxchannels[];
struct
{	char text[100];
	char time[20];
	char dev[50];
	char lname[50];
	char property[10];
	int	 cls[MAX_ALM_CLASS];
}telask_buf;
	  	
FILE		*fp;
int			i, j, chdev1, chdev2, chdev3, chdev4, cls1, cls2, cls3, cls4;
char		NumStr[20], cmd[200],	tmp1[50], tmp2[50], tmp3[50], tmp4[50], tmp5[50];
int 		cares;
DX_CAP 		capp;

void TelDial()
{
#ifdef	  DIALOGIC_VALID
	for (i=CHANSTARTNUM;i<CHANNUM+CHANSTARTNUM;i++)
	{	
	if(CHANUSE[i]!=1)continue;
	  strcpy(dev[i].voxname,voxchannels[i]);
		if ((dev[i].vox = dx_open(dev[i].voxname,0)) == -1 )
		{	printf("Error　opening　channel　%s\n",dev[i].voxname);
		    exit(1);
		}
	}	
	for(;;)
	{
		printf("\nTelDail() :  === 等待电话报警 ===\n");
//		dx_sethook(chdev1, DX_OFFHOOK, EV_SYNC);
 //       	printf("THE PHONE IS OFFHOOK\n ");
		while( _access(TELASK_FILE,0)==-1 ){ Sleep(2*1000);}

		if( (fp=fopen(TELASK_FILE,"r"))==NULL )
		{
			printf("can't open %s\n",TELASK_FILE); Sleep(3*1000); continue;
		}
		else
		{
			while( fscanf(fp,"%s %s %s %s %s %d %d %d %d",tmp1,tmp2,tmp3,tmp4,tmp5,&cls1,&cls2,&cls3,&cls4)!=EOF ) 
			{		strcpy( telask_buf.text,tmp1 );
					strcpy( telask_buf.time,tmp2 ); 
					strcpy( telask_buf.dev,tmp3 );
					strcpy( telask_buf.lname,tmp4 );
					strcpy( telask_buf.property,tmp5 );
					telask_buf.cls[0] = cls1;
					telask_buf.cls[1] = cls2;
					telask_buf.cls[2] = cls3;
					telask_buf.cls[3] = cls4;
			}			
			fclose( fp ); 
			sprintf(cmd,"del %s",TELASK_FILE); system(cmd); printf("%s\n",cmd);
		}

		ReadDailNumber();
					
		for(i=0;i<MAX_ALM_CLASS;i++)
		{
			dx_sethook(chdev1, DX_ONHOOK, EV_SYNC);

			if ( telask_buf.cls[i]!=1 )continue;			
				
			for(j=0;j<MAX_BP_NUM;j++)//判断BP机
			{ 	
				if( strcmp(BpNumber[i][j],"no")!=0 )
				{
					dx_sethook(chdev1, DX_ONHOOK, EV_SYNC);	Sleep(1000);

				    printf("\n@@@拨出BP机号码 == %s ( class=%d num=%d )\n",BpNumber[i][j],i+1,j+1); 
					strcpy(NumStr,BpNumber[i][j]);

					dx_sethook(chdev1, DX_ONHOOK, EV_SYNC);
				    dx_clrcap(&capp);
				    capp.ca_nbrdna = 2;
					dx_sethook(chdev1, DX_OFFHOOK, EV_SYNC);

			    	if ((cares = dx_dial(chdev1,NumStr,&capp,DX_CALLP | EV_SYNC )) ==-1)
					{ 
						printf("dx_dial bpnumber=%s error\n",NumStr); 
						while( LockFlag!=NO )Sleep(10);
						LockFlag=YES; strcat(NumStr,"_BP"); DialRecord(telask_buf.text,NumStr,DIAL_ERR); LockFlag=NO;
					}
					else
					{ 
						while( LockFlag!=NO )Sleep(10);
						LockFlag=YES; strcat(NumStr,"_BP"); DialRecord(telask_buf.text,NumStr,DIAL_OK); LockFlag=NO; 
					}
				}
			}
			
			for(j=0;j<MAX_TEL_NUM;j++)//判断电话号码
			{			
				if( strcmp(TelNumber[i][j],"no")!=0 )
				{
					dx_sethook(chdev1, DX_ONHOOK, EV_SYNC);Sleep(1000);

					printf("\n###拨出电话号码 == %s ( class=%d num=%d )\n",TelNumber[i][j],i+1,j+1); 
					strcpy(NumStr,TelNumber[i][j]);

					dx_sethook(chdev1, DX_ONHOOK, EV_SYNC);
					dx_clrcap(&capp);
					capp.ca_nbrdna = 8;	//Allow 8 rings before no answer
					capp.ca_noanswer =15;
					capp.ca_ansrdgl = 1;
					capp.ca_maxansr = 1;
					
					dx_sethook(chdev1, DX_OFFHOOK, EV_SYNC);

					if ( (cares = dx_dial(chdev1,NumStr,&capp,DX_CALLP | EV_SYNC )) !=CR_CNCT//==-1  call connected 
							|| (ATDX_CPTERM(chdev1)== CR_BUSY)                       //line was busy 
							|| (ATDX_CPTERM(chdev1)== CR_NOANS) )                    //no answer 
					{
						printf("dx_dial dial_number=%s error\n",NumStr);
						dx_sethook(chdev1, DX_ONHOOK, EV_SYNC);
						while( LockFlag!=NO )Sleep(10);
					    LockFlag=YES; strcat(NumStr,"_AUD"); DialRecord(telask_buf.text,NumStr,DIAL_ERR); LockFlag=NO;
					}
					else
					{	
						while( LockFlag!=NO )Sleep(10);
						LockFlag=YES; strcat(NumStr,"_AUD"); DialRecord(telask_buf.text,NumStr,DIAL_OK); LockFlag=NO;

						sprintf(cmd,"\\语音合成\\bin\\cwav.exe 1 0 %s","您好,水电厂计算机监控系统向您报告"); system( cmd ); 
						if( tel_playwav_msg( chdev1,"\\语音合成\\h9000wav\\test.wav")<0) 
							continue; 

						sprintf(cmd,"\\语音合成\\bin\\cwav.exe 1 0 %s",telask_buf.time); system( cmd );
						if( tel_playwav_msg( chdev1,"\\语音合成\\h9000wav\\test.wav")<0 ) 
							continue;
						else 
							printf("telephone-->>> %s ",telask_buf.time);

						sprintf(cmd,"\\语音合成\\bin\\cwav.exe 1 0 %s",telask_buf.dev); system( cmd );
						if( tel_playwav_msg( chdev1,"\\语音合成\\h9000wav\\test.wav")<0 ) 
							continue; 
						else 
							printf("%s ",telask_buf.dev);

						sprintf(cmd,"\\users\\ems\\project\\dps\\v3.0a\\aud\\%s",telask_buf.lname); 
						if( tel_playwav_msg( chdev1,cmd)<0 ) 
							continue;
						else 
							printf("%s ",telask_buf.lname);

						sprintf(cmd,"\\语音合成\\bin\\cwav.exe 1 0 %s",telask_buf.property); system( cmd ); 
						if( tel_playwav_msg( chdev1,"\\语音合成\\h9000wav\\test.wav")<0 ) 
							continue;
						else 
							printf("%s \n",telask_buf.property);

						Sleep(500);
						sprintf(cmd,"\\语音合成\\bin\\cwav.exe 1 0 %s","详情请拨打计算机监控系统查询电话,谢谢"); system( cmd ); 
						if( tel_playwav_msg( chdev1,"\\语音合成\\h9000wav\\test.wav")<0 ) 
							continue;
					}

					Sleep(500);
				}
			}               
		}  
		dx_sethook(chdev1, DX_ONHOOK, EV_SYNC);
	}  //for(;;)

	dx_close(chdev1);
#endif

}


void ReadDailNumber()
{	FILE	*fp;
	int		i, j;
	char	n1[50], n2[50], n3[50], n4[50], tmp[200];
	while( (fp=fopen(TELNUMBER_FILE,"r"))==NULL )
	{	printf("Can't open %s\n",TELNUMBER_FILE); Sleep(4*1000); }

	for(i=0;i<MAX_ALM_CLASS;i++)
	{ 
		for(j=0;j<MAX_TEL_NUM;j++) strcpy(TelNumber[i][j],"");
	}

	for(i=0;i<MAX_ALM_CLASS;i++)
	{ 
		for(j=0;j<MAX_BP_NUM;j++) strcpy(BpNumber[i][j],"");
	}

	fscanf(fp,"%s %s %s",tmp,tmp,tmp);
	for(i=0;i<(MAX_TEL_NUM+MAX_BP_NUM);i++)
	{
		fscanf(fp,"%s %s %s %s %s",tmp,n1,n2,n3,n4);
		if( i<MAX_TEL_NUM )
		{ strcpy(TelNumber[0][i],n1); 
		  strcpy(TelNumber[1][i],n2); 
		  strcpy(TelNumber[2][i],n3); 
		  strcpy(TelNumber[3][i],n4); 
		}
		else
		{ strcpy(BpNumber[0][i-MAX_TEL_NUM],n1); 
		  strcpy(BpNumber[1][i-MAX_TEL_NUM],n2);
		  strcpy(BpNumber[2][i-MAX_TEL_NUM],n3);
		  strcpy(BpNumber[3][i-MAX_TEL_NUM],n4);
		}
	}
	fclose(fp);
}


void DialRecord( char *textstr, char *PhoneNumber, char *Status )
{ 
  FILE          *recfp;
  char 	    	FileName[200];
  struct tm 	*today;
  time_t		ltime;
  
  ltime=time(NULL);    today=localtime(&ltime);	

  // 删除历史拨号记录文件 (保留"DIAL_RECORD_SAVEDAY"天)
  ltime = ltime - (60*60)*24*DIAL_RECORD_SAVEDAY;
  today=localtime(&ltime);

  sprintf(FileName,"del %s%04d-%02d-%02d.txt", DIAL_RECORD_PATH, 
	    	(*today).tm_year+1900,	(*today).tm_mon+1,	(*today).tm_mday  ); 
	
  if( _access(FileName,0)!=-1 )
  { 
	  printf("DIALALM.EXE: %s\n",FileName); system(FileName);   
  }	
		
  ltime=time(NULL);    today=localtime(&ltime);	
  sprintf( FileName, "%s%04d-%02d-%02d.txt",DIAL_RECORD_PATH,
	     (*today).tm_year+1900, (*today).tm_mon+1, (*today).tm_mday );		
	
  if( (recfp=fopen(FileName,"a+"))==NULL )
  { 
	  printf("\nDIALALM.EXE: can't open(w) the file %s!\n",FileName); return;
  }  
  else
  {   
     fprintf( recfp,"%04d-%02d-%02d %02d:%02d:%02d %25s  dialout===>>> %18s : %6s\n",
	             (*today).tm_year+1900, (*today).tm_mon+1, 				 
				 (*today).tm_mday,      (*today).tm_hour, 
				 (*today).tm_min,       (*today).tm_sec,
				 textstr, PhoneNumber, Status						);	
  }
  
  fclose(recfp);
  return;
}

*/

