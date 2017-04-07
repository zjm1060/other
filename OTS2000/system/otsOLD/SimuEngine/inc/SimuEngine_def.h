#ifndef _SIMULOG_DEF_HEAD
#define _SIMULOG_DEF_HEAD

#define	MAX_SEQUENCE		10000
#define	MAX_SCENE			500
#define	MAX_SHOW			MAX_SEQUENCE*10
#define	MAX_STEP			MAX_SEQUENCE*10
#define	MAX_OR				MAX_STEP*3
#define	MAX_AND				MAX_OR*6
#define	MAX_INIT			MAX_SEQUENCE*5
#define	MAX_SCENESEG		MAX_SCENE*20
#define	MAX_TIMER			200
#define	MAX_TFB_NUM			10000
#define	MAX_PARAM_NUM		10000
#define MAX_SPARE_NUMBER	20000
#define MAX_MODEL_NUMBER	1000
#define	MAX_NLF_X			1000
#define	MAX_NLF_Y			1000
#define	MAX_NLF_F			MAX_NLF_X*MAX_NLF_Y
#define	MAX_NLF				1000
#define	MAX_CURVES			50000
#define	MAX_UNIT_NUMBER		50
#define NORMAL_SPEED		4

#define DBNAME_SIZE		64
//yyp 2008.3.27
#define	SIMULOG_DATA_HOME	"/home/ems/h9000/ots/"
#define OTS_VER_TRAIN	0				
#define OTS_VER_OPR		1

/*----------------------------------------------------------------------------------------------*/
/*					CONSTANTS 						*/
/*----------------------------------------------------------------------------------------------*/
#define	FIRST_F_LINE	1
#define	OTHER_F_LINE	2
#define	FIRST_STEP		1
#define	DEFAULT_DELAY	10
#define	DEFAULT_INTERVAL	NULL
#define	ASTERIX	42
#define	SADD	43
#define	SSUBTRACT	45
#define	SPOINT	46

