#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "../inc/gtk_widgets.h" 
#include "../inc/gtk_msg_text_def.h"
#include "../inc/gtk_oix_cstr.h"
#include "../../../dms/v4.0a/inc/dms_com.h"
#include	"../inc/oix.h"

int tagtype;
GtkWidget *TagWindow;
DMS_COMMON tag_dms_cmm;
extern char *_toUtf8EX(char *c);
extern void 	CmdBufInit ( int crt_n );
int ProgDoTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm);
int ProgOnTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm);
int ProgOffTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm);
int Tag_val_set(int crt_n,int tag_type);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
	g_object_set_data_full (G_OBJECT (component), name, \
gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
g_object_set_data (G_OBJECT (component), name, widget)

void
on_list1_select_child                  (GtkList         *list,
                                        GtkWidget       *widget,
                                        gpointer         user_data)
{
	
	GList           *dlist;
	GtkWidget *list_item;
	dlist=GTK_LIST(list)->selection;
	
	list_item=dlist->data;
	
	tagtype=gtk_object_get_data(GTK_OBJECT(list_item), "list_item_data_key");
	
	
}

void
on_btnOnTag_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	int crt_n=0;
	GetCrtNumberOfWgt ( GTK_WIDGET(button), &crt_n );
    if( crt_n < 0 )
		return ;    
	printf("ontag\n");

	Tag_val_set(crt_n,tagtype);

	tagtype=-1;
	gtk_widget_destroy(TagWindow);
}

void
on_btnOffTag_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	int crt_n=0;
	GetCrtNumberOfWgt ( GTK_WIDGET(button), &crt_n );
    if( crt_n < 0 )
		return ;    
	printf("offtag\n");
	
	tagtype=0;
	Tag_val_set(crt_n,tagtype);

	gtk_widget_destroy(TagWindow);
}


int Tag_val_set(int crt_n,int tag_type)
{
//	int new_val;
	
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id		= tag_dms_cmm.point.stn_id;
	cmd_buf[crt_n].dev_id		= tag_dms_cmm.point.dev_id;
	cmd_buf[crt_n].point_id		= tag_dms_cmm.point.pt_id;
	cmd_buf[crt_n].data_type 	= tag_dms_cmm.point.data_type;  
	cmd_buf[crt_n].ctrl_addr	= 0;

	
	cmd_buf[crt_n].type_id		=DPS_IOST_SET;
	cmd_buf[crt_n].data.iValue 	= 0;
	cmd_buf[crt_n].status.state	= tag_type;
		
	
	cmd_buf[crt_n].status.opr	= 7;	
	
	ProgDoAckCelBrdcst ( crt_n );
	
	
	return TRUE;
}


