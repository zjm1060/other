#include <gtk/gtk.h>
#include "../inc/oix.h"
char buf[64] ; 
extern GtkWidget *alm_pop_shell[MAX_CRT_NUM]  ; 

#ifndef GLADE_HOOKUP_OBJECT
#define GLADE_HOOKUP_OBJECT(component,widget,name) \
	g_object_set_data_full (G_OBJECT (component), name, \
gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)
#endif

void
on_ok_btn                      (GtkButton       *button,
								gpointer         user_data)
{   
	char   buf[64] ,crt_n; 
	int  ch_event_tab =0 ,ch_db_tab =0  ,ch_alm_tab = 0 ; 
	GtkWidget*picker  ;
	picker = g_object_get_data( (GtkWidget*)user_data,"picker");
	strcpy(buf, gtk_font_button_get_font_name  (   picker));
	////////////////////change alarm and event tab font size!//////////////////////////
	if  (strcmp(buf,info[0].event_font))
	{
		ch_event_tab  =1 ; 
		strcpy (&info[0].event_font,buf);
	}
	
	for (crt_n =0 ;crt_n<3; crt_n++)
	{
		if (ch_event_tab&&alm_pop_shell[crt_n])
		{
			AlmDspProc( crt_n ) ;
			ch_event_tab = 0 ;
		}
		
	}
	////////////////////change dbtab font size!//////////////////////////
	picker = g_object_get_data( (GtkWidget*)user_data,"picker1");
	strcpy(buf, gtk_font_button_get_font_name  (   picker));
	if  (strcmp(buf,info[0].db_font))
	{
		ch_db_tab  =1 ; 
		strcpy (&info[0].db_font,buf);
	}
	
	for (crt_n =0 ;crt_n<3; crt_n++)
	{
		if (ch_db_tab&&db_pop_shell[crt_n])
		{
			DbDspProc(   crt_n  )  ;
			ch_db_tab  =0 ; 
		}
		
	}
	
	gtk_widget_destroy ((GtkWidget*)user_data);
}

void
on_cancle_btn                      (GtkButton       *button,
									gpointer         user_data)
{
	gtk_widget_destroy ((GtkWidget*)user_data);
	
}

GtkWidget *
do_font_pickers (GtkWidget *parent)
{
	static GtkWidget *window = NULL;
	GtkWidget *table, *label, *picker,*picker1,*picker2,*btnOK ,*btnCancel;
	gchar *pConvert=0;
	if (!window)
	{
		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_screen (GTK_WINDOW (window),
			gtk_widget_get_screen (parent));
		gtk_window_set_title (GTK_WINDOW (window), pConvert=_toUtf8("字体选择"));
		G_SAFE_FREE(pConvert);//dzy1017
		g_signal_connect (window, "destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&window);
		
		gtk_container_set_border_width (GTK_CONTAINER (window), 10);
		
		table = gtk_table_new (4, 2, FALSE);    
		gtk_table_set_col_spacing (GTK_TABLE (table), 0, 10);
		gtk_table_set_row_spacings (GTK_TABLE (table), 3);
		gtk_container_add (GTK_CONTAINER (window), table);
		
		gtk_container_set_border_width (GTK_CONTAINER (table), 10);
		
		label = gtk_label_new (pConvert=_toUtf8("报警一览表字体:"));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
		picker = gtk_font_button_new ();
		gtk_font_button_set_font_name   (picker,info[0].event_font);
		gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
		gtk_table_attach_defaults (GTK_TABLE (table), picker, 1, 2, 0, 1);
		
		label = gtk_label_new (pConvert=_toUtf8("数据库一览表字体:"));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
		picker1 = gtk_font_button_new ();
		gtk_font_button_set_font_name   (picker1,info[0].db_font);
		
		gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 1, 2);
		gtk_table_attach_defaults (GTK_TABLE (table), picker1, 1, 2, 1, 2);
		
		// 		label = gtk_label_new (_toUtf8("数据库一览表字体:"));
		// 		gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
		// 		picker2 = gtk_font_button_new ();
		// 		gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);
		// 		gtk_table_attach_defaults (GTK_TABLE (table), picker2, 1, 2, 2, 3);
		
		
		btnOK = gtk_button_new_with_mnemonic	 (pConvert=_toUtf8EX(OIX_CSTR_OK));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_widget_set_usize(btnOK,50,-1)  ; 
		gtk_widget_show (btnOK);
		btnCancel = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_widget_show (btnCancel);
		gtk_widget_set_usize(btnCancel,50,-1)  ; 
		gtk_table_attach_defaults (GTK_TABLE (table), btnOK, 0, 1, 3, 4);
		gtk_table_attach_defaults (GTK_TABLE (table), btnCancel, 1, 2, 3, 4);
		
		GLADE_HOOKUP_OBJECT (window, picker, "picker");
		GLADE_HOOKUP_OBJECT (window, picker1, "picker1");
		//		GLADE_HOOKUP_OBJECT (window, picker2, "picker2");
		
		
		g_signal_connect ((gpointer) btnOK, "pressed",
			G_CALLBACK (on_ok_btn),
			(gpointer) window);
		g_signal_connect ((gpointer) btnCancel, "pressed",
			G_CALLBACK (on_cancle_btn),
			(gpointer) window);
	}
	
	if (!GTK_WIDGET_VISIBLE (window))
    {
		gtk_widget_show_all (window);
    }
	else
    {    
		gtk_widget_destroy (window);
		window = NULL;
    }
	
	return window;
}