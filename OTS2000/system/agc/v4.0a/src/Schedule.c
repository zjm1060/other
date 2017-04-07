#include	<string.h>
#include        <stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include        "../../../agc/v4.0a/inc/agc.h"

extern int BdSms(char* sms);
extern void CurveAllPoint(unsigned char,unsigned char,unsigned short,unsigned short,unsigned short,float*);
extern int GetControlState(void);
T_Schedule Schedule[97];
extern char  hostname[20];

void ReadLimitsDef(void)
{
	int i;
	char filebuf[]="/home/ems/h9000/agc/LIMITS.DEF";
	FILE *pFile;
	char strTmp1[20],strTmp2[20],strTmp3[20];
	pFile=fopen(filebuf,"r");
	if(pFile==NULL)
	{
		printf("File %s not exit!!!\n",filebuf);
		exit(0);
	}
	for(i=0;i<6;i++)
	{
		fscanf(pFile,"%s %s %s",strTmp1,strTmp2,strTmp3);
		if((strcmp(strTmp1,"MAX_ACTIVE_POWER")==0)&&(strcmp(strTmp2,"=")==0))
			LIMITS_MAX_ACTIVE_POWER=atoi(strTmp3);
		else if((strcmp(strTmp1,"MIN_ACTIVE_POWER")==0)&&(strcmp(strTmp2,"=")==0))
			LIMITS_MIN_ACTIVE_POWER=atoi(strTmp3);
		else if((strcmp(strTmp1,"MAX_SPINN_RESERVE")==0)&&(strcmp(strTmp2,"=")==0))
			LIMITS_MAX_SPINN_RESERVE=atoi(strTmp3);
		else if((strcmp(strTmp1,"MIN_SPINN_RESERVE")==0)&&(strcmp(strTmp2,"=")==0))
			LIMITS_MIN_SPINN_RESERVE=atoi(strTmp3);
		else if((strcmp(strTmp1,"MAX_VOLTAGE")==0)&&(strcmp(strTmp2,"=")==0))
			LIMITS_MAX_VOLTAGE=atoi(strTmp3);
		else if((strcmp(strTmp1,"MIN_VOLTAGE")==0)&&(strcmp(strTmp2,"=")==0))
			LIMITS_MIN_VOLTAGE=atoi(strTmp3);
	}
	fclose(pFile);
}