GtkWidget*
create_window1 (int crt_n,POINTER pt)
{
	GtkWidget *window1;
	GtkWidget *vbox1;
	GtkWidget *frame2;
	GtkWidget *alignment2;
	GtkWidget *list1;
	GtkWidget *label2;
	GtkWidget *hseparator2;
	GtkWidget *hbox1;
	GtkWidget *btnOnTag;
	GtkWidget *btnOffTag;
	GtkWidget *list_item;
	GList            *dlist = NULL;
   	int i;
   	gchar buf[50];
//	GtkWidget *gtklist;

	DMS_COMMON dms_cmm;
//	GdkColor color;

	gchar *pConvert=0;
	
	if ( ReadEntryById ( &pt, &dms_cmm ) == -1 )
	{		
		PopupMessageBox(main_w[crt_n],OIX_STR_DB_NO_POINT);
		return 0;
	}
	
	window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_title (GTK_WINDOW (window1), pConvert=_toUtf8EX(OIX_STR_DO_TAG));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(window1), GTK_WINDOW(main_w[crt_n]));
	
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (window1), vbox1);
	
	frame2 = gtk_frame_new (NULL);
	gtk_widget_show (frame2);
	gtk_box_pack_start (GTK_BOX (vbox1), frame2, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_NONE);
	
	alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment2);
	gtk_container_add (GTK_CONTAINER (frame2), alignment2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);
	
	list1 = gtk_list_new ();
	gtk_widget_show (list1);
	for(i = 0; i < 3; ++i)
	{
		sprintf(buf, "TAG%d",i);
		
		list_item = gtk_list_item_new_with_label(buf);
		gtk_object_set_data(GTK_OBJECT(list_item),
			"list_item_data_key",
			GINT_TO_POINTER(i+1));
		
		dlist = g_list_append(dlist, list_item);
	}

	gtk_list_append_items(GTK_LIST(list1), dlist);
	gtk_container_add (GTK_CONTAINER (alignment2), list1);
	
	label2 = gtk_label_new ( ("<b>\346\214\202\347\211\214\346\240\267\345\274\217</b>"));
	gtk_widget_show (label2);
	gtk_frame_set_label_widget (GTK_FRAME (frame2), label2);
	gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);
	
	hseparator2 = gtk_hseparator_new ();
	gtk_widget_show (hseparator2);
	gtk_box_pack_start (GTK_BOX (vbox1), hseparator2, TRUE, TRUE, 0);
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);
	
	btnOnTag = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_STR_TAG_ON));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (btnOnTag);
	gtk_box_pack_start (GTK_BOX (hbox1), btnOnTag, FALSE, FALSE, 0);
	
	btnOffTag = gtk_button_new_with_mnemonic (pConvert=_toUtf8EX(OIX_STR_TAG_OFF));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (btnOffTag);
	gtk_box_pack_start (GTK_BOX (hbox1), btnOffTag, FALSE, FALSE, 0);
	

	g_signal_connect ((gpointer) list1, "select_child",
		G_CALLBACK (on_list1_select_child),
		NULL);
	g_signal_connect ((gpointer) btnOnTag, "clicked",
		G_CALLBACK (on_btnOnTag_clicked),
		NULL);
	g_signal_connect ((gpointer) btnOffTag, "clicked",
		G_CALLBACK (on_btnOffTag_clicked),
		NULL);

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF (window1, window1, "window1");
	GLADE_HOOKUP_OBJECT (window1, vbox1, "vbox1");
	GLADE_HOOKUP_OBJECT (window1, frame2, "frame2");
	GLADE_HOOKUP_OBJECT (window1, alignment2, "alignment2");
	GLADE_HOOKUP_OBJECT (window1, list1, "list1");
	GLADE_HOOKUP_OBJECT (window1, label2, "label2");
	GLADE_HOOKUP_OBJECT (window1, hseparator2, "hseparator2");
	GLADE_HOOKUP_OBJECT (window1, hbox1, "hbox1");
	GLADE_HOOKUP_OBJECT (window1, btnOnTag, "btnOnTag");
	GLADE_HOOKUP_OBJECT (window1, btnOffTag, "btnOffTag");
	
		gtk_widget_show_all(window1);

	return window1;
}



int ProgDoTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm)
{
	LCU_ID	lcu_id;
	if(!CheckUserPrivilege(cur_usr_name, PRIV_TAG_OPR))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_IO_ATTR_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= dms_cmm.point.stn_id;
	lcu_id.dev_id= dms_cmm.point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}

	tag_dms_cmm=dms_cmm;
	TagWindow=create_window1(crt_n,tag_dms_cmm.point);

	return TRUE;
}

int ProgOnTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm)
{
	LCU_ID	lcu_id;
	if(!CheckUserPrivilege(cur_usr_name, PRIV_TAG_OPR))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_IO_ATTR_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= dms_cmm.point.stn_id;
	lcu_id.dev_id= dms_cmm.point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}
	
	tag_dms_cmm=dms_cmm;
	//TagWindow=create_window1(crt_n,tag_dms_cmm.point);

	Tag_val_set(crt_n,1);

	return TRUE;
}
int ProgOffTag(GtkWidget *parent,int crt_n,DMS_COMMON  dms_cmm)
{
	LCU_ID	lcu_id;
	if(!CheckUserPrivilege(cur_usr_name, PRIV_TAG_OPR))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_IO_ATTR_SETTING );
		return FALSE;
	}
	lcu_id.stn_id= dms_cmm.point.stn_id;
	lcu_id.dev_id= dms_cmm.point.dev_id;
	if(!CheckHostPrivilege(local_hostname, lcu_id))
	{
		PopupMessageBox ( db_detail_info[crt_n], MSG_HOST_NO_CTRL_PRIORITY );
		return FALSE;
	}
	
	tag_dms_cmm=dms_cmm;
	//TagWindow=create_window1(crt_n,tag_dms_cmm.point);
	Tag_val_set(crt_n,0);
	return TRUE;
}