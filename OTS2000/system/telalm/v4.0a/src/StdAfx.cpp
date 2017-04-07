// stdafx.cpp : source file that includes just the standard includes
//	teldef.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "../inc/stdafx.h"

CString	TelAudioNum[MAX_ALM_CLASS][MAX_TEL_NUM];  //[cls][num]
CString	TelCallNum[MAX_ALM_CLASS][MAX_BP_NUM];
CString	TelSmsNum[MAX_ALM_CLASS][MAX_MOBILE_NUM];
CString	TelSmsDef[MAX_SMSDEF_NUM][MAX_SMSDEF_STY];
CString	TelDef[MAX_TELDEF_NUM][MAX_TELDEF_STY];

CString	TelSmsSend[MAX_SMSSEND_NUM][3];

CString	SmsName[MAX_SMSNUM_NUM][1];
CString	SmsNum[MAX_SMSNUM_NUM][1];
int     SmsPre[MAX_SMSNUM_NUM][1];
int     SmsBytimePre[MAX_SMSNUM_NUM][1];
STN		Stn[STATION_ENTRY_NUMBER+1];		
STN		AnaStn[STATION_ENTRY_NUMBER+1];		

REAL_SW RealSW[MAX_SW_NUM];
REAL_ANA RealANA[MAX_ANA_NUM];

int		Num, AnaNum, StnNum, PageNum, CurrentPage, CurrentStnId;
int     AnaStnNum,AnaCurrentStnId;
int     smsPageNum, smsCurrentPage;
int     telPageNum, telCurrentPage;
int     smsnumPageNum, smsnumCurrentPage;
int 	smssendCurPag,	smssendPageNum;
int 	anadefCurPag,	anadefPageNum;

struct
{	char			CName[12];
	char			StnName[20];
	unsigned char	StnStartDevid;
	unsigned char	StnEndDevid;
}StnDef[STATION_ENTRY_NUMBER];

struct
{	char			CName[12];
	char			StnName[20];
	unsigned char	StnStartDevid;
	unsigned char	StnEndDevid;
}StnAnaDef[STATION_ENTRY_NUMBER];

void GetTelephoneNum()
{
	FILE	*fp;
	int		i, j;
	char	str[200], n1[50], n2[50], n3[50], n4[50], tmp[200];



	if( (fp=fopen(TELNUMBER_FILE,"r"))==NULL )
	{	sprintf(str,"Can't open %s !!!\n",TELNUMBER_FILE);
		AfxMessageBox(str,MB_OK, 0); exit(0);
	}

	for(i=0;i<MAX_ALM_CLASS;i++)
	{ 
		for(j=0;j<MAX_TEL_NUM;j++) TelAudioNum[i][j] = "no";
	}

	for(i=0;i<MAX_ALM_CLASS;i++)
	{ 
		for(j=0;j<MAX_BP_NUM;j++) TelCallNum[i][j] = "no";
	}

	fscanf(fp,"%s %s %s",tmp,tmp,tmp);
	for(i=0;i<(MAX_TEL_NUM+MAX_BP_NUM);i++)
	{
		if(fscanf(fp,"%s %s %s %s %s",tmp,n1,n2,n3,n4)==EOF) break;
		if( i<MAX_TEL_NUM )
		{ TelAudioNum[0][i] = n1; 
		  TelAudioNum[1][i] = n2; 
		  TelAudioNum[2][i] = n3; 
		  TelAudioNum[3][i] = n4; 
		}
		else
		{ TelCallNum[0][i-MAX_TEL_NUM] = n1; 
		  TelCallNum[1][i-MAX_TEL_NUM] = n2;
		  TelCallNum[2][i-MAX_TEL_NUM] = n3;
		  TelCallNum[3][i-MAX_TEL_NUM] = n4;
		}
	}
	fclose(fp);
/*
	for(i=0;i<MAX_ALM_CLASS;i++)
	{ 
		for(j=0;j<MAX_TEL_NUM;j++)
		{
			sprintf(str,"class=%d num=%d  tel=%s",i,j,TelAudioNum[i][j]);
			AfxMessageBox(str,MB_OK, 0); 
		}
	}
	
	for(i=0;i<MAX_ALM_CLASS;i++)
	{ 
		for(j=0;j<MAX_BP_NUM;j++) 
		{
			sprintf(str,"class=%d num=%d  bp=%s",i,j,TelCallNum[i][j]);
			AfxMessageBox(str,MB_OK, 0); 
		}
	}
*/
}

