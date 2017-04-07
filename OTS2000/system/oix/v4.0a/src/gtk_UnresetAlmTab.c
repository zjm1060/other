
/****
 2006.6 created by zyp 
 2007.08.09 : have been SYNC  by chichi;
 LAST UPATE : 2007.10.24 by chichi ; 
*****************/



#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
/*#define	DEBUG*/
 
#include	"../inc/oix.h"
#include        "../../../dps/v4.0a/inc/fault_table_def.h"
 
#include        "../inc/gtk_fault_alm_tab.h"
 
#define SHOW_ALL     99
#define SHOW_NONE    100
#define MAX_FILTER_NUMBER		32
#define MAX_FILTER_NAME_LEN		64
#define MAX_UNIT_STATUS_NUM		32



extern int PrintTitleLocation[8];
extern int PrintElementLocation[8] ;
extern char PrintLocationRuler[PRINT_RULER_LEN];



void UnresetAlmDspProc1 ( GdkDrawable *drawable , int crt_n, int isPrint);
extern  char OIX_EVENT_DSP_LOGICNAME ; 
extern  char OIX_EVENT_ALIAN_MOD ; 
extern int	GetAlmStrings(REC_ENTRY_UNION *rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
void	UnresetAlarmAckEX (GtkWidget *w,int crt_n ,int startI , int endI  );
extern int  	SetTimeFilter  (GtkWidget *entry, time_t *TimeRes);
extern void	DrawAlmTabHead(GdkDrawable *drawable ,GdkGC *gc ,int crt_n );
extern gint 	isDisplayMore(GtkWidget       *widget,   gpointer     data);
extern int  SetFileterToEntry(GtkWidget *entry, time_t *TimeRes) ; 
extern int getMenuDataPointer(int crtn,int data);
extern int getCrtNumFromPointer(int data);
extern int getDataFromPointer(int data);

static  GtkWidget* CreateDevFilterDlgUnset(GtkWidget *parent ,int crt_n);
static  GtkWidget* CreateUnresetAlarmTab (GtkWidget *parent,	int crt_n   );
static GtkWidget *CreateUnresetAlmToolbar (GtkWidget *,int );
void  unreset_scroll_page(GtkAdjustment *adj , int crt_n) ;
void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n) ;
void    windk_draw_text_EX(GdkDrawable * drawable ,char  *fontdescr ,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n ) ;
UNRESET_INDEX   	UnresetAlmIndex_tmp[MAX_CRT_NUMBER];
SEL_PT   	un_sel_pt[MAX_CRT_NUMBER]; /*add by hcl for print*/

gint UnresetAlmFilterInit(GtkWidget*w , gpointer data);
gint  UnresetAlmDspProc(int crt_n );

UINT    	UnresetDevMask[MAX_CRT_NUMBER]= {0xffffffff,0xffffffff,0xffffffff};
UINT		UnresetGroupMask[MAX_CRT_NUMBER][MAX_GROUP_MASK_NUM] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
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

FILTER_T 	FilterUnresetAlm[MAX_CRT_NUMBER];
GtkWidget     	*UnresetAlmfilterDlg[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};
GtkWidget       *UnresetSetFilterDlg[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};

GtkWidget 	*UnresetAlm_pop_shell[MAX_CRT_NUMBER];
GtkWidget 	*UnresetAlm_canvas[MAX_CRT_NUMBER];
GdkGC  		*UnresetAlm_draw_gc[MAX_CRT_NUMBER];
GtkWidget 	*Unresetitem[MAX_CRT_NUMBER][GROUP_ENTRY_NUMBER];
GtkWidget	*UnresetAlmFaultItem[MAX_CRT_NUMBER][UNRESETALM_DATATYPE_NUM ];
GtkWidget	*UnresetAlmTypeIdItem[MAX_CRT_NUMBER][UNRESETALM_DATATYPE_NUM ];
GdkPixmap 	*UnresetAlmOffDrawable[MAX_CRT_NUMBER] ;
GtkListStore *UnresetStoreFilter[MAX_CRT_NUMBER],*UnresetStoreDev[MAX_CRT_NUMBER],*UnresetStoreGroup[MAX_CRT_NUMBER];
gint		IsLastPage[MAX_CRT_NUMBER]={0,0,0};
static int Unreset_select_start_id=0,Unreset_select_end_id=0,Unreset_print_select_tag[MAX_CRT_NUMBER];
GtkAdjustment  *unreset_adj[MAX_CRT_NUMBER] ;
int UnresetCurDev;
int	UnresetFilterNum,UnresetCurFilter;
gboolean	Unreset_auto_filter_flag[MAX_CRT_NUMBER];
STNDEF	UnresetAlarmStnDef[STATION_ENTRY_NUMBER];
gint 	Unreset_autofilter_timeout_id[MAX_CRT_NUMBER] ={0,0,0} ;
/*add by hcl for print */
char *UnresetAlarmHeadTitle[] = {OIX_STR_REC_NUMBER   , OIX_STR_TIME  ,OIX_STR_REC_LOGNAME, OIX_STR_MSG_NAME  ,OIX_STR_RT_STATUS,OIX_STR_USER};

extern void	DrawRcd (int crt_n, GdkDrawable	*drawable, GdkGC *gc, BACKRCD *rcd ,BOOL isTrutype);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void PrintList(GtkWidget*parent,int crt_n,int prt_obj_type ,char *output_file);
extern void SetPageInfo(PAGE_INFO *PageInfo );
extern void SetAdjInfo(PAGE_INFO *PageInfo,GtkAdjustment *AdjSet);
extern int    gdk_rgb(int red  ,int green, int  blue , GdkColor *clr  );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern void 	CmdBufInit ( int crt_n );
extern windk_text_height    (GdkFont        *font,     const gchar    *text,   gint txt_length ,char *fontdesc);
extern GtkWidget* PopupDetailInfoBox(GtkWidget *parent,int crt_n,POINTER pt); 
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern char *_fromUtf8(char *c);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern BOOL  isStnInMask (UCHAR stn_id);
extern int ReadFilterXml(GtkListStore *listStore,const char *nodeName);
extern int SaveFilterXml(gchar *filterName,FILTER_T *pFilter,const char *nodeName);
extern int DelFilterXml(gchar *filterName,const char *nodeName);

int   unreset_scroll_event (GtkWidget*widget,   GdkEventScroll *event, gpointer  data)  
{   int crt_n  = (int) data; 
    
	if (event->direction == GDK_SCROLL_DOWN)
	{
      // down code
		unreset_adj[crt_n]->value ++ ; 
//printf ("---------scroll++++++++++++++++++++1\n");
	}
    else if (event->direction == GDK_SCROLL_UP)
    {
      // up code
		unreset_adj[crt_n]->value -- ; 
//printf ("---------scroll---------------------1\n");
    }

    
    
	unreset_scroll_page(unreset_adj[crt_n] ,   crt_n);

	return TRUE;
		
}

void DrawUnresetItemTag ( int crt_n, GdkDrawable *drawable )
{
	int	i;
        BACKRCD unreset_tag_rcd[MAX_CRT_NUMBER];
	/*if ( db_tag[crt_n] != ITEM_TAG_ON )
		return;*/

	/*y_med	= ( db_tab_window_info[crt_n].select_pt.pt_id - db_page_cnt[crt_n]*DBTAB_PAGE_SIZE )*DBH0 + DBY0;*/
        if(Unreset_print_select_tag[crt_n]==1)
            {
                Unreset_select_end_id= Unreset_select_start_id;
        }
        for(i=Unreset_select_start_id;i<=Unreset_select_end_id;i++)
        {
            unreset_tag_rcd[crt_n].fg		= WHITE;  
            unreset_tag_rcd[crt_n].bg		= BLACK3;
            unreset_tag_rcd[crt_n].rect.x1	= ALM_X0;
            unreset_tag_rcd[crt_n].rect.x2	= LOCAL_WIDTH;
            unreset_tag_rcd[crt_n].rect.y1	= ALM_Y0+(i-unreset_page[crt_n].CurPage*ALM_PAGE_SIZE)*ALM_ROW-10;
            unreset_tag_rcd[crt_n].rect.y2	= ALM_Y0+(i-unreset_page[crt_n].CurPage*ALM_PAGE_SIZE)*ALM_ROW+8;

            unreset_tag_rcd[crt_n].draw_type	= DrwT_RECTANGLE;
            unreset_tag_rcd[crt_n].fill.fill_type	= FILLED;
            unreset_tag_rcd[crt_n].line_width	= 1;
            
            DrawRcd ( crt_n,  UnresetAlmOffDrawable[crt_n], UnresetAlm_draw_gc[crt_n], &unreset_tag_rcd[crt_n],FALSE);
        }
}








void PrintUnresetTab(GtkWidget *widget, gpointer data)
{
    int crt_n=(gint)data;
    /* hcl 此处是大系统打印 此代码在 PrintList()中调用
    char    prt_cmd_str[50];
	
	  if(Unreset_print_select_tag[crt_n]==0)
	  printf("please select print region\n");
	  else
	  {
	  sprintf(prt_cmd_str,"%s%s","lpr -P HP ",UnresetPrintFile);
	  system(prt_cmd_str);
	  printf("Unreset file is located in /h9000tmp\n");        
}*/
	
	
	if(!CheckUserPrivilege(cur_usr_name, PRIV_PRINT))
	{
        PopupMessageBox ( UnresetAlm_pop_shell[crt_n], MSG_RPT_PRINTING  ); 
        return  ;
    }
    
    
    // if(Unreset_print_select_tag[crt_n]==0 )
// 	if(unreset_page[crt_n].isSelected==0)
//         PopupMessageBox ( UnresetAlm_pop_shell[crt_n],MSG_SELECT_PRINT_RANGE); 
//     else  
    {
				
        UnresetAlmDspProc1(UnresetAlmOffDrawable[crt_n], crt_n,TRUE);
		PrintList(UnresetAlm_pop_shell[crt_n],crt_n,ALM_LIST,UnresetPrintFile); 
    }
    return  ;
}


void  InitUnrestToolBarButton(GtkWidget*parent,int crt_n)
{
	GtkWidget  *opmenu; 

	opmenu = g_object_get_data(G_OBJECT(parent),"StationOpMenu" );
	gtk_option_menu_set_history     ( GTK_OPTION_MENU( opmenu) ,0);

	opmenu = g_object_get_data(G_OBJECT(parent),"DataTypeOpt" );
	gtk_option_menu_set_history     ( GTK_OPTION_MENU( opmenu) ,0);
                                             
    opmenu = g_object_get_data(G_OBJECT(parent),"TypeIdOpt" );
	gtk_option_menu_set_history     ( GTK_OPTION_MENU(opmenu) ,0);
                                             	
 

}




/*gint SelectUnresetPrintRegion(GtkWidget *widget,GdkEvent *event , gpointer data)
{
    int crt_n = (gint)data;
    int x,y,tmp_id;
    
    if (event->type == GDK_BUTTON_PRESS) 
    {  
        GdkEventButton *bevent = (GdkEventButton *) event; 
        x = bevent->x ;
        y = bevent->y ;
        if (bevent->button == 1) 
        {       
                Unreset_print_select_tag[crt_n]++;
                if(Unreset_print_select_tag[crt_n]>2)
                    {
                    Unreset_print_select_tag[crt_n]=0;
                    Unreset_select_start_id=Unreset_select_end_id=0;
                }
       printf("***print_select_tag=%d,start_id=%d,end_id=%d\n",Unreset_print_select_tag[crt_n],Unreset_select_start_id, Unreset_select_end_id);
 
                if(Unreset_print_select_tag[crt_n]==1)
                    {   
                        printf("print_select_tag[crt_n]==1\n");
                    Unreset_select_start_id = MIN(UnresetAlmIndex_tmp[crt_n].count-1,unreset_page[crt_n].CurPage*ALM_PAGE_SIZE+(y-ALM_Y0+ALM_ROW)/ALM_ROW);                  
                }
                else if(Unreset_print_select_tag[crt_n]==2)
                    {
                       printf("log_cnt=%d\n",UnresetAlmIndex_tmp[crt_n].count); 
                    Unreset_select_end_id = MIN(UnresetAlmIndex_tmp[crt_n].count-1,unreset_page[crt_n].CurPage*ALM_PAGE_SIZE+(y-ALM_Y0+ALM_ROW)/ALM_ROW);                    
                }
       printf("^^^^^x=%d,y=%d,start_id=%d,end_id=%d\n",x,y, Unreset_select_start_id, Unreset_select_end_id);       
                if(Unreset_select_end_id<Unreset_select_start_id&&Unreset_print_select_tag[crt_n]==2)
                    {
                       tmp_id= Unreset_select_end_id;
                       Unreset_select_end_id= Unreset_select_start_id;
                       Unreset_select_start_id= tmp_id;
                }
                printf("^^^^^button 1 is pressed:x=%d,y=%d,start_id=%d,end_id=%d\n",x,y, Unreset_select_start_id, Unreset_select_end_id);                
        }
        return FALSE;
    }
    return FALSE;
}*/

void InitUnresetPageInfo(PAGE_INFO *PageInfo , int crt_n)
{
       PageInfo->RecCount = shm_fault_addr->head.count ;
       PageInfo->PageCount = (PageInfo->RecCount+PageInfo->PageSize-1)/PageInfo->PageSize ;
       PageInfo->CurPage   = MAX(PageInfo->PageCount-1 , 0) ;
       PageInfo->isSelected =0;
	   PageInfo->SelectStart = PageInfo->SelectEnd =0;
       PageInfo->PageStart =  (PageInfo->CurPage) * (PageInfo->PageSize) ;
       PageInfo->PageEnd   =   MIN ((PageInfo->PageStart + PageInfo->PageSize -1) , (PageInfo->RecCount-1) )   ;        
}

