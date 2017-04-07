/********************************************
Windows Have  SYN to  UNIX  by hcl 07.12.04 ;
********************************************/
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"
/*#include "callbacks.h"
#include "interface.h"
#include "support.h"*/

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)
extern  int last_crt ;
extern  DEMO_DSP_CLASS demo_dsp_class[MAX_DEMO_CLASS_NUM];
static cur_selected_class[MAX_CRT_NUMBER]={0,0,0};
static tmp_selected_class[MAX_CRT_NUMBER]={0,0,0};
char cur_demo_dsp_name[MAX_DEMO_DSP_NUM][80];
extern int getCrtNumFromPointer(int data);
extern int getDataFromPointer(int data);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern int getMenuDataPointer(int crtn,int data);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
/*char TurnClass_name[30];*/
GtkWidget *AddTurnDspdialog[MAX_CRT_NUMBER];
GtkWidget *AddClassEntry[MAX_CRT_NUMBER]; 
GtkWidget *ClassNameOptionMenu[MAX_CRT_NUMBER];

GtkWidget * DelCurClass(GtkWidget*w);

void PopAddTurnDspWin(int crt_n);
void DestroyTurnDspConfigDlg(GtkWidget *w);
void DestroyTurnDspConfigDlgAndUpdateFile(GtkWidget *w);

void
on_dspname_input1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);
void updateCurDemoDsp(GtkWidget *w);

void UpdateTurnDspName(GtkWidget* widget, gpointer data)
{
    int i,j,crt_n;
    GtkWidget *dsp_name_text;
    char  text[MAX_DEMO_DSP_NUM*80];
    
    //GetCrtNumberOfWgt(widget, &crt_n);
	//hcl
	crt_n=getCrtNumFromPointer((int)data);
	data=(gpointer)getDataFromPointer((int)data);
    if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;	
    
    i= (int)data;
    cur_selected_class[crt_n] = i;
    printf("you select %d menu\n",i);
    printf("AddTurnDspdialog[crt_n]=%d\n",AddTurnDspdialog[crt_n]);
    dsp_name_text = g_object_get_data(G_OBJECT(AddTurnDspdialog[crt_n]),"dspname_input1");
    printf("class[%d].demo_dsp_name_str[0]=%s\n",i,demo_dsp_class[i].demo_dsp_name_str[0]);
    strcpy(text,demo_dsp_class[i].demo_dsp_name_str[0]);
    for(j=1;j<demo_dsp_class[i].demo_dsp_cnt;j++)
        {
            strcat(text,"\n");
            strcat(text,demo_dsp_class[i].demo_dsp_name_str[j]);
        printf("dsp_name=%s,text=%s\n",demo_dsp_class[i].demo_dsp_name_str[j],text);
    }
     gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (dsp_name_text)),
	text, -1);
}

void UpdateTurnClassMenu(GtkWidget* widget, gpointer data) 
{
        int crt_n;	
	int		i;
	int MenuPointer ; 
 	GtkWidget /**optionmenu,*/ *menu,*items;
	gchar *pConvert=0;

        GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;	
        
	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(ClassNameOptionMenu[crt_n]));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(ClassNameOptionMenu[crt_n]));

	menu =gtk_menu_new();
	/*gtk_widget_set_usize(menu, 130, -1);	*/	
	
 	for(i=0; i<demo_class_cnt; i++) 
  	{ 		
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(demo_dsp_class[i].class_name));	
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		MenuPointer=getMenuDataPointer(crt_n,i);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateTurnDspName), (gpointer)MenuPointer);	
	}

        gtk_option_menu_set_menu(GTK_OPTION_MENU(ClassNameOptionMenu[crt_n]),menu);		
	gtk_option_menu_set_history(GTK_OPTION_MENU(ClassNameOptionMenu[crt_n]), (guint)data);
	gtk_widget_show_all(ClassNameOptionMenu[crt_n]);
UpdateTurnDspName(widget,data);
}

