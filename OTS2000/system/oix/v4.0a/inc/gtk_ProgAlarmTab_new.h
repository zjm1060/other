#ifndef _PROGALARMTAB_NEW_H
#define _PROGALARMTAB_NEW_H

#define SHOW_NONE    100



typedef struct 
{
	PAGE_INFO		page[MAX_CRT_NUMBER];
	GdkGC  		*alm_draw_gc[MAX_CRT_NUMBER];
	GdkPixmap 	*AlmOffDrawable[MAX_CRT_NUMBER];
	//GtkWidget 	*alm_pop_shell[MAX_CRT_NUMBER];
GtkWidget 	*alm_canvas[MAX_CRT_NUMBER];

UINT    	devMask[MAX_CRT_NUMBER];// = {0xffffffff,0xffffffff,0xffffffff};
UINT		groupMask[MAX_CRT_NUMBER][MAX_GROUP_MASK_NUM] ;/*= {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff};
*/
UINT		attribMask[MAX_CRT_NUMBER][2];// = {0,0,0,0,0,0};
int     	alm_rec_cnt[MAX_CRT_NUMBER];
gint		isLastPage[MAX_CRT_NUMBER];//={0,0,0};

GtkListStore *storeFilter_a[MAX_CRT_NUMBER],*storeDev_a[MAX_CRT_NUMBER],*storeGroup_a[MAX_CRT_NUMBER];
int	filterNum,curFilter;

GtkWidget       *filterDlg[MAX_CRT_NUMBER] ;
GtkWidget		*AttribFilterDlg[MAX_CRT_NUMBER];

FILTER_T 	FilterAlarm[MAX_CRT_NUMBER];
FILTER_T FilterAlarm1[MAX_CRT_NUMBER];
FILTER_T 	FilterAlarmFault[MAX_CRT_NUMBER];
//GtkWidget       *filterDlg[MAX_CRT_NUMBER] ;
//GtkWidget       *HistDBDlg[MAX_CRT_NUMBER] ;
//GtkWidget       *HistFileDBDlg[MAX_CRT_NUMBER] ;

GtkWidget 	*item[MAX_CRT_NUMBER][GROUP_ENTRY_NUMBER];
GtkWidget	*DataTypeItem[MAX_CRT_NUMBER][MAX_DATATYPE_NUM + 2];
GtkWidget	*DevItem[MAX_CRT_NUMBER][SHOW_NONE];
GtkWidget	*attribCheck[MAX_CRT_NUMBER][MAX_ATTRIB_NUM],*attribToggle[MAX_CRT_NUMBER][MAX_ATTRIB_NUM];
gint 	alm_timeout_id[MAX_CRT_NUMBER]  ;
//gint 	fault_alm_timeout_id[MAX_CRT_NUMBER]  ;
STNDEF	alarmStnDef[STATION_ENTRY_NUMBER];
gboolean	auto_filter_flag[MAX_CRT_NUMBER];
  BOOL	alm_exit_flag[MAX_CRT_NUMBER] ;//= {FALSE,FALSE,FALSE};
INDEX   	index_tmp[MAX_CRT_NUMBER];
INDEX index_tmp1[MAX_CRT_NUMBER];
INDEX   	index_fault[MAX_CRT_NUMBER];
SEL_PT   	sel_pt[MAX_CRT_NUMBER];
int			iHisNameMode[MAX_CRT_NUMBER] ;
GtkAdjustment  *adj1[MAX_CRT_NUMBER] ;	
}PROGALARMTAB_PARA;

PROGALARMTAB_PARA Proalarmtab_para; 

GSList * ListProalarmtab_para[MAX_CRT_NUMBER];

#endif _PROGALARMTAB_NEW_H