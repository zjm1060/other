/* 
 * File:   dyn_form_def_new.h
 * Author: chi.hailong
 *
 * Created on 2006年8月22日, 上午11:58
 * SYN by hcl 2007.12.11
 */

#ifndef _dyn_form_def_new_H
#define	_dyn_form_def_new_H

#ifdef	__cplusplus
extern "C" {
#endif


#define	TMPLT_DIALOG_NAME_LEN	32

#define	TMPLT_SCALAR_FORM	0
#define	TMPLT_ARRAY_FORM	1
#define	TMPLT_FORM_CNT		2


#define	REVERT_BTN	0
#define	VALIDATE_BTN	1
#define	ACCEPT_BTN	2
#define	CANCEL_BTN	3

 
int	dsp_fmt_type;
#define DF_DYN_COLOR    1
#define DF_BAR_CHART    2
#define DF_ANA_STATUS	3
#define DF_SBO_STATUS   4
#define DF_DIAL_METER   5
#define DF_GEN_ECON     6
#define DF_ENTRY_NAME   7
#define	DF_IMP_SET	8
#define	DF_HIST_BAR	9
#define DF_LINE_PLOT    10
#define DF_INTEGER	11
#define DF_NUMERIC      12
#define DF_PIE_CHART    13
#define DF_PQ_CHART	14
#define DF_RPT_DATE     15
#define DF_RPT_DB_TEXT  16
#define DF_RPT_NUMERIC  17
#define DF_RPT_STR_GRP  18
#define DF_RPT_TIME     19
#define DF_STEP_PLOT    20
#define DF_STRING_GRP   21
#define DF_POINT_NAME   22
#define DF_SYMBOL_GRP   23
#define DF_CUR_LOAD     24
#define DF_BAR_GRAPH    25
#define DF_PROG_REQ     26
#define DF_DSP_REQ      27
#define DF_MENU_REQ     28
#define DF_OPER_GUIDE_REQ       29
#define DF_TEMP_CHAR    31    /* added by chichi */
#define DF_CURVE_PT       32   /* added by chichi */
#define DF_CURVE_PT_INPUT       33   /* added by chichi */
#define DF_VIBRA_AREA       34   /* added by chichi */


#define	DF_SCENE_REDISP		35   //chichi
#define DF_REAL_CURVE		36		 //chichi
#define DF_MAN_SET_TIME 37     //hcl

  
 

#define CTRL_REQ_NULL           0
#define CTRL_REQ_DSP            1
#define CTRL_REQ_PROG           2
#define CTRL_REQ_MENU           3
#define	CTRL_REQ_SUB_DSP	4

#define DYN_CGRP_TAG_OFF        0
#define DYN_CGRP_TAG_ON         1

#define REQ_HOURLY              0
#define REQ_DAYLY               1
#define REQ_MONTHLY             2
#define REQ_YEARLY              3
 
/**  XtIntervalId  **/

#ifdef DSP_TIMEOUT_DEF
	gint	dsp_timer_id[MAX_CRT_NUMBER];
        int	ctrl_req_type;
#else
	extern	gint	dsp_timer_id[MAX_CRT_NUMBER]; 
        extern  int	ctrl_req_type;
#endif 
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* _dyn_form_def_new_H */