GtkWidget* CreateTurnClassMenu(int crt_n) 
{
	int	i;
 	GtkWidget *optionmenu, *menu,*items;
	int MenuPointer;//hcl
	gchar *pConvert=0;
  
	menu =gtk_menu_new();
	/*gtk_widget_set_usize(menu, 130, -1);	*/	
	
 	for(i=0; i<demo_class_cnt; i++) 
  	{ 		
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(demo_dsp_class[i].class_name));
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		MenuPointer=getMenuDataPointer(crt_n,i);	
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateTurnDspName), (gpointer)MenuPointer);
		
	}

	optionmenu = gtk_option_menu_new();
        gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
        ClassNameOptionMenu[crt_n] = optionmenu;
        gtk_widget_show_all (optionmenu);
	return optionmenu;
}

gint  SetDemoTimePeriod(GtkWidget *w, gpointer data)
{	
        int crt_n,time_period;
	char		time_period_str[128];	
        GtkWidget *timeperiod_entry;
        
        crt_n = (int)data;
        
	time_period = demo_period;
        
        timeperiod_entry = g_object_get_data(G_OBJECT(AddTurnDspdialog[crt_n]),"timeperiod_entry");	
       
        if(strcmp(gtk_entry_get_text(GTK_ENTRY(timeperiod_entry)),"")!=0)
        {                    
                strcpy(time_period_str, gtk_entry_get_text(GTK_ENTRY(timeperiod_entry)));
		time_period = atoi(time_period_str);  
                printf("in SetDemoTimePeriod: time_period=%d\n",time_period);
        } 
        demo_period = time_period;
        return time_period;
}


GtkWidget* create_turndsp_config_dlg (GtkWidget *parent,int crt_n)
{
  GtkWidget *turndsp_config_dlg;
  GtkWidget *dialog_vbox1;
  GtkWidget *table1;
  GtkWidget *class_label1;
//  GtkWidget *class_comb;
  GtkWidget *timeperiod_lab;
  GtkWidget *timeperiod_entry;
  GtkWidget *del_class_btn;
  GtkWidget *add_dsp_btn;
//  GtkWidget *del_dsp_btn;
//  GtkWidget *up_btn;
//  GtkWidget *down_btn;
  GtkWidget *add_class_btn;
  GtkWidget *dspname_input;
GtkWidget *dspname_input1;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;
  int i;
  char  tmp_str[10],text[MAX_DEMO_DSP_NUM*80];
  gchar *pConvert=0;
//  GtkTextBuffer* text_buffer;

  turndsp_config_dlg = gtk_dialog_new ();
  /*gtk_widget_set_size_request (turndsp_config_dlg, 10, 10);*/
  gtk_container_set_border_width (GTK_CONTAINER (turndsp_config_dlg), 5);
gtk_window_set_title (GTK_WINDOW (turndsp_config_dlg), pConvert=_toUtf8EX(OIX_SELECT_TURNDSP_CONFIG));
G_SAFE_FREE(pConvert);
 /* GTK_WINDOW (turndsp_config_dlg)->type = GTK_WINDOW_POPUP;*/
  gtk_window_set_position (GTK_WINDOW (turndsp_config_dlg), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_default_size (GTK_WINDOW (turndsp_config_dlg), 350, 250);
  gtk_window_set_type_hint (GTK_WINDOW (turndsp_config_dlg), GDK_WINDOW_TYPE_HINT_DIALOG);
  gtk_window_set_transient_for(GTK_WINDOW(turndsp_config_dlg ), GTK_WINDOW(parent));
  
    gtk_widget_show (turndsp_config_dlg);

  dialog_vbox1 = GTK_DIALOG (turndsp_config_dlg)->vbox;
  gtk_widget_show (dialog_vbox1);

  table1 = gtk_table_new (9, 3, FALSE);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), table1, TRUE, TRUE, 0);

