#ifndef	_VIP_CMD_HEADER_H
#define	_VIP_CMD_HEADER_H



//***DEFINITIONS FOR VIP COMMUNICATION***************************************

//Record for one VIP-R Block

#define IL_DATA_NUM	10
#define R_DATA_NUM	30

//2个短整型，10个长整型，30个浮点数
typedef struct  
{
  USHORT		len;
  USHORT		msg_id;
  USHORT		yk_flag; //指示哪个长整型数的哪一位有效(遥控命令)
  USHORT		yt_flag; //指示哪个浮点数有效(遥调命令)
  UINT			IL_VALUE[IL_DATA_NUM];	//遥控命令: 
  float			R_VALUE[R_DATA_NUM];	//遥调命令: 全厂有功给定、全厂无功给定、1F有功给定、1F无功给定...
}T_VIP_R;


typedef struct 
{
	USHORT	port;
	USHORT	msg_id;
	char	ipaddr[48];
}VIP_CONFIG;

#endif