/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*					KEY WORDS						*/
/*----------------------------------------------------------------------------------------------*/
#define	A					"A:"				/* tfb parameter A		*/
#define	ALARM_DELAY			"ALARM_DELAY:"		/* delay to issue alarm when step is blocked	*/
#define	ASTART_TIME			"ASTART_TIME:"		/* absolute time to start a process	*/
#define	B					"BS:"				/* tfb parameter B		*/
#define	C					"C:"				/* tfb parameter C		*/
#define	C1					"C1:"				/* tfb C1	*/
#define	D					"DS:"				/* tfb parameter D		*/
#define	DELAY				"DELAY:"			/* step action delay time in s	*/
#define	DIAMETER			"DIAMETER:"			/* Unit rotor diameter in m	*/
#define DO_WHILE			"DO_WHILE:"
#define	E1					"E1:"				/* tfb E1	*/
#define	ELSE_MODEL			"ELSE_MODEL:"		/* else go run a new sequence	*/
#define	ELSE_SEQUENCE		"ELSE_SEQUENCE:"	/* else go run a new sequence	*/
#define	ELSE_STEP			"ELSE_STEP:"		/* Must be first line of a else step		*/
#define	END_FILE			"END_FILE:"
#define	END_FUNCTION		"END_FUNCTION:"		/*	*/
#define	END_GEN_UNIT		"END_GEN_UNIT:"		/* end of an unit specification	*/
#define	END_INITIALIZE		"END_INITIALIZE:"	/* Must be located after INITIALIZE	*/
#define	END_MODEL			"END_MODEL:"
#define	END_PARAM			"END_PARAM:"
#define	END_SEQUENCE		"END_SEQUENCE:"		/* Must be end of a sequence		*/
#define	END_SCENE			"END_SCENE:"		/* Must be end of a sequence		*/
#define	END_SHOW			"END_SHOW:"		/* Must be end of a sequence		*/
#define	EQUATION			"EQUATION:"			/* Must be first line of equation	*/
#define	EQUA_TYPE			"EQUA_TYPE:"			/* Must be first line of equation	*/
#define	FACTOR				"FACTOR:"			/* 		*/
#define	F_DATA				"F_DATA:"			/* NLF data F entry		*/
#define	F_NAME				"F_NAME:"			/* name of F variable in NLF		*/
#define	FILE_NAME			"FILE_NAME:"		/* Must be first line of a file		*/
#define	FROM				"FROM:"				/* used to specify TFB connection	*/
#define	FUNCTION			"FUNCTION:"			/* NLF is bi-dimensional	*/
#define	GEN_UNIT			"GEN_UNIT:"			/* begin unit data specification	*/
#define	GO_SEQUENCE			"GO_SEQUENCE:"		/* go run a new sequence			*/
#define	GO_MODEL			"GO_MODEL:"			/* go run a new model				*/
#define	GO_SCENE			"GO_SCENE:"			/* go run a new scene				*/
#define	HN					"Hn:"				/* Unit nomainal head in m	*/
#define	INITIALIZE			"INITIALIZE:"		/* Must be located before STEPs		*/
#define	INCLUDE				"INCLUDE:"			/* include specified script file	*/
#define	INPUT_DBNAME		"INPUT_DBNAME:"		/* tfb input variable dbname	*/
#define	INPUT_LONGNAME		"INPUT_LONGNAME:"	/* tfb input variable longname	*/
#define	INTERVAL			"INTERVAL:"			/* time interval to re-run a process	*/
#define	LINEARITY			"LINEARITY:"		/* typical non linearity	*/
#define	NEXT_LINE			">"					/* for multiplie line NLF curve data input	*/
#define	MODEL				"MODEL:"			/* line begin to specify a model	*/
#define	NAME				"NAME:"				/* 		*/
#define	NLF_NAME			"NLF_NAME:"			/* non-linear function name			*/
#define	NLF_TYPE			"NLF_TYPE:"			/* non-linear function type			*/
#define	NN					"Nn:"				/* Unit nomainal speed in r/m	*/
#define	ORDER				"ORDER:"			/* tfb order		*/
#define	OUTPUT_DBNAME		"OUTPUT_DBNAME:"	/* tfb output variable dbname	*/
#define	OUTPUT_LONGNAME		"OUTPUT_LONGNAME:"	/* tfb output variable longname	*/
#define	PN					"Pn:"				/* Unit nomainal P in MW	*/
#define	QN					"Qn:"				/* Unit nomainal flow Q in m3/s	*/
#define POS					"POS:"
#define	RESULT				"RESULT:"			/*					*/
#define	SPEED				"SPEED:"			/* model running speed added by yyp 2010.7.21 */
#define	RSTART_TIME			"RSTART_TIME:"		/* relative time to start a process	*/
#define SBIN				"SBIN"
#define	SEQUENCE			"SEQUENCE:"			/* begin a sequence	*/
#define	SCENE				"SCENE:"			/* begin a sequence	*/
#define	SHOW				"SHOW:"				/* begin a sequence	*/
#define	SIM_STEPS			"SIM_STEPS:"		/* 		*/
#define	STEP				"STEP:"				/* Must be first line of a step		*/
#define	STOP_MODEL			"STOP_MODEL:"		/* first line of a stop model	*/
#define	STOP_SEQUENCE		"STOP_SEQUENCE:"	/* first line of a stop sequence	*/
#define	STOP_SCENE			"STOP_SCENE:"	/* first line of a stop sequence	*/
#define	TA					"Ta:"				/* Unit time in s	*/
#define	TEMPLATE			"TEMPLATE:"			/* 		*/
#define TMPL_NORMAL			"NORMAL"
#define TMPL_TEMPLATE		"TEMPLATE"
#define TMPL_FROMTEMPLATE	"FROMTEMPLATE"
#define	TFB					"TFB:"				/* Must be first line of a tfb specification	*/
#define	TFB_CONNECTION		"TFB_CONNECTION:"	/* Must be first line of TFB matrix specification		*/
#define	TIME_STEP			"TIME_STEP:"		/* 		*/
#define	TIMER_MODEL			"TIMER_MODEL:"		/* Keyword for model Timer	*/
#define	TIMER_SEQUENCE		"TIMER_SEQUENCE:"	/* Keyword for sequence Timer	*/
#define	TO					"TO:"				/* used to specify TFB connection	*/
#define	TW					"Tw:"				/* Unit time in s	*/
#define	TYPE				"TYPE:"				/* signal type: ana/pol/int...		*/
#define	PARAM				"PARAM:"				/* signal type: ana/pol/int...		*/
#define	WAIT_FOR			"WAIT_FOR:"			/* wait for expression satisfied	*/
#define	X0					"X0:"				/* tfb status variable initial	*/
#define	X_DATA				"X_DATA:"			/* NLF data X entry		*/
#define	X_NAME				"X_NAME:"			/* name of X variable in NLF		*/
#define	Y0					"Y0:"				/* tfb output variable initial		*/
#define	Y_DATA				"Y_DATA:"			/* NLF data Y entry		*/
#define	YMAX				"Ymax:"				/* Unit max relay distance in m	*/
#define	Y_NAME				"Y_NAME:"			/* name of Y variable in NLF		*/
#define	RUNMODE				"RUNMODE:"		
#define	SEQ_TYPE			"SEQ_TYPE:"		
#define	SEQ_EQUATION		"SEQ_EQUATION:"	
		