class_label1 = gtk_label_new (pConvert=_toUtf8EX(OIX_SELECT_TURNDSP_GROUP   ));
G_SAFE_FREE(pConvert);
  gtk_widget_show (class_label1);
  gtk_table_attach (GTK_TABLE (table1), class_label1, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (class_label1), 0, 0.5);

  /*class_comb = gtk_combo_box_entry_new_text ();
  gtk_widget_show (class_comb);*/
  
  gtk_widget_show (CreateTurnClassMenu(crt_n));
  gtk_table_attach (GTK_TABLE (table1), CreateTurnClassMenu(crt_n), 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
 

  timeperiod_lab = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_TIME_INTERVAL));
  G_SAFE_FREE(pConvert);
  gtk_widget_show (timeperiod_lab);
  gtk_table_attach (GTK_TABLE (table1), timeperiod_lab, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_misc_set_alignment (GTK_MISC (timeperiod_lab), 0, 0.5);

  timeperiod_entry = gtk_entry_new ();
  gtk_widget_show (timeperiod_entry);
  gtk_table_attach (GTK_TABLE (table1), timeperiod_entry, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 10, 5);
  gtk_widget_set_size_request (timeperiod_entry, 30, -1);  
  sprintf(tmp_str,"%d",demo_period);
  gtk_entry_set_text(GTK_ENTRY(timeperiod_entry),tmp_str);
  
  gtk_signal_connect(GTK_OBJECT(timeperiod_entry), "activate",
		       GTK_SIGNAL_FUNC(SetDemoTimePeriod),(gpointer)crt_n);

del_class_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_DEL_GROUP));
G_SAFE_FREE(pConvert);
  gtk_widget_show (del_class_btn);
  gtk_table_attach (GTK_TABLE (table1), del_class_btn, 2, 3, 4, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 10);
  gtk_signal_connect_object (GTK_OBJECT (del_class_btn), "clicked",
			       GTK_SIGNAL_FUNC(DelCurClass),
			       GTK_OBJECT (turndsp_config_dlg));
  
add_dsp_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_SAVE_CONFIG));
G_SAFE_FREE(pConvert);
  gtk_widget_show (add_dsp_btn);
  gtk_table_attach (GTK_TABLE (table1), add_dsp_btn, 2, 3, 6, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 15);
  gtk_signal_connect_object (GTK_OBJECT (add_dsp_btn), "clicked",
			       GTK_SIGNAL_FUNC(updateCurDemoDsp),
			       GTK_OBJECT (turndsp_config_dlg));

 add_class_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_ADD_GROUP));
 G_SAFE_FREE(pConvert);
  gtk_widget_show (add_class_btn);
  gtk_table_attach (GTK_TABLE (table1), add_class_btn, 2, 3, 2, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 10);
  
  gtk_signal_connect_object (GTK_OBJECT (add_class_btn), "clicked",
			       GTK_SIGNAL_FUNC(PopAddTurnDspWin),
			       (gpointer)crt_n);

  dspname_input = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (dspname_input);
  gtk_table_attach (GTK_TABLE (table1), dspname_input, 1, 2, 1, 18,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 10);
  
