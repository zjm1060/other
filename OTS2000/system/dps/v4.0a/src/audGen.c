#include	"windows.h"
#include	"stdio.h"
#include	"../inc/jtaud.h"
#include	"../inc/msaud.h"
#include	"../inc/dps_functions_def.h"
#include	"../../../dms/v4.0a/inc/ems_init_def.h"



void	HandlerRoutine(DWORD); 
void	CreateAudioFile( char *audStr, char *filename,int ms_flag);
void	commonAudGen(void);
void	customAudGen(void);
void	projectAudGen(void);
void	LcuAudTableGen(POINTER pt);
void	audFilesGen(int audPtNum);
BOOL	TryAudAndNeedModify(int audPtNum);


#define	READ_MAX_FILE_NUM	20
#define	MAX_WAV_RECORD		10000


struct
{	
	char	filename[80];
	char	audStr[80];
	int     ms_voice;
} audTable[MAX_WAV_RECORD];


HANDLE	hConsoleOutput;

static	BOOL	exLangState;

int    faultpoint_use_msvoice=0;
int    failpoint_use_msvoice=0;
int    otherpoint_use_msvoice=0;
int    customAudGen_use_msvoice=0;
int    commonAudGen_use_msvoice=0;
int    speedof_msvoice=0;
void main()
{
	int		n;
	char	strTemp[256];
	COORD	dwSize= {256, 256};

	if(-1==InitStrTextDefine())
	{
		printf("\nFail to init str text define");
		exit(0);
	}
	hConsoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCtrlHandler((void *)HandlerRoutine,TRUE);
	SetConsoleTitle(_CS_("AUD_GEN_STR_TITLE"));
	SetConsoleTextAttribute(hConsoleOutput,  FOREGROUND_GREEN | FOREGROUND_INTENSITY );
	SetConsoleScreenBufferSize(hConsoleOutput, dwSize);

	CreateMutex(NULL,FALSE,"aud_gen");
	if( GetLastError()==ERROR_ALREADY_EXISTS )
	{
		printf("THE H9000 AUD GEN IS RUNNING NOW, CAN'T RESTART IT AGAIN !!! \n"); 
		Sleep(3*1000); 
		exit(0);
	}

	if(-1==MountDB(NULL))
	{
		printf("Fail to initialize DB!\n");
		exit(-1);
	}


#ifdef WINDOWS_OS 

	strTemp[0]='y';
	printf("\n%s  ", _CS_("AUD_GEN_STR_FAULTPOINT_USE_MSVOICE")); 
	gets(strTemp);	
	if(strTemp[0]=='y' || strTemp[0]=='Y')
		faultpoint_use_msvoice=1;
	else
		faultpoint_use_msvoice=0;
	
	strTemp[0]='y';
	printf("\n%s  ", _CS_("AUD_GEN_STR_FAILPOINT_USE_MSVOICE")); 
	gets(strTemp);	
	if(strTemp[0]=='y' || strTemp[0]=='Y')
		failpoint_use_msvoice=1;
	else
		failpoint_use_msvoice=0;

	strTemp[0]='y';
	printf("\n%s  ", _CS_("AUD_GEN_STR_OTHERPOINT_USE_MSVOICE")); 
	gets(strTemp);	
	if(strTemp[0]=='y' || strTemp[0]=='Y')
		otherpoint_use_msvoice=1;
	else
		otherpoint_use_msvoice=0;
	
	
	strTemp[0]='y';
	printf("\n%s  ", _CS_("AUD_GEN_STR_COMMON_USE_MSVOICE")); 
	gets(strTemp);	
	if(strTemp[0]=='y' || strTemp[0]=='Y')
		commonAudGen_use_msvoice=1;
	else
		commonAudGen_use_msvoice=0;
	
	strTemp[0]='y';
	printf("\n%s  ", _CS_("AUD_GEN_STR_CUSTOM_USE_MSVOICE")); 
	gets(strTemp);	
	if(strTemp[0]=='y' || strTemp[0]=='Y')
		customAudGen_use_msvoice=1;
	else
		customAudGen_use_msvoice=0;
	
	if(faultpoint_use_msvoice + failpoint_use_msvoice + customAudGen_use_msvoice+commonAudGen_use_msvoice>0)
	{
		strTemp[0]=0;
		printf("\n%s  ", _CS_("AUD_GEN_STR_SPEEDOF_MSVOICE")); 
		gets(strTemp);	
		speedof_msvoice= atoi(strTemp);

		if(speedof_msvoice<-10)
			speedof_msvoice=-10;
		if(speedof_msvoice>10)
			speedof_msvoice=10;

	}
#endif


	ReleaseAudioLibraryJT();
	if(-1==InitAudioLibraryJT(5, 9, 5, NULL))
	{
		printf("Fail to initialize JT audio kernal!\n");
		exit(-1);
	}
	
#ifdef WINDOWS_OS 
	ReleaseAudioLibraryMS();
	
	if(speedof_msvoice<-10) speedof_msvoice=-10;

	if(speedof_msvoice>10) speedof_msvoice=10;

	if(-1==InitAudioLibraryMS(0, -7, 100, speedof_msvoice, NULL, 0))
	{
		printf("Fail to initialize MS audio kernal!\n");
		exit(-1);
	}
#endif 
	
#ifdef EXTEND_LANGUAGE_SYSTEM
	for(;;)
	{ 
		printf("\n«Î—°‘Ò”Ô—‘ Please select language"); 
		printf("\n1. ÷–ŒƒChinese");
		printf("\n2. ”¢”ÔEnglish");
		printf("\n«Î ‰»Î Please input: ");
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);	
		n= atoi(strTemp);
		if(n==1)
		{
			SetExLangState(0);
			exLangState= 0;
			break;
		}
		else if(n==2)
		{
			SetExLangState(1);
			exLangState= 1;
			break;
		}
	}
