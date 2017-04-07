#ifndef _HDB_LIB_HEAD
#define _HDB_LIB_HEAD
#include <ocidfn.h>
#include <ctype.h>
#include <ocidem.h>
#include <oci.h>
typedef struct 
{	short	RtnCode;
	float	fvalue[MAX_SELECT_REC];
	float	max_value[MAX_SELECT_REC];	
	float	min_value[MAX_SELECT_REC];	
	float	avg_value[MAX_SELECT_REC];		
	char	TimeStr[MAX_SELECT_REC][22];	
}HDB_RECSTRU;

typedef struct 
{	short	RtnCode;
	int	id[MAX_SELECT_REC];
	char	TimeStr[MAX_SELECT_REC][22];
	float	fvalue[MAX_SELECT_REC];	
	int	qid[MAX_SELECT_REC];
}WMS_RECSTRU;


typedef struct
{	
	unsigned char	dev_id;
	char		data_type;
	short		point_id;
	unsigned short	status;
	unsigned short	quality;
	float		fvalue;
	float		HI_limit;	  
	IOATTR		ioattr;
	short		year;
	char		month;
	char		day;
	char		hour;
	char		minute;
	char		second;
	short		ms;
	unsigned int    LongValue;
} DB_DATA;

#define	MAX_CALC_REC		10


typedef struct cocictx
{
  OCIEnv *envhp;
  OCIServer *srvhp;
  OCISvcCtx *svchp;
  OCIError *errhp;
  OCISession *authp;
  OCIStmt *stmthp;
} cocictx;

typedef struct
{
	unsigned long id;//unsigned int id;;
	char datetime[22];
    float data[60];
}SECDATA;

struct hdb_id
{
	POINTER			point;
	char			l_name[55];
	float			offset;
	int			id;
	int			iscond;
	int			isvalid;
} ;

struct hdb_dev_cond
{
	int id;
	char logicalname[55];
	short status;
};

typedef struct
{ 	
	unsigned short	status;
	unsigned short	data;
}ANA;

struct al_rec
{ 
	char			user_name[20];
	char			l_name[55];				/* 逻辑名 */
	unsigned char	type_id;				/*  */
	unsigned char	al_type;
	POINTER			pointer;
	char			TimeTag[22];			/* 报警时间 */
	int			msecond;					/* 毫秒 */
	short			status;					/* 点状态 */
	float			fvalue;					/* 模拟量的值，只对模拟量有用 */
	short			ChgFlag;				/* 1表示该点需要写入报警记录表，0表示不写 */
	char			alarm_str[200];			
} al[MAX_AL_NUM];

struct yx_seg
{	
	int			id;
	char		LogiName[55];
	IND_DATA	status;
	float		fvalue;
	unsigned char	quality;
	unsigned char	data_type;
	IOATTR		ioattr;
	short		ms;
	char		TimeTag[22];
	char		ReadTime[22];
	char		ChgFlag;
	char		IsValid;		
	int			status_id;
}yx[MAX_YX_NUM];

struct yc_seg
{	
	int			id;
	char		LogiName[55];
	IOATTR		ioattr;
	float		offset;
	float		HI_limit;
	char		TimeTag[22];
	char		ChgFlag;
	char		StatusChgFlag;
	float		value;
	unsigned short	status;
	unsigned int    LongValue;
}yc[MAX_YC_NUM];


struct scene_data
{
	POINTER		pt_id;
	char		LogiName[55];
	unsigned short	status;	
	float		value;	
}scdata[MAX_PT_NUM];

struct stat_seg
{
	int id;
	short oldstatus;
	short curstatus;
	char lasttime[22];
	char starttime[22];
	char isstat[16];
	char istrue[16];
	char name[20][16];
	float totaltime[16];
	char ischg;		
}yxstat[MAX_STAT_NUM];

struct yx_revert 
{
	int		id;
	char	starttime[22];
	char	endtime[22];	
};

SECDATA data_all[MAX_IN_ROWS];
SECDATA data_all_hdb[MAX_IN_ROWS];
sb2		sec_ischg[60][MAX_IN_ROWS];
struct hdb_dev_cond cond[MAX_STN_NUM+1][MAX_LCU_NUM+1];

static  Lda_Def lda_rt; 
static  Lda_Def lda_al; 
static  Lda_Def lda_cv; 
static  Lda_Def lda_cvw;
static  Lda_Def lda_rw;
static  Lda_Def lda;
static  Cda_Def	cda_rt; 
static  Cda_Def	cda_al;
static  Cda_Def	cda_cv;
static  Cda_Def	cda_cvw;
static  Cda_Def	cda_rw;
static  Cda_Def	cda;
static	cocictx ctx;
static	cocictx *ctxptr;

extern char	LoginID[50];
extern char	SIDSTR[100];
extern char	strPwd[];
extern char	strUsername[];
extern char	strPassword[];
extern int	Interval;
extern char	Yc_Update_Mode;
extern int	HdbConn;
extern struct hdb_id hdb_pt_id[MAX_DEV_NUM_HDB][MAX_PT_NUM_DEV];

extern int	hdb_id_num;
extern int	hdb_all_num;
extern int  StatNum;
extern LCUID dev_id[MAX_DEV_NUM_HDB];
extern int dev_pos[MAX_STN_NUM+1][MAX_LCU_NUM+1];
extern int pt_pos[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_DATATYPE_NUM+1];
extern int hdb_dev_num;

extern struct lcu_pt_conf ptconf[MAX_STN_NUM+1][MAX_LCU_NUM+1];

int curve_array_insert(cocictx *ctxptr,int hdb_pt_num);
int InsertHdbByYcStatus(Cda_Def cda,int id,  char *DateTime, int istatus  );
int InsertHdbBySceneData( Cda_Def cda, int scene_id, int ptid, float fvalue, short status); 
int InsertHdbByScene(Cda_Def cda, int scene_id, char *TimeStr, int type, char *InsertTime );
int InsertOneYcSceneFromHDB(Cda_Def cda,char *logicalname, char *time_str, int scene_id);
int InsertOneYxSceneFromHDB(Cda_Def cda,char *logicalname, char *time_str, int scene_id);
int SelectSceneListFromHdb(Cda_Def cda , unsigned int *p_num, struct hdb_scene_seg *p_scene);
int SelectSceneDataFromHdb(Cda_Def cda , int scene_id,  unsigned int *p_num, struct scene_data *p_data);
int DelSceneFromHDB(Cda_Def cda, int scene_id);

int UpdateHdbByYc(Cda_Def ,int, char *, float *, int);
int UpdateHdbByYc_A(cocictx *ctxptr, struct yc_seg *p_yc, int rownum);
int UpdateHdbByChara(Cda_Def cda, int id, char *DateTime, float max, float min, float avg,
					 float max_change, float min_change,float standard,unsigned short max_no,
					 unsigned short min_no,unsigned short max_change_no,unsigned short min_change_no);
int InsertHdbByChara(Cda_Def cda, int id, char *DateTime, float max, float min, float avg,
					 float max_change, float min_change,float standard,unsigned short max_no,
					 unsigned short min_no,unsigned short max_change_no,unsigned short min_change_no);
int UpdateHdbByStat(Cda_Def cda, struct stat_seg *p_stat);
int UpdateHdbByPeriod(cocictx *ctxptr, struct hdb_yc_period *p_data);
int InsertHdbByYcEvent(Cda_Def ,char *, char *, int, float *);
int UpdateHdbByYx(Cda_Def ,int, char *, int, int);
int UpdateHdbByYx_A(cocictx *ctxptr, struct yx_seg *p_yx, int rownum);
int UpdateHdbByJh(Cda_Def, int, int, float, float,float );
int InsertHdbByJh( Cda_Def cda, int, int num);
int InsertHdbAlarm(Cda_Def ,char *, int, int, int,float,unsigned char);
int InsertHdbAlarm_A(cocictx *ctxptr, struct yx_seg *p_yx, int rownum);
int InsertHdbSmsAlarm(Cda_Def cda, char *DateTime, char *message, unsigned char typeid  );
int InsertHdbByYxEvent( Cda_Def ,char *, char *, int, int  );
int InsertHdbByYx_A(cocictx *ctxptr, struct yx_seg *p_yx, int rownum);
int	InsertHdbRecord( Cda_Def cda, int user_id,int host_id,int groups_id, char *datetime,short ms, 
					unsigned char type, char *content, unsigned char station_id, unsigned char dev_id,int task_id, int send_task_id,
					char *logicalname, unsigned char type_id, short status,float fvalue);
int SelectYcRecFromHdb( Cda_Def ,unsigned char curid, struct hdb_curve *shm_curve_addr);
int SelectCurveFromHdb( Cda_Def ,unsigned char curid,struct  hdb_curve *p_rs);
int SelectRevertFromHdb( Cda_Def cda,struct  hdb_revert *p_rs, struct point_value *p_data);
int SelectYxRevertFromHdb(Cda_Def cda,struct  hdb_revert *p_rs,struct point_value *p_data);
int SelectGenPowerPlanCurveFromHdb(Cda_Def cda, unsigned char cur_id,struct  hdb_curve *p_rs);

int SelectYxRevertFromHdb_A(cocictx *ctxptr, struct point_value *p_data, struct  yx_revert *p_yx, int rownum);
int SelectYxRevertFromHdb_First(Cda_Def cda, unsigned short curid, struct  hdb_revert *p_rs);
int SelectAlRecFromHdb( Cda_Def ,struct  hdb_al *p_rs);
int SelectIdFromHdb(Cda_Def cda);
int SelectStatPtFromHdb(Cda_Def cda_rw);
int SelectStatTrueFromHdb(Cda_Def cda, struct stat_seg *p_stat);
int	SelectTrendInfoFromHdb(Cda_Def cda,HDB_TREND *tdata);
int SelectDevCondFromHdb(Cda_Def );
int SelectOneIdFromHdb(Cda_Def cda, struct hdb_id *p_id );
int SelectLogicalNameFromHdb(Cda_Def cda, struct hdb_id *p_id );
int ConnectHDB(Lda_Def * lda,Cda_Def *cda, char * );
int ConnectHDB_B(cocictx *ctxptr,char *svrname,char *username,char *password);
int DisconnectHDB(Lda_Def * lda,Cda_Def *cda);
int DisconnectHDB_B(cocictx *ctxptr);
int checkerr(OCIError *errhp,sword status);

void OracleError(Lda_Def * lda); 
void EndProc();
int ExecuteSQL(Cda_Def , char * );
int MSleep(int mSec);
int GetPtConf();
int GetPointNumber( POINTER * p_point );

int ReadDBByName( char *name,  DB_DATA *db);
int IsPtIntoHdb(POINTER * p_point);
int IsPtIntoCurveHdb(POINTER * p_point);
int GetHdbIDByName(unsigned int *p_id,char * logicalname);
int GetHdbNameByID(unsigned int *p_id,char * logicalname);
int GetHdbCondByName(unsigned int *p_cond,char * logicalname);
void GetDevCond();
int GetPtHdbOffsetByName( char *, float *) ;
void ReadDefFile();
int GetLocalStat( char *name );
unsigned short	GetCharBits (unsigned short v,unsigned short start_bit,unsigned short bit_num );
int StrToTime(char *timestr,struct tm *p_datetime,unsigned char interval,unsigned char flag);
int DateAdd(struct tm *p_datetime,unsigned char interval);
struct tm DateAddSec(struct tm datetime,int num);
int DateMinus(struct tm *p_datetime,unsigned char interval);
int tmtostring(struct tm dtime, char *str);
void sig_termp();
char *myasctime(struct tm dtime);
#endif

