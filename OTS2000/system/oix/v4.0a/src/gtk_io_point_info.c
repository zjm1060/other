
/**************************************************
 2007.05.22 : have been SYNC  by chichi;
 LAST UPATE : 2007.06.18 by chichi ; 
***************************************************/

#include <stdio.h>


#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

#ifdef SUN_OS
#define VBOX_LAB_SPACING 13
#else 
#define VBOX_LAB_SPACING 15
#endif
 
#define	SEG_PER_PAGE	24
#define COL_PER_PAGE	3

#define IO_VALUE	1
#define OPEN_CLOSE	2
#define IMP_PARAM	3
#define LMT_VAL		4
#define CURVE_VAL	5
#define RT_VAL          6
#define MAN_SET_TIME_VAL 7

extern DB_TAB_WINDOW_INFO db_tab_window_info[];
extern void DestroyDetailInfoBox(GtkWidget *w,gpointer data);
extern int db_detail_timeout_id[MAX_CRT_NUMBER];
void DspInfo(int crt_n,GtkWidget *notebook);
int	SetPointNameLabel(POINTER pt, GtkWidget *label);

extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern int IO_val_set(int crt_n,char input[20],int item);
extern int IMP_param_set(int crt_n,char input[20],int item);
extern int LMT_val_set(int crt_n,char input[20],int item);
extern int OPEN_close_num_set(int crt_n,char input[20],int item);
extern int CURVE_val_set(int crt_n,char input[20],int seg_num);
extern int RT_val_set(int crt_n,char input[20],int item);
extern void GetAnaLmtStateStr(DMS_COMMON *db_cmm,char *value_str);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void 	CmdBufInit ( int crt_n );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern void GetYesNoStr(int value,char *str);
extern void	UnmanageDbIoBox ( GtkWidget *w  );
extern void	GetDbDetailFixedInfo(DMS_COMMON *db_cmm, int *item_num, char entry_str[][64], char value_str[][POINT_LONGNAME_SIZE] );
extern char *_toUtf8EX(char *c);
extern void FormCAnaForcePacket (int crt_n, DMS_COMMON *int_data, char	*oper_status );
extern void	ProgDoAckCel ( int crt_n );
extern PopupValueEntryBox (GtkWidget *parent ,int crt_n ,POINTER *pt,int entry_data_class);
extern char *_toUtf8(char *c);
extern GtkWidget *CreateManSetTimeInputDlg(GtkWidget *parent, int crt_n ,int inputParaflag,GBRCD *rcd,POINTER* point);
int UpdateDspInfo(GtkWidget *parent);
extern int OpenDspFile (int crt_n, char	*filename);
extern void	FormCEnterValueAdjItem (int crt_n,float ent_value,LIMIT_VAL *lmtVal, int useLmt);
extern void GetAnalogLimitValue (int lmt_type,DMS_COMMON dms_cmm, LIMIT_VAL *lmtVal);

static DMS_COMMON pt_dms_cmm[MAX_CRT_NUMBER];
static int ana_lmt_type[30],imp_param_type[30];
unsigned short opr_state[50],open_close_state[30];
extern gint num_screen;

POINTER		db_detail_pt[MAX_CRT_NUMBER];


GtkWidget *InputBox[MAX_CRT_NUMBER],*InputEntry[MAX_CRT_NUMBER];
static GtkWidget *CurveSeg_Entry[MAX_CRT_NUMBER][300],*ana_lmt_entry[MAX_CRT_NUMBER][30],
                *imp_param_entry[MAX_CRT_NUMBER][30],*open_close_num[MAX_CRT_NUMBER][30];

static GtkWidget*yes_btn[MAX_CRT_NUMBER][50],*IO_entry[MAX_CRT_NUMBER][50],*base_info_entry[MAX_CRT_NUMBER][30],
               *cmd_info_entry[MAX_CRT_NUMBER][30], *rt_val_entry[MAX_CRT_NUMBER][30],*rt_state_entry[MAX_CRT_NUMBER][30];
static GtkWidget* fb_status1_entry[MAX_CRT_NUMBER][30],*fb_status2_entry[MAX_CRT_NUMBER][30];

gchar *ANA_BASE_INFO_LABEL[] = {
    			OIX_STR_IO_ADDRESS,OIX_CSTR_NAME,OIX_CSTR_LONGNAME,OIX_STR_CTRL_ADDR,
    			OIX_STR_MSG_ALM_SEL, OIX_STR_MSG_CMD_SEL,OIX_STR_PAIR_ADDRESS,
    			OIX_CSTR_GROUP_NO, OIX_CSTR_TERMINAL,OIX_CSTR_UNIT_NO,
    			OIX_CSTR_SLOT,  OIX_CSTR_CHANNEL, OIX_CSTR_CABINET_NO,OIX_STR_POPUP_DSPFILE
    		     };
int ANA_BASE_COL_NUM =  sizeof(ANA_BASE_INFO_LABEL)/sizeof(ANA_BASE_INFO_LABEL[0]);   	/*13*/	      
    		      
gchar *ANA_IOST_LABEL[] = {
    			OIX_STR_VIEW_POINT,OIX_CSTR_ALM_REPEAT,OIX_CSTR_NO_UPDATE,OIX_CSTR_ScanEnable,
    			OIX_CSTR_AlarmEnable, OIX_CSTR_AlarmNoAck,OIX_STR_CTRL_LOCK,
    			OIX_STR_TAG_STATE,OIX_STR_HDB_IO_GROUP
    		     };   
int ANA_IOST_COL_NUM =  sizeof(ANA_IOST_LABEL)/sizeof(ANA_IOST_LABEL[0]);  /*8*/
    		     
gchar *ANA_IOATTR_LABEL[] = {
    			OIX_STR_PROC_TYPE,OIX_STR_ALM_LOCK,OIX_STR_STATISTIC,OIX_STR_FAULT,
    			OIX_STR_FAIL, OIX_STR_CTRL,OIX_STR_SOE,OIX_STR_TREND_ALM,OIX_STR_SIGNAL,
    			OIX_STR_FAULT_REC,OIX_STR_AUD_ALM,OIX_STR_NORMAL_CLOSE,OIX_STR_DOUBLE_PT,
    			OIX_STR_SYS_VIRTUAL,OIX_STR_LCU_VIRTUAL,OIX_STR_CASCADE,OIX_STR_HDB_REC,
    			OIX_STR_HDB_CURVE,/*OIX_STR_HDB_IO_GROUP,*/OIX_CSTR_HDB_PERIOD,OIX_CSTR_ON_ALM,
                OIX_CSTR_OFF_ALM,OIX_CSTR_NORM_STAT
    		     }; 
int ANA_IOATTR_COL_NUM =  sizeof(ANA_IOATTR_LABEL)/sizeof(ANA_IOATTR_LABEL[0]);   /*19*/   		   
    		   
gchar *ANA_RTVALUE_LABEL[] = {
    			OIX_CSTR_RT_VALUE,OIX_STR_UPDATE_TIME,OIX_CSTR_CHANNEL_QULITY,OIX_CSTR_BLOCK,OIX_CSTR_OVERFLOW,
                        OIX_STR_LMT_STATUS,OIX_STR_TRLMT_STATUS, OIX_STR_ANA_UNITNAME
    		     }; 
int ANA_RTVALUE_COL_NUM =  sizeof(ANA_RTVALUE_LABEL)/sizeof(ANA_RTVALUE_LABEL[0]);      /*6*/   		         		   		     

gchar *ANA_LMT_LABEL[] = {
    			OIX_STR_H_ALM_LMT,OIX_STR_H_WRN_LMT,OIX_STR_H_RECOVER,OIX_STR_L_RECOVER,OIX_STR_L_WRN_LMT,
    			OIX_STR_L_ALM_LMT, OIX_CSTR_TREND_LMT,OIX_CSTR_TREND_RESET_LMT,OIX_STR_DBAND,
    			OIX_STR_ENG_FACTOR,OIX_STR_HI_RANGE,OIX_STR_LO_RANGE,OIX_STR_BASE,
    			OIX_STR_COMPENSATE,OIX_STR_H_CODE,OIX_STR_L_CODE
    		     }; 
int ANA_LMT_COL_NUM =  sizeof(ANA_LMT_LABEL)/sizeof(ANA_LMT_LABEL[0]); /*15*/ 
    
gchar *POL_RTSTATE_LABEL[] = {
    			OIX_CSTR_RT_STATE,OIX_STR_DOUBLE_STATE,OIX_STR_UPDATE_TIME,OIX_STR_CHANGE_TIME,
    			OIX_STR_FAULT_FLAG, OIX_CSTR_CHANNEL_QULITY,OIX_CSTR_MAN_SET,OIX_STR_STC_NORM_OPEN,OIX_STR_STC_NORM_CLOSE,
    			OIX_STR_STC_FAULT_OPEN,OIX_STR_STC_FAULT_CLOSE
    		     }; 
int POL_RTSTATE_COL_NUM =  sizeof(POL_RTSTATE_LABEL)/sizeof(POL_RTSTATE_LABEL[0]); /*11*/

gchar *IMP_RTVALUE_LABEL[] = {
    			OIX_STR_REAL_ENERGY,OIX_STR_BASE_ENERGY,OIX_STR_UPDATE_TIME,OIX_STR_DATA_QUALITY,
    			OIX_STR_IMP_CONSTANT,OIX_STR_ACCUM,OIX_STR_IMP_INIT_YEAR,
    			OIX_STR_IMP_INIT_MONTH,OIX_STR_IMP_INIT_DAY,
                        OIX_STR_IMP_PT,OIX_STR_IMP_CT,
                        OIX_STR_ENG_FACTOR,
						OIX_STR_IMP_DAY_RATE,
						OIX_STR_IMP_YEAR_RATE
    		     }; 
int IMP_RTVALUE_COL_NUM =  sizeof(IMP_RTVALUE_LABEL)/sizeof(IMP_RTVALUE_LABEL[0]);      /*8*/   

gchar *IMP_PARA_LABEL[] = {OIX_STR_DAILY_PEAK_ENERGY,
						OIX_STR_MONTH_PEAK_ENERGY,OIX_STR_YEAR_PEAK_ENERGY,OIX_STR_DAILY_VALLEY_ENERGY,
						OIX_STR_MONTH_VALLEY_ENERGY,OIX_STR_YEAR_VALLEY_ENERGY,OIX_STR_DAILY_MEAN_ENERGY,
						OIX_STR_MONTH_MEAN_ENERGY,OIX_STR_YEAR_MEAN_ENERGY,OIX_STR_DAILY_TTL_ENERGY,
						OIX_STR_MONTH_TTL_ENERGY,OIX_STR_YEAR_TTL_ENERGY,OIX_STR_DAILY_PPEAK_ENERGY,
    						OIX_STR_MONTH_PPEAK_ENERGY,   			
    						OIX_STR_YEAR_PPEAK_ENERGY    			
    		     }; 
int IMP_PARA_COL_NUM =  sizeof(IMP_PARA_LABEL)/sizeof(IMP_PARA_LABEL[0]);      /*17*/  

gchar *CHARA_PARA_LABEL[] = {
    			OIX_STR_UPDATE_TIME,OIX_STR_START_POINTNAME,OIX_STR_POINT_NUM,OIX_STR_MAX_POINT,
    			OIX_STR_MIN_POINT,OIX_STR_MAX_RATE_POINT,OIX_STR_MIN_RATE_POINT,OIX_STR_MAX,
    			OIX_STR_MIN,OIX_STR_MEAN,OIX_STR_VARIANCE,OIX_STR_MAX_RATE,OIX_STR_MIN_RATE
    			
    		     }; 
int CHARA_PARA_COL_NUM =  sizeof(CHARA_PARA_LABEL)/sizeof(CHARA_PARA_LABEL[0]);      /*13*/ 

gchar *CURVE_PARA_LABEL[] = {
    			OIX_STR_IO_ADDRESS,OIX_CSTR_NAME,OIX_CSTR_LONGNAME,OIX_STR_UPDATE_TIME,OIX_STR_SEGMENT_NUM,
    			OIX_STR_HI_RANGE,OIX_STR_LO_RANGE    			
    		     }; 
int CURVE_PARA_COL_NUM =  sizeof(CURVE_PARA_LABEL)/sizeof(CURVE_PARA_LABEL[0]);      /*13*/
  
gchar *TIME_VALUE_LABEL[] = {
	OIX_CSTR_SET_TIME_VALUE,OIX_STR_UPDATE_TIME,"X","Y","Z",OIX_CSTR_USR_NAME,OIX_CSTR_HOST_ID,OIX_CSTR_OPERATE_STATE,
	OIX_CSTR_TYPE_ID,OIX_CSTR_RETURN_STATE, OIX_CSTR_SOURCE_ID
}; 

int TIME_VALUE_COL_NUM =  sizeof(TIME_VALUE_LABEL)/sizeof(TIME_VALUE_LABEL[0]);      /*6*/   		         		   		     


typedef struct 
{
	int id;
	int item;
}ITEM;

extern GtkWidget *man_set_time_dlg[MAX_CRT_NUMBER];

void	GetTimetoStr(DATE_TIME *time, char *str)
{
	sprintf(str, "%4d-%02d-%02d %02d:%02d:%02d:%03d", time->year, time->month, time->day,
		time->hour, time->minute, time->second, time->ms);
}
void DestroyInputBox(GtkWidget *w,gpointer data)
{
    int crt_n =(int)data ;
    if(InputBox[crt_n]!=NULL)
		gtk_widget_destroy(InputBox[crt_n]);
	InputBox[crt_n] = NULL;
}
void DestroyInputBoxOK(GtkWidget *w,gpointer data)
{
	char InputVal[20];
	int item , id; 
	ITEM *item_id ;
	int crt_n=0;
      
	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return ;
        
	item_id=(ITEM*)data;
	
	item= item_id->item;
	id = item_id->id;
	if (InputBox[crt_n] ==NULL) return ;
	strcpy(InputVal,gtk_entry_get_text(GTK_ENTRY(InputEntry[crt_n])));
	
	if(id==IO_VALUE)
		IO_val_set( crt_n,InputVal,item);
	if(id==IMP_PARAM)
		IMP_param_set(crt_n,InputVal,item);
	if(id==LMT_VAL)
		LMT_val_set( crt_n,InputVal,item);
	if(id==OPEN_CLOSE)
		OPEN_close_num_set(crt_n,InputVal,item);
	if(id==CURVE_VAL)
		CURVE_val_set( crt_n,InputVal,item);
        if(id==RT_VAL)
		RT_val_set( crt_n,InputVal,item);
        DestroyInputBox( w, GINT_TO_POINTER (crt_n));
	
}