#endif
	
	
	if(faultpoint_use_msvoice + failpoint_use_msvoice + customAudGen_use_msvoice+commonAudGen_use_msvoice==0)
	{
		PlayStrAudioJT( _CS_("AUD_GEN_STR_WELCOME") );
	}
	else if(faultpoint_use_msvoice + failpoint_use_msvoice + customAudGen_use_msvoice+commonAudGen_use_msvoice==4)
	{
#ifdef WINDOWS_OS 
		PlayStrAudioMS( _CS_("AUD_GEN_STR_WELCOME") );
#else
		PlayStrAudioJT( _CS_("AUD_GEN_STR_WELCOME") );
#endif
	}
	else
	{
		PlayStrAudioJT( _CS_("AUD_GEN_STR_WELCOME") );
#ifdef WINDOWS_OS 
		PlayStrAudioMS( _CS_("AUD_GEN_STR_WELCOME") );
#endif
	}

	for(;;)
	{ 
		printf("\n%s", _CS_("AUD_GEN_STR_SELECT_SUBENTRY")); 
		printf("\n%s", _CS_("AUD_GEN_STR_COM_AUD"));
		printf("\n%s", _CS_("AUD_GEN_STR_LCU_AUD"));
		printf("\n%s", _CS_("AUD_GEN_STR_DEFINE_AUD"));
		printf("\n%s", _CS_("AUD_GEN_STR_EXIT"));
		printf("\n%s", _CS_("AUD_GEN_STR_INPUT_SELECT"));
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);	
		n= atoi(strTemp);

		if( n==1 )
			commonAudGen();
		else if( n==2 ) 
			projectAudGen();
		else if( n==3 ) 
			customAudGen();
		else if( n==4 )
		{
#ifdef WINDOWS_OS 
			if(faultpoint_use_msvoice + failpoint_use_msvoice + customAudGen_use_msvoice+commonAudGen_use_msvoice==0)
				PlayStrAudioJT( _CS_("AUD_GEN_STR_THANKS") );
			else if(faultpoint_use_msvoice + failpoint_use_msvoice + customAudGen_use_msvoice+commonAudGen_use_msvoice==4)
			{
				PlayStrAudioMS( _CS_("AUD_GEN_STR_THANKS") );
			}
			else
			{
				PlayStrAudioMS( _CS_("AUD_GEN_STR_THANKS") );
				PlayStrAudioJT( _CS_("AUD_GEN_STR_THANKS") );
			}
#else
			PlayStrAudioJT( _CS_("AUD_GEN_STR_THANKS") );
		 
#endif
			printf("\n%s", _CS_("AUD_GEN_STR_END"));
			exit(0);
		}
	}
}


