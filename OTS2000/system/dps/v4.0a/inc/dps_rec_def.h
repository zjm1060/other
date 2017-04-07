#ifndef     _DPS_REC_DEF_H
#define     _DPS_REC_DEF_H


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"


#define	SMS_MESSAGE_SIZE	26
#define	MAX_USR_NAME_SIZE	8

typedef	struct
{
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_num;
	USHORT			pt_id;
	IND_DATA		ind_status;
	USHORT			ms;
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
}SP_IND_ALM_PKG;

typedef	struct
{
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_num;
	USHORT			pt_id;
	IND_DATA		ind_status;
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
}SP_IND_ALLSEND_PKG;



typedef	struct
{
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_num;
	USHORT			pt_id;
	ANLG_STATUS		ana_status;
	char			data[4];
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
}SP_ANA_INFO_PKG;


typedef	struct
{
	UCHAR		type_id;
	UCHAR		stn_id;
	UCHAR		dev_id;
	UCHAR		data_type;
	USHORT		point_id;
	CMD_STATUS	status;
	union
	{
		int	iValue;
		FLOAT	fValue;
	}data;
	USHORT		ctrl_addr;
	UCHAR		host_id;
	UCHAR		usr_id;
	UCHAR		res;
	UCHAR		second;
	UCHAR		minute;
	UCHAR		hour;
	UCHAR		day;
	UCHAR		month;
	USHORT		year;
	char		usr_login_name[8];
} COMMAND_MSG;

typedef struct
{
    USHORT		buf_size;
    USHORT		header_ptr;
    USHORT		end_ptr;
    USHORT		count;
}REC_HEAD;



typedef	struct
{
	GULONG			rec_sn;
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_id;
	union
	{
		ANLG_STATUS		ana_status;
		IMP_STATUS		imp_status;
	} status;
	GLONG			data;
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
	IOST			iost;
	UCHAR			proc_id;
}ANLG_REC;


typedef	struct
{
	GULONG			rec_sn;
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_id;
	IND_DATA		ind_status;
	USHORT			ms;
	IOST			iost;
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
	UCHAR			proc_id;
}IND_REC;



typedef	struct
{
	GULONG			rec_sn;
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_id;
	COMMAND_STATUS	cmd_status;
	union
	{
		GLONG	iValue;
		FLOAT	fValue;
	} data;
	USHORT			ctrl_addr;
	UCHAR			host_id;
	UCHAR			usr_id;
	UCHAR			res;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
	CHAR			usr_name[USR_NAME_SIZE];
}CMD_REC;


typedef	struct
{
	GULONG			rec_sn;
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_id;
	COMMAND_STATUS	cmd_status;
	union
	{
		GLONG	iValue;
		FLOAT	fValue;
	} data_new;
	union
	{
		GLONG	iValue;
		FLOAT	fValue;
	} data_old;
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
	CHAR			usr_name[USR_NAME_SIZE];
}PARAM_SET_REC;


typedef struct
{
	GULONG			rec_sn;
	UCHAR			type_id;
	UCHAR			stn_id;
	USHORT			msg_ptr;
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
}LMS_REC;



typedef struct
{
	GULONG			rec_sn;
	UCHAR			type_id;
	UCHAR			host_id;
	char			message[SMS_MESSAGE_SIZE];
	time_t			time;
}SMS_REC;



typedef	struct
{
	GULONG			rec_sn;
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_id;
	FB_STATUS1		fb_status1;
	FB_STATUS2		fb_status2;
	USHORT			param;
	USHORT			ms;
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
	IOST			iost;
	UCHAR			proc_id;//added by wzg 20101219
}OBJ_REC;



typedef union
{
	IND_REC			ind_rec;
	ANLG_REC		ana_rec;
	CMD_REC			cmd_rec;
	PARAM_SET_REC	param_set_rec;
	LMS_REC			lms_rec;
	SMS_REC			sms_rec;
	OBJ_REC			obj_rec;
}REC_ENTRY_UNION;



#define MAX_SHM_RECORD_NUM		12000
#define MAX_FILE_RECORD_NUM		6000
#define MAX_HDB_RECORD_NUM		6000
#define MAX_INFO_RECORD_NUM		6000


typedef struct
{
	REC_HEAD		head;
	REC_ENTRY_UNION		rec[MAX_SHM_RECORD_NUM];
}SHM_RECORD;



typedef struct
{
	REC_HEAD		head;
	REC_ENTRY_UNION		rec[MAX_FILE_RECORD_NUM];
}HISTORY_RECORD;


typedef struct
{
	REC_HEAD		head;
	REC_ENTRY_UNION		rec[MAX_HDB_RECORD_NUM];
}HDB_HISTORY_RECORD;


typedef struct
{
	REC_HEAD		head;
	REC_ENTRY_UNION		rec[MAX_INFO_RECORD_NUM];
}INFO_RECORD;


#define REC_HEAD_SIZE		sizeof(REC_HEAD)
#define REC_ENTRY_SIZE		sizeof(REC_ENTRY_UNION)
#define SHM_REC_SIZE		sizeof(SHM_RECORD)

#endif /*_DPS_REC_DEF_H*/

