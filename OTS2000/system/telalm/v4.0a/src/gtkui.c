/***************************************************************************************
PROGRAM NAME:	ONCALL

2010-12-12	Added by chenxs	
***************************************************************************************/
#include	"../../../telalm/v4.0a/inc/tele.h"

enum {Toggle_COL,PIXBUF_COL,TEXT_COL,TEXT_COL1,TEXT_COL2};
enum {Toggle_MALMCOL,TEXT_MALMCOL,TEXT_MALMCOL1};
/*gboolean maneselselection_changed(GtkTreeSelection *selection) 
{
     GtkTreeView *treeView;  
	GtkTreeModel *model;  
	GtkTreeIter iter;  
	gchar *active;   
	
    if(selection)
	{ 
		treeView = gtk_tree_selection_get_tree_view(selection); 
		model = gtk_tree_view_get_model(treeView);    
		gtk_tree_selection_get_selected(selection, &model, &iter);  
		if(iter.stamp)
		{ 
			//gtk_tree_model_get(model, &iter,1, &active,-1);    
		
		}
	} 
}
*/
GtkTreeView * selecttelalmdevtreeview;//2012 03 22

GtkTreeView * selectsmsalmdevtreeview;//2012 03 22

void selecttelalmdevtreeviewcb_selectck_toggled(GtkCellRendererToggle *cell, gchar *path_str, gpointer data)
{
	GtkTreeModel *treemodle;
	GtkTreePath *path;
	GtkTreeIter iter;
	gboolean fixed;
	//gchar path_str1[100];

	int i;
	
	if(path_str)
	{
		treemodle = (GtkTreeModel *)data;
		path= gtk_tree_path_new_from_string(path_str);
		
		gtk_tree_model_get_iter(treemodle, &iter, path);
		gtk_tree_model_get(treemodle, &iter, Toggle_COL, &fixed, -1);
		fixed ^= 1;
		
		i=atoi(path_str);
		
		if(i>=0 && i<oncall_alm_dev_def.real_oncall_alm_dev_def_num)
		{
			if(fixed)
				oncall_alm_dev_def.TEL_ALM[i]=1;
			else
				oncall_alm_dev_def.TEL_ALM[i]=0;	
		}

		gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_COL, fixed, -1);
		
		gtk_tree_path_free(path);
	}
	
}


void selectsmsalmdevtreeviewcb_selectck_toggled(GtkCellRendererToggle *cell, gchar *path_str, gpointer data)
{
	GtkTreeModel *treemodle;
	GtkTreePath *path;
	GtkTreeIter iter;
	gboolean fixed;
	//gchar path_str1[100];
	
	int i;
	
	if(path_str)
	{
		treemodle = (GtkTreeModel *)data;
		path= gtk_tree_path_new_from_string(path_str);
		
		gtk_tree_model_get_iter(treemodle, &iter, path);
		gtk_tree_model_get(treemodle, &iter, Toggle_COL, &fixed, -1);
		fixed ^= 1;
		
		i=atoi(path_str);
		
		if(i>=0 && i<oncall_alm_dev_def.real_oncall_alm_dev_def_num)
		{
			if(fixed)
				oncall_alm_dev_def.SMS_ALM[i]=1;
			else
				oncall_alm_dev_def.SMS_ALM[i]=0;	
		}
		
		gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_COL, fixed, -1);
		
		gtk_tree_path_free(path);
	}
	
}


void oncall_tree_view_toggle_cb(GtkCellRendererToggle *cell, gchar *path_str, gpointer data)
{
	GtkTreeModel *treemodle;
	GtkTreePath *path;
	GtkTreeIter iter;
	gboolean fixed;
	gchar path_str1[100];
	GtkTreePath  *path1;
	int i;
 
	treemodle = (GtkTreeModel *)data;
	path= gtk_tree_path_new_from_string(path_str);
	
	gtk_tree_model_get_iter(treemodle, &iter, path);
	gtk_tree_model_get(treemodle, &iter, Toggle_COL, &fixed, -1);
	fixed ^= 1;
	gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_COL, fixed, -1);
	
    if(!strstr(path_str,":"))
	{
		for(i=0;i<TELPHONE_NUM;i++)
		{
			memset(path_str1,0,sizeof(path_str1));
			sprintf(path_str1,"%s:%d",path_str,i);
			path1=gtk_tree_path_new_from_string(path_str1);
			if(gtk_tree_model_get_iter(treemodle, &iter, path1))
			{
				gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_COL, fixed, -1);
			}
			else i=-1;
			gtk_tree_path_free(path1);
			if(i<0)break;
		}
	}
	gtk_tree_path_free(path);
}

void createnamaselectModel(GtkTreeView * treeView) 
{
	GtkCellRenderer *renderer;   
	GtkTreeViewColumn *column;  
 	GtkWidget *cellView;
	GdkPixbuf *pixbuf;
	GtkTreeIter iter1, iter2;
	GtkTreeStore *store;
	gint i, j;
	char usedc[TELPHONE_NUM];

	memset(usedc,0,TELPHONE_NUM);
	store = gtk_tree_store_new(5,G_TYPE_BOOLEAN, GDK_TYPE_PIXBUF, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	cellView = gtk_cell_view_new();    
	for(i = 0; i <telphone_def.real_telphone_def_num; i++)
	{ 
		if(telphone_def.namecode[i][0]==0)break;
		if(usedc[i]==1)continue;
		pixbuf = gdk_pixbuf_new_from_file(nameselectgladepicpath, NULL);
		gtk_tree_store_append(store, &iter1, NULL);
		gtk_tree_store_set(store, &iter1,Toggle_COL,FALSE,PIXBUF_COL, pixbuf,
			TEXT_COL1, g_locale_to_utf8(telphone_def.department[i],sizeof(telphone_def.department[i]),NULL,NULL,NULL),-1); 
		gdk_pixbuf_unref(pixbuf);
		
		for(j = 0; j < telphone_def.real_telphone_def_num; j++)
		{
			if(strcmp(telphone_def.department[i],telphone_def.department[j])!=0) continue;

			usedc[j]=1;
			pixbuf = gdk_pixbuf_new_from_file(nameselectgladepicpath1, NULL);
			gtk_tree_store_append(store, &iter2, &iter1);
			gtk_tree_store_set(store, &iter2,Toggle_COL,FALSE,PIXBUF_COL, pixbuf,
				TEXT_COL,  g_locale_to_utf8(telphone_def.namecode[j],sizeof(telphone_def.namecode[j]),NULL,NULL,NULL),
				TEXT_COL1,  g_locale_to_utf8(telphone_def.name[j],sizeof(telphone_def.name[j]),NULL,NULL,NULL),
				TEXT_COL2,  g_locale_to_utf8(telphone_def.telphone[j],sizeof(telphone_def.telphone[j]),NULL,NULL,NULL),-1);

			gdk_pixbuf_unref(pixbuf);
		}  
	}   
	gtk_tree_view_set_model(treeView,(GtkTreeModel *)store);

	renderer = gtk_cell_renderer_toggle_new();
	column = gtk_tree_view_column_new_with_attributes("check", renderer,"active", Toggle_COL,NULL);   
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);
	//gtk_cell_renderer_toggle_set_active((GtkCellRendererToggle* )(renderer),0);
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(oncall_tree_view_toggle_cb), store);

	renderer = gtk_cell_renderer_pixbuf_new();   
	column = gtk_tree_view_column_new_with_attributes("icon", renderer,"pixbuf", PIXBUF_COL,NULL);   
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);   
	renderer = gtk_cell_renderer_text_new();    
	column = gtk_tree_view_column_new_with_attributes("namecode", renderer,"text", TEXT_COL,NULL); 
	column->visible=FALSE;
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);   
	renderer = gtk_cell_renderer_text_new();    
	column = gtk_tree_view_column_new_with_attributes("names", renderer,"text", TEXT_COL1,NULL);    
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);   
	renderer = gtk_cell_renderer_text_new();   
	column = gtk_tree_view_column_new_with_attributes("telnum", renderer,"text", TEXT_COL2,NULL);    
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);   
    
	gdk_pixbuf_unref((GdkPixbuf *)store);
}

void createselectmsgalmtModel(GtkTreeView * treeView) 
{
	GtkCellRenderer *renderer;   
	GtkTreeViewColumn *column;  
	GtkWidget *cellView;
	GtkTreeStore *store;

	store = gtk_tree_store_new(2,G_TYPE_BOOLEAN, G_TYPE_STRING);
	cellView = gtk_cell_view_new();    
	//gtk_tree_store_append(store, &iter, NULL);
	//gtk_tree_store_set(store, &iter,Toggle_MALMCOL,1,TEXT_MALMCOL,g_locale_to_utf8("  是否发送",10,NULL,NULL,NULL),-1); 
	gtk_tree_view_set_model(treeView,(GtkTreeModel *)store);
	
	renderer = gtk_cell_renderer_toggle_new();
	column = gtk_tree_view_column_new_with_attributes(g_locale_to_utf8("  是否发送",10,NULL,NULL,NULL), renderer,"active", Toggle_COL,NULL);   
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(oncall_tree_view_toggle_cb), store);

	renderer = gtk_cell_renderer_text_new();   
	column = gtk_tree_view_column_new_with_attributes(g_locale_to_utf8("  报警描述",10,NULL,NULL,NULL), renderer,"text", TEXT_MALMCOL,NULL);    
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);   
	gdk_pixbuf_unref((GdkPixbuf *)store);
}


void set_selecttelalmdevtModel() //20120322
{
	int i;
	GtkTreeIter iter;
	GtkTreeModel *treemodle;
	GtkTreeStore *store;
	treemodle=gtk_tree_view_get_model((GtkTreeView *)selecttelalmdevtreeview);
	store=(GtkTreeStore *)treemodle;
	gtk_tree_store_clear(store);
	for(i=0;i<oncall_alm_dev_def.real_oncall_alm_dev_def_num;i++)
	{	
		gtk_tree_store_append(store, &iter, NULL);
		gtk_tree_store_set(treemodle, &iter,Toggle_MALMCOL,oncall_alm_dev_def.TEL_ALM[i],TEXT_MALMCOL,
			g_locale_to_utf8(oncall_alm_dev_def.name[i],sizeof(oncall_alm_dev_def.name[i]),NULL,NULL,NULL),-1); 
	}
	gtk_onvall_mainform_created=1;
}
void set_selectsmsalmdevtModel() //20120322
{
	int i;
	GtkTreeIter iter;
	GtkTreeModel *treemodle;
	GtkTreeStore *store;
	treemodle=gtk_tree_view_get_model((GtkTreeView *)selectsmsalmdevtreeview);
	store=(GtkTreeStore *)treemodle;
	gtk_tree_store_clear(store);
	for(i=0;i<oncall_alm_dev_def.real_oncall_alm_dev_def_num;i++)
	{	
		gtk_tree_store_append(store, &iter, NULL);
		gtk_tree_store_set(treemodle, &iter,Toggle_MALMCOL,oncall_alm_dev_def.SMS_ALM[i],TEXT_MALMCOL,
			g_locale_to_utf8(oncall_alm_dev_def.name[i],sizeof(oncall_alm_dev_def.name[i]),NULL,NULL,NULL),-1); 
	}
	gtk_onvall_mainform_created=1;
}


void createselecttelalmdevtModel(GtkTreeView * treeView) 
{
	GtkCellRenderer *renderer;   
	GtkTreeViewColumn *column;  
	GtkWidget *cellView;
	GtkTreeStore *store;


	store = gtk_tree_store_new(2,G_TYPE_BOOLEAN, G_TYPE_STRING);
	cellView = gtk_cell_view_new();    
	
	gtk_tree_view_set_model(treeView,(GtkTreeModel *)store);
	
	renderer = gtk_cell_renderer_toggle_new();
	column = gtk_tree_view_column_new_with_attributes(g_locale_to_utf8("电话报警",10,NULL,NULL,NULL), renderer,"active", Toggle_COL,NULL);   
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(selecttelalmdevtreeviewcb_selectck_toggled), store);
	
	renderer = gtk_cell_renderer_text_new();   
	column = gtk_tree_view_column_new_with_attributes(g_locale_to_utf8("设备逻辑名",10,NULL,NULL,NULL), renderer,"text", TEXT_MALMCOL,NULL);    
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);   
	gdk_pixbuf_unref((GdkPixbuf *)store);	

	set_selecttelalmdevtModel();//20120322
}



void createselectsmsalmdevtModel(GtkTreeView * treeView) 
{
	GtkCellRenderer *renderer;   
	GtkTreeViewColumn *column;  
	GtkWidget *cellView;
	GtkTreeStore *store;
	
	

	
	store = gtk_tree_store_new(2,G_TYPE_BOOLEAN, G_TYPE_STRING);
	cellView = gtk_cell_view_new();    
	
	gtk_tree_view_set_model(treeView,(GtkTreeModel *)store);
	
	renderer = gtk_cell_renderer_toggle_new();
	column = gtk_tree_view_column_new_with_attributes(g_locale_to_utf8("短信报警",10,NULL,NULL,NULL), renderer,"active", Toggle_COL,NULL);   
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(selectsmsalmdevtreeviewcb_selectck_toggled), store);
	
	renderer = gtk_cell_renderer_text_new();   
	column = gtk_tree_view_column_new_with_attributes(g_locale_to_utf8("设备逻辑名",10,NULL,NULL,NULL), renderer,"text", TEXT_MALMCOL,NULL);    
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);   
	gdk_pixbuf_unref((GdkPixbuf *)store);

	set_selectsmsalmdevtModel();//20120322
	
}


void createselecttelalmtModel(GtkTreeView * treeView) 
{
	GtkCellRenderer *renderer;   
	GtkTreeViewColumn *column;  
	GtkWidget *cellView;
	GtkTreeStore *store;
	
	store = gtk_tree_store_new(2,G_TYPE_BOOLEAN, G_TYPE_STRING);
	cellView = gtk_cell_view_new();    
	
	gtk_tree_view_set_model(treeView,(GtkTreeModel *)store);
	
	renderer = gtk_cell_renderer_toggle_new();
	column = gtk_tree_view_column_new_with_attributes(g_locale_to_utf8("  是否发送",10,NULL,NULL,NULL), renderer,"active", Toggle_COL,NULL);   
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(oncall_tree_view_toggle_cb), store);
	
	renderer = gtk_cell_renderer_text_new();   
	column = gtk_tree_view_column_new_with_attributes(g_locale_to_utf8("  报警描述",10,NULL,NULL,NULL), renderer,"text", TEXT_MALMCOL,NULL);    
	gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);   
	gdk_pixbuf_unref((GdkPixbuf *)store);
}


gint uidelete_event( GtkWidget *widget,GdkEvent  *event,gpointer data )
{
	int rtn;
	GtkWidget *dialog;
	GtkResponseType result;
	rtn=0;
	dialog = gtk_message_dialog_new((GtkWindow *)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
		g_locale_to_utf8("您确认要退出ONCALL系统?",25,NULL,NULL,NULL));
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	result=gtk_dialog_run(GTK_DIALOG(dialog));
		
	if (result == GTK_RESPONSE_YES)
	{
		rtn=1;	
	}
	gtk_widget_destroy(dialog);

	if(rtn==1)
	{
		//writelognow(" user uidelete exit!",1);
		//char temp_strs[400];
		//sprintf(temp_strs,"user",_ONCALLCOM_PORTS[i]);	
		insert_gtk_logs("oncall exit");
		exit(1); 
		return FALSE;
	}
	else
		return TRUE;
}

void uidestroy( GtkWidget *widget,gpointer data )
{
    gtk_main_quit();
}
void oncall_showmessage(char * str,int strlen)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new((GtkWindow *)window,
		GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
			g_locale_to_utf8(str,strlen,NULL,NULL,NULL), "title");
	gtk_window_set_title(GTK_WINDOW(dialog), "Information");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
    
}

void selecttelalmdevcb_selectck_toggled(GtkCheckButton *celectck,gpointer data)
{
	GtkTreeModel *treemodle;
	GtkTreeIter iter;
	gboolean fixed;
	gchar path_str[100];
	GtkTreePath *path;
	int i;
	
	memset(path_str,0,sizeof(path_str));
	treemodle = (GtkTreeModel *)(gtk_tree_view_get_model(data));
	
	fixed=celectck->toggle_button.active;
	
	for(i=0;i<oncall_alm_dev_def.real_oncall_alm_dev_def_num;i++)
	{
		sprintf(path_str,"%d",i);
		path= gtk_tree_path_new_from_string(path_str);
		if(!path)
		{
			gtk_tree_path_free(path);
			break;
		}

		if(gtk_tree_model_get_iter(treemodle, &iter, path))
		{	
			gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_MALMCOL, fixed, -1);
			if(fixed)
				oncall_alm_dev_def.TEL_ALM[i]=1;
			else
				oncall_alm_dev_def.TEL_ALM[i]=0;	
		}
		else i=-1;

		gtk_tree_path_free(path);
		if(i<0)break;
	}
}



void selectsmsalmdevcb_selectck_toggled(GtkCheckButton *celectck,gpointer data)
{
	GtkTreeModel *treemodle;
	GtkTreeIter iter;
	gboolean fixed;
	gchar path_str[100];
	GtkTreePath *path;
	int i;
	
	memset(path_str,0,sizeof(path_str));
	treemodle = (GtkTreeModel *)(gtk_tree_view_get_model(data));
	
	fixed=celectck->toggle_button.active;
	
	for(i=0;i<oncall_alm_dev_def.real_oncall_alm_dev_def_num;i++)
	{
		sprintf(path_str,"%d",i);
		path= gtk_tree_path_new_from_string(path_str);
		if(!path)
		{
			gtk_tree_path_free(path);
			break;
		}
		
		if(gtk_tree_model_get_iter(treemodle, &iter, path))
		{	
			gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_MALMCOL, fixed, -1);
			if(fixed)
				oncall_alm_dev_def.SMS_ALM[i]=1;
			else
				oncall_alm_dev_def.SMS_ALM[i]=0;	
		}
		else i=-1;
		
		gtk_tree_path_free(path);
		if(i<0)break;
	}
}


void selectmsgalmcb_selectck_toggled(GtkCheckButton *celectck,gpointer data)
{
	GtkTreeModel *treemodle;
	GtkTreeIter iter;
	gboolean fixed;
	gchar path_str[100];
	GtkTreePath *path;
	int i;
	
	memset(path_str,0,sizeof(path_str));
	treemodle = (GtkTreeModel *)(gtk_tree_view_get_model(data));
	
	fixed=celectck->toggle_button.active;
	
	for(i=0;i<ONCALL_SMS_MANUALMSENDBUF_SIZE;i++)
	{
		sprintf(path_str,"%d",i);
		path= gtk_tree_path_new_from_string(path_str);
		if(!path){gtk_tree_path_free(path);break;}
		if(gtk_tree_model_get_iter(treemodle, &iter, path))
			gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_MALMCOL, fixed, -1);
		else i=-1;
		gtk_tree_path_free(path);
		if(i<0)break;
	}
}
void selecttelalmcb_selectck_toggled(GtkCheckButton *celectck,gpointer data)
{
	GtkTreeModel *treemodle;
	GtkTreeIter iter;
	gboolean fixed;
	gchar path_str[100];
//	gchar path_str1[100];
	GtkTreePath *path;
	int i;
	
	memset(path_str,0,sizeof(path_str));
	treemodle = (GtkTreeModel *)(gtk_tree_view_get_model(data));
	
	fixed=celectck->toggle_button.active;
	
	for(i=0;i<ONCALL_DIAL_MANUALMBUF_SIZE;i++)
	{
		sprintf(path_str,"%d",i);
		path= gtk_tree_path_new_from_string(path_str);
		if(!path){gtk_tree_path_free(path);break;}
		if(gtk_tree_model_get_iter(treemodle, &iter, path))
			gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_MALMCOL, fixed, -1);
		else i=-1;
		gtk_tree_path_free(path);
		if(i<0)break;
	}
}
void namaselectb_selectck_toggled(GtkCheckButton *celectck,gpointer data)
{
	GtkTreeModel *treemodle;
	GtkTreeIter iter;
	gboolean fixed;
	gchar path_str[100];
	gchar path_str1[100];
	GtkTreePath *path;
	GtkTreePath  *path1;
	int i,j;
	

	treemodle = (GtkTreeModel *)(gtk_tree_view_get_model(data));

	fixed=celectck->toggle_button.active;
	
	for(i=0;i<TELPHONE_NUM;i++)
	{
		memset(path_str,0,sizeof(path_str));
		sprintf(path_str,"%d",i);
		path= gtk_tree_path_new_from_string(path_str);
		if(!path){gtk_tree_path_free(path);break;}
		if(gtk_tree_model_get_iter(treemodle, &iter, path))
		{
			gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_COL, fixed, -1);
			for(j=0;j<TELPHONE_NUM;j++)
			{
				memset(path_str1,0,sizeof(path_str1));
				sprintf(path_str1,"%s:%d",path_str,j);
				path1=gtk_tree_path_new_from_string(path_str1);
				if(gtk_tree_model_get_iter(treemodle, &iter, path1))
				{
					gtk_tree_store_set(GTK_TREE_STORE(treemodle), &iter, Toggle_COL, fixed, -1);
				}
				else j=-1;
				gtk_tree_path_free(path1);
				if(j<0)break;
			}
		}
		else i=-1;
		gtk_tree_path_free(path);
		if(i<0)break;
	}
}
void namaselectb_selectck_tostr(GtkTreeView *treeview)
{
	GtkTreeModel *treemodle;
	GtkTreeIter iter;
	gboolean fixed;
	gchar path_str[100];
	gchar path_str1[100];
	GtkTreePath *path;
	GtkTreePath  *path1;
	gchar *strsc;  
	int i,j,k;

	treemodle = (GtkTreeModel *)(gtk_tree_view_get_model(treeview));
	k=0;

	for(i=0;i<telphone_def.real_telphone_def_num;i++)
		telphone_def.manmsgsel[i]=0;
	for(i=0;i<telphone_def.real_telphone_def_num;i++)
	{
		memset(path_str,0,sizeof(path_str));
		sprintf(path_str,"%d",i);
		path= gtk_tree_path_new_from_string(path_str);
		if(!path){gtk_tree_path_free(path);break;}
		if(gtk_tree_model_get_iter(treemodle, &iter, path))
		{
			for(j=0;j<telphone_def.real_telphone_def_num;j++)
			{
				memset(path_str1,0,sizeof(path_str1));
				sprintf(path_str1,"%s:%d",path_str,j);
				path1=gtk_tree_path_new_from_string(path_str1);
				if(gtk_tree_model_get_iter(treemodle, &iter, path1))
				{
					gtk_tree_model_get(treemodle, &iter, Toggle_COL, &fixed, -1);
					if(fixed==1)
					{
						gtk_tree_model_get(treemodle, &iter, TEXT_COL, &strsc, -1);
						for(k=0;k<telphone_def.real_telphone_def_num;k++)
						{
							if(strcmp(strsc, g_locale_to_utf8(telphone_def.namecode[k],sizeof(telphone_def.namecode[k]),NULL,NULL,NULL))==0)
							{
								telphone_def.manmsgsel[k]=1;
								break;
							}
						}
					}
				}
				else j=-1;
				gtk_tree_path_free(path1);
				if(j<0)break;
			}
		}
		else i=-1;
		gtk_tree_path_free(path);
		if(i<0)break;
	}
}

void namaselectokbt_clicked(GtkButton *button,gpointer * user_data)
{
	* user_data=1;
}

void namaclearbt_clicked(GtkButton *button,gpointer user_data)
{
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	buffer = gtk_text_view_get_buffer((GtkTextView *)user_data);
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	gtk_text_buffer_delete (buffer, &start, &end);
}
void exitbtt_clicked(GtkButton *button,gpointer user_data)
{
	int rtn;
	GtkWidget *dialog;
	GtkResponseType result;
	rtn=0;
	dialog = gtk_message_dialog_new((GtkWindow *)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
		g_locale_to_utf8("您确认要退出ONCALL系统?",25,NULL,NULL,NULL));
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_YES)
	{
		rtn=1;	
	}
	gtk_widget_destroy(dialog);
	
	if(rtn==1)
	{
		//char temp_strs[400];
		//sprintf(temp_strs,"user",_ONCALLCOM_PORTS[i]);	
		insert_gtk_logs("oncall will exit by user");
		//writelognow(" user bt exit!",1);
		exit(1); 
	}
}
void cleartimingsendbufbt_clicked(GtkButton *button,gpointer user_data)
{
	int rtn;
	GtkWidget *dialog;
	GtkResponseType result;
	rtn=0;
	dialog = gtk_message_dialog_new((GtkWindow *)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
		g_locale_to_utf8("您确认清空定时发送缓冲区?",25,NULL,NULL,NULL));
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_YES)
	{
		rtn=1;	
	}
	gtk_widget_destroy(dialog);
	
	if(rtn==1)
	{
	 
		//char temp_strs[400];
		//sprintf(temp_strs,"user",_ONCALLCOM_PORTS[i]);	
		insert_gtk_logs("user clear timesendbuf!");


		oncall_sms_timingsendbuf.canceled=1;
		oncall_sms_timingsendbuf.add_Ptr=0;
		oncall_sms_timingsendbuf.send_Ptr=0;
		oncall_sms_timingsendbuf.writed_Ptr=0;
		//writelognow(" user clear timesendbuf!",1);
	}

}





void clearautoalmsendbufbt_clicked(GtkButton *button,gpointer user_data)
{
	int rtn;
	GtkWidget *dialog;
	GtkResponseType result;
	rtn=0;
	dialog = gtk_message_dialog_new((GtkWindow *)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
		g_locale_to_utf8("您确认清空自动报警发送缓冲区?",29,NULL,NULL,NULL));
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_YES)
	{
		rtn=1;	
	}
	gtk_widget_destroy(dialog);
	
	if(rtn==1)
	{
		
		//char temp_strs[400];
		//sprintf(temp_strs,"user",_ONCALLCOM_PORTS[i]);	
		insert_gtk_logs("user clear timesendbuf!");
		oncall_sms_autoalmsendbuf.canceled=1;
		oncall_sms_autoalmsendbuf.add_Ptr=0;
		oncall_sms_autoalmsendbuf.send_Ptr=0;
		oncall_sms_autoalmsendbuf.writed_Ptr=0;
		//writelognow(" user clear timesendbuf!",1);
	}
	
}



void clearmanualmsendbufbt_clicked(GtkButton *button,gpointer user_data)
{
	int rtn;
	GtkWidget *dialog;
	GtkResponseType result;
	rtn=0;
	dialog = gtk_message_dialog_new((GtkWindow *)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
		g_locale_to_utf8("您确认清空半自动报警发送缓冲区?",31,NULL,NULL,NULL));
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_YES)
	{
		rtn=1;	
	}
	gtk_widget_destroy(dialog);
	
	if(rtn==1)
	{
		
		//char temp_strs[400];
		//sprintf(temp_strs,"user",_ONCALLCOM_PORTS[i]);	
		insert_gtk_logs("user clear timesendbuf!");
		oncall_sms_manualmsendbuf.canceled=1;
		oncall_sms_manualmsendbuf.add_Ptr=0;
		oncall_sms_manualmsendbuf.send_Ptr=0;
		oncall_sms_manualmsendbuf.writed_Ptr=0;
		//writelognow(" user clear timesendbuf!",1);
	}
	
}

void clearmanxendbufbt_clicked(GtkButton *button,gpointer user_data)
{
	int rtn;
	GtkWidget *dialog;
	GtkResponseType result;
	rtn=0;
	dialog = gtk_message_dialog_new((GtkWindow *)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
		g_locale_to_utf8("您确认清空手动发送缓冲区?",25,NULL,NULL,NULL));
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_YES)
	{
		rtn=1;	
	}
	gtk_widget_destroy(dialog);
	
	if(rtn==1)
	{
		 
		//char temp_strs[400];
		//sprintf(temp_strs,"user",_ONCALLCOM_PORTS[i]);	
		insert_gtk_logs(" user clear mansendbuf!");

		oncall_sms_manusendbuf.canceled=1;
		oncall_sms_manusendbuf.add_Ptr=0;
		oncall_sms_manusendbuf.send_Ptr=0;
		oncall_sms_manusendbuf.writed_Ptr=0;
		//writelognow(" user clear mansendbuf!",1);
	}	
}
void startsendbt_clicked(GtkButton *button,gpointer user_data)
{
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	int i,j,k,l,m;
    gchar * sendnumstr,*sendtextstr;
	char  telnum[20];
	char  textstr[MAX_MESSAGE_SIZE];
    int canincnum;
	GtkWidget *dialog;
	GtkResponseType result;
	dialog = gtk_message_dialog_new((GtkWindow *)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
		g_locale_to_utf8("您确认开始发送短信?",19,NULL,NULL,NULL));
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if (result != GTK_RESPONSE_YES)
	{
		gtk_widget_destroy(dialog);
		return;
	}
	else
	{
		gtk_widget_destroy(dialog);
	}

	buffer = gtk_text_view_get_buffer((GtkTextView *)user_data);
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	sendnumstr=gtk_text_buffer_get_text(buffer, &start, &end,TRUE);

	memset(textstr,0,sizeof(textstr));
	buffer = gtk_text_view_get_buffer(mansendtexttextview);
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	sendtextstr=gtk_text_buffer_get_text(buffer, &start, &end,TRUE);
	
	k=0;
	j=0;
	l=0;
	canincnum=1;
	for(i=0;i<MAX_MESSAGE_SIZE;i++)
	{
		if(sendtextstr[i]==0)
			break;
		textstr[i]=sendtextstr[i];
	}
	UTF8ToAscii(textstr);
	if(textstr[0]==0)
	{
		oncall_showmessage("请您输入短信内容",16);
		return;
	}
	memset(telnum,0,sizeof(telnum));
	for(m=0;m<TELPHONE_NUM;m++)
		memset(oncall_sms_manusendbuf.telnum[oncall_sms_manusendbuf.add_Ptr][m],0,
			sizeof(oncall_sms_manusendbuf.telnum[oncall_sms_manusendbuf.add_Ptr][m]));
	for(i=0;i<TELPHONE_NUM*40;i++)
	{
		if(sendnumstr[i]==40)
			canincnum=0;
		if(sendnumstr[i]==41)
			canincnum=1;
		if(canincnum==0)continue;
	
		if(sendnumstr[i]==0)
		{
			if(j>2)
			{
				memcpy(oncall_sms_manusendbuf.telnum[oncall_sms_manusendbuf.add_Ptr][k],telnum,sizeof(telnum));
				if(k==0)
					memcpy(oncall_sms_manusendbuf.message[oncall_sms_manusendbuf.add_Ptr],textstr,sizeof(textstr));
				l++;
				k++;
				if(k>TELPHONE_NUM-1)
				{
					k=0;
					oncall_sms_manusendbuf.add_Ptr++;
					if(oncall_sms_manusendbuf.add_Ptr>ONCALL_SMS_MANUSENDBUF_SIZE-1)
						oncall_sms_manusendbuf.add_Ptr=0;
					for(m=0;m<TELPHONE_NUM;m++)
						memset(oncall_sms_manusendbuf.telnum[oncall_sms_manusendbuf.add_Ptr][m],0,
						sizeof(oncall_sms_manusendbuf.telnum[oncall_sms_manusendbuf.add_Ptr][m]));
				}
			}
			break;
		}
		if(sendnumstr[i]==59)
		{
			if(j>2)
			{
				memcpy(oncall_sms_manusendbuf.telnum[oncall_sms_manusendbuf.add_Ptr][k],telnum,sizeof(telnum));
				if(k==0)
					memcpy(oncall_sms_manusendbuf.message[oncall_sms_manusendbuf.add_Ptr],textstr,sizeof(textstr));
				l++;
				k++;
				if(k>TELPHONE_NUM-1)
				{
					k=0;
					oncall_sms_manusendbuf.add_Ptr++;
					if(oncall_sms_manusendbuf.add_Ptr>ONCALL_SMS_MANUSENDBUF_SIZE-1)
						oncall_sms_manusendbuf.add_Ptr=0;
					for(m=0;m<TELPHONE_NUM;m++)
						memset(oncall_sms_manusendbuf.telnum[oncall_sms_manusendbuf.add_Ptr][m],0,
						sizeof(oncall_sms_manusendbuf.telnum[oncall_sms_manusendbuf.add_Ptr][m]));
				}
			}
			j=0;
			memset(telnum,0,sizeof(telnum));
			continue;
		}
		if(sendnumstr[i]>=48 && sendnumstr[i]<=57 )
		{
			telnum[j]=sendnumstr[i];
			j++;
			if(j>19) {j=19;continue;}
		}
	}
	if(l>0)
	{
		oncall_sms_manusendbuf.add_Ptr++;
		if(oncall_sms_manusendbuf.add_Ptr>ONCALL_SMS_MANUSENDBUF_SIZE-1)
						oncall_sms_manusendbuf.add_Ptr=0;
	}
	else
	{
		oncall_showmessage("请您正确输入或选择电话号码",22);
	}
}

void cancelsendbt_clicked(GtkButton *button,gpointer user_data)
{
	int rtn;
	GtkWidget *dialog;
	GtkResponseType result;
	rtn=0;
	dialog = gtk_message_dialog_new((GtkWindow *)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
		g_locale_to_utf8("您确认清空手动发送缓冲区?",25,NULL,NULL,NULL));
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	result=gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_YES)
	{
		rtn=1;	
	}
	gtk_widget_destroy(dialog);
	
	if(rtn==1)
	{
		//char temp_strs[400];
		//sprintf(temp_strs,"user",_ONCALLCOM_PORTS[i]);	
		insert_gtk_logs(" user clear mansendbuf!");

		oncall_sms_manusendbuf.canceled=1;
		oncall_sms_manusendbuf.add_Ptr=0;
		oncall_sms_manusendbuf.send_Ptr=0;
		oncall_sms_manusendbuf.writed_Ptr=0;
		//writelognow(" user clear mansendbuf!",1);
	}		
}

void cancelselectmsgalmbt_clicked(GtkButton *button,gpointer data)
{
	oncall_sms_manualmsendbuf.startdo=1;
}

void cancelselecttelalmbt_clicked(GtkButton *button,gpointer data)
{
	oncall_dial_manualmsendbuf.startdo=1;
}

void namaselectbt_clicked(GtkButton *button,gpointer user_data)
{
	GladeXML  *nameselgxml;
	GtkWidget *dialog;
	GtkWidget *nameseltreeView;
	GtkTextView * namestrtextview;

	GtkTreeSelection *nameselselection;
 	//GtkResponseType result;
	GtkButton *okbutton;
	//GtkWidget *okbutton;

	GtkCheckButton *celectck;
 
	int resultnum,k;
	GtkTextBuffer *buffer;
	GtkTextIter iter;
 
	resultnum=0;
	nameselgxml = glade_xml_new(nameselectgladefilepath,NULL,NULL);

	nameseltreeView=(GtkWidget *)glade_xml_get_widget(nameselgxml,"treeview");
	okbutton=(GtkButton * )glade_xml_get_widget(nameselgxml,"okbt");
    g_signal_connect(G_OBJECT (okbutton), "clicked",G_CALLBACK (namaselectokbt_clicked), &resultnum);
	celectck=(GtkCheckButton * )glade_xml_get_widget(nameselgxml,"celectck");
	g_signal_connect(G_OBJECT (celectck), "toggled",G_CALLBACK (namaselectb_selectck_toggled), nameseltreeView);

	createnamaselectModel((GtkTreeView *)nameseltreeView);
	nameselselection = gtk_tree_view_get_selection(GTK_TREE_VIEW((GtkTreeView *)nameseltreeView));   
//	g_signal_connect(G_OBJECT(nameselselection), "changed", G_CALLBACK(maneselselection_changed), NULL);

    dialog = glade_xml_get_widget(nameselgxml, "namaseldialog");
	g_object_unref(G_OBJECT (nameselgxml));
	//result=
	gtk_dialog_run(GTK_DIALOG(dialog));

	if(resultnum==1)//确认按钮
	{ 
		namaselectb_selectck_tostr((GtkTreeView *)nameseltreeView);
		namestrtextview=(GtkTextView *)user_data;
		//va_start (varlist, format);
		//sprintf (selstr, varlist);
		buffer = gtk_text_view_get_buffer (namestrtextview);
		gtk_text_buffer_get_iter_at_offset (buffer, &iter, -1);
		resultnum=0;
		for(k=0;k<telphone_def.real_telphone_def_num;k++)
		{ 
			if(telphone_def.manmsgsel[k]==1)
			{	
				if(resultnum>0)gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(";",1,NULL,NULL,NULL) ,-1);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8("(",1,NULL,NULL,NULL) ,-1);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(telphone_def.name[k],20,NULL,NULL,NULL) ,-1);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(")",1,NULL,NULL,NULL) ,-1);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(telphone_def.telphone[k],20,NULL,NULL,NULL) ,-1);
			
				resultnum++;
			}
		}
	}

	

	gtk_widget_destroy(dialog);
}

gboolean timesendbuftextviewtime_handler(GtkWidget *widget)
{
	if(oncall_sms_timingsendbuf.add_Ptr==oncall_sms_timingsendbuf.send_Ptr && first_glag_forgtkui>100)
	{
		
		return TRUE;
	}
	else
	{
		GtkTextBuffer *buffer;
		GtkTextIter start, end;
		GtkTextIter iter;
		unsigned char str[256];
		int num;
		
		if(first_glag_forgtkui<200)
			first_glag_forgtkui++;
		buffer = gtk_text_view_get_buffer((GtkTextView *)widget);
		gtk_text_buffer_get_bounds (buffer, &start, &end);
		gtk_text_buffer_delete (buffer, &start, &end);
		gtk_text_buffer_get_iter_at_offset (buffer, &iter, -1);
		
		memset(str,0,sizeof(str));
		
		if(oncall_sms_timingsendbuf.add_Ptr>=oncall_sms_timingsendbuf.send_Ptr)
			num=oncall_sms_timingsendbuf.add_Ptr-oncall_sms_timingsendbuf.send_Ptr;
		else
			num=ONCALL_SMS_TIMINGSENDBUF_SIZE-oncall_sms_timingsendbuf.add_Ptr+oncall_sms_timingsendbuf.send_Ptr;
		sprintf(str," 缓冲区大小:%d组(每组可最多发送至%d人)\n 已添加信息指向第 %d 组\n 已发送信息指向第 %d 组\n 等待发送组数 %d",
			ONCALL_SMS_TIMINGSENDBUF_SIZE,TELPHONE_NUM,oncall_sms_timingsendbuf.add_Ptr,oncall_sms_timingsendbuf.send_Ptr,num);
		gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
		
		return TRUE;
	}
}


gboolean autoalmsendbuftextviewtime_handler(GtkWidget *widget)
{	
	if(oncall_sms_autoalmsendbuf.add_Ptr==oncall_sms_autoalmsendbuf.send_Ptr && first_glag_forgtkui>100)
	{
		
		return TRUE;
	}
	else
	{
		GtkTextBuffer *buffer;
		GtkTextIter start, end;
		GtkTextIter iter;
		unsigned char str[256];
		int num;

		if(first_glag_forgtkui<200)
			first_glag_forgtkui++;
		
		buffer = gtk_text_view_get_buffer((GtkTextView *)widget);
		gtk_text_buffer_get_bounds (buffer, &start, &end);
		gtk_text_buffer_delete (buffer, &start, &end);
		gtk_text_buffer_get_iter_at_offset (buffer, &iter, -1);
		
		memset(str,0,sizeof(str));
		
		
		if(oncall_sms_autoalmsendbuf.add_Ptr>=oncall_sms_autoalmsendbuf.send_Ptr)
			num=oncall_sms_autoalmsendbuf.add_Ptr-oncall_sms_autoalmsendbuf.send_Ptr;
		else
			num=ONCALL_SMS_AUTOALMSENDBUF_SIZE-oncall_sms_autoalmsendbuf.add_Ptr+oncall_sms_autoalmsendbuf.send_Ptr;
		sprintf(str," 缓冲区大小:%d组(每组可最多发送至%d人)\n 已添加信息指向第 %d 组\n 已发送信息指向第 %d 组\n 等待发送组数 %d",
			ONCALL_SMS_AUTOALMSENDBUF_SIZE,TELPHONE_NUM,oncall_sms_autoalmsendbuf.add_Ptr,oncall_sms_autoalmsendbuf.send_Ptr,num);
		gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
		
		return TRUE;
	}
}
gboolean manualmsendbuftextviewtime_handler(GtkWidget *widget)
{
	if(oncall_sms_manualmsendbuf.add_Ptr==oncall_sms_manualmsendbuf.send_Ptr && first_glag_forgtkui>100)
	{
		
		return TRUE;
	}
	else
	{
		GtkTextBuffer *buffer;
		GtkTextIter start, end;
		GtkTextIter iter;
		unsigned char str[256];
		int num;
		
		
		if(first_glag_forgtkui<200)
			first_glag_forgtkui++;
		buffer = gtk_text_view_get_buffer((GtkTextView *)widget);
		gtk_text_buffer_get_bounds (buffer, &start, &end);
		gtk_text_buffer_delete (buffer, &start, &end);
		gtk_text_buffer_get_iter_at_offset (buffer, &iter, -1);
		
		memset(str,0,sizeof(str));
		
		
		if(oncall_sms_manualmsendbuf.add_Ptr>=oncall_sms_manualmsendbuf.send_Ptr)
			num=oncall_sms_manualmsendbuf.add_Ptr-oncall_sms_manualmsendbuf.send_Ptr;
		else
			num=ONCALL_SMS_MANUALMSENDBUF_SIZE-oncall_sms_manualmsendbuf.add_Ptr+oncall_sms_manualmsendbuf.send_Ptr;
		sprintf(str," 缓冲区大小:%d组(每组可最多发送至%d人)\n 已添加信息指向第 %d 组\n 已发送信息指向第 %d 组\n 等待发送组数 %d",
			ONCALL_SMS_MANUALMSENDBUF_SIZE,TELPHONE_NUM,oncall_sms_manualmsendbuf.add_Ptr,oncall_sms_manualmsendbuf.send_Ptr,num);
		gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
		
		return TRUE;
	}
}

gboolean runinglogstextviewtime_handler(GtkWidget *widget)
{
	
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	GtkTextIter iter;
	unsigned char str[512];
	int _ii,j,writednum,g_str_len;
	gchar * g_str;
	
 
	buffer = gtk_text_view_get_buffer((GtkTextView *)widget);
	gtk_text_buffer_get_bounds (buffer, &start, &end);
 
	//gtk_text_buffer_delete (buffer, &start, &end);

	
	  g_str=g_strdup(gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer),&start,&end,FALSE));
	//      printf("the string is : %s\n",string);
	  g_str_len = strlen(g_str);
 
	  if(g_str_len>10000)
	  {
		  gtk_text_buffer_get_iter_at_offset (buffer, &start, 0);
		  gtk_text_buffer_get_iter_at_offset (buffer, &end, 2000);
 
		  gtk_text_buffer_delete(buffer, &start, &end);
		  
	  }
	  gtk_text_buffer_get_iter_at_offset (buffer, &iter, -1);
 
		memset(str,0,sizeof(str));
	 
		for(_ii=0;_ii<MAX_sms_dev_num;_ii++)
		{	
			if(_ONCALLCOM_PORTS[_ii][0]==0)
				continue;
		 
			if(oncall_sms_logsbufs[_ii].view_ptr!=oncall_sms_logsbufs[_ii].a_ptr)
			{
				j=0;
				while(oncall_sms_logsbufs[_ii].view_ptr!=oncall_sms_logsbufs[_ii].a_ptr)
				{
				
					if(oncall_sms_logsbufs[_ii].flag[oncall_sms_logsbufs[_ii].view_ptr]!=1)
					{
						oncall_sms_logsbufs[_ii].view_ptr++;
						if(oncall_sms_logsbufs[_ii].view_ptr>=MAX_LOGNUM_PER_BUF)
							oncall_sms_logsbufs[_ii].view_ptr=0;
						j++;
						if(j>=MAX_LOGNUM_PER_BUF-1)
							break;
						continue;
					}
					if(oncall_sms_logsbufs[_ii].str[oncall_sms_logsbufs[_ii].view_ptr][0]==0)
					{
						oncall_sms_logsbufs[_ii].view_ptr++;
						if(oncall_sms_logsbufs[_ii].view_ptr>=MAX_LOGNUM_PER_BUF)
							oncall_sms_logsbufs[_ii].view_ptr=0;
						j++;
						if(j>=MAX_LOGNUM_PER_BUF-1)
							break;
						continue;
					}
					sprintf(str,"\nS%d:%s", _ii,oncall_sms_logsbufs[_ii].str[oncall_sms_logsbufs[_ii].view_ptr]);
					gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);

					writednum++;
					
					oncall_sms_logsbufs[_ii].view_ptr++;
					if(oncall_sms_logsbufs[_ii].view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_sms_logsbufs[_ii].view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
				}
			 
			}
		}
 
		for(_ii=0;_ii<MAXCHAN;_ii++)
		{	
			if(CHANUSE[_ii]!=1)
			continue;
		 
		 
			if(oncall_tel_logsbufs[_ii].view_ptr!=oncall_tel_logsbufs[_ii].a_ptr)
			{
			 
				j=0;
				while(oncall_tel_logsbufs[_ii].view_ptr!=oncall_tel_logsbufs[_ii].a_ptr)
				{
					
					if(oncall_tel_logsbufs[_ii].flag[oncall_tel_logsbufs[_ii].view_ptr]!=1)
					{
						oncall_tel_logsbufs[_ii].view_ptr++;
						if(oncall_tel_logsbufs[_ii].view_ptr>=MAX_LOGNUM_PER_BUF)
							oncall_tel_logsbufs[_ii].view_ptr=0;
						j++;
						if(j>=MAX_LOGNUM_PER_BUF-1)
							break;
						continue;
					}
					if(oncall_tel_logsbufs[_ii].str[oncall_tel_logsbufs[_ii].view_ptr][0]==0)
					{
						oncall_tel_logsbufs[_ii].view_ptr++;
						if(oncall_tel_logsbufs[_ii].view_ptr>=MAX_LOGNUM_PER_BUF)
							oncall_tel_logsbufs[_ii].view_ptr=0;
						j++;
						if(j>=MAX_LOGNUM_PER_BUF-1)
							break;
						continue;
					}
				 	sprintf(str,"\nS%d:%s", _ii,oncall_tel_logsbufs[_ii].str[oncall_tel_logsbufs[_ii].view_ptr]);
					gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
					writednum++;
					
					oncall_tel_logsbufs[_ii].view_ptr++;
					if(oncall_tel_logsbufs[_ii].view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_tel_logsbufs[_ii].view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
				}
			 
			}
		}
	 
		
		if(oncall_others_logsbuf.view_ptr!=oncall_others_logsbuf.a_ptr)
		{
			j=0;
			while(oncall_others_logsbuf.view_ptr!=oncall_others_logsbuf.a_ptr)
			{
				if(oncall_others_logsbuf.flag[oncall_others_logsbuf.view_ptr]!=1)
				{
					oncall_others_logsbuf.view_ptr++;
					if(oncall_others_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_others_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_others_logsbuf.str[oncall_others_logsbuf.view_ptr][0]==0)
				{
					oncall_others_logsbuf.view_ptr++;
					if(oncall_others_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_others_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				sprintf(str,"\nS%d:%s", _ii,oncall_others_logsbuf.str[oncall_others_logsbuf.view_ptr]);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
				writednum++;
				
				oncall_others_logsbuf.view_ptr++;
				if(oncall_others_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_others_logsbuf.view_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
		 
		}

		if(oncall_timingsend_logsbuf.view_ptr!=oncall_timingsend_logsbuf.a_ptr)
		{
		 
			j=0;
			while(oncall_timingsend_logsbuf.view_ptr!=oncall_timingsend_logsbuf.a_ptr)
			{
				if(oncall_timingsend_logsbuf.flag[oncall_timingsend_logsbuf.view_ptr]!=1)
				{
					oncall_timingsend_logsbuf.view_ptr++;
					if(oncall_timingsend_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_timingsend_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_timingsend_logsbuf.str[oncall_timingsend_logsbuf.view_ptr][0]==0)
				{
					oncall_timingsend_logsbuf.view_ptr++;
					if(oncall_timingsend_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_timingsend_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				 sprintf(str,"\nS%d:%s", _ii,oncall_timingsend_logsbuf.str[oncall_timingsend_logsbuf.view_ptr]);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
			
				writednum++;
				
				oncall_timingsend_logsbuf.view_ptr++;
				if(oncall_timingsend_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_timingsend_logsbuf.view_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
		 
		}

 
		
		if(oncall_manusend_logsbuf.view_ptr!=oncall_manusend_logsbuf.a_ptr)
		{
			 
			j=0;
			while(oncall_manusend_logsbuf.view_ptr!=oncall_manusend_logsbuf.a_ptr)
			{
				if(oncall_manusend_logsbuf.flag[oncall_manusend_logsbuf.view_ptr]!=1)
				{
					oncall_manusend_logsbuf.view_ptr++;
					if(oncall_manusend_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_manusend_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_manusend_logsbuf.str[oncall_manusend_logsbuf.view_ptr][0]==0)
				{
					oncall_manusend_logsbuf.view_ptr++;
					if(oncall_manusend_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_manusend_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
			 	sprintf(str,"\nS%d:%s", _ii,oncall_manusend_logsbuf.str[oncall_manusend_logsbuf.view_ptr]);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
			
				writednum++;
				
				oncall_manusend_logsbuf.view_ptr++;
				if(oncall_manusend_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_manusend_logsbuf.view_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
		 
		}


		if(oncall_gtk_logsbuf.view_ptr!=oncall_gtk_logsbuf.a_ptr)
		{
		 
			j=0;
			while(oncall_gtk_logsbuf.view_ptr!=oncall_gtk_logsbuf.a_ptr)
			{
				if(oncall_gtk_logsbuf.flag[oncall_gtk_logsbuf.view_ptr]!=1)
				{
					oncall_gtk_logsbuf.view_ptr++;
					if(oncall_gtk_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_gtk_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_gtk_logsbuf.str[oncall_gtk_logsbuf.view_ptr][0]==0)
				{
					oncall_gtk_logsbuf.view_ptr++;
					if(oncall_gtk_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_gtk_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
			 	sprintf(str,"\nS%d:%s", _ii,oncall_gtk_logsbuf.str[oncall_gtk_logsbuf.view_ptr]);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
			
				writednum++;
				
				oncall_gtk_logsbuf.view_ptr++;
				if(oncall_gtk_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_gtk_logsbuf.view_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
		 
		}

		if(oncall_almsearch_logsbuf.view_ptr!=oncall_almsearch_logsbuf.a_ptr)
		{
		 
			j=0;
			while(oncall_almsearch_logsbuf.view_ptr!=oncall_almsearch_logsbuf.a_ptr)
			{
				if(oncall_almsearch_logsbuf.flag[oncall_almsearch_logsbuf.view_ptr]!=1)
				{
					oncall_almsearch_logsbuf.view_ptr++;
					if(oncall_almsearch_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_almsearch_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_almsearch_logsbuf.str[oncall_almsearch_logsbuf.view_ptr][0]==0)
				{
					oncall_almsearch_logsbuf.view_ptr++;
					if(oncall_almsearch_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_almsearch_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
			    sprintf(str,"\nS%d:%s", _ii,oncall_almsearch_logsbuf.str[oncall_almsearch_logsbuf.view_ptr]);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
			
				writednum++;
				
				oncall_almsearch_logsbuf.view_ptr++;
				if(oncall_almsearch_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_almsearch_logsbuf.view_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
		}

		if(oncall_doublehost_logsbuf.view_ptr!=oncall_doublehost_logsbuf.a_ptr)
		{
		 
			j=0;
			while(oncall_doublehost_logsbuf.view_ptr!=oncall_doublehost_logsbuf.a_ptr)
			{
				if(oncall_doublehost_logsbuf.flag[oncall_doublehost_logsbuf.view_ptr]!=1)
				{
					oncall_doublehost_logsbuf.view_ptr++;
					if(oncall_doublehost_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_doublehost_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
				if(oncall_doublehost_logsbuf.str[oncall_doublehost_logsbuf.view_ptr][0]==0)
				{
					oncall_doublehost_logsbuf.view_ptr++;
					if(oncall_doublehost_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
						oncall_doublehost_logsbuf.view_ptr=0;
					j++;
					if(j>=MAX_LOGNUM_PER_BUF-1)
						break;
					continue;
				}
			 	sprintf(str,"\nS%d:%s", _ii,oncall_doublehost_logsbuf.str[oncall_doublehost_logsbuf.view_ptr]);
				gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
			
				
				writednum++;
				
				oncall_doublehost_logsbuf.view_ptr++;
				if(oncall_doublehost_logsbuf.view_ptr>=MAX_LOGNUM_PER_BUF)
					oncall_doublehost_logsbuf.view_ptr=0;
				j++;
				if(j>=MAX_LOGNUM_PER_BUF-1)
					break;
			}
		 
}
	return TRUE;
	
}


gboolean harddevicestattextviewtime_handler(GtkWidget *widget)
{
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	GtkTextIter iter;
	unsigned char str[256];
	int num,j;
	
	if(lock_oncall_sms_err_times==1)
		return TRUE;
	buffer = gtk_text_view_get_buffer((GtkTextView *)widget);
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	gtk_text_buffer_delete (buffer, &start, &end);
	gtk_text_buffer_get_iter_at_offset (buffer, &iter, -1);
	
	memset(str,0,sizeof(str));
	num=0;
	strcpy(str,"故障短信设备列表\n");
	for(j=0;j<MAX_sms_dev_num;j++)
	{
		if(oncall_sms_err_times[j]>6)
		{
			strcat(str,_BAK_ONCALLCOM_PORTS[j]);
			strcat(str,"\n");
			num++;
		}
	}

	if(num>0)
	{

		
		
	}
	else
	{
		strcpy(str,"正在检测短信设备......");
	}

 	gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
	
	return TRUE;
}



gboolean selectmsgalmtreeviewtime_handler(GtkWidget *widget)
{
	GtkTreeStore *store;
	GtkTreeModel *treemodle;
	GtkTreePath *path;
	GtkTreeIter iter;
	gchar path_str[100];
	int i,k;
	gboolean fixed;
	fixed=0;
	
 	treemodle=gtk_tree_view_get_model((GtkTreeView *)widget);
	
	store=(GtkTreeStore *)treemodle;
	
	for(i=oncall_sms_manualmsendbuf.confirmPtr;i!=oncall_sms_manualmsendbuf.add_Ptr;)
	{
		gtk_tree_store_append(store, &iter, NULL);
		gtk_tree_store_set(store, &iter,Toggle_MALMCOL,0,TEXT_MALMCOL,g_locale_to_utf8(oncall_sms_manualmsendbuf.longname[oncall_sms_manualmsendbuf.confirmPtr],
			sizeof(oncall_sms_manualmsendbuf.longname[oncall_sms_manualmsendbuf.confirmPtr]),NULL,NULL,NULL),-1); 
		
		i++;
		if(i>ONCALL_SMS_MANUALMSENDBUF_SIZE-1) i=0;

		oncall_sms_manualmsendbuf.confirmPtr++;
		if(oncall_sms_manualmsendbuf.confirmPtr>=ONCALL_SMS_MANUALMSENDBUF_SIZE)
		{
			oncall_sms_manualmsendbuf.confirmPtr=0;
			//Sleep(1000);//防止死循环
			//break;
		}
	}

	if(oncall_sms_manualmsendbuf.startdo==1)
	{
		for(i=oncall_sms_manualmsendbuf.startdoPtr,k=0;k<ONCALL_SMS_MANUALMSENDBUF_SIZE && i!=oncall_sms_manualmsendbuf.confirmPtr;k++)
		{
			memset(path_str,0,sizeof(path_str));
			sprintf(path_str,"%d",k);
			path= gtk_tree_path_new_from_string(path_str);
			if(!path){gtk_tree_path_free(path);break;}
			if(gtk_tree_model_get_iter(treemodle, &iter, path))
				gtk_tree_model_get(treemodle, &iter, Toggle_MALMCOL, &fixed, -1);
			else i=-1;
			gtk_tree_path_free(path);
			if(i<0)break;
			
			if(fixed==1)
				oncall_sms_manualmsendbuf.confirmed[i]=1;
			else
				oncall_sms_manualmsendbuf.confirmed[i]=2;
			
			i++;
			if(i>ONCALL_SMS_MANUALMSENDBUF_SIZE-1) i=0;
			
			oncall_sms_manualmsendbuf.startdoPtr++;
			if(oncall_sms_manualmsendbuf.startdoPtr>=ONCALL_SMS_MANUALMSENDBUF_SIZE)
				oncall_sms_manualmsendbuf.startdoPtr=0;
		}
		
		if (gtk_tree_model_get_iter_first(treemodle, &iter) == FALSE) 
			return FALSE;
		gtk_tree_store_clear(store);

		oncall_sms_manualmsendbuf.startdo=0;
	}

	return TRUE;
}
gboolean selecttelalmtreeviewtime_handler(GtkWidget *widget)
{
	GtkTreeStore *store;
	GtkTreeModel *treemodle;
	GtkTreePath *path;
	GtkTreeIter iter;
	gchar path_str[100];
	int i,k;
	gboolean fixed;
	fixed=0;
	treemodle=gtk_tree_view_get_model((GtkTreeView *)widget);
	
	store=(GtkTreeStore *)treemodle;
	
	for(i=oncall_dial_manualmsendbuf.confirmPtr;i!=oncall_dial_manualmsendbuf.add_Ptr;)
	{
		gtk_tree_store_append(store, &iter, NULL);
		gtk_tree_store_set(store, &iter,Toggle_MALMCOL,0,TEXT_MALMCOL,g_locale_to_utf8(oncall_dial_manualmsendbuf.longname[oncall_dial_manualmsendbuf.confirmPtr],
			sizeof(oncall_dial_manualmsendbuf.longname[oncall_dial_manualmsendbuf.confirmPtr]),NULL,NULL,NULL),-1); 
		
		i++;
		if(i>ONCALL_SMS_MANUALMSENDBUF_SIZE-1) i=0;
		
		oncall_dial_manualmsendbuf.confirmPtr++;
		if(oncall_dial_manualmsendbuf.confirmPtr>=ONCALL_SMS_MANUALMSENDBUF_SIZE)
		{
			oncall_dial_manualmsendbuf.confirmPtr=0;
			//Sleep(1000);//防止死循环
			//break;
		}
	}
	
	if(oncall_dial_manualmsendbuf.startdo==1)
	{
		for(i=oncall_dial_manualmsendbuf.startdoPtr,k=0;k<ONCALL_SMS_MANUALMSENDBUF_SIZE && i!=oncall_dial_manualmsendbuf.confirmPtr;k++)
		{
			memset(path_str,0,sizeof(path_str));
			sprintf(path_str,"%d",k);
			path= gtk_tree_path_new_from_string(path_str);
			if(!path){gtk_tree_path_free(path);break;}
			if(gtk_tree_model_get_iter(treemodle, &iter, path))
				gtk_tree_model_get(treemodle, &iter, Toggle_MALMCOL, &fixed, -1);
			else i=-1;
			gtk_tree_path_free(path);
			if(i<0)break;
			
			if(fixed==1)
				oncall_dial_manualmsendbuf.confirmed[i]=1;
			else
				oncall_dial_manualmsendbuf.confirmed[i]=2;
			
			i++;
			if(i>ONCALL_SMS_MANUALMSENDBUF_SIZE-1) i=0;
			
			oncall_dial_manualmsendbuf.startdoPtr++;
			if(oncall_dial_manualmsendbuf.startdoPtr>=ONCALL_SMS_MANUALMSENDBUF_SIZE)
				oncall_dial_manualmsendbuf.startdoPtr=0;
		}
		
		if (gtk_tree_model_get_iter_first(treemodle, &iter) == FALSE) 
			return FALSE;
		gtk_tree_store_clear(store);
		
		oncall_dial_manualmsendbuf.startdo=0;
	}
	return TRUE;
}
gboolean mansendbuftextviewtime_handler(GtkWidget *widget)
{
	if(oncall_sms_manusendbuf.add_Ptr==oncall_sms_manusendbuf.send_Ptr && first_glag_forgtkui>100)
	{
		
		return TRUE;
	}
	else
	{
		GtkTextBuffer *buffer;
		GtkTextIter start, end;
		GtkTextIter iter;
		unsigned char str[256];
		int num;
		if(first_glag_forgtkui<200)
			first_glag_forgtkui++;
		buffer = gtk_text_view_get_buffer((GtkTextView *)widget);
		gtk_text_buffer_get_bounds (buffer, &start, &end);
		gtk_text_buffer_delete (buffer, &start, &end);
		gtk_text_buffer_get_iter_at_offset (buffer, &iter, -1);
		
		memset(str,0,sizeof(str));
		
		if(oncall_sms_manusendbuf.add_Ptr>=oncall_sms_manusendbuf.send_Ptr)
			num=oncall_sms_manusendbuf.add_Ptr-oncall_sms_manusendbuf.send_Ptr;
		else
			num=ONCALL_SMS_MANUSENDBUF_SIZE-oncall_sms_manusendbuf.add_Ptr+oncall_sms_manusendbuf.send_Ptr;
		sprintf(str," 缓冲区大小:%d组(每组可最多发送至%d人)\n 已添加信息指向第 %d 组\n 已发送信息指向第 %d 组\n 等待发送组数 %d",
			ONCALL_SMS_MANUSENDBUF_SIZE,TELPHONE_NUM,oncall_sms_manusendbuf.add_Ptr,oncall_sms_manusendbuf.send_Ptr,num);
		gtk_text_buffer_insert(buffer, &iter,g_locale_to_utf8(str,sizeof(str),NULL,NULL,NULL) ,-1);
		
		return TRUE;
	}
}
int oncallgtkmain(int argc,char *argv[])
{
//    GtkWidget *button;
//	GdkScreen *screen;
//	GdkDisplay *display;
	//gchar       screenname[20],screenname1[20];
	//char  local_hostname[MAX_HOST_NAME_SIZE];
	//int i;
	GladeXML    *maingxml;
	GtkButton   *namaselectbt;
	GtkTextView * namestrtextview;
	GtkTextView * timesendbuftextview;
	 
	GtkTextView * autoalmsendbuftextview;//0401
	GtkTextView * manualmsendbuftextview;//0401
	GtkTextView * runinglogstexttextview;//0401
	GtkTextView * harddevicestattexttextview;//0401
	GtkTextView * mansendbuftextview;

	//GtkLayout   * mainlayout;
	//GtkLabel    *label;
	//GtkWidget *treeView;  
	//GtkTreeSelection *selection;

	GtkButton * namaclearbt;
	GtkButton * exitbt;
	GtkButton * cleartimingsendbufbt;

	GtkButton * clearautoalmsendbufbt;//0401
	GtkButton * clearmanualmsendbufbt;//0401

	
	GtkButton * clearmanxendbufbt;
	GtkButton * startsendbt;
	GtkButton * cancelsendbt;
	GtkButton * cancelselectmsgalmbt;
	GtkButton * cancelselecttelalmbt;



	GtkTreeView * selectmsgalmtreeview;
	GtkTreeView * selecttelalmtreeview;


	GtkCheckButton * selecttelalmdevcb;//2012 01 13
	GtkCheckButton * selectsmsalmdevcb;//2012 01 15

	GtkCheckButton * selectmsgalmcb;
	GtkCheckButton * selecttelalmcb;

	GtkVBox* timesendhbox;//0331
	GtkHBox * main_hbox;
	gint temp_hight;
	gint temp_width;


	GdkColor color_Blue,color_Orange,color_Red,color_Black,color_LightSkyBlue;//0331
	gdk_color_parse ("Blue", &color_Blue);
	gdk_color_parse ("Orange", &color_Orange);
	gdk_color_parse ("Red", &color_Red);
	gdk_color_parse ("Black", &color_Black);
	gdk_color_parse ("LightSkyBlue", &color_LightSkyBlue);
	
 
	/*
	GdkColor color;
	
	  gdk_color_parse ("颜色(如green,red,blue)", &color);
	  
		gtk_widget_modify_text (GTK_WIDGET(控件), GTK_STATE_NORMAL, &color);//文本颜色
		
		  gtk_widget_modify_bg (GTK_WIDGET(控件), GTK_STATE_NORMAL, &color);//背景色
		  
			gtk_widget_modify_fg (GTK_WIDGET(控件), GTK_STATE_NORMAL, &color);//前景色
	*/


	gtk_set_locale();
    gtk_init (&argc, &argv);
    maingxml = glade_xml_new(maingladefilepath,NULL,NULL);
    window = glade_xml_get_widget(maingxml, "oncallmainwindow");
	//glade_xml_signal_autoconnect(gxml);
	//mainlayout=(GtkLayout * )glade_xml_get_widget(maingxml,"mainlayout");
	//gtk_layout_set_hadjustment()
	//gtk_layout_set_size(mainlayout,1280,700);
	//treeView=(GtkTreeView * )glade_xml_get_widget(maingxml,"treeview1");

	namestrtextview=(GtkTextView * )glade_xml_get_widget(maingxml,"namestrtextview");
	namaclearbt=(GtkButton * )glade_xml_get_widget(maingxml,"namaclearbt");
	g_signal_connect(G_OBJECT (namaclearbt), "clicked",G_CALLBACK (namaclearbt_clicked), namestrtextview);

	timesendbuftextview=(GtkTextView * )glade_xml_get_widget(maingxml,"timesendbuftextview");
	autoalmsendbuftextview=(GtkTextView * )glade_xml_get_widget(maingxml,"autoalmsendbuftextview");//0401
	manualmsendbuftextview=(GtkTextView * )glade_xml_get_widget(maingxml,"manualmsendbuftextview");//0401
	runinglogstexttextview=(GtkTextView * )glade_xml_get_widget(maingxml,"runinglogstexttextview");//0401
	harddevicestattexttextview=(GtkTextView * )glade_xml_get_widget(maingxml,"harddevicestattexttextview");//0401

	
	
	mansendbuftextview=(GtkTextView * )glade_xml_get_widget(maingxml,"mansendbuftextview");
	mansendtexttextview=(GtkTextView * )glade_xml_get_widget(maingxml,"mansendtexttextview");

	selecttelalmdevtreeview=(GtkTreeView * )glade_xml_get_widget(maingxml,"selecttelalmdevtreeview");//20120213
	selectsmsalmdevtreeview=(GtkTreeView * )glade_xml_get_widget(maingxml,"selectsmsalmdevtreeview");//20120213

	selectmsgalmtreeview=(GtkTreeView * )glade_xml_get_widget(maingxml,"selectmsgalmtreeview");
	selecttelalmtreeview=(GtkTreeView * )glade_xml_get_widget(maingxml,"selecttelalmtreeview");

	timesendhbox=(GtkVBox * )glade_xml_get_widget(maingxml,"timesendhbox");//0331	
	main_hbox=(GtkHBox * )glade_xml_get_widget(maingxml,"main_hbox");//0331	
 
	createselecttelalmdevtModel(selecttelalmdevtreeview);//20120213
	createselectsmsalmdevtModel(selectsmsalmdevtreeview);//20120215

	createselectmsgalmtModel(selectmsgalmtreeview);
	createselecttelalmtModel(selecttelalmtreeview);

 
	
	
	selecttelalmdevcb=(GtkCheckButton * )glade_xml_get_widget(maingxml,"selecttelalmdevcb");//2012
	g_signal_connect(G_OBJECT (selecttelalmdevcb), "toggled",G_CALLBACK (selecttelalmdevcb_selectck_toggled), selecttelalmdevtreeview);//2012

	selectsmsalmdevcb=(GtkCheckButton * )glade_xml_get_widget(maingxml,"selectsmsalmdevcb");//2012
	g_signal_connect(G_OBJECT (selectsmsalmdevcb), "toggled",G_CALLBACK (selectsmsalmdevcb_selectck_toggled), selectsmsalmdevtreeview);//2012


	selectmsgalmcb=(GtkCheckButton * )glade_xml_get_widget(maingxml,"selectmsgalmcb");
	g_signal_connect(G_OBJECT (selectmsgalmcb), "toggled",G_CALLBACK (selectmsgalmcb_selectck_toggled), selectmsgalmtreeview);

	selecttelalmcb=(GtkCheckButton * )glade_xml_get_widget(maingxml,"selecttelalmcb");
	g_signal_connect(G_OBJECT (selecttelalmcb), "toggled",G_CALLBACK (selecttelalmcb_selectck_toggled), selecttelalmtreeview);

	namaselectbt=(GtkButton * )glade_xml_get_widget(maingxml,"namaselectbt");
    g_signal_connect(G_OBJECT (namaselectbt), "clicked",G_CALLBACK (namaselectbt_clicked), namestrtextview);

	exitbt=(GtkButton * )glade_xml_get_widget(maingxml,"exitbt");
    g_signal_connect(G_OBJECT (exitbt), "clicked",G_CALLBACK (exitbtt_clicked), NULL);

	cleartimingsendbufbt=(GtkButton * )glade_xml_get_widget(maingxml,"cleartimingsendbufbt");
	g_signal_connect(G_OBJECT (cleartimingsendbufbt), "clicked",G_CALLBACK (cleartimingsendbufbt_clicked), NULL);

	clearautoalmsendbufbt=(GtkButton * )glade_xml_get_widget(maingxml,"clearautoalmsendbufbt");
	g_signal_connect(G_OBJECT (clearautoalmsendbufbt), "clicked",G_CALLBACK (clearautoalmsendbufbt_clicked), NULL);
	
	clearmanualmsendbufbt=(GtkButton * )glade_xml_get_widget(maingxml,"clearmanualmsendbufbt");
	g_signal_connect(G_OBJECT (clearmanualmsendbufbt), "clicked",G_CALLBACK (clearmanualmsendbufbt_clicked), NULL);
	


	clearmanxendbufbt=(GtkButton * )glade_xml_get_widget(maingxml,"clearmanxendbufbt");
	g_signal_connect(G_OBJECT (clearmanxendbufbt), "clicked",G_CALLBACK (clearmanxendbufbt_clicked), NULL);

	startsendbt=(GtkButton * )glade_xml_get_widget(maingxml,"startsendbt");
	g_signal_connect(G_OBJECT (startsendbt), "clicked",G_CALLBACK (startsendbt_clicked), namestrtextview);

	cancelsendbt=(GtkButton * )glade_xml_get_widget(maingxml,"cancelsendbt");
	g_signal_connect(G_OBJECT (cancelsendbt), "clicked",G_CALLBACK (cancelsendbt_clicked), NULL);
	
	cancelselectmsgalmbt=(GtkButton * )glade_xml_get_widget(maingxml,"cancelselectmsgalmbt");
	g_signal_connect(G_OBJECT (cancelselectmsgalmbt), "clicked",G_CALLBACK (cancelselectmsgalmbt_clicked), selectmsgalmtreeview);

	cancelselecttelalmbt=(GtkButton * )glade_xml_get_widget(maingxml,"cancelselecttelalmbt");
	g_signal_connect(G_OBJECT (cancelselecttelalmbt), "clicked",G_CALLBACK (cancelselecttelalmbt_clicked), selecttelalmtreeview);
	
	// g_signal_connect (G_OBJECT (gtkbutton), "delete_event",G_CALLBACK (uidelete_event), NULL);
    // g_signal_connect (G_OBJECT (gtkbutton), "destroy",G_CALLBACK (uidestroy), NULL);
 	//label=(GtkLabel * ) glade_xml_get_widget(maingxml,"label");
    //gtk_widget_set_size_request(label, 300, 200);
    g_object_unref(G_OBJECT (maingxml));
	g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (uidelete_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy",G_CALLBACK (uidestroy), NULL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title (GTK_WINDOW (window), "H9000-ONCALL(V4.2) Running Form");

	gtk_window_set_resizable (GTK_WINDOW (window), TRUE);
 
	//gethostname(local_hostname,sizeof(local_hostname));
	//strcpy (screenname ,  "\\") ;
	//sprintf(screenname1 , "%s:0.%d\0",local_hostname,i) ;
	//strcat(screenname,screenname1) ;
	//printf ("screenname: %s \n",screenname);	
	//display = gdk_display_open (screenname);
	//screen=gdk_display_get_default_screen(display);
	//gtk_widget_set_usize(window,gdk_screen_get_width(screen)-5,gdk_screen_get_height(screen)-65);
	//gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	//gtk_widget_set_usize(gtkmainlayout,gdk_screen_get_width(screen)-5,gdk_screen_get_height(screen)-65);
	//createModel(treeView);
	//selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeView));   
	//g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(selection_changed), label);
 
	gtk_window_maximize(window);
 

 	
	g_timeout_add(ONCALL_GtkWidget_INFRO_REDRAW, (GSourceFunc)timesendbuftextviewtime_handler,timesendbuftextview);
	g_timeout_add(ONCALL_GtkWidget_INFRO_REDRAW, (GSourceFunc)mansendbuftextviewtime_handler,mansendbuftextview);

	g_timeout_add(ONCALL_GtkWidget_INFRO_REDRAW, (GSourceFunc)autoalmsendbuftextviewtime_handler,autoalmsendbuftextview);//0401
	g_timeout_add(ONCALL_GtkWidget_INFRO_REDRAW, (GSourceFunc)manualmsendbuftextviewtime_handler,manualmsendbuftextview);//0401
	g_timeout_add(ONCALL_GtkWidget_INFRO_REDRAW_L, (GSourceFunc)runinglogstextviewtime_handler,runinglogstexttextview);//0401


	g_timeout_add(ONCALL_GtkWidget_INFRO_REDRAW_L, (GSourceFunc)harddevicestattextviewtime_handler,harddevicestattexttextview);//0401


	g_timeout_add(ONCALL_GtkWidget_INFRO_REDRAW, (GSourceFunc)selectmsgalmtreeviewtime_handler,selectmsgalmtreeview);
	g_timeout_add(ONCALL_GtkWidget_INFRO_REDRAW, (GSourceFunc)selecttelalmtreeviewtime_handler,selecttelalmtreeview);
	gtk_widget_show (window);
	gtk_widget_set_uposition(window,0,0);
	
	gtk_window_get_size(GTK_WINDOW(window), &temp_width, &temp_hight);//0331


    gtk_widget_set_usize((GtkWidget *)main_hbox,temp_width-50,temp_hight-50);//0331


 	//gtk_widget_modify_text (GTK_WIDGET(timesendbuftextview), GTK_STATE_NORMAL, &color_Orange); 
	//gtk_widget_modify_fg (GTK_WIDGET(timesendbuftextview), GTK_STATE_NORMAL, &color_Red); 
	gtk_widget_modify_bg (GTK_WIDGET(timesendbuftextview), GTK_STATE_NORMAL, &color_LightSkyBlue); //0401 
	gtk_widget_modify_bg (GTK_WIDGET(autoalmsendbuftextview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_bg (GTK_WIDGET(manualmsendbuftextview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_bg (GTK_WIDGET(mansendbuftextview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_bg (GTK_WIDGET(namestrtextview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_bg (GTK_WIDGET(mansendtexttextview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_bg (GTK_WIDGET(runinglogstexttextview), GTK_STATE_NORMAL, &color_Blue); 
	gtk_widget_modify_bg (GTK_WIDGET(harddevicestattexttextview), GTK_STATE_NORMAL, &color_Red); 

	

	gtk_widget_modify_bg (GTK_WIDGET(selecttelalmdevtreeview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_bg (GTK_WIDGET(selectsmsalmdevtreeview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_bg (GTK_WIDGET(selectmsgalmtreeview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_bg (GTK_WIDGET(selecttelalmtreeview), GTK_STATE_NORMAL, &color_LightSkyBlue); 
	gtk_widget_modify_text(GTK_WIDGET(exitbt), GTK_STATE_NORMAL, &color_Orange); 
 
    gtk_main ();
  /*  i=0;
	   gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (delete_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy",G_CALLBACK (destroy), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    button = gtk_button_new_with_label("Hello World");
    g_signal_connect(G_OBJECT (button), "clicked",G_CALLBACK (oncall_message_dialog_clicked), NULL);
	//g_signal_connect_swapped (G_OBJECT (button), "clicked",G_CALLBACK (gtk_widget_destroy),window); 
    gtk_container_add (GTK_CONTAINER (window), button);
	gtk_window_set_title (GTK_WINDOW (window), "H9000 ONCALL");
	gethostname(local_hostname,sizeof(local_hostname));
	strcpy (screenname ,  "\\") ;
	sprintf(screenname1 , "%s:0.%d\0",local_hostname,i) ;
	strcat(screenname,screenname1) ;
	printf ("screenname: %s \n",screenname);	
	display = gdk_display_open (screenname);
	screen=gdk_display_get_default_screen(display);
	gtk_widget_set_usize(window,gdk_screen_get_width(screen)-5,gdk_screen_get_height(screen)-65);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    gtk_widget_show (button);
    gtk_widget_show (window);
	gtk_widget_set_uposition(window,0,0);
    gtk_main();
	*/
    return 0;
} 