GtkWidget* CreateValInputBox(int crt_n,char old_val[20],int id,int item)
{
	GtkWidget *hbox,*vbox,*button,*label;	
	static ITEM val_item;
	char *text ; 
	val_item.id = id;
	val_item.item=item;	
	
	InputBox[crt_n] = gtk_dialog_new();
        
	gtk_widget_set_usize( GTK_WIDGET (InputBox[crt_n]), 300, 150);
	if(db_detail_info[crt_n]!=NULL)	
		gtk_window_set_transient_for(GTK_WINDOW(InputBox[crt_n]),GTK_WINDOW(db_detail_info[crt_n]) );
	gtk_window_set_title(GTK_WINDOW(InputBox[crt_n]),text = _toUtf8EX( OIX_CSTR_DATA_INPUT));
	G_SAFE_FREE(text);//dzy1017
        
        hbox=gtk_hbox_new(FALSE, 15);
    	gtk_widget_show(hbox);    	
    	
 	vbox=gtk_vbox_new(FALSE, 5);
    	gtk_widget_show(vbox);
  	label = gtk_label_new(text = _toUtf8EX( OIX_CSTR_DATA_INPUT ));
	G_SAFE_FREE(text);//dzy1017
    	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);  	
    	InputEntry[crt_n] =gtk_entry_new();    	

		gtk_entry_set_text(GTK_ENTRY(InputEntry[crt_n]),old_val);
    	gtk_widget_set_usize(GTK_WIDGET(InputEntry[crt_n]), 90, -1);
    		
    	gtk_widget_show(InputEntry[crt_n]); 
    	gtk_box_pack_start(GTK_BOX(hbox),InputEntry[crt_n],TRUE,TRUE,0);
    	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(InputBox[crt_n])->vbox), hbox, TRUE, TRUE, 0); 
    	  
    	button = gtk_button_new_with_label (text = _toUtf8EX( OIX_CSTR_OK));
		G_SAFE_FREE(text);//dzy1017
    	gtk_widget_set_usize( GTK_WIDGET (button), 50, 30);
    	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(InputBox[crt_n])->action_area), button, TRUE, TRUE, 0);
        gtk_signal_connect(GTK_OBJECT (InputBox[crt_n]), "destroy",
			       GTK_SIGNAL_FUNC(DestroyInputBox),
			       GINT_TO_POINTER(crt_n));
    	gtk_signal_connect(GTK_OBJECT (button), "clicked",
			       GTK_SIGNAL_FUNC(DestroyInputBoxOK),
			       &val_item);
   
    	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    	gtk_widget_grab_default (button);
    	gtk_widget_show_all(InputBox[crt_n]);
    	return InputBox[crt_n];
} 
 

GtkWidget* CreateManSetTimeBox(int crt_n,char old_val[20],int id,int item)
{
	GtkWidget *hbox,*vbox,*button,*label;	
	static ITEM val_item;
	char *text ; 
	val_item.id = id;
	val_item.item=item;	
	
	InputBox[crt_n] = gtk_dialog_new();
	
	gtk_widget_set_usize( GTK_WIDGET (InputBox[crt_n]), 300, 150);
	if(db_detail_info[crt_n]!=NULL)	
		gtk_window_set_transient_for(GTK_WINDOW(InputBox[crt_n]),GTK_WINDOW(db_detail_info[crt_n]) );
	gtk_window_set_title(GTK_WINDOW(InputBox[crt_n]),text = _toUtf8EX( OIX_CSTR_DATA_INPUT));
	G_SAFE_FREE(text);//dzy1017
	
	hbox=gtk_hbox_new(FALSE, 15);
	gtk_widget_show(hbox);    	
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_widget_show(vbox);
	label = gtk_label_new(text = _toUtf8EX( OIX_CSTR_DATA_INPUT ));
	G_SAFE_FREE(text);//dzy1017
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);  	
	InputEntry[crt_n] =gtk_entry_new();    	
	
	gtk_entry_set_text(GTK_ENTRY(InputEntry[crt_n]),old_val);
	gtk_widget_set_usize(GTK_WIDGET(InputEntry[crt_n]), 90, -1);
	
	gtk_widget_show(InputEntry[crt_n]); 
	gtk_box_pack_start(GTK_BOX(hbox),InputEntry[crt_n],TRUE,TRUE,0);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(InputBox[crt_n])->vbox), hbox, TRUE, TRUE, 0); 
	
	button = gtk_button_new_with_label (text = _toUtf8EX( OIX_CSTR_OK));
	G_SAFE_FREE(text);//dzy1017
	gtk_widget_set_usize( GTK_WIDGET (button), 50, 30);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(InputBox[crt_n])->action_area), button, TRUE, TRUE, 0);
	gtk_signal_connect(GTK_OBJECT (InputBox[crt_n]), "destroy",
		GTK_SIGNAL_FUNC(DestroyInputBox),
		GINT_TO_POINTER(crt_n));
	gtk_signal_connect(GTK_OBJECT (button), "clicked",
		GTK_SIGNAL_FUNC(DestroyInputBoxOK),
		&val_item);
	
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default (button);
	gtk_widget_show_all(InputBox[crt_n]);
	return InputBox[crt_n];
} 

void GetAnaRtVal(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE])
{
	int		item=0;	
	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.fValue); 
	item++;
	
	sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.anlg.update_time.year,db_cmm->var.anlg.update_time.month,db_cmm->var.anlg.update_time.day,db_cmm->var.anlg.update_time.hour,
			db_cmm->var.anlg.update_time.minute,db_cmm->var.anlg.update_time.second,db_cmm->var.anlg.update_time.ms );
	item++;	
	
	if(db_cmm->var.anlg.value.status.chan_fail)
		strcpy(value_str[item],DPS_STR_FAIL);
        else
		strcpy(value_str[item],DPS_STR_NORMAL);
        item++;
        
	if(db_cmm->var.anlg.value.status.ctrl_block)
		strcpy(value_str[item],DPS_STR_CTRL_BLOCK);
	else
		strcpy(value_str[item],DPS_STR_NORMAL);
        item++;
        
        if(db_cmm->var.anlg.value.status.overflow)
		strcpy(value_str[item],DPS_STR_OVERFLOW);
	else
		strcpy(value_str[item],DPS_STR_NORMAL);
	item++;
	
	GetAnaLmtStateStr(db_cmm, value_str[item]);
	item++;
	
	if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_NORMAL)
		strcpy(value_str[item],DPS_STR_NORMAL);
	else if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_TR_WARN)
		strcpy(value_str[item],DPS_STR_H_WRN_LMT);
	else if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_TR_RECOVER)
		strcpy(value_str[item],DPS_STR_H_RECOVER);
	item++;
	
	sprintf(value_str[item], "%s", db_cmm->var.anlg.unitName );
	item++;
} 

void GetAnaLmtVal(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE])
{
	int		item=0;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.HI_ALARM );
	ana_lmt_type[item]= ANA_PARAM_SET_HI_ALARM;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.HI_WARN );
	ana_lmt_type[item]= ANA_PARAM_SET_HI_WARN;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.HI_RECOVER );
	ana_lmt_type[item]=ANA_PARAM_SET_HI_RECOVER; 
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.LO_RECOVER ); 
	ana_lmt_type[item]=ANA_PARAM_SET_LO_RECOVER;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.LO_WARN ); 
	ana_lmt_type[item]=ANA_PARAM_SET_LO_WARN;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.LO_ALARM ); 
	ana_lmt_type[item]=ANA_PARAM_SET_LO_ALARM;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.TR_WARN );
	ana_lmt_type[item]=ANA_PARAM_SET_TR_WARN;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.TR_RECOVER );
	ana_lmt_type[item]=ANA_PARAM_SET_TR_RECOVER;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.D_BAND );
	ana_lmt_type[item]=ANA_PARAM_SET_D_BAND;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.ENG_CVT );
	ana_lmt_type[item]=ANA_PARAM_SET_ENG_CVT;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.HI_RANGE );
	ana_lmt_type[item]=ANA_PARAM_SET_HI_RANGE;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.LO_RANGE );
	ana_lmt_type[item]=ANA_PARAM_SET_LO_RANGE;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.ZERO );
	ana_lmt_type[item]=ANA_PARAM_SET_ZERO;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.COMPSATE );
	ana_lmt_type[item]=ANA_PARAM_SET_COMPSATE;
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.HI_CODE );	
	item++;		
	sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.LO_CODE );
}

gint ModifyLmtVal(GtkWidget *btn, gpointer data)
{
	char old_val_str[20];
	int  item;
	int crt_n=0;
	LCU_ID	lcu_id;

	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
        
	if(!CheckUserPrivilege(cur_usr_name, PRIV_LIMIT_PARA_SET))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_LMT_PARAM_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= pt_dms_cmm[crt_n].point.stn_id;
	lcu_id.dev_id= pt_dms_cmm[crt_n].point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}

	item=(int)data;	
	
	strcpy(old_val_str,gtk_entry_get_text(GTK_ENTRY(ana_lmt_entry[crt_n][item])));
	
	CreateValInputBox(crt_n,old_val_str,LMT_VAL,item);

	return FALSE;/*hcl 070918*/
}

int LMT_val_set(int crt_n,char input[20],int item)
{	
    float new_val;
	static LIMIT_VAL  _lmtVal  ;
	DMS_COMMON	dms_cmm;
	
	
	
	if(ReadEntryById ( &pt_dms_cmm[crt_n].point, &dms_cmm )==-1)
	{	printf("no this entry \n");
		return FALSE;
	}
	
	sscanf(input,"%f",&new_val);
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= pt_dms_cmm[crt_n].point.stn_id;
	cmd_buf[crt_n].dev_id		= pt_dms_cmm[crt_n].point.dev_id;
	cmd_buf[crt_n].point_id		= pt_dms_cmm[crt_n].point.pt_id;
	cmd_buf[crt_n].data_type 	= pt_dms_cmm[crt_n].point.data_type;  
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].type_id		= DPS_ANA_PARAM_SET;
	cmd_buf[crt_n].status.opr	= ana_lmt_type[item];
	cmd_buf[crt_n].data.fValue 	= new_val;

	GetAnalogLimitValue ( cmd_buf[crt_n].status.opr, dms_cmm, &_lmtVal/*->cur_val */ );
	FormCEnterValueAdjItem (crt_n,  new_val,&_lmtVal,TRUE);
//	ProgDoAckCelBrdcst ( crt_n );
	return TRUE;
}

gint ModifyRtValue(GtkWidget *btn, gpointer data)
{
	char old_val_str[20];
	int  item ;
	int crt_n=0;
	LCU_ID	lcu_id;
	
	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
        
	if(!CheckUserPrivilege(cur_usr_name, PRIV_RTVALUE_MAN_SET))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_MANUAL_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= pt_dms_cmm[crt_n].point.stn_id;
	lcu_id.dev_id= pt_dms_cmm[crt_n].point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}
	item=(int)data;	
	
	strcpy(old_val_str,gtk_entry_get_text(GTK_ENTRY(rt_val_entry[crt_n][item])));
	
	CreateValInputBox(crt_n,old_val_str,RT_VAL,item);
	return TRUE;
}

int RT_val_set(int crt_n,char input[20],int item)
{	
    float new_val;

	sscanf(input,"%f",&new_val);

	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= pt_dms_cmm[crt_n].point.stn_id;
	cmd_buf[crt_n].dev_id		= pt_dms_cmm[crt_n].point.dev_id;
	cmd_buf[crt_n].point_id		= pt_dms_cmm[crt_n].point.pt_id;
	cmd_buf[crt_n].data_type 	= pt_dms_cmm[crt_n].point.data_type;  
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].type_id		= DPS_MAN_VALUE_SET;
	cmd_buf[crt_n].status.opr	= 0;
	cmd_buf[crt_n].data.iValue 	= (int)(new_val*1000);
	ProgDoAckCelBrdcst ( crt_n );
	return TRUE;
}


gint gotoDSP(GtkWidget *btn, gpointer data)
{
	int crt_n=0;
	char dspname[200];
	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
	
	//sprintf(dspname,"%s%s",FILE_DSP_PATH,pt_dms_cmm[crt_n].fixed.dsp_filename);

	sprintf(dspname,"%s",pt_dms_cmm[crt_n].fixed.dsp_filename);

    if (num_screen>1);
    {
		crt_n=num_screen-1;

    }
return	OpenDspFile ( crt_n, dspname);


}
gint ModifyImpParam(GtkWidget *btn, gpointer data)
{
	char old_val_str[20];
	int  item;
	int crt_n=0;
	LCU_ID	lcu_id;
	
	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
	if(!CheckUserPrivilege(cur_usr_name, PRIV_IMP_MAN_SET))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_IMP_MANUAL_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= pt_dms_cmm[crt_n].point.stn_id;
	lcu_id.dev_id= pt_dms_cmm[crt_n].point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}

	item=(int)data;
	if(item>=IMP_RTVALUE_COL_NUM)
            strcpy(old_val_str,gtk_entry_get_text(GTK_ENTRY(imp_param_entry[crt_n][item])));
        else
            strcpy(old_val_str,gtk_entry_get_text(GTK_ENTRY(rt_val_entry[crt_n][item])));
	
	CreateValInputBox(crt_n,old_val_str,IMP_PARAM,item);

	return FALSE;/*hcl 070918*/
}
	
int IMP_param_set(int crt_n,char input[20],int item)
{
	float eng_cvt;
	int other_param;	
	
	if(imp_param_type[item]==IMP_PARAM_SET_CONSTANT
		||imp_param_type[item]==IMP_PARAM_SET_BASE_ACCUM
		||imp_param_type[item]==IMP_PARAM_SET_IMP_ACCUM)           
		sscanf(input,"%d",&other_param); 
	else
		sscanf(input,"%f",&eng_cvt);
	
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= pt_dms_cmm[crt_n].point.stn_id;
	cmd_buf[crt_n].dev_id		= pt_dms_cmm[crt_n].point.dev_id;
	cmd_buf[crt_n].point_id         = pt_dms_cmm[crt_n].point.pt_id;
	cmd_buf[crt_n].data_type 	= pt_dms_cmm[crt_n].point.data_type;  
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].type_id		= DPS_IMP_PARAM_SET;
	cmd_buf[crt_n].status.opr	= imp_param_type[item];
	if(imp_param_type[item]==IMP_PARAM_SET_CONSTANT
		||imp_param_type[item]==IMP_PARAM_SET_BASE_ACCUM
		||imp_param_type[item]==IMP_PARAM_SET_IMP_ACCUM	)
		cmd_buf[crt_n].data.iValue 	= other_param;
	else
		cmd_buf[crt_n].data.fValue = eng_cvt;
	ProgDoAckCelBrdcst ( crt_n );
	return TRUE;
}

gint ModifyOpenCloseNum(GtkWidget *btn, gpointer data)
{
	char old_val_str[20];
	int  item;
	int crt_n=0;
	LCU_ID	lcu_id;
	
	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
	if(!CheckUserPrivilege(cur_usr_name, PRIV_LIMIT_PARA_SET))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_LMT_PARAM_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= pt_dms_cmm[crt_n].point.stn_id;
	lcu_id.dev_id= pt_dms_cmm[crt_n].point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}

	item=(int)data;

	strcpy(old_val_str,gtk_entry_get_text(GTK_ENTRY(open_close_num[crt_n][item])));
	CreateValInputBox(crt_n,old_val_str,OPEN_CLOSE,item);
	return TRUE;
}

