/***************************************************************************************

PROGRAM NAME:	hdb_data.h

FUNCTION:		data structure definition of history database interface

VERSION:		v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.11.22	create the file for H9000 V4.0		yyp

***************************************************************************************/

#ifndef _HDB_DATA_HEAD
#define _HDB_DATA_HEAD

#define ONE_SEC	0					
#define ONE_MIN	1
#define FIVE_MIN	2
#define FIFTEEN_MIN	3
#define ONE_HOUR	4
#define ONE_DAY		5
#define IMP_HOUR	6
#define MAX_SELECT_SEG	6					
#define LOGICALNAME_LEN	38
#define TIMETAG_LEN	22					
#define	MAX_SELECT_REC	1441
#define	MAX_REVERT_REC	300					
#define	MAX_SELECT_TIME	10
#define	MAX_CRT_NUM	3
#define	MIN_NEG_NUM	-32767
#define	MIN_OFF		0.001
#define	MAX_POINT_NUM	1500
#define	MAXLOOPCOUNT	1800
#define MAX_ONE_PKGBUF	1000				
#define	MAXPKGBUF	100
#define	MAX_YC_NUM	20000
#define	MAX_YX_NUM	60000
#define MAX_PT_NUM	100000
#define	MAX_CHARA_NUM	2000
#define	MAX_STAT_NUM	3000
#define	MAX_TREND_NUM	1000
#define	MAX_JH_NUM	2
#define MAX_TYPE_NUM	15
#define MAX_HDB_SCENE	1000
#define MAX_PT_NUM_DEV	5000
#define	MAX_AL_NUM	MAX_HDB_RECORD_NUM
#define	MAX_SMS_NUM	100	
#define	MAX_REVERT_NUM	500
#define MAX_IN_ROWS MAX_YC_NUM
#define MAX_DEV_NUM_HDB 50 
#define BUFSIZE		200
#define YES		1 
#define NO		0
#define MAX_HDB_STACK_SIZE	(1024000 * 50)		/* bytes */
#define OUTNET_HOST	"RBWEBDBS"
#define CONST_NEG	-32767
#define CONST_NEG_STR "-32767"


struct lcu_pt_conf
{
	unsigned char stnid;
	unsigned char devid;
	unsigned short	point_num;
	BOOL is_curve_point[MAX_POINT_NUM];
};

struct curve_sec_seg
{
	POINTER point;
	char logicalname[55];
	float	offset;
	IOATTR ioattr;
	float value[60];
	short status[60];
	short isvalid[60];
	short ischg[60];
	short chgflag;
};

typedef struct curve_sec
{
	unsigned char stnid;
	unsigned char devid;
	unsigned short num;
	char timestr[30];
	struct curve_sec_seg point_data[MAX_POINT_NUM];
}CURVE_SEC;

typedef struct hdb_curve_seg 
{
	unsigned char stnid;
	unsigned char devid;
	unsigned short pointid;
	char	logicalname[40];					/* logical name of point */
	short	lastrscount;						/* old recordset number */
	short	rscount;						/* recordset number ,0: no recordset, -1: error */
	unsigned char	errcode;							
	float	data[MAX_SELECT_REC];					/* data buffer of query */
	char	quality[MAX_SELECT_REC];				/* data quality, 0: normal    other: error code */	
	char	timestr[MAX_SELECT_REC][22];				/* time of the record */
	char	lasttime[22];						/* last record's time of query */			
} HDB_CURVE_SEG;

typedef struct hdb_curve
{
	char	queryflag;						/* changing flag,1:need to be update,0 or -1:needn't */
	char 	querytype;						/* query type  0: history curve    1: real changing curve */
	char	lockflag;						/* lock flag,1:locked, 0 or -1:unlocked */
	char	finishedFlag;					//查询完成标志，0未完成，1完成
	unsigned char	num;						/* number of the curves to be queried */
	char	interval;						/* interval type,0: one second  1:one minutes  
												2: five minutes  3:fifteen minutes  4:one hour */	
	char	starttime[22];						/* start time of query */	
	char	endtime[22];						/* end time of query */		
	struct	hdb_curve_seg    seg[MAX_SELECT_SEG]; 
	unsigned char connflag;	
} HDB_CURVE;

