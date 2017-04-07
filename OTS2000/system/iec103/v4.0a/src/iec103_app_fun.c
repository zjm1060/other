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
#include <time.h>

#include "\home\ems\system\iec103\v4.0a\inc\iec103_com.h"
#include "\home\ems\system\iec103\v4.0a\inc\iec103_def.h"

#define	DEBUG

extern	unsigned char	sendbuf[1024];
ASDUCOMDDATA	send_data;
ASDUDATA		recv_data;
extern	HANDLE	hRecvSemaphore;
extern	BOOL	resetlinkf[STN_NUM+1],resetframe[STN_NUM+1],initflag[STN_NUM+1];
extern	BOOL	counterflag[STN_NUM+1],ClockSyncflag[STN_NUM+1],interrogationflag[STN_NUM+1];
extern	int		cmd_ack[STN_NUM+1];
extern	BOOL	fcb[STN_NUM+1];
extern	unsigned char	stn_id,comdflag,comdflag1;



void	IEC870_5_Recv()
{
DWORD	dwWaitRecvASDU;
USHORT	recvdatapt,datalength,lengthfactor;
USHORT	baseadd;
short	ana16data;
int 	i,k,datapoint;

unsigned short   millisec;
unsigned char	second,minute, hour;

unsigned char	stnd,type_id,varstru,causetra,infonum,qoc,infmax;
unsigned char	infoaddr,funcode;

printf("============IEC870_5_Recv thread start!!!===========\n");
		

recvdatapt = 0;

for(;;)
	{
	//dwWaitRecvASDU = WaitForSingleObject(hRecvSemaphore,INFINITE);
	if(tmptmp==0) 
	{
		Sleep(500);
		//printf("\n%%%%%%%%%%%%%%%%\n");
		continue;
	}
    else   
	{
		//printf("\n$$$$$$$$$$$$$$$$$$$$\n");  
		tmptmp=0;
	}
#ifdef DEBUG
	printf("IEC870_5_Recv: RecvSempahore set, remote ASDU has received...\n");
#endif

	while(recvdatapt != recv_data.pnt)
		{
		//zm stnd = recv_data.data[recvdatapt][1];
		stnd=GetStnId(recv_data.data[recvdatapt][1]); //zm

		type_id = recv_data.data[recvdatapt][2];
		varstru = recv_data.data[recvdatapt][3];
		causetra = recv_data.data[recvdatapt][4];

		switch(type_id)
			{
			case M_TTM_TA_3:	//protect point with time tag 1
			case M_TMR_TA_3:	//protect point with relative time tag 2
//					infonum = varstru & 0x7F;
					/*
					if(type_id == M_TTM_TA_3)
						datalength = 17;
					else
						datalength = 21;
					if(datalength != recv_data.length[recvdatapt])
						{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
						break;
						}
					*/
					funcode = recv_data.data[recvdatapt][6];
					infoaddr = recv_data.data[recvdatapt][7];
					qoc = recv_data.data[recvdatapt][8] & 0x3;
					if(type_id == M_TTM_TA_3)
						{
						if(causetra==CMD_NACK)	//15H
							{
							printf("STN %d Command NACK\n",stnd);
							comdflag = 0;
							comdflag1 = 0;
							break;
							}
						if(causetra==CMD_ACK)	//14H
							{
							printf("STN %d Command ACK\n",stnd);
							if(comdflag1)
								comdflag1 = 0;
							break;
							}
						}

					infmax = dioinf[stnd-1];
					for(k=0;k<infmax;k++)
						{
						if((infoaddr == dinf[stnd-1][k])&&(funcode == dfun[stnd-1][k]))
							break;
						}
					if(k>=infmax)
						{
						printf("stn %d DIN Fun %d Info addr = %d error!\n",stnd,funcode,infoaddr);
						break;
						}

					if(type_id == M_TTM_TA_3)//zm
					{
/*						memcpy(&millisec,&sendbuf[9],2);
						memcpy(&minute,&sendbuf[11],1);
						memcpy(&hour,&sendbuf[12],1);
*/
						memcpy(&millisec,&recv_data.data[recvdatapt][9],2);
						second = millisec / 1000;
						millisec = millisec % 1000;
						minute = recv_data.data[recvdatapt][11];
						hour = recv_data.data[recvdatapt][12];
					}
					if(type_id == M_TMR_TA_3)//zm
					{
/*						memcpy(&millisec,&sendbuf[13],2);
						memcpy(&minute,&sendbuf[15],1);
						memcpy(&hour,&sendbuf[16],1);
*/
						memcpy(&millisec,&recv_data.data[recvdatapt][13],2);
						second = millisec / 1000;
						millisec = millisec % 1000;
						minute = recv_data.data[recvdatapt][15];
						hour = recv_data.data[recvdatapt][16];
					}

					if(qoc == 2)
						{
#ifdef DEBUG
						printf("stn %d DIN Fun %d Info addr = %d %02d:%02d:%02d %03d\n",stnd,funcode,infoaddr,hour,minute,second,millisec);
						printf("protect point ASDU1 has received info addr=%d sft=%d num=%d ON\n",infoaddr,baseadd,k);
#endif
						ProcYx( stnd-1, k, 1, millisec, second, minute);
						}
					else if(qoc == 1)
						{
#ifdef DEBUG
						printf("stn %d DIN Fun %d Info addr = %d %02d:%02d:%02d %03d\n",stnd,funcode,infoaddr,hour,minute,second,millisec);
						printf("protect point ASDU1 has received info addr=%d sft=%d num=%d OFF\n",infoaddr,baseadd,k);
#endif
						ProcYx( stnd-1, k, 0, millisec, second, minute);
						}
					break;

			case M_SP_NA_3:	//single point GI 40
			case M_DP_NA_3:	//double point GI 42

					if(varstru & 0x80)				//SQ=1
						{
						infonum = varstru & 0x7F;
						datalength = infonum*3+7;
						if(datalength != recv_data.length[recvdatapt])
							{
							printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
									type_id,infonum,recv_data.length[recvdatapt]);
							break;
							}

						infmax = dioinf[stnd-1];
						for(i=0;i<infonum;i++)
							{
							datapoint = i*3+6;
							funcode = recv_data.data[recvdatapt][datapoint];
							infoaddr = recv_data.data[recvdatapt][datapoint+1];
							for(k=0;k<infmax;k++)
								{
								if((infoaddr == dinf[stnd-1][k])&&(funcode == dfun[stnd-1][k]))
									break;
								}
							if(k>=infmax)
								{
								printf("stn %d DIN INF addr = %d error!\n",stnd,infoaddr);
								break;
								}
							
							qoc = recv_data.data[recvdatapt][datapoint+2];
							if(type_id ==M_SP_NA_3)
								{
								if(qoc & 1)
									{
#ifdef DEBUG
									printf("single point GI 40 has received fun %d info addr=%04x ON\n",funcode,infoaddr);
#endif
									ProcYx( stnd-1, k, 1, -1, -1, -1 );
									}
								else
									{
#ifdef DEBUG
									printf("single point GI 40 has received fun %d info addr=%04x OFF\n",funcode,infoaddr);
#endif
									ProcYx( stnd-1, k, 0, -1, -1, -1 );
									}
								}
							else
								{
								switch(qoc & 0x03)
									{
									case 0:	
										ProcYx( stnd-1, k,   2, -1, -1, -1 );
										break;
									case 1:	
										ProcYx( stnd-1, k,   0, -1, -1, -1 );
										break;
									case 2:
										ProcYx( stnd-1, k,   1, -1, -1, -1 );
										break;
									case 3:
										ProcYx( stnd-1, k,   2, -1, -1, -1 );
										break;
									}
								}//if(type_id ==M_SP_NA_3)
							}//for()
						}//if(SQ)
					else
						{
						infonum = varstru & 0x7F;		//SQ=0
						datalength = infonum+9;
						if(datalength != recv_data.length[recvdatapt])
							{
							printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
									type_id,infonum,recv_data.length[recvdatapt]);
							break;
							}

						funcode = recv_data.data[recvdatapt][6];
						infoaddr = recv_data.data[recvdatapt][7];
						infmax = dioinf[stnd-1];
						baseadd = diobase[stnd-1];
						for(k=0;k<infmax;k++)
							{
							if((infoaddr == dinf[stnd-1][k])&&(funcode == dfun[stnd-1][k]))
								break;
							}
						if(k>=infmax)
							{
							printf("stn %d DIN INF addr = %d error!\n",stnd,infoaddr);
							break;
							}
						
						for(i=0;i<infonum;i++)
							{
							datapoint = i+8;

							qoc = recv_data.data[recvdatapt][datapoint];
							if(type_id ==M_SP_NA_3)
								{
								if(qoc & 1)
									{
#ifdef DEBUG
									printf("single point GI 40 has received info addr=%04x ON\n",infoaddr);
#endif
									ProcYx( stnd-1, k,   1, -1, -1, -1 );
									}
								else
									{
#ifdef DEBUG
									printf("single point GI 40 has received info addr=%04x OFF\n",infoaddr);
#endif
									ProcYx( stnd-1, k,   0, -1, -1, -1 );
									}
								}
							else
								{
								switch(qoc & 0x03)
									{
									case 0:	
										ProcYx( stnd-1, k,   2, -1, -1, -1 );	
										break;
									case 1:	
										ProcYx( stnd-1, k,   0, -1, -1, -1 );	
										break;
									case 2:
										ProcYx( stnd-1, k,   1, -1, -1, -1 );
										break;
									case 3:
										ProcYx( stnd-1, k,   2, -1, -1, -1 );
										break;
									}
								}//if(type_id ==M_SP_NA_3)
							}//for()
						}
						break;

			case M_SP_TA_3:	//single point with time tag 41
			case M_DP_TA_3:	//double point with time tag 43

					infonum = varstru & 0x7F;		//SQ=1
					datalength = 7*infonum+7;
					if(datalength != recv_data.length[recvdatapt])
						{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
						break;
						}
					infmax = dioinf[stnd-1];

					for(i=0;i<infonum;i++)
						{
						datapoint = 7*i+6;
						baseadd = diobase[stnd-1];
						funcode = recv_data.data[recvdatapt][datapoint];
						infoaddr = recv_data.data[recvdatapt][datapoint+1];
						for(k=0;k<infmax;k++)
							{
							if((infoaddr == dinf[stnd-1][k])&&(funcode == dfun[stnd-1][k]))
								break;
							}
						if(k>=infmax)
							{
							printf("stn %d DIN INF addr = %d error!\n",stnd,infoaddr);
							break;
							}
						
						qoc = recv_data.data[recvdatapt][datapoint+2];
						memcpy(&millisec,&recv_data.data[recvdatapt][datapoint+3],2);
						second = millisec / 1000;
						millisec = millisec % 1000;
						minute = recv_data.data[recvdatapt][datapoint+5];
						hour = recv_data.data[recvdatapt][datapoint+6];
						if(type_id ==M_SP_TA_3)
							{
							if(qoc & 1)
								{
#ifdef DEBUG
								printf("single point time tag 41 has received info addr=%04x ON\n",infoaddr);
#endif
								ProcYx( stnd-1, k, 1, millisec, second, minute);
//								ProcYx( stnd-1, k,   1, -1, -1, -1 );
								}
							else
								{
#ifdef DEBUG
								printf("single point time tag 41 has received info addr=%04x OFF\n",infoaddr);
#endif
								ProcYx( stnd-1, k, 0, millisec, second, minute);
//								ProcYx( stnd-1, k,   0, -1, -1, -1);
								}
							}
						else
							{
							switch(qoc & 0x03)
								{
								case 0:	
									ProcYx( stnd-1, k, 2, millisec, second, minute);
//									ProcYx( stnd-1, k,   2, -1, -1, -1 );
									break;
								case 1:	
									ProcYx( stnd-1, k, 0, millisec, second, minute);
//									ProcYx( stnd-1, k,   0, -1, -1, -1 );	
									break;
								case 2:
									ProcYx( stnd-1, k, 1, millisec, second, minute);
//									ProcYx( stnd-1, k,   1, -1, -1, -1 );
									break;
								case 3:
									ProcYx( stnd-1, k, 3, millisec, second, minute);
//									ProcYx( stnd-1, k,   2, -1, -1, -1 );
									break;
								}
							}
						}//for()
					break;

			case M_SS_NA_3:	//Total GI Single change detect 44
			case M_DS_NA_3:	//Total GI  double change detect 46
/*zm---->
					infonum = varstru & 0x7F;		//SQ=0
					datalength = infonum*5+9;
					if(datalength != recv_data.length[recvdatapt])
						{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
						break;
						}
					infoaddr = recv_data.data[recvdatapt][7];
					if(infoaddr != STADDR)
						{
						printf("received ASDU 44/46 INF =%d error!\n",infoaddr);
						break;
						}
#ifdef DEBUG
					printf("GI status detect 44/46 has received info addr=%02d\n",
									infoaddr);
#endif
					baseadd = diobase[stnd-1];
					if(stnd <=4 )	//bh din data
						{
						baseadd = baseadd + 2;
						st16data = recv_data.data[recvdatapt][8];
						st16data = st16data << 8;
						dinstat = lcu[gy_lcu_number].din_new[baseadd] & 0x00FF;
						lcu[gy_lcu_number].din_new[baseadd] = dinstat | st16data;
//						lcu[gy_lcu_number].din_new[baseadd] = lcu[gy_lcu_number].din_new[baseadd] | st16data; 
						scan_din(comm_dev[gy_lcu_number],(WORD)(baseadd*16),(WORD)((baseadd+1)*16),0,0);
						break;
						}
					for(i=0;i<infonum;i++)
						{
						datapoint = i*5+8;
						memcpy(&st16data,&recv_data.data[recvdatapt][datapoint],2);
//						memcpy(&cd16data,&recv_data.data[recvdatapt][datapoint+2],2);
						lcu[gy_lcu_number].din_new[baseadd] = st16data; 
//						din_new[baseadd] = st16data;
						baseadd ++;
						}//for()
					scan_din(comm_dev[gy_lcu_number],(WORD)(diobase[stnd-1]*16),(WORD)((diobase[stnd-1]+infonum)*16),0,0);
<---zm*/					break;

			case M_SS_TA_3:	//Single change detect time tag 45
			case M_DS_TA_3:	//double change detect time tag 47
/*zm----->
					infonum = varstru & 0x7F;		//SQ=1
					datalength = infonum*11+7;
					if(datalength != recv_data.length[recvdatapt])
						{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
						break;
						}
					if(stnd <=4 )	//bh din data
						{
						infoaddr = recv_data.data[recvdatapt][7];
						if(infoaddr != STADDR)
							{
							printf("received ASDU 45/47 INF =%d error!\n",infoaddr);
							break;
							}
#ifdef DEBUG
						printf("status detect 45/47 has received info addr=%02d\n",
										infoaddr);
#endif
						baseadd = diobase[stnd-1];
						baseadd = baseadd + 2;
						st16data = recv_data.data[recvdatapt][8];
						st16data = st16data << 8;
						dinstat = lcu[gy_lcu_number].din_new[baseadd] & 0x00FF;
						lcu[gy_lcu_number].din_new[baseadd] = dinstat | st16data;
						scan_din(comm_dev[gy_lcu_number],(WORD)(baseadd*16),(WORD)((baseadd+1)*16),0,0);
						break;
						}
					for(i=0;i<infonum;i++)
						{
						datapoint = i*11+7;
						infoaddr = recv_data.data[recvdatapt][datapoint];
						if(infoaddr == STADDR)
							baseadd = diobase[stnd-1];
						else if(infoaddr == STADDR+16)
							{
							baseadd = diobase[stnd-1];
							baseadd ++;
							}
						else
							break;
#ifdef DEBUG
						printf("status detect 45/47 has received info addr=%02d\n",
										infoaddr);
#endif
						memcpy(&st16data,&recv_data.data[recvdatapt][datapoint+1],2);
						memcpy(&cd16data,&recv_data.data[recvdatapt][datapoint+3],2);
						lcu[gy_lcu_number].din_new[baseadd] = st16data;
						}//for()
					scan_din(comm_dev[gy_lcu_number],(WORD)(diobase[stnd-1]*16),(WORD)((diobase[stnd-1]+infonum)*16),0,0);
<---zm*/					break;

			case M_MEVII_NA_3:	//measure value VII 50 P129
					infonum = varstru & 0x7F;			//number of information object
					if(varstru & 0x80 )
						{
#ifdef DEBUG
						printf("stn=%d Cycle Measured value 50 M_MEVII_NA_3 SQ=1\n",stnd);
#endif
						datalength = infonum*3+7;
						lengthfactor = 3;
						if(datalength != recv_data.length[recvdatapt])
							{
							printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
									type_id,infonum,recv_data.length[recvdatapt]);
							break;
							}
						infmax = anainf[stnd-1];
						infoaddr = recv_data.data[recvdatapt][7];
						/*zm
						if(infoaddr != ANAADDR)
							{
							printf("received ASDU 50 sq=1 INF =%d error!\n",infoaddr);
							break;
							}
						zm*/
						for(i=0;i<infonum;i++)
							{
							datapoint = lengthfactor*i+7;
							infoaddr = recv_data.data[recvdatapt][datapoint];
							memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint+1],2);	 //address of object
#ifdef DEBUG
							printf("INF=%d value= %x %d\n",infoaddr,ana16data,ana16data>>3);
#endif
							for(k=0;k<infmax;k++)
								{
								if(infoaddr == ainf[stnd-1][k])
									break;
								}
							if(k>=infmax)
								{
								printf("stn %d ANA INF addr = %d error!\n",stnd,infoaddr);
								continue;
								}
							ProcYc( stnd-1, k, (short)(ana16data>>3) );
							}
						}
					else
						{
#ifdef DEBUG
						printf("stn=%d 50 Measured value M_MEVII_NA_3\n",stnd);
#endif
						datalength = infonum*2+8;
						lengthfactor = 2;
						if(datalength != recv_data.length[recvdatapt])
							{
							printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
									type_id,infonum,recv_data.length[recvdatapt]);
							break;
							}
						infoaddr = recv_data.data[recvdatapt][7];
						infmax = anainf[stnd-1];
						for(k=0;k<infmax;k++)
							{
							if(infoaddr == ainf[stnd-1][k])
								break;
							}
						if(k>=infmax)
							{
							printf("stn %d ANA INF addr = %d error!\n",stnd,infoaddr);
							continue;
							}
						for(i=0;i<infonum;i++)
							{
							datapoint = lengthfactor*i+8;
							memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint],2);	 //address of object