void GetMobileNum()
{
	FILE	*fp;
	int		i, j;
	char	str[200], n1[50], n2[50], n3[50], n4[50], tmp[200];
	if( (fp=fopen(MOBILENUMBER_FILE,"r"))==NULL )
	{	sprintf(str,"Can't open %s !!!\n",MOBILENUMBER_FILE);
		AfxMessageBox(str,MB_OK, 0); exit(0);
	}

	for(i=0;i<MAX_ALM_CLASS;i++)
	{ 
		for(j=0;j<MAX_MOBILE_NUM;j++) TelSmsNum[i][j] = "no";
	}

	fscanf(fp,"%s %s %s",tmp,tmp,tmp);
	for(i=0;i<MAX_MOBILE_NUM;i++)
	{
		if(fscanf(fp,"%s %s %s %s %s",tmp,n1,n2,n3,n4)==EOF) break;
		TelSmsNum[0][i] = n1; 
		TelSmsNum[1][i] = n2; 
		TelSmsNum[2][i] = n3; 
		TelSmsNum[3][i] = n4; 
	}
	fclose(fp);
/*
	for(i=0;i<MAX_ALM_CLASS;i++)
	{ 
		for(j=0;j<MAX_MOBILE_NUM;j++)
		{
			sprintf(str,"class=%d num=%d  tel=%s",i,j,TelSmsNum[i][j]);
			AfxMessageBox(str,MB_OK, 0); 
		}
	}	
*/
}

void SaveTelephoneNum()
{ 	FILE	*fp;
    int i;
	fp=fopen(TELNUMBER_FILE,"w");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	fprintf(fp,"*Number************Class1************Class2*************Class3***********Class4*****\n");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	for(i=0;i<MAX_TEL_NUM;i++)
	fprintf(fp,"phone%d      %15s %15s %15s %15s\n",i+1,TelAudioNum[0][i],TelAudioNum[1][i],TelAudioNum[2][i],TelAudioNum[3][i]);
	fclose(fp);
}


void SaveMobileNum()
{ 	FILE	*fp;
    int i;
	fp=fopen(MOBILENUMBER_FILE,"w");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	fprintf(fp,"*Number************Class1************Class2*************Class3***********Class4*****\n");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	for(i=0;i<MAX_MOBILE_NUM;i++)
	fprintf(fp,"mobile%d   %15s %15s %15s %15s\n",i+1,TelSmsNum[0][i],TelSmsNum[1][i],TelSmsNum[2][i],TelSmsNum[3][i]);
	fclose(fp);
}

void SaveSmsDef()
{ 	FILE	*fp;
    int     i;
	fp=fopen(SMSDEF_FILE,"w");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	fprintf(fp,"*Num*****Inquiryingcode***************logicname***************\n");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	for(i=0;i<MAX_SMSDEF_NUM;i++)
	fprintf(fp,"%4d %20s %50s\n",i+1,TelSmsDef[i][0],TelSmsDef[i][2]);

	fclose(fp);
}

void SaveSmsSend()
{ 	FILE	*fp;
    int     i;
	fp=fopen(SMSSEND_FILE,"w");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	fprintf(fp,"*Num****************logicname***************\n");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	for(i=0;i<MAX_SMSSEND_NUM;i++)
	fprintf(fp,"%4d  %50s\n",i+1,TelSmsSend[i][1]);

	fclose(fp);
}

