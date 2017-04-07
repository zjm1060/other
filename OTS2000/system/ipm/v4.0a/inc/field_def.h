#define	BAR_GRPH_UP		0
#define	BAR_GRPH_RIGHT		1
#define	BAR_GRPH_LEFT		2
#define	BAR_GRPH_DOWN		3
#define	BAR_GRPH_UP_C		4
#define BAR_GRPH_RIGHT_C	5
#define	BAR_GRPH_LEFT_C		6
#define	BAR_GRPH_DOWN_C		7		


int	dio_type;
#define	OUTPUT_ONLY		0
#define	IN_OUT_PUT		1
#define	INPUT_ONLY		2


   	
int	qi_loc_type;
#define	QI_LOC_NONE		0
#define	QI_LOC_LEFT		1
#define QI_LOC_RIGHT		2
#define	QI_LOC_UP		3
#define QI_LOC_DOWN		4



int	tag_loc_type;
#define	TAG_NONE		0
#define	TAG_LEFT 		1
#define	TAG_RIGHT		2
#define	TAG_ABOVE		3
#define	TAG_BELOW		4
#define	TAG_ABOVE_RIGHT		5
#define	TAG_BELOW_RIGHT		6
#define	TAG_ABOVE_LEFT		7
#define	TAG_BELOW_LEFT		8

int	data_class_type;
#define	LONG_NAME		0
#define	LOGIC_NAME		1
#define	RT_VALUE		2
#define	RT_STATE		3
#define	ALM_PROHIBIT		4
#define OUT_OF_SCAN		5
#define	DATA_QUALITY		6
#define	PT_CALC			7
#define	H_LMT			8
#define	L_LMT			9
#define	HH_LMT			10
#define	LL_LMT			11
#define	HRTN_LMT		12
#define	LRTN_LMT		13
#define	DEAD_BAND		14
#define	ENG_CVTER		15
#define	ALM_ACK			16
#define	SCAN_ADDR		17
#define CTRL_ADDR_OPEN          18
#define CTRL_ADDR_CLOSE         19
#define Pt_Sgcz              20
#define Pt_Impt               21
#define IO_HDB                22
#define AUDIO_ALM               23
#define Pt_SBO                  24
#define Pt_CTRL                 25
#define Open_Num                26
#define Close_Num               27
#define	TestAddOpn		28
#define	TestAddCls		29
#define	CancelSyn		30
#define	Sgzy_Pnt		31
#define TstOpnNum               32
#define TstClsNum               33
#define SgOpnNum                34
#define SgClsNum                35
#define	DevName			36
#define	Pt_Trnd			37

int	Impulse_Set_Type;
#define	PEAK_CLASS1		0
#define	PEAK_CLASS2		1
#define	PEAK_CLASS3		2
#define	PEAK_CLASS4		3
#define	PEAK_DAY		4
#define	PEAK_MONTH		5
#define PEAK_YEAR		6
#define	VALLEY_CLASS1		7
#define	VALLEY_CLASS2		8
#define	VALLEY_CLASS3		9
#define	VALLEY_CLASS4		10
#define	VALLEY_DAY		11
#define	VALLEY_MONTH 		12
#define	VALLEY_YEAR 		13
#define	CVT_F			14


	

#define	FORM_WGT_NORMAL		0
#define	ADD_FORM_WGT		1


#define	MODIFY_DYN_TYPE		0
#define	CREATE_DYN_TYPE		1
#define	ADD_CTRL_TYPE		2


#define	NORMAL_VALUE		0	
#define	OVER_LMT_W		1
#define	OVER_LMT_A		2

#define REQ_HOURLY      0
#define REQ_DAYLY       1
#define REQ_MONTHLY     2
#define REQ_YEARLY      3

#define NULL_PKVLY      0
#define PEAK            1
#define PPEAK           2			// modified for v3.0	01/11/23
#define VALLY           3
#define OPEN_NUM        4
#define CLS_NUM         5
#define SG_OPN_NUM      6
#define SG_CLS_NUM      7
#define TST_OPN_NUM     8
#define TST_CLS_NUM     9
#define DAY_MAX			10 
#define	AVERG			11
#define	SUM				12
                                  