/*----------------------------------------------------------------------------------------------*/

#define	INTEGRATION			0
#define	PROPORTIONAL_INTEGRATION	1
#define	INERTIE				2
#define	INERTIE_PROPORTIONAL	3
#define	PROPORTIONAL		4
#define	DIFFERENTIAL		5
#define	OSCILATION2			6

//modified by yyp 2009.4.28
#define	FT_GENERAL			1
#define	FT_SECOND_ORDER		2

#define	LINEAR				0
#define	PRE_SATURATION		1
#define	PRE_INSENSITIVE		2
#define	PRE_GEAR_GAP		3
#define	PRE_RELAY			4
#define	PRE_INSENSITIVE_RELAY	5
#define	PRE_INERTIE_RELAY	6
#define	PRE_NONLINEAR		7
#define	POST_SATURATION		11
#define	POST_INSENSITIVE	12
#define	POST_GEAR_GAP		13
#define	POST_RELAY			14
#define	POST_INSENSITIVE_RELAY	15
#define	POST_INERTIE_RELAY	16
#define	POST_NONLINEAR		18

/*	for non-linear function type nlf_type.	*/
#define	N1D	1			// 1 dimension
#define	N2D	2			// bi-dimension

#define	RD_NLF_X		1		/* read X	*/
#define	RD_NLF_F		2		/* read F	*/


#define OPEN		0
#define CLOSE		1

#define TYPE_NORMAL		0
#define TYPE_DISTURB	1


#define	OFF	0
#define	ON	1

#define	PRC_INACTIVATED	0
#define	PRC_ACTIVATED	1
#define	PRC_NORMAL		2
#define	PRC_BLOCKED		3
#define	PRC_SUSPENDED	4
#define	PRC_WAITING		5

#define	UNSATISFIED	0		/*	for flag setting	*/
#define	SATISFIED	1		/*	for flag setting	*/

#define	LINE_BUF_LENTH	256
#define	DATANAME_LENTH	36

#define	TAB		9
#define LF		10		/* line feed		*/
#define	CR		13		/* carriage return	*/

#define	BitS_STATUS		2
#define	BitN_STATUS		4
#define	BitS_ANA_POINT	5
#define	BitN_ANA_POINT	2
#define	BitS_ANA_SIGN	7

#define STAT_1		4
#define STAT_0		0

#define	M_LOGIC		0
#define	LOGIC		0x00ff
#define	CALC		0x0fff
#define DIFF		0x0ff0

#define	Simu_RESET	0	// wdk 2008.3.22 to avoid confliction
#define	ADD		0x0100
#define	MIN		0x0200
#define	MUL		0x0300
#define	DIV		0x0400

#define MASK_C		0x0f00
#define	RESET_C		0x1000
#define	ADD_C		0x1100
#define	MIN_C		0x1200
#define	MUL_C		0x1300
#define	DIV_C		0x1400


#define	BIN_LG		0
#define	LT		1	/*	less than	*/
#define	LE		2	/*	less equal	*/
#define	GT		3	/*	greater than	*/
#define	GE		4	/*	greater equal	*/
#define	EQ		5	/*	equal		*/
#define	NE		6	/*	not equal	*/


#define	MASK_STEP			0
#define	MASK_ELSE_STEP		1
#define	MASK_SEQUENCE		2
#define	MASK_ELSE_SEQUENCE	3
#define	MASK_STOP_SEQUENCE	4
#define	MASK_TIMER_SEQUENCE	5
#define	MASK_MODEL			6
#define	MASK_ELSE_MODEL		7
#define	MASK_STOP_MODEL		8
#define	MASK_TIMER_MODEL	9
#define	MASK_WAIT_FOR		10
#define	MASK_DO_WHILE		11
#define	MASK_SCENE			13

