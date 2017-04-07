#ifndef _HDB_SCAN_HEAD
#define _HDB_SCAN_HEAD

#define YC_UPDATE_REAL		1
#define YC_UPDATE_PERIOD	2		

#define YC_TABLE	"/home/ems/h9000/his/trap/yc_table.txt"
#define YX_TABLE	"/home/ems/h9000/his/trap/yx_table.txt"

#define MIN_OFFSET	0.001
#define	HH_LMT		1
#define	H_LMT		2
#define	L_LMT		3
#define	LL_LMT		4
#define	HR_LMT		5
#define	LR_LMT		6
#define DD_LCU_START 50			//added by yyp 2011-01-10

void	GenInfoTable(), DellBlank( char *, char *, int ), BlankFillCopy( char *, char *, int );
int	GetLongNameByPoint( POINTER,	char * );
int	CreatYcInfo(  FILE *, unsigned char ), CreatYxInfo(  FILE *, unsigned char ),CreatCharaInfo(unsigned char stnid );
float	AnaDataToFloat( DB_DATA );
int	GetLocalStat( char * );

void	GetYcData(int),	GetCharaData(int),	GetYxData(int),	GetAlData(int),GetJhData(int),GetTrendData(), UpdateStatInHDB(),
	UpdateYcInHDB(),UpdateCharaInHDB(),	UpdateYxInHDB(), UpdateAlInHDB(),UpdateJhInHDB(),UpdateObjAlInHDB(),
	UpdateAlFixInHdb(),	InsertYcEventInHDB(int,int), InsertYxEventInHDB(int,int),InsertTrendInHDB(),InsertYxInHDB(),
	ReadDefFile(),	EndProc();
int	InitProc();		
int	LinkHdb();
int	UnLinkHdb();
void	StatisticProc( char *, char *, int );
void *	CurveScan(void *); 
void *	RevertScan(void *); 
void *	AlarmScan(void *); 
int InitCurve(struct hdb_curve *p_rs,int CrtID); 
int UpdateCurveShm(struct hdb_curve *p_rs, int CrtID);
int InterCurveData(struct hdb_curve *p_rs, int CrtID);
int InterGenPowerPlanCurveData(struct hdb_curve *p_rs, int CrtID, unsigned char curid, char interval);
int CreateCurveShm();
int MapCurveShm();

int InitRevert(struct hdb_revert *p_rs); 
int WriteRevertData();
int WriteInitData();
int InterRevertData(struct hdb_revert *p_rs);

int GetQueryInfo(struct hdb_al *p_rs);
int UpdateAlarmShm(struct hdb_al *p_rs);

void GetYxInitData();
void GetYcInitData();

#endif
