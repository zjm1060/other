


#include 	"../../../dms/v4.0a/inc/dms_com.h"
#include 	"../inc/dps_rec_def.h"
#include	"../inc/dps_type_def.h"
#include	"../inc/ipc_common.h"

#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(lib, "../../../lan/v4.0a/lib/lan.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")



void SwapRecEntryByte(REC_ENTRY_UNION *rec_entry)
{

	switch(rec_entry->ana_rec.type_id)
	{
	case	DPS_ANA_ALM:
	case	DPS_IMP_ALM:
		Swap4Byte(&rec_entry->ana_rec.rec_sn);
		Swap2Byte(&rec_entry->ana_rec.pt_id);
		Swap2Byte(&rec_entry->ana_rec.year);
		Swap2Byte(&rec_entry->ana_rec.status);
		Swap4Byte(&rec_entry->ana_rec.data);
		Swap2Byte(&rec_entry->ana_rec.iost);
		break;

	case	DPS_IND_ALM:
		Swap4Byte(&rec_entry->ind_rec.rec_sn);
		Swap2Byte(&rec_entry->ind_rec.pt_id);
		Swap2Byte(&rec_entry->ind_rec.year);
		Swap2Byte(&rec_entry->ind_rec.ind_status);
		Swap2Byte(&rec_entry->ind_rec.ms);
		Swap2Byte(&rec_entry->ind_rec.iost);
		break;

	case	DPS_ANA_PARAM_SET:
	case	DPS_IMP_PARAM_SET:
	case	DPS_C_ONOFF_SET:
		Swap4Byte(&rec_entry->param_set_rec.rec_sn);
		Swap2Byte(&rec_entry->param_set_rec.pt_id);
		Swap2Byte(&rec_entry->param_set_rec.year);
		Swap2Byte(&rec_entry->param_set_rec.cmd_status);
		Swap4Byte(&rec_entry->param_set_rec.data_new);
		Swap4Byte(&rec_entry->param_set_rec.data_old);
		break;

	case	DPS_LMS:
		Swap4Byte(&rec_entry->lms_rec.rec_sn);
		Swap2Byte(&rec_entry->lms_rec.msg_ptr);
		Swap2Byte(&rec_entry->lms_rec.year);
		break;

	case	DPS_SMS:
		Swap4Byte(&rec_entry->sms_rec.rec_sn);
		Swap4Byte(&rec_entry->sms_rec.time);
		break;

	default:
		Swap4Byte(&rec_entry->cmd_rec.rec_sn);
		Swap2Byte(&rec_entry->cmd_rec.pt_id);
		Swap2Byte(&rec_entry->cmd_rec.year);
		Swap2Byte(&rec_entry->cmd_rec.cmd_status);
		Swap4Byte(&rec_entry->cmd_rec.data);
		Swap2Byte(&rec_entry->cmd_rec.ctrl_addr);
		break;
	}
}



void main(void)
{
	SHM_RECORD	*shm_rec_addr;
	int		head_ptr, end_ptr, count, pos_ptr;
	REC_ENTRY_UNION	record;
	int		i, j, byte_num;
	char		input;
	UCHAR		output[sizeof(REC_ENTRY_UNION)];
	int num;
	SetModuleName("ReadShm");

	/*      打开记录区共享内存      */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		Err_log("Fail to link record shm");
		return;
	}

	num = sizeof(SMS_REC);

START:
	printf("\n**********************************************");
	head_ptr= shm_rec_addr->head.header_ptr;
	count= shm_rec_addr->head.count;

	if(head_ptr>=count)
		end_ptr= head_ptr-count;
	else
		end_ptr= (head_ptr+shm_rec_addr->head.buf_size)-count;
		
	printf("\n buf_size= %d  header_ptr= %d end_ptr= %d count=%d ", shm_rec_addr->head.buf_size,
		head_ptr, end_ptr, count);

	for(i=0; i<count; i++)
	{
		pos_ptr= (end_ptr+i)%shm_rec_addr->head.buf_size;
		record= shm_rec_addr->rec[pos_ptr];
		SwapRecEntryByte(&record);
			
		switch(record.ana_rec.type_id)
		{
		case	DPS_ANA_ALM:
		case	DPS_IMP_ALM:
			byte_num= sizeof(ANLG_REC);
			printf("\n%04dANA: ", pos_ptr);
			break;

		case	DPS_IND_ALM:
			byte_num= sizeof(IND_REC);
			printf("\n%04dIND: ", pos_ptr);
			break;

		case	DPS_ANA_PARAM_SET:
		case	DPS_IMP_PARAM_SET:
		case	DPS_C_ONOFF_SET:
			byte_num= sizeof(PARAM_SET_REC);
			printf("\n%04dSET: ", pos_ptr);
			break;

		case	DPS_LMS:
			byte_num= sizeof(LMS_REC);
			printf("\n%04dLMS: ", pos_ptr);
			break;

		case	DPS_SMS:
			byte_num= sizeof(LMS_REC);
			printf("\n%04dSMS: ", pos_ptr);
			break;

		default:
			byte_num= sizeof(CMD_REC);
			printf("\n%04dCMD: ", pos_ptr);
			break;
		}
		
		memcpy(&output, &record, byte_num);
		for(j=0; j<byte_num; j++)
			printf("%02x ", output[j]);
		printf("\nnum= %d", byte_num);
		
	}
	
	printf("\ninput \'q\' to exit, or continue");
	input= getchar();
	if(input!='q' && input!='Q')
		goto START;	

}


