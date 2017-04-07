#include "/home/ems/system/telalm/v4.0a/inc/tele.h"

extern	int  get1,get2;
extern	void FilterAudStr(char *audStr);

int		TID_pd[20],TID_px[20];
int     dnum, dxn[MAXCHAN];
char    line_input[MAXCHAN][128];
char    user_hookon[MAXCHAN];

typedef  struct {
    unsigned char strRiff[4];       // 00H 4      char        "RIFF"标志
	unsigned long   lngFileLen;     // 04H 4      long int    文件长度
	unsigned char strWave[4];       // 08H 4      char        "WAVE"标志
	unsigned char strFmt[4];        // 0CH 4      char        "fmt"标志
	unsigned long lngTmp;           // 10H 4      过渡字节（不定）
	unsigned short int intFormat;   // 14H 2      int    格式类别（10H为PCM形式的声音数据)
	unsigned short int intChan;     // 16H 2      int    通道数，单声道为1，双声道为2
	unsigned short int intFreq;     // 18H 3      int    采样率（每秒样本数），表示每个通道的播放速度，
	unsigned long    lngSendSpeed;  // 1CH 3      long int   波形音频数据传送速率，其值为通道数×每秒数据位数×每样本的数据位数／8。播放软件利用此值可以估计缓冲区的大小。
	unsigned short int intBlock;    // 20H 2      int    数据块的调整数（按字节算的），其值为通道数×每样本的数据位值／8。播放软件需要一次处理多个该值大小的字节数据，以便将其值用于缓冲区的调整。
	unsigned short int intBit;      // 22H 2      每样本的数据位数，表示每个声道中各个样本的数据位数。如果有多个声道，对每个声道而言，样本大小都一样。
	unsigned char strData[4];       // 24H 4      char    数据标记符＂data＂
	unsigned long   lngDataLenth;   // 28H 4      long int 语音数据的长度 
}wavhead;

int mygjytest(unsigned long event_handle)
{
	int chdev,i,index;
	DX_CST *cstp;
	
	chdev=0;
    chdev = sr_getevtdev(event_handle);
	cstp = (DX_CST*)sr_getevtdatap(event_handle);
	index=-1;

	for(i=CHANSTARTNUM;i<CHANNUM+CHANSTARTNUM;i++)
	{
		if(CHANUSE[i]!=1)continue;
		if(dev[i].vox==chdev)
		{
			index=i;
			break;
		}
	}
	
	if(index<0) return -2;
	printf("\ncst_event=%d,cst_data=%d,%s, hookontimes=%d",cstp->cst_event,cstp->cst_data,dev[index].voxname,user_hookon[index]);
	if(cstp->cst_data==DLDST_DISCTONE
		|| cstp->cst_data==DLDDT_DISCTONE
		|| cstp->cst_data==DLDSTCAD_DISCTONE
		|| cstp->cst_data==DLDDTCAD_DISCTONE)
	{
		
		switch(cstp->cst_event)
		{
		case DE_TONEON:
			if(user_hookon[index]<0)
				user_hookon[index]=0;
			else if(user_hookon[index]<HOOKONBUSYTONETIMES)
				user_hookon[index]++;
			else
				user_hookon[index]=HOOKONBUSYTONETIMES;
			break;
		case DE_RINGS:
			//SetEvent(hRingEvent[index]);
			break;
		default:
			break;
			
		}
		return -1;
	}

	return 0;
}
void joinwavfile(char * file1nm,char * file2nm,char * fileOutnm)
{
	wavhead head1,head2,head3;
	unsigned int temp;
	long j=0, in1length=0, in2length=0;
	FILE *fin1,*fin2,*fout;
	fin1 = fopen(file1nm, "rb" );
	fin2 = fopen(file2nm, "rb" );
	fout = fopen(fileOutnm, "wb" );
	fseek(fin1,0,SEEK_SET);
	fread(&head1, sizeof(head1), 1, fin1);//读取文件头
	fread(&head2, sizeof(head2), 1, fin2);
	head3=head1;//将文件一的头复制给文件三
	head3.lngFileLen = head2.lngFileLen + head1.lngFileLen - 44; //修改文三一的文件头中的文件长度
	head3.lngDataLenth = head2.lngDataLenth + head1.lngDataLenth; //修改文件三文件头中的数据长度
	fwrite(&head3,sizeof(head3), 1, fout);//将文件头写入文件中
	fseek(fin2, sizeof(head1), SEEK_SET);
	fseek(fout, sizeof(head1), SEEK_SET);
	fread(&temp, 4, 1, fin2);
	while(!feof(fin2))//复制第一个文件
	{
		fwrite(&temp, 4, 1, fout);
		fread(&temp, 4, 1, fin2);
	}
//	printf("first file copy Complete!");
	fseek(fin1,sizeof(head1),SEEK_SET);//定位到数据区开始
	fseek(fout,sizeof(head1)+head2.lngDataLenth,SEEK_SET);//定位到末尾
	fread(&temp,4,1,fin1);//读取
	while(!feof(fin1))//复制第二个文件
	{
		fwrite(&temp,4,1,fout);
		fread(&temp,4,1,fin1);
	}
	fclose(fout);
	fclose(fin2);
	fclose(fin1);
}
void copywavfile(char * file1nm,char * file2nm)
{
	unsigned int temp;
	long j=0, in1length=0, in2length=0;
	FILE *fin1,*fin2;
	fin1 = fopen(file1nm, "rb" );
	fin2 = fopen(file2nm, "wb" );
	fseek(fin1,0,SEEK_SET);
	while(!feof(fin1))
	{
		fread(&temp,4,1,fin1);
		fwrite(&temp,4,1,fin2);
	}
	fclose(fin2);
	fclose(fin1);
}