#ifdef DEBUG
//							printf("INF=%d value= %x,%d\n",infoaddr,ana16data,ana16data >> 3 );
#endif
//							ain_st[baseadd] = aistatus[stnd-1][i];
							ProcYc( stnd-1, k+i, (short)(ana16data>>3) );
							}
					}
					break;

			case M_MEIII_NA_3:	//measure value VIII 15 (NARI Defined)
					infonum = varstru & 0x7F;			//number of information object
					if(!(varstru & 0x80 ))
					{
#ifdef DEBUG
						printf("stn=%d Cycle Measured value 15 M_MEIII_NA_3 SQ=0\n",stnd);
#endif
						datalength = infonum*2+8;
						lengthfactor = 2;
						if(datalength != recv_data.length[recvdatapt])
						{
							printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
									type_id,infonum,recv_data.length[recvdatapt]);
							break;
						}
						infoaddr = recv_data.data[recvdatapt][7];
						infmax = anainf[stnd-1];
						if(infoaddr != ainf[stnd-1][0])
						{
							printf("received ASDU 15 sq=0 INF =%d not equal %d error! check yc table!\n",infoaddr,ainf[stnd-1][0]);
							break;
						}
						for(i=0;i<infonum;i++)
						{
							datapoint = lengthfactor*i+8;
							memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint],2);	 //address of object
