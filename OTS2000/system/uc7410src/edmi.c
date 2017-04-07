
#include "main.h"
#include "edmi.h"



#define READ_DATA_NUM	16

UINT regAddr[READ_DATA_NUM]= 
						{  0x0169,0x0160,0x0161,0x0162,
							0x0069,0x0060,0x0061,0x0062,
							0x0369,0x0360,0x0361,0x0362,
							0x0269,0x0260,0x0261,0x0262
						};					




unsigned char sequence[MAX_EDMI_METER_NUM];

int edmi(int port_fd, COMM_TASK_INFO *pTaskInfo, COMM_PARAM_INFO *comm_param_info, EDMI_PARAM_TABLE	*edmi_param);


int GetCustomParamConf(COMM_PARAM_INFO *comm_param_info, COMM_TASK_INFO *pTaskInfo, EDMI_PARAM_TABLE *edmi_param)
{
	char	*filename=CONFIG_DEFINE_FILE;
	char	string[256], sect_name[MAX_DEF_SECTION_NAME_SIZE];
	UINT	value;
	int		i;
	
//SendTraceMsg("sectname: %s", pTaskInfo->sect_name);
	strcpy(sect_name, pTaskInfo->sect_name);

	if(!GetProfInt(filename, sect_name, "SOURCE_ADDR", &value))
		return FALSE;
	edmi_param->sourceAddr= value;
	SendTraceMsg("comm source addr: 0x%08x", edmi_param->sourceAddr);
	
	for(i=0; i<comm_param_info->unit_num; i++)
	{
		char	name[48];

		sprintf(name , "DEST_ADDR%d", i+1);
		if(!GetProfInt(filename, sect_name, name, &value))
			return FALSE;
		edmi_param->destAddr[i]= value;
		SendTraceMsg("comm dest addr: 0x%08x", edmi_param->destAddr[i]);
	}
	
	if(!GetProfString(filename, sect_name, "USER", string))
		return FALSE;
	if(strlen(string)>=MAX_EDMI_USER_NAME_SIZE)
	{
		SendTraceMsg("comm user name %s too long: max %d", string, MAX_EDMI_USER_NAME_SIZE);
		return FALSE;
	}
	strcpy(edmi_param->user, string);
	SendTraceMsg("comm user: %s", edmi_param->user);
	
	if(!GetProfString(filename, sect_name, "PASSWD", string))
		return FALSE;
	if(strlen(string)>=MAX_EDMI_PASSWD_SIZE)
	{
		SendTraceMsg("comm user passwd %s too long: max %d", string, MAX_EDMI_PASSWD_SIZE);
		return FALSE;
	}
	strcpy(edmi_param->passwd, string);
	SendTraceMsg("comm passwd: %s", edmi_param->passwd);
	
	return TRUE;
}

int main(int argc, char *argv[])
{
	int		i, task_id;
	COMM_TASK_INFO	*pTaskInfo=NULL;
	COMM_PARAM_INFO	comm_param_info;
	EDMI_PARAM_TABLE	edmi_param;
	int		port_fd=-1;	
	
	SetModuleName("EDMI");
	SetErrLogMode(argc, argv);
	
	SendTraceMsg("start to run EDMI comm program");
	
	if(-1==(task_id=GetTaskId(argc, argv)))
	{
		SendTraceMsg("Usage: program_name task_id, for example: edmi.dxe 2");
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

	if(comm_param_info.unit_num>MAX_EDMI_METER_NUM)
	{
		SendTraceMsg("meter num %d error max %d", comm_param_info.unit_num, MAX_EDMI_METER_NUM);
		exit(-1);
	}

	if(!GetCustomParamConf(&comm_param_info, pTaskInfo, &edmi_param))
	{
		SendTraceMsg("Fail to read comm define file");
		exit(-1);
	}
	SendTraceMsg("Success to read comm define file");
	
	for(i=0; i<MAX_EDMI_METER_NUM; i++)
		sequence[i]= 0x20;
	
	for(;;)
	{
		if(-1==port_fd)
			port_fd=ComInit(&comm_param_info);
		
		if(-1!=port_fd)
		{
			if(!edmi(port_fd, pTaskInfo, &comm_param_info, &edmi_param))
			{
				close(port_fd);
				port_fd= -1;
			}
		}
		sleep(3);
	}
}

int	AddDLEByte(int size, BYTE *buf)
{
	int		i, len;
	char	tmp[1024];

	tmp[0]= buf[0];
	len= 1;
	for(i=1; i<size; i++)
	{
		if(buf[i]==EDMI_STX || buf[i]==EDMI_ETX || buf[i]==EDMI_XON || buf[i]==EDMI_XOFF || buf[i]==EDMI_DLE)
		{
			tmp[len++]= EDMI_DLE;
			tmp[len++]= buf[i] + 0x40;
		}
		else
			tmp[len++]= buf[i];
	}
	memcpy(buf, tmp, len);
	return len;
}

int	DelDLEByte(int size, BYTE *buf)
{
	int		i, len;
	char	tmp[1024];

	for(i=0; i<size; i++)
	{
		if(buf[i] == EDMI_STX)
			break;
	}

	len= 0;
	for(; i<(size-1); i++)
	{
		BYTE	value= buf[i+1] - 0x40;

		if(buf[i]==EDMI_DLE &&
			(value==EDMI_STX || value==EDMI_ETX || value==EDMI_XON 
			|| value==EDMI_XOFF || value==EDMI_DLE))
		{
			tmp[len++]= value;
			i++;
		}
		else
			tmp[len++]= buf[i];
	}
	tmp[len++]= buf[i];

	memcpy(buf, tmp, len);
	return len;
}


WORD GetCrcCode(int byteNum, BYTE *buff)
{
	WORD	m,j,k,crc1;
	int		tmp;
    WORD	i,crc=0;

	for(i=0; i<byteNum; i++)
	{
		m=(crc >> 8 ) ^ buff[i] ;
		{
			k = m << 8;
			crc1 = 0;
			for( j = 0 ; j < 8 ; j++ )
			{
				if ( ( crc1 ^ k ) & 0x8000 )
					crc1 = ( crc1 << 1 ) ^ 0x1021;
				else
					crc1 <<= 1;
				k <<= 1;
			}
		}
		crc = (crc << 8 ) ^ crc1;
	}
	return crc;
}	



int LoginEDMI(int port_fd, int unit, EDMI_PARAM_TABLE *edmi_param, COMM_PARAM_INFO *comm_param_info)
{
	BYTE	sendBuf[256], tmp[256];
	int		ret, send_size, recv_size;
	WORD	check_code;
	long	CRCCount;
	UINT	sourceAddr, destAddr;
	
	read(port_fd, tmp, sizeof(tmp));
	
	sequence[unit]++;
	
	sendBuf[0] = EDMI_STX;	
	sendBuf[1] = 'E';
	memcpy(&sendBuf[2], &edmi_param->destAddr[unit], 4);
	memcpy(&sendBuf[6], &edmi_param->sourceAddr, 4); //0x012b1668
	sendBuf[10] = 0x00;	
	sendBuf[11] = sequence[unit];

	sprintf(tmp, "L%s,%s", edmi_param->user, edmi_param->passwd);
	send_size= strlen(tmp)+1; //add null termination
	memcpy(&sendBuf[12], tmp, send_size); 
	send_size += 12;

	check_code= GetCrcCode(send_size, sendBuf);
	sendBuf[send_size++]= (BYTE)(check_code>>8);
	sendBuf[send_size++]= (BYTE)check_code;

	send_size= AddDLEByte(send_size, sendBuf);
	sendBuf[send_size++]= EDMI_ETX;
	
	PrintfData(send_size, sendBuf, 0);
	
	ret=write(port_fd, sendBuf, send_size);
	if(ret!=send_size)
	{
		SendTraceMsg("Fail to write port: ret= %d", ret);
		return FALSE;
	}

	usleep(1000*500); //500ms
	recv_size= read(port_fd, tmp, sizeof(tmp));
	if(recv_size<5)
	{
		SendTraceMsg("Fail to read port: recv_size= %d", recv_size);
		return FALSE;
	}
	
	PrintfData(recv_size, tmp, 1);
	
	recv_size= DelDLEByte(recv_size, tmp);

	if(recv_size!=16)
	{
		SendTraceMsg("Received data length error: %d", recv_size);
		return FALSE;
	}

	if(tmp[0]!=EDMI_STX || tmp[recv_size-1]!=EDMI_ETX)
	{
		SendTraceMsg("Received data head or end byte error");
		return FALSE;
	}

	memcpy(&sourceAddr, &tmp[2], 4);
	memcpy(&destAddr, &tmp[6], 4);
	if(sourceAddr!=edmi_param->sourceAddr || destAddr!=edmi_param->destAddr[unit])
	{
		SendTraceMsg("Received addr error: source=0x%08x, dest=0x%08x", sourceAddr, destAddr);
		return FALSE;
	}

	if(tmp[11]!=sequence[unit])
	{
		SendTraceMsg("Received sequence error: %d", tmp[11]);
		return FALSE;
	}
	
	check_code = GetCrcCode(recv_size-3, tmp);
	
	if(tmp[recv_size-3] != (BYTE)(check_code>>8) || tmp[recv_size-2] != (BYTE)(check_code))
	{
		SendTraceMsg("Received data crc check error");
		return FALSE;
	}

	if(tmp[12]!=EDMI_ACK)
	{
		SendTraceMsg("Received respond is not ACK: %02x", tmp[12]);
		return FALSE;
	}
	return TRUE;
}

int SendSingleReadCmd(int port_fd, int unit, WORD addr, EDMI_PARAM_TABLE *edmi_param)
{
	BYTE	sendBuf[256], tmp[256];
	int		i, ret, send_size, recv_size;
	WORD	check_code;
	long	CRCCount;
	UINT	sourceAddr, destAddr;
	
	read(port_fd, tmp, sizeof(tmp));
	
	sequence[unit]++;
	
	sendBuf[0] = EDMI_STX;	
	sendBuf[1] = 'E';
	memcpy(&sendBuf[2], &edmi_param->destAddr[unit], 4);
	memcpy(&sendBuf[6], &edmi_param->sourceAddr, 4); //0x012b1668
	sendBuf[10] = 0x00;	
	sendBuf[11] = sequence[unit];
	
	send_size= 12;
	sendBuf[send_size++]= 'R';
	memcpy(&sendBuf[send_size], &addr, sizeof(addr));
	send_size += sizeof(addr);
	
	check_code= GetCrcCode(send_size, sendBuf);
	sendBuf[send_size++]= (BYTE)(check_code>>8);
	sendBuf[send_size++]= (BYTE)check_code;
	
	SendTraceMsg("data_num= %d bytes= %d", READ_DATA_NUM, send_size+1);
	
	send_size= AddDLEByte(send_size, sendBuf);
	sendBuf[send_size++]= EDMI_ETX;
	
	PrintfData(send_size, sendBuf, 0);
	
	ret=write(port_fd, sendBuf, send_size);
	if(ret!=send_size)
	{
		SendTraceMsg("Fail to write port: ret= %d", ret);
		return FALSE;
	}
	
	return TRUE;
}


int SendMultiReadCmd(int port_fd, int unit, EDMI_PARAM_TABLE *edmi_param)
{
	BYTE	sendBuf[256], tmp[256];
	int		i, ret, send_size, recv_size;
	WORD	check_code;
	long	CRCCount;
	UINT	sourceAddr, destAddr;
	
	read(port_fd, tmp, sizeof(tmp));
	
	sequence[unit]++;
	
	sendBuf[0] = EDMI_STX;	
	sendBuf[1] = 'E';
	memcpy(&sendBuf[2], &edmi_param->destAddr[unit], 4);
	memcpy(&sendBuf[6], &edmi_param->sourceAddr, 4); //0x012b1668
	sendBuf[10] = 0x00;	
	sendBuf[11] = sequence[unit];

	send_size= 12;
	sendBuf[send_size++]= 'M';
	sendBuf[send_size++]= 0;
	sendBuf[send_size++]= 0;
	sendBuf[send_size++]= 0xFF;
	sendBuf[send_size++]= 0xF0;

	for(i=0; i<READ_DATA_NUM; i++)
	{
		memcpy(&sendBuf[send_size], &regAddr[i], sizeof(regAddr[i]));
		send_size += sizeof(regAddr[i]);
	}
	
	check_code= GetCrcCode(send_size, sendBuf);
	sendBuf[send_size++]= (BYTE)(check_code>>8);
	sendBuf[send_size++]= (BYTE)check_code;

	SendTraceMsg("data_num= %d bytes= %d", READ_DATA_NUM, send_size+1);

	send_size= AddDLEByte(send_size, sendBuf);
	sendBuf[send_size++]= EDMI_ETX;
	
	PrintfData(send_size, sendBuf, 0);
	
	ret=write(port_fd, sendBuf, send_size);
	if(ret!=send_size)
	{
		SendTraceMsg("Fail to write port: ret= %d", ret);
		return FALSE;
	}

	return TRUE;
}

int GetSingleReadData(int port_fd, int unit, WORD addr, EDMI_PARAM_TABLE *edmi_param, float *fValue)
{
	int		ret, send_size, recv_size;
	WORD	check_code;
	WORD	recv_addr;
	long	CRCCount;
	UINT	sourceAddr, destAddr;
	BYTE	tmp[256];
	
	recv_size= read(port_fd, tmp, sizeof(tmp));
	if(recv_size<5)
	{
		SendTraceMsg("Fail to read port: recv_size= %d", recv_size);
		return FALSE;
	}
	
	recv_size= DelDLEByte(recv_size, tmp);
	
	PrintfData(recv_size, tmp, 1);
	
	if(recv_size!=22)
	{
		SendTraceMsg("Received data length error: %d", recv_size);
		return FALSE;
	}
	
	if(tmp[0]!=EDMI_STX || tmp[recv_size-1]!=EDMI_ETX)
	{
		SendTraceMsg("Received data head or end byte error");
		return FALSE;
	}
	
	memcpy(&sourceAddr, &tmp[2], 4);
	memcpy(&destAddr, &tmp[6], 4);
	if(sourceAddr!=edmi_param->sourceAddr || destAddr!=edmi_param->destAddr[unit])
	{
		SendTraceMsg("Received addr error: source=0x%08x, dest=0x%08x", sourceAddr, destAddr);
		return FALSE;
	}
	
	if(tmp[11]!=sequence[unit])
	{
		SendTraceMsg("Received sequence error: %d", tmp[11]);
		return FALSE;
	}
	
	check_code = GetCrcCode(recv_size-3, tmp);
	
	if(tmp[recv_size-3] != (BYTE)(check_code>>8) || tmp[recv_size-2] != (BYTE)(check_code))
	{
		SendTraceMsg("Received data crc check error");
		return FALSE;
	}
	
	if(tmp[12]!='R')
	{
		SendTraceMsg("Received respond is not 'M': %02x", tmp[12]);
		if(tmp[12]==EDMI_CAN)
			SendTraceMsg("Received respond error code: %d", tmp[13]);
		return FALSE;
	}
	
	memcpy(&recv_addr, &tmp[13], 2);
	if(recv_addr != addr)
	{
		SendTraceMsg("Received data addr error: recv 0x%02x need 0x%02x", recv_addr, addr);
		return FALSE;
	}
	memcpy(fValue, &tmp[15], 4);
	return TRUE;
}



int GetMultiReadData(int port_fd, int unit, EDMI_PARAM_TABLE *edmi_param, BYTE *buf)
{
	int		ret, send_size, recv_size;
	WORD	check_code;
	long	CRCCount;
	UINT	sourceAddr, destAddr;
	BYTE	tmp[1024];
	
	recv_size= read(port_fd, tmp, sizeof(tmp));
	if(recv_size<5)
	{
		SendTraceMsg("Fail to read port: recv_size= %d", recv_size);
		return FALSE;
	}
	
	recv_size= DelDLEByte(recv_size, tmp);

	PrintfData(recv_size, tmp, 1);
	
	if(recv_size<20)
	{
		SendTraceMsg("Received data length error: %d", recv_size);
		return FALSE;
	}

	if(tmp[0]!=EDMI_STX || tmp[recv_size-1]!=EDMI_ETX)
	{
		SendTraceMsg("Received data head or end byte error");
		return FALSE;
	}

	memcpy(&sourceAddr, &tmp[2], 4);
	memcpy(&destAddr, &tmp[6], 4);
	if(sourceAddr!=edmi_param->sourceAddr || destAddr!=edmi_param->destAddr[unit])
	{
		SendTraceMsg("Received addr error: source=0x%08x, dest=0x%08x", sourceAddr, destAddr);
		return FALSE;
	}

	if(tmp[11]!=sequence[unit])
	{
		SendTraceMsg("Received sequence error: %d", tmp[11]);
		return FALSE;
	}
	
	check_code = GetCrcCode(recv_size-3, tmp);
	
	if(tmp[recv_size-3] != (BYTE)(check_code>>8) || tmp[recv_size-2] != (BYTE)(check_code))
	{
		SendTraceMsg("Received data crc check error");
		return FALSE;
	}

	if(tmp[12]!='M')
	{
		SendTraceMsg("Received respond is not 'M': %02x", tmp[12]);
		if(tmp[12]==EDMI_CAN)
			SendTraceMsg("Received respond error code: %d", tmp[13]);
		return FALSE;
	}

	if(recv_size!=(20+READ_DATA_NUM*8))
	{
		SendTraceMsg("Received data length error: %d", recv_size);
		return FALSE;
	}
	
	memcpy(buf, &tmp[17], READ_DATA_NUM*8);
	return TRUE;
}



int edmi(int port_fd, COMM_TASK_INFO *pTaskInfo, COMM_PARAM_INFO *comm_param_info, EDMI_PARAM_TABLE	*edmi_param)
{
	int		i, unit;
	BYTE	TempBuf[READ_DATA_NUM*10];

	for(unit= 0; unit<comm_param_info->unit_num; unit++)
	{
		if(!LoginEDMI(port_fd, unit, edmi_param, comm_param_info))
		{
			SendTraceMsg("Fail to login edmi meter: 0x%08x", edmi_param->destAddr[unit]);
			continue;
		}

		SendTraceMsg("Success to login EDMI meter %d", unit+1);

		if(!SendMultiReadCmd(port_fd, unit, edmi_param))
		{
			SendTraceMsg("Fail to send read cmd to edmi meter: 0x%08x", edmi_param->destAddr[unit]);
			continue;
		}
		
		SendTraceMsg("Success to send read request to EDMI meter %d", unit+1);

		usleep(1000*3000); //2000ms

		if(!GetMultiReadData(port_fd, unit, edmi_param, TempBuf))
		{
			SendTraceMsg("Fail to get read respond from edmi meter: 0x%08x", edmi_param->destAddr[unit]);
			continue;
		}
		
		SendTraceMsg("Success to get read data from EDMI meter %d", unit+1);
		
		for(i=0; i<READ_DATA_NUM; i++)
		{
			double	dValue;
			int		iValue;
			
			memcpy(&dValue, &TempBuf[i*8], 8);
			SendTraceMsg("Success to get 0x%02x data %f from EDMI meter %d", regAddr[i], dValue, unit+1);
			iValue= (int)(dValue*100);
			memcpy(&pTaskInfo->data_buf[(i+unit*READ_DATA_NUM)*2], &iValue, 4); 			
		}
		
/*
		for(i=0; i<READ_DATA_NUM; i++)
		{
			float	fValue;

			if(!SendSingleReadCmd(port_fd, unit, regAddr[i], edmi_param))
			{
				SendTraceMsg("Fail to send read cmd to edmi meter: 0x%08x", edmi_param->destAddr[unit]);
				continue;
			}
			
			usleep(1000*1000); //2000ms
			
			if(!GetSingleReadData(port_fd, unit, regAddr[i], edmi_param, &fValue))
			{
				SendTraceMsg("Fail to get read respond from edmi meter: 0x%08x", edmi_param->destAddr[unit]);
				continue;
			}
			
			SendTraceMsg("Success to get 0x%02x data %f from EDMI meter %d", regAddr[i], fValue, unit+1);
			
			memcpy(&pTaskInfo->data_buf[(i+unit*READ_DATA_NUM)*2], &fValue, 4); 

		}
*/

		pTaskInfo->update_flag++;
		SendTraceMsg("update_flag= %d", pTaskInfo->update_flag);

		usleep(1000*3000); //3000ms
		
	}  //for(unit)
}