void GetSmsSend()
{
	FILE	*fp;
	int		i, j;
	char	str[200], n0[50],  n2[50],  tmp[200];



	if( (fp=fopen(SMSSEND_FILE,"r"))==NULL )
	{	sprintf(str,"Can't open %s !!!\n",SMSSEND_FILE);
		AfxMessageBox(str,MB_OK, 0); exit(0);
	}

	for(i=0;i<MAX_SMSSEND_NUM;i++)
	{ 
		for(j=0;j<3;j++) TelSmsSend[i][j] = "no";
	}

	fscanf(fp,"%s %s %s",tmp,tmp,tmp);
	for(i=0;i<MAX_SMSSEND_NUM;i++)
	{
//		if(fscanf(fp,"%s %s %s %s",n0,n1,n2,n3)==EOF) break;
		if(fscanf(fp,"%s %s",n0,n2)==EOF) break;
//		TelSmsSend[i][0] = n1; 
		TelSmsSend[i][1] = n2; 
//		TelSmsSend[i][2] = n3; 
	}
	smssendPageNum	   = MAX_SMSSEND_NUM/10;  if( (MAX_SMSSEND_NUM%10)!=0 )smssendPageNum++;
	smssendCurPag	= 1;

	fclose(fp);
}

void GetSmsDef()
{
	FILE	*fp;
	int		i, j;
	char	str[200], n0[50], n1[50], n3[50], tmp[200];



	if( (fp=fopen(SMSDEF_FILE,"r"))==NULL )
	{	sprintf(str,"Can't open %s !!!\n",SMSDEF_FILE);
		AfxMessageBox(str,MB_OK, 0); exit(0);
	}

	for(i=0;i<MAX_SMSDEF_NUM;i++)
	{ 
		for(j=0;j<MAX_SMSDEF_STY;j++) TelSmsDef[i][j] = "no";
	}

	fscanf(fp,"%s %s %s",tmp,tmp,tmp);
	for(i=0;i<MAX_SMSDEF_NUM;i++)
	{
		if(fscanf(fp,"%s %s %s",n0,n1,n3)==EOF) break;
		TelSmsDef[i][0] = n1; 
//		TelSmsDef[i][1] = n2; 
		TelSmsDef[i][2] = n3; 
//		TelSmsDef[i][3] = n4; 
	}
	smsPageNum	   = MAX_SMSDEF_NUM/10;  if( (MAX_SMSDEF_NUM%10)!=0 )smsPageNum++;
	smsCurrentPage	= 1;

	fclose(fp);
}

void GetTelDef()
{
	FILE	*fp;
	int		i, j;
	char	str[200], n0[50], n1[50], n3[50],  tmp[200];



	if( (fp=fopen(TELDEF_FILE,"r"))==NULL )
	{	sprintf(str,"Can't open %s !!!\n",TELDEF_FILE);
		AfxMessageBox(str,MB_OK, 0); exit(0);
	}

	for(i=0;i<MAX_TELDEF_NUM;i++)
	{ 
		for(j=0;j<MAX_TELDEF_STY;j++) TelDef[i][j] = "no";
	}

	fscanf(fp,"%s %s %s",tmp,tmp,tmp);
	for(i=0;i<MAX_TELDEF_NUM;i++)
	{
//		if(fscanf(fp,"%s %s %s %s %s",n0,n1,n2,n3,n4)==EOF) break;
		if(fscanf(fp,"%s %s %s",n0,n1,n3)==EOF) break;
		TelDef[i][0] = n1; 
//		TelDef[i][1] = n2; 
		TelDef[i][2] = n3; 
//		TelDef[i][3] = n4; 
	}
	telPageNum	   = MAX_TELDEF_NUM/10;  if( (MAX_TELDEF_NUM%10)!=0 )telPageNum++;
	telCurrentPage	= 1;

	fclose(fp);
}

void SaveTelDef()
{ 	FILE	*fp;
    int     i;
	fp=fopen(TELDEF_FILE,"w");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	fprintf(fp,"*Num*****Inquiryingcode***********logicname***************\n");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	for(i=0;i<MAX_TELDEF_NUM;i++)
	fprintf(fp,"%4d %20s  %50s \n",i+1,TelDef[i][0],TelDef[i][2]);

	fclose(fp);
}