#ifdef DEBUG
							printf("INF=%d value= %02xH\n",infoaddr,ana16data);
#endif
							ProcYc( stnd-1, i, (short)(ana16data>>3) );
						}
					}
					else
					{
#ifdef DEBUG
						printf("stn=%d Cycle Measured value 15 M_MEIII_NA_3 SQ=1 Wrong!\n",stnd);
#endif
					}
					break;

			case M_MEI_NA_3:	//measure value VI 3 P27
			case M_MEII_NA_3:	//measure value II 9 P29
					infonum = varstru & 0x7F;			//number of information object
					if(!(varstru & 0x80 ))
					{
#ifdef DEBUG
						printf("stn=%d Cycle Measured value 3 M_MEI_NA_3 SQ=0\n",stnd);
#endif
						datalength = infonum*2+8;
						lengthfactor = 2;
						if(datalength != recv_data.length[recvdatapt])
						{
							printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
									type_id,infonum,recv_data.length[recvdatapt]);
							break;
						}
						infoaddr = recv_data.data[recvdatapt][7];
						infmax = anainf[stnd-1];
						if(infoaddr != ainf[stnd-1][0])
						{
							printf("received ASDU 3 sq=0 INF =%d not equal %d error! check yc table!\n",infoaddr,ainf[stnd-1][0]);
							break;
						}
						baseadd = anabase[stnd-1];
						for(i=0;i<infonum;i++)
						{
							datapoint = lengthfactor*i+8;
							memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint],2);	 //address of object