dspname_input1 = gtk_text_view_new ();
  gtk_widget_show (dspname_input1);
  gtk_container_add (GTK_CONTAINER (dspname_input), dspname_input1);  
  gtk_widget_set_size_request (dspname_input1, 10, 50);
  gtk_container_set_border_width (GTK_CONTAINER (dspname_input), 5);
  strcpy(text,demo_dsp_class[0].demo_dsp_name_str[0]);
  printf("0000::text=%s\n",text);
  for(i=1;i<demo_dsp_class[0].demo_dsp_cnt/*MAX_DEMO_DSP_NUM*/;i++)
  {
        strcat(text,"\n");
        printf("demo_dsp_class[0].demo_dsp_name_str[%d]=%s\n",i,demo_dsp_class[0].demo_dsp_name_str[i]);
        strcat(text,demo_dsp_class[0].demo_dsp_name_str[i]);
  }
  printf("text=%s\n",text);
  gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (dspname_input1)),
	text, -1);  
 
  
  dialog_action_area1 = GTK_DIALOG (turndsp_config_dlg)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
  G_SAFE_FREE(pConvert);
	  //gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (turndsp_config_dlg), cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_OK));
  G_SAFE_FREE(pConvert);
	  //gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (turndsp_config_dlg), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);
  gtk_signal_connect_object (GTK_OBJECT (okbutton1), "clicked",
			       GTK_SIGNAL_FUNC(DestroyTurnDspConfigDlgAndUpdateFile),
			       GTK_OBJECT (turndsp_config_dlg));
  gtk_signal_connect_object (GTK_OBJECT (cancelbutton1), "clicked",
			       GTK_SIGNAL_FUNC(DestroyTurnDspConfigDlg),
			       GTK_OBJECT (turndsp_config_dlg));

  gtk_dialog_set_alternative_button_order (GTK_DIALOG (turndsp_config_dlg), GTK_RESPONSE_OK, GTK_RESPONSE_CANCEL, -1);
  
  g_object_set_data(G_OBJECT(turndsp_config_dlg),"timeperiod_entry",timeperiod_entry);
  g_object_set_data(G_OBJECT(turndsp_config_dlg),"dspname_input1",dspname_input1);
  
  AddTurnDspdialog[crt_n] = turndsp_config_dlg;

  /* Store pointers to all widgets, for use by lookup_widget(). */
 
 /* g_object_set_data(GTK_OBJECT(AddTurnDspdialog[crt_n]),"timeperiod_entry",timeperiod_entry);
  g_object_set_data(GTK_OBJECT(AddTurnDspdialog[crt_n]),"dspname_input1",dspname_input1);*/
 
  printf("in create_turndsp_config_dlg\n");
  printf("create::AddTurnDspdialog[crt_n]=%d\n",AddTurnDspdialog[crt_n]);
  
  return turndsp_config_dlg;
}

void GetDemoDspName(char	buffer[MAX_DEMO_DSP_NUM*80] ) {
    char *token;
    int i;
    for(i=0;i<MAX_DEMO_DSP_NUM;i++)
        strcpy(cur_demo_dsp_name[i],"");
    i=0;
    printf("tokenizing \"%s\" with strtok():\n", buffer);
    if ((token = strtok(buffer, "\n")) != NULL) {
        strcpy(cur_demo_dsp_name[i], token);
        i++;
        while ((token = strtok(NULL, "\n")) != NULL) {
            /* printf("token = \"%s\"\n", token);*/  
           
                strcpy(cur_demo_dsp_name[i], token);
     printf("GetDemoDspName::   cur_demo_dsp_name[%d]=%s\n",i,cur_demo_dsp_name[i]);    
            i++;
        }
    }
}

void updateCurDemoDsp(GtkWidget *w)
{
    GtkWidget *dsp_name_text;
    GtkTextIter start_iter,end_iter;
    GtkTextBuffer *text_buffer;
    char text[MAX_DEMO_DSP_NUM*80]/*,dsp_name[80]*/;
    int i,j,crt_n;
    
    GetCrtNumberOfWgt ( w, &crt_n );
    if( crt_n < 0 )
		return ;
printf("in updateCurDemoDsp,AddTurnDspdialog[crt_n]=%d\n",AddTurnDspdialog[crt_n]);
    dsp_name_text = g_object_get_data(G_OBJECT(AddTurnDspdialog[crt_n]),"dspname_input1");
    text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (dsp_name_text));
    gtk_text_buffer_get_start_iter(text_buffer,&start_iter);
    gtk_text_buffer_get_end_iter(text_buffer,&end_iter);
   
    if(gtk_text_buffer_get_text (text_buffer,&start_iter,&end_iter,FALSE)!=NULL)
    {
        strcpy(text,gtk_text_buffer_get_text (text_buffer,&start_iter,&end_iter,TRUE)); 
       printf("in updateCurDemoDsp::text=%s\n",text);
        GetDemoDspName(text);
        
        j=0;
        for(i=0;i<MAX_DEMO_DSP_NUM;i++)
        {
            printf("***in saveDsp  updateCurDemoDsp::cur_demo_dsp_name[%d]=%s\n",i,cur_demo_dsp_name[i]);
            if(strcmp(cur_demo_dsp_name[i],"")!=0)
            {
                strcpy(demo_dsp_class[cur_selected_class[crt_n]].demo_dsp_name_str[i],cur_demo_dsp_name[i]);
                j++;
            }
        }
        demo_dsp_class[cur_selected_class[crt_n]].demo_dsp_cnt =j;        
    }  
    
}

