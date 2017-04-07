/*******************************************************************************************************************************
FILENAME:	
FUNCTION:		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, XP

HISTORY:
2008.4.10	start to modify the code for H9000 V4.0		zyf
********************************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <windows.h>         
#include <winsock.h>
#include "\home\ems\system\iec103tcp\v4.0a\inc\iec103_com.h"
 
#define DEBUG

void	IEC870_5_Recv(void);
void	IEC870_5_Send(void);
BOOL	RequestLinkStatus(void);
BOOL	ResetLinkStatus(void);
BOOL	ResetFrameStatus(void);
BOOL	RepeatTransmit(void);
BOOL	CommandExec(void);
BOOL	CommandSend();
BOOL	ClockSynchronisation(void);
BOOL	GeneralInterrogation(void);
BOOL	CounterInterrogation(void);
BOOL	RequestUserDataClass1(void);
BOOL	RequestUserDataClass2(void);
void	ClearRecvFault(void);
void	PrintRecvData();

extern	ASDUCOMDDATA	send_data;
extern	ASDUDATA		recv_data;

HANDLE	hPort_com;
HANDLE	hRecvSemaphore;



unsigned char recvbuf[1024];
unsigned char sendbuf[1024];
unsigned char cmdbuf[36];
DWORD	dwErrors;
COMSTAT	cstat;
BOOL	fcb[STN_NUM+1];
BOOL	resetlinkf[STN_NUM+1],resetframe[STN_NUM+1],initflag[STN_NUM+1],pollflag;
BOOL	counterflag[STN_NUM+1],ClockSyncflag,interrogationflag[STN_NUM+1];
unsigned int	repeatnum[STN_NUM+1],timeoutnum[STN_NUM+1];
int		cmd_ack[STN_NUM+1],localinitfg;
unsigned char	stn_id,comdflag,comdflag1;
char			hostname[20];

void main(void)
{
	HANDLE	hThread_Recv,hThread_Send;
	int		ThreadID_Recv,ThreadID_Send;
	BOOL	fSuccess;					  	
	BOOL	dataflag[STN_NUM+1],dataflow[STN_NUM+1];
	LONG	cMaxCount,cInitCount;
	DWORD	byte_recvd;

	unsigned int	sendbufheadpt,recvbufheadpt;
	unsigned char	chksum,ctrlfield,ctrlfunc,offset;
	unsigned int	i,ses,wat_num[STN_NUM+1],cyclenum[STN_NUM+1];

  
	WSADATA WSAData;	
	//OpenNewWindow();
	if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0)
	{ printf("\ndhsw.exe:: WSAStart() err!\n");exit(0); }
	gethostname(hostname, 20);
	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
    tmptmp=0;
	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}
	while( ReadDefineFile()!=SUCCESS )
	{
		printf("iec103.EXE: ReadDefineFile ERROR!\n"); exit(-1);
	}

	cMaxCount= 10;
	cInitCount = 0;
/*
	hPort_com=ComInit(COM_PORT,Com_Baud_Rate,8,Parity_Type,ONESTOPBIT);
	printf("\n %s init compelete !!!\n\n",COM_PORT);

*/

	hRecvSemaphore = CreateSemaphore(NULL,cInitCount,cMaxCount,"sem_recv");

	if (hRecvSemaphore == NULL)
		printf("Error in CreateSemaphore sem_recv:%d\n",GetLastError());
	else
		printf("sem_recv: OK!\n");

	hThread_Recv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEC870_5_Recv,
                                 NULL, CREATE_SUSPENDED, &ThreadID_Recv);
	if (hThread_Recv == NULL)
		printf("Error in CreateThread ThreadID_Recv:%d\n",GetLastError());
	else
		printf("hThread_Recv: %d OK!\n",hThread_Recv);

	SetThreadPriority(hThread_Recv,THREAD_PRIORITY_BELOW_NORMAL);	
	ResumeThread( hThread_Recv);	
    /*
 	hThread_Send = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEC870_5_Send,
                                 NULL, CREATE_SUSPENDED, &ThreadID_Send);
	if (hThread_Send == NULL)
		printf("Error in CreateThread ThreadID_Send:%d\n",GetLastError());
	else
		printf("hThread_Send: %d OK!\n",hThread_Send);

	SetThreadPriority(hThread_Send,THREAD_PRIORITY_BELOW_NORMAL);
	ResumeThread( hThread_Send);	
    */
		
	// IEC 870-5 Link layer communication service
	recvbufheadpt = 0;
	send_data.pnt = 0;
	recv_data.pnt = 0;
	sendbufheadpt = 0;
	localinitfg = 1;
	ses = 0;
	cmdbuf[6] = 0;

