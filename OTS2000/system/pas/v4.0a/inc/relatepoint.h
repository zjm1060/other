#ifndef    _RELATE_POINT_H
#define    _RELATE_POINT_H

#include "../../../dms/v4.0a/inc/dms_com.h"

#define	RELATEPOINT_NUM	60000 /*3000*/	/*1200*/
#define CHILDPOINT_NUM 64
#define	O_NAME_SIZE	60	/*36*/
#define	LONG_NAME_SIZE	100	/*72*/
/*#define NAME_SIZE	60*/	/*Replaced by POINT_NAME_SIZE*/
#ifndef DATA_NAME_LONG
#define	DATA_NAME_LONG	64
#endif 
/*
#define	INTERLOCK_FILE_DEF	"InterLockFile.def"
*/
#define OPEN		0
#define CLOSE		1

#define	LINE_BUF_CHAR	256

#define	TAB		9
#define CR		0xd
#define LF		0xa
/*
#ifdef WINDOWS_OS
#define LF		0xd
#else
#define LF		0xa
#endif
*/
#define STAT_1		4
#define STAT_0		0

/* Define Entry Logic operator type for each entry */
#define	BIN_LG		0
#define	LESS		1
#define	LESS_E		2
#define	LARGE		3
#define	LARGE_E		4
#define	EQUAL		5
#define	N_EQUAL		6
#define	CHANFAIL	16

/* Define Bracket Flag for each entry */
#define	BRACKET_F	(unsigned short)0xFFFF

/* Define Bracket Flag for each entry */
#define	NOT_MEET	0
#define	MEET		1
#define	NOT_MEET_FAIL	2
#define	MEETWITH_FAIL	3

#define	RPVERSTR	"RELATEPT V4.0A"
#define	VERLEN	32

typedef struct
{
	char		o_name[O_NAME_SIZE];
	short		child_num;
	POINTER childpt[CHILDPOINT_NUM];
	char childname[CHILDPOINT_NUM][DATA_NAME_LONG];

}RelatePoint;

int	RelatePointFileLoad();
int	RelatePoint_Judge(char *);
void	RelatePointFileFree();
#endif  /*_RELATE_POINT_H*/