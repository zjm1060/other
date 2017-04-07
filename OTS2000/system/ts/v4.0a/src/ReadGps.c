#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"

#ifdef SUN_OS
#include <termios.h>
#endif


#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "../../../lan/v4.0a/lib/lan.lib")

#define EVEN            0
#define ODD             1
#define NONE            2

#define	YES		1
#define	NO		0

char		commbuf[100],gps[100];
int		    fp, k,val,timeout;
char		comm_port[60];
struct tm	devtime;
int 		n;

char 	buf[100],bufw[100],bufr[100];

#ifdef SUN_OS
time_t time_sec;

//void sleep(int n);

/* Open the SUN Communication Port */  
int ComInit(char * strComDev, int Baud_Rate, int ByteSize,int Parity, int Modem) 
{
	int    fd,iBaud;  
	char   strDevName[100];  
	struct termios sTerm;  

    /* first open the device */  
    strcpy(strDevName, strComDev);

	fd = -1;
	while(fd < 0)
	{
		if ( (fd = open(strDevName, O_RDWR)) < 0)
		{  printf("ReadGps: can't open %s",strDevName); sleep(3); continue;  }
	} 

	if (isatty(fd) == 0)
	{  printf("!!!ReadGps: %s is not a tty\n", strDevName);  return(-1);  }  

    /* fetch then set modem's terminal status */  
	if (tcgetattr(fd, &sTerm) < 0)  
	{  	printf("!!!ReadGps: tcgetattr error\n");  	return(-1); }

	if (ByteSize == 8)  
	{  printf("ReadGps: Byte Size = 8\n");  sTerm.c_cflag = CS8;	}
	else 
	if (ByteSize == 7)  
	{  printf("Byte Size = 7\n");  sTerm.c_cflag = CS7; }
	else
	{  printf("ReadGps: warning: unknown Byte Size %d\n",ByteSize); sTerm.c_cflag = CS8; }

	if (Parity == EVEN)  sTerm.c_cflag |= PARENB;  
	else 
	if (Parity == ODD)   sTerm.c_cflag |= PARENB | PARODD;  
	else 
	if (Parity != NONE)
	{
		printf("ReadGps: unknown parity %d\n",Parity);  sTerm.c_cflag |= PARENB;		
	}  

	sTerm.c_cflag |= CREAD |		/* enable receiver */  
			HUPCL;			/* lower modem lines on last close */  
						/* 1 stop bit (since CSTOPB off) */  
	if (Modem == 0)  
		sTerm.c_cflag |= CLOCAL;	/* ignore modem status lines */

	sTerm.c_oflag  = 0;			/* turn off all output processing */  
	sTerm.c_iflag  = 0;			/* turn off all input processing */  
	sTerm.c_iflag  = /* IXON | IXOFF |	 Xon/Xoff flow control (default) */  
			IGNBRK |		/* ignore breaks */  
/*			IGNPAR;*/			/* ignore input parity errors */  
			INPCK;			/*Enable input parity check.*/
	sTerm.c_lflag  = 0;
						/* everything off in local flag:  
						disables canonical mode, disables  
						signal generation, disables echo */  

	sTerm.c_cc[VMIN]  = 255;		/* 1 byte at a time */  
	sTerm.c_cc[VTIME] = 2;			/* timeout 1/10 sec. */  

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
	{  	printf("!!!ReadGps: invalid baud rate: %s\n", Baud_Rate);  return(-1); }  

	cfsetispeed(&sTerm, iBaud);  
	cfsetospeed(&sTerm, iBaud);  
	if(tcsetattr(fd, TCSANOW, &sTerm) < 0) /* set attributes */  
	{  printf("!!!ReadGps: tcsetattr error!\n");  return(-1);  }  

	printf("ReadGps: %s init OK.\n", strDevName); 
	return(fd);  
}

