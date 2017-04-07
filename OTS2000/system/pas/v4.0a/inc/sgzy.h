/*	sgzy.h head file for sgzy */

#define SGZY_NUM		80	/*40*/
#define	SGH_NUM			40
#define MAX_SGZY_COUNT		850
#define SG_NAME_LONG		60
#define SG_FILENAME_LONG	80
#define CYCLE			1

#define	WDQS_NUM		900	/*300*/
#define	MAX_WDQS_COUNT		80
#define	WD_CYCLE		1 /*cycle should be: 1-6 or 10 */

typedef struct
{
	POINTER		buffer[MAX_SGZY_COUNT];
	SHORT		next_pos;
}SGZY_DY;

typedef struct
{
	POINTER		ptr;
	SHORT		status;
	DATE_TIME	time;
}INT_ALRM;

typedef struct
{
	SHORT		sg_flg;
	INT_ALRM	alarm;
}SGHD;

typedef struct
{
	SGHD		sghd;
	SHORT		nextpt;
	SHORT		flg;
	double		dCurTime;		//当前时间(秒)
	union
	{
		float		fvalue[MAX_SGZY_COUNT][SGZY_NUM];
		int		status[MAX_SGZY_COUNT][SGZY_NUM];
	} sg_dat;
}SGZY_SHM_DATA;

typedef struct
{
	SHORT		zynum;
	CHAR		sgname[POINT_LONGNAME_SIZE];
	CHAR		longname[MAX_SGZY_COUNT][POINT_LONGNAME_SIZE];
	CHAR		data_type[MAX_SGZY_COUNT];	/*v4.0 added*/
	float		hi_alm[MAX_SGZY_COUNT];		/*v3.0 modified*/
	float		lo_alm[MAX_SGZY_COUNT];		/*v3.0 modified*/
	POINTER		pointer[MAX_SGZY_COUNT];	/*zlb 2011-9-8*/
}ZYHD;

typedef struct
{
	INT_ALRM	alarm;
	ZYHD		zyhd;
	DATE_TIME	dateTime;
	union
	{
		float		fvalue[MAX_SGZY_COUNT][SGZY_NUM];
		int		status[MAX_SGZY_COUNT][SGZY_NUM];
	} sg_dat;
}SGZY_DATA;

/* Following structs are defined for temperature treading!*/

typedef	struct
{
	POINTER		buffer[MAX_WDQS_COUNT];
	SHORT		next_pos;
}WDQS_DY;

typedef	struct
{
	SHORT		nextpt;
	SHORT		flg;
	SHORT		save_flg;
	double		dCurTime;		//当前时间(秒)
	union
	{
		float		fvalue[MAX_WDQS_COUNT][WDQS_NUM];
		int		status[MAX_WDQS_COUNT][WDQS_NUM];
	} wd_dat;
}WDQS_SHM_DATA;

typedef	struct
{
	DATE_TIME	time;
	SHORT		zynum;
	CHAR		longname[MAX_WDQS_COUNT][POINT_LONGNAME_SIZE];
	CHAR		data_type[MAX_WDQS_COUNT];	/*v4.0 added*/
	float		hi_alm[MAX_WDQS_COUNT];		/*v3.0 modified*/
	float		lo_alm[MAX_WDQS_COUNT];		/*v3.0 modified*/
	POINTER		pointer[MAX_WDQS_COUNT];	/*zlb 2011-9-8*/
}WDHD;

typedef	struct
{
	WDHD		wdhd;
	union
	{
		float		fvalue[MAX_WDQS_COUNT][WDQS_NUM];
		int		status[MAX_WDQS_COUNT][WDQS_NUM];
	} wd_dat;
}WDQS_DATA;

