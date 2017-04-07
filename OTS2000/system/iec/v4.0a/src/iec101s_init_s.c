/************************************************
* Source file :	com_init_s.c			*
* Function :	serial port initial program	*
*  NAME :					*
*  DATE :	August,2006       		*
************************************************/
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "../inc/iec101s_com.h"

#define	DEBUG
                        

#ifdef WINDOWS_OS

HANDLE ComInit(char *COM,DWORD Baud_Rate,BYTE ByteSize,BYTE Parity,BYTE StopBits)
{
	DCB            dcb;
	COMMTIMEOUTS   timeout;
	HANDLE         hCom;
	BOOL	       fSuccess;

	hCom = INVALID_HANDLE_VALUE;

	while(hCom == INVALID_HANDLE_VALUE)
	{
		hCom = CreateFile(COM,GENERIC_READ|GENERIC_WRITE,0,NULL,
						OPEN_EXISTING,0,NULL);
		if (hCom == INVALID_HANDLE_VALUE)	     
		{
			OutputDspInfo(OUTPUT_ERROR, "Fail to open %s errno= %d\n",COM, GetLastError());
			Sleep(2000);
		}
	}
 
   /* ************* */
	fSuccess=SetupComm(hCom,CBINQU,CBOUTQU);
	if(!fSuccess)
	{
		OutputDspInfo(OUTPUT_ERROR, "SetupComm ERRNO= %d \n",GetLastError());
		CloseHandle(hCom);
		return INVALID_HANDLE_VALUE;
	}

   /* ************* */
	fSuccess=GetCommState(hCom,&dcb);
	if(!fSuccess)
	{
		OutputDspInfo(OUTPUT_ERROR, "GetCommState ERRNO= %d \n",GetLastError());
		CloseHandle(hCom);
		return INVALID_HANDLE_VALUE;
	}

	OutputDspInfo(0, "GetCommState fParity=%d fErrorChar=%d ErrorChar=%x Parity=%d BaudRate=%d\n",
		dcb.fParity,dcb.fErrorChar,dcb.ErrorChar,dcb.Parity,dcb.BaudRate);

	
  /* ******************* */
	dcb.BaudRate=Baud_Rate ;
	dcb.ByteSize=ByteSize;	//8
	dcb.Parity=Parity;		//EVENPARITY;
	dcb.StopBits=StopBits;	//ONESTOPBIT;
	dcb.fParity = TRUE;
	dcb.fErrorChar = TRUE;
	dcb.ErrorChar = (char)0;

	fSuccess=SetCommState(hCom,&dcb);
	if(!fSuccess)
	{
		OutputDspInfo(OUTPUT_ERROR, "SetCommState ERRNO= %d \n",GetLastError());
		CloseHandle(hCom);
		return INVALID_HANDLE_VALUE;
	}

	OutputDspInfo(0, "SetCommState fParity=%d fErrorChar=%d ErrorChar=%x Parity=%d BaudRate=%d\n",
		dcb.fParity,dcb.fErrorChar,dcb.ErrorChar,dcb.Parity,dcb.BaudRate);

	
  /* ******************* */
	fSuccess=GetCommTimeouts(hCom,&timeout);
	if(!fSuccess)
	{
		OutputDspInfo(OUTPUT_ERROR, "GetCommTimeouts ERRNO= %d \n",GetLastError());
		CloseHandle(hCom);
		return INVALID_HANDLE_VALUE;
	}

  /* ********************** */
//	timeout.ReadIntervalTimeout         = 20; /* 2000ms */
//	timeout.ReadTotalTimeoutMultiplier  = 5;   /* 50ms */
//	timeout.ReadTotalTimeoutConstant    = 10; /* 3.0s */
//	timeout.WriteTotalTimeoutMultiplier = 50;   /* 50ms */
//	timeout.WriteTotalTimeoutConstant   = 300; /* 3.0s */

//	timeout.ReadIntervalTimeout         = RD_INTERVAL_TIMEOUT; // 13ms
	timeout.ReadIntervalTimeout         = 200; //200; // 13ms
	timeout.ReadTotalTimeoutMultiplier  = RD_TOTAL_TIMEOUT_MUL; // 5ms
	timeout.ReadTotalTimeoutConstant    = RD_TOTAL_TIMEOUT_CNT; // 14ms
	timeout.WriteTotalTimeoutMultiplier = WT_TOTAL_TIMEOUT_MUL; // 5ms
	timeout.WriteTotalTimeoutConstant   = WT_TOTAL_TIMEOUT_CNT; // 14ms
	
	fSuccess=SetCommTimeouts(hCom,&timeout);
	if(!fSuccess)
	{    
		OutputDspInfo(OUTPUT_ERROR, "SetCommTimeouts ERRNO= %d \n",GetLastError());
		CloseHandle(hCom);
		return INVALID_HANDLE_VALUE;
	}
	return(hCom);
}
#endif //WINDOWS_OS






