#include <stdio.h>
#include <gtk/gtk.h>
#ifdef SUN_OS
#include <pthread.h>
#endif
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"
int   fault_redisplay_on[MAX_CRT_NUM]  ; 
extern struct shm_hdb *shm_realdb_addr;
extern void	*WriteDbFromHDB(void*);
extern void *PeriodRequestData(void *arg);	
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
struct tm fault_starttime,fault_endtime;
extern int connect_id;

static GtkWidget*CreateFaultRedisplayBox (GtkWidget *parent ,int crt_n );
typedef struct 
{
	gint year;
	gint month;
	gint day;
	gint hour;
	gint minute;
	gint second;
}FAULT_REDISPLAY_TIME;
extern connect_id;

int step_id=0,fault_redisplay_crt_num=0;
char fault_redisplay_dsp_name[MAX_FULL_FILE_NAME_LEN]="";
void on_year_btn_value_changed              (GtkSpinButton   *spinbutton,gpointer         user_data);
void start_btn_clicked                      (GtkButton       *button, gpointer         user_data);
void cancel_btn_clicked                     (GtkButton       *button, gpointer         user_data);
void on_mon_btn_change_value                (GtkSpinButton   *spinbutton, GtkScrollType    arg1, gpointer         user_data);
void on_end_mon_btn_change_value            (GtkSpinButton   *spinbutton, GtkScrollType    arg1,gpointer         user_data);
void PopupFaultRedisplayBox (GtkWidget *parent ,int crt_n);
static GtkWidget* CreateValueEntryBox (GtkWidget *parent ,int crt_n,int entry_data_class);
void EndFaultRedisplay(GtkWidget *parent ,int crt_n );
GtkWidget *fault_redisplay_box[MAX_CRT_NUMBER];
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern int OpenDspFile (int crt_n, char	*filename);

void DestoryFaultRedisplayBox (GtkWidget *widget , gpointer data )
{
	int crt_n = (gint) data ;
	printf("in DestoryFaultRedisplayBox: crt_n=%d\n",crt_n);
	gtk_widget_destroy(GTK_WIDGET(fault_redisplay_box[crt_n ]));
	
	fault_redisplay_box[crt_n ] = NULL ;    
	
}

void PopupFaultRedisplayBox (GtkWidget *parent ,int crt_n)
{
	printf("in PopupFaultRedisplayBox: crt_n=%d\n",crt_n);
    if (fault_redisplay_box[crt_n ]== NULL) 
    {
        fault_redisplay_box[crt_n ] =  CreateFaultRedisplayBox(parent, crt_n); 
        gtk_widget_show( fault_redisplay_box[crt_n ]);
    }
	else       
        gtk_window_present(GTK_WINDOW(fault_redisplay_box[crt_n ]));      
}

void EndFaultRedisplay(GtkWidget *parent ,int crt_n )
{
	GtkWidget  *lab_fault_redisplay,*btn_pause,/**btn_fault_redisplay,*/*btn_step;	
	gchar *pConvert=0;
	//fault_redisplay_on =0;
	fault_redisplay_on[crt_n] = 0 ; 
	shm_hdb_addr->data_revert.queryflag =2;/*End fault redisplay flag*/
	lab_fault_redisplay = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"label_fault_redisplay");
	if (lab_fault_redisplay != NULL)
	{
		gtk_label_set_text(GTK_LABEL(lab_fault_redisplay),pConvert=_toUtf8EX( OIX_CSTR_FAULT_REDISPLAY_OFF));
		G_SAFE_FREE(pConvert);//dzy1017
	}
	else
		printf("lab_fault_redisplay is null\n");
	
	btn_step = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"btn_step");
	if(btn_step!=NULL)
		gtk_widget_hide(btn_step);
	btn_pause = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"pause_btn");	
	if(btn_pause!=NULL)
		gtk_widget_hide(btn_pause);
		/*shmdt(shm_hdb_addr);
		thr_exit(0);*/
	UnMountDB ();		
	MountDB (NULL);
	
}

static GtkWidget*
CreateFaultRedisplayBox (GtkWidget *parent ,int crt_n )
{
	GtkWidget *table1;
	GtkWidget *label1;
	GtkWidget *label2;
	GtkWidget *hbox1;
	GtkObject *year_btn_adj;
	GtkWidget *year_btn;
	GtkWidget *label3;
	GtkWidget *hbox3;
	GtkObject *day_btn_adj;
	GtkWidget *day_btn;
	GtkWidget *label5;
	GtkWidget *hbox4;
	GtkObject *hour_btn_adj;
	GtkWidget *hour_btn;
	GtkWidget *label6;
	GtkWidget *hbox5;
	GtkObject *min_btn_adj;
	GtkWidget *min_btn;
//	GtkWidget *label7;
//	GtkWidget *hbox6;
//	GtkObject *sec_btn_adj;
//	GtkWidget *sec_btn;
	GtkWidget *hbox7;
	GtkObject *end_year_btn_adj;
	GtkWidget *end_year_btn;
	GtkWidget *label8;
	GtkWidget *hbox9;
	GtkObject *end_day_btn_adj;
	GtkWidget *end_day_btn;
	GtkWidget *label10;
	GtkWidget *hbox10;
	GtkObject *end_hour_btn_adj;
	GtkWidget *end_hour_btn;
	GtkWidget *label11;
	GtkWidget *hbox11;
	GtkObject *end_min_btn_adj;
	GtkWidget *end_min_btn;
//	GtkWidget *label12;
//	GtkWidget *hbox12;
//	GtkObject *end_sec_btn_adj;
//	GtkWidget *end_sec_btn;
	GtkWidget *start_btn;
	GtkWidget *cancel_btn;
	GtkWidget *hbox13;
	GtkObject *mon_btn_adj;
	GtkWidget *mon_btn;
	GtkWidget *label13;
	GtkWidget *hbox14;
	GtkObject *end_mon_btn_adj;
	GtkWidget *end_mon_btn;
	GtkWidget *label14;
	GtkWidget *consistent_btn;
	GtkWidget *step_btn;

	gchar *pConvert=0;

	time_t     newtime;
	struct  tm      devtime;
	
	if(fault_redisplay_box[crt_n ]!=NULL)
	{
		PopupMessageBox(main_w[crt_n],"fault_redisplay_box is existed\n");
		return NULL;
	}
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	
	fault_redisplay_box[crt_n ] = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (fault_redisplay_box[crt_n ]),pConvert=_toUtf8EX(OIX_CSTR_FAULT_REDISPLAY) );
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_window_set_position (GTK_WINDOW (fault_redisplay_box[crt_n ]), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (fault_redisplay_box[crt_n ]), 11);  
	gtk_window_set_default_size (GTK_WINDOW (fault_redisplay_box[crt_n ]), 5, 5);
	gtk_window_set_resizable (GTK_WINDOW (fault_redisplay_box[crt_n ]), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(fault_redisplay_box[crt_n ]), GTK_WINDOW(parent));
	
	table1 = gtk_table_new (9, 8, FALSE);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (fault_redisplay_box[crt_n ]), table1);
	
	label1 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_START_TIME));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label1);
	gtk_table_attach (GTK_TABLE (table1), label1, 0, 1, 1, 2,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);
	
	label2 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_END_TIME));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label2);
	gtk_table_attach (GTK_TABLE (table1), label2, 0, 1, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_table_attach (GTK_TABLE (table1), hbox1, 2, 3, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	year_btn_adj = gtk_adjustment_new (2006, 2006, 2015, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(year_btn_adj) ,devtime.tm_year+1900);
	year_btn = gtk_spin_button_new (GTK_ADJUSTMENT (year_btn_adj), 1, 0);
	gtk_widget_show (year_btn);
	gtk_box_pack_start (GTK_BOX (hbox1), year_btn, TRUE, TRUE, 0);
	
	label3 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_YEAR));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label3);
	gtk_box_pack_start (GTK_BOX (hbox1), label3, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);
	
	hbox3 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox3);
	gtk_table_attach (GTK_TABLE (table1), hbox3, 4, 5, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	day_btn_adj = gtk_adjustment_new (1, 1, 31, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(day_btn_adj) ,devtime.tm_mday );
	day_btn = gtk_spin_button_new (GTK_ADJUSTMENT (day_btn_adj), 1, 0);
	gtk_widget_show (day_btn);
	gtk_box_pack_start (GTK_BOX (hbox3), day_btn, TRUE, TRUE, 0);
	
	label5 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_DAY));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label5);
	gtk_box_pack_start (GTK_BOX (hbox3), label5, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);
	
	hbox4 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox4);
	gtk_table_attach (GTK_TABLE (table1), hbox4, 5, 6, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	hour_btn_adj = gtk_adjustment_new (1, 0, 23, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(hour_btn_adj) ,devtime.tm_hour  );
	hour_btn = gtk_spin_button_new (GTK_ADJUSTMENT (hour_btn_adj), 1, 0);
	gtk_widget_show (hour_btn);
	gtk_box_pack_start (GTK_BOX (hbox4), hour_btn, TRUE, TRUE, 0);
	
	label6 = gtk_label_new (pConvert=_toUtf8(":"));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label6);
	gtk_box_pack_start (GTK_BOX (hbox4), label6, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);
	
	hbox5 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox5);
	gtk_table_attach (GTK_TABLE (table1), hbox5, 6, 7, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	min_btn_adj = gtk_adjustment_new (1, 0, 59, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(min_btn_adj) ,devtime.tm_min  );
	min_btn = gtk_spin_button_new (GTK_ADJUSTMENT (min_btn_adj), 1, 0);
	gtk_widget_show (min_btn);
	gtk_box_pack_start (GTK_BOX (hbox5), min_btn, TRUE, TRUE, 0);
	
	/*label7 = gtk_label_new (_toUtf8(":",strlen(":"),
	NULL,NULL,NULL));
	gtk_widget_show (label7);
	gtk_box_pack_start (GTK_BOX (hbox5), label7, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_LEFT);
	
	 hbox6 = gtk_hbox_new (FALSE, 0);
	 gtk_widget_show (hbox6);
	 gtk_table_attach (GTK_TABLE (table1), hbox6, 7, 8, 1, 2,
	 (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	 (GtkAttachOptions) (GTK_FILL), 0, 0);
	 
	  /* sec_btn_adj = gtk_adjustment_new (0, 0, 59, 1, 10, 10);
	  sec_btn = gtk_spin_button_new (GTK_ADJUSTMENT (sec_btn_adj), 1, 0);
	  gtk_widget_show (sec_btn);
	gtk_box_pack_start (GTK_BOX (hbox6), sec_btn, TRUE, TRUE, 0);*/
	
	hbox7 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox7);
	gtk_table_attach (GTK_TABLE (table1), hbox7, 2, 3, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_year_btn_adj = gtk_adjustment_new (2006, 2006, 2015, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(end_year_btn_adj) ,devtime.tm_year+1900); 
	end_year_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_year_btn_adj), 1, 0);
	gtk_widget_show (end_year_btn);
	gtk_box_pack_start (GTK_BOX (hbox7), end_year_btn, TRUE, TRUE, 0);
	
	
	label8 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_YEAR));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label8);
	gtk_box_pack_start (GTK_BOX (hbox7), label8, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);
	
	hbox9 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox9);
	gtk_table_attach (GTK_TABLE (table1), hbox9, 4, 5, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_day_btn_adj = gtk_adjustment_new (1, 1, 31, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(end_day_btn_adj) ,devtime.tm_mday ); 
	end_day_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_day_btn_adj), 1, 0);
	gtk_widget_show (end_day_btn);
	gtk_box_pack_start (GTK_BOX (hbox9), end_day_btn, TRUE, TRUE, 0);
	
	label10 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_DAY));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label10);
	gtk_box_pack_start (GTK_BOX (hbox9), label10, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label10), GTK_JUSTIFY_LEFT);
	
	hbox10 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox10);
	gtk_table_attach (GTK_TABLE (table1), hbox10, 5, 6, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_hour_btn_adj = gtk_adjustment_new (1, 0, 23, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(end_hour_btn_adj) ,devtime.tm_hour  );
	end_hour_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_hour_btn_adj), 1, 0);
	gtk_widget_show (end_hour_btn);
	gtk_box_pack_start (GTK_BOX (hbox10), end_hour_btn, TRUE, TRUE, 0);
	
	label11 = gtk_label_new (pConvert=_toUtf8(":"));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label11);
	gtk_box_pack_start (GTK_BOX (hbox10), label11, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label11), GTK_JUSTIFY_LEFT);
	
	hbox11 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox11);
	gtk_table_attach (GTK_TABLE (table1), hbox11, 6, 7, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_min_btn_adj = gtk_adjustment_new (1, 0, 59, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(end_min_btn_adj) ,devtime.tm_min  );
	end_min_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_min_btn_adj), 1, 0);
	gtk_widget_show (end_min_btn);
	gtk_box_pack_start (GTK_BOX (hbox11), end_min_btn, TRUE, TRUE, 0);
	
	/* label12 = gtk_label_new (_toUtf8(":",strlen(":"),
	NULL,NULL,NULL));
	gtk_widget_show (label12);
	gtk_box_pack_start (GTK_BOX (hbox11), label12, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label12), GTK_JUSTIFY_LEFT);
	
	 hbox12 = gtk_hbox_new (FALSE, 0);
	 gtk_widget_show (hbox12);
	 gtk_table_attach (GTK_TABLE (table1), hbox12, 7, 8, 4, 5,
	 (GtkAttachOptions) (GTK_FILL),
	 (GtkAttachOptions) (GTK_FILL), 0, 0);
	 
	  /* end_sec_btn_adj = gtk_adjustment_new (1, 0, 59, 1, 10, 10);
	  end_sec_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_sec_btn_adj), 1, 0);
	  gtk_widget_show (end_sec_btn);
	gtk_box_pack_start (GTK_BOX (hbox12), end_sec_btn, TRUE, TRUE, 0);*/
	
	start_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_START));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (start_btn);
	gtk_table_attach (GTK_TABLE (table1), start_btn, 2, 3, 9, 10,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	cancel_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (cancel_btn);
	gtk_table_attach (GTK_TABLE (table1), cancel_btn, 4, 5, 9, 10,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	consistent_btn = gtk_radio_button_new_with_mnemonic (NULL,pConvert=_toUtf8EX(OIX_CSTR_CONSISTENT));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (consistent_btn);
	gtk_table_attach (GTK_TABLE (table1), consistent_btn, 2, 3, 7, 8,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	step_btn = gtk_radio_button_new_with_label (gtk_radio_button_group(GTK_RADIO_BUTTON(consistent_btn)),
		pConvert=_toUtf8EX(OIX_CSTR_STEP_BY_STEP) );
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (step_btn);
	gtk_table_attach (GTK_TABLE (table1), step_btn, 4, 5, 7, 8,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	hbox13 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox13);
	gtk_table_attach (GTK_TABLE (table1), hbox13, 3, 4, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	mon_btn_adj = gtk_adjustment_new (1, 1, 12, 1, 10, 10);
	mon_btn = gtk_spin_button_new (GTK_ADJUSTMENT (mon_btn_adj), 1, 0);
	gtk_widget_show (mon_btn);
	gtk_box_pack_start (GTK_BOX (hbox13), mon_btn, TRUE, TRUE, 0);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(mon_btn_adj) ,devtime.tm_mon  +1);
	label13 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_MONTH));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label13);
	gtk_box_pack_start (GTK_BOX (hbox13), label13, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label13), GTK_JUSTIFY_LEFT);
	
	hbox14 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox14);
	gtk_table_attach (GTK_TABLE (table1), hbox14, 3, 4, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_mon_btn_adj = gtk_adjustment_new (1, 1, 12, 1, 10, 10);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(end_mon_btn_adj) ,devtime.tm_mon  +1);
	end_mon_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_mon_btn_adj), 1, 0);
	gtk_widget_show (end_mon_btn);
	gtk_box_pack_start (GTK_BOX (hbox14), end_mon_btn, TRUE, TRUE, 0);
	gtk_widget_set_usize (end_mon_btn, 30, -2);
	
	label14 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_MONTH));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (label14);
	gtk_box_pack_start (GTK_BOX (hbox14), label14, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label14), GTK_JUSTIFY_LEFT);
	
	gtk_signal_connect (GTK_OBJECT (year_btn), "value_changed",
		GTK_SIGNAL_FUNC (on_year_btn_value_changed),
		GINT_TO_POINTER(crt_n));
	
	gtk_signal_connect (GTK_OBJECT (start_btn), "clicked",
		GTK_SIGNAL_FUNC (start_btn_clicked),
		GINT_TO_POINTER(crt_n));
	gtk_signal_connect (GTK_OBJECT (cancel_btn), "clicked",
		GTK_SIGNAL_FUNC (cancel_btn_clicked),
		GINT_TO_POINTER(crt_n));
	gtk_signal_connect (GTK_OBJECT (mon_btn), "change_value",
		GTK_SIGNAL_FUNC (on_mon_btn_change_value),
		GINT_TO_POINTER(crt_n));
	gtk_signal_connect (GTK_OBJECT (end_mon_btn), "change_value",
		GTK_SIGNAL_FUNC (on_end_mon_btn_change_value),
		GINT_TO_POINTER(crt_n));
	
	/* Store pointers to all widgets, for use by lookup_widget(). */
	
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"year_btn",year_btn);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"day_btn",day_btn);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"hour_btn",hour_btn);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"min_btn",min_btn);
	/*gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"sec_btn",sec_btn);*/
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"end_year_btn",end_year_btn);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"end_day_btn",end_day_btn);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"end_hour_btn",end_hour_btn);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"end_min_btn",end_min_btn);
	/*gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"end_sec_btn",end_sec_btn);*/
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"mon_btn",mon_btn);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"end_mon_btn",end_mon_btn);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"end_min_btn_adj",end_min_btn_adj);
	gtk_object_set_data(GTK_OBJECT(fault_redisplay_box[crt_n ]),"consistent_btn",consistent_btn);
	
	return fault_redisplay_box[crt_n ]; 
}

void GetSpinBtnData(int crt_n,FAULT_REDISPLAY_TIME *start,FAULT_REDISPLAY_TIME *end,int *redisplay_mode)
{	
	GtkWidget* radio_btn;
	GtkSpinButton *start_year , *start_month ,  *start_day , *start_hour, *start_minute ;
	GtkSpinButton *end_year  ,*end_month , *end_day, *end_hour, *end_minute ;

	start_year = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"year_btn");
	start->year = gtk_spin_button_get_value_as_int(start_year);
	
	start_month = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"mon_btn");
	start->month = gtk_spin_button_get_value_as_int(start_month);
	
	start_day = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"day_btn");
	start->day = gtk_spin_button_get_value_as_int(start_day);
	
	start_hour = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"hour_btn");
	start->hour = gtk_spin_button_get_value_as_int(start_hour);
	
	start_minute = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"min_btn");
	start->minute = gtk_spin_button_get_value_as_int(start_minute);
	
	/*GtkSpinButton *start_sec = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"sec_btn");
	start->second = gtk_spin_button_get_value_as_int(start_sec);*/
	
	end_year = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"end_year_btn");
	end->year = gtk_spin_button_get_value_as_int(end_year);
	
	end_month = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"end_mon_btn");
	end->month = gtk_spin_button_get_value_as_int(end_month);
	
	end_day = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"end_day_btn");
	end->day = gtk_spin_button_get_value_as_int(end_day);
	
	end_hour = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"end_hour_btn");
	end->hour = gtk_spin_button_get_value_as_int(end_hour);
	
	end_minute = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"end_min_btn");
	end->minute = gtk_spin_button_get_value_as_int(end_minute);
	
	/*GtkSpinButton *end_sec = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"end_sec_btn");
	end->second = gtk_spin_button_get_value_as_int(end_sec);*/
	
	radio_btn = gtk_object_get_data(GTK_OBJECT(fault_redisplay_box[crt_n]),"consistent_btn");
	
	*redisplay_mode = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_btn));
	
}

void
on_year_btn_value_changed              (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
	
}

void PauseRedisplay(GtkButton       *button, gpointer         user_data)
{
	int crt_n=(int)user_data;
	GtkWidget  *btn_pause;
	gchar *pConvert=0;

	btn_pause = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"pause_btn");
	if(shm_hdb_addr->data_revert.stopflag ==0)
	{
		shm_hdb_addr->data_revert.stopflag =1;
		gtk_object_set(GTK_OBJECT(btn_pause),"GtkButton::label",pConvert=_toUtf8EX(OIX_CSTR_CONTINUE),NULL);
	}
	else
	{
		shm_hdb_addr->data_revert.stopflag =0;
		gtk_object_set(GTK_OBJECT(btn_pause),"GtkButton::label",pConvert=_toUtf8EX(OIX_CSTR_PAUSE),NULL);
	}
	G_SAFE_FREE(pConvert);//dzy1017
}

void StepRedisplay(GtkButton       *button, gpointer         user_data)
{
	int crt_n=(int)user_data;
	GtkWidget  *btn_step;
	if(connect_id==1)
	{
		PopupMessageBox(main_w[crt_n],"Connecting HDB! Please wait!");
		return;
	}
	printf("!!!step_id=%d,crt_n=%d\n",step_id,crt_n);
	btn_step = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"btn_step");
	if(shm_hdb_addr->data_revert.stopflag ==0)
	{
		shm_hdb_addr->data_revert.stopflag =1;	
		step_id =1;	
	}
	else
	{
		shm_hdb_addr->data_revert.stopflag =0;	
		step_id =0;	
	}
}

void
start_btn_clicked                     (GtkButton       *button,
									   gpointer         user_data)
{
	int crt_n=(int)user_data;
	int res ; 
	char		cmd_str[100];
	FAULT_REDISPLAY_TIME start_time,end_time;
	GtkWidget  *lab_fault_redisplay,/**btn_consistent,*/*btn_pause,*btn_step/*,*radio_btn*/;
	char label[50],tmp[30]/*,dsp_filename[20]*/;
#ifdef  SUN_OS
	pthread_t tid1,tid2;
	int error;
#endif
#ifdef	WINDOWS_OS
	HANDLE  Hthread ; 
#endif 
	//int error/*,total_period*/;
	gchar *pConvert=0;
	int redisplay_mode;
	time_t tTim_start,tTim_end,tTim_now;
	
	time_t	newtime;
	struct	tm	devtime;
	struct tm now_time;
	char	*filename=NULL;
	
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	now_time.tm_year = devtime.tm_year;
	now_time.tm_mon =devtime.tm_mon;
	now_time.tm_mday= devtime.tm_mday-7;/*only save 7 days data*/
	now_time.tm_hour = devtime.tm_hour;
	now_time.tm_min = devtime.tm_min;
	now_time.tm_sec = devtime.tm_sec;
	tTim_now = mktime(&now_time);
	
	
	/* struct tm fault_starttime,fault_endtime;*/
	
	
	GetSpinBtnData(crt_n,&start_time,&end_time,&redisplay_mode);
	start_time.second =0;
	end_time.second =0;
	
	fault_starttime.tm_year		= start_time.year-1900;
	fault_starttime.tm_mon		= start_time.month-1;
	fault_starttime.tm_mday		= start_time.day;
	fault_starttime.tm_hour		= start_time.hour;
	fault_starttime.tm_min		= start_time.minute;
	fault_starttime.tm_sec		= start_time.second;
	fault_endtime.tm_year		= end_time.year-1900;
	fault_endtime.tm_mon		= end_time.month-1;
	fault_endtime.tm_mday		= end_time.day;
	fault_endtime.tm_hour		= end_time.hour;
	fault_endtime.tm_min		= end_time.minute;
	fault_endtime.tm_sec		= end_time.second;
	
	tTim_start = mktime(&fault_starttime);
	
	printf("*****tTim_now=%d,tTim_start=%d\n",tTim_now,tTim_start);
	
	if(0  && tTim_start<tTim_now) //rm by chichichi....
	{
		PopupMessageBox(main_w[crt_n],MSG_FAULT_DATA_ONLY_SAVE_SEVEN_DAYS);
		return;
	}
	
	//fault_redisplay_on = 1;
	lab_fault_redisplay = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"label_fault_redisplay");
	if (lab_fault_redisplay != NULL)
	{
		strcpy(label,OIX_CSTR_FAULT_REDISPLAY_ON);
		strcat(label,":");
		sprintf(tmp,"%d-%d-%d %d:%d:%d",start_time.year,start_time.month,start_time.day,
			start_time.hour,start_time.minute,start_time.second);
		strcat(label,tmp);
		gtk_label_set_text(GTK_LABEL(lab_fault_redisplay),pConvert=_toUtf8( label));	
		G_SAFE_FREE(pConvert);//dzy1017
	}
	
	strcpy(shm_hdb_addr->data_revert.starttime,tmp);
	sprintf(shm_hdb_addr->data_revert.endtime,"%d-%d-%d %d:%d:%d",end_time.year,end_time.month,end_time.day,
		end_time.hour,end_time.minute,end_time.second);
	
	if(redisplay_mode==TRUE)
	{
		btn_pause = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"pause_btn");	
		gtk_widget_show (btn_pause);
		gtk_signal_connect (GTK_OBJECT (btn_pause),"clicked",GTK_SIGNAL_FUNC(PauseRedisplay),GINT_TO_POINTER(crt_n)) ;
		gtk_widget_set_sensitive(btn_pause, TRUE);	
		shm_hdb_addr->data_revert.querytype = CONSISTENT;
	}
	else
	{
		shm_hdb_addr->data_revert.querytype = STEP_BY_STEP;
		btn_step = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"btn_step");	
		gtk_object_set(GTK_OBJECT(btn_step),"GtkButton::label",pConvert=_toUtf8EX(OIX_CSTR_NEXT),NULL);
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_signal_connect (GTK_OBJECT (btn_step),"clicked",GTK_SIGNAL_FUNC(StepRedisplay),GINT_TO_POINTER(crt_n)) ;
		gtk_widget_show (btn_step);
	}
	shm_hdb_addr->data_revert.stopflag	= 0;	
	
	/* Submit the start_time and end_time to HDB */
#ifdef  SUN_OS	
	strcpy(cmd_str,"cp //home//ems//h9000//dat//");	
	strcat(cmd_str,DATABASE_NAME);	
	strcat(cmd_str," ");	
	strcat(cmd_str,HIS_DATABASE_NAME);	
#endif
#ifdef  WINDOWS_OS
	strcpy(cmd_str,"copy \\home\\ems\\h9000\\dat\\");	
	strcat(cmd_str,DATABASE_NAME);	
	strcat(cmd_str," ");	
	strcat(cmd_str,HIS_DATABASE_NAME);
#endif 
	res = system(cmd_str);		
    MySleep(5); 	
    UnMountDB ();
	if(-1==MountDBFile(HIS_DATABASE_NAME,TRUE))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}		
	DestoryFaultRedisplayBox(GTK_WIDGET(button),user_data);
	
	strcpy(fault_redisplay_dsp_name,cur_dsp_name[crt_n]);
	fault_redisplay_crt_num = crt_n;
	//OpenDspFile (crt_n,cur_dsp_name[crt_n]+strlen("//home//ems//h9000//dsp//") ); /* get logicalname,ycnum ,yxnum*/
#ifdef WINDOWS_OS
	if(filename=strrchr(fault_redisplay_dsp_name, '\\'))
#else
	if(filename=strrchr(fault_redisplay_dsp_name, '/'))
#endif
		filename= filename+1;
	printf("\nOIX: cur fault redisplay dsp file is: %s", filename);
	fault_redisplay_on[crt_n] = 1 ; 
	OpenDspFile (crt_n,filename ); /* get logicalname,ycnum ,yxnum*/
	
	
	shm_hdb_addr->data_revert.queryflag = 1;
	shm_realdb_addr = shm_hdb_addr;
	
	
	tTim_end   = mktime(&fault_endtime);
#ifdef  SUN_OS
	error = pthread_create(&tid1, NULL, WriteDbFromHDB, (void*)(crt_n ));
	if(error!=-1)
	{
		printf("Create WriteDb Success!\n");
		fault_redisplay_on[crt_n] = 1 ; 
	}
	else
	{
		printf("Create WriteDb failed!!! error=%d\n",error);
		fault_redisplay_on[crt_n] = 0 ; 
	}
#endif 

#ifdef  WINDOWS_OS
	Hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WriteDbFromHDB, (void *)(crt_n) , 0, NULL) ; 
	if(Hthread!=NULL)
	{
		printf("Create WriteDb Success!\n");
		fault_redisplay_on[crt_n] = 1 ; 
	}
	else
	{
		printf("Create WriteDb failed!!! error=%d\n",Hthread);
		fault_redisplay_on[crt_n] = 0 ; 
	}
#endif
	
	

}


void
cancel_btn_clicked						(GtkButton       *button,
                                        gpointer         user_data)
{
	DestoryFaultRedisplayBox(GTK_WIDGET(button),user_data);
	
	shm_hdb_addr->data_revert.queryflag = 0;
}


void
on_mon_btn_change_value                (GtkSpinButton   *spinbutton,
                                        GtkScrollType    arg1,
                                        gpointer         user_data)
{
	
}


void
on_end_mon_btn_change_value            (GtkSpinButton   *spinbutton,
                                        GtkScrollType    arg1,
                                        gpointer         user_data)
{
	
}