int Read_GPSclock ()
{
	int	i,j,k,n;
	int    yy,MM,dd,hh,mm,ss,num;
	time_t newtime;
        
	for ( i = 0; i<100; i++ ) 
	{ 
		buf[i] = 0; 
	}

	while(( fp=ComInit(comm_port,9600,8,EVEN,0))==-1 )
	{ 
		printf ( "Device %s can't opened\n", comm_port ); 
		sleep (10); 
	}

	printf ( "comm_port(GPS) READY!\n" );

	if (( n = read ( fp, buf,100)) ==-1)
	{ 
		printf ( "read %s error!\n", comm_port );
		close(fp);
		return(-1); 
	}
	close(fp);

	printf ( "GPS CLOCK: %s\n", buf );

	for ( i = 0; i<80; i++ ) if ( buf[i] == 1) { i++; break; } /* '1' is ascii 'SOH' */
	if ( i>79){ printf ( "read devtime: data error1 \n" ); return(-1); }
	for ( j= 0; j<20; j++ ) { buf[j] = buf[i]; i++; }
	if (( buf[2] !=':')|( buf[5] !=':')|( buf[8] !=':')|( buf[11] !=':')|( buf[14] !=':'))
	{ printf ( "read devtime: data error2 \n" ); return(-1); }
	yy = ( buf[0]&0x0f )*10+( buf[1]&0x0f );
	MM = ( buf[3]&0x0f )*10+( buf[4]&0x0f );
	dd = ( buf[6]&0x0f )*10+( buf[7]&0x0f );
	hh = ( buf[9]&0x0f )*10+( buf[10]&0x0f );
	mm = ( buf[12]&0x0f )*10+( buf[13]&0x0f );
	ss = ( buf[15]&0x0f )*10+( buf[16]&0x0f );
	if (( MM>12)||( dd>31)||(hh>23)||(mm>59)||( ss>59))
	{ 
		printf ( "time data error\n" ); 
		return (-1); 
	}

	/* set time  */
	if(strcmp(gps,"IWHR_GPSCLOCK")==0)
	{	
	 	/*tm_year is years since 1900, after 2000 must add 100*/
		if (yy<90)
		 	k= 100; 
		else 	
		 	k= 0;	
		devtime.tm_year =yy+k; devtime.tm_mon =MM-1; devtime.tm_mday =dd;
		devtime.tm_hour = hh;	 devtime.tm_min =mm;	 devtime.tm_sec= ss;

	}
	else
		return -1;
	
	 	
	if (( newtime =mktime ( &devtime )) ==-1)
	{ 
	 	printf ( "server mktime () error!\n" );
	 	return -1;
	}	
	
	 	 	
	if(stime(&newtime)<0)	
		return -1;
	
	
	printf("current time is %02d,%02d,%02d,%02d,%02d,%02d\n ,set clock ok\n",devtime.tm_year,devtime.tm_mon,devtime.tm_mday,
	   devtime.tm_hour,devtime.tm_min,devtime.tm_sec);
	   
		  
	return 0;	
		
}