gint SaveUnresetCustomFilter(GtkWidget	*widget,  gpointer	data)
{   
	GtkWidget *filterNameEntry,*filterList;
	int crt_n,i;
//	GtkTreeModel *model;
//	GtkListStore *store;
	GtkTreeIter  iter; 	
	GtkWidget *window = data;
//	gint num;
	gchar *filterName;
	gchar stn_id[2],cmd_type[2],fault_mask[9],type_id_mask[9],dev_mask[9],group_mask[450],datatype_mask[9];
	int offset =0;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	
	if (!FilterUnresetAlm[crt_n].change_flag)
	{
		PopupMessageBox(UnresetAlm_pop_shell[crt_n],MSG_FILTER_NOT_CHANGE);
		return FALSE;
	}
	filterList = g_object_get_data(G_OBJECT(window),"customFilterList");
	filterNameEntry  = g_object_get_data(G_OBJECT(window),"customFilterNameEntry");
	if (UnresetFilterNum > MAX_FILTER_NUMBER)
	{
		PopupMessageBox(UnresetAlm_pop_shell[crt_n],MSG_FILTER_OUTNUMBER);
		return FALSE;
	}
	else
	{
		if (strlen(gtk_entry_get_text(GTK_ENTRY(filterNameEntry))) > MAX_FILTER_NAME_LEN)
		{
			PopupMessageBox(UnresetAlm_pop_shell[crt_n],MSG_FILTER_NAME_MAX_LEN); 
			return FALSE;
		}
		else if (strcmp(gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),"")==0)
		{
			PopupMessageBox(UnresetAlm_pop_shell[crt_n],MSG_INPUT_FILTER_NAME);
			return FALSE;
		}
		else
		{
			if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(UnresetStoreFilter[crt_n]),&iter))
			{
				do 
				{
					gtk_tree_model_get(GTK_TREE_MODEL(UnresetStoreFilter[crt_n]),&iter,eName_Filter,&filterName,-1);
					if (strcmp(filterName,gtk_entry_get_text(GTK_ENTRY(filterNameEntry)))==0)
					{
						PopupMessageBox(UnresetAlm_pop_shell[crt_n],MSG_FILTER_NAME_REPEAT);
						return FALSE;
					}
						
				} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(UnresetStoreFilter[crt_n]),&iter));
			}
			sprintf(stn_id,"%u",FilterUnresetAlm[crt_n].stn_id);
			sprintf(cmd_type,"%u",FilterUnresetAlm[crt_n].cmd_type);
			sprintf(fault_mask,"%u",FilterUnresetAlm[crt_n].fault_mask);
			sprintf(type_id_mask,"%u",FilterUnresetAlm[crt_n].type_id_mask);
			sprintf(dev_mask,"%08X",FilterUnresetAlm[crt_n].dev_mask[0]);
			for (i=0;i<MAX_GROUP_MASK_NUM;i++)
			{
				offset += sprintf(group_mask + offset, "%08X", FilterUnresetAlm[crt_n].group_mask[i]); 
 			}
			sprintf(datatype_mask,"%08X",FilterUnresetAlm[crt_n].datatype_mask);
			gtk_list_store_insert(GTK_LIST_STORE(UnresetStoreFilter[crt_n]), &iter,1);	
			gtk_list_store_set(GTK_LIST_STORE(UnresetStoreFilter[crt_n]), &iter, eName_Filter,gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),
																			 	 eStnId_Filter,stn_id,
																				 eFaultMask_Filter,fault_mask,
																				 eTypeIdMask_Filter,type_id_mask,
																				 eDevMask_Filter,dev_mask,
																				 eGroupMask_Filter,group_mask,
																				 eDataTypeMask_Filter,datatype_mask,-1);
			SaveFilterXml(gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),&FilterUnresetAlm[crt_n],"Predefine");
			gtk_entry_set_text(GTK_ENTRY(filterNameEntry),"");	 
			UnresetFilterNum++;
		}
	}
	FilterUnresetAlm[crt_n].change_flag = FALSE;
	return TRUE;
}

gint DelUnresetCustomFilter(GtkWidget	*widget,  gpointer	data)
{
	int crt_n;
	GtkWidget *filterList;
	gchar *filterName;
//	GtkListStore *store;
	GtkTreeModel *model;
	GtkTreeIter  iter, filter_iter;
	GtkTreeSelection *selection; 
	GtkWidget *window = data;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	
	filterList = g_object_get_data(G_OBJECT(window),"customFilterList");
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (filterList));
	selection  = gtk_tree_view_get_selection(GTK_TREE_VIEW(filterList));
	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) 
		return FALSE;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) 
	{
		gtk_tree_model_get(model,&iter,eName_Filter,&filterName,-1);
		gtk_tree_model_filter_convert_iter_to_child_iter(GTK_TREE_MODEL_FILTER(model), &filter_iter, &iter);
		gtk_list_store_remove(GTK_LIST_STORE(UnresetStoreFilter[crt_n]), &filter_iter);	
		DelFilterXml(filterName,"Predefine");
		g_free(filterName);
		UnresetFilterNum--;
	}
	return TRUE;
}

gboolean  UnresetRowVisible(GtkTreeModel *model,GtkTreeIter *iter,gpointer data)
{
	gchar *filterName;
	gchar *pConvert=0, *pConvert1=0;
	gtk_tree_model_get(model, iter, eName_Filter, &filterName, -1);
	if (filterName == NULL)
		return FALSE;
	if (strcmp(filterName, pConvert=_toUtf8EX(OIX_STR_FILTER_SET))==0 || 
		strcmp(filterName, pConvert1=_toUtf8EX(OIX_STR_FILTER_INIT))==0)
	{
		G_SAFE_FREE(pConvert);
		G_SAFE_FREE(pConvert1);
		return FALSE;
	}
	else
	{
		G_SAFE_FREE(pConvert);
		G_SAFE_FREE(pConvert1);
		return TRUE;
	}
}
gint CustomUnresetFilterDlgDelete(GtkWidget *w , gpointer data);
static GtkWidget*
CreateUnresetCustomFilterDlg(GtkWidget *parent ,int crt_n)
{
	GtkWidget *window;
	GtkWidget *vbox,*hbox1,*hbox2;
	GtkWidget *list;
	GtkWidget *scrolled_window; 
	GtkWidget *separator; 
	GtkWidget *button1,*button2,*button3; 
	//	GtkWidget *list_item; 
	GtkWidget *entry;
	GtkWidget *label;
	GtkTreeModel *modelfilter;
	GtkCellRenderer    *renderer;
	GtkTreeViewColumn  *column;
	//	GtkListStore       *store;
	//	int i;

	gchar *pConvert=0;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), pConvert=_toUtf8EX(OIX_STR_FILTER_SET));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent)); 
	gtk_window_set_destroy_with_parent(GTK_WINDOW(window),TRUE);
	
	gtk_signal_connect (GTK_OBJECT (window), "delete_event", 
		GTK_SIGNAL_FUNC (CustomUnresetFilterDlgDelete), GINT_TO_POINTER (crt_n)); 
	
	gtk_widget_show (window);
	vbox = gtk_vbox_new (FALSE, 5);
	gtk_widget_show (vbox);
	gtk_container_border_width(GTK_CONTAINER(vbox), 5); 
	gtk_container_add (GTK_CONTAINER (window), vbox);
	
	scrolled_window=gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_usize(scrolled_window, 250, 150); 
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(vbox), scrolled_window); 
	gtk_widget_show(scrolled_window); 
	
	list = gtk_tree_view_new();
	//gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), list);
	g_object_set_data(G_OBJECT(window),"customFilterList",list);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(pConvert=_toUtf8EX(OIX_STR_FILTER_NAME),renderer, "text", eName_Filter, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column(GTK_TREE_VIEW (list), column);
	modelfilter = gtk_tree_model_filter_new(GTK_TREE_MODEL(UnresetStoreFilter[crt_n]),NULL);
	gtk_tree_model_filter_set_visible_func(GTK_TREE_MODEL_FILTER(modelfilter),UnresetRowVisible,NULL,NULL);
	gtk_tree_view_set_model(GTK_TREE_VIEW (list), GTK_TREE_MODEL(modelfilter));
	g_object_unref(UnresetStoreFilter[crt_n]);
	gtk_container_add (GTK_CONTAINER (scrolled_window), list);
	gtk_widget_show(list);
	
	hbox2 = gtk_hbox_new (FALSE, 2);
	gtk_widget_show (hbox2);
	gtk_container_border_width(GTK_CONTAINER(hbox2), 2); 
	gtk_container_add (GTK_CONTAINER (vbox), hbox2);
	
	label = gtk_label_new(pConvert=_toUtf8EX(OIX_STR_FILTER_NAME));
	G_SAFE_FREE(pConvert);
	gtk_container_add(GTK_CONTAINER(hbox2), label);
	gtk_widget_show (label);
	
	entry = gtk_entry_new ();
	gtk_container_add(GTK_CONTAINER(hbox2), entry);
	gtk_widget_show (entry);
	g_object_set_data(G_OBJECT(window),"customFilterNameEntry",entry);
	gtk_entry_set_max_length(GTK_ENTRY(entry),MAX_FILTER_NAME_LEN);
	
	separator=gtk_hseparator_new(); 
	gtk_container_add(GTK_CONTAINER(vbox), separator); 
	gtk_widget_show(separator); 
	
	hbox1 = gtk_hbox_new (FALSE, 2);
	gtk_widget_show (hbox1);
	gtk_container_border_width(GTK_CONTAINER(hbox1), 2); 
	gtk_container_add (GTK_CONTAINER (vbox), hbox1);
	
	button1=gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ADD)); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (SaveUnresetCustomFilter),  window);
	gtk_container_add(GTK_CONTAINER(hbox1), button1); 
	gtk_widget_show(button1); 
	
	button2=gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_DEL)); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (DelUnresetCustomFilter),  window);
	gtk_container_add(GTK_CONTAINER(hbox1), button2); 
	gtk_widget_show(button2); 
	
	button3=gtk_button_new_with_label(pConvert=_toUtf8EX("OIX_CSTR_EXIT")); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button3), "clicked",GTK_SIGNAL_FUNC (CustomUnresetFilterDlgDelete),  GINT_TO_POINTER(crt_n));
	gtk_container_add(GTK_CONTAINER(hbox1), button3); 
	gtk_widget_show(button3);
	
	return  window ;	
}


gint ChgUnresetCustomFilter(GtkComboBox* pComboBox, gpointer data)
{
	GtkTreeModel *pModel;
	GtkTreeIter iter;
	GtkWidget  *opmenu;
	int i,j;
	char *filterName;
	gchar *stn_id,*fault_mask,*type_id_mask,*dev_mask,*group_mask,*datatype_mask;
	int offset = 0;
	gchar *pConvert=0, *pConvert1=0;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBox));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBox), &iter))
	{
		return FALSE;
	}
	gtk_tree_model_get(pModel, &iter, eName_Filter, &filterName, -1);
	if (strcmp(filterName, pConvert=_toUtf8EX(OIX_STR_FILTER_SET))==0)
	{
		if (UnresetSetFilterDlg[crt_n] == (GtkWidget *)NULL)
		{
			UnresetSetFilterDlg[crt_n] = CreateUnresetCustomFilterDlg( UnresetAlm_pop_shell[crt_n] ,crt_n);
			gtk_widget_show(UnresetSetFilterDlg[crt_n]);
		}
		else
			gtk_window_present (GTK_WINDOW(UnresetSetFilterDlg[crt_n]));
		if (FilterUnresetAlm[crt_n].change_flag)
			gtk_combo_box_set_active(pComboBox,-1);
		else
			gtk_combo_box_set_active(pComboBox,UnresetCurFilter);
	}
	else if (strcmp(filterName, pConvert1=_toUtf8EX(OIX_STR_FILTER_INIT))==0)
	{
		UnresetAlmFilterInit(UnresetAlm_pop_shell[crt_n], GINT_TO_POINTER(crt_n));
	}
	else
	{
		gtk_tree_model_get(pModel, &iter, eStnId_Filter, &stn_id,
										  eFaultMask_Filter,&fault_mask, 
										  eTypeIdMask_Filter,&type_id_mask,
										  eDevMask_Filter,&dev_mask,
										  eGroupMask_Filter,&group_mask,
										  eDataTypeMask_Filter,&datatype_mask,-1);
		FilterUnresetAlm[crt_n].stn_id = (UCHAR)atoi(stn_id);
		FilterUnresetAlm[crt_n].fault_mask = (UINT)atoi(fault_mask);
		FilterUnresetAlm[crt_n].type_id_mask = (UINT)atoi(type_id_mask);
		sscanf(dev_mask,"%08X",&FilterUnresetAlm[crt_n].dev_mask[0]);
		for (i=0;i<MAX_GROUP_MASK_NUM;i++)
		{
			sscanf(group_mask+offset,"%08X",&FilterUnresetAlm[crt_n].group_mask[i]);
			offset += 8;
		}
		sscanf(datatype_mask,"%08X",&FilterUnresetAlm[crt_n].datatype_mask);
		
		opmenu = g_object_get_data(G_OBJECT(UnresetAlm_pop_shell[crt_n]),"StationOpMenu" );
		gtk_option_menu_set_history( GTK_OPTION_MENU(opmenu) ,(guint)FilterUnresetAlm[crt_n].stn_id);
		
		opmenu = g_object_get_data(G_OBJECT(UnresetAlm_pop_shell[crt_n]),"DataTypeOpt" );
		for (j = 0 ;j < UNRESETALM_DATATYPE_NUM ; j++)
		{
			if (FilterUnresetAlm[crt_n].fault_mask & 0x01 <<j)
			{
				gtk_option_menu_set_history( GTK_OPTION_MENU(opmenu) ,(guint)j);
				break ;
			}
		}
		
		opmenu = g_object_get_data(G_OBJECT(UnresetAlm_pop_shell[crt_n]),"TypeIdOpt" );
		for (j = 0 ;j < UNRESETALM_DATATYPE_NUM ; j++)
		{
			if (FilterUnresetAlm[crt_n].type_id_mask & 0x01 <<j)
			{
				gtk_option_menu_set_history( GTK_OPTION_MENU(opmenu) ,(guint)j);
				break ;
			}
		}
		FilterUnresetAlm[crt_n].change_flag = FALSE;
	}
	UnresetCurFilter = gtk_combo_box_get_active(pComboBox);
	FilterUnresetAlm[crt_n].is_change = TRUE;
	UnresetAlmDspProc( crt_n);
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);
	return TRUE;
}

void InitUnresetAlmFilter(FILTER_T *pFilter)
{    
	int i,station_num;
	GetStationNum(&station_num);
	if (station_num>1)
		pFilter->stn_id  = 0 ;
	else
		pFilter->stn_id  = StnDef[1].Id ;
	pFilter->stn_mask = 0xffffffff;
	pFilter->rec_type   = 0 ;
	pFilter->fault_mask   = 0xffffffff ;
	pFilter->cmd_type	=	0;
	pFilter->type_id_mask =	0xffffffff;
	for (i=0;i<MAX_DEV_MASK_NUM;i++)
	{
		pFilter->dev_mask[i] = 0xffffffff;
	}
	pFilter->datatype_mask = 0xffffffff ;
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		pFilter->group_mask[i] = 0xffffffff;
	}
	pFilter->attrib_mask[0] = 0;
	pFilter->attrib_mask[1] = 0;
	pFilter->datatype_mask = 0xffffffff ;
	pFilter->start_time  = -1 ;
	pFilter->end_time   = -1 ;
        strcpy(pFilter->search_str, "");     
        pFilter->phead =  &shm_fault_addr->head;
	pFilter->pRec   = (REC_ENTRY_UNION* )&shm_fault_addr->rec;	
	pFilter->is_change =  FALSE;	 
	pFilter->change_flag = FALSE;
}

GdkRectangle   UnrestAlarmCanasRECT[MAX_CRT_NUMBER];
void  unreset_alarm_canvas_allocate(GtkWidget *widget,GtkAllocation *a ,gpointer data) 
{
        int crt_n = (gint) data ; 
        UnrestAlarmCanasRECT[crt_n].width =  a->width ;
        UnrestAlarmCanasRECT[crt_n].height =  a->height ;
        unreset_page[crt_n].PageSize =   UnrestAlarmCanasRECT[crt_n].height/ (int)(ALM_ROW*yscale[crt_n]) ;
        unreset_page[crt_n].PageSize -- ;   
 /* g_message("alloresize ****** alarm page is %d \n", page[crt_n].PageSize);  */
        
}


