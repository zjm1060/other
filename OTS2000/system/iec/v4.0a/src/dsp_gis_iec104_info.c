/************************************************************
*  FILE NAME:           dsp_iec104_info.c   			*
*  FUNCTION:            Display message of iec104 communication	*
*			frame saved in shared memory		*
*  NAME :  					        	*
*  DATE:                Sept.,2006                		*
*								*
************************************************************/
#include "../inc/iec104_com.h"
#include "../inc/iec_cmm_def.h"


char infobuf[1024];
extern IEC_COMM_DSP_BUF *comm_dsp_buf_ptr;

#ifdef WINDOWS_OS
void ExitRoutine ( DWORD signal_type )
{
	char ch= getchar();

	if(ch=='q')
		exit(0);
}
#else ifdef SUN_OS
void Endproc ( int sig_no )
{
	char ch= getchar();
	
	if(ch=='q')
		exit(0);

	signal(SIGTERM,Endproc);
	signal(SIGINT,Endproc);
}
#endif

int main(int argc,char *argv[])
{ 
	char	tmp[1024];
	int		ptr, headptr, len;

	printf("\n### dsp_gis_iec104_info: for display the transmitted packet message of GIS IEC104\n\n");

	comm_dsp_buf_ptr= LinktoCommDspBuf();
	if((IEC_COMM_DSP_BUF*)-1==comm_dsp_buf_ptr)
	{
		printf("\nFail to link to gis iec104 comm dsp buf");
		exit(0);
	}

#ifdef WINDOWS_OS
	SetConsoleCtrlHandler ((void *)ExitRoutine,TRUE );
#else ifdef SUN_OS
	signal(SIGTERM,Endproc);
	signal(SIGINT,Endproc);
#endif
	
	ptr = 0;
	for(;;)
	{
		headptr = comm_dsp_buf_ptr->header_ptr;
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
			memcpy(tmp, &comm_dsp_buf_ptr->buf[ptr], len);
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
				memcpy(tmp, &comm_dsp_buf_ptr->buf[ptr], len);
				tmp[len] = 0;
				printf("%s",tmp);
				ptr = ptr + len;
				continue;
			}
			memcpy(tmp, &comm_dsp_buf_ptr->buf[ptr], len);
			tmp[len] = 0;
			printf("%s",tmp);
			if(headptr > 1022)
			{
				len = 1022;
				memcpy(tmp, &comm_dsp_buf_ptr->buf[0], len);
				tmp[len] = 0;
				printf("%s",tmp);
				ptr = len;
				continue;
			}
			memcpy(tmp, &comm_dsp_buf_ptr->buf[0], headptr);
			tmp[headptr] = 0;
			printf("%s",tmp);
			ptr = headptr;
		}
	}
}

