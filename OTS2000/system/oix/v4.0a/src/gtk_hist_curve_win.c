
/*-----------------------------------------------------------------------

	 
	   Created by chi.hailong
	   Create  Date : 2005.11.26
	   Last Update  : 2006.6.11
	   
		 
---------------------------------------------------------------------*/



#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <time.h>
#include 	<pthread.h>
#include 	<signal.h>
#include 	<sys/shm.h>
#include	"oix.h"



short	hist_dev = 1,histCurv_stn=1,hist_dtype=1;
DB_TAB_WINDOW_INFO hist_curve_db_info[MAX_CRT_NUMBER];

/********************* gtk_hist_curve.h ************************/

#define		MAX_INTERVAL_NUM	5   	 /*  interval num  : 1's / 1'min /5'min /15'min /1'hour */
#define		SEL_ANA_LIST		0
#define		ANA_LIST		1
#define		STAR_T			0
#define		END_T			1
#define		MAX_POINTER_NUM		5000      /*  The number of points which will draw curve */
#define		CANVAS_MAX_POINTER	900
#define		MAX_CURVE_NUM		6

#define 	NUM_THREADS 		5
#define 	SLEEP_TIME 		10
#define		X_ORGIN			50		/*  margin  of right down  **/
#define		Y_ORGIN			50
#define		X2_ORGIN		50
#define		Y2_ORGIN		50	
#define		IDLE_FLAG		0
#define		LOCK_FLAG		1

typedef struct
{
	XRECT          rc ;
	double          xstep;
	SHORT          xLength ;
	SHORT          yLength  ;
	int            XaxisY;   /* X - axis y value  */
	GtkAllocation  alloc ;
}       COORDINATE;
typedef struct
{
	POINTER         ptr[MAX_POINTER_NUM];
	SHORT           next_pos;
}       TRENDDATAHIS;
typedef struct
{
	GdkRectangle   rect;
	int    fg; 
	int   bg;
	float xstep ;
	int   val[MAX_CURVE_NUM];
	int   unit[MAX_CURVE_NUM];
	int   hi_range[MAX_CURVE_NUM];
	struct tm Time  ;
	gint  canMove ;
}	RCD_TAG;

GtkWidget 	*trend_main_w[MAX_CRT_NUMBER];
GdkPixmap	*off_draw_area[MAX_CRT_NUMBER];
GdkGC		*canvas_gc[MAX_CRT_NUMBER];
COORDINATE       coordin[MAX_CRT_NUMBER];
GtkWidget       *hist_draw_area[MAX_CRT_NUMBER] ;
XRECT  mycur ;
RCD_TAG rcdTag[MAX_CRT_NUMBER] ;   /* label */ 

GtkWidget 	*starTmEntry,*endTmEntry,*list_ana_val,*sel_ana_val_list,   *scroll_win ;
GList 		*trend_interval_list	=	NULL ;



int 		mins , seconds, hours;    /* How many minitues or seconds or hours  */
/*  int  		interval_Type ;   */
enum 		T_Interval{A_SEC1,A_MIN1,FIV_MIN1,FIFTEEN_MIN1,A_HOUR1};

SHM_HDB        data_curve,*shm_curve_addr ;
int		shm_id_curve;

void UpdateHisDevMenu(int crt_n) ;

pthread_t 	tid[NUM_THREADS];      /* array of thread IDs */
gint    curve_timer_id = 0;
short   COLOR_GROUP[MAX_CURVE_NUM] = {RED,RED4,BLUE,WHITE,BLACK4,YELLOW} ;

/********************** end ********************************/


extern void 	DestroyDlg(GtkWidget *w);
extern void 	SgzyDevSelectCB (GtkWidget* w, gpointer data);
extern void	DrawSgzyCurve(int crt_n,GtkWidget* w,GdkGC* gc,SHORT now_i,SHORT num,int x_left,int y_top,int width,int height,float ys,float ye,char longname[DBNAME_LEN*2]);
void 		*FetchDataFromSrv(void *);     /* thread routine */
void WriteShareMem ();
void hist_draw_area_realize(GtkWidget *widget,gpointer data);
void	HisCurvRedraw (int crt_n,GdkDrawable *drawable , GdkGC *gc ,int width ,int height);

void UpdateHistDataTypeMenu(int crt_n) ;

const 		gchar *list_item_data_k = "list_item_row_no" ;
const		gchar *list_item_data   = "list_item_data"   ;

/****** **                static	variable    *******/
extern foreground[9][10];
short	FetchFloor();	
static	FILE	*fp;
static  short   trend_add_item ;
static  short   trend_del_item ;
static  int	zero_y , y_h;    /*  x` axis from left top of canvas    */
static  gboolean	exit_flag = FALSE;
static  short   Interval ;
static  struct tm starwhen ;


int Xscale[MAX_CRT_NUM] = {0,0,0} ;

int trend_station_id;
int  select_point,select_points[MAX_WDQS_COUNT],select_points_num1=0;
POINTER  curv_ptr[6]  ;
#ifdef  SUN
TRENDDATAHIS   trenddatahis;
TRENDDATA	seldatahis ;
#endif

GtkWidget *CreateHisCurvToolbar (GtkWidget * parent, int crt_n);
static GtkWidget *CreateProgHisCurveShell (GtkWidget *parent , int  crt_n );

void DrawrcdTag(int crt_n ,GdkDrawable *drawable, GdkGC *gc ,RCD_TAG *rcd )
{
	rcd->bg	= canvas_bg_color;
	
	char record[20]; 
	gdk_gc_set_function(gc, GDK_COPY);
	gdk_gc_set_foreground(gc,&pixels[crt_n][BLACK]);
	gdk_draw_rectangle ( drawable,gc , TRUE,rcd->rect.x,rcd->rect.y, rcd->rect.width , rcd->rect.height); 
	gdk_gc_set_foreground(gc,&pixels[crt_n][WHITE]);
	gdk_draw_rectangle ( drawable,gc , FALSE,rcd->rect.x,rcd->rect.y, rcd->rect.width , rcd->rect.height); 
	
	
	
	gdk_gc_set_foreground(gc,&pixels[crt_n][GREEN]);
	strcpy(record , "Ê±   ¼ä:"); 
	gdk_draw_string( drawable, font_set[0], gc,  rcd->rect.x +15, rcd->rect.y +15 , record);
	sprintf(record , "%dÈÕ%d:%d ", rcd->Time.tm_mday , rcd->Time.tm_hour,rcd->Time.tm_min); 
	gdk_draw_string( drawable, font_set[0], gc,  rcd->rect.x +10, rcd->rect.y +30 , record);
	sprintf(record , "X :%d ",  rcd->val[0]); 
	gdk_draw_string( drawable, font_set[0], gc,  rcd->rect.x +10, rcd->rect.y +50 , record);
	
	
	
	sprintf(record , "Y :%d ",  rcd->val[1]); 
	gdk_draw_string( drawable, font_set[0], gc,  rcd->rect.x +10, rcd->rect.y +70 , record);  
	
	
    
}
void DrawX_Line(int crt_n ,GdkDrawable *drawable, GdkGC *gc ,int x , int h , short mod )
{  
    char ss[30];
	
    struct tm when;
    short  xi =1  ;
    gdk_gc_set_fill ( gc, GDK_SOLID );	
    if (!(mod %2)){
		
		gdk_gc_set_line_attributes(gc,0,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND) ;
		
		
		
		memcpy (&when, &starwhen ,sizeof (struct tm));
		MakeFormatTime( &when ,ss , 60 * mod/2 ,Interval );  /* 60 :  900/15 = 60 */
		switch (Interval)
		{
		case 0 :  	/* 1 `s */
			sprintf ( ss,"%02dm",  when.tm_min);
			xi = 2 ;
			break;
		case 1 :  	/* 1 `m */
			sprintf ( ss,"%02dh%02dm",  when.tm_hour,when.tm_min);
			xi = 2 ;
			break;
		case 2 :  	/* 5 `m */
			sprintf ( ss,"%02dD:%02dH",  when.tm_mday,when.tm_hour);
			xi = 1 ;
			break;
		case 3 :  	/* 15 `m */
			sprintf ( ss,"%02dD:%02dH",  when.tm_mday,when.tm_hour);
			
			break;
		case 4 :  	/* 1 `h */
			sprintf ( ss,"%02D:%02H",  when.tm_mday,when.tm_hour);
			xi = 1 ;
			break; 
		default:
			sprintf ( ss,"%02dD:%02dH",  0,0);
			xi = 1 ;
			break;                          
		}
        if (mod % 4 )
			gdk_gc_set_foreground (gc, &pixels[ crt_n][PURPLE2]) ;
        else
			gdk_gc_set_foreground ( gc, &pixels[ crt_n][GRN_BLUE8]) ;	
		
		
		
		gdk_draw_text( drawable, font_set[0], gc,  x-30/xi, Y_ORGIN + h +20, 
			ss, strlen(ss));
    }else
    {
		gdk_gc_set_line_attributes(gc,0,GDK_LINE_ON_OFF_DASH,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);	
    }
    
    
    gdk_gc_set_foreground ( gc, &pixels[ crt_n][GREEN4]) ;  /* CHOCLT1 */
    gdk_draw_line (  drawable, gc, x , Y_ORGIN ,x, Y_ORGIN + h);
	
}


int Ytoy(int *y)
{   
    
    return zero_y - *y ; 
	
}

void DrawY_Line(int crt_n ,GdkDrawable *drawable ,GdkGC *gc,int y , int w , short mod )
{  
    char str[20] ;
    int   i; 
    gdk_gc_set_fill (  gc , GDK_SOLID );	
    gdk_gc_set_foreground (  gc , &pixels[ crt_n][GREEN4]) ;  /* Default color is CHOCoLaTe */
    if (!(mod %2)){
		i = mod /2;
		sprintf ( str,"%02d",abs ( 100 - i * 20 ));
		strcat(str,"%");
		
        if (100 - i * 20 >= 0 )	
            gdk_gc_set_foreground (  gc , &pixels[ crt_n][RED1]);
        else
            gdk_gc_set_foreground (  gc , &pixels[ crt_n][GREEN]);    
		gdk_draw_text( drawable, font_set[0],  gc ,  50 + w +10, y, 
			str, strlen(str));  
		
		if (i == 5)   /*  draw x axis  */
		{
			gdk_gc_set_foreground (  gc , &pixels[ crt_n][RED1]) ;
			gdk_gc_set_line_attributes( gc ,2,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND) ;
			zero_y = y ;
		}
		else	 
		{
			gdk_gc_set_foreground (  gc , &pixels[ crt_n][GREEN4]) ;  /* Default color is CHOCoLaTe */
			gdk_gc_set_line_attributes( gc ,0,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND) ;
			
		}
		
		
		
		
    }
    else
    {
		gdk_gc_set_line_attributes( gc ,0,GDK_LINE_ON_OFF_DASH,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);	
    }
    gdk_draw_line (  drawable,  gc , 50 , y,50 +w, y);
	
}


void HisCurvXY_Line(int crt_n ,GdkWindow *drawable ,GdkGC *gc,int width, int height ) 
{  
	
	float stepx ,stepy ; int i;
	
	
	BACKRCD		rcd;
	stepx = (coordin[crt_n].xLength ) / 30.0 ;   /*  15 x line ; */
	stepy = (coordin[crt_n].yLength) / 20.0;
	rcd.rect.x1  =  coordin[crt_n].rc.x1;
	rcd.rect.y1  =  coordin[crt_n].rc.y1;
	rcd.rect.x2  =  coordin[crt_n].alloc.width   - coordin[crt_n].rc.x2 ;
	rcd.rect.y2  =  coordin[crt_n].alloc.height  - coordin[crt_n].rc.y2 ;
	
	rcd.fill_type =  UNFILLED;
	rcd.line_width = 2 ; 
	rcd.bg	= BLACK;
	rcd.fg = CHOCLT1;
	
	rcd.draw_type = DrwT_RECTANGLE ;
	DrawRcd ( crt_n, drawable, gc, &rcd ,NULL) ;
	
	for (i= 0 ; i <=30 ; i++)
	{
		
	       DrawX_Line( crt_n ,drawable ,gc,  rcd.rect.x1  + i*stepx ,  rcd.rect.y2 - rcd.rect.y1,i );		
	}
	for (i= 0 ; i <=20 ; i++)
	{
		
	       DrawY_Line( crt_n , drawable , gc , rcd.rect.y1  + i*stepy ,  rcd.rect.x2 - rcd.rect.x1,i );	
		   
	}
	y_h = 10 * stepy ;  
	
	
    
}



void  hisCurve_expose_events(	GtkWidget *widget,GdkEventExpose *event, gpointer data)
{     
	int crt_n = (gint) data ; 
	
	GdkRectangle rc ;
	rc.x = rc.y = 0 ;
	rc.width = widget->allocation.width ;
	rc.height = widget->allocation.height ;
	HisCurvRedraw (  crt_n,off_draw_area[crt_n] , canvas_gc[crt_n] , rc.width , rc.height);
	gdk_draw_pixmap(widget->window, canvas_gc[crt_n]  ,off_draw_area[crt_n]  ,event->area.x ,event->area.y , 
		event->area.x ,event->area.y ,event->area.width, event->area.height );
}


int    motion_hist_CB(GtkWidget  *widget,   GdkEventMotion  *event,
					  gpointer     user_data)
{
	int dx , dy ;
	int crt_n = 0 ;
	int i ; 
	struct tm when ;
	char ss[30];
	GdkCursor  *cur ; 
	GdkRectangle rcTmp ;
	
	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
	
	
    
    GdkEventButton *bevent = (GdkEventButton *) event; 
    if(  !(event->state & GDK_BUTTON1_MASK))
        return FALSE ;
	
	printf ("\n button is %d ,%d", (bevent)->button , (bevent)->state );
	if (event->x >= coordin[crt_n].rc.x1  && event->x <= coordin[crt_n].rc.x1 + coordin[crt_n].xLength  )
	{
		gdk_gc_set_function(canvas_gc[crt_n], GDK_XOR);
		dx	= mycur.x2	- mycur.x1;
		dy	= mycur.y2	- mycur.y1;
		
		if (rcdTag[crt_n].canMove)
		{
			/* gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_ON_OFF_DASH,GDK_CAP_ROUND,GDK_JOIN_MITER);  */
			gdk_gc_set_foreground(canvas_gc[crt_n],&pixels[crt_n][WHITE]);
			gdk_draw_rectangle (  widget->window,canvas_gc[crt_n] , FALSE,rcdTag[crt_n].rect.x +dx -2 ,rcdTag[crt_n].rect.y +dy -2, rcdTag[crt_n].rect.width +4 , rcdTag[crt_n].rect.height +4); 
			
		}else  /* move  x scale */
		{
			gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_MITER);
			gdk_gc_set_foreground(canvas_gc[crt_n], &pixels[crt_n][WHITE]);
			gdk_draw_line  ( widget->window,canvas_gc[crt_n] ,Xscale[crt_n] + dx   ,coordin[crt_n].rc.y1,Xscale[crt_n]+ dx ,coordin[crt_n].rc.y1+coordin[crt_n].yLength   ) ;	
			
		}
		
		
		mycur.x2 = event->x ; mycur.y2 = event->y ;
		dx	=mycur.x2	- mycur.x1;
		dy	=mycur.y2       - mycur.y1 ;
		
		
		if (rcdTag[crt_n].canMove)
		{
			/* gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_ON_OFF_DASH,GDK_CAP_ROUND,GDK_JOIN_MITER);*/
			gdk_gc_set_foreground(canvas_gc[crt_n],&pixels[crt_n][WHITE]);
			gdk_draw_rectangle (  widget->window,canvas_gc[crt_n] , FALSE,rcdTag[crt_n].rect.x +dx -2 ,rcdTag[crt_n].rect.y +dy -2, rcdTag[crt_n].rect.width +4 , rcdTag[crt_n].rect.height +4); 
		}else
		{
			gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_MITER);
			gdk_draw_line  ( widget->window,canvas_gc[crt_n] ,Xscale[crt_n] + dx ,coordin[crt_n].rc.y1,Xscale[crt_n] +dx ,coordin[crt_n].rc.y1+coordin[crt_n].yLength   ) ;	
			
		}
		
		int yi ;
		i =   (int)((event->x - coordin[crt_n].rc.x1)  /  coordin[crt_n].xstep) ;
		if  (i > 0 && i < 900)
		{
			yi= (int)( data_curve.data_curve[crt_n].seg[0].data[i]/100.00  ) ;
			memcpy (&when, &starwhen ,sizeof (struct tm));
			MakeFormatTime( &when ,ss , i ,Interval );  /* 60 :  900/15 = 60 */
			memcpy (&rcdTag[crt_n].Time, &when ,sizeof (struct tm));  
		}
		rcdTag[crt_n].val[0] =   i  ;
		rcdTag[crt_n].val[1] =   yi  ;
		memcpy(&rcTmp ,&rcdTag[crt_n].rect, sizeof(GdkRectangle));
		rcTmp.x -=2 ;rcTmp.y -= 2 ;
		rcTmp.width +=4 ;rcTmp.height += 4 ;
		DrawrcdTag( crt_n ,off_draw_area[crt_n], canvas_gc[crt_n] , &rcdTag[crt_n] );
		gdk_window_invalidate_rect(widget->window ,&rcTmp,1);
		
	}
	
	return FALSE;
}

gint button_release_CB( GtkWidget *widget,GdkEvent *event , gpointer *p)
{
	GdkEventButton *bevent ; 
	int dx , dy ,x ,y; 
	int crt_n = 0 ;
	GdkRectangle rc ; 
	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) 
		return ;
	
	
	
	
	rc.x= rc.y = 0 ;
	rc.width  = widget->allocation.width ;
	rc.height = widget->allocation.height ;
    
	
	if (event->type == GDK_BUTTON_RELEASE) {  /*  GDK_2BUTTON_PRESS ---double clicked !!!! */
        bevent = (GdkEventButton *) event; 
        if ((bevent->button == 1))   /* button 1 pressed   */
        {
			g_message("\n   Button release !!!!!!! "); 
			x = bevent->x ;
			y = bevent->y ;
			gdk_gc_set_function(canvas_gc[crt_n], GDK_COPY);
			dx	=mycur.x2	- mycur.x1;
			dy	=mycur .y2	- mycur.y1;
			
			if  (rcdTag[crt_n].canMove)
			{
				rcdTag[crt_n].canMove = FALSE ; 
				rcdTag[crt_n].rect.x += dx ;
				rcdTag[crt_n].rect.y += dy ;
				
				g_message("\n   Button release !!!!!!! ");                    
			}else
			{
				Xscale[crt_n] += dx ;
				if (( x  >=  coordin[crt_n].rc.x1) && ( x  <=  coordin[crt_n].rc.x1 + coordin[crt_n].xLength ) &&
					( y  >=  coordin[crt_n].rc.y1) && ( y  <= coordin[crt_n].rc.y1 + coordin[crt_n].yLength))
				{
					gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_MITER);
					gdk_gc_set_foreground(canvas_gc[crt_n], &pixels[crt_n][WHITE]);
					gdk_draw_line  ( widget->window,canvas_gc[crt_n] ,Xscale[crt_n]   ,coordin[crt_n].rc.y1,
						Xscale[crt_n]   ,coordin[crt_n].rc.y1+coordin[crt_n].yLength   ) ;	
				}
			}
			
			HisCurvRedraw (  crt_n, off_draw_area[crt_n] , canvas_gc[crt_n],rc.width ,rc.height);
			gdk_window_invalidate_rect(widget->window ,&rc ,1);    
			return TRUE;
			
			
		}
		
		
		
	}
	
}


gint button_press_CB( GtkWidget *widget,GdkEvent *event , gpointer *p)
{
	
    int x ,y ,dx, dy; 
    int crt_n = 0 ;
    GdkEventButton *bevent ;
    GdkRectangle rc ; 
    
	
	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;     
    
    
    
    rc.x= rc.y = 0 ;
    rc.width  = widget->allocation.width ;
    rc.height = widget->allocation.height ;
	
	
	
    if (event->type == GDK_BUTTON_PRESS) {  /*  GDK_2BUTTON_PRESS ---double clicked !!!! */
        bevent = (GdkEventButton *) event; 
        if ((bevent->button == 1))   /* button 1 pressed   */
        {       
			
            g_message("\n @@@@@@   Button  press down !! \n");
			x = (bevent->x) ; y = bevent->y ;
			mycur.x1 = mycur.x2 =  x;
			mycur.y1 = mycur.y2 =  y;
			
			if ( ( x  >=  rcdTag[crt_n].rect.x) && ( x  <=  rcdTag[crt_n].rect.width + rcdTag[crt_n].rect.x ) &&
				( y  >= rcdTag[crt_n].rect.y ) && ( y  <= rcdTag[crt_n].rect.y + rcdTag[crt_n].rect.height))					 
				
			{
				rcdTag[crt_n].canMove  = TRUE ;
				
				
			}
			else
			{
				rcdTag[crt_n].canMove  = FALSE ; 
				Xscale[crt_n] =  (bevent->x) ;
				if (( x  >=  coordin[crt_n].rc.x1) && ( x  <=  coordin[crt_n].rc.x1 + coordin[crt_n].xLength ) &&
					( y  >=  coordin[crt_n].rc.y1) && ( y  <= coordin[crt_n].rc.y1 + coordin[crt_n].yLength))
				{
					gdk_gc_set_function(canvas_gc[crt_n], GDK_XOR); 
					gdk_gc_set_foreground(canvas_gc[crt_n], &pixels[crt_n][WHITE]);
					gdk_draw_line  ( widget->window,canvas_gc[crt_n] ,Xscale[crt_n]   ,coordin[crt_n].rc.y1,Xscale[crt_n] ,coordin[crt_n].rc.y1+coordin[crt_n].yLength   ) ;	
				}
				
			}
			
			
			
			
			return TRUE;
        }
        
    }
	
	
}



int     InitShareMemory_S(int crt_n )
{
	int i ,res;
	
	   
	if (get_hdb_shm()==FALSE)
	{
		printf("can't get shared memory\n");
		return FALSE;
	}
	
	shm_curve_addr= shm_hdb_addr;
	
	
	
	
	
	for(;;){
		
		if(shm_curve_addr->data_curve[crt_n].lockflag == IDLE_FLAG)	break;
		
		Sleep(10 );
	}  
	
	
	shm_curve_addr->data_curve[crt_n].lockflag=LOCK_FLAG  ;    
	shm_curve_addr->data_curve[crt_n].querytype=0;
	shm_curve_addr->data_curve[crt_n].num = 0 ;
	shm_curve_addr->data_curve[crt_n].queryflag = 1 ;
	shm_curve_addr->data_curve[crt_n].lockflag=IDLE_FLAG    ; 
	
	
	/* ********  here will create  a thread    ******** */               
	printf(" \n ^_^  Here will create   thread_ !!!!!()\n");     
	
	res =   pthread_create(&tid[0], NULL, FetchDataFromSrv, (void *)SLEEP_TIME);
	if (res == -1  ) 
	{	    
		printf ("Oix :Error when create thread res:d% !!!  \n",res);
		return(FALSE);
	}		
	else 
		printf("Oix : reporting that  thread  have create successfully\n" );	    
	
}    



void	SelValueListCB (GtkWidget* w, gpointer list)
{
	GList *dlist;
	GtkObject       *list_item;
	int	select_row;	
	
	dlist = GTK_LIST(w)->selection;
	if(!dlist)
	{
		return;
	}
	while(dlist)
	{
		list_item=GTK_OBJECT(dlist->data);		
		
		select_row=gtk_object_get_data(list_item,list_item_data_k );	
		
		dlist= dlist->next;
	}
	
	switch  ( (int) list )
	{
	case	SEL_ANA_LIST:	
		trend_del_item	= select_row;
		select_point	= select_row-1;
		break;
		
	case	ANA_LIST:	
		trend_add_item	=select_row ;		
		break;
		
	default:	;
		
	}
	printf("OIX: trend_add_item is %d  !!!\n" , trend_add_item);
	
}


void	UpdateAnaValLst ( int crt_n  )
{
	GtkWidget       *list_item;
	short		i,j;
	POINTER		ptr;
	DMS_COMMON	pdt;
	char  LongNamerStr1[128];
	char	        longname[DBNAME_LEN];
	gchar *pConvert=0;
	int		db_num[MAX_CRT_NUM];			
	
	
	gtk_list_clear_items(list_ana_val,0,-1);
	trenddatahis.next_pos  = 0 ;
	
    
	ptr = hist_curve_db_info[crt_n].select_pt ;
	
	
	db_num[crt_n] =  0   ;        
	if(-1==GetEntryNumById( hist_curve_db_info[crt_n].select_pt.stn_id, 
		hist_curve_db_info[crt_n].select_pt.dev_id, hist_curve_db_info[crt_n].select_pt.data_type,&db_num[crt_n]))
		return;
	for (i=0; i< db_num[crt_n]; i++)
	{
		ptr.pt_id	=i ;
		
		
		if ( ReadEntryById ( &ptr, &pdt ) == -1)
		{
			printf("OIX: TrendName No this point in the database file !!!\n");
		}
		else
		{
			if (!pdt.fixed.ioattr.hdbRec)
				continue ;
			GetEntryLongnameById (&pdt.point,LongNamerStr1 );
			BlankFillCopy (longname, LongNamerStr1,ENTRY_LONGNAME_SIZE);
			trenddatahis.ptr[trenddatahis.next_pos] = ptr ; 
			
			trenddatahis.next_pos ++ ; 
			
			list_item = gtk_list_item_new_with_label(pConvert=_toUtf8(longname));
			G_SAFE_FREE(pConvert);//dzy1017
			gtk_container_add(GTK_CONTAINER(list_ana_val),list_item );
			gtk_widget_show(list_item);
			gtk_object_set_data(GTK_OBJECT(list_item),list_item_data_k,trenddatahis.next_pos);	
		}
	}
	
	printf("OIX:  total nextpos is %d  and dbnum is%d !!!\n" ,trenddatahis.next_pos ,  db_num);
	
}



void	UpdateAnaListSel (int crt_n )
{
	GtkWidget       *list_item;
	short           i;
	POINTER         *ptr;
	DMS_COMMON      pdt;
	char            longname[DBNAME_LEN];   
	char  LongNamerStr1[128];
	gchar *pConvert=0;
	gtk_list_clear_items(GTK_LIST(sel_ana_val_list),0,-1);
	
	
	if( seldatahis.next_pos > MAX_POINTER_NUM)
	{
		printf("Oix Error:in gtk_hist_curve.c UpdateAnaListSel:  seldatahis.next_pos=%d > MAX_POINTER_NUM \n", seldatahis.next_pos);
		seldatahis.next_pos= 0;
		return;
	}
	
	
	for (i=0; i<seldatahis.next_pos; i++)
	{
		ptr	= &seldatahis.ptr[i];
		
		printf("in UpdateAnaListSel: dev_id=%d, data_type=%d, pt_id = %d\n",
			ptr->dev_id, ptr->data_type, ptr->pt_id);
		
		
		if ( ReadEntryById ( ptr, &pdt )  == -1)
		{
			printf("OIX: TrendList no this point in the database file !!!\n");
		}
		else
		{			
			
			/*  strcpy(longname,GetDevNameRemark(pdt.entry.anlg.variable.ptr.dev_id));  */
			GetEntryLongnameById (&pdt.point,LongNamerStr1 );
			strcpy(longname,LongNamerStr1);
			
			list_item = gtk_list_item_new_with_label(pConvert=_toUtf8(longname));
			G_SAFE_FREE(pConvert);//dzy1017
			gtk_container_add(GTK_CONTAINER(sel_ana_val_list),list_item );			
			gtk_widget_show(list_item);
			gtk_object_set_data(GTK_OBJECT(list_item),list_item_data_k,i+1);
		}
	}
}


void	ListValAddCB (GtkWidget *w  )
{
	
	int crt_n = (gint) 0 ;
	GetCrtNumberOfWgt(w, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
    
	int             i;
	
	if ( seldatahis.next_pos >= MAX_CURVE_NUM)
	{
		
		gdk_beep();
		PopupMessageBox(crt_n,"\nOIX: seldatahis.next_pos >= MAX_CURVE_NUM\n");
		return;
	}
	
	for(i=0; i<seldatahis.next_pos;i++)
	{
		if((seldatahis.ptr[i].stn_id  == trenddatahis.ptr[trend_add_item-1].stn_id) &&
			(seldatahis.ptr[i].dev_id  == trenddatahis.ptr[trend_add_item-1].dev_id) &&
			(seldatahis.ptr[i].data_type  == trenddatahis.ptr[trend_add_item-1].data_type) && 
			(seldatahis.ptr[i].pt_id  == trenddatahis.ptr[trend_add_item-1].pt_id))
		{
			gdk_beep();
			return ;
		}
	}    
	
	if ( trend_del_item == 0 )
		trend_del_item = seldatahis.next_pos;
	else
	{
		for (i=seldatahis.next_pos; i>trend_del_item; i--)
			seldatahis.ptr[i] = seldatahis.ptr[i-1];
	}
	
	seldatahis.ptr[trend_del_item] = trenddatahis.ptr[trend_add_item-1] ;   
	
	
	
	seldatahis.next_pos++;
	UpdateAnaListSel ( crt_n );
	
	
	
	trend_del_item  = 0; 
	
	
	
}

void	ListValRemoveCB ( )
{
	int crt_n=0;
	int             i;
	
	if (trend_del_item ==0)
	{
		gdk_beep();
		return;
	}
	
	
	
	gtk_list_clear_items(sel_ana_val_list,trend_del_item-1,trend_del_item);
	
	seldatahis.next_pos = seldatahis.next_pos-1;
	for (i=trend_del_item; i<seldatahis.next_pos+1; i++)
	{ seldatahis.ptr[i-1]	= seldatahis.ptr[i]; }
	
	
	
	UpdateAnaListSel(crt_n);	 
	
	
	trend_del_item	= 0;
	
	
}
HisCurvRedrawE (int crt_n)
{
	GdkRectangle rc ;
	rc.x = rc.y = 0 ;
	rc.width = hist_draw_area[crt_n]->allocation.width ;
	rc.height = hist_draw_area[crt_n]->allocation.height ;
	HisCurvRedraw (  crt_n,off_draw_area[crt_n] , canvas_gc[crt_n] , rc.width , rc.height);
	gdk_window_invalidate_rect(hist_draw_area[crt_n]->window , &rc    ,TRUE);
	
}
void	HisCurvRedraw (int crt_n,GdkDrawable *drawable , GdkGC *gc ,int width ,int height)
{
	int 	i, i1, i2,k ,curid ;
	POINTER         *ptr;
	DMS_COMMON      pdt;
	char            longname[DBNAME_LEN*2];
	ANLG_DATA	anlg_data;
	
	float   hi_range, lo_range;
	float	xstep;
	GdkPoint pt1, pt2;
	int  xi ,yi;
	int canvas_w, canvas_h ;
	BACKRCD  rcdtmp ;  
	canvas_w = width  ;
	canvas_h =height ; 
	
	
    /*	printf (" canvas_w  is %d ,canvas_h  is %d\n" , canvas_w, canvas_h);  */
	
	if (drawable == NULL )
	{
		printf("Oix of HisCurvRedraw Error  :draw_area is null!!! \n"); 
		return  ;
	}
	
	/************  clear offscreen buffer of display *********************/	 
	rcdtmp.fg	= canvas_bg_color;
	rcdtmp.rect.x1	= 0;
	rcdtmp.rect.y1	= 0;
	rcdtmp.rect.x2	= rcdtmp.rect.x1+coordin[crt_n].alloc.width;
	rcdtmp.rect.y2	= rcdtmp.rect.y1+coordin[crt_n].alloc.height;
	rcdtmp.fill_type	= FILLED;
	rcdtmp.draw_type	= DrwT_RECTANGLE;
	gdk_gc_set_function(gc, GDK_COPY);
	DrawRcd ( crt_n,   drawable, gc, &rcdtmp ); 
	
	HisCurvXY_Line(crt_n ,drawable ,gc,coordin[crt_n].alloc.width,coordin[crt_n].alloc.height );	
	
	
	xstep = coordin[crt_n].xstep ;
    
	for(curid=0;curid<data_curve.data_curve[crt_n].num;curid++)       
	{       
		gdk_gc_set_foreground (gc, &pixels[ crt_n][COLOR_GROUP[curid]]);  
		yi= (int)( data_curve.data_curve[crt_n].seg[curid].data[0]/10000.00 * y_h) ;
        
		for (i = 1 ,pt1.x= 50,pt1.y=  Ytoy( &yi) ; i<data_curve.data_curve[crt_n].seg[curid].rscount ; i++ )
		{     
			xi = (int) ((float)i * xstep) ;
			yi= (int)( data_curve.data_curve[crt_n].seg[curid].data[i]/10000.00 * (float)y_h) ;
			pt2.x = xi + X_ORGIN ; pt2.y=  Ytoy( &yi) ; 
			gdk_draw_line (  drawable, gc, pt1.x , pt1.y , pt2.x , pt2.y );
			pt1.x = pt2.x ; pt1.y= pt2.y ;	
      		}
	} /*  end for */
	
	DrawrcdTag( crt_n ,drawable, gc , &rcdTag[crt_n] );
}



void	DisplayCB (GtkWidget* pWidget, gpointer data)
{
	int i;
	int crt_n=(gint)data;
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;
	
  	 
	
	if ( seldatahis.next_pos == 0)
	{
		gdk_beep();
		PopupMessageBox(crt_n,"OIX: Seldatahis.next_pos == 0!");
		return;
	}

// 	WriteShareMem ();
// 	if (curve_timer_id  !=0 )
// 		gtk_timeout_remove(curve_timer_id ); 
// 	curve_timer_id     =gtk_timeout_add (  1000, (GtkFunction)HisCurvRedrawE,crt_n); 
// 	
// 	HisCurvRedrawE(crt_n);
// 	printf("@@@@  in DisplayCB\n");
}




void AnaValueADD(GtkWidget *w, gpointer data )
{
	int crt_n,i;
	
	printf("####in AnaValueADD,select_points_num1=%d,select_point=%d\n",select_points_num1,select_point);
	
	/*GetCrtNumberOfWgt ( w, &crt_n );*/	
	
	if(select_points_num1>CURVE_NUM)
	   {	
		gdk_beep(); 	
		//PopupMessageBox(crt_n,"???è¯?ï¼??????©ç????²çº¿ä¸???°å·²ç»?è¶?è¿?6ä¸?!");
	}
	else
	{
		select_points_num1 ++;			
		select_points[select_points_num1-1]=select_point;
		
	}		   	
}




void FocusInOutEntry(GtkWidget *text_w, GdkEventFocus *e ,gpointer  data  )
{
	char *ss1,   ss2[30];
	struct tm when ;
	time_t now,result;
	int year ,mon, day, hh, mm , ss ;
	
	if ((gint)data == STAR_T ){
		ss1 = gtk_entry_get_text(starTmEntry);
	}   
	else
	{
		ss1 = gtk_entry_get_text(endTmEntry);
	}    
	
	
	sscanf(ss1 , "%4d-%2d-%2d %2d:%2d:%2d" , &year,&mon,&day,&hh,&mm,&ss);
	/*   printf ("scanf is :%4d-%2d-%2d %2d:%2d:%2d \n" ,year,mon,day,hh,mm,ss );  */
	when.tm_year = year -1900 ; 
	when.tm_mon = mon -1 ;
	when.tm_mday = day ;
	when.tm_hour = hh  ; 
	when.tm_min = mm ;
	when.tm_sec = ss ;
	when.tm_isdst = -1 ;
	
	if ((result = mktime(&when)) != (time_t) -1 )
	{
		printf( "success is : %4d-%2d-%2d %2d:%2d:%2d\n",when.tm_year+1900,when.tm_mon+1,
			when.tm_mday,when.tm_hour,when.tm_min,when.tm_sec ) ; 
	}
	else
	{   
		printf( "%s","YYYY-MM-DD HH:MM:SS\n");
		return    ;  /* error!!! */
	}
	
	
    
	sprintf(ss1 , "%4d-%2d-%2d %2d:%2d:%2d",when.tm_year+1900,when.tm_mon+1,when.tm_mday,when.tm_hour,when.tm_min,when.tm_sec ) ;  
	
	if ((gint)data == STAR_T ){
		gtk_entry_set_text(starTmEntry,ss1);
		strcpy (ss2,ss1);
		MakeFormatTime( &when ,ss2 , 900 ,Interval );
		
		gtk_entry_set_text(endTmEntry,ss2);    
		
	}   
	else
	{
		gtk_entry_set_text(endTmEntry,ss1);
		MakeFormatTime( &when ,ss2 , -900 ,Interval);
		memcpy (&starwhen, &when ,sizeof (struct tm));
		gtk_entry_set_text(starTmEntry,ss2);
	}     
	
	
	
}


void SetOtherTime(GtkWidget *text_w, gpointer  data)
{
    if ((gint)data == 0 )
        printf("set end time ");
    else
		printf("set start time "); 
}


/**************************************************************************************************              
s--return time string ; add_num--want add num , type--kind         

************************************************************************************************* */
int MakeFormatTime(struct tm  *Tm , char *s ,int add_num , enum T_Interval type )
{
	struct tm when ;
	time_t now,result;  
	
	switch(type)
	{
	case A_SEC1:
		Tm->tm_sec += add_num;
		break;
	case A_MIN1:
		Tm->tm_min += add_num; 
		break;
	case FIV_MIN1:
		Tm->tm_min  +=  add_num * 5; 
		break;
	case FIFTEEN_MIN1:
		Tm->tm_min +=  add_num * 15;
		break;
	case A_HOUR1:
		Tm->tm_hour +=  add_num  ;
		break;
	default:
		break; 	
		
	}
	Tm->tm_isdst = -1 ;
	if ((result = mktime(Tm)) != (time_t) -1 )
	{
        sprintf(s,"%4d-%2d-%2d %2d:%2d:%2d\n",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,Tm->tm_min,Tm->tm_sec ) ; 
        return 0  ;  /* success!!! */
	}
	else
	{   
		sprintf(s, "%s","YYYY-MM-DD HH:MM:SS\n");
		return -1  ;  /* error!!! */
	}
	
	
}


void ChangeInterval(GtkWidget * widget, gpointer data)
{	 
	
	Interval =  (gint) data; 
	
	/*	g_message ("interval    is %d\n" ,  Interval   );  */
}


void hist_draw_area_allocate(GtkWidget *widget,GtkAllocation *a ,gpointer data)
{  
    int crt_n = (gint) data;
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
    memcpy(&coordin[crt_n].alloc , a , sizeof(GtkAllocation)) ;
    coordin[crt_n].rc.x1 = 50 ;  /*coordin[crt_n].alloc.width * .05 */
    coordin[crt_n].rc.x1 = 50 ;
    coordin[crt_n].rc.x2 = 50 ;
    coordin[crt_n].rc.y1 = 50 ;
    coordin[crt_n].rc.y2 = 50 ;
    coordin[crt_n].xLength = coordin[crt_n].alloc.width- coordin[crt_n].rc.x1 -  coordin[crt_n].rc.x2;
    coordin[crt_n].yLength = coordin[crt_n].alloc.height- coordin[crt_n].rc.y1 -  coordin[crt_n].rc.y2;
    coordin[crt_n].xstep = (double)(coordin[crt_n].xLength) /  (double) CANVAS_MAX_POINTER ;
    
	HisCurvRedraw (  crt_n,off_draw_area[crt_n]  , canvas_gc[crt_n] ,   a->width    ,a->height); 
	g_message("\n Allocation ! x:%d ,y:%d , width:%d height : %d \n ",a->x ,a->y ,a->width, a->height);
}

void hist_draw_area_realize(GtkWidget *widget,gpointer data)
{
	g_message ( " hist_draw_area_realize!!!  \n");
    int crt_n = (gint) data ;
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
    
	
    g_message ( " hist_draw_area_realize!!!  \n");
    canvas_gc[crt_n] = gdk_gc_new(widget->window);
	/* draw_area = widget ;   */
    off_draw_area[crt_n] = gdk_pixmap_new(widget->window , 1200, 1024,-1 ); 
    gdk_window_set_background(widget->window, &pixels[crt_n][BLACK]);
    rcdTag[crt_n].rect.x	= 100;
    rcdTag[crt_n].rect.y	= 100;
    rcdTag[crt_n].rect.width	= 100;
    rcdTag[crt_n].rect.height  = 200;
    
	
}

void ExitHistCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	gtk_widget_destroy(GTK_WIDGET(trend_main_w[crt_n])); 
}

void DestoryHistCurveShell(GtkWidget *widget,   gpointer data)
{     
	
	
    
	int crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
	int i ;  
	g_message("\n quit histcurve !!!!! \n");
	exit_flag = TRUE;
	gtk_timeout_remove(curve_timer_id ); 
//	gtk_widget_destroy(GTK_WIDGET(trend_main_w[crt_n])); 
	trend_main_w[crt_n] = (GtkWidget *)NULL ;
	return FALSE;
}

gint	HistDatatypePressProc (GtkWidget *w, gpointer data )
{
	int i;
	char cur_datatype_name[20];
	int	crt_n=0;
	gchar *pConvert=0;

	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return; 
  		
	strcpy(cur_datatype_name,gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(data)->entry)));
	
	strcpy(cur_datatype_name,pConvert=_fromUtf8(cur_datatype_name,strlen(cur_datatype_name),NULL,NULL,NULL));
	G_SAFE_FREE(pConvert);//dzy1017
	if(strcmp(cur_datatype_name,"")==0)
		return;
	for(i=0;i<sizeof(recordlongname)/sizeof(recordlongname[0]);i++)
	{
		if(strcmp(recordlongname[i],cur_datatype_name)==0)  
		{	
			break;
		}
	}
	
	hist_dtype    		= i;
	/*
	UpdateTrendNameList ( crt_n, trend_stn,hist_dev );	 
	*/   	
	return TRUE;
}

void ProgHisCurve (GtkWidget *parent ,int  crt_n )
{
    if (  InitShareMemory_S(crt_n )== FALSE)  
    {    
		
		gdk_beep(); 
		PopupMessageBox(crt_n , "OIX: Open share Mem error!");
		return ; 
    } 
	if (trend_main_w[crt_n]==(GtkWidget*)NULL)
	{
		trend_main_w[crt_n]  = CreateProgHisCurveShell  ( parent, crt_n   ) ;
		gtk_window_set_position(trend_main_w[crt_n] ,GTK_WIN_POS_CENTER_ON_PARENT);
		
		gtk_widget_show(trend_main_w[crt_n] );
		UpdateAnaValLst ( crt_n ); 
		seldatahis.next_pos  = trenddatahis.next_pos = 0; 
	}else
	{
		gtk_window_present(trend_main_w[crt_n] );
	}
}



static GtkWidget *CreateProgHisCurveShell (GtkWidget *parent , int  crt_n )
{
	GtkWidget  *vbox,*vbox1, *hbox,*hbox1,*button_add_point,*button_del_point;
	GtkWidget  *frame;
	GtkWidget  *toolbar,*toolbarDataSel,*ana_name_list_scrollw,*sel_ana_list_scrollw;
	GtkWidget  *table,*hrule, *vrule;
	GtkWidget  *windows ;
	SHORT	i   ;
	int	shmid2;
	gchar *pConvert=0;
	/*      gtk_set_locale();
	gtk_rc_add_default_file("./gtkrc.zh");*/
	
	windows=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	/*     gtk_widget_realize(GTK_WIDGET(windows)) ;  */
	
//	gtk_window_set_default_size(GTK_WIDGET( windows),1224, 1000);
//	gtk_window_maximize(GTK_WINDOW(windows));
	
	gtk_window_set_title(GTK_WINDOW(windows), pConvert=_toUtf8EX(OIX_CSTR_CURVE_VIEW));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
	
	gtk_signal_connect(GTK_OBJECT(windows),
		"destroy",
		GTK_SIGNAL_FUNC( DestoryHistCurveShell),
		crt_n);
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(windows), vbox);
	gtk_widget_show(vbox);
	
	toolbar =  CreateHisCurvToolbar(windows, crt_n);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_widget_show(toolbar);    
	
	
    /************************** frame construct!! ******************************/
	
	hbox=gtk_hbox_new(FALSE, 3);    
	
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE,TRUE, 0);  
	gtk_widget_show(hbox);
	vbox1=gtk_vbox_new(FALSE, 3);
	
	gtk_widget_set_usize (vbox1, 160, -1);
	gtk_box_pack_start(GTK_BOX(hbox ), vbox1, FALSE, TRUE, 0);
	gtk_widget_show(vbox1);    
	
	
	
	/***************** list1 window code ***************************************/                     
	
	
	sel_ana_list_scrollw=gtk_scrolled_window_new (NULL, NULL);      
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sel_ana_list_scrollw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_widget_set_usize (sel_ana_list_scrollw, -1, 750); 
	
	gtk_box_pack_start(GTK_BOX(vbox1), sel_ana_list_scrollw, FALSE, FALSE, 0);
	gtk_widget_show (sel_ana_list_scrollw); 
	
	list_ana_val = gtk_list_new(); 
	
	gtk_scrolled_window_add_with_viewport(GTK_CONTAINER(sel_ana_list_scrollw),list_ana_val );  
	gtk_widget_show (list_ana_val); 
	
	gtk_signal_connect(GTK_OBJECT(list_ana_val), "selection_changed",
		GTK_SIGNAL_FUNC(SelValueListCB), ANA_LIST); 
	
	
	/********************* two button code here !! *********************************/
	
	hbox1=gtk_hbox_new(FALSE, 15);
	gtk_widget_set_usize (GTK_WIDGET(hbox1),-1 , 30);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox1 ,FALSE, FALSE, 0);
	gtk_widget_show(hbox1);
	
	GtkWidget *image1 ;
	button_add_point = gtk_button_new_with_label("Add:"); 
	
	
	
	gtk_box_pack_start (GTK_BOX (hbox1), button_add_point, TRUE, TRUE, 0);
	button_del_point = gtk_button_new_with_label("Delete:");
	gtk_box_pack_start (GTK_BOX (hbox1), button_del_point, TRUE, TRUE, 0);     
	
	gtk_signal_connect(GTK_OBJECT(button_add_point), "clicked",
		GTK_SIGNAL_FUNC(ListValAddCB), button_add_point);
	
   	gtk_signal_connect(GTK_OBJECT(button_del_point), "clicked",
		GTK_SIGNAL_FUNC(ListValRemoveCB), button_del_point);			      
	
   	gtk_widget_show(button_add_point);
	gtk_widget_show(button_del_point);   	       
	
	/***************** list2 window code ***************************************/ 
	
	ana_name_list_scrollw=gtk_scrolled_window_new (NULL, NULL);      
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ana_name_list_scrollw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	
	
	gtk_box_pack_start(GTK_BOX(vbox1), ana_name_list_scrollw, TRUE,TRUE, 0);
	/*   gtk_widget_show (ana_name_list_scrollw);  */
	
	sel_ana_val_list = gtk_list_new ();    
	gtk_scrolled_window_add_with_viewport(GTK_CONTAINER(ana_name_list_scrollw),sel_ana_val_list );    
	gtk_widget_show (sel_ana_val_list);     
	
	gtk_signal_connect(GTK_OBJECT(sel_ana_val_list), "selection_changed",
		GTK_SIGNAL_FUNC(SelValueListCB),SEL_ANA_LIST); 		        
	
	
	
	
	
    
    /***************** draw area code ***************************************/     
	
	frame = gtk_frame_new("frame1");    
	gtk_box_pack_start(GTK_BOX(hbox ), frame, TRUE,TRUE, 0);
	gtk_widget_show(frame);
	
	
	hist_draw_area[crt_n]= gtk_drawing_area_new();
	
	
	gtk_container_add( (frame),hist_draw_area[crt_n] );
	
	
	gtk_widget_set_events( hist_draw_area[crt_n],  GDK_POINTER_MOTION_MASK |
		GDK_BUTTON_PRESS_MASK  |
		GDK_BUTTON_RELEASE_MASK  |
		GDK_EXPOSURE_MASK );
	
	gtk_signal_connect (GTK_OBJECT (hist_draw_area[crt_n]), "realize", 
		GTK_SIGNAL_FUNC (hist_draw_area_realize), crt_n);
	gtk_signal_connect (GTK_OBJECT (hist_draw_area[crt_n]), "size_allocate", 
		GTK_SIGNAL_FUNC (hist_draw_area_allocate), crt_n);
	
	gtk_signal_connect (GTK_OBJECT (hist_draw_area[crt_n]),"expose_event",
		GTK_SIGNAL_FUNC(hisCurve_expose_events), crt_n) ; 
	
	
	gtk_signal_connect(GTK_OBJECT (hist_draw_area[crt_n]), "button_press_event",
		GTK_SIGNAL_FUNC (button_press_CB),  crt_n);
	gtk_signal_connect(GTK_OBJECT (hist_draw_area[crt_n]), "button_release_event",
		GTK_SIGNAL_FUNC (button_release_CB),  crt_n);
	gtk_signal_connect (GTK_OBJECT (hist_draw_area[crt_n]),"motion_notify_event",
		GTK_SIGNAL_FUNC(motion_hist_CB),crt_n) ;
	
	gtk_widget_realize(GTK_WIDGET(hist_draw_area[crt_n])) ;
	gtk_widget_show (hist_draw_area[crt_n]);
	gtk_widget_show_all(windows);	  
	
	
	
	return windows;
}
void WriteShareMem (int crt_n)
{
	crt_n = 0 ;
	for(;;){
		
		if(shm_curve_addr->data_curve[crt_n].lockflag == IDLE_FLAG)	break;
		
		Sleep( 1000);
	}  
	
	shm_curve_addr->data_curve[crt_n].querytype = 0;
	shm_curve_addr->data_curve[crt_n].lockflag = LOCK_FLAG  ;    /* IDLE_FLAG */
	shm_curve_addr->data_curve[crt_n].num=6;
	sprintf(shm_curve_addr->data_curve[crt_n].starttime,gtk_entry_get_text(GTK_ENTRY(starTmEntry)));
	sprintf(shm_curve_addr->data_curve[crt_n].endtime,  gtk_entry_get_text(GTK_ENTRY(endTmEntry)));
	sprintf(shm_curve_addr->data_curve[crt_n].seg[0].logicalname,"LYX.LYX_F1.ANA.DZ_A_V");
	sprintf(shm_curve_addr->data_curve[crt_n].seg[1].logicalname,"LYX.LYX_F1.ANA.DZ_A_A");
	sprintf(shm_curve_addr->data_curve[crt_n].seg[2].logicalname,"LYX.LYX_F1.ANA.DZ_B_V");
	
	sprintf(shm_curve_addr->data_curve[crt_n].seg[3].logicalname,"LYX.LYX_F1.ANA.DZ_C_V");	
	sprintf(shm_curve_addr->data_curve[crt_n].seg[4].logicalname,"LYX.LYX_F1.ANA.DZ_A_V");
	sprintf(shm_curve_addr->data_curve[crt_n].seg[5].logicalname,"LYX.LYX_F1.ANA.DZ_A_V");							
	shm_curve_addr->data_curve[crt_n].interval=ONE_MIN; 
	shm_curve_addr->data_curve[crt_n].queryflag=1;
	shm_curve_addr->data_curve[crt_n].lockflag= IDLE_FLAG      ;  	
	
	
}



void *FetchDataFromSrv(void *arg)
{
	int  i;
	unsigned char curid=0;
	exit_flag = FALSE;     
	while(  exit_flag == FALSE )   
	{  
        if(shm_curve_addr == NULL)
		{
			printf ("OIX: error when  copy shm_curve_addr to  data_curve !!!  ");	
			goto ExitThr;
		}     
		memcpy( &data_curve,  shm_curve_addr , sizeof(HDB_CURVE )); 
		/*      printf("thread %d sleeping 5 seconds ...\n", thr_self() );   */
		Sleep(1); 
		
	}      
ExitThr:    
	printf("\nthread %d return \n", pthread_self());
	/* shmdt(shm_curve_addr);  */
	pthread_exit(0);
	return (NULL);
}

GtkWidget *cmbHisCurvDev;
GList *histCurv_stn_lst=NULL,*histCurv_dev_lst=NULL,*histCurv_datatype_lst=NULL;





void UpdateHistStnSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
	
	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;
	
	hist_curve_db_info[crt_n].select_pt.stn_id= (UCHAR)(int)data;
	UpdateHisDevMenu(crt_n );
	UpdateAnaValLst ( crt_n );  
}


void UpdateHistDevSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
	
	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;
	
	hist_curve_db_info[crt_n].select_pt.dev_id= (UCHAR)(int)data;
	UpdateHistDataTypeMenu(crt_n);
	UpdateAnaValLst (   crt_n );
}



void UpdateHistDataTypeSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
	
	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;
	
	hist_curve_db_info[crt_n].select_pt.data_type= (UCHAR)(int)data;
	UpdateAnaValLst ( crt_n );  
}




void UpdateHisDevMenu(int crt_n) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	UCHAR	stn_id, dev_id;
	int		i,grp_num;
	GtkWidget *menu,*items;
	gchar *pConvert=0;
	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(hist_curve_db_info[crt_n].dev_optionmenu));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(hist_curve_db_info[crt_n].dev_optionmenu));
	
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);
	
	stn_id= hist_curve_db_info[crt_n].select_pt.stn_id;
	if(-1==GetGroupNumById(stn_id, &grp_num))
		return;
	
	dev_id= 0;
	for(i=1; i<=grp_num; i++) 
	{  		
		if(-1==GetGroupCommentById(stn_id, (UCHAR)i, dev_comment))
			continue;
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(dev_comment));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateHistDevSelect), (gpointer)i);
		if(!dev_id)
			dev_id= i;
	}
	
	gtk_option_menu_set_menu(GTK_OPTION_MENU(hist_curve_db_info[crt_n].dev_optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(hist_curve_db_info[crt_n].dev_optionmenu), 0);
	hist_curve_db_info[crt_n].select_pt.dev_id= dev_id;
	gtk_widget_show_all(hist_curve_db_info[crt_n].dev_optionmenu);
}



void UpdateHistDataTypeMenu(int crt_n) 
{
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	UCHAR	stn_id, dev_id, data_type;
	int		i,j,rec_num,entry_num;
	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	gchar *pConvert=0;
	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(hist_curve_db_info[crt_n].datatype_optionmenu));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(hist_curve_db_info[crt_n].datatype_optionmenu));
	
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);
	
	stn_id= hist_curve_db_info[crt_n].select_pt.stn_id;
	dev_id= hist_curve_db_info[crt_n].select_pt.dev_id;
	if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
		return;
	
	data_type= 0;
	menu_index= active_index= 0;
	for(i=1,j=0; j<rec_num; i++) 
	{  		
		if(i>=MAX_DATATYPE_NUM)
			break;
		if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)i, &entry_num))
			continue;
		GetRecLongnameByType(i, datatype_longname);
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(datatype_longname));	 
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateHistDataTypeSelect), (gpointer)i);
		j++;
		if(!data_type)
		{
			data_type= i;
			active_index= 0;
		}
		if(i==hist_curve_db_info[crt_n].select_pt.data_type)
		{
			data_type= i;
			active_index= menu_index;
		}
		menu_index++;
	}
	
	gtk_option_menu_set_menu(GTK_OPTION_MENU(hist_curve_db_info[crt_n].datatype_optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(hist_curve_db_info[crt_n].datatype_optionmenu), active_index);
	hist_curve_db_info[crt_n].select_pt.data_type= data_type;
	gtk_widget_show_all(hist_curve_db_info[crt_n].datatype_optionmenu);
}


GtkWidget* CreateIntervalMenu(int crt_n) 
{
	char *interval[] = {OIX_CSTR_1_SEC , OIX_CSTR_1_MIN , OIX_CSTR_5_MIN , OIX_CSTR_15_MIN , OIX_CSTR_1_HOUR };  
	/* char *interval[] = {"1 sec","1 min","5 min","15 min","1 hour"};   */
	int		i;
	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	gchar *pConvert=0;
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 150, -1);
	menu_index= active_index= 0;
	for(i=0; i< MAX_INTERVAL_NUM  ; i++) 
	{  		
		
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(interval[i]));	 
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(ChangeInterval), (gpointer)i);
	}
	optionmenu = gtk_option_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	return optionmenu;
}







GtkWidget* CreateHistStnMenu(int crt_n) 
{
	char	stn_comment[STATION_COMMENT_SIZE];
	int		i,stn_num;
	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	UCHAR	stn_id;
	gchar *pConvert=0;
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);
	
	GetStationNum(&stn_num);
	stn_id= 0;
	menu_index= active_index= 0;
	for(i=1; i<=stn_num; i++) 
	{  		
		UCHAR	stn_id;
		
		if(-1==GetStnIdByIndex(i, &stn_id))
			continue;
		
		if( GetStnCommentById(stn_id, stn_comment)==-1 )
			continue;
		
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(stn_comment));	 
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateHistStnSelect), (gpointer)i);

		if(stn_id==hist_curve_db_info[crt_n].select_pt.stn_id)
			active_index= menu_index;

		menu_index++;
	}
	
	optionmenu = gtk_option_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	hist_curve_db_info[crt_n].select_pt.stn_id= stn_id;
	hist_curve_db_info[crt_n].stn_optionmenu= optionmenu;
	return optionmenu;
}




GtkWidget* CreateHistDevMenu(int crt_n) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	UCHAR	stn_id, dev_id;
	int		i,grp_num;
	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	gchar *pConvert=0;
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);
	
	stn_id= hist_curve_db_info[crt_n].select_pt.stn_id;
	if(-1==GetGroupNumById(stn_id, &grp_num))
		return;
	
	dev_id= 0;
	menu_index= active_index= 0;
	for(i=1; i<=grp_num; i++) 
	{  		
		if(-1==GetGroupCommentById(stn_id, (UCHAR)i, dev_comment))
			continue;
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(dev_comment));	 
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateHistDevSelect), (gpointer)i);
		if(!dev_id)
		{
			dev_id= i;
			active_index= 0;
		}
		if(i==hist_curve_db_info[crt_n].select_pt.dev_id)
		{
			dev_id= i;
			active_index= menu_index;
		}
		menu_index++;
	}
	
	optionmenu = gtk_option_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	hist_curve_db_info[crt_n].select_pt.dev_id= dev_id;
	hist_curve_db_info[crt_n].dev_optionmenu= optionmenu;
	return optionmenu;
}



GtkWidget* CreateHistDataTypeMenu(int crt_n) 
{
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	UCHAR	stn_id, dev_id, data_type;
	int		i,j,rec_num,entry_num;
	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	gchar *pConvert=0;
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);
	
	stn_id= hist_curve_db_info[crt_n].select_pt.stn_id;
	dev_id= hist_curve_db_info[crt_n].select_pt.dev_id;
	if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
		return;
	
	data_type= 0;
	menu_index= active_index= 0;
	for(i=1,j=0; j<rec_num; i++) 
	{  		
		if(i>=MAX_DATATYPE_NUM)
			break;
		if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)i, &entry_num))
			continue;
		GetRecLongnameByType(i, datatype_longname);
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(datatype_longname,strlen(datatype_longname),NULL,NULL,NULL));	 
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateHistDataTypeSelect), (gpointer)i);
		j++;
		if(!data_type)
		{
			data_type= i;
			active_index= 0;
		}
		if(i==hist_curve_db_info[crt_n].select_pt.data_type)
		{
			data_type= i;
			active_index= menu_index;
		}
		menu_index++;
	}
	
	optionmenu = gtk_option_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	hist_curve_db_info[crt_n].select_pt.data_type= data_type;
	hist_curve_db_info[crt_n].datatype_optionmenu= optionmenu;
	gtk_widget_show(optionmenu);
	return optionmenu;
}




GtkWidget * CreateDataSelToolbar (GtkWidget * parent, int crt_n)
{
	GtkWidget *box;  	
	GtkWidget *toolbar; 
	GtkWidget *menuStn ,*menuDev ,*menuDataType ; 
	
	toolbar = gtk_handle_box_new();
	
	box = gtk_hbox_new (FALSE, 5);
	gtk_container_add(GTK_CONTAINER(toolbar), box);
	gtk_container_border_width(GTK_CONTAINER(box), 5);
	menuStn =  CreateHistStnMenu(crt_n) ;
	menuDev =  CreateHistDevMenu(crt_n) ;
	menuDataType = CreateHistDataTypeMenu(crt_n) ; 
	gtk_widget_show (menuStn);
	gtk_widget_show (menuDev);
	gtk_widget_show (menuDataType);
	gtk_box_pack_start(GTK_BOX(box), menuStn , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box),menuDev  , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), menuDataType , FALSE, FALSE, 0);
	
	
	
	return (toolbar);
}


GtkWidget *
CreateHisCurvToolbar (GtkWidget* parent£¬ int crt_n)
{
	int	i ;
	GtkWidget *box, *tempwid,*menuInterval;  	
	GtkWidget *vbox, *hbox;	
	GtkWidget *toolbar; 
	GtkWidget *cmbInterval;    	
	GtkWidget	*button_display,*button_exit;
	
	char timeVal[22];
	struct tm when ;
	time_t now1; 
	gchar *pConvert=0;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER)
		return;
	
	
	toolbar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);
	
	/*  *************  Time Interval  combobox  ****************** */
	
   	tempwid = gtk_label_new ( pConvert=_toUtf8(OIX_CSTR_TIME_INTERVAL) );
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);
	
	
	
	/*  *************  star time entry!!!!  ****************** */ 
	menuInterval = CreateIntervalMenu(  crt_n) ;        
	gtk_widget_show (menuInterval);
	gtk_box_pack_start(GTK_BOX(box), menuInterval , FALSE, FALSE, 0);
	
	
	
	
	
	tempwid = gtk_label_new (pConvert=_toUtf8( OIX_CSTR_START_TIME ));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);
	
	starTmEntry = gtk_entry_new_with_max_length (50); 	
	
	gtk_widget_set_usize (starTmEntry, 180, -1);
	
	time(&now1);  
	when = *localtime(&now1);
	memcpy (&starwhen, &when ,sizeof (struct tm));
	sprintf(timeVal,"%4d-%2d-%2d %2d:%2d:%2d\n",when.tm_year+1900,when.tm_mon+1,when.tm_mday,when.tm_hour,when.tm_min,when.tm_sec ) ; 
	gtk_entry_set_text(GTK_ENTRY(starTmEntry),timeVal);
  	 
	
    
	gtk_entry_set_position(GTK_ENTRY (starTmEntry),0);
	gtk_entry_set_editable(GTK_ENTRY (starTmEntry),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(starTmEntry),TRUE);
	gtk_box_pack_start (GTK_BOX (box), starTmEntry, FALSE, FALSE, 0);
	gtk_widget_show (starTmEntry); 
	gtk_widget_set_events(starTmEntry,  GDK_FOCUS_CHANGE_MASK );
	
   	
	gtk_signal_connect_after(GTK_OBJECT(starTmEntry),
		"focus_out_event",
		GTK_SIGNAL_FUNC(FocusInOutEntry),
		      STAR_T);
	
	gtk_signal_connect(GTK_OBJECT(starTmEntry),
		"activate",
		GTK_SIGNAL_FUNC(SetOtherTime),
		NULL);
	
	/*  *************  end  time entry!!!!  ****************** */ 
	
	tempwid = gtk_label_new (pConvert=_toUtf8( OIX_CSTR_END_TIME ));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);
	
	endTmEntry = gtk_entry_new_with_max_length (50); 	
	
	gtk_widget_set_usize (endTmEntry, 180, -1);
  	 
	gtk_entry_set_text(GTK_ENTRY(endTmEntry),timeVal);
	
	
    
	gtk_entry_set_position(GTK_ENTRY (endTmEntry),0);
	gtk_entry_set_editable(GTK_ENTRY (endTmEntry),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(endTmEntry),TRUE);
	gtk_box_pack_start (GTK_BOX (box), endTmEntry, FALSE, FALSE, 0);
	gtk_widget_show (endTmEntry); 
	
	gtk_widget_set_events(endTmEntry,  GDK_FOCUS_CHANGE_MASK );
	
   	
	gtk_signal_connect_after(GTK_OBJECT(endTmEntry),
		"focus_out_event",
		GTK_SIGNAL_FUNC(FocusInOutEntry),
		END_T);
	gtk_signal_connect(GTK_OBJECT(endTmEntry),
		"activate",
		GTK_SIGNAL_FUNC(SetOtherTime),
		0);	
	
	
	
	/******************  display  button   *********************/
	
	button_display = gtk_button_new_with_label(pConvert=_toUtf8(OIX_CSTR_DISPLAY));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_box_pack_start (GTK_BOX (box), button_display, FALSE, FALSE, 0);    	
	
    
	gtk_signal_connect(GTK_OBJECT(button_display), "clicked",
		GTK_SIGNAL_FUNC(DisplayCB),crt_n); 	
	
	gtk_widget_show(button_display);
	
	button_exit = gtk_button_new_with_label("exit");
	gtk_box_pack_end (GTK_BOX (box), button_exit, FALSE, FALSE, 0); 
	gtk_widget_show (button_exit);
	
	gtk_signal_connect(GTK_OBJECT(button_exit),
		"clicked",
		GTK_SIGNAL_FUNC(ExitHistCurveShell),
		crt_n  );
	
	
	GtkWidget *menuStn ,*menuDev ,*menuDataType ; 
	
	/***********************************************************************/
	
	
	
  	 
	menuStn =  CreateHistStnMenu(crt_n) ;
	menuDev =  CreateHistDevMenu(crt_n) ;
	menuDataType = CreateHistDataTypeMenu(crt_n) ; 
	gtk_widget_show (menuStn);
	gtk_widget_show (menuDev);
	gtk_widget_show (menuDataType);
	gtk_box_pack_start(GTK_BOX(box), menuStn , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box),menuDev  , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), menuDataType , FALSE, FALSE, 0);
	
	
	
	return (toolbar);
}

