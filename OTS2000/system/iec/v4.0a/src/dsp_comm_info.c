/****************************************************************
*  FILE NAME:           dsp_iec104_info.c   			*
*  FUNCTION:            Display message of iec104 communication	*
*			frame saved in shared memory		*
*  NAME :  					        	*
*  DATE:                Sept.,2006                		*
*								*
****************************************************************/
#include "../inc/iec104s_com.h"

char infobuf[1024];

SHM_BUF *shm_buf_addr;
int shm_id_buf;

struct shm_rec *shm_rec_addr;
int shm_id_rec;

void Endproc(int sig_no)
{
	int i;

	printf("  ---- DSP_COMM_INFO exit signal %d received!  ----\n",sig_no);
	exit(0);
} 
                                                                
int main(int argc,char *argv[])
{ 
	char	tmp[1024];
	int	ptr, headptr,i,len;

	i = 0;
	signal(SIGKILL,Endproc);
	signal(SIGTERM,Endproc);
	signal(SIGINT,Endproc);

	printf("\n### dsp_iec_info: for display the transmitted messages with dispatch centers ***\n\n");
	for(;;)
	{
		printf("Please Select the station bellow to display ( number 1 -- 8 )\n\n");
		printf("\t1 NDC (国调主机1 IEC60870-5-104)\n");
		printf("\t2 NDC (国调主机2 IEC60870-5-104)\n");
		printf("\t3 CCDC (网调主机1 IEC60870-5-104)\n");
		printf("\t4 CCDC (网调主机2 IEC60870-5-104)\n");
		printf("\t5 BACKUP (备用)\n");
		printf("\t6 NDC (国调 IEC60870-5-101)\n");
		printf("\t7 CCDC (网调 IEC60870-5-101)\n");
		printf("\t8 CDC (梯调 IEC60870-6-TASE2)\n");
		printf("\nPlease Select the station ( number 1 -- 8 ):");

		scanf("%d",&i);
		if(i < 1 || i> 16)
		{
			printf("\n\nWrong Number %d! Please Select the station again\n\n",i);
			continue;
		}
		i = i -1;
	
/*      打开显示信息记录共享内存      */

		if((shm_id_buf=shmget( ftok (IEC104_FILE_KEY,(SHM_BUF_KEY_CHAR+i)),sizeof(SHM_BUF),0))==-1)
		{
			printf ( "shmkey %d shmid %d is not existed!\n",(SHM_BUF_KEY_CHAR+i),shm_id_buf);
			continue;
		}
		else
			printf( "create_shm shm_id_buf:shmget ");

		printf ("the shm_id_buf is:%d\n",shm_id_buf);

/*      获取记录区共享内存地址  */
		shm_buf_addr=( SHM_BUF *)shmat( shm_id_buf,0,SHM_RND);

		printf ( "the shm_buf_addr is:0x%x\n",shm_buf_addr );

		if ((long *)shm_buf_addr==(long *)-1)
		{
			printf("\n::Error shmat shm_buf_addr: errno=%d\n",errno);
			continue;
		}
		else
			printf("\n:: shmat shm_buf_addr: OK!\n");
		break;
	}

	ptr = 0;
	for(;;)
	{
		headptr = shm_buf_addr->header_ptr;
		if(ptr == headptr)
		{
			sleep(1);
			continue;
		}
		else if(ptr < headptr)
		{
			len = headptr-ptr;
			if(len > 1022)
				len = 1022;
			memcpy(tmp, &shm_buf_addr->buf[ptr], len);
			tmp[len] = 0;
			printf("%s",tmp);
			ptr = ptr + len;
			continue;
		}
		else
		{
			len = DSP_INFO_SIZE-ptr;
			if(len > 1022)
			{
				len = 1022;
				memcpy(tmp, &shm_buf_addr->buf[ptr], len);
				tmp[len] = 0;
				printf("%s",tmp);
				ptr = ptr + len;
				continue;
			}
			memcpy(tmp, &shm_buf_addr->buf[ptr], len);
			tmp[len] = 0;
			printf("%s",tmp);
			if(headptr > 1022)
			{
				len = 1022;
				memcpy(tmp, &shm_buf_addr->buf[0], len);
				tmp[len] = 0;
				printf("%s",tmp);
				ptr = len;
				continue;
			}
			memcpy(tmp, &shm_buf_addr->buf[0], headptr);
			tmp[headptr] = 0;
			printf("%s",tmp);
			ptr = headptr;
		}
	}
}