void updateDemoDefFile(GtkWidget *w,char	*demo_file_name)
{
    FILE	*fp;
    int i,j,crt_n;   
	gchar *pConvert=0;
    
    GetCrtNumberOfWgt ( w, &crt_n );
    if( crt_n < 0 )
		return ;
    printf("crt_n=%d\n",crt_n);
    
    
    SetDemoTimePeriod(w, (gpointer)crt_n);
	if(demo_period<=0)
	{
		PopupMessageBox(main_w[crt_n], pConvert=_toUtf8EX(MSG_DDMO_PERIOD_INVALID));
		G_SAFE_FREE(pConvert);
		return;
	}    
    fp = fopen( demo_file_name, "w") ;	
     /* printf("demo_period = %d\n",demo_period); */ 
    fprintf (fp,"%d\n",demo_period);       
       
    for(i=0;i<demo_class_cnt;i++)
    {                
                fprintf (fp,"%s\n",demo_dsp_class[i].class_name);
                for(j=0;j<demo_dsp_class[i].demo_dsp_cnt;j++)
                {
                        if((j==demo_dsp_class[i].demo_dsp_cnt-1) && (i==demo_class_cnt-1))
                            fprintf(fp,"%s",demo_dsp_class[i].demo_dsp_name_str[j]);
                        else
                        fprintf(fp,"%s\n",demo_dsp_class[i].demo_dsp_name_str[j]);
                }
                
    }
    fclose (fp);
    
}

void DestroyTurnDspConfigDlgAndUpdateFile(GtkWidget *w)
{
    updateDemoDefFile(w,DEMO_DSP_NAME);
    gtk_widget_destroy(w);	
}

void DestroyTurnDspConfigDlg(GtkWidget *w)
{    
    gtk_widget_destroy(w);	
}


