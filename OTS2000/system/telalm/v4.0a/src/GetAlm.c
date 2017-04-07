/***************************************************************************************
PROGRAM NAME:	ONCALL
FUNCTION:		1. Query EMS real status by telephone;
2. Play the alarm messages on local PC while EMS event happen;
3. Autodial to report alarm messages when EMS event happen; 
4. Send short message about EMS event to special mobile phone;		 
NOTE:			1. This program only be run in the PC on which the multimedia 
board has been installed, and "语音合成" directory is present.
2. All of the relatived files are defined in the path of
"\\home\\ems\\h9000\\def",  Users can modify  
the ..\\inc\\tele.h to define the different functions.
VERSION:		v4.0a
OPERATE SYSTEM:	window 2000, windows NT, window 2003
HISTORY:		2010-11-02~2012-05-01 Added and Programmed by chenxs	
HISTORY:		2010-10-10  Added and Programmed by chenxs	
2009-02-17	Programmed by YUJJI							
2004-02-13	Add short message functions for mobile phone by zm
2004-01-10	Programmed by zm
***************************************************************************************/
#include	"../../../telalm/v4.0a/inc/tele.h"
#include	"../../../oix/v4.0a/inc/alarm_str.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"

#define GROUP_REMARK_SIZE	20
char	groupremark[GROUP_ENTRY_NUMBER][GROUP_REMARK_SIZE]; /* added by zm 040726 */

//REAL_SW RealSW[MAX_SW_NUM];
//REAL_ANA RealANA[MAX_ANA_NUM];
SHM_RECORD	 *shm_rec_addr;
ALM_STR     alm_str;

char    DevStr[10],StnStr[10], NameStr[20];

//HANDLE  SMS_SENDRECEIVE_COM;//串口处理

int     handle[MAXCHAN];
HANDLE  hThread[MAXCHAN],tThread;
long ALLREAD;
char *voxchannels[]={"dxxxB1C1","dxxxB1C2","dxxxB1C3","dxxxB1C4","dxxxB2C1","dxxxB2C2","dxxxB2C3","dxxxB2C4",
"dxxxB3C1","dxxxB3C2","dxxxB3C3","dxxxB3C4","dxxxB4C1","dxxxB4C2","dxxxB4C3","dxxxB4C4",
"dxxxB5C1","dxxxB5C2","dxxxB5C3","dxxxB5C4", "dxxxB6C1","dxxxB6C2","dxxxB6C3","dxxxB6C4",
"dxxxB7C1","dxxxB7C2","dxxxB7C3","dxxxB7C4","dxxxB8C1","dxxxB8C2",NULL,};

/*char *dtichannels[]={"dtiB1T1","dtiB1T2","dtiB1T3","dtiB1T4","dtiB1T5","dtiB1T6","dtiB1T7","dtiB1T8",
"dtiB1T9","dtiB1T10","dtiB1T11","dtiB1T12", "dtiB1T13","dtiB1T14","dtiB1T15","dtiB1T16",
"dtiB1T17","dtiB1T18","dtiB1T19","dtiB1T20","dtiB1T21","dtiB1T22","dtiB1T23","dtiB1T24",
"dtiB1T25","dtiB1T26","dtiB1T27","dtiB1T28", "dtiB1T29","dtiB1T30",NULL,};
*/
//int bsig[15][2]={{1140,1020},{1140,900},{1020,900},{1140,780},{1020,780},{900,780},{1140,660},{1020,660},
//{900,660},{780,660},{1140,540},{1020,540},{900,540},{780,540},{660,540}};