gint UnresetAlmFilterInit(GtkWidget*w , gpointer data)
{
    int crt_n = (gint)data ;
 
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return  FALSE;
    InitUnresetAlmFilter(&FilterUnresetAlm[crt_n]) ;
	InitUnrestToolBarButton( UnresetAlm_pop_shell[crt_n],crt_n);
     FilterUnresetAlm[crt_n].is_change = TRUE;
	 return TRUE;
}

int	Unreset_alm_rec_filter(FILTER_T filter, UNRESET_INDEX *index)
{
	/*USHORT		i, count_p, end_p, start_p;*/
	time_t		time;
	ALM_STR		alm_str;
	BOOL		blink;
	COLOR_PAIR	color;
	POINTER		point;
	BOOL		faultPt,failPt;
	int		move_bit_num;
	USHORT		i, count_rec, start_rec;
    char		err_str[128];
    POINTER  pt ; 
	USHORT io_group;
	index->count= 0;
	   
	/*count_p= filter.phead->count;
     
	if(count_p == filter.phead->buf_size) 
		count_p= filter.phead->buf_size-1;
        
	end_p= filter.phead->header_ptr;

	start_p= (end_p+ filter.phead->buf_size-count_p)%filter.phead->buf_size;

	for(i=start_p; i!=end_p; i++, i=(i>=filter.phead->buf_size)? 0:i )*/
        count_rec= filter.phead->count;
	if(count_rec > filter.phead->buf_size)
	{
		sprintf(err_str, "Event table count overrange: %d", count_rec);
		Err_log(err_str);
		return -1;
	}

	start_rec= (filter.phead->header_ptr+ filter.phead->buf_size-count_rec)%filter.phead->buf_size;

	for(i=start_rec; count_rec>0; count_rec--,i++,i=(i>=filter.phead->buf_size)? 0:i )
	{
		if(filter.stn_id!=0)
		{
			if(filter.stn_id!= filter.pRec[i].ana_rec.stn_id)
				continue;
		}
		if  (isStnInMask(filter.pRec[i].ana_rec.stn_id)==0)  //ADDED BY CHI 2010-01-11
			continue ;  

		if(filter.dev_mask[0]!=(UINT)-1)
		{
			if(!(0x01 & (filter.dev_mask[0]>>(filter.pRec[i].ana_rec.dev_id-1))))
				continue;
		}
		
		if(GetPointFromRec(&filter.pRec[i], &pt)!=-1)	//added by dm
		{
			if (-1!=GetPtIoGroupById(&pt,&io_group))	
			{
				if (filter.group_mask[filter.pRec[i].ana_rec.dev_id-1]!=(UINT)-1)
				{
					if(!(0x01 & (filter.group_mask[filter.pRec[i].ana_rec.dev_id-1]>>(io_group-1))))
						continue;
				}
			}
		}

		if(filter.datatype_mask!=(UINT)-1)
		{
			if(!(0x01 & (filter.datatype_mask>>(filter.pRec[i].ana_rec.data_type-1))))
				continue;
		}
		
		if(filter.fault_mask!=(UINT)-1)
		{			
			point.stn_id= filter.pRec[i].ana_rec.stn_id;
			point.dev_id= filter.pRec[i].ana_rec.dev_id;
			point.data_type= filter.pRec[i].ana_rec.data_type;
			point.pt_id= filter.pRec[i].ana_rec.pt_id;	
			if(-1!=IsPtFaultById(&point, &faultPt)  )
			{	
				if(!faultPt)
				{
					IsPtFailById(&point, &failPt);
					if(!failPt)
						continue;
					else
					{
						if(!(0x01 & (filter.fault_mask>>2)))
							continue;
					}
				}
				else
				{
					
 					if(!(0x01 & (filter.fault_mask>>1)))
 						continue;
				}
			}
		}

		if(filter.type_id_mask!=(UINT)-1)
		{
			if(filter.pRec[i].ana_rec.type_id==DPS_ANA_ALM)
				move_bit_num = 1;
			else 
				move_bit_num = 2;
			if(!(0x01 & (filter.type_id_mask>>move_bit_num)))
				continue;
				
		}
		if(filter.start_time>=0)
		{
			time= GetTimefromRec(&filter.pRec[i]);
			if(-1==time)
			{
				Err_log( "Time convert failed");
				continue;
			}

			if(time < filter.start_time || time > filter.end_time)
				continue;
		}
	  	
		if(strcmp(filter.search_str,"")!= 0 )
		{
			GetAlmStrings(&filter.pRec[i], &alm_str, &blink, &color);
				 
			if(!strstr(alm_str.longname, filter.search_str))
				continue;
		}
 
		index->index_pt[index->count++]= i;
                if (index->count >= index->buff_size) 
			break;
	}

	return 0;
}
int  isInUnSel(crt_n ,iRec)
{   UINT j  ;
for  (j= 0 ; j<  un_sel_pt[crt_n].nextpos ; j++ )
{
	if (iRec == un_sel_pt[crt_n].sel[j]  ) return TRUE;
	
}
return   FALSE ; 
}


/*****************************  begin  code *****************************************/

void UnresetAlmDspProc1 ( GdkDrawable *drawable , int crt_n, int isPrint)
{
	int i,j ,X_OFFSET=0;
	ALM_STR			alarm_str ;
	BOOL			blink ;
	COLOR_PAIR		color ;
	GdkColor  bkColor ; 
	GtkWidget *labelPage ;
	static short blinkstate[MAX_CRT_NUMBER];
	char buff[20] ;	
	char record[20];
	char hostname[25];
	char msgRes[512] ;
	/*char logicName[POINT_NAME_SIZE];*/
	int ruler=0;
	FILE *fp;
	gchar *pConvert=0;
	
	DrawAlmTabHead( drawable ,UnresetAlm_draw_gc[crt_n],crt_n  );
    UnresetAlmIndex_tmp[crt_n].buff_size = MAX_FAULT_RECORD_NUM;
	 
/*
			if(FilterUnresetAlm[crt_n].is_change)
				unreset_page[crt_n].isSelected=0;*/

		  //hcl 修改为每次刷新都进入，UnresetAlmIndex_tmp[crt_n].header != shm_fault_addr->head.header_ptr并不能作为
	//报警一览表共享区修改条件
    if (1)//(UnresetAlmIndex_tmp[crt_n].header != shm_fault_addr->head.header_ptr || FilterUnresetAlm[crt_n].is_change    )  
    {         
		UnresetAlmIndex_tmp[crt_n].count = shm_fault_addr->head.count ;
		UnresetAlmIndex_tmp[crt_n].header = shm_fault_addr->head.header_ptr;
	//	printf("######UnresetAlmIndex_tmp[crt_n].header=%d，shu_fau_addr->head.header_ptr=%d \n",UnresetAlmIndex_tmp[crt_n].header,shm_fault_addr->head.header_ptr);	
		Unreset_alm_rec_filter(FilterUnresetAlm[crt_n], &UnresetAlmIndex_tmp[crt_n]);  
		
		FilterUnresetAlm[crt_n].is_change = FALSE ;
		unreset_page[crt_n].RecCount = UnresetAlmIndex_tmp[crt_n].count ;                
	} 
	
	SetPageInfo( &unreset_page[crt_n] );  
	if (IsLastPage[crt_n])
	{
		unreset_page[crt_n].CurPage = MAX(unreset_page[crt_n].PageCount -1 ,0 )   ;
		unreset_page[crt_n].PageStart =  (unreset_page[crt_n].CurPage) * (unreset_page[crt_n].PageSize) ;
		unreset_page[crt_n].PageEnd   =   MIN ((unreset_page[crt_n].PageStart + unreset_page[crt_n].PageSize -1) , (unreset_page[crt_n].RecCount-1) )   ;
	}
	else
		unreset_page[crt_n].CurPage = (unreset_page[crt_n].PageStart+ 2 )/ unreset_page[crt_n].PageSize    ;  
	
	
	SetPageInfo( &unreset_page[crt_n] );
	unreset_page[crt_n].PageEnd   =   MIN ((unreset_page[crt_n].PageStart + unreset_page[crt_n].PageSize -1) , (unreset_page[crt_n].RecCount-1) )   ;
	SetAdjInfo( &unreset_page[crt_n],unreset_adj[crt_n] );   
	
	labelPage = gtk_object_get_data(GTK_OBJECT(UnresetAlm_pop_shell[crt_n]),"labelPage");
	
	sprintf(buff , "%s%d/%d ", pConvert=_toUtf8EX(OIX_CSTR_PAGE_NUM), (unreset_page[crt_n].CurPage+1>unreset_page[crt_n].PageCount?unreset_page[crt_n].PageCount:unreset_page[crt_n].CurPage+1),unreset_page[crt_n].PageCount);
	G_SAFE_FREE(pConvert);
	if (labelPage != NULL)
		gtk_label_set_text(GTK_LABEL(labelPage), buff);
#ifdef 	DEBUG
	printf ("\n pagestart[%d]:%d \t  pageEnd: %d  \n",crt_n, unreset_page[crt_n].PageStart ,unreset_page[crt_n].PageEnd );   
#endif 
	/*add by hcl*/
	if (isPrint)
	{
		fp = fopen(UnresetPrintFile,"w");
		if (fp ) 
		{
// #ifdef SUN_OS
// 			fprintf(fp,"%s          %s        %s        %s          \n", 
// 				UnresetAlarmHeadTitle[0],UnresetAlarmHeadTitle[1],UnresetAlarmHeadTitle[2],UnresetAlarmHeadTitle[3]);
// #endif
			
			//初始化标尺 
			ruler=0;
			while(ruler<PRINT_RULER_LEN)
			{
				PrintLocationRuler[ruler]=' ';
				ruler++;
			}
			PrintLocationRuler[ruler]='\0';					
			
			for (i=0,j=0;i<=PRINTELEMENT;i++)
			{	
				strncpy(PrintLocationRuler+PrintTitleLocation[j], _CS_(UnresetAlarmHeadTitle[i]), strlen(_CS_(UnresetAlarmHeadTitle[i])));	    	
				j++;
			}
			/*printf("PrintLocationRuler:%s** \n",PrintLocationRuler);*/
			fprintf(fp,"%s \n",PrintLocationRuler);

			
			
		}   /* if(fp)end  */
	}/*if printf end*/
	
	for (i = unreset_page[crt_n].PageStart , j= 0 ; i <=  unreset_page[crt_n].PageEnd ;j++, i++)
	{ 
		/* printf("#### index_pt[%d]=%d\n",i,UnresetAlmIndex_tmp[crt_n].index_pt[i]);*/
		
		
		
		if (i%2) 
		{
			gdk_rgb(20,20,20,&bkColor )  ;         
		}else
			gdk_rgb( 25,30,30,&bkColor ) ; 
			
		
		GetAlmStrings(&shm_fault_addr->rec[UnresetAlmIndex_tmp[crt_n].index_pt[i]], &alarm_str , &blink , &color );       
		
		
		/***************************************************************************************************/
		
		sprintf(record , "%s: ",  alarm_str.sn_str);
		strcat(record , alarm_str.repeatAlm);	
		
		if (strcmp(alarm_str.usrname,"")!=0)
			sprintf(hostname , "%s@%s", alarm_str.usrname, alarm_str.hostname);
		else
			sprintf(hostname , "%s",   alarm_str.hostname);
		
		
		if ((unreset_page[crt_n].isSelected)&& isInUnSel(crt_n , UnresetAlmIndex_tmp[crt_n].index_pt[i]) )
		{
			gdk_rgb( 150,150,200,&bkColor )  ;
			gdk_gc_set_foreground ( UnresetAlm_draw_gc[crt_n], &bkColor);
			gdk_draw_rectangle(drawable,UnresetAlm_draw_gc[crt_n],1,(int)(ALM_X0*xscale[crt_n]),(int)((FAULT_ALM_Y1 + (j+1) * ALM_ROW)*yscale[crt_n]-10),(int)(1200*xscale[crt_n]),(int)((ALM_ROW-4)*yscale[crt_n])); 
			
// 			if (isPrint && fp)  
// 			{
// #ifdef SUN_OS
// 				fprintf(fp,"%s %s \t  %s \t %s \t %s  \t%s \t %s  \n",record ,alarm_str.time_str,alarm_str.longname,alarm_str.cmd_result ,alarm_str.message,alarm_str.state_str , hostname);
// #endif 
// 				
// #ifdef WINDOWS_OS
// 				
// 				//初始化标尺 
// 				ruler=0;
// 				while(ruler<PRINT_RULER_LEN)
// 				{
// 					PrintLocationRuler[ruler]=' ';
// 					ruler++;
// 				}
// 				PrintLocationRuler[ruler]='\0';
// 				strncpy(PrintLocationRuler+PrintElementLocation[0], record, strlen(record));			
// 				strncpy(PrintLocationRuler+PrintElementLocation[1], alarm_str.time_str+2, strlen(alarm_str.time_str)-2);				
// 				strncpy(PrintLocationRuler+PrintElementLocation[2], alarm_str.logicname, strlen(alarm_str.logicname));
// 				if(alarm_str.message[0]==0)
// 				{
// 					strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.longname, strlen(alarm_str.longname));					
// 				}
// 				else
// 				{
// 					strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.message , strlen(alarm_str.message ));										
// 				}
// 				strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.longname, strlen(alarm_str.longname));		
// 				strncpy(PrintLocationRuler+PrintElementLocation[4], alarm_str.state_str, strlen(alarm_str.state_str));								
// 				strncpy(PrintLocationRuler+PrintElementLocation[5], alarm_str.cmd_result, strlen(alarm_str.cmd_result));				
// 				strncpy(PrintLocationRuler+PrintElementLocation[6], hostname, strlen(hostname));
// 				
// 				fprintf(fp,"%s '\n'",PrintLocationRuler);
// 				
// #endif
// 			}
		}
		else
		{
			gdk_gc_set_foreground ( UnresetAlm_draw_gc[crt_n], &bkColor);           
			gdk_draw_rectangle(drawable,UnresetAlm_draw_gc[crt_n],1,(int)(ALM_X0*xscale[crt_n]),(int)((FAULT_ALM_Y1 + (j+1) * ALM_ROW)*yscale[crt_n]-10),(int)(1200*xscale[crt_n]),(int)(ALM_ROW*yscale[crt_n])); 
		}

		if (isPrint && fp)  
		{
// #ifdef SUN_OS
// 			fprintf(fp,"%s %s \t  %s \t %s \t %s  \t%s \t %s  \n",record ,alarm_str.time_str,alarm_str.longname,alarm_str.cmd_result ,alarm_str.message,alarm_str.state_str , hostname);
// #endif 
			
			
			
			//初始化标尺 
			ruler=0;
			while(ruler<PRINT_RULER_LEN)
			{
				PrintLocationRuler[ruler]=' ';
				ruler++;
			}
			PrintLocationRuler[ruler]='\0';
			strncpy(PrintLocationRuler+PrintElementLocation[0], record, strlen(record));			
			strncpy(PrintLocationRuler+PrintElementLocation[1], alarm_str.time_str, strlen(alarm_str.time_str));				
			strncpy(PrintLocationRuler+PrintElementLocation[2], alarm_str.logicname, strlen(alarm_str.logicname));
			if(alarm_str.message[0]==0)
			{
				strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.longname, strlen(alarm_str.longname));					
			}
			else
			{
				strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.message , strlen(alarm_str.message ));										
			}
			strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.longname, strlen(alarm_str.longname));		
			strncpy(PrintLocationRuler+PrintElementLocation[4], alarm_str.state_str, strlen(alarm_str.state_str));								
			strncpy(PrintLocationRuler+PrintElementLocation[5], alarm_str.cmd_result, strlen(alarm_str.cmd_result));				
			strncpy(PrintLocationRuler+PrintElementLocation[6], hostname, strlen(hostname));
			
			fprintf(fp,"%s '\n'",PrintLocationRuler);
			

			}
		/***************************************************************************************************/
				
		
		if (blink)		
			gdk_gc_set_foreground ( UnresetAlm_draw_gc[crt_n], &pixels[crt_n][ color.clr[blinkstate[crt_n]]]);		
		else
			gdk_gc_set_foreground ( UnresetAlm_draw_gc[crt_n], &pixels[crt_n][color.clr[0]]);                
		
		if (blink)
			windk_draw_text( drawable, font_set[0], UnresetAlm_draw_gc[crt_n],  ALM_X0-20 , ALM_Y1 + j * ALM_ROW , "★" , strlen("★"),crt_n );
		
		gdk_gc_set_foreground ( UnresetAlm_draw_gc[crt_n], &pixels[crt_n][color.clr[0]]);
		
/*
				sprintf(record , "%s: ",  alarm_str.sn_str);
				strcat(record , alarm_str.repeatAlm);*/
		
		windk_draw_text( drawable, font_set[0], UnresetAlm_draw_gc[crt_n],  ALM_X0, ALM_Y1 + j * ALM_ROW , record ,strlen(record),crt_n);
		windk_draw_text( drawable, font_set[0], UnresetAlm_draw_gc[crt_n],  ALM_X1, ALM_Y1 + j * ALM_ROW , alarm_str.time_str ,strlen(alarm_str.time_str) ,crt_n);
		if (OIX_EVENT_DSP_LOGICNAME )
		{
			X_OFFSET  = 180 ; 
            windk_draw_text( drawable, font_set[0], UnresetAlm_draw_gc[crt_n],  ALM_X2, ALM_Y1 + j * ALM_ROW , alarm_str.logicname , strlen(alarm_str.logicname),crt_n); 
		}
		sprintf(msgRes,"%s   %s" ,alarm_str.longname   , alarm_str.state_str  ) ; 
		if (OIX_EVENT_ALIAN_MOD)  //需要命令状态对齐；
		{
			windk_draw_text( drawable, font_set[2], UnresetAlm_draw_gc[crt_n],  ALM_X3+X_OFFSET, ALM_Y1 + j * ALM_ROW ,   alarm_str.longname  ,strlen(alarm_str.longname),crt_n);
			windk_draw_text( drawable, font_set[0], UnresetAlm_draw_gc[crt_n],  ALM_X4+X_OFFSET, ALM_Y1 + j * ALM_ROW , alarm_str.state_str ,strlen(alarm_str.state_str),crt_n);  
	
		}
		else 
			windk_draw_text( drawable, font_set[0], UnresetAlm_draw_gc[crt_n],  ALM_X3+30, ALM_Y1 + j * ALM_ROW , msgRes/*alarm_str.longname*/ ,strlen( alarm_str.longname),crt_n);
        windk_draw_text( drawable, font_set[0], UnresetAlm_draw_gc[crt_n],  ALM_X3 + 500, ALM_Y1 + j * ALM_ROW , alarm_str.cmd_result, strlen(alarm_str.cmd_result),crt_n);
        windk_draw_text( drawable, font_set[2], UnresetAlm_draw_gc[crt_n],  ALM_X3, ALM_Y1 + j * ALM_ROW , alarm_str.message ,strlen( alarm_str.message),crt_n);
		windk_draw_text( drawable, font_set[0], UnresetAlm_draw_gc[crt_n],  ALM_X5, ALM_Y1 + j * ALM_ROW , hostname , strlen(hostname),crt_n);		
	}/*for end*/
	
	if (isPrint && fp) fclose(fp);
	
	if   (blinkstate[crt_n] >0)
		blinkstate[crt_n] = 0 ;
	else
		blinkstate[crt_n] = 1 ;
#ifdef 	DEBUG
	g_message("FilterUnresetAlm[%d].is_change:%d ,UnresetAlmIndex_tmp[%d].count :%d \n",crt_n ,  FilterUnresetAlm[crt_n].is_change , crt_n  ,UnresetAlmIndex_tmp[crt_n].count);  
#endif	
	
}

gint UnresetAutoFilterProc(int crt_n)
{
	int i,j,k;
	char    unit_state_str[POINT_NAME_SIZE+12];
	for(i=1;i<=UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev_num;i++)
	{
		for (j=1;j<=UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev[i].group_num;j++)
		{	
			for (k=0;k<MAX_UNIT_STATUS_NUM;k++)
			{
				sprintf(unit_state_str,"%s.%s.%s%d.",UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].Name,UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev[i].Name,"AUTO.FILTER.STATE",k);
				if (InterLock_Judge(unit_state_str)==1)
				{
					if (UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev[i].group[j].Status & (0X1<<k))
					{
						UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = FALSE;
						FilterUnresetAlm[crt_n].group_mask[i-1] = FilterUnresetAlm[crt_n].group_mask[i-1] | (1<<(j-1));
					}
					else
					{
						UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = TRUE;
						FilterUnresetAlm[crt_n].group_mask[i-1] = FilterUnresetAlm[crt_n].group_mask[i-1] & (~(1<<(j-1)));
					}
					FilterUnresetAlm[crt_n].is_change = TRUE;
					UnresetAlmDspProc(crt_n);
				}
			}
		}
	}
	return TRUE;
}

/****************  Timer  call  !!!      ************/
gint  UnresetAlmDspProc(int crt_n )
{      
	GdkColor clr ;
	GtkWidget  *scroll ; 
	gdk_rgb(20,20,20,&clr);
	 gdk_gc_set_foreground(UnresetAlm_draw_gc[crt_n], &clr);
      gdk_draw_rectangle (UnresetAlmOffDrawable[crt_n],UnresetAlm_draw_gc[crt_n],TRUE, 0,0,LOCAL_WIDTH, 1600); 
	 UnresetAlmDspProc1( UnresetAlmOffDrawable[crt_n] ,crt_n,FALSE);
	 gdk_window_copy_area (UnresetAlm_canvas[crt_n]->window,UnresetAlm_draw_gc[crt_n],0,0,UnresetAlmOffDrawable[crt_n],0,0,LOCAL_WIDTH, 1600);
	 scroll = g_object_get_data(G_OBJECT(UnresetAlm_pop_shell[crt_n]),"scrollbar" );
	 gtk_widget_queue_draw(scroll);
	 
	 return TRUE;    	
}


void  UnresetAlm_expose(GtkWidget *w ,  GdkEventExpose *event, gpointer data)  
{ 
    	int crt_n = (gint ) data;
       
printf("^^^crt_n=%d\n",crt_n);
     	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) 
     		return ;
	 
  	UnresetAlmDspProc( crt_n);
        
}

gint Prev( GtkWidget *w , gpointer data)
{
	gint crt_n = (gint )data;
 	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) 
 		return FALSE;

	unreset_page[crt_n].CurPage -- ;
        if (unreset_page[crt_n].CurPage < 0  )  
    	{
		gdk_beep(    ); 
 		unreset_page[crt_n].CurPage  = 0 ;
                return TRUE; 
	}
	SetPageInfo (&unreset_page[crt_n]);
        
        unreset_page[crt_n].PageStart =  (unreset_page[crt_n].CurPage) * (unreset_page[crt_n].PageSize) ;
        unreset_page[crt_n].PageEnd   =   MIN ((unreset_page[crt_n].PageStart + unreset_page[crt_n].PageSize -1) , (unreset_page[crt_n].RecCount-1) )   ;
        SetAdjInfo(&unreset_page[crt_n],unreset_adj[crt_n]);
	
        IsLastPage[crt_n] = FALSE;
     	UnresetAlmDspProc( crt_n );

		return TRUE;
}

gint Next( GtkWidget *w ,    gpointer data)
{
	gint crt_n = (gint )data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return FALSE;

	unreset_page[crt_n].CurPage ++ ;
        if ( unreset_page[crt_n].CurPage > MAX( unreset_page[crt_n].PageCount -1   ,0)) 
	{
		gdk_beep(    ); 
 		unreset_page[crt_n].CurPage  = unreset_page[crt_n].PageCount  -1 ;
                return TRUE;
	} 
	SetPageInfo (&unreset_page[crt_n]);
        unreset_page[crt_n].PageStart =  (unreset_page[crt_n].CurPage) * (unreset_page[crt_n].PageSize) ;
        unreset_page[crt_n].PageEnd   =   MIN ((unreset_page[crt_n].PageStart + unreset_page[crt_n].PageSize -1) , (unreset_page[crt_n].RecCount-1) )   ;
        SetAdjInfo(&unreset_page[crt_n],unreset_adj[crt_n]);
	
        IsLastPage[crt_n] = FALSE;
	UnresetAlmDspProc( crt_n );

	return TRUE;
}

void  unreset_scroll_page(GtkAdjustment *adj , int crt_n)
{
    /*printf ("Unresetadj.value = %d,adj.upper = %8.3f,adj.page_increment = %8.3f,crt = %d\n",(int)adj->value,adj->upper,adj->page_increment,crt_n);
    */unreset_page[crt_n].PageStart = (int)adj->value  ; 
    unreset_page[crt_n].PageEnd   =  MIN ((unreset_page[crt_n].PageStart + unreset_page[crt_n].PageSize -1) , (unreset_page[crt_n].RecCount-1) )   ;
 /*printf("****in unreset_scroll_page,pageStart=%d,pageEnd=%d\n",unreset_page[crt_n].PageStart,unreset_page[crt_n].PageEnd);
   */ UnresetAlmDspProc( crt_n);
}

void UnresetAlm_draw_area_realize(GtkWidget *widget,gpointer data)
{
         int crt_n = (gint)data ;
	
         if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
        UnresetAlm_canvas[crt_n] = widget ;
        UnresetAlm_draw_gc[crt_n] = gdk_gc_new(widget->window);
 	UnresetAlmOffDrawable[crt_n] = gdk_pixmap_new(widget->window,LOCAL_WIDTH, 1600,-1);
	gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]);   
       
}

gint UnresetAlmFormQuit(GtkWidget *widget,   gpointer data)
{
      int crt_n = (gint) data ;
   
     if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE; 
     if(unresetalm_timeout_id[crt_n]!=0)
	 {
		gtk_timeout_remove(unresetalm_timeout_id[crt_n]);
		unresetalm_timeout_id[crt_n]= 0;
	 }	
	 if (Unreset_autofilter_timeout_id[crt_n]!=0)
	 {
		 gtk_timeout_remove(Unreset_autofilter_timeout_id[crt_n]);
		 Unreset_autofilter_timeout_id[crt_n] = 0;
	 }
 	g_object_unref (UnresetAlmOffDrawable[crt_n]); 	
       
	gtk_widget_destroy(GTK_WIDGET(UnresetAlm_pop_shell[crt_n])); 
        UnresetAlm_pop_shell[crt_n] = (GtkWidget *)NULL ;
         UnresetAlmfilterDlg[crt_n] = (GtkWidget *)NULL ;   
		 
		 return TRUE;
}

gint CreateDevFilter(GtkWidget *widget,   gpointer data)
{	
	int crt_n   =   0;
	int realdata;
	int i;
 
	//	GetCrtNumberOfWgt (widget, &crt_n );
	crt_n		=	getCrtNumFromPointer	((int)data);
	realdata	=	getDataFromPointer((int)data);
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	 
	FilterUnresetAlm[crt_n].stn_id = (gint)realdata ;
	FilterUnresetAlm[crt_n].is_change = TRUE;
	FilterUnresetAlm[crt_n].change_flag = TRUE;
	for (i=0;i<MAX_DEV_MASK_NUM;i++)
	{
		FilterUnresetAlm[crt_n].dev_mask[i] = 0xffffffff;
	}
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		FilterUnresetAlm[crt_n].group_mask[i] = 0xffffffff;
	}
	UnresetAlmDspProc(  crt_n );

#ifdef DEBUG	 
 	printf ("cFilterUnresetAlm[%d].stn_id = %d \n",   crt_n ,FilterUnresetAlm[crt_n].stn_id );
#endif  

	return TRUE;
}


gint SetUnresetAlmDevMask(GtkWidget       *widget,   gpointer     data)
{   int i = (gint) data ;
    int crt_n ,j ;
   
    GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 )
		return FALSE;
    
    if (i == SHOW_ALL)  /* show all !!!*/
        {
          for (j = 1 ; j <= UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev_num ; j++)
              {
                  if (GTK_IS_TOGGLE_BUTTON (Unresetitem[crt_n][j]))
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Unresetitem[crt_n][j]), FALSE);
          }
         
    }else if (i ==SHOW_NONE) /*  show none !!!*/
        {
             for (j = 1 ; j <= UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev_num ; j++)
              { if (GTK_IS_TOGGLE_BUTTON (Unresetitem[crt_n][j]))
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Unresetitem[crt_n][j]), TRUE); 
  /*    printf ("j ========== %d\n",j );   */              
                    
          }
        
    }else
        {
    
    
            int  bool = gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget));
            if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget)))
                       UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  | (1<<(i-1) )   ;
            else
                      UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  & (~(1<<(i-1))) ;   /*  Clear bit N */   

        #ifdef DEBUG
                g_message("toggle[%d] is %d UnresetDevMask: %x \n" ,i, bool ,UnresetDevMask[crt_n] );
        #endif 	
    }
    
/*    
    int  bool = gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget));
    if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget)))
               UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  | (1<<i-1 )   ;
    else
              UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  & (~(1<<i-1)) ;   

#ifdef DEBUG
	g_message("toggle[%d] is %d UnresetDevMask: %x \n" ,i, bool ,UnresetDevMask[crt_n] );
#endif 	

*/
	return TRUE; 
}

gint SetUnresetAlmfaultMask(GtkWidget       *widget,   gpointer     data)
{   
	int i  ;
	int crt_n , j;
	
	//GetCrtNumberOfWgt (widget, &crt_n );
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	//i= (gint) data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	if(i==0)/* All*/
	{
		for(j=0;j<UNRESETALM_DATATYPE_NUM ; j++ )
			FilterUnresetAlm[crt_n].fault_mask = (FilterUnresetAlm[crt_n].fault_mask)  | (1<<j) ;  /*  Set bit N */
	}
	else
	{
	 	for(j=0;j<UNRESETALM_DATATYPE_NUM ; j++ )
		 	FilterUnresetAlm[crt_n].fault_mask = (FilterUnresetAlm[crt_n].fault_mask)  & (~(1<<j)) ;  /*  Clear bit N */

		FilterUnresetAlm[crt_n].fault_mask = (FilterUnresetAlm[crt_n].fault_mask)  | (1<<i) ;  /*  Set bit N */ 
	}
	
	FilterUnresetAlm[crt_n].is_change = TRUE;
	FilterUnresetAlm[crt_n].change_flag = TRUE;

#ifdef  DEBUG 
    g_message("FilterUnresetAlm[crt_n].datatype_mask is %x  " ,FilterUnresetAlm[crt_n].datatype_mask );
    g_message("you pressed button %d " ,i);
#endif 

	return  TRUE;	
}


gint SetUnresetAlmTypeIdMask(GtkWidget       *widget,   gpointer     data)
{   
	int i  ;
	int crt_n ,j;
	//GetCrtNumberOfWgt (widget, &crt_n );
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	//i= (gint) data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	if(i==0)/* All*/
	{
		for(j=0;j<UNRESETALM_DATATYPE_NUM ; j++ )
			FilterUnresetAlm[crt_n].type_id_mask = (FilterUnresetAlm[crt_n].type_id_mask)  | (1<<j) ;  /*  Set bit N */
	}
	else
	{
	 	for(j=0;j<UNRESETALM_DATATYPE_NUM ; j++ )
		 	FilterUnresetAlm[crt_n].type_id_mask = (FilterUnresetAlm[crt_n].type_id_mask)  & (~(1<<j)) ;  /*  Clear bit N */

		FilterUnresetAlm[crt_n].type_id_mask = (FilterUnresetAlm[crt_n].type_id_mask)  | (1<<i) ;  /*  Set bit N */ 
	}
	
	FilterUnresetAlm[crt_n].is_change = TRUE;
	FilterUnresetAlm[crt_n].change_flag = TRUE;

#ifdef  DEBUG 
    g_message("FilterUnresetAlm[crt_n].type_id_mask is %x  " ,FilterUnresetAlm[crt_n].type_id_mask );
    g_message("you pressed button %d " ,i);
#endif 

	return  TRUE;	
}

gint 
UnresetAlmdevBtnPress(GtkWidget       *widget,  gpointer     data)
{   


  
	gint crt_n = (gint) data ;

	if (UnresetAlmfilterDlg[crt_n] == (GtkWidget *)NULL)
	{
    		UnresetAlmfilterDlg[crt_n] = CreateDevFilterDlgUnset( UnresetAlm_pop_shell[crt_n] ,crt_n);
    		gtk_widget_show(UnresetAlmfilterDlg[crt_n]);
	}
	else
    		gtk_window_present (GTK_WINDOW(UnresetAlmfilterDlg[crt_n]));

	return TRUE;
	if (FilterUnresetAlm[crt_n].stn_id == 0) 
	{
		printf("###Please select station\n");
		return FALSE;
	}
	

	return TRUE;
}
void  Unreset_alarm_motion(GtkWidget *w ,GdkEventMotion *event,   gpointer data)
{
    int crt_n =(int) data ; 
    
    
    if(   !(event->state & GDK_BUTTON1_MASK))
        return  ;
     unreset_page[crt_n].mycur.x2 = (int)event->x ; unreset_page[crt_n].mycur.y2 = (int)event->y ;
   
         
}
void 	ProgUnresetAlarmTab (GtkWidget *parent,	int crt_n   )
{
   
	if (UnresetAlm_pop_shell[crt_n]==(GtkWidget*)NULL)
        {
             UnresetAlm_pop_shell[crt_n] = CreateUnresetAlarmTab ( parent, crt_n   ) ;
             gtk_window_set_position(GTK_WINDOW(UnresetAlm_pop_shell[crt_n]),GTK_WIN_POS_CENTER_ON_PARENT);
             gtk_widget_show(UnresetAlm_pop_shell[crt_n]);
             
             unreset_page[crt_n].CurPage = 0;
             IsLastPage[crt_n] = FALSE;/*TRUE;*/
            InitUnresetPageInfo(&unreset_page[crt_n] , crt_n) ;
             SetAdjInfo( &unreset_page[crt_n],unreset_adj[crt_n]) ; /* moved by zyp on 2006.9.6*/
	     InitUnresetAlmFilter( &FilterUnresetAlm[crt_n]);
			FilterUnresetAlm[crt_n].is_change = TRUE; 
	     if (unresetalm_timeout_id[crt_n] != 0 )
	    {
		 	gtk_timeout_remove(unresetalm_timeout_id[crt_n]);
			unresetalm_timeout_id[crt_n]= 0;
	    }
            		unresetalm_timeout_id[crt_n]=gtk_timeout_add ( 1000,(GtkFunction)UnresetAlmDspProc, (gpointer)crt_n ); 
        }
	else       
            gtk_window_present(GTK_WINDOW(UnresetAlm_pop_shell[crt_n]));       
}



void  Unreset_alarm_button_release( GtkWidget *w ,GdkEvent *event  , gpointer data)
{
    int  crt_n = (gint)  data ;
    int i,j,S,E;     
    gint rowNum ,height ; 
    if (event->type == GDK_BUTTON_RELEASE) {   
        GdkEventButton *bevent = (GdkEventButton *) event; 
        if (bevent->button == 1)   
        {	            
                height = windk_text_height(font_set[DBTAB_FONT] ,"娑", strlen("娑"),FONT_SIZE_ALARM);
                //rowNum = ( MIN(unreset_page[crt_n].mycur.y1,unreset_page[crt_n].mycur.y2) - ALM_Y1 +  height) / ALM_ROW   ;
				rowNum = ( MIN(unreset_page[crt_n].mycur.y1,unreset_page[crt_n].mycur.y2) - (int)((ALM_Y1 -  height)*yscale[crt_n])) / (int)(ALM_ROW*yscale[crt_n] )  ;
                rowNum = MAX(rowNum ,0);
                unreset_page[crt_n].isSelected  =  1 ;   
                unreset_page[crt_n].SelectStart  = rowNum + unreset_page[crt_n].PageStart ; 
                S=rowNum+unreset_page[crt_n].PageStart ; 
				
				rowNum = (  MAX(unreset_page[crt_n].mycur.y1,unreset_page[crt_n].mycur.y2) - ALM_Y1 +  height) / ALM_ROW   ;
				rowNum = (  MAX(unreset_page[crt_n].mycur.y1,unreset_page[crt_n].mycur.y2) - (int)((ALM_Y1 -  height)*yscale[crt_n])) / (int)(ALM_ROW*yscale[crt_n] )  ;
                unreset_page[crt_n].SelectEnd  = MIN((rowNum + unreset_page[crt_n].PageStart),unreset_page[crt_n].PageEnd) ; 
				E=MIN((rowNum + unreset_page[crt_n].PageStart),unreset_page[crt_n].PageEnd) ;

				un_sel_pt[crt_n].nextpos = 0  ;
				for (i = S ,j =0 ; i<=E ; j ++ , i++ )
				{
					un_sel_pt[crt_n].sel[j] = UnresetAlmIndex_tmp[crt_n].index_pt[i] ;
					un_sel_pt[crt_n].nextpos++ ; 
					 printf ("_____*******sel = %d ,nextpos = %d \n" ,un_sel_pt[crt_n].sel[j] , un_sel_pt[crt_n].nextpos) ;  
				}
				printf ("+++++++++++rownum = %d , selectstart = %d , selectend = %d ,S = %d ,E = %d \n" ,
				rowNum,unreset_page[crt_n].SelectStart ,unreset_page[crt_n].SelectEnd,S,E ) ;
				


				UnresetAlmDspProc(crt_n) ;    
 		 
     } 
            
   }
}

extern DB_TAB_WINDOW_INFO db_tab_window_info[MAX_CRT_NUMBER];

gint UnresetAlm_button_press( GtkWidget *w ,GdkEvent *event  , gpointer data)
{
    gint  crt_n = (gint)  data ;
    POINTER  dbpt ;
    extern POINTER    db_selected_ptr[];
    gint rowNum ,height ,count; 
    int x,y,tmp_id;
    
    if (event->type == GDK_BUTTON_PRESS) 
    {  
        GdkEventButton *bevent = (GdkEventButton *) event; 
        x = (int)(bevent->x) ;
        y = (int)(bevent->y) ;
        if (bevent->button == 1) /* Left MB is pressed*/ 
        {       
                Unreset_print_select_tag[crt_n]++;
                if(Unreset_print_select_tag[crt_n]>2)
                    {
                    Unreset_print_select_tag[crt_n]=0;
                    Unreset_select_start_id=Unreset_select_end_id=0;
                }
 
                if(Unreset_print_select_tag[crt_n]==1)
                    {   
                        printf("print_select_tag[crt_n]==1\n");
                    Unreset_select_start_id = MIN(UnresetAlmIndex_tmp[crt_n].count-1,unreset_page[crt_n].CurPage*ALM_PAGE_SIZE+(y-ALM_Y0+ALM_ROW)/ALM_ROW);                  
                }
                else if(Unreset_print_select_tag[crt_n]==2)
                    {
                       printf("log_cnt=%d\n",UnresetAlmIndex_tmp[crt_n].count); 
                    Unreset_select_end_id = MIN(UnresetAlmIndex_tmp[crt_n].count-1,unreset_page[crt_n].CurPage*ALM_PAGE_SIZE+(y-ALM_Y0+ALM_ROW)/ALM_ROW);                    
                }
      
                if(Unreset_select_end_id<Unreset_select_start_id&&Unreset_print_select_tag[crt_n]==2)
                    {
                       tmp_id= Unreset_select_end_id;
                       Unreset_select_end_id= Unreset_select_start_id;
                       Unreset_select_start_id= tmp_id;
                }
				unreset_page[crt_n].mycur.x1 = unreset_page[crt_n].mycur.x2 = (int) (bevent->x);
                unreset_page[crt_n].mycur.y1 = unreset_page[crt_n].mycur.y2 =  (int)(bevent->y);   
                       
		}
        else if (bevent->button == 3) /* right button is pressed*/ 
        {            
				/*height = gdk_text_height(font_set[DBTAB_FONT] ,"娑??", strlen("娑??"));
		 
                rowNum = ( bevent->y - ALM_Y1 +  height) / ALM_ROW  + 1 ;  */
                height = windk_text_height(font_set[DBTAB_FONT] ,"娑", strlen("娑"),FONT_SIZE_ALARM);		 
			    rowNum =(int) (( bevent->y - (int)((ALM_Y1 -  height)*yscale[crt_n])) / (int)(ALM_ROW*yscale[crt_n])  + 1 );

                //count  = rowNum + unreset_page[crt_n].CurPage * unreset_page[crt_n].PageSize ; 
                count  =rowNum + unreset_page[crt_n].PageStart ;
				//unreset_page[crt_n].SelectStart  = rowNum + unreset_page[crt_n].PageStart ;
				
				
				printf("rowNum=%d,count=%d\n",rowNum,count);
                if ( count   > UnresetAlmIndex_tmp[crt_n].count ||  count <=  unreset_page[crt_n].PageStart )
                      return 0 ;
			   unreset_page[crt_n].SelectStart = unreset_page[crt_n].SelectEnd = count-1 ;
               unreset_page[crt_n].isSelected = 1 ; 

                if(-1==GetPointFromRec(&shm_fault_addr->rec[UnresetAlmIndex_tmp[crt_n].index_pt[count -1 ]],&dbpt))
                      return 0 ;
                db_tab_window_info[crt_n].select_pt  =  dbpt ;
                printf("dbpt.stn_id=%d,dev_id=%d,data_type=%d,pt_id=%d\n",dbpt.stn_id,dbpt.dev_id,dbpt.data_type,dbpt.pt_id);
                
				/***hcl*****************/				
				un_sel_pt[crt_n].sel[0] = UnresetAlmIndex_tmp[crt_n].index_pt[count-1];			
				un_sel_pt[crt_n].nextpos =1  ; 
				unreset_page[crt_n].isSelected = 1 ; 
				/*********************/
				
				
				PopupDetailInfoBox(UnresetAlm_pop_shell[crt_n], crt_n, db_tab_window_info[crt_n].select_pt);
				UnresetAlmDspProc( crt_n);
#ifdef  DEBUG               
  printf ("You select row :%d && count : %d \n" , rowNum, shm_fault_addr->rec[UnresetAlmIndex_tmp[crt_n].index_pt[count -1 ]].ana_rec.rec_sn) ;
  printf ("\nPt.stn_id:%d,Pt.dev_id:%d,Pt.data_type:%d,Pt.pt_id:%d\n",
                dbpt.stn_id,dbpt.dev_id,dbpt.data_type,dbpt.pt_id);                 
#endif 
        }
        return FALSE;
    } 
  return FALSE;
}

/***************************************************************************

     create a main alarm windows ; 
     entrance of process of alarm ;

****************************************************************************/
static  GtkWidget*
CreateUnresetAlarmTab (GtkWidget *parent,	int crt_n   )
{
	GtkWidget /**scrolled_window,*/*draw_area ,*window;
	GtkWidget *vbox, *hbox  , *hbox4;
	GtkWidget  *toolbar,*scrollbar; 
	gchar *pConvert=0;
/*
    gtk_set_locale();
    gtk_rc_add_default_file(GTK_RC_FILE);*/
					
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(window),GTK_WINDOW(parent));  
 
	
 	gtk_window_set_default_size(GTK_WINDOW(window),(int)(ALM_CANVAS_WIDTH*xscale[crt_n]), (int)(ALM_CANVAS_HEIGHT*yscale[crt_n]));
   	
	gtk_window_set_title(GTK_WINDOW(window), pConvert=_toUtf8EX(OIX_CSTR_UNRESET_ALM_TAB));  
	G_SAFE_FREE(pConvert);
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE); 
    
#ifdef WINDOWS_OS
	gtk_window_move(GTK_WINDOW(window) , 
		ALM_CANVAS_X_OFFSET+LOCAL_WIDTH*crt_n,(int)(500*yscale[crt_n])); 
#endif   	
#ifdef SUN_OS
	gtk_window_move(GTK_WINDOW(window) , 
		ALM_CANVAS_X_OFFSET,(int)(500*yscale[crt_n])); 
#endif


    vbox=gtk_vbox_new(FALSE, 5);

    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);
  
	//hcl
    toolbar =  CreateUnresetAlmToolbar(window,crt_n);
	
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
//gtk_box_pack_start(GTK_BOX(vbox), toolbar, TRUE, TRUE, 0);
    gtk_widget_show(toolbar);
    	gtk_signal_connect(GTK_OBJECT(window),  "destroy", GTK_SIGNAL_FUNC(UnresetAlmFormQuit),(gpointer)crt_n);   
    
          
    draw_area = gtk_drawing_area_new();
    hbox4 =  gtk_hbox_new(FALSE, 0);
    gtk_widget_show(hbox4); 
    gtk_box_pack_start(GTK_BOX(vbox), hbox4, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox4), draw_area, TRUE, TRUE, 0);
    unreset_adj[crt_n] = (GtkAdjustment* )gtk_adjustment_new(0.0,0.0,101.0,0.1,1.0,1.0);
    scrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(unreset_adj[crt_n]));
    gtk_widget_show(scrollbar)   ;
    g_object_set_data(G_OBJECT(window),"scrollbar",scrollbar);
    gtk_box_pack_start(GTK_BOX(hbox4), scrollbar, FALSE, FALSE, 0);
	//gtk_widget_set_usize(draw_area, (int)(ALM_CANVAS_WIDTH*xscale[crt_n]), (int)(ALM_CANVAS_HEIGHT *yscale[crt_n]));
    g_signal_connect(G_OBJECT(unreset_adj[crt_n]),"value_changed",G_CALLBACK(unreset_scroll_page),GINT_TO_POINTER(crt_n));
 
	gtk_widget_set_events(draw_area, GDK_EXPOSURE_MASK 
			 | GDK_SCROLL_MASK
  			 | GDK_LEAVE_NOTIFY_MASK
			 | GDK_BUTTON_PRESS_MASK
			 | GDK_BUTTON_RELEASE_MASK
			 | GDK_POINTER_MOTION_MASK );
			 
       
	gtk_signal_connect (GTK_OBJECT (draw_area),"scroll-event",GTK_SIGNAL_FUNC(unreset_scroll_event),GINT_TO_POINTER(crt_n)) ;    
	gtk_signal_connect (GTK_OBJECT (draw_area),"expose_event",GTK_SIGNAL_FUNC(UnresetAlm_expose),(gpointer)crt_n) ;
    gtk_signal_connect (GTK_OBJECT (draw_area),"motion_notify_event",GTK_SIGNAL_FUNC(Unreset_alarm_motion),GINT_TO_POINTER(crt_n)) ;
	gtk_signal_connect (GTK_OBJECT (draw_area), "realize", GTK_SIGNAL_FUNC (UnresetAlm_draw_area_realize), (gpointer)crt_n);
    gtk_signal_connect(GTK_OBJECT (draw_area), "button_press_event", GTK_SIGNAL_FUNC (UnresetAlm_button_press),GINT_TO_POINTER(crt_n) ); 
    
    gtk_signal_connect (GTK_OBJECT (draw_area),"size_allocate", GTK_SIGNAL_FUNC( unreset_alarm_canvas_allocate),(gpointer)crt_n) ;

	gtk_signal_connect (GTK_OBJECT (draw_area), "button_release_event", GTK_SIGNAL_FUNC (Unreset_alarm_button_release),GINT_TO_POINTER(crt_n) );    
   // gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), draw_area);
    gtk_widget_realize(GTK_WIDGET( draw_area)) ;
    gtk_widget_show(draw_area);
   
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
   	gtk_widget_show(hbox);
 
    gtk_widget_show(window); 

    return  window ;
   	  
}


gint UnresetAlmDevFilterDlgDelete(GtkWidget *w , gpointer data) ;
gint UnresetAlmDevFilterDlgOK(GtkWidget *w , gpointer data)
{
    int crt_n,i;
    char *ss1 ;
    GtkWidget *startEntry ,  *endEntry ,*searchEntry ; 	
	gchar *pConvert=0;
    GetCrtNumberOfWgt (w, &crt_n );
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ;
     FilterUnresetAlm[crt_n].dev_mask[0] = UnresetDevMask[crt_n];
	 for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	 {
		 FilterUnresetAlm[crt_n].group_mask[i] = UnresetGroupMask[crt_n][i];
	 }
     startEntry  = gtk_object_get_data(GTK_OBJECT(UnresetAlmfilterDlg[crt_n]),"startWgt");
     endEntry   = gtk_object_get_data(GTK_OBJECT(UnresetAlmfilterDlg[crt_n]),"endWgt");
     searchEntry = gtk_object_get_data(GTK_OBJECT(UnresetAlmfilterDlg[crt_n]),"searchWgt");
     if (SetTimeFilter  ( startEntry, &FilterUnresetAlm[crt_n].start_time)==FALSE) goto Fail  ;
     if (SetTimeFilter  ( endEntry,     &FilterUnresetAlm[crt_n].end_time)==FALSE ) goto Fail ; 
     if (FilterUnresetAlm[crt_n].start_time ==  FilterUnresetAlm[crt_n].end_time )
          FilterUnresetAlm[crt_n].start_time =  FilterUnresetAlm[crt_n].end_time = -1 ;
      ss1  =(char *) gtk_entry_get_text(GTK_ENTRY(searchEntry));
      /*if (strcmp(ss1,"")!=0)*/
           strcpy(FilterUnresetAlm[crt_n].search_str, pConvert=_fromUtf8(ss1));
		   G_SAFE_FREE(pConvert);
    
   
     FilterUnresetAlm[crt_n].is_change = TRUE;
	 FilterUnresetAlm[crt_n].change_flag = TRUE;
      UnresetAlmDevFilterDlgDelete( w ,   data);  
      return FALSE ;
      
 Fail:
      PopupMessageBox(UnresetAlm_pop_shell[crt_n] ,MSG_INVALID_DATA_FORMATE ); 
      return TRUE ;
      
}

gint UnresetAlmDevFilterDlgDelete(GtkWidget *w , gpointer data)
{
    int  crt_n = (gint)data;
    GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ;
    gtk_widget_destroy(UnresetAlmfilterDlg[crt_n]);
    UnresetAlmfilterDlg[crt_n] = (GtkWidget*)NULL ;

	return TRUE;
}

int CustomUnresetFilterDlgDelete(GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(UnresetSetFilterDlg[crt_n]);
	UnresetSetFilterDlg[crt_n] = (GtkWidget*)NULL ;
	return TRUE;
}

static void
UnresetSetDevMask (GtkCellRendererToggle *cell,
			gchar                 *path_str,
			gpointer               data)
{
	GtkTreeIter  iterDev,iterGroup;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
//	int j; 
	gint crt_n = (gint) data ;
	gint i = (gint)g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(UnresetStoreDev[crt_n]), &iterDev, path);
	gtk_tree_model_get (GTK_TREE_MODEL(UnresetStoreDev[crt_n]), &iterDev, COLUMN_CHECKED, &fixed, -1);
	if (!fixed)
	{
		UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  | (1<<i);
		UnresetGroupMask[crt_n][i] = 0xFFFFFFFF;
		if (i==UnresetCurDev&&gtk_tree_model_get_iter_first(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]),&iterGroup))
		{
			do 
			{
				gtk_list_store_set (UnresetStoreGroup[crt_n], &iterGroup, COLUMN_CHECKED, TRUE, -1);
			} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]),&iterGroup));
		}
	}
	else
	{
		UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  & (~(1<<i)); /*  Clear bit N */  
		UnresetGroupMask[crt_n][i] = 0;
		if (i==UnresetCurDev&&gtk_tree_model_get_iter_first(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]),&iterGroup))
		{
			do 
			{
				gtk_list_store_set (UnresetStoreGroup[crt_n], &iterGroup, COLUMN_CHECKED, FALSE, -1);
			} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]),&iterGroup));
		}
	}

	fixed ^= 1;
	gtk_list_store_set (UnresetStoreDev[crt_n], &iterDev, COLUMN_CHECKED, fixed, -1);
	gtk_tree_path_free (path);  
}

static void
UnresetSetGroupMask (GtkCellRendererToggle *cell,
			  gchar                 *path_str,
			  gpointer               data)
{
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
//	int j; 
	gint crt_n = (gint) data ;
	gint i =(gint) g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	
	gtk_tree_model_get_iter (GTK_TREE_MODEL(UnresetStoreGroup[crt_n]), &iter, path);
	gtk_tree_model_get (GTK_TREE_MODEL(UnresetStoreGroup[crt_n]), &iter, COLUMN_CHECKED, &fixed, -1);
	if (!fixed)
		UnresetGroupMask[crt_n][UnresetCurDev] = UnresetGroupMask[crt_n][UnresetCurDev]  | (1<<i);
	else
		UnresetGroupMask[crt_n][UnresetCurDev] = UnresetGroupMask[crt_n][UnresetCurDev]  & (~(1<<i)); /*  Clear bit N */  
	
	fixed ^= 1;
	gtk_list_store_set (UnresetStoreGroup[crt_n], &iter, COLUMN_CHECKED, fixed, -1);
	gtk_tree_path_free (path);  
}

gint UnresetSetDevMaskAllOrNone(GtkWidget *widget , gpointer data)
{
	int i ;
	int crt_n ,j;
	GtkTreeIter  iter;
	int stn_idx,station_num;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	GetStationNum(&station_num);
	stn_idx=0;
	if (station_num>=1)
	{
		for(i=1;i<=station_num;i++) 
		{
			if (UnresetAlarmStnDef[i].Id==FilterUnresetAlm[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
			
		}
	}

	if ((gint) data == SHOW_NONE)  /* show all !!!*/
	{
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(UnresetStoreDev[crt_n]),&iter))
		{
			gtk_list_store_set (UnresetStoreDev[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
			UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  & (~(1<<0));
		}
		for (j = 1 ; j <= UnresetAlarmStnDef[stn_idx].dev_num ; j++)
		{			
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(UnresetStoreDev[crt_n]),&iter))
			{
				gtk_list_store_set (UnresetStoreDev[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
				UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  & (~(1<<j));
			}
		}
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]),&iter))
		{
			gtk_list_store_set (UnresetStoreGroup[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
		}
		for (j=1;j<=UnresetAlarmStnDef[stn_idx].dev[UnresetCurDev+1].group_num;j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]),&iter))
			{
				gtk_list_store_set (UnresetStoreGroup[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
			}
		}
		for (j=0;j<MAX_GROUP_MASK_NUM;j++)
		{
			UnresetGroupMask[crt_n][j] = 0;
		}
	}else if ((gint) data ==SHOW_ALL) /*  show none !!!*/
	{
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(UnresetStoreDev[crt_n]),&iter))
		{
			gtk_list_store_set (UnresetStoreDev[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
			UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  | (1<<0);
		}
		for (j = 1 ; j <= UnresetAlarmStnDef[stn_idx].dev_num ; j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(UnresetStoreDev[crt_n]),&iter))
			{
				gtk_list_store_set (UnresetStoreDev[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);  
				UnresetDevMask[crt_n] = UnresetDevMask[crt_n]  | (1<<j);
			}
		}
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]),&iter))
		{
			gtk_list_store_set (UnresetStoreGroup[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
		}
		for (j=1;j<=UnresetAlarmStnDef[stn_idx].dev[UnresetCurDev+1].group_num;j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]),&iter))
			{
				gtk_list_store_set (UnresetStoreGroup[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
			}
		}
		for (j=0;j<MAX_GROUP_MASK_NUM;j++)
		{
			UnresetGroupMask[crt_n][j] = 0xffffffff;
		}
	}
	return TRUE;
}
gboolean UnresetDevClicked(GtkTreeView *treeview,
					   gpointer     data)
{
	GtkTreePath  *path;
	GtkTreeModel *model;
	GtkTreeIter  iter;
	int i,j;
	gchar *devName;
	BOOL groupIsChecked,fixed;
	gint *path_num;
	int stn_idx,station_num;
	gchar *pConvert=0, *pConvert1=0;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	gtk_tree_view_get_cursor(treeview,&path,NULL);
	path_num = gtk_tree_path_get_indices(path);
	UnresetCurDev = *path_num;
	model = gtk_tree_view_get_model(treeview);
	gtk_tree_model_get_iter(model,&iter,path);
	gtk_tree_model_get(model,&iter,COLUMN_CHECKED,&fixed,-1);
	gtk_tree_model_get (model, &iter, COLUMN_NAME, &devName, -1);
	gtk_list_store_clear(UnresetStoreGroup[crt_n]);
	GetStationNum(&station_num);

	stn_idx=0;
	if (station_num>=1)
	{
		for(i=1;i<=station_num;i++) 
		{
			if (UnresetAlarmStnDef[i].Id==FilterUnresetAlm[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
			
		}
	}
	
	if (fixed)
	{
		for(i=1;i<=UnresetAlarmStnDef[stn_idx].dev_num;i++)
		{
			if (!strcmp(UnresetAlarmStnDef[stn_idx].dev[i].CName,pConvert1=_fromUtf8(devName)))
			{
				for (j=1;j<=UnresetAlarmStnDef[stn_idx].dev[i].group_num;j++)
				{
					if(strcmp(UnresetAlarmStnDef[stn_idx].dev[i].group[j].CName,"")!=0
						&&UnresetAlarmStnDef[stn_idx].dev[i].group[j].isFilter[crt_n] == FALSE)
					{ 
						gtk_list_store_append (UnresetStoreGroup[crt_n], &iter);
						if (UnresetGroupMask[crt_n][UnresetCurDev] & (0X1<<(j-1)) ) 
							groupIsChecked = TRUE;  
						else
							groupIsChecked = FALSE;
						gtk_list_store_set (UnresetStoreGroup[crt_n], &iter,COLUMN_CHECKED, groupIsChecked,COLUMN_NAME, 
							pConvert=_toUtf8(UnresetAlarmStnDef[stn_idx].dev[i].group[j].CName),-1);
						G_SAFE_FREE(pConvert);
					}
				}
			}
			G_SAFE_FREE(pConvert1);
		}
	}
	g_free(devName);
	return TRUE;
}
static GtkWidget*
CreateDevFilterDlgUnset(GtkWidget *parent ,int crt_n)
{
	gint i;  
	GtkWidget *windows1,*vbox1  ,*hbox, *frame1 ;
	GtkWidget *table1 ,*label4 ,*label5,  *label6 ,*entry1, *entry2;
	GtkWidget *entry3 ,*hbox1 ,*button1  ,*button2 ,*checkbutton3;   
	GtkWidget *devAll,*devNone;
	GtkWidget *listDev,*listGroup;
	GtkTreeIter iter;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	BOOL	devIsChecked;   
	time_t    newtime;
	struct  tm      devtime; 
	char *sName[] ={OIX_CSTR_ALM_REC_NON ,OIX_CSTR_ALM_REC_ALL      };
	char timer[30]; 
	gchar *pConvert=0;
	int station_num,stn_idx;
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	UnresetDevMask[crt_n] =  FilterUnresetAlm[crt_n].dev_mask[0] ;
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		UnresetGroupMask[crt_n][i] = FilterUnresetAlm[crt_n].group_mask[i];
	}
    sprintf (timer , "%4d-%02d-%02d %02d:%02d:%02d" ,devtime.tm_year+1900,
		devtime.tm_mon+1 ,devtime.tm_mday , devtime.tm_hour, devtime.tm_min ,devtime.tm_sec) ;
	
	
	windows1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(windows1), pConvert=_toUtf8EX(OIX_STR_DEV_FILTER));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows1), GTK_WINDOW(parent)); 
	
	gtk_signal_connect (GTK_OBJECT (windows1), "delete_event", 
		      GTK_SIGNAL_FUNC (UnresetAlmDevFilterDlgDelete),  (gpointer)crt_n); 
	
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows1),TRUE);
	gtk_window_set_position(GTK_WINDOW(windows1),GTK_WIN_POS_CENTER);
	gtk_widget_show (windows1);
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (windows1), vbox1);
	printf("crt_n=%d,cur_stn_id=%d\n",crt_n,FilterUnresetAlm[crt_n].stn_id);
	hbox = gtk_hbox_new (FALSE, 0);
 	gtk_widget_show (hbox);
 	UnresetStoreDev[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);
	UnresetStoreGroup[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);

	GetStationNum(&station_num);
	stn_idx=0;
	if (station_num>=1)
	{
		for(i=1;i<=station_num;i++) 
		{
			if (UnresetAlarmStnDef[i].Id==FilterUnresetAlm[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
			
		}
	}
	for(i=1;i<=UnresetAlarmStnDef[stn_idx].dev_num;i++)
	{
		if(strcmp(UnresetAlarmStnDef[stn_idx].dev[i].CName,"")!=0)
		{ 
			if (FilterUnresetAlm[crt_n].dev_mask[0] & (0X1<<(i-1)) ) 
				devIsChecked = TRUE;  
			else
				devIsChecked = FALSE;
			gtk_list_store_append (UnresetStoreDev[crt_n], &iter);
			gtk_list_store_set (UnresetStoreDev[crt_n], &iter,COLUMN_CHECKED, devIsChecked,COLUMN_NAME, 
				pConvert=_toUtf8(UnresetAlarmStnDef[stn_idx].dev[i].CName),-1);
			G_SAFE_FREE(pConvert);
		}
	}
	listDev = gtk_tree_view_new_with_model (GTK_TREE_MODEL(UnresetStoreDev[crt_n]));
	gtk_signal_connect(GTK_OBJECT(listDev),"cursor-changed",G_CALLBACK(UnresetDevClicked),GINT_TO_POINTER(crt_n));
	g_object_unref (UnresetStoreDev[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (UnresetSetDevMask), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"active", COLUMN_CHECKED,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listDev), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listDev), column);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listDev),FALSE);
	gtk_box_pack_start (GTK_BOX (hbox), listDev, FALSE, FALSE, 0);
	
	listGroup = gtk_tree_view_new_with_model(GTK_TREE_MODEL(UnresetStoreGroup[crt_n]));
	g_object_unref (UnresetStoreGroup[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (UnresetSetGroupMask), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"active", COLUMN_CHECKED,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listGroup), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listGroup), column);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listGroup),FALSE);
	gtk_box_pack_start (GTK_BOX (hbox), listGroup, TRUE, TRUE, 0);
	
	gtk_box_pack_start (GTK_BOX (vbox1), hbox, FALSE, FALSE, 0);
	gtk_widget_show(listDev);
	gtk_widget_show(listGroup);
	devNone = gtk_button_new_with_label(pConvert=_toUtf8(_CS_(sName[0])));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(devNone);
	gtk_box_pack_start (GTK_BOX (vbox1),  devNone, FALSE, FALSE, 0);
	gtk_signal_connect (GTK_OBJECT (devNone), "clicked",GTK_SIGNAL_FUNC (UnresetSetDevMaskAllOrNone),GINT_TO_POINTER( SHOW_NONE));
	
	devAll = gtk_button_new_with_label(pConvert=_toUtf8(_CS_(sName[1])));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(devAll);
	gtk_box_pack_start (GTK_BOX (vbox1),  devAll, FALSE, FALSE, 0);
	gtk_signal_connect (GTK_OBJECT (devAll), "clicked",GTK_SIGNAL_FUNC (UnresetSetDevMaskAllOrNone),GINT_TO_POINTER( SHOW_ALL));

		
		frame1 = gtk_frame_new (NULL);
		
		gtk_object_set_data(GTK_OBJECT(windows1),"frameWgt",frame1);
		gtk_box_pack_start (GTK_BOX (vbox1), frame1, FALSE, FALSE, 0);
		
		table1 = gtk_table_new (3, 2, FALSE);
		gtk_widget_show (table1);
		gtk_container_add (GTK_CONTAINER (frame1), table1);
		
		label4 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_STATR_DATE));
		G_SAFE_FREE(pConvert);
		gtk_widget_show (label4);
		gtk_table_attach (GTK_TABLE (table1), label4, 0, 1, 0, 1,
			(GtkAttachOptions) (GTK_FILL),
			(GtkAttachOptions) (0), 0, 0);
		gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);
		
		
		
		label5 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_END_DATE));
		G_SAFE_FREE(pConvert);
		gtk_widget_show (label5);
		gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 1, 2,
			(GtkAttachOptions) (GTK_FILL),
			(GtkAttachOptions) (0), 0, 0);
		gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);
		
		
		
		label6 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_SEARCH_STR));
		G_SAFE_FREE(pConvert);
		gtk_widget_show (label6);
		gtk_table_attach (GTK_TABLE (table1), label6, 0, 1, 2, 3,
			(GtkAttachOptions) (GTK_FILL),
			(GtkAttachOptions) (0), 0, 0);
		gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);
		gtk_misc_set_alignment (GTK_MISC (label6), 0, 0.5);
		
		entry1 = gtk_entry_new ();
		gtk_widget_show (entry1);
		gtk_table_attach (GTK_TABLE (table1), entry1, 1, 2, 0, 1,
			(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			(GtkAttachOptions) (0), 0, 0);
		gtk_object_set_data(GTK_OBJECT(windows1),"startWgt",entry1);
		gtk_entry_set_max_length(GTK_ENTRY(entry1),20);
		SetFileterToEntry  ( entry1, &FilterUnresetAlm[crt_n].start_time)    ;
		
		entry2 = gtk_entry_new ();
		gtk_widget_show (entry2);
		gtk_table_attach (GTK_TABLE (table1), entry2, 1, 2, 1, 2,
			(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			(GtkAttachOptions) (0), 0, 0);
		
		gtk_object_set_data(GTK_OBJECT(windows1),"endWgt",entry2);
		gtk_entry_set_max_length(GTK_ENTRY(entry2),20);
		SetFileterToEntry  ( entry2, &FilterUnresetAlm[crt_n].end_time)    ;
		
		entry3 = gtk_entry_new ();
		gtk_widget_show (entry3);
		gtk_table_attach (GTK_TABLE (table1), entry3, 1, 2, 2, 3,
			(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			(GtkAttachOptions) (0), 0, 0);
		
		
		
		gtk_object_set_data(GTK_OBJECT(windows1),"searchWgt",entry3);
		gtk_entry_set_max_length(GTK_ENTRY(entry3),MAX_SEARCH_NAME_SIZE);
		
		if (FilterUnresetAlm[crt_n].search_str[0] !=0)  
		{
			gtk_entry_set_text(GTK_ENTRY(entry3),FilterUnresetAlm[crt_n].search_str);
		}
		
		
		hbox1 = gtk_hbox_new (FALSE, 0);
		gtk_widget_show (hbox1);
		gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);
		
		button1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
		G_SAFE_FREE(pConvert);
		//gtk_button_new_from_stock ("gtk-cancel");
		gtk_widget_show (button1);
		gtk_box_pack_end (GTK_BOX (hbox1), button1, FALSE, FALSE, 0);
		
		
		
		button2 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_APPLY));
		G_SAFE_FREE(pConvert);
		//gtk_button_new_from_stock ("gtk-apply");
		gtk_widget_show (button2);
		gtk_box_pack_end (GTK_BOX (hbox1), button2, FALSE, FALSE, 0);
		
		checkbutton3 = gtk_check_button_new_with_label (pConvert=_toUtf8EX(OIX_STR_MORE));
		G_SAFE_FREE(pConvert);
		gtk_widget_show (checkbutton3);
		gtk_box_pack_end (GTK_BOX (hbox1), checkbutton3, FALSE, FALSE, 0);
        
		gtk_signal_connect (GTK_OBJECT (checkbutton3), "toggled",GTK_SIGNAL_FUNC (isDisplayMore), windows1);   
		gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (UnresetAlmDevFilterDlgDelete), windows1);  
		gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (UnresetAlmDevFilterDlgOK), windows1);          
        
		gtk_toggle_button_set_active(checkbutton3,1) ; //added by chi 
		gtk_window_move(GTK_WINDOW(windows1) , 100, 100);  
		return  windows1 ;  	
}

void
last_page_press  (GtkToggleButton *w,   gpointer         user_data)
{
	GtkWidget *prev  ,*next ,*scrollbar ;
     GtkWidget *toolbar  ;
	int crt_n =   0;
	 toolbar =  ( GtkWidget* )user_data ;
	 
        GetCrtNumberOfWgt (GTK_WIDGET(w), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
        
        prev = gtk_object_get_data(GTK_OBJECT(toolbar),"prevWgt");
        next = gtk_object_get_data(GTK_OBJECT(toolbar),"nextWgt");
        scrollbar = g_object_get_data(G_OBJECT(UnresetAlm_pop_shell[crt_n]),"scrollbar");
		IsLastPage[crt_n]=	gtk_toggle_button_get_active(w);
        if (IsLastPage[crt_n])
        {
            gtk_widget_set_sensitive(prev, FALSE);
            gtk_widget_set_sensitive(next, FALSE);
             gtk_widget_set_sensitive(scrollbar, FALSE);
        }
        else
       {
            gtk_widget_set_sensitive(prev, TRUE);
            gtk_widget_set_sensitive(next, TRUE);
            gtk_widget_set_sensitive(scrollbar, TRUE);
        }
}

gint doUnresetAlarmAckRange(GtkWidget       *widget,   gpointer     data)
{ 
   int crt_n = (int)data ;
   if (!unreset_page[crt_n].isSelected)
       {
           PopupMessageBox(UnresetAlm_pop_shell[crt_n],MSG_SELECT_ACK_RANGE);
           return FALSE;
   }
	printf ("$$$$$$select start =%d , end = %d \n" ,unreset_page[crt_n].SelectStart ,unreset_page[crt_n].SelectEnd ) ;
   UnresetAlarmAckEX( widget,crt_n, unreset_page[crt_n].SelectStart ,unreset_page[crt_n].SelectEnd   ) ;
   UnresetAlmDspProc(crt_n) ;
   return TRUE;
}

void	UnresetAlarmAck (GtkWidget *w,int crt_n )
{
	 UnresetAlarmAckEX (w,crt_n ,unreset_page[crt_n].PageStart , unreset_page[crt_n].PageEnd  );
}


void	UnresetAlarmAckEX (GtkWidget *w,int crt_n ,int startI , int endI  )
{
	int	i, ackn_st,send_cnt = 0;
	POINTER tmp_ptr;
	DMS_COMMON	db_cmm;
        
	if(!CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
	{
		PopupMessageBox ( UnresetAlm_pop_shell[crt_n], MSG_ALARM_ACK);
		return ;
	}   /*unreset_page[crt_n].PageStart  */
   /* printf("    PageStart=%d,PageEnd=%d,shm_fault_addr->head.count=%d\n",
    unreset_page[crt_n].PageStart,unreset_page[crt_n].PageEnd,shm_fault_addr->head.count);*/
	for ( i= startI; i <= endI; i++)
        {
                if(-1==GetPointFromRec(&shm_fault_addr->rec[UnresetAlmIndex_tmp[crt_n].index_pt[i]],&tmp_ptr))
                    continue ;		
		
		if ( ReadEntryById ( &tmp_ptr, &db_cmm ) == -1)
		{
			printf ("AcknUnresetAlmFlag: No this point in the database file !!!\n" );
		}
		else
		{
			ackn_st = db_cmm.fixed.iost.almNoAck;
			if ( ackn_st == UNACKN )
			{
				CmdBufInit ( crt_n );
				cmd_buf[crt_n].stn_id		= db_cmm.point.stn_id;
        			cmd_buf[crt_n].dev_id		= db_cmm.point.dev_id;
				cmd_buf[crt_n].point_id	= db_cmm.point.pt_id;
				cmd_buf[crt_n].data_type	= db_cmm.point.data_type;
				cmd_buf[crt_n].type_id		= DPS_IOST_SET;  
				cmd_buf[crt_n].status.state = ACKN;
				cmd_buf[crt_n].status.opr  = IOST_SET_ALM_NOACK;

				ProgDoAckCelBrdcst ( crt_n );
				send_cnt++;	
				if ( send_cnt > MAX_NETCMD_SEG_NUM )
				{
					MySleep ( 10 );
					send_cnt = 0;
				}
             }
        }
	}  /*for */
}

void	UnresetAlarmListAck (GtkWidget *w,int crt_n )
{
	int	i, ackn_st,send_cnt = 0;
	POINTER tmp_ptr;
	DMS_COMMON	db_cmm;
        
	if(!CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
	{
		PopupMessageBox ( UnresetAlm_pop_shell[crt_n], MSG_ALARM_ACK);
		return ;
	}
   /* printf("   PageStart=%d,PageEnd=%d,shm_fault_addr->head.count=%d\n",
    unreset_page[crt_n].PageStart,unreset_page[crt_n].PageEnd,shm_fault_addr->head.count);*/
	for ( i= 0; i < shm_fault_addr->head.count; i++)
        {
    
                if(-1==GetPointFromRec(&shm_fault_addr->rec[UnresetAlmIndex_tmp[crt_n].index_pt[i]],&tmp_ptr))
                    continue ;		
		
		if ( ReadEntryById ( &tmp_ptr, &db_cmm ) == -1)
		{
			printf ("AcknUnresetAlmFlag: No this point in the database file !!!\n" );
		}
		else
		{
			ackn_st = db_cmm.fixed.iost.almNoAck;
			if ( ackn_st == UNACKN )
			{
				CmdBufInit ( crt_n );
				cmd_buf[crt_n].stn_id		= db_cmm.point.stn_id;
        			cmd_buf[crt_n].dev_id		= db_cmm.point.dev_id;
				cmd_buf[crt_n].point_id	= db_cmm.point.pt_id;
				cmd_buf[crt_n].data_type	= db_cmm.point.data_type;
				cmd_buf[crt_n].type_id		= DPS_IOST_SET;  
				cmd_buf[crt_n].status.state = ACKN;
				cmd_buf[crt_n].status.opr  = IOST_SET_ALM_NOACK;

				ProgDoAckCelBrdcst ( crt_n );
				send_cnt++;	
				if ( send_cnt > MAX_NETCMD_SEG_NUM )
				{
                    printf("####send_cnt > MAX_NETCMD_SEG_NUM::send_cnt=%d\n",send_cnt);
					MySleep ( 10 );
					send_cnt = 0;
				}
                        }
                }
	}
}

gint UnresetAutoFilterToggled(GtkWidget *widget,gpointer data)
{
	int i,j;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	Unreset_auto_filter_flag[crt_n] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	if (FilterUnresetAlm[crt_n].stn_id != 0)
	{
		if (Unreset_auto_filter_flag[crt_n])
		{
			if (!Unreset_autofilter_timeout_id[crt_n])
				Unreset_autofilter_timeout_id[crt_n] = gtk_timeout_add ( 1000,(GtkFunction)UnresetAutoFilterProc, GINT_TO_POINTER (crt_n) );
		}
		else
		{
			if (Unreset_autofilter_timeout_id[crt_n])
			{
				gtk_timeout_remove (Unreset_autofilter_timeout_id[crt_n]);
				Unreset_autofilter_timeout_id[crt_n] = 0;
			}
			for(i=1;i<=UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev_num;i++)
				for (j=1;j<=UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev[i].group_num;j++)	
				{
					UnresetAlarmStnDef[FilterUnresetAlm[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = FALSE;
					FilterUnresetAlm[crt_n].group_mask[i-1] = (FilterUnresetAlm[crt_n].group_mask[i-1]) | (1<<(j-1));
				}
				FilterUnresetAlm[crt_n].is_change = TRUE;
				UnresetAlmDspProc(crt_n);		
		}
	}			
	return TRUE;
}
/*
static gboolean
is_separator (GtkTreeModel *model,
			  GtkTreeIter  *iter,
			  gpointer      data)
{
	GtkTreePath *path;
	gboolean result;
	gint iter_num; 
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	path = gtk_tree_model_get_path (model, iter);
	iter_num = gtk_tree_model_iter_n_children(model,NULL);
	result = gtk_tree_path_get_indices (path)[0] == (iter_num-2);
	gtk_tree_path_free (path);
	
	return result;
}
*/

gint UnresetPopupCustomFilter(GtkComboBox *widget,gpointer data) 
{
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	gtk_combo_box_set_active(widget,-1);
}
static GtkWidget *
CreateUnresetAlmToolbar (GtkWidget * parent,int	crt_n)
{
  	
	GtkWidget *box,*box1,*last_button,*init_button,*range_ack_btn;
	GtkWidget *lbPage ;  	
	GtkWidget *vbox;	
	GtkWidget *toolbar   ; 
	GtkWidget *Prev_button,*next_button,*button_exit,*dev_button,*ack_button,*list_ack_button,*button_print;
	GtkWidget *optionmenu, *menu,*items;
	int	i,j;
	int	station_num;
	int MenuPointer;
	GtkWidget *filterCombo;
	GtkWidget *auto_filter;
	GtkTreeIter  iter;
	GtkCellRenderer *pRenderFilter;
//	GtkTreeModel *testmodel;
	gchar *pConvert=0;
	UCHAR	stn_id;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return (GtkWidget *)NULL;
	
	memcpy(UnresetAlarmStnDef,StnDef,sizeof(StnDef));
	toolbar = gtk_handle_box_new ();
	
	vbox=gtk_vbox_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER (toolbar), vbox);
	gtk_widget_show (vbox);
	
	box = gtk_hbox_new (FALSE, 4);
	//gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_add (GTK_CONTAINER (vbox), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);
	
	
	
	box1 = gtk_hbox_new (FALSE, 0);
	/*gtk_container_add (GTK_CONTAINER (toolbar), box1);*/
	gtk_box_pack_start (GTK_BOX (vbox), box1, FALSE, FALSE, 0);
	gtk_container_border_width (GTK_CONTAINER (box1), 0);
	gtk_widget_show (box1);
	if (xscale[crt_n] >=1 ) box1 = box ;
	
	/*  *************  Power station combobox  ****************** */
	
  	 GetStationNum(&station_num);
	 
	 optionmenu = gtk_option_menu_new();
   	 gtk_widget_show(optionmenu);
   	 gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
   	 menu =gtk_menu_new();
	 gtk_widget_set_usize (menu, 130, -1);
	 
	 if(station_num>1)
	 {
		 items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(OIX_STR_ALL_STATION));	 
		 G_SAFE_FREE(pConvert);
		 gtk_widget_show( items);
		 gtk_container_add(GTK_CONTAINER(menu),items);
		 MenuPointer=getMenuDataPointer(crt_n,0);
		 gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (CreateDevFilter), (gpointer)MenuPointer);
		 FilterUnresetAlm[crt_n].stn_id =0;
	 }
	 else
		 FilterUnresetAlm[crt_n].stn_id=StnDef[1].Id;
	 
	 for(i=1;i<=station_num;i++) 
	 {
		 if(strcmp(UnresetAlarmStnDef[i].CName,"")!=0)
		 {  
			 if(-1==GetStnIdByIndex(i, &stn_id))
				 continue;

			 
			 if (isStnInMask((UCHAR)i)==0) continue ; /*屏蔽不显示*/
			 items = gtk_menu_item_new_with_label(pConvert=_toUtf8(UnresetAlarmStnDef[i].CName));
			 G_SAFE_FREE(pConvert);
			 gtk_widget_show( items);
			 gtk_container_add(GTK_CONTAINER(menu),items);
			 MenuPointer=getMenuDataPointer(crt_n,stn_id);
			 gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (CreateDevFilter), (gpointer)MenuPointer);
		 }
	 }
	 g_object_set_data   ( G_OBJECT(parent),"StationOpMenu",optionmenu);
	 gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	 
	 /*  *************  dev   combobox  ****************** */ 
	 
	 dev_button  = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_DEV_FILTER));
	 G_SAFE_FREE(pConvert);
	 gtk_signal_connect (GTK_OBJECT (dev_button), "clicked",G_CALLBACK (UnresetAlmdevBtnPress),  (gpointer)crt_n);
	 gtk_widget_show(dev_button);
	 gtk_box_pack_start (GTK_BOX (box), dev_button, FALSE, FALSE, 0);
	 
	 
	 
	 /*  ************* fault type  combobox  *******************/ 
	 optionmenu = gtk_option_menu_new();
	 gtk_widget_show(optionmenu);
	 gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
	 menu =gtk_menu_new();
	 gtk_widget_set_usize (menu, 130, -1);
	 
	 for(i=0;i<sizeof(UnresetAlm_faulttype)/sizeof(UnresetAlm_faulttype[0]);i++)
	 {
		 if(strcmp(UnresetAlm_faulttype[i],"")!=0)
		 {  			 
     	     		UnresetAlmFaultItem[crt_n][i] = gtk_menu_item_new_with_label(pConvert=_toUtf8(_CS_(UnresetAlm_faulttype[i])));				
					G_SAFE_FREE(pConvert);

					gtk_widget_show(UnresetAlmFaultItem[crt_n][i]);
					gtk_container_add(GTK_CONTAINER(menu),UnresetAlmFaultItem[crt_n][i]);
					MenuPointer=getMenuDataPointer(crt_n,i);
					gtk_signal_connect (GTK_OBJECT (UnresetAlmFaultItem[crt_n][i]), "activate",GTK_SIGNAL_FUNC (SetUnresetAlmfaultMask), (gpointer)MenuPointer); 
					
		 }
	 }
	 for (j = 0 ;j < UNRESETALM_DATATYPE_NUM ; j++)
	 {
		 if (FilterUnresetAlm[crt_n].fault_mask & 0x01 <<j) break ;
	 }
	 
	 g_object_set_data(G_OBJECT(parent),"DataTypeOpt",optionmenu);
	 gtk_menu_set_active ( GTK_MENU(menu),j   );   //added bychi 
	 gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	 /*  *************   type id combobox  *******************/ 
	 optionmenu = gtk_option_menu_new();
	 gtk_widget_show(optionmenu);
	 gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
	 menu =gtk_menu_new();
	 gtk_widget_set_usize (menu, 130, -1);
	 
	 for(i=0;i<sizeof(UnresetAlm_typeid)/sizeof(UnresetAlm_typeid[0]);i++)
	 {
		 if(strcmp(UnresetAlm_typeid[i],"")!=0)
		 {  			 
			 UnresetAlmTypeIdItem[crt_n][i] = gtk_menu_item_new_with_label(pConvert=_toUtf8(_CS_(UnresetAlm_typeid[i])));				
			 G_SAFE_FREE(pConvert);

			 gtk_widget_show(UnresetAlmTypeIdItem[crt_n][i]);
			 gtk_container_add(GTK_CONTAINER(menu),UnresetAlmTypeIdItem[crt_n][i]);
			 MenuPointer=getMenuDataPointer(crt_n,i);
			 gtk_signal_connect (GTK_OBJECT (UnresetAlmTypeIdItem[crt_n][i]), "activate",GTK_SIGNAL_FUNC (SetUnresetAlmTypeIdMask), (gpointer)MenuPointer); 
		 }
	 }
	 
	 for (j = 0 ;j < UNRESETALM_DATATYPE_NUM ; j++)
	 {
		 if (FilterUnresetAlm[crt_n].type_id_mask & 0x01 <<j) break ;
	 }
     g_object_set_data(G_OBJECT(parent),"TypeIdOpt",optionmenu);
	 
	 gtk_menu_set_active ( GTK_MENU(menu),j );   //added bychi 
	 gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);

	 Prev_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_UP));
	 G_SAFE_FREE(pConvert);
	 
	 gtk_box_pack_start (GTK_BOX (box), Prev_button, FALSE, FALSE, 0);     	     
	 gtk_signal_connect(GTK_OBJECT(Prev_button), "clicked", GTK_SIGNAL_FUNC(Prev), (gpointer)crt_n);
	 gtk_widget_show (Prev_button);
	 
	 gtk_object_set_data(GTK_OBJECT(toolbar),"prevWgt",Prev_button );
	 
	 next_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_DOWN));
	 G_SAFE_FREE(pConvert);
	 gtk_box_pack_start (GTK_BOX (box), next_button, FALSE, FALSE, 0);     	     
	 gtk_signal_connect(GTK_OBJECT(next_button), "clicked", GTK_SIGNAL_FUNC(Next),   (gpointer)crt_n);
	 gtk_widget_show (next_button);
	 
	 gtk_object_set_data(GTK_OBJECT(toolbar),"nextWgt",next_button );
	 