void DestroyAddTurnDspDlg(GtkWidget *w)
{	
        int   recent_class_cnt;
	int	crt_n=0, i, repeat_flag=0;
	char	str[256];
	
	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return ;
	
	
        if(gtk_entry_get_text(GTK_ENTRY(AddClassEntry[crt_n])))
        {
            strcpy(str,gtk_entry_get_text(GTK_ENTRY(AddClassEntry[crt_n])));
            if(str[0]) 
            {
            	for(i=0; i<demo_class_cnt; i++)
            	{
            		if(!strcmp(str, demo_dsp_class[i].class_name))
            		{
	    			repeat_flag = 1;
	    			break;
	    		}
	    	}
	    	if(repeat_flag)
 	    		PopupMessageBox(AddTurnDspdialog[crt_n], MSG_TURNDSP_NAME_EXIST);
           	else
           	{
            		strcpy(cur_class[crt_n].added_name,str);
            		demo_class_cnt = demo_class_cnt +1;
            		strcpy(demo_dsp_class[demo_class_cnt-1].class_name,cur_class[crt_n].added_name);
        		recent_class_cnt = demo_class_cnt;
        		UpdateTurnClassMenu(w,(gpointer)(recent_class_cnt-1) );
        	}
	    }
	    else
	    	PopupMessageBox(AddTurnDspdialog[crt_n], MSG_TURNDSP_NAME_NULL);
        }        
	gtk_widget_destroy(w);
}
GtkWidget * create_TurnDsp_window(GtkWidget *w,int crt_n)
{
	GtkWidget /**label,*/*button,*dialog[MAX_CRT_NUMBER];
	gchar *pConvert=0;
	
	dialog[crt_n] = gtk_dialog_new();

	gtk_window_set_position (GTK_WINDOW(dialog[crt_n]),GTK_WIN_POS_CENTER_ALWAYS);				        
	gtk_widget_set_usize(dialog[crt_n],214,117);
	gtk_window_set_policy(GTK_WINDOW(dialog[crt_n]),FALSE,FALSE,FALSE);
	gtk_window_set_title(GTK_WINDOW(dialog[crt_n]), pConvert=_toUtf8EX(OIX_ADD_GROUP));
	G_SAFE_FREE(pConvert);
gtk_window_set_transient_for(GTK_WINDOW(dialog[crt_n]), GTK_WINDOW(main_w[crt_n]));
        
	AddClassEntry[crt_n] = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog[crt_n])->vbox),AddClassEntry[crt_n],TRUE,TRUE,0);	
		       
	button = gtk_button_new_with_label(pConvert=_toUtf8EX("OIX_CSTR_OK"));
	G_SAFE_FREE(pConvert);
	GTK_WIDGET_SET_FLAGS(button,GTK_CAN_DEFAULT);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog[crt_n])->action_area),button,FALSE,FALSE,0);
	gtk_widget_grab_default(button);
	
	gtk_signal_connect_object(GTK_OBJECT(AddClassEntry[crt_n]), "activate",
		       GTK_SIGNAL_FUNC(DestroyAddTurnDspDlg),GTK_OBJECT (dialog[crt_n]));
		       
	gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			       GTK_SIGNAL_FUNC(DestroyAddTurnDspDlg),
			       GTK_OBJECT (dialog[crt_n]));
	gtk_widget_show_all(dialog[crt_n]);	
        return dialog[crt_n];
}
GtkWidget * DelCurClass(GtkWidget*w)
{
    int i/*,j*/;
    int	crt_n=0;
    int MenuPointer ; 
	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return NULL;
    printf("crt_n=%d\n",crt_n);
    printf("cur_selected_class[crt_n]=%d\n",cur_selected_class[crt_n]);
    for(i=cur_selected_class[crt_n];i<demo_class_cnt-1;i++)
    {
        demo_dsp_class[i] = demo_dsp_class[i+1];     
        printf("class_name[%d]=%s\n",i,demo_dsp_class[i].class_name);
    }
	memset(&demo_dsp_class[demo_class_cnt-1],0,sizeof(DEMO_DSP_CLASS));
    demo_class_cnt = demo_class_cnt-1;
    UpdateTurnClassMenu(w,0 );
	MenuPointer=getMenuDataPointer(crt_n,0);	
    UpdateTurnDspName(w, (gpointer)MenuPointer);
	return w;
}


void PopAddTurnDspWin(int crt_n)
{	
	 create_TurnDsp_window (main_w[crt_n], crt_n);      // updata by hcl
}

void Destroyselected_classDlg(GtkWidget *w)
{    
    gtk_widget_destroy(w);	
}

void DestroyDlgAndGetClass(GtkWidget *w,int crt_n)
{    
    //int crt_n;
    GetCrtNumberOfWgt ( w, &crt_n );
    if( crt_n < 0 )
		return ;
    gtk_widget_destroy(w);
    selected_class[crt_n]=tmp_selected_class[crt_n];
   printf("in DestroyDlgAndGetClass::selected_class[crt_n]=%d\n",selected_class[crt_n]);
}

void GetSelectedClass(GtkWidget *w,gint user_data)
{
    int crt_n;
    /*GetCrtNumberOfWgt ( w, &crt_n );*/
    crt_n=getCrtNumFromPointer((int)user_data);
	user_data=getDataFromPointer((int)user_data);
    if( crt_n < 0 )
		return ;
    tmp_selected_class[crt_n]= user_data;
    printf("in GetSelectedClass:::tmp_selected_class[crt_n]=%d\n",tmp_selected_class[crt_n]);
}

