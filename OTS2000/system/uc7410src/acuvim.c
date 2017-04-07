/****************************************************************************************
    File name:	acuvim.c
	
    History: acuvim.c交流采样通讯；规约MODBUS   
    Version	邓小刚 	Date	05.8	Comment 
	wenzhengguo 20090207
****************************************************************************************/
#include        <stdio.h>
#include        <stdlib.h>
#include        <fcntl.h>
#include        <errno.h>
#include        <termios.h>
#include        <time.h>
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <sys/ioctl.h>
#include        <linux/if.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>
#include		<moxadevice.h>
#include	    <stddef.h>
#include	    <unistd.h>
#include        <pthread.h>
#include        <linux/sem.h>


#include "main.h"
#include "acuvim.h"




int acuvim(int port_fd, COMM_TASK_INFO *pTaskInfo, COMM_PARAM_INFO *comm_param_info);




int main(int argc, char *argv[])
{
	int		i, task_id;
	COMM_TASK_INFO	*pTaskInfo=NULL;
	COMM_PARAM_INFO	comm_param_info;
	int		port_fd=-1;	
	
	SetModuleName("ACUVIM");
	SetErrLogMode(argc, argv);
	
	SendTraceMsg("start to run acuvim comm program");
	
	if(-1==(task_id=GetTaskId(argc, argv)))
	{
		SendTraceMsg("Usage: program_name task_id, for example: acuvim.dxe 2");
		exit(-1);
	}
	if(task_id<0 || task_id>=MAX_COMM_TASK_NUM)
	{
		SendTraceMsg("Task id %d error: 0~%d", task_id, MAX_COMM_TASK_NUM-1);
		exit(-1);
	}

	if(!InitShareCommTable())
	{
		SendTraceMsg("Fail to mount share comm table");
		exit(-1);
	}	
	SendTraceMsg("Success to mount share comm table");
	
	pTaskInfo= &share_table->dev[task_id];
	SetModuleName(pTaskInfo->sect_name);
	
	if(!GetCommParamConf(&comm_param_info, pTaskInfo))
	{
		SendTraceMsg("Fail to read comm define file");
		exit(-1);
	}	
	SendTraceMsg("Success to read comm define file");
	
	for(;;)
	{
		if(-1==port_fd)
			port_fd=ComInit(&comm_param_info);
		
		if(-1!=port_fd)
		{
			if(!acuvim(port_fd, pTaskInfo, &comm_param_info))
			{
				close(port_fd);
				port_fd= -1;
			}
		}
		sleep(1);
	}
}

                                                                

int acuvim(int port_fd, COMM_TASK_INFO *pTaskInfo, COMM_PARAM_INFO *comm_param_info)
{
	int		i, unit, read_unit, ret;
	WORD	ReadStartAdd,CheckCode;     
	int		ReadLength,WriteLength,DataLen,RealLength;
	long	CRCCount;
	BYTE	ReadCmdBuf[10], TempBuf[READ_DATA_NUM*3];
	short	start_pnt;
	WORD	send_data[SEND_DATA_NUM];

	for(unit = 1; unit <= comm_param_info->unit_num; unit++)
	{
		ReadCmdBuf[0]	= (BYTE)unit;	/*address of slave*/
		ReadCmdBuf[1]   = 0x03;  /* function code */

		ReadStartAdd   =  READ_START_ADDR;
		ReadCmdBuf[2]  = (BYTE)(ReadStartAdd >>8) ;
		ReadCmdBuf[3]  = (BYTE)(ReadStartAdd);

		DataLen		   = READ_DATA_NUM;
		ReadCmdBuf[4]  = (BYTE)(DataLen>>8);
		ReadCmdBuf[5]  = (BYTE)(DataLen);

		CRCCount = 6;
		CheckCode = GetCrcCheck(CRCCount,ReadCmdBuf);
		ReadCmdBuf[CRCCount] = (BYTE)(CheckCode);
		ReadCmdBuf[CRCCount+1] = (BYTE)(CheckCode>>8);

    	WriteLength=8;
		
		PrintfData(WriteLength, ReadCmdBuf, 0);

		ret=write(port_fd, ReadCmdBuf, WriteLength);
        if(ret!=WriteLength)
		{
			SendTraceMsg("Fail to write port: ret= %d", ret);
			return FALSE;
		}
		
		usleep(1000*500); //500ms
		ReadLength = DataLen*2 + 5;
		ret= read(port_fd, TempBuf, ReadLength);

		PrintfData(ret, TempBuf, 1);
		
		if(ret!=ReadLength)
		{
			SendTraceMsg("Fail to read port: ret= %d", ret);
			continue;
		}

		CRCCount = DataLen*2 + 3;
		CheckCode = GetCrcCheck(CRCCount,TempBuf);

		if( TempBuf[0] > comm_param_info->unit_num)
		{
			SendTraceMsg("Received data addr error: %d", TempBuf[0]);
			continue;
		}

		if( TempBuf[1] != 0x03)
		{
			SendTraceMsg("Received cmd code error: %d", TempBuf[1]);
			continue;
		}
		
		if( TempBuf[2] != (BYTE)(DataLen*2))
		{
			SendTraceMsg("Received data length error");
			continue;
		}
		
		if( (TempBuf[DataLen*2+3] != (BYTE)(CheckCode))
			|| (TempBuf[DataLen*2+4] != (BYTE)((CheckCode>>8))))
		{
			SendTraceMsg("Received data crc check error");
			continue;
		}
		
		memset(send_data, 0, sizeof(send_data));
		send_data[0] = ((TempBuf[14])+(((WORD)(TempBuf[13]))<<8));//uab
		send_data[1] = ((TempBuf[16])+(((WORD)(TempBuf[15]))<<8));//ubc
		send_data[2] = ((TempBuf[18])+(((WORD)(TempBuf[17]))<<8));//uca
		send_data[3] = ((TempBuf[22])+(((WORD)(TempBuf[21]))<<8));//ia
		send_data[4] = ((TempBuf[24])+(((WORD)(TempBuf[23]))<<8));//ib
		send_data[5] = ((TempBuf[26])+(((WORD)(TempBuf[25]))<<8));//ic
		send_data[6] = ((TempBuf[38])+(((WORD)(TempBuf[37]))<<8));//p
		send_data[7] = ((TempBuf[46])+(((WORD)(TempBuf[45]))<<8));//q
		send_data[8] = ((TempBuf[4])+(((WORD)(TempBuf[3]))<<8));//hz
		send_data[9] = ((TempBuf[62])+(((WORD)(TempBuf[61]))<<8));//cos

		read_unit = TempBuf[0];
		start_pnt = (read_unit-1)*SEND_DATA_NUM;

		for(i = 0; i < SEND_DATA_NUM; i++)
			pTaskInfo->data_buf[i+start_pnt]= send_data[i];

		pTaskInfo->update_flag++;
		SendTraceMsg("update_flag= %d", pTaskInfo->update_flag);

		usleep(1000*500); //500ms
		
	}  //for(unit)
}


