//Init, Clock Syn, General interrogation
	
	pollflag = 1;
	for(stn_id =0; stn_id <= STN_NUM; stn_id++)
	{
		fcb[stn_id] = 0;
		comdflag = 0;
		dataflow[stn_id] = 0;
		dataflag[stn_id] = 0;
		initflag[stn_id] = 1;
		resetlinkf[stn_id] = 1;
		resetframe[stn_id] = 1;

		if(stn_id ==1)
			ClockSyncflag = 1;
		else
			ClockSyncflag = 0;
		interrogationflag[stn_id] = 1;	//È«ÕÙ»½±êÖ¾
		counterflag[stn_id] = 1;		//ÕÙ»½Âö³å±êÖ¾

		repeatnum[stn_id] = 0;
		timeoutnum[stn_id] = 0;
		cmd_ack[stn_id] = 0;
		wat_num[stn_id] = 0;
		cyclenum[stn_id] = 0;
	}
	sendbufheadpt = send_data.pnt;
	stn_id = 0;

	SockStatus=-1;
	for(;;)
		{
		if(SockStatus==-1 )
		   SockStatus=MakeLink();
		if(SockStatus==-1 )
		{	closesocket( iec_sock );
			Sleep(1000);
			continue;
		}

		if(repeatnum[stn_id] !=0)
			{
			if(repeatnum[stn_id] < REPEAT_NUM)
				{
				if(!RepeatTransmit())
				{	
					SockStatus=-1;
					closesocket( iec_sock );
					continue;
				}
				}
			else
				{
				repeatnum[stn_id] = 0;
				comdflag1 = 0;
				ses = SES_NUM;
				{	
					SockStatus=-1;
					closesocket( iec_sock );
					continue;
				}
				}
			}
/*
		else if((comdflag == 2) && (cmdbuf[6] != 0))
			{
			//zm stn_id = cmdbuf[5];
			stn_id = GetStnId( cmdbuf[5] );//zm
			ses = 0;
			if(!CommandExec())
				continue;
			comdflag = 3;
			comdflag1 = 1;
			cmdbuf[6] = 0;			//cmd type clear zero
			}
*/
		else
			{
//			comdflag = 0;
//			repeatnum[stn_id] =0;
			ses ++;
			if((ses > SES_NUM)||(dataflag[stn_id] != 1))
				{
				ses = 0;
				stn_id ++;
				if(stn_id > STN_NUM)
					stn_id =1;
				}
			if(initflag[stn_id]!=0)
				cyclenum[stn_id]=0;
			else
				{
				cyclenum[stn_id]++;
				if(cyclenum[stn_id] > CYCLE_NUM)
					{
//					initflag[stn_id] = 2;
					if(stn_id == 1)
						ClockSyncflag = 1;
					interrogationflag[stn_id] = 1;
					//zm if((stn_id ==5)||(stn_id ==7))
					//zm	counterflag[stn_id] = 1;	//Âö³å
					repeatnum[stn_id] = 0;
					timeoutnum[stn_id] = 0;
		//			cmd_ack[stn_id] = 0;
					wat_num[stn_id] = 0;
					cyclenum[stn_id] = 0;
					}
				}
			if(timeoutnum[stn_id] >= TIMEOUT_NUM)
				{
	//			pollflag = 0;
				initflag[stn_id] = 1;
				resetlinkf[stn_id] = 1;
				resetframe[stn_id] = 1;
				if(stn_id == 1)
					ClockSyncflag = 1;
				interrogationflag[stn_id] = 1;
				counterflag[stn_id] = 1;
				wat_num[stn_id] = 0;
				repeatnum[stn_id] = 0;
				timeoutnum[stn_id] = 0;
				dataflag[stn_id] = 0;
	//			break;
	//			continue;
				}
			if(dataflag[stn_id])
				{
				if(!RequestUserDataClass1())
				{	
					SockStatus=-1;
					closesocket( iec_sock );
					continue;
				}
				}

			else if((initflag[stn_id] ==1) && (resetlinkf[stn_id] == 1))
				{
				if(!ResetLinkStatus())
				{	
					SockStatus=-1;
					closesocket( iec_sock );
					continue;
				}
				}
			else if((initflag[stn_id] ==1) && (resetframe[stn_id] == 1))
				{
				if(!ResetFrameStatus())
				{	
					SockStatus=-1;
					closesocket( iec_sock );
					continue;
				}
				}
/*			
			else if((ClockSyncflag == 1) && (initflag[stn_id] ==2))
				{
//				comdflag = 1;
				ClockSyncflag = 0;
				ClockSynchronisation();
				ReadFile(hPort_com,(LPSTR)recvbuf,36,&byte_recvd,NULL);
				
				if(byte_recvd == 0)
					{
					continue;
					}
#ifdef DEBUG
				printf("\nIEC received clock ACK byte number %u :\n",byte_recvd);
				PrintRecvData(byte_recvd);
#endif
				Sleep(50);
				continue;
				}
			else if(sendbufheadpt != send_data.pnt)		
				{
				if(!CommandSend(sendbufheadpt))
					continue;
				comdflag = 1;
				comdflag1 = 1;
				//zm stn_id = cmdbuf[5];
				stn_id = GetStnId( cmdbuf[5] ); //zm
				ses = 0;
				sendbufheadpt ++;
				if(sendbufheadpt>= CMD_ENTRY_NUMBER)
					sendbufheadpt = 0;
				}
			else if((initflag[stn_id] ==2) && (interrogationflag[stn_id] == 1))
				{
				if(!GeneralInterrogation())
					continue;
//					interrogationflag[stn_id] = 0;
				}
			else if((initflag[stn_id] ==3) && (counterflag[stn_id] == 1))
				{
				if(!CounterInterrogation())
					continue;
//					counterflag[stn_id] = 0;
				}
*/
			else	
				{
				if(!RequestUserDataClass2())
				{	
					SockStatus=-1;
					closesocket( iec_sock );
					continue;
				}
				}
			}//repeat,cmd
		//fSuccess = ReadFile(hPort_com,(LPSTR)recvbuf,VARFRAMELEN,&byte_recvd,NULL);
		printf("\n waiting for data coming in\n");
		fSuccess = recv(iec_sock, (char *)(recvbuf), VARFRAMELEN, RECV_FLAGS);
		if(!fSuccess)
			{
			if(comdflag1)
				printf("CommandSend: ReadFile ERRNO= %d \n",GetLastError());
			else
				printf("RequestData: ReadFile ERRNO= %d \n",GetLastError());
			ClearRecvFault();
				{	
					SockStatus=-1;
					closesocket( iec_sock );
					continue;
				}
			}

		if(byte_recvd == 0)
			{
			timeoutnum[stn_id]++;
			if((initflag[stn_id]==0)||(comdflag1))
				repeatnum[stn_id]++;
			if(comdflag1)
				{
				printf("=!!!!!=CommandSend: timeout! No: %d",timeoutnum[stn_id]);
				}
			else
				printf("RequestData: timeout! No: %d",timeoutnum[stn_id]);
				{	
					SockStatus=-1;
					closesocket( iec_sock );
					continue;
				}
			}

#ifdef DEBUG
		if(comdflag1)
			{
			printf("\n=+=+=+=Waiting CMD_ACK! Received STN %d  cmdflag=%d %d initflag=%d :\n",stn_id,comdflag1,comdflag,initflag[stn_id]);
			PrintRecvData(10);
			}
		else
			{
			printf("\n---received STN %d  cmdflag=%d %d initflag=%d :\n",stn_id,comdflag1,comdflag,initflag[stn_id]);
			PrintRecvData(10);
			}
		//printf("\n---received STN %d %u bytes cmdflag=%d %d initflag=%d :\n",stn_id,byte_recvd,comdflag1,comdflag,initflag[stn_id]);
#endif
/*
		if(initflag[stn_id])	//general init flag process
			{
			wat_num[stn_id] ++;
			if((wat_num[stn_id] > 3) && (initflag[stn_id]==1))
				{
				initflag[stn_id] = 2;
				wat_num[stn_id] = 0;
				}
			if((wat_num[stn_id] > 100) && (initflag[stn_id]==2))
				{
				if((stn_id == 5) || (stn_id == 7))
					{
					initflag[stn_id] = 3;
					}
				else
					{
					initflag[stn_id] = 0;
					counterflag[stn_id] = 0;
					}

				wat_num[stn_id] = 0;
				}
			}
*/
		//getchar();
		for(i=0; i<=byte_recvd - 5; i++)
			{
			if((recvbuf[i]==0x10)||(recvbuf[i]==0x68))
				break;
			}
		offset = i;
		switch (recvbuf[offset])
			{
//Single byte response NACK	for no data class2 and class1
/*
			case 0xE5:	fcb[stn_id] = !fcb[stn_id];			   //No user data class2 and class1	   
						timeoutnum[stn_id] =0;
						repeatnum[stn_id] = 0;
						dataflag[stn_id] = 0;
						if(byte_recvd != 1)
							{
							ClearRecvFault();
							}
						break;
*/
//fix frame respond for NACK for no requested data but with control field

			case 0x10:
						//zm if(recvbuf[offset+2] != stn_id)
						if( GetStnId( recvbuf[offset+2] ) != stn_id) //zm
							{
							printf("received station id = %d not equal to request %d\n",recvbuf[offset+2],stn_id);
//							if(comdflag1)
								repeatnum[stn_id]++;
							break;
							}
						if((byte_recvd<(DWORD)(FIXFRAMELEN+offset))||(recvbuf[offset+4]!=0x16))
							{
							ClearRecvFault();
							break;
							}
						chksum = recvbuf[offset+1] + recvbuf[offset+2];	
			
						if(chksum != recvbuf[offset+3])
							{
							ClearRecvFault();
							break;
							}
						ctrlfield= recvbuf[offset+1] & 0xc0;

						if(recvbuf[offset+1] & 0x20)	   //if ACD = 1
							dataflag[stn_id] = 1;
						else
							dataflag[stn_id] = 0;
				
						if(recvbuf[offset+1] & 0x10)	   //if DFC = 1
							dataflow[stn_id] = 1;
						else
							dataflow[stn_id] = 0;
				
						ctrlfunc = recvbuf[offset+1] & 0x0f;
		
						switch(ctrlfunc)
							{
							case 0:
#ifdef DEBUG
								printf("Station %d ACK or reset link response\n",recvbuf[offset+2]);
#endif
/*
								if(comdflag1)
									comdflag1 = 0;
								if(initflag[stn_id] ==1)
									{
									if(resetlinkf[stn_id] == 1)
										{
										resetlinkf[stn_id] = 0;
										}
									else
										{
										initflag[stn_id] =2;
										}
									}
								else if((initflag[stn_id] ==2) && (ClockSyncflag[stn_id] == 1) )
									{
									initflag[stn_id] =3;
									ClockSyncflag[stn_id] = 0;
									wat_num[stn_id] = 0;
									}
								else if((initflag[stn_id] ==2) && (interrogationflag[stn_id] == 1))
									{
									interrogationflag[stn_id] = 0;
									wat_num[stn_id] = 0;
									}
								else if((initflag[stn_id] ==3) && (counterflag[stn_id] == 1))
									{
									counterflag[stn_id] = 0;
									wat_num[stn_id] = 0;
									initflag[stn_id] =0;
									}
*/
								break;
							case 1:
#ifdef DEBUG
								printf("NACK or link busy response\n");
#endif
								if(comdflag1)
									{
									comdflag = 0;
									comdflag1 = 0;	//cmd refused by RTU, flag set to zero
									}
								break;
							case 9:
#ifdef DEBUG
								printf("No requested data response frame\n");
#endif
								break;
							case 11:
#ifdef DEBUG
								printf("Link status or access request response frame\n");
#endif
								break;
							default :
								printf("Error response function code %2x\n",ctrlfunc);
								break;
							}
						fcb[stn_id] = !fcb[stn_id];
						timeoutnum[stn_id] =0;
						repeatnum[stn_id] = 0;
						break;

//variable frame for requested data

			case 0x68:
						//zm if(recvbuf[offset+5] != stn_id)
						if( GetStnId( recvbuf[offset+5] )!= stn_id) //zm
							{
							printf("received station id = %d not equal to request %d\n",recvbuf[offset+2],stn_id);
//							if(comdflag1)
								repeatnum[stn_id]++;
							break;
							}
						if(byte_recvd<(DWORD)(MINFRAMELEN+offset))
							{
							ClearRecvFault();
							break;
							}
		
//						if((recvbuf[offset+3]!=0x68)||(recvbuf[offset+1]!= recvbuf[offset+2])||(byte_recvd<(DWORD)(recvbuf[offset+1]+6)))
						if((recvbuf[offset+3]!=0x68)||(byte_recvd<(DWORD)(recvbuf[offset+1]+6)))
							{
							ClearRecvFault();
							break;
							}

						byte_recvd = recvbuf[offset+1]+6;
						if(recvbuf[offset+byte_recvd - 1] != 0x16)
							{
							ClearRecvFault();
							break;
							}

						chksum = 0;
						for(i=4;i<byte_recvd - 2;i++)
							chksum = chksum + recvbuf[offset+i];	
		
						if(chksum != recvbuf[offset+byte_recvd - 2])
							{
							ClearRecvFault();
							break;
							}

						ctrlfunc = recvbuf[offset+4] & 0x0f;
		
						if((ctrlfunc == 8)||(ctrlfunc == 0))
							{
//							recvbufheadpt = recv_data.pnt;
#ifdef DEBUG
							printf("A Variable Frame is Received! pt=%d\n",recvbufheadpt);
#endif
							memset(&recv_data.data[recvbufheadpt],0,VARFRAMELEN);
							memcpy(&recv_data.data[recvbufheadpt],&recvbuf[offset+4],byte_recvd - 6);
							recv_data.length[recvbufheadpt] =(unsigned short) byte_recvd - 6;
							recvbufheadpt ++;
							if(recvbufheadpt >= ASDU_ENTRY_NUMBER)
								recvbufheadpt = 0;
							recv_data.pnt = recvbufheadpt;
							//ReleaseSemaphore(hRecvSemaphore,1,NULL);
							tmptmp=1;
							}
//						if(comdflag)
							Sleep(100);
						if(recvbuf[offset+4] & 0x20)	   //if ACD =1
							dataflag[stn_id] = 1;
						else
							dataflag[stn_id] = 0;

						if(recvbuf[offset+4] & 0x10)	   //if DFC = 1
							dataflow[stn_id] = 1;
						else
							dataflow[stn_id] = 0;

						fcb[stn_id] = !fcb[stn_id];
						timeoutnum[stn_id] =0;
						repeatnum[stn_id] = 0;
						break;
			default :
#ifdef DEBUG
						printf("IEC received data start byte error! %2x :\n",recvbuf[offset]);
#endif

						ClearRecvFault();
//						timeoutnum[stn_id]++;
//						if(initflag[stn_id]==0)
//							repeatnum[stn_id]++;
						break;			
			}//switch(recvbuf[0])
		}//for(;;)
	UnMountDB();
}





  

