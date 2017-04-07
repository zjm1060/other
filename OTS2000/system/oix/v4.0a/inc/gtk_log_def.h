/*****
created by zyp on 2005.11 for v4.0a gtk 
SYN by hcl 2007.12.12 
****/

#ifndef _LOG_DEF_H_
#define _LOG_DEF_H_

#define LOG_REC_REAL 0
#define LOG_REC_HISTRY 1

#if defined(WINDOWS_OS)
#define TrapPrintFile   "\\home\\ems\\h9000\\tmp\\TrapPrintFile.tmp"
#define	TRAP_SEARCH_TMP_FILE	"\\home\\ems\\h9000\\tmp\\TrapSearchFile.tmp"
#define TRAP_FILE_PATH		"\\home\\ems\\h9000\\his\\trap\\"
#endif

#ifdef SUN_OS
#define TrapPrintFile   "/home/ems/h9000/tmp/TrapPrintFile.tmp"
#define	TRAP_SEARCH_TMP_FILE	"/home/ems/h9000/tmp/TrapSearchFile.tmp"
#define TRAP_FILE_PATH		"/home/ems/h9000/his/trap/"

#endif

LOG_INFO	log_rec;	

typedef struct
{
	char   no[10];
	char errtype[2+10];
	char errcode[2+10];
	char datetime[256];
	char appid[10+10];
	char info[MAX_LOG_INFO_SIZE];
}TRAPLOG;
TRAPLOG hist_log_rec[MAX_LOG_NUM],real_log_rec[MAX_LOG_NUM];

struct
{
    int prog;
    char prog_name[20];
    int type;
    char type_name[5];
	int str;
	char str_name[20];
}LOG_FILTER;

int	log_rec_mode[MAX_CRT_NUMBER];
static short log_rec_cnt[MAX_CRT_NUMBER];


GtkWidget *loglist_scrolled_window,*log_shell[MAX_CRT_NUMBER];
GtkWidget *loglist;


int log_cur_page_startid,log_cur_page_endid;

#define LOG_PAGE_SIZE		30
#define WgtN_log_page_down 	0
#define WgtN_log_page_up	1
#define TRAP_LOG_X0             10
#define TRAP_LOG_Y0             20
#define TRAP_LOG_ROW_HEIGHT     30
#define TRAP_LOG_COL_WIDTH1      60
#define TRAP_LOG_X4		500


#endif

