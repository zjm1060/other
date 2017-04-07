#ifndef _OTS_COMMON_HEAD
#define _OTS_COMMON_HEAD



#ifdef __cplusplus
extern "C" {
#endif



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

extern	int	n_sequence, n_scene, n_step,n_show, n_group, n_entry, n_init,n_sceneseg, n_tfb;
extern	int	n_timer, n_unit, n_model, n_nlf, n_nlf_data, n_param,n_param_seq,n_spare, n_nlf_crv;
extern	int	ots_status;
extern	int	next_step_mask;
extern	char g_ots_hdb_type[10];
extern	char g_ots_hdb_ip[40];
extern	char g_ots_hdb_sid[10];
extern	int g_is_simuserver;
extern	double	time_cnt;
extern	int	nlf_rd_st;	/* NLF read mode status	*/
extern	char tch_bg_pic[50];
extern	int g_ots_ver;
extern	int g_all_send;
extern	int g_hdb_write;

extern	SEQUENCE_DEF	sequencedb[MAX_SEQUENCE];
extern	SCENE_DEF		scenedb[MAX_SCENE];
extern	STEP_DEF		stepdb[MAX_STEP];
extern	SHOW_DEF		showdb[MAX_SHOW];
extern	GROUP_DEF		groupdb[MAX_OR];
extern	ENTRY_DEF		entrydb[MAX_AND];
extern	INIT_DEF		initdb[MAX_INIT];
extern	SCENESEG_DEF	scenesegdb[MAX_SCENESEG];
extern	TIMER_DEF		timerdb[MAX_TIMER];
extern	MODEL_DEF		mdb[MAX_MODEL_NUMBER];
extern	TFB_DEF			tfbdb[MAX_TFB_NUM];
extern	PARAM_DEF		paramdb[MAX_PARAM_NUM],paramseqdb[MAX_PARAM_NUM];
extern	SPARE_DEF		sparedb[MAX_SPARE_NUMBER];
extern	NLF_DEF			nlfdb[MAX_NLF];
extern	HE_PTR_DEF		nlf_ptr[MAX_CURVES];
extern	NLF_DATA_DEF	nlfdata;
extern	UNIT_DATA_DEF	unitdb[MAX_UNIT_NUMBER];
extern	SEQID_DEF		seqid,seqid_auto;	



void	SequenceFileFree ();
int		SimulogDataLoad ();
double	TriInterpolation ();
int		FillDisturbScene();


int		ReadSequenceID();
int		ReadSequenceAuto();
int		GetSeqNameFromID(OTS_LINK_DEF *lnk);
int		GetSequenceIdByName ( char *sq_name );
void 	BdANADisturb( SCENESEG_DEF seg);


int		GetAnaValueFromName(char *name,float *fvalue);
int		BdYGParam(unsigned char stnid,unsigned char devid,float dst);
int		BdWGParam(unsigned char stnid,unsigned char devid,float dst);
int		GetFZStyle();

int		GetMaxIdInHdb(unsigned char table_id);
int		GetInexistenceIdInHdb(unsigned char table_id);
void	GetCurTimeStr(char*);
int		tmtostring(struct tm dtime, char *str);
int		WriteRec(char *str);
int		WriteRecData(int user_id,int type,char *str);
int		BdPollingCmd(unsigned char stnid, unsigned char devid);
int		SetPtScanEnable(POINTER *p_pt, unsigned short status);
int		SetAllPtScanEnable();

int		CvtArrayToStr(char *str, int len);
void	ReadOtsDefFile();
int		GetYx( char yx_LogiName[][55] );
int		GetYc( char yc_LogiName[][55] );
int		CheckStrIsNumeric ( char str[] );
unsigned short	GetCharBits (unsigned short v,unsigned short start_bit,unsigned short bit_num );
int		ReadDBByName( char *name,  DB_DATA *db);
int		GetPointNumber( POINTER * p_point );

#ifdef __cplusplus
}
#endif


#endif