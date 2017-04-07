 /*name mod_test1.c*/

/*
This sample program uses the SA85.SYS device driver to access the
Modbus Plus network to read some holding registers from PLC cpu address 12.
*/

#define STRICT
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <signal.h>
#include "netbios.h"
#include "netlib.h"

int main(void);

int main()
	{
	NCB *nd;
	int ret_val;

	unsigned char rcv_buff[1024];
	
	unsigned char cmd_buff[20];
    unsigned char send_buff[256];

	unsigned int i,j,ii,jj;
	unsigned short data,data1,data2;
	if ((nd = ncb_open("DM.01.0.0.0.0", 0)) == NULL) 
	{
		printf("Unable to open DATA MASTER path.\n");
		exit(1);
	}

	printf("Path %02X opened\n", nd->NCB_NUM);
	printf("NCB.length = %d \n",nd->NCB_LENGTH);
	printf("NCB.RTO = %d \n",nd->NCB_RTO);
	printf("NCB.STO = %d \n",nd->NCB_STO);
	printf("Routing info: %c%c.%d.%d.%d.%d.%d\n",
	nd->NCB_CALLNAME[0],
	nd->NCB_CALLNAME[1],
	nd->NCB_CALLNAME[2],
	nd->NCB_CALLNAME[3],
	nd->NCB_CALLNAME[4],
	nd->NCB_CALLNAME[5],
	nd->NCB_CALLNAME[6]);
    i = 5;
	j = 5;
	while( !kbhit() )
	{   
		
		Sleep(9000);
		/******** Read data from PLC **********/
		printf("=================== Read data from PLC ========================\n");
		cmd_buff[0] = 0x01;	/*slave address*/
		cmd_buff[1] = 0x03;	/*command*/
	    cmd_buff[2] = 0x00;	/*offset high*/
		cmd_buff[3] = 0x01;	/*offset low*/
		cmd_buff[4] = 0x00;	/*reg count high*/
		cmd_buff[5] = 0x10;   /*reg count low*/

		if (ncb_send(nd, 6, cmd_buff, 5) != 0)
		{
		  /*send the command*/
		  printf("Send error: %d.\n", nd->NCB_RETCODE);
		  ret_val = ncb_close(nd);	/*close the path*/
		  exit(1);
		}
		//printf("send cmd. OK!\n");
		//printf("test_4\n");
		if (ncb_receive_wait(nd,rcv_buff,200) != 0)
		{
		  /*try to receive*/
		  printf("\nReceive error: %d.\n", nd->NCB_RETCODE);
		  exit(1);
		}
		else
		{    
			 //printf(" RECEIVE READ DATA OK!\n");
             jj=0;
             for(ii=0;ii<35;ii++)
			 { 
				  printf(" %02x",rcv_buff[ii]);
                  jj = jj +1;
				  if(jj==9) { jj=0;printf("\n");} 
             }
             printf("<== second\n"); 
			 printf("\n");
             
			 jj=0;
             for(ii=3;ii<35;)
			 { 
                  data1 = (unsigned int)(rcv_buff[ii])<<8;
                  data2 = (unsigned int)(rcv_buff[ii+1]);
				  data = data1 + data2;
                  printf(" %04x",data);
				  ii=ii+2;
				  jj = jj +1;
				  if(jj==9) { jj=0;printf("\n");} 
             }
             printf("<== second\n"); 
			 printf("\n");

		}

		/******** Write data toPLC **********/
		printf("=================== Write data to PLC ========================\n");
		send_buff[0] = 0x01;	/*slave address*/
		send_buff[1] = 0x10;	/*command*/
	    send_buff[2] = 0x01;	/*offset high*/
		send_buff[3] = 0xf3;	/*offset low*/
		send_buff[4] = 0x00;	/*reg count high*/
		send_buff[5] = 0x06;	/*reg count low*/
		send_buff[6] = 0x0c;	/*bytes count high*/
		
		send_buff[7] = 0x00;  /*cmd. length high */
        send_buff[8] = 0x0a;	/*cmd. length low */

		send_buff[9]  = 0x11+i;  /*R1_H */                                
        send_buff[10] = 0x22+i;	/*R1_L */

		send_buff[11] = 0x33+i;  /*R2_H */
        send_buff[12] = 0x44+i;	/*R2_L */

		send_buff[13] = 0x55;  /*R3_H */
        send_buff[14] = 0x66;	/*R3_L */

		send_buff[15] = 0x77;  /*R4_H */
        send_buff[16] = 0x88;	/*R4_L */
		
		send_buff[17] = 0x99;  /*R5_H */
        send_buff[18] = 0xAA;	/*R5_L*/
        i = i + 1 ;
		if (ncb_send(nd, 19, send_buff, 5) != 0)
		{
		  /*send the command*/
		  printf("Send WRITE CMD.error: %d.\n", nd->NCB_RETCODE);
		  ret_val = ncb_close(nd);	/*close the path*/
		  exit(1);
		}
		
		if (ncb_receive_wait(nd,rcv_buff,10) != 0)
		{
		  /*try to receive*/
		  printf("\nReceive error: %d.\n", nd->NCB_RETCODE);
		  exit(1);
		}
		else
		{    //printf(" RECEIVE CMD. DATA OK!\n");
             jj=0;
             for(ii=0;ii<6;ii++)
			 { 
				  printf(" %02x",rcv_buff[ii]);
                  jj = jj +1;
				  if(jj==9) { jj=0;printf("\n");} 
             }
              printf("\n");
		   /*
             jj=0;
             for(ii=0;ii<17;ii++)
			 { 
                  i =   (unsigned int)(rcv_buff[ii]) << 8
					  + (unsigned int)(rcv_buff[ii+1]);

				  printf(" %04X=(%05d) ",i,i);
                  jj = jj +1;
				  if(jj==8) { jj=0;printf("\n");} 
             }
              printf("\n");
	       */

		}


	} // while()

	ncb_close(nd);		/*close the path*/
	//return 0;
	}