void commonAudGen(void)
{
	extern  DBS_GBL_COMMON  *db_ptr;
	int		i, j, k;
	char	strTemp[256];

	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_EVENT_AUD"));
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
			break;
		if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
		{
			for(i=0; i<MAX_PROCTYPE_NUM; i++)
			{
				for(j=0; j<MAX_MSG_GROUP_NUM; j++)
				{
					for(k=0; k<MAX_IND_STATE_NUM; k++)
					{
						char	string[128];
#ifdef EXTEND_LANGUAGE_SYSTEM
						if(exLangState)
							strcpy(string, db_ptr->msg_def[i].group[j].Exdef[k]);
						else
#endif
							strcpy(string, db_ptr->msg_def[i].group[j].def[k]);
						if(!strstr(string, "Err"))
						{
							printf("\n%s", string);
							CreateAudioFile(string, string,commonAudGen_use_msvoice);
						}
					}
				}
			}
			for(i=0; i<MAX_IND_STATE_NUM; i++)
			{
				sprintf(strTemp, "Err%d", i);
				printf("\n%s", strTemp);
				CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);
			}

			break;
		}
	}

	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_COMMAND_AUD"));
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
			break;
		if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
		{
			for(i=0; i<MAX_CMDTYPE_NUM; i++)
			{
				for(j=0; j<MAX_MSG_GROUP_NUM; j++)
				{
					for(k=0; k<MAX_CMD_OPER_TYPE_NUM; k++)
					{
						char	string[128];
#ifdef EXTEND_LANGUAGE_SYSTEM
						if(exLangState)
							strcpy(string, db_ptr->cmd_msg[i].group[j].Exdef[k]);
						else
#endif
							strcpy(string, db_ptr->cmd_msg[i].group[j].def[k]);
						if(!strstr(string, "Err"))
						{
							printf("\n%s", string);
							CreateAudioFile(string, string,commonAudGen_use_msvoice);
						}
					}
				}
			}
			for(i=0; i<MAX_CMD_OPER_TYPE_NUM; i++)
			{
				sprintf(strTemp, "Err%d", i);
				printf("\n%s", strTemp);
				CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);
			}

			break;
		}
	}

	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_ALM_STATE_AUD"));
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
			break;
		if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
		{
			strcpy(strTemp, _CS_("ALM_STR_H_ALM_LMT"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);
			
			strcpy(strTemp, _CS_("ALM_STR_H_WRN_LMT"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			strcpy(strTemp, _CS_("ALM_STR_L_WRN_LMT"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			strcpy(strTemp, _CS_("ALM_STR_L_ALM_LMT"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			strcpy(strTemp, _CS_("ALM_STR_TR_WRN_LMT"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			strcpy(strTemp, _CS_("ALM_STR_CTRL_BLOCK"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			strcpy(strTemp, _CS_("ALM_STR_OVERFLOW"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			strcpy(strTemp, _CS_("ALM_STR_CHANNEL_FAIL"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);
			break;
		}
	}
	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_NUMBER_AUD"));
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
			break;
		if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
		{
			for(i=0; i<10; i++)
			{
				sprintf(strTemp, "%d", i);
				printf("\n%s", strTemp);
				CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);
			}
			break;
		}
	}
	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_TIME_AUD"));
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
			break;
		if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
		{
			sprintf(strTemp, _CS_("AUD_GEN_STR_YEAR"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			sprintf(strTemp, _CS_("AUD_GEN_STR_MONTH"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			sprintf(strTemp, _CS_("AUD_GEN_STR_DAY"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			sprintf(strTemp, _CS_("AUD_GEN_STR_POINT"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			sprintf(strTemp, _CS_("AUD_GEN_STR_HOUR"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			sprintf(strTemp, _CS_("AUD_GEN_STR_MINUTE"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			sprintf(strTemp, _CS_("AUD_GEN_STR_SECOND"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			sprintf(strTemp, _CS_("AUD_GEN_STR_MILLISEC"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			for(i=0; i<60; i++)
			{
				sprintf(strTemp, "%02d", i);
				printf("\n%s", strTemp);
				CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);
			}

			for(i=0; i<50; i++)
			{
				sprintf(strTemp, "%d", i+1990);
				printf("\n%s", strTemp);
				CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);
			}
			break;
		}
	}
	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_UNIT_AUD"));
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
			break;
		if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
		{
			strcpy(strTemp, "KW");
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			strcpy(strTemp, "MW");
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);

			strcpy(strTemp, _CS_("AUD_GEN_STR_UNIT_METER"));
			printf("\n%s", strTemp);
			CreateAudioFile(strTemp, strTemp,commonAudGen_use_msvoice);
			
			break;
		}
	}
}



void customAudGen(void)
{
	FILE	*fp_def;
	char	strTemp[256], com_def_file[256];
	int		audPtNum;

LOOP1:
	printf(COM_DEFINE_FILE);
	printf("\n%s", _CS_("AUD_GEN_STR_MODIFY_PROMPT"));
	gets(strTemp);
	sprintf(com_def_file, "%s%s", SYS_DEFINE_PATH, COM_DEFINE_FILE);
	strcpy( strTemp,"%SystemRoot%\\system32\\notepad.exe "); 
	strcat(strTemp, com_def_file);
	system(strTemp);

	if((fp_def=fopen(com_def_file,"r"))==NULL )
	{	
		printf("\n!!! Can not open %s !!!\n\n", com_def_file ); 
		return;
	}

	audPtNum = 0;

#ifdef WINDOWS_OS 

	while( fgets(strTemp, sizeof(strTemp), fp_def) )
	{
		if(sscanf(strTemp,"%s %s %s %d",audTable[audPtNum].audStr,
			strTemp, audTable[audPtNum].filename,&audTable[audPtNum].ms_voice)==4 )
		{
			printf("%003d  %40s =====> %s  %d\n",audPtNum, audTable[audPtNum].audStr,
				audTable[audPtNum].filename,audTable[audPtNum].ms_voice);
			audPtNum++;
		}
	}

#else

	while( fgets(strTemp, sizeof(strTemp), fp_def) )
	{
		if(sscanf(strTemp,"%s %s %s",audTable[audPtNum].audStr,
			strTemp, audTable[audPtNum].filename)==3 )
		{
			audTable[audPtNum].ms_voice=customAudGen_use_msvoice;
			printf("%003d  %40s =====> %s\n",audPtNum, audTable[audPtNum].audStr,
				audTable[audPtNum].filename);
			audPtNum++;
		}
	}
#endif 

	fclose( fp_def );

	if(TryAudAndNeedModify(audPtNum))
		goto LOOP1;
	audFilesGen(audPtNum);	
}

void projectAudGen(void)
{
	char	strTemp[256], *pStr;
	int		stn_id, dev_id, rec_num;
	POINTER	pt;
	int		i;
	UCHAR	rec_type;
	char	rec_name[RECORD_NAME_SIZE], rec_longname[RECORD_LONGNAME_SIZE];

	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_INPUT_LCU"));
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if(strTemp[0]==0)
			break;

		pStr= strchr(strTemp, '.');
		if(pStr)
			*pStr= ' ';
		else
		{
			printf("\n%s", _CS_("AUD_GEN_STR_INPUT_ERROR"));
			continue;
		}

		if(sscanf(strTemp, "%d %d", &stn_id, &dev_id) != 2)
		{
			printf("\n%s", _CS_("AUD_GEN_STR_INPUT_ERROR"));
			continue;
		}

		pt.stn_id= (UCHAR)stn_id;
		pt.dev_id= (UCHAR)dev_id;
		if(-1==GetRecordNumById(pt.stn_id, pt.dev_id, &rec_num))
		{
			printf("\n%s", _CS_("AUD_GEN_STR_INPUT_LCU_ERROR"));
			continue;
		}

		for(i=0; i<rec_num; i++)
		{
			if(-1==GetRecTypeByOffset(pt.stn_id, pt.dev_id, (UCHAR)i, &rec_type))
				continue;
			pt.data_type= rec_type;
			GetRecNameByType(rec_type, rec_name);
			GetRecLongnameByType(rec_type, rec_longname);
			printf("\n%s %s %s", rec_longname, rec_name, _CS_("AUD_GEN_STR_NEW_GEN")); 
			FlushConsoleInputBuffer( hConsoleOutput );
			gets(strTemp);	
			if(strTemp[0]=='Y' || strTemp[0]=='y' || strTemp[0]==0)
				LcuAudTableGen(pt);
		}/*for(i)*/
	}/*for(;;)*/
}



void LcuAudTableGen(POINTER pt)
{
	char	strTemp[256], strTemp1[256];
	char	prj_def_file[256];
	int		iNum;
	FILE	*fp_def;
	int		entry_num;
	char	stn_name[STATION_COMMENT_SIZE];
	char	dev_name[GROUP_COMMENT_SIZE];
	char	entry_longname[ENTRY_LONGNAME_SIZE];
	char	ptNameStr[POINT_NAME_SIZE], ptLongName[256];
	BOOL	audAlm;
	int		i, audPtNum=0;
	int     faultpt=0,failpt=0;

	if(-1==GetEntryNumById(pt.stn_id, pt.dev_id, pt.data_type, &entry_num))
		return;
	if(entry_num==0)
		return;

	for(i=0; i<entry_num; i++)
	{
		pt.pt_id= i;
		IsPtAudAlmById(&pt, &audAlm);
	
		if(!audAlm)
			continue;
		GetPtNameById(&pt, ptNameStr, NULL);

		stn_name[0]= 0;
		dev_name[0]= 0;
		entry_longname[0]= 0;
		GetStnCommentById(pt.stn_id, stn_name);
		GetGroupCommentById(pt.stn_id, pt.dev_id, dev_name);
		GetEntryLongnameById(&pt, entry_longname);
		sprintf(ptLongName, "%s%s%s", stn_name, dev_name, entry_longname);
		FilterAudStr(ptLongName);
		
		audTable[audPtNum].ms_voice=0;
		faultpt=0;
		IsPtFaultById(&pt,&faultpt);
		failpt=0;
		IsPtFailById(&pt,&failpt);
		
		
		if(failpt)
		{
			if(failpoint_use_msvoice==1)
				audTable[audPtNum].ms_voice=1;
		}	
		
		if(faultpt)
		{	
			if(faultpoint_use_msvoice==1)
				
				audTable[audPtNum].ms_voice=1;	
		}	
		
		if(!faultpt && !failpt)
		{	
			if(otherpoint_use_msvoice==1)
				
				audTable[audPtNum].ms_voice=1;	
		}		

		strcpy(audTable[audPtNum].filename, ptNameStr);
		strcpy(audTable[audPtNum++].audStr, ptLongName);

	}

	if(audPtNum==0)
	{
		printf("\nNo aud point is defined\n\n"); 
		return;
	}

	sprintf(prj_def_file, "%s%s", SYS_DEFINE_PATH, PRJ_DEFINE_FILE);
	if( (fp_def=fopen(prj_def_file, "w"))==NULL )
	{	
		printf("\n!!! Can not open %s !!!\n\n", prj_def_file ); 
		exit(0);
	}

	for(iNum=0;iNum<audPtNum;iNum++)
	{ 
		fprintf(fp_def,"%003d  %40s  ====>  %s  %d\n",iNum,
			audTable[iNum].audStr, audTable[iNum].filename,audTable[iNum].ms_voice); 
	}
	fclose( fp_def );
	
LOOP2: //-------------------- modify define file to make appropriate sentences
	printf(PRJ_DEFINE_FILE);
	printf("\n%s", _CS_("AUD_GEN_STR_MODIFY_PROMPT"));
	gets(strTemp);
	strcpy( strTemp,"%SystemRoot%\\system32\\notepad.exe "); 
	strcat( strTemp,prj_def_file );
	system( strTemp );

	if( (fp_def=fopen(prj_def_file,"r"))==NULL )
	{	
		printf("\n!!! Can not open %s !!!\n\n", prj_def_file ); 
		exit(0);
	}

	audPtNum = 0;
	while( fgets(strTemp, sizeof(strTemp), fp_def) )
	{
		if(sscanf(strTemp,"%d %s %s %s %d",&iNum,audTable[audPtNum].audStr,
			 strTemp1, strTemp1,&audTable[audPtNum].ms_voice)==5)
		{
			printf("%003d  %30s  ====>  %s  %d\n", audPtNum, audTable[audPtNum].audStr,
				audTable[audPtNum].filename,audTable[audPtNum].ms_voice);
			audPtNum++;
		}
	}
	fclose( fp_def );
	if(TryAudAndNeedModify(audPtNum))
		goto LOOP2;
	audFilesGen(audPtNum);
}

BOOL TryAudAndNeedModify(int audPtNum)
{
	char	strTemp[256];
	int		iNum;

	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_TRY_LISTEN")); 
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
			return FALSE;
		if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
		{
			for(;;)
			{ 
				printf("\n%s", _CS_("AUD_GEN_STR_SENTENCE_NO")); 
				FlushConsoleInputBuffer( hConsoleOutput );
				gets(strTemp);
				if(strTemp[0]==0)
					break;
				iNum = atoi(strTemp); 
				if( iNum<0 || iNum>=audPtNum)
				{
#ifdef WINDOWS_OS 
					if(commonAudGen_use_msvoice==1)
						PlayStrAudioMS( _CS_("AUD_GEN_STR_SENTENCE_ERROR" ));
					else
						PlayStrAudioJT( _CS_("AUD_GEN_STR_SENTENCE_ERROR" ));
#else
					PlayStrAudioJT( _CS_("AUD_GEN_STR_SENTENCE_ERROR" ));
#endif 
					
					
					

				}
				else
				{   
					
					printf("================>>>>>> %s ",audTable[iNum].audStr);
					strcpy(strTemp, audTable[iNum].audStr);
					FilterAudStr(strTemp);
							
#ifdef WINDOWS_OS 
					if(audTable[iNum].ms_voice==1)
						PlayStrAudioMS( strTemp );
					else
						PlayStrAudioJT( strTemp );
#else
					PlayStrAudioJT( strTemp );
#endif 
				}
			}

			for(;;)
			{
				printf(PRJ_DEFINE_FILE);
				printf("\n%s", _CS_("AUD_GEN_STR_CONTINUE_MODIFY")); 
				FlushConsoleInputBuffer( hConsoleOutput );
				gets(strTemp);
				if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
					return TRUE;
				if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
					return FALSE;
			}
		}
	}
}

void audFilesGen(int audPtNum)
{
	int		iNum;
	char	strTemp[256];

	for(;;)
	{
		printf("\n%s", _CS_("AUD_GEN_STR_ASK_AUD_GEN")); 
		FlushConsoleInputBuffer( hConsoleOutput );
		gets(strTemp);
		if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
		{
			printf("\n%s", _CS_("AUD_GEN_STR_AUD_GENERATING"));
			for(iNum=0;iNum<audPtNum;iNum++)
			{
				if(!(iNum%80))
					printf("\n");
				printf(".");
				CreateAudioFile( audTable[iNum].audStr, audTable[iNum].filename,audTable[iNum].ms_voice);
			}
			return;
		}
		else if( strcmp(strTemp,"N")==0 || strcmp(strTemp,"n")==0 )
			return;
	}
}


BOOL	IsAudFileNormal(int fileLen, int strLen)
{
	if(strLen<2)
		return ((float)fileLen/strLen) < 10000.;
	else if(strLen<3)
		return ((float)fileLen/strLen) < 8000.;
	else if(strLen<5)
		return ((float)fileLen/strLen) < 5000.;
	else if(strLen<10)
		return ((float)fileLen/strLen) < 3600.;
	else if(strLen<15)
		return ((float)fileLen/strLen) < 2800.;
	else
		return ((float)fileLen/strLen) < 1800.;

}

void CreateAudioFile( char *audStr, char *audFileName,int ms_flag)
{
#define	TRY_TIMES	6
	char	filename[256], strTemp[256];
	int		strLen, fileLen=0, times=TRY_TIMES;
	FILE	*fp;

	if(!audFileName[0])
		return;

//	FilterAudStr(audFileName);
	sprintf(filename, "%s%s.wav", AUD_FILE_PATH, audFileName);
	strcpy(strTemp, audStr);
	FilterAudStr(strTemp);
	strLen= strlen(strTemp);
	
	while(times--)
	{
		
#ifdef WINDOWS_OS 
		if(ms_flag==1)
			CreateStrAudFileMS(strTemp, filename);
		else
			CreateStrAudFileJT(strTemp, filename);
#else
		CreateStrAudFileJT(strTemp, filename);
#endif 
		

		fp= fopen(filename, "r");
		if(fp)
		{
			fseek(fp, 0L, 2);
			fileLen= ftell(fp);
			fclose(fp);
		}
		if(IsAudFileNormal(fileLen, strLen))
			break;

		if(times<=1)
		{
			printf("\n%s %6.0f \n%s", filename, (float)fileLen/strLen, _CS_("AUD_GEN_STR_CHECK_FILE"));
			FlushConsoleInputBuffer( hConsoleOutput );
			gets(strTemp);
			if( strcmp(strTemp,"Y")==0 || strcmp(strTemp,"y")==0 )
				break;
			else
			{
				printf("\n%s \n%s", _CS_("AUD_GEN_STR_CONT_GEN_FILE"), filename);
				times= TRY_TIMES;
			}
		}
	}
}	

void HandlerRoutine(DWORD signal_type)
{
	PlayStrAudioJT( _CS_("AUD_GEN_STR_THANKS") );
	ReleaseAudioLibraryJT();
//	ReleaseAudioLibraryMS();
	printf("\n%s", _CS_("AUD_GEN_STR_END"));
	exit(0);		
	
}

