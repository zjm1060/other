/********************************************
* Source file :	com_init_s.c				*
* Function :	serial port initial program	*
*  NAME :  			              			*
*  DATE:                August,2006         *
********************************************/

#include "..\inc\iec101s_com.h"

#define	DEBUG
                        
extern int	g_iAlarmFlag;                        

/* Open the serial communication Port */  

int ComInit(char * strComDev, int Baud_Rate, int ByteSize,int Parity, int Modem) 
{
	int    fd,iBaud;  
	char   strDevName[100];  
	struct termios sTerm;  

/* first open the device */  

	strcpy(strDevName, "/dev/");  
	strcat(strDevName, strComDev);  

#ifdef DEBUG
	sprintf(infobuf,"open %s\n",strDevName);
#endif

	fd = -1;
	while(fd < 0)
	{
		if ( (fd = open(strDevName, O_RDWR)) < 0)
		{  
			sprintf(infobuf,"iec101s_com: can't open %s: %s\n",strDevName, strerror(errno));  
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			sleep(3);
/*			return(-1); */
			continue;  
		}
	} 
	if (isatty(fd) == 0)
	{  
		sprintf(infobuf,"!!!iec101s_com: COM device: %s is not a tty\n", strDevName);  
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
/*
		sprintf(gMsgString,"iec101s_com: Com device %s is not a tty\n",strDevName);
		gLogFile.LogMsg(INITCOM,gMsgString);
*/
		return(-1);  
	}  
	sprintf(infobuf,"%s is a tty, fd=%d\n",strDevName,fd);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
/* fetch then set serial com port's status */  

	if (tcgetattr(fd, &sTerm) < 0)  
	{  
		sprintf(infobuf,"iec101s_com: tcgetattr error\n");  
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
/*
		sprintf(gMsgString,"iec101s_com: tcgetattr error!\n");
		gLogFile.LogMsg(INITCOM,gMsgString);
*/
		return(-1);  
	}
	if (ByteSize == 8)  
	{  
		sprintf(infobuf,"Byte Size = 8\n");  
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
		sTerm.c_cflag = CS8;  
	}
	else if (ByteSize == 7)  
	{  
		sprintf(infobuf,"Byte Size = 7\n");  
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
		sTerm.c_cflag = CS7;  
	}
	else
	{
		sprintf(infobuf,"warning: unknown Byte Size %d\n",ByteSize);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
		sTerm.c_cflag = CS8;  
	}

	if (Parity == EVEN)  
		sTerm.c_cflag |= PARENB;  
	else if (Parity == ODD)  
		sTerm.c_cflag |=  PARENB | PARODD;  
	else if (Parity != NONE)
	{
		sprintf(infobuf,"unknown parity %d\n",Parity);  
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
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
		sprintf(infobuf,"iec101s_com: invalid baud rate: %s\n", Baud_Rate);  
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
/*
		ssprintf(infobuf,gMsgString,"iec101cc_com: invalid baud rate: %s\n", Baud_Rate);
		gLogFile.LogMsg(INITCOM,gMsgString);
*/
		return(-1);  
	}  
	cfsetispeed(&sTerm, iBaud);  
	cfsetospeed(&sTerm, iBaud);  
	if(tcsetattr(fd, TCSANOW, &sTerm) < 0) /* set attributes */  
	{  
		sprintf(infobuf,"iec101cc_com: tcsetattr error!\n");  
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
/*
		ssprintf(infobuf,gMsgString,"iec101cc_com: tcsetattr error!\n");
		gLogFile.LogMsg(INITCOM,gMsgString);
*/
		return(-1);  
	}  
	return(fd);  
}
  
void sig_alrm(int signo)  
{  
	g_iAlarmFlag = 1;  
/*	sprintf(infobuf,"Recevice Timeout\n"); */
	return;  
}  
