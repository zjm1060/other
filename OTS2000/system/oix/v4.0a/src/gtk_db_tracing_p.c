/*
By zyp for 4.0a  ;
2007.06.08 UNIX WINDOWS  SYNCRONIZE  by chichi  ;
LAST UPDATE : 2007.06.24  by chi 
*/


#include <gtk/gtk.h>
#include <gdk/gdk.h>



#define	DEBUG	

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../inc/oix.h"



/*#define 	ALM_ROW      30*/


int	db_pid_cnt[MAX_CRT_NUMBER]={0,0,0};
int	db_page_numb[MAX_CRT_NUMBER]={0,0,0};
int	db_page_cnt[MAX_CRT_NUMBER]={0,0,0};

GtkWidget *db_pop_shell[MAX_CRT_NUMBER]= {NULL,NULL,NULL},
	*db_detail_info[MAX_CRT_NUMBER]= {NULL,NULL,NULL},
	*db_tab_canvas[MAX_CRT_NUMBER]= {NULL,NULL,NULL},
	*db_detail_info_canvas[MAX_CRT_NUMBER]= {NULL,NULL,NULL},
	*db_io_set_box[MAX_CRT_NUMBER]= {NULL,NULL,NULL};

GtkWidget *db_scrolled_window, *cmbDbDev,*db_detail_info_scrolled_window;

GdkGC *db_draw_gc[MAX_CRT_NUMBER],*db_detail_draw_gc[MAX_CRT_NUMBER];


POINTER		db_selected_ptr[MAX_CRT_NUMBER];	/* for db tracing	*/
int	db_select_type[MAX_CRT_NUMBER]; 
char		name_item[POINT_LONGNAME_SIZE];

BACKRCD	db_tag_rcd[MAX_CRT_NUMBER];			/* db io tagged item rcd	*/
int	db_tag[MAX_CRT_NUMBER]; 


gchar	*button_titles[]={
	OIX_CSTR_DeadBandSet,	OIX_CSTR_HH_LMT_Set, OIX_CSTR_H_LMT_Set,OIX_CSTR_L_LMT_Set,
		OIX_CSTR_LL_LMT_Set,
		OIX_CSTR_H_RecorerySet, OIX_CSTR_L_RecorerySet, OIX_CSTR_EngValueSet,
		OIX_CSTR_HiRangeSet, OIX_CSTR_LoRangeSet, OIX_CSTR_BaseSet, OIX_CSTR_CompensateSet,
		OIX_CSTR_TrWarnset,OIX_CSTR_TrRecoverset,OIX_CSTR_ClsDevice,OIX_CSTR_OpnDevice,
		OIX_CSTR_AlarmAck,OIX_CSTR_AlarmEnable,OIX_CSTR_AlarmDisable,OIX_CSTR_ScanEnable,
		OIX_CSTR_ScanDisable,OIX_CSTR_RTValueSet,OIX_STR_FORCEON,OIX_STR_FORCEOFF,OIX_STR_UNFORCE,
		OIX_STR_UNFORCE,OIX_STR_ANA_FORCE
};	

gchar *ANA_titles[] = {
	OIX_STR_IO_ADDRESS,OIX_STR_ANALOG_NAME,OIX_STR_UPDATE_TIME,OIX_STR_RT_VALUE,
		OIX_STR_H_ALM_LMT, OIX_STR_H_WRN_LMT,OIX_STR_L_WRN_LMT,
		OIX_STR_L_ALM_LMT, OIX_STR_H_RECOVER,OIX_STR_L_RECOVER,
		OIX_STR_HI_RANGE,  OIX_STR_LO_RANGE, OIX_STR_BASE,
		OIX_STR_COMPENSATE,OIX_STR_DBAND,OIX_STR_ENG_FACTOR,
		OIX_STR_DATA_QUALITY,OIX_STR_SCAN,OIX_STR_ALM,OIX_CSTR_AlarmAck
}; 

gchar *TEMP_titles[] = {
	OIX_STR_IO_ADDRESS,OIX_STR_TEMP_NAME,OIX_STR_MAX_POINT,
		OIX_STR_MAX,OIX_STR_MIN_POINT,OIX_STR_MIN,OIX_STR_MEAN,
		OIX_STR_VARIANCE,OIX_STR_MAX_RATE_POINT,OIX_STR_MAX_RATE,
		OIX_STR_MIN_RATE_POINT,OIX_STR_MIN_RATE
};

gchar *POL_titles[] = {
	OIX_STR_IO_ADDRESS,OIX_STR_SCAN_NAME,OIX_STR_UPDATE_TIME,OIX_STR_RT_STATUS,
	OIX_CSTR_CHANNEL_QULITY,OIX_CSTR_MAN_SET,OIX_STR_SCAN,OIX_STR_ALM,OIX_CSTR_AlarmAck/*,OIX_CSTR_GROUP_NO,
	OIX_CSTR_TERMINAL,OIX_CSTR_UNIT_NO,OIX_CSTR_SLOT,OIX_CSTR_CHANNEL*/
};

gchar *IMP_titles[] = {
	OIX_STR_IO_ADDRESS,OIX_STR_UPDATE_TIME,OIX_CSTR_NAME,
		OIX_STR_PULSE_NAME,OIX_STR_REAL_ENERGY,
		/*OIX_STR_BASE_ENERGY,*/OIX_STR_ACCUM ,OIX_STR_DAILY_PPEAK_ENERGY,
		OIX_STR_DAILY_PEAK_ENERGY,OIX_STR_DAILY_MEAN_ENERGY,
		OIX_STR_DAILY_VALLEY_ENERGY,OIX_STR_DAILY_TTL_ENERGY
};

gchar *CURVE_titles[]={
	OIX_STR_IO_ADDRESS,OIX_STR_CURVE_NAME, OIX_STR_UPDATE_TIME,OIX_STR_SEGMENT_NUM,OIX_STR_HI_RANGE,
		OIX_STR_LO_RANGE,OIX_STR_BASE,OIX_STR_COMPENSATE,OIX_STR_DBAND,OIX_STR_ENG_FACTOR
};






extern unsigned short opr_state[50];
extern PAGE_INFO		pageDb[MAX_CRT_NUMBER]; 
extern void	FillIostCmdBuf ( int crt_n, DMS_COMMON	*dms_cmm, CMD_STATUS cmd_status,short io_type );

void	UnmanageDbIoBox ( GtkWidget *w );

void	DbIoBtnPressCB (GtkWidget * window, gpointer data);
void 	PopupDetailInfoBox(GtkWidget* , int crt_n, POINTER pt);
extern void	ProgSetValue (int crt_n,GtkWidget*parent,DMS_COMMON dms_cmm);

extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern int    gdk_rgb(int red  ,int green, int  blue , GdkColor *clr  );
extern int  DbDspProc( int crt_n  );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern short   GetCurWSCtrlPrvg();
extern int 	PopupMessageBoxRet (GtkWidget *parent,char msg[] );
extern void FormCOnOffForcePacket (int crt_n, DMS_COMMON *int_data, char	*oper_status );
extern void	ProgDoAckCel ( int crt_n );
extern void FormCAnaForcePacket (int crt_n, DMS_COMMON *int_data, char	*oper_status );
extern PopupValueEntryBox (GtkWidget *parent ,int crt_n ,POINTER *pt,int entry_data_class);
extern void 	CmdBufInit ( int crt_n );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern void ModifyDataClass(int crt_n,GtkWidget *parent, DMS_COMMON	*dms_cmm, POINTER	*entry_ptr,int entry_data_class);
extern char *_toUtf8(char *c);
extern char *_toUtf8EX(char *c);

void	PopupIoMgrBox (int crt_n ,POINTER pt);
BOOL	GetForceRight (int crt_n,DMS_COMMON dms_cmm,int IsCascade);

int db_detail_timeout_id[MAX_CRT_NUMBER];
int	db_io_mgr_timeout_id[MAX_CRT_NUMBER];
//GdkPixmap *DbDetailOffDrawable[MAX_CRT_NUMBER];

extern DB_TAB_WINDOW_INFO db_tab_window_info[];

#define	MAX_DB_DETAIL_ITEM_NUM	100



void DestroyDetailInfoBox(GtkWidget *w,gpointer data)
{	
	int crt_n= (int)data;
	
	if(db_detail_timeout_id[crt_n]!=0)
	{
		gtk_timeout_remove(db_detail_timeout_id[crt_n]);
		db_detail_timeout_id[crt_n]= 0;
	}
	if(db_detail_info[crt_n])
	{
		gtk_widget_destroy(db_detail_info[crt_n]);	
		db_detail_info[crt_n] =NULL;
	}
}


void	UnmanageDbIoBox ( GtkWidget *w  )
{
	int		crt_n;
	
	if ( w == NULL ) 
		return;
	
	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 || crt_n>=MAX_CRT_NUMBER)
		return;
	
	if(db_io_mgr_timeout_id[crt_n]!=0)
	{
		gtk_timeout_remove(db_io_mgr_timeout_id[crt_n]);
		db_io_mgr_timeout_id[crt_n]= 0;
	}
	if(db_io_set_box[crt_n])
	{	
		gtk_widget_destroy(db_io_set_box[crt_n]);
		db_io_set_box[crt_n]=NULL;
		db_tab_window_info[crt_n].subwindow_select_flag= 0;
	}	
	
	db_select_type[crt_n] = SEL_NO_DB_PNT;
	
}


void UnmanageDbIoandInfoBox(int crt_n)
{
	DestroyDetailInfoBox(db_detail_info[crt_n], GINT_TO_POINTER(crt_n));
	UnmanageDbIoBox(db_io_set_box[crt_n]);
}
void DrawDbItemTagNew ( int crt_n, GdkDrawable *drawable,GdkGC  *gc ,int x ,int y )
{
	
	GdkColor  bkColor ;
    gdk_rgb( 150,150,200,&bkColor )  ;
    gdk_gc_set_foreground ( gc, &bkColor);
    gdk_draw_rectangle(drawable,gc,TRUE, (int)(x*xscale[crt_n]),(int)(y*yscale[crt_n]),(int)(1290*xscale[crt_n]),(int)((ALM_ROW-4)*yscale[crt_n]));    
}
void DrawDbItemTag ( int crt_n, GdkDrawable *drawable,GdkGC  *gc )
{
	int	y_med;
	GdkColor  bkColor ;
	return ;
	if ( db_tag[crt_n] != ITEM_TAG_ON )
		return;
	
	//	y_med	= ( db_tab_window_info[crt_n].select_pt.pt_id - db_page_cnt[crt_n]*DBTAB_PAGE_SIZE )*DBH0 + DBY0;
	y_med	= ( pageDb[crt_n].SelectEnd - pageDb[crt_n].CurPage *pageDb[crt_n].PageSize )*DBH0 + DBY0;
	
	//	y_med	= ( pageDb[crt_n].SelectStart   )*DBH0 + DBY0;
	y_med -= 20 ;
	
    if (y_med >1500 ) return ;
    gdk_rgb( 150,150,200,&bkColor )  ;
    gdk_gc_set_foreground ( gc, &bkColor);
    gdk_draw_rectangle(drawable,gc,TRUE, (int)(DB_TAG_RECT_X1*xscale[crt_n]),(int)(y_med*yscale[crt_n]),(int)(1290*xscale[crt_n]),(int)((ALM_ROW-4)*yscale[crt_n]));    
}
int	SetPointNameLabel(GtkWidget *label)
{
	char	LongName[128];
	int		crt_n;
	gchar *pConvert=0;
	GetCrtNumberOfWgt(label, &crt_n);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return 1; 
	
	if(GetPtLongnameById(&db_tab_window_info[crt_n].select_pt, LongName ) == -1 )
		return 1;
	gtk_label_set_text(GTK_LABEL(label), pConvert=_toUtf8(LongName));
	G_SAFE_FREE(pConvert);//dzy1017
	
	return 1;
}

void	DbTracingCB (GtkWidget * window, GdkEventButton *event,	gpointer data )
{	
	int	id, crt_n,y,cur_page_startid,cur_page_endid;
	extern POINTER		db_detail_pt[MAX_CRT_NUMBER];
	GtkWidget  *label ; 
	int rowNum ; 
	crt_n= (int)data;
	if( crt_n < 0 )
		return;
	
	cur_page_startid= db_page_cnt[crt_n] * DBTAB_PAGE_SIZE;
	cur_page_endid	= MIN (( db_page_cnt[crt_n]+1)*DBTAB_PAGE_SIZE, db_pid_cnt[crt_n] );
	if (event->type==GDK_BUTTON_PRESS || event->type==GDK_2BUTTON_PRESS) 
	{
		
		
		
		y = (int)(event->y /yscale[crt_n] );
		
		id= ( y - DBY0 + DBH0/2)/DBH0;  
		
        pageDb[crt_n].SelectStart = id ; 
        rowNum = id ; 
		
		id  = rowNum + pageDb[crt_n].PageStart ; 	
		if ( id < pageDb[crt_n].PageStart || id > pageDb[crt_n].PageEnd)
		{	
			gdk_beep ( );					
			return;
		}
		
		
		pageDb[crt_n].SelectEnd = id ;	
		db_tab_window_info[crt_n].select_pt.pt_id= id;
		if(db_detail_info[crt_n])
			db_detail_pt[crt_n]= db_tab_window_info[crt_n].select_pt;
		
		db_tag[crt_n]= ITEM_TAG_ON;
		DbDspProc(crt_n);
		if (db_io_set_box[crt_n])
		{
			label = (GtkWidget*)gtk_object_get_data(GTK_OBJECT(db_io_set_box[crt_n]),"PtLabel");
			SetPointNameLabel(label) ;
		}
		if(event->type==GDK_BUTTON_PRESS && event->button==3)
			PopupDetailInfoBox(db_pop_shell[crt_n],crt_n, db_tab_window_info[crt_n].select_pt);
		else if(event->type==GDK_2BUTTON_PRESS && event->button==1)
			PopupIoMgrBox ( crt_n ,db_tab_window_info[crt_n].select_pt);
	}
}





void	PopupIoMgrBox (int crt_n ,POINTER pt)
{
	GtkWidget *label,*button,*hbox;
	int i;		
    LCU_ID  lcu_id;
    gchar *pConvert=0;
	
	lcu_id.stn_id= db_tab_window_info[crt_n].select_pt.stn_id;
	lcu_id.dev_id= db_tab_window_info[crt_n].select_pt.dev_id;
	if(CheckHostPrivilege(local_hostname, lcu_id)==0)
	{
		 PopupMessageBox ( db_pop_shell[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		 return ;
	}

	if (db_tab_window_info[crt_n].select_pt.data_type==IMP_TYPE
		||db_tab_window_info[crt_n].select_pt.data_type==CALC_TYPE
		||db_tab_window_info[crt_n].select_pt.data_type==CURVE_TYPE
		||db_tab_window_info[crt_n].select_pt.data_type==CHARA_TYPE
		||db_tab_window_info[crt_n].select_pt.data_type==PARAM_TYPE
		||db_tab_window_info[crt_n].select_pt.data_type==DEVOBJ_TYPE)
		return ;

	if( db_tab_window_info[crt_n].subwindow_select_flag==1 )
		return;
	db_tab_window_info[crt_n].subwindow_select_flag= 1;

	if(db_detail_info[crt_n])
		DestroyDetailInfoBox(db_detail_info[crt_n],(gpointer)crt_n);

    if(db_io_set_box[crt_n])
    {
		gtk_window_present(GTK_WINDOW(db_io_set_box[crt_n]));
		return;
    }

	db_io_set_box[crt_n]= gtk_dialog_new() ;

	if(db_tab_window_info[crt_n].select_pt.data_type==ANA_TYPE)
		gtk_widget_set_usize(db_io_set_box[crt_n],250,550);
	else
		gtk_widget_set_usize(db_io_set_box[crt_n],250,250);
    
	gtk_window_set_policy(GTK_WINDOW(db_io_set_box[crt_n]),FALSE,FALSE,FALSE);
                
	if(db_pop_shell[crt_n]!=NULL)
	{
		gtk_window_set_transient_for(GTK_WINDOW(db_io_set_box[crt_n]),GTK_WINDOW(db_pop_shell[crt_n]));
		gtk_window_set_destroy_with_parent(GTK_WINDOW(db_io_set_box[crt_n]),TRUE);                    
	}
		
	gtk_window_set_title(GTK_WINDOW(db_io_set_box[crt_n]),pConvert=_toUtf8EX(DB_IO_SetBoxTitle));
	G_SAFE_FREE(pConvert);//dzy1017
	label = gtk_label_new("");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),label,TRUE,TRUE,0);		
	SetPointNameLabel(label);
    db_io_mgr_timeout_id[crt_n]= gtk_timeout_add (1000, (GtkFunction)SetPointNameLabel, (gpointer)label);	 
    gtk_object_set_data((GtkObject*)db_io_set_box[crt_n],"PtLabel" ,(void *)label ) ;
	hbox = gtk_hbox_new (TRUE, 0);
		
	button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_AlarmAck));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_set_usize(button,20,10);			
	gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
	gtk_box_pack_start( GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox) ,hbox,TRUE,TRUE,0);		
	gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC(DbIoBtnPressCB),  GINT_TO_POINTER(DB_IO_ALM_ACK));
		
	if(  db_tab_window_info[crt_n].select_pt.data_type!=ANA_TYPE)  /*chichi*/
	{	  
		if(db_tab_window_info[crt_n].select_pt.data_type!=OUTS_TYPE
			&&db_tab_window_info[crt_n].select_pt.data_type!=OUTA_TYPE)
		{
			for(i=DB_IO_MAN_SET_ON;i<=DB_IO_MAN_SET_OFF;i++)
			{
				if(i%2==0)
				{
					hbox = gtk_hbox_new (FALSE, 0);	
					gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),hbox,TRUE,TRUE,0);
				}
				button = gtk_button_new_with_label(pConvert=_toUtf8EX(button_titles[i]));	
				G_SAFE_FREE(pConvert);//dzy1017
				gtk_widget_set_usize(button,20,10);			
				gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);		
				gtk_signal_connect (GTK_OBJECT (button), "clicked",	GTK_SIGNAL_FUNC(DbIoBtnPressCB), GINT_TO_POINTER(i));
			}
                        if(IsIoForceDisable()==True)
                        {
                            for(i=   DB_IO_OUTS_FORCE_ON;i<=  DB_IO_OUTS_UNFORCE;i++)
                            {
				if(i%2==0)
				{
					hbox = gtk_hbox_new (FALSE, 0);	
					gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),hbox,TRUE,TRUE,0);
				}
				button = gtk_button_new_with_label(pConvert=_toUtf8EX(button_titles[i]));	
				G_SAFE_FREE(pConvert);//dzy1017
				gtk_widget_set_usize(button,20,10);			
				gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);		
				gtk_signal_connect (GTK_OBJECT (button), "clicked",	GTK_SIGNAL_FUNC(DbIoBtnPressCB), GINT_TO_POINTER(i));
                            }
                        }
		}
                if(IsIoForceDisable()==True)
                {
                    if(db_tab_window_info[crt_n].select_pt.data_type==OUTS_TYPE)
                    {
			for(i=   DB_IO_OUTS_FORCE_ON;i<=  DB_IO_OUTS_UNFORCE;i++)
			{
				if(i%2==0)
				{
					hbox = gtk_hbox_new (FALSE, 0);	
					gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),hbox,TRUE,TRUE,0);
				}
				button = gtk_button_new_with_label(pConvert=_toUtf8EX(button_titles[i]));	
				G_SAFE_FREE(pConvert);//dzy1017
				gtk_widget_set_usize(button,20,10);			
				gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);		
				gtk_signal_connect (GTK_OBJECT (button), "clicked",	GTK_SIGNAL_FUNC(DbIoBtnPressCB), GINT_TO_POINTER(i));
			}
                    }
                    else if(db_tab_window_info[crt_n].select_pt.data_type==OUTA_TYPE)
                    {
			for(i=   DB_IO_OUTA_UNFORCE;i<=  DB_IO_OUTA_FORCE_VAL;i++)
			{
				if(i%2!=0)                                   
					hbox = gtk_hbox_new (FALSE, 0);	
                                         
				button = gtk_button_new_with_label(pConvert=_toUtf8EX(button_titles[i]));	
				G_SAFE_FREE(pConvert);//dzy1017
				gtk_widget_set_usize(button,20,10);			
				gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);		
				gtk_signal_connect (GTK_OBJECT (button), "clicked",	GTK_SIGNAL_FUNC(DbIoBtnPressCB), GINT_TO_POINTER(i));
				gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),hbox,TRUE,TRUE,0);
			}
                    }
		}
	}
		
	for(i=DB_IO_ALM_ENABLE;i<=DB_IO_SCAN_DISABLE;i++)
	{
		if(i%2!=0)
			hbox = gtk_hbox_new (FALSE, 0);	
		       	       
		button = gtk_button_new_with_label(pConvert=_toUtf8EX(button_titles[i]));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_widget_set_usize(button,20,10);
		gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
		if((i%2)==0)	
			gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),hbox,TRUE,TRUE,0);	
		gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC(DbIoBtnPressCB), GINT_TO_POINTER(i));
	}   		
			       
	if(  db_tab_window_info[crt_n].select_pt.data_type==ANA_TYPE)   /*chichi*/
	{ 	
		hbox = gtk_hbox_new (FALSE, 0);				       
		button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_RTValueSet));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_widget_set_usize(button,20,10);	
		gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),hbox,TRUE,TRUE,0);	
		gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC(DbIoBtnPressCB),   GINT_TO_POINTER(DB_IO_VALUE_SET));

		for(i= DB_IO_DEAD_BAND_SET;i<=  DB_IO_TR_RECOVER_SET;i++)
        {
			hbox = gtk_hbox_new (FALSE, 0);	       
			button = gtk_button_new_with_label(pConvert=_toUtf8EX(button_titles[i]));
			G_SAFE_FREE(pConvert);//dzy1017
			gtk_widget_set_usize(button,20,10);	
			gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);	
			gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),hbox,TRUE,TRUE,0);
			gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC(DbIoBtnPressCB), GINT_TO_POINTER(i));
		}
                    
        for(i=   DB_IO_OUTA_UNFORCE;i<=  DB_IO_OUTA_FORCE_VAL;i++)
		{
			if(i%2!=0) 
            {
				hbox = gtk_hbox_new (FALSE, 0);	
				gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->vbox),hbox,TRUE,TRUE,0);
            }
                                   
			button = gtk_button_new_with_label(pConvert=_toUtf8EX(button_titles[i]));	
			G_SAFE_FREE(pConvert);//dzy1017
			gtk_widget_set_usize(button,20,10);	
			gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);	
			gtk_signal_connect (GTK_OBJECT (button), "clicked",	GTK_SIGNAL_FUNC(DbIoBtnPressCB), GINT_TO_POINTER(i));
		}
	}	       
		 	       
	button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);//dzy1017
	GTK_WIDGET_SET_FLAGS(button,GTK_CAN_DEFAULT);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_io_set_box[crt_n])->action_area),button,FALSE,FALSE,0);
	gtk_widget_grab_default(button);
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
		       GTK_SIGNAL_FUNC(UnmanageDbIoBox),
		       GTK_OBJECT (db_io_set_box[crt_n]));
	gtk_signal_connect(GTK_OBJECT(db_io_set_box[crt_n]),
	       "destroy",
	       GTK_SIGNAL_FUNC(UnmanageDbIoBox),
	       NULL);
	gtk_widget_show_all(db_io_set_box[crt_n]);		
	
	db_select_type[crt_n] = SEL_DB_PNT;
}
void DbSelectTypeInit (int crt_n )
{
	if( db_select_type[crt_n] == SEL_DB_PNT)
	{
		if( ( db_io_set_box[crt_n] ) !=  NULL )
			UnmanageDbIoBox ( db_io_set_box[crt_n] );
		else
			db_select_type[crt_n] = SEL_NO_DB_PNT;
	}
}

void	DbIoBtnPressCB (GtkWidget * window, gpointer data)
{
	int		rtn_num	= (int)data;
	int		crt_n;
	UCHAR	dev_id;
	LCU_ID	lcu_id;
	DMS_COMMON	dms_cmm;
	short   ws_ctrl_prvg;
	int		data_class;
	int     IsCascade=IsLocalHostDispatch();
    char  LongName[128];
	
	GetCrtNumberOfWgt ( window, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return; 
	 ws_ctrl_prvg=GetCurWSCtrlPrvg();

	if( ws_ctrl_prvg != 1)
    {
		if (ws_ctrl_prvg==-1 )
			PopupMessageBox ( db_io_set_box[crt_n], OIX_STR_DB_NO_POINT );
		else
			PopupMessageBox ( db_io_set_box[crt_n], MSG_HOST_NOT_ON );
		return;
    }
	if ( ReadEntryById  (&db_tab_window_info[crt_n].select_pt, &dms_cmm ) == -1)
	{
		printf("OIX: in :: %d.%d.%d.%d is not found in database.\n",
			db_tab_window_info[crt_n].select_pt.stn_id,db_tab_window_info[crt_n].select_pt.dev_id, db_tab_window_info[crt_n].select_pt.pt_id, 
			db_tab_window_info[crt_n].select_pt.data_type );
		return;
    }
    GetPtLongnameById ( &db_tab_window_info[crt_n].select_pt, LongName );
	
	if(	dms_cmm.fixed.iost.tagState)
	{
		PopupMessageBox ( db_io_set_box[crt_n], MSG_PROHIBIT_TO_OPERATE );
		return;
	}
	
	
	lcu_id.stn_id= db_tab_window_info[crt_n].select_pt.stn_id;
	lcu_id.dev_id= db_tab_window_info[crt_n].select_pt.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_io_set_box[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return;
	}
	switch	( rtn_num )
	{
	   case DB_IO_OUTS_FORCE_ON:
	   case DB_IO_OUTS_FORCE_OFF:
	   case DB_IO_OUTS_UNFORCE:
		   if(dms_cmm.point.data_type==OUTS_TYPE)
		   {
			   if(!CheckUserPrivilege(cur_usr_name, PRIV_OUT_FORCE_SET))
			   {
				   PopupMessageBox ( db_io_set_box[crt_n], MSG_OUT_FORCE_SETTING );
				   return;
			   }
		   }
		   else if(dms_cmm.point.data_type==SOE_TYPE || dms_cmm.point.data_type==POL_TYPE)
		   {
			   if(!CheckUserPrivilege(cur_usr_name, PRIV_IN_FORCE_SET))
			   {
				   PopupMessageBox (db_io_set_box[crt_n], MSG_IN_FORCE_SETTING );
				   return;
			   }
		   }
		   else
		   {
			   PopupMessageBox ( db_io_set_box[crt_n], MSG_OPR_DATATYPE_ERROR );
			   return;
		   }
		   
		   if(!GetForceRight(crt_n,dms_cmm,IsCascade))
			   return;
		   
		   UnmanageDbIoBox (  db_io_set_box[crt_n] );
		   if (PopupMessageBoxRet ( main_w[crt_n] , strcat(LongName ,_CS_(MSG_FORCE_INFO )))==GTK_RESPONSE_CANCEL)
			   return;
		   
		   if(rtn_num==DB_IO_OUTS_FORCE_ON)
			   FormCOnOffForcePacket ( crt_n, &dms_cmm, "ForceOn" );
		   else if(rtn_num==DB_IO_OUTS_FORCE_OFF)
			   FormCOnOffForcePacket ( crt_n, &dms_cmm, "ForceOff" );
		   else if(rtn_num==DB_IO_OUTS_UNFORCE)
			   FormCOnOffForcePacket ( crt_n, &dms_cmm, "UnForce" );
		   ProgDoAckCel ( crt_n );
		   break;
		   
	   case DB_IO_OUTA_UNFORCE:
	   case DB_IO_OUTA_FORCE_VAL:
		   if(dms_cmm.point.data_type==OUTA_TYPE)
		   {
			   if(!CheckUserPrivilege(cur_usr_name, PRIV_OUT_FORCE_SET))
			   {
				   PopupMessageBox ( db_io_set_box[crt_n], MSG_OUT_FORCE_SETTING );
				   return;
			   }
		   }
		   else if(dms_cmm.point.data_type==ANA_TYPE)
		   {
			   if(!CheckUserPrivilege(cur_usr_name, PRIV_IN_FORCE_SET))
			   {
				   PopupMessageBox ( db_io_set_box[crt_n], MSG_IN_FORCE_SETTING );
				   return;
			   }
		   }
		   else
		   {
			   PopupMessageBox ( db_io_set_box[crt_n], MSG_OPR_DATATYPE_ERROR );
			   return;
		   }
		   
		   if(!GetForceRight(crt_n,dms_cmm,IsCascade))
			   return;
		   
		   UnmanageDbIoBox (  db_io_set_box[crt_n] );
		   if (PopupMessageBoxRet ( main_w[crt_n] , strcat(LongName ,_CS_(MSG_FORCE_INFO) ))==GTK_RESPONSE_CANCEL)
			   return;
		   
		   if(rtn_num==DB_IO_OUTA_UNFORCE)
		   {
			   FormCAnaForcePacket ( crt_n, &dms_cmm, "UnForce" );
			   ProgDoAckCel ( crt_n ); 
		   }
		   else if(rtn_num==DB_IO_OUTA_FORCE_VAL)
		   {
			   FormCAnaForcePacket ( crt_n, &dms_cmm, "Force" );
			   PopupValueEntryBox (main_w[crt_n], crt_n , &dms_cmm.point,-1);
		   }
		   break;
		   
	   case	DB_IO_MAN_SET_ON:
	   case	DB_IO_MAN_SET_OFF:
		   switch( db_tab_window_info[crt_n].select_pt.data_type)
		   {
		   case	SOE_TYPE:
		   case	POL_TYPE:
		   case	SYSMSG_TYPE:
		   case    OUTS_TYPE:
		   case DEVOBJ_TYPE:
			   if(CheckUserPrivilege(cur_usr_name, PRIV_RTVALUE_MAN_SET))
			   {
				   CmdBufInit ( crt_n );
				   cmd_buf[crt_n].stn_id = db_tab_window_info[crt_n].select_pt.stn_id;
				   cmd_buf[crt_n].dev_id = db_tab_window_info[crt_n].select_pt.dev_id;
				   
				   cmd_buf[crt_n].type_id = DPS_MAN_STAT_SET;
				   
				   cmd_buf[crt_n].data_type	= db_tab_window_info[crt_n].select_pt.data_type;
				   cmd_buf[crt_n].point_id = db_tab_window_info[crt_n].select_pt.pt_id;
				   
				   if( rtn_num == DB_IO_MAN_SET_ON)
					   cmd_buf[crt_n].status.opr =  MAN_SET_CLOSE;
				   else if ( rtn_num == DB_IO_MAN_SET_OFF)
					   cmd_buf[crt_n].status.opr =  MAN_SET_OPEN;
				   
				   ProgDoAckCelBrdcst ( crt_n );				
			   }			
			   else
				   PopupMessageBox ( db_io_set_box[crt_n], MSG_MANUAL_SETTING );
			   break;
			   
		   default:
			   break;
		   }		
		   break;
		   
		   case	DB_IO_ALM_ACK:
			   if( db_tab_window_info[crt_n].select_pt.data_type != PARAM_TYPE)
			   {
				   if(CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
				   {	
					   CmdBufInit ( crt_n );
					   cmd_buf[crt_n].type_id 	= DPS_IOST_SET;
					   cmd_buf[crt_n].status.opr	= IOST_SET_ALM_NOACK;
					   cmd_buf[crt_n].status.state = ACKN;
					   ReadEntryById  ( &db_tab_window_info[crt_n].select_pt, &dms_cmm );
					   FillIostCmdBuf ( crt_n, &dms_cmm,cmd_buf[crt_n].status,IOST_TYPE);
					   ProgDoAckCelBrdcst ( crt_n );
				   }
				   else
					   PopupMessageBox ( db_io_set_box[crt_n], MSG_ALARM_ACK);
			   }
			   else
				   PopupMessageBox ( db_io_set_box[crt_n],MSG_APPLY_NO_PERMIT);			
			   break;
			   
		   case	DB_IO_ALM_ENABLE:
		   case	DB_IO_ALM_DISABLE:
		   case	DB_IO_SCAN_ENABLE:
		   case	DB_IO_SCAN_DISABLE:
			   if( db_tab_window_info[crt_n].select_pt.data_type != PARAM_TYPE)
			   {
				   if(CheckUserPrivilege(cur_usr_name, PRIV_SCAN_ALM_SET))
				   {	
					   CmdBufInit ( crt_n );
					   cmd_buf[crt_n].type_id = DPS_IOST_SET;
					   if(rtn_num == DB_IO_ALM_ENABLE||rtn_num ==DB_IO_ALM_DISABLE)	
						   cmd_buf[crt_n].status.opr= IOST_SET_ALM_STATE;
					   else if(rtn_num == DB_IO_SCAN_ENABLE||rtn_num ==DB_IO_SCAN_DISABLE)
						   cmd_buf[crt_n].status.opr= IOST_SET_SCAN_STATE;
					   if ( rtn_num == DB_IO_ALM_ENABLE||rtn_num ==DB_IO_SCAN_ENABLE)								
						   cmd_buf[crt_n].status.state = ENABLE;				
					   else if ( rtn_num == DB_IO_ALM_DISABLE||rtn_num == DB_IO_SCAN_DISABLE)
						   cmd_buf[crt_n].status.state = DISABLE;
					   ReadEntryById  ( &db_tab_window_info[crt_n].select_pt, &dms_cmm );
					   FillIostCmdBuf ( crt_n, &dms_cmm, cmd_buf[crt_n].status,IOST_TYPE);
					   
					   ProgDoAckCelBrdcst ( crt_n );
				   }
				   else
					   PopupMessageBox ( db_io_set_box[crt_n], MSG_SCAN_ALM_ENABLE );
			   }
			   else
				   PopupMessageBox ( db_io_set_box[crt_n],MSG_APPLY_NO_PERMIT);		
			   break;
			   
		   case	DB_IO_VALUE_SET:
			   GetLocalSysLcuId(&dev_id);
			   
			   if(db_tab_window_info[crt_n].select_pt.dev_id==dev_id )		
			   {
				   if(CheckUserPrivilege(cur_usr_name, PRIV_RTVALUE_MAN_SET))
					   ProgSetValue ( crt_n, db_pop_shell[crt_n],dms_cmm);
				   else				
					   PopupMessageBox ( db_io_set_box[crt_n], MSG_MANUAL_SETTING);
				   /*UnmanageDbIoBox(  db_io_set_box[crt_n] );wzg*/
				   break;
			   }
			   if(CheckUserPrivilege(cur_usr_name, PRIV_RTVALUE_MAN_SET))
				   ProgSetValue ( crt_n, db_pop_shell[crt_n],dms_cmm);		
			   else			
				   PopupMessageBox ( db_io_set_box[crt_n], MSG_MANUAL_SETTING );		
			   break;
			   
		   case	DB_IO_DEAD_BAND_SET:
		   case	DB_IO_HH_LMT_SET:
		   case	DB_IO_H_LMT_SET:
		   case	DB_IO_L_LMT_SET:
		   case	DB_IO_LL_LMT_SET:
		   case	DB_IO_HLMT_RTN_SET:
		   case	DB_IO_LLMT_RTN_SET:
		   case	DB_IO_EGNR_VAL_SET:
		   case	DB_IO_HIRANGE_SET:
		   case	DB_IO_LORANGE_SET:
		   case	DB_IO_BASE_SET:
		   case	DB_IO_COMPENSATE_SET:
		   case	DB_IO_TR_WARN_SET:
		   case	DB_IO_TR_RECOVER_SET:
			   if( db_tab_window_info[crt_n].select_pt.data_type != PARAM_TYPE)
			   {
				   if(CheckUserPrivilege(cur_usr_name, PRIV_LIMIT_PARA_SET))
				   {
					   switch	( rtn_num )
					   {
					   case	DB_IO_DEAD_BAND_SET:
						   data_class = DEAD_BAND;
						   break;
						   
					   case	DB_IO_HH_LMT_SET:
						   data_class = HH_LMT;
						   break;
						   
					   case	DB_IO_H_LMT_SET:
						   data_class = H_LMT;
						   break;
						   
					   case	DB_IO_L_LMT_SET:
						   data_class = L_LMT;
						   break;
						   
					   case	DB_IO_LL_LMT_SET:
						   data_class = LL_LMT;
						   break;
						   
					   case    DB_IO_HLMT_RTN_SET:
						   data_class = HRTN_LMT;
						   break;
						   
					   case    DB_IO_LLMT_RTN_SET:
						   data_class = LRTN_LMT;
						   break;
						   
					   case	DB_IO_EGNR_VAL_SET:
						   data_class = ENG_CVTER;
						   break;
						   
					   case	DB_IO_HIRANGE_SET:
						   data_class = HIRANGE;
						   break;
						   
					   case	DB_IO_LORANGE_SET:
						   data_class = LORANGE;
						   break;
						   
					   case	DB_IO_BASE_SET:
						   data_class = BASE;
						   break;
						   
					   case	DB_IO_COMPENSATE_SET:
						   data_class = COMPENSATE;
						   break;
						   
					   case	DB_IO_TR_WARN_SET:
						   data_class = Tr_WARN;
						   break;
						   
					   case	DB_IO_TR_RECOVER_SET:
						   data_class = Tr_RECOVER;
						   break;
						   
					   default:
						   break;
					   }
					   ModifyDataClass( crt_n,db_pop_shell[crt_n], &dms_cmm, &db_tab_window_info[crt_n].select_pt, data_class);
				   }
				   else							
					   PopupMessageBox ( db_io_set_box[crt_n], MSG_LMT_PARAM_SETTING );			
				   break;
			   }
			   else
				   PopupMessageBox ( db_io_set_box[crt_n], MSG_DATA_TYPE_ERROR);
			   
		   default:
			   break;
	}	
	/*UnmanageDbIoBox (  db_io_set_box[crt_n] );wzg*/
}




void	GetDbDetailRowCol(int item, int *row, int *col)
{
	*col= (item+DB_DETAIL_ROW_NUM_PER_COL-1)/DB_DETAIL_ROW_NUM_PER_COL;	
	*row= DB_DETAIL_ROW_NUM_PER_COL;
}


void GetYesNoStr(int value,char *str)
{
	if(value==TRUE)
		strcpy(str,(OIX_CSTR_YES));
	else
		strcpy(str,(OIX_CSTR_NO));
}


void	GetDbDetailFixedInfo(DMS_COMMON *db_cmm, int *item_num, char entry_str[][64], char value_str[][POINT_LONGNAME_SIZE] )
{	
	int		item= *item_num;
	
	strcpy(entry_str[item], OIX_STR_IO_ADDRESS);
	sprintf(value_str[item],"%d.%d.%d.%d",db_cmm->point.stn_id,db_cmm->point.dev_id,db_cmm->point.data_type,db_cmm->point.pt_id);	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_NAME);
	GetPtNameById(&db_cmm->point,value_str[item],NULL);	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_LONGNAME);
	GetEntryLongnameById(&db_cmm->point,value_str[item]);	
	item++;
	
	strcpy(entry_str[item], OIX_STR_CTRL_ADDR);
	sprintf(value_str[item], "%d", db_cmm->fixed.ctrl_addr);	
	item++;
	
	strcpy(entry_str[item], OIX_STR_MSG_ALM_SEL);
	sprintf(value_str[item], "%d", db_cmm->fixed.msg_sel.alm);	
	item++;
	
	strcpy(entry_str[item], OIX_STR_MSG_CMD_SEL);
	sprintf(value_str[item], "%d", db_cmm->fixed.msg_sel.cmd);	
	item++;
	
	strcpy(entry_str[item], OIX_STR_PAIR_ADDRESS);
	sprintf(value_str[item],"%d.%d.%d.%d",db_cmm->fixed.pair_pt.stn_id,
		db_cmm->fixed.pair_pt.dev_id,db_cmm->fixed.pair_pt.data_type,db_cmm->fixed.pair_pt.pt_id);	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_GROUP_NO);
	sprintf(value_str[item], "%d", db_cmm->fixed.io_group);	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_TERMINAL);
	strcpy(value_str[item], db_cmm->fixed.hard_addr.terminal_no);	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_UNIT_NO);
	strcpy(value_str[item],db_cmm->fixed.hard_addr.unit_no);	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_SLOT);
	strcpy(value_str[item],db_cmm->fixed.hard_addr.slot_no);	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_CHANNEL);
	strcpy(value_str[item],db_cmm->fixed.hard_addr.channel_no);
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_CABINET_NO);
	strcpy(value_str[item],db_cmm->fixed.hard_addr.cabinet_no);       
	item++;
	
	strcpy(entry_str[item], OIX_STR_POPUP_DSPFILE);
	sprintf(value_str[item],"%s",db_cmm->fixed.dsp_filename);
	item++;
	
	/*IOST*/
	strcpy(entry_str[item], OIX_STR_VIEW_POINT);
	sprintf(value_str[item], "%d", db_cmm->fixed.iost.viewPoint);
	opr_state[item]=0;		
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_ALM_REPEAT);
	GetYesNoStr(db_cmm->fixed.iost.almRepeat, value_str[item]);
	opr_state[item]=IOST_SET_ALM_REPEAT;	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_NO_UPDATE);
	GetYesNoStr(db_cmm->fixed.iost.noUpdate, value_str[item]);
	opr_state[item]=IOST_SET_NO_UPDATE;	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_ScanEnable);
	GetYesNoStr(db_cmm->fixed.iost.scanEnable, value_str[item]);
	opr_state[item]=IOST_SET_SCAN_STATE;	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_AlarmEnable);
	GetYesNoStr(db_cmm->fixed.iost.almEnable, value_str[item]);
	opr_state[item]=IOST_SET_ALM_STATE;	
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_AlarmNoAck);
	GetYesNoStr(db_cmm->fixed.iost.almNoAck, value_str[item]);
	opr_state[item]=IOST_SET_ALM_NOACK;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_CTRL_LOCK);
	GetYesNoStr(db_cmm->fixed.iost.ctrlLock, value_str[item]);
	opr_state[item]=IOST_SET_CTRL_LOCK;
	item++;
	
	strcpy(entry_str[item], OIX_STR_TAG_STATE);
	sprintf(value_str[item], "%d", db_cmm->fixed.iost.tagState);
	opr_state[item]=IOST_SET_TAG_STATE;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_HDB_IO_GROUP);
	sprintf(value_str[item], "%d", db_cmm->fixed.io_group);
	opr_state[item]=0;	
	item++;
	
	/*IOATTR*/
	strcpy(entry_str[item], OIX_STR_PROC_TYPE);
	sprintf(value_str[item],"%d",db_cmm->fixed.ioattr.procType);	
	opr_state[item]=IOATTR_SET_PROCTYPE;
	item++;
	
	strcpy(entry_str[item], OIX_STR_ALM_LOCK);
	GetYesNoStr(db_cmm->fixed.ioattr.almCba,value_str[item]);
	opr_state[item]=IOATTR_SET_ALM_CBA;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_STATISTIC);
	GetYesNoStr(db_cmm->fixed.ioattr.statistic,value_str[item]);
	opr_state[item]=IOATTR_SET_STATISTIC;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_FAULT);
	GetYesNoStr(db_cmm->fixed.ioattr.fault,value_str[item]);
	opr_state[item]=IOATTR_SET_FAULT_PNT;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_FAIL);
	GetYesNoStr(db_cmm->fixed.ioattr.fail,value_str[item]);	
	opr_state[item]=IOATTR_SET_FAIL_PNT;
	item++;
	
	strcpy(entry_str[item], OIX_STR_CTRL);
	GetYesNoStr(db_cmm->fixed.ioattr.ctrl,value_str[item]);	
	opr_state[item]=IOATTR_SET_CTRL_PNT;
	item++;
	
	strcpy(entry_str[item], OIX_STR_SOE);
	GetYesNoStr(db_cmm->fixed.ioattr.soe,value_str[item]);	
	opr_state[item]=IOATTR_SET_SOE_PNT;
	item++;
	
	strcpy(entry_str[item], OIX_STR_TREND_ALM);
	GetYesNoStr(db_cmm->fixed.ioattr.trendAlm,value_str[item]);
	opr_state[item]=IOATTR_SET_TREND_ALM;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_SIGNAL);
	GetYesNoStr(db_cmm->fixed.ioattr.keySignal,value_str[item]);	
	opr_state[item]=IOATTR_SET_KEY_SIGNAL;
	item++;
	
	strcpy(entry_str[item], OIX_STR_FAULT_REC);
	GetYesNoStr(db_cmm->fixed.ioattr.faultRec,value_str[item]);
	opr_state[item]=IOATTR_SET_FAULT_REC;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_AUD_ALM);
	GetYesNoStr(db_cmm->fixed.ioattr.audAlm,value_str[item]);
	opr_state[item]=IOATTR_SET_AUD_ALM;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_NORMAL_CLOSE);
	GetYesNoStr(db_cmm->fixed.ioattr.normalClose,value_str[item]);
	opr_state[item]=IOATTR_SET_NORM_CLOSE;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_DOUBLE_PT);
	GetYesNoStr(db_cmm->fixed.ioattr.doublePt,value_str[item]);
	opr_state[item]=IOATTR_SET_DOUBLE_PNT;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_SYS_VIRTUAL);
	GetYesNoStr(db_cmm->fixed.ioattr.sysVirtual,value_str[item]);
	opr_state[item]=IOATTR_SET_SYS_VIRTUAL;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_LCU_VIRTUAL);
	GetYesNoStr(db_cmm->fixed.ioattr.lcuVirtual,value_str[item]);	
	opr_state[item]=IOATTR_SET_LCU_VIRTUAL;
	item++;
	
	strcpy(entry_str[item], OIX_STR_CASCADE);
	GetYesNoStr(db_cmm->fixed.ioattr.cascade,value_str[item]);
	opr_state[item]=IOATTR_SET_CASCADE_PNT;	
	item++;
	
	strcpy(entry_str[item], OIX_STR_HDB_REC);
	GetYesNoStr(db_cmm->fixed.ioattr.hdbRec,value_str[item]);
	opr_state[item]=IOATTR_SET_HDB_REC;
	item++;
	
	strcpy(entry_str[item], OIX_STR_HDB_CURVE);
	GetYesNoStr(db_cmm->fixed.ioattr.hdbCurve,value_str[item]);	
	opr_state[item]=IOATTR_SET_HDB_CURVE;
	item++;
	
	/*strcpy(entry_str[item], OIX_STR_HDB_IO_GROUP);
	sprintf(value_str[item],"%d",db_cmm->fixed.io_group);
	opr_state[item]=IOATTR_SET_IO_GROUP;	
	item++;*/
	
	strcpy(entry_str[item], OIX_CSTR_HDB_PERIOD);        
	switch(db_cmm->fixed.ioattr.hdbPeriod)
	{
	case	0:
		strcpy(value_str[item],OIX_STR_HDB_PERIOD0);
		break;
	case	1:
		strcpy(value_str[item],OIX_STR_HDB_PERIOD1);
		break;
	case	2:
		strcpy(value_str[item],OIX_STR_HDB_PERIOD2);
		break;
	case	3:
		strcpy(value_str[item],OIX_STR_HDB_PERIOD3);
		break;
	case	4:
		strcpy(value_str[item],OIX_STR_HDB_PERIOD4);
		break;
	case	5:
		strcpy(value_str[item],OIX_STR_HDB_PERIOD5);
		break;
	default:
		break;
	}
	opr_state[item]=IOATTR_SET_HDB_PERIOD;
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_ON_ALM);
	GetYesNoStr(db_cmm->fixed.ioattr.onAlm,value_str[item]);	
	opr_state[item]=IOATTR_SET_ON_ALM;
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_ON_ALM);
	GetYesNoStr(db_cmm->fixed.ioattr.offAlm,value_str[item]);	
	opr_state[item]=IOATTR_SET_OFF_ALM;
	item++;
	
	strcpy(entry_str[item], OIX_CSTR_OFF_ALM);
	GetYesNoStr(db_cmm->fixed.ioattr.normalState,value_str[item]);	
	opr_state[item]=IOATTR_SET_NORM_STATE;
	item++;
	
	*item_num= item;
}

void GetAnaLmtStateStr(DMS_COMMON *db_cmm,char *value_str)
{
	if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_NORMAL)
		strcpy(value_str,DPS_STR_NORMAL);
	else if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_HI_ALARM)
		strcpy(value_str,DPS_STR_H_ALM_LMT);
	else if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_HI_WARN)
		strcpy(value_str,DPS_STR_H_WRN_LMT);
	else if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_LO_WARN)
		strcpy(value_str,DPS_STR_L_WRN_LMT);
	else if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_LO_ALARM)
		strcpy(value_str,DPS_STR_L_ALM_LMT);
	else if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_HI_RECOVER)
		strcpy(value_str,DPS_STR_H_RECOVER);
	else if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_LO_RECOVER)
		strcpy(value_str,DPS_STR_L_RECOVER);
	else if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_HH_RECOVER)
		strcpy(value_str,DPS_STR_HH_RECOVER);
	else if(db_cmm->var.anlg.value.status.lmt_state==ANA_ALM_LL_RECOVER)
		strcpy(value_str,DPS_STR_LL_RECOVER);
}

void GetAnaTrLmtStateStr(DMS_COMMON *db_cmm,char *value_str)
{
	if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_NORMAL)
		strcpy(value_str,DPS_STR_NORMAL);
	else if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_TR_WARN)
		strcpy(value_str,DPS_STR_OVER_TREND_LMT);
	else if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_TR_RECOVER)
		strcpy(value_str,DPS_STR_RECOVER_TREND_LMT);	
}

/*
void	DspDbDetailInfo(int crt_n,GdkDrawable *drawable,GdkGC *gc,int item_num, 
char entry_str[][64], char value_str[][POINT_LONGNAME_SIZE] )
{
int			i, j, row, col, item;

  GetDbDetailRowCol(item_num, &row, &col);
  
	gtk_widget_set_usize(GTK_WIDGET(db_detail_info[crt_n]), 
	col*DB_DETAIL_COL_WIDTH+DB_DETAIL_ENTRY_X, row*DB_DETAIL_ROW_HEIGHT+DB_DETAIL_ENTRY_Y+20);	
	
	  item= 0;
	  for(i=0; i<col; i++)
	  {
	  for(j=0; j<row; j++, item++)
	  {
	  if(item>=item_num)
	  return;
	  
		gdk_gc_set_foreground(gc, &pixels[crt_n][L_BLUE3]);
		gdk_draw_text(drawable, font_set[DBTAB_FONT], gc, 
		DB_DETAIL_ENTRY_X+DB_DETAIL_COL_WIDTH*i, DB_DETAIL_ENTRY_Y+DB_DETAIL_ROW_HEIGHT*j,
		entry_str[item], strlen(entry_str[item]));
		gdk_gc_set_foreground(gc, &pixels[crt_n][WHITE]);
		gdk_draw_text(drawable, font_set[DBTAB_FONT], gc, 
		DB_DETAIL_VALUE_X+DB_DETAIL_COL_WIDTH*i, DB_DETAIL_ENTRY_Y+DB_DETAIL_ROW_HEIGHT*j,
		value_str[item], strlen(value_str[item]));
		}
		}
		}
		
		  
			void	DspAnaDetailInfo ( int crt_n,GdkDrawable *drawable,GdkGC *gc, DMS_COMMON *db_cmm )
			{	
			int			item=0;
			char		entry_str[MAX_DB_DETAIL_ITEM_NUM][64], value_str[MAX_DB_DETAIL_ITEM_NUM][POINT_LONGNAME_SIZE];
			
			  strcpy(entry_str[item], OIX_STR_RT_VALUE);
			  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.fValue); 
			  item++;
			  
				strcpy(entry_str[item], OIX_STR_UPDATE_TIME);
				sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.anlg.update_time.year,db_cmm->var.anlg.update_time.month,db_cmm->var.anlg.update_time.day,db_cmm->var.anlg.update_time.hour,
				db_cmm->var.anlg.update_time.minute,db_cmm->var.anlg.update_time.second,db_cmm->var.anlg.update_time.ms );
				item++;
				
				  strcpy(entry_str[item], OIX_STR_DATA_QUALITY);
				  if(db_cmm->var.anlg.value.status.chan_fail)
				  strcpy(value_str[item],DPS_STR_FAIL);
				  else if(db_cmm->var.anlg.value.status.ctrl_block)
				  strcpy(value_str[item],DPS_STR_CTRL_BLOCK);
				  else
				  strcpy(value_str[item],DPS_STR_NORMAL);
				  item++;
				  
					strcpy(entry_str[item], OIX_STR_LMT_STATUS);
					GetAnaLmtStateStr(db_cmm, value_str[item]);
					item++;
					
					  strcpy(entry_str[item], OIX_STR_TRLMT_STATUS);
					  if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_NORMAL)
					  strcpy(value_str[item],DPS_STR_NORMAL);
					  else if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_TR_WARN)
					  strcpy(value_str[item],DPS_STR_H_WRN_LMT);
					  else if(db_cmm->var.anlg.value.status.trend_state==ANA_ALM_TR_RECOVER)
					  strcpy(value_str[item],DPS_STR_H_RECOVER);
					  item++;
					  
						strcpy(entry_str[item], OIX_STR_ANA_UNITNAME);
						sprintf(value_str[item], "%s", db_cmm->var.anlg.unitName );
						item++;
						
						  GetDbDetailFixedInfo(db_cmm, &item, entry_str, value_str);
						  
							strcpy(entry_str[item], OIX_STR_H_ALM_LMT);
							sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.HI_ALARM );
							item++;
							
							  strcpy(entry_str[item], OIX_STR_H_WRN_LMT);
							  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.HI_WARN );
							  item++;
							  
								strcpy(entry_str[item], OIX_STR_H_RECOVER);
								sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.HI_RECOVER ); 
								item++;
								
								  strcpy(entry_str[item], OIX_STR_L_RECOVER);
								  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.LO_RECOVER ); 
								  item++;
								  
									strcpy(entry_str[item], OIX_STR_L_WRN_LMT);
									sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.LO_WARN ); 
									item++;
									
									  strcpy(entry_str[item], OIX_STR_L_ALM_LMT);
									  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.LO_ALARM ); 
									  item++;
									  
										strcpy(entry_str[item], OIX_CSTR_TREND_LMT);
										sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.TR_WARN );
										item++;
										
										  strcpy(entry_str[item], OIX_CSTR_TREND_RESET_LMT);
										  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.lmt_para.TR_RECOVER );
										  item++;
										  
											strcpy(entry_str[item], OIX_STR_DBAND);
											sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.D_BAND );
											item++;
											
											  strcpy(entry_str[item], OIX_STR_ENG_FACTOR);
											  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.ENG_CVT );
											  item++;
											  
												strcpy(entry_str[item], OIX_STR_HI_RANGE);
												sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.HI_RANGE );
												item++;
												
												  strcpy(entry_str[item], OIX_STR_LO_RANGE);
												  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.LO_RANGE );
												  item++;
												  
													strcpy(entry_str[item], OIX_STR_BASE);
													sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.ZERO );
													item++;
													
													  strcpy(entry_str[item], OIX_STR_COMPENSATE);
													  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.COMPSATE );
													  item++;
													  
														strcpy(entry_str[item], OIX_STR_H_CODE);
														sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.HI_CODE );
														item++;
														
														  strcpy(entry_str[item], OIX_STR_L_CODE);
														  sprintf(value_str[item], "%.3f", db_cmm->var.anlg.eng_para.LO_CODE );
														  item++;
														  
															DspDbDetailInfo(crt_n,drawable,gc,item,entry_str,value_str);	
															}
															
															  
																
																  void DspPolDetailInfo (int crt_n,GdkDrawable *drawable,GdkGC *gc, DMS_COMMON *db_cmm)
																  {
																  int			item=0;
																  char		entry_str[MAX_DB_DETAIL_ITEM_NUM][64], value_str[MAX_DB_DETAIL_ITEM_NUM][POINT_LONGNAME_SIZE];
																  
																	strcpy(entry_str[item], OIX_STR_RT_STATE);
																	
																	  GetIndPtStateStrById( &db_cmm->point,value_str[item]);
																	  printf("### value_str[%d]=%s\n",item,value_str[item]);
																	  item++;
																	  
																		if(db_cmm->fixed.ioattr.doublePt)
																		{
																		strcpy(entry_str[item], OIX_STR_DOUBLE_STATE);
																		sprintf(value_str[item], "%d", db_cmm->var.ind.status.state); 
																		item++;
																		}
																		
																		  strcpy(entry_str[item], OIX_STR_UPDATE_TIME);
																		  sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.ind.update_time.year,db_cmm->var.ind.update_time.month,db_cmm->var.ind.update_time.day,db_cmm->var.ind.update_time.hour,
																		  db_cmm->var.ind.update_time.minute,db_cmm->var.ind.update_time.second,db_cmm->var.ind.update_time.ms );
																		  item++;
																		  
																			strcpy(entry_str[item], OIX_STR_CHANGE_TIME);
																			sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.ind.change_time.year,db_cmm->var.ind.change_time.month,db_cmm->var.ind.change_time.day,db_cmm->var.ind.change_time.hour,
																			db_cmm->var.ind.change_time.minute,db_cmm->var.ind.change_time.second,db_cmm->var.ind.change_time.ms );
																			item++;
																			
																			  strcpy(entry_str[item], OIX_STR_FAULT_FLAG);
																			  GetYesNoStr(db_cmm->var.ind.status.fault_tag,value_str[item]);	
																			  item++;
																			  
																				strcpy(entry_str[item], OIX_STR_DATA_QUALITY);
																				if(db_cmm->var.ind.status.chan_fail)
																				strcpy(value_str[item],DPS_STR_FAIL);
																				else if(db_cmm->var.ind.status.man_set)
																				strcpy(value_str[item],DPS_STR_MANUAL);
																				else
																				strcpy(value_str[item],DPS_STR_NORMAL);
																				item++;
																				
																				  strcpy(entry_str[item], OIX_STR_STC_NORM_OPEN);
																				  sprintf(value_str[item], "%d", db_cmm->var.ind.stc.norm_open); 
																				  item++;
																				  
																					strcpy(entry_str[item], OIX_STR_STC_NORM_CLOSE);
																					sprintf(value_str[item], "%d", db_cmm->var.ind.stc.norm_close); 
																					item++;
																					
																					  strcpy(entry_str[item], OIX_STR_STC_FAULT_OPEN);
																					  sprintf(value_str[item], "%d", db_cmm->var.ind.stc.fault_open); 
																					  item++;
																					  
																						strcpy(entry_str[item], OIX_STR_STC_FAULT_CLOSE);
																						sprintf(value_str[item], "%d", db_cmm->var.ind.stc.fault_close); 
																						item++;
																						
																						  GetDbDetailFixedInfo(db_cmm, &item, entry_str, value_str);
																						  
																							DspDbDetailInfo(crt_n,drawable,gc,item,entry_str,value_str);	
																							}
																							
																							  
																								void	DspImpDetailInfo (int crt_n,GdkDrawable *drawable,GdkGC * gc, DMS_COMMON *db_cmm)
																								{
																								int			item=0;
																								char		entry_str[MAX_DB_DETAIL_ITEM_NUM][64], value_str[MAX_DB_DETAIL_ITEM_NUM][POINT_LONGNAME_SIZE];
																								
																								  strcpy(entry_str[item], OIX_STR_REAL_ENERGY);
																								  sprintf(value_str[item], "%d", db_cmm->var.imp.entry.rt_value); 
																								  item++;
																								  
																									strcpy(entry_str[item], OIX_STR_BASE_ENERGY);
																									sprintf(value_str[item], "%d", db_cmm->var.imp.entry.base_value); 
																									item++;
																									
																									  strcpy(entry_str[item], OIX_STR_UPDATE_TIME);
																									  sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.imp.entry.update_time.year,db_cmm->var.imp.entry.update_time.month,db_cmm->var.imp.entry.update_time.day,db_cmm->var.imp.entry.update_time.hour,
																									  db_cmm->var.imp.entry.update_time.minute,db_cmm->var.imp.entry.update_time.second,db_cmm->var.imp.entry.update_time.ms );
																									  item++;
																									  
																										strcpy(entry_str[item], OIX_STR_DATA_QUALITY);
																										if(db_cmm->var.imp.entry.status.chan_fail)
																										strcpy(value_str[item],DPS_STR_FAIL);
																										else if(db_cmm->var.imp.entry.status.man_set)
																										strcpy(value_str[item],DPS_STR_MANUAL);
																										else
																										strcpy(value_str[item],DPS_STR_NORMAL);
																										item++;
																										
																										  GetDbDetailFixedInfo(db_cmm, &item, entry_str, value_str);
																										  
																											strcpy(entry_str[item], OIX_STR_IMP_CONSTANT);
																											sprintf(value_str[item], "%u", db_cmm->var.imp.entry.eng_para.CONSTANT); 
																											item++;
																											
																											  strcpy(entry_str[item], OIX_STR_ACCUM);
																											  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.d_accum); 
																											  item++;
																											  
																												strcpy(entry_str[item], OIX_STR_ENG_FACTOR);
																												sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.eng_para.ENG_CVT); 
																												item++;
																												
																												  strcpy(entry_str[item], OIX_STR_IMP_INIT_YEAR);
																												  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.year_init); 
																												  item++;
																												  
																													strcpy(entry_str[item], OIX_STR_IMP_INIT_MONTH);
																													sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.month_init); 
																													item++;
																													
																													  strcpy(entry_str[item], OIX_STR_IMP_INIT_DAY);
																													  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.day_init); 
																													  item++;
																													  
																														strcpy(entry_str[item], OIX_STR_DAILY_PPEAK_ENERGY);
																														sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.ppeak.day); 
																														item++;
																														
																														  strcpy(entry_str[item], OIX_STR_DAILY_PEAK_ENERGY);
																														  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.peak.day); 
																														  item++;
																														  
																															strcpy(entry_str[item], OIX_STR_DAILY_VALLEY_ENERGY);
																															sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.valley.day); 
																															item++;
																															
																															  strcpy(entry_str[item], OIX_STR_DAILY_MEAN_ENERGY);
																															  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.normal.day); 
																															  item++;
																															  
																																strcpy(entry_str[item], OIX_STR_DAILY_TTL_ENERGY);
																																sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.sum.day); 
																																item++;
																																
																																  strcpy(entry_str[item], OIX_STR_MONTH_PPEAK_ENERGY);
																																  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.ppeak.month); 
																																  item++;
																																  
																																	strcpy(entry_str[item], OIX_STR_MONTH_PEAK_ENERGY);
																																	sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.peak.month); 
																																	item++;
																																	
																																	  strcpy(entry_str[item], OIX_STR_MONTH_VALLEY_ENERGY);
																																	  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.valley.month); 
																																	  item++;
																																	  
																																		strcpy(entry_str[item], OIX_STR_MONTH_MEAN_ENERGY);
																																		sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.normal.month); 
																																		item++;
																																		
																																		  strcpy(entry_str[item], OIX_STR_MONTH_TTL_ENERGY);
																																		  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.sum.month); 
																																		  item++;
																																		  
																																			strcpy(entry_str[item], OIX_STR_YEAR_PPEAK_ENERGY);
																																			sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.ppeak.year); 
																																			item++;
																																			
																																			  strcpy(entry_str[item], OIX_STR_YEAR_PEAK_ENERGY);
																																			  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.peak.year); 
																																			  item++;
																																			  
																																				strcpy(entry_str[item], OIX_STR_YEAR_VALLEY_ENERGY);
																																				sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.valley.year); 
																																				item++;
																																				
																																				  strcpy(entry_str[item], OIX_STR_YEAR_MEAN_ENERGY);
																																				  sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.normal.year); 
																																				  item++;
																																				  
																																					strcpy(entry_str[item], OIX_STR_YEAR_TTL_ENERGY);
																																					sprintf(value_str[item], "%.3f", db_cmm->var.imp.entry.sum.year); 
																																					item++;
																																					
																																					  DspDbDetailInfo(crt_n,drawable,gc,item,entry_str,value_str);	
																																					  }
																																					  
																																						
																																						  
																																							void	DspCharaDetailInfo (int crt_n,GdkDrawable *drawable,GdkGC * gc, DMS_COMMON *db_cmm)
																																							{
																																							int			item=0;
																																							char		entry_str[MAX_DB_DETAIL_ITEM_NUM][64], value_str[MAX_DB_DETAIL_ITEM_NUM][POINT_LONGNAME_SIZE];
																																							
																																							  strcpy(entry_str[item], OIX_STR_UPDATE_TIME);
																																							  sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.chara.update_time.year,db_cmm->var.chara.update_time.month,db_cmm->var.chara.update_time.day,db_cmm->var.chara.update_time.hour,
																																							  db_cmm->var.chara.update_time.minute,db_cmm->var.chara.update_time.second,db_cmm->var.chara.update_time.ms );
																																							  item++;
																																							  
																																								strcpy(entry_str[item], OIX_STR_START_POINTNAME);
																																								sprintf(value_str[item], "%s", db_cmm->var.chara.start_pt_name); 
																																								item++;
																																								
																																								  strcpy(entry_str[item], OIX_STR_POINT_NUM);
																																								  sprintf(value_str[item], "%d", db_cmm->var.chara.pt_num); 
																																								  item++;
																																								  
																																									GetDbDetailFixedInfo(db_cmm, &item, entry_str, value_str);
																																									
																																									  strcpy(entry_str[item], OIX_STR_MAX_POINT);
																																									  sprintf(value_str[item], "%d", db_cmm->var.chara.chara_data.max_no); 
																																									  item++;
																																									  
																																										strcpy(entry_str[item], OIX_STR_MIN_POINT);
																																										sprintf(value_str[item], "%d", db_cmm->var.chara.chara_data.min_no); 
																																										item++;
																																										
																																										  strcpy(entry_str[item], OIX_STR_MAX_RATE_POINT);
																																										  sprintf(value_str[item], "%d", db_cmm->var.chara.chara_data.max_rate_no); 
																																										  item++;
																																										  
																																											strcpy(entry_str[item], OIX_STR_MIN_RATE_POINT);
																																											sprintf(value_str[item], "%d", db_cmm->var.chara.chara_data.min_rate_no); 
																																											item++;
																																											
																																											  strcpy(entry_str[item], OIX_STR_MAX);
																																											  sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.max_value); 
																																											  item++;
																																											  
																																												strcpy(entry_str[item], OIX_STR_MIN);
																																												sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.min_value); 
																																												item++;
																																												
																																												  strcpy(entry_str[item], OIX_STR_MEAN);
																																												  sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.avg_value); 
																																												  item++;
																																												  
																																													strcpy(entry_str[item], OIX_STR_VARIANCE);
																																													sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.std_value); 
																																													item++;
																																													
																																													  strcpy(entry_str[item], OIX_STR_MAX_RATE);
																																													  sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.max_rate); 
																																													  item++;
																																													  
																																														strcpy(entry_str[item], OIX_STR_MIN_RATE);
																																														sprintf(value_str[item], "%.3f", db_cmm->var.chara.chara_data.min_rate); 
																																														item++;
																																														
																																														  DspDbDetailInfo(crt_n,drawable,gc,item,entry_str,value_str);	
																																														  }
																																														  
																																															
																																															  
																																																void	DspCurveDetailInfo(int crt_n,GdkDrawable *drawable,GdkGC * gc, DMS_COMMON *db_cmm)
																																																{
																																																#define	SEG_PER_ITEM	3
																																																int			i, j, end_seg, item=0;
																																																char		entry_str[MAX_DB_DETAIL_ITEM_NUM*2][64], value_str[MAX_DB_DETAIL_ITEM_NUM*2][POINT_LONGNAME_SIZE];
																																																char		*pStr;
																																																
																																																  strcpy(entry_str[item], OIX_STR_IO_ADDRESS);
																																																  sprintf(value_str[item],"%d.%d.%d.%d",db_cmm->point.stn_id,db_cmm->point.dev_id,db_cmm->point.data_type,db_cmm->point.pt_id);	
																																																  item++;
																																																  
																																																	strcpy(entry_str[item], OIX_CSTR_NAME);
																																																	GetPtNameById(&db_cmm->point,value_str[item],NULL);	
																																																	item++;
																																																	
																																																	  strcpy(entry_str[item], OIX_CSTR_LONGNAME);
																																																	  GetEntryLongnameById(&db_cmm->point,value_str[item]);	
																																																	  item++;
																																																	  
																																																		strcpy(entry_str[item], OIX_STR_UPDATE_TIME);
																																																		sprintf(value_str[item],"%d-%d-%d %02d:%02d:%02d:%03d",db_cmm->var.curve.update_time.year,db_cmm->var.curve.update_time.month,db_cmm->var.curve.update_time.day,db_cmm->var.curve.update_time.hour,
																																																		db_cmm->var.curve.update_time.minute,db_cmm->var.curve.update_time.second,db_cmm->var.curve.update_time.ms );
																																																		item++;
																																																		
																																																		  strcpy(entry_str[item], OIX_STR_SEGMENT_NUM);
																																																		  sprintf(value_str[item], "%d", db_cmm->var.curve.seg_num); 
																																																		  item++;
																																																		  
																																																			strcpy(entry_str[item], OIX_STR_HI_RANGE);
																																																			sprintf(value_str[item], "%.3f", db_cmm->var.curve.eng_para.HI_RANGE );
																																																			item++;
																																																			
																																																			  strcpy(entry_str[item], OIX_STR_LO_RANGE);
																																																			  sprintf(value_str[item], "%.3f", db_cmm->var.curve.eng_para.LO_RANGE );
																																																			  item++;
																																																			  
																																																				for(i=0; i<(db_cmm->var.curve.seg_num+SEG_PER_ITEM-1)/SEG_PER_ITEM; i++)
																																																				{
																																																				end_seg= (i+1)*SEG_PER_ITEM < db_cmm->var.curve.seg_num ? (i+1)*SEG_PER_ITEM : db_cmm->var.curve.seg_num;
																																																				sprintf(entry_str[item], "seg%03d-%03d", i*SEG_PER_ITEM, end_seg-1);
																																																				
																																																				  pStr= value_str[item];
																																																				  for(j=i*SEG_PER_ITEM; j<end_seg; j++)
																																																				  pStr += sprintf(pStr, "%.3f ", db_cmm->var.curve.data.value[j]);
																																																				  item++;
																																																				  }			
																																																				  
																																																					DspDbDetailInfo(crt_n,drawable,gc,item,entry_str,value_str);	
																																																					}
																																																					
																																																					  
																																																						
																																																						  int DbDetailDspProc( int crt_n )
																																																						  {
																																																						  if(db_detail_info[crt_n] == NULL)
																																																						  return FALSE;
																																																						  
																																																							gdk_gc_set_foreground(db_detail_draw_gc[crt_n], &pixels[crt_n][BL_PURPLE5]);
																																																							gdk_draw_rectangle (DbDetailOffDrawable[crt_n],db_detail_draw_gc[crt_n],TRUE, 0,0,DBINFO_WINDOW_WIDTH, DBINFO_WINDOW_HEIGHT); 
																																																							DbDetailDspProc1( crt_n, DbDetailOffDrawable[crt_n] );
																																																							gdk_window_copy_area (db_detail_info_canvas[crt_n]->window,db_detail_draw_gc[crt_n],0,0,DbDetailOffDrawable[crt_n],0,0,DBINFO_WINDOW_WIDTH, DBINFO_WINDOW_HEIGHT);
																																																							return TRUE;
																																																							}
																																																							
																																																							  
																																																								int DbDetailDspProc1(int crt_n, GdkDrawable *drawable )
																																																								{
																																																								DMS_COMMON	db_cmm;
																																																								
																																																								  if(ReadEntryById(&db_tab_window_info[crt_n].select_pt, &db_cmm) == -1)
																																																								  {
																																																								  printf ("No this point in the database file !!!\n" );
																																																								  return FALSE;
																																																								  }
																																																								  
																																																									switch	( db_tab_window_info[crt_n].select_pt.data_type )
																																																									{
																																																									case	ANA_TYPE:
																																																									case	PARAM_TYPE:
																																																									case	OUTA_TYPE: 
																																																									case	CALC_TYPE:
																																																									DspAnaDetailInfo(crt_n ,drawable, db_detail_draw_gc[crt_n], &db_cmm);		
																																																									break;
																																																									
																																																									  case    POL_TYPE:
																																																									  case    SOE_TYPE:
																																																									  case    SYSMSG_TYPE:
																																																									  case    OUTS_TYPE:		
																																																									  DspPolDetailInfo(crt_n,drawable,db_detail_draw_gc[crt_n], &db_cmm );
																																																									  break;
																																																									  
																																																										case    IMP_TYPE:
																																																										DspImpDetailInfo(crt_n,drawable,db_detail_draw_gc[crt_n], &db_cmm );
																																																										break;
																																																										
																																																										  case	CHARA_TYPE:		
																																																										  DspCharaDetailInfo(crt_n,drawable,db_detail_draw_gc[crt_n], &db_cmm );
																																																										  break;
																																																										  
																																																											case	CURVE_TYPE:
																																																											DspCurveDetailInfo(crt_n,drawable,db_detail_draw_gc[crt_n], &db_cmm);
																																																											break;
																																																											
																																																											  default:
																																																											  break;
																																																											  }
																																																											  return TRUE;
																																																											  }
																																																											  
																																																												void db_detail_draw_area_realize(GtkWidget *widget, gpointer data)
																																																												{
																																																												int crt_n= (int)data;
																																																												
																																																												  db_detail_draw_gc[crt_n] =  gdk_gc_new(widget->window);
																																																												  DbDetailOffDrawable[crt_n] = gdk_pixmap_new(widget->window,DBINFO_WINDOW_WIDTH,DBINFO_WINDOW_HEIGHT,-1);
																																																												  gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]);
																																																												  }
																																																												  
																																																													void  db_detail_expose_events(GtkWidget *widget,GdkEventExpose *event, gpointer data)
																																																													{
																																																													int crt_n= (int)data;
																																																													
																																																													  DbDetailDspProc(crt_n);
}*/