#define	PRC_SEQUENCE		1
#define	PRC_MODEL			2

#define	TIME_ABSOLUTE		1
#define	TIME_RELATIVE		2

#define	MODE_SERIAL			0
#define	MODE_PARALLEL		1

#define	SEQ_NORMAL			0
#define	SEQ_DEAD_LOOP		1
#define SEQ_LOGICAL			2
#define SEQ_AUTO_LOGICAL	3
#define EQUA_NORMAL			0
#define EQUA_CHANGE			1

#define	SHOWNAME_SIZE		DBNAME_SIZE * 2 +2 


#define	TIME_SPAN	0.080
#define TIME_SMALL	0.0002
#define	MAX_TIME_CNT	6555

typedef	struct
{
	int	head, end;
}	HE_PTR_DEF;

typedef	struct
{
	double	initial;	/* abs time when sq/mdl is activated */ 
	double	t;			/* relative time from sq/mdl is activated */
}	TIME_PTR_DEF;	/* time ptr for sq/mdl */

typedef struct
{
	int	opr;
	union
	{
		int	status;
		double	value;
	}	data;
	char	name[DBNAME_SIZE];
	//added by yyp 2012.11.16
	unsigned char isnum;
	char	value_name[DBNAME_SIZE];
}	ENTRY_DEF;

typedef struct
{
	union
	{
		int	status;
		double	value;
	}	data;
	char	name[DBNAME_SIZE];
}	INIT_DEF;

typedef struct
{
	union
	{
		int	status;
		float	value;
	}	data;
	int		speed;
	float		len;
	char	name[DBNAME_SIZE];
}	SCENESEG_DEF;

typedef struct
{
	int	head;
	int	num;
}	GROUP_DEF;

typedef struct
{
	char o_name[DBNAME_SIZE];
	int	type;
	int	result;
	int speed;
	float	delay_time;
	double	alarm_delay;	// delay in seconds to issue alarm when step is blocked.
	int	head;			//equation
	int	num;		
	int	mask;		/*	=0: MASK_STEP (default)	*/
	int	timer_id;	/* timer id in timerdb	*/
	int showid;		// added by yyp 2009.7.8
	int equa_type;
	int equa_old;	
	int old_ms;		// last loop time, ms		
}	STEP_DEF;



typedef struct
{
	char name[DBNAME_SIZE*2+2];
	char filename[128];
	int	type;
	int x,y;
	float	delay_time;
}	SHOW_DEF;



typedef struct
{
	int	year, month, day;	/* year, month, day	*/
}	DATE_DEF;

typedef struct
{
	int	hour, minute, second, ms;	/* hour, minute, second, million second	*/
}	TIME_DEF;

typedef struct
{
	DATE_DEF	date;	/* year, month, day	*/
	TIME_DEF	time, interval;
}	DATE_TIME_DEF;


typedef	struct
{
	char	name[DBNAME_SIZE];
	int		prc_type;				/* sequence/model	*/
	int		timer_type;				/* absolute/relative	*/
	char	start_date[DBNAME_SIZE];
	char	start_time[DBNAME_SIZE];
	char	interval[DBNAME_SIZE];
	DATE_TIME_DEF	dt, rt_dt;		/* dt used for SimuGen, rt_dt for SimuLog realtime */
	int	rts;			/* run-time ststus = ACTIVATED when the model is activated
								default = INACTIVATED */
}	TIMER_DEF;


typedef	struct
{
	char seq_name[DBNAME_SIZE];
	char seqid[30];
	char dbin[100][2];

	int	parentid;

	int	head;
	int	num;
	TIME_PTR_DEF	time;
	HE_PTR_DEF	param;
	int	cur_step;	/* save current step number	*/
	int	rts;		/* run-time ststus = ACTIVATED when the sequence is activated
								default = INACTIVATED */
	GROUP_DEF	init;		/* for sequence initial conditions	*/
	int	status;		/*	
						=2: NORMAL (default),
						=3: BLOCKED
						=5: WAITING
					*/
	double	blocked_time;	/* counted when sequence is blocked */
	int type;		//0 普通 NORMAL，1 循环自动 DEAD_LOOP，2 逻辑 LOGICAL，3 逻辑自动  AUTO_LOGICAL
	int	runmode;	//上次cond的状态，0，串行；1，并行
	int blocked_count;
}	SEQUENCE_DEF;