void Schedule_Broacast(int Today_Tomorrow,int Segment,int Type,float *Value)
{      //Today 1,Tomorrow 2; Segment: 1 RB1,2 RB2,9 Plant;Type:1 Active Power,2 voltage,3 Spinn; 
	int i,segment;
	
	DMS_COMMON common_aa;
	char ErrSms[100];
	float Value_Tmp[48];
   
   	if(Segment==1)
   		segment=1;
   	else if(Segment==2)
   		segment=2;
   	else if(Segment==9)
   		segment=0;
   	else
   		printf("Segment error!\n");

	if(Today_Tomorrow==1)
	{
		if(Type==1)
		{
			if(ReadEntryByNameStr(DB_NAME_TD_JRFHQX_SD[segment],&common_aa)==-1)
			{
		     		printf("No point %s in DB!\n",DB_NAME_TD_JRFHQX_SD[segment]);
		     		sprintf(ErrSms,"No point %s in DB!",DB_NAME_TD_JRFHQX_SD[segment]);
           	     		Err_log(ErrSms);
              		}
                	for(i=0;i<48;i++){Value_Tmp[i]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value_Tmp);
                
                	for(i=48;i<96;i++){Value_Tmp[i-48]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value_Tmp);
		}
		else if(Type==2)
		{
			if(ReadEntryByNameStr(DB_NAME_TD_JRDYQX[segment],&common_aa)==-1)
			{
		     		printf("No point %s in DB!\n",DB_NAME_TD_JRDYQX[segment]);
		     		sprintf(ErrSms,"No point %s in DB!",DB_NAME_TD_JRDYQX[segment]);
           	     		Err_log(ErrSms);
              		}
                	for(i=0;i<48;i++){Value_Tmp[i]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value_Tmp);
                
                	for(i=48;i<96;i++){Value_Tmp[i-48]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value_Tmp);
		}
		else if(Type==3)
		{
			if(ReadEntryByNameStr(DB_NAME_TD_JRFHQX_XZ[segment],&common_aa)==-1)
			{
		     		printf("No point %s in DB!\n",DB_NAME_TD_JRFHQX_XZ[segment]);
		     		sprintf(ErrSms,"No point %s in DB!",DB_NAME_TD_JRFHQX_XZ[segment]);
           	     		Err_log(ErrSms);
              		}
                	for(i=0;i<48;i++){Value_Tmp[i]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value_Tmp);
                
                	for(i=48;i<96;i++){Value_Tmp[i-48]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value_Tmp);
		}
        
        }
        else if(Today_Tomorrow==2)
        {
        	if(Type==1)
		{
			if(ReadEntryByNameStr(DB_NAME_TD_MRFHQX_SD[segment],&common_aa)==-1)
			{
		     		printf("No point %s in DB!\n",DB_NAME_TD_MRFHQX_SD[segment]);
		     		sprintf(ErrSms,"No point %s in DB!",DB_NAME_TD_MRFHQX_SD[segment]);
           	     		Err_log(ErrSms);
              		}
                	for(i=0;i<48;i++){Value_Tmp[i]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value_Tmp);
                
                	for(i=48;i<96;i++){Value_Tmp[i-48]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value_Tmp);
		}
		else if(Type==2)
		{
			if(ReadEntryByNameStr(DB_NAME_TD_MRDYQX[segment],&common_aa)==-1)
			{
		     		printf("No point %s in DB!\n",DB_NAME_TD_MRDYQX[segment]);
		     		sprintf(ErrSms,"No point %s in DB!",DB_NAME_TD_MRDYQX[segment]);
           	     		Err_log(ErrSms);
              		}
                	for(i=0;i<48;i++){Value_Tmp[i]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value_Tmp);
                
                	for(i=48;i<96;i++){Value_Tmp[i-48]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value_Tmp);
		}
		else if(Type==3)
		{
			if(ReadEntryByNameStr(DB_NAME_TD_MRFHQX_XZ[segment],&common_aa)==-1)
			{
		     		printf("No point %s in DB!\n",DB_NAME_TD_MRFHQX_XZ[segment]);
		     		sprintf(ErrSms,"No point %s in DB!",DB_NAME_TD_MRFHQX_XZ[segment]);
           	     		Err_log(ErrSms);
              		}
                	for(i=0;i<48;i++){Value_Tmp[i]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value_Tmp);
                
                	for(i=48;i<96;i++){Value_Tmp[i-48]=Value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value_Tmp);
		}
        }
}


int AckForSchedule(int Today_Tomorrow)
{
	int i,Num,Err,CurrentPoint;
	double Sum_AP,Sum_Voltage,Sum_Spin;
	time_t tim;
    	struct tm tmpt;
      
        tim=time (NULL);
        tmpt=*localtime (&tim);
        CurrentPoint=tmpt.tm_hour*4+tmpt.tm_min/15;
	Err=0;
	Sum_AP=0;
	Sum_Voltage=0;
	Sum_Spin=0;
	for(i=0;i<96;i++)
	{
		Sum_AP+=Schedule[i].APower;
		Sum_Voltage+=Schedule[i].Voltage;
		Sum_Spin+=Schedule[i].Spinn;
		Num=Schedule[i].Hour*4+Schedule[i].Minute/15;
		if(Num!=Schedule[i].Position)
		{
			Err=1;
			Schedule[i].Error=1;
		}
		if((Today_Tomorrow==1)&&(Num<CurrentPoint))continue;  //当日当前时刻前的点不校验
		if((Schedule[i].APower<LIMITS_MIN_ACTIVE_POWER)||(Schedule[i].APower>LIMITS_MAX_ACTIVE_POWER))
		{
			Err=1;
			Schedule[i].Error=1;
		}
		if((Schedule[i].Voltage<LIMITS_MIN_VOLTAGE)||(Schedule[i].Voltage>LIMITS_MAX_VOLTAGE))
		{
			Err=1;
			Schedule[i].Error=1;
		}
		if((Schedule[i].Spinn<LIMITS_MIN_SPINN_RESERVE)||(Schedule[i].Spinn>LIMITS_MAX_SPINN_RESERVE))
		{
			Err=1;
			Schedule[i].Error=1;
		}
	}
	/*if(Sum_AP!=Schedule[96].APower)
	{
		Err=1;
		Schedule[96].Error=1;
	}  GCL 20070919*/
	if(Sum_Voltage!=Schedule[96].Voltage)
	{   
		Err=1;
		Schedule[96].Error=1;
	}
	if(Sum_Spin!=Schedule[96].Spinn)
	{   
		Err=1;
		Schedule[96].Error=1;
	}
	return Err;
}

