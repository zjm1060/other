 /*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2006.4.8 
     

---------------------------------------------------------------------*/

#ifndef   _PRIVILEGE_DEF_H 
#define   _PRIVILEGE_DEF_H

 
#include "../../../dms/v4.0a/inc/dms_com.h"



enum USER_PRIVILEGE_TYPE 
{
	PRIV_ALM_ACK=0,
	PRIV_PRINT,
	PRIV_LCU_CTRL,
	PRIV_POWER_ADJ,
	PRIV_SYS_RUN_SET,
	PRIV_CTRL_BLOCK_SET,
	PRIV_IN_FORCE_SET,
	PRIV_OUT_FORCE_SET,
	PRIV_SYS_CONF_MGR,
	PRIV_TAG_OPR,
	PRIV_SCAN_ALM_SET,
	PRIV_IO_ATTR_SET,
	PRIV_SGZY_SET,
	PRIV_RTVALUE_MAN_SET,
	PRIV_IMP_MAN_SET,
	PRIV_LIMIT_PARA_SET,
	PRIV_SYS_MAINT,
	PRIV_LCU_SELECT_SET,
	PRIV_MAX_TYPE_NUM
};



#define	MAX_USER_NAME_LEN		8
#define	MAX_USER_PASSWD_LEN		16
#define MAX_USER_NUM			100 
#define MAX_PRIV_TYPE_NUM		PRIV_MAX_TYPE_NUM
#define MAX_CTRL_LCU_NUM		128




typedef struct
{
	CHAR	username[MAX_USER_NAME_LEN];
	CHAR	passwd[MAX_USER_PASSWD_LEN];
	UCHAR	privilege[(MAX_PRIV_TYPE_NUM+7)/8];
} USER_DEF;


typedef struct
{
	UCHAR	stn_id;
	UCHAR	dev_id;
} LCU_ID;


typedef struct
{
	UCHAR	lcu_num;
	CHAR	hostname[MAX_HOST_NAME_SIZE];
	LCU_ID	lcu[MAX_CTRL_LCU_NUM];
} HOST_DEF;


typedef struct
{
	int			check_sum;
	UCHAR		user_num;
	UCHAR		host_num;
	USER_DEF	user_list[MAX_USER_NUM];
	HOST_DEF	host_list[MAX_HOST_NUM];
} PRIVILEGE_DB;

typedef struct 
{
	UCHAR start_id ;
	UCHAR end_id ; 
	
}STN_LCU_POS  ; 

typedef struct
{
	LCU_ID	lcu_id;
	char	comment[GROUP_COMMENT_SIZE];
} LCU_COMMENT;


extern PRIVILEGE_DB		privilege_db;
int	LoadUserPrivilegeDatabase(void);
int	SaveUserPrivilegeDatabase(void);
BOOL CheckUserPrivilege(char *userName, enum USER_PRIVILEGE_TYPE priv_type);
BOOL CheckHostPrivilege(char *hostName, LCU_ID lcu_id);
BOOL CheckUserLogin(char *userName, char *passwd);
BOOL CheckUserExist(char *userName);
int UpdateUserPasswd(char *userName, char *passwd, char *new_passwd);




#endif /*_PRIVILEGE_DEF_H*/
