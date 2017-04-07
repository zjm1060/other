
#ifndef _EDMI_DEF_H
#define _EDMI_DEF_H



/*红相表有关通讯参数*/

#define MAX_EDMI_METER_NUM	4
#define MAX_EDMI_USER_NAME_SIZE	12
#define MAX_EDMI_PASSWD_SIZE	12





#define EDMI_STX	0x02
#define EDMI_ETX	0x03
#define EDMI_XON	0x11
#define EDMI_XOFF	0x13
#define EDMI_DLE	0x10
#define EDMI_ACK	0x06
#define EDMI_CAN	0x18


typedef struct _EDMI_PARAM_TABLE
{
	UINT	sourceAddr;
	UINT	destAddr[MAX_EDMI_METER_NUM];
	char	user[MAX_EDMI_USER_NAME_SIZE];
	char	passwd[MAX_EDMI_PASSWD_SIZE];
} EDMI_PARAM_TABLE;




#endif //_EDMI_DEF_H








