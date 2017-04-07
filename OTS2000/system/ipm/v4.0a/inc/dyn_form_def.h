
/*
#define	TMPLT_DIALOG_NAME_LEN	32

#define	TMPLT_SCALAR_FORM	0
#define	TMPLT_ARRAY_FORM	1
#define	TMPLT_FORM_CNT		2


#define	REVERT_BTN	0
#define	VALIDATE_BTN	1
#define	ACCEPT_BTN	2
#define	CANCEL_BTN	3
*/
 

int	dyn_format;
int	dsp_fmt_type;		 	// no used
#define DF_REQ_NULL		0
#define DF_DYN_COLOR    1
#define DF_BAR_CHART    2
#define DF_ANA_STATUS	3
#define DF_SBO_STATUS   4
#define DF_DIAL_METER   5
#define DF_GEN_ECON     6
#define DF_ENTRY_NAME   7
#define DF_IMP_SET		8
#define DF_HIST_BAR		9
#define DF_LINE_PLOT    10
#define DF_INTERGER		11
#define DF_NUMERIC      12
#define DF_PIE_CHART    13
#define DF_PQ_CHART		14
#define DF_RPT_DATE     15
#define DF_CASCAD_CLR 	16
#define DF_RPT_NUMERIC  17
#define DF_RPT_STR_GRP  18
#define DF_RPT_TIME     19
#define DF_STEP_PLOT    20
#define DF_STRING_GRP   21
#define DF_POINT_NAME   22
#define DF_SYMBOL_GRP   23
#define DF_CUR_LOAD     24
#define DF_BAR_GRAPH    25
#define	DF_PROG_REQ		26
#define	DF_DSP_REQ		27
#define	DF_MENU_REQ		28
#define	DF_OPER_GUIDE_REQ  29
#define	DF_ADD_COLOR	30
#define	DF_CURVE_PT		31   //chichi
#define	DF_VIBRA_ARAEA		34   //chichi
#define	DF_SCENE_REDISP		35   //chichi
#define DF_REAL_CURVE		36		 //chichi
  
 
int	ctrl_req_type;
#define CTRL_REQ_NULL           0
#define CTRL_REQ_DSP            1
#define CTRL_REQ_PROG           2
#define CTRL_REQ_MENU           3
#define CTRL_REQ_OPER_GUIDE		4


#define NO_ADD_COLOR		-1
#define	HAVE_ADD_COLOR		1

#define DYN_CGRP_TAG_OFF        0
#define DYN_CGRP_TAG_ON         1

NEW_GBRCD	*ctrl_rcd;
NEW_GBRCD	*tag_rcd;

#define	MAX_ITEM_CNT	20	

