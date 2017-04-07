/************************************************************
*  FILE NAME:           iec_com.C      						*
*  FUNCTION:            Controlling Station Main Process	*
*						Link Layer Communication Program	*
*  NAME :  					        						*
*  DATE:                March,2007                			*
*															*
************************************************************/

#include <stdio.h>
#include <string.h>
#include <windows.h>         
#include <winsock.h>
#include "..\inc\iec101_com.h"

#define	DEBUG
      
extern 	HANDLE ComInit(char *COM,DWORD Baud_Rate,
                      BYTE ByteSize,BYTE Parity,BYTE StopBits);

void	IEC870_5_Recv(void);
BOOL	RequestLinkStatus(void);
BOOL	ResetLinkStatus(void);
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
BOOL	GenSetPCtrlTable(void);

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
BOOL	resetlinkf[STN_NUM+1],initflag[STN_NUM+1],pollflag;
BOOL	counterflag[STN_NUM+1],ClockSyncflag[STN_NUM+1],interrogationflag[STN_NUM+1];
unsigned int	repeatnum[STN_NUM+1],timeoutnum[STN_NUM+1];
int		cmd_ack[STN_NUM+1],localinitfg;
unsigned char	stn_id,comdflag,comdflag1;

void main(void)
{
	HANDLE	hThread_Recv,hThread_Hdb;
	int		ThreadID_Recv,ThreadID_Hdb;
	BOOL	fSuccess;					  	
	BOOL	dataflag[STN_NUM+1],dataflow[STN_NUM+1];
	LONG	cMaxCount,cInitCount;
	int	byte_recvd;

	unsigned int	sendbufheadpt,recvbufheadpt;
	unsigned char	chksum,ctrlfield,ctrlfunc,offset;
	unsigned int	i,ses,wat_num[STN_NUM+1],cyclenum[STN_NUM+1];
	
	//Initialization
	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	lan_init_all ();
	

	cMaxCount= 10;
	cInitCount = 0;
	
	//Open Port
	hPort_com=ComInit(COM_PORT,Com_Baud_Rate,8,EVENPARITY,ONESTOPBIT);
	printf("\n %s init compelete !!!\n\n",COM_PORT);

	//Signal
	hRecvSemaphore = CreateSemaphore(NULL,cInitCount,cMaxCount,"sem_recv");

	if (hRecvSemaphore == NULL)
		printf("Error in CreateSemaphore sem_recv:%d\n",GetLastError());
	else
		printf("sem_recv: OK!\n");	


	//Create Receive Thread
	hThread_Recv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEC870_5_Recv,
                                 NULL, CREATE_SUSPENDED, &ThreadID_Recv);
	if (hThread_Recv == NULL)
		printf("Error in CreateThread ThreadID_Recv:%d\n",GetLastError());
	else
		printf("hThread_Recv: %d OK!\n",hThread_Recv);

 	
	ResumeThread( hThread_Recv);
	SetThreadPriority(hThread_Recv,THREAD_PRIORITY_BELOW_NORMAL);
	//Create WriteHdb Thread
	
	hThread_Hdb = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Write_Hdb,
                                 NULL, CREATE_SUSPENDED, &ThreadID_Hdb);
	if (hThread_Hdb == NULL)
		printf("Error in CreateThread ThreadID_Hdb:%d\n",GetLastError());
	else
		printf("hThread_Hdb: %d OK!\n",hThread_Hdb);

	SetThreadPriority(hThread_Hdb,THREAD_PRIORITY_BELOW_NORMAL);	 
	ResumeThread( hThread_Hdb);
	  // IEC 870-5 Link layer communication service

	recvbufheadpt = 0;
	send_data.pnt = 0;
	recv_data.pnt = 0;
	sendbufheadpt = 0;
	localinitfg = 1;
	ses = 0;
	cmdbuf[6] = 0;

	if(PurgeComm(hPort_com,PURGE_TXABORT|PURGE_RXABORT|
					PURGE_RXCLEAR|PURGE_TXCLEAR)==FALSE)
		printf("INIT: PurgeComm ERRNO= %d \n",GetLastError());


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
		if(stn_id ==1)
			ClockSyncflag[stn_id] = 1;
		else
			ClockSyncflag[stn_id] = 0;
		interrogationflag[stn_id] = 1;
		counterflag[stn_id] = 1;
		repeatnum[stn_id] = 0;
		timeoutnum[stn_id] = 0;
		cmd_ack[stn_id] = 0;
		wat_num[stn_id] = 0;
		cyclenum[stn_id] = 0;
	}
	sendbufheadpt = send_data.pnt;

	stn_id = 1;
	
	for(;;)
		{
		
		Sleep(1000);
		//RepeatTransmit
		if(repeatnum[stn_id] !=0)
			{
			if(repeatnum[stn_id] < REPEAT_NUM)
				{
				if(!RepeatTransmit())
					continue;
				}
			else
				{
				repeatnum[stn_id] = 0;
				comdflag1 = 0;
				ses = SES_NUM;
				continue;
				}
			}
		//CommandExec
		else if((comdflag == 2) && (cmdbuf[6] != 0))
			{
			stn_id = cmdbuf[5];
			ses = 0;
			if(!CommandExec())
				continue;
			comdflag = 3;
			comdflag1 = 1;
			cmdbuf[6] = 0;			//cmd type clear zero
			}
		else
			{
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
					initflag[stn_id] = 2;
					if(stn_id == 1)
						ClockSyncflag[stn_id] = 1;
					interrogationflag[stn_id] = 1;
					counterflag[stn_id] = 1;
					repeatnum[stn_id] = 0;
					timeoutnum[stn_id] = 0;
	//				cmd_ack[stn_id] = 0;
					wat_num[stn_id] = 0;
					cyclenum[stn_id] = 0;
					}
				}
			if(timeoutnum[stn_id] >= TIMEOUT_NUM)
				{
	//			pollflag = 0;
				initflag[stn_id] = 1;
				resetlinkf[stn_id] = 1;
				if(stn_id == 1)
					ClockSyncflag[stn_id] = 1;
				interrogationflag[stn_id] = 1;
				counterflag[stn_id] = 1;
				wat_num[stn_id] = 0;
				repeatnum[stn_id] = 0;
				timeoutnum[stn_id] = 0;
	//			break;
	//			continue;
				}
			//reset link status
			if(initflag[stn_id] ==1)
				{
				if(resetlinkf[stn_id] == 1)
					{
					if(!ResetLinkStatus())
					continue;
					}
				else
					{
					if(!RequestLinkStatus())
					continue;
					}
				}
			//ClockSynchronisation	
			else if((initflag[stn_id] ==2) && (ClockSyncflag[stn_id] == 1) )
				{
//				comdflag = 1;
				ClockSyncflag[stn_id] = 0;
				ClockSynchronisation();
			}
			//send one cmd in send_data
			else if(sendbufheadpt != send_data.pnt)		
				{
				if(!CommandSend(sendbufheadpt))
					continue;
				comdflag = 1;
				comdflag1 = 1;
				stn_id = cmdbuf[5];
				ses = 0;
				sendbufheadpt ++;
				if(sendbufheadpt>= CMD_ENTRY_NUMBER)
					sendbufheadpt = 0;
				}
			//ask for all data
			else if((initflag[stn_id] ==2) && (interrogationflag[stn_id] != 0))
				{
				if(interrogationflag[stn_id] == 1)
					{
					if(!GeneralInterrogation())
						continue;
//					interrogationflag[stn_id] = 0;
					}
				//ask for class one data
				else if(dataflag[stn_id])
					{
					if(!RequestUserDataClass1())
						continue;
					}
				//ask for class two data
				else	
					{
					if(!RequestUserDataClass2())
						continue;
					}
				}
			//ask for counter interogation
			else if((initflag[stn_id] ==3) && (counterflag[stn_id] != 0)) 
				{
				//ask for counter data
				if(counterflag[stn_id] == 1)
					{
					if(!CounterInterrogation())
						continue;
					}
				//ask for class one data
				else if(dataflag[stn_id])
					{
					if(!RequestUserDataClass1())
						continue;
					}
				//ask for class two data
				else	
					{
					if(!RequestUserDataClass2())
						continue;
					}
				}
			else if(dataflag[stn_id])
				{
				if(!RequestUserDataClass1())
					continue;
				}
			else	
				{
				if(!RequestUserDataClass2())
					continue;
				}
			}//repeat,cmd
		//read from com port
		memset(recvbuf,0,1024);
		fSuccess = ReadFile(hPort_com,(LPSTR)recvbuf,VARFRAMELEN,&byte_recvd,NULL);
		//if read error
		if(!fSuccess)
			{
			if(comdflag1)
				printf("CommandSend: ReadFile ERRNO= %d \n",GetLastError());
			else
				printf("RequestData: ReadFile ERRNO= %d \n",GetLastError());
			ClearRecvFault();
			continue;
			}
		//if read byte number is zero
		if(byte_recvd == 0)
			{
			timeoutnum[stn_id]++;
//			if((initflag[stn_id]==0)||(comdflag1))
				repeatnum[stn_id]++;
			if(comdflag1)
				{
				printf("=!!!!!=CommandSend: timeout! No: %d",timeoutnum[stn_id]);
				}
			else
				printf("RequestData: timeout! No: %d",timeoutnum[stn_id]);
			continue;
			}

#ifdef DEBUG

		if(comdflag1)
			{
			printf("\n=+=+=+=Waiting CMD_ACK! Received STN %d %u bytes cmdflag=%d %d initflag=%d :\n",stn_id,byte_recvd,comdflag1,comdflag,initflag[stn_id]);
			PrintRecvData(byte_recvd);
			}
		else
			{
			printf("\n---received STN %d %u bytes cmdflag=%d %d initflag=%d :\n",stn_id,byte_recvd,comdflag1,comdflag,initflag[stn_id]);
			PrintRecvData(byte_recvd);
			}
#endif
//		printf("\n---received STN %d %u bytes cmdflag=%d %d initflag=%d :\n",stn_id,byte_recvd,comdflag1,comdflag,initflag[stn_id]);
		
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
		
		for(i=0; i<=(unsigned)(byte_recvd-5); i++)
		{
			if((recvbuf[i]==0x10)||(recvbuf[i]==0x68))
				break;
		}
		offset = i;

		switch (recvbuf[offset])
			{

			//Single byte response NACK	for no data class2 and class1

			case 0x10:
						//fix frame data
						if(recvbuf[offset+2] != stn_id)
							{
							printf("received station id = %d not equal to request %d\n",recvbuf[offset+2],stn_id);
//							if(comdflag1)
								repeatnum[stn_id]++;
							break;
							}
						if((byte_recvd<FIXFRAMELEN)||(recvbuf[offset+4]!=0x16))
							{
							ClearRecvFault();
							break;
							}
						//check sum
						chksum = recvbuf[offset+1] + recvbuf[offset+2];	
			
						if(chksum != recvbuf[offset+3])
							{
							ClearRecvFault();
							break;
							}
						//ctrl field
						ctrlfield= recvbuf[offset+1] & 0xc0;
						//if have data
						if(recvbuf[offset+1] & 0x20)	   //if ACD = 1
							dataflag[stn_id] = 1;
						else
							dataflag[stn_id] = 0;
						//if have more data
						if(recvbuf[offset+1] & 0x10)	   //if DFC = 1
							dataflow[stn_id] = 1;
						else
							dataflow[stn_id] = 0;
						
						//function code
						ctrlfunc = recvbuf[offset+1] & 0x0f;

						switch(ctrlfunc)
							{
							case 0:			//ACK
#ifdef DEBUG
								printf("Station %d ACK or reset link response\n",recvbuf[offset+2]);
#endif
								if(comdflag1)
									comdflag1 = 0;
								if((initflag[stn_id] ==1)&&(resetlinkf[stn_id] == 1))
									{
									resetlinkf[stn_id] = 0;
									}
/*								else if((initflag[stn_id] ==2) && (ClockSyncflag[stn_id] == 1) )
									{
									initflag[stn_id] =3;
									ClockSyncflag[stn_id] = 0;
									wat_num[stn_id] = 0;
									}
*/
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
								if((initflag[stn_id] ==1)&&(resetlinkf[stn_id] == 0))
									{
									initflag[stn_id] =2;
									}
								break;
							default :
								printf("Error response function code %2x\n",ctrlfunc);
								break;
							}
						timeoutnum[stn_id] =0;
						repeatnum[stn_id] = 0;
						break;

//variable frame for requested data

			case 0x68:
						
							//station id
							if(recvbuf[offset+5] != stn_id)
							{
							printf("received station id = %d not equal to request %d\n",recvbuf[offset+5],stn_id);
							repeatnum[stn_id]++;
							break;
							}

						if(byte_recvd<MINFRAMELEN)
							{
							ClearRecvFault();
							break;
							}
						//if data invalid	
						if((recvbuf[offset+3]!=0x68)||(recvbuf[offset+1]!= recvbuf[offset+2])||(byte_recvd<(DWORD)(recvbuf[offset+1]+6)))
							{
							ClearRecvFault();
							break;
							}
						//end byte check
						byte_recvd = recvbuf[offset+1]+6;
						if(recvbuf[offset+byte_recvd - 1] != 0x16)
							{
							ClearRecvFault();
							break;
							}
						//check sum
						chksum = 0;
						for(i=4;i<byte_recvd - 2;i++)
							chksum = chksum + recvbuf[offset+i];	
		
						if(chksum != recvbuf[offset+byte_recvd - 2])
							{
							ClearRecvFault();
							break;
							}
						//ctrl function code
						ctrlfunc = recvbuf[offset+4] & 0x0f;
						
						//get wanting request
						if((ctrlfunc == 8)||(ctrlfunc == 0))
							{
#ifdef DEBUG
							printf("##################A Variable Frame is Received! pt=%d\n",recvbufheadpt);
#endif
							memset(&recv_data.data[recvbufheadpt],0,VARFRAMELEN);
							memcpy(&recv_data.data[recvbufheadpt],&recvbuf[offset+4],byte_recvd - 6);
							recv_data.length[recvbufheadpt] =(unsigned short) byte_recvd - 6;
							recvbufheadpt ++;
							if(recvbufheadpt >= ASDU_ENTRY_NUMBER)
								recvbufheadpt = 0;
							recv_data.pnt = recvbufheadpt;
							//signal
							ReleaseSemaphore(hRecvSemaphore,1,NULL);
							}
							Sleep(100);
						if(recvbuf[offset+4] & 0x20)	   //if ACD =1
							dataflag[stn_id] = 1;
						else
							dataflag[stn_id] = 0;

						if(recvbuf[offset+4] & 0x10)	   //if DFC = 1
							dataflow[stn_id] = 1;
						else
							dataflow[stn_id] = 0;

						timeoutnum[stn_id] =0;
						repeatnum[stn_id] = 0;
						break;
			default :
#ifdef DEBUG
						printf("IEC received data start byte error! %2x :\n",recvbuf[offset]);
#endif

						ClearRecvFault();

						break;			
			}//switch(recvbuf[0])
		}//for(;;)
	UnMountDB();
}
