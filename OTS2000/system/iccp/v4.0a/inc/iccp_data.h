/************************************************************************/
/* TASE.2 SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright BITC                                                 */
/*          2006-2009, All Rights Reserved                              */
/*									*/
/* MODULE NAME : iccp_data.h  						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : iccp shm def header file                                */
/*                                                                      */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 10/12/10  yyp     1      Created                                      */
/************************************************************************/
#ifndef _ICCP_DATA_INCLUDED
#define _ICCP_DATA_INCLUDED

#define MAX_DC_NUM		4
#define MAX_SLAVE_STN_NUM  	MAX_DC_NUM
#define	MAX_SP_NUM		20000
#define	MAX_DP_NUM		20000
#define	MAX_ME_NUM		5000
#define	MAX_CLI_DP_NUM		1000
#define	MAX_CLI_ME_NUM		500
#define	MAX_SC_NUM		512
#define	MAX_SE_NUM		256
#define	SEND_SOE_NUM		20


/*#define SHM_BUF_KEY_CHAR_ICCP	'7' 2010.3.15 zy */
#define SHM_BUF_KEY_CHAR_ICCP	'o' // modified by yyp because of conflict 20111009
#define	ICCP_BUF_FILE_KEY		"/home/ems/h9000/iccp_buf"	
#define SHM_DATA_KEY_CHAR_ICCP	SHM_BUF_KEY_CHAR_ICCP+2 
#define	ICCP_DATA_FILE_KEY		"/home/ems/h9000/iccp_data"	

struct dp_rec
{
	char name[38];
	unsigned char	stat;
	DATE_TIME	time;	
	unsigned char valid;
};

struct me_rec
{
	char name[38];
	float fvalue;
	unsigned char	stat;
	DATE_TIME	time;	
	unsigned char valid;
};

struct iccp_data_seg
{
	unsigned char stn_id;
	char stn_name[38];
	int dp_num;
	int me_num;
	struct  dp_rec  dp[MAX_DP_NUM];
	struct  me_rec  me[MAX_ME_NUM];
};


typedef struct shm_iccp_data
{
	struct  iccp_data_seg  iccp_data[MAX_SLAVE_STN_NUM];
}SHM_ICCP_DATA;


#define SHM_ICCP_DATA_SIZE sizeof(SHM_ICCP_DATA)

extern struct shm_iccp_data		data_iccp,*shm_iccp_data_addr;
int get_iccp_data_shm();


#endif