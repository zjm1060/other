/* calcugen.h file
*/

/*
#include "../../../dms/v4.0a/inc/dms_com.h"
*/
#define	CALCU_NUM	4000
#define	OR_NUM		CALCU_NUM*2
#define	AND_NUM		OR_NUM*6
#define	LAYER_NUM	20
/*
#define	O_NAME_SIZE	36
#define NAME_SIZE	36

#define	CALCU_FILE_DEF	"CalcuFile.def"
#define	CALCU1_FILE_DEF	"CalcuFile1.def"
*/

#define OPEN		0
#define CLOSE		1

#define	LINE_BUF_CHAR	256
#define	DATA_NAME_LONG	16

#define	TAB		9
#define CR		0xd
#define LF		0xa
/*
#ifdef	WINDOWS_OS
#define LF		0xd
#else
#define LF		0xa
#endif
*/
#define	BitS_STATUS	2
#define	BitN_STATUS	4
#define	BitS_ANA_POINT	5
#define	BitN_ANA_POINT	2
#define	BitS_ANA_SIGN	7

#define STAT_1		4
#define STAT_0		0

/* Define calcu type for each object*/
#define	M_LOGIC		0
#define	LOGIC		0x00ff
#define	CALCU		0x0fff
#define DIFF		0x0ff0
#define D_CLEAR		0x0ff1
#define M_CLEAR		0x0ff2
#define Y_CLEAR		0x0ff3

/* Define Entry Calcu operator type for each entry */
#define	RESET		0
#define	ADD		0x0100
#define	MIN		0x0200
#define	MUL		0x0300
#define	DIV		0x0400
#define SQRT		0x0500

#define MASK_C		0x0f00
#define	RESET_C		0x1000
#define	ADD_C		0x1100
#define	MIN_C		0x1200
#define	MUL_C		0x1300
#define	DIV_C		0x1400

#define DIFF_S		0
#define DIFF_C		0x1000

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
#define	NOT_MEET		0
#define	MEET			1
#define	NOT_MEET_FAIL	2
#define	MEETWITH_FAIL	3

#define	VERSTR	"CALCU V4.0A"
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
		float	value;
	}data;
	char	name[POINT_NAME_SIZE];
}C_Entry;

typedef struct
{
	short	layer;
	short	e_num;
	short	offset;
}C_Group;

typedef struct
{
	short	layer;
	short	g_num;
	short	offset;
}C_Bracket;

typedef struct
{
	char	o_name[POINT_NAME_SIZE];
	unsigned short	type;
	short	g_num;
	short	offset;
}Calcu;

typedef struct 
{
	unsigned char	type_id;
	unsigned char 	stn_id;
	unsigned char 	dev_id;
	char    data_type;
	short	number;
	short	point_id;
	IND_DATA   status;
	short   ms;	  
	unsigned char 	host_id;	
	char    sec;
	char    min;
	char    hour;
	char    day;
	char    month;
	short   year;
} SW_MSG;