void ReadDefineFile( int flag )
{	FILE	*fp;
	int		n, i, j, cls1, cls2, cls3, cls4; 	     
	char	str[200], tmp1[150], tmp2[150], tmp3[150];

	//LoadInitDefine("LCU_STN");
	for(i=0;i<STATION_ENTRY_NUMBER;i++) 
	{ 
		strcpy(StnDef[i].CName,"");	strcpy(StnDef[i].StnName,"");
		StnDef[i].StnStartDevid=0;	StnDef[i].StnEndDevid=0;
	}
	if( (fp=fopen("/home/ems/h9000/gen/DBSTATION.DEF","r"))==NULL )
	{
		printf("\nCan't open /home/ems/h9000/gen/DBSTATION.DEF !!!\n");exit(-1);
	}
	else
	{	n = 0;
		while( fscanf(fp,"%s",tmp1)!=EOF)
		{					 
			if( strcmp(tmp1,"END")==0 ) break;
			if(strcmp(tmp1,"STATION" )!=0 ) continue;				
			for(i=0;i<4;i++)
			{	fscanf(fp,"%s %s %s ",tmp1,tmp2,tmp3);
				if( strcmp(tmp1,"NAME")==0 && strcmp(tmp2,"=")==0 ) strcpy(StnDef[n].StnName,tmp3);
				if( strcmp(tmp1,"REMARK")==0 && strcmp(tmp2,"=")==0) strcpy(StnDef[n].CName,tmp3);
				if( strcmp(tmp1,"FIRST_GROUP")==0 && strcmp(tmp2,"=")==0) StnDef[n].StnStartDevid=atoi(tmp3);
				if( strcmp(tmp1,"LAST_GROUP")==0 && strcmp(tmp2,"=")==0) StnDef[n].StnEndDevid=atoi(tmp3);
			}
			n++;
		}
		fclose(fp);
		if( n==0 ){ printf("\n!!! YOU HAVE TO DEFINE A STATION IN THE FILE DATABASE.DEF !!!\n"); exit(-1); }
	}

	for(i=0;i<MAX_SW_NUM;i++)
	{	
		strcpy(RealSW[i].l_name,""); 
		strcpy(RealSW[i].c_name,"");
	}

	if( (fp=fopen(SW_ALM_FILE,"r"))==NULL )
	{   sprintf(str,"Can't open %s !!!\n",SW_ALM_FILE);
		AfxMessageBox(str,MB_OK, 0); exit(0);
	}

	fscanf( fp,"%s  %s  %s\n",tmp1,tmp1,tmp1);
	
    Num=-1;	
  	while(  fscanf( fp,"%d %s %s %d %d %d %d", &j, tmp1, tmp3, &cls1, &cls2, &cls3, &cls4 )!=EOF)              
	{   Num++;
		RealSW[Num].class1 = cls1;
		RealSW[Num].class2 = cls2;
		RealSW[Num].class3 = cls3;
		RealSW[Num].class4 = cls4;
	
		strcpy(RealSW[Num].property,tmp1); 			
//		strcpy(RealSW[Num].c_name,tmp2);
		strcpy(RealSW[Num].l_name,tmp3); 
/*		
		sprintf(str, "%03d %10s %20s %20s  %d %d %d %d\n",Num,RealSW[Num].property,
				RealSW[Num].c_name, RealSW[Num].l_name, 
				RealSW[Num].class1, RealSW[Num].class2, RealSW[Num].class3, RealSW[Num].class4 );
		AfxMessageBox(str,MB_OK, 0);
*/
	}
	
	if( flag==0 )
	{ 
		PageNum	   = Num/12;  if( (Num%12)!=0 )PageNum++;
	    CurrentPage	= 1;
		CurrentStnId= 0;	
	}
   
	fclose(fp); 

	strcpy( Stn[0].StnCName, "Total" );
	strcpy( Stn[0].StnLName, "" );
	Stn[0].StartPnt = 0;
	Stn[0].EndPnt	= Num;
	Stn[0].PntNum	= Stn[0].EndPnt - Stn[0].StartPnt;
	Stn[0].PageNum	= Stn[0].PntNum/12;  if( (Stn[0].PntNum%12)!=0 ) Stn[0].PageNum++;

	StnNum=0;
	for(i=0;i<STATION_ENTRY_NUMBER;i++)
	{
		if( strcmp(StnDef[i].CName,"")!=0 && strcmp(StnDef[i].StnName,"")!=0 )
		{
			StnNum++;
			strcpy( Stn[StnNum].StnCName, StnDef[i].CName );
			strcpy( Stn[StnNum].StnLName, StnDef[i].StnName );

			sprintf(tmp1,"%s_",Stn[StnNum].StnLName);
			sprintf(tmp2,"%s.",Stn[StnNum].StnLName);

			for(j=0;j<Num;j++)
			{
				if( strstr( RealSW[j].l_name, tmp1 )!=NULL || strstr( RealSW[j].l_name, tmp2 )!=NULL)break;	
			}
			if( j>=Num ) Stn[StnNum].StartPnt=0; else Stn[StnNum].StartPnt=j;

			for(j=j;j<Num;j++)
			{
				if( strstr( RealSW[j].l_name, Stn[StnNum].StnLName )==NULL )break;
			}
			if( j>=Num ) Stn[StnNum].EndPnt=Num; else Stn[StnNum].EndPnt=j-1;
		
			Stn[StnNum].PntNum = Stn[StnNum].EndPnt - Stn[StnNum].StartPnt +1;
			Stn[StnNum].PageNum = Stn[StnNum].PntNum/12;  
			if( (Stn[StnNum].PntNum%12)!=0 ) Stn[StnNum].PageNum++;
			
			if( i!=0 && Stn[StnNum].StartPnt==0 && Stn[StnNum].EndPnt==Num )
			{
				StnNum--; continue;
			}
		}
	}
} 