gint AnaForce(GtkWidget *btn, gpointer data)
{
	int  item;
	int crt_n=0;
  //  LCU_ID	lcu_id;
    
	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
	item=(int)data;
	
	if(!CheckUserPrivilege(cur_usr_name, PRIV_IN_FORCE_SET))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_IN_FORCE_SETTING );
		return FALSE;
	}

	if(item==0)
	{
			   FormCAnaForcePacket ( crt_n, &pt_dms_cmm[crt_n], "UnForce" );
			   ProgDoAckCel ( crt_n ); 
	}
	else if(item==1)
	{
			   FormCAnaForcePacket ( crt_n, &pt_dms_cmm[crt_n], "Force" );
			   PopupValueEntryBox (main_w[crt_n], crt_n , &pt_dms_cmm[crt_n].point,-1);
	}
	return TRUE;
}



gint ModifyRtState(GtkWidget *btn, gpointer data)
{
	int  item;
	int crt_n=0;
    LCU_ID	lcu_id;
    
	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
	item=(int)data;
	
	if(!CheckUserPrivilege(cur_usr_name, PRIV_RTVALUE_MAN_SET)&&(item<2))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_MANUAL_SETTING );
		return FALSE;
	}
	if(!CheckUserPrivilege(cur_usr_name, PRIV_OUT_FORCE_SET)&&(item>1))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_IN_FORCE_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= pt_dms_cmm[crt_n].point.stn_id;
	lcu_id.dev_id= pt_dms_cmm[crt_n].point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}

	

    CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id = pt_dms_cmm[crt_n].point.stn_id;
	cmd_buf[crt_n].dev_id = pt_dms_cmm[crt_n].point.dev_id;
                                
	

	cmd_buf[crt_n].data_type	= pt_dms_cmm[crt_n].point.data_type;
	cmd_buf[crt_n].point_id = pt_dms_cmm[crt_n].point.pt_id;
	if( item <2 )
	{
		cmd_buf[crt_n].type_id = DPS_MAN_STAT_SET;
		if(item==1)
			cmd_buf[crt_n].status.opr =  MAN_SET_CLOSE;
		else if(item==0)
			cmd_buf[crt_n].status.opr =  MAN_SET_OPEN;
		ProgDoAckCelBrdcst ( crt_n );	
	}	
	else
	{
		if( item == 2)
		{
			cmd_buf[crt_n].type_id		= C_ONOFF_FORCE;
			cmd_buf[crt_n].status.opr =  FORCEON;		
		}
		else if ( item == 3)
		{
			cmd_buf[crt_n].type_id		= C_ONOFF_FORCE;
			cmd_buf[crt_n].status.opr =  FORCEOFF;		
		}
		else if ( item == 4)
		{
			cmd_buf[crt_n].type_id		= C_ONOFF_FORCE;
			cmd_buf[crt_n].status.opr =  UNFORCE;		
		}
		ProgDoAckCel(crt_n);		
	}
	
	return FALSE;
}
void CreateSetManTimeBox (GtkWidget *w , int crt_n , int item)
{
	
	if (man_set_time_dlg[crt_n ]== NULL) 
	{
		man_set_time_dlg[crt_n]=CreateManSetTimeInputDlg(w, crt_n,1,NULL,&pt_dms_cmm[crt_n].point);
		gtk_widget_show( man_set_time_dlg[crt_n ]);
	}else
	{
		gtk_window_present(GTK_WINDOW(man_set_time_dlg[crt_n ]));
    }
}
gint ModifyManSetTime(GtkWidget *btn, gpointer data)
{
	int  item;
	int crt_n=0;

	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
	item=(int)data;

	if(!CheckUserPrivilege(cur_usr_name, PRIV_RTVALUE_MAN_SET)&&(item<2))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_MANUAL_SETTING );
		return FALSE;
	}
	if(!CheckUserPrivilege(cur_usr_name, PRIV_OUT_FORCE_SET)&&(item>1))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_IN_FORCE_SETTING );
		return FALSE;
	}
	

	item=(int)data;	


	CreateSetManTimeBox (main_w[crt_n] , crt_n , item);
return TRUE;
}


int OPEN_close_num_set(int crt_n,char input[20],int item)
{
	int new_num;
	sscanf(input,"%d",&new_num);
	
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= pt_dms_cmm[crt_n].point.stn_id;
	cmd_buf[crt_n].dev_id		= pt_dms_cmm[crt_n].point.dev_id;
	cmd_buf[crt_n].point_id		= pt_dms_cmm[crt_n].point.pt_id;
	cmd_buf[crt_n].data_type 	= pt_dms_cmm[crt_n].point.data_type;  
	cmd_buf[crt_n].ctrl_addr	= 0;
	cmd_buf[crt_n].type_id		= DPS_C_ONOFF_SET;
	cmd_buf[crt_n].status.opr	= open_close_state[item];
	cmd_buf[crt_n].data.iValue 	= new_num;
	ProgDoAckCelBrdcst ( crt_n );
	return TRUE;
}

gint YesBtnPressed(GtkWidget *btn, gpointer data)
{		
	int crt_n=0;
	int  item,PrivilgeType;
	short	state;
	LCU_ID	lcu_id;
	char msg[100];
    char *text ; 
	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
	item=(int)data;
	

    if (opr_state[item]==IOST_SET_SCAN_STATE || opr_state[item]==IOST_SET_ALM_STATE) /*item ==17 || item ==18 报警扫查允许*/
	{
		strcpy(msg,MSG_SCAN_ALM_ENABLE);
		PrivilgeType = PRIV_SCAN_ALM_SET ;
	}
    else
	{
		PrivilgeType = PRIV_IO_ATTR_SET ;
		strcpy(msg,MSG_IO_ATTR_SETTING) ;
	}
	 
	if(!CheckUserPrivilege(cur_usr_name, PrivilgeType))
	{
		PopupMessageBox ( db_detail_info[crt_n], msg );
		return FALSE;
	}
 
	lcu_id.stn_id= pt_dms_cmm[crt_n].point.stn_id;
	lcu_id.dev_id= pt_dms_cmm[crt_n].point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}
	
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn))==TRUE)
	{	
                if( opr_state[item]==IOATTR_SET_NORM_STATE)
                    gtk_object_set(GTK_OBJECT(btn),"GtkButton::label",text  = _toUtf8EX(OIX_CSTR_0),NULL);
                else
                    gtk_object_set(GTK_OBJECT(btn),"GtkButton::label",text = _toUtf8EX(OIX_CSTR_NO),NULL);
				G_SAFE_FREE(text);//dzy1017
		state = FALSE;		
	}
	else
	{		
                if( opr_state[item]==IOATTR_SET_NORM_STATE)
                    gtk_object_set(GTK_OBJECT(btn),"GtkButton::label",text = _toUtf8EX(OIX_CSTR_1),NULL);
                else	
                    gtk_object_set(GTK_OBJECT(btn),"GtkButton::label",text = _toUtf8EX(OIX_CSTR_YES),NULL);
				G_SAFE_FREE(text);//dzy1017
		
		state = TRUE;	
	}
	
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= pt_dms_cmm[crt_n].point.stn_id;
	cmd_buf[crt_n].dev_id		= pt_dms_cmm[crt_n].point.dev_id;
	cmd_buf[crt_n].point_id		= pt_dms_cmm[crt_n].point.pt_id;
	cmd_buf[crt_n].data_type 	= pt_dms_cmm[crt_n].point.data_type;  
	cmd_buf[crt_n].ctrl_addr	= 0;
	if(item>=ANA_BASE_COL_NUM+ANA_IOST_COL_NUM)
		cmd_buf[crt_n].type_id		= DPS_IOATTR_SET;
	else
		cmd_buf[crt_n].type_id		=DPS_IOST_SET;  
	cmd_buf[crt_n].status.opr	= opr_state[item];
	cmd_buf[crt_n].status.state	= state;
	cmd_buf[crt_n].data.iValue 	= 0;
	if(cmd_buf[crt_n].type_id==DPS_IOST_SET &&cmd_buf[crt_n].status.opr==IOST_SET_ALM_NOACK )
	{
		PopupMessageBox(db_detail_info[crt_n],MSG_PROHIBIT_ACK);
		return TRUE;
	}
	ProgDoAckCelBrdcst ( crt_n );
	
	return TRUE;
}


gint ModifyIOBtnPressed(GtkWidget *btn, gpointer data)
{
	char old_val_str[20];
	int crt_n=0;
	int  item,privilegeType;
	char msg[100] ;
	LCU_ID	lcu_id;
    item=(int)data;

	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
    if (opr_state[item]==IOST_SET_TAG_STATE)  /*item ==21挂牌操作*/
	{
		privilegeType = PRIV_TAG_OPR ; 
		strcpy(msg,MSG_TAG_OPR_SETTING);
	}
	else
	{
		privilegeType = PRIV_IO_ATTR_SET ;
		strcpy(msg,MSG_IO_ATTR_SETTING);

	}

	if(!CheckUserPrivilege(cur_usr_name, privilegeType))
	{
		PopupMessageBox ( db_detail_info[crt_n], msg );
		return FALSE;
	}
	lcu_id.stn_id= pt_dms_cmm[crt_n].point.stn_id;
	lcu_id.dev_id= pt_dms_cmm[crt_n].point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}


	
	strcpy(old_val_str,gtk_entry_get_text(GTK_ENTRY(IO_entry[crt_n][item])));
	CreateValInputBox(crt_n,old_val_str,IO_VALUE,item);	
	
	return FALSE;/*hcl 070918*/
}

int IO_val_set(int crt_n,char input[20],int item)
{
	int new_val;
	
		sscanf(input,"%d",&new_val);
		CmdBufInit ( crt_n );
		cmd_buf[crt_n].stn_id		= pt_dms_cmm[crt_n].point.stn_id;
		cmd_buf[crt_n].dev_id		= pt_dms_cmm[crt_n].point.dev_id;
		cmd_buf[crt_n].point_id		= pt_dms_cmm[crt_n].point.pt_id;
		cmd_buf[crt_n].data_type 	= pt_dms_cmm[crt_n].point.data_type;  
		cmd_buf[crt_n].ctrl_addr	= 0;
		if(item>=ANA_BASE_COL_NUM+ANA_IOST_COL_NUM)
		{
			cmd_buf[crt_n].type_id		= DPS_IOATTR_SET;
			cmd_buf[crt_n].data.iValue 	= new_val;
			cmd_buf[crt_n].status.state	= 0;
		}		
                else
		{
                    if(item!=ANA_BASE_COL_NUM+ANA_IOST_COL_NUM-1)
                        {
			cmd_buf[crt_n].type_id		=DPS_IOST_SET;
			cmd_buf[crt_n].data.iValue 	= 0;
			cmd_buf[crt_n].status.state	= new_val;
                    }
                    else{
			cmd_buf[crt_n].type_id		=DPS_IO_GROUP_SET;
			cmd_buf[crt_n].data.iValue 	= new_val;
			cmd_buf[crt_n].status.state	= 0;
                    }
		}
		cmd_buf[crt_n].status.opr	= opr_state[item];	
	
		ProgDoAckCelBrdcst ( crt_n );
		
	
	return TRUE;
}

gint InputCurveSeg(GtkWidget *btn, gpointer data)
{
	char old_val_str[20];	
	LCU_ID	lcu_id;
	
	int  seg_num=(int)data;
	int crt_n=0;
	
	GetCrtNumberOfWgt ( btn, &crt_n );
	if( crt_n < 0 )
		return FALSE;
	if(!CheckUserPrivilege(cur_usr_name, PRIV_RTVALUE_MAN_SET))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_MANUAL_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= pt_dms_cmm[crt_n].point.stn_id;
	lcu_id.dev_id= pt_dms_cmm[crt_n].point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}
	strcpy(old_val_str,gtk_entry_get_text(GTK_ENTRY(CurveSeg_Entry[crt_n][seg_num])));
	CreateValInputBox(crt_n,old_val_str,CURVE_VAL,seg_num);	

	return FALSE;/*hcl 070918*/
}

int CURVE_val_set(int crt_n,char input[20],int seg_num)
{	
	float new_val;

	sscanf(input,"%f",&new_val);

	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= pt_dms_cmm[crt_n].point.stn_id;
	cmd_buf[crt_n].dev_id		= pt_dms_cmm[crt_n].point.dev_id;
	cmd_buf[crt_n].point_id		= pt_dms_cmm[crt_n].point.pt_id;
	cmd_buf[crt_n].data_type 	= CURVE_TYPE;  
	cmd_buf[crt_n].ctrl_addr	= seg_num;
	cmd_buf[crt_n].type_id		= DPS_MAN_VALUE_SET;
	cmd_buf[crt_n].data.iValue =( gint) (new_val *1000);
	ProgDoAckCelBrdcst ( crt_n );
	return TRUE;
}	

void GetPolRtState(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE])
{
	int		item=0;	
    char        tmp_state[50];
        
	GetIndPtRtStateStrById(&db_cmm->point, value_str[item] );
	/*GetIndPtStateStrById( &db_cmm->point,value_str[item]);*/
        sprintf(tmp_state,"(%d)",db_cmm->var.ind.status.rt_state);
        strcat(value_str[item],tmp_state);
	item++;
    tmp_state[0] = 0 ;
	GetIndPtStateStrById( &db_cmm->point,tmp_state);
	if(db_cmm->fixed.ioattr.doublePt)			
		sprintf(value_str[item], "%s(%d)", tmp_state,db_cmm->var.ind.status.state);		
	else
		sprintf(value_str[item], "%s", _CS_(MSG_NOT_DOUBLE_PT));
	item++;
	
	sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.ind.update_time.year,db_cmm->var.ind.update_time.month,db_cmm->var.ind.update_time.day,db_cmm->var.ind.update_time.hour,
			db_cmm->var.ind.update_time.minute,db_cmm->var.ind.update_time.second,db_cmm->var.ind.update_time.ms );
	item++;	
	
	sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.ind.change_time.year,db_cmm->var.ind.change_time.month,db_cmm->var.ind.change_time.day,db_cmm->var.ind.change_time.hour,
			db_cmm->var.ind.change_time.minute,db_cmm->var.ind.change_time.second,db_cmm->var.ind.change_time.ms );
	item++;	
	
	GetYesNoStr(db_cmm->var.ind.status.fault_tag,value_str[item]);	
	item++;
	
	/*if(db_cmm->var.ind.status.chan_fail)
		strcpy(value_str[item],DPS_STR_FAIL);
	else if(db_cmm->var.ind.status.man_set)
		strcpy(value_str[item],DPS_STR_MANUAL);
	else
		strcpy(value_str[item],DPS_STR_NORMAL);*/
        if(db_cmm->var.ind.status.chan_fail==0)                    
                    strcpy(value_str[item],DPS_STR_NORMAL);
        else
                    strcpy(value_str[item],DPS_STR_FAIL);
	item++;
        
	GetYesNoStr(db_cmm->var.ind.status.man_set, value_str[item]);
        item++;
        
	sprintf(value_str[item], "%d", db_cmm->var.ind.stc.norm_open); 
	open_close_state[item]= IND_PARAM_SET_NORM_OPEN_NUM;
	item++;
	
	sprintf(value_str[item], "%d", db_cmm->var.ind.stc.norm_close); 
	open_close_state[item]= IND_PARAM_SET_NORM_CLOSE_NUM;
	item++;
	
	sprintf(value_str[item], "%d", db_cmm->var.ind.stc.fault_open); 
	open_close_state[item]=IND_PARAM_SET_FAULT_OPEN_NUM;
	item++;
	
	sprintf(value_str[item], "%d", db_cmm->var.ind.stc.fault_close); 
	open_close_state[item]=IND_PARAM_SET_FAULT_CLOSE_NUM;
	item++;
}  