void oncallTelQuery()    
{        
	//#ifdef DIALOGIC_VALID
    int   i;
    int   ringcount = 6;
	int	  line_no;
	for (i=CHANSTARTNUM;i<CHANNUM+CHANSTARTNUM;i++)
	{
		if(CHANUSE[i]!=1)continue;
		dxn[i]=-1;
	}
	
	for (line_no=CHANSTARTNUM;line_no<CHANNUM+CHANSTARTNUM;line_no++)
	{ 
		if(CHANUSE[line_no]!=1)continue;
		strcpy(dev[line_no].voxname,voxchannels[line_no]);
		if ((dev[line_no].vox = dx_open(dev[line_no].voxname,0)) == -1 )
		{  
			char temp_strs[400];
			sprintf(temp_strs,"Error openning channel %s ERRNO=%d",dev[line_no].voxname,dx_fileerrno());	
			write_main_logs_now(temp_strs,1);
			//printf("\nOnCall:%s",temp_strs);

			//printf("Error openning channel %s ERRNO=%d\n",dev[line_no].voxname,dx_fileerrno()); 
			//writelognow("Error openning channel",1);
			exit(1);
		}
		dx_sethook(dev[line_no].vox, DX_ONHOOK, EV_SYNC);
	}

	if(1==1)
	{
		char temp_strs[400];
		sprintf(temp_strs," TelQueryDial():Waiting for telephone alarm and inquiry");	
		insert_tel_logs(0,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
	}
	//printf("\nTelQueryDial() : ===Waiting for telephone alarm and inquiry===\n");
	
	for(line_no=CHANSTARTNUM;line_no<CHANNUM+CHANSTARTNUM;line_no++) 
	{
		if(CHANUSE[line_no]!=1)continue;
		initialize_gtd(dev[line_no].vox);
	}
	
	for(line_no=CHANSTARTNUM;line_no<CHANNUM+CHANSTARTNUM;line_no++)
	{
		if(CHANUSE[line_no]!=1)continue;
		if (dx_setevtmsk(dev[line_no].vox,DM_RINGS|DM_LCOF) == -1)
		{ 
			char temp_strs[400];
			sprintf(temp_strs,"ERROR: on sr_setevtmsk() on device %s",ATDV_ERRMSGP(dev[line_no].vox));	
			insert_tel_logs(line_no,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			sprintf(temp_strs,"ERROR: is 0x%x %s",ATDV_LASTERR(dev[line_no].vox), ATDV_ERRMSGP(dev[line_no].vox));	
			insert_tel_logs(line_no,temp_strs);
			//printf("\nOnCall:%s",temp_strs);


			//wait for CST event(DM_RINGS,DM_RNGOFF...)  
			//printf("ERROR: on sr_setevtmsk() on device %s\n",ATDV_ERRMSGP(dev[line_no].vox));
			//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(dev[line_no].vox), ATDV_ERRMSGP(dev[line_no].vox));
			//writelognow("ERROR: on sr_setevtmsk() on device",1);
		}  
		if (dx_setparm(dev[line_no].vox, DXCH_RINGCNT,(void *)&ringcount) == -1)
		{
			char temp_strs[400];
			sprintf(temp_strs,"ERROR:on sr_setparm() on device %s",ATDV_ERRMSGP(dev[line_no].vox));	
			insert_tel_logs(line_no,temp_strs);
			//printf("\nOnCall:%s",temp_strs);
			
			sprintf(temp_strs,"ERROR: is 0x%x %s",ATDV_LASTERR(dev[line_no].vox), ATDV_ERRMSGP(dev[line_no].vox));	
			insert_tel_logs(line_no,temp_strs);
			//printf("\nOnCall:%s",temp_strs);


			// process error
			//printf("ERROR: on sr_setparm() on device %s\n",ATDV_ERRMSGP(dev[line_no].vox));
			//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(dev[line_no].vox), ATDV_ERRMSGP(dev[line_no].vox));	
			//writelognow("ERROR: on sr_setparm() on device ",1);
		}  	
	}

	if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)oncallTelDial,NULL,0,&ONCALLTelDial_IDThread)==NULL) 					
	{
		char temp_strs[400];							
		sprintf(temp_strs,"Create oncallTelQuery Thread() error");	
		write_main_logs_now(temp_strs,1);
		//printf("\nOnCall:%s",temp_strs);
								
		//printf("Create oncallTelQuery Thread() error!\n");
		//writelognow("Create oncallTelQuery Thread()  ERR",1);
		exit(-1);
	}
	else
	{
		char temp_strs[400];							
		sprintf(temp_strs,"Thread oncallTelDial created ,index=%d",line_no);	
		insert_tel_logs(0,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
		
	 
	}
	//printf("\nONCALL: Thread oncallTelDial created!"); 	
	
	//for(;;)
	//{  
	//if(readedxml==1)
	//{
	//telusexmlbufoverR=0;//不能读取配置文件
	for (line_no=CHANSTARTNUM;line_no<CHANNUM+CHANSTARTNUM;line_no++)
	{   
		if(CHANUSE[line_no]!=1)continue;
		//if (dxn[line_no]==-1)
		//{  
		//dxn[line_no]=1;//20110130
		//	dx_sethook(dev[line_no].vox, DX_ONHOOK, EV_SYNC);
		//dxn[line_no] = dx_wtring(dev[line_no].vox,1,DX_OFFHOOK,0);
		//if (dxn[line_no] == 0)
		//{  
		//printf("\n### Inquirying phone is dialing line %d %s ### \n", line_no, dev[line_no].voxname);
		dx_sethook(dev[line_no].vox, DX_ONHOOK, EV_SYNC);//2011 0209
		if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReQuery,(LPVOID)line_no,0,&TID_pd[line_no])==NULL)						
		{ 
			char temp_strs[400];							
			sprintf(temp_strs,"Error in Create ReQueryThread,index=%d",line_no);	
			write_main_logs_now(temp_strs,1);
			//printf("\nOnCall:%s",temp_strs);

		//	printf("Error in Create ReQueryThread TID_pd[%02d]!\n",line_no); 
			//writelognow("Error in Create TelDialThread TID_px[%02d] ",1);
			exit(-1);
		}
	    	   
		Sleep(50); 
		//continue;
	}
	//telusexmlbufoverR=1;
	//}
	//else
	//{
	//	telusexmlbufoverR=1;
	//			printf("\n!!!!配置文件未读取完成，系统暂无法处理电话查询!!!!\n");
	//	Sleep(2000);//供检测
	//}
	//Sleep(200);
	//	}   
	//#endif
}   

void oncallTelDial()
{   
	int   i,Ah,Mh;
	int   outpa[4];
	for(;;)
	{ 
		if(readedxml!=1)
		{
			char temp_strs[400];							
			sprintf(temp_strs,"配置文件未读取完成，系统暂无法处理电话查询");	
			insert_tel_logs(0,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			telusexmlbufoverA=1;
//			printf("\n!!!!配置文件未读取完成，系统暂无法处理电话查询!!!!\n");
			Sleep(3000);//供检测
			continue;
		}
		telusexmlbufoverA=0;
		if(oncall_dial_autoalmsendbuf.send_Ptr!=oncall_dial_autoalmsendbuf.add_Ptr)
		{ 
			dnum=0;
			for(i=oncall_dial_autoalmsendbuf.send_Ptr;i!=oncall_dial_autoalmsendbuf.add_Ptr;)
			{ 
				Ah=0;
				for (dnum=CHANSTARTNUM;dnum<CHANNUM+CHANSTARTNUM;dnum++)
				{ 
					if(CHANUSE[dnum]!=1)continue;
					if (dxn[dnum]==-1)
					{
						Sleep(50);//20110131
						if (dxn[dnum]!=-1)//20110131 
							continue;//20110131
						dxn[dnum]=1;
						outpa[0]=dnum;
						outpa[1]=i;
						if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)autoalmTelDial,outpa,0,&TID_px[dnum])==NULL)						
						{ 
							char temp_strs[400];							
							sprintf(temp_strs,"Error in Create TelDialThread,index=%d",dnum);	
							write_main_logs_now(temp_strs,1);
							//printf("\nOnCall:%s",temp_strs);

							//printf("Error in Create TelDialThread TID_px[%02d]!\n",dnum); 
							//writelognow("Error in Create TelDialThread TID_px ",1);
							exit(-1); 
						}
						Ah=1;
						Sleep(200);
						break;
					} 
					Sleep(200);
				}
				if(Ah==0)//20110127
				{
					Sleep(200);
					break;
				}
				i++;
				if(i>ONCALL_DIAL_AUTOALMBUF_SIZE-1) i=0;
				oncall_dial_autoalmsendbuf.send_Ptr++;
				if(oncall_dial_autoalmsendbuf.send_Ptr>=ONCALL_DIAL_AUTOALMBUF_SIZE)
				{
					oncall_dial_autoalmsendbuf.send_Ptr=0;
					Sleep(200);//防止死循环
					break;
				}
			}
		} 
		
		Sleep(500);
		
		for(i=oncall_dial_manualmsendbuf.send_Ptr;i!=oncall_dial_manualmsendbuf.add_Ptr;i++)
		{    	
			if(i>ONCALL_DIAL_MANUALMBUF_SIZE-1) i=0;
			if(oncall_dial_manualmsendbuf.canceled==1)
			{
				oncall_dial_manualmsendbuf.canceled=0;
				break;
			}
			if(oncall_dial_manualmsendbuf.confirmed[i]==2)
			{
				oncall_dial_manualmsendbuf.send_Ptr++;
				if(oncall_dial_manualmsendbuf.send_Ptr>=ONCALL_DIAL_MANUALMBUF_SIZE)
				{
					oncall_dial_manualmsendbuf.send_Ptr=0;
					Sleep(200);//防止死循环
					break;
				}
				oncall_dial_manualmsendbuf.confirmed[i]=0;
				continue;
			}
			if(oncall_dial_manualmsendbuf.confirmed[i]!=1)break;//未确认，取消执行

			Mh=0;
			for (dnum=CHANSTARTNUM;dnum<CHANNUM+CHANSTARTNUM;dnum++)
			{ 
				if(CHANUSE[dnum]!=1)continue;
				if (dxn[dnum]==-1)
				{
					dxn[dnum]=1;
					outpa[0]=dnum;
					outpa[1]=i;
					if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)manualmTelDial,outpa,0,&TID_px[dnum])==NULL)						
					{ 
						char temp_strs[400];							
						sprintf(temp_strs,"Error in Create TelDialThread,index=%d",dnum);	
						write_main_logs_now(temp_strs,1);
						//printf("\nOnCall:%s",temp_strs);

						//printf("Error in Create TelDialThread TID_px[%02d]!\n",dnum); 
						//writelognow("Error in Create TelDialThread TID_px ",1);
						exit(-1); 
					}
					Mh=1;
					Sleep(200);
					break;
				} 
				Sleep(200);
			}
			
			if(Mh==0)//20110127
			{
				Sleep(200);
				break;
			}

			oncall_dial_manualmsendbuf.confirmed[i]=0;
			oncall_dial_manualmsendbuf.send_Ptr++;
			if(oncall_dial_manualmsendbuf.send_Ptr>=ONCALL_DIAL_MANUALMBUF_SIZE)
			{
				oncall_dial_manualmsendbuf.send_Ptr=0;
				Sleep(200);//防止死循环
				break;
			}
			
		}
		telusexmlbufoverA=1;
	}
}
float AnaDataToFloat( DB_DATA db )
{ 
	float value;
	switch( (db.status & 0x60) >> 5 )
	{ 
	case 0:   
		value = (float)(db.value/1e0); break;
	case 1:   
		value = (float)(db.value/1e1); break;
	case 2:  
		value = (float)(db.value/1e2); break;
	case 3:   
		value = (float)(db.value/1e3); break;
	default:  
		value = (float)(db.value/1e0); break;
	}
	if((db.status & 0x80) !=0 )
		value = value*( (float)-1 );
	return( value );
}
void replace_hzfh_str(char * str_in)
{
	char	*hz_dh= "，";
	char	*hz_Jh= "。";
	char	*hz_dunh= "、";
	char	*hz_th= "！";
	char	en_dh= ',';
	char    * finestr;

	finestr=str_in;
	while(*finestr)
	{
		if(*finestr==*hz_dh && *(finestr+1)==*(hz_dh+1))
		{
			*finestr=en_dh;
			*(finestr+1)=en_dh;
			finestr +=2;
		}
		else if(*finestr==*hz_Jh && *(finestr+1)==*(hz_Jh+1))
		{
			*finestr=en_dh;
			*(finestr+1)=en_dh;
			finestr +=2;
		}
		else if(*finestr==*hz_dunh && *(finestr+1)==*(hz_dunh+1))
		{
			*finestr=en_dh;
			*(finestr+1)=en_dh;
			finestr +=2;
		}
		else if(*finestr==*hz_th && *(finestr+1)==*(hz_th+1))
		{
			*finestr=en_dh;
			*(finestr+1)=en_dh;
			finestr +=2;
		}
		else
		{
			if(*finestr)
			{
				finestr++;
				continue;
			}
			break;
		}
	}
}

