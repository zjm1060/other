#ifndef	_DPS_FAULT_TABLE_DEF
#define _DPS_FAULT_TABLE_DEF



#include	"../inc/dps_rec_def.h"




#define		MAX_FAULT_RECORD_NUM	6000


typedef struct
{
	REC_HEAD			head;
	REC_ENTRY_UNION		rec[MAX_FAULT_RECORD_NUM];
} FAULT_RECORD_TABLE;




#endif /*_DPS_FAULT_TABLE_DEF*/

