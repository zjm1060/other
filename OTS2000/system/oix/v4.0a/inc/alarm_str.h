// 
// File:   alarm_str.h
// Author: ems
//
// Created on 2006年10月16日, 下午3:49
// SYN 2007.12.10
//

#ifndef _alarm_str_H
#define	_alarm_str_H
#include    "../../../dms/v4.0a/inc/dms_com.h"

#define	SN_STR_LEN				12
#define	TIME_STR_LEN			64
#define	REPEAT_ALM_STR_LEN		12
#define	STATE_STR_LEN			128
#define	MAX_MESSAGE_SIZE		256
#define	MAX_CMD_SRC_NAME_SIZE	64
#define	MAX_SEARCH_NAME_SIZE	64
#define	MAX_CMD_RESULT_SIZE		64


typedef struct
{
	char	sn_str[SN_STR_LEN];
	char	time_str[TIME_STR_LEN];
	char	repeatAlm[REPEAT_ALM_STR_LEN];
	char	logicname[POINT_NAME_SIZE];
	char	longname[POINT_LONGNAME_SIZE];
	char	state_str[STATE_STR_LEN];
	char	cmd_src[MAX_CMD_SRC_NAME_SIZE];
	char	cmd_result[MAX_CMD_RESULT_SIZE];
	char	hostname[MAX_HOST_NAME_SIZE];
	char	usrname[USR_NAME_SIZE];
	char	message[MAX_MESSAGE_SIZE];
} ALM_STR;

#endif	/* _alarm_str_H */

