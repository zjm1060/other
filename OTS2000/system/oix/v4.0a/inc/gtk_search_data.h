#ifndef    _GTK_SEARCH_DATA_H
#define    _GTK_SEARCH_DATA_H

#define MAX_SEARCH_STR_SIZE 12
#define OPT_ITEM_NUM 4
static char *com_property[]={OIX_STR_FAULT ,OIX_STR_FAIL,OIX_STR_DATA_QUALITY,OIX_CSTR_MAN_SET_OR_BLOCK,OIX_CSTR_ScanEnable,
OIX_CSTR_AlarmEnable,OIX_CSTR_AlarmNoAck,OIX_STR_CTRL_LOCK,OIX_CSTR_NO_UPDATE,OIX_CSTR_ALM_REPEAT,
OIX_CSTR_LONGNAME,OIX_CSTR_NAME,OIX_CSTR_UNIT_NO,OIX_CSTR_SLOT,OIX_CSTR_CHANNEL};
int com_property_col;

static char *Ana_property[]={OIX_CSTR_RT_VALUE/*,OIX_CSTR_BLOCK*/};
#define    Ana_property_col (sizeof(Ana_property)/sizeof(Ana_property[0]))

static char *Imp_property[]={OIX_STR_ACCUM};
int Imp_property_col;

static char *Pol_SOE_property[]={OIX_CSTR_RT_STATE};
int Pol_SOE_property_col;



#define ANA_PROPERTY (com_property_col+1)

#define POL_PROPERTY (ANA_PROPERTY+1)
#define IMP_PROPERTY  (POL_PROPERTY+1)

#define MAX_RESULT_NUM  ENTRY_STATUS_NUMBER

static int selected_items[MAX_CRT_NUMBER]={0,0,0};
static char Optlabel_str[OPT_ITEM_NUM][256];
static char *Optentry_name[]={"opt1_entry","opt2_entry","opt3_entry","opt4_entry"}; 
static char *OptComb_name[]={"opt1_comb","opt2_comb","opt3_comb","opt4_comb"};
static char *opr_name[]={"=","<",">","<=",">=","!="};
int opr_num;

typedef struct
{
    int start_id;
    int end_id;
    int page_no;
    int page_num;
}CUR_PAGE_INFO;
CUR_PAGE_INFO cur_search_page;
POINTER result_pt[MAX_RESULT_NUM];

struct 
{
	GtkWidget*	btn[256];
	int			btn_num;
}	property_btn_index;

typedef struct 
{	
	int    stn_id;
	UINT   stn_mask;
	int    dev_id;
	UINT   dev_mask;
	int    data_type;
	char   property[OPT_ITEM_NUM][256];
	char   opr[OPT_ITEM_NUM][3];
	union 
	{
		float fvalue[OPT_ITEM_NUM];
		int   ivalue[OPT_ITEM_NUM];       
		char  str[OPT_ITEM_NUM][MAX_SEARCH_STR_SIZE];
	}data;
	
	
} SEARCH_PT_INFO;

SEARCH_PT_INFO search_pt;
GtkWidget* SearchResultWin[MAX_CRT_NUMBER];
#endif