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

#include "\home\ems\system\iec103tcp\v4.0a\inc\iec103_com.h"
                        
BOOL	       fSuccess;

int MakeLink()
{
	struct sockaddr_in	serv_addr;
    int on,timeout;
	if( strcmp(IEC103_IP,"")==0 )return( -1 );
    memset((char *)&serv_addr,0,sizeof(serv_addr));	
	
   	serv_addr.sin_addr.s_addr=inet_addr(IEC103_IP);     	
    serv_addr.sin_family     =AF_INET;
    serv_addr.sin_port       =htons( (unsigned short)IEC103_PORT );
    	
    if((iec_sock=socket(AF_INET,SOCK_STREAM,0))<0)    	
    {  
		printf("\n IEC103:can't open stream socket\n");
		closesocket( iec_sock );
        return( -1 );  
	}
    printf("\n IEC103:open stream socket OK \n");
	timeout=10000;
	setsockopt(iec_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)); 
    setsockopt(iec_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)); 
    if( connect(iec_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0 ) 
    { 
      printf("\n IEC103: Can't connect to WEB_DB(%s) !!!\n",IEC103_IP);
	  closesocket( iec_sock );
      return( -1 ); 
    }
    else
    { 
      printf("IEC103: make link to device(%s) ok ...\n",IEC103_IP);
	  on=1;
      setsockopt(iec_sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&on,sizeof (on));
 	  return( iec_sock );
 	}
}