void SaveDefineFile()
{	FILE	*fp;
	int		i; 	     

	fp=fopen(SW_ALM_FILE,"w");
	fprintf(fp,"-------------------------------------------------------------------------------------------------------------\n");
	fprintf(fp,"***********Fail******************************logicname*********************Class1***Class2***Class3***Class4\n");
	fprintf(fp,"-------------------------------------------------------------------------------------------------------------\n");
  	
	i = 0;
//	while(  strcmp(RealSW[i].l_name,"")!=0 &&  strcmp(RealSW[i].c_name,"")!=0  )        
	while(  strcmp(RealSW[i].l_name,"")!=0   )        
	{   
		fprintf(fp, "%4d %10s %50s		 %d     %d     %d     %d\n",i+1,RealSW[i].property,
				 RealSW[i].l_name, 
				RealSW[i].class1, RealSW[i].class2, RealSW[i].class3, RealSW[i].class4 );	
		i++;
	}
	
	fclose(fp); 
} 
     
void SaveAnaDefineFile()
{	FILE	*fp;
	int		i; 	     

	fp=fopen(ANA_ALM_FILE,"w");
	fprintf(fp,"-------------------------------------------------------------------------------------------------------------\n");
	fprintf(fp,"num***********Fail******************************logicname*********************Class1***Class2***Class3***Class4\n");
	fprintf(fp,"-------------------------------------------------------------------------------------------------------------\n");
  	
	i = 0;
//	while(  strcmp(RealANA[i].l_name,"")!=0 &&  strcmp(RealANA[i].c_name,"")!=0  )        
	while(  strcmp(RealANA[i].l_name,"")!=0  )        
	{   
		fprintf(fp, " %4d  %50s		 %d     %d     %d     %d\n",i+1,
//				RealANA[i].c_name, RealANA[i].l_name, 
			    RealANA[i].l_name, 
				RealANA[i].class1, RealANA[i].class2, RealANA[i].class3, RealANA[i].class4 );	
		i++;
	}
	
	fclose(fp); 
} 

