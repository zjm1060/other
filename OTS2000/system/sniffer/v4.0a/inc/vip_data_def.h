#ifndef	_VIP_DATA_HEADER_H
#define	_VIP_DATA_HEADER_H


#define PT_NUM_PER_DS		384		
#define DS_NUM_PER_PACKET	10
#define MAX_VIP_PACKET_NUM		32

#define MAX_VIP_GROUP_NUM	10
#define	MAX_VIP_AI_NUM_IN_GROUP	23

enum VIP_DATA_TYPE {VIP_AI_TYPE=1, VIP_DI_TYPE, VIP_COUNT_TYPE};

typedef struct {
	unsigned short size;
	unsigned short ch_id;
	unsigned short pack_id;
	unsigned short no_group;
	unsigned short v_flag;
	unsigned short s_tick;
	unsigned short reservered;
	enum VIP_DATA_TYPE type;
	unsigned int   quality[120];
	unsigned int   status[120];
} VIP_DATA_M;


typedef struct  
{
	UCHAR	pack_id;
	enum VIP_DATA_TYPE	type;
} DS_CONFIG;



#endif