struct yc_period_data
{
	float startdata;
	float max;
	float min;
	float sum;
};

struct array_period_data
{
	int		id;
	char	timestr[22];
	float startdata;
	float max;
	float min;
	float avg;
};

struct yc_period_seg
{
	POINTER point;
	int id;									/* added by yyp 07.12.27 for hdb */
	char logicalname[50];
	unsigned char period;
	float realvalue;
	struct yc_period_data data_cur;
	struct yc_period_data data_last;
};

typedef struct hdb_yc_period
{
	int num;
	int flag;
	struct yc_period_seg seg[MAX_YC_NUM];
} HDB_YC_PERIOD;

struct trend_seg
{
	int id;
	POINTER point;
	char logicalname[50];
	float changerate;
	unsigned char period;
	unsigned char method;
	int overthd;
	int overcount;
	int rank;
	int group;
	int iscontinue;
	int isvalid;
	float realvalue;
	struct yc_period_data data_cur;
	struct yc_period_data data_last;		
};

typedef struct hdb_trend
{
	int num;
	int flag;
	struct trend_seg seg[MAX_TREND_NUM];
} HDB_TREND;

struct point_value
{
	POINTER point;
	char	logicalname[55];
	struct	tm basetime;
	float	value[MAX_REVERT_REC];
	unsigned short	status[MAX_REVERT_REC];
	char	timestr[MAX_REVERT_REC][22];
	short	rscount;
	char	chgflag;
};

typedef struct hdb_revert
{
	char	queryflag;
	char 	querytype;
	char	stopflag;
	char	curptr;
	char	starttime[22];
	char	endtime[22];
	char	lasttime[22];	
	struct point_value yc_data[MAX_REVERT_NUM];
	struct point_value yx_data[MAX_REVERT_NUM];
	unsigned int ycnum;
	unsigned int yxnum;
	char	lockflag;
	unsigned char connflag;
}HDB_REVERT;

struct al_seg
{ 
	char	l_name[55];						/* 逻辑名 */
	unsigned char	type_id;					/* 类型标识 */
	POINTER	pointer;
	char	TimeTag[22];						/* 报警时间 */
	int	msecond;						/* 毫秒 */
	short	status;							/* 点状态 */
	float	fvalue;							/* 模拟量的值，只对模拟量有用 */
};

typedef struct hdb_al
{
	char	queryflag;
	char	querytype;					/* 默认为0，如果为 2 则为事故回放模式 */			
	char	starttime[22];
	char	endtime[22];
	char 	filter_stnid;
	char	filter_devid;
	char	filter_typeid;
	char	cname[100];
	unsigned char cname_type;			/* 1:longname ; 2: logicalname */
	char	lockflag;
	unsigned int alnum;
	struct al_seg rec_data[MAX_AL_NUM];	
	HDB_HISTORY_RECORD	hdb_rec;
}HDB_AL;

typedef struct hdb_scene_seg
{
	int scene_id;
	char timestr[30];
	int type;
	char inserttime[30];
	char remark[500];
}HDB_SCENE_SEG;

typedef struct hdb_scene
{
	char	queryflag;					/* 0: no new query;   1: new query */
	char	querytype;					/* 0: query scene list;  1: query scene data;   2: write from real;   3: write from hdb */
	int 	scene_id;					/* when querytype = 1 */
	char	scene_time[30];				/* when querytype = 3 */
	char	lockflag;					
	unsigned int scenenum;				/* when querytype = 0 */	
	struct hdb_scene_seg scene_data[MAX_HDB_SCENE];	
}HDB_SCENE;



typedef struct shm_hdb
{
	HDB_CURVE	data_curve[MAX_CRT_NUM];
	HDB_REVERT	data_revert;
	HDB_AL		data_al;
	HDB_SCENE	data_scene;	
	unsigned char	connflag_curve;		
	unsigned char	connflag_revert;	
	unsigned char	connflag_al;
	unsigned char	connflag_scene;
	unsigned char	readpt_real;
	unsigned char	readpt_curve;
	unsigned char	readpt_period;
} SHM_HDB;

#define SHM_HDB_SIZE   sizeof(struct shm_hdb)
extern SHM_RECORD data_rec,*shm_rec_addr; 

#endif