void SaveSmsNum()
{ 	FILE	*fp;
    int     i;
	fp=fopen(SMSNUM_FILE,"w");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	fprintf(fp,"Num********name*********phone_number***********Inquirying**********Timing_sending*\n");
	fprintf(fp,"-------------------------------------------------------------------------------\n");
	for(i=0;i<MAX_SMSNUM_NUM;i++)
	fprintf(fp,"%4d %15s %20s        %d %d\n",i+1,SmsName[i][0],SmsNum[i][0],SmsPre[i][0],SmsBytimePre[i][0]);
	fclose(fp);
}

void GetSmsNum()
{
	FILE	*fp;
	int		i, j, cls,cls1;
	char	str[200], n0[10], n1[20],  tmp[200];



	if( (fp=fopen(SMSNUM_FILE,"r"))==NULL )
	{	sprintf(str,"Can't open %s !!!\n",SMSNUM_FILE);
		AfxMessageBox(str,MB_OK, 0); exit(0);
	}

	for(i=0;i<MAX_SMSNUM_NUM;i++)
	{ 
		SmsName[i][0] = "no" ; SmsNum[i][0] = "no" ;
	}

	fscanf(fp,"%s %s %s",tmp,tmp,tmp);
	for(i=0;i<MAX_SMSNUM_NUM;i++)
	{
		if(fscanf(fp,"%d %s %s %d %d",&j, n0, n1, &cls, &cls1 )==EOF) break;
		SmsName[i][0] = n0; 
		SmsNum[i][0] = n1; 
		SmsPre[i][0]  = cls; 
		SmsBytimePre[i][0]  = cls1;
	}
	smsnumPageNum	   = MAX_SMSNUM_NUM/10;  if( (MAX_SMSNUM_NUM%10)!=0 )smsnumPageNum++;
	smsnumCurrentPage	= 1;

	fclose(fp);
}

void ReadAnaDefineFile( int flag )
{	FILE	*fp;
	int		n, i, j, cls1, cls2, cls3, cls4; 	     
	char	str[200], tmp1[150], tmp2[150], tmp3[150];

	//LoadInitDefine("LCU_STN");
	for(i=0;i<STATION_ENTRY_NUMBER;i++) 
	{ 
		strcpy(StnAnaDef[i].CName,"");	strcpy(StnAnaDef[i].StnName,"");
		StnAnaDef[i].StnStartDevid=0;	StnAnaDef[i].StnEndDevid=0;
	}
	if( (fp=fopen("/home/ems/h9000/gen/DBSTATION.DEF","r"))==NULL )
	{
		printf("\nCan't open /home/ems/h9000/gen/DBSTATION.DEF !!!\n");exit(-1);
	}
	else
	{	n = 0;
		while( fscanf(fp,"%s",tmp1)!=EOF)
		{					 
			if( strcmp(tmp1,"END")==0 ) break;
			if(strcmp(tmp1,"STATION" )!=0 ) continue;				
			for(i=0;i<4;i++)
			{	fscanf(fp,"%s %s %s ",tmp1,tmp2,tmp3);
				if( strcmp(tmp1,"NAME")==0 && strcmp(tmp2,"=")==0 ) strcpy(StnAnaDef[n].StnName,tmp3);
				if( strcmp(tmp1,"REMARK")==0 && strcmp(tmp2,"=")==0) strcpy(StnAnaDef[n].CName,tmp3);
				if( strcmp(tmp1,"FIRST_GROUP")==0 && strcmp(tmp2,"=")==0) StnAnaDef[n].StnStartDevid=atoi(tmp3);
				if( strcmp(tmp1,"LAST_GROUP")==0 && strcmp(tmp2,"=")==0) StnAnaDef[n].StnEndDevid=atoi(tmp3);
			}
			n++;
		}
		fclose(fp);
		if( n==0 ){ printf("\n!!! YOU HAVE TO DEFINE A STATION IN THE FILE DATABASE.DEF !!!\n"); exit(-1); }
	}

	for(i=0;i<MAX_ANA_NUM;i++)
	{	
		strcpy(RealANA[i].l_name,""); 
		strcpy(RealANA[i].c_name,"");
	}

	if( (fp=fopen(ANA_ALM_FILE,"r"))==NULL )
	{   sprintf(str,"Can't open %s !!!\n",ANA_ALM_FILE);
		AfxMessageBox(str,MB_OK, 0); 
		exit(0);
	}

	fscanf( fp,"%s  %s  %s\n",tmp1,tmp1,tmp1);
	
    AnaNum=-1;	
  	while(  fscanf( fp,"%d  %s %d %d %d %d", &j, tmp3, &cls1, &cls2, &cls3, &cls4 )!=EOF)              
	{   AnaNum++;
		RealANA[AnaNum].class1 = cls1;
		RealANA[AnaNum].class2 = cls2;
		RealANA[AnaNum].class3 = cls3;
		RealANA[AnaNum].class4 = cls4;
	
//		strcpy(RealANA[AnaNum].c_name,tmp2);
		strcpy(RealANA[AnaNum].l_name,tmp3); 
/*		
		sprintf(str, "%03d %10s %20s %20s  %d %d %d %d\n",Num,RealSW[Num].property,
				RealSW[Num].c_name, RealSW[Num].l_name, 
				RealSW[Num].class1, RealSW[Num].class2, RealSW[Num].class3, RealSW[Num].class4 );
		AfxMessageBox(str,MB_OK, 0);
*/
	}
	if( flag==0 )
	{ 
		anadefPageNum	   = AnaNum/12;  if( (AnaNum%12)!=0 )anadefPageNum++;
	    anadefCurPag	= 1;
		AnaCurrentStnId= 0;	
	}
   
	fclose(fp); 

	strcpy( AnaStn[0].StnCName, "Total" );
	strcpy( AnaStn[0].StnLName, "" );
	AnaStn[0].StartPnt = 0;
	AnaStn[0].EndPnt	= AnaNum;
	AnaStn[0].PntNum	= AnaStn[0].EndPnt - AnaStn[0].StartPnt;
	AnaStn[0].PageNum	= AnaStn[0].PntNum/12;  if( (AnaStn[0].PntNum%12)!=0 ) AnaStn[0].PageNum++;

	AnaStnNum=0;
	for(i=0;i<STATION_ENTRY_NUMBER;i++)
	{
		if( strcmp(StnAnaDef[i].CName,"")!=0 && strcmp(StnAnaDef[i].StnName,"")!=0 )
		{
			AnaStnNum++;
			strcpy( AnaStn[AnaStnNum].StnCName, StnAnaDef[i].CName );
			strcpy( AnaStn[AnaStnNum].StnLName, StnAnaDef[i].StnName );

			sprintf(tmp1,"%s_",AnaStn[AnaStnNum].StnLName);
			sprintf(tmp2,"%s.",AnaStn[AnaStnNum].StnLName);

			for(j=0;j<AnaNum;j++)
			{
				if( strstr( RealANA[j].l_name, tmp1 )!=NULL || strstr( RealANA[j].l_name, tmp2 )!=NULL)break;	
			}
			if( j>=AnaNum ) AnaStn[AnaStnNum].StartPnt=0; else AnaStn[StnNum].StartPnt=j;

			for(j=j;j<AnaNum;j++)
			{
				if( strstr( RealANA[j].l_name, AnaStn[AnaStnNum].StnLName )==NULL )break;
			}
			if( j>=AnaNum ) AnaStn[AnaStnNum].EndPnt=Num; else AnaStn[AnaStnNum].EndPnt=j-1;
		
			AnaStn[StnNum].PntNum = AnaStn[AnaStnNum].EndPnt - AnaStn[AnaStnNum].StartPnt +1;
			AnaStn[StnNum].PageNum = AnaStn[AnaStnNum].PntNum/12;  
			if( (AnaStn[AnaStnNum].PntNum%12)!=0 ) AnaStn[StnNum].PageNum++;
			
			if( i!=0 && AnaStn[AnaStnNum].StartPnt==0 && AnaStn[AnaStnNum].EndPnt==AnaNum )
			{
				AnaStnNum--; continue;
			}
		}
	}
} 