// 	 init_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_STR_FILTER_INIT));
// 	 G_SAFE_FREE(pConvert);
// 	 gtk_box_pack_start (GTK_BOX (box), init_button, FALSE, FALSE, 0);     	     
// 	 gtk_signal_connect(GTK_OBJECT(init_button), "clicked", GTK_SIGNAL_FUNC(UnresetAlmFilterInit),  (gpointer)crt_n);
// 	 gtk_widget_show (init_button);    
	 
	 /**************************************************Filter combobox*****************************************************/
	 UnresetCurFilter = 0;
	 UnresetStoreFilter[crt_n]=gtk_list_store_new(num_columns_Filter,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
													G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,
													G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	 filterCombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(UnresetStoreFilter[crt_n]));
	 
	 pRenderFilter = gtk_cell_renderer_text_new();
	 gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(filterCombo), pRenderFilter, FALSE);
	 gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(filterCombo), pRenderFilter, "text", eName_Filter, NULL);
	 g_object_unref(UnresetStoreFilter[crt_n]);
	 
	 gtk_box_pack_start (GTK_BOX (box), filterCombo, FALSE, FALSE, 0);
	 gtk_widget_set_usize (filterCombo, 130, -1);
	 gtk_widget_show(filterCombo);
	 gtk_list_store_insert(UnresetStoreFilter[crt_n], &iter,0);
	 gtk_list_store_set (UnresetStoreFilter[crt_n], &iter, eName_Filter, pConvert=_toUtf8EX(OIX_STR_FILTER_INIT), -1);
	 G_SAFE_FREE(pConvert);
//	 gtk_combo_box_set_row_separator_func (GTK_COMBO_BOX (filterCombo), is_separator, GINT_TO_POINTER(crt_n), NULL);
//	 gtk_list_store_insert(UnresetStoreFilter[crt_n], &iter,1);
//	 gtk_list_store_set (UnresetStoreFilter[crt_n], &iter, eName_Filter,NULL, -1);
	 gtk_list_store_insert(UnresetStoreFilter[crt_n], &iter,1);
	 gtk_list_store_set (UnresetStoreFilter[crt_n], &iter, eName_Filter, pConvert=_toUtf8EX(OIX_STR_FILTER_SET), -1);
	 G_SAFE_FREE(pConvert);
	 UnresetFilterNum = ReadFilterXml(UnresetStoreFilter[crt_n],"Predefine");
	 gtk_combo_box_set_active(GTK_COMBO_BOX(filterCombo),UnresetCurFilter);
	 g_signal_connect (G_OBJECT(filterCombo), "changed", G_CALLBACK(ChgUnresetCustomFilter), GINT_TO_POINTER(crt_n)); 
	 g_signal_connect(G_OBJECT(filterCombo), "popup", G_CALLBACK(UnresetPopupCustomFilter), GINT_TO_POINTER(crt_n));
	 /********************************************************************************************************************/

	 last_button  = gtk_toggle_button_new_with_label (pConvert=_toUtf8EX(OIX_CSTR_NEW_PAGE));
	 G_SAFE_FREE(pConvert);
	 gtk_widget_show (last_button);
	 gtk_box_pack_start (GTK_BOX (box), last_button, FALSE, FALSE, 0);     	     
	 gtk_signal_connect(GTK_OBJECT(last_button), "toggled", GTK_SIGNAL_FUNC(last_page_press), (gpointer)toolbar);
     
	 lbPage = gtk_label_new ("Page:1/3");
	 gtk_box_pack_end (GTK_BOX (box), lbPage, FALSE, FALSE, 0);
	 gtk_widget_show (lbPage);
	 gtk_object_set_data(GTK_OBJECT(parent),"labelPage",lbPage );
	 
	 range_ack_btn = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_RANGE_ACKN));
	 G_SAFE_FREE(pConvert);
	 gtk_box_pack_start (GTK_BOX (box),  range_ack_btn, FALSE, FALSE, 0);     	     
	 gtk_signal_connect(GTK_OBJECT( range_ack_btn), "clicked", GTK_SIGNAL_FUNC(doUnresetAlarmAckRange),   GINT_TO_POINTER(crt_n));
	 gtk_widget_show ( range_ack_btn); 
	 
	 
	 
	 ack_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_TAB_ACKN));
	 G_SAFE_FREE(pConvert);
	 gtk_box_pack_start (GTK_BOX (box), ack_button, FALSE, FALSE, 0);     	     
	 gtk_signal_connect(GTK_OBJECT(ack_button), "clicked", GTK_SIGNAL_FUNC(UnresetAlarmAck),   GINT_TO_POINTER(crt_n));
	 gtk_widget_show (ack_button);  
	 if(CheckUserPrivilege(cur_usr_name, PRIV_SCAN_ALM_SET))
	 {
		 list_ack_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_LIST_ACKN));
		 G_SAFE_FREE(pConvert);
		 gtk_box_pack_start (GTK_BOX (box), list_ack_button, FALSE, FALSE, 0);     	     
		 gtk_signal_connect(GTK_OBJECT(list_ack_button), "clicked", GTK_SIGNAL_FUNC(UnresetAlarmListAck),   GINT_TO_POINTER(crt_n));
		 gtk_widget_show (list_ack_button); 
	 }
#ifdef WINDOWS_OS
	 
	 button_print = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PRINT));
	 G_SAFE_FREE(pConvert);
    	gtk_box_pack_start (GTK_BOX (box), button_print, FALSE, FALSE, 0); 
        
        gtk_signal_connect(GTK_OBJECT(button_print), "clicked",
			GTK_SIGNAL_FUNC(PrintUnresetTab), (gpointer)crt_n);
        gtk_widget_show(button_print); 
#endif       
		if (xml_auto_filter)
	{
		auto_filter = gtk_check_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_AUTO_FILTER));
		G_SAFE_FREE(pConvert);
		gtk_widget_show(auto_filter);
		gtk_box_pack_start(GTK_BOX(box1),auto_filter,FALSE,FALSE,0);
		gtk_signal_connect(GTK_OBJECT(auto_filter), "toggled",GTK_SIGNAL_FUNC(UnresetAutoFilterToggled), GINT_TO_POINTER(crt_n));   
		Unreset_auto_filter_flag[crt_n] = FALSE;
		}

		button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT)); 
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start(GTK_BOX(box), button_exit, FALSE, FALSE, 0);   
		gtk_signal_connect(GTK_OBJECT(button_exit), "clicked",GTK_SIGNAL_FUNC(UnresetAlmFormQuit), (gpointer)crt_n);   
		gtk_widget_show(button_exit);
		
		return (toolbar);
}