int tel_playwav_msg (int xh,int chdev, char *msg)//电话放音，音(file.wav)在path里
{
	//#ifdef	 DIALOGIC_VALID
	char    *dvl,*dvlC; 
	DV_TPT tpt1[2];
	DX_IOTT iott;
	DX_XPB  xpb;
	char	cmd[512];  
	char    Amsg[1024];  
	char    wvmsg[200];
	char    lastflname[128];
	char    newflname[128];
	char    outflname[128];
	int i,j;


	i=0;
	dvlC=",";
	memset(lastflname,0,sizeof(lastflname));
	memset(newflname,0,sizeof(newflname));
	memset(outflname,0,sizeof(outflname));
	memset(Amsg,0,sizeof(Amsg));
	strcpy(Amsg,msg);

	replace_hzfh_str(Amsg);//20111011 add
	FilterAudStr(Amsg);//20111011 add

	dvl=strtok(Amsg,dvlC);

	sprintf(outflname,"\\语音合成\\h9000wav\\BCCXSH9SOCWVTST%d_out.wav",chdev);
	while(dvl)
	{
		memset(wvmsg,0,sizeof(wvmsg));//20111011 add
			
		for(j=0;j<200;j++)
		{
			wvmsg[j]=dvl[j];
			if(dvl[j]==0)
				break;
		}
		if(wvmsg[199]!=0)//20111011 chg
			wvmsg[199]=0;
		sprintf(newflname,"\\语音合成\\h9000wav\\BCCXSH9SOCWVTST%d_C%d.wav",chdev,i);
		sprintf(cmd,"\\语音合成\\bin\\cwav.exe 1 0 %s %s",newflname,wvmsg); 
		system(cmd);
	
		if(i>0)
		{
			copywavfile(outflname,lastflname);
			joinwavfile(newflname,lastflname,outflname);
		}
		else
			copywavfile(newflname,outflname);

		strcpy(lastflname,newflname);
	
		i++;
		if(i>9)
			break;	
		dvl=strtok(NULL,dvlC);	
		Sleep(100);
	}

	if((iott.io_fhandle = dx_fileopen(outflname, O_RDONLY|O_BINARY)) == -1) 
	{
		char temp_strs[400];							
		sprintf(temp_strs,"Error opening wav file ,index=%d",xh);	
		insert_tel_logs(xh,temp_strs);
		//printf("\nOnCall:%s",temp_strs);

//		printf(	"Error opening wav file\n");
		return( (int)-1 );
	}
	iott.io_type = IO_DEV|IO_EOT; 
	iott.io_bufp = 0;
	iott.io_offset = 0;
	iott.io_length = -1;
	
	dx_clrtpt(tpt1,sizeof(tpt1)/sizeof(DV_TPT));
	tpt1[0].tp_type = IO_CONT; 
	tpt1[0].tp_termno = DX_DIGMASK; 
	tpt1[0].tp_length = 0x1000; 
	tpt1[0].tp_flags = TF_DIGMASK; 
	tpt1[1].tp_type = IO_EOT;
	tpt1[1].tp_termno = DX_MAXDTMF;
	tpt1[1].tp_length = 4;
	tpt1[1].tp_flags = TF_MAXDTMF;
	
	xpb.wFileFormat = FILE_FORMAT_VOX;
	xpb.wDataFormat = DATA_FORMAT_PCM;
	xpb.nSamplesPerSec = DRT_8KHZ;
	xpb.wBitsPerSample = 8;
	
	if (dx_clrdigbuf(chdev) == -1) 
	{
		char temp_strs[400];							
		sprintf(temp_strs,"Error clear dx_clrdigbuf,index=%d",xh);	
		insert_tel_logs(xh,temp_strs);
		//printf("\nOnCall:%s",temp_strs);


		//printf("clear error\n");
	}
	if (dx_playiottdata(chdev,&iott,tpt1,&xpb,EV_SYNC) < 0) 
	{
		char temp_strs[400];							
		sprintf(temp_strs,"playwav failed,index=%d",xh);	
		insert_tel_logs(xh,temp_strs);
		//printf("\nOnCall:%s",temp_strs);

		dx_fileclose(iott.io_fhandle);	
		//printf("playwav failed\n");
		return(-1);
	}
	dx_fileclose(iott.io_fhandle);

	if(1==1)
	{
		char temp_strs[400];							
		sprintf(temp_strs," %s play  voice: %s",dev[xh].voxname, msg);	
		insert_tel_logs(xh,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
	}
	//printf("\n %s play  voice: %s",dev[xh].voxname, msg);
	//#endif
	return(OK);
}

int GetLineInput(int xh,int chdev,int timeout)
{
	int  numdigs, cnt,i,j;
	DV_TPT tpt[6];
	DV_DIGIT digp; 
	unsigned char        content_tmp[128];

	dx_clrtpt(tpt,sizeof(tpt)/sizeof(DV_TPT));
	
	/* 接收一个"#"键 "*"或 等待timeout结束 */
    tpt[0].tp_type = IO_CONT; 
    tpt[0].tp_termno = DX_MAXDTMF; // Maximum number of digits 
    tpt[0].tp_length = 4; // terminate on 4 digits 
    tpt[0].tp_flags = TF_MAXDTMF; // terminate if already in buf. 
	
    tpt[1].tp_type = IO_CONT; 
    tpt[1].tp_termno = DX_LCOFF; // LC off termination 
    tpt[1].tp_length = 3; // Use 30 ms (10 ms resolution * timer) 
    tpt[1].tp_flags = TF_LCOFF|TF_10MS; // level triggered, clear history, * 10 ms resolution
	
    tpt[2].tp_type = IO_CONT; 
    tpt[2].tp_termno = DX_MAXTIME; // Function Time 
    tpt[2].tp_length = timeout*10; // 6 seconds (100 ms resolution * timer) 
    tpt[2].tp_flags = TF_MAXTIME; // Edge-triggered  
	
    tpt[3].tp_type = IO_EOT; 
    tpt[3].tp_termno = DX_DIGMASK; 
    tpt[3].tp_length = 0x1000; 
    tpt[3].tp_flags = TF_DIGMASK; 
	
	memset(line_input[xh],0,sizeof(line_input[xh]));  

	for(i=0;i<8;i++)
	{
		if ((numdigs = dx_getdig(chdev, tpt, &digp, EV_SYNC)) == -1) 
		{ 
			char temp_strs[400];							
			sprintf(temp_strs,"get input error,index=%d",xh);	
			insert_tel_logs(xh,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			//printf("\nget input error"); 
			i=-1;
			break;
		} 
		else if (digp.dg_type[0]!=48)
		{
			//printf("\nget input timeout"); 
			break;
		}
		else  
		{	
			for (cnt=0; cnt < numdigs, digp.dg_type[cnt] == 48; cnt++)
			{
				memset(content_tmp,0,sizeof(content_tmp));
				sprintf(content_tmp,"%c",digp.dg_value[cnt]);
				for(j=60;j<sizeof(content_tmp);j++)
				{
					if(content_tmp[j]!=0)
					{
						cnt=-1;
					break;
					}
				}
				if(cnt<0)
					break;

				strcat(line_input[xh], content_tmp);  
			 
				if(content_tmp[0]==0)continue;
				if(strstr(content_tmp,"#"))
				{
					cnt=-1;
					break;
				}
				if(strstr(content_tmp,"*"))
				{
					cnt=-1;
					break;
				}
			}
			if(1==1)
			{
				char temp_strs[400];							
				sprintf(temp_strs,"line[%d] input: %s",xh,line_input[xh]);	
				insert_tel_logs(xh,temp_strs);
				//printf("\nOnCall:%s",temp_strs);
			}
			//printf("\nline[%d] input: %s",xh,line_input[xh]);
			if(cnt<0)
				break;
		}
		if(cnt<0)
			break;
	}
	if(i<0)
		return FALSE;
	else
		return TRUE;
}

DWORD  ReQuery(LONG IParam)//20110226 取消 DWORD WINAPI ReQuery(LONG IParam)
{	
	int		line_no;
	int		timeout_num;
	int     klev,allklev,lastalllevl,i,j,Jtimes,fd,fd0,fd1;
	char    tinput[80];
	char    alltinput[80];
	struct tm 	*today;
	time_t		ltime;
	int     tmmin;
	char     Jc;
 	POINTER		point;
	DMS_COMMON  oncalldms_common;
	char  array[256];
    int rtms;
	line_no  = IParam;

	for(;;)//20110131
	{
		Sleep(500);
		if(	dxn[line_no]!=-1)continue;
		dxn[line_no]=1;
		dx_sethook(dev[line_no].vox, DX_ONHOOK, EV_SYNC);
		//20110217 dxn[line_no] = dx_wtring(dev[line_no].vox,1,DX_OFFHOOK,-1);
		//dxn[line_no] = dx_wtring(dev[line_no].vox,1,DX_OFFHOOK,0); 20110226
		dxn[line_no] = dx_wtring(dev[line_no].vox,1,DX_OFFHOOK,1);
       
		//printf("BBB dxn[%d]=%d\n",line_no,dxn[line_no]);
		if (dxn[line_no] == 0)
		{
			char temp_strs[400];							
			sprintf(temp_strs,"tel Inquirying line %d %s",line_no,dev[line_no].voxname);	
			insert_tel_logs(line_no,temp_strs);
			//printf("\nOnCall:%s",temp_strs);
			//printf("\nONCALL phone Inquirying line %d %s ### \n", line_no, dev[line_no].voxname);
		}
		else
		{
			dx_sethook(dev[line_no].vox, DX_ONHOOK, EV_SYNC);
			dxn[line_no]=-1;
			continue;
		}
		if(readedxml==1)
		{
			telusexmlbufoverR=0;//
		}
		else
		{
			char temp_strs[400];							
			sprintf(temp_strs,"配置文件未读取完成，系统暂无法处理电话查询");	
			insert_tel_logs(line_no,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			//printf("\n!!!!配置文件未读取完成，系统暂无法处理电话查询!!!!\n");
			dxn[line_no]=-1;//20110209
			continue;
		}
		
		Jc=35;
		ltime=time(NULL);
		today=localtime(&ltime);
		tmmin=today->tm_hour*60+today->tm_min;
		
		allklev=0;

		if(1==1)
		{
			char temp_strs[400];							
			sprintf(temp_strs,"start to deal with line %d call",line_no);	
			insert_tel_logs(line_no,temp_strs);
			//printf("\nOnCall:%s",temp_strs);
		}
		//printf("\n>>> ONCALL  start to deal with line %d call\n", line_no);
		//tel_playwav_msg(line_no,dev[line_no].vox,_CS_("TELALM_STR_HELLO"));
		timeout_num= 0;
		Jtimes=0;
		
		i=0;
		//writelognow(">>开始处理线路查询",1);
		
		user_hookon[line_no]=0;
		sr_enbhdlr(dev[line_no].vox, TDX_CST, mygjytest);
		
		if(oncallinquiry_def.telpassword[0]!=0)
		{
			for(i=0;i<3;i++)
			{ 
				if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
				if(i>0)tel_playwav_msg(line_no,dev[line_no].vox,"密码输入错误");
				if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
				tel_playwav_msg(line_no,dev[line_no].vox,"请您输入查询密码,以井号结束");
				if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
				if(!GetLineInput(line_no, dev[line_no].vox,2))
				{
					char temp_strs[400];							
					sprintf(temp_strs," error happened, end query ,index=%d",line_no);	
					insert_tel_logs(line_no,temp_strs);
					//printf("\nOnCall:%s",temp_strs);

					//printf("\n error happened, end query");
					goto ReQuery_Err;
				}
				if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
				Sleep(300);	
				for(j=1;j<sizeof(line_input[line_no]);j++)
				{
					if(line_input[line_no][j]==0 && line_input[line_no][j-1]!=0)
					{
						if(line_input[line_no][j-1]==Jc)
							line_input[line_no][j-1]=0;
					}
				}
				if(strcmp(oncallinquiry_def.telpassword,line_input[line_no])==0)
				{
					i=-1;break;
				}
			}
			if(i>=0)
			{
				tel_playwav_msg(line_no,dev[line_no].vox,"密码输入错误");
				goto ReQuery_Over;
			}
		}
		for(;;)
		{
			if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
			ltime=time(NULL);today=localtime(&ltime);
			if(today->tm_hour*60+today->tm_min-tmmin>10 || today->tm_hour*60+today->tm_min<tmmin)///////
				goto ReQuery_AlltimeOut;
			if(timeout_num>6)
				goto ReQuery_timeOut_loop;
			if(Jtimes>6)
				goto ReQuery_timeOut_loop;
			rtms=0;
			
			if(allklev<0)
			{
				allklev=0;
				lastalllevl=0;
				klev=0;
				memset(alltinput,0,sizeof(alltinput));
			}
			if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
			if(allklev<=0)
				tel_playwav_msg(line_no,dev[line_no].vox,oncallinquiry_def.telerrcodere);
			else
			{	
				while(1)
				{
					ltime=time(NULL);today=localtime(&ltime);
					if(today->tm_hour*60+today->tm_min-tmmin>20 || today->tm_hour*60+today->tm_min<tmmin)///////
						goto ReQuery_AlltimeOut;
					
					fd=-1;
					fd1=-1;
					
					if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
					for(i=0;i<oncallinquiry_def.real_oncallinquiry_def_num;i++)
					{
						if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
						if(oncallinquiry_def.TEL_code[i][allklev]==0)//
							if(oncallinquiry_def.TEL_code[i][allklev-1]!=0)//
							{
								if(strcmp(oncallinquiry_def.TEL_code[i],alltinput)==0)
								{
									if(oncallinquiry_def.longname[i][0]!=0)
									{ 
										if(fd<0)
											fd=i;
										if(oncallinquiry_def.logicnames[i][0][0]==0)
										{ 
											fd1=i;
											break;
										}
									}
								}
							}
					}
					if(fd1>=0)
						fd=fd1;
					if(fd<0)
					{ 
					if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
						if(allklev!=lastalllevl || rtms>0)
						{
							rtms=0;
							if(fd1<0)
								allklev--;
							lastalllevl=allklev;
						}
						rtms++;
						if(allklev>0)
						{ 
							for(i=allklev;i<sizeof(alltinput);i++)
								alltinput[i]=0;
							
							fd0=-1;//查找上一级可念
							for(i=0;i<oncallinquiry_def.real_oncallinquiry_def_num;i++)
							{
								if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
								if(oncallinquiry_def.TEL_code[i][allklev]==0)//
									if(oncallinquiry_def.TEL_code[i][allklev-1]!=0)//
									{
										if(oncallinquiry_def.logicnames[i][0][0]==0)
										{
											if(oncallinquiry_def.longname[i][0]!=0)
											{
												fd0=i;//可念
												tel_playwav_msg(line_no,dev[line_no].vox,oncallinquiry_def.longname[i]);
												tel_playwav_msg(line_no,dev[line_no].vox," 请输入查询编号 案星号加井号键返回上一级");
												goto ReQuery_checkinput;
												//	break;
											}
										}
									}
							}
						}
						else
						{
							allklev=0;
							if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
							tel_playwav_msg(line_no,dev[line_no].vox,oncallinquiry_def.telerrcodere);
							break;
						}
						
						continue;
					}
					else
					{
					if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
						if(fd1<0)
						{	
							tel_playwav_msg(line_no,dev[line_no].vox,oncallinquiry_def.longname[fd]);
							for(j=0;j<20;j++)
							{
							if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
								if(oncallinquiry_def.logicnames[fd][j][0]==0)
								{
									i=-99;
									break;
								}
								
								if(-1==GetPtId(oncallinquiry_def.logicnames[fd][j], &point))
								{
									char temp_strs[400];							
									sprintf(temp_strs," 查询逻辑名定义错误 %s,index=%d",oncallinquiry_def.logicnames[fd][j],line_no);	
									insert_tel_logs(line_no,temp_strs);
									//printf("\nOnCall:%s",temp_strs);

									//printf("\n>>>>>查询逻辑名定义错误");
									//writelognow(" >>查询逻辑名定义错误 ",1);
									tel_playwav_msg(line_no,dev[line_no].vox,"逻辑名定义错误，请联系管理员");
									i=-2;
									break;
								}
								else
								{  
									if(-1==ReadEntryById(&point, &oncalldms_common))
									{
										char temp_strs[400];							
										sprintf(temp_strs,"No this point in database %s,index=%d",oncallinquiry_def.logicnames[fd][j],line_no);	
										insert_tel_logs(line_no,temp_strs);
										//printf("\nOnCall:%s",temp_strs);


										//printf("\n>>>ONCALL ReadEntryById查询错误 No this point in database!");
										//writelognow(">>ONCALL ReadEntryById查询错误 No this point in database",1);
										tel_playwav_msg(line_no,dev[line_no].vox,"未查询到数据名称,请联系管理员");
										break;
									} 
									else
									{
										//if((point.data_type ==SOE_TYPE)|(point.data_type ==POL_TYPE)|(point.data_type ==OUTS_TYPE)|(point.data_type ==SYSMSG_TYPE))   
										Getarray(array,oncalldms_common,point);
										//GetPtLongnameById(&oncalldms_common.point, tmp2);
										tel_playwav_msg(line_no,dev[line_no].vox,array);
									}
								}
							}
							allklev--;
							lastalllevl=allklev;//
							if(allklev<0)
								allklev=0;
						}
						else
						{
							//找到数据，处理
						if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
							tel_playwav_msg(line_no,dev[line_no].vox,oncallinquiry_def.longname[fd1]);
							tel_playwav_msg(line_no,dev[line_no].vox,"请输入查询编号,案星号加井号键返回上一级");
						}
						goto ReQuery_checkinput;
						break;
					}
					break;
			}
		}
ReQuery_checkinput:	
		if(user_hookon[line_no]>=HOOKONBUSYTONETIMES)goto ReQuery_hookoff;
		if(!GetLineInput(line_no, dev[line_no].vox,2))
		{
			char temp_strs[400];							
			sprintf(temp_strs,"error happened, end query,index=%d",line_no);	
			insert_tel_logs(line_no,temp_strs);
			//printf("\nOnCall:%s",temp_strs);

			//printf("\n error happened, end query");
			goto ReQuery_Err;
		}
		Sleep(300);	
		if(line_input[line_no][0]==0)
		{
			timeout_num++;
			if(timeout_num>3)
				goto ReQuery_timeOut_loop;
			
			continue;
		}
		
		klev=0;
		if(strstr(line_input[line_no],"*"))
		{
			//timeout_num++;
			if(timeout_num>6)
				goto ReQuery_timeOut_loop;
			timeout_num=0;
			if(lastalllevl<0)
				lastalllevl=0;
			klev=0;
			
			allklev--; 
			if(allklev<0)allklev=0;
			//allklev=lastalllevl;
			for(i=allklev;i<sizeof(alltinput);i++)
				alltinput[i]=0;
			Jtimes++;
			if(Jtimes>6)
				goto ReQuery_timeOut_loop;
			continue;
		}
		
		if(line_input[line_no][0]==Jc)
		{
			klev=0;
			timeout_num++;
			if(timeout_num>3)
				goto ReQuery_timeOut_loop;
			Jtimes++;
			if(Jtimes>6)
				goto ReQuery_timeOut_loop;
		}
		else
		{
			Jtimes=0;//
			for(i=0;i<60;i++)
			{
				klev=i;
				if(line_input[line_no][i]==Jc)
					break;
				if(line_input[line_no][i]==0)
				{
					line_input[line_no][i]=Jc;
					break;
				}
			}
		}
		
		Jtimes=0;
		timeout_num=0;
		memset(tinput,0,sizeof(tinput));
		for(i=0;i<sizeof(tinput);i++)
			tinput[i]=line_input[line_no][i];
		
		
		if(allklev<0)allklev=0;
		lastalllevl=allklev;
		if(klev<0)klev=0;
		
		for(i=allklev;i<sizeof(alltinput) && i<allklev+klev;i++)
			alltinput[i]=tinput[i-allklev];
		
		allklev=allklev+klev;
		for(i=allklev;i<sizeof(alltinput);i++)
			alltinput[i]=0;
	}
	
ReQuery_AlltimeOut:
	tel_playwav_msg(line_no, dev[line_no].vox,"查询总时间超过10分钟,系统将自动挂机"); 
	goto ReQuery_Over;
ReQuery_timeOut_loop:
	tel_playwav_msg(line_no, dev[line_no].vox,"查询超时,系统将自动挂机"); 
	goto  ReQuery_Over;
ReQuery_Err:
	tel_playwav_msg(line_no, dev[line_no].vox,"查询出错,系统将自动挂机"); 
	goto  ReQuery_Over;
	
ReQuery_Over:
	tel_playwav_msg(line_no, dev[line_no].vox,"谢谢查询,再见"); 
	goto  ReQuery_OK;
ReQuery_hookoff:
	tel_playwav_msg(line_no, dev[line_no].vox,"用户已挂机"); 
ReQuery_OK:
	sr_dishdlr(dev[line_no].vox, TDX_CST, mygjytest );
	user_hookon[line_no]=0;
	dx_sethook(dev[line_no].vox, DX_ONHOOK, EV_SYNC);
	//writelognow(">>线路查询完毕",1);
	dxn[line_no]=-1;
	for(i=0;i<MAXCHAN;i++)//20110131
	{
		if(CHANUSE[i]!=1)
			continue;
		if(dxn[line_no]!=-1)
		{
			i=-1;
			break;
		}
	}
	if(i>=0)
	{
		telusexmlbufoverR=1;
		//printf("\nONCALL LINE_NO=%d i>=0\n",line_no);
	}
}
//	return 1;  20110130
}
void manualmTelDial(int * instr)
{ 
    int		i,j,ki,lid,cares, single, playsound;
    char	NumStr[20];
    DX_CAP 	capp;
	char    almedAr[TELPHONE_NUM];
	lid=instr[0];
    i=instr[1];
	
	single = 0;
	playsound = 0;
	memset(almedAr,0,sizeof(almedAr));
	dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);

	dx_clrcap(&capp);
	capp.ca_dtn_pres = 100;
	capp.ca_dtn_npres = 300;
	capp.ca_dtn_deboff = 10;
	capp.ca_noanswer = 3000;
	capp.ca_intflg = DX_PVDENABLE; //DX_OPTEN;///DX_PVDENABLE
	capp.ca_nbrdna=12;//defual 4:无人应答等待震铃次数
	capp.ca_stdely=25;//25;//d:25拨号完开始分析之间的延时
	
	for(ki=0;ki<oncallinquiry_def.almfailtrytimes;ki++)
	{
		for(j=0;j<telphone_def.real_telphone_def_num;j++)       
		{			
			if(almedAr[j]>0)continue;
			if(oncall_dial_manualmsendbuf.telnum[i][j][0]==0)continue;
			dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
			//strcpy(NumStr,telphone_def.telphone[j]);
			if(oncall_dial_manualmsendbuf.dial_out_add_num[i][j][0]!=0)
			{
				strcpy(NumStr,oncall_dial_manualmsendbuf.dial_out_add_num[i][j]);
				strcat(NumStr,oncall_dial_manualmsendbuf.telnum[i][j]);
			}
			else
			{
				strcpy(NumStr,oncall_dial_manualmsendbuf.telnum[i][j]);
			}
			dx_sethook(dev[lid].vox, DX_OFFHOOK, EV_SYNC);
			cares = dx_dial(dev[lid].vox,NumStr,&capp,DX_CALLP | EV_SYNC );
			
			//switch(ATDX_CPTERM(dev[lid].vox))20110121 chg
			switch(cares)
			{ 
			case CR_CNCT:     //call connected
				switch (ATDX_CONNTYPE(dev[lid].vox) )
				{
				case CON_CAD: // 有人接听，但一段时间内无人说话(韵律连接音)
					 if(1==1)
					 {
						 char temp_strs[400];							
						 sprintf(temp_strs,"%s:%s [%d] cadence",dev[lid].voxname,NumStr,lid);	
						 insert_tel_logs(lid,temp_strs);
						 //printf("\nOnCall:%s",temp_strs);
					 }
					//printf( ">>>>>>ONCALL %s:%s ( class=%d num=%d )cadence \n", dev[lid].voxname,NumStr,i+1,j+1);
					almedAr[j]++;
					if(oncallinquiry_def.BftelAlmSpk[0]!=0)
						if(tel_playwav_msg( lid,dev[lid].vox,oncallinquiry_def.BftelAlmSpk)<0 ) 
						{
							
							char temp_strs[400];							
							sprintf(temp_strs,"%s:%s [%d] manualmTelDial BftelAlmSpk play sound error",dev[lid].voxname,NumStr,lid);	
							insert_tel_logs(lid,temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							
							//printf(">>>ONCALL manualmTelDial BftelAlmSpk play sound error0!\n");
							break;
						}
					    if(oncall_dial_manualmsendbuf.longname[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_manualmsendbuf.longname[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] manualmTelDial cadence play sound error",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							
							//	printf(">>>ONCALL manualmTelDial cadence play sound error0!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,
									oncall_dial_manualmsendbuf.time_str[0],oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							
							}
							//printf("\noncall line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								//oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
						}
						if(oncall_dial_manualmsendbuf.message[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_manualmsendbuf.message[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] manualmTelDial cadence play sound error1",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL manualmTelDial cadence play sound error1!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"manualmTelDial line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall manualmTelDial line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								//oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
						}
						tel_playwav_msg( lid,dev[lid].vox,"报警完毕,再见");
						break;
				case CON_LPC:  //(循环流连接音)
					
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] conlpc",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>>>>ONCALL %s:%s ( class=%d num=%d )conlpc \n", dev[lid].voxname,NumStr,i+1,j+1);
					almedAr[j]++;
					if(oncallinquiry_def.BftelAlmSpk[0]!=0)
						if(tel_playwav_msg( lid,dev[lid].vox,oncallinquiry_def.BftelAlmSpk)<0 ) 
						{
							char temp_strs[400];							
							sprintf(temp_strs,"%s:%s [%d] manualmTelDial BftelAlmSpk play sound error0",dev[lid].voxname,NumStr,lid);	
							insert_tel_logs(lid,temp_strs);
							//printf("\nOnCall:%s",temp_strs);

							//printf(">>>ONCALL manualmTelDial BftelAlmSpk play sound error0!\n");
							break;
						}
						if(oncall_dial_manualmsendbuf.longname[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_manualmsendbuf.longname[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] manualmTelDial conlpc play sound error01",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);

//								printf(">>>ONCALL manualmTelDial conlpc play sound error01!\n");
								break;
							}
							if(1==1)
							{

								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								//oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
						}
						if(oncall_dial_manualmsendbuf.message[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_manualmsendbuf.message[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] manualmTelDial conlpc play sound error11",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL manualmTelDial conlpc play sound error11!\n");
								break;
							}
							if(1==1)
							{
								
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_manualmsendbuf.time_str[0],
									oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall manualmTelDial line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								//oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
						}
						tel_playwav_msg( lid,dev[lid].vox,"报警完毕,再见");
						break;
				case CON_PVD: // 有人接听电话，并且说话了(阳机音连接)
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] conpvd",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>>>>ONCALL %s:%s ( class=%d num=%d )conpvd \n", dev[lid].voxname,NumStr,i+1,j+1);
					almedAr[j]++;
					if(oncallinquiry_def.BftelAlmSpk[0]!=0)
						if(tel_playwav_msg( lid,dev[lid].vox,oncallinquiry_def.BftelAlmSpk)<0 ) 
						{
							char temp_strs[400];							
							sprintf(temp_strs,"%s:%s [%d] manualmTelDial BftelAlmSpk play sound error0",dev[lid].voxname,NumStr,lid);	
							insert_tel_logs(lid,temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//printf(">>>ONCALL manualmTelDial BftelAlmSpk play sound error0!\n");
							break;
						}
						if(oncall_dial_manualmsendbuf.longname[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_manualmsendbuf.longname[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] manualmTelDial conpvd play sound error011",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL manualmTelDial conpvd play sound error011!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_manualmsendbuf.time_str[0],
									oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								//oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
						}
						if(oncall_dial_manualmsendbuf.message[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_manualmsendbuf.message[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] manualmTelDial conpvd play sound error111",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							 
								//printf(">>>ONCALL manualmTelDial conpvd play sound error111!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_manualmsendbuf.time_str[0],
									oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall manualmTelDial line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								//oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
						}
						tel_playwav_msg( lid,dev[lid].vox,"报警完毕,再见");
						break;
				case CON_PAMD: // 对方是自动应答机(如：电信局返回的无此电话号码、对方正忙、无人接听请留言等提示音)(阳极应答机连接音)
					
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] conpamd",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>>>>ONCALL %s:%s ( class=%d num=%d )conpamd \n", dev[lid].voxname,NumStr,i+1,j+1);
					almedAr[j]++;
					if(oncallinquiry_def.BftelAlmSpk[0]!=0)
						if(tel_playwav_msg( lid,dev[lid].vox,oncallinquiry_def.BftelAlmSpk)<0 ) 
						{
							char temp_strs[400];							
							sprintf(temp_strs,"%s:%s [%d] BftelAlmSpk play sound error0!",dev[lid].voxname,NumStr,lid);	
							insert_tel_logs(lid,temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//printf(">>>ONCALL manualmTelDial BftelAlmSpk play sound error0!\n");
							break;
						}
						if(oncall_dial_manualmsendbuf.longname[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_manualmsendbuf.longname[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] manualmTelDial conpamd play sound error0111",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL manualmTelDial conpamd play sound error0111!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_manualmsendbuf.time_str[0],
									oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_manualmsendbuf.time_str[0],
							//	oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
						}
						if(oncall_dial_manualmsendbuf.message[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_manualmsendbuf.message[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] manualmTelDial conpamd play sound error1111",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL manualmTelDial conpamd play sound error1111!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_manualmsendbuf.time_str[0],
									oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall manualmTelDial line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_manualmsendbuf.time_str[0],
								//oncall_dial_manualmsendbuf.longname[i],oncall_dial_manualmsendbuf.message[i]);
						}
						tel_playwav_msg( lid,dev[lid].vox,"报警完毕,再见");
						break;
				default: 
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] Unknown connection type",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>ONCALL manualmTelDial %s:%s ( class=%d num=%d )Unknown connection type\n", dev[lid].voxname ,NumStr,i+1,j+1); 
					break; 
				}
				dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
				break;
				case CR_BUSY:     // line busy
					
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] busy",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf("\n %s dial %s ( class=%d num=%d )busy\n",dev[lid].voxname,NumStr,i+1,j+1);
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
					break;
				case CR_NOANS:  // no answer
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] no answer",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf("\n %s dial %s ( class=%d num=%d )no answer\n",dev[lid].voxname,NumStr,i+1,j+1);
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
					break;
				case CR_NODIALTONE:  // no dial tone 
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] no dial tone",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf("\n %s dial %s ( class=%d num=%d )no dial tone  \n",dev[lid].voxname,NumStr,i+1,j+1); 
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
					break;	
				case CR_NORB:  // no ring back
					
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] no ring back",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					single = -1;
					//printf("\n %s dial %s ( class=%d num=%d )no dial tone  \n",dev[lid].voxname,NumStr,i+1,j+1); 
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
					break;
					
				default :
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] error##",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf("\n %s dial %s ( class=%d num=%d )error\n",dev[lid].voxname,NumStr,i+1,j+1);
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
			}
			dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
			Sleep(2000);
	}
	}
	dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
	dxn[lid]=-1;	
}

void autoalmTelDial(int * instr)
{   	  	
    int		i,j,ki,lid,cares, single, playsound;
    char	NumStr[20];
    DX_CAP 	capp;
	char    almedAr[TELPHONE_NUM];
	lid=instr[0];
    i=instr[1];
	
	single = 0;
	playsound = 0;
	memset(almedAr,0,sizeof(almedAr));
	dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);

	dx_clrcap(&capp);
	capp.ca_dtn_pres = 100;
	capp.ca_dtn_npres = 300;
	capp.ca_dtn_deboff = 10;
	capp.ca_noanswer = 3000;
	capp.ca_intflg = DX_PVDENABLE; //DX_OPTEN;///DX_PVDENABLE
	capp.ca_nbrdna=8;//defual 4:无人应答等待震铃次数
	capp.ca_stdely=25;//25;//d:25拨号完开始分析之间的延时
	
		
	for(ki=0;ki<oncallinquiry_def.almfailtrytimes;ki++)
	{
		for(j=0;j<telphone_def.real_telphone_def_num;j++)       
		{			
			if(almedAr[j]>0)continue;
			if(oncall_dial_autoalmsendbuf.telnum[i][j][0]==0)continue;
			dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
		 
			//strcpy(NumStr,telphone_def.telphone[j]);
			//strcpy(NumStr,oncall_dial_autoalmsendbuf.telnum[i][j]);
			if(oncall_dial_autoalmsendbuf.dial_out_add_num[i][j][0]!=0)
			{
				strcpy(NumStr,oncall_dial_autoalmsendbuf.dial_out_add_num[i][j]);
				strcat(NumStr,oncall_dial_autoalmsendbuf.telnum[i][j]);
			}
			else
			{
				strcpy(NumStr,oncall_dial_autoalmsendbuf.telnum[i][j]);
			}
			dx_sethook(dev[lid].vox, DX_OFFHOOK, EV_SYNC);
			cares = dx_dial(dev[lid].vox,NumStr,&capp,DX_CALLP | EV_SYNC );
			
			//switch(ATDX_CPTERM(dev[lid].vox))20110121 chg
			switch(cares)
			{ 
			case CR_CNCT:     //call connected
				switch (ATDX_CONNTYPE(dev[lid].vox) )
				{
				case CON_CAD: // 有人接听，但一段时间内无人说话(韵律连接音)
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] cadence",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>>>>ONCALL %s:%s ( class=%d num=%d )cadence \n", dev[lid].voxname,NumStr,i+1,j+1);
					almedAr[j]++;
					if(oncallinquiry_def.BftelAlmSpk[0]!=0)
						if(tel_playwav_msg( lid,dev[lid].vox,oncallinquiry_def.BftelAlmSpk)<0 ) 
						{
							char temp_strs[400];							
							sprintf(temp_strs,"%s:%s [%d] autoalmTelDial BftelAlmSpk play sound error",dev[lid].voxname,NumStr,lid);	
							insert_tel_logs(lid,temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//printf(">>>ONCALL autoalmTelDial BftelAlmSpk play sound error0!\n");
							break;
						}
					    if(oncall_dial_autoalmsendbuf.longname[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_autoalmsendbuf.longname[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] autoalmTelDial cadence play sound error",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL autoalmTelDial cadence play sound error0!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,
									oncall_dial_autoalmsendbuf.time_str[0],oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								
							}
							//printf("\noncall line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
								//oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
						}
						if(oncall_dial_autoalmsendbuf.message[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_autoalmsendbuf.message[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] autoalmTelDial cadence play sound error1",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL autoalmTelDial cadence play sound error1!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"autoalmTelDial line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
									oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall autoalmTelDial line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
								//oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
						}
						tel_playwav_msg( lid,dev[lid].vox,"报警完毕,再见");
						break;
				case CON_LPC:  //(循环流连接音)
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] conlpc",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>>>>ONCALL %s:%s ( class=%d num=%d )conlpc \n", dev[lid].voxname,NumStr,i+1,j+1);
					almedAr[j]++;
					if(oncallinquiry_def.BftelAlmSpk[0]!=0)
						if(tel_playwav_msg( lid,dev[lid].vox,oncallinquiry_def.BftelAlmSpk)<0 ) 
						{
							char temp_strs[400];							
							sprintf(temp_strs,"%s:%s [%d] autoalmTelDial BftelAlmSpk play sound error0",dev[lid].voxname,NumStr,lid);	
							insert_tel_logs(lid,temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//printf(">>>ONCALL autoalmTelDial BftelAlmSpk play sound error0!\n");
							break;
						}
						if(oncall_dial_autoalmsendbuf.longname[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_autoalmsendbuf.longname[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] autoalmTelDial conlpc play sound error01",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);

								//printf(">>>ONCALL autoalmTelDial conlpc play sound error01!\n");
								break;
							}
							if(1==1)
							{
								
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
									oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
								//oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
						}
						if(oncall_dial_autoalmsendbuf.message[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_autoalmsendbuf.message[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] autoalmTelDial conlpc play sound error11",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL autoalmTelDial conlpc play sound error11!\n");
								break;
							}
							if(1==1)
							{
								
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
									oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall autoalmTelDial line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
								//oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
						}
						tel_playwav_msg( lid,dev[lid].vox,"报警完毕,再见");
						break;
				case CON_PVD: // 有人接听电话，并且说话了(阳机音连接)
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] conpvd",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>>>>ONCALL %s:%s ( class=%d num=%d )conpvd \n", dev[lid].voxname,NumStr,i+1,j+1);
					almedAr[j]++;
					if(oncallinquiry_def.BftelAlmSpk[0]!=0)
						if(tel_playwav_msg( lid,dev[lid].vox,oncallinquiry_def.BftelAlmSpk)<0 ) 
						{
							char temp_strs[400];							
							sprintf(temp_strs,"%s:%s [%d] autoalmTelDial BftelAlmSpk play sound error0",dev[lid].voxname,NumStr,lid);	
							insert_tel_logs(lid,temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//printf(">>>ONCALL autoalmTelDial BftelAlmSpk play sound error0!\n");
							break;
						}
						if(oncall_dial_autoalmsendbuf.longname[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_autoalmsendbuf.longname[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] autoalmTelDial conpvd play sound error011",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL autoalmTelDial conpvd play sound error011!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
									oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
								//oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
						}
						if(oncall_dial_autoalmsendbuf.message[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_autoalmsendbuf.message[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] autoalmTelDial conpvd play sound error111",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL autoalmTelDial conpvd play sound error111!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
									oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall autoalmTelDial line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
								//oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
						}
						tel_playwav_msg( lid,dev[lid].vox,"报警完毕,再见");
						break;
				case CON_PAMD: // 对方是自动应答机(如：电信局返回的无此电话号码、对方正忙、无人接听请留言等提示音)(阳极应答机连接音)
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] conpamd",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>>>>ONCALL %s:%s ( class=%d num=%d )conpamd \n", dev[lid].voxname,NumStr,i+1,j+1);
					almedAr[j]++;
					if(oncallinquiry_def.BftelAlmSpk[0]!=0)
						if(tel_playwav_msg( lid,dev[lid].vox,oncallinquiry_def.BftelAlmSpk)<0 ) 
						{
							char temp_strs[400];							
							sprintf(temp_strs,"%s:%s [%d] BftelAlmSpk play sound error0!",dev[lid].voxname,NumStr,lid);	
							insert_tel_logs(lid,temp_strs);
							//printf("\nOnCall:%s",temp_strs);
							//printf(">>>ONCALL autoalmTelDial BftelAlmSpk play sound error0!\n");
							break;
						}
						if(oncall_dial_autoalmsendbuf.longname[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_autoalmsendbuf.longname[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] autoalmTelDial conpamd play sound error0111",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL autoalmTelDial conpamd play sound error0111!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
									oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
								//oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
						}
						if(oncall_dial_autoalmsendbuf.message[i][0]!=0)
						{	
							if(tel_playwav_msg( lid,dev[lid].vox,oncall_dial_autoalmsendbuf.message[i])<0 ) 
							{
								char temp_strs[400];							
								sprintf(temp_strs,"%s:%s [%d] autoalmTelDial conpamd play sound error1111",dev[lid].voxname,NumStr,lid);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
								//printf(">>>ONCALL autoalmTelDial conpamd play sound error1111!\n");
								break;
							}
							if(1==1)
							{
								char temp_strs[400];							
								sprintf(temp_strs,"line:%s Buffer%d: num%s time%s longname%s message %s",dev[lid].voxname,lid,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
									oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);	
								insert_tel_logs(lid,temp_strs);
								//printf("\nOnCall:%s",temp_strs);
							}
							//printf("\noncall autoalmTelDial line:%s Buffer%d: num%s time%s longname%s message %s\n",dev[lid].voxname,i+1,NumStr,oncall_dial_autoalmsendbuf.time_str[0],
								//oncall_dial_autoalmsendbuf.longname[i],oncall_dial_autoalmsendbuf.message[i]);
						}
						tel_playwav_msg( lid,dev[lid].vox,"报警完毕,再见");
						break;
				default: 
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] Unknown connection type",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf( ">>>ONCALL autoalmTelDial %s:%s ( class=%d num=%d )Unknown connection type\n", dev[lid].voxname ,NumStr,i+1,j+1); 
					break; 
				}
				dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
				break;
				case CR_BUSY:     // line busy
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] busy",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf("\n %s dial %s ( class=%d num=%d )busy\n",dev[lid].voxname,NumStr,i+1,j+1);
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
					break;
				case CR_NOANS:  // no answer
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] no answer",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf("\n %s dial %s ( class=%d num=%d )no answer\n",dev[lid].voxname,NumStr,i+1,j+1);
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
					break;
				case CR_NODIALTONE:  // no dial tone 
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] no dial tone",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf("\n %s dial %s ( class=%d num=%d )no dial tone  \n",dev[lid].voxname,NumStr,i+1,j+1); 
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
					break;	
				case CR_NORB:  // no ring back
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] no ring back",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					single = -1;
					//printf("\n %s dial %s ( class=%d num=%d )no dial tone  \n",dev[lid].voxname,NumStr,i+1,j+1); 
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
					break;
					
				default :
					if(1==1)
					{
						char temp_strs[400];							
						sprintf(temp_strs,"%s:%s [%d] error##",dev[lid].voxname,NumStr,lid);	
						insert_tel_logs(lid,temp_strs);
						//printf("\nOnCall:%s",temp_strs);
					}
					//printf("\n %s dial %s ( class=%d num=%d )error\n",dev[lid].voxname,NumStr,i+1,j+1);
					dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
			}
			dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
			Sleep(2000);
	}
	}
	dx_sethook(dev[lid].vox, DX_ONHOOK, EV_SYNC);
	dxn[lid]=-1;
}

void initialize_gtd (int device)
{				   
	if (dx_deltones(device) == -1) 
	{
		// process error
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_deltones() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);

		//printf("ERROR: on dx_deltones() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));	
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_deltones(device) == -1 ",1);
	}

	if (dx_blddtcad(DLDDTCAD_DISCTONE,
		350,50,    
		440,50,    
		25,5,
		25,5,
		4
	) == -1)
	{
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_blddtcad() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
		//printf("ERROR: on dx_blddtcad() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_blddtcad(device) == -1 ",1);
	}	
   	if (dx_addtone(device,(unsigned char)NULL,0) == -1)
	{
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_addtone() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
		//printf("ERROR: on dx_addtone() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));	
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_addtone(device) == -1 ",1);
	}
	if (dx_bldstcad(DLDSTCAD_DISCTONE,
		450,40,    
		490,40,  
		500,50,
		2) == -1)
	{
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_bldstcad() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
		//printf("ERROR: on dx_bldstcad() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_bldstcad(device) == -1 ",1);
	}	
	if (dx_addtone(device,(unsigned char)NULL,0) == -1)
	{
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_addtone() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);
		//printf("ERROR: on dx_addtone() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));	
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_addtone(device) == -1 ",1);
	}
	if (dx_bldst(DLDST_DISCTONE,
		oncallinquiry_def.guajiHz,oncallinquiry_def.guajifanwei, 2) == -1) 
	{

		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_bldst() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);

		//printf("ERROR: on dx_bldst() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_bldst(device) == -1 ",1);
	}	

	if (dx_addtone(device,(unsigned char)NULL,0) == -1)
	{
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_addtone() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);

		//printf("ERROR: on dx_addtone() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));	
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_addtone(device) == -1 ",1);
	}
	
	if(dx_enbtone(device, DLDST_DISCTONE,   DM_TONEON)==-1) 
	{
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_enbtone() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);

		//printf("ERROR: on dx_enbtone() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));	
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_enbtone(device) == -1 ",1);
	}
	
	if (dx_distone(device,DLDST_DISCTONE,DM_TONEOFF) == -1) 
	{
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_distone() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);

		//printf("ERROR: on dx_distone() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_distone(device) == -1 ",1);
	}

	if (dx_initcallp(device) == -1)
	{
		char temp_strs[400];							
		sprintf(temp_strs,"ERROR on dx_initcallp() on device %s is 0x%x %s ERRNO=%d",
			ATDV_NAMEP(device),ATDV_LASTERR(device), ATDV_ERRMSGP(device),dx_fileerrno());	
		insert_tel_logs(device,temp_strs);
		//printf("\nOnCall:%s",temp_strs);

		//printf("ERROR: on dx_initcallp() on device %s\n", ATDV_NAMEP(device));
		//printf("ERROR: is 0x%x %s\n",ATDV_LASTERR(device), ATDV_ERRMSGP(device));	
		//printf("Error　%s ERRNO=%d\n",dx_fileerrno()); 
		//writelognow(" dx_initcallp(device) == -1 ",1);
	}
}