void main(int argc,char	*argv[])
{	
	
	int R;
	printf("\n\nONCALL start ......\n\n");
	if(-1==InitStrTextDefine())
	{
		//printf("\nFail to init str text define:Exit(0)");	
		write_main_logs_now("Fail to init str text define:Exit(0)",1);
		Sleep(8000);
		exit(0);	
	}
	if(-1==InitIPC())
	{
		Err_log("\nFail to initialize IPCs:Exit(-1)");
		write_main_logs_now("Fail to initialize IPCs:Exit(0)",1);
		Sleep(8000);
		exit(-1);
	}
    if(-1==MountDB(NULL))
	{
		//printf("\nFail to MountDB:Exit(0)");
		write_main_logs_now("Fail to MountDB:Exit(-1)",1);
		Sleep(8000);
		exit(-1);
	}
	if(-1==InitAudioLibraryJT(5, 9, 5, NULL))
	{
		//printf("\nFail to initialize JT audio:Exit(0)");
		write_main_logs_now("Fail to initialize JT audio:Exit(-1)",1);
		Sleep(8000);
		exit(-1);
	}
	igc_WSAStartup(); 
	igc_lan_out_init();
	
	gtk_onvall_mainform_created=0;//20120322
	printflevel=1;
	readedxml=0;
    telusexmlbufoverR=1;
	telusexmlbufoverA=1;
	smsusexmlbufover=1;
	
	ONCALLTelQuery_IDThread=0;
	ONCALLTelDial_IDThread=0;
	SMS_SENDRECEIVE_HANDLE_IDThread=0;
	ONCALLWRITEFILE_HANDLE_IDThread=0;
	ONCALL_ALMSEARCH_HANDLE_IDThread=0;
//	Oncall_sms_timingsend_HANDLE_IDThread=0;
	
	ONCALLTelQuery_IDThread_run=0;
	ONCALLTelDial_IDThread_run=0;
	SMS_SENDRECEIVE_HANDLE_IDThread_run=0;
	ONCALLWRITEFILE_HANDLE_IDThread_run=0;
	ONCALL_ALMSEARCH_HANDLE_IDThread_run=0;
//	Oncall_sms_timingsend_HANDLE_IDThread_run=0;
	
//	SMS_SENDRECEIVE_COM_STAT=0;

	memset(&_ONCALLCOM_PORTS,'\0',sizeof(_ONCALLCOM_PORTS));
	memset(&_BAK_ONCALLCOM_PORTS,'\0',sizeof(_BAK_ONCALLCOM_PORTS));//20120327
	memset(&_ONCALLBAUD_RATES,'\0',sizeof(_ONCALLBAUD_RATES));
	memset(&_ONCALLPARITY_TYPES,'\0',sizeof(_ONCALLPARITY_TYPES));
	memset(&_SMS_SENDRECEIVE_COM_STATS,'\0',sizeof(_SMS_SENDRECEIVE_COM_STATS));
	memset(&_strSmsCenterS,'\0',sizeof(_strSmsCenterS));
	memset(&_ONCALL_SMS_DEV_TYPES,'\0',sizeof(_ONCALL_SMS_DEV_TYPES));

	memset(&oncall_sms_logsbufs,'\0',sizeof(oncall_sms_logsbufs));//20120327
	memset(&oncall_tel_logsbufs,'\0',sizeof(oncall_tel_logsbufs));//20120327
	memset(&oncall_almsearch_logsbuf,'\0',sizeof(oncall_almsearch_logsbuf));//20120327
	memset(&oncall_doublehost_logsbuf,'\0',sizeof(oncall_doublehost_logsbuf));//20120327
	memset(&oncall_others_logsbuf,'\0',sizeof(oncall_others_logsbuf));//20120327

	memset(&oncall_timingsend_logsbuf,'\0',sizeof(oncall_timingsend_logsbuf));//20120329
	memset(&oncall_manusend_logsbuf,'\0',sizeof(oncall_manusend_logsbuf));//20120329


	memset(&oncall_gtk_logsbuf,'\0',sizeof(oncall_gtk_logsbuf));//20120327
	memset(&oncall_sms_err_times,'\0',sizeof(oncall_sms_err_times));//20120327
	lock_oncall_sms_err_times;//20120327

  
	first_glag_forgtkui=0;
	defaultTONNPI[0]=57;defaultTONNPI[1]=49;
	memset(&oncall_dial_autoalmsendbuf,'\0',sizeof(oncall_dial_autoalmsendbuf));
	memset(&oncall_dial_manualmsendbuf,'\0',sizeof(oncall_dial_manualmsendbuf));
	memset(&oncall_sms_autoalmsendbuf,'\0',sizeof(oncall_sms_autoalmsendbuf));
	memset(&oncall_sms_manualmsendbuf,'\0',sizeof(oncall_sms_manualmsendbuf));
	memset(&oncall_sms_manusendbuf,'\0',sizeof(oncall_sms_manusendbuf));
	memset(&oncall_sms_timingsendbuf,'\0',sizeof(oncall_sms_timingsendbuf));
	//memset(&oncallL_dialquiry_writebuf,'\0',sizeof(oncallL_dialquiry_writebuf));
//	memset(&oncallL_smsquiry_writebuf,'\0',sizeof(oncallL_smsquiry_writebuf));
	
	memset(&_oncallL_sms_send_tempbufs,'\0',sizeof(_oncallL_sms_send_tempbufs));
	memset(&fault_alm_logicnames,'\0',sizeof(fault_alm_logicnames));
	InterLockFileLoad();//20120405
	
	
	if(readOncallDefineXML()<1)
	{
		//printf("\nFail to readOncallDefineXML:Exit(-1)\n");
		write_main_logs_now("Fail to readOncallDefineXML:Exit(-1)",1);
		Sleep(8000);
		exit(-1);
	}
 
	for(R=0;R<MAX_sms_dev_num;R++)
	{
		if(_ONCALLCOM_PORTS[R][0]==0)	
		{
			_oncallL_sms_send_tempbufs[R].use_over=0;
		}
		else
		{
			_oncallL_sms_send_tempbufs[R].use_over=1;
		}
	}
	
	
	threadstat_Oncall_almsearch=-1;//20120330
	threadstat_oncallTelQuery=-1;//20120330
	threadstat_Oncall_writefie=-1;//20120330
	threadstat_Oncall_ALM_manuconfirm=-1;//20120330
	threadstat_Oncall_sms_timingsend=-1;//20120330
	threadstat_SMS_SENDRECEIVE=-1;//20120330

	if(ONCALLSMS_VALID==1 || ONCALLDIALOGIC_VALID==1)
	{ 
		//报警查询
		threadstat_Oncall_almsearch=1;//2012
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Oncall_almsearch,NULL,0,&ONCALL_ALMSEARCH_HANDLE_IDThread)==NULL) 					
		{ 
			//printf("\nCreate Oncall almsearch Thread() error:Exit(-1)");
			write_main_logs_now("Create Oncall almsearch Thread() error:Exit(-1)",1);
			Sleep(8000);
			exit(-1); 
		}
		//printf("\nONCALL:Thread Oncall almsearch created......"); 	
		write_main_logs_now("Thread Oncall almsearch created......",1);
	}
	
	if(ONCALLDIALOGIC_VALID==1) //TELPHONE 	
	{
		threadstat_oncallTelQuery=1;
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)oncallTelQuery,NULL,0,&ONCALLTelQuery_IDThread)==NULL) 					
		{
			//printf("\nCreate oncallTelQuery Thread() error:Exit(-1)"); 
			write_main_logs_now("Create oncallTelQuery Thread() error:Exit(-1)",1);
			Sleep(8000);
			exit(-1); 
		}
		//printf("\nONCALL:Thread oncallTelQuery created!"); 
		write_main_logs_now("Thread Oncall oncallTelQuery created......",1);
	}
	if(ONCALLSMS_VALID==1 || ONCALLDIALOGIC_VALID==1)
	{ 
		threadstat_Oncall_writefie=1;
		//写文件 ，相关文件等，读取配置
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Oncall_writefie,NULL,0,&ONCALLWRITEFILE_HANDLE_IDThread)==NULL) 					
		{
			//printf("\nCreate Oncall_writefie Thread() error:Exit(-1)");
			write_main_logs_now("Create Oncall Oncall_writefie Thread() error:Exit(-1)",1);
			Sleep(8000);
			exit(-1); 
		}
		//printf("\nONCALL:Thread Oncall Oncall_writefie created......"); 	
		write_main_logs_now("Create Oncall Oncall_writefie Thread() created......",1);
		
		//手动报警确认线程
	 

		threadstat_Oncall_ALM_manuconfirm=1;
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Oncall_ALM_manuconfirm,NULL,0,&Oncall_ALM_manuconfirm_HANDLE_IDThread)==NULL) 					
		{ 
			//printf("\nCreate Oncall_ALM_manuconfirm Thread() error:Exit(-1)\n");
			write_main_logs_now("Create Oncall_ALM_manuconfirm Thread() error:Exit(-1)",1);
			Sleep(8000);
			exit(-1);
		}
		//printf("\nONCALL:Thread Oncall ALM_manuconfirm created......"); 	
		write_main_logs_now("Thread Oncall ALM_manuconfirm created......",1);
	}
	if(ONCALLSMS_VALID==1)
	{  
		threadstat_Oncall_sms_timingsend=1;
		//定时短信线程
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Oncall_sms_timingsend,NULL,0,&Oncall_sms_timingsend_HANDLE_IDThread)==NULL) 					
		{ 
			//printf("\nCreate Oncall_sms_timingsend Thread() error:Exit(-1)"); 
			write_main_logs_now("Create Oncall_sms_timingsend Thread() error:Exit(-1)",1);
			Sleep(8000);
			exit(-1);
		}
		//printf("\nONCALL:Thread Oncall timingsend created......"); 	
		write_main_logs_now("Thread Oncall timingsend created......",1);

		threadstat_SMS_SENDRECEIVE=1;
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SMS_SENDRECEIVE,NULL,0,&SMS_SENDRECEIVE_HANDLE_IDThread)==NULL)					
		{ 
			//printf("\nCreate SMS_SENDRECEIVE Thread() error:Exit(-1)");
			write_main_logs_now("Create SMS_SENDRECEIVE Thread() error:Exit(-1)",1);
			Sleep(8000);
			exit(-1); 
		}
		//printf("\nONCALL: Thread SMS_SENDRECEIVE_HANDLE created......"); 
		write_main_logs_now("Thread SMS_SENDRECEIVE_HANDLE created......",1);

		//SMS_GSM INQUERY
		/*if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReceiveShortMsg,NULL,0,&IDThread)==NULL)						
		{ printf("Create ReceiveShortMsg Thread() error!\n"); exit(-1); }
		printf("\n ONCALL: Thread ReceiveShortMsg created!"); */
		//SMS_GSM TIMINGSEND
		
		/*if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendBytimeShortMsg,NULL,0,&IDThread)==NULL)						
		{ printf("Create SendBytimeShortMsg Thread() error!\n"); exit(-1); }
		printf("\n ONCALL: Thread SendBytimeShortMsg created!\n"); */
		//SMS_CDMA ALM
		/*	if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CDMA_shortmsg,NULL,0,&IDThread)==NULL)						
		{ printf("Create CDMA_shortmsg Thread() error!\n"); exit(-1); }
		printf("\n ONCALL: Thread CDMA_shortmsg created!\n"); 
		//SIEMENS SMS_CDMA INQUERY
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CDMA_ReceiveShortMsg,NULL,0,&IDThread)==NULL)						
		{ printf("Create CDMA_ReceiveShortMsg Thread() error!\n"); exit(-1); }
		printf("\n ONCALL: Thread CDMA_ReceiveShortMsg created!"); 
		//SIEMENS SMS_CDMA TIMINGSEND
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CDMA_SendBytimeShortMsg,NULL,0,&IDThread)==NULL)						
		{ printf("Create CDMA_SendBytimeShortMsg Thread() error!\n"); exit(-1); }
		printf("\nONCALL: Thread CDMA_SendBytimeShortMsg created!\n"); 
		*/
	}
	
	R=	oncallgtkmain(argc,argv);	
	 
	while(1)	
	{   
		Sleep(2000);
		
	}

	InterLockFree();//20120405
}
int write_main_logs_now(char * str,int wtp)
{
	char 	    main_logs_FileName[200];
	struct tm 	*today;
	time_t		ltime;
	FILE    *main_logs_File;
	
	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(main_logs_FileName,"%smain_logs_%04d-%02d-%02d.txt",OUTDATA_RECORD_PATH,today->tm_year+1900,today->tm_mon+1,today->tm_mday); 
	if(main_logs_File=fopen(main_logs_FileName,"a"))
	{
		if(wtp==1)
		{
			fprintf(main_logs_File,"%02d:%02d:%02d ", today->tm_hour,today->tm_min,today->tm_sec);
		}
		if(str)
		{
			fprintf(main_logs_File,"%s\n ", str);
		}
		fclose(main_logs_File);
	}
	else
		fclose(main_logs_File);
 
	return 1;
}
int get_telphone_from_group(GROUP_TO_TELPHONE_DEF * group_to_telphone_def,char * groupcode)
{
 
 return 0;
}
void  update_oncallsendbuf(int oncall_alm_def_index, char *cname, char *timein)
{
	if(ONCALLSMS_VALID==1 && oncall_alm_def.SMS_ALM[oncall_alm_def_index]>=1  && oncall_alm_def.SMS_ALM[oncall_alm_def_index]<=2)
	{
		int Ix,Jx,kx,Lx;
		int add_telnum_idx;
		struct tm 	*today;
		time_t		ltime;	
		char temp_strs[800];//20120326 add
		ltime=time(NULL);
		today=localtime(&ltime);
 		Ix=0,Jx=0,kx=0,Lx=0;
		add_telnum_idx=0;
		timein[19] = '\0';
		if ( oncall_alm_def.almtype[oncall_alm_def_index] == 1 )
		{
			add_telnum_idx=0;
			oncall_sms_manualmsendbuf.confirmed[oncall_sms_manualmsendbuf.add_Ptr]=0;
			strcpy(oncall_sms_manualmsendbuf.logicname[oncall_sms_manualmsendbuf.add_Ptr], oncall_alm_def.logicname[oncall_alm_def_index]);
			strcpy(oncall_sms_manualmsendbuf.longname[oncall_sms_manualmsendbuf.add_Ptr], oncall_alm_def.longname[oncall_alm_def_index]);
			strcpy(oncall_sms_manualmsendbuf.time_str[oncall_sms_manualmsendbuf.add_Ptr], timein);
			strcpy(oncall_sms_manualmsendbuf.message[oncall_sms_manualmsendbuf.add_Ptr], cname);

			//20120326 add srart
			sprintf(temp_strs,"write alm to manuSmsBuf: %s %s %s %s",timein,oncall_alm_def.longname[oncall_alm_def_index],oncall_alm_def.logicname[oncall_alm_def_index],cname);	
			insert_almsearch_logs(temp_strs);
			//printf("\nOnCall:%s",temp_strs);
			//20120326 add end
			
			for(Jx=0;Jx<TELPHONE_NUM;Jx++)
				oncall_sms_manualmsendbuf.telnum[oncall_sms_manualmsendbuf.add_Ptr][Jx][0]='\0';
			for(Ix=0;Ix<TELPHONE_NUM;Ix++)
			{
				if(add_telnum_idx>=TELPHONE_NUM)
					break;
				memset(oncall_sms_manualmsendbuf.telnum[oncall_sms_manualmsendbuf.add_Ptr][Ix],'\0',20);
				if(oncall_alm_def.namecode[oncall_alm_def_index][Ix][0]!='\0')
				{
					for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
					{
						if(strcmp(telphone_def.namecode[Jx],oncall_alm_def.namecode[oncall_alm_def_index][Ix])==0)
						{
							strcpy(oncall_sms_manualmsendbuf.telnum[oncall_sms_manualmsendbuf.add_Ptr][add_telnum_idx],telphone_def.telphone[Jx]);
							
							//20120326 add srart
							sprintf(temp_strs,"telnum:%s",telphone_def.telphone[Jx]);	
							insert_almsearch_logs(temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//20120326 add end
							
							add_telnum_idx++;
							if(add_telnum_idx>=TELPHONE_NUM)//可重复定义号码
								break;
						}
					}
				}
			}

			for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
			{
				if(add_telnum_idx>=TELPHONE_NUM)
					break;
				if(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix][0]==0)
					continue;
				for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
				{
					if(add_telnum_idx>=TELPHONE_NUM)
						break;
					if(telphonegroup_def.grpnamecode[kx][0]==0)
						continue;
					if(strcmp(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix],telphonegroup_def.grpnamecode[kx])==0)
					{
						if(telphonegroup_def.months[kx][today->tm_mon]!=1)
							continue;
						if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
							continue;
						if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
							continue;
						if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
						{
							if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
							{
								for(Jx=0;Jx<TELPHONE_NUM;Jx++)
								{
									if(add_telnum_idx>=TELPHONE_NUM)
										break;
									if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
										continue;
									
									strcpy(oncall_sms_manualmsendbuf.telnum[oncall_sms_manualmsendbuf.add_Ptr][add_telnum_idx],telphonegroup_def.telphonenums[kx][Jx]);
									//20120326 add srart
									sprintf(temp_strs,"telnum_G:%s",telphonegroup_def.telphonenums[kx][Jx]);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									//20120326 add end
									add_telnum_idx++;
								}
							}
						}
					}
				}
			}


		/*	for(Ix=0;Ix<TELPHONE_NUM;Ix++)
			{
				memset(oncall_sms_manualmsendbuf.telnum[oncall_sms_manualmsendbuf.add_Ptr][Ix],'\0',20);
				if(oncall_alm_def.namecode[oncall_alm_def_index][Ix][0]!='\0')
				{
					for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
					{
						
						if(strcmp(telphone_def.namecode[Jx],oncall_alm_def.namecode[oncall_alm_def_index][Ix])==0)
						{
							strcpy(oncall_sms_manualmsendbuf.telnum[oncall_sms_manualmsendbuf.add_Ptr][Ix],telphone_def.telphone[Jx]);
						}
					}
				}
			}*/
		/*	for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
			{
				if(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix][0]==0)
					continue;
				for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
				{
					if(telphonegroup_def.grpnamecode[kx][0]==0)
						continue;
					if(strcmp(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix],telphonegroup_def.grpnamecode[kx])==0)
					{
						if(telphonegroup_def.months[kx][today->tm_mon]!=1)
							continue;
						if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
							continue;
						if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
							continue;
						if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
						{
							if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
							{
								for(Jx=0;Jx<TELPHONE_NUM;Jx++)
								{
									if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
										continue;
									for(Lx=0;Lx<TELPHONE_NUM;Lx++)
									{
										if(oncall_sms_manualmsendbuf.telnum[oncall_sms_manualmsendbuf.add_Ptr][Lx][0]==0)
										{
											strcpy(oncall_sms_manualmsendbuf.telnum[oncall_sms_manualmsendbuf.add_Ptr][Lx],telphonegroup_def.telphonenums[kx][Jx]);
											break;
										}
									}
								}
							}
						}
					}
				}
			}*/
			oncall_sms_manualmsendbuf.add_Ptr++;
			if(oncall_sms_manualmsendbuf.add_Ptr>ONCALL_SMS_MANUALMSENDBUF_SIZE-1)
				oncall_sms_manualmsendbuf.add_Ptr=0;
		}
		else if ( oncall_alm_def.almtype[oncall_alm_def_index] == 2 )
		{
			add_telnum_idx=0;
			strcpy(oncall_sms_autoalmsendbuf.logicname[oncall_sms_autoalmsendbuf.add_Ptr], oncall_alm_def.logicname[oncall_alm_def_index]);
			strcpy(oncall_sms_autoalmsendbuf.longname[oncall_sms_autoalmsendbuf.add_Ptr], oncall_alm_def.longname[oncall_alm_def_index]);
			strcpy(oncall_sms_autoalmsendbuf.time_str[oncall_sms_autoalmsendbuf.add_Ptr], timein);
			strcpy(oncall_sms_autoalmsendbuf.message[oncall_sms_autoalmsendbuf.add_Ptr], cname);
			
			//20120326 add srart
			sprintf(temp_strs,"write alm to autoSmsBuf: %s %s %s %s",timein,oncall_alm_def.longname[oncall_alm_def_index],oncall_alm_def.logicname[oncall_alm_def_index],cname);	
			insert_almsearch_logs(temp_strs);
			//printf("\nOnCall:%s",temp_strs);
			//20120326 add end

			for(Jx=0;Jx<TELPHONE_NUM;Jx++)
				oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][Jx][0]='\0';
			for(Ix=0;Ix<TELPHONE_NUM;Ix++)
			{
				if(add_telnum_idx>=TELPHONE_NUM)
					break;
				memset(oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][Ix],'\0',20);
				if(oncall_alm_def.namecode[oncall_alm_def_index][Ix][0]!='\0')
				{
					for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
					{
						if(strcmp(telphone_def.namecode[Jx],oncall_alm_def.namecode[oncall_alm_def_index][Ix])==0)
						{
							strcpy(oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][add_telnum_idx],telphone_def.telphone[Jx]);
							//20120326 add srart
							sprintf(temp_strs,"telnum:%s",telphone_def.telphone[Jx]);	
							insert_almsearch_logs(temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//20120326 add end
							add_telnum_idx++;
							if(add_telnum_idx>=TELPHONE_NUM)//可重复定义号码
								break;
						}
					}
				}
			}
			
			for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
			{
				if(add_telnum_idx>=TELPHONE_NUM)
					break;
				if(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix][0]==0)
					continue;
				for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
				{
					if(add_telnum_idx>=TELPHONE_NUM)
						break;
					if(telphonegroup_def.grpnamecode[kx][0]==0)
						continue;
					if(strcmp(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix],telphonegroup_def.grpnamecode[kx])==0)
					{
						if(telphonegroup_def.months[kx][today->tm_mon]!=1)
							continue;
						if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
							continue;
						if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
							continue;
						if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
						{
							if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
							{
								for(Jx=0;Jx<TELPHONE_NUM;Jx++)
								{
									if(add_telnum_idx>=TELPHONE_NUM)
										break;
									if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
										continue;
									
									strcpy(oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][add_telnum_idx],telphonegroup_def.telphonenums[kx][Jx]);
									//20120326 add srart
									sprintf(temp_strs,"telnum_G:%s",telphonegroup_def.telphonenums[kx][Jx]);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									//20120326 add end
									add_telnum_idx++;
								}
							}
						}
					}
				}
			}

			/*for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
				oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][Jx][0]='\0';
			for(Ix=0;Ix<TELPHONE_NUM;Ix++)
			{
				memset(oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][Ix],'\0',20);
				if(oncall_alm_def.namecode[oncall_alm_def_index][Ix][0]!='\0')
				{
					for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
					{
						if(strcmp(telphone_def.namecode[Jx],oncall_alm_def.namecode[oncall_alm_def_index][Ix])==0)
						{
							strcpy(oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][Ix],telphone_def.telphone[Jx]);
						}	
					}
				}
			}
			for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
			{
				if(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix][0]==0)
					continue;
				for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
				{
					if(telphonegroup_def.grpnamecode[kx][0]==0)
						continue;
					if(strcmp(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix],telphonegroup_def.grpnamecode[kx])==0)
					{
						if(telphonegroup_def.months[kx][today->tm_mon]!=1)
							continue;
						if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
							continue;
						if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
							continue;
						if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
						{
							if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
							{
								for(Jx=0;Jx<TELPHONE_NUM;Jx++)
								{
									if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
										continue;
									for(Lx=0;Lx<TELPHONE_NUM;Lx++)
									{
										if(oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][Lx][0]==0)
										{
											strcpy(oncall_sms_autoalmsendbuf.telnum[oncall_sms_autoalmsendbuf.add_Ptr][Lx],telphonegroup_def.telphonenums[kx][Jx]);
											break;
										}
									}
								}
							}
						}
					}
				}
			}*/
			oncall_sms_autoalmsendbuf.add_Ptr++;
			if(oncall_sms_autoalmsendbuf.add_Ptr>ONCALL_SMS_AUTOALMSENDBUF_SIZE-1)
				oncall_sms_autoalmsendbuf.add_Ptr=0;
			
		}
	}
	
	
	if(ONCALLDIALOGIC_VALID==1 && oncall_alm_def.TEL_ALM[oncall_alm_def_index]>=1 && oncall_alm_def.TEL_ALM[oncall_alm_def_index]<=2)
	{
		int Ix,Jx,kx,Lx;
		int add_telnum_idx;
		struct tm 	*today;
		time_t		ltime;	
		char temp_strs[800];//20120326 add
		ltime=time(NULL);
		today=localtime(&ltime);
		Ix=0,Jx=0,kx=0,Lx=0;
		add_telnum_idx=0;
		timein[19] = '\0' ;
		if ( oncall_alm_def.almtype[oncall_alm_def_index] == 1 )
		{		 
			add_telnum_idx=0;
			oncall_dial_manualmsendbuf.confirmed[oncall_dial_manualmsendbuf.add_Ptr]=0;
			strcpy(oncall_dial_manualmsendbuf.logicname[oncall_dial_manualmsendbuf.add_Ptr], oncall_alm_def.logicname[oncall_alm_def_index]);
			strcpy(oncall_dial_manualmsendbuf.longname[oncall_dial_manualmsendbuf.add_Ptr], oncall_alm_def.longname[oncall_alm_def_index]);
			strcpy(oncall_dial_manualmsendbuf.time_str[oncall_dial_manualmsendbuf.add_Ptr], timein);
			strcpy(oncall_dial_manualmsendbuf.message[oncall_dial_manualmsendbuf.add_Ptr], cname);

			//20120326 add srart
			sprintf(temp_strs,"write alm to manuDialBuf: %s %s %s %s",timein,oncall_alm_def.longname[oncall_alm_def_index],oncall_alm_def.logicname[oncall_alm_def_index],cname);	
			insert_almsearch_logs(temp_strs);
			//printf("\nOnCall:%s",temp_strs);
			//20120326 add end
			
			for(Jx=0;Jx<TELPHONE_NUM;Jx++)
				oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][Jx][0]='\0';
			for(Ix=0;Ix<TELPHONE_NUM;Ix++)
			{
				if(add_telnum_idx>=TELPHONE_NUM)
					break;
				memset(oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][Ix],'\0',20);
				if(oncall_alm_def.namecode[oncall_alm_def_index][Ix][0]!='\0')
				{
					for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
					{
						if(strcmp(telphone_def.namecode[Jx],oncall_alm_def.namecode[oncall_alm_def_index][Ix])==0)
						{
							strcpy(oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][add_telnum_idx],telphone_def.telphone[Jx]);
							//20120326 add srart
							sprintf(temp_strs,"telnum:%s",telphone_def.telphone[Jx]);	
							insert_almsearch_logs(temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//20120326 add end

							add_telnum_idx++;
							if(add_telnum_idx>=TELPHONE_NUM)//可重复定义号码
								break;
						}
					}
				}
			}
			
			for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
			{
				if(add_telnum_idx>=TELPHONE_NUM)
					break;
				if(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix][0]==0)
					continue;
				for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
				{
					if(add_telnum_idx>=TELPHONE_NUM)
						break;
					if(telphonegroup_def.grpnamecode[kx][0]==0)
						continue;
					if(strcmp(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix],telphonegroup_def.grpnamecode[kx])==0)
					{
						if(telphonegroup_def.months[kx][today->tm_mon]!=1)
							continue;
						if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
							continue;
						if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
							continue;
						if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
						{
							if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
							{
								for(Jx=0;Jx<TELPHONE_NUM;Jx++)
								{
									if(add_telnum_idx>=TELPHONE_NUM)
										break;
									if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
										continue;
									
									strcpy(oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][add_telnum_idx],telphonegroup_def.telphonenums[kx][Jx]);
									//20120326 add srart
									sprintf(temp_strs,"telnum_G:%s",telphonegroup_def.telphonenums[kx][Jx]);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									//20120326 add end
									add_telnum_idx++;
								}
							}
						}
					}
				}
			}

		/*	for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
				oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][Jx][0]='\0';
			for(Ix=0;Ix<TELPHONE_NUM;Ix++)
			{
				memset(oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][Ix],'\0',20);
				memset(oncall_dial_manualmsendbuf.dial_out_add_num[Ix],0,sizeof(oncall_dial_manualmsendbuf.dial_out_add_num[Ix]));

				if(oncall_alm_def.namecode[oncall_alm_def_index][Ix][0]!='\0')
				{
					for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
					{
						if(strcmp(telphone_def.namecode[Jx],oncall_alm_def.namecode[oncall_alm_def_index][Ix])==0)
						{
							strcpy(oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][Ix],telphone_def.telphone[Jx]);
							if(telphone_def.dial_out_add_num[Jx][0]!=0)
								strcpy(oncall_dial_manualmsendbuf.dial_out_add_num[oncall_dial_manualmsendbuf.add_Ptr][Ix],telphone_def.dial_out_add_num[Jx]);
					
						}	
					}
				}
			}
			for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
			{
				if(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix][0]==0)
					continue;
				for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
				{
					if(telphonegroup_def.grpnamecode[kx][0]==0)
						continue;
					if(strcmp(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix],telphonegroup_def.grpnamecode[kx])==0)
					{
						if(telphonegroup_def.months[kx][today->tm_mon]!=1)
							continue;
						if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
							continue;
						if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
							continue;
						if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
						{
							if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
							{
								for(Jx=0;Jx<TELPHONE_NUM;Jx++)
								{
									if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
										continue;
									for(Lx=0;Lx<TELPHONE_NUM;Lx++)
									{
										if(oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][Lx][0]==0)
										{
											strcpy(oncall_dial_manualmsendbuf.telnum[oncall_dial_manualmsendbuf.add_Ptr][Lx],telphonegroup_def.telphonenums[kx][Jx]);
											break;
										}
									}
								}
							}
						}
					}
				}
			}
			*/
			oncall_dial_manualmsendbuf.add_Ptr++;
			if(oncall_dial_manualmsendbuf.add_Ptr>ONCALL_DIAL_MANUALMBUF_SIZE-1)
				oncall_dial_manualmsendbuf.add_Ptr=0;
		}
		else if (oncall_alm_def.almtype[oncall_alm_def_index] == 2)
		{
			add_telnum_idx=0;
			strcpy(oncall_dial_autoalmsendbuf.logicname[oncall_dial_autoalmsendbuf.add_Ptr], oncall_alm_def.logicname[oncall_alm_def_index]);
			strcpy(oncall_dial_autoalmsendbuf.longname[oncall_dial_autoalmsendbuf.add_Ptr], oncall_alm_def.longname[oncall_alm_def_index]);
			strcpy(oncall_dial_autoalmsendbuf.time_str[oncall_dial_autoalmsendbuf.add_Ptr], timein);
			strcpy(oncall_dial_autoalmsendbuf.message[oncall_dial_autoalmsendbuf.add_Ptr], cname);
			//20120326 add srart
			sprintf(temp_strs,"write alm to autoDialBuf: %s %s %s %s",timein,oncall_alm_def.longname[oncall_alm_def_index],oncall_alm_def.logicname[oncall_alm_def_index],cname);	
			insert_almsearch_logs(temp_strs);
			//printf("\nOnCall:%s",temp_strs);
			//20120326 add end
			for(Jx=0;Jx<TELPHONE_NUM;Jx++)
				oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][Jx][0]='\0';
			for(Ix=0;Ix<TELPHONE_NUM;Ix++)
			{
				if(add_telnum_idx>=TELPHONE_NUM)
					break;
				memset(oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][Ix],'\0',20);
				if(oncall_alm_def.namecode[oncall_alm_def_index][Ix][0]!='\0')
				{
					for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
					{
						if(strcmp(telphone_def.namecode[Jx],oncall_alm_def.namecode[oncall_alm_def_index][Ix])==0)
						{
							strcpy(oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][add_telnum_idx],telphone_def.telphone[Jx]);
							//20120326 add srart
							sprintf(temp_strs,"telnum:%s",telphone_def.telphone[Jx]);	
							insert_almsearch_logs(temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//20120326 add end

							add_telnum_idx++;
							if(add_telnum_idx>=TELPHONE_NUM)//可重复定义号码
								break;
						}
					}
				}
			}
			
			for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
			{
				if(add_telnum_idx>=TELPHONE_NUM)
					break;
				if(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix][0]==0)
					continue;
				for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
				{
					if(add_telnum_idx>=TELPHONE_NUM)
						break;
					if(telphonegroup_def.grpnamecode[kx][0]==0)
						continue;
					if(strcmp(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix],telphonegroup_def.grpnamecode[kx])==0)
					{
						if(telphonegroup_def.months[kx][today->tm_mon]!=1)
							continue;
						if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
							continue;
						if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
							continue;
						if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
						{
							if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
							{
								for(Jx=0;Jx<TELPHONE_NUM;Jx++)
								{
									if(add_telnum_idx>=TELPHONE_NUM)
										break;
									if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
										continue;
									
									strcpy(oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][add_telnum_idx],telphonegroup_def.telphonenums[kx][Jx]);
									//20120326 add srart
									sprintf(temp_strs,"telnum_G:%s",telphonegroup_def.telphonenums[kx][Jx]);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									//20120326 add end
									add_telnum_idx++;
								}
							}
						}
					}
				}
			}

			/*
			for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
				oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][Jx][0]='\0';

			for(Ix=0;Ix<TELPHONE_NUM;Ix++)
			{
				memset(oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][Ix],'\0',20);
				
				memset(oncall_dial_autoalmsendbuf.dial_out_add_num[Ix],0,sizeof(oncall_dial_autoalmsendbuf.dial_out_add_num[Ix]));

				if(oncall_alm_def.namecode[oncall_alm_def_index][Ix][0]!='\0')
				{
					for(Jx=0;Jx<telphone_def.real_telphone_def_num;Jx++)
					{
						if(strcmp(telphone_def.namecode[Jx],oncall_alm_def.namecode[oncall_alm_def_index][Ix])==0)
						{
							strcpy(oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][Ix],telphone_def.telphone[Jx]);
							if(telphone_def.dial_out_add_num[Jx][0]!=0)
								strcpy(oncall_dial_autoalmsendbuf.dial_out_add_num[oncall_dial_autoalmsendbuf.add_Ptr][Ix],telphone_def.dial_out_add_num[Jx]);
						
						}	
					}
				}
			}
			for(Ix=0;Ix<TELPHONEGROUP_NUM;Ix++)
			{
				if(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix][0]==0)
					continue;
				for(kx=0;kx<TELPHONEGROUP_NUM;kx++)
				{
					if(telphonegroup_def.grpnamecode[kx][0]==0)
						continue;
					if(strcmp(oncall_alm_def.groupnamecode[oncall_alm_def_index][Ix],telphonegroup_def.grpnamecode[kx])==0)
					{
						if(telphonegroup_def.months[kx][today->tm_mon]!=1)
							continue;
						if(telphonegroup_def.days[kx][today->tm_mday-1]!=1)
							continue;
						if(telphonegroup_def.weeklimit[kx][today->tm_wday]!=1)
							continue;
						if(telphonegroup_def.shours[kx]*60+telphonegroup_def.smins[kx]<=today->tm_hour*60+today->tm_min)
						{
							if(telphonegroup_def.ehours[kx]*60+telphonegroup_def.emins[kx]>=today->tm_hour*60+today->tm_min)
							{
								for(Jx=0;Jx<TELPHONE_NUM;Jx++)
								{
									if(telphonegroup_def.telphonenums[kx][Jx][0]==0)
										continue;
									for(Lx=0;Lx<TELPHONE_NUM;Lx++)
									{
										if(oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][Lx][0]==0)
										{
											strcpy(oncall_dial_autoalmsendbuf.telnum[oncall_dial_autoalmsendbuf.add_Ptr][Lx],telphonegroup_def.telphonenums[kx][Jx]);
											break;
										}
									}
								}
							}
						}
					}
				}
			}
			*/
			oncall_dial_autoalmsendbuf.add_Ptr++;
			if(oncall_dial_autoalmsendbuf.add_Ptr>ONCALL_DIAL_AUTOALMBUF_SIZE-1)
				oncall_dial_autoalmsendbuf.add_Ptr=0;
			
		}
	}
}

