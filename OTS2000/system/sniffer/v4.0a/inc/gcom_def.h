#ifndef _GCOM_HEADER_H
#define _GCOM_HEADER_H



typedef struct 
{
	char		dest_addr[6];
	char		src_addr[6];
	USHORT		length;
	char		dsap;
	char		ssap;
	char		u;
} GCOM_DL_HEADER;





enum MESSAGE_TYPE  {GCOM_DATA=0, GCOM_ACK= 1, GCOM_INIT_R, GCOM_INIT_S};
enum PRIO_TYPE  {LOW_PRIO=2, NORM_PRIO, HIGH_PRIO, EMERG_PRIO};
enum DEST_CHAN_ID {CHAN_ID_PROC_EVENT=16, CHAN_ID_SYSTEM_EVENT=46};
enum SIGNAL_ID	{SIGNAL_ID_PROC_EVENT= 18};


typedef struct
{
	char type:4;
	char prio:4;

} GCOM_TYPE_PRIO;


typedef struct 
{
	GCOM_TYPE_PRIO type_prio;
	char node;
	char seq;
} GCOM_DATA_HEADER;



typedef struct
{
	char	dest_netw;
	char	dest_node;
	USHORT	dest_ch_id;
	char	src_netw;
	char	src_node;
	USHORT	resp_ch_id;
	char	priority;
	char	m_code;
	USHORT	size;
	char	id;
	char	dual;
} GCOM_DATA_ADDR;


typedef struct
{
	USHORT	symname_length;
	char	symname[20];
	USHORT	descr_length;
	char	descr[28];
	char	unkown1[10];
	UCHAR	state;
	char	unkown4[2];
	UCHAR	alm_state;
	char	unkown5[4];
	char	date[4];
	char	time[4];
	char	unkown2[16];
	USHORT	value_TR;
	char	unkown3[4];
} GCOM_EVENT_DATA;




#endif
