/************************************************************
*  FILE NAME:           iec101cc_fun.cpp						*
*  FUNCTION:            Controlled Station					*
*						Basic Communication Function		*
*  NAME :  			              							*
*  DATE:                July,2002                			*
*															*
************************************************************/

#include "/usr/gwaysw/gateway/v1.0a/inc/stdafx.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_LogFile.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_SharedMemory.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_CWatchDog.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_RealTimeValues.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_EventFifo.h"
#include "/usr/gwaysw/gateway/v1.0a/inc/PAU_Command.h"
#include "../inc/iec101cc_com.h"

//#define	DEBUG

bool CIEC101Slave::RespondLinkStatus(void)
{
	m_nSendBuf[0] = 0x10;
	m_nSendBuf[1] = 0x8B;	//Link OK
	
	if(m_bACD)
		m_nSendBuf[1] = m_nSendBuf[1] | 0x20;

	m_nSendBuf[2] = m_nStnId;
	m_nSendBuf[3] = m_nSendBuf[1] + m_nSendBuf[2];
	m_nSendBuf[4] = 0x16;
	m_dwByteToSend = FIXFRAMELEN;

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nRespondLinkStatus: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nRespondLinkStatus: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::ResetLinkStatus(void)
{
	m_nSendBuf[0] = 0x10;
	m_nSendBuf[1] = 0x80;
	m_nSendBuf[2] = m_nStnId;
	if(m_bACD)
		m_nSendBuf[1] = m_nSendBuf[1] | 0x20;

	m_nSendBuf[3] = m_nSendBuf[1] + m_nSendBuf[2];
	m_nSendBuf[4] = 0x16;
	m_dwByteToSend = FIXFRAMELEN;

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nResetLinkStatus: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwByteToSend);
#endif
	
	m_cRespGenInterroGrpNum=m_cRespCountGrpNum=0;

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nResetLinkStatus: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::AckCommand(unsigned char nAck)
{
	m_nSendBuf[0] = 0x10;
	m_nSendBuf[1] = 0x80 | nAck;
	m_nSendBuf[2] = m_nStnId;
	if(m_bACD)
		m_nSendBuf[1] = m_nSendBuf[1] | 0x20;

	m_nSendBuf[3] = m_nSendBuf[1] + m_nSendBuf[2];
	m_nSendBuf[4] = 0x16;
	m_dwByteToSend = FIXFRAMELEN;

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nAckCommand: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwByteToSend);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nAckCommand: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::NoRequestData(void)
{
	m_nSendBuf[0] = 0x10;
	m_nSendBuf[1] = 0x89;			//NACK No Request Data	
	m_nSendBuf[2] = m_nStnId;
	if( m_bACD || m_iCmdTermFlag)
		m_nSendBuf[1] = m_nSendBuf[1] | 0x20;

	m_nSendBuf[3] = m_nSendBuf[1] + m_nSendBuf[2];
	m_nSendBuf[4] = 0x16;
	m_dwByteToSend = FIXFRAMELEN;

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nNoRequestData: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwByteToSend);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nNoRequestData: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::SingleRespondByte(void)
{
	m_nSendBuf[0] = 0xE5;
	m_dwByteToSend = 1;

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nSingleRespondByte: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwByteToSend);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
		{
		printf("\nSingleRespondByte: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
		}
	return(true);
}

bool CIEC101Slave::RepeatTransmit(void)
{
	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nIEC repeat transmitted data byte nNumber %u :\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nRepeatTransmit: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}
	
bool CIEC101Slave::SinglePointCtrlAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nPntNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 9;		//L
	m_nSendBuf[4] = 0x80;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = C_SC_NA;			//45 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//7 ActCon cause of transmit:active
	m_nSendBuf[9] = m_nStnId;			//ASDU common address
		
	nPntNum = m_sEvent.nAddress - SC_CMD_BASE_ADDR;
	if((nPntNum < 0 ) || (nPntNum >= MAX_SC_NUM))
	{
		printf("C_SC_NA info adress = %d Error!\n",m_sEvent.nAddress);
		m_iNoChangeData = 1;
		return(true);
	}
	else
	{
		CrossCopy(&m_nSendBuf[10],&m_sEvent.nAddress,2);	//information object nLength
		m_nSendBuf[12] = m_sEvent.nQuality;			//QCC:=CP8{SCS,QU,S/E}
									//SCS 1:=ON 0:=OFF  S/E := 0:=Execute 1:= Select
	}
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	m_dwByteToSend = 15;

	nChkSum = 0;
	for(n=4;n<13;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[13] = nChkSum;				//checksum char
	m_nSendBuf[14] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

	memcpy(m_nCmdSendBuf,m_nSendBuf,m_dwByteToSend);	//copy cmd for actterm
#ifdef DEBUG
	printf("\nSingle Point Ctrl Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nSingle Point Ctrl Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::DoublePointCtrlAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nPntNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 9;		//L
	m_nSendBuf[4] = 0x80;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = C_DC_NA;			//46 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//7 ActCon cause of transmit:active
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nPntNum = m_sEvent.nAddress - DC_CMD_BASE_ADDR;
	if((nPntNum < 0 ) || (nPntNum >= MAX_DC_NUM))
	{
		printf("C_DC_NA info nLength = %d Error!\n",m_sEvent.nAddress);
		m_iNoChangeData = 1;
		return(true);
	}
	else
	{
		CrossCopy(&m_nSendBuf[10],&m_sEvent.nAddress,2);	//information object address
		m_nSendBuf[12] = m_sEvent.nQuality;			//QCC:=CP8{DCS,QU,S/E}
									//DCS 2:=ON 1:=OFF  S/E := 0:=Execute 1:= Select
	}
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	m_dwByteToSend = 15;

	nChkSum = 0;
	for(n=4;n<13;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[13] = nChkSum;				//checksum char
	m_nSendBuf[14] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
	memcpy(m_nCmdSendBuf,m_nSendBuf,m_dwByteToSend);	//copy cmd for actterm
#ifdef DEBUG
	printf("\nDouble Point Ctrl Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nDouble Point Ctrl Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::RegulatStepCtrlAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nPntNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 9;			//L
	m_nSendBuf[4] = 0x80;					//control field
//	if(m_bACD || m_iCmdTermFlag)
//		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = C_RC_NA;				//47 type identifier field
	m_nSendBuf[7] = 1;					//variable structure specify
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;			//7 ActCon cause of transmit:active
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nPntNum = m_sEvent.nAddress - RC_CMD_BASE_ADDR;
	if((nPntNum < 0 ) || (nPntNum >= MAX_RC_NUM))
	{
		printf("C_RC_NA info nLength = %d Error!\n",m_sEvent.nAddress);
		m_iNoChangeData = 1;
		return(true);
	}
	else
	{
		CrossCopy(&m_nSendBuf[10],&m_sEvent.nAddress,2);	//information object address
		m_nSendBuf[12] = m_sEvent.nQuality;			//QCC:=CP8{RCS,QU,S/E}
									//RCS 2:=Higher 1:=Lower  S/E := 0:=Execute 1:= Select
	}
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	m_dwByteToSend = 15;

	nChkSum = 0;
	for(n=4;n<13;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[13] = nChkSum;				//checksum char
	m_nSendBuf[14] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
	memcpy(m_nCmdSendBuf,m_nSendBuf,m_dwByteToSend);	//copy cmd for actterm
#ifdef DEBUG
	printf("\nRegulating Step Ctrl Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nRegulating Step Ctrl Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::SetPointCmdNAAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nPntNum;
	short			nNormVal;
	float			fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 11;		//L
	m_nSendBuf[4] = 0x80;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = C_SE_NA;			//48 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//7 ACTCON cause of transmit:active
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nPntNum = m_sEvent.nAddress - SE_CMD_BASE_ADDR;
	if((nPntNum < 0 ) || (nPntNum >= MAX_SE_NUM))
	{
		printf("C_SE_NA info nLength = %d Error!\n",m_sEvent.nAddress);
		m_iNoChangeData = 1;
		return(true);
	}
	else
	{
		CrossCopy(&m_nSendBuf[10],&m_sEvent.nAddress,2);	//information object address

		fVal = m_sEvent.uValue.fValue;
		nNormVal=(short)((fVal-m_sMEDef.fLoRange[nPntNum])*RESOLUTE/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);

		CrossCopy(&m_nSendBuf[12],&nNormVal,2);		//Normalized Value

		m_nSendBuf[14] = m_sEvent.nQuality;		//QOC:= S/E[8] QL[1..7]
								//S/E := 0:=Execute 1:= Select
	}
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

//	if(se)
//		m_nSendBuf[14] = m_nSendBuf[14] | 0x80;		//S/E := 0:=Execute 1:= Select

	m_dwByteToSend = 17;

	nChkSum = 0;
	for(n=4;n<15;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[15] = nChkSum;				//checksum char
	m_nSendBuf[16] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nSet Point Command NA Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nSet Point Command NA Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::SetPointCmdNBAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nPntNum;
	short			nScalVal;
	float			fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 11;			//L
	m_nSendBuf[4] = 0x80;					//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = C_SE_NB;				//49 type identifier field
	m_nSendBuf[7] = 1;					//variable structure specify
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;			//7 ACTCON cause of transmit:active
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nPntNum = m_sEvent.nAddress - SE_CMD_BASE_ADDR;
	if((nPntNum < 0 ) || (nPntNum >= MAX_SE_NUM))
	{
		printf("C_SE_NB info nLength = %d Error!\n",m_sEvent.nAddress);
		m_iNoChangeData = 1;
		return(true);
	}
	else
	{
		CrossCopy(&m_nSendBuf[10],&m_sEvent.nAddress,2);	//information object address

		fVal = m_sEvent.uValue.fValue;

		nScalVal=(short)(fVal * m_sMEDef.fLoRange[nPntNum]);

		CrossCopy(&m_nSendBuf[12],&nScalVal,2);		//Scaled Value

		m_nSendBuf[14] = m_sEvent.nQuality;		//QOC:= S/E[8] QL[1..7]
								//S/E := 0:=Execute 1:= Select
	}
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}
	
//	if(se)
//		m_nSendBuf[14] = m_nSendBuf[14] | 0x80;	//S/E := 0:=Execute 1:= Select

	m_dwByteToSend = 17;

	nChkSum = 0;
	for(n=4;n<15;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[15] = nChkSum;				//checksum char
	m_nSendBuf[16] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nSet Point Command NB Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nSet Point Command NB Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::BitStringCtrlAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nPntNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 12;			//L
	m_nSendBuf[4] = 0x80;					//control field
//	if(m_bACD || m_iCmdTermFlag)
//		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = C_BO_NA;				//51 type identifier field
	m_nSendBuf[7] = 1;					//variable structure specify
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;			//7 ActCon cause of transmit:active
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nPntNum = m_sEvent.nAddress - BC_CMD_BASE_ADDR;
	if((nPntNum < 0 ) || (nPntNum >= MAX_BC_NUM))
	{
		printf("C_BC_NA info nLength = %d Error!\n",m_sEvent.nAddress);
		m_iNoChangeData = 1;
		return(true);
	}
	else
	{
		CrossCopy(&m_nSendBuf[10],&m_sEvent.nAddress,2);	//information object address
		CrossCopy(&m_nSendBuf[12],&m_sEvent.uValue.iCountRead,4);	//bit string
	}
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	m_dwByteToSend = 18;

	nChkSum = 0;
	for(n=4;n<16;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[16] = nChkSum;				//checksum char
	m_nSendBuf[17] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
	memcpy(m_nCmdSendBuf,m_nSendBuf,m_dwByteToSend);	//copy cmd for actterm
#ifdef DEBUG
	printf("\nBitstring Ctrl Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nBitstring Ctrl Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::ParamValueNAAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nPntNum;
	short			nNormVal;
	float			fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 11;		//L
	m_nSendBuf[4] = 0x80;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = P_ME_NA;			//110 type identifier field,normialized
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//7 ACTCON cause of transmit:active
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nPntNum = m_sEvent.nAddress - PM_BASE_ADDR;
	if((nPntNum < 0 ) || (nPntNum >= MAX_PM_NUM))
	{
		printf("P_ME_NA info nLength = %d Error!\n",m_sEvent.nAddress);
		m_iNoChangeData = 1;
	}
	else
	{
		CrossCopy(&m_nSendBuf[10],&m_sEvent.nAddress,2);	//information object address

		fVal = m_sEvent.uValue.fValue;
#ifdef ME_POSITIVE_ONLY					
		nNormVal=(short)((fVal-m_sPMDef.fLoRange[nPntNum])*RESOLUTE/
				(m_sPMDef.fHiRange[nPntNum]-m_sPMDef.fLoRange[nPntNum])+0.5);
#else
		nNormVal=(short)((fVal - (m_sPMDef.fHiRange[nPntNum] + m_sPMDef.fLoRange[nPntNum])/2)*0xffff/
				(m_sPMDef.fHiRange[nPntNum]-m_sPMDef.fLoRange[nPntNum])+0.5);
#endif

		CrossCopy(&m_nSendBuf[12],&nNormVal,2);		//Normalized Value

		m_nSendBuf[14] = m_sEvent.nQuality;		//KPA[1-6] LPC POP
								//KPA 1:= threshold 2:= filter factor
								//3:= low limit  4:= high limit
	}
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	if(m_iNoChangeData == 1)
		return(true);

	m_dwByteToSend = 17;

	nChkSum = 0;
	for(n=4;n<15;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[15] = nChkSum;				//checksum char
	m_nSendBuf[16] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nParameter value NA Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nParameter value NA Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::ParamValueNBAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nPntNum;
	short		nScalVal;
	float		fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 11;		//L
	m_nSendBuf[4] = 0x80;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = P_ME_NB;			//111 type identifier field,scaled
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = 7;				//cause of transmit:active
	m_nSendBuf[9] = m_nStnId;			//ASDU common address
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//7 ACTCON cause of transmit:active
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nPntNum = m_sEvent.nAddress - PM_BASE_ADDR;
	if((nPntNum < 0 ) || (nPntNum >= MAX_PM_NUM))
	{
		printf("P_ME_NB info nLength = %d Error!\n",m_sEvent.nAddress);
		m_iNoChangeData = 1;
	}
	else
	{
		CrossCopy(&m_nSendBuf[10],&m_sEvent.nAddress,2);	//information object address

		fVal = m_sEvent.uValue.fValue;
		nScalVal=(short)(fVal * m_sPMDef.fLoRange[nPntNum]);

		CrossCopy(&m_nSendBuf[12],&nScalVal,2);		//Normalized Value

		m_nSendBuf[14] = m_sEvent.nQuality;		//KPA[1-6] LPC POP
								//KPA 1:= threshold 2:= filter factor
								//3:= low limit  4:= high limit
	}
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	if(m_iNoChangeData == 1)
		return(true);

	m_dwByteToSend = 17;

	nChkSum = 0;
	for(n=4;n<15;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[15] = nChkSum;				//checksum char
	m_nSendBuf[16] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nParameter value NB Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nParameter value NB Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::ClockSynchroniseAck(void)
{
	unsigned char	nChkSum,n;
	unsigned short	nMillisec;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;	//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 15;	//L
	m_nSendBuf[4] = 0x80;			//control field

	m_nSendBuf[5] = m_nStnId;		//link nLength field
	m_nSendBuf[6] = C_CS_NA;		//103 type identifier field
	m_nSendBuf[7] = 1;			//variable structure specify
	m_nSendBuf[8] = ACTCON;			//7 cause of transmit:actcon
	m_nSendBuf[9] = m_nStnId;		//ASDU common address
	m_nSendBuf[10] = 0;			//information object low byte
	m_nSendBuf[11] = 0;			//information object high byte

	nMillisec = m_sEvent.nMilliseconds;
	CrossCopy(&m_nSendBuf[12],&nMillisec,2);		//Milliseconds(0-59999)
	m_nSendBuf[14] = (unsigned char)m_sEvent.nMinutes;	//Minutes(0-59)
	m_nSendBuf[15] = (unsigned char)m_sEvent.nHours;	//Hours(0-23)
	m_nSendBuf[16] = (unsigned char)m_sEvent.nDay + (unsigned char)(m_sEvent.nDayOfWeek << 5);
								//Days of week(1-7) Days(1-31)
	m_nSendBuf[17] = (unsigned char)m_sEvent.nMonth;	//Months(1-12)
	m_nSendBuf[18] = (unsigned char)m_sEvent.nYear;		//Years(0-99)

	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;		//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	m_dwByteToSend = 21;

	nChkSum = 0;
	for(n=4;n<19;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[19] = nChkSum;				//checksum char
	m_nSendBuf[20] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nClockSynchronisationAck: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nClockSynchronisationAck: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::ClockSynchroniseAckResp(unsigned int nPnt)
{
	unsigned char	nChkSum,n;
	struct timespec sTimeSpec;
	struct tm sTimetm;
	int iRet;
	unsigned short	nMilliSec;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 15;		//L
	m_nSendBuf[4] = 0x80;				//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = C_CS_NA;			//103 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = ACTCON;				//7 cause of transmit:actcon
	m_nSendBuf[9] = m_nStnId;			//ASDU common address
	m_nSendBuf[10] = 0;				//information object low byte
	m_nSendBuf[11] = 0;				//information object high byte

//	memcpy(&m_nSendBuf[12],&m_sRecvCmdData.nData[nPnt][8],7);	//Milliseconds

	iRet = getclock(1, &sTimeSpec);
	if(iRet == -1)
	{
		printf("\nClockSynchronisation: getclock() error: %s\n",strerror(errno));
		return(false);
	}
	sTimetm = *localtime(&sTimeSpec.tv_sec);
	nMilliSec = sTimetm.tm_sec * 1000 + sTimeSpec.tv_nsec/1000000;
	CrossCopy(&m_nSendBuf[12],&nMilliSec,2);			//Milliseconds
	m_nSendBuf[14] = (unsigned char)sTimetm.tm_min;		//Minutes
	m_nSendBuf[15] = (unsigned char)sTimetm.tm_hour;	//Hour
	if(sTimetm.tm_wday == 0)
		sTimetm.tm_wday = 7;
	n = sTimetm.tm_wday << 5;
	n = n & 0xE0;
	m_nSendBuf[16] = (unsigned char)sTimetm.tm_mday | n;	//DayofWeek[6..8] day[1..5]
	m_nSendBuf[17] = (unsigned char)sTimetm.tm_mon + 1;		//Month
	m_nSendBuf[18] = (unsigned char)(sTimetm.tm_year%100);	//Year

	m_dwByteToSend = 21;

	nChkSum = 0;
	for(n=4;n<19;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[19] = nChkSum;				//checksum char
	m_nSendBuf[20] = 0x16;					//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nClockSynchroniseAckResp: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
		{
		printf("\nClockSynchroniseAckResp: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
		}
	return(true);
}

bool CIEC101Slave::GeneralInterrogationAck(void)
{
	unsigned char	nChkSum,n;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;	//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 9;	//L
	m_nSendBuf[4] = 0x80;			//control field

	m_nSendBuf[5] = m_nStnId;		//link nLength field
	m_nSendBuf[6] = C_IC_NA;		//100 type identifier field
	m_nSendBuf[7] = 1;			//variable structure specify
	m_nSendBuf[8] = ACTCON;			//7 ACTCON cause of transmit:active
//	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;	//7 ACTCON cause of transmit:active
	m_nSendBuf[9] = m_nStnId;		//ASDU common address
	m_nSendBuf[10] = 0;			//information object low byte
	m_nSendBuf[11] = 0;			//information object high byte
	m_nSendBuf[12] = 20;			//QOI							
//	m_nSendBuf[12] = m_sEvent.nQuality;	//QOI: 20							
	
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	m_dwByteToSend = 15;

	nChkSum = 0;
	for(n=4;n<13;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[13] = nChkSum;			//checksum char
	m_nSendBuf[14] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nGeneralInterrogation: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nGeneralInterrogation: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::GeneralInterrogationAckResp(unsigned char nGI_Cause)
{
	unsigned char	nChkSum,n;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 9;		//L
	m_nSendBuf[4] = 0x88;				//control field

	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field ACD = 1
	}

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = C_IC_NA;			//100 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = nGI_Cause;			//ActTerm: 10 Confirm: 7 cause of transmit:active
	m_nSendBuf[9] = m_nStnId;			//ASDU common address
	m_nSendBuf[10] = 0;				//information object low byte
	m_nSendBuf[11] = 0;				//information object high byte
	m_nSendBuf[12] = 20;				//QOI							
	
	m_dwByteToSend = 15;

	nChkSum = 0;
	for(n=4;n<13;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[13] = nChkSum;			//checksum char
	m_nSendBuf[14] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nGeneralInterrogationActResp %d: %u byte transmitted\n",nGI_Cause,m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nGeneralInterrogationActResp: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::CounterInterrogationAck(void)
{
	unsigned char	nChkSum,n;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 9;		//L
	m_nSendBuf[4] = 0x80;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = C_CI_NA;			//101 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
//	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//7 ACTCON 10 Actterm cause of transmit
	m_nSendBuf[8] = ACTCON;				//7 ACTCON 10 Actterm cause of transmit
	m_nSendBuf[9] = m_nStnId;			//ASDU common address
	m_nSendBuf[10] = 0;				//information object low byte
	m_nSendBuf[11] = 0;				//information object high byte
	m_nSendBuf[12] = m_sEvent.nQuality;		//QCC 5 general request counter							

							//QCC:=CP8{RQT:=UI6[1..6],FRZ:=UI2[7..8]}
							//RQT 5:=general counter interrogation
							//RQT 1-4 := 1-4 group counter interrogation
							//FRZ 0:=request counter value							
	if(GetUserDataClass1(m_sEvent))
	{
		m_bACD = 1;
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field ACD = 1
	}
	else
	{
		m_bACD = 0;
		m_sEvent.nTypeId = 0;
	}

	m_dwByteToSend = 15;

	nChkSum = 0;
	for(n=4;n<13;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[13] = nChkSum;			//checksum char
	m_nSendBuf[14] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nCounter Interrogation Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nCounter Interrogation Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::CounterInterrogationAckResp(unsigned char nGI_Cause)
{
	unsigned char	nChkSum,n;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 9;		//L
	m_nSendBuf[4] = 0x88;				//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = C_CI_NA;			//101 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = nGI_Cause;			//ActTerm: 10 Confirm: 7 cause of transmit:active
	m_nSendBuf[9] = m_nStnId;			//ASDU common address
	m_nSendBuf[10] = 0;				//information object low byte
	m_nSendBuf[11] = 0;				//information object high byte
	m_nSendBuf[12] = 5;				//QCC 5 general request counter							
							//QCC:=CP8{RQT:=UI6[1..6],FRZ:=UI2[7..8]}
							//RQT 5:=general counter interrogation
							//RQT 1-4 := 1-4 group counter interrogation
							//FRZ 0:=request counter value							
	m_dwByteToSend = 15;

	nChkSum = 0;
	for(n=4;n<13;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[13] = nChkSum;			//checksum char
	m_nSendBuf[14] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nCounter Interrogation Ack Resp %d: %u byte transmitted\n",nGI_Cause,m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nCounter Interrogation Ack Resp: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::RespondTestAck()
{
	unsigned char	nChkSum,n;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 10;		//L
	m_nSendBuf[4] = 0x80;				//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = C_TS_NA;			//104 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = ACTCON | 0x80;			//7 Actcon cause of transmit:active confirm
	m_nSendBuf[9] = m_nStnId;			//ASDU common address
	m_nSendBuf[10] = 0;				//information object low byte
	m_nSendBuf[11] = 0;				//information object high byte
	m_nSendBuf[12] = 0xAA;				//test low byte AA
	m_nSendBuf[13] = 0x55;				//test high byte 55

	m_dwByteToSend = 16;

	nChkSum = 0;
	for(n=4;n<14;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[14] = nChkSum;			//checksum char
	m_nSendBuf[15] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nTest channel Ack: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nTest channel Ack: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::SinglePointInformCyc(unsigned char nGI_Cause, unsigned char nInfoGroup)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned short	nPntNum;
	unsigned short	nChnNo;
	unsigned short	nFirstInfoAddr;
	unsigned short	nStartNum,nEndNum;
	unsigned char	nQlty;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[4] = 0x88;					//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = M_SP_NA;				//1 type single point information
	m_nSendBuf[8] = nGI_Cause;				//cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nStartNum = SP_GROUP_NUM*nInfoGroup;			//nInfoGroup: 0,1,2...8
	if(m_sSPDef.nNumber < nStartNum)
		{
		NoRequestData();
		return(true);
		}
	nFirstInfoAddr = nStartNum + SP_BASE_ADDR;
	nEndNum = nStartNum + SP_GROUP_NUM;
	if(m_sSPDef.nNumber < nEndNum)
		nEndNum = m_sSPDef.nNumber;

	m_nSendBuf[10] = (unsigned char)(nFirstInfoAddr& 0xFF);	//i-addr low byte
	m_nSendBuf[11] = (unsigned char)(nFirstInfoAddr>>8);	//i-addr high byte
	nInfoNum = 0;
	for(nPntNum=nStartNum;nPntNum<nEndNum;nPntNum++)	//Cycle for SP status
	{
		nChnNo = m_sSPDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty)==false)
		{
			printf("Single point: %d Channel No. Error!\n",nPntNum);

			m_nSendBuf[12+nInfoNum] = m_nOrgSPbuf[nInfoNum] =0;	//QDS IV NT SB BL RES3 SPI
			nInfoNum++;
			continue;
		}

		m_nSendBuf[12+nInfoNum] = m_nOrgSPbuf[nInfoNum] =nQlty;		//QDS IV NT SB BL RES3 SPI
		nInfoNum++;
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("SinglePointInformCyc: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	m_nSendBuf[7] = nInfoNum | 0x80;		//SQ=1 variable structure specify (nNumber of i-object)
	nLength = nInfoNum + 8;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;		//checksum char
	m_nSendBuf[nLength+1] = 0x16;		//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nSingle Point information Cyc: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nSingle Point information Cyc: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::SinglePointInform(void)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned short	nPntNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_SP_NA;			//1 type single point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for SP status Change
	{
		nPntNum = m_sEvent.nAddress - SP_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_SP_NUM))
		{
			printf("M_SP_NA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*3],&m_sEvent.nAddress,2);	//information object address
			m_nSendBuf[12+nInfoNum*3] = m_sEvent.nQuality;			//QDS IV NT SB BL RES3 SPI
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_SP_NA) || (nInfoNum >= 81))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Single Point information M_SP_NA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 3 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//ByteToSend = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;		//checksum char
	m_nSendBuf[nLength+1] = 0x16;		//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nSingle Point information: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nSingle Point information: write error: %s\n",strerror(errno));
		sleep(1);
		return(false);
	}
	return(true);
}

bool CIEC101Slave::SinglePointInformTA(void)
{
//	SYSTEMTIME	sys_tm;
	unsigned short	nMillisec,nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_SP_TA;			//2 type single point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)					//Check for SP status Event
	{
		nPntNum = m_sEvent.nAddress - SP_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_SP_NUM))
		{
			printf("M_SP_TA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*6],&m_sEvent.nAddress,2);	//information object address
			m_nSendBuf[12+nInfoNum*6] = m_sEvent.nQuality;			//QDS IV NT SB BL RES3 SPI

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[13+nInfoNum*6],&nMillisec,2);		//Milliseconds
			m_nSendBuf[15+nInfoNum*6] = (unsigned char)m_sEvent.nMinutes;	//Minutes
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_SP_TA) || (nInfoNum >= 40))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Single Point information M_SP_TA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 6 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nSingle Point information TA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nSingle Point information TA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::SinglePointInformTB(void)
{
	unsigned short	nMillisec,nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_SP_TB;			//30 type single point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)			//Check for SP status Event
	{
		nPntNum = m_sEvent.nAddress - SP_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_SP_NUM))
		{
			printf("M_SP_TB info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*10],&m_sEvent.nAddress,2);	//information object address
			m_nSendBuf[12+nInfoNum*10] = m_sEvent.nQuality;			//QDS IV NT SB BL RES3 SPI

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[13+nInfoNum*10],&nMillisec,2);		//Milliseconds(0-59999)
			m_nSendBuf[15+nInfoNum*10] = (unsigned char)m_sEvent.nMinutes;	//Minutes(0-59)
			m_nSendBuf[16+nInfoNum*10] = (unsigned char)m_sEvent.nHours;	//Hours(0-23)
			m_nSendBuf[17+nInfoNum*10] = (unsigned char)m_sEvent.nDay + (unsigned char)(m_sEvent.nDayOfWeek << 5);
											//Days of week(1-7) Days(1-31)
			m_nSendBuf[18+nInfoNum*10] = (unsigned char)m_sEvent.nMonth;	//Months(1-12)
			m_nSendBuf[19+nInfoNum*10] = (unsigned char)m_sEvent.nYear;	//Years(0-99)
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_SP_TB) || (nInfoNum >= 24))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Single Point information M_SP_TB: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 10 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nSingle Point information TB: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nSingle Point information TB: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::DoublePointInformCyc(unsigned char nGI_Cause, unsigned char nInfoGroup)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned short	nPntNum;
	unsigned short	nChnNo;
	unsigned short	nFirstInfoAddr;
	unsigned short	nStartNum,nEndNum;
	unsigned char	nQlty,nQlty1;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[4] = 0x88;					//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = M_DP_NA;				//3 type double point information
	m_nSendBuf[8] = nGI_Cause;				//cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nStartNum = DP_GROUP_NUM*nInfoGroup;			//nInfoGroup: 0,1,2...8
	if(m_sDPDef.nNumber < nStartNum)
		{
		NoRequestData();
		return(true);
		}
	nFirstInfoAddr = nStartNum + DP_BASE_ADDR;	
	nEndNum = nStartNum + DP_GROUP_NUM;
	if(m_sDPDef.nNumber < nEndNum)
		nEndNum = m_sDPDef.nNumber;

	m_nSendBuf[10] = (unsigned char)(nFirstInfoAddr& 0xFF);		//i-addr low byte
	m_nSendBuf[11] = (unsigned char)(nFirstInfoAddr>>8);		//i-addr high byet
	nInfoNum = 0;
	for(nPntNum=nStartNum;nPntNum<nEndNum;nPntNum++)		//Cycle for DP status
	{
		nChnNo = m_sDPDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty)==false)
		{
			printf("Double point: %d Channel No. Error!\n",nPntNum);

			m_nSendBuf[12+nInfoNum] = m_nOrgDPbuf[nInfoNum] =0;	//QDS IV NT SB BL RES2 DPI
			nInfoNum++;
			continue;
		}
		m_nOrgDPbuf[nInfoNum] = nQlty;		//QDS IV NT SB BL RES2 DPI
		m_nSendBuf[12+nInfoNum] = nQlty & 0xF0;

		nChnNo = m_sDPDef.nChannelNo1[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty1)==false)
		{
			printf("Double point: %d Channel No. %d Error!\n",nPntNum,nChnNo);

			m_nSendBuf[12+nInfoNum] = m_nOrgDPbuf1[nInfoNum] =0;	//QDS IV NT SB BL RES2 DPI
			nInfoNum++;
			continue;
		} 
		m_nOrgDPbuf1[nInfoNum] = nQlty1;		//QDS IV NT SB BL RES2 DPI

		if(nQlty & 1)
		{
			if(nQlty1 & 1)
				m_nSendBuf[12+nInfoNum] = m_nSendBuf[12+nInfoNum] | 3;
			else
				m_nSendBuf[12+nInfoNum] = m_nSendBuf[12+nInfoNum] | 2;
		}
		else
		{
			if(nQlty1 & 1)
				m_nSendBuf[12+nInfoNum] = m_nSendBuf[12+nInfoNum] | 1;
		}
		nInfoNum++;
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("DoublePointInformCyc: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	m_nSendBuf[7] = nInfoNum | 0x80;		//SQ=1 variable structure specify (nNumber of i-object)
	nLength = nInfoNum + 8;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;		//checksum char
	m_nSendBuf[nLength+1] = 0x16;		//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

//#ifdef DEBUG
	printf("\nDouble Point information Cyc: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
//#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nDouble Point information Cyc: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::DoublePointInform(void)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned short	nPntNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_DP_NA;			//3 type double point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for DP Status Event
	{
		nPntNum = m_sEvent.nAddress - DP_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_DP_NUM))
		{
			printf("M_DP_NA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*3],&m_sEvent.nAddress,2);	//information object address
			m_nSendBuf[12+nInfoNum*3] = m_sEvent.nQuality;			//QDS IV NT SB BL RES2 DPI
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_DP_NA) || (nInfoNum >= 81))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Double Point information M_DP_NA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 3 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nDouble Point information: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nDouble Point information: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::DoublePointInformTA(void)
{
	unsigned short	nMillisec,nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_DP_TA;			//4 type single point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for SP status Event
	{
		nPntNum = m_sEvent.nAddress - DP_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_DP_NUM))
		{
			printf("M_DP_TA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*6],&m_sEvent.nAddress,2);	//information object address
			m_nSendBuf[12+nInfoNum*6] = m_sEvent.nQuality;			//QDS IV NT SB BL RES2 DPI

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[13+nInfoNum*6],&nMillisec,2);		//Milliseconds
			m_nSendBuf[15+nInfoNum*6] = (unsigned char)m_sEvent.nMinutes;	//Minutes
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_DP_TB) || (nInfoNum >= 40))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Double Point information M_DP_TA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 6 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nDouble Point information TA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nDouble Point information TA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::DoublePointInformTB(void)
{
	unsigned short	nMillisec,nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_DP_TB;			//31 type single point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for SP status Event
	{
		nPntNum = m_sEvent.nAddress - DP_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_DP_NUM))
		{
			printf("M_DP_TB info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*10],&m_sEvent.nAddress,2);	//information object address
			m_nSendBuf[12+nInfoNum*10] = m_sEvent.nQuality;			//QDS IV NT SB BL RES2 DPI

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[13+nInfoNum*10],&nMillisec,2);		//Milliseconds(0-59999)
			m_nSendBuf[15+nInfoNum*10] = (unsigned char)m_sEvent.nMinutes;	//Minutes(0-59)
			m_nSendBuf[16+nInfoNum*10] = (unsigned char)m_sEvent.nHours;	//Hours(0-23)
			m_nSendBuf[17+nInfoNum*10] = (unsigned char)m_sEvent.nDay + (unsigned char)(m_sEvent.nDayOfWeek << 5);
											//Days of week(1-7) Days(1-31)
			m_nSendBuf[18+nInfoNum*10] = (unsigned char)m_sEvent.nMonth;	//Months(1-12)
			m_nSendBuf[19+nInfoNum*10] = (unsigned char)m_sEvent.nYear;	//Years(0-99)
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_DP_TB) || (nInfoNum >= 24))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Double Point information M_DP_TB: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 10 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
//	Sleep(10);

#ifdef DEBUG
	printf("\nDouble Point information TB: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nDouble Point information TB: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::StepPositionInformCyc(unsigned char nGI_Cause, unsigned char nInfoGroup)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	unsigned short	nPntNum;
	unsigned short	nFirstInfoAddr;
	unsigned short	nStartNum,nEndNum;
	unsigned short	nChnNo;
	char			cVTI;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[4] = 0x88;					//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = M_ST_NA;				//5 type double point information
	m_nSendBuf[8] = nGI_Cause;				//cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nStartNum = ST_GROUP_NUM*nInfoGroup;			//nInfoGroup: 0,1,2...8
	if(m_sSTDef.nNumber < nStartNum)
		{
		NoRequestData();
		return(true);
		}
	nFirstInfoAddr = nStartNum + ST_BASE_ADDR;	
	nEndNum = nStartNum + ST_GROUP_NUM;
	if(m_sSTDef.nNumber < nEndNum)
		nEndNum = m_sSTDef.nNumber;

	m_nSendBuf[10] = (unsigned char)(nFirstInfoAddr& 0xFF);		//i-addr low byte
	m_nSendBuf[11] = (unsigned char)(nFirstInfoAddr>>8);		//i-addr high byte
	nInfoNum = 0;
	for(nPntNum=nStartNum;nPntNum<nEndNum;nPntNum++)		//Cycle for Step Position status
	{
		nChnNo = m_sSTDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty, cVTI)==false)
		{
			printf("StepPositionInformCyc M_ST_NA point: %d Channel No. Error!\n",nPntNum);

			m_nSendBuf[12+nInfoNum*2] = 0;	//VTI(-64 63)[1-7]
			m_nSendBuf[13+nInfoNum*2] = 0;	//QDS IV NT SB BL RES3 OV
			nInfoNum++;
			continue;
		}
		m_nSendBuf[12+nInfoNum*2] = (unsigned char)m_sEvent.uValue.cVTI;	//VTI(-64 63)[1-7]
		m_nSendBuf[13+nInfoNum*2] = m_sEvent.nQuality;				//QDS IV NT SB BL RES3 OV
		nInfoNum++;
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("StepPositionInformCyc M_ST_NA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	m_nSendBuf[7] = nInfoNum | 0x80;		//SQ=1 variable structure specify (nNumber of i-object)
	nLength = nInfoNum*2 + 8;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;		//checksum char
	m_nSendBuf[nLength+1] = 0x16;		//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nStep Position Information Cyc: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nStep Position Information Cyc: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::StepPositionInform(void)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned short	nPntNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ST_NA;			//5 type double point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Step Status Event
	{
		nPntNum = m_sEvent.nAddress - ST_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_ST_NUM))
		{
			printf("M_ST_NA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*4],&m_sEvent.nAddress,2);		//information object address
			m_nSendBuf[12+nInfoNum*4] = (unsigned char)m_sEvent.uValue.cVTI;	//VTI(-64 63)[1-7]
			m_nSendBuf[13+nInfoNum*4] = m_sEvent.nQuality;				//QDS IV NT SB BL RES3 OV
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_ST_NA) || (nInfoNum >= 60))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Step Position information M_ST_NA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 4 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nStep Position information: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nStep Position information: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::StepPositionInformTA(void)
{
	unsigned short	nMillisec,nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ST_TA;			//6 type double point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Step Status Event
	{
		nPntNum = m_sEvent.nAddress - ST_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_ST_NUM))
		{
			printf("M_ST_TA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*7],&m_sEvent.nAddress,2);		//information object address
			m_nSendBuf[12+nInfoNum*7] = (unsigned char)m_sEvent.uValue.cVTI;	//VTI(-64 63)[1-7]
			m_nSendBuf[13+nInfoNum*7] = m_sEvent.nQuality;				//QDS IV NT SB BL RES3 OV

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[13+nInfoNum*7],&nMillisec,2);			//Milliseconds(0-59999)
			m_nSendBuf[15+nInfoNum*7] = (unsigned char)m_sEvent.nMinutes;		//Minutes(0-59)
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_ST_TA) || (nInfoNum >= 34))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Step Position information M_ST_TA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 7 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
//	Sleep(10);

#ifdef DEBUG
	printf("\nStep position information: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nStep position information: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::StepPositionInformTB(void)
{
	unsigned short	nMillisec,nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ST_TB;			//32 type double point information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Step Status Event
	{
		nPntNum = m_sEvent.nAddress - ST_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_ST_NUM))
		{
			printf("M_ST_TB info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*11],&m_sEvent.nAddress,2);		//information object address
			m_nSendBuf[12+nInfoNum*11] = (unsigned char)m_sEvent.uValue.cVTI;	//VTI(-64 63)[1-7]
			m_nSendBuf[13+nInfoNum*11] = (unsigned char)m_sEvent.nQuality;		//QDS IV NT SB BL RES3 OV

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[13+nInfoNum*11],&nMillisec,2);			//Milliseconds(0-59999)
			m_nSendBuf[15+nInfoNum*7] = (unsigned char)m_sEvent.nMinutes;		//Minutes(0-59)
			m_nSendBuf[16+nInfoNum*10] = (unsigned char)m_sEvent.nHours;		//Hours(0-23)
			m_nSendBuf[17+nInfoNum*10] = (unsigned char)m_sEvent.nDay + (unsigned char)(m_sEvent.nDayOfWeek << 5);
												//Days of week(1-7) Days(1-31)
			m_nSendBuf[18+nInfoNum*10] = (unsigned char)m_sEvent.nMonth;		//Months(1-12)
			m_nSendBuf[19+nInfoNum*10] = (unsigned char)m_sEvent.nYear;		//Years(0-99)
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_ST_TB) || (nInfoNum >= 22))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Step Position information M_ST_TB: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 11 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nStep position information: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nStep position information: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::Bitstringof32Bit(void)
{
	unsigned short	nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	int		iBitstring;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_BO_NA;			//7 type Integrated Totals information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Bitstring of 32 Bit Event
	{
		nPntNum = m_sEvent.nAddress - BO_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_BO_NUM))
		{
			printf("M_BO_NA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*7],&m_sEvent.nAddress,2);	//information object address
			nQlty = (unsigned char)m_sEvent.nQuality;

			iBitstring = m_sEvent.uValue.iCountRead;
			CrossCopy(&m_nSendBuf[12+nInfoNum*7],&iBitstring,4);
			m_nSendBuf[16+nInfoNum*7] = nQlty;

			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_BO_NA) || (nInfoNum >= 34))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Bitstring of 32 Bit M_BO_NA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 7 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;				//checksum char
	m_nSendBuf[nLength+1] = 0x16;				//end char
	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
//	Sleep(10);

#ifdef DEBUG
	printf("\nBitstring of 32 Bit M_BO_NA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nBitstring of 32 Bit M_BO_NA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::Bitstringof32BitTA(void)
{
	unsigned short	nMillisec,nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	int				iBitstring;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_BO_TA;			//8 type Integrated Totals information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Bitstring of 32 Bit Event
	{
		nPntNum = m_sEvent.nAddress - BO_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_BO_NUM))
		{
			printf("M_BO_TA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*10],&m_sEvent.nAddress,2);	//information object address
			nQlty = (unsigned char)m_sEvent.nQuality;

			iBitstring = m_sEvent.uValue.iCountRead;
			CrossCopy(&m_nSendBuf[12+nInfoNum*10],&iBitstring,4);

			m_nSendBuf[16+nInfoNum*10] = nQlty;

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[17+nInfoNum*10],&nMillisec,2);		//Milliseconds(0-59999)
			m_nSendBuf[19+nInfoNum*10] = (unsigned char)m_sEvent.nMinutes;	//Minutes(0-59)

			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_BO_TA) || (nInfoNum >= 24))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Bitstring of 32 Bit M_BO_TA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 10 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char
	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nBitstring of 32 Bit M_BO_TA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nBitstring of 32 Bit M_BO_TA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::IntegratedTotalsCyc(unsigned char nGI_Cause, unsigned char nInfoGroup)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	unsigned short	nPntNum;
	unsigned short	nFirstInfoAddr;
	unsigned short	nStartNum;
	unsigned short	nEndNum;
	unsigned short	nChnNo;
	int		iCount;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[4] = 0x88;					//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = M_IT_NA;				//15 type double point information
	m_nSendBuf[8] = nGI_Cause;				//cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nStartNum = IT_GROUP_NUM*nInfoGroup;	//nInfoGroup: 0,1,2...8
	if(m_sITDef.nNumber < nStartNum)
		{
		NoRequestData();
		return(true);
		}
	nFirstInfoAddr = nStartNum + IT_BASE_ADDR;	
	nEndNum = nStartNum + IT_GROUP_NUM;
	if(m_sITDef.nNumber < nEndNum)
		nEndNum = m_sITDef.nNumber;

	m_nSendBuf[10] = (unsigned char)(nFirstInfoAddr& 0xFF);	//i-addr low byte
	m_nSendBuf[11] = (unsigned char)(nFirstInfoAddr>>8);	//i-addr high byte
	nInfoNum = 0;
	for(nPntNum=nStartNum;nPntNum<nEndNum;nPntNum++)	//Cycle for Step Position status
	{
		nChnNo = m_sITDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty, iCount)==false)
		{
			printf("IntegratedTotalsCyc M_IT_NA point: %d Channel No. Error!\n",nPntNum);

			m_nSendBuf[12+nInfoNum*5] = 0;		//count[0-7]
			m_nSendBuf[13+nInfoNum*5] = 0;		//count[8-15]
			m_nSendBuf[14+nInfoNum*5] = 0;		//count[16-23]
			m_nSendBuf[15+nInfoNum*5] = 0;		//count[24-31]
			m_nSendBuf[16+nInfoNum*5] = nInfoNum;	//QDS: IV CA CY SQ
			nInfoNum++;
			continue;
		}
		CrossCopy(&m_nSendBuf[12+nInfoNum*5],&iCount,4);
		m_nSendBuf[16+nInfoNum*5] = nQlty;		//QDS IV NT SB BL RES3 OV
		nInfoNum++;
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("IntegratedTotalsCyc M_IT_NA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	m_nSendBuf[7] = nInfoNum | 0x80;			//SQ=1 variable structure specify (nNumber of i-object)
	nLength = nInfoNum*5 + 8;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;		//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;		//checksum char
	m_nSendBuf[nLength+1] = 0x16;		//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nIntegrated Totals Cycle M_IT_NA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nIntegrated Totals Cycle M_IT_NA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::IntegratedTotals(void)
{
	unsigned short	nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	int		iCount;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_IT_NA;			//16 type Integrated Totals information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Integrated Totals Event
	{
		nPntNum = m_sEvent.nAddress - IT_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_IT_NUM))
		{
			printf("M_IT_NA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*7],&m_sEvent.nAddress,2);	//information object address
			nQlty = m_sEvent.nQuality;
			iCount = m_sEvent.uValue.iCountRead;

			CrossCopy(&m_nSendBuf[12+nInfoNum*7],&iCount,4);
			m_nSendBuf[16+nInfoNum*7] = nQlty;

			m_iOrgITBuf[nPntNum] = iCount;
			m_nOrgITQlt[nPntNum] = nQlty;

			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_IT_NA) || (nInfoNum >= 34))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("IntegratedTotals M_IT_NA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 7 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char
	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nIntegrated Totals M_IT_NA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nIntegrated Totals M_IT_NA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::IntegratedTotalsTA(void)
{
	unsigned short	nMillisec;
	unsigned short	nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	int				iCount;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_IT_TA;			//16 type Integrated Totals information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Integrated Totals TA Event
	{
		nPntNum = m_sEvent.nAddress - IT_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_IT_NUM))
		{
			printf("M_IT_TA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*10],&m_sEvent.nAddress,2);	//information object address
			nQlty = m_sEvent.nQuality;
			iCount = m_sEvent.uValue.iCountRead;

			CrossCopy(&m_nSendBuf[12+nInfoNum*10],&iCount,4);
			m_nSendBuf[16+nInfoNum*10] = nQlty;

			m_iOrgITBuf[nPntNum] = iCount;
			m_nOrgITQlt[nPntNum] = nQlty;

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[17+nInfoNum*10],&nMillisec,2);		//Milliseconds(0-59999)
			m_nSendBuf[19+nInfoNum*10] = (unsigned char)m_sEvent.nMinutes;	//Minutes(0-59)

			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_IT_TA) || (nInfoNum >= 24))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("IntegratedTotalsTA M_IT_TA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 10 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nIntegrated Totals M_IT_TA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nIntegrated Totals M_IT_TA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::IntegratedTotalsTB(void)
{
	unsigned short	nMillisec;
	unsigned short	nPntNum;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	int				iCount;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_IT_TB;			//17 type Integrated Totals information
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Step Position Event
	{
		nPntNum = m_sEvent.nAddress - IT_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= MAX_IT_NUM))
		{
			printf("M_IT_TB info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*14],&m_sEvent.nAddress,2);	//information object address
			nQlty = m_sEvent.nQuality;
			iCount = m_sEvent.uValue.iCountRead;

			CrossCopy(&m_nSendBuf[12+nInfoNum*14],&iCount,4);
			m_nSendBuf[16+nInfoNum*14] = nQlty;

			m_iOrgITBuf[nPntNum] = iCount;
			m_nOrgITQlt[nPntNum] = nQlty;

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[17+nInfoNum*14],&nMillisec,2);			//Milliseconds(0-59999)
			m_nSendBuf[19+nInfoNum*14] = (unsigned char)m_sEvent.nMinutes;		//Minutes(0-59)
			m_nSendBuf[20+nInfoNum*14] = (unsigned char)m_sEvent.nHours;		//Hours(0-23)
			m_nSendBuf[21+nInfoNum*14] = (unsigned char)m_sEvent.nDay + (unsigned char)(m_sEvent.nDayOfWeek << 5);
												//Days of week(1-7) Days(1-31)
			m_nSendBuf[22+nInfoNum*14] = (unsigned char)m_sEvent.nMonth;		//Months(1-12)
			m_nSendBuf[23+nInfoNum*14] = (unsigned char)m_sEvent.nYear;		//Years(0-99)


			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_IT_TB) || (nInfoNum >= 17))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("IntegratedTotalsTB M_IT_TB: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 10 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//L	Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
//	Sleep(10);

#ifdef DEBUG
	printf("\nIntegrated Totals M_IT_TB: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nSinglePointCtrl: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::MeasureValueNA(void)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	unsigned short	nPntNum;
	short		iValue;
	unsigned short	nChnNo;
	float		fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ME_NA;			//9 type normalized without quality descriptor
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;
	for(nPntNum=0;nPntNum<m_sMEDef.nNumber;nPntNum++)
	{
		nChnNo = m_sMEDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty, fVal)==false)
		{
			printf("ME: point: %d Channel No. Error!\n",nPntNum);

			continue;
		}

		if(fVal <= m_sMEDef.fLoRange[nPntNum])
#ifdef ME_POSITIVE_ONLY					
			iValue = 0;
#else
			iValue = 0x8000;
#endif
		else	if(fVal >= m_sMEDef.fHiRange[nPntNum])
			iValue = 0x7FFF;
		else
		{
#ifdef ME_POSITIVE_ONLY					
			iValue=(short)((fVal-m_sMEDef.fLoRange[nPntNum])*RESOLUTE/
				(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#else
			iValue=(short)((fVal - (m_sMEDef.fHiRange[nPntNum] + m_sMEDef.fLoRange[nPntNum])/2)*0xffff/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#endif
		}

		if(((iValue - m_iOrgAnaBuf[nPntNum]) >= m_nDataband) || ((m_iOrgAnaBuf[nPntNum] - iValue) >= m_nDataband))
		{
#ifdef DEBUG
			printf("ANA %d real %d save %d m_nDataband %d\n",nPntNum,
					iValue,m_iOrgAnaBuf[nPntNum],m_nDataband);
#endif
			m_nSendBuf[10+nInfoNum*5] = (unsigned char)((ME_BASE_ADDR + nPntNum)& 0xFF);	//i-addr low byte
			m_nSendBuf[11+nInfoNum*5] = (unsigned char)((ME_BASE_ADDR + nPntNum)>>8);	//i-addr high byte
			m_nSendBuf[12+nInfoNum*5] = (unsigned char)(iValue & 0xFF);						//N Value							
			m_nSendBuf[13+nInfoNum*5] = (unsigned char)(iValue >> 8);						//N Value
			m_nSendBuf[14+nInfoNum*5] = nQlty;												//QDS IV NT SB BL RES3 OV
			nInfoNum++;
			m_iOrgAnaBuf[nPntNum] = iValue;
			m_nOrgAnaQlt[nPntNum] = nQlty;
			if(nInfoNum >= 48)
			{
				break;
			}
		}
	}

	m_nSendBuf[7] = nInfoNum;	//SQ=0

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}

	nLength = nInfoNum * 5 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;		//L	ByteToSend = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;				//checksum char

	m_nSendBuf[nLength+1] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nMeasure Value M_ME_NA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nMeasure Value M_ME_NA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::MeasureValueNACyc(unsigned char nGI_Cause,unsigned char nInfoGroup)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	unsigned short	nPntNum;
	short			iValue;
	unsigned short	nChnNo;
	float			fVal;
	unsigned short	nFirstInfoAddr;
	unsigned short	nStartNum;
	unsigned short	nEndNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ME_NA;			//9 type normalized without quality descriptor

	m_nSendBuf[8] = nGI_Cause;			//cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nStartNum = ME_GROUP_NUM*nInfoGroup;		//nInfoGroup: 0,1,2,3
	if(m_sMEDef.nNumber < nStartNum)
	{
		NoRequestData();
		return(true);
	}
	nFirstInfoAddr = nStartNum + ME_BASE_ADDR;	//nInfoGroup: 0,1,2,3
	nEndNum = nStartNum + ME_GROUP_NUM;
	if(m_sMEDef.nNumber < nEndNum)
		nEndNum = m_sMEDef.nNumber;

	m_nSendBuf[10] = (unsigned char)(nFirstInfoAddr& 0xFF);	//i-addr low byte
	m_nSendBuf[11] = (unsigned char)(nFirstInfoAddr>>8);	//i-addr high byte
	nInfoNum = 0;
	for(nPntNum=nStartNum;nPntNum<nEndNum;nPntNum++)
		{
		nChnNo = m_sMEDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty, fVal)==false)
		{
			printf("ME: point: %d Channel No. Error!\n",nPntNum);

			m_nSendBuf[12+nInfoNum*3] = (unsigned char)0;		//N Value							
			m_nSendBuf[13+nInfoNum*3] = (unsigned char)0;		//N Value
			m_nSendBuf[14+nInfoNum*3] = 0;				//QDS IV NT SB BL RES3 OV
			nInfoNum++;
			continue;
		}

		if(fVal <= m_sMEDef.fLoRange[nPntNum])
#ifdef ME_POSITIVE_ONLY					
			iValue = 0;
#else
			iValue = 0x8000;
#endif
		else	if(fVal >= m_sMEDef.fHiRange[nPntNum])
			iValue = 0x7FFF;
		else
		{
#ifdef ME_POSITIVE_ONLY					
			iValue=(short)((fVal-m_sMEDef.fLoRange[nPntNum])*RESOLUTE/
				(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#else
			iValue=(short)((fVal - (m_sMEDef.fHiRange[nPntNum] + m_sMEDef.fLoRange[nPntNum])/2)*0xffff/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#endif
		}

		m_nSendBuf[12+nInfoNum*3] = (unsigned char)(iValue & 0xFF);	//N Value							
		m_nSendBuf[13+nInfoNum*3] = (unsigned char)(iValue >> 8);	//N Value
		m_nSendBuf[14+nInfoNum*3] = nQlty;				//QDS IV NT SB BL RES3 OV
		nInfoNum++;
		m_iOrgAnaBuf[nPntNum] = iValue;
		m_nOrgAnaQlt[nPntNum] = nQlty;
	}

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNACyc: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	m_nSendBuf[7] = nInfoNum | 0x80;		//SQ=1

	nLength = nInfoNum * 3 + 8;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//L ByteToSend = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char

	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nCyc Measure Value M_ME_NA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nCyc Measure Value M_ME_NA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::MeasureValueNB(void)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	unsigned short	nPntNum;
	short			iValue;
	unsigned short	nChnNo;
	float			fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[4] = 0x88;					//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = M_ME_NB;				//11 type scaled without quality descriptor
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;			//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nInfoNum = 0;
	for(nPntNum=0;nPntNum<m_sMEDef.nNumber;nPntNum++)
	{
		nChnNo = m_sMEDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty, fVal)==false)
		{
			printf("ME: point: %d Channel No. Error!\n",nPntNum);
			continue;
		}

		iValue=(short)(fVal * m_sMEDef.fLoRange[nPntNum]);
		if(((iValue - m_iOrgAnaBuf[nPntNum]) >= m_nDataband) || ((m_iOrgAnaBuf[nPntNum] - iValue) >= m_nDataband))
		{
#ifdef DEBUG
			printf("ANA %d real %d save %d m_nDataband %d\n",nPntNum,
					iValue,m_iOrgAnaBuf[nPntNum],m_nDataband);
#endif
			m_nSendBuf[10+nInfoNum*5] = (unsigned char)((ME_BASE_ADDR + nPntNum)& 0xFF);	//i-addr low byte
			m_nSendBuf[11+nInfoNum*5] = (unsigned char)((ME_BASE_ADDR + nPntNum)>>8);	//i-addr high byte
			m_nSendBuf[12+nInfoNum*5] = (unsigned char)(iValue & 0xFF);			//N Value							
			m_nSendBuf[13+nInfoNum*5] = (unsigned char)(iValue >> 8);			//N Value
			m_nSendBuf[14+nInfoNum*5] = nQlty;								//QDS IV NT SB BL RES3 OV
			nInfoNum++;
			m_iOrgAnaBuf[nPntNum] = iValue;
			m_nOrgAnaQlt[nPntNum] = (unsigned char)nQlty;
			if(nInfoNum >= 48)
			{
				break;
			}
		}
	}

	m_nSendBuf[7] = nInfoNum;	//SQ=0

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNB: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}

	nLength = nInfoNum * 5 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//L ByteToSend = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;				//checksum char

	m_nSendBuf[nLength+1] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nMeasure Value M_ME_NB: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
		{
		printf("\nMeasure Value M_ME_NB: write error: %s\n",strerror(errno));
		return(false);
		}
	return(true);
}

bool CIEC101Slave::MeasureValueNBCyc(unsigned char nGI_Cause,unsigned char nInfoGroup)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	unsigned short	nPntNum;
	short			iValue;
	unsigned short	nChnNo;
	float			fVal;
	unsigned short	nFirstInfoAddr;
	unsigned short	nStartNum;
	unsigned short	nEndNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[4] = 0x88;					//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = M_ME_NB;				//11 type scaled without quality descriptor

	m_nSendBuf[8] = nGI_Cause;				//cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nStartNum = ME_GROUP_NUM*nInfoGroup;			//nInfoGroup: 0,1,2,3
	if(m_sMEDef.nNumber < nStartNum)
	{
		NoRequestData();
		return(true);
	}
	nFirstInfoAddr = nStartNum + ME_BASE_ADDR;		//nInfoGroup: 0,1,2,3
	nEndNum = nStartNum + ME_GROUP_NUM;
	if(m_sMEDef.nNumber < nEndNum)
		nEndNum = m_sMEDef.nNumber;

	m_nSendBuf[10] = (unsigned char)(nFirstInfoAddr& 0xFF);	//i-addr low byte
	m_nSendBuf[11] = (unsigned char)(nFirstInfoAddr>>8);	//i-addr high byte
	nInfoNum = 0;
	for(nPntNum=nStartNum;nPntNum<nEndNum;nPntNum++)
	{
		nChnNo = m_sMEDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty, fVal)==false)
		{
			printf("ME: point: %d Channel No. Error!\n",nPntNum);

			m_nSendBuf[12+nInfoNum*3] = (unsigned char)0;		//N Value							
			m_nSendBuf[13+nInfoNum*3] = (unsigned char)0;		//N Value
			m_nSendBuf[14+nInfoNum*3] = 0;				//QDS IV NT SB BL RES3 OV
			nInfoNum++;
			continue;
		}

		iValue=(short)(fVal * m_sMEDef.fLoRange[nPntNum]);
		m_nSendBuf[12+nInfoNum*3] = (unsigned char)(iValue & 0xFF);	//N Value							
		m_nSendBuf[13+nInfoNum*3] = (unsigned char)(iValue >> 8);	//N Value
		m_nSendBuf[14+nInfoNum*3] = nQlty;				//QDS IV NT SB BL RES3 OV
		nInfoNum++;
		m_iOrgAnaBuf[nPntNum] = iValue;
		m_nOrgAnaQlt[nPntNum] = (unsigned char)nQlty;
	}

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNBCyc: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	m_nSendBuf[7] = nInfoNum | 0x80;		//SQ=1

	nLength = nInfoNum * 3 + 8;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//L ByteToSend = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char

	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nCyc Measure Value M_ME_NB: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nCyc Measure Value M_ME_NB: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::MeasureValueND(void)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	unsigned short	nPntNum;
	short			iValue;
	unsigned short	nChnNo;
	float			fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;			//start flag
	m_nSendBuf[4] = 0x88;					//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;				//link nLength field
	m_nSendBuf[6] = M_ME_ND;				//21 type normalized without quality descriptor
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;			//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;				//ASDU common address

	nInfoNum = 0;
	for(nPntNum=0;nPntNum<m_sMEDef.nNumber;nPntNum++)
	{
		nChnNo = m_sMEDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty, fVal)==false)
		{
			printf("ME: point: %d Channel No. Error!\n",nPntNum);
			continue;
		}

		if(fVal <= m_sMEDef.fLoRange[nPntNum])
#ifdef ME_POSITIVE_ONLY					
			iValue = 0;
#else
			iValue = 0x8000;
#endif
		else	if(fVal >= m_sMEDef.fHiRange[nPntNum])
			iValue = 0x7FFF;
		else
		{
#ifdef ME_POSITIVE_ONLY					
			iValue=(short)((fVal-m_sMEDef.fLoRange[nPntNum])*RESOLUTE/
				(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#else
			iValue=(short)((fVal - (m_sMEDef.fHiRange[nPntNum] + m_sMEDef.fLoRange[nPntNum])/2)*0xffff/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#endif
		}

		if(((iValue - m_iOrgAnaBuf[nPntNum]) >= m_nDataband) || ((m_iOrgAnaBuf[nPntNum] - iValue) >= m_nDataband))
		{
#ifdef DEBUG
			printf("ME %d real %d save %d m_nDataband %d\n",nPntNum,
					iValue,m_iOrgAnaBuf[nPntNum],m_nDataband);
#endif
			m_nSendBuf[10+nInfoNum*4] = (unsigned char)((ME_BASE_ADDR + nPntNum)& 0xFF);	//i-addr low byte
			m_nSendBuf[11+nInfoNum*4] = (unsigned char)((ME_BASE_ADDR + nPntNum)>>8);	//i-addr high byte
			m_nSendBuf[12+nInfoNum*4] = (unsigned char)(iValue & 0xFF);		//N Value							
			m_nSendBuf[13+nInfoNum*4] = (unsigned char)(iValue >> 8);		//N Value
			nInfoNum++;
			m_iOrgAnaBuf[nPntNum] = iValue;
			m_nOrgAnaQlt[nPntNum] = (unsigned char)nQlty;
			if(nInfoNum >= 60)
			{
				break;
			}
		}
	}

	m_nSendBuf[7] = nInfoNum;	//SQ=0

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}

	nLength = nInfoNum * 4 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//ByteToSend = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;				//checksum char

	m_nSendBuf[nLength+1] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nMeasure Value M_ME_ND: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nMeasure Value M_ME_ND: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::MeasureValueNDCyc(unsigned char nGI_Cause,unsigned char nInfoGroup)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	unsigned short	nPntNum;
	short		iValue;
	unsigned short	nChnNo;
	float		fVal;
	unsigned short	nFirstInfoAddr;
	unsigned short	nStartNum;
	unsigned short	nEndNum;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ME_ND;			//21 type normalized without quality descriptor

	m_nSendBuf[8] = nGI_Cause;			// INTRO9:29 INTRO10:30
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nStartNum = ME_GROUP_NUM*nInfoGroup;		//nInfoGroup: 0,1,2,3
	if(m_sMEDef.nNumber < nStartNum)
	{
		NoRequestData();
		return(true);
	}
	nFirstInfoAddr = nStartNum + ME_BASE_ADDR;	//nInfoGroup: 0,1,2,3
	nEndNum = nStartNum + ME_GROUP_NUM;
	if(m_sMEDef.nNumber < nEndNum)
		nEndNum = m_sMEDef.nNumber;

	m_nSendBuf[10] = (unsigned char)(nFirstInfoAddr& 0xFF);	//i-addr low byte
	m_nSendBuf[11] = (unsigned char)(nFirstInfoAddr>>8);	//i-addr high byte
	nInfoNum = 0;
	for(nPntNum=nStartNum;nPntNum<nEndNum;nPntNum++)
	{
		nChnNo = m_sMEDef.nChannelNo[nPntNum];
		if(m_pCRTValue->PAU_ReadRealTimeValue(nChnNo, nQlty, fVal)==false)
		{
			printf("ME: point: %d Channel No. Error!\n",nPntNum);

			m_nSendBuf[12+nInfoNum*2] = (unsigned char)0;		//N Value							
			m_nSendBuf[13+nInfoNum*2] = (unsigned char)0;		//N Value
			nInfoNum++;
			continue;
		}
		if(fVal <= m_sMEDef.fLoRange[nPntNum])
#ifdef ME_POSITIVE_ONLY					
			iValue = 0;
#else
			iValue = 0x8000;
#endif
		else	if(fVal >= m_sMEDef.fHiRange[nPntNum])
			iValue = 0x7FFF;
		else
		{
#ifdef ME_POSITIVE_ONLY					
			iValue=(short)((fVal-m_sMEDef.fLoRange[nPntNum])*RESOLUTE/
				(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#else
			iValue=(short)((fVal - (m_sMEDef.fHiRange[nPntNum] + m_sMEDef.fLoRange[nPntNum])/2)*0xffff/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#endif
		}

		m_nSendBuf[12+nInfoNum*2] = (unsigned char)(iValue & 0xFF);	//N Value							
		m_nSendBuf[13+nInfoNum*2] = (unsigned char)(iValue >> 8);	//N Value
		nInfoNum++;
		m_iOrgAnaBuf[nPntNum] = iValue;
		m_nOrgAnaQlt[nPntNum] = (unsigned char)nQlty;
	}

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNDCyc: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	m_nSendBuf[7] = nInfoNum | 0x80;	//SQ=1

	nLength = nInfoNum * 2 + 8;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char

	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nCyc Measure Value M_ME_NA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	m_iNoChangeData = 0;
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nCyc Measure Value M_ME_NA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::MeasureValueTA(void)
{
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned short	nPntNum;
	unsigned char	nQlty;
	unsigned short	nMillisec;
	short			iValue;
	float			fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ME_TA;			//10 type normalized without quality descriptor
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Mesure value Event
	{
		nPntNum = m_sEvent.nAddress - ME_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= m_sMEDef.nNumber))
		{
			printf("M_ME_TA info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*8],&m_sEvent.nAddress,2);	//information object nLength
			fVal = m_sEvent.uValue.fValue;
			nQlty = (unsigned char)m_sEvent.nQuality;

			if(fVal <= m_sMEDef.fLoRange[nPntNum])
#ifdef ME_POSITIVE_ONLY					
				iValue = 0;
#else
				iValue = 0x8000;
#endif
			else	if(fVal >= m_sMEDef.fHiRange[nPntNum])
				iValue = 0x7FFF;
			else
			{
#ifdef ME_POSITIVE_ONLY					
				iValue=(short)((fVal-m_sMEDef.fLoRange[nPntNum])*RESOLUTE/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#else
				iValue=(short)((fVal - (m_sMEDef.fHiRange[nPntNum] + m_sMEDef.fLoRange[nPntNum])/2)*0xffff/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#endif
			}

			m_nSendBuf[12+nInfoNum*8] = (unsigned char)(iValue & 0xFF);	//N Value							
			m_nSendBuf[13+nInfoNum*8] = (unsigned char)(iValue >> 8);	//N Value
			m_nSendBuf[14+nInfoNum*8] = nQlty;				//QDS IV NT SB BL RES3 OV
			
			m_iOrgAnaBuf[nPntNum] = iValue;
			m_nOrgAnaQlt[nPntNum] = nQlty;

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[15+nInfoNum*8],&nMillisec,2);		//Milliseconds(0-59999)
			m_nSendBuf[17+nInfoNum*8] = (unsigned char)m_sEvent.nMinutes;	//Minutes(0-59)
			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_ME_TA) || (nInfoNum >= 34))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueTA M_ME_TA: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 8 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;				//checksum char
	m_nSendBuf[nLength+1] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nMeasure Value M_ME_TA: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nMeasure Value M_ME_TA: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::MeasureValueTB(void)
{
	unsigned short	nMillisec;
	unsigned short	nPntNum;
	short			iValue;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	float			fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ME_TB;			//11 type normalized without quality descriptor
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Mesure value Event
	{
		nPntNum = m_sEvent.nAddress - ME_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= m_sMEDef.nNumber))
		{
			printf("M_ME_TB info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*8],&m_sEvent.nAddress,2);	//information object address
			nQlty = (unsigned char)m_sEvent.nQuality;
			fVal = m_sEvent.uValue.fValue;

			iValue=(short)(fVal * m_sMEDef.fLoRange[nPntNum]);

			m_nSendBuf[12+nInfoNum*8] = (unsigned char)(iValue & 0xFF);	//N Value							
			m_nSendBuf[13+nInfoNum*8] = (unsigned char)(iValue >> 8);	//N Value
			m_nSendBuf[14+nInfoNum*8] = m_sEvent.nQuality;			//QDS IV NT SB BL RES3 OV

			m_iOrgAnaBuf[nPntNum] = iValue;
			m_nOrgAnaQlt[nPntNum] = nQlty;

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[15+nInfoNum*8],&nMillisec,2);		//Milliseconds(0-59999)
			m_nSendBuf[17+nInfoNum*8] = (unsigned char)m_sEvent.nMinutes;	//Minutes(0-59)

			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_ME_TB) || (nInfoNum >= 34))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueTB M_ME_TB: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 8 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//Byte To Send = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;			//checksum char
	m_nSendBuf[nLength+1] = 0x16;			//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
//	Sleep(10);

#ifdef DEBUG
	printf("\nMeasure Value M_ME_TB: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nMeasure Value M_ME_TB: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::MeasureValueTD(void)
{
	unsigned short	nMillisec;
	unsigned short	nPntNum;
	short			iValue;
	unsigned char	nLength;
	unsigned char	nChkSum,n;
	unsigned char	nInfoNum;
	unsigned char	nQlty;
	float			fVal;

	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[4] = 0x88;				//control field

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_ME_TD;			//34 type normalized without quality descriptor CP52Time2a
	m_nSendBuf[8] = m_sEvent.nCauseOfTrans;		//cause of transmit:requested
	m_nSendBuf[9] = m_nStnId;			//ASDU common address

	nInfoNum = 0;

	for(; ;)		//Check for Mesure value Event
	{
		nPntNum = m_sEvent.nAddress - ME_BASE_ADDR;
		if((nPntNum < 0 ) || (nPntNum >= m_sMEDef.nNumber))
		{
			printf("M_ME_TB info adress = %d Error!\n",m_sEvent.nAddress);
		}
		else
		{
			CrossCopy(&m_nSendBuf[10+nInfoNum*8],&m_sEvent.nAddress,2);	//information object address
			fVal = m_sEvent.uValue.fValue;
			nQlty= (unsigned char)m_sEvent.nQuality;

			if(fVal <= m_sMEDef.fLoRange[nPntNum])
#ifdef ME_POSITIVE_ONLY					
				iValue = 0;
#else
				iValue = 0x8000;
#endif
			else	if(fVal >= m_sMEDef.fHiRange[nPntNum])
				iValue = 0x7FFF;
			else
			{
#ifdef ME_POSITIVE_ONLY					
				iValue=(short)((fVal-m_sMEDef.fLoRange[nPntNum])*RESOLUTE/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#else
				iValue=(short)((fVal - (m_sMEDef.fHiRange[nPntNum] + m_sMEDef.fLoRange[nPntNum])/2)*0xffff/
					(m_sMEDef.fHiRange[nPntNum]-m_sMEDef.fLoRange[nPntNum])+0.5);
#endif
			}

			m_nSendBuf[12+nInfoNum*12] = (unsigned char)(iValue & 0xFF);	//N Value							
			m_nSendBuf[13+nInfoNum*12] = (unsigned char)(iValue >> 8);	//N Value
			m_nSendBuf[14+nInfoNum*12] = nQlty;				//QDS IV NT SB BL RES3 OV

			m_iOrgAnaBuf[nPntNum] = iValue;
			m_nOrgAnaQlt[nPntNum] = nQlty;

			nMillisec = m_sEvent.nMilliseconds;
			CrossCopy(&m_nSendBuf[15+nInfoNum*12],&nMillisec,2);		//Milliseconds(0-59999)
			m_nSendBuf[17+nInfoNum*12] = (unsigned char)m_sEvent.nMinutes;	//Minutes(0-59)
			m_nSendBuf[18+nInfoNum*12] = (unsigned char)m_sEvent.nHours;	//Hours(0-23)
			m_nSendBuf[19+nInfoNum*12] = (unsigned char)m_sEvent.nDay + (unsigned char)(m_sEvent.nDayOfWeek << 5);
											//Days of week(1-7) Days(1-31)
			m_nSendBuf[20+nInfoNum*12] = (unsigned char)m_sEvent.nMonth;	//Months(1-12)
			m_nSendBuf[21+nInfoNum*12] = (unsigned char)m_sEvent.nYear;	//Years(0-99)

			nInfoNum++;
		}
		if(GetUserDataClass1(m_sEvent))
		{
			if((m_sEvent.nTypeId != M_ME_TD) || (nInfoNum >= 20))
			{
				m_bACD = 1;
				break;
			}
		}
		else
		{
			m_bACD = 0;
			m_sEvent.nTypeId = 0;
			break;
		}
	}
	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("MeasureValueTD M_ME_TD: NoRequestData\n");
#endif
		m_iNoChangeData = 1;
		return(true);
	}
	if(m_bACD)
	{
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;	//control field
	}
	m_nSendBuf[7] = nInfoNum;			//variable structure specify (nNumber of i-object)
	nLength = m_nSendBuf[7] * 12 + 6;
	m_nSendBuf[1] = m_nSendBuf[2] = nLength;	//ByteToSend = L + 6;
	m_dwByteToSend = nLength + 6;
	nLength = nLength + 4;

	nChkSum = 0;
	for(n=4;n<nLength;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[nLength] = nChkSum;				//checksum char
	m_nSendBuf[nLength+1] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);

#ifdef DEBUG
	printf("\nMeasure Value M_ME_TD: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif
	
	if( m_dwSendByte != m_dwByteToSend )
	{
		printf("\nMeasure Value M_ME_TD: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::EndofInitialization(void)
{
	unsigned char	nChkSum,n;

	m_bACD=0;
	m_nSendBuf[0] = m_nSendBuf[3] = 0x68;		//start flag
	m_nSendBuf[1] = m_nSendBuf[2] = 9;		//L
	m_nSendBuf[4] = 0x88;				//control field
	if(m_bACD)
		m_nSendBuf[4] = m_nSendBuf[4] | 0x20;

	m_nSendBuf[5] = m_nStnId;			//link nLength field
	m_nSendBuf[6] = M_EI_NA;			//70 type identifier field
	m_nSendBuf[7] = 1;				//variable structure specify
	m_nSendBuf[8] = 4;				//4 Actterm cause of transmit:end
	m_nSendBuf[9] = 0;				//ASDU common address
	m_nSendBuf[10] = 0;				//information object low byte
	m_nSendBuf[11] = 0;				//information object high byte
	m_nSendBuf[12] = 5;				//COI 0 local power on							

	m_dwByteToSend = 15;

	nChkSum = 0;
	for(n=4;n<13;n++)
		nChkSum = nChkSum + m_nSendBuf[n];

	m_nSendBuf[13] = nChkSum;			//checksum char
	m_nSendBuf[14] = 0x16;				//end char

	m_dwSendByte = write(m_fd_com, m_nSendBuf, m_dwByteToSend);
	
#ifdef DEBUG
	printf("\nEnd of Initialization: %u byte transmitted\n",m_dwSendByte);
	PrintSendData(m_dwSendByte);
#endif

	if( m_dwSendByte != m_dwByteToSend )
	{
		sleep(1);
		printf("\nEnd of Initialization: write error: %s\n",strerror(errno));
		return(false);
	}
	return(true);
}

bool CIEC101Slave::RespondGroupInterrogation(unsigned char nGI_Cause)
{

	switch	( nGI_Cause ) 
	{
		case  21:
			SinglePointInformCyc(INTRO1,0);  ///21 respond to Group 0 (1) Interrogation
			break;

		case  22:
			SinglePointInformCyc(INTRO2,1);  //22 respond to Group 1 (2) general Interrogation
			break;

		case  23:
			SinglePointInformCyc(INTRO3,2);	//23 respond to Group 2 (3 general Interrogation
			break;

		case  24:
			DoublePointInformCyc(INTRO4,3);	//24 respond to Group 3 (4) general Interrogation
			break;
		case  29:
			MeasureValueNDCyc(INTRO9,0);	//29 respond to Group 0 (9) general Interrogation
			break;

		case  30:
			MeasureValueNDCyc(INTRO10,1);	//30 respond to Group 1 (10) general Interrogation
			break;

		case  31:
			MeasureValueNDCyc(INTROGEN,2);	//31 respond to Group 2 (11) general Interrogation
			break;

		case  32:
			MeasureValueNDCyc(INTROGEN,3);	//32 respond to Group 3 (12) general Interrogation
			break;

		default :
#ifdef DEBUG
			printf("No this group data cause=%d\n",nGI_Cause);
#endif
			m_iNoChangeData = 1;
			return(false);
			break;			
	}//switch(nGI_Cause)

	if(m_iNoChangeData)
	{
		if(m_bACD)
			NoRequestData();
		else
			SingleRespondByte();
	}
	return(true);
}

void CIEC101Slave::RespondGeneralInterrogation(void)
{
	short	grpno;
	
	if(m_cRespGenInterroGrpNum < 128)
	{
		SinglePointInformCyc(INTROGEN, m_cRespGenInterroGrpNum);	//20 respond to general Interrogation SP
	}
	else if(m_cRespGenInterroGrpNum < 192)
	{
		DoublePointInformCyc(INTROGEN, m_cRespGenInterroGrpNum-128);	//20 respond to general Interrogation DP
	}
	else if(m_cRespGenInterroGrpNum < 254)
	{
		MeasureValueNACyc(INTROGEN,m_cRespGenInterroGrpNum-192);	//20 respond to general Interrogation ME
	}
	else
	{
		GeneralInterrogationAckResp(ACTTERM);	//10 ActTrem to general Interrogation
	}
	m_cRespGenInterroGrpNum++;
//#ifdef DEBUG
	printf("++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
//#endif
	if(m_cRespGenInterroGrpNum >= 255)
	{
		m_cRespGenInterroGrpNum = 0;
		m_iInterrogationFlag = 0;
	}
	else if(m_cRespGenInterroGrpNum < 128)
	{
		grpno = m_sSPDef.nNumber/SP_GROUP_NUM;
		if(m_sSPDef.nNumber%SP_GROUP_NUM)
			grpno ++;
		if(m_cRespGenInterroGrpNum >= grpno)
		{
			if(m_sDPDef.nNumber != 0)
				m_cRespGenInterroGrpNum = 128;
			else
				m_cRespGenInterroGrpNum = 192;
		}
	}
	else if(m_cRespGenInterroGrpNum < 192)
	{
		grpno = m_sDPDef.nNumber/DP_GROUP_NUM;
		if(m_sDPDef.nNumber%DP_GROUP_NUM)
			grpno ++;
		if(m_cRespGenInterroGrpNum >= (grpno+128))
			m_cRespGenInterroGrpNum = 192;
	}
	else
	{
		grpno = m_sMEDef.nNumber/ME_GROUP_NUM;
		if(m_sMEDef.nNumber%ME_GROUP_NUM)
			grpno ++;
		if(m_cRespGenInterroGrpNum >= (grpno+192))
			m_cRespGenInterroGrpNum = 254;
	}
}

bool CIEC101Slave::RespondCountGroupInterrogation(unsigned char nGI_Cause)
{

	switch	( nGI_Cause ) 
		{
		case  2:
			IntegratedTotalsCyc(REQCO1,0);  //cause:38 respond to group 1 Counter Interrogation
			break;

		case  3:
			IntegratedTotalsCyc(REQCO2,1);  //cause:39 respond to group 2 Counter Interrogation
			break;

		case  4:
			IntegratedTotalsCyc(REQCO3,2);  //cause:40 respond to group 3 Counter Interrogation
			break;

		case  5:
			IntegratedTotalsCyc(REQCO4,3);  //cause:41 respond to group 4 Counter Interrogation
			break;

		default :
#ifdef DEBUG
			printf("No this IC data group cause= %d\n",nGI_Cause);
#endif
			m_iNoChangeData = 1;
			return(false);
			break;			
		}//switch(nGI_Cause)

	if(m_iNoChangeData)
	{
		if(m_bACD)
			NoRequestData();
		else
			SingleRespondByte();
	}

	return(true);
}
						  
void CIEC101Slave::RespondCountInterrogation(void)
{
	short	grpno;

	if(m_sITDef.nNumber == 0)
		m_cRespCountGrpNum = 128;
	
	if(m_cRespCountGrpNum < 128)
	{
		IntegratedTotalsCyc(REQCOGCN,m_cRespCountGrpNum);  //cause:cause:37 respond to Counter Interrogation
	}
	else
	{
		CounterInterrogationAckResp(ACTTERM);		//10 ActTrem to Counter Interrogation
	}
	m_cRespCountGrpNum++;
	if(m_cRespCountGrpNum >= 128)
	{
		m_cRespCountGrpNum = 0;
		m_iCounterFlag = 0;
	}
	else
	{
		grpno = m_sITDef.nNumber/IT_GROUP_NUM;
		if(m_sITDef.nNumber%IT_GROUP_NUM)
			grpno ++;
		if(m_cRespCountGrpNum >= grpno)
			m_cRespCountGrpNum = 128;
	}
}
						  
void CIEC101Slave::RespondUserDataClass1(void)
{
	unsigned char	nTypeID,nCause,nCmd;
	unsigned short	nChnNo;

	if(m_bACD ==1)
	{
		nTypeID = m_sEvent.nTypeId;
		nChnNo = m_sEvent.nChnNum;
		nCause = m_sEvent.nCauseOfTrans;
		nCmd = m_sEvent.nQuality;

		switch	( nTypeID ) 
			{
			case  M_SP_NA:

				SinglePointInform();
				break;

			case  M_SP_TA:

				SinglePointInformTA();
				break;

			case  M_SP_TB:

				SinglePointInformTB();
				break;

			case  M_DP_NA:

				DoublePointInform();
				break;

			case  M_DP_TA:

				DoublePointInformTA();
				break;

			case  M_DP_TB:

				DoublePointInformTB();
				break;

			case  M_ST_NA:

				StepPositionInform();
				break;

			case  M_ST_TA:

				StepPositionInformTA();
				break;

			case  M_ST_TB:

				StepPositionInformTB();
				break;

			case  M_IT_NA:				//Not used

				IntegratedTotals();
				break;

			case  M_IT_TA:

				IntegratedTotalsTA();
				break;

			case  M_IT_TB:

				IntegratedTotalsTB();
				break;

			case  M_BO_NA:

				Bitstringof32Bit();
				break;

			case  M_BO_TA:			//Not used

				Bitstringof32BitTA();
				break;

			case  M_ME_TA:			//Not used

				MeasureValueTA();
				break;

			case  M_ME_TB:			//Not used

				MeasureValueTB();
				break;

			case  M_ME_TD:

				MeasureValueTD();
				break;
/*
			case  M_ME_NA:

				MeasureValueNA();
				break;

			case  M_ME_NB:				//Not used

				MeasureValueNB();
				break;

			case  M_ME_ND:				//Not used

				MeasureValueND();
				break;
*/

			case  C_SC_NA:

				SinglePointCtrlAck();
				break;
			
			case  C_DC_NA:

				DoublePointCtrlAck();
				break;
			
			case  C_RC_NA:

				RegulatStepCtrlAck();
				break;

			case  C_SE_NA:

				SetPointCmdNAAck();
				break;

			case  C_SE_NB:

				SetPointCmdNBAck();
				break;

			case  P_ME_NA:

				ParamValueNAAck();
				break;

			case  P_ME_NB:

				ParamValueNBAck();
				break;

			case  C_BO_NA:

				BitStringCtrlAck();
				break;

		default :
			m_iNoChangeData = 1;
#ifdef DEBUG
			printf("No this TypeID = %d\n",nTypeID);
#endif
			break;			
		}//switch(nTypeID) 
	}
	else
	{
		m_iNoChangeData = 1;
	}
}
	
bool CIEC101Slave::GetUserDataClass1(SEVENT &sEvent)
{
	unsigned char	nTypeId;
	unsigned short	nChnNo;			// Channel Number
	unsigned short	nPntNum,nNum;

	for(; ;)
	{
		if(m_pCEvent->PAU_FetchEvent(m_nEventPtr,sEvent) == false)
		{
#ifdef DEBUG
			printf("PAU_FetchEvent() return false!\n");
#endif
			return false;
		}
		m_nEventPtr ++;
		if(m_nEventPtr >= MAX_EVENT_BUF)
		{
			m_nEventPtr = 0;
		}
		nTypeId = sEvent.nTypeId;
		nChnNo = sEvent.nChnNum;

		printf("Type %d Channel No %d!\n",nTypeId,nChnNo);

		switch	( nTypeId ) 
		{
			case  M_SP_NA:
			case  M_SP_TA:
			case  M_SP_TB:
//			case  M_PS_NA:				//Not used

				nNum = m_sSPDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)		//Check for SP event
				{
					if(nChnNo == m_sSPDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Single point: %d Channel No. State Change!\n",nPntNum);
#endif
						sEvent.nAddress = SP_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				nNum = m_sDPDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for DP and convert
				{
					if(nChnNo == m_sDPDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("double point: %d on State Change!\n",nPntNum);
#endif
						sEvent.nAddress = DP_BASE_ADDR + nPntNum;	//information address
						if(nTypeId == M_SP_TB)
							sEvent.nTypeId = nTypeId + 1;
						else
							sEvent.nTypeId = nTypeId + 2;
						m_nOrgDPbuf[nPntNum] = sEvent.nQuality;		//QDS IV NT SB BL RES2 DPI
						if(sEvent.nQuality & 1)
						{
							if(m_nOrgDPbuf1[nPntNum] & 1)
								sEvent.nQuality = sEvent.nQuality | 3;
							else
								sEvent.nQuality = (sEvent.nQuality & 0xF0) | 2;
						}
						else
						{
							if(m_nOrgDPbuf1[nPntNum] & 1)
								sEvent.nQuality = sEvent.nQuality | 1;
						}
						return true;
					}
					if(nChnNo == m_sDPDef.nChannelNo1[nPntNum])
					{
#ifdef DEBUG
						printf("double point: %d off State Change!\n",nPntNum);
#endif
						sEvent.nAddress = DP_BASE_ADDR + nPntNum;	//information address
						if(nTypeId == M_SP_TB)
							sEvent.nTypeId = nTypeId + 1;
						else
							sEvent.nTypeId = nTypeId + 2;
						m_nOrgDPbuf1[nPntNum] = sEvent.nQuality;		//QDS IV NT SB BL RES2 DPI
						if(sEvent.nQuality & 1)
						{
							if(m_nOrgDPbuf[nPntNum] & 1)
								sEvent.nQuality = sEvent.nQuality | 3;
							else
								sEvent.nQuality = (sEvent.nQuality & 0xF0) | 1;
						}
						else
						{
							if(m_nOrgDPbuf[nPntNum] & 1)
								sEvent.nQuality = sEvent.nQuality | 2;
						}
						return true;
					}
				}
				break;

			case  M_DP_NA:
			case  M_DP_TA:
			case  M_DP_TB:

				nNum = m_sDPDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for DP event
				{
					if(nChnNo == m_sDPDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("double point: %d Channel No. State Change!\n",nPntNum);
#endif
						sEvent.nAddress = DP_BASE_ADDR + nPntNum*2;	//information address
						return true;
					}
				}
				break;

			case  M_ST_NA:
			case  M_ST_TA:
			case  M_ST_TB:

				nNum = m_sSTDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for DP event
				{
					if(nChnNo == m_sSTDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Step point: %d Channel No. State Change!\n",nPntNum);
#endif
						sEvent.nAddress = ST_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  M_IT_NA:				//Not used
			case  M_IT_TA:
			case  M_IT_TB:

				nNum = m_sITDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for IT event
				{
					if(nChnNo == m_sITDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Integrated Total: %d Channel No. frame!\n",nPntNum);
#endif
						sEvent.nAddress = IT_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  M_ME_NA:
			case  M_ME_TA:				//Not used
			case  M_ME_NB:				//Not used
			case  M_ME_TB:				//Not used
			case  M_ME_ND:				//Not used
			case  M_ME_TD:

				nNum = m_sMEDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for ME event
				{
					if(nChnNo == m_sMEDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Mesured Value: %d Channel No. frame!\n",nPntNum);
#endif
						sEvent.nAddress = ME_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  M_BO_NA:
			case  M_BO_TA:

				nNum = m_sBODef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for BO event
				{
					if(nChnNo == m_sBODef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Bitstring Value: %d Channel No. frame!\n",nPntNum);
#endif
						sEvent.nAddress = BO_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  C_SC_NA:

				nNum = m_sSCDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for SP Command
				{
					if(nChnNo == m_sSCDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Sigle Point Command frame! Channel No.: %d \n",nPntNum);
#endif
						sEvent.nAddress = SC_CMD_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  C_DC_NA:

				nNum = m_sDCDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for DP Command
				{
					if(nChnNo == m_sDCDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Double Point Command frame! Channel No.: %d \n",nPntNum);
#endif
						sEvent.nAddress = DC_CMD_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  C_RC_NA:

				nNum = m_sRCDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for Regulating Command
				{
					if(nChnNo == m_sRCDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Regulating Command frame! Channel No.: %d \n",nPntNum);
#endif
						sEvent.nAddress = RC_CMD_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  C_SE_NA:
			case  C_SE_NB:				//Not Used
			case  C_SE_NC:				//Not Used

				nNum = m_sSEDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for Set point Command
				{
					if(nChnNo == m_sSEDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Set point Command frame! Channel No.: %d \n",nPntNum);
#endif
						sEvent.nAddress = SE_CMD_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  P_ME_NA:
			case  P_ME_NB:				//Not Used
			case  P_AC_NA:				//Not Used

				nNum = m_sPMDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for Set point Command
				{
					if(nChnNo == m_sPMDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Set parameter Command frame! Channel No.: %d \n",nPntNum);
#endif
						sEvent.nAddress = PM_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			case  M_EI_NA:
			case  C_IC_NA:
			case  C_CI_NA:
			case  C_CS_NA:
//				return true;
				break;

			case  C_BO_NA:

				nNum = m_sBCDef.nNumber;

				for(nPntNum=0;nPntNum<nNum;nPntNum++)	//Check for Bitstring of 32 bit Command
				{
					if(nChnNo == m_sBCDef.nChannelNo[nPntNum])
					{
#ifdef DEBUG
						printf("Bitstring of 32 bit Command frame! Channel No.: %d \n",nPntNum);
#endif
						sEvent.nAddress = BC_CMD_BASE_ADDR + nPntNum;	//information address
						return true;
					}
				}
				break;

			default :
//#ifdef DEBUG
				printf("No this data type id = %d\n",nTypeId);
//#endif
			}
	}//for(;;)

}

void CIEC101Slave::RespondUserDataClass2(void)
{
	MeasureValueNA();
}

void CIEC101Slave::PrintSendData(unsigned int nbsend)
{
	unsigned int d;

	for(d=1;d<=nbsend;d++)
	{
		printf("=%2x",m_nSendBuf[d-1]);
		if((d%10)==0)
			printf("\n");
	}
	printf("\n");
}

void CIEC101Slave::PrintSendCmdData(unsigned int nbsend)
{
	unsigned int d;

	for(d=1;d<=nbsend;d++)
	{
		printf(" %02X",m_nCmdSendBuf[d-1]);
		if((d%10)==0)
			printf("\n");
	}
	printf("\n");
}

void CIEC101Slave::PrintRecvData(unsigned int nbrecv)
{
	unsigned int	d;

	for(d=1;d<=nbrecv;d++)
	{
		printf(" %02X",m_nRecvBuf[d-1]);
		if((d%10)==0)
			printf("\n");
	}
	printf("\n");
}

void CIEC101Slave::CrossCopy(void *strDest, const void *strSrc, size_t nNum)
{
	unsigned int n;
	
	for(n=0; n< nNum; n++)
	{
		*((char *)strDest+n) = *((char *)strSrc+nNum-1-n);
	}
}