void TakeApart( char *string )
{ int  n;
char *pdest;

pdest = strchr( string, '.' );
n = pdest - string;
strncpy( StnStr,string,n ); 

string = string + n + 1;
pdest = strchr( string, '.' );
n = pdest - string;
strncpy( DevStr,string,n ); 

string = string + n + 1;
pdest = strchr( string, '.' );
n = pdest - string;
string = string + n + 1;
strcpy( NameStr,string);
}

int	GetPtId(char *pt_name, POINTER *point)
{
	int		len;
	char	name[POINT_NAME_SIZE];
	char	str[5][64];
	int     stn_id, dev_id, data_type, pt_id;
	
	strcpy(name, pt_name);
	len= strlen(name);
	while(len--)
	{
		if(name[len-1]=='.')
			name[len-1]= ' ';
	}
	
	if(4!=sscanf(name, "%s %s %s %s", &str[0], &str[1], &str[2], &str[3]))
	{
		printf("\nInvalid point name");
		return -1;
	}
	
	if(str[2][0]>='0' && str[2][0]<='9')
	{
		sscanf(name, "%d %d %d %d", &stn_id, &dev_id, 
			&data_type, &pt_id);
		point->stn_id= (UCHAR)stn_id;
		point->dev_id= (UCHAR)dev_id;
		point->data_type= (UCHAR)data_type;
		point->pt_id= (USHORT)pt_id;
		return 0;
	}
	else
		return GetPtIdByNameStr(pt_name, point);
}


int InitIPC()
{	
	/*      打开记录区共享内存      */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		Err_log("Fail to link record shm");
		return -1;
	}
	
	return 0;
}

void UpdateONCALLALMBUF(REC_ENTRY_UNION *rec,char	namestr[POINT_NAME_SIZE])
{
	POINTER		point;
	GULONG		normalState;
	USHORT		state;
	int		    i,j;

 
	int ind_rt_state; 

	if(-1==GetPointIdFromRec(rec, &point))
		return;
	if(rec->ana_rec.type_id==DPS_ANA_ALM)
	{
		if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_LMT)
		{
			state= rec->ana_rec.status.ana_status.lmt_state;
			if(state==ANA_ALM_HI_ALARM || state==ANA_ALM_HI_WARN|| state==ANA_ALM_LO_WARN || state==ANA_ALM_LO_ALARM)
			{
				GetAlmStrings (rec, &alm_str, NULL, NULL);
				for(i=0; i<oncall_alm_def.real_oncall_alm_buf_num; i++)
				{					
					if ((strcmp(oncall_alm_def.logicname[i], namestr)==0) && ( oncall_alm_def.almtype[i] >0 ))
					{
						for(j=0; j<oncall_alm_dev_def.real_oncall_alm_dev_def_num; j++)
						{
							if ((strstr(oncall_alm_def.logicname[i],oncall_alm_dev_def.logicname[j])) 
								&&  ((oncall_alm_dev_def.SMS_ALM[j] == 1 && ONCALLSMS_VALID==1) || (oncall_alm_dev_def.TEL_ALM[j] == 1 && ONCALLDIALOGIC_VALID==1)))
							{

								//20120210 start
						 		
								ind_rt_state=0;
								if( !GettelalmCbaState(&point,rec->ana_rec.type_id,ind_rt_state,0) )
								{
									char temp_strs[400];//20120326 add
									sprintf(temp_strs,"ANA_ALM interlock false ,Point:%d.%d.%d.%d",point.stn_id,point.dev_id,point.data_type,point.pt_id);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									break;
								}
								//20120210 end

								strcat( alm_str.state_str,".");	 
								update_oncallsendbuf(i, alm_str.state_str, alm_str.time_str);
								break;
							}
						}
						break;
					}					
				}
			}//if(state==ANA_ALM_HI_ALARM || state==ANA_ALM_HI_WARN..
			else
				return;
		} //模拟量越限报警
		else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_TR)
		{
			if(rec->ana_rec.status.ana_status.trend_state==ANA_ALM_TR_WARN)
			{
				GetAlmStrings (rec, &alm_str, NULL, NULL);
				
				for(i=0; i<oncall_alm_def.real_oncall_alm_buf_num; i++)
				{
					if ((0 == strcmp(oncall_alm_def.logicname[i], namestr)) && ( oncall_alm_def.almtype[i] >0 ))   // add by yujji 090615
					{
						for(j=0; j<oncall_alm_dev_def.real_oncall_alm_dev_def_num; j++)
						{
							if ((strstr(oncall_alm_def.logicname[i],oncall_alm_dev_def.logicname[j])) 
								&&  ((oncall_alm_dev_def.SMS_ALM[j] == 1 && ONCALLSMS_VALID==1) || (oncall_alm_dev_def.TEL_ALM[j] == 1 && ONCALLDIALOGIC_VALID==1)))
							{
								
								//20120210 start
							 
								ind_rt_state=0;
								if( !GettelalmCbaState(&point,rec->ana_rec.type_id,ind_rt_state,0) )
								{
									char temp_strs[400];//20120326 add
									sprintf(temp_strs,"ANA_ALM interlock false ,Point:%d.%d.%d.%d",point.stn_id,point.dev_id,point.data_type,point.pt_id);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									break;
								 
								}
								//20120210 end

								strcat( alm_str.state_str,".");	 
								update_oncallsendbuf(i, alm_str.state_str, alm_str.time_str);
								break;
							}
						}
						break;
					}					
				}
			} //if(rec->ana_rec.status.ana_status.trend_state==ANA_ALM_TR_WARN)
			else
				return;
		}//模拟量趋势报警
		else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_CHANFAIL)
		{
			if(rec->ana_rec.status.ana_status.chan_fail)
			{
				GetAlmStrings (rec, &alm_str, NULL, NULL);
				for(i=0; i<oncall_alm_def.real_oncall_alm_buf_num; i++)
				{
					if ((0 == strcmp(oncall_alm_def.logicname[i], namestr)) && ( oncall_alm_def.almtype[i] >0 ))   // add by yujji 090615
					{
						for(j=0; j<oncall_alm_dev_def.real_oncall_alm_dev_def_num; j++)
						{
							if ((strstr(oncall_alm_def.logicname[i],oncall_alm_dev_def.logicname[j])) 
								&&  ((oncall_alm_dev_def.SMS_ALM[j] == 1 && ONCALLSMS_VALID==1) || (oncall_alm_dev_def.TEL_ALM[j] == 1 && ONCALLDIALOGIC_VALID==1)))
							{
								
								//20120210 start
							 	
								ind_rt_state=0;
								if( !GettelalmCbaState(&point,rec->ana_rec.type_id,ind_rt_state,1) )
								{
									char temp_strs[400];//20120326 add
									sprintf(temp_strs,"ANA_ALM interlock false ,Point:%d.%d.%d.%d",point.stn_id,point.dev_id,point.data_type,point.pt_id);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									break;
								}
								//20120210 end

								strcat( alm_str.state_str,".");	 
								update_oncallsendbuf(i, alm_str.state_str, alm_str.time_str);
								break;
							}
						}
						break;
					}					
				}
			} //if(rec->ana_rec.status.ana_status.chan_fail)
			else
                return;
		}//模拟量通道故障
		else if(rec->ana_rec.status.ana_status.alm_type==ANA_ALM_TYPE_OVERFLOW)
		{
			if(rec->ana_rec.status.ana_status.overflow)
			{
				GetAlmStrings (rec, &alm_str, NULL, NULL);
				for(i=0; i<oncall_alm_def.real_oncall_alm_buf_num; i++)
				{
					if ((0 == strcmp(oncall_alm_def.logicname[i], namestr)) && ( oncall_alm_def.almtype[i] >0 ))   // add by yujji 090615
					{
						for(j=0; j<oncall_alm_dev_def.real_oncall_alm_dev_def_num; j++)
						{
							if ((strstr(oncall_alm_def.logicname[i],oncall_alm_dev_def.logicname[j])) 
								&&  ((oncall_alm_dev_def.SMS_ALM[j] == 1 && ONCALLSMS_VALID==1) || (oncall_alm_dev_def.TEL_ALM[j] == 1 && ONCALLDIALOGIC_VALID==1)))
							{
								
								//20120210 start
								ind_rt_state=0;
								if( !GettelalmCbaState(&point,rec->ana_rec.type_id,ind_rt_state,0) )
								{
									char temp_strs[400];//20120326 add
									sprintf(temp_strs,"ANA_ALM interlock false ,Point:%d.%d.%d.%d",point.stn_id,point.dev_id,point.data_type,point.pt_id);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									break;
								}
								//20120210 end

								strcat( alm_str.state_str,".");	 
								update_oncallsendbuf(i, alm_str.state_str, alm_str.time_str);
								break;
							}
						}
						break;
					}					
				}
			} //if(rec->ana_rec.status.ana_status.overflow)
			else
                return;
		}//模拟量数据溢出
	}
	else if(rec->ind_rec.type_id==DPS_IND_ALM)
	{
		if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_CHANGE)
		{
			GetPtNormalStateById(&point, &normalState);
			//	if(rec->ind_rec.ind_status.rt_state==normalState)
			//	return;
			//else
			//	{
			GetAlmStrings (rec, &alm_str, NULL, NULL);
			for(i=0; i<oncall_alm_def.real_oncall_alm_buf_num; i++)
			{
				if ((0 == strcmp(oncall_alm_def.logicname[i], namestr)) && ( oncall_alm_def.almtype[i] >0 ))   // add by yujji 090615
				{
					for(j=0; j<oncall_alm_dev_def.real_oncall_alm_dev_def_num; j++)
					{
						if ((strstr(oncall_alm_def.logicname[i],oncall_alm_dev_def.logicname[j])) 
							&&  ((oncall_alm_dev_def.SMS_ALM[j] == 1 && ONCALLSMS_VALID==1) || (oncall_alm_dev_def.TEL_ALM[j] == 1 && ONCALLDIALOGIC_VALID==1)))
						{
							
							 
							if(rec->ind_rec.ind_status.rt_state==normalState && oncall_alm_def.SMS_ALM[i]!=2 &&  oncall_alm_def.SMS_ALM[i]!=2)
							{
								return;
							}
							else
							{
								//20120210 start
								ind_rt_state=rec->ind_rec.ind_status.rt_state;
								if( !GettelalmCbaState(&point,rec->ana_rec.type_id,ind_rt_state,0) )
								{
									char temp_strs[400];//20120326 add
									sprintf(temp_strs,"IND_ALM interlock false ,Point:%d.%d.%d.%d",point.stn_id,point.dev_id,point.data_type,point.pt_id);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									break;
								}
								//20120210 end

								strcat( alm_str.state_str,".");	 
								update_oncallsendbuf(i, alm_str.state_str, alm_str.time_str);
								break;
							}
						}
					}
					break;
				}					
			}			
			//	}
		}//开关量变位报警
		else if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_MANFORCE)
			return;
		else if(rec->ind_rec.ind_status.alm_type==IND_ALM_TYPE_CHANFAIL)
		{
			if(rec->ind_rec.ind_status.chan_fail)
			{
				GetAlmStrings (rec, &alm_str, NULL, NULL);
				for(i=0; i<oncall_alm_def.real_oncall_alm_buf_num; i++)
				{
					if ((0 == strcmp(oncall_alm_def.logicname[i], namestr)) && ( oncall_alm_def.almtype[i] >0 ))   // add by yujji 090615
					{
						for(j=0; j<oncall_alm_dev_def.real_oncall_alm_dev_def_num; j++)
						{
							if ((strstr(oncall_alm_def.logicname[i],oncall_alm_dev_def.logicname[j])) 
								&&  ((oncall_alm_dev_def.SMS_ALM[j] == 1 && ONCALLSMS_VALID==1) || (oncall_alm_dev_def.TEL_ALM[j] == 1 && ONCALLDIALOGIC_VALID==1)))
							{
								
								//20120210 start
							 	ind_rt_state=rec->ind_rec.ind_status.rt_state;
								if( !GettelalmCbaState(&point,rec->ana_rec.type_id,ind_rt_state,1) )
								{
									char temp_strs[400];//20120326 add
									sprintf(temp_strs,"IND_ALM interlock false ,Point:%d.%d.%d.%d",point.stn_id,point.dev_id,point.data_type,point.pt_id);	
									insert_almsearch_logs(temp_strs);
									//printf("\nOnCall:%s",temp_strs);
									break;
								}
								//20120210 end
								strcat( alm_str.state_str,".");	 
								update_oncallsendbuf(i, alm_str.state_str, alm_str.time_str);
								break;
							}
						}
						break;
					}					
				}			
			}
			else
                return;
		}//开关量通道故障
	}
}


int GetPointIdFromRec(REC_ENTRY_UNION *rec, POINTER *pt)
{   
    switch(rec->ana_rec.type_id)
	{
	case	DPS_ANA_ALM:
	case	DPS_IMP_ALM:
		pt->stn_id =  rec->ana_rec.stn_id ;
		pt->dev_id =  rec->ana_rec.dev_id ;
		pt->data_type =  rec->ana_rec.data_type ;
		pt->pt_id =  rec->ana_rec.pt_id ;
		return 0;
		
	case	DPS_IND_ALM:
		pt->stn_id =  rec->ind_rec.stn_id ;
		pt->dev_id =  rec->ind_rec.dev_id ;
		pt->data_type =  rec->ind_rec.data_type ;
		pt->pt_id =  rec->ind_rec.pt_id ;
		return 0;
		
	case	DPS_ANA_PARAM_SET:
	case	DPS_IMP_PARAM_SET:
	case	DPS_C_ONOFF_SET:
		pt->stn_id =  rec->param_set_rec.stn_id ;
		pt->dev_id =  rec->param_set_rec.dev_id ;
		pt->data_type =  rec->param_set_rec.data_type ;
		pt->pt_id =  rec->param_set_rec.pt_id ;
		return 0;
		
	case	DPS_LMS:
	case	DPS_SMS:
	case	DPS_SM_ALM:
		return -1;
		
	default:
		pt->stn_id =  rec->cmd_rec.stn_id ;
		pt->dev_id =  rec->cmd_rec.dev_id ;
		pt->data_type =  rec->cmd_rec.data_type;
		pt->pt_id =  rec->cmd_rec.pt_id ;
		return 0;
	}
}

int readOncallDefineXML()// chenxs 20101112
{
	FILE * fp;
	char    *dvl,*dvlC,*startptr; 
	char	err_str[256];	
	char	keyword[64];
	int		k, nci,row,kx,rsp;
	char	str[BUFSIZE],strValue[BUFSIZE],strValuetm[BUFSIZE];
	int		index, sstep,canrowinc,ended,hwfg,hdfg,hmfg;
	char * ckstartchar, * ckendchar, * ckendchar2,* ckendchar3,* ckendchar4,*ckendcharc,*ckendcharidx;
	
	ckstartchar=">";
	
	ckendchar="</Data></Cell>";
	ckendchar2="</ss:Data></Cell>";
	ckendchar3="><Data ";
	ckendchar4="><ss:Data ";
	ckendcharc="<Cell";
	dvlC=";";

	readedxml=0;
	index=0;
	ckendcharidx= "ss:Index" ;
	hwfg=0;
	hmfg=0;
	hdfg=0;
	
RDXMLP0:	if(telusexmlbufoverA!=1 || telusexmlbufoverR!=1  || smsusexmlbufover!=1)//如果正在使用配置文件，则等待重新读取，次数超时退出
			{
				Sleep(100);
				index++;
				if(index<10000)
					goto RDXMLP0;
				else
					return -100;
			}
			/*	fp=fopen(SMS_XML_FILE,"r");
			if(!fp)
			{
			printf("\n ONCALL readOncallDefineXML Err,file: %s not exist\n",SMS_XML_FILE);
			return -1;
}*/
			if((fp=fopen(SMS_XML_FILE, "r"))==NULL )
			{
				sprintf(err_str, "Fail to open %s", SMS_XML_FILE);
				Err_log(err_str);
				fclose(fp);
				return -1;
			}
			//电话定义表配置读取 start
			sstep=1;
			strcpy(keyword, "电话定义表");
			k=sizeof(str);
			while(sstep>0)
			{
				if(fgets(str, k, fp)==NULL)break;
				UTF8ToAscii(str);
				if(sstep==1){if(strstr(str, "<Worksheet")!=NULL)sstep=2;}
				if(sstep==2){if(strstr(str, keyword)!=NULL){sstep=3;break;}}
			}
			
			while(fgets(str, sizeof(str), fp)!=NULL )
			{
				UTF8ToAscii(str);
				if(strstr(str, "</Row>")){break;}
				if(strstr(str, "<Row"))	{continue;}		
			}
			
			
			row=0;canrowinc=0;
			memset(&telphone_def,0,sizeof(telphone_def));
			
			
			rsp=0;//
			while(fgets(str, sizeof(str), fp))
			{
				UTF8ToAscii(str);
				if( strstr(str, "</Table>"))
				{memset(str,0,sizeof(str));break;}
				if(strstr(str, "<Row"))
				{
					if(canrowinc==1)
					{ 
						row++;
						if(row>TELPHONE_NUM-1)break;
						canrowinc=0;
					}
					k=0;
					rsp=0;//
				}
				
				if(rsp==0)
				{
					if(strstr(str, ckendcharc))
						rsp=1;
				}
				if(rsp==1)
				{
					if(strstr(str, ckendcharc))
					{	
						if(strstr(str, "/>"))
						{
							startptr=strstr(str, ckendcharc);
							startptr+=strlen(ckendcharc);
							if(!strstr(startptr, "<"))
								k++;
						}
					}
					if(GetIntValueByIndexStr(str, ckendcharidx, &index))
						k= index-1;
					
					startptr=strstr(str, ckendchar3);
					if(startptr)
					{
						rsp=2;
						startptr+= strlen(ckendchar3);
					}
					else
					{
						startptr=strstr(str, ckendchar4);
						if(startptr)
						{
							rsp=2;
							startptr+= strlen(ckendchar4);
						}
					}
				}
				if(rsp==2)
				{
					memset(strValue,0,sizeof(strValue));
					XmlCellcheckadgetvalue(startptr,strValue,ckstartchar,ckendchar,ckendchar2,1,&ended);
					if(ended==1)
						rsp=88;
					else
						rsp=24;
				}
				if(rsp==24)
				{
					memset(strValuetm,0,sizeof(strValuetm));
					XmlCellcheckadgetvalue(startptr,strValuetm,ckstartchar,ckendchar,ckendchar2,1,&ended);
					if(strValuetm)
						strcat(strValue,strValuetm);
					if(ended==1)
						rsp=88;
				}
				if(rsp!=88)
					continue;
				rsp=0;
				
				switch(k)
				{
				case 0:
					strcpy(telphone_def.department[row], strValue);
					break;
				case 1:
					strcpy(telphone_def.name[row], strValue);
					break;
				case 2:
					strcpy(telphone_def.namecode[row], strValue);
					break;
				case 3:
					strcpy(telphone_def.telphone[row], strValue);
					break;
				case 4:
					nci=0;
					dvl=strtok(strValue,dvlC);
					while(dvl)
					{
						strcpy(telphone_def.Other_telphones[row][nci], dvl);
						dvl=strtok(NULL,dvlC);
						nci++;
						if(nci>4)break;
					}
					break;
				case 5:
					strcpy(telphone_def.dial_out_add_num[row], strValue);
					break;
				}
				canrowinc=1;
				k++;
					}
					
					if(telphone_def.namecode[row][0]=='\0' && telphone_def.namecode[row][1]=='\0')
						row--;
					row=row<-1?-1:row;	row=row>TELPHONE_NUM-2?TELPHONE_NUM-2:row;
					
					telphone_def.real_telphone_def_num=row+1;
					//电话定义表配置读取 end
					
					
					//人员分组定义表配置读取 start
					sstep=1;
					strcpy(keyword, "人员分组定义表");
					k=sizeof(str);
				
					while(sstep>0)
					{
						if(fgets(str, k, fp)==NULL)break;
						UTF8ToAscii(str);
						if(sstep==1){if(strstr(str, "<Worksheet")!=NULL)sstep=2;}
						if(sstep==2){if(strstr(str, keyword)!=NULL){sstep=3;break;}}
					}
					
					while(fgets(str, sizeof(str), fp)!=NULL )
					{
						UTF8ToAscii(str);
						if(strstr(str, "</Row>")){break;}
						if(strstr(str, "<Row"))	{continue;}		
					}
					
					
					row=0;canrowinc=0;
					memset(&telphonegroup_def,0,sizeof(telphonegroup_def));
					for(nci=0;nci<TELPHONEGROUP_NUM;nci++)
					{
						memset(&telphonegroup_def.weeklimit[nci],1,sizeof(telphonegroup_def.weeklimit[nci]));
						memset(&telphonegroup_def.days[nci],1,sizeof(telphonegroup_def.days[nci]));
						memset(&telphonegroup_def.months[nci],1,sizeof(telphonegroup_def.months[nci]));
						telphonegroup_def.intit[nci]=2;
						telphonegroup_def.shours[nci]=0; 
						telphonegroup_def.ehours[nci]=23; 
						telphonegroup_def.smins[nci]=0; 
						telphonegroup_def.emins[nci]=59; 	
					}
					
					rsp=0;//
					while(fgets(str, sizeof(str), fp))
					{
						UTF8ToAscii(str);
						if( strstr(str, "</Table>"))
						{memset(str,0,sizeof(str));break;}
						if(strstr(str, "<Row"))
						{
							if(canrowinc==1)
							{ 
								row++;
								if(row>TELPHONE_NUM-1)break;
								canrowinc=0;
							
							}
							k=0;
							rsp=0;//
						}
						
						if(rsp==0)
						{
							if(strstr(str, ckendcharc))
								rsp=1;
						}
						if(rsp==1)
						{
							if(strstr(str, ckendcharc))
							{	
								if(strstr(str, "/>"))
								{
									startptr=strstr(str, ckendcharc);
									startptr+=strlen(ckendcharc);
									if(!strstr(startptr, "<"))
										k++;
								}
							}
							if(GetIntValueByIndexStr(str, ckendcharidx, &index))
								k= index-1;
							
							startptr=strstr(str, ckendchar3);
							if(startptr)
							{
								rsp=2;
								startptr+= strlen(ckendchar3);
							}
							else
							{
								startptr=strstr(str, ckendchar4);
								if(startptr)
								{
									rsp=2;
									startptr+= strlen(ckendchar4);
								}
							}
						}
						if(rsp==2)
						{
							memset(strValue,0,sizeof(strValue));
							XmlCellcheckadgetvalue(startptr,strValue,ckstartchar,ckendchar,ckendchar2,1,&ended);
							if(ended==1)
								rsp=88;
							else
								rsp=24;
						}
						if(rsp==24)
						{
							memset(strValuetm,0,sizeof(strValuetm));
							XmlCellcheckadgetvalue(startptr,strValuetm,ckstartchar,ckendchar,ckendchar2,1,&ended);
							if(strValuetm)
								strcat(strValue,strValuetm);
							if(ended==1)
								rsp=88;
						}
						if(rsp!=88)
							continue;
						rsp=0;
						
						switch(k)
						{
						case 0:
							strcpy(telphonegroup_def.name[row], strValue);
							telphonegroup_def.valid[row]=1;
							break;
						case 1:
							strcpy(telphonegroup_def.grpnamecode[row], strValue);
							break;
						case 2:
							nci=0;
							hwfg=0;
							hdfg=0;
							hmfg=0;
							dvl=strtok(strValue,dvlC);
							while(dvl)
							{   	
								if(dvl[0]=='W' || dvl[0]=='w')
								{
									if(dvl[1]>=48 && dvl[1]<56)
									{
										if(dvl[1]==55)
											dvl[1]=48;
										if(hwfg==0)
											memset(&telphonegroup_def.weeklimit[row],0,sizeof(telphonegroup_def.weeklimit[row]));
										telphonegroup_def.weeklimit[row][dvl[1]-48]=1;
									
									telphonegroup_def.intit[row]=9;
										hwfg=1;
									}
								}
								else if(dvl[0]=='D' || dvl[0]=='d')
								{
									if(dvl[2]>47 && dvl[2]<58)
									{
										nci=(dvl[1]-48)*10+dvl[2]-48;
									}
									else
									{
										nci=dvl[1]-48;
									}
									if(nci>0 && nci<32)
									{	
										if(hdfg==0)
									 memset(&telphonegroup_def.days[row],0,sizeof(telphonegroup_def.days[row]));
										
										telphonegroup_def.days[row][nci-1]=1;
										telphonegroup_def.intit[row]=9;
										hdfg=1;
									}
								}
								else if(dvl[0]=='M' || dvl[0]=='m')
								{
									if(dvl[2]>47 && dvl[2]<58)
									{
										nci=(dvl[1]-48)*10+dvl[2]-48;
									}
									else
									{
										nci=dvl[1]-48;
									}
									if(nci>0 && nci<13)
									{	
										if(hmfg==0)
											memset(&telphonegroup_def.months[row],0,sizeof(telphonegroup_def.months[row]));
										
										telphonegroup_def.months[row][nci-1]=1;
										telphonegroup_def.intit[row]=10;
										hmfg=1;
									}
								}
								else if(dvl[0]=='S' || dvl[0]=='s')
								{
									if(dvl[2]==':') 
									{ 
										if(dvl[1]<48 || dvl[1]>57) break;
										if(dvl[3]<48 || dvl[3]>57) break;
										if(dvl[4]<48 || dvl[4]>57) break;
										telphonegroup_def.shours[row]=dvl[1]-48; 
										if(telphonegroup_def.shours[row]>23)
											telphonegroup_def.shours[row]=23; 
										telphonegroup_def.smins[row]=(dvl[3]-48)*10+dvl[4]-48;
										if(telphonegroup_def.smins[row]>59)
											telphonegroup_def.smins[row]=59; 
									}
									else if(dvl[3]==':')
									{
										if(dvl[1]<48 || dvl[1]>57) break;
										if(dvl[2]<48 || dvl[2]>57) break;
										if(dvl[4]<48 || dvl[4]>57) break;
										if(dvl[5]<48 || dvl[5]>57) break;
										telphonegroup_def.shours[row]=(dvl[1]-48)*10+dvl[2]-48;
										if(telphonegroup_def.shours[row]>23)
											telphonegroup_def.shours[row]=23; 
										telphonegroup_def.smins[row]=(dvl[4]-48)*10+dvl[5]-48;
										if(telphonegroup_def.smins[row]>59)
											telphonegroup_def.smins[row]=59; 
									}
								}
								else if(dvl[0]=='E' || dvl[0]=='e')
								{
									if(dvl[2]==':')
									{ 
										if(dvl[1]<48 || dvl[1]>57) break;
										if(dvl[3]<48 || dvl[3]>57) break;
										if(dvl[4]<48 || dvl[4]>57) break;
										telphonegroup_def.ehours[row]=dvl[1]-48; 
										if(telphonegroup_def.ehours[row]>23)
											telphonegroup_def.ehours[row]=23; 
										telphonegroup_def.emins[row]=(dvl[3]-48)*10+dvl[4]-48;
										if(telphonegroup_def.emins[row]>59)
											telphonegroup_def.emins[row]=59; 
									}
									else if(dvl[3]==':')
									{
										if(dvl[1]<48 || dvl[1]>57) break;
										if(dvl[2]<48 || dvl[2]>57) break;
										if(dvl[4]<48 || dvl[4]>57) break;
										if(dvl[5]<48 || dvl[5]>57) break;
										telphonegroup_def.ehours[row]=(dvl[1]-48)*10+dvl[2]-48;
										if(telphonegroup_def.ehours[row]>23)
											telphonegroup_def.ehours[row]=23; 
										telphonegroup_def.emins[row]=(dvl[4]-48)*10+dvl[5]-48;
										if(telphonegroup_def.emins[row]>59)
											telphonegroup_def.emins[row]=59; 
										if(nci>TELPHONEGROUP_NUM-1)
											break;
									}
								}
								dvl=strtok(NULL,dvlC);
							}
							if(hwfg==0)
								memset(&telphonegroup_def.weeklimit[row],1,sizeof(telphonegroup_def.weeklimit[row]));
							if(hdfg==0)
								memset(&telphonegroup_def.days[row],1,sizeof(telphonegroup_def.days[row]));
							if(hmfg==0)
								memset(&telphonegroup_def.months[row],1,sizeof(telphonegroup_def.months[row]));
					
							break;
						case 3:
							nci=0;
							dvl=strtok(strValue,dvlC);
							while(dvl)
							{
								for(hwfg=0;hwfg<telphone_def.real_telphone_def_num;hwfg++)
								{
									if(telphone_def.namecode[hwfg][0]==0) continue;
									if(strcmp(telphone_def.namecode[hwfg],dvl)==0)
									{
										strcpy(telphonegroup_def.telphonenums[row][nci], telphone_def.telphone[hwfg]);
										strcpy(telphonegroup_def.namecodes[row][nci], telphone_def.namecode[hwfg]);
										break;
									}
								}
								dvl=strtok(NULL,dvlC);
								nci++;
								if(nci>TELPHONE_NUM-1)
									break;
							}
							break;
						}
						canrowinc=1;
						k++;
					}
					
					if(telphonegroup_def.grpnamecode[row][0]=='\0' && telphonegroup_def.grpnamecode[row][1]=='\0')
						row--;
					row=row<-1?-1:row;	row=row>TELPHONEGROUP_NUM-2?TELPHONEGROUP_NUM-2:row;
					
					telphonegroup_def.real_telphonegroup_def_num=row+1;


					for(nci=telphonegroup_def.real_telphonegroup_def_num;nci<TELPHONEGROUP_NUM;nci++)
					{
						memset(&telphonegroup_def.weeklimit[nci],0,sizeof(telphonegroup_def.weeklimit[nci]));
						memset(&telphonegroup_def.days[nci],0,sizeof(telphonegroup_def.days[nci]));
						memset(&telphonegroup_def.months[nci],0,sizeof(telphonegroup_def.months[nci]));
						telphonegroup_def.intit[nci]=0;
						telphonegroup_def.shours[nci]=0; 
						telphonegroup_def.ehours[nci]=0; 
						telphonegroup_def.smins[nci]=0; 
						telphonegroup_def.emins[nci]=0; 
						telphonegroup_def.valid[nci]=0; 
					}
					//人员分组定义表配置读取 end
					
					
					//报警设备配置读取 start
					
					//if(readcfgxmlltimes==0)//20120312
				//	{
						//readcfgxmlltimes++;
				 
					
						sstep=1;
						strcpy(keyword, "报警设备配置表");
						k=sizeof(str);
						while(sstep>0)
						{
							if(fgets(str, k, fp)==NULL)break;
							UTF8ToAscii(str);
							if(sstep==1){if(strstr(str, "<Worksheet")!=NULL)sstep=2;}
							if(sstep==2){if(strstr(str, keyword)!=NULL){sstep=3;break;}}
						}
						
						while(fgets(str, sizeof(str), fp)!=NULL )
						{
							UTF8ToAscii(str);
							if(strstr(str, "</Row>")){break;}
							if(strstr(str, "<Row"))	{continue;}		
						}
						
						row=0;canrowinc=0;
						
						oncall_alm_dev_def.real_oncall_alm_dev_def_num=0;
						//memset(&oncall_alm_dev_def,0,sizeof(oncall_alm_dev_def));20120312
						 
						rsp=0;//
						while(fgets(str, sizeof(str), fp))
						{
							UTF8ToAscii(str);
							if( strstr(str, "</Table>"))
							{memset(str,0,sizeof(str));break;}
							if(strstr(str, "<Row"))
							{
								if(canrowinc==1)
								{ 
									row++;
									if(row>MAXDEV_NUM-1)break;
									canrowinc=0;
								}
								k=0;
								rsp=0;//
							}
							
							if(rsp==0)
							{
								if(strstr(str, ckendcharc))
									rsp=1;
							}
							if(rsp==1)
							{
								if(strstr(str, ckendcharc))
								{	
									if(strstr(str, "/>"))
									{
										startptr=strstr(str, ckendcharc);
										startptr+=strlen(ckendcharc);
										if(!strstr(startptr, "<"))
											k++;
									}
								}
								if(GetIntValueByIndexStr(str, ckendcharidx, &index))
									k= index-1;
								
								startptr=strstr(str, ckendchar3);
								if(startptr)
								{
									rsp=2;
									startptr+= strlen(ckendchar3);
								}
								else
								{
									startptr=strstr(str, ckendchar4);
									if(startptr)
									{
										rsp=2;
										startptr+= strlen(ckendchar4);
									}
								}
							}
							if(rsp==2)
							{
								memset(strValue,0,sizeof(strValue));
								XmlCellcheckadgetvalue(startptr,strValue,ckstartchar,ckendchar,ckendchar2,1,&ended);
								if(ended==1)
									rsp=88;
								else
									rsp=24;
							}
							if(rsp==24)
							{
								memset(strValuetm,0,sizeof(strValuetm));
								XmlCellcheckadgetvalue(startptr,strValuetm,ckstartchar,ckendchar,ckendchar2,1,&ended);
								if(strValuetm)
									strcat(strValue,strValuetm);
								if(ended==1)
									rsp=88;
							}
							if(rsp!=88)
								continue;
							rsp=0;
							switch(k)
							{
							case 0:
								strupr(strValue);strValue[3]='\0';
								if(strstr(strValue,"YES")!=NULL)
									oncall_alm_dev_def.replacemanuformset[row]=1;
								else
									oncall_alm_dev_def.replacemanuformset[row]=0;
								break;
								
							case 1:
								if(oncall_alm_dev_def.replacemanuformset[row]==1 || gtk_onvall_mainform_created==0)
									strcpy(oncall_alm_dev_def.name[row], strValue);
								break;
								
							case 2:
								if(oncall_alm_dev_def.replacemanuformset[row]==1 || gtk_onvall_mainform_created==0)
									strcpy(oncall_alm_dev_def.logicname[row], strValue);
								break;
							case 3:
								if(oncall_alm_dev_def.replacemanuformset[row]==1 || gtk_onvall_mainform_created==0)
								{
									
									strupr(strValue);strValue[3]='\0';
									if(strstr(strValue,"YES")!=NULL)
										oncall_alm_dev_def.SMS_ALM[row]=1;
									else
										oncall_alm_dev_def.SMS_ALM[row]=0;
								}
								break;
							case 4:
								if(oncall_alm_dev_def.replacemanuformset[row]==1 || gtk_onvall_mainform_created==0)
								{
									
									strupr(strValue);strValue[3]='\0';
									if(strstr(strValue,"YES")!=NULL)
										oncall_alm_dev_def.TEL_ALM[row]=1;
									else
										oncall_alm_dev_def.TEL_ALM[row]=0;
								}
								break;
							}
							canrowinc=1;
							k++;
						}
						
						if(oncall_alm_dev_def.logicname[row][0]=='\0' && oncall_alm_dev_def.logicname[row][1]=='\0')
							row--;
						row=row<-1?-1:row;	row=row>MAXDEV_NUM-2?MAXDEV_NUM-2:row;
						
						oncall_alm_dev_def.real_oncall_alm_dev_def_num=row+1;
						//	}
						//报警设备配置读取 end
						
						for(nci=oncall_alm_dev_def.real_oncall_alm_dev_def_num;nci<MAXDEV_NUM;nci++)
						{
							oncall_alm_dev_def.replacemanuformset[nci]=0;
							oncall_alm_dev_def.name[nci][0]=0;						 
							oncall_alm_dev_def.logicname[nci][0]=0;										 
							oncall_alm_dev_def.TEL_ALM[nci]=0;
							oncall_alm_dev_def.SMS_ALM[nci]=0;						 
							
						}

						if(gtk_onvall_mainform_created==1)
						{
							set_selecttelalmdevtModel();
							set_selectsmsalmdevtModel();
						}
					//报警信息定义表配置读取 start
					sstep=1;
					strcpy(keyword, "报警信息定义表");
					k=sizeof(str);
					while(sstep>0)
					{
						if(fgets(str, k, fp)==NULL)break;
						UTF8ToAscii(str);
						if(sstep==1){ if(strstr(str, "<Worksheet")!=NULL)sstep=2;}
						if(sstep==2){if(strstr(str, keyword)!=NULL){sstep=3;break;}}
					}
					
					while(fgets(str, sizeof(str), fp)!=NULL )
					{
						UTF8ToAscii(str);
						if(strstr(str, "</Row>")){break;}
						if(strstr(str, "<Row"))	{continue;}		
					}
					
					row=0;canrowinc=0;
					memset(&oncall_alm_def,0,sizeof(oncall_alm_def));
					
					
					rsp=0;//
					while(fgets(str, sizeof(str), fp))
					{
						UTF8ToAscii(str);
						if( strstr(str, "</Table>"))
						{memset(str,0,sizeof(str));break;}
						if(strstr(str, "<Row"))
						{
							if(canrowinc==1)
							{ 
								row++;
								if(row>ALARM_NUM-1)break;
								canrowinc=0;
							}
							k=0;
							rsp=0;//
						}
						
						if(rsp==0)
						{
							if(strstr(str, ckendcharc))
								rsp=1;
						}
						if(rsp==1)
						{
							if(strstr(str, ckendcharc))
							{	
								if(strstr(str, "/>"))
								{
									startptr=strstr(str, ckendcharc);
									startptr+=strlen(ckendcharc);
									if(!strstr(startptr, "<"))
										k++;
								}
							}
							if(GetIntValueByIndexStr(str, ckendcharidx, &index))
								k= index-1;
							
							startptr=strstr(str, ckendchar3);
							if(startptr)
							{
								rsp=2;
								startptr+= strlen(ckendchar3);
							}
							else
							{
								startptr=strstr(str, ckendchar4);
								if(startptr)
								{
									rsp=2;
									startptr+= strlen(ckendchar4);
								}
							}
						}
						if(rsp==2)
						{
							memset(strValue,0,sizeof(strValue));
							XmlCellcheckadgetvalue(startptr,strValue,ckstartchar,ckendchar,ckendchar2,1,&ended);
							if(ended==1)
								rsp=88;
							else
								rsp=24;
						}
						if(rsp==24)
						{
							memset(strValuetm,0,sizeof(strValuetm));
							XmlCellcheckadgetvalue(startptr,strValuetm,ckstartchar,ckendchar,ckendchar2,1,&ended);
							if(strValuetm)
								strcat(strValue,strValuetm);
							if(ended==1)
								rsp=88;
						}
						if(rsp!=88)
							continue;
						rsp=0;
						
						switch(k)
						{
						case 0:
							strcpy(oncall_alm_def.longname[row], strValue);
							break;
						case 1:
							strcpy(oncall_alm_def.logicname[row], strValue);
							break;
						case 2:
							strcpy(oncall_alm_def.property[row], strValue);
							break;
						case 3:
							strupr(strValue);
							//strValue[3]='\0';
							if(strstr(strValue,"YES_NORMAL")!=NULL)
								oncall_alm_def.SMS_ALM[row]=2;
							else if(strstr(strValue,"YES")!=NULL)
								oncall_alm_def.SMS_ALM[row]=1;
							else
								oncall_alm_def.SMS_ALM[row]=0;
							break;
						case 4:
							strupr(strValue);
							strValue[3]='\0';
							if(strstr(strValue,"YES")!=NULL)
								oncall_alm_def.TEL_ALM[row]=1;
							break;
						case 5:
							strupr(strValue);
							strValue[3]='\0';
							oncall_alm_def.almtype[row] = atoi(strValue);
							break;
						case 6:
							nci=0;
							dvl=strtok(strValue,dvlC);
							while(dvl)
							{
								if(dvl[0]=='[')
								{
									for(hwfg=0;hwfg<20;hwfg++)
									{
										if(dvl[hwfg+1]==0)
											break;
										if(dvl[hwfg+1]==']')
											break;
										oncall_alm_def.groupnamecode[row][nci][hwfg]=dvl[hwfg+1];
									}
								}
								else
									strcpy(oncall_alm_def.namecode[row][nci], dvl);
								dvl=strtok(NULL,dvlC);
								nci++;
								if(nci>TELPHONE_NUM-1)
									break;
							}
							break;
						}
						canrowinc=1;
						k++;
					}
					
					if(oncall_alm_def.logicname[row][0]=='\0' && oncall_alm_def.logicname[row][1]=='\0')
						row--;
					row=row<-1?-1:row;	row=row>ALARM_NUM-2?ALARM_NUM-2:row;
					
					oncall_alm_def.real_oncall_alm_buf_num=row+1;
					//报警信息定义表配置读取 end
					
					
					//查询代码定义表读取 start
					sstep=1;
					strcpy(keyword, "查询代码定义表");
					k=sizeof(str);
					while(sstep>0)
					{
						if(fgets(str, k, fp)==NULL)break;
						UTF8ToAscii(str);
						if(sstep==1){if(strstr(str, "<Worksheet")!=NULL)sstep=2;}
						if(sstep==2){if(strstr(str, keyword)!=NULL){sstep=3;break;}}
					}
					
					while(fgets(str, sizeof(str), fp)!=NULL )
					{
						UTF8ToAscii(str);
						if(strstr(str, "</Row>")){break;}
						if(strstr(str, "<Row"))	{continue;}		
					}
					
					row=-2;canrowinc=0;
					memset(&oncallinquiry_def,0,sizeof(oncallinquiry_def));
					rsp=0;//
					while(fgets(str, sizeof(str), fp))
					{
						UTF8ToAscii(str);
						if( strstr(str, "</Table>"))
						{memset(str,0,sizeof(str));break;}
						if(strstr(str, "<Row"))
						{
							if(canrowinc==1)
							{ 
								row++;
								if(row>ONCALLINQUIRY_NUM-1)break;
								canrowinc=0;
							}
							k=0;
							rsp=0;//
						}
						
						if(rsp==0)
						{
							if(strstr(str, ckendcharc))
								rsp=1;
						}
						if(rsp==1)
						{
							if(strstr(str, ckendcharc))
							{	
								if(strstr(str, "/>"))
								{
									startptr=strstr(str, ckendcharc);
									startptr+=strlen(ckendcharc);
									if(!strstr(startptr, "<"))
										k++;
								}
							}
							if(GetIntValueByIndexStr(str, ckendcharidx, &index))
								k= index-1;
							
							startptr=strstr(str, ckendchar3);
							if(startptr)
							{
								rsp=2;
								startptr+= strlen(ckendchar3);
							}
							else
							{
								startptr=strstr(str, ckendchar4);
								if(startptr)
								{
									rsp=2;
									startptr+= strlen(ckendchar4);
								}
							}
						}
						if(rsp==2)
						{
							memset(strValue,0,sizeof(strValue));
							XmlCellcheckadgetvalue(startptr,strValue,ckstartchar,ckendchar,ckendchar2,1,&ended);
							if(ended==1)
								rsp=88;
							else
								rsp=24;
						}
						if(rsp==24)
						{
							memset(strValuetm,0,sizeof(strValuetm));
							XmlCellcheckadgetvalue(startptr,strValuetm,ckstartchar,ckendchar,ckendchar2,1,&ended);
							if(strValuetm)
								strcat(strValue,strValuetm);
							if(ended==1)
								rsp=88;
						}
						if(rsp!=88)
							continue;
						rsp=0;
						
						switch(row)
						{
						case -2:
							if(k==2)
								strcpy(oncallinquiry_def.telerrcodere, strValue);//处理
							break;
						case -1:
							if(k==2)
								strcpy(oncallinquiry_def.smserrcodere, strValue);//处理
							break;
						default:
							switch(k)
							{
							case 0:
								strcpy(oncallinquiry_def.TEL_code[row], strValue);
								break;
							case 1:
								strcpy(oncallinquiry_def.SMS_code[row], strValue);
								break;
							case 2:
								strcpy(oncallinquiry_def.longname[row], strValue);
								break;
							case 3:
								nci=0;
								dvl=strtok(strValue,dvlC);
								while(dvl)
								{
									strcpy(oncallinquiry_def.logicnames[row][nci], dvl);
									dvl=strtok(NULL,dvlC);
									nci++;
									if(nci>19)
										break;
								}
								break;
							case 4:
							/*if(oncallinquiry_def.logicnames[row][0][0]==0)//20101217取消 
							{
							for(nci=0;nci<telphone_def.real_telphone_def_num;nci++)
							strcpy(oncallinquiry_def.namecode[row][nci], telphone_def.namecode[nci]);
							}
							else
								{*/
								nci=0;
								dvl=strtok(strValue,dvlC);
								while(dvl)
								{
									if(dvl[0]=='[')
									{
										for(hwfg=0;hwfg<20;hwfg++)
										{
											if(dvl[hwfg+1]==0)
												break;
											if(dvl[hwfg+1]==']')
												break;
											oncallinquiry_def.groupnamecode[row][nci][hwfg]=dvl[hwfg+1];
										}
									}
									else
										strcpy(oncallinquiry_def.namecode[row][nci], dvl);
									dvl=strtok(NULL,dvlC);
									nci++;
									if(nci>TELPHONE_NUM-1)
										break;
								}
								//}
								break;
							}
							break;
						}
						canrowinc=1;
						k++;	
			}
			if(oncallinquiry_def.logicnames[row][0][0]=='\0' && oncallinquiry_def.logicnames[row][0][1]=='\0')
				row--;
			row=row<-1?-1:row;	row=row>ONCALLINQUIRY_NUM-2?ONCALLINQUIRY_NUM-2:row;
			
			oncallinquiry_def.real_oncallinquiry_def_num=row+1;
			//查询代码定义表读取 end
			
			
			//定时发送短消息定义表读取 start
			sstep=1;
			strcpy(keyword, "定时发送短消息定义表");
			k=sizeof(str);
			while(sstep>0)
			{
				if(fgets(str, k, fp)==NULL)break;
				UTF8ToAscii(str);
				if(sstep==1){if(strstr(str, "<Worksheet")!=NULL)sstep=2;}
				if(sstep==2){if(strstr(str, keyword)!=NULL){sstep=3;break;}}
			}
			
			while(fgets(str, sizeof(str), fp)!=NULL )
			{
				UTF8ToAscii(str);
				if(strstr(str, "</Row>")){break;}
				if(strstr(str, "<Row"))	{continue;}		
			}
			
			row=0;canrowinc=0;
			memset(&timingsend_def,0,sizeof(timingsend_def));
			
			rsp=0;//
			while(fgets(str, sizeof(str), fp))
			{
				UTF8ToAscii(str);
				if( strstr(str, "</Table>"))
				{memset(str,0,sizeof(str));break;}
				if(strstr(str, "<Row"))
				{
					if(canrowinc==1)
					{ 
						row++;
						if(row>TIMING_NUM-1)break;
						canrowinc=0;
					}
					k=0;
					rsp=0;//
				}
				
				if(rsp==0)
				{
					if(strstr(str, ckendcharc))
						rsp=1;
				}
				if(rsp==1)
				{
					if(strstr(str, ckendcharc))
					{	
						if(strstr(str, "/>"))
						{
							startptr=strstr(str, ckendcharc);
							startptr+=strlen(ckendcharc);
							if(!strstr(startptr, "<"))
								k++;
						}
					}
					if(GetIntValueByIndexStr(str, ckendcharidx, &index))
						k= index-1;
					
					startptr=strstr(str, ckendchar3);
					if(startptr)
					{
						rsp=2;
						startptr+= strlen(ckendchar3);
					}
					else
					{
						startptr=strstr(str, ckendchar4);
						if(startptr)
						{
							rsp=2;
							startptr+= strlen(ckendchar4);
						}
					}
				}
				if(rsp==2)
				{
					memset(strValue,0,sizeof(strValue));
					XmlCellcheckadgetvalue(startptr,strValue,ckstartchar,ckendchar,ckendchar2,1,&ended);
					if(ended==1)
						rsp=88;
					else
						rsp=24;
				}
				if(rsp==24)
				{
					memset(strValuetm,0,sizeof(strValuetm));
					XmlCellcheckadgetvalue(startptr,strValuetm,ckstartchar,ckendchar,ckendchar2,1,&ended);
					if(strValuetm)
						strcat(strValue,strValuetm);
					if(ended==1)
						rsp=88;
				}
				if(rsp!=88)
					continue;
				rsp=0;
				switch(k)
				{
				case 0:
					strcpy(timingsend_def.longname[row], strValue);
					break;
				case 1:
					nci=0;
					dvl=strtok(strValue,dvlC);
					while(dvl)
					{
						strcpy(timingsend_def.logicnames[row][nci], dvl);
						dvl=strtok(NULL,dvlC);
						nci++;
						if(nci>19)
							break;
					}
					break;
				case 2:
					nci=0;
					dvl=strtok(strValue,dvlC);
					while(dvl)
					{
						//strcpy(timingsend_def.namecode[row][nci], dvl);//处理时间
						
						if(dvl[1]==':')
						{ 
							if(dvl[0]<48 || dvl[0]>57) break;
							if(dvl[2]<48 || dvl[2]>57) break;
							if(dvl[3]<48 || dvl[3]>57) break;
							timingsend_def.hour[row][nci]=dvl[0]-48; 
							if(timingsend_def.hour[row][nci]>23)break;
							timingsend_def.min[row][nci]=(dvl[2]-48)*10+dvl[3]-48; 
							if(timingsend_def.min[row][nci]>59)break;
							timingsend_def.valid[row][nci]=1;
						}
						else if(dvl[2]==':')
						{
							if(dvl[0]<48 || dvl[0]>57) break;
							if(dvl[1]<48 || dvl[1]>57) break;
							if(dvl[3]<48 || dvl[3]>57) break;
							if(dvl[4]<48 || dvl[4]>57) break;
							timingsend_def.hour[row][nci]=(dvl[0]-48)*10+dvl[1]-48; 
							if(timingsend_def.hour[row][nci]>23)break;
							timingsend_def.min[row][nci]=(dvl[3]-48)*10+dvl[4]-48;
							if(timingsend_def.min[row][nci]>59)break;
							timingsend_def.valid[row][nci]=1;
						}
						
						dvl=strtok(NULL,dvlC);
						nci++;
						if(nci>48-1)
							break;
					}
					break;
				case 3:
					nci=0;
					dvl=strtok(strValue,dvlC);
					while(dvl)
					{
						if(dvl[0]=='[')
						{
							for(hwfg=0;hwfg<20;hwfg++)
							{
								if(dvl[hwfg+1]==0)
									break;
								if(dvl[hwfg+1]==']')
									break;
								timingsend_def.groupnamecode[row][nci][hwfg]=dvl[hwfg+1];
							}
						}
						else
							strcpy(timingsend_def.namecode[row][nci], dvl);
						dvl=strtok(NULL,dvlC);
						nci++;
						if(nci>TELPHONE_NUM-1)
							break;
					}
					break;
				}
				canrowinc=1;
				k++;
					}
					
					if(timingsend_def.logicnames[row][0][0]=='\0' && timingsend_def.logicnames[row][0][1]=='\0')
						row--;
					row=row<-1?-1:row;	row=row>TIMING_NUM-2?TIMING_NUM-2:row;
					
					timingsend_def.real_timingsend_def_num=row+1;
					//定时发送短消息定义表读取 end
					
					
					ONCALLSMS_VALID=0;				
					ONCALLDIALOGIC_VALID=0;
					ONCALLSTATION_NAME[0]=0;	
					ONCAL_LOGSRECORD_SAVEDAY=60;
				 
					//strcpy( ONCALLCOM_PORT, "COM1");	    	
					//ONCALLBAUD_RATE=9600;
					//ONCALLPARITY_TYPE=0;//  NOPARITY,  ODDPARITY, EVENPARITY    0,1,2
					strcpy(ONCALLDIAL_OUT_PORT, "dxxxB1C1");
					strcpy(ONCALLDIAL_IN_PORT, "dxxxB1C1");
					//strcpy(strSmsCenter, "8613800100500");
					//ONCALL_SMS_DEV_TYPE=0;//SIEMENS-GSM/WAVECOM-GSM/WAVECOM-CDMA 
					memset(oncallinquiry_def.telpassword,0,sizeof(oncallinquiry_def.telpassword));
					memset(oncallinquiry_def.BftelAlmSpk,0,sizeof(oncallinquiry_def.BftelAlmSpk));
					oncallinquiry_def.almfailtrytimes=5;
					oncallinquiry_def.guajiHz=450;
					oncallinquiry_def.guajifanwei=20;
					
					memset(CHANUSE,0,sizeof(CHANUSE));

					memset(_ONCALLCOM_PORTS,0,sizeof(_ONCALLCOM_PORTS));//20120327
					memset(_BAK_ONCALLCOM_PORTS,0,sizeof(_BAK_ONCALLCOM_PORTS));//20120327
					memset(oncall_sms_err_times,0,sizeof(oncall_sms_err_times));//20120327
					
					CHANSTARTNUM=0;
					CHANNUM=0;
					
					
					sstep=1;
					strcpy(keyword, "其它配置表");
					k=sizeof(str);
					while(sstep>0)
					{
						if(fgets(str, k, fp)==NULL)break;
						UTF8ToAscii(str);
						if(sstep==1){ if(strstr(str, "<Worksheet")!=NULL)sstep=2;}
						if(sstep==2){if(strstr(str, keyword)!=NULL){sstep=3;break;}}
					}
					
					while(fgets(str, sizeof(str), fp)!=NULL )
					{
						UTF8ToAscii(str);
						if(strstr(str, "</Row>")){break;}
						if(strstr(str, "<Row"))	{continue;}		
					}
					
					row=0;canrowinc=0;
					rsp=0;//
					while(fgets(str, sizeof(str), fp))
					{
						UTF8ToAscii(str);
						if( strstr(str, "</Table>"))
						{memset(str,0,sizeof(str));break;}
						if(strstr(str, "<Row"))
						{
							if(canrowinc==1)
							{ 
								row++;
								if(row>100)break;
								canrowinc=0;
							}
							k=0;
							rsp=0;//
						}
						
						if(rsp==0)
						{
							if(strstr(str, ckendcharc))
								rsp=1;
						}
						if(rsp==1)
						{
							if(strstr(str, ckendcharc))
							{	
								if(strstr(str, "/>"))
								{
									startptr=strstr(str, ckendcharc);
									startptr+=strlen(ckendcharc);
									if(!strstr(startptr, "<"))
										k++;
								}
							}
							if(GetIntValueByIndexStr(str, ckendcharidx, &index))
								k= index-1;
							
							startptr=strstr(str, ckendchar3);
							if(startptr)
							{
								rsp=2;
								startptr+= strlen(ckendchar3);
							}
							else
							{
								startptr=strstr(str, ckendchar4);
								if(startptr)
								{
									rsp=2;
									startptr+= strlen(ckendchar4);
								}
							}
						}
						if(rsp==2)
						{
							memset(strValue,0,sizeof(strValue));
							XmlCellcheckadgetvalue(startptr,strValue,ckstartchar,ckendchar,ckendchar2,1,&ended);
							if(ended==1)
								rsp=88;
							else
								rsp=24;
						}
						if(rsp==24)
						{
							memset(strValuetm,0,sizeof(strValuetm));
							XmlCellcheckadgetvalue(startptr,strValuetm,ckstartchar,ckendchar,ckendchar2,1,&ended);
							if(strValuetm)
								strcat(strValue,strValuetm);
							if(ended==1)
								rsp=88;
						}
						if(rsp!=88)
							continue;
						rsp=0;
						if(k==1)
						{ 
							switch(row)
							{
							case 0:
								//strcpy(strSmsCenter, strValue);

								nci=0;
								dvl=strtok(strValue,dvlC);
								while(dvl)
								{
								  
									strcpy(_strSmsCenterS[nci], dvl);
									if(nci>=MAX_sms_dev_num-1)
										break;

									dvl=strtok(NULL,dvlC);
									nci++;
								}


								break;
							case 1:
								strupr(strValue);

								nci=0;
								dvl=strtok(strValue,dvlC);
								while(dvl)
								{
								 
									
									if(strstr(dvl,"SIEMENS-GSM")!=NULL)
										_ONCALL_SMS_DEV_TYPES[nci]=0;
									else 	if(strstr(dvl,"WAVECOM-GSM")!=NULL)
										_ONCALL_SMS_DEV_TYPES[nci]=1;
									else 	if(strstr(dvl,"WAVECOM-CDMA")!=NULL)
										_ONCALL_SMS_DEV_TYPES[nci]=2;
									
									if(nci>=MAX_sms_dev_num-1)
										break;
									
									dvl=strtok(NULL,dvlC);
									nci++;
								}



								/*if(strstr(strValue,"SIEMENS-GSM")!=NULL)
									ONCALL_SMS_DEV_TYPE=0;
								else 	if(strstr(strValue,"WAVECOM-GSM")!=NULL)
									ONCALL_SMS_DEV_TYPE=1;
								else 	if(strstr(strValue,"WAVECOM-CDMA")!=NULL)
									ONCALL_SMS_DEV_TYPE=2;*/
								break;
							case 2:
								nci=0;
								dvl=strtok(strValue,dvlC);
								while(dvl)
								{
									nci=0;
									if(dvl[0]>=48 && dvl[0]<=57)
									{	
										if(dvl[1]>=48 && dvl[1]<=57)
											nci=(dvl[0]-48)*10+dvl[1]-48;
										else
											nci=dvl[0]-48;
									}
									else
									{
										nci=-1;
									}
									dvl=strtok(NULL,dvlC);
									nci--;
									if(nci<0)
										continue;
									if(nci>MAXCHAN-1)
										continue;
									CHANUSE[nci]=1;
								}
								break;
							 
								case 3:
									strcpy(fault_alm_logicnames, strValue);
								break;
								/*case 4:
								if(strValue[0]>=48 && strValue[0]<=57)
								{	
								if(strValue[1]>=48 && strValue[1]<=57)
								CHANNUM=(strValue[0]-48)*10+strValue[1]-48;
								else
								CHANNUM=strValue[0]-48;
								}
								if(CHANNUM<1)
								CHANNUM=0;
								if(CHANNUM>MAXCHAN)
								CHANNUM=MAXCHAN;
								break;
								*/
							case 5:
								strupr(strValue);

								nci=0;
								dvl=strtok(strValue,dvlC);
								while(dvl)
								{
									
									strcpy(_ONCALLCOM_PORTS[nci], dvl);

									strcpy(_BAK_ONCALLCOM_PORTS[nci], _ONCALLCOM_PORTS[nci]);//20120327
								 

									if(nci>=MAX_sms_dev_num-1)
										break;
									
									dvl=strtok(NULL,dvlC);
									nci++;
								}



								//strcpy(ONCALLCOM_PORT, strValue);
								break;
							case 6:
								//ONCALLBAUD_RATE=atoi(strValue);

								nci=0;
								dvl=strtok(strValue,dvlC);
								while(dvl)
								{
									 
									_ONCALLBAUD_RATES[nci]=atoi(dvl);
								
									if(nci>=MAX_sms_dev_num-1)
										break;
									
									dvl=strtok(NULL,dvlC);
									nci++;
								}
								break;
							case 7:
								strupr(strValue);


								nci=0;
								dvl=strtok(strValue,dvlC);
								while(dvl)
								{
							 
									
									if(strstr(dvl,"NOPARITY")!=NULL)
										_ONCALLPARITY_TYPES[nci]=0;
									else 	if(strstr(dvl,"ODDPARITY")!=NULL)
										_ONCALLPARITY_TYPES[nci]=1;
									else 	if(strstr(dvl,"EVENPARITY")!=NULL)
									_ONCALLPARITY_TYPES[nci]=2;
									
									if(nci>=MAX_sms_dev_num-1)
										break;
									
									dvl=strtok(NULL,dvlC);
									nci++;
								}


								/*if(strstr(strValue,"NOPARITY")!=NULL)
									ONCALLPARITY_TYPE=0;
								else 	if(strstr(strValue,"ODDPARITY")!=NULL)
									ONCALLPARITY_TYPE=1;
								else 	if(strstr(strValue,"EVENPARITY")!=NULL)
									ONCALLPARITY_TYPE=2;*/
								break;
							case 8:
								strupr(strValue);strValue[3]='\0';
								if(strstr(strValue,"YES")!=NULL)
									ONCALLSMS_VALID=1;
								else 
								{
									ONCALLSMS_VALID=0;
									//for(nci=0;nci<oncall_alm_dev_def.real_oncall_alm_dev_def_num;nci++)
										//oncall_alm_dev_def.SMS_ALM[nci]=0;

								}
								break;
							case 9:
								strupr(strValue);strValue[3]='\0';
								if(strstr(strValue,"YES")!=NULL)
									ONCALLDIALOGIC_VALID=1;
								else 
								{
									ONCALLDIALOGIC_VALID=0;
								//	for(nci=0;nci<oncall_alm_dev_def.real_oncall_alm_dev_def_num;nci++)
									//	oncall_alm_dev_def.TEL_ALM[nci]=0;
								}
								break;
							case 10:
								if(sizeof(strValue)>0)
								{
									nci=0;
									if(strValue[0]>47 && strValue[0]<58)nci=1;
									if(nci>0)if(strValue[1]>47 && strValue[1]<58)nci=2;
									if(nci>1)if(strValue[2]>47 && strValue[2]<58)nci=3;
									if(nci>2)if(strValue[3]>47 && strValue[3]<58)nci=4;
									switch(nci)
									{
									case 0:
										ONCAL_LOGSRECORD_SAVEDAY=60;//默认
										break;
									case 1:
										ONCAL_LOGSRECORD_SAVEDAY=strValue[0]-48;//默认
										break;
									case 2:
										ONCAL_LOGSRECORD_SAVEDAY=(strValue[0]-48)*10+strValue[1]-48;//默认
										break;
									case 3:
										ONCAL_LOGSRECORD_SAVEDAY=(strValue[0]-48)*100+(strValue[1]-48)*10+strValue[2]-48;//默认
										break;
									case 4:
										ONCAL_LOGSRECORD_SAVEDAY=(strValue[0]-48)*1000+(strValue[1]-48)*100+(strValue[2]-48)*10+strValue[3]-48;//默认
										break;
									}
									if(ONCAL_LOGSRECORD_SAVEDAY<1)ONCAL_LOGSRECORD_SAVEDAY=60;
								}
								break;
							/*case 11:
								if(sizeof(strValue)>0)
								{
									nci=0;
									if(strValue[0]>47 && strValue[0]<58)nci=1;
									if(nci>0)if(strValue[1]>47 && strValue[1]<58)nci=2;
									if(nci>1)if(strValue[2]>47 && strValue[2]<58)nci=3;
									if(nci>2)if(strValue[3]>47 && strValue[3]<58)nci=4;
									switch(nci)
									{
									case 0:
										ONCALLSMS_RECORD_SAVEDAY=60;//默认
										break;
									case 1:
										ONCALLSMS_RECORD_SAVEDAY=strValue[0]-48;//默认
										break;
									case 2:
										ONCALLSMS_RECORD_SAVEDAY=(strValue[0]-48)*10+strValue[1]-48;//默认
										break;
									case 3:
										ONCALLSMS_RECORD_SAVEDAY=(strValue[0]-48)*100+(strValue[1]-48)*10+strValue[2]-48;//默认
										break;
									case 4:
										ONCALLSMS_RECORD_SAVEDAY=(strValue[0]-48)*1000+(strValue[1]-48)*100+(strValue[2]-48)*10+strValue[3]-48;//默认
										break;
									}
									if(ONCALLSMS_RECORD_SAVEDAY<1)ONCALLSMS_RECORD_SAVEDAY=60;
								}
								break;*/
							case 12:
								kx=0;
								nci=0;
								for(kx=0;kx<10;kx++)
								{
									if(strValue[kx]>=48 && strValue[kx]<=57)
									{
										telphone_def.countrynum[nci]=strValue[kx];
										nci++;
									}
								}
								strcpy(telphone_def.countrynum, strValue);
								break;
							case 13:
								if(strValue[0]!=0)
									strcpy(oncallinquiry_def.telpassword,strValue);
								break;
							case 14:
								if(strValue[0]!=0)
									strcpy(oncallinquiry_def.BftelAlmSpk,strValue);
								break;
							case 15:
								if(strValue[0]>=48 && strValue[0]<=57)
								{	
									if(strValue[1]>=48 && strValue[1]<=57)
										oncallinquiry_def.almfailtrytimes=(strValue[0]-48)*10+strValue[1]-48;
									else
										oncallinquiry_def.almfailtrytimes=strValue[0]-48;
								}
								if(oncallinquiry_def.almfailtrytimes<1)
									oncallinquiry_def.almfailtrytimes=1;
								break;
							case 16:
								if(strValue[0]>=48 && strValue[0]<=57)
								{	
									if(strValue[1]>=48 && strValue[1]<=57)
									{
										if(strValue[2]>=48 && strValue[2]<=57)
										{
											oncallinquiry_def.guajiHz=(strValue[0]-48)*100+(strValue[1]-48)*10+strValue[2]-48;
										}
										else
										{
											oncallinquiry_def.guajiHz=(strValue[0]-48)*10+strValue[1]-48;
										}
									}
									else
										oncallinquiry_def.guajiHz=strValue[0]-48;
								}
								if(oncallinquiry_def.guajiHz<100)
									oncallinquiry_def.guajiHz=100;
								if(oncallinquiry_def.guajiHz>999)
									oncallinquiry_def.guajiHz=999;
								break;
							case 17:
								if(strValue[0]>=48 && strValue[0]<=57)
								{	
									if(strValue[1]>=48 && strValue[1]<=57)
										oncallinquiry_def.guajifanwei=(strValue[0]-48)*10+strValue[1]-48;
									else
										oncallinquiry_def.guajifanwei=strValue[0]-48;
								}
								if(oncallinquiry_def.guajifanwei<2)
									oncallinquiry_def.guajifanwei=2;
								if(oncallinquiry_def.guajifanwei>99)
									oncallinquiry_def.guajifanwei=99;
								break;
						
							}
						}
						canrowinc=1;
						k++;
					}
					CHANSTARTNUM=MAXCHAN-1;
					CHANNUM=0;
					printf("\n");
					for(nci=0;nci<MAXCHAN;nci++)
					{
						if(CHANUSE[nci]==1)
						{
							if(CHANSTARTNUM>nci)
								CHANSTARTNUM=nci;
							if(CHANNUM<nci+1)
								CHANNUM=nci+1;
							printf("LINEUSE:%d  ",nci+1);
						}
					}
					printf("\n");
					if(CHANNUM+CHANSTARTNUM>MAXCHAN)
					{
						CHANSTARTNUM=MAXCHAN-1;
						CHANNUM=1;
					}
					printf("\nONCALL CHANSTARTNUM=%d,CHANNUM=%d\n",CHANSTARTNUM,CHANNUM);
					
					//其它配置表读取 end	
					//MYDISPONSESM:
					
					readedxml=1;
					fclose(fp);
					return 1;
}

 
void insert_sms_logs(int bufidx,char * strs_in)
{
	char 	    temp_strs[30];
	struct tm 	*today;
	time_t		ltime;

	if(bufidx<0 || bufidx>=MAX_sms_dev_num)
		return;
	if(!strs_in)
		return;

	
	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(temp_strs,"%04d-%02d-%02d %02d:%02d:%02d ",today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,today->tm_min,today->tm_sec); 


	oncall_sms_logsbufs[bufidx].flag[oncall_sms_logsbufs[bufidx].a_ptr]=1;
	 
	strcpy(oncall_sms_logsbufs[bufidx].str[oncall_sms_logsbufs[bufidx].a_ptr],temp_strs);
	strcat(oncall_sms_logsbufs[bufidx].str[oncall_sms_logsbufs[bufidx].a_ptr],strs_in);

	
	oncall_sms_logsbufs[bufidx].a_ptr++;
	if(oncall_sms_logsbufs[bufidx].a_ptr>=MAX_LOGNUM_PER_BUF)
		oncall_sms_logsbufs[bufidx].a_ptr=0;

	printf("\n%s%s",temp_strs,strs_in);
}