#else
/*
void sleep(int n)
{
	Sleep(n*1000);
}
*/
int stime(time_t newtime)
{ 
  SYSTEMTIME st;
  struct tm timeptr;
  if( newtime==0 )return(-1);
  timeptr=*localtime(&newtime);
  GetLocalTime(&st);
  st.wYear       = timeptr.tm_year+1900;
  st.wMonth	     = timeptr.tm_mon+1;	  
  st.wDay        = timeptr.tm_mday;	
  st.wHour       = timeptr.tm_hour;
  st.wMinute     = timeptr.tm_min;
  st.wSecond     = timeptr.tm_sec; 
  st.wDayOfWeek  = timeptr.tm_wday;
  if(!SetLocalTime(&st)) 
  { 
	  printf("SetLocalTime() error!\n"); return(-1); 
  }
  else
  { printf("\nSET LOCAL TIME: %04d-%02d-%02d %02d:%02d:%02d\n",
					st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
  }
  return(0);
}  	
void Read_GPSclock()
{ DCB    dcb;
  HANDLE hCom;
  BOOL   fSuccess;
  int    i,k,yy,MM,dd,hh,mm,ss,num;
  time_t newtime;
  char bufr[1], buf[100];
  struct tm devtime;

  for(;;)
  { for(i=0;i<100;i++) { buf[i]=0; }/* clear the receive buffer */
	hCom=CreateFile
	(comm_port,GENERIC_READ | GENERIC_WRITE,
	   0,    
	   NULL, 
	   OPEN_EXISTING, 
	   0,    
	   NULL 
	);
	if(hCom==INVALID_HANDLE_VALUE)
    { printf("%s can't open!\n",comm_port); Sleep(4000); continue; }
	fSuccess=GetCommState(hCom,&dcb);
	if(!fSuccess)
	{ printf("can't get %s state!\n",comm_port); Sleep(4000); continue; }
	/*Fill in the DCB: baud=9600,8 data bits,even party,1 stop bit*/
	dcb.BaudRate=9600;
	dcb.ByteSize=8;
	dcb.Parity  =EVENPARITY;
	dcb.StopBits=ONESTOPBIT;
	fSuccess=SetCommState(hCom, &dcb);
	if(!fSuccess)
	{ printf("can't set %s state!\n",comm_port); Sleep(4000); continue; }
    i=0;  
    printf("%s init OK!\n",comm_port);
    for(;;)
    { ReadFile(hCom,bufr,1,&num,NULL);
      if(bufr[0]==1)break; else continue; /*'1' is ascii 'SOH'*/
    }	
    i=0;
    for(;;)
    { ReadFile(hCom,bufr,1,&num,NULL); buf[i]=bufr[0]; i++; 
      if((bufr[0]==10)||(bufr[0]==13)||(i>=100))break; 
      /*<LF>is 10, <CR> is 13,end of receive */
    }
    CloseHandle(hCom);   
    if(i>=100){ printf("readdevice error!\n"); Sleep(4000); continue;} 
    printf(">>>>>>>>>> IWHR GPS CLOCK: %s\n",buf);	
    if((buf[2]!=':')||(buf[5]!=':')||(buf[8]!=':')||(buf[11]!=':')||(buf[14]!=':'))
    { printf("read %s data err!\n",comm_port); Sleep(4000); continue; }
    yy=(buf[0]&0x0f)*10+(buf[1]&0x0f);
    MM=(buf[3]&0x0f)*10+(buf[4]&0x0f); 
    dd=(buf[6]&0x0f)*10+(buf[7]&0x0f);
    hh=(buf[9]&0x0f)*10+(buf[10]&0x0f);
    mm=(buf[12]&0x0f)*10+(buf[13]&0x0f);
    ss=(buf[15]&0x0f)*10+(buf[16]&0x0f);
    if((MM>12)||(dd>31)||(hh>23)||(mm>59)||(ss>59))	
    { printf("time data err!\n"); sleep(4); continue; }
    if( (hh==0)&&(mm==0)&&(ss==0) ){sleep(1); continue;}
    if((newtime=time(NULL))<0) {sleep(1); continue;}
    devtime=*localtime(&newtime);		 
    if(yy<30)k=100; else k=0;	/* tm_year is years since 1900 */
    devtime.tm_year=yy+k;  devtime.tm_mon=MM-1; devtime.tm_mday=dd;
    devtime.tm_hour=hh;	devtime.tm_min=mm;   devtime.tm_sec=ss;
    if((newtime=mktime(&devtime))<0)
    { printf("mktime() error!\n"); sleep(1); continue; }
    if(stime(newtime)==-1)
    { printf("stime() error!\n"); sleep(1); continue; }
  }				   		 
} 

#endif

int main(int argc, char *argv[])
{
	CreateLockFile(".readgps.lock");
	if(-1==InitSysLog(H9000_MODULE_READGPS, "ReadGps"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	

	if(-1==MountDB(DATABASE_NAME))
	{
		printf( "Fail to MountDB!\n");
	}

	GetEmsString("GPS_PORT", (char *)comm_port);
	
	GetEmsString("GPS_CLOCK", (char *)gps); 
	
	
	printf("comm_port=%s\n",comm_port);
 
	for (;;)
	{
		sleep(1);
			
		if (strcmp(gps,"IWHR_GPSCLOCK")==0)		Read_GPSclock ();	
		else							
		{ 	printf ( "\n No this CLOCK device !\n" ); 
			exit(0); 
		}
 	} 

}
