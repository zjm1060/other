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
void control_c(int sig);

/*
global variables
*/

//char mbuffer[256];
int completed;

int main()
	{
	NCB *nd;
	int ret_val;

	unsigned char data_rcv_buff[1024];
	
	unsigned char read_data_cmd_buff[20];
    unsigned char write_data_cmd_buff[256];

	unsigned int i,j,ii,jj;
	unsigned short data,data1,data2;

/*
    if( signal( SIGINT, control_c ) == SIG_ERR ) {
        printf( "Unable to redirect Control-C.\n" );
        exit( 1 );
    }
*/
printf("test_1\n");
	if ((nd = ncb_open("DM.01.0.0.0.0", 0)) == NULL) {
		printf("Unable to open DATA MASTER path.\n");
		exit(1);
		}

printf("test_2\n");
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


printf("test_3\n");
    i = 5;
	j = 5;
	while( !kbhit() )
	{   
		
        loop_1:
		Sleep(900);
	    
		//read_data_cmd_buff[0] = 0x01;	/*slave address*/
		//read_data_cmd_buff[1] = 0x03;	/*command*/
	    //read_data_cmd_buff[2] = 0x00;	/*offset high*/
		//read_data_cmd_buff[3] = 0x00;	/*offset low*/
		//read_data_cmd_buff[4] = 0x00;	/*reg count high*/
		//read_data_cmd_buff[5] = 0x10; /*reg count low*/

		read_data_cmd_buff[0] = 0x01;	/*slave address*/
		read_data_cmd_buff[1] = 0x03;	/*command*/
	    read_data_cmd_buff[2] = 0x0f;	/*offset high*/
		read_data_cmd_buff[3] = 0xf0;	/*offset low*/
		read_data_cmd_buff[4] = 0x00;	/*reg count high*/
		read_data_cmd_buff[5] = 0x10;   /*reg count low*/

		if (ncb_send(nd, 6, read_data_cmd_buff, 5) != 0)
		{
		  /*send the command*/
		  printf("Send error: %d.\n", nd->NCB_RETCODE);
		  ret_val = ncb_close(nd);	/*close the path*/
		  exit(1);
		}
//printf("send cmd. OK!\n");
//printf("test_4\n");
		if (ncb_receive_wait(nd,data_rcv_buff,200) != 0)
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
				  printf(" %02x",data_rcv_buff[ii]);
                  jj = jj +1;
				  if(jj==9) { jj=0;printf("\n");} 
             }
             printf("<== second\n"); 
			 printf("\n");
             
			 jj=0;
             for(ii=3;ii<35;)
			 { 
                  data1 = (unsigned int)(data_rcv_buff[ii])<<8;
                  data2 = (unsigned int)(data_rcv_buff[ii+1]);
				  data = data1 + data2;
                  printf(" %04x",data);
				  ii=ii+2;
				  jj = jj +1;
				  if(jj==9) { jj=0;printf("\n");} 
             }
             printf("<== second\n"); 
			 printf("\n");

		}
        //goto loop_1;
		write_data_cmd_buff[0] = 0x01;	/*slave address*/
		write_data_cmd_buff[1] = 0x10;	/*command*/
	    write_data_cmd_buff[2] = 0x01;	/*offset high*/
		write_data_cmd_buff[3] = 0xf3;	/*offset low*/
		write_data_cmd_buff[4] = 0x00;	/*reg count high*/
		write_data_cmd_buff[5] = 0x06;  /*reg count low*/
		write_data_cmd_buff[6] = 0x0c;	/*bytes count high*/
		
		write_data_cmd_buff[7] = 0x00;  /*cmd. length high */
        write_data_cmd_buff[8] = 0x0a;	/*cmd. length low */

		write_data_cmd_buff[9]  = 0x11+i;  /*R1_H */
        write_data_cmd_buff[10] = 0x22+i;	/*R1_L */

		write_data_cmd_buff[11] = 0x33+i;  /*R2_H */
        write_data_cmd_buff[12] = 0x44+i;	/*R2_L */

		write_data_cmd_buff[13] = 0x55;  /*R3_H */
        write_data_cmd_buff[14] = 0x66;	/*R3_L */

		write_data_cmd_buff[15] = 0x77;  /*R4_H */
        write_data_cmd_buff[16] = 0x88;	/*R4_L */
		
		write_data_cmd_buff[17] = 0x99;  /*R5_H */
        write_data_cmd_buff[18] = 0xAA;	/*R5_L*/
        i = i + 1 ;
		if (ncb_send(nd, 19, write_data_cmd_buff, 5) != 0)
		{
		  /*send the command*/
		  printf("Send WRITE CMD.error: %d.\n", nd->NCB_RETCODE);
		  ret_val = ncb_close(nd);	/*close the path*/
		  exit(1);
		}
		
		if (ncb_receive_wait(nd,data_rcv_buff,10) != 0)
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
				  printf(" %02x",data_rcv_buff[ii]);
                  jj = jj +1;
				  if(jj==9) { jj=0;printf("\n");} 
             }
              printf("\n");
		   /*
             jj=0;
             for(ii=0;ii<17;ii++)
			 { 
                  i =   (unsigned int)(data_rcv_buff[ii]) << 8
					  + (unsigned int)(data_rcv_buff[ii+1]);

				  printf(" %04X=(%05d) ",i,i);
                  jj = jj +1;
				  if(jj==8) { jj=0;printf("\n");} 
             }
              printf("\n");
	       */

		}


	} // while()

	ncb_close(nd);		/*close the path*/

	exit(0);

	//return 0;
	}



/*
control_c


This routine replaces the control-C handler supplied by DOS.  When you
type a control_C, the program will vector to this routine, which will
set the completed flag to non-zero.  In this test program, this will cause
the infinite loop within main to terminate.
*/
void control_c(int sig)
{
    signal( SIGINT, SIG_IGN );          /* disable control-c */
    completed = 1;                      /* will cause main loop to complete */
    signal( SIGINT, control_c );        /* reset control-c handler */
}