void insert_tel_logs(int bufidx,char * strs_in)
{
	char 	    temp_strs[30];
	struct tm 	*today;
	time_t		ltime;

	if(bufidx<0 || bufidx>=MAXCHAN)
		return;
	if(!strs_in)
		return;

	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(temp_strs,"%04d-%02d-%02d %02d:%02d:%02d ",today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,today->tm_min,today->tm_sec); 


	oncall_tel_logsbufs[bufidx].flag[oncall_tel_logsbufs[bufidx].a_ptr]=1;
	 
	strcpy(oncall_tel_logsbufs[bufidx].str[oncall_tel_logsbufs[bufidx].a_ptr],temp_strs);
	strcat(oncall_tel_logsbufs[bufidx].str[oncall_tel_logsbufs[bufidx].a_ptr],strs_in);


	oncall_tel_logsbufs[bufidx].a_ptr++;
	if(oncall_tel_logsbufs[bufidx].a_ptr>=MAX_LOGNUM_PER_BUF)
		oncall_tel_logsbufs[bufidx].a_ptr=0;
	printf("\n%s%s",temp_strs,strs_in);
}


void insert_almsearch_logs(char * strs_in)
{
	char 	    temp_strs[30];
	struct tm 	*today;
	time_t		ltime;

	if(!strs_in)
		return;

	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(temp_strs,"%04d-%02d-%02d %02d:%02d:%02d ",today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,today->tm_min,today->tm_sec); 

	strcpy(oncall_almsearch_logsbuf.str[oncall_almsearch_logsbuf.a_ptr],temp_strs);
	strcat(oncall_almsearch_logsbuf.str[oncall_almsearch_logsbuf.a_ptr],strs_in);

	oncall_almsearch_logsbuf.a_ptr++;
	if(oncall_almsearch_logsbuf.a_ptr>=MAX_LOGNUM_PER_BUF)
		oncall_almsearch_logsbuf.a_ptr=0;
	printf("\n%s%s",temp_strs,strs_in);
}


void insert_doublehost_logs(char * strs_in)
{
	char 	    temp_strs[30];
	struct tm 	*today;
	time_t		ltime;

	if(!strs_in)
		return;

	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(temp_strs,"%04d-%02d-%02d %02d:%02d:%02d ",today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,today->tm_min,today->tm_sec); 


	oncall_doublehost_logsbuf.flag[oncall_doublehost_logsbuf.a_ptr]=1;
 	
	strcpy(oncall_doublehost_logsbuf.str[oncall_doublehost_logsbuf.a_ptr],temp_strs);
	strcat(oncall_doublehost_logsbuf.str[oncall_doublehost_logsbuf.a_ptr],strs_in);

	oncall_doublehost_logsbuf.a_ptr++;
	if(oncall_doublehost_logsbuf.a_ptr>=MAX_LOGNUM_PER_BUF)
		oncall_doublehost_logsbuf.a_ptr=0;
	printf("\n%s%s",temp_strs,strs_in);
}



void insert_gtk_logs(char * strs_in)
{
	char 	    temp_strs[30];
	struct tm 	*today;
	time_t		ltime;

	if(!strs_in)
		return;

	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(temp_strs,"%04d-%02d-%02d %02d:%02d:%02d ",today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,today->tm_min,today->tm_sec); 



	oncall_gtk_logsbuf.flag[oncall_gtk_logsbuf.a_ptr]=1;
	strcpy(oncall_gtk_logsbuf.str[oncall_gtk_logsbuf.a_ptr],temp_strs);
	strcat(oncall_gtk_logsbuf.str[oncall_gtk_logsbuf.a_ptr],strs_in);
	
	oncall_gtk_logsbuf.a_ptr++;
	if(oncall_gtk_logsbuf.a_ptr>=MAX_LOGNUM_PER_BUF)
		oncall_gtk_logsbuf.a_ptr=0;
	printf("\n%s%s",temp_strs,strs_in);
}

void insert_timingsend_logs(char * strs_in)//20120329
{
	char 	    temp_strs[30];
	struct tm 	*today;
	time_t		ltime;
	
	if(!strs_in)
		return;
	
	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(temp_strs,"%04d-%02d-%02d %02d:%02d:%02d ",today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,today->tm_min,today->tm_sec); 
	
	
	oncall_timingsend_logsbuf.flag[oncall_timingsend_logsbuf.a_ptr]=1;
	strcpy(oncall_timingsend_logsbuf.str[oncall_timingsend_logsbuf.a_ptr],temp_strs);
	strcat(oncall_timingsend_logsbuf.str[oncall_timingsend_logsbuf.a_ptr],strs_in);
	
	oncall_timingsend_logsbuf.a_ptr++;
	if(oncall_timingsend_logsbuf.a_ptr>=MAX_LOGNUM_PER_BUF)
		oncall_timingsend_logsbuf.a_ptr=0;
	printf("\n%s%s",temp_strs,strs_in);
}
void insert_manusend_logs(char * strs_in)//20120329
{
	char 	    temp_strs[30];
	struct tm 	*today;
	time_t		ltime;
	
	if(!strs_in)
		return;
	
	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(temp_strs,"%04d-%02d-%02d %02d:%02d:%02d ",today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,today->tm_min,today->tm_sec); 
	
	
	oncall_manusend_logsbuf.flag[oncall_manusend_logsbuf.a_ptr]=1;
	strcpy(oncall_manusend_logsbuf.str[oncall_manusend_logsbuf.a_ptr],temp_strs);
	strcat(oncall_manusend_logsbuf.str[oncall_manusend_logsbuf.a_ptr],strs_in);
	
	oncall_manusend_logsbuf.a_ptr++;
	if(oncall_manusend_logsbuf.a_ptr>=MAX_LOGNUM_PER_BUF)
		oncall_manusend_logsbuf.a_ptr=0;
	printf("\n%s%s",temp_strs,strs_in);
}


void insert_others_logs(char * strs_in)
{
	char 	    temp_strs[30];
	struct tm 	*today;
	time_t		ltime;

	if(!strs_in)
		return;

	ltime=time(NULL);
	today=localtime(&ltime);
	
	sprintf(temp_strs,"%04d-%02d-%02d %02d:%02d:%02d ",today->tm_year+1900,today->tm_mon+1,today->tm_mday,today->tm_hour,today->tm_min,today->tm_sec); 


	oncall_others_logsbuf.flag[oncall_others_logsbuf.a_ptr]=1;
 	strcpy(oncall_others_logsbuf.str[oncall_others_logsbuf.a_ptr],temp_strs);
	strcat(oncall_others_logsbuf.str[oncall_others_logsbuf.a_ptr],strs_in);

	oncall_others_logsbuf.a_ptr++;
	if(oncall_others_logsbuf.a_ptr>=MAX_LOGNUM_PER_BUF)
		oncall_others_logsbuf.a_ptr=0;
	printf("\n%s%s",temp_strs,strs_in);
}


void Oncall_almsearch()
{
	USHORT		rec_ptr;
	POINTER		point;
	char		namestr[POINT_NAME_SIZE];



	rec_ptr= shm_rec_addr->head.header_ptr;
	while(1)
	{
		if(threadstat_Oncall_almsearch>-1)
			threadstat_Oncall_almsearch++;
		while(rec_ptr != shm_rec_addr->head.header_ptr && readedxml==1)
		{
			if(threadstat_Oncall_almsearch>-1)
				threadstat_Oncall_almsearch++;
			if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_ANA_ALM || shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_IND_ALM)
			{
				if(-1==GetPointIdFromRec(&shm_rec_addr->rec[rec_ptr], &point))
				{
					char temp_strs[400];
					sprintf(temp_strs,"No this Point:%d.%d.%d.%d",point.stn_id,point.dev_id,point.data_type,point.pt_id);	
					insert_almsearch_logs(temp_strs);
					
					rec_ptr++;//20120213
					if(rec_ptr>=shm_rec_addr->head.buf_size)//20120213
						rec_ptr= 0;//20120213
					continue;
				}
				
				if(-1==GetPtNameById(&point, namestr, NULL))
				{	
					char temp_strs[400];
					sprintf(temp_strs,"Get Name By Point Err Point:%d.%d.%d.%d",point.stn_id,point.dev_id,point.data_type,point.pt_id);	
					insert_almsearch_logs(temp_strs);
					
					rec_ptr++;//20120213
					if(rec_ptr>=shm_rec_addr->head.buf_size)//20120210
						rec_ptr= 0;//20120213
					continue;//20120213
				}
				
				UpdateONCALLALMBUF(&shm_rec_addr->rec[rec_ptr],namestr);
			}
			rec_ptr++;
			if(rec_ptr>=shm_rec_addr->head.buf_size)
				rec_ptr= 0;
		}
		Sleep(2000);
	}
}


//20120213 for telalm
int	GettelalmCbaState(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag)
{
	char	ptNameStr[POINT_NAME_SIZE+12];
	char	ptNameStr1[POINT_NAME_SIZE+16];
	BOOL    can_alarm;
	
	USHORT  iogrp=0;
	POINT_NAME	ptName;
	
	if(-1==GetPtNameById(point, ptNameStr, NULL))
		return -1;
	if(-1==StrtoPointName((char *)(&ptNameStr), &ptName))
		return -1;
	
	can_alarm=TRUE;
	
	
	
	sprintf(ptNameStr1,"%s.%s..%s",ptName.stnName,ptName.grpName,OALM_DCBA_SUFFIX_DEFINE);	
	
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	
	GetPtIoGroupById(point, &iogrp);
	
	sprintf(ptNameStr1,"%s.%s_%d..%s",ptName.stnName,ptName.grpName,iogrp,OALM_GCBA_SUFFIX_DEFINE);
	
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	
	if(type_id==DPS_IND_ALM)
	{
		sprintf(ptNameStr1,"%s%s_%d",ptNameStr,OALM_CBA_SUFFIX_DEFINE,ind_rt_state);
	 
		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}
		
	}
	sprintf(ptNameStr1,"%s%s",ptNameStr,OALM_CBA_SUFFIX_DEFINE);
 
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	
	
	
	return can_alarm;
}
int igc_WSAStartup()
{
#ifdef	WINDOWS_OS
	WSADATA		WSAData;
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		return -1;
	}
	else
		return 0;
#endif
	return 0;
}
int igc_lan_out_init()
{
	if(!lan_out_init())
	{
		write_main_logs_now("init lanout err......,will not send fault_alm",1);
		return  -1;
	}
	else
		return 0;
}
void Oncall_writefie()//循环将各缓冲区的数据写入文件
{
	//char 	    FileName1[200],FileName2[200];//20120326 del
	struct tm 	*today;
	time_t		ltime;
	int         _ii,j,k,delflag;
	int         rdcfg;
	//FILE        * Oncallfile1,* Oncallfile2;//20120326 del
	long         writednum;

	
	//20120326 add start
	char 	    sms_logs_filename[200];
	char 	    tel_logs_filename[200];
    char 	    others_logs_filename[200];
	FILE    *file_sms_logs,*file_tel_logs,*file_others_logs;
	int fileopendflag;
	//20120326 add end

	struct   stat   deffile_statbuf;  //20120323 add start
	FILE   *lastmodTime_def;  
	char    FileName_def[200];
	time_t		last_mod_time_t;
	int should_Reread_deffile;
	int sleep_times;
	int lastday_of_date;//20120326
	sprintf(FileName_def,"%s",SMS_XML_FILE);
	last_mod_time_t=0;
	should_Reread_deffile=0;  //20120323 add end
	sleep_times=0;
	lastday_of_date=-1;//20120326
	delflag=0;
	Sleep(10000);//20120326

	while(1)
	{
		Sleep(2000);
		//20120323 add start
		should_Reread_deffile=0;
		if ((lastmodTime_def = fopen(FileName_def, "r")) == NULL)   
		{   
			printf("\nget define_file last Mod-time Err\n");
			write_main_logs_now("get define_file last Mod-time Err......",1);
			fclose(lastmodTime_def);
		}   
		else
		{ 
			stat(FileName_def,   &deffile_statbuf); 
		 
			if(last_mod_time_t==0)
			{
			 
			}
			else if(last_mod_time_t!=deffile_statbuf.st_mtime)
			{
				should_Reread_deffile=1;
				sleep_times=180;
			}
			last_mod_time_t=deffile_statbuf.st_mtime;
			fclose(lastmodTime_def);   
		}
		//20120323 add end
		
		Sleep(3000);
		//Sleep(30000);//20120323 del
		
		lock_oncall_sms_err_times=1;//20120327 add start
		Sleep(1000);
		for(j=0;j<MAX_sms_dev_num;j++)
		{
			if(oncall_sms_err_times[j]>6)
			{
				if(oncall_sms_err_times[j]<5000)
				{
					char temp_str[200]; 
					strcpy(_BAK_ONCALLCOM_PORTS[j],_ONCALLCOM_PORTS[j]);
					memset(_ONCALLCOM_PORTS[j],0,sizeof(_ONCALLCOM_PORTS[j]));
					oncall_sms_err_times[j]=10000;
					sprintf(temp_str,"sms dev err Num > 6,try to stop:%s",_BAK_ONCALLCOM_PORTS[j]); 
					printf("\n%s",temp_str);
					write_main_logs_now(temp_str,1);
					if(fault_alm_logicnames[0]!=0)
						ind_alm_by_logicalname(fault_alm_logicnames,1,0,-1,-1,1,1);
				}
				else
				{
					if(oncall_sms_err_times[j]>9900)
					{
						oncall_sms_err_times[j]--;
					}
					else
					{
						if(_BAK_ONCALLCOM_PORTS[j][0]!=0 && _ONCALLCOM_PORTS[j][0]==0)
						{
							char temp_str[200]; 
							strcpy(_ONCALLCOM_PORTS[j],_BAK_ONCALLCOM_PORTS[j]);
							oncall_sms_err_times[j]=0;
							sprintf(temp_str,"try to reuse sms dev:%s",_ONCALLCOM_PORTS[j]); 
							printf("\n%s",temp_str);
							write_main_logs_now(temp_str,1);
							//	ind_alm_by_logicalname(fault_alm_logicnames,0,0,-1,-1,1,1);
						}
					}
				}	
			}
		}	
		lock_oncall_sms_err_times=0;
		//20120327 add end

 		sleep_times++;
		if(sleep_times>1)
			sleep_times=0;
		else
			continue;

		if(threadstat_Oncall_almsearch>-1)//20120330 s
		{
			if(threadstat_Oncall_almsearch<1)
			{
				write_main_logs_now("threadstat_Oncall_almsearch may exit......",1);
				ind_alm_by_logicalname(fault_alm_logicnames,1,0,-1,-1,1,1);
			}
			threadstat_Oncall_almsearch=0;
		}
		if(threadstat_oncallTelQuery>-1)
		{
			if(threadstat_oncallTelQuery<1)//
			{
			//	write_main_logs_now("threadstat_oncallTelQuery may exit......",1);
			//	ind_alm_by_logicalname(fault_alm_logicnames,1,0,-1,-1,1,1);
			}
			threadstat_oncallTelQuery=0;
		}
		if(threadstat_Oncall_writefie>-1)
		{
			if(threadstat_Oncall_writefie<1)//
			{
			//	write_main_logs_now("threadstat_Oncall_writefie may exit......",1);
			//	ind_alm_by_logicalname(fault_alm_logicnames,1,0,-1,-1,1,1);
			}
			threadstat_Oncall_writefie=0;
		}
		if(threadstat_Oncall_ALM_manuconfirm>-1)
		{
			if(threadstat_Oncall_ALM_manuconfirm<1)//
			{
			//	write_main_logs_now("threadstat_Oncall_ALM_manuconfirm may exit......",1);
			//	ind_alm_by_logicalname(fault_alm_logicnames,1,0,-1,-1,1,1);
			}
			threadstat_Oncall_ALM_manuconfirm=0;
		}
		if(threadstat_Oncall_sms_timingsend>-1)
		{
			if(threadstat_Oncall_sms_timingsend<1)//
			{
				write_main_logs_now("threadstat_Oncall_sms_timingsend may exit......",1);
				ind_alm_by_logicalname(fault_alm_logicnames,1,0,-1,-1,1,1);
			}
			threadstat_Oncall_sms_timingsend=0;
		}
		if(threadstat_SMS_SENDRECEIVE>-1)
		{
			if(threadstat_SMS_SENDRECEIVE<1)//
			{
				write_main_logs_now("threadstat_SMS_SENDRECEIVE may exit......",1);
				ind_alm_by_logicalname(fault_alm_logicnames,1,0,-1,-1,1,1);
			}
			threadstat_SMS_SENDRECEIVE=0;
		}
		//20120330 e
	

		writednum=0;
		ltime=time(NULL);
		today=localtime(&ltime);

		if(lastday_of_date!=today->tm_mday)//20120326 add start
		{
			sprintf(sms_logs_filename,"%ssms_logs_%04d-%02d-%02d.txt",OUTDATA_RECORD_PATH,today->tm_year+1900,today->tm_mon+1,today->tm_mday); 
			sprintf(tel_logs_filename,"%stel_logs_%04d-%02d-%02d.txt",OUTDATA_RECORD_PATH,today->tm_year+1900,today->tm_mon+1,today->tm_mday); 
			sprintf(others_logs_filename,"%sothers_logs_%04d-%02d-%02d.txt",OUTDATA_RECORD_PATH,today->tm_year+1900,today->tm_mon+1,today->tm_mday); 
		}
		//20120326 add end
	
		
		//20120326 del sprintf(FileName1,"%sSMS_%04d-%02d-%02d.txt",OUTDATA_RECORD_PATH,today->tm_year+1900,today->tm_mon+1,today->tm_mday); 
		//20120326 del sprintf(FileName2,"%sSMS_%04d-%02d-%02d.txt",INDATA_RECORD_PATH,today->tm_year+1900,today->tm_mon+1,today->tm_mday); 

		if(today->tm_min>=23 && today->tm_min<=24 && today->tm_hour==1)//20120323 today->tm_hour==1)
		{
			should_Reread_deffile=1;//20120323
		}
		else
		{
			rdcfg=0;
		}

		if(should_Reread_deffile==1)//20120323 add
		{
			if(rdcfg<1)//20120323 move here
			{
				readedxml=0;
				k=0;
				if(ONCALLSMS_VALID)
					k++; 
				if(ONCALLDIALOGIC_VALID)
					k++;
				
				if(readOncallDefineXML()<1)
				{ 
					printf("\nRead ONcall configfile Err......");
					write_main_logs_now("Read ONcall configfile Err......",1);
					/*if(Oncallfile2=fopen(FileName2,"a"))
					{
					fprintf(Oncallfile2,"\nreadOncallDefineXML_ERR %02d-%02d-%02d %02d:%02d\n",today->tm_year,today->tm_mon,today->tm_mday,today->tm_hour,today->tm_min);
					fclose(Oncallfile2);
				}*/
				}
				else
				{
					printf("\nRead ONcall configfile Over......");
					write_main_logs_now("Read ONcall configfile Over......",1);
				}
				
				rdcfg=1;
				readedxml=1;
			}
		}


		if(today->tm_hour==1)
		{
			if(today->tm_min>=18 && today->tm_min<=24 && delflag==0)//防止重复删除文件
			{
				char temp_file_name_delete[212];//20120326 add start
				char temp_date_str[20];
				
				ltime = ltime - (60*60)*24*ONCAL_LOGSRECORD_SAVEDAY;
				today=localtime(&ltime);
				sprintf(temp_date_str,"%04d-%02d-%02d.txt",today->tm_year+1900,today->tm_mon+1,today->tm_mday);
			
				sprintf(temp_file_name_delete,"%ssms_logs_%s",OUTDATA_RECORD_PATH,temp_date_str); 
				k=_access(temp_file_name_delete,0);
				if( k!=-1 )
				{ 
					sprintf(temp_file_name_delete,"del %ssms_logs_%s",OUTDATA_RECORD_PATH,temp_date_str); 
			
					printf("\n%s\n",temp_file_name_delete);
					write_main_logs_now(temp_file_name_delete,1);
					system(temp_file_name_delete);   
				}
				
				
				sprintf(temp_file_name_delete,"%stel_logs_%s",OUTDATA_RECORD_PATH,temp_date_str); 
				k=_access(temp_file_name_delete,0);
				if( k!=-1 )
				{ 
					sprintf(temp_file_name_delete,"del %stel_logs_%s",OUTDATA_RECORD_PATH,temp_date_str); 
			
					printf("\n%s\n",temp_file_name_delete);
					write_main_logs_now(temp_file_name_delete,1);
					system(temp_file_name_delete);   
				}

				sprintf(temp_file_name_delete,"%sothers_logs_%s",OUTDATA_RECORD_PATH,temp_date_str); 
				k=_access(temp_file_name_delete,0);
				if( k!=-1 )
				{ 
					sprintf(temp_file_name_delete,"del %sothers_logs_%s",OUTDATA_RECORD_PATH,temp_date_str); 
			
					printf("\n%s\n",temp_file_name_delete);
					write_main_logs_now(temp_file_name_delete,1);
					system(temp_file_name_delete);   
				}


				sprintf(temp_file_name_delete,"%smain_logs_%s",OUTDATA_RECORD_PATH,temp_date_str); 
				k=_access(temp_file_name_delete,0);
				if( k!=-1 )
				{ 
					sprintf(temp_file_name_delete,"del %smain_logs_%s",OUTDATA_RECORD_PATH,temp_date_str); 
			
					printf("\n%s\n",temp_file_name_delete);
					write_main_logs_now(temp_file_name_delete,1);
					system(temp_file_name_delete);   
				}
				//20120326 add end
				/*k=_access(FileName1,0); 20120326 del
				if( k!=-1 )
				{ 
					printf("DIALALM.EXE: %s\n",FileName1); 
					system(FileName1);   
				}
				
				sprintf(FileName2,"del %sSMS_%04d-%02d-%02d.txt",INDATA_RECORD_PATH,today->tm_year+1900,today->tm_mon+1,today->tm_mday); 
				k=_access(FileName2,0);
				if( k!=-1 )
				{ 
					printf("DIALALM.EXE: %s\n",FileName1); 
					system(FileName2);   
				}*/
			}
			delflag=1;
		}
		else
		{
			delflag=0;
		}
		
		ltime=time(NULL);
		today=localtime(&ltime);


		
		// 20120326 add start

		
		fileopendflag=0;
		for(_ii=0;_ii<MAX_sms_dev_num;_ii++)
		{	
			if(_ONCALLCOM_PORTS[_ii][0]==0)
				continue;
			if(fileopendflag=-1)
				break;
			if(oncall_sms_logsbufs[_ii].w_ptr!=oncall_sms_logsbufs[_ii].a_ptr)
			{
				if(fileopendflag==0)
				{	
					if(file_sms_logs=fopen(sms_logs_filename,"a"))
					{
						fileopendflag=1;
					}
					else
					{
						fileopendflag=-1;
						write_main_logs_now("Fail to fopen sms_logs_filename",1);
						break;
					}
				}

				j=0;
				while(oncall_sms_logsbufs[_ii].w_ptr!=oncall_sms_logsbufs[_ii].a_ptr)
				{
				
					if(oncall_sms_logsbufs[_ii].flag[oncall_sms_logsbufs[_ii].w_ptr]!=1)
					{
						oncall_sms_logsbufs[_ii].w_ptr++;
						if(oncall_sms_logsbufs[_ii].w_ptr>=MAX_LOGNUM_PER_BUF)
							oncall_sms_logsbufs[_ii].w_ptr=0;
						j++;
						if(j>=MAX_LOGNUM_PER_BUF-1)
							break;
						continue;
					}
					if(oncall_sms_logsbufs[_ii].str[oncall_sms_logsbufs[_ii].w_ptr][0]==0)
					{
						oncall_sms_logsbufs[_ii].w_ptr++;
						if(oncall_sms_logsbufs[_ii].w_ptr>=MAX_LOGNUM_PER_BUF)
							oncall_sms_logsbufs[_ii].w_ptr=0;
						j++;
						if(j>=MAX_LOGNUM_PER_BUF-1)
							break;
						continue;
					}
					fprintf(file_sms_logs,"S%d:%s", _ii,oncall_sms_logsbufs[_ii].str[oncall_sms_logsbufs[_ii].w_ptr]);
					k=oncall_sms_logsbufs[_ii].w_ptr;
					oncall_sms_logsbufs[_ii].flag[k]=0;
					writednum++;
					
					oncall_sms_logsbufs[_ii].w_ptr++;
					if(oncall_sms_logsbufs[_ii].w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_sms_logsbufs[_ii].w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
				}
				if(fileopendflag=-1)
					break;
			}
		}
		if(fileopendflag==1)		 
			fclose(file_sms_logs);
		
		
		fileopendflag=0;
		for(_ii=0;_ii<MAXCHAN;_ii++)
		{	
			if(CHANUSE[_ii]!=1)
			continue;
		 
			if(fileopendflag=-1)
				break;
			if(oncall_tel_logsbufs[_ii].w_ptr!=oncall_tel_logsbufs[_ii].a_ptr)
			{
				if(fileopendflag==0)
				{	
					if(file_tel_logs=fopen(tel_logs_filename,"a"))
					{
						fileopendflag=1;
					}
					else
					{
						fileopendflag=-1;
						write_main_logs_now("Fail to fopen tel_logs_filename",1);
						break;
					}
				}
				j=0;
				while(oncall_tel_logsbufs[_ii].w_ptr!=oncall_tel_logsbufs[_ii].a_ptr)
				{
					
					if(oncall_tel_logsbufs[_ii].flag[oncall_tel_logsbufs[_ii].w_ptr]!=1)
					{
						oncall_tel_logsbufs[_ii].w_ptr++;
						if(oncall_tel_logsbufs[_ii].w_ptr>=MAX_LOGNUM_PER_BUF)
							oncall_tel_logsbufs[_ii].w_ptr=0;
						j++;
						if(j>=MAX_LOGNUM_PER_BUF-1)
							break;
						continue;
					}
					if(oncall_tel_logsbufs[_ii].str[oncall_tel_logsbufs[_ii].w_ptr][0]==0)
					{
						oncall_tel_logsbufs[_ii].w_ptr++;
						if(oncall_tel_logsbufs[_ii].w_ptr>=MAX_LOGNUM_PER_BUF)
							oncall_tel_logsbufs[_ii].w_ptr=0;
						j++;
						if(j>=MAX_LOGNUM_PER_BUF-1)
							break;
						continue;
					}
					fprintf(file_tel_logs,"S%d:%s", _ii,oncall_tel_logsbufs[_ii].str[oncall_tel_logsbufs[_ii].w_ptr]);
					k=oncall_tel_logsbufs[_ii].w_ptr;
					oncall_tel_logsbufs[_ii].flag[k]=0;
					writednum++;
					
					oncall_tel_logsbufs[_ii].w_ptr++;
					if(oncall_tel_logsbufs[_ii].w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_tel_logsbufs[_ii].w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
				}
				if(fileopendflag=-1)
					break;
			}
		}
		if(fileopendflag==1)		 
			fclose(file_tel_logs);

		


		fileopendflag=0;
		
		if(oncall_others_logsbuf.w_ptr!=oncall_others_logsbuf.a_ptr)
		{
			if(fileopendflag==0)
			{	
				if(file_others_logs=fopen(others_logs_filename,"a"))
				{
					fileopendflag=1;
				}
				else
				{
					fileopendflag=-1;
					write_main_logs_now("Fail to fopen others_logs_filename",1);
					break;
				}
			}
			j=0;
			while(oncall_others_logsbuf.w_ptr!=oncall_others_logsbuf.a_ptr)
			{
				if(oncall_others_logsbuf.flag[oncall_others_logsbuf.w_ptr]!=1)
				{
					oncall_others_logsbuf.w_ptr++;
					if(oncall_others_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_others_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_others_logsbuf.str[oncall_others_logsbuf.w_ptr][0]==0)
				{
					oncall_others_logsbuf.w_ptr++;
					if(oncall_others_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_others_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				fprintf(file_others_logs,"%s", oncall_others_logsbuf.str[oncall_others_logsbuf.w_ptr]);
				k=oncall_others_logsbuf.w_ptr;
				oncall_others_logsbuf.flag[k]=0;
				writednum++;
				
				oncall_others_logsbuf.w_ptr++;
				if(oncall_others_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_others_logsbuf.w_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
			if(fileopendflag=-1)
				break;
		}

		
	 
		
		if(oncall_timingsend_logsbuf.w_ptr!=oncall_timingsend_logsbuf.a_ptr)
		{
			if(fileopendflag==0)
			{	
				if(file_others_logs=fopen(others_logs_filename,"a"))
				{
					fileopendflag=1;
				}
				else
				{
					fileopendflag=-1;
					write_main_logs_now("Fail to fopen others_logs_filename",1);
					break;
				}
			}
			j=0;
			while(oncall_timingsend_logsbuf.w_ptr!=oncall_timingsend_logsbuf.a_ptr)
			{
				if(oncall_timingsend_logsbuf.flag[oncall_timingsend_logsbuf.w_ptr]!=1)
				{
					oncall_timingsend_logsbuf.w_ptr++;
					if(oncall_timingsend_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_timingsend_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_timingsend_logsbuf.str[oncall_timingsend_logsbuf.w_ptr][0]==0)
				{
					oncall_timingsend_logsbuf.w_ptr++;
					if(oncall_timingsend_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_timingsend_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				fprintf(file_others_logs,"%s", oncall_timingsend_logsbuf.str[oncall_timingsend_logsbuf.w_ptr]);
				k=oncall_timingsend_logsbuf.w_ptr;
				oncall_timingsend_logsbuf.flag[k]=0;
				writednum++;
				
				oncall_timingsend_logsbuf.w_ptr++;
				if(oncall_timingsend_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_timingsend_logsbuf.w_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
			if(fileopendflag=-1)
				break;
		}

 
		
		if(oncall_manusend_logsbuf.w_ptr!=oncall_manusend_logsbuf.a_ptr)
		{
			if(fileopendflag==0)
			{	
				if(file_others_logs=fopen(others_logs_filename,"a"))
				{
					fileopendflag=1;
				}
				else
				{
					fileopendflag=-1;
					write_main_logs_now("Fail to fopen others_logs_filename",1);
					break;
				}
			}
			j=0;
			while(oncall_manusend_logsbuf.w_ptr!=oncall_manusend_logsbuf.a_ptr)
			{
				if(oncall_manusend_logsbuf.flag[oncall_manusend_logsbuf.w_ptr]!=1)
				{
					oncall_manusend_logsbuf.w_ptr++;
					if(oncall_manusend_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_manusend_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_manusend_logsbuf.str[oncall_manusend_logsbuf.w_ptr][0]==0)
				{
					oncall_manusend_logsbuf.w_ptr++;
					if(oncall_manusend_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_manusend_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				fprintf(file_others_logs,"%s", oncall_manusend_logsbuf.str[oncall_manusend_logsbuf.w_ptr]);
				k=oncall_manusend_logsbuf.w_ptr;
				oncall_manusend_logsbuf.flag[k]=0;
				writednum++;
				
				oncall_manusend_logsbuf.w_ptr++;
				if(oncall_manusend_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_manusend_logsbuf.w_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
			if(fileopendflag=-1)
				break;
		}


		if(oncall_gtk_logsbuf.w_ptr!=oncall_gtk_logsbuf.a_ptr)
		{
			if(fileopendflag==0)
			{	
				if(file_others_logs=fopen(others_logs_filename,"a"))
				{
					fileopendflag=1;
				}
				else
				{
					fileopendflag=-1;
					write_main_logs_now("Fail to fopen others_logs_filename",1);
					break;
				}
			}
			j=0;
			while(oncall_gtk_logsbuf.w_ptr!=oncall_gtk_logsbuf.a_ptr)
			{
				if(oncall_gtk_logsbuf.flag[oncall_gtk_logsbuf.w_ptr]!=1)
				{
					oncall_gtk_logsbuf.w_ptr++;
					if(oncall_gtk_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_gtk_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_gtk_logsbuf.str[oncall_gtk_logsbuf.w_ptr][0]==0)
				{
					oncall_gtk_logsbuf.w_ptr++;
					if(oncall_gtk_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_gtk_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				fprintf(file_others_logs,"%s", oncall_gtk_logsbuf.str[oncall_gtk_logsbuf.w_ptr]);
				k=oncall_gtk_logsbuf.w_ptr;
				oncall_gtk_logsbuf.flag[k]=0;
				writednum++;
				
				oncall_gtk_logsbuf.w_ptr++;
				if(oncall_gtk_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_gtk_logsbuf.w_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
			if(fileopendflag=-1)
				break;
		}

		if(oncall_almsearch_logsbuf.w_ptr!=oncall_almsearch_logsbuf.a_ptr)
		{
			if(fileopendflag==0)
			{	
				if(file_others_logs=fopen(others_logs_filename,"a"))
				{
					fileopendflag=1;
				}
				else
				{
					fileopendflag=-1;
					write_main_logs_now("Fail to fopen others_logs_filename",1);
					break;
				}
			}
			j=0;
			while(oncall_almsearch_logsbuf.w_ptr!=oncall_almsearch_logsbuf.a_ptr)
			{
				if(oncall_almsearch_logsbuf.flag[oncall_almsearch_logsbuf.w_ptr]!=1)
				{
					oncall_almsearch_logsbuf.w_ptr++;
					if(oncall_almsearch_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_almsearch_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_almsearch_logsbuf.str[oncall_almsearch_logsbuf.w_ptr][0]==0)
				{
					oncall_almsearch_logsbuf.w_ptr++;
					if(oncall_almsearch_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_almsearch_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				fprintf(file_others_logs,"%s", oncall_almsearch_logsbuf.str[oncall_almsearch_logsbuf.w_ptr]);
				k=oncall_almsearch_logsbuf.w_ptr;
				oncall_almsearch_logsbuf.flag[k]=0;
				writednum++;
				
				oncall_almsearch_logsbuf.w_ptr++;
				if(oncall_almsearch_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_almsearch_logsbuf.w_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
			if(fileopendflag=-1)
				break;
		}

		if(oncall_doublehost_logsbuf.w_ptr!=oncall_doublehost_logsbuf.a_ptr)
		{
			if(fileopendflag==0)
			{	
				if(file_others_logs=fopen(others_logs_filename,"a"))
				{
					fileopendflag=1;
				}
				else
				{
					fileopendflag=-1;
					write_main_logs_now("Fail to fopen others_logs_filename",1);
					break;
				}
			}
			j=0;
			while(oncall_doublehost_logsbuf.w_ptr!=oncall_doublehost_logsbuf.a_ptr)
			{
				if(oncall_doublehost_logsbuf.flag[oncall_doublehost_logsbuf.w_ptr]!=1)
				{
					oncall_doublehost_logsbuf.w_ptr++;
					if(oncall_doublehost_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_doublehost_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_doublehost_logsbuf.str[oncall_doublehost_logsbuf.w_ptr][0]==0)
				{
					oncall_doublehost_logsbuf.w_ptr++;
					if(oncall_doublehost_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_doublehost_logsbuf.w_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				fprintf(file_others_logs,"%s", oncall_doublehost_logsbuf.str[oncall_doublehost_logsbuf.w_ptr]);
				k=oncall_doublehost_logsbuf.w_ptr;
				oncall_doublehost_logsbuf.flag[k]=0;
				
				writednum++;
				
				oncall_doublehost_logsbuf.w_ptr++;
				if(oncall_doublehost_logsbuf.w_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_doublehost_logsbuf.w_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
			if(fileopendflag=-1)
				break;
		}
		if(fileopendflag==1)		 
			fclose(file_others_logs);

		// 20120326 add end

		/* 20120326 del start
		Sleep(1000);
		//自动报警电话
		if(oncall_dial_autoalmsendbuf.writed_Ptr!=oncall_dial_autoalmsendbuf.send_Ptr)
		{
			if(Oncallfile1=fopen(FileName1,"a"))
			{
				for(i=oncall_dial_autoalmsendbuf.writed_Ptr;i!=oncall_dial_autoalmsendbuf.send_Ptr;i++)
				{
					fprintf(Oncallfile1,"SMSAUTOALMSEND:%s  %s  %s  %s", oncall_dial_autoalmsendbuf.time_str[i],oncall_dial_autoalmsendbuf.logicname[i],oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
					for(j=0;j<TELPHONE_NUM;j++)
					{
						if(oncall_dial_autoalmsendbuf.telnum[i][j][0]=='\0')continue;
						{
							fprintf(Oncallfile1,"%s;", oncall_dial_autoalmsendbuf.telnum[i][j]);
						}
					}
					fprintf(Oncallfile1,"\n");
					writednum++;
					oncall_dial_autoalmsendbuf.writed_Ptr++;
					if(oncall_dial_autoalmsendbuf.writed_Ptr>=ONCALL_DIAL_AUTOALMBUF_SIZE)
					{
						oncall_dial_autoalmsendbuf.writed_Ptr=0;
						break;
					}
				}
				
				fclose(Oncallfile1);
			}
		}
		
		Sleep(1000);
		//半自动报警电话
		if(oncall_dial_manualmsendbuf.writed_Ptr!=oncall_dial_manualmsendbuf.send_Ptr)
		{
			if(Oncallfile1=fopen(FileName1,"a"))
			{
				for(i=oncall_dial_manualmsendbuf.writed_Ptr;i!=oncall_dial_manualmsendbuf.send_Ptr;i++)
				{
					fprintf(Oncallfile1,"SMSAUTOALMSEND:CTP|%d  %s  %s  %s  %s", oncall_dial_manualmsendbuf.confirmed[i],oncall_dial_manualmsendbuf.time_str[i],oncall_dial_manualmsendbuf.logicname[i],oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
					for(j=0;j<TELPHONE_NUM;j++)
					{
						if(oncall_dial_manualmsendbuf.telnum[i][j][0]=='\0')continue;
						{
							fprintf(Oncallfile1,"%s;", oncall_dial_manualmsendbuf.telnum[i][j]);
						}
					}
					fprintf(Oncallfile1,"\n");
					writednum++;
					oncall_dial_manualmsendbuf.writed_Ptr++;
					if(oncall_dial_manualmsendbuf.writed_Ptr>=ONCALL_DIAL_MANUALMBUF_SIZE)
					{
						oncall_dial_manualmsendbuf.writed_Ptr=0;
						break;
					}
				}
				fclose(Oncallfile1);
			}
		}
		
		Sleep(1000);
		//自动报警短信
		if(oncall_sms_autoalmsendbuf.writed_Ptr!=oncall_sms_autoalmsendbuf.send_Ptr)
		{
			if(Oncallfile1=fopen(FileName1,"a"))
			{
				for(i=oncall_sms_autoalmsendbuf.writed_Ptr;i!=oncall_sms_autoalmsendbuf.send_Ptr;i++)
				{
					fprintf(Oncallfile1,"SMSAUTOALMSEND:%s  %s  %s  %s", oncall_sms_autoalmsendbuf.time_str[i],oncall_sms_autoalmsendbuf.logicname[i],oncall_sms_autoalmsendbuf.longname[i],oncall_sms_autoalmsendbuf.message[i]);
					for(j=0;j<TELPHONE_NUM;j++)
					{
						if(oncall_sms_autoalmsendbuf.telnum[i][j][0]=='\0')continue;
						{
							fprintf(Oncallfile1,"%s;", oncall_sms_autoalmsendbuf.telnum[i][j]);
						}
					}
					fprintf(Oncallfile1,"\n");
					writednum++;
					oncall_sms_autoalmsendbuf.writed_Ptr++;
					if(oncall_sms_autoalmsendbuf.writed_Ptr>=ONCALL_SMS_AUTOALMSENDBUF_SIZE)
					{
						oncall_sms_autoalmsendbuf.writed_Ptr=0;
						break;
					}
				}
				fclose(Oncallfile1);
			}
		}
		
		Sleep(1000);
		//半自动报警短信
		if(oncall_sms_manualmsendbuf.writed_Ptr!=oncall_sms_manualmsendbuf.send_Ptr)
		{
			if(Oncallfile1=fopen(FileName1,"a"))
			{
				for(i=oncall_sms_manualmsendbuf.writed_Ptr;i!=oncall_sms_manualmsendbuf.send_Ptr;i++)
				{
					fprintf(Oncallfile1,"SMSMANUALMSEND:CTP|%d  %s  %s  %s  %s", oncall_sms_manualmsendbuf.confirmed[i],oncall_sms_manualmsendbuf.time_str[i],
						oncall_sms_manualmsendbuf.logicname[i],oncall_sms_manualmsendbuf.longname[i],oncall_sms_manualmsendbuf.message[i]);
					for(j=0;j<TELPHONE_NUM;j++)
					{	
						if(oncall_sms_manualmsendbuf.telnum[i][j][0]=='\0')continue;
						{
							fprintf(Oncallfile1,"%s;", oncall_sms_manualmsendbuf.telnum[i][j]);
						}
					}
					fprintf(Oncallfile1,"\n");
					writednum++;
					oncall_sms_manualmsendbuf.writed_Ptr++;
					if(oncall_sms_manualmsendbuf.writed_Ptr>=ONCALL_SMS_MANUALMSENDBUF_SIZE)
					{
						oncall_sms_manualmsendbuf.writed_Ptr=0;
						break;
					}
				}
				fclose(Oncallfile1);
			}
		}
		
		Sleep(1000);
		//定时短信
		if(oncall_sms_timingsendbuf.writed_Ptr!=oncall_sms_timingsendbuf.send_Ptr)
		{
			if(Oncallfile1=fopen(FileName1,"a"))
			{
				for(i=oncall_sms_timingsendbuf.writed_Ptr;i!=oncall_sms_timingsendbuf.send_Ptr;i++)
				{
					for(j=0;j<TELPHONE_NUM;j++)
					{
						if(oncall_sms_timingsendbuf.sendlongnames[i][j][0]!=0)
							fprintf(Oncallfile1," %s ",oncall_sms_timingsendbuf.sendlongnames[i][j]);
						if(oncall_sms_timingsendbuf.logicnames[i][j][0]!=0)
							fprintf(Oncallfile1," %s ",oncall_sms_timingsendbuf.logicnames[i][j]);
					}
					fprintf(Oncallfile1," %s", oncall_sms_timingsendbuf.message[i]);
					for(j=0;j<TELPHONE_NUM;j++)
					{
						if(oncall_sms_timingsendbuf.telnum[i][j][0]=='\0')continue;
						{
							fprintf(Oncallfile1,"%s;", oncall_sms_timingsendbuf.telnum[i][j]);
						}
					}
					fprintf(Oncallfile1,"\n");
					writednum++;
					oncall_sms_timingsendbuf.writed_Ptr++;
					if(oncall_sms_timingsendbuf.writed_Ptr>=ONCALL_SMS_TIMINGSENDBUF_SIZE)
					{
						oncall_sms_timingsendbuf.writed_Ptr=0;
						break;
					}
				}
				fclose(Oncallfile1);
			}
		}
		
		Sleep(1000);
		//手动短信
		if(oncall_sms_manusendbuf.writed_Ptr!=oncall_sms_manusendbuf.send_Ptr)
		{
			if(Oncallfile1=fopen(FileName1,"a"))
			{
				for(i=oncall_sms_manusendbuf.writed_Ptr;i!=oncall_sms_manusendbuf.send_Ptr;i++)
				{
					fprintf(Oncallfile1,"SMSMANUSEND:%s  %s", oncall_sms_manusendbuf.time_str[i],oncall_sms_manusendbuf.message[i]);
					for(j=0;j<TELPHONE_NUM;j++)
					{
						if(oncall_sms_manusendbuf.telnum[i][j][0]=='\0')continue;
						{
							fprintf(Oncallfile1,"%s;", oncall_sms_manusendbuf.telnum[i][j]);
						}
					}
					fprintf(Oncallfile1,"\n");
					writednum++;
					oncall_sms_manusendbuf.writed_Ptr++;
					if(oncall_sms_manusendbuf.writed_Ptr>=ONCALL_SMS_MANUSENDBUF_SIZE)
					{
						oncall_sms_manusendbuf.writed_Ptr=0;
						break;
					}
				}
				fclose(Oncallfile1);
			}
		}
		
		Sleep(1000);
		//电话查询
		if(oncallL_dialquiry_writebuf.writed_Ptr!=oncallL_dialquiry_writebuf.add_Ptr)
		{
			if(Oncallfile2=fopen(FileName2,"a"))
			{
				for(i=oncallL_dialquiry_writebuf.writed_Ptr;i!=oncallL_dialquiry_writebuf.add_Ptr;i++)
				{
					fprintf(Oncallfile2,"SMSQUIRYSEND:%s  %s \n", oncallL_dialquiry_writebuf.time_str[i],oncallL_dialquiry_writebuf.message[i]);
					writednum++;
					oncallL_dialquiry_writebuf.writed_Ptr++;
					if(oncallL_dialquiry_writebuf.writed_Ptr>=MAX_WRITEFILEBUFNUM)
					{
						oncallL_dialquiry_writebuf.writed_Ptr=0;
						break;
					}
				}
				fclose(Oncallfile2);
			}
		}
		
		Sleep(1000);
		//短信查询
		if(oncallL_smsquiry_writebuf.writed_Ptr!=oncallL_smsquiry_writebuf.add_Ptr)
		{
			if(Oncallfile2=fopen(FileName2,"a"))
			{
				for(i=oncallL_smsquiry_writebuf.writed_Ptr;i!=oncallL_smsquiry_writebuf.add_Ptr;i++)
				{
					fprintf(Oncallfile2,"SEND:%s  %s \n", oncallL_smsquiry_writebuf.time_str[i],oncallL_smsquiry_writebuf.message[i]);
					writednum++;
					oncallL_smsquiry_writebuf.writed_Ptr++;
					if(oncallL_smsquiry_writebuf.writed_Ptr>=MAX_WRITEFILEBUFNUM)
					{
						oncallL_smsquiry_writebuf.writed_Ptr=0;
						break;
					}
				}
				fclose(Oncallfile2);
			}
		} 20120326 del end
		*/ 

		if(writednum>0)
		{
			char temp_write_num_str[200];//20120326 add
			sprintf(temp_write_num_str,"oncall write log Num:%d",writednum); //20120326 add
			printf("\noncall write log Num:%d",writednum);
			write_main_logs_now(temp_write_num_str,1);//20120326 add
		}
	}
}

int ind_alm_by_logicalname(char * logicalname,int alm_status,int stand_by,int MAN_SET,int CHAN_FAIL,int alarm_type,int sendsms_falg)
{
	UCHAR istnid,idevid,itypeid;
	unsigned short istartid,iendid;
	POINTER  point;
	unsigned short status;
	char sendbuf[200];
	if(alm_status<0)
		alm_status=0;
	if(alm_status>15)
		alm_status=15;
	
	status=(unsigned short)alm_status;
	
	
	
	/*	if(DATA_QUALITY>=0 && DATA_QUALITY<=1)
	status=status | (DATA_QUALITY*0x2000);*/
	
	if(MAN_SET>=0 && MAN_SET<=1)
		status=status | (MAN_SET*0x4000);
	
	if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status=status | (CHAN_FAIL*0x8000);
	
	
	if(alarm_type==2)
		status = status + 512;
	else if(alarm_type==3)
		status = status + 768;
	else
		status = status + 256;
	
		/*	if(MAN_SET>=0 && MAN_SET<=1)
		status = status + 512;
		else if(CHAN_FAIL>=0 && CHAN_FAIL<=1)
		status = status + 768;
		else 
		status = status + 256;
	*/
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{	
		sprintf(sendbuf,"IGCALM_ERR,NO POINT %s",logicalname);
		BdSms(sendbuf);
		return -1;
	}
	
	istnid=(UCHAR)point.stn_id;
	idevid=(UCHAR)point.dev_id;
	itypeid=(UCHAR)point.data_type;
	istartid=(unsigned short)point.pt_id;
	iendid=istartid;
	
	if(itypeid !=SOE_TYPE &&  itypeid !=POL_TYPE && itypeid !=OUTS_TYPE && itypeid !=SYSMSG_TYPE)
	{
		printf("\nind_alm_by_logicalname,type ERR\n");
		return -1;
		
	}
	
	MultiPointPack_ALM((UCHAR)4,istnid,idevid,itypeid,istartid,iendid,(unsigned short)status,(int)status,1,sendsms_falg);
	return 1;
}


void MultiPointPack_ALM(UCHAR type_id,UCHAR stn,UCHAR dev,UCHAR type,unsigned short first,unsigned short end,unsigned short status,int data,int almfag,int smsfalg)
{
	LAN_HEAD	head;
	COMMON_MSG  msg;
	short		num;
	DATE_TIME	devtime;
	unsigned short ii=0;
	char 		buf[MAX_BUF_SIZE]; 
	

	//H9000_IGC_data tempigcdt;
 

	memset(buf,0,MAX_BUF_SIZE);

	almfag=1;////
	if(almfag==1)
	{
		sprintf(buf,"TEL_ALM %d.%d.%d.%d N=%d",stn,dev,type,first,end-first+1);  
	}
	else
	{
	
	}

	if(smsfalg==1)
		BdSms(buf);

	head.dest_id=0;
	GetSrcId(&head.src_id);
	head.dp_type=BJ_DB_TIME;
	
	memset(buf,0,MAX_BUF_SIZE);
	
	GetSysTime(&devtime);
	
	ii= first;
	for(;ii<=end;)
	{
		msg.type_id = type_id;
		msg.stn_id = stn;	
		msg.dev_id = dev;
		msg.data_type=type;
		GetSrcId(&msg.host_id);
		
		msg.ms  =devtime.ms;
		msg.sec  =devtime.second;
		msg.min  =devtime.minute;
		msg.hour  =devtime.hour;
		msg.day  =devtime.day;
		msg.month  =devtime.month;
		msg.year  =devtime.year; 
		
		head.length=6;
		num=0;
		for(;ii<=end;ii++,num++)
		{
			if(head.length>=(MAX_BUF_SIZE-18))
			{
				printf(" length >MAX_BUF_SIZE\n");
				break;
			}		
			
			memcpy(buf+head.length,&ii,2);
			memcpy(buf+head.length+2,&status,2);
			Swap2Byte(&buf[head.length]);
			Swap2Byte(&buf[head.length+2]);
			
			if(msg.type_id == DPS_IND_ALM)
			{
				head.length=head.length+4;
			}
			else
			{	
				memcpy(buf+head.length+4,&data,4);
				Swap4Byte(&buf[head.length+4]);
				head.length=head.length+8;
			}
		}	
		
		msg.number=num;
	
		
		Swap2Byte(&msg.number);	
		Swap2Byte(&msg.ms);
		Swap2Byte(&msg.year);
		
		memcpy(buf,&msg,6);
		if(type_id !=  DPS_IND_ALM)
		{
			memcpy(buf+head.length,&msg.host_id, 8);	
			head.length=head.length+8;
		}
		else
		{	
			memcpy(buf+head.length,&msg.ms, 10);
			head.length=head.length+10;
		}

		Swap2Byte(&head.length);
		head.dp_type=BJ_DB_TIME; 
		lan_out((char *)buf,head,0);
		
	}
	return;
}



void Oncall_ALM_manuconfirm()
{
	while(1)//报警手动确认
	{
		Sleep(3000);
	}
}