void GetIMPRtVal(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE])
{
	int			item=0;	
	char    UnitName[20] ;
	sprintf(value_str[item], "%u", db_cmm->var.imp.entry.rt_value); 
	imp_param_type[item] = IMP_PARAM_SET_IMP_ACCUM;
	item++;
	
	sprintf(value_str[item], "%d", db_cmm->var.imp.entry.base_value); 
	imp_param_type[item] = IMP_PARAM_SET_BASE_ACCUM;
	item++;
	
	sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.imp.entry.update_time.year,db_cmm->var.imp.entry.update_time.month,db_cmm->var.imp.entry.update_time.day,db_cmm->var.imp.entry.update_time.hour,
			db_cmm->var.imp.entry.update_time.minute,db_cmm->var.imp.entry.update_time.second,db_cmm->var.imp.entry.update_time.ms );
	item++;	
	
	if(db_cmm->var.imp.entry.status.chan_fail)
		strcpy(value_str[item],DPS_STR_FAIL);
	else if(db_cmm->var.imp.entry.status.man_set)
		strcpy(value_str[item],DPS_STR_MANUAL);
	else
		strcpy(value_str[item],DPS_STR_NORMAL);
	item++;
	
	sprintf(value_str[item], "%u", db_cmm->var.imp.entry.eng_para.CONSTANT); 
        imp_param_type[item] =IMP_PARAM_SET_CONSTANT;
	item++;	
        
    if (GetImpPtStatisticUnitNameById( &db_cmm->point, RD_IMP_YEAR,  UnitName )!=-1)	
			sprintf(value_str[item], "%.3f(%s)", db_cmm->var.imp.entry.d_accum , UnitName);
	else 
		    sprintf(value_str[item], "%.3f(*)", db_cmm->var.imp.entry.d_accum  );

	imp_param_type[item] = IMP_PARAM_SET_D_ACCUM;
	item++;
        
	sprintf(value_str[item], "%.3f", db_cmm->var.imp.init.year); 
        imp_param_type[item] =IMP_PARAM_SET_INIT_YEAR;
	item++;
	
	sprintf(value_str[item], "%.3f", db_cmm->var.imp.init.month); 
        imp_param_type[item] =IMP_PARAM_SET_INIT_MONTH;
	item++;
	
	sprintf(value_str[item], "%.3f", db_cmm->var.imp.init.day);
        imp_param_type[item] =IMP_PARAM_SET_INIT_DAY;
	item++;
        
        sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.eng_para.PT);
        imp_param_type[item] =IMP_PARAM_SET_PT;
	item++;
	
	sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.eng_para.CT); 
        imp_param_type[item] =IMP_PARAM_SET_CT;
	item++;
        
        sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.eng_para.ENG_CVT); 
	imp_param_type[item] =IMP_PARAM_SET_CVT_F;
	item++;	

	sprintf(value_str[item], "%d", db_cmm->var.imp.entry.eng_para.DAY_RATE); 	 
	item++;	

	sprintf(value_str[item], "%d", db_cmm->var.imp.entry.eng_para.YEAR_RATE);  
	item++;	
}

void GetImpParm(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE])
{
	int			item=0;
	int day_rate,mon_rate,year_rate;
        day_rate = 1 ;/*GetImpDayRate();  */
        mon_rate = 1 ; /*GetImpMonthRate();  */
        year_rate= 1 ;  /*GetImpYearRate();  */

	sprintf(value_str[item], "%.3f", db_cmm->var.imp.peak.day);        
 	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_PEAK_DAY;
	item++;

	sprintf(value_str[item], "%.1f", db_cmm->var.imp.peak.month);
	imp_param_type[item+IMP_RTVALUE_COL_NUM] = IMP_PARAM_SET_PEAK_MONTH;
	item++;

	sprintf(value_str[item], "%.1f", db_cmm->var.imp.peak.year); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_PEAK_YEAR;
	item++;
	sprintf(value_str[item], "%.3f", db_cmm->var.imp.valley.day); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_VALLEY_DAY;
	item++;
	sprintf(value_str[item], "%.1f", db_cmm->var.imp.valley.month); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_VALLEY_MONTH;
	item++;

	sprintf(value_str[item], "%.1f", db_cmm->var.imp.valley.year); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_VALLEY_YEAR;
	item++;
	sprintf(value_str[item], "%.3f", db_cmm->var.imp.normal.day); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_NORM_DAY;
	item++;
	sprintf(value_str[item], "%.1f", db_cmm->var.imp.normal.month); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_NORM_MONTH;
	item++;
	sprintf(value_str[item], "%.1f", db_cmm->var.imp.normal.year); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_NORM_YEAR;
	item++;
	sprintf(value_str[item], "%.3f", db_cmm->var.imp.sum.day); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_SUM_DAY;
	item++;
	sprintf(value_str[item], "%.1f", db_cmm->var.imp.sum.month); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_SUM_MONTH;
	item++;
	sprintf(value_str[item], "%.1f", db_cmm->var.imp.sum.year); 	
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_SUM_YEAR;
	item++;
	sprintf(value_str[item], "%.1f", db_cmm->var.imp.ppeak.day); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] = IMP_PARAM_SET_PPEAK_DAY;
	item++;	
	
	sprintf(value_str[item], "%.1f", db_cmm->var.imp.ppeak.month); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_PPEAK_MONTH;
	item++;

	sprintf(value_str[item], "%.1f", db_cmm->var.imp.ppeak.year); 
	imp_param_type[item+IMP_RTVALUE_COL_NUM] =IMP_PARAM_SET_PPEAK_YEAR;
	item++;
	
}

void GetCharaParm(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE])
{	
	int			item=0;	
	
	sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.chara.update_time.year,db_cmm->var.chara.update_time.month,db_cmm->var.chara.update_time.day,db_cmm->var.chara.update_time.hour,
			db_cmm->var.chara.update_time.minute,db_cmm->var.chara.update_time.second,db_cmm->var.chara.update_time.ms );
	item++;	
	sprintf(value_str[item], "%s", db_cmm->var.chara.start_pt_name); 
	item++;	
	sprintf(value_str[item], "%d", db_cmm->var.chara.pt_num); 
	item++;	
	sprintf(value_str[item], "%.2f", db_cmm->var.chara.chara_data.max_no/100.); //wzg
	item++;
	sprintf(value_str[item], "%.2f", db_cmm->var.chara.chara_data.min_no/100.); 
	item++;	
	sprintf(value_str[item], "%.2f", db_cmm->var.chara.chara_data.max_rate_no/100.); 
	item++;	
	sprintf(value_str[item], "%.2f", db_cmm->var.chara.chara_data.min_rate_no/100.); 
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.max_value); 
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.min_value); 
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.avg_value); 
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.std_value); 
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.max_rate); 
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.min_rate); 
}

void GetCurveParm(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE])
{	
	int		item=0;
		
	sprintf(value_str[item],"%d.%d.%d.%d",db_cmm->point.stn_id,db_cmm->point.dev_id,db_cmm->point.data_type,db_cmm->point.pt_id);	
	item++;
	GetPtNameById(&db_cmm->point,value_str[item],NULL);	
	item++;	
	GetEntryLongnameById(&db_cmm->point,value_str[item]);	
	item++;	
	sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.curve.update_time.year,db_cmm->var.curve.update_time.month,db_cmm->var.curve.update_time.day,db_cmm->var.curve.update_time.hour,
			db_cmm->var.curve.update_time.minute,db_cmm->var.curve.update_time.second,db_cmm->var.curve.update_time.ms );
	item++;	
	sprintf(value_str[item], "%d", db_cmm->var.curve.seg_num); 
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.curve.eng_para.HI_RANGE );
	item++;	
	sprintf(value_str[item], "%.3f", db_cmm->var.curve.eng_para.LO_RANGE );	
}

void GetCurveSegVal(int crt_n,DMS_COMMON *db_cmm,char value_str[300][POINT_LONGNAME_SIZE])
{	

	int			i;

	for(i=0;i<db_cmm->var.curve.seg_num;i++)
	{
		sprintf(value_str[i],"%.0f ", db_cmm->var.curve.data.value[i]);
		/*printf("value_str[%d]=%s\n",i,value_str[i]);*/
	}
}

void GetObjParam(int crt_n,DMS_COMMON *db_cmm,char objvalue_str[30][POINT_LONGNAME_SIZE],char label_str[20][POINT_LONGNAME_SIZE],int *col_num)
{	
	int		item=0;
	
        strcpy(label_str[item],_CS_(OIX_CSTR_CMD_TIME));
        GetTimetoStr(&db_cmm->var.devobj.cmd_time,objvalue_str[item]); 
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_USR_NAME));
	strcpy(	objvalue_str[item],db_cmm->var.devobj.usr_name);
	item++;	
        
        strcpy(label_str[item],_CS_(OIX_CSTR_HOST_ID));
	sprintf(objvalue_str[item],"%d",db_cmm->var.devobj.host_id);
	item++;	
        
        strcpy(label_str[item],_CS_(OIX_CSTR_OPERATE_STATE));
	sprintf(objvalue_str[item],"%d",db_cmm->var.devobj.status.opr);
	item++;	
        
	strcpy(label_str[item],_CS_(OIX_CSTR_TYPE_ID));
	sprintf(objvalue_str[item],"%d",db_cmm->var.devobj.status.state);
	item++;	
        
        strcpy(label_str[item],_CS_(OIX_CSTR_RETURN_STATE));
	sprintf(objvalue_str[item],"%d",db_cmm->var.devobj.status.result);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_SOURCE_ID));
	sprintf(objvalue_str[item],"%d",db_cmm->var.devobj.status.src);
	item++;
        
	strcpy(label_str[item],_CS_(OIX_CSTR_DATA));
	sprintf(objvalue_str[item],"%d(%.3f)",db_cmm->var.devobj.data.iValue, db_cmm->var.devobj.data.fValue);
	item++;
        *col_num = item;
}

void GetFbParam(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE],char label_str[20][POINT_LONGNAME_SIZE],int *col_num)
{	
	int		item=0;
	
        strcpy(label_str[item],_CS_(OIX_CSTR_ACTT));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.param);
	item++;
        *col_num = item;
}

void GetFbStatus1(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE],char label_str[20][POINT_LONGNAME_SIZE],int *col_num)
{	
	int		item=0;
	
        strcpy(label_str[item],_CS_(OIX_CSTR_CONDC0));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.condc0);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_CENC0));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.c0);
	item++;	
        
        strcpy(label_str[item],_CS_(OIX_CSTR_CONDC1));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.condc1);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_CENC1));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.c1);
	item++;	
        
         strcpy(label_str[item],_CS_(OIX_CSTR_A0));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.a0);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_A1));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.a1);
	item++;	
        
        strcpy(label_str[item],_CS_(OIX_CSTR_SWGF_N));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.swgf_n);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_P0));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.p0);
	item++;	
        
        strcpy(label_str[item],_CS_(OIX_CSTR_P1));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.p1);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_MA));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.ma);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_LOCALM));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.localm);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_LCULM));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.lculm);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_REMOTE));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.remote);
	item++;
         *col_num = item;
}

void GetFbStatus2(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE],char label_str[20][POINT_LONGNAME_SIZE],int *col_num)
{	
	int		item=0;
	
        strcpy(label_str[item],_CS_(OIX_CSTR_Q0));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status2.q0);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_Q1));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status2.q1);
	item++;	
        
        strcpy(label_str[item],_CS_(OIX_CSTR_ORDERM));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status2.orderm);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_POSD0I));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status2.posd0);
	item++;
        
        strcpy(label_str[item],_CS_(OIX_CSTR_POSD1I));
        sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status2.posd1);
	item++;
         *col_num = item;
}

void GetTimeVal(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE])
{
	int		item=0;	
	struct tm newTime;
	newTime = *localtime(&db_cmm->var.time.value_sec);
	sprintf(value_str[item],"%04d-%02d-%02d %02d:%02d:%02d ",newTime.tm_year+1900,
		newTime.tm_mon+1, newTime.tm_mday, newTime.tm_hour,
		newTime.tm_min, newTime.tm_sec);	
	item++;
	sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.chara.update_time.year,db_cmm->var.chara.update_time.month,db_cmm->var.chara.update_time.day,db_cmm->var.chara.update_time.hour,
		db_cmm->var.chara.update_time.minute,db_cmm->var.chara.update_time.second,db_cmm->var.chara.update_time.ms );
	item++;	
	sprintf(value_str[item],"%.4f",db_cmm->var.time.coords.x);
	item++;
	sprintf(value_str[item],"%.4f",db_cmm->var.time.coords.y);
	item++;
	sprintf(value_str[item],"%.4f",db_cmm->var.time.coords.z);
	item++;

//	strcpy(label_str[item],_CS_(OIX_CSTR_USR_NAME));
	strcpy(	value_str[item],db_cmm->var.devobj.usr_name);
	item++;	
	
//	strcpy(label_str[item],_CS_(OIX_CSTR_HOST_ID));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.host_id);
	item++;	
	
//	strcpy(label_str[item],_CS_(OIX_CSTR_OPERATE_STATE));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.status.opr);
	item++;	
	
//	strcpy(label_str[item],_CS_(OIX_CSTR_TYPE_ID));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.status.state);
	item++;	
	
//	strcpy(label_str[item],_CS_(OIX_CSTR_RETURN_STATE));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.status.result);
	item++;
	
//	strcpy(label_str[item],_CS_(OIX_CSTR_SOURCE_ID));
	sprintf(value_str[item],"%d",db_cmm->var.devobj.status.src);
	item++;

}

void
copy_button_clicked (GtkWidget *button,
                     gpointer   user_data)
{
	GtkWidget *entry;
	GtkClipboard *clipboard;
	
	entry = GTK_WIDGET (user_data);
	
	/* Get the clipboard object */
	clipboard = gtk_widget_get_clipboard (entry,
		GDK_SELECTION_CLIPBOARD);
	
	/* Set clipboard text */
	gtk_clipboard_set_text (clipboard, gtk_entry_get_text (GTK_ENTRY (entry)), -1);
}
 
 void DspPageInfo(int crt_n,BOOL IsEditable,GtkWidget *notebook,char *label_title[],int base_col,
		char value_str[50][POINT_LONGNAME_SIZE],int label_col,char *page_title)
{    	
    	 GtkWidget*label[30],*modify_btn[30];
		 GtkWidget *labelbtn;
		 char *text ;
    	 int i,first_col;
    	GtkWidget *vbox,*hbox,*hbox1,*tab_label,*exit_btn;
        hbox=gtk_hbox_new(FALSE, 15);
    	gtk_widget_show(hbox);
     
 	vbox=gtk_vbox_new(FALSE, 5);
    	gtk_widget_show(vbox);
    	if(label_col%2==0)
    		first_col = label_col/2;
    	else
    		first_col = label_col/2+1;


    	for(i=0;i<first_col;i++)
    	{
    		hbox1=gtk_hbox_new(FALSE, 2);
    		gtk_widget_show(hbox1);

			//add by hcl 系统维护权限 可以复制逻辑名
			labelbtn=NULL;
			if((strcmp(label_title[i],OIX_STR_IO_ADDRESS)==0
                ||strcmp(label_title[i],OIX_CSTR_NAME)==0
				||strcmp(label_title[i],OIX_CSTR_LONGNAME)==0
				)&&(CheckUserPrivilege(cur_usr_name, PRIV_SYS_MAINT)))
			{		
				labelbtn=label[i] =gtk_button_new_with_label  (text =_toUtf8EX(label_title[i]));
				G_SAFE_FREE(text);//dzy1017
				gtk_widget_show(label[i]); 
			}else{
				label[i] =gtk_label_new(text =_toUtf8EX(label_title[i]));
				G_SAFE_FREE(text);//dzy1017
				gtk_widget_show(label[i]); 
				gtk_label_set_justify (GTK_LABEL (label[i]), GTK_JUSTIFY_LEFT);
				gtk_misc_set_alignment (GTK_MISC (label[i]), 0, 0);
				
			}
			
			gtk_box_pack_start(GTK_BOX(hbox1),label[i],FALSE,FALSE,0);
			gtk_widget_set_usize(GTK_WIDGET(label[i]), 75, -1);
			if(strcmp(label_title[i],OIX_CSTR_RT_STATE)==0)
				gtk_widget_set_usize(GTK_WIDGET(label[i]), 60, -1);
			label[i] =gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(label[i]),text= _toUtf8EX(value_str[i+base_col]));
			G_SAFE_FREE(text);//dzy1017
			gtk_entry_set_editable(GTK_ENTRY(label[i]),FALSE);
			gtk_widget_set_sensitive(label[i],FALSE);
			
			//add by hcl 
			if((labelbtn)&&(CheckUserPrivilege(cur_usr_name, PRIV_SYS_MAINT)))
			{
				g_signal_connect (labelbtn, "clicked",
					G_CALLBACK (copy_button_clicked), label[i]);
				labelbtn=NULL;
			}

    		if(IsEditable||strcmp(label_title[i],OIX_CSTR_RT_VALUE)==0
                ||(((i==1)||(i>3)) && strcmp(label_title[i],IMP_RTVALUE_LABEL[i])==0))
    			gtk_widget_set_usize(GTK_WIDGET(label[i]), (((i==1)||(i>3)) && strcmp(label_title[i],IMP_RTVALUE_LABEL[i])==0)?180:90, -1);
            else if(strcmp(label_title[i],OIX_CSTR_RT_STATE)==0)
			gtk_widget_set_usize(GTK_WIDGET(label[i]), 80, -1);
    		else
                    {
    			gtk_widget_set_usize(GTK_WIDGET(label[i]), 240, -1);
                        if(strcmp(page_title,OIX_CSTR_RT_STATE)==0)
                            gtk_widget_set_usize(GTK_WIDGET(label[i]), 200, -1);
                        else if(strcmp(page_title, OIX_CSTR_RT_VALUE)==0)
                            gtk_widget_set_usize(GTK_WIDGET(label[i]), 180, -1);
                }
    		gtk_widget_show(label[i]); 
    		gtk_box_pack_start(GTK_BOX(hbox1),label[i],FALSE,FALSE,0);
    		if(strcmp(page_title,OIX_CSTR_BASE_INFO)==0)
    			base_info_entry[crt_n][i]=label[i];
    		if(strcmp(page_title,OIX_CSTR_RT_VALUE)==0)
    			rt_val_entry[crt_n][i]= label[i];
    		if(strcmp(page_title,OIX_CSTR_RT_STATE)==0)
    			rt_state_entry[crt_n][i]= label[i];
    		if(IsEditable||strcmp(label_title[i],OIX_CSTR_RT_STATE)==0
                ||strcmp(label_title[i],OIX_CSTR_RT_VALUE)==0
				||strcmp(label_title[i],OIX_CSTR_SET_TIME_VALUE)==0
                ||(((i==1)||(i>3)) && strcmp(label_title[i],IMP_RTVALUE_LABEL[i])==0))
    		{
				if(strcmp(label_title[i],OIX_CSTR_RT_STATE)==0) 
				{
					//zlb 开出量设置和功能未实现，隐藏按钮
// 					modify_btn[i] =gtk_button_new_with_label(text = _toUtf8(_CS_(OIX_CSTR_ClsDevice)));
// 					G_SAFE_FREE(text);//dzy1017
// 					gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 53, -1);
				}
				else
				{
						modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_MODIFY));
						G_SAFE_FREE(text);//dzy1017
    				
						gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 50, -1);
						if(pt_dms_cmm[crt_n].point.data_type==CALC_TYPE&&strcmp(label_title[i],OIX_CSTR_RT_VALUE)==0)
							gtk_widget_set_sensitive(modify_btn[i], FALSE);

						gtk_widget_show(modify_btn[i]);				
						gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);

						gtk_widget_show(modify_btn[i]);				
						gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);

				
				}
//				gtk_widget_show(modify_btn[i]);				
//				gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);

				if(strcmp(label_title[i],OIX_CSTR_RT_STATE)==0)
				{
					//zlb 开出量设置和功能未实现，隐藏按钮
//					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyRtState), (gpointer)1);
				}
    			
			if(strcmp(label_title[i],OIX_CSTR_SET_TIME_VALUE)==0)
			gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyManSetTime), (gpointer)0);
			
				if(strcmp(label_title[i],OIX_CSTR_RT_VALUE)==0/*&&pt_dms_cmm[crt_n].point.data_type!=CALC_TYPE*/)
				{
					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyRtValue),(gpointer)i);
    			 
					if(pt_dms_cmm[crt_n].point.data_type==ANA_TYPE||pt_dms_cmm[crt_n].point.data_type==OUTA_TYPE)
					{
						modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_STR_ANA_FORCE));
						G_SAFE_FREE(text);//dzy1017
						gtk_widget_show(modify_btn[i]);
						gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 53, -1);
						gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(AnaForce), (gpointer)1);
						gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);

						modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_STR_UNFORCE));
						G_SAFE_FREE(text);//dzy1017
						gtk_widget_show(modify_btn[i]);
						gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 53, -1);
						gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(AnaForce), (gpointer)0);
						gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);

					}

				}
				if(strcmp(page_title,OIX_CSTR_LMT_VAL)==0)
    			{
    				ana_lmt_entry[crt_n][i]= label[i];
    				gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyLmtVal), (gpointer)i);
    			}
				if(strcmp(page_title,OIX_CSTR_PARAM)==0)
    			{
    				imp_param_entry[crt_n][i+IMP_RTVALUE_COL_NUM]= label[i];
    				gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyImpParam), (gpointer)(i+IMP_RTVALUE_COL_NUM));
    			}
				if(strcmp(label_title[i],IMP_RTVALUE_LABEL[i])==0)
					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyImpParam), (gpointer)i);

				if(strcmp(label_title[i],OIX_CSTR_RT_STATE)!=0)
    				gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
                        
				if(strcmp(label_title[i],OIX_CSTR_RT_STATE)==0)
				{
					//zlb 开出量设置分功能未实现，去除按钮
// 					modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_OpnDevice));
// 					G_SAFE_FREE(text);//dzy1017
// 					gtk_widget_show(modify_btn[i]);
// 					gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 53, -1);
// 					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyRtState), (gpointer)0);
// 					gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);

					modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_STR_FORCEON));
					G_SAFE_FREE(text);//dzy1017					
					gtk_widget_show(modify_btn[i]);
					gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 53, -1);
					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyRtState), (gpointer)2);
					gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
					
					modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_STR_FORCEOFF));
					G_SAFE_FREE(text);//dzy1017
					gtk_widget_show(modify_btn[i]);
					gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 53, -1);
					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyRtState), (gpointer)3);
					gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);

					modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_STR_UNFORCE));
					G_SAFE_FREE(text);//dzy1017
					gtk_widget_show(modify_btn[i]);
					gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 53, -1);
					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyRtState), (gpointer)4);
					gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
				}
                        
    		}
    		gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);	
    	}
    	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);
 
    	vbox=gtk_vbox_new(FALSE,5);
    	gtk_widget_show(vbox);
    	for(i=first_col;i<label_col;i++)
    	{
    	    	hbox1=gtk_hbox_new(FALSE, 10);
    		gtk_widget_show(hbox1);
    		label[i] =gtk_label_new(text = _toUtf8EX(label_title[i]));
			G_SAFE_FREE(text);//dzy1017
    		gtk_widget_show(label[i]); 
		gtk_label_set_justify (GTK_LABEL (label[i]), GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment (GTK_MISC (label[i]), 0, 0);
    		gtk_box_pack_start(GTK_BOX(hbox1),label[i],FALSE,FALSE,0);
		gtk_widget_set_usize(GTK_WIDGET(label[i]), 65, -1);
		
    		label[i] =gtk_entry_new();
    		gtk_entry_set_text(GTK_ENTRY(label[i]),text = _toUtf8EX(value_str[i+base_col]));
			G_SAFE_FREE(text);//dzy1017
    		gtk_entry_set_editable(GTK_ENTRY(label[i]),FALSE);
    		gtk_widget_set_sensitive(label[i],FALSE);
    		
		if(IsEditable||strcmp(page_title,OIX_CSTR_RT_STATE)==0)
    			gtk_widget_set_usize(GTK_WIDGET(label[i]), 70, -1);
    		else
    			gtk_widget_set_usize(GTK_WIDGET(label[i]), 100, -1);
    		
    		gtk_widget_show(label[i]); 
    		gtk_box_pack_start(GTK_BOX(hbox1),label[i],FALSE,FALSE,0);
    		
    		if(strcmp(page_title,OIX_CSTR_RT_VALUE)==0)
    			rt_val_entry[crt_n][i]= label[i];
    		if(strcmp(page_title,OIX_CSTR_BASE_INFO)==0)
    			base_info_entry[crt_n][i]=label[i];
               
    		if(i<IMP_RTVALUE_COL_NUM)
            {
				if( strcmp(label_title[i],IMP_RTVALUE_LABEL[i])==0)
				{
					modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_MODIFY));	
					G_SAFE_FREE(text);//dzy1017
    				
					gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 50, -1); 
                       
					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyImpParam), (gpointer)i);
					gtk_widget_show(modify_btn[i]); 
					gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
				}

				if(strcmp(label_title[i],OIX_STR_POPUP_DSPFILE)==0) 
				{
					modify_btn[i] =gtk_button_new_with_label(text = _toUtf8(_CS_(OIX_STR_POPUP_DSPFILE)));
					G_SAFE_FREE(text);//dzy1017
					gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 50, -1);

					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(gotoDSP), (gpointer)0);
					gtk_widget_show(modify_btn[i]); 
					gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
				}
			}
    		if(IsEditable||strcmp(page_title,OIX_CSTR_RT_STATE)==0)
    		{
    			modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_MODIFY));	
				G_SAFE_FREE(text);//dzy1017
    				
    			gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 50, -1); 
    			
    			if(strcmp(page_title,OIX_CSTR_LMT_VAL)==0)
    			{
    				gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyLmtVal), (gpointer)i);
    				ana_lmt_entry[crt_n][i]= label[i];
    				if(i<label_col-2)
    				{
    					gtk_widget_show(modify_btn[i]); 
    					gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
    				}
    			}
			else if(strcmp(page_title,OIX_CSTR_PARAM)==0)
    			{
    				imp_param_entry[crt_n][i+IMP_RTVALUE_COL_NUM]= label[i];
    				gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyImpParam), (gpointer)(i+IMP_RTVALUE_COL_NUM));
				gtk_widget_show(modify_btn[i]); 
				gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
    			} 
    			else if (strcmp(page_title,OIX_CSTR_RT_STATE)==0) 
    			{
    				rt_state_entry[crt_n][i]=label[i];
    				/*if(i>=label_col-4)*/
                                if(open_close_state[i]==IND_PARAM_SET_NORM_OPEN_NUM||open_close_state[i]== IND_PARAM_SET_NORM_CLOSE_NUM
                                ||open_close_state[i]==IND_PARAM_SET_FAULT_OPEN_NUM ||open_close_state[i]==IND_PARAM_SET_FAULT_CLOSE_NUM)
    				{
    					open_close_num[crt_n][i]=label[i];
    					
    					gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyOpenCloseNum), (gpointer)i);
					gtk_widget_show(modify_btn[i]); 
					gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
				}
    			} 			
    		}
    		gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);	
    	}   	
 	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0); 	

        hbox1 = gtk_hbox_new(FALSE, 15);
    	gtk_widget_show(hbox1);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);
    	exit_btn =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_EXIT));
		G_SAFE_FREE(text);//dzy1017
    	gtk_widget_show(exit_btn); 
    	gtk_box_pack_start(GTK_BOX(hbox1),exit_btn,FALSE,FALSE,0);  
    	gtk_widget_set_usize(GTK_WIDGET(exit_btn), 50, 30);  		
    	gtk_signal_connect(GTK_OBJECT(exit_btn),"clicked",GTK_SIGNAL_FUNC(DestroyDetailInfoBox), (gpointer)crt_n);
    	
      	tab_label= gtk_label_new(text = _toUtf8EX(page_title));
		G_SAFE_FREE(text);//dzy1017
    	gtk_widget_show(tab_label);    	
    	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),hbox,tab_label);
 }   	  
 
 
void DspIOPageInfo(int crt_n,GtkWidget *notebook,char *label_title[],int base_col,
		char value_str[50][POINT_LONGNAME_SIZE],int label_col,int vbox_lab_spacing,int vbox_entry_spacing,char *page_title)
{    	
    	 GtkWidget*label[30],*modify_btn[30];
    	 int i,first_col;
		 char *text ;
    	GtkWidget *vbox,*vbox1,*hbox,*hbox1,*tab_label,*exit_btn;
        hbox=gtk_hbox_new(FALSE, 10);
    	gtk_widget_show(hbox);
    	
 	vbox=gtk_vbox_new(FALSE, vbox_lab_spacing);
    	gtk_widget_show(vbox);

    	if(label_col%2==0)
    		first_col = label_col/2;
    	else
    		first_col = label_col/2+1;
    	for(i=0;i<first_col;i++)
    	{
    		label[i] =gtk_label_new(text = _toUtf8EX(label_title[i]));
			G_SAFE_FREE(text);//dzy1017
    		gtk_widget_show(label[i]); 
    		gtk_widget_set_usize(GTK_WIDGET(label[i]), 80, -1); 
			gtk_label_set_justify (GTK_LABEL (label[i]), GTK_JUSTIFY_LEFT);
			gtk_misc_set_alignment (GTK_MISC (label[i]), 0, 0);
    		gtk_box_pack_start(GTK_BOX(vbox),label[i],FALSE,FALSE,0);
    	}	
    	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0); 
   
    	vbox=gtk_vbox_new(FALSE, vbox_entry_spacing);
    	gtk_widget_show(vbox);
    	for(i=0;i<first_col;i++)
    	{    
    		if(strcmp(value_str[i+base_col],OIX_CSTR_YES)==0 ||strcmp(value_str[i+base_col],OIX_CSTR_NO)==0)
    		{
    		     
				if( opr_state[i+base_col]==IOST_SET_NO_UPDATE)
				{
					IO_entry[crt_n][i+base_col] =gtk_entry_new();    			
					gtk_entry_set_text(GTK_ENTRY(IO_entry[crt_n][i+base_col]),text = _toUtf8EX(value_str[i+base_col]));
					G_SAFE_FREE(text);//dzy1017
					gtk_entry_set_editable(GTK_ENTRY(IO_entry[crt_n][i+base_col]),FALSE);
					gtk_widget_set_sensitive(IO_entry[crt_n][i+base_col],FALSE); 
					gtk_widget_set_usize(GTK_WIDGET(IO_entry[crt_n][i+base_col]), 100, -1);
					gtk_widget_show(IO_entry[crt_n][i+base_col]); 
					gtk_box_pack_start(GTK_BOX(vbox),IO_entry[crt_n][i+base_col],FALSE,FALSE,0);   
					gtk_widget_show(IO_entry[crt_n][i+base_col]); 
				}    
				else
				{
					yes_btn[crt_n][i+base_col] =gtk_toggle_button_new_with_label(text =_toUtf8EX(OIX_CSTR_YES)); 
					G_SAFE_FREE(text);//dzy1017
					gtk_widget_set_usize(GTK_WIDGET(yes_btn[crt_n][i+base_col]), 100, -1);
    				if(strcmp(value_str[i+base_col],OIX_CSTR_YES)==0)    			
    					gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(yes_btn[crt_n][i+base_col]),TRUE);     			
    				else
    				{
    			    	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(yes_btn[crt_n][i+base_col]),FALSE);
    			    	gtk_object_set(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"GtkButton::label",text =_toUtf8EX(OIX_CSTR_NO),NULL);
						G_SAFE_FREE(text);//dzy1017
    				}
					gtk_widget_show(yes_btn[crt_n][i+base_col]); 
					if ((i+base_col)== 15  || (i+base_col)==19    )   /****封掉重复报警设置按钮***/
						gtk_widget_set_sensitive(yes_btn[crt_n][i+base_col],FALSE);
				}
    			
    			gtk_box_pack_start(GTK_BOX(vbox),yes_btn[crt_n][i+base_col],FALSE,FALSE,0); 
			

 			gtk_signal_connect(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"pressed",GTK_SIGNAL_FUNC(YesBtnPressed), (gpointer)(i+base_col)); 						 
    		}
    		else
    		{    
    			hbox1=gtk_hbox_new(FALSE, 10);
    			gtk_widget_show(hbox1);		
    			IO_entry[crt_n][i+base_col] =gtk_entry_new();
    			
    			gtk_entry_set_text(GTK_ENTRY(IO_entry[crt_n][i+base_col]),text = _toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
    			gtk_entry_set_editable(GTK_ENTRY(IO_entry[crt_n][i+base_col]),FALSE);
    			gtk_widget_set_sensitive(IO_entry[crt_n][i+base_col],FALSE); 
    			gtk_widget_show(IO_entry[crt_n][i+base_col]); 
    			gtk_box_pack_start(GTK_BOX(hbox1),IO_entry[crt_n][i+base_col],FALSE,FALSE,0);
    			if(base_col!=ANA_BASE_COL_NUM)
    			{      			
    				gtk_widget_set_usize(GTK_WIDGET(IO_entry[crt_n][i+base_col]), 50, -1);	    				
    				  
    				modify_btn[i] =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_MODIFY));   
					G_SAFE_FREE(text);//dzy1017
    				gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 50, -1); 
    				gtk_widget_show(modify_btn[i]);    			
    				gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);  
    				
    				gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyIOBtnPressed), (gpointer)(i+base_col));    						
    			}
    			else
    			{    				
    				gtk_widget_set_usize(GTK_WIDGET(IO_entry[crt_n][i+base_col]), 100, -1); 
    			}
    			
    			gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);	
    		}		
    	}    	
    	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);
    		
    	vbox=gtk_vbox_new(FALSE,vbox_lab_spacing);
    	gtk_widget_show(vbox);
  	
    	for(i=first_col;i<label_col;i++)
    	{
    		label[i] =gtk_label_new(text = _toUtf8EX(label_title[i]));
			G_SAFE_FREE(text);//dzy1017
    		gtk_widget_show(label[i]); 
    		gtk_widget_set_usize(GTK_WIDGET(label[i]), 80, -1); 
		gtk_label_set_justify (GTK_LABEL (label[i]), GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment (GTK_MISC (label[i]), 0, 0);
    		gtk_box_pack_start(GTK_BOX(vbox),label[i],FALSE,FALSE,0);
    	}
    	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0); 	
  	
    	vbox=gtk_vbox_new(FALSE, vbox_entry_spacing);
    	gtk_widget_show(vbox);    	
    	for(i=first_col;i<label_col;i++)
    	{    	
    		if(strcmp(value_str[i+base_col],OIX_CSTR_YES)==0 ||strcmp(value_str[i+base_col],OIX_CSTR_NO)==0)
    		{
                        if( opr_state[i+base_col]==IOATTR_SET_DOUBLE_PNT)
                        {
                            IO_entry[crt_n][i+base_col] =gtk_entry_new();    			
                            gtk_entry_set_text(GTK_ENTRY(IO_entry[crt_n][i+base_col]),text = _toUtf8EX(value_str[i+base_col]));
							G_SAFE_FREE(text);//dzy1017
                            gtk_entry_set_editable(GTK_ENTRY(IO_entry[crt_n][i+base_col]),FALSE);
                            gtk_widget_set_sensitive(IO_entry[crt_n][i+base_col],FALSE); 
                            gtk_widget_set_usize(GTK_WIDGET(IO_entry[crt_n][i+base_col]), 100, -1);
                            gtk_widget_show(IO_entry[crt_n][i+base_col]); 
                            gtk_box_pack_start(GTK_BOX(vbox),IO_entry[crt_n][i+base_col],FALSE,FALSE,0);                            
                         }                       
                        else
                        {                            
                            if(opr_state[i+base_col]==IOATTR_SET_NORM_STATE)                           
						    	yes_btn[crt_n][i+base_col] =gtk_toggle_button_new_with_label(text = _toUtf8(OIX_CSTR_1));  
                            else
                                yes_btn[crt_n][i+base_col] =gtk_toggle_button_new_with_label(text =_toUtf8(OIX_CSTR_YES)); 
                            G_SAFE_FREE(text);//dzy1017                             
                            gtk_widget_set_usize(GTK_WIDGET(yes_btn[crt_n][i+base_col]), 100, -1); 
                            if(strcmp(value_str[i+base_col],OIX_CSTR_YES)==0
                             ||strcmp(value_str[i+base_col],OIX_CSTR_1)==0)
    							gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(yes_btn[crt_n][i+base_col]),TRUE);
                            else
                            {
    			    			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(yes_btn[crt_n][i+base_col]),FALSE);
    			    	        if(opr_state[i+base_col]==IOATTR_SET_NORM_STATE) 
                                    gtk_object_set(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"GtkButton::label",text =_toUtf8EX(OIX_CSTR_0),NULL);
                                else
                                    gtk_object_set(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"GtkButton::label",text =_toUtf8EX(OIX_CSTR_NO),NULL);
                                G_SAFE_FREE(text);//dzy1017  
                            }		
                            gtk_widget_show(yes_btn[crt_n][i+base_col]); 
                            
                            gtk_box_pack_start(GTK_BOX(vbox),yes_btn[crt_n][i+base_col],FALSE,FALSE,0);
                            gtk_signal_connect(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"pressed",GTK_SIGNAL_FUNC(YesBtnPressed), (gpointer)(i+base_col));			 
                       
                        }
                }
    		else
    		{
    			hbox1=gtk_hbox_new(FALSE, 10);
    			gtk_widget_show(hbox1);
    			IO_entry[crt_n][i+base_col] =gtk_entry_new();
    			gtk_entry_set_text(GTK_ENTRY(IO_entry[crt_n][i+base_col]),text = _toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
    			gtk_widget_set_usize(GTK_WIDGET(IO_entry[crt_n][i+base_col]), 50, -1); 
    			gtk_widget_show(IO_entry[crt_n][i+base_col]); 
    			
    			gtk_box_pack_start(GTK_BOX(hbox1),IO_entry[crt_n][i+base_col],FALSE,FALSE,0);
    			
    			gtk_entry_set_editable(GTK_ENTRY(IO_entry[crt_n][i+base_col]),FALSE);
    			gtk_widget_set_sensitive(IO_entry[crt_n][i+base_col],FALSE);    			
    			modify_btn[i] =gtk_button_new_with_label(text =_toUtf8EX(OIX_CSTR_MODIFY));
				G_SAFE_FREE(text);//dzy1017
    				
    			gtk_widget_set_usize(GTK_WIDGET(modify_btn[i]), 50, -1); 		
    			
    			gtk_widget_show(modify_btn[i]); 
    			
    			gtk_box_pack_start(GTK_BOX(hbox1),modify_btn[i],FALSE,FALSE,0);
    			gtk_signal_connect(GTK_OBJECT(modify_btn[i]),"clicked",GTK_SIGNAL_FUNC(ModifyIOBtnPressed), (gpointer)(i+base_col));
    			gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);
    		}
    	}
    	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);      	
      
    	vbox1=gtk_vbox_new(FALSE, vbox_entry_spacing);
    	gtk_widget_show(vbox1);	
    	exit_btn =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_EXIT));
		G_SAFE_FREE(text);//dzy1017
    	gtk_widget_show(exit_btn); 
    	gtk_box_pack_start(GTK_BOX(vbox1),exit_btn,TRUE,FALSE,0);  
    	gtk_widget_set_usize(GTK_WIDGET(exit_btn), 50, 30);  		
    	gtk_signal_connect(GTK_OBJECT(exit_btn),"clicked",GTK_SIGNAL_FUNC(DestroyDetailInfoBox), (gpointer)crt_n);
    		
    	gtk_box_pack_start(GTK_BOX(hbox),vbox1,FALSE,FALSE,0);     	
    	
    	tab_label= gtk_label_new(text =_toUtf8EX(page_title));
		G_SAFE_FREE(text);//dzy1017
    	gtk_widget_show(tab_label);    	
    	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),hbox,tab_label);
 }   	  
 
void UpdateDspPageInfo(int crt_n,BOOL IsEditable,int base_col,
		char value_str[50][POINT_LONGNAME_SIZE],int label_col,char *page_title)
{
    int i;
	char *text ;

    	for(i=0;i<label_col;i++)
    	{
           /* printf("PAGEINFO:: i+base_col=%d,value_str=%s\n",i+base_col,value_str[i+base_col]);*/
    		if(strcmp(page_title,OIX_CSTR_BASE_INFO)==0)    
    		{				
    			if(base_info_entry[crt_n][i]!=NULL)
    				gtk_entry_set_text(GTK_ENTRY(base_info_entry[crt_n][i]),text = _toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
    		}
                if(strcmp(page_title,OIX_CSTR_CMD_INFO)==0)    
    		{				
    			if(cmd_info_entry[crt_n][i]!=NULL)
    				gtk_entry_set_text(GTK_ENTRY(cmd_info_entry[crt_n][i]),text =_toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
    		}
    		if(strcmp(page_title,OIX_CSTR_RT_VALUE)==0 )
    		{    		
    			if(rt_val_entry[crt_n][i]!=NULL)
                            gtk_entry_set_text(GTK_ENTRY(rt_val_entry[crt_n][i]),text =_toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
    		}
    		if(strcmp(page_title,OIX_CSTR_RT_STATE)==0)
    		{
    			if(rt_state_entry[crt_n][i]!=NULL)
                            gtk_entry_set_text(GTK_ENTRY(rt_state_entry[crt_n][i]),text =_toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
    		}
                if(strcmp(page_title,OIX_CSTR_FB_STATUS1)==0)
    		{
    			if(fb_status1_entry[crt_n][i]!=NULL)
                            gtk_entry_set_text(GTK_ENTRY(fb_status1_entry[crt_n][i]),text =_toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
    		}
                if(strcmp(page_title,OIX_CSTR_FB_STATUS2)==0)
    		{
    			if(fb_status2_entry[crt_n][i]!=NULL)
                            gtk_entry_set_text(GTK_ENTRY(fb_status2_entry[crt_n][i]),text =_toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
    		}
    		if(IsEditable)
    		{    			
    			if(strcmp(page_title,OIX_CSTR_LMT_VAL)==0)
    			{
    				if(ana_lmt_entry[crt_n][i]!=NULL)
                                    gtk_entry_set_text(GTK_ENTRY(ana_lmt_entry[crt_n][i]),text =_toUtf8EX(value_str[i+base_col]));
					G_SAFE_FREE(text);//dzy1017
     			}	
                        else if(strcmp(page_title, OIX_CSTR_PARAM)==0)
			{
				if(imp_param_entry[crt_n][i+IMP_RTVALUE_COL_NUM]!=NULL)
                                    gtk_entry_set_text(GTK_ENTRY(imp_param_entry[crt_n][i+IMP_RTVALUE_COL_NUM]),text = _toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
     			 }   			
    		}    		
    	}
 }   	  
void UpdateDspIOPageInfo(int crt_n,int base_col,char value_str[50][POINT_LONGNAME_SIZE],int label_col)
{    	    	 
    	int i;
        char *text ;
    	for(i=0;i<label_col;i++)
    	{    
           /* printf("IOPAGE::i+base_col=%d,value_str=%s\n",i+base_col,value_str[i+base_col]);*/
    		if(strcmp(value_str[i+base_col],OIX_CSTR_YES)==0 ||strcmp(value_str[i+base_col],OIX_CSTR_NO)==0)
    		{    
                   /* printf("i+base_col=%d,value_str=%s\n",i+base_col,value_str[i+base_col]);*/
                        if( opr_state[i+base_col]==IOATTR_SET_DOUBLE_PNT)
						{
                            gtk_entry_set_text(GTK_ENTRY(IO_entry[crt_n][i+base_col]),text =_toUtf8EX(value_str[i+base_col]));
							G_SAFE_FREE(text);//dzy1017
						}
                        else
                        {
                            if(strcmp(value_str[i+base_col],OIX_CSTR_YES)==0) 
                            {   			
    							gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(yes_btn[crt_n][i+base_col]),TRUE);                                
    							if( opr_state[i+base_col]==IOATTR_SET_NORM_STATE)
									gtk_object_set(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"GtkButton::label",text =_toUtf8EX(OIX_CSTR_1),NULL);
                                else
                                    gtk_object_set(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"GtkButton::label",text =_toUtf8EX(OIX_CSTR_YES),NULL);
                                
								G_SAFE_FREE(text);//dzy1017
                            }     			
                            else
                            {
    			    			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(yes_btn[crt_n][i+base_col]),FALSE);
    			    			if( opr_state[i+base_col]==IOATTR_SET_NORM_STATE)
                                  gtk_object_set(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"GtkButton::label",text =_toUtf8EX(OIX_CSTR_0),NULL);
                                else
                                 gtk_object_set(GTK_OBJECT(yes_btn[crt_n][i+base_col]),"GtkButton::label",text =_toUtf8EX(OIX_CSTR_NO),NULL);								
								G_SAFE_FREE(text);//dzy1017
                                   
                            }
                        }					 
    		}
    		else
			{
				gtk_entry_set_text(GTK_ENTRY(IO_entry[crt_n][i+base_col]),text =_toUtf8EX(value_str[i+base_col]));
				G_SAFE_FREE(text);//dzy1017
			}   			
    	}   
 }   	  
 

void DspCurvePageInfo(int crt_n,GtkWidget *notebook,int seg_num,char value_str[300][POINT_LONGNAME_SIZE])
{    	
    	GtkWidget*label[30],*modify_btn[30];
    	int i,j,end_seg,page_num=(seg_num+SEG_PER_PAGE-1)/SEG_PER_PAGE;
    	GtkWidget *vbox,*vbox1,*vbox2,*hbox,*tab_label,*exit_btn;
    	char	label_title[30],page_title[10];
    	int row_num = SEG_PER_PAGE/COL_PER_PAGE;
    	char  *text ;
        hbox=gtk_hbox_new(FALSE, 10);
    	gtk_widget_show(hbox);
    	
    	for(i=0;i<page_num;i++)
    	{    		
    		sprintf(page_title,"s%d",i);
    			
    		hbox=gtk_hbox_new(FALSE, 10);
    		gtk_widget_show(hbox);
    		
    		end_seg= (i+1)*SEG_PER_PAGE < seg_num ? (i+1)*SEG_PER_PAGE : seg_num;
    			
    		for(j=i*SEG_PER_PAGE;j<end_seg;j++)
    		{      			
    			if(j%row_num==0)
    			{    				
    				vbox=gtk_vbox_new(FALSE, 15);
    				gtk_widget_show(vbox);
    				gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);  
    			}			
    			sprintf(label_title,"seg%d",j);    				
    			label[j-i*SEG_PER_PAGE] =gtk_label_new(text = _toUtf8(label_title));
					G_SAFE_FREE(text);//dzy1017
    			gtk_widget_show(label[j-i*SEG_PER_PAGE]);
    			gtk_widget_set_usize(GTK_WIDGET(label[j-i*SEG_PER_PAGE]), 50, 19);
    			gtk_label_set_justify (GTK_LABEL (label[j-i*SEG_PER_PAGE]), GTK_JUSTIFY_LEFT);
				gtk_misc_set_alignment (GTK_MISC (label[j-i*SEG_PER_PAGE]), 0, 0);
    			gtk_box_pack_start(GTK_BOX(vbox),label[j-i*SEG_PER_PAGE],FALSE,FALSE,0);
    				
    				
    			CurveSeg_Entry[crt_n][j] =gtk_entry_new();
    			gtk_entry_set_text(GTK_ENTRY(CurveSeg_Entry[crt_n][j]),value_str[j]);
    			gtk_entry_set_editable(GTK_ENTRY(CurveSeg_Entry[crt_n][j]),FALSE);                        
    			gtk_widget_set_sensitive(CurveSeg_Entry[crt_n][j],FALSE); 
    			gtk_widget_set_usize(GTK_WIDGET(CurveSeg_Entry[crt_n][j]), 50, 25); 
    			gtk_widget_show(CurveSeg_Entry[crt_n][j]); 
    				
    			if(j%row_num==0)
    			{	
    				vbox1=gtk_vbox_new(FALSE, 8);
    				gtk_widget_show(vbox1);
    				gtk_box_pack_start(GTK_BOX(hbox),vbox1,FALSE,FALSE,0);  
    			}
    			gtk_box_pack_start(GTK_BOX(vbox1),CurveSeg_Entry[crt_n][j],FALSE,FALSE,0);    				
    				
    			modify_btn[j-i*SEG_PER_PAGE] =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_MODIFY)); 	
					G_SAFE_FREE(text);//dzy1017
    				
    			gtk_widget_set_usize(GTK_WIDGET(modify_btn[j-i*SEG_PER_PAGE]), 40, 25); 
    			gtk_widget_show(modify_btn[j-i*SEG_PER_PAGE]); 
    			gtk_signal_connect(GTK_OBJECT(modify_btn[j-i*SEG_PER_PAGE]),"clicked",GTK_SIGNAL_FUNC(InputCurveSeg), (gpointer)j);
    			if(j%row_num==0)
    			{	
    				vbox2=gtk_vbox_new(FALSE, 8);
    				gtk_widget_show(vbox2);
    				gtk_box_pack_start(GTK_BOX(hbox),vbox2,FALSE,FALSE,0);  
    			}
    			gtk_box_pack_start(GTK_BOX(vbox2),modify_btn[j-i*SEG_PER_PAGE],FALSE,FALSE,0);
    		}
    		exit_btn =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_EXIT));
				G_SAFE_FREE(text);//dzy1017
    		gtk_widget_show(exit_btn); 
    		gtk_box_pack_start(GTK_BOX(vbox2),exit_btn,TRUE,FALSE,0);                
    		gtk_widget_set_usize(GTK_WIDGET(exit_btn), 50, 25);  		
    		gtk_signal_connect(GTK_OBJECT(exit_btn),"clicked",GTK_SIGNAL_FUNC(DestroyDetailInfoBox), (gpointer)crt_n);
    		
    	
    		tab_label= gtk_label_new(text = _toUtf8EX(page_title));
				G_SAFE_FREE(text);//dzy1017
    		gtk_widget_show(tab_label);    	
    		gtk_notebook_append_page(GTK_NOTEBOOK(notebook),hbox,tab_label);    		
    	} 
 } 
 
 void DspObjPageInfo(int crt_n,GtkWidget *notebook,char value_str[50][POINT_LONGNAME_SIZE],char label_title[50][POINT_LONGNAME_SIZE],int label_col,char page_title[20])
{    	
        GtkWidget*label[30];
    	int i,first_col;char *text ;
    	GtkWidget *vbox,*hbox,*hbox1,*tab_label,*exit_btn;
        hbox=gtk_hbox_new(FALSE, 15);
    	gtk_widget_show(hbox);
    	
 	vbox=gtk_vbox_new(FALSE, 5);
    	gtk_widget_show(vbox);
    	if(label_col%2==0)
    		first_col = label_col/2;
    	else
    		first_col = label_col/2+1;
    	for(i=0;i<first_col;i++)
    	{
    		hbox1=gtk_hbox_new(FALSE, 5);
    		gtk_widget_show(hbox1);
               
    		label[i] =gtk_label_new(text =_toUtf8(label_title[i]));
				G_SAFE_FREE(text);//dzy1017
    		gtk_widget_show(label[i]); 
		gtk_label_set_justify (GTK_LABEL (label[i]), GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment (GTK_MISC (label[i]), 0, 0);
    		gtk_box_pack_start(GTK_BOX(hbox1),label[i],FALSE,FALSE,0);
  		gtk_widget_set_usize(GTK_WIDGET(label[i]), 110, -1);
  		
    		label[i] =gtk_entry_new();
                
    		gtk_entry_set_text(GTK_ENTRY(label[i]),text = _toUtf8EX(value_str[i]));
				G_SAFE_FREE(text);//dzy1017
    		gtk_entry_set_editable(GTK_ENTRY(label[i]),FALSE);
    		gtk_widget_set_sensitive(label[i],FALSE);
    		
    		gtk_widget_set_usize(GTK_WIDGET(label[i]),110, -1);               
               
    		gtk_widget_show(label[i]); 
    		gtk_box_pack_start(GTK_BOX(hbox1),label[i],FALSE,FALSE,0);
    		if(strcmp(page_title,OIX_CSTR_CMD_INFO)==0)
    			cmd_info_entry[crt_n][i]=label[i];
    		if(strcmp(page_title,OIX_CSTR_FB_STATUS1)==0)
    			fb_status1_entry[crt_n][i]=label[i];         
    		if(strcmp(page_title,OIX_CSTR_FB_STATUS2)==0)
    			fb_status2_entry[crt_n][i]=label[i]; 	
                gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);
    		
    	}
    	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);
 
    	vbox=gtk_vbox_new(FALSE,5);
    	gtk_widget_show(vbox);
    	for(i=first_col;i<label_col;i++)
    	{
    	    	hbox1=gtk_hbox_new(FALSE, 5);
    		gtk_widget_show(hbox1);
    		label[i] =gtk_label_new(text =_toUtf8(label_title[i]));
				G_SAFE_FREE(text);//dzy1017
    		gtk_widget_show(label[i]); 
		gtk_label_set_justify (GTK_LABEL (label[i]), GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment (GTK_MISC (label[i]), 0, 0);
    		gtk_box_pack_start(GTK_BOX(hbox1),label[i],FALSE,FALSE,0);
		gtk_widget_set_usize(GTK_WIDGET(label[i]), 135, -1);
		
    		label[i] =gtk_entry_new();
    		gtk_entry_set_text(GTK_ENTRY(label[i]),text = _toUtf8EX(value_str[i]));
				G_SAFE_FREE(text);//dzy1017
    		gtk_entry_set_editable(GTK_ENTRY(label[i]),FALSE);
    		gtk_widget_set_sensitive(label[i],FALSE);
    		
		gtk_widget_set_usize(GTK_WIDGET(label[i]), 110, -1);    		
    		gtk_widget_show(label[i]); 
    		gtk_box_pack_start(GTK_BOX(hbox1),label[i],FALSE,FALSE,0);
    		
    		if(strcmp(page_title,OIX_CSTR_CMD_INFO)==0)
    			cmd_info_entry[crt_n][i]=label[i];
    		if(strcmp(page_title,OIX_CSTR_FB_STATUS1)==0)
    			fb_status1_entry[crt_n][i]=label[i];         
    		if(strcmp(page_title,OIX_CSTR_FB_STATUS2)==0)
    			fb_status2_entry[crt_n][i]=label[i];  
                gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);
    	}   	
 	gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0); 	

        hbox1 = gtk_hbox_new(FALSE, 15);
    	gtk_widget_show(hbox1);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);
    	exit_btn =gtk_button_new_with_label(text = _toUtf8EX(OIX_CSTR_EXIT));
			G_SAFE_FREE(text);//dzy1017
    	gtk_widget_show(exit_btn); 
    	gtk_box_pack_start(GTK_BOX(hbox1),exit_btn,FALSE,FALSE,0);  
    	gtk_widget_set_usize(GTK_WIDGET(exit_btn), 50, 30);  		
    	gtk_signal_connect(GTK_OBJECT(exit_btn),"clicked",GTK_SIGNAL_FUNC(DestroyDetailInfoBox), (gpointer)crt_n);
    	
      	tab_label= gtk_label_new(text = _toUtf8EX(page_title));
			G_SAFE_FREE(text);//dzy1017
    	gtk_widget_show(tab_label);    	
    	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),hbox,tab_label);	
 } 
 
 void UpdateDspCurvePageInfo(int crt_n,int seg_num,char value_str[300][POINT_LONGNAME_SIZE])
{        	
    	int i,j,end_seg,page_num=(seg_num+SEG_PER_PAGE-1)/SEG_PER_PAGE;  
    	
    	for(i=0;i<page_num;i++)
    	{  
    		end_seg= (i+1)*SEG_PER_PAGE < seg_num ? (i+1)*SEG_PER_PAGE : seg_num;
    			
    		for(j=i*SEG_PER_PAGE;j<end_seg;j++)
    			gtk_entry_set_text(GTK_ENTRY(CurveSeg_Entry[crt_n][j]),value_str[j]);
    	} 
 } 
 

GtkWidget* PopupDetailInfoBox(GtkWidget *parent,int crt_n,POINTER pt)
{
	char  EntryName[100] ;
	char  PtInfoTitle[150] ;
	char LongName[100];
	GtkWidget *window;
	char *text ;
	GtkWidget *notebook;

	if(db_detail_info[crt_n])
		DestroyDetailInfoBox(db_detail_info[crt_n],GINT_TO_POINTER(crt_n));
	if(db_io_set_box[crt_n])
		UnmanageDbIoBox(db_io_set_box[crt_n]);

	if ( ReadEntryById ( &pt, &pt_dms_cmm[crt_n] ) == -1 )
	{		
		PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return 0;
	}
	 
	if ( GetPtNameById(&pt,EntryName,NULL)  == -1 )
	{		
		PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return 0;
	}
	if ( GetPtLongnameById ( &pt, LongName ) == -1 )
	{		
        PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return FALSE;
	}
    sprintf (PtInfoTitle, "%s-%s",LongName , EntryName);

	db_detail_pt[crt_n]= pt;
       
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(window),GTK_WINDOW(parent));  
	//gtk_widget_set_usize(GTK_WIDGET(window), 660, 340);
	gtk_widget_set_usize(GTK_WIDGET(window), 660, 460);
	gtk_window_set_destroy_with_parent(GTK_WINDOW(window),TRUE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), text =_toUtf8(PtInfoTitle));  
		G_SAFE_FREE(text);//dzy1017
    gtk_window_set_policy(GTK_WINDOW(window),FALSE,FALSE,TRUE);

    notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window),notebook);
    gtk_widget_show(notebook);	
        
    DspInfo( crt_n,notebook);
    
    gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(DestroyDetailInfoBox), (gpointer)crt_n);	
    db_detail_timeout_id[crt_n]= gtk_timeout_add (1000, (GtkFunction)UpdateDspInfo, (gpointer)parent);	 

    gtk_widget_show_all(window);
	db_detail_info[crt_n]= window;
    return  window ;
         
}



void DspInfo(int crt_n,GtkWidget *notebook)
{
		char	 entry_str[50][64],rt_val[50][POINT_LONGNAME_SIZE],lmt_val[50][POINT_LONGNAME_SIZE];
		char 	value_str[50][POINT_LONGNAME_SIZE],imp_str[50][POINT_LONGNAME_SIZE],curve_val[300][POINT_LONGNAME_SIZE];
	
        char    val_obj[50][POINT_LONGNAME_SIZE],label_obj[50][POINT_LONGNAME_SIZE];
        
        int i,col_num;	
        i=0;        
        GetDbDetailFixedInfo(&pt_dms_cmm[crt_n], &i,entry_str, value_str );
        
    	switch(pt_dms_cmm[crt_n].point.data_type)
    	{
    		case	ANA_TYPE:
    		case	PARAM_TYPE:
    		case	OUTA_TYPE: 
			case	CALC_TYPE:
			/****   基本信息****/ 
				DspPageInfo(crt_n,FALSE,notebook,ANA_BASE_INFO_LABEL,0,
				value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);
			/****   实时值****/  
    			GetAnaRtVal(crt_n,&pt_dms_cmm[crt_n],rt_val);    			
				DspPageInfo(crt_n,FALSE,notebook,ANA_RTVALUE_LABEL,0,
				rt_val,ANA_RTVALUE_COL_NUM,OIX_CSTR_RT_VALUE);	

			/****   限值****/ 
				GetAnaLmtVal(crt_n,&pt_dms_cmm[crt_n],lmt_val);    			
				DspPageInfo(crt_n,TRUE,notebook,ANA_LMT_LABEL,0,lmt_val,ANA_LMT_COL_NUM,OIX_CSTR_LMT_VAL);
			    	 /****   I/O点属性****/      	 
    	 		DspIOPageInfo(crt_n,notebook,ANA_IOATTR_LABEL,ANA_BASE_COL_NUM+ANA_IOST_COL_NUM,
					value_str,ANA_IOATTR_COL_NUM,VBOX_LAB_SPACING,2,OIX_CSTR_IOATTR); 
    			/****   I/O点状态****/     	
    			DspIOPageInfo(crt_n,notebook,ANA_IOST_LABEL,ANA_BASE_COL_NUM,
				value_str,ANA_IOST_COL_NUM,VBOX_LAB_SPACING,2,OIX_CSTR_IOST);  	
    			break;
    			
    		case	POL_TYPE:
			case	SOE_TYPE:
			case	SYSMSG_TYPE:
			case    OUTS_TYPE:
			/****   基本信息****/ 
				DspPageInfo(crt_n,FALSE,notebook,ANA_BASE_INFO_LABEL,0,
					value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);		
				GetPolRtState(crt_n,&pt_dms_cmm[crt_n],rt_val);			
				DspPageInfo(crt_n,FALSE,notebook,POL_RTSTATE_LABEL,0,rt_val,POL_RTSTATE_COL_NUM,OIX_CSTR_RT_STATE);	
			    	 /****   I/O点属性****/      	 
    	 		DspIOPageInfo(crt_n,notebook,ANA_IOATTR_LABEL,ANA_BASE_COL_NUM+ANA_IOST_COL_NUM,
				value_str,ANA_IOATTR_COL_NUM,VBOX_LAB_SPACING,2,OIX_CSTR_IOATTR); 
    			/****   I/O点状态****/     	
    			DspIOPageInfo(crt_n,notebook,ANA_IOST_LABEL,ANA_BASE_COL_NUM,
				value_str,ANA_IOST_COL_NUM,VBOX_LAB_SPACING,2,OIX_CSTR_IOST); 		
    			break;
    				
    		case    IMP_TYPE:
    			/****   基本信息****/ 
				DspPageInfo(crt_n,FALSE,notebook,ANA_BASE_INFO_LABEL,0,
					value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);	
    		 	GetIMPRtVal(crt_n,&pt_dms_cmm[crt_n],imp_str);    		 	
    		 	DspPageInfo(crt_n,FALSE,notebook,IMP_RTVALUE_LABEL,0,imp_str,IMP_RTVALUE_COL_NUM,OIX_CSTR_RT_VALUE);
    		 	GetImpParm(crt_n,&pt_dms_cmm[crt_n],imp_str);    		 		
				DspPageInfo(crt_n,TRUE,notebook,IMP_PARA_LABEL,0,imp_str,IMP_PARA_COL_NUM,OIX_CSTR_PARAM);
    			break;	
    			
    		case	CHARA_TYPE:
    			/****   基本信息****/ 
				DspPageInfo(crt_n,FALSE,notebook,ANA_BASE_INFO_LABEL,0,
					value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);	
    			GetCharaParm(crt_n,&pt_dms_cmm[crt_n],imp_str);				
				DspPageInfo(crt_n,FALSE,notebook,CHARA_PARA_LABEL,0,imp_str,CHARA_PARA_COL_NUM,OIX_CSTR_PARAM);
			break;

		case	CURVE_TYPE:
		        GetCurveParm(crt_n,&pt_dms_cmm[crt_n],rt_val);
        		DspPageInfo(crt_n,FALSE,notebook,CURVE_PARA_LABEL,0,
				rt_val,CURVE_PARA_COL_NUM,OIX_CSTR_RT_VALUE);
				GetCurveSegVal(crt_n,&pt_dms_cmm[crt_n],curve_val);
				DspCurvePageInfo(crt_n,notebook,pt_dms_cmm[crt_n].var.curve.seg_num,curve_val);	
			break;
                        
            case    DEVOBJ_TYPE:
                /****   基本信息****/ 
				DspPageInfo(crt_n,FALSE,notebook,ANA_BASE_INFO_LABEL,0,
					value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);		
			/****   I/O点属性****/      	 
    	 		DspIOPageInfo(crt_n,notebook,ANA_IOATTR_LABEL,ANA_BASE_COL_NUM+ANA_IOST_COL_NUM,
				value_str,ANA_IOATTR_COL_NUM,VBOX_LAB_SPACING,2,OIX_CSTR_IOATTR); 
    			/****   I/O点状态****/     	
    			DspIOPageInfo(crt_n,notebook,ANA_IOST_LABEL,ANA_BASE_COL_NUM,
				value_str,ANA_IOST_COL_NUM,VBOX_LAB_SPACING,2,OIX_CSTR_IOST); 
                        
                GetObjParam(crt_n,&pt_dms_cmm[crt_n],val_obj,label_obj,&col_num);
                DspObjPageInfo(crt_n,notebook,val_obj,label_obj,col_num,OIX_CSTR_CMD_INFO);
                GetFbStatus1(crt_n,&pt_dms_cmm[crt_n],val_obj,label_obj,&col_num);
                DspObjPageInfo(crt_n,notebook,val_obj,label_obj,col_num,OIX_CSTR_FB_STATUS1);
                GetFbStatus2(crt_n,&pt_dms_cmm[crt_n],val_obj,label_obj,&col_num);
                DspObjPageInfo(crt_n,notebook,val_obj,label_obj,col_num,OIX_CSTR_FB_STATUS2);
                GetFbParam(crt_n,&pt_dms_cmm[crt_n],val_obj,label_obj,&col_num);
                DspObjPageInfo(crt_n,notebook,val_obj,label_obj,col_num,OIX_CSTR_FB_PARAM);
            break;

	case TIME_TYPE:
		/****   基本信息****/ 
		DspPageInfo(crt_n,FALSE,notebook,ANA_BASE_INFO_LABEL,0,
		value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);	
		
		/****   实时值****/  
		GetTimeVal(crt_n,&pt_dms_cmm[crt_n],rt_val);    			
		DspPageInfo(crt_n,FALSE,notebook,TIME_VALUE_LABEL,0,
			rt_val,TIME_VALUE_COL_NUM,OIX_CSTR_RT_VALUE);	

		/****   I/O点属性****/      	 
		DspIOPageInfo(crt_n,notebook,ANA_IOATTR_LABEL,ANA_BASE_COL_NUM+ANA_IOST_COL_NUM,
			value_str,ANA_IOATTR_COL_NUM,VBOX_LAB_SPACING,2,OIX_CSTR_IOATTR); 
		/****   I/O点状态****/     	
		DspIOPageInfo(crt_n,notebook,ANA_IOST_LABEL,ANA_BASE_COL_NUM,
		value_str,ANA_IOST_COL_NUM,VBOX_LAB_SPACING,2,OIX_CSTR_IOST); 
		default:
			break;
    	}  	
}

int UpdateDspInfo(GtkWidget *parent)
{
	char	entry_str[50][64],rt_val[50][POINT_LONGNAME_SIZE],lmt_val[50][POINT_LONGNAME_SIZE];
	char 	value_str[50][POINT_LONGNAME_SIZE],imp_str[50][POINT_LONGNAME_SIZE],curve_val[300][POINT_LONGNAME_SIZE];
	char    val_obj[50][POINT_LONGNAME_SIZE],label_obj[50][POINT_LONGNAME_SIZE];
	char  EntryName[100] ;char *text ;
	char  PtInfoTitle[150] ;
	char LongName[100];
	int		i,col_num;
	POINTER pt;
	int		crt_n;    
	
	GetCrtNumberOfWgt(parent, &crt_n);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return FALSE;
        
	pt = db_detail_pt[crt_n];
	
	if ( ReadEntryById ( &pt, &pt_dms_cmm[crt_n] ) == -1 )
	{
		if(db_detail_info[crt_n])
			DestroyDetailInfoBox(db_detail_info[crt_n],GINT_TO_POINTER(crt_n));
		PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return FALSE;
	}
		
	i=0;        
	GetDbDetailFixedInfo(&pt_dms_cmm[crt_n], &i,entry_str, value_str );
        
	if ( GetPtNameById(&pt,EntryName,NULL)  == -1 )
	{		
		PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return FALSE;
	}
	if ( GetPtLongnameById ( &pt, LongName ) == -1 )
	{		
        PopupMessageBox(parent,OIX_STR_DB_NO_POINT);
		return FALSE;
	}
    sprintf (PtInfoTitle, "%s-%s",LongName , EntryName)  ;
    gtk_window_set_title(GTK_WINDOW(db_detail_info[crt_n]), text = _toUtf8(PtInfoTitle)); 
		G_SAFE_FREE(text);//dzy1017
   	
    	switch(pt_dms_cmm[crt_n].point.data_type)
    	{
    		case	ANA_TYPE:
    		case	PARAM_TYPE:
    		case	OUTA_TYPE: 
			case	CALC_TYPE:
			/****   基本信息****/ 
		   
			UpdateDspPageInfo(crt_n,FALSE,0,
			value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);
		/****   实时值****/  
    		GetAnaRtVal(crt_n,&pt_dms_cmm[crt_n],rt_val);    			
			UpdateDspPageInfo(crt_n,FALSE,0,
			rt_val,ANA_RTVALUE_COL_NUM,OIX_CSTR_RT_VALUE);	

		/****   限值****/ 
			GetAnaLmtVal(crt_n,&pt_dms_cmm[crt_n],lmt_val);    			
			UpdateDspPageInfo(crt_n,TRUE,0,lmt_val,ANA_LMT_COL_NUM,OIX_CSTR_LMT_VAL);
		/****   I/O点属性****/      	 
    	 	UpdateDspIOPageInfo(crt_n,ANA_BASE_COL_NUM+ANA_IOST_COL_NUM,value_str,ANA_IOATTR_COL_NUM); 
    	/****   I/O点状态****/     	
    		UpdateDspIOPageInfo(crt_n,ANA_BASE_COL_NUM,value_str,ANA_IOST_COL_NUM);  	
    		break;
    		
    		case	POL_TYPE:
			case	SOE_TYPE:
			case	SYSMSG_TYPE:
			case    OUTS_TYPE:
			/****   基本信息****/ 
				UpdateDspPageInfo(crt_n,FALSE,0,
					value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);		
				GetPolRtState(crt_n,&pt_dms_cmm[crt_n],rt_val);			
				UpdateDspPageInfo(crt_n,FALSE,0,rt_val,POL_RTSTATE_COL_NUM,OIX_CSTR_RT_STATE);	
			/****   I/O点属性****/      	 
    	 		UpdateDspIOPageInfo(crt_n,ANA_BASE_COL_NUM+ANA_IOST_COL_NUM,
				value_str,ANA_IOATTR_COL_NUM); 
    		/****   I/O点状态****/     	
    			UpdateDspIOPageInfo(crt_n,ANA_BASE_COL_NUM,
				value_str,ANA_IOST_COL_NUM); 		
    			break;
    				
    		case    IMP_TYPE:
    		/****   基本信息****/ 
				UpdateDspPageInfo(crt_n,FALSE,0,
					value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);	
    		 	GetIMPRtVal(crt_n,&pt_dms_cmm[crt_n],imp_str);    		 	
    		 	UpdateDspPageInfo(crt_n,FALSE,0,imp_str,IMP_RTVALUE_COL_NUM,OIX_CSTR_RT_VALUE);
    		 	GetImpParm(crt_n,&pt_dms_cmm[crt_n],imp_str);    		 		
				UpdateDspPageInfo(crt_n,TRUE,0,imp_str,IMP_PARA_COL_NUM,OIX_CSTR_PARAM);
    			break;	
    			
    		case	CHARA_TYPE:
    			/****   基本信息****/ 
				UpdateDspPageInfo(crt_n,FALSE,0,
					value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);	
    			GetCharaParm(crt_n,&pt_dms_cmm[crt_n],imp_str);				
				UpdateDspPageInfo(crt_n,FALSE,0,imp_str,CHARA_PARA_COL_NUM,OIX_CSTR_PARAM);
				break;

			case	CURVE_TYPE:
		        GetCurveParm(crt_n,&pt_dms_cmm[crt_n],rt_val);
        		UpdateDspPageInfo(crt_n,FALSE,0,
				rt_val,CURVE_PARA_COL_NUM,OIX_CSTR_RT_VALUE);
				GetCurveSegVal(crt_n,&pt_dms_cmm[crt_n],curve_val);
				UpdateDspCurvePageInfo(crt_n,pt_dms_cmm[crt_n].var.curve.seg_num,curve_val);	
				break;
                        
            case    DEVOBJ_TYPE:
                    /****   基本信息****/                     
				UpdateDspPageInfo(crt_n,FALSE,0,
					value_str,ANA_BASE_COL_NUM,OIX_CSTR_BASE_INFO);		
			
			    	 /****   I/O点属性****/      	 
    	 		UpdateDspIOPageInfo(crt_n,ANA_BASE_COL_NUM+ANA_IOST_COL_NUM,
				value_str,ANA_IOATTR_COL_NUM);
                        
    			/****   I/O点状态****/     	
    			UpdateDspIOPageInfo(crt_n,ANA_BASE_COL_NUM,
				value_str,ANA_IOST_COL_NUM);                         
                        
				GetObjParam(crt_n,&pt_dms_cmm[crt_n],val_obj,label_obj,&col_num);                       
                UpdateDspPageInfo(crt_n,FALSE,0,val_obj,col_num,OIX_CSTR_CMD_INFO);
                        
                GetFbStatus1(crt_n,&pt_dms_cmm[crt_n],val_obj,label_obj,&col_num);
                UpdateDspPageInfo(crt_n,FALSE,0,val_obj,col_num,OIX_CSTR_FB_STATUS1);
                GetFbStatus2(crt_n,&pt_dms_cmm[crt_n],val_obj,label_obj,&col_num);
                UpdateDspPageInfo(crt_n,FALSE,0,val_obj,col_num,OIX_CSTR_FB_STATUS2);
            break;
		default:
			break;
    	}  	
	return TRUE;
}