GtkWidget*
create_selected_class_window1 (GtkWidget *parent,int crt_n)
{
       
  GtkWidget *window1;
  GtkWidget *vbox1;
  GtkWidget *radiobutton[MAX_DEMO_CLASS_NUM];
  GSList *radiobutton1_group = NULL;
//  GtkWidget *radiobutton2;
  GtkWidget *hbox1;
  GtkWidget *button1;
  GtkWidget *button2;
  int i;
  int MenuPointer;/*hcl*/
  gint height;
  gchar *pConvert=0;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
//  gtk_window_set_transient_for(GTK_WINDOW(window1),GTK_WINDOW(parent));  
  gtk_window_set_position(GTK_WINDOW(window1),GTK_WIN_POS_CENTER_ALWAYS) ;            
  gtk_window_set_title (GTK_WINDOW (window1), pConvert=_toUtf8EX(OIX_SELECT_TURNDSP_GROUP));
  G_SAFE_FREE(pConvert);
  height= 21*(demo_class_cnt+2);
  gtk_widget_set_usize(window1,200,height);
	gtk_window_set_transient_for(GTK_WINDOW(window1), GTK_WINDOW(main_w[crt_n]));
    
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  if(demo_class_cnt<=0)
  {
	  PopupMessageBox(main_w[crt_n],MSG_PLEASE_CONFIG_DEF_FILE);
	  return NULL;
  }
  for(i=0;i<demo_class_cnt;i++)
  {
        radiobutton[i] = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8(demo_dsp_class[i].class_name));
		G_SAFE_FREE(pConvert);
        gtk_widget_show (radiobutton[i]);
        gtk_box_pack_start (GTK_BOX (vbox1), radiobutton[i], FALSE, FALSE, 0);
        gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton[i]), radiobutton1_group);
        radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton[i]));         
		MenuPointer=getMenuDataPointer(crt_n,i);	
		
		gtk_signal_connect (GTK_OBJECT (radiobutton[i]), "toggled",
                      GTK_SIGNAL_FUNC (GetSelectedClass),
                      (gpointer)MenuPointer);  
  } 
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton[selected_class[crt_n]]),TRUE);
  
  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);

  button1 = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX("OIX_CSTR_OK"));
  G_SAFE_FREE(pConvert);
  gtk_widget_show (button1);
  gtk_box_pack_start (GTK_BOX (hbox1), button1, FALSE, FALSE, 0);

  button2 = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX("OIX_CSTR_CANCEL"));
  G_SAFE_FREE(pConvert);
  gtk_widget_show (button2);
  gtk_box_pack_start (GTK_BOX (hbox1), button2, FALSE, FALSE, 0);
  gtk_widget_show_all(window1);
  
  gtk_signal_connect_object (GTK_OBJECT (button1), "clicked",
	  GTK_SIGNAL_FUNC(DestroyDlgAndGetClass),
	  GTK_OBJECT (window1));
  gtk_signal_connect_object (GTK_OBJECT (button2), "clicked",
	  GTK_SIGNAL_FUNC(Destroyselected_classDlg),
	  GTK_OBJECT (window1));
  
  return window1;
}


void
on_dspname_input1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}

GtkWidget *demo_screen_config_win[MAX_CRT_NUMBER];
extern int disable_demo_flag[MAX_CRT_NUMBER];
extern gint num_screen;
GtkWidget *checkbtn_screen[MAX_CRT_NUMBER];
int iSetScrNum  = 0  ; 

void
on_demo_ok_btn_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
    int crt_n =(int)user_data;
    int i;
    
    for(i=0; i<num_screen; i++)
    	disable_demo_flag[i]= !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbtn_screen[i]));    
    last_crt  = iSetScrNum  ; 
    gtk_widget_destroy(demo_screen_config_win[crt_n]);
    demo_screen_config_win[crt_n]= NULL;

}

void
on_demo_Cancel_btn_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
    int crt_n =(int)user_data;
    
    
    gtk_widget_destroy(demo_screen_config_win[crt_n]);
    demo_screen_config_win[crt_n]= NULL;
}


void SetDspScrNum (GtkWidget * widget, gpointer data)
{    
	int crt_n ;
	crt_n=getCrtNumFromPointer((int)data);
	iSetScrNum =getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;

}	

GtkWidget* create_selected_demo_screen_window (GtkWidget *w,int crt_n)
{
  
	GtkWidget *table1,*lbl_scrSet,*optionmenu,*menu,*items;
	int  MenuPointer ; 
	GtkWidget *ok_btn;
	GtkWidget *Cancel_btn;
	 char	str[64];
	UCHAR i,j;
	gchar *pConvert=0;
 
	if(!CheckUserPrivilege(cur_usr_name, PRIV_SYS_CONF_MGR))
	{
		PopupMessageBox ( main_w[crt_n], MSG_SYS_CONFIG_MGR );
		return NULL;
	}
    if(demo_screen_config_win[crt_n]!=NULL )
	{
        gtk_window_present(GTK_WINDOW(demo_screen_config_win[crt_n]));
        return NULL;
    }
	demo_screen_config_win[crt_n] = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (demo_screen_config_win[crt_n]), pConvert=_toUtf8EX(OIX_SELECT_TURNSCR_CONFIG));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(demo_screen_config_win[crt_n]), GTK_WINDOW(main_w[crt_n]));
	gtk_window_set_position(GTK_WINDOW(demo_screen_config_win[crt_n]),GTK_WIN_POS_CENTER);
	gtk_widget_show (demo_screen_config_win[crt_n]);
	gtk_widget_set_usize(demo_screen_config_win[crt_n],100,100);
	table1 = gtk_table_new (num_screen+2, 2, FALSE);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (demo_screen_config_win[crt_n]), table1);

        for(j=0;j<   num_screen ;j++)
        {
           
            sprintf(str, "SCREEN %d", j+1);
            checkbtn_screen[j] = gtk_check_button_new_with_mnemonic (pConvert=_toUtf8(str));
			G_SAFE_FREE(pConvert);
            gtk_widget_show (checkbtn_screen[j]);
            gtk_table_attach (GTK_TABLE (table1), checkbtn_screen[j], 1, 2, j, j+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
            
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbtn_screen[j]), !disable_demo_flag[j] );
            
        }

    lbl_scrSet  = gtk_label_new(pConvert=_toUtf8EX("ÍÆÆÁÄ»ºÅ£º"));
	G_SAFE_FREE(pConvert);
    gtk_widget_show (lbl_scrSet); 
	gtk_table_attach (GTK_TABLE (table1), lbl_scrSet, 0, 1, num_screen, num_screen+1,
		(GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (0), 0, 0);

	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_table_attach (GTK_TABLE (table1), optionmenu, 1, 2, num_screen, num_screen+1,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);


	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	/*  OIX_CSTR_REALTIME_REC ,OIX_CSTR_HIST_REC*/
	for (i = 0 ; i <  num_screen   ; i++)
	{
		sprintf(str, "SCREEN %d", i+1);
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(str));	 
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		MenuPointer=getMenuDataPointer(crt_n,i); 		 	 
		gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (SetDspScrNum), GINT_TO_POINTER(MenuPointer));
		
	}   
	gtk_menu_set_active ( GTK_MENU(menu),last_crt  );  
//	gtk_box_pack_start (GTK_BOX (box), menu, FALSE, FALSE, 0);
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	





	ok_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX("OIX_CSTR_OK"));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (ok_btn);
	gtk_table_attach (GTK_TABLE (table1), ok_btn, 0, 1, num_screen+1, num_screen+2,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (0), 0, 0);

	Cancel_btn = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX("OIX_CSTR_CANCEL"));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (Cancel_btn);
	gtk_table_attach (GTK_TABLE (table1), Cancel_btn, 2, 3, num_screen+1, num_screen+2,
					  (GtkAttachOptions) (GTK_FILL),
					  (GtkAttachOptions) (0), 0, 0);

	g_signal_connect ((gpointer) ok_btn, "clicked",
					  G_CALLBACK (on_demo_ok_btn_clicked),
					  (gpointer)crt_n);

	g_signal_connect ((gpointer) Cancel_btn, "clicked",
					  G_CALLBACK (on_demo_Cancel_btn_clicked),
					  (gpointer)crt_n); 

	return demo_screen_config_win[crt_n];
}

