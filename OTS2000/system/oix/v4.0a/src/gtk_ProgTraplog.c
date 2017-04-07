
/********************   History of Modifications	**************************
 *
 *      by zyp for gtk 2005.12
 *	    WINDOWS HAVE SYN TO UNIX by chi 2007.09.05 ; 
 *********************************************************************************/

#include <gtk/gtk.h>
#include <gdk/gdk.h>


#include <stdio.h>

/*  #define	DEBUG   */
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

#ifndef F_OK
#define  F_OK  0
#endif 




extern void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength ,int crt_n)  ;
extern void PrintList(GtkWidget*parent,int crt_n,int prt_obj_type,char* filename);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void	DrawRcd (int crt_n, GdkDrawable	*drawable, GdkGC *gc, BACKRCD *rcd ,BOOL isTrutype);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern FILE    *OpenFileX (char    *full_name);
extern char *_fromUtf8(char *c);
extern void PopFileSelectionDlg(GtkWidget *parent ,int crt_n,char *title,char *dir);
extern int getMenuDataPointer(int crtn,int data);
extern int getCrtNumFromPointer(int data);
extern int getDataFromPointer(int data);
extern char *_toUtf8EX(char *c);
extern int get_app_name(enum app_code appid,char *log_filename);

static int log_count;
gint		LastPage[MAX_CRT_NUMBER]={0, 0, 0};

void DspLogList(GdkPixmap *, int  );
int	AdvancedSearch(int crt_n,DATE_TIME start_time,DATE_TIME end_time,char SearchObj[20]);


char     logstrbuf[5][MAX_LOG_INFO_SIZE];
GtkWidget *Trap_str_search_dlg[MAX_CRT_NUMBER],*Trap_str_advanced_search_dlg[MAX_CRT_NUMBER];


int log_page_numb[MAX_CRT_NUMBER], log_page_cnt[MAX_CRT_NUMBER];

#define LOG_COL  6
gchar *LOG_titles[LOG_COL] = {
    OIX_CSTR_SEQ_NO, OIX_CSTR_LABLE, OIX_CSTR_ERR_NO, OIX_CSTR_TIMER, OIX_CSTR_PROGRAM, OIX_CSTR_INFO
};

// const int LOG_COL=  sizeof(LOG_titles)/sizeof(LOG_titles[0]);


static char *type_name[]={"I","W","E"};
int type_num=sizeof(type_name)/sizeof(type_name[0]);

GdkPixmap *loglistOffDrawable[MAX_CRT_NUMBER] ;
GdkGC  *loglist_draw_gc[MAX_CRT_NUMBER];
GtkWidget *loglist_canvas[MAX_CRT_NUMBER];
static gint log_timeout_id[MAX_CRT_NUMBER];
static int select_start_id=0, select_end_id=0, print_select_tag[MAX_CRT_NUMBER];

void DestroyLogDlg(GtkWidget *w) {
    int crt_n;

    GetCrtNumberOfWgt( w, &crt_n );
    if( crt_n < 0 )
        return;

    if(log_timeout_id[crt_n]!=0) {
        gtk_timeout_remove(log_timeout_id[crt_n]);
        log_timeout_id[crt_n]= 0;
    }
	g_object_unref (loglistOffDrawable[crt_n]); //added by zyp on 2008.12.25 
    //gtk_widget_destroy(log_shell[crt_n]);
    log_shell[crt_n]=NULL;
    loglist_canvas[crt_n]=NULL;
	LOG_FILTER.prog =0;
    LOG_FILTER.type =0;
    LOG_FILTER.str =0;
}

void ExitLogDlg(GtkWidget *w) 
{
    int crt_n;

    GetCrtNumberOfWgt( w, &crt_n );
    if( crt_n < 0 )
        return;
  
	gtk_widget_destroy(log_shell[crt_n]);
}

void DrawTrapItemTag( int crt_n, GdkDrawable *drawable ) {
    int	i;
    BACKRCD trap_tag_rcd[MAX_CRT_NUMBER];
    
    if(print_select_tag[crt_n]==1) {
        select_end_id=select_start_id;
    }
    for(i=select_start_id;i<=select_end_id;i++) {
        trap_tag_rcd[crt_n].fg		= WHITE;
        trap_tag_rcd[crt_n].bg		= BLACK3;
        trap_tag_rcd[crt_n].rect.x1	= TRAP_LOG_X0;
        trap_tag_rcd[crt_n].rect.x2	= LOCAL_WIDTH;
        trap_tag_rcd[crt_n].rect.y1	= TRAP_LOG_Y0+(i+1)%LOG_PAGE_SIZE/*(i+1-log_page_cnt[crt_n] * LOG_PAGE_SIZE)*/*TRAP_LOG_ROW_HEIGHT-10;
        trap_tag_rcd[crt_n].rect.y2	= TRAP_LOG_Y0+(i+1)%LOG_PAGE_SIZE/*(i+1-log_page_cnt[crt_n] *LOG_PAGE_SIZE)*/*TRAP_LOG_ROW_HEIGHT+8;
    // printf("log_page_cnt=%d,i=%d\n",log_page_cnt[crt_n],i);   
       // printf("TRAP_LOG_Y0=%d,LOG_PAGE_SIZE=%d,TRAP_LOG_ROW_HEIGHT=%d\n",TRAP_LOG_Y0,LOG_PAGE_SIZE,TRAP_LOG_ROW_HEIGHT);
        // printf("y1=%d,y2=%d,cnt=%d\n",trap_tag_rcd[crt_n].rect.y1,trap_tag_rcd[crt_n].rect.y2,i-LOG_PAGE_SIZE*log_page_cnt[crt_n]);
         trap_tag_rcd[crt_n].draw_type	= DrwT_RECTANGLE;
         trap_tag_rcd[crt_n].fill.fill_type	= FILLED;
         trap_tag_rcd[crt_n].line_width	= 1;
//printf("###trap_tag_rcd[crt_n].rect.y1=%d,",trap_tag_rcd[crt_n].rect.y1);
         if(trap_tag_rcd[crt_n].rect.y1>TRAP_LOG_Y0&&trap_tag_rcd[crt_n].rect.y1<1024)
             DrawRcd( crt_n,  loglistOffDrawable[crt_n], loglist_draw_gc[crt_n], &trap_tag_rcd[crt_n], FALSE);
    }
}



void PrintTrapList(GtkWidget *widget, gpointer data) {
    int crt_n=(gint)data;
    
    if(print_select_tag[crt_n]!=2)
        {
            PopupMessageBox(log_shell[crt_n],MSG_NOT_SELECT_PRT_REGION);
        printf("please select print region\n");
        return;
    }
    else
	{
        //PrintList(log_shell[crt_n],crt_n,TRAP_LIST,TrapPrintFile);
		if(print_select_tag[crt_n]==2) 
		{
			FILE	*fp;

			if(fp = fopen(TrapPrintFile, "w"))
			{
				int	i;

				fprintf(fp, "%4s", _CS_(LOG_titles[0]));
				fprintf(fp, "%48s", _CS_(LOG_titles[1]));
				fprintf(fp, "%10s", _CS_(LOG_titles[2]));
				fprintf(fp, "%15s", _CS_(LOG_titles[3]));
				fprintf(fp, "%23s", _CS_(LOG_titles[4]));
				fprintf(fp, "%5s\n", _CS_(LOG_titles[5]));

				for(i=select_start_id;i<=select_end_id;i++) 
				{
					fprintf(fp, "%4s%8s%18s%32s%18s%s\n", real_log_rec[i].no, real_log_rec[i].errtype, real_log_rec[i].errcode,
						real_log_rec[i].datetime, real_log_rec[i].appid,  real_log_rec[i].info);
				}
				fclose(fp);
			}
		}
	}	   
}

gint SelectPrintRegion(GtkWidget *widget, GdkEvent *event , gpointer data) {
    int crt_n = (gint)data;
    int x, y, tmp_id;
    
    if (event->type == GDK_BUTTON_PRESS) {
        GdkEventButton *bevent = (GdkEventButton *) event;
        x = (int)(bevent->x) ;
        y = (int)(bevent->y) ;
        if (bevent->button == 1) /* Left MB is pressed*/ 
		{
            print_select_tag[crt_n]++;
            if(print_select_tag[crt_n]>2) 
			{
                print_select_tag[crt_n]=0;
                select_start_id=select_end_id=0;
            }
            
            if(print_select_tag[crt_n]==1)
			{
                select_start_id = MIN(log_count-1, log_page_cnt[crt_n] * LOG_PAGE_SIZE+(y-TRAP_LOG_Y0+TRAP_LOG_ROW_HEIGHT)/TRAP_LOG_ROW_HEIGHT-1);
				
			}
            else if(print_select_tag[crt_n]==2)                
                select_end_id = MIN(log_count-1, log_page_cnt[crt_n] * LOG_PAGE_SIZE+(y-TRAP_LOG_Y0/*-20*/+TRAP_LOG_ROW_HEIGHT)/TRAP_LOG_ROW_HEIGHT-1);
            
            if(select_end_id<select_start_id&&print_select_tag[crt_n]==2) {
                tmp_id= select_end_id;
                select_end_id= select_start_id;
                select_start_id= tmp_id;
            }
            //printf("^^^^^button 1 is pressed:x=%d,y=%d,start_id=%d,end_id=%d\n", x, y, select_start_id, select_end_id);
        }
        return FALSE;
    }
    return FALSE;
}

void GetLogstr(char	buffer[MAX_LOG_INFO_SIZE+100] ) {
    char *token;
    int i=0;
    
    //printf("tokenizing \"%s\" with strtok():\n", buffer);
    if ((token = strtok(buffer, ";")) != NULL) {
        strcpy(logstrbuf[i], token+7);
        i++;
        while ((token = strtok(NULL, ";")) != NULL) {
            /* printf("token = \"%s\"\n", token);*/
            if(i<4)
            strncpy(logstrbuf[i], token+8,MAX_LOG_INFO_SIZE-50);
            else
                strncpy(logstrbuf[i], token,MAX_LOG_INFO_SIZE-50);
            
            i++;
        }
    }

}


void ReadHistLogFile(int crt_n, char filename[MAX_FULL_FILE_NAME_LEN]) {
    int	i;
    char 	log_line[MAX_LOG_INFO_SIZE+100];
    FILE	*fp;
    
    /*printf("**** in ReadHistLogFile\n");*/
    fp	 = OpenFileX( filename );
    if ( fp == NULL) {
        gdk_beep();
        return;
    }
	log_rec_mode[crt_n]	=LOG_REC_HISTRY;
    i=0;
    for(;;) {
        if(NULL!=fgets(	log_line, sizeof(log_line), fp)) {
            GetLogstr(log_line);
          //  printf("&&&& logstrbuf=%s;%s;%s;%s;%s\n",logstrbuf[0],logstrbuf[1],logstrbuf[2],logstrbuf[3],logstrbuf[4]);
            sprintf(hist_log_rec[i].no, "%d", i);
           // strcpy(hist_log_rec[i].errtype, logstrbuf[0]);
			sprintf(hist_log_rec[i].errtype,"%8s", logstrbuf[0]);
           // strcpy(hist_log_rec[i].errcode, logstrbuf[1]);
			sprintf(hist_log_rec[i].errcode,"%8s", logstrbuf[1]);
            //strcpy(hist_log_rec[i].datetime, logstrbuf[2]);
			sprintf(hist_log_rec[i].datetime,"%32s", logstrbuf[2]);
            strncpy(hist_log_rec[i].appid, logstrbuf[3], sizeof(hist_log_rec[i].appid));
			//sprintf(hist_log_rec[i].appid,"%18s",logstrbuf[3]);
            strncpy(hist_log_rec[i].info, logstrbuf[4],MAX_LOG_INFO_SIZE-50);
       /*     printf("logstrbuf[%d]=%s; %s;%s;%s;%s,strlen=%d\n",i,hist_log_rec[i].errtype,hist_log_rec[i].errcode,hist_log_rec[i].datetime,hist_log_rec[i].appid,hist_log_rec[i].info,strlen(hist_log_rec[i].errtype));	
          */  
			
            i++;
			if(i>MAX_LOG_NUM)
			{
				printf("log count overflow!\n");
				break ;
			}
        }
        else
            break;
    }
    fclose( fp );
    log_count = i;
    log_page_numb[crt_n] = log_count/LOG_PAGE_SIZE +1;
    /*printf("&&&& ReadHistLogFile: log_count=%d\n",log_count);*/
}

gint
SetHistLogBtn(GtkWidget *widget, gpointer data) {
    int crt_n=0;    
    gchar txt[10];
	gchar *pConvert=0;
    
    GetCrtNumberOfWgt( widget, &crt_n );
    if( crt_n < 0 )
        return FALSE;
    strcpy(txt, gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(data)->entry)));
    strcpy(txt, pConvert=_fromUtf8(txt));
	G_SAFE_FREE(pConvert);
    GetCrtNumberOfWgt( widget, &crt_n );//add by hcl 
//printf("###in SetHistLogBtn::txt=%s,OIX_CSTR_REALTIME_REC=%s,OIX_CSTR_HIST_REC=%s\n",txt,OIX_CSTR_REALTIME_REC,OIX_CSTR_HIST_REC);
	
    if(strcmp(txt, _CS_(OIX_CSTR_REALTIME_REC))==0) 
    {
        log_rec_mode[crt_n]	= LOG_REC_REAL;
        printf("*****SetHistLogBtn:: LOG_REC_REAL is selected\n");
    }
    else if(strcmp(txt, _CS_(OIX_CSTR_HIST_REC))==0) 
    {
		log_rec_mode[crt_n]	=LOG_REC_HISTRY;
        file_mode[crt_n]       = LOG_HIST_FILE;  
        log_page_cnt[crt_n]	= 0;
        log_count		=0;
        PopFileSelectionDlg(main_w[crt_n], crt_n, _CS_(OIX_STR_TRAP_FILE_SELECT_BOX), H9000_HIS_TRAP);        
        printf("****in 	SetHistLogBtn::: LOG_REC_HISTRY\n"); 
    }
    DspLogList( loglistOffDrawable[crt_n], crt_n);
    return TRUE;
}

void DrawLogString(GdkPixmap *drawable, int crt_n, TRAPLOG traplog, int i) 
{  
	int y;
	y= TRAP_LOG_Y0+(i+1)*TRAP_LOG_ROW_HEIGHT;
    windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0, y, traplog.no ,strlen(traplog.no),crt_n);
    windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0+TRAP_LOG_COL_WIDTH1-20, y, traplog.errtype,strlen(traplog.errtype),crt_n  );
    windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0+TRAP_LOG_COL_WIDTH1*2-10, y, traplog.errcode, strlen(traplog.errcode) ,crt_n);
//#ifdef SUN_OS
	windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0+TRAP_LOG_COL_WIDTH1*3-40, y, traplog.datetime,strlen (traplog.datetime),crt_n);
 //printf("%%%%traplog.appid=%s\n",traplog.appid);  
	windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X4, y, traplog.appid ,strlen( traplog.appid ),crt_n);
    windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X4+TRAP_LOG_COL_WIDTH1, y+10, traplog.info  ,strlen( traplog.info),crt_n);
/*#else
    windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0+TRAP_LOG_COL_WIDTH1*3-20, y, traplog.datetime,strlen (traplog.datetime));
    windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X4-40, y, traplog.appid ,strlen( traplog.appid ));
    windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X4+TRAP_LOG_COL_WIDTH1-60, y, traplog.info  ,strlen( traplog.info));
#endif*/
	 //printf("info=%s,y=%d\n",traplog.info,y);
}

TRAPLOG FilterTraplog[MAX_LOG_NUM];

void DspLogList(GdkPixmap *drawable, int crt_n) {
    long tloc;
    time_t now_time;
    struct tm now_time_ptr;
    struct log_info log_rec;
    int i, j, file_no;
    int i0=0, i1=0;
    char	tmp1[256];
   
    gdk_gc_set_foreground( loglist_draw_gc[crt_n], &pixels[crt_n][L_BLUE1]);
    
    for(j=0;j<LOG_COL;j++) 
    {
        if(j<4)
          windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0+TRAP_LOG_COL_WIDTH1*j, TRAP_LOG_Y0, _CS_(LOG_titles[j]) , strlen(_CS_(LOG_titles[j])) ,crt_n);
		/*else if(j==1)
		windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0+TRAP_LOG_COL_WIDTH1*j, TRAP_LOG_Y0, _CS_(LOG_titles[j]) , strlen(_CS_(LOG_titles[j])) );
		else if(j==2)
			windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0+TRAP_LOG_COL_WIDTH1*j, TRAP_LOG_Y0, _CS_(LOG_titles[j]) , strlen(_CS_(LOG_titles[j])) );
		else if(j==3)
			windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X0+TRAP_LOG_COL_WIDTH1*j, TRAP_LOG_Y0, _CS_(LOG_titles[j]) , strlen(_CS_(LOG_titles[j])) );
		*/
		else if(j==4)
            windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X4, TRAP_LOG_Y0, _CS_(LOG_titles[j]) , strlen(_CS_(LOG_titles[j])) ,crt_n);
        else if(j==5)
            windk_draw_text( drawable, font_set[0], loglist_draw_gc[crt_n], TRAP_LOG_X4+TRAP_LOG_COL_WIDTH1+100, TRAP_LOG_Y0-20, _CS_(LOG_titles[j]) , strlen(_CS_(LOG_titles[j])),crt_n );
    
    }
    now_time = time(&tloc);
    now_time_ptr = *localtime(&now_time);
    
    if ( log_rec_mode[crt_n] == LOG_REC_REAL ) 
    {        
        log_count =GetSysLogCount();        
        
        if(/*log_count<(log_page_cnt[crt_n]*LOG_PAGE_SIZE) ||*/(LastPage[crt_n] == TRUE))
            log_page_cnt[crt_n] = log_count/LOG_PAGE_SIZE;             
        
        file_no = log_count/MAX_LOG_NUM+1;       
        sprintf(select_hist_log_file[crt_n], "/home/ems/h9000/his/trap/H9000LOG%04d-%02d-%02d%03d.LOG",now_time_ptr.tm_year+1900,
        now_time_ptr.tm_mon+1, now_time_ptr.tm_mday,file_no);        
    }
    //else
        //printf("in DspLogList::: LOG_REC_HIST\n");
    
    gdk_gc_set_foreground( loglist_draw_gc[crt_n], &pixels[crt_n][GREEN3]);    
    
    log_cur_page_startid= log_page_cnt[crt_n] * LOG_PAGE_SIZE;
    log_cur_page_endid	= MIN(( log_page_cnt[crt_n]+1)*LOG_PAGE_SIZE, log_count );
    
    if(print_select_tag[crt_n]!=0)
        DrawTrapItemTag(crt_n, drawable);    
  if(log_count>MAX_LOG_NUM)
  {
	  printf("log count overflow!\n");
	  return ;
  }
    for(i=0;i<log_count;i++)
    {
        if ( log_rec_mode[crt_n] == LOG_REC_REAL ) 
        {
            //log_rec	= shm_log_addr->shm_log_seg[i];
			if(-1==GetSysLogInfo(i, &log_rec))
				continue;

            sprintf(real_log_rec[i].no, "%d", i);
            switch (log_rec.logType) 
            {
                case H9000_LOG_ERROR:
                   // strcpy(real_log_rec[i].errtype, "E");
					sprintf(real_log_rec[i].errtype,"%8s","E");
                    gdk_gc_set_foreground( loglist_draw_gc[crt_n], &pixels[crt_n][RED]);
                    break;
                case H9000_LOG_WARN:
                    gdk_gc_set_foreground( loglist_draw_gc[crt_n], &pixels[crt_n][YELLOW]);
                    //strcpy(real_log_rec[i].errtype, "W");
					sprintf(real_log_rec[i].errtype,"%8s","W");
                    break;
                case H9000_LOG_INFO:
                   // strcpy(real_log_rec[i].errtype, "I");
					sprintf(real_log_rec[i].errtype,"%8s","I");
                    break;
                default:
                    printf("!!!!! errtype err,%d\n", log_rec.logType);
                    break;
            }
            sprintf(real_log_rec[i].errcode, "%8d", 0);
            sprintf(tmp1, "%d-%d-%d %d:%d:%d:%d", log_rec.datetime.year,
            log_rec.datetime.month, log_rec.datetime.day, log_rec.datetime.hour,
            log_rec.datetime.minute, log_rec.datetime.second, log_rec.datetime.ms);
			sprintf(real_log_rec[i].datetime,"%32s",tmp1);
           
            GetModuleName(log_rec.module_id, real_log_rec[i].appid);
			//sprintf(real_log_rec[i].appid,"%18s",tmp1);
			//printf("$$$$$appid=%s\n",real_log_rec[i].appid);
            sprintf(real_log_rec[i].info, "%s", log_rec.info);
        }
        else
            real_log_rec[i] = hist_log_rec[i];        
 
        gdk_gc_set_foreground( loglist_draw_gc[crt_n], &pixels[crt_n][GREEN3]);
        gdk_gc_set_background( loglist_draw_gc[crt_n], &pixels[crt_n][RED]);
        if(LOG_FILTER.prog==0&&LOG_FILTER.type==0&&LOG_FILTER.str==0) 
        {             
            if(i>=log_cur_page_startid&&i<log_cur_page_endid)
            {       
                DrawLogString(drawable, crt_n, real_log_rec[i], i0);
                i0++;
            }            
        }
        else 
        {
            if(LOG_FILTER.prog==1) 
            {
                if(LOG_FILTER.type==0) 
                {                   
                    if( strcmp(real_log_rec[i].appid, LOG_FILTER.prog_name)==0) 
                    {
                        FilterTraplog[i1]=real_log_rec[i];
                        i1++;
                    }
                }
                else 
                {
                    if(LOG_FILTER.type==1 && strcmp(real_log_rec[i].errtype, LOG_FILTER.type_name)==0
                    &&strcmp(real_log_rec[i].appid, LOG_FILTER.prog_name)==0) 
                    {
                        FilterTraplog[i1]=real_log_rec[i];
                        i1++;
                    }
                }
            }
            else 
            {
				printf("here is str_name,info=%s,LOG_FILTER.str=%d,LOG_FILTER.str_name=%s\n",real_log_rec[i].info,LOG_FILTER.str,LOG_FILTER.str_name);
                if(LOG_FILTER.type==1 && strcmp(real_log_rec[i].errtype, LOG_FILTER.type_name)==0) 
                {
                    FilterTraplog[i1]=real_log_rec[i];
                    i1++;
                }
				else if( LOG_FILTER.str==1 && strstr(real_log_rec[i].info,LOG_FILTER.str_name)!=0)
				{
					FilterTraplog[i1]=real_log_rec[i];
					printf("search_str=%s\n",LOG_FILTER.str_name);
                    i1++;
				}
            }
        }
    }
    if(LOG_FILTER.prog!=0||LOG_FILTER.type!=0||LOG_FILTER.str!=0) 
    {        
        if(LastPage[crt_n] == TRUE)
            log_page_cnt[crt_n]=i1/LOG_PAGE_SIZE;
        log_cur_page_startid= log_page_cnt[crt_n] * LOG_PAGE_SIZE;
        log_cur_page_endid	= MIN(( log_page_cnt[crt_n]+1)*LOG_PAGE_SIZE, i1 );
        for(i=log_cur_page_startid;i<log_cur_page_endid;i++)
		{
            DrawLogString(drawable, crt_n, FilterTraplog[i], i%LOG_PAGE_SIZE);
		}
    }
}

gint	TrapLogDspProc1(GdkPixmap *drawable, int crt_n ) {
    if (! log_shell[crt_n] ) {
        gtk_timeout_remove(log_timeout_id[crt_n]);
        log_timeout_id[crt_n]= 0;
        return FALSE;
    }
    log_page_numb[crt_n]	= log_count/LOG_PAGE_SIZE +1;
    
    DspLogList(drawable, crt_n);

	return TRUE;
}
gint  TrapLogDspProc  (int crt_n  ) 
{
    gdk_gc_set_foreground(loglist_draw_gc[crt_n], &pixels[crt_n][BLACK]);
    gdk_draw_rectangle(loglistOffDrawable[crt_n], loglist_draw_gc[crt_n], TRUE, 0, 0, LOCAL_WIDTH, LOCAL_HEIGHT);
    TrapLogDspProc1(loglistOffDrawable[crt_n], crt_n);
    gdk_window_copy_area(loglist_canvas[crt_n]->window, loglist_draw_gc[crt_n], 0, 0, loglistOffDrawable[crt_n], 0, 0, LOCAL_WIDTH, LOCAL_HEIGHT);
    return TRUE;
}

void GetLogFile(GtkWidget * widget, gpointer data) {
    int crt_n;
    long tloc;
    time_t tTim, now_time;
    struct tm file_time, now_time_ptr;
    
    char tmp_title[100];
    char cur_sel_file[50], tmp_file[50], tmp[10], tmp1[25], tmp2[5], tmp3[5], tmp4[5];
    int  year, month, date, file_no;
	//char tmp_str[100];
    crt_n = 0 ;
    file_mode[crt_n]       = LOG_HIST_FILE;
    
    
	
    strncpy(tmp1, select_hist_log_file[crt_n]+strlen("/home/ems/h9000/his/trap/H9000LOG"), strlen("yyyy-mm-dd001"));
    
    strncpy(tmp, tmp1, 4);
    year = atoi(tmp);
    
    strncpy(tmp4, tmp1+strlen("yyyy-"), 2);
    month = atoi(tmp4);
    
    strncpy(tmp2, tmp1+strlen("yyyy-mm-"), 2);
    
    date = atoi(tmp2);
    
    strncpy(tmp3, tmp1+strlen("year-mm-dd"), 3);
    
    file_no = atoi(tmp3);
    
    now_time = time(&tloc);
    now_time_ptr = *localtime(&now_time);
    
    file_time.tm_sec =0;
    file_time.tm_min =0;
    file_time.tm_hour=2;
    file_time.tm_mday= date;
    file_time.tm_mon= month-1;
    file_time.tm_year = year-1900;
    file_time.tm_isdst = 0;
    
    for(;;) {
        if((tTim = mktime(&file_time))!=(time_t)-1) {
            if((int)data==0)
                tTim =tTim-3600*24; /* the Prev file */
            else
                tTim =tTim+3600*24; /* the back file */
            file_time = *localtime(&tTim);
            year = file_time.tm_year+1900;
            month = file_time.tm_mon+1;
            date = file_time.tm_mday;
            
            if(file_time.tm_year>=now_time_ptr.tm_year && file_time.tm_mon>=now_time_ptr.tm_mon
            && file_time.tm_mday>now_time_ptr.tm_mday) {
                printf("this is the last file \n");
                break;
            }
            if(year<2000) {
                printf("this is the first file\n");
                break;
            }
        }
        else {
            printf("mktime error\n");
            break;
        }
        
        strcpy(cur_sel_file, select_hist_log_file[crt_n]);
        
        sprintf(tmp, "%02d", date);
        strncpy(cur_sel_file+strlen("/home/ems/h9000/his/trap/H9000LOGyyyy-mm-"), tmp, 2);
        sprintf(tmp, "%02d", month);
        strncpy(cur_sel_file+strlen("/home/ems/h9000/his/trap/H9000LOGyyyy-"), tmp, 2);
        sprintf(tmp, "%04d", year);
        strncpy(cur_sel_file+strlen("/home/ems/h9000/his/trap/H9000LOG"), tmp, 4);
        
        sprintf(tmp, "%03d", 1);
        strncpy(cur_sel_file+strlen("/home/ems/h9000/his/trap/H9000LOGyyyy-mm-dd"), tmp, 3);
        if(month==12)
            printf("2: cur_sel_file=%s\n", cur_sel_file);
        
        if(access(cur_sel_file, F_OK)!=-1) {
            printf("####cur_sel_file=%s is existed!!!\n", cur_sel_file);
            break;
        }
    }
    /*printf("###cur_sel_file=%s\n", cur_sel_file);*/
    if(access(cur_sel_file, F_OK)==-1) {
        printf("!!!! no this file %s\n", cur_sel_file);
        gdk_beep();
        return;
    }
    else {
        for(file_no=1;file_no<999;file_no++) {
            sprintf(tmp, "%03d", file_no);
            strcpy(tmp_file, cur_sel_file);
            
            strncpy(cur_sel_file+strlen("/home/ems/h9000/his/trap/H9000LOGyyyy-mm-dd"), tmp, 3);
            if(month==12 &&year==2005)
                printf("!!!: cur_sel_file=%s,tmp=%s\n", cur_sel_file, tmp);
            if(access(cur_sel_file, F_OK)==-1) {
                strcpy(cur_sel_file, tmp_file);
                printf("::::cur_sel_file=%s\n", cur_sel_file);
                break;
            }
        }
        if(access(cur_sel_file, F_OK)!=-1)
            printf("@@@@ find the prev file OK: cur_sel_file=%s\n", cur_sel_file);
    }
    strcpy( select_hist_log_file[crt_n], cur_sel_file);
    
    ReadHistLogFile(crt_n, cur_sel_file);
    strcpy(tmp_title, "TrapLog------");
    strcat(tmp_title, cur_sel_file);
    
    gtk_window_set_title(GTK_WINDOW(log_shell[crt_n]), tmp_title);
    
	
}


void	LogBarIncrease(GtkWidget *w, gpointer data) {
    GtkWidget *page_up , *page_down ,*toolbar,*last_page_btn; 
	int	crt_n=0;

	toolbar =  ( GtkWidget* )data ;
    page_up = gtk_object_get_data(GTK_OBJECT(toolbar), "pageup");
	page_down=gtk_object_get_data(GTK_OBJECT(toolbar), "pagedown");
	last_page_btn=gtk_object_get_data(GTK_OBJECT(toolbar), "lastpage");

    GetCrtNumberOfWgt( w, &crt_n );
    if( crt_n < 0 )
        return ;
    printf("####log_page_cnt=%d\n",log_page_cnt[crt_n]);
    if ( (log_page_cnt[crt_n] < log_page_numb[crt_n]-1)/*|| (log_page_cnt[crt_n]==0)*/ )
	{
        log_page_cnt[crt_n]++;
		gtk_widget_set_sensitive(page_up, TRUE);
	}
    else
	{
        gdk_beep(  );
		gtk_widget_set_sensitive(page_down, FALSE);
	}
    gtk_widget_set_sensitive(last_page_btn, TRUE);
    LastPage[crt_n]= FALSE;
    printf("in LogBarIncrease!!! log_page_cnt=%d,LastPage[%d]=%d\n",	 log_page_cnt[crt_n], crt_n, LastPage[crt_n]);
    TrapLogDspProc(crt_n);
    
}

void	LogBarDecrease(GtkWidget *w, gpointer data) {
    GtkWidget *page_up , *page_down,*toolbar,*last_page_btn ; 
	int	crt_n=0;
	toolbar =  ( GtkWidget* )data ;
    page_up = gtk_object_get_data(GTK_OBJECT(toolbar), "pageup");
	page_down=gtk_object_get_data(GTK_OBJECT(toolbar), "pagedown");

	last_page_btn=gtk_object_get_data(GTK_OBJECT(toolbar), "lastpage");
	
    GetCrtNumberOfWgt( w, &crt_n );
    if( crt_n < 0 )
        return;

    printf("####log_page_cnt=%d\n",log_page_cnt[crt_n]);

    if ( log_page_cnt[crt_n] > 0 )
	{
        log_page_cnt[crt_n]--;
		gtk_widget_set_sensitive(page_down, TRUE);
	}
    else
	{
        gdk_beep(  );
		gtk_widget_set_sensitive(page_up, FALSE);		
	}
    gtk_widget_set_sensitive(last_page_btn, TRUE);
    LastPage[crt_n]= FALSE;
    
    printf("in LogBarDecrease!!!log_page_cnt=%d,LastPage[%d]=%d\n",	 log_page_cnt[crt_n], crt_n, LastPage[crt_n]);
    TrapLogDspProc(crt_n);
}

void LastPageBtnPressed  (GtkToggleButton *w,   gpointer         user_data) 
{
	GtkWidget *prev , *next ; 
    GtkWidget *toolbar  ;
    int crt_n ;
    GetCrtNumberOfWgt(GTK_WIDGET(w), &crt_n );
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
        return  ;
    toolbar =  ( GtkWidget* )user_data ;
    prev = gtk_object_get_data(GTK_OBJECT(toolbar), "pageup");
    next = gtk_object_get_data(GTK_OBJECT(toolbar), "pagedown");
    LastPage[crt_n]=	gtk_toggle_button_get_active(w);
    printf("in LastPageBtnPressed: LastPage[crt_n]=%d\n", LastPage[crt_n]);
    if (LastPage[crt_n]) {
        gtk_widget_set_sensitive(prev, FALSE);
        gtk_widget_set_sensitive(next, FALSE);
    }
    else {
        gtk_widget_set_sensitive(prev, TRUE);
        gtk_widget_set_sensitive(next, TRUE);
    }
}

int InitLogFilter(GtkWidget * widget, gpointer data) {
    int i = (gint) data ;
    int crt_n ;
    //GetCrtNumberOfWgt(widget, &crt_n );
    crt_n=(gint) data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
        return FALSE;
    
    LOG_FILTER.prog=0;

	return FALSE;   
}

void
InitLogTypeFilter(GtkWidget * widget, gpointer data) {
    int i = (gint) data ;
    int crt_n ;
    //GetCrtNumberOfWgt(widget, &crt_n );
    crt_n=(gint)data;
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
        return ;
    
    LOG_FILTER.type=0;
    LastPage[crt_n] = TRUE;
}

void
GetProgName(GtkWidget * widget, gpointer data) {
    int i  ;
    int crt_n ;
   //GetCrtNumberOfWgt(widget, &crt_n );
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);

	//i = (gint) data ;
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
        return ;
    
    LOG_FILTER.prog=1;
    GetModuleName(i+1, LOG_FILTER.prog_name);
    LastPage[crt_n] = TRUE;
}

void
GetTypeName(GtkWidget * widget, gpointer data) {
    int i  ;
    int crt_n ;
    //GetCrtNumberOfWgt(widget, &crt_n );
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	
    //i = (gint) data ;
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
        return ;
    
    LOG_FILTER.type=1;
    strcpy(LOG_FILTER.type_name, type_name[i]);
    LastPage[crt_n] = TRUE;
}



void DestroyTrapStrSearchDlg(GtkWidget *w, gpointer data)
{	
	int crt_n= (int)data;
        
	gtk_widget_destroy(Trap_str_search_dlg[crt_n]);
    Trap_str_search_dlg[crt_n]=NULL;
}

void DestroyTrapStrAdvancedSearchDlg(GtkWidget *w, gpointer data)
{	
	int crt_n= (int)data;
        
	gtk_widget_destroy(Trap_str_advanced_search_dlg[crt_n]);
    Trap_str_advanced_search_dlg[crt_n]=NULL;
}

void SearchTrapStr(GtkWidget *w, gpointer data)
{
	int crt_n;
//	char		search_str[128];
//	GtkWidget *SearchStr_entry;
	GtkWidget	*str_entry;


	crt_n = (gint)data;
	str_entry = g_object_get_data(G_OBJECT(Trap_str_search_dlg[crt_n]),"SearchStr_entry");
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(str_entry)),"")!=0)        
               
		strcpy(LOG_FILTER.str_name, gtk_entry_get_text(GTK_ENTRY(str_entry)));
}

void GetAdvancedSearchData(GtkWidget *w, gpointer data)
{
	int crt_n;
	char		tmp_str[128];
//	GtkWidget *SearchStr_entry;
	GtkWidget	*str_entry;
	DATE_TIME start_time,end_time;
	int y,m,d;


	crt_n = (gint)data;
	str_entry = g_object_get_data(G_OBJECT(Trap_str_advanced_search_dlg[crt_n]),"StartTime_entry");
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(str_entry)),"")!=0)    
	{
		strcpy(tmp_str, gtk_entry_get_text(GTK_ENTRY(str_entry)));
		printf("tmp_str=%s\n",tmp_str);
		//sscanf(tmp_str,"%d-%d-%d",&start_time.year,&start_time.month,&start_time.day);
		sscanf(tmp_str,"%d-%d-%d",&y,&m,&d);
		printf("y=%d,mon=%d,day=%d\n",y,m,d);
		start_time.year = y;
		start_time.month=m;
		start_time.day=d;
	}
	str_entry = g_object_get_data(G_OBJECT(Trap_str_advanced_search_dlg[crt_n]),"EndTime_entry");
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(str_entry)),"")!=0)    
	{
		strcpy(tmp_str, gtk_entry_get_text(GTK_ENTRY(str_entry)));
		sscanf(tmp_str,"%d-%d-%d",&y,&m,&d);
		end_time.year = y;
		end_time.month=m;
		end_time.day=d;
	}
	str_entry = g_object_get_data(G_OBJECT(Trap_str_advanced_search_dlg[crt_n]),"AdvancedSearchStr_entry");
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(str_entry)),"")!=0)    
	{
		strcpy(tmp_str, gtk_entry_get_text(GTK_ENTRY(str_entry)));		
	}
	AdvancedSearch(crt_n,start_time,end_time,tmp_str);
}

void PopupPromptDlg(GtkWidget *w, int crt_n )
{
	GtkWidget *button_ack, *button_exit;
//	char tmp_str[30];
	GtkWidget *StartTime_entry,*EndTime_entry,*AdvancedSearchStr_entry;
    GtkWidget *start_time_lab,*end_time_lab,*data_lab;
	gchar *pConvert=0;
    
        if(Trap_str_advanced_search_dlg[crt_n])
            {
            gtk_window_present(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]));
            return;
        }
        
	Trap_str_advanced_search_dlg[crt_n] = gtk_dialog_new();
	 
	gtk_signal_connect(GTK_OBJECT(Trap_str_advanced_search_dlg[crt_n]),
        "destroy",  GTK_SIGNAL_FUNC(DestroyTrapStrAdvancedSearchDlg),  (gpointer)  crt_n );

	gtk_window_set_policy(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]),FALSE,FALSE,FALSE);
	gtk_window_set_title(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]),"");
	gtk_window_set_transient_for(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]), GTK_WINDOW(log_shell[crt_n]));
	gtk_window_set_destroy_with_parent(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]),TRUE);

	start_time_lab = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_STIME ));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (start_time_lab); 
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_advanced_search_dlg[crt_n])->vbox),start_time_lab,TRUE,TRUE,0);
	StartTime_entry =  gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_advanced_search_dlg[crt_n])->vbox),StartTime_entry,TRUE,TRUE,0); 
	gtk_window_set_focus(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]), StartTime_entry);
 	gtk_window_activate_focus(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]));
             
   
    g_object_set_data(G_OBJECT(Trap_str_advanced_search_dlg[crt_n]),"StartTime_entry",StartTime_entry);
	
	end_time_lab = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_ETIME));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (end_time_lab);
 	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_advanced_search_dlg[crt_n])->vbox),end_time_lab,TRUE,TRUE,0);
	EndTime_entry =  gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_advanced_search_dlg[crt_n])->vbox),EndTime_entry,TRUE,TRUE,0); 
	gtk_window_set_focus(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]), EndTime_entry);
 //	gtk_window_activate_focus(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]));
             
   
    g_object_set_data(G_OBJECT(Trap_str_advanced_search_dlg[crt_n]),"EndTime_entry",EndTime_entry);

//	sprintf(tmp_str,"%s£º",OIX_CSTR_DATA_INPUT);
	data_lab = gtk_label_new (pConvert=_toUtf8EX(OIX_CSTR_SEARCH_STR));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (data_lab);
 	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_advanced_search_dlg[crt_n])->vbox),data_lab,TRUE,TRUE,0);

	
               
	AdvancedSearchStr_entry =  gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_advanced_search_dlg[crt_n])->vbox),AdvancedSearchStr_entry,TRUE,TRUE,0); 
	gtk_window_set_focus(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]), AdvancedSearchStr_entry);
 //	gtk_window_activate_focus(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]));
             
   
    g_object_set_data(G_OBJECT(Trap_str_advanced_search_dlg[crt_n]),"AdvancedSearchStr_entry",AdvancedSearchStr_entry);
 	       
	
	button_ack = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_advanced_search_dlg[crt_n])->action_area),button_ack,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT(button_ack), "clicked", GTK_SIGNAL_FUNC(GetAdvancedSearchData), (gpointer)crt_n);

	button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_advanced_search_dlg[crt_n])->action_area),button_exit,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT (button_exit), "clicked",
			       GTK_SIGNAL_FUNC(DestroyTrapStrAdvancedSearchDlg), (gpointer)crt_n);

	gtk_window_set_position(GTK_WINDOW(Trap_str_advanced_search_dlg[crt_n]),GTK_WIN_POS_CENTER);
	gtk_widget_show_all(Trap_str_advanced_search_dlg[crt_n]);

}

void PopupSearchStrDlg(GtkWidget *w, int crt_n )
{
	GtkWidget *button_ack, *button_exit;	
	GtkWidget *SearchStr_entry;
	gchar *pConvert=0;
        
        if(Trap_str_search_dlg[crt_n])
            {
            gtk_window_present(GTK_WINDOW(Trap_str_search_dlg[crt_n]));
            return;
        }
        
	Trap_str_search_dlg[crt_n] = gtk_dialog_new();
	 
	gtk_signal_connect(GTK_OBJECT(Trap_str_search_dlg[crt_n]),
        "destroy",  GTK_SIGNAL_FUNC(DestroyTrapStrSearchDlg),  (gpointer)  crt_n );

	gtk_window_set_policy(GTK_WINDOW(Trap_str_search_dlg[crt_n]),FALSE,FALSE,FALSE);
	gtk_window_set_title(GTK_WINDOW(Trap_str_search_dlg[crt_n]),"");
	gtk_window_set_transient_for(GTK_WINDOW(Trap_str_search_dlg[crt_n]), GTK_WINDOW(log_shell[crt_n]));
	gtk_window_set_destroy_with_parent(GTK_WINDOW(Trap_str_search_dlg[crt_n]),TRUE);	
	
               
	SearchStr_entry =  gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_search_dlg[crt_n])->vbox),SearchStr_entry,TRUE,TRUE,0); 
	gtk_window_set_focus(GTK_WINDOW(Trap_str_search_dlg[crt_n]), SearchStr_entry);
 	gtk_window_activate_focus(GTK_WINDOW(Trap_str_search_dlg[crt_n]));
             
   
    g_object_set_data(G_OBJECT(Trap_str_search_dlg[crt_n]),"SearchStr_entry",SearchStr_entry);
 	       
	
	button_ack = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_search_dlg[crt_n])->action_area),button_ack,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT(button_ack), "clicked", GTK_SIGNAL_FUNC(SearchTrapStr), (gpointer)crt_n);

	button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Trap_str_search_dlg[crt_n])->action_area),button_exit,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT (button_exit), "clicked",
			       GTK_SIGNAL_FUNC(DestroyTrapStrSearchDlg), (gpointer)crt_n);

	gtk_window_set_position(GTK_WINDOW(Trap_str_search_dlg[crt_n]),GTK_WIN_POS_CENTER);
	gtk_widget_show_all(Trap_str_search_dlg[crt_n]);
}


void
GetSearchStr(GtkWidget * widget, gpointer data) {
    int i  ;
    int crt_n ;
    //GetCrtNumberOfWgt(widget, &crt_n );
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	
    //i = (gint) data ;
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
        return ;
    
    LOG_FILTER.str=1;
	PopupSearchStrDlg(widget,  crt_n );
  //  strcpy(LOG_FILTER.str_name, "Function"/*str_name[i]*/);
    LastPage[crt_n] = TRUE;
}

void InitTrapFilter(int crt_n)
{
	LOG_FILTER.prog =0;
    LOG_FILTER.type =0;
    LOG_FILTER.str =0;
}

GtkWidget *
CreateTrapLogToolbar(GtkWidget * parent) {
    int	crt_n, i ;
    GtkWidget *box, *tempwid;
    GtkWidget *toolbar, *cmblogType, *button_page_up, *button_page_down, *last_page_btn;
    GtkWidget  *optionmenu, *menu, *items;
    GList *cmbloglist=NULL;
	GtkWidget *Advanced_Search_button,*Search_button;

    GtkWidget *Prev_button, *next_button, *button_exit, *button_print,*button_init_filter;
    GdkColor btn_color;
	int MenuPointer;
	gchar *pConvert=0;
    gdk_color_parse("red",&btn_color);
    
    GetCrtNumberOfWgt( parent, &crt_n );
    if( crt_n < 0 )
        return FALSE;
    
    toolbar = gtk_handle_box_new();
    
    box = gtk_hbox_new(FALSE, 4);
    gtk_container_add(GTK_CONTAINER(toolbar), box);
    gtk_container_border_width(GTK_CONTAINER(box), 5);
    gtk_widget_show(box);
    
    tempwid = gtk_label_new(pConvert=_toUtf8EX(OIX_CSTR_RECORD_TYPE));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_start(GTK_BOX(box), tempwid, FALSE, FALSE, 0);
    gtk_widget_show(tempwid);
    
    /*  *************  combobox  ****************** */
    
    cmbloglist = g_list_append(cmbloglist, pConvert=_toUtf8EX(OIX_CSTR_REALTIME_REC));
	G_SAFE_FREE(pConvert);
    cmbloglist = g_list_append(cmbloglist, pConvert=_toUtf8EX(OIX_CSTR_HIST_REC));
	G_SAFE_FREE(pConvert);
    
    cmblogType = gtk_combo_new();
    gtk_combo_set_case_sensitive(GTK_COMBO(cmblogType), 1);
    gtk_widget_set_usize(cmblogType, 130, -1);
    
    gtk_combo_set_popdown_strings(GTK_COMBO(cmblogType), cmbloglist);
    g_signal_connect(GTK_OBJECT(GTK_COMBO(cmblogType)->entry), "changed",
    G_CALLBACK(SetHistLogBtn), cmblogType);
    
    cmbloglist = NULL ;
    
    gtk_combo_disable_activate(GTK_COMBO(cmblogType));
    gtk_box_pack_start(GTK_BOX(box), cmblogType, FALSE, FALSE, 0);
    gtk_widget_show(cmblogType);
    
    optionmenu = gtk_option_menu_new();
    gtk_widget_show(optionmenu);
    gtk_box_pack_start(GTK_BOX(box), optionmenu, FALSE, FALSE, 0);
    menu =gtk_menu_new();
    gtk_widget_set_usize(menu, 130, -1);
    
    
    items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALL_PROG));
	G_SAFE_FREE(pConvert);
    gtk_widget_show( items);
    gtk_container_add(GTK_CONTAINER(menu), items);
    gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(InitLogFilter), (gpointer) crt_n);
    
    for(i=1;i<=MAX_H9000_MODULE_NUM;i++) 
	{
		char moduleName[MAX_H9000_MODULE_NAME_SIZE];

		GetModuleName(i, moduleName);
        if(moduleName[0])
        {
            items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(moduleName));
			G_SAFE_FREE(pConvert);
            gtk_widget_show( items);
            gtk_container_add(GTK_CONTAINER(menu), items);
           MenuPointer=getMenuDataPointer(crt_n,i);
            gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(GetProgName), (gpointer) MenuPointer);
        }
    }
    
    gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), menu);
    
    optionmenu = gtk_option_menu_new();
    gtk_widget_show(optionmenu);
    gtk_box_pack_start(GTK_BOX(box), optionmenu, FALSE, FALSE, 0);
    menu =gtk_menu_new();
    gtk_widget_set_usize(menu, 130, -1);
    
    items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALL_TYPE));
	G_SAFE_FREE(pConvert);
    gtk_widget_show( items);
    gtk_container_add(GTK_CONTAINER(menu), items);
     gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(InitLogTypeFilter), (gpointer) crt_n);
    
    for(i=0;i<type_num;i++) {
        items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(type_name[i]));
		G_SAFE_FREE(pConvert);
        gtk_widget_show( items);
        gtk_container_add(GTK_CONTAINER(menu), items);
		
		MenuPointer=getMenuDataPointer(crt_n,i);
        gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(GetTypeName), (gpointer) MenuPointer);

	}
    
    gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), menu);

		Search_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_STR_CUR_SEARCH));
		G_SAFE_FREE(pConvert);
//=======
//		Search_button = gtk_button_new_with_label(_toUtf8EX(/*"×Ö·û´®²éÑ¯"*/OIX_STR_STRING_SEARCH));
    gtk_widget_modify_fg(Search_button,GTK_STATE_NORMAL,&btn_color);
    gtk_box_pack_start(GTK_BOX(box), Search_button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(Search_button),
    "clicked",
    GTK_SIGNAL_FUNC(GetSearchStr),
    (gpointer)0);
    gtk_widget_show(Search_button);

	Advanced_Search_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_STRING_ADVANCED_SEARCH));
	G_SAFE_FREE(pConvert);
    gtk_widget_modify_fg(Advanced_Search_button,GTK_STATE_NORMAL,&btn_color);
    gtk_box_pack_start(GTK_BOX(box), Advanced_Search_button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(Advanced_Search_button),
    "clicked",
    GTK_SIGNAL_FUNC(PopupPromptDlg),
    (gpointer)0);
    gtk_widget_show(Advanced_Search_button);
    
    /******** prev next button *******/
    Prev_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PREV_FILE));
	G_SAFE_FREE(pConvert);
    gtk_widget_modify_fg(Prev_button,GTK_STATE_NORMAL,&btn_color);
    gtk_box_pack_start(GTK_BOX(box), Prev_button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(Prev_button),
    "clicked",
    GTK_SIGNAL_FUNC(GetLogFile),
    (gpointer)0);
    gtk_widget_show(Prev_button);
    
    next_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_NEXT_FILE));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_start(GTK_BOX(box), next_button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(next_button),
    "clicked",
    GTK_SIGNAL_FUNC(GetLogFile),
    (gpointer)1);
    gtk_widget_show(next_button);
    
    button_page_up = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_UP));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_start(GTK_BOX(box), button_page_up, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button_page_up), "clicked",
    GTK_SIGNAL_FUNC(LogBarDecrease),
    toolbar);
    gtk_widget_show(button_page_up);
    gtk_object_set_data(GTK_OBJECT(toolbar), "pageup", button_page_up );
    
    
    button_page_down = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_DOWN));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_start(GTK_BOX(box), button_page_down, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button_page_down), "clicked",
    GTK_SIGNAL_FUNC(LogBarIncrease),
    toolbar);
    gtk_widget_show(button_page_down);
    gtk_object_set_data(GTK_OBJECT(toolbar), "pagedown", button_page_down );
    
    
    last_page_btn  = gtk_toggle_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_NEW_PAGE));
	G_SAFE_FREE(pConvert);
    gtk_widget_show(last_page_btn);
    gtk_box_pack_start(GTK_BOX(box), last_page_btn, FALSE, FALSE, 0);
    gtk_object_set_data(GTK_OBJECT(toolbar), "lastpage", last_page_btn );
    
    gtk_widget_set_sensitive(last_page_btn, /*TRUE*/FALSE);
    
    gtk_signal_connect(GTK_OBJECT(last_page_btn), "clicked", GTK_SIGNAL_FUNC(LastPageBtnPressed),  toolbar);
    
    button_print = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_OUTPUT_TO_FILE));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_start(GTK_BOX(box), button_print, FALSE, FALSE, 0);
    
    gtk_signal_connect(GTK_OBJECT(button_print), "clicked",
    GTK_SIGNAL_FUNC(PrintTrapList), (gpointer) crt_n);
    gtk_widget_show(button_print);

	button_init_filter = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_STR_FILTER_INIT));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_start(GTK_BOX(box), button_init_filter, FALSE, FALSE, 0);
    
    gtk_signal_connect(GTK_OBJECT(button_init_filter), "clicked",
    GTK_SIGNAL_FUNC(InitTrapFilter), (gpointer) crt_n);
    gtk_widget_show(button_init_filter);
    
    button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_start(GTK_BOX(box), button_exit, FALSE, FALSE, 0);

    gtk_signal_connect_object(GTK_OBJECT(button_exit), "clicked",
    GTK_SIGNAL_FUNC(ExitLogDlg),
    GTK_OBJECT(log_shell[crt_n]));
    gtk_widget_show(button_exit);
    
    return (toolbar);
}

void loglist_draw_area_realize(GtkWidget *widget, gpointer data) {
    int crt_n = (gint)data ;
    printf("###in loglist_draw_area_realize\n");
    loglist_canvas[crt_n] = widget ;
    loglist_draw_gc[crt_n] = gdk_gc_new(widget->window);
    loglistOffDrawable[crt_n] = gdk_pixmap_new(widget->window, LOCAL_WIDTH, 1600, -1);
    /*gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]);  */
}

void  loglist_expose(	GtkWidget *widget, GdkEventExpose *event, gpointer data) {
    int crt_n = (gint)data;
    /*printf(" %%%%%%  in loglist_expose_events::: widget->window=%d,crt_n=%d\n",widget->window,crt_n);*/
    
    TrapLogDspProc(crt_n );
}

void 	ProgTrapLog( GtkWidget *parent, int crt_n)
{	 
        
        GtkWidget *vbox, *hbox;
        GtkWidget  *toolbar, *loglist_draw_area;
		gchar *pConvert=0;

		if(!CheckUserPrivilege(cur_usr_name, PRIV_SYS_CONF_MGR))
		{
			PopupMessageBox ( main_w[crt_n], MSG_SYS_CONFIG_MGR );
			return ;
		}

        gtk_set_locale();
        /*gtk_rc_add_default_file("./gtkrc.zh");  */

        if(log_shell[crt_n]) 
        {
            gtk_window_present(GTK_WINDOW(log_shell[crt_n]));
           /* PopupMessageBox(log_shell[crt_n], "log_list_shell existed !!\n");*/
            return;
        }
        print_select_tag[crt_n] =0;
        LastPage[crt_n] = TRUE;
        log_page_cnt[crt_n]	= 0;
        log_page_numb[crt_n]	= (GetSysLogCount()+LOG_PAGE_SIZE-1)/LOG_PAGE_SIZE;
        
        LOG_FILTER.prog =0;
        LOG_FILTER.type =0;
        LOG_FILTER.str =0;
		log_rec_mode[crt_n]	= LOG_REC_REAL;
        log_shell[crt_n]=gtk_window_new(GTK_WINDOW_TOPLEVEL);
        
        gtk_window_set_transient_for(GTK_WINDOW(log_shell[crt_n]), GTK_WINDOW(parent));
        
        gtk_window_set_position(GTK_WINDOW(log_shell[crt_n]),GTK_WIN_POS_CENTER);
        
        gtk_window_set_default_size(GTK_WINDOW(log_shell[crt_n]), LOCAL_WIDTH, LOCAL_HEIGHT);
        
        gtk_window_set_title(GTK_WINDOW(log_shell[crt_n]), pConvert=_toUtf8EX(OIX_CSTR_TRAP_LOG_TITLE));
		G_SAFE_FREE(pConvert);
        gtk_signal_connect(GTK_OBJECT(log_shell[crt_n]),
        "destroy",
        GTK_SIGNAL_FUNC(DestroyLogDlg
        ),
        NULL);
        
        vbox=gtk_vbox_new(FALSE, 5);
        gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
        gtk_container_add(GTK_CONTAINER(log_shell[crt_n]), vbox);
        gtk_widget_show(vbox);
        
        toolbar =  CreateTrapLogToolbar(log_shell[crt_n]);
        gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
        gtk_widget_show(toolbar);
        
        /* Create a scrolled log_shell[crt_n] to pack the CList widget into */
        
        loglist_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(loglist_scrolled_window),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
        
        gtk_box_pack_start(GTK_BOX(vbox), loglist_scrolled_window, TRUE, TRUE, 0);
        gtk_widget_show(loglist_scrolled_window);
        
        loglist_draw_area = gtk_drawing_area_new();
        gtk_widget_set_usize(loglist_draw_area, 1024, 1000);
  
        
        gtk_widget_set_events(loglist_draw_area, GDK_EXPOSURE_MASK
        | GDK_LEAVE_NOTIFY_MASK
        | GDK_BUTTON_PRESS_MASK
        | GDK_POINTER_MOTION_MASK );
        
        
        gtk_signal_connect(GTK_OBJECT(loglist_draw_area), "expose_event", GTK_SIGNAL_FUNC(loglist_expose), (gpointer)crt_n) ;
        
        gtk_signal_connect(GTK_OBJECT(loglist_draw_area), "realize", GTK_SIGNAL_FUNC(loglist_draw_area_realize), (gpointer)crt_n);
        gtk_signal_connect(GTK_OBJECT(loglist_draw_area), "button_press_event",
        GTK_SIGNAL_FUNC(SelectPrintRegion), (gpointer)crt_n );
        
        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(loglist_scrolled_window), loglist_draw_area);        
       
        gtk_widget_realize(GTK_WIDGET( loglist_draw_area)) ;
        gtk_widget_show(loglist_draw_area);
        
        
        hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
        gtk_widget_show(hbox);
        
        gtk_widget_show(log_shell[crt_n]);
        
        if (log_timeout_id[crt_n] != 0 ) {
            gtk_timeout_remove(log_timeout_id[crt_n]);
            log_timeout_id[crt_n]= 0;
        }
        log_timeout_id[crt_n]	=gtk_timeout_add( 1000, (GtkFunction)TrapLogDspProc, GINT_TO_POINTER(crt_n) );
        
        /*	gtk_main(); */
}


int	AdvancedSearch(int crt_n,DATE_TIME start_time,DATE_TIME end_time,char SearchObj[20])
{
	DATE_TIME		rec_file_time;
	FILE			*rec_fp;
	char			rec_filename[256];
	int				rec_file_no, i/*, rec_num*/;
	long			/*file_size, *//*rec_time,*/ start_tt, end_tt, search_file_tt, curr_time_tt,cur_time;
	int				search_end_flag= 0;
	struct tm		file_tm;
	struct tm p_datetime1;
	int year,mon,day,hour,min,sec;
	
    char 	log_line[MAX_LOG_INFO_SIZE+100];    
	
	curr_time_tt= time(NULL);
	start_tt= GetAbsTimeFromDateTime(&start_time);
	end_tt= GetAbsTimeFromDateTime(&end_time);

	if(end_tt<start_tt)
		return 0;


	rec_file_time= start_time;
	rec_file_time.hour= 0;
	rec_file_time.minute= 0;
	rec_file_time.second= 0;
	search_file_tt= GetAbsTimeFromDateTime(&rec_file_time);

	rec_file_no= 1;
	i=0;
	while(1)
	{
		sprintf(rec_filename, "%sH9000LOG%04d-%02d-%02d%03d.LOG", TRAP_FILE_PATH,rec_file_time.year,
			rec_file_time.month,rec_file_time.day,   rec_file_no++); 		

		if(NULL==(rec_fp=fopen(rec_filename, "rb")))
		{
			search_file_tt +=60*60*24;
			if(search_file_tt>=curr_time_tt)
				break;

			file_tm= *localtime(&search_file_tt);
			rec_file_time.year= file_tm.tm_year+1900;
			rec_file_time.month=file_tm.tm_mon+1;
			rec_file_time.day= file_tm.tm_mday;
			rec_file_no= 1;
//printf("open file %s fail\n",rec_filename);
			continue;
		}  
		else
			printf("open file %s \n",rec_filename);
		if(search_file_tt>end_tt)
		{
			search_end_flag= 1;
			break;
		}
		

		log_rec_mode[crt_n]	=LOG_REC_HISTRY;
		
		for(;;) 
		{
			if(NULL!=fgets(	log_line, sizeof(log_line), rec_fp)) 
			{
				GetLogstr(log_line);

				if(strstr(logstrbuf[4],SearchObj)==0)
					continue;
            /*printf("&&&& logstrbuf=%s;%s;%s;%s;%s\n",logstrbuf[0],logstrbuf[1],logstrbuf[2],logstrbuf[3],logstrbuf[4]);*/
				sprintf(hist_log_rec[i].no, "%d", i);
				strcpy(hist_log_rec[i].errtype, logstrbuf[0]);
				strcpy(hist_log_rec[i].errcode, logstrbuf[1]);
				strcpy(hist_log_rec[i].datetime, logstrbuf[2]);
				strncpy(hist_log_rec[i].appid, logstrbuf[3], sizeof(hist_log_rec[i].appid));
				strcpy(hist_log_rec[i].info, logstrbuf[4]);
         //   printf("logstrbuf[%d]=%s; %s;%s;%s;%s,strlen=%d\n",i,hist_log_rec[i].errtype,hist_log_rec[i].errcode,hist_log_rec[i].datetime,hist_log_rec[i].appid,hist_log_rec[i].info,strlen(hist_log_rec[i].errtype));	
            
				sscanf(hist_log_rec[i].datetime,"%d-%d-%d %d:%d:%d",&year,&mon,&day,
	 				&hour,&min,&sec);

				p_datetime1.tm_year=year-1900;
				printf("####p_datetime->tm_year=%d\n",p_datetime1.tm_year);
				p_datetime1.tm_mon=mon-1;

				p_datetime1.tm_mday=day;
				p_datetime1.tm_hour=hour;
				p_datetime1.tm_min=min;
				p_datetime1.tm_sec=sec;
				p_datetime1.tm_isdst =-1;
				mktime( &p_datetime1 );
				cur_time=	GetAbsTimeFromDateTime((DATE_TIME *)&p_datetime1);
				if(cur_time<start_tt)
				{
					i++;
					continue;
				}
				else if(cur_time>end_tt)
				{
					search_end_flag= 1;
					break;
				}
				else
					i++;
				if(i>MAX_LOG_NUM)
				{
					printf("log count overflow!\n");
					search_end_flag= 1;
					break ;
				}
			}
			else
				break;
		}/* for loop end*/
		if(search_end_flag)
			break;
		fclose( rec_fp );
	}/* while end*/
	
	log_count = i;
	log_page_numb[crt_n] = log_count/LOG_PAGE_SIZE +1;	
	return 0;
}