void ProcessSchedule(void)
{
	char strBuf[BUF_NUM],strBuf1[BUF_NUM],*pdest;
	char filebuf[50]="/home/ems/h9000/iccp/sch/schfilelist.txt";
	char FilePath[50]="/home/ems/h9000/iccp/sch/";
	char AckFilePath[50]="/home/ems/h9000/iccp/ack/";
	char Sms[100];
	float Value[96];
	
	int nLng=0,i,j,Pos,Num;
	int Today_Tomorrow,Segment;
	int RowNum,PointNum;
	char Date[32],Sch[32],FileName[80];
	char StrTmp1[10],StrTmp2[10],StrTmp3[10],StrTmp4[10],StrTmp5[10],StrTmp6[10];
	double Sum_AP,Sum_Voltage,Sum_Spin;
	int Year,Month,Day;
	struct tm  *Now;
	time_t    shjian;

	FILE *pFile,*pFile1;  
	Sum_AP=0;
	Sum_Voltage=0;
	Sum_Spin=0;
	
	sprintf(strBuf, "ls -1 %s*.SCH >%s",FilePath, filebuf);

	system(strBuf);  
	pFile = fopen(filebuf, "r");
	if (pFile != (FILE *)NULL)
	{		
		PointNum=0;
		memset(strBuf,0,BUF_NUM);
		while(fgets(strBuf, BUF_NUM, pFile) != (char *)NULL)
		{
			RowNum=0;
			nLng = strlen(strBuf);
			if(nLng==0)break;
			memset(strBuf+nLng-1,0,1);
			memset(strBuf1,0,BUF_NUM);
			strcpy(strBuf1,strBuf);
			pFile1=fopen(strBuf1,"r");
			if(pFile1!=(FILE*)NULL)
			{
				memset(strBuf,0,BUF_NUM);
				fgets(strBuf,BUF_NUM,pFile1);
				RowNum+=1;
				while(!((strstr(strBuf,"$.")!=NULL)&&(strstr(strBuf,"End")!=NULL)))
				{
					if((strstr(strBuf,"$.")!=NULL)||(strBuf[0]=='\n'))
					{
						memset(strBuf,0,BUF_NUM);
						fgets(strBuf,BUF_NUM,pFile1);
						RowNum+=1;
						continue;
					}
					if(strstr(strBuf,"$_")!=NULL)
					{
						if((pdest=strstr(strBuf,"DATE"))!=NULL)
						{
							memset(Date,0,32);
							Pos=pdest-strBuf;
							for(i=Pos+4;i<BUF_NUM;i++)
							{
								if(strBuf[i]!=' ')break;
							}
							for(j=0;j<30;j++,i++)
							{
								if((strBuf[i]==' ')||(strBuf[i]=='\n'))break;
								Date[j]=strBuf[i];
							}
						}
						else if((pdest=strstr(strBuf,"SCHEDULE"))!=NULL)
						{
							memset(Sch,0,32);
							Pos=pdest-strBuf;
							for(i=Pos+8;i<BUF_NUM;i++)
							{
								if(strBuf[i]!=' ')break;
							}
							for(j=0;j<30;j++,i++)
							{
								if((strBuf[i]==' ')||(strBuf[i]=='\n'))break;
								Sch[j]=strBuf[i];
							}
						}
						else if((pdest=strstr(strBuf,"FILENAME"))!=NULL)
						{
							memset(FileName,0,80);
							Pos=pdest-strBuf;
							for(i=Pos+8;i<BUF_NUM;i++)
							{
								if(strBuf[i]!=' ')break;
							}
							for(j=0;j<80;j++,i++)
							{
								if((strBuf[i]==' ')||(strBuf[i]=='\n'))break;
								FileName[j]=strBuf[i];
							}
						}
						RowNum+=1;
						memset(strBuf,0,BUF_NUM);
						fgets(strBuf,BUF_NUM,pFile1);
						continue;
					}

					memset(StrTmp1,0,10);
					memset(StrTmp2,0,10);
					memset(StrTmp3,0,10);
					memset(StrTmp4,0,10);
					memset(StrTmp5,0,10);
					memset(StrTmp6,0,10);
					
					Schedule[PointNum].RowNum=RowNum;
					for(i=0;i<BUF_NUM;i++)
						if((strBuf[i]!=' ')&&(strBuf[i]!='\t'))break;
					for(j=0;j<10;j++,i++)
					{
						if((strBuf[i]==' ')||(strBuf[i]=='\t'))break;
						StrTmp1[j]=strBuf[i];
					}
					Schedule[PointNum].Position=atoi(StrTmp1);

					if(PointNum<96)
					{
					    for(j=i;j<BUF_NUM;j++)
						  if((strBuf[j]!=' ')&&(strBuf[j]!='\t'))break;
					    i=j;
					    for(j=0;j<10;j++,i++)
						{
						   if((strBuf[i]==':')||(strBuf[i]==' '))break;
						   StrTmp2[j]=strBuf[i];
						}
					    Schedule[PointNum].Hour=atoi(StrTmp2);

						i++;
						for(j=0;j<10;j++,i++)
						{
							if((strBuf[i]==' ')||(strBuf[i]=='\t'))break;
							StrTmp3[j]=strBuf[i];
						}
						Schedule[PointNum].Minute=atoi(StrTmp3);
					}

					for(j=i;j<BUF_NUM;j++)
						if((strBuf[j]!=' ')&&(strBuf[j]!='\t'))break;
					i=j;
					for(j=0;j<10;j++,i++)
					{
						if((strBuf[i]==' ')||(strBuf[i]=='\t'))break;
						StrTmp4[j]=strBuf[i];
					}
					Schedule[PointNum].APower=atof(StrTmp4);

					for(j=i;j<BUF_NUM;j++)
						if((strBuf[j]!=' ')&&(strBuf[j]!='\t'))break;
					i=j;
					for(j=0;j<10;j++,i++)
					{
						if((strBuf[i]==' ')||(strBuf[i]=='\t'))break;
						StrTmp5[j]=strBuf[i];
					}
					Schedule[PointNum].Voltage=atof(StrTmp5);

					for(j=i;j<BUF_NUM;j++)
						if((strBuf[j]!=' ')&&(strBuf[j]!='\t'))break;
					i=j;
					for(j=0;j<10;j++,i++)
					{
						if((strBuf[i]==' ')||(strBuf[i]=='\t'))break;
						StrTmp6[j]=strBuf[i];
					}
                    			Schedule[PointNum].Spinn=atof(StrTmp6);
                    			PointNum+=1;
					memset(strBuf,0,BUF_NUM);
					RowNum+=1;
					fgets(strBuf,BUF_NUM,pFile1);
				}
			}
			fclose(pFile1);
			memset(strBuf,0,BUF_NUM);
			sprintf(strBuf,"rm %s%s",FilePath,FileName);
			system(strBuf);
			PointNum=0;
			pdest=strstr(FileName,".SCH"),
			Pos=pdest-FileName;
			memset(FileName+Pos,0,4);

			Today_Tomorrow=0;
			Segment=0;

			time ( &shjian );
			Now = localtime ( &shjian );
			memset(StrTmp1,0,10);
			for(i=0;i<=3;i++)
				StrTmp1[i]=Date[i];
			Year=atoi(StrTmp1);
			memset(StrTmp1,0,10);
			for(i=0;i<2;i++)
				StrTmp1[i]=Date[i+5];
			Month=atoi(StrTmp1);
			memset(StrTmp1,0,10);
			for(i=0;i<2;i++)
				StrTmp1[i]=Date[i+8];
			Day=atoi(StrTmp1);
			//printf("Year=%d,Month=%d,Day=%d,Now->tm_year=%d,Now->tm_mon=%d,Now->tm_mday=%d\n",Year,Month,Day,Now->tm_year,Now->tm_mon,Now->tm_mday);
			if((Year==Now->tm_year+1900)&&(Month==Now->tm_mon+1))
			{
				if(Day==Now->tm_mday)
					Today_Tomorrow=1;
				else if(Day==(Now->tm_mday+1))
					Today_Tomorrow=2;
			}
			if(strcmp(Sch,"RBSEG1")==0)
				Segment=1;
			else if(strcmp(Sch,"RBSEG2")==0)
				Segment=2;
			else if(strcmp(Sch,"RBPLANT")==0)
				Segment=9;	
			
			if((AckForSchedule(Today_Tomorrow)==1)||(Today_Tomorrow==0)||(Segment==0))
			{
				strcpy(Sms,"收到错误梯调曲线!");
            			if(GetControlState()==1)
              				BdSms(Sms);
				memset(strBuf1,0,BUF_NUM);
				strcpy(strBuf1,FilePath);
				strcat(FileName,".ER");
				strcat(strBuf1,FileName);	
				pFile1=fopen(strBuf1,"w");
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"$. Begin");
				fwrite(strBuf,strlen(strBuf),1,pFile1); 
                               
				if(Today_Tomorrow==0)
				{
					memset(strBuf,0,BUF_NUM);
					strcpy(strBuf,"$_DATE error!");
					fwrite(strBuf,strlen(strBuf),1,pFile1);
				}
				if(Segment==0)
				{
					memset(strBuf,0,BUF_NUM);
					strcpy(strBuf,"\n$_SCHEDULE error!");
					fwrite(strBuf,strlen(strBuf),1,pFile1);
				}
				for(i=0;i<96;i++)
				{
					Sum_AP+=Schedule[i].APower;
					Sum_Voltage+=Schedule[i].Voltage;
					Sum_Spin+=Schedule[i].Spinn;
					Num=Schedule[i].Hour*4+Schedule[i].Minute/15;
					if(Schedule[i].Error==1)
					{
						if(Num!=Schedule[i].Position)
						{
							memset(strBuf,0,BUF_NUM);
							sprintf(strBuf,"\nLine %d 'Number' and 'Time' do not match !",Schedule[i].RowNum);
							fwrite(strBuf,strlen(strBuf),1,pFile1);
						}
		
						if((Schedule[i].APower<LIMITS_MIN_ACTIVE_POWER)||(Schedule[i].APower>LIMITS_MAX_ACTIVE_POWER))
						{
							memset(strBuf,0,BUF_NUM);
							sprintf(strBuf,"\nLine %d,Number=%d- Setpoint for 'Active Power' out of range !",Schedule[i].RowNum,Schedule[i].Position);
							fwrite(strBuf,strlen(strBuf),1,pFile1);
						}

						if((Schedule[i].Voltage<LIMITS_MIN_VOLTAGE)||(Schedule[i].Voltage>LIMITS_MAX_VOLTAGE))
						{
							memset(strBuf,0,BUF_NUM);
							sprintf(strBuf,"\nLine %d,Number=%d- Setpoint for 'Voltage' out of range !",Schedule[i].RowNum,Schedule[i].Position);
							fwrite(strBuf,strlen(strBuf),1,pFile1);
						}
						if((Schedule[i].Spinn<LIMITS_MIN_SPINN_RESERVE)||(Schedule[i].Spinn>LIMITS_MAX_SPINN_RESERVE))
						{
							memset(strBuf,0,BUF_NUM);
							sprintf(strBuf,"\nLine %d,Number=%d- Setpoint for 'Spinning Reserve' out of range !",Schedule[i].RowNum,Schedule[i].Position);
							fwrite(strBuf,strlen(strBuf),1,pFile1);
						}
					}
				}
				if(Schedule[96].Error==1)
				{
					/*if(Sum_AP!=Schedule[96].APower)
					{
						memset(strBuf,0,BUF_NUM);
						sprintf(strBuf,"\nLine %d,Number=%d- Wrong checksum in column 'Active Power' !",Schedule[i].RowNum,Schedule[i].Position);
						fwrite(strBuf,strlen(strBuf),1,pFile1);
					}GCL 20070919*/
					if(Sum_Voltage!=Schedule[96].Voltage)
					{   
						memset(strBuf,0,BUF_NUM);
						sprintf(strBuf,"\nLine %d,Number=%d- Wrong checksum in column 'Voltage' !",Schedule[i].RowNum,Schedule[i].Position);
						fwrite(strBuf,strlen(strBuf),1,pFile1);
					}
					if(Sum_Spin!=Schedule[96].Spinn)
					{   
						memset(strBuf,0,BUF_NUM);
						sprintf(strBuf,"\nLine %d,Number=%d- Wrong checksum in column 'Spinning Reserve' !",Schedule[i].RowNum,Schedule[i].Position);
						fwrite(strBuf,strlen(strBuf),1,pFile1);
					}
				}
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$. End");
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				fclose(pFile1);
				
				memset(strBuf,0,BUF_NUM);
				sprintf(strBuf,"rcp -rp %s %s",strBuf1,AckFilePath);
				system(strBuf);
				memset(strBuf,0,BUF_NUM);
				sprintf(strBuf,"rm %s",strBuf1);
				system(strBuf);             
			}
			else 
			{	
				strcpy(Sms,"收到正确梯调曲线!");
            			if(GetControlState()==1)
              				BdSms(Sms);
				memset(strBuf1,0,BUF_NUM);
				strcpy(strBuf1,FilePath);
				strcat(FileName,".OK");
				strcat(strBuf1,FileName);	
				pFile1=fopen(strBuf1,"w");
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$. Begin");
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$. ----------------------------------------");
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$. O.K. No errors in file detected");
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$. O.K. File transferred to H9000 v4.0 System");
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				sprintf(strBuf,"\nCurrent Time:%02d-%02d-%02d  %02d:%02d:%02d",1900+Now->tm_year,Now->tm_mon+1,Now->tm_mday, Now->tm_hour, Now->tm_min, Now->tm_sec);
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$. ----------------------------------------");
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$_DATE ");
				strcat(strBuf, Date);
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$_SCHEDULE ");
				strcat(strBuf, Sch);
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$_FILENAME ");
				strcat(strBuf, FileName);
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				memset(strBuf,0,BUF_NUM);
				strcpy(strBuf,"\n$. End");
				fwrite(strBuf,strlen(strBuf),1,pFile1);
				fclose(pFile1);
				
				for(i=0;i<96;i++)
				{
					Value[i]=Schedule[i].APower;  
				//	printf("Apower[%d]=%f\t",i,Value[i]);
				//	if((i+1)%6==0)printf("\n");
			        }
				Schedule_Broacast(Today_Tomorrow,Segment,1,Value);
				for(i=0;i<96;i++)
				{
					Value[i]=Schedule[i].Voltage;
				//	printf("Voltage[%d]=%f\t",i,Value[i]);
				//	if((i+1)%6==0)printf("\n");
				}
				Schedule_Broacast(Today_Tomorrow,Segment,2,Value);
				for(i=0;i<96;i++)
				{
					Value[i]=Schedule[i].Spinn;
				//	printf("Spinn[%d]=%f\t",i,Value[i]);
				//	if((i+1)%6==0)printf("\n");
				}
				Schedule_Broacast(Today_Tomorrow,Segment,3,Value);
				
				memset(strBuf,0,BUF_NUM);
				sprintf(strBuf,"rcp -rp %s %s",strBuf1,AckFilePath);
				system(strBuf);
				memset(strBuf,0,BUF_NUM);
				sprintf(strBuf,"rm %s",strBuf1);
				system(strBuf);
			}
		}	
	}		
	fclose(pFile);
} 


	