#ifdef SUN_OS

extern int	g_iAlarmFlag;                        

/* Open the serial communication Port */  

int ComInit(char * strComDev, int Baud_Rate, int ByteSize,int Parity, int Modem) 
{
	int    fd,iBaud;  
	char   strDevName[256];  
	struct termios sTerm;  


	strcpy(strDevName, "/dev/");  
	strcat(strDevName, strComDev);  

	fd = -1;
	while((fd = open(strDevName, O_RDWR)) < 0)
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to open %s: %s\n",strDevName, strerror(errno));  
		sleep(2);
	}

	if (isatty(fd) == 0)
	{  
		OutputDspInfo(OUTPUT_ERROR,"COM device: %s is not a tty\n", strDevName);  
		close(fd);
		return(-1);  
	}

	/* fetch then set serial com port's status */  
	if (tcgetattr(fd, &sTerm) < 0)  
	{  
		OutputDspInfo(OUTPUT_ERROR,"tcgetattr error\n");  
		close(fd);
		return(-1);  
	}
	if (ByteSize == 8)  
		sTerm.c_cflag = CS8;  
	else if (ByteSize == 7)  
		sTerm.c_cflag = CS7;  
	else
	{
		OutputDspInfo(OUTPUT_WARN,"warning: unknown Byte Size %d, use CS8\n",ByteSize);
		sTerm.c_cflag = CS8;  
	}

	if (Parity == EVEN)  
		sTerm.c_cflag |= PARENB;  
	else if (Parity == ODD)  
		sTerm.c_cflag |=  PARENB | PARODD;  
	else if (Parity != NONE)
	{
		OutputDspInfo(OUTPUT_WARN,"warning: unknown parity %d, use EVEN\n",Parity);  
		sTerm.c_cflag |= PARENB;
	}  

	sTerm.c_cflag |= CREAD |	/* enable receiver */  
			HUPCL;			/* lower modem lines on last close */  
					/* 1 stop bit (since CSTOPB off) */  
	if (Modem == 0)  
		sTerm.c_cflag |= CLOCAL;		/* ignore modem status lines */

	sTerm.c_oflag  = 0;			/* turn off all output processing */  
	sTerm.c_iflag  = 0;			/* turn off all input processing */  
	sTerm.c_iflag  = /* IXON | IXOFF |	 Xon/Xoff flow control (default) */  
			IGNBRK |		/* ignore breaks */  
/*			IGNPAR;	 */		/* ignore input parity errors */  
			INPCK;			/*Enable input parity check.

	sTerm.c_lflag  = 0;
						/* everything off in local flag:  
						disables canonical mode, disables  
						signal generation, disables echo */  

	sTerm.c_cc[VMIN]  = 255;	/* 255 byte at a time */  
	sTerm.c_cc[VTIME] = 1;		/* timeout 1/10 sec. */  

	if (Baud_Rate == 38400) iBaud = B38400;  
	else if (Baud_Rate == 19200) iBaud = B19200;  
	else if (Baud_Rate == 9600) iBaud = B9600;  
	else if (Baud_Rate == 4800) iBaud = B4800;  
	else if (Baud_Rate == 2400) iBaud = B2400;  
	else if (Baud_Rate == 1800) iBaud = B1800;  
	else if (Baud_Rate == 1200) iBaud = B1200;  
	else if (Baud_Rate == 600) iBaud = B600;  
	else if (Baud_Rate == 300) iBaud = B300;  
	else if (Baud_Rate == 200) iBaud = B200;  
	else if (Baud_Rate == 150) iBaud = B150;  
	else if (Baud_Rate == 134) iBaud = B134;  
	else if (Baud_Rate == 110) iBaud = B110;  
	else if (Baud_Rate == 75) iBaud = B75;  
	else if (Baud_Rate == 50) iBaud = B50;  
	else
	{  
		OutputDspInfo(OUTPUT_WARN,"warning: invalid baud rate: %s\n", Baud_Rate);  
		close(fd);
		close(fd);
		return(-1);  
	}  
	cfsetispeed(&sTerm, iBaud);  
	cfsetospeed(&sTerm, iBaud);  
	if(tcsetattr(fd, TCSANOW, &sTerm) < 0) /* set attributes */  
	{  
		OutputDspInfo(OUTPUT_ERROR,"tcsetattr error!\n");  
		close(fd);
		return(-1);  
	}  
	return(fd);  
}
  
#endif