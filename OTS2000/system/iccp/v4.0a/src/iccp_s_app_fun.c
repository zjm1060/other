/************************************************************************
*  FILE NAME:           iccp_s_app_fun.c      				*
*  FUNCTION:            Controlled Station				*
*			Application Layer Function			*
*  's NAME : 	              						*
*  DATE:                Dec,2006                			*
*									*
* MODIFICATION LOG :							*
*  Date     Who   Rev                     Comments			*
* --------  ---  ------   ----------------------------------------------*
* 05/17/07  zy    3       Remote Client data and ds function added      *
* 03/28/07  zy    2       updated					*
* 12/06/06  zy    1       Created for SUN ICCP TASE.2 application	*
************************************************************************/

#include <time.h>
#include "../inc/mit_main.h"
#include "str_util.h"

#include "cppinc.h"

#include "../inc/iccp_main.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"
#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"

/*
#define	DEBUG
*/
//#define BY_RECORD		/* use shared memory record area to catch SOE */
#define CMD_TO_LCU

#define GW_DEVID	20

SHM_RECORD *shm_rec_addr;

extern	char	infobuf[1024];
extern	SHM_BUF *shm_buf_addr;

extern	int	soe_overflow_flag,soe_overflow_Sendflag;
extern	unsigned char	cmd_src;

extern	SOEBUF	soebuf;
short	rcv_end;

unsigned char	orgdpsoebuf[MAX_DP_NUM];

#ifdef REC_YF_AGC_CMD
	unsigned short	daysaved;
	char		cmdrecfile[80];
	FILE		*fagc;
#endif

extern		SPDEF		spdef;
extern		DPDEF		dpdef;
extern		MEDEF		medef;
extern		DPDEF		dpdef;
extern		MEDEF		medef;
extern		SEDEF		sedef;
extern		CMDDEF		scdef;

static	unsigned char	me_buf[1440];
static	SW_MSG		sw_buf[70];
static	short		me_num = 0;
static	short		sw_num = 0;
static	int		ana_all_flag = 0;

char    src_id;



