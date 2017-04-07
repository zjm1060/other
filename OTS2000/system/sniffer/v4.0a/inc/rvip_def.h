#ifndef	_RVIP_HEADER_H
#define	_RVIP_HEADER_H



#define MAX_VIP_GROUP_NUM	10
#define	MAX_VIP_AI_NUM_IN_GROUP	23

enum VIP_DATA_TYPE {AI_TYPE=1, DI_TYPE};

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
} VIP_M;


#endif