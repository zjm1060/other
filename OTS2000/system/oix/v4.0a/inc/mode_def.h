/***********************************************
   SYN by hcl 2007.12.12
**********************************************/
#define	DISPLAY_EDIT	0
#define	SYMBOL_EDIT	1
#define	SYMBOL_GRP_EDIT	2
#define	COLOR_GRP_EDIT	3
#define	STRING_GRP_EDIT	4

#define	MENU_EDIT	5
#define	SOFT_KEY_EDIT	6

#define	DSP_FILE	0
#define	GIF_FILE	1
#define	SFTCP_FILE	2
#define	SGZY_HIST_FILE    3
#define TREND_HIST_FILE    4
#define	FAIL_FILE	5
#define	ALM_HIST_FILE	6
#define	RPT_HIST_FILE	7
#define LOG_HIST_FILE	8  /* added by for v4.0a gtk*/
#define DEL_SGZY_FILE	9
#define DEL_SFTCP_FILE  10
#define SUB_DSP_FILE    11

#define		TO_RBIN		0
#define		TO_DBIN		1

int	file_mode[MAX_CRT_NUMBER];
int	edit_mode;