typedef	struct
{
	char	scene_name[DBNAME_SIZE];
	int	type;		/* 0: normal    1:disturb */
	int	head;
	int	num;
}	SCENE_DEF;


typedef struct
{
	char	longname[DBNAME_SIZE];
	char	dbname[DBNAME_SIZE];
	double	value;
	int		flag;
}	VARIABLE_DEF;


#define	IS_CONSTANT		0
#define	IS_NLF		1

typedef struct
{
	char	nlf_name[DBNAME_SIZE];
	double	data;	/* final coef data	*/
	double	dtmp;
	int	nlf_tag;	/* specify nlf: 1: nlf, 0:constant	*/
}	TFB_COEF_DEF;	/* Transfer Function Block NLF Coefficient	*/

typedef struct
{
	char	name[DBNAME_SIZE];
	char	nlf_name[DBNAME_SIZE];
	int	order, type, linearity;
	int	nlf_type;		/* specify type of non-linear function.	*/
	int	coef_type;		/* specify TFB coefficient is constant or variable.	*/
	TFB_COEF_DEF	a, b, c, d;
	double	e, f, g;
	double	e2[2][2], f2[2], g2[2];
	double	k;
	double	input, prev_input;
	double	prev_output;
	VARIABLE_DEF	output;
	VARIABLE_DEF	vin;		/* function block input value setting, or noice input	*/
	VARIABLE_DEF	x, x1, x2;
	double	c1, e1;
	TIME_PTR_DEF	time;
	int	status;
}	TFB_DEF;	/* Transfer Function Block	*/

typedef struct
{
	double value;
	int i, j;
}	SPARE_DEF;

typedef struct
{
	char param_name[DBNAME_SIZE];
	char pt_name[DBNAME_SIZE];
}	PARAM_DEF;

typedef struct
{
	double delta;	/*	delta value */
	int id;		/*	tfb id */
}	INTERFERENCE_DEF;


typedef	struct
{
	char	name[DBNAME_SIZE];
	unsigned char type[DBNAME_SIZE];
	char	template_name[DBNAME_SIZE];	
	double	t_step;
	int	iterations;
	int speed;
	int seq_id;
	INTERFERENCE_DEF	input;
	HE_PTR_DEF	tfb, spare, param;
	TIME_PTR_DEF	time;
	int	rts;			/* run-time ststus = ACTIVATED when the model is activated
								default = INACTIVATED */
}	MODEL_DEF;


typedef	struct
{
	int	nlf_type;
	HE_PTR_DEF	x, y, f;
	char	name[DBNAME_SIZE];
	char	xname[DBNAME_SIZE];
	char	yname[DBNAME_SIZE];
	char	fname[DBNAME_SIZE];
	double	min_x, max_x, min_y, max_y;
	TIME_PTR_DEF	time;
	int	rts;		/* run-time ststus = ACTIVATED when the sequence is activated
								default = INACTIVATED */
}	NLF_DEF;


typedef	struct
{
	double	x[MAX_NLF_X];
	double	y[MAX_NLF_Y];
	double	f[MAX_NLF_F];
}	NLF_DATA_DEF;


typedef struct
{
	char	name[DBNAME_SIZE];
	char	unit[DBNAME_SIZE];
	double	pn, hn, nn, qn, ymax, diameter, ta, tw;
}	UNIT_DATA_DEF;

//added by yyp 
typedef struct
{
	unsigned char stn_id;
	unsigned char dev_id;
	unsigned char type_id;
	unsigned short obj_id;
	unsigned short opr;
}	CMD_CODE;  

typedef struct 
{
	char seqid[30];
	char seqname[100];
}SEQID_SEG;

typedef struct 
{
	unsigned short num;
	SEQID_SEG seg[MAX_SEQUENCE];
}SEQID_DEF;

typedef struct
{
	POINTER	ptr;
	char	cmd_name[100];
	CMD_CODE cmd_code;
	double	value;
}	OTS_LINK_DEF;

typedef struct
{
	char	user_name[DBNAME_SIZE];
	char	company_name[DBNAME_SIZE];
	int	user_id;
	int	computer_id;
	int	password;
}	OTS_LICENSE_DEF;


// wdk the following are moved from dbwrite.h of v1.0 and can be removed if already defined.
typedef struct
{
	char	second;
	char	minute;
}	RTU_TIME;

typedef struct
{
	short	ms;
	char	second;
	char	minute;
}	RTU_TIME_MS;






#endif