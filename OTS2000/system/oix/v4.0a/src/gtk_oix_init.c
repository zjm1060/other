/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2005.9.22
    2007.08.10 : have been SYNC  by chichi;
    LAST UPATE : 2007.08.10 by chichi ;

---------------------------------------------------------------------*/



#include <gdk/gdk.h>
#include	"../inc/oix.h"
extern FILTER_T 	FilterAlarm[MAX_CRT_NUMBER] ;
extern FILTER_T 	FilterAlarm_k[MAX_CRT_NUMBER] ;
extern FILTER_T     FilterUnresetAlm[MAX_CRT_NUMBER] ; 
extern void            InitAlarmFilter(FILTER_T *pFilter,int crt_n); 
extern void	ZoomInit (int crt_n );
extern void InitUnresetAlmFilter(FILTER_T *pFilter);
extern gint ChgCommonDir(GtkWidget* widget, gpointer data);
/*****************************************************************/
/*  定义oix 需要的配置全局变量 */
/*****************************************************************/ 

char   OIX_USE_XFONT = 0 ;      //use  Xfont ;
char   OIX_EVENT_DSP_LOGICNAME =0  ;   //display logic name in event list; 
char   OIX_EVENT_ALIAN_MOD = 0  ;     // 1 代表信息状态需要对齐 0 紧跟报警信息名;
char   OIX_FAULTTAB_NO_VISIBLE =0 ;   //  决定低端故障表是否显示;
char   OIX_LIST_FONT[128] = {"sans 10"}  ; 
char   OIX_DSP_TAG  = 0  ; 
char   OIX_KICK_SUSER  =  0  ; 
BOOL   oix_use_msg_def_str_flag= 1;

UINT	 stnMask4alarm;//config stn alarm display 


void    OixReadConfig(void)
{	

	char  Entry[128] ; 
	OIX_USE_XFONT  = 0 ; //default ;
	if(-1!=GetEmsString("OIX_USE_XFONT", Entry)) 
		if (strcmp(Entry,"YES")==0)
			OIX_USE_XFONT = 1 ; 
	OIX_EVENT_DSP_LOGICNAME = 0 ; 
	if(-1!=GetEmsString("OIX_EVENT_DSP_LOGICNAME", Entry)) 
		if (strcmp(Entry,"YES")==0)
			OIX_EVENT_DSP_LOGICNAME = 1 ; 
	OIX_EVENT_ALIAN_MOD = 0 ; 
	if(-1!=GetEmsString("OIX_EVENT_ALIAN_MOD", Entry)) 
		if (strcmp(Entry,"YES")==0)
		OIX_EVENT_ALIAN_MOD = 1 ; 

	OIX_FAULTTAB_NO_VISIBLE = 0 ;
	if(-1!=GetEmsString("OIX_FAULTTAB_NO_VISIBLE", Entry)) 
			if (strcmp(Entry,"YES")==0)
				OIX_FAULTTAB_NO_VISIBLE = 1 ; 

 
	if(-1!=GetEmsString("OIX_LIST_FONT", Entry)) 
		if (strcmp(Entry,""))
			strcpy (OIX_LIST_FONT,Entry) ;

	OIX_DSP_TAG = 0 ;
	if(-1!=GetEmsString("OIX_DSP_TAG", Entry)) 
		if (strcmp(Entry,"YES")==0)
				OIX_DSP_TAG = 1 ; 


	if(-1!=GetEmsString("OIX_USE_MSG_DEF_STRING", Entry) && strcmp(Entry,"NO")==0) 
			oix_use_msg_def_str_flag = 0 ; 

	
		OIX_KICK_SUSER = 0 ;
	if(-1!=GetEmsString("OIX_KICK_SUSER", Entry)) 
		if (strcmp(Entry,"YES")==0)
				OIX_KICK_SUSER = 1 ;

	getInitInfoFormXml();
}

int getInitInfoFormXml()
{
	xmlDocPtr   pdoc = NULL;
	xmlNodePtr  proot = NULL, pOIXInitNode = NULL,pAlamStnDsp = NULL,pNewNodw=NULL,pNameNode = NULL;
	char	err_str[256];
	//xmlChar *key; 
	gchar *key;
	int OIXInit_flag=0;
	int num = 0;
	xmlKeepBlanksDefault(0);
	pdoc = xmlReadFile (FILE_OIX_CONFIG_XML, "GB2312", XML_PARSE_NOBLANKS);
	if (pdoc == NULL)
    {
		sprintf(err_str, "Fail to open %s", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	proot = xmlDocGetRootElement (pdoc);
	if (proot == NULL)
    {
		sprintf(err_str, "%s not have root", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	pOIXInitNode = proot->xmlChildrenNode;
	while (pOIXInitNode != NULL)
    {
		if (!xmlStrcmp (pOIXInitNode->name, BAD_CAST "OIXInit"))
        {
			OIXInit_flag=1;
			pAlamStnDsp = pOIXInitNode->xmlChildrenNode;

			while(pAlamStnDsp != NULL)
			{
				if((!xmlStrcmp(pAlamStnDsp->name, BAD_CAST "ALM_STN_MASK")))
				{
					key=xmlNodeListGetString(pdoc, pAlamStnDsp->xmlChildrenNode, 1);					
					stnMask4alarm=(UINT)atoi(key);
					xmlFree(key);
				}
				else if((!xmlStrcmp(pAlamStnDsp->name, BAD_CAST "CMMDIR_BTN")))
				{
					key=xmlNodeListGetString(pdoc, pAlamStnDsp->xmlChildrenNode, 1);					
					xml_comdir_btn=(UINT)atoi(key);
					xmlFree(key);
				}else if((!xmlStrcmp(pAlamStnDsp->name, BAD_CAST "AUTO_FILTER")))
				{
					key=xmlNodeListGetString(pdoc, pAlamStnDsp->xmlChildrenNode, 1);					
					xml_auto_filter=(UINT)atoi(key);
					xmlFree(key);
				}
				pAlamStnDsp = pAlamStnDsp->next;
			}	
}

		if (!xmlStrcmp (pOIXInitNode->name, BAD_CAST "DspDefine"))
        {
			pNameNode = pOIXInitNode->xmlChildrenNode;
			while(pNameNode != NULL)
			{
				if((!xmlStrcmp(pNameNode->name, BAD_CAST "CommonDir")))
				{
					key=xmlNodeListGetString(pdoc, pNameNode->xmlChildrenNode, 1);
					if (key != NULL)
					{				
						strcpy(commonDirName,key);
					}
					xmlFree(key);
				}
				if((!xmlStrcmp(pNameNode->name, BAD_CAST "ZJX")))
				{
					key=xmlNodeListGetString(pdoc, pNameNode->xmlChildrenNode, 1);
					if (key != NULL)
					{					
						strcpy(zjxName,key);
					}
					xmlFree(key);	
				}
				pNameNode = pNameNode->next;
			}	
		}


		pOIXInitNode = pOIXInitNode->next;
    }
	
	if (!OIXInit_flag)
	{
		pNewNodw = xmlNewNode(NULL,BAD_CAST"OIXInit");
		xmlAddChild(proot,pNewNodw);
		stnMask4alarm=0xFFFFFFFF;

		xml_comdir_btn=0;
		xml_auto_filter=0;


	}

	xmlSaveFormatFile(FILE_OIX_CONFIG_XML,pdoc,1);
	xmlFreeDoc (pdoc);
	xmlCleanupParser ();
	return num;
}
void	OixInit (int crt_n )
{
	int i;
	int fontsize;	
	extern short cur_ws_crt_num;	 
	
	int evtidx;

	 for (i=0;i<LAYERNUM;i++)
	 {
		 SelectLayer[crt_n][i]=1;
	 }
	 UseLayerFilter[crt_n].UseFilter=0;
	 UseLayerFilter[crt_n].changeflag=0;
 
	 pixmap[crt_n]	= gdk_pixmap_new (canvas[crt_n]->window  ,
					canvas_width, canvas_height, -1);
     pixmap2[crt_n]	= gdk_pixmap_new (canvas2[crt_n]->window  ,
					canvas_width, canvas_height, -1);
	 bakgrd_pixmap[crt_n]	= gdk_pixmap_new (canvas[crt_n]->window  ,
					canvas_width, canvas_height, -1);    

	 view_pixmap[crt_n]	= gdk_pixmap_new (canvas[crt_n]->window  ,
					canvas_width, canvas_height, -1); 
	
	

	if (!pixmap[crt_n] || !bakgrd_pixmap[crt_n] || !pixmap2[crt_n] )
		g_message("\nOix Error: pixmap(0/2[)crt_n] and    bckgrdpixmap[crt_n]   create failure!\n");
	/* initialize gdbase -- graphics database. */
	gdbase[crt_n].nextpos	= 0;
	cur_dsp_ptr[crt_n]	= 0;

	for( i=0; i < MAX_DSP_CNT; i++)
		strcpy( rem_dsp_name[crt_n][i], "");

	ZoomInit ( crt_n );
  
	alm_sound =0;		 
	alm_bell_number =0;
	fontsize = 10 ;
  	fontsize=(int)( fontsize*xscale[crt_n]) ;
    sprintf(fontdesc[crt_n] , "sans  %d " ,fontsize);

	InitAlarmFilter( &FilterAlarm[crt_n],  crt_n);
	InitAlarmFilter( &FilterAlarm_k[crt_n],  crt_n);
	alm_rec_mode[crt_n]	=  0 ; /* 0---- ALM_REC_REAL1  */ 

	InitUnresetAlmFilter( &FilterUnresetAlm[crt_n]);

	strcpy(info[crt_n].event_font,"sans 12")  ;
	strcpy(info[crt_n].db_font,"sans 11")  ;
	strcpy(info[crt_n].alm_font,"sans 12")  ;



	for (evtidx=0;evtidx<EVENT_MAX_NUM;evtidx++)
	{
		PROGALARMTAB_PARA* evtpara=(PROGALARMTAB_PARA*)malloc(sizeof(PROGALARMTAB_PARA)); 

		evtpara->alm_canvas[crt_n]=NULL;
		evtpara->alm_draw_gc[crt_n]=NULL;
		evtpara->AlmOffDrawable[crt_n]=NULL;
		
		InitAlarmFilter( &(evtpara->FilterAlarm[crt_n]),  crt_n);

		evtpara->devMask[crt_n] = 0xffffffff;
	
		for (i=0;i<MAX_GROUP_MASK_NUM;i++)
		{
			evtpara->groupMask[crt_n][i]=0xffffffff;
		}
		
		
		
		
		
		ListProalarmtab_para[crt_n]=g_slist_append(ListProalarmtab_para[crt_n],evtpara);



	}

	

}



int ReadCommonDirXml(GtkMenu *menuCommonDir,int crt_n)
{
	xmlDocPtr   pdoc = NULL;
	xmlNodePtr  proot = NULL, pCommonDirNode = NULL,pNameNode = NULL;
	GtkTreeIter iter;
	char	err_str[256];
	xmlChar *key; 
	GtkWidget *items;
	int num = 0;

	xmlKeepBlanksDefault(0);
	pdoc = xmlReadFile (FILE_OIX_CONFIG_XML, "GB2312", XML_PARSE_NOBLANKS);
	if (pdoc == NULL)
    {
		sprintf(err_str, "Fail to open %s", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	proot = xmlDocGetRootElement (pdoc);
	if (proot == NULL)
    {
		sprintf(err_str, "%s not have root", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	pCommonDirNode = proot->xmlChildrenNode;
	while (pCommonDirNode != NULL)
    {
		if (!xmlStrcmp (pCommonDirNode->name, BAD_CAST "DspDefine"))
        {
			pNameNode = pCommonDirNode->xmlChildrenNode;
			while(pNameNode != NULL)
			{
				if((!xmlStrcmp(pNameNode->name, BAD_CAST "CommonDir")))
				{
					key=xmlNodeListGetString(pdoc, pNameNode->xmlChildrenNode, 1);
					if (key != NULL)
					{
						items = gtk_menu_item_new_with_label(key);	
						gtk_widget_show( items);
						gtk_container_add(GTK_CONTAINER(menuCommonDir),items);
						gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (ChgCommonDir), GINT_TO_POINTER(crt_n));
						strcpy(commonDirName,key);
						num++;
					}
					xmlFree(key);
				}
				if((!xmlStrcmp(pNameNode->name, BAD_CAST "ZJX")))
				{
					key=xmlNodeListGetString(pdoc, pNameNode->xmlChildrenNode, 1);
					if (key != NULL)
					{
						items = gtk_menu_item_new_with_label(key);	
						gtk_widget_show( items);
						gtk_container_add(GTK_CONTAINER(menuCommonDir),items);
						gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (ChgCommonDir), GINT_TO_POINTER(crt_n));
						strcpy(zjxName,key);
						num++;
					}
					xmlFree(key);	
				}
				if((!xmlStrcmp(pNameNode->name, BAD_CAST "Name")))
				{
					key=xmlNodeListGetString(pdoc, pNameNode->xmlChildrenNode, 1);
					if (key != NULL)
					{
						items = gtk_menu_item_new_with_label(key);	
						gtk_widget_show( items);
						gtk_container_add(GTK_CONTAINER(menuCommonDir),items);
						gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (ChgCommonDir), GINT_TO_POINTER(crt_n));
						num++;
					}		
					xmlFree(key);
				}
				pNameNode = pNameNode->next;
			}	
		}
		pCommonDirNode = pCommonDirNode->next;
    }
	xmlFreeDoc (pdoc);
	xmlCleanupParser ();
	return num;
}
