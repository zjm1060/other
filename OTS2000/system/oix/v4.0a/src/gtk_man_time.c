/*  sample :
    extern   DATE_TIME_ man_time_set[MAX_CRT_NUMBER]; 

	man_time_box[0]  = CreateManTimeSetBox ( main_w[0] ,  0 )  ;  //chichi
	gtk_widget_show(man_time_box[0])  ; 
	return  man_time_set[crt_n] 为时间的返回值；

*/

#include <stdio.h>
#include <gtk/gtk.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"
GtkWidget  *man_time_box[MAX_CRT_NUMBER ];


typedef struct 
{
	gint year;
	gint month;
	gint day;
	gint hour;
	gint minute;
	gint second;
}DATE_TIME_;
struct tm set_starttime ;
DATE_TIME_ man_time_set[MAX_CRT_NUMBER];
extern void FormManTimeSetPacket(int crt_n,DMS_COMMON *dms_cmm, char	*oper_status);
extern void	ProgDoAckCelBrdcst (int crt_n )  ; 
GtkWidget*CreateManTimeSetBox (GtkWidget *parent ,int crt_n );

gint DestoryTimeSetBox (GtkWidget *widget , gpointer data )
{
	int crt_n = (gint) data ;
	gtk_widget_destroy(GTK_WIDGET(man_time_box[crt_n ]));	
	man_time_box[crt_n ] = NULL ;   
	return 1 ; 
}



void PopupTimeManSetBox (GtkWidget *parent ,int crt_n)
{
	printf("in PopupFaultRedisplayBox: crt_n=%d\n",crt_n);
    if (man_time_box[crt_n ]== NULL) 
    {
        man_time_box[crt_n ] =  CreateManTimeSetBox(parent, crt_n); 
        gtk_widget_show( man_time_box[crt_n ]);
    }
	else       
        gtk_window_present(GTK_WINDOW(man_time_box[crt_n ]));      
}

void GetManTimeData(int crt_n,DATE_TIME_ *time_set)
{	
	GtkWidget* radio_btn;
	GtkSpinButton *start_year , *start_month ,  *start_day , *start_hour, *start_minute ;
	GtkSpinButton *end_year  ,*end_month , *end_day, *end_hour, *end_minute ;

	start_year = gtk_object_get_data(GTK_OBJECT(man_time_box[crt_n]),"year_btn");
	time_set->year = gtk_spin_button_get_value_as_int(start_year);
	
	start_month = gtk_object_get_data(GTK_OBJECT(man_time_box[crt_n]),"mon_btn");
	time_set->month = gtk_spin_button_get_value_as_int(start_month);
	
	start_day = gtk_object_get_data(GTK_OBJECT(man_time_box[crt_n]),"day_btn");
	time_set->day = gtk_spin_button_get_value_as_int(start_day);
 
 
	
	end_year = gtk_object_get_data(GTK_OBJECT(man_time_box[crt_n]),"end_year_btn");
	time_set->hour  = gtk_spin_button_get_value_as_int(end_year);
	
	end_month = gtk_object_get_data(GTK_OBJECT(man_time_box[crt_n]),"end_mon_btn");
	time_set->month = gtk_spin_button_get_value_as_int(end_month);
	
	end_day = gtk_object_get_data(GTK_OBJECT(man_time_box[crt_n]),"end_day_btn");
	time_set->day = gtk_spin_button_get_value_as_int(end_day);
 
	
}


void
on_time_set_clicked                     (GtkButton       *button,
									   gpointer         user_data)
{
	time_t  tvalue ;
	struct tm twhen ; 
	
	int crt_n=(int)user_data ;  
	GetManTimeData(crt_n,&man_time_set[crt_n] );
     
	twhen.tm_year = man_time_set[crt_n].year - 1900 ; 
	twhen.tm_mon  = man_time_set[crt_n].month -1  ;
	twhen.tm_mday =man_time_set[crt_n].day      ;
	twhen.tm_hour  = man_time_set[crt_n].hour   ;
	twhen.tm_min  = man_time_set[crt_n].minute   ;
	twhen.tm_sec  = man_time_set[crt_n].second ; 
	tvalue = mktime(&twhen);
	cmd_buf[crt_n].data.iValue = tvalue;   //1970年1月1日以来的累计秒数 ; 
	ProgDoAckCelBrdcst ( crt_n );




	gtk_widget_destroy(GTK_WIDGET(man_time_box[crt_n ]));	
	man_time_box[crt_n ] = NULL ; 
 
}

void
on_time_cancle_clicked						(GtkButton       *button,
										 gpointer         user_data)
{
	DestoryTimeSetBox(GTK_WIDGET(button),user_data);
 
}





GtkWidget* CreateManTimeSetBox (GtkWidget *parent ,int crt_n )
{
	GtkWidget *table1 ,*window;
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
	GtkWidget *label7;
	GtkWidget *hbox6;
	GtkObject *sec_btn_adj;
	GtkWidget *sec_btn;
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
	GtkWidget *label12;
	GtkWidget *hbox12;
	GtkObject *end_sec_btn_adj;
	GtkWidget *end_sec_btn;
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
	
	time_t     newtime;
	struct  tm      devtime;
	
	gchar *pConvert=0;
 
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), pConvert=_toUtf8EX("手动时间设定") );
	G_SAFE_FREE(pConvert);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 11);  
	gtk_window_set_default_size (GTK_WINDOW (window), 5, 5);
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));
	
	table1 = gtk_table_new (9, 8, FALSE);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (window), table1);
	
	label1 = gtk_label_new (pConvert=_toUtf8EX("时间设定：年/月/日"));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label1);
	gtk_table_attach (GTK_TABLE (table1), label1, 0, 1, 1, 2,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);
	
	label2 = gtk_label_new (pConvert=_toUtf8EX("时间设定：时：分：秒"));     //"OIX_CSTR_END_TIME"
	G_SAFE_FREE(pConvert);
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
	
	year_btn_adj = gtk_adjustment_new (2006, 2006, 2025, 1, 10, 10);
	gtk_adjustment_set_value(year_btn_adj ,devtime.tm_year+1900);
	year_btn = gtk_spin_button_new (GTK_ADJUSTMENT (year_btn_adj), 1, 0);
	gtk_widget_show (year_btn);
	gtk_box_pack_start (GTK_BOX (hbox1), year_btn, TRUE, TRUE, 0);
	
	label3 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_YEAR));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label3);
	gtk_box_pack_start (GTK_BOX (hbox1), label3, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);
	
	hbox3 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox3);
	gtk_table_attach (GTK_TABLE (table1), hbox3, 4, 5, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	day_btn_adj = gtk_adjustment_new (1, 1, 31, 1, 10, 10);
	gtk_adjustment_set_value(day_btn_adj ,devtime.tm_mday );
	day_btn = gtk_spin_button_new (GTK_ADJUSTMENT (day_btn_adj), 1, 0);
	gtk_widget_show (day_btn);
	gtk_box_pack_start (GTK_BOX (hbox3), day_btn, TRUE, TRUE, 0);
	
	label5 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_DAY));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label5);
	gtk_box_pack_start (GTK_BOX (hbox3), label5, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);
	
	hbox4 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox4);
	gtk_table_attach (GTK_TABLE (table1), hbox4, 5, 6, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	hour_btn_adj = gtk_adjustment_new (1, 0, 23, 1, 10, 10);
	gtk_adjustment_set_value(hour_btn_adj ,devtime.tm_hour  );
	hour_btn = gtk_spin_button_new (GTK_ADJUSTMENT (hour_btn_adj), 1, 0);
//	gtk_widget_show (hour_btn);
	gtk_box_pack_start (GTK_BOX (hbox4), hour_btn, TRUE, TRUE, 0);
	
	label6 = gtk_label_new (pConvert=_toUtf8(":"));
	G_SAFE_FREE(pConvert);
//	gtk_widget_show (label6);
	gtk_box_pack_start (GTK_BOX (hbox4), label6, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);
	
	hbox5 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox5);
	gtk_table_attach (GTK_TABLE (table1), hbox5, 6, 7, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	min_btn_adj = gtk_adjustment_new (1, 0, 59, 1, 10, 10);
	gtk_adjustment_set_value(min_btn_adj ,devtime.tm_min  );
	min_btn = gtk_spin_button_new (GTK_ADJUSTMENT (min_btn_adj), 1, 0);
//	gtk_widget_show (min_btn);
	gtk_box_pack_start (GTK_BOX (hbox5), min_btn, TRUE, TRUE, 0);
	
 
	
	hbox7 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox7);
	gtk_table_attach (GTK_TABLE (table1), hbox7, 2, 3, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_year_btn_adj = gtk_adjustment_new (1, 0, 23, 1, 10, 10);  // hour_btn
	gtk_adjustment_set_value(end_year_btn_adj ,devtime.tm_hour); 
	end_year_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_year_btn_adj), 1, 0);
	gtk_widget_show (end_year_btn);
	gtk_box_pack_start (GTK_BOX (hbox7), end_year_btn, TRUE, TRUE, 0);
	
	
	label8 = gtk_label_new (pConvert=_toUtf8EX(":"));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label8);
	gtk_box_pack_start (GTK_BOX (hbox7), label8, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);
	
	hbox9 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox9);
	gtk_table_attach (GTK_TABLE (table1), hbox9, 4, 5, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_day_btn_adj = gtk_adjustment_new (1, 0, 59, 1, 10, 10);
	gtk_adjustment_set_value(end_day_btn_adj ,devtime.tm_sec ); 
	end_day_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_day_btn_adj), 1, 0);
	gtk_widget_show (end_day_btn);
	gtk_box_pack_start (GTK_BOX (hbox9), end_day_btn, TRUE, TRUE, 0);
	
	label10 = gtk_label_new (pConvert=_toUtf8EX(":"));
	G_SAFE_FREE(pConvert);
//	gtk_widget_show (label10);
	gtk_box_pack_start (GTK_BOX (hbox9), label10, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label10), GTK_JUSTIFY_LEFT);
	
	hbox10 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox10);
	gtk_table_attach (GTK_TABLE (table1), hbox10, 5, 6, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_hour_btn_adj = gtk_adjustment_new (1, 0, 23, 1, 10, 10);
	gtk_adjustment_set_value(end_hour_btn_adj ,devtime.tm_sec  );
	end_hour_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_hour_btn_adj), 1, 0);
	//gtk_widget_show (end_hour_btn);
	gtk_box_pack_start (GTK_BOX (hbox10), end_hour_btn, TRUE, TRUE, 0);
	
	label11 = gtk_label_new (pConvert=_toUtf8(":"));
	G_SAFE_FREE(pConvert);
	//gtk_widget_show (label11);
	gtk_box_pack_start (GTK_BOX (hbox10), label11, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label11), GTK_JUSTIFY_LEFT);
	
	hbox11 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox11);
	gtk_table_attach (GTK_TABLE (table1), hbox11, 6, 7, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_min_btn_adj = gtk_adjustment_new (1, 0, 59, 1, 10, 10);
	gtk_adjustment_set_value(end_min_btn_adj ,devtime.tm_min  );
	end_min_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_min_btn_adj), 1, 0);
	//gtk_widget_show (end_min_btn);
	gtk_box_pack_start (GTK_BOX (hbox11), end_min_btn, TRUE, TRUE, 0);
	
 
	
	start_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX("确定"));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (start_btn);
	gtk_table_attach (GTK_TABLE (table1), start_btn, 2, 3, 9, 10,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	cancel_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (cancel_btn);
	gtk_table_attach (GTK_TABLE (table1), cancel_btn, 4, 5, 9, 10,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
// 	consistent_btn = gtk_radio_button_new_with_mnemonic (NULL,_toUtf8EX(OIX_CSTR_CONSISTENT));
// 	gtk_widget_show (consistent_btn);
// 	gtk_table_attach (GTK_TABLE (table1), consistent_btn, 2, 3, 7, 8,
// 		(GtkAttachOptions) (GTK_FILL),
// 		(GtkAttachOptions) (0), 0, 0);
// 	
// 	step_btn = gtk_radio_button_new_with_label (gtk_radio_button_group(GTK_RADIO_BUTTON(consistent_btn)),
// 		_toUtf8EX(OIX_CSTR_STEP_BY_STEP) );
// 	gtk_widget_show (step_btn);
// 	gtk_table_attach (GTK_TABLE (table1), step_btn, 4, 5, 7, 8,
// 		(GtkAttachOptions) (GTK_FILL),
// 		(GtkAttachOptions) (0), 0, 0);
	
	hbox13 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox13);
	gtk_table_attach (GTK_TABLE (table1), hbox13, 3, 4, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	mon_btn_adj = gtk_adjustment_new (1, 1, 12, 1, 10, 10);
	mon_btn = gtk_spin_button_new (GTK_ADJUSTMENT (mon_btn_adj), 1, 0);
	gtk_widget_show (mon_btn);
	gtk_box_pack_start (GTK_BOX (hbox13), mon_btn, TRUE, TRUE, 0);
	gtk_adjustment_set_value(mon_btn_adj ,devtime.tm_mon  +1);
	label13 = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_MONTH));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label13);
	gtk_box_pack_start (GTK_BOX (hbox13), label13, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label13), GTK_JUSTIFY_LEFT);
	
	hbox14 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox14);
	gtk_table_attach (GTK_TABLE (table1), hbox14, 3, 4, 4, 5,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (GTK_FILL), 0, 0);
	
	end_mon_btn_adj = gtk_adjustment_new (1, 0, 59, 1, 10, 10);
	gtk_adjustment_set_value(end_mon_btn_adj ,devtime.tm_min);
	end_mon_btn = gtk_spin_button_new (GTK_ADJUSTMENT (end_mon_btn_adj), 1, 0);
	gtk_widget_show (end_mon_btn);
	gtk_box_pack_start (GTK_BOX (hbox14), end_mon_btn, TRUE, TRUE, 0);
	gtk_widget_set_usize (end_mon_btn, 30, -2);
	
	label14 = gtk_label_new (pConvert=_toUtf8EX(":"));//OIX_CSTR_MONTH
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label14);
	gtk_box_pack_start (GTK_BOX (hbox14), label14, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label14), GTK_JUSTIFY_LEFT);
 	
 
	gtk_signal_connect (GTK_OBJECT (start_btn), "clicked",
		GTK_SIGNAL_FUNC (on_time_set_clicked),
		(gpointer)crt_n);
	gtk_signal_connect (GTK_OBJECT (cancel_btn), "clicked",
		GTK_SIGNAL_FUNC (on_time_cancle_clicked),
		(gpointer)crt_n);
 
	
	/* Store pointers to all widgets, for use by lookup_widget(). */
	
	gtk_object_set_data(GTK_OBJECT(window),"year_btn",year_btn);
	gtk_object_set_data(GTK_OBJECT(window),"day_btn",day_btn);
	gtk_object_set_data(GTK_OBJECT(window),"hour_btn",hour_btn);
	gtk_object_set_data(GTK_OBJECT(window),"min_btn",min_btn);
	/*gtk_object_set_data(GTK_OBJECT(window),"sec_btn",sec_btn);*/
	gtk_object_set_data(GTK_OBJECT(window),"end_year_btn",end_year_btn);
	gtk_object_set_data(GTK_OBJECT(window),"end_day_btn",end_day_btn);
	gtk_object_set_data(GTK_OBJECT(window),"end_hour_btn",end_hour_btn);
	gtk_object_set_data(GTK_OBJECT(window),"end_min_btn",end_min_btn);
	/*gtk_object_set_data(GTK_OBJECT(window),"end_sec_btn",end_sec_btn);*/
	gtk_object_set_data(GTK_OBJECT(window),"mon_btn",mon_btn);
	gtk_object_set_data(GTK_OBJECT(window),"end_mon_btn",end_mon_btn);
	gtk_object_set_data(GTK_OBJECT(window),"end_min_btn_adj",end_min_btn_adj);
	gtk_object_set_data(GTK_OBJECT(window),"consistent_btn",consistent_btn);
	
	return window; 
}
