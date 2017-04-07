/* interlockgen.h file
*/

#include "../../../dms/v4.0a/inc/dms_com.h"

#define	INTERLOCK_NUM	10000 /*3000*/	/*1200*/
#define	OR_NUM		INTERLOCK_NUM*2
#define	AND_NUM		OR_NUM*6
#define	LAYER_NUM	20

#define	O_NAME_SIZE	60	/*36*/
#define	LONG_NAME_SIZE	100	/*72*/
/*#define NAME_SIZE	60*/	/*Replaced by POINT_NAME_SIZE*/
#define	DATA_NAME_LONG	16
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

#define	VERSTR	"INTLK V4.0A"
#define	VERLEN	32

typedef struct
{
	unsigned short	opr;
	short	layer;
	union
	{
		struct
		{
			short	g_num;
			short	offset;
		}group;
		unsigned int	status;
		float		value;
	}data;
	char	name[POINT_NAME_SIZE];
}I_Entry;

typedef struct
{
	short	layer;
	short	e_num;
	short	offset;
}I_Group;

typedef struct
{
	char		o_name[O_NAME_SIZE];
	short		g_num;
	short		offset;
	char		long_name[LONG_NAME_SIZE];
}InterLock;

int	InterLockFileLoad();
int	InterLock_Judge(char *);
void	InterLockFileFree();