#ifdef DEBUG
							printf("INF=%d value= %02xH\n",infoaddr,ana16data);
#endif
							ProcYc( stnd-1, i, (short)(ana16data>>3) );
						}
					}
					else
					{
#ifdef DEBUG
						printf("stn=%d Cycle Measured value 3 M_MEI_NA_3 SQ=1 Wrong!\n",stnd);
#endif
					}
					break;

			case M_TME_TA_3:	//measure value with relative time 4 P27
//			case M_MEII_NA_3:	//measure value II 9 P29
			case M_MEIII_TA_3:	//measure value II 32 P126
			case M_MEIV_TA_3:	//measure value II 33 P126
			case M_MEV_TA_3:	//measure value II 34 P127
			case M_MEVI_TA_3:	//measure value II 35 P128
					infonum = varstru & 0x7F;			//number of information object
#ifdef DEBUG
					printf("++++++stn=%d Measured value tpye=%d++++++ not processes\n",stnd,type_id);
#endif
					lengthfactor = 2;
					datalength = lengthfactor*infonum+6;
					break;

			case M_IT_NA_3:	//pulse value 36 P132
			case M_IT_TA_3:	//pulse value 37 P133
				/*zm---->
					infonum = varstru & 0x7F;			//number of information object
					if(type_id == M_IT_NA_3)
						{
						lengthfactor = 5;
						datalength = 5*infonum + 9;
						}
					else
						{
						lengthfactor = 9;
						datalength = 9*infonum + 9;
						}
					if(datalength != recv_data.length[recvdatapt])
						{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
						break;
						}
#ifdef DEBUG
					printf("stn=%d pulse value M_IT_NA_3\n",stnd);
#endif
					counterflag[stnd] = 0;

					infoaddr = recv_data.data[recvdatapt][7];
					if(infoaddr != PIADDR)
						{
						printf("received Pulse ASDU %d INF =%d error!\n",type_id,infoaddr);
						break;
						}
#ifdef DEBUG
					printf("Pulse ASDU %d has received info addr=%02d\n",type_id,infoaddr);
#endif
					baseadd = pibase[stnd-1];
					for(i=0;i<infonum;i++)
						{
						datapoint = lengthfactor*i+8;
						memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint+2],2);	 //address of object
#ifdef DEBUG
						printf("INF=%d value= %d\n",infoaddr,ana16data);
#endif
						lcu[gy_lcu_number].aiall[baseadd].status = 0;
						lcu[gy_lcu_number].aiall[baseadd].data = ana16data;
						baseadd ++;
						}
		<----zm*/	break;

			case M_ST_NA_3:	//step point without time tag
			case M_ST_TA_3:	//step point with time tag
					infoaddr = recv_data.data[recvdatapt][7];
#ifdef DEBUG
					printf("++++++stn=%d step point tpye=%d INF =%d not processes++++++\n",stnd,type_id,infoaddr);
#endif
					break;

			case M_TGI_NA_3:	//integrated totals terminated+
					datalength = 9;
					if(datalength != recv_data.length[recvdatapt])
						{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
						break;
						}
					if(causetra  == 0xA)
						{
#ifdef DEBUG
						printf("integrated totals terminated received\n");
#endif
						if((stnd == 5)||(stnd == 7))
							initflag[stnd] = 3;
						else
							initflag[stnd] = 0;
						interrogationflag[stnd] = 0;
						}
					else if(causetra  == 0x9)
						{
#ifdef DEBUG
						printf("integrated totals ACK received\n");
#endif
						}
					break;

			case C_CI_NA_3:	//pulse counter interrogation command 88
					datalength = 10;
					if(datalength != recv_data.length[recvdatapt])
						{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
						break;
						}
#ifdef DEBUG
					printf("pulse counter interrogation command ACK received\n");
#endif
					initflag[stnd] = 0;
					counterflag[stnd] = 0;
					break;

			case C_DC_NA_3:	//confirm of double point control command
			case C_RC_NA_3:	//confirm of regulating step command
			case C_CC_NA_3: //confirm of control command 
					if(causetra==CMD_NACK)	//15H
						{
#ifdef DEBUG
						printf("STN %d Command %d NACK\n",
							stnd,recv_data.data[recvdatapt][8]);
#endif
						comdflag = 0;
						}
#ifdef DEBUG
/*
					else if(causetra==CMD_ACK)	//14H
						{
						printf("STN %d Command %x ACK\n",
							stnd,recv_data.data[recvdatapt][8]);
						}
					else
*/						{
						printf("======STN %d Command %d %x\n",
							stnd,type_id,recv_data.data[recvdatapt][8]);
						}
#endif
					infoaddr = recv_data.data[recvdatapt][7];
					qoc = recv_data.data[recvdatapt][8];
					switch(qoc & 0xC0)
						{
						case 0x80:		//S/E=1,ACT=0 select
#ifdef DEBUG
							printf("IEC SC/DC/RC %u select command INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
							if(comdflag == 1)
								comdflag = 2;
							break;
						case 0x0:		//S/E=0,ACT=0 exec
#ifdef DEBUG
							printf("IEC SC/DC/RC %u exec command INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
							comdflag = 0;
							break;
						case 0xC0:		//S/E=1,ACT=1 DEACT
#ifdef DEBUG
							printf("IEC SC/DC/RC %u DEACT command INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
							comdflag = 0;
							break;
						default: 
							printf("IEC SC/DC/RC %u command INF=%d com=%x ERROR\n",type_id,infoaddr,qoc);
						}					
					break;

			case C_SE_NA_3:	//confirm of set-point command
					
					if(causetra == CMD_ACK)		//20 positive 21 negative confi
						{
						cmd_ack[stnd] = 1;		//command confirm
#ifdef DEBUG
						printf("IEC set-point command is ACK by RTU!\n");
#endif
						}
					else if(causetra == CMD_NACK)
						{
						cmd_ack[stnd] = -1;		//negative confirm
#ifdef DEBUG
						printf("IEC set-point command is NACK by RTU!\n");
#endif
						}
					
					break;

			case M_SYN_TA_3:	//confirm of clock synchronization command
					
					if(causetra == CMD_NACK)		//21 0 positive 1 negative confirm
						{
						cmd_ack[stnd] = -1;		//negative confirm
#ifdef DEBUG
						printf("clock synchronization command is NACK by remote system!\n");
#endif
						}
					else if(causetra == CMD_ACK)			//20
						{
						cmd_ack[stnd] = 1;		//execute command confirm
						ClockSyncflag[stnd] = 0;
#ifdef DEBUG
						printf("clock synchronization command is ACK by remote system!\n");
#endif
						}
					else
						printf("clock synchronization command: Received cause of transmission=%2x wrong!\n",causetra);
					
					break;

			case M_IRC_NA_3:	//idential flag

					if(causetra==RESET_CU)	//4H
					{
#ifdef DEBUG
						printf("IEC103 idential flag 5 reset communication unit is received!\n");
#endif
						resetlinkf[stn_id]=0;
					}
					else if(causetra==RESET_FCB)	//3H
					{
#ifdef DEBUG
						printf("IEC103 idential flag 3 RESET FCB is received!\n");
#endif
						resetframe[stn_id]=0;
						initflag[stn_id]=0;
						fcb[stn_id]=0;
					}
					else if(causetra==START)	//5H
					{
#ifdef DEBUG
						printf("IEC103 idential flag 5 start or restart is received!\n");
#endif
					}
					break;
/*
#ifdef DEBUG
					printf("IEC idential flag 5 is received!\n");
#endif
					if(initflag[stnd] ==1)
						{
						initflag[stnd] =2;
						}
					break;
*/
			case M_LRD_TA_3:	//disturbance data with time tag
#ifdef DEBUG
					printf("ASDU23 disturbance data table is received!\n");
#endif
					break;

			default :
					printf("IEC type_id = %u is unavailable at this time!\n",type_id);
					break;
			}//switch(type_id)

		recvdatapt++;
		if( recvdatapt >= ASDU_ENTRY_NUMBER )
			recvdatapt = 0;
		}//while(recvdatapt!= recv_data.pnt)
			
	}//for(;;)
}