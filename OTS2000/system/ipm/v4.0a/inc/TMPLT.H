
#define	TMPLT_FILE_NAME	"/tmplt/ipm.tpl"
#define	OPTION_NAME_LEN		14	

#define	FLD_NAME_LEN	20
#define	FLD_TYPE_NAME_LEN	20	
#define	FLD_TYPE_CNT	4

#define	MAX_FILEDS	25
#define	MAX_CTRL_FLDS	10
#define	TMPLT_NAME_LEN	20
#define	LIST_CNT	100	  // old --- 50 
#define	MAX_INPUT_LEN	35
#define   MAX_INPUT_TEXT_LEN  50
#define	MAX_TMPLTS	50	  //30

#define	FLD_TYPE_OPT_GRP	0
#define	FLD_TYPE_OPT_MBR	1
#define FLD_TYPE_TEXT		2
#define FLD_TYPE_BOOL		3		


#define GRP_TYPE_COLOR			0
#define	GRP_TYPE_SYMBOL			1		 		
#define	GRP_TYPE_STRING			2		 		
#define GRP_TYPE_CTRL_MENU		3	
#define GRP_TYPE_CTRL_PROG		4	
#define	GRP_TYPE_REQ_TYPE		5
#define	GRP_TYPE_RPT_HOUR_ID		6
#define	GRP_TYPE_RPT_DATE_ID		7
#define	GRP_TYPE_RPT_MONTH_ID		8


#define	MBR_TYPE_DIO		0
#define	MBR_TYPE_TAG		1
#define	MBR_TYPE_QULI		2
#define	MBR_TYPE_IMP_CLASS	3
#define	MBR_TYPE_DATA_CLASS	4
#define	MBR_TYPE_DIR		5
#define	MBR_TYPE_REQ		6	
#define	MBR_TYPE_SCHED		7	
#define	MBR_TYPE_DSP_REQ	8	
#define MBR_TYPE_IMP_TIME	9
#define MBR_TYPE_SIGN_TYPE	10
#define	MBR_TYPE_NUM_LEN	11
#define	MBR_TYPE_DEC_NUM	12
#define	MBR_TYPE_RPT_PERIOD	13
#define	MBR_TYPE_RPT_SIGN	14	
#define	MBR_TYPE_STATUS		15
#define	MBR_TYPE_RPT_SPECIAL 	16
#define	MBR_TYPE_PT_STATE	17
#define MBR_TYPE_TMP_CHARA	18
#define MBR_TYPE_V_A_S		19


#define	OPT_TYPE_CNT	25	/* max of MBR_TYPE and  GRP_TYPE numbers */	

#define	REQ_HOURLY		0
#define	REQ_DAYLY		1
#define	REQ_MONTHLY		2
#define	REQ_YEARLY		3

#define	TIME_NULL	0


#define	DB_ENTRY		0
#define	DSP_ENTRY		1	
#define	F_B_OFF			2
#define	HIGH_LIMIT		3
#define	LOW_LIMIT		4	
#define	DATA_LEN		5	
#define	DEC_NUM			6
#define	EQUAT_ENTRY		7
#define UPDATE_TIME		8	
#define	HIGH_LIMIT_2		9
#define	LOW_LIMIT_2		10
#define	CURVE_PT_NUM		11    //chichi
#define	LOAD_CUR_HI		12        //chichi
#define	LOAD_CUR_LOW		13    //chichi
#define DATA_LEN_TEMP		14    //chichi 
#define DB_ENTRY_V_L     15       // chichi  vibrate area  low limit 
#define	HIGH_LIMIT_V		16    // chichi  vibrate area  high limit 
#define	LOW_LIMIT_V		17        // chichi  vibrate area  low limit
#define DB_ENTRY_R_C     18       // chichi  curve load real curve  
#define START_TIME     19		  // chichi scene redisplay  
#define TIME_LENGTH     20			// chichi scene redisplay
#define DSP_FILE_NM    21		// chichi scene redisplay  filename

 
 
#define	ERR_TEXT_REQ_TYPE	-1

int	ctrl_tag;			//no use		
#define	CTRL_TAG_OFF		0
#define	CTRL_TAG_ON		1

#define	ERROR_MBR_ITEM_VALUE	-1

typedef	struct
{
	int		list_cnt;
	char	list_name[LIST_CNT][OPTION_NAME_LEN];	
	int		dflt_value;
	int		list_value[LIST_CNT];
}	OPT_MBR_FLD;

typedef struct
{
	int		grp_type;
}	OPT_GRP_FLD;


typedef	struct
{
	int	bool_num;
	
}	BOOL_FLD;


typedef	struct
{
	char	input_text[MAX_INPUT_LEN];
	int	req_type;	
}	TEXT_FLD;

typedef	union
{
	OPT_MBR_FLD	opt_mbr_fld;	
	OPT_GRP_FLD	opt_grp_fld;
	BOOL_FLD	bool_fld;
	TEXT_FLD	text_fld;
}	FLD_DEF;


typedef	struct
{
	char	fld_type_name[FLD_TYPE_NAME_LEN];
	char	fld_name[FLD_NAME_LEN];
	int	x;
	int	y;
	int	width;
	FLD_DEF	fld_def;
}	GBFLD;

typedef struct
{
	char	tmplt_name[TMPLT_NAME_LEN];
	int	format;
	int	fld_cnt;
	int	width,height;
	GBFLD	fld_buff[MAX_FILEDS];
}	TMPLT;


TMPLT	cur_tmplt;
typedef struct
{
	TMPLT	tmplt[MAX_TMPLTS];
	int	nextpos;
}	TMPLT_BUFF;

TMPLT_BUFF	tmplt_buff;
int	tmplt_def_cnt;

typedef	struct
{
	char	name[TMPLT_NAME_LEN];
	int	format_num;
	int	size;
	int	field_ptr;
	int	fld_size;
}	TMPLT_NAME_PTR;

TMPLT_NAME_PTR	tmplt_nm_ptrs[MAX_TMPLTS];


typedef	struct
{	
	int	mbr_type;
	int	mbr_value;	
}	OPT_BTN_DATA;
int	fld_top_position;

