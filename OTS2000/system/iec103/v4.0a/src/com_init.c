/*******************************************************************************************************************************
FILENAME:	
FUNCTION:		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, XP

HISTORY:
2008.4.10	start to modify the code for H9000 V4.0		zyf
********************************************************************************************************************************/
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#include "\home\ems\system\iec103\v4.0a\inc\iec103_com.h"
                        
DCB            dcb;
COMMTIMEOUTS   timeout;
HANDLE         hCom;
BOOL	       fSuccess;

/* SYSTEM define   */

/*#define   Tmp_Baud_Rate  9600  /* select com. baud rate, 300 ---- 9600 etc. */

HANDLE ComInit(char *COM,DWORD Baud_Rate,BYTE ByteSize,BYTE Parity,BYTE StopBits)
	{
	hCom = INVALID_HANDLE_VALUE;
	while(hCom == INVALID_HANDLE_VALUE)
		{
		hCom = CreateFile(COM,GENERIC_READ|GENERIC_WRITE,0,NULL,
						OPEN_EXISTING,0,NULL);
		if (hCom == INVALID_HANDLE_VALUE)	     
			{
			printf("%s =%d   CreateFile ERRNO= %d \n",
						COM, hCom,GetLastError());
//			return(hCom);
			Sleep(2000);
			}
		
		}
	printf("Init %s  success ,Handle=%d  \n",COM,hCom);
 
   /* ************* */
	fSuccess=SetupComm(hCom,CBINQU,CBOUTQU);
	if(!fSuccess)
		{
		printf("SetupComm ERRNO= %d \n",GetLastError());
//		return(hCom);
		}

   /* ************* */
	fSuccess=GetCommState(hCom,&dcb);
	if(!fSuccess)
		{
		printf("GetCommState ERRNO= %d \n",GetLastError());
		return(hCom);
		}

  /* ******************* */
	dcb.BaudRate=Baud_Rate ;
	dcb.ByteSize=ByteSize; //8
	dcb.Parity=Parity;//NOPARITY;
	dcb.StopBits=StopBits;//ONESTOPBIT;

	fSuccess=SetCommState(hCom,&dcb);
	if(!fSuccess)
		{
		printf("SetCommState ERRNO= %d \n",GetLastError());
		return(FALSE);
		}

  /* ******************* */
	fSuccess=GetCommTimeouts(hCom,&timeout);
	if(!fSuccess)
		{
		printf("GetCommTimeouts ERRNO= %d \n",GetLastError());
		return(hCom);
		}

  /* ********************** */
//	timeout.ReadIntervalTimeout         = 20; /* 2000ms */
//	timeout.ReadTotalTimeoutMultiplier  = 5;   /* 50ms */
//	timeout.ReadTotalTimeoutConstant    = 10; /* 3.0s */
//	timeout.WriteTotalTimeoutMultiplier = 50;   /* 50ms */
//	timeout.WriteTotalTimeoutConstant   = 300; /* 3.0s */

//	timeout.ReadIntervalTimeout         = RD_INTERVAL_TIMEOUT; // 13ms
//	timeout.ReadIntervalTimeout         = 300; // 13ms
	timeout.ReadIntervalTimeout         = 300; // 13ms
	timeout.ReadTotalTimeoutMultiplier  = RD_TOTAL_TIMEOUT_MUL; // 5ms
	timeout.ReadTotalTimeoutConstant    = RD_TOTAL_TIMEOUT_CNT; // 14ms
	timeout.WriteTotalTimeoutMultiplier = WT_TOTAL_TIMEOUT_MUL; // 5ms
	timeout.WriteTotalTimeoutConstant   = WT_TOTAL_TIMEOUT_CNT; // 14ms
	
	fSuccess=SetCommTimeouts(hCom,&timeout);
	if(!fSuccess)
		{    
		printf("SetCommTimeouts ERRNO= %d \n",GetLastError());
		return(FALSE);
		}
	return(hCom);
}

