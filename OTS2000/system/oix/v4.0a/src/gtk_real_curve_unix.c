
/*-----------------------------------------------------------------------
	
	 
	Created by chi.hailong
	Create  Date : 2005.11.26
	Last Update  : 2006.6.11
     

---------------------------------------------------------------------*/
#define	 REAL_CUEVE
#ifdef   REAL_CUEVE


#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <time.h>
#include 	<pthread.h>
#include 	<signal.h>
#include 	<sys/shm.h>
#include	"../inc/oix.h"
 


short	real_dev = 1,realCurv_stn=1,real_dtype=1;
DB_TAB_WINDOW_INFO real_curve_db_info[MAX_CRT_NUMBER];
WDQS_SHM_DATA  *real_curve_pt ;
int real_timeout_id ; 
/********************* gtk_real_curve.h ************************/

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
        RECT          rc ;
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
        char  dbLongName[MAX_CURVE_NUM][256];
        float   hi_range[MAX_CURVE_NUM];   /* hi_limit */
        float   low_range[MAX_CURVE_NUM];
        struct tm Time  ;
        int  canMove ;
}	RCD_LABLE;

GtkWidget 	*real_curve_main_w[MAX_CRT_NUMBER];
GdkPixmap	*off_draw_area[MAX_CRT_NUMBER];
GdkGC		*canvas_gc[MAX_CRT_NUMBER];
COORDINATE       coordin[MAX_CRT_NUMBER];
GtkWidget       *real_draw_area[MAX_CRT_NUMBER] ;
RECT  mycur ;
RCD_LABLE rcdLabel[MAX_CRT_NUMBER] ;   /* label */ 

GtkWidget 	*starTmEntry,*endTmEntry,*list_ana_val,*sel_ana_val_list,   *scroll_win ;
 


int 		mins , seconds, hours;     /* How many minitues or seconds or hours  */
/*  int  		interval_Type ;   */
enum 		T_Interval{A_SEC1,A_MIN1,FIV_MIN1,FIFTEEN_MIN1,A_HOUR1};
 
SHM_HDB        data_curve,*shm_curve_addr ;
int		shm_id_curve;
 
void UpdateRealDevMenu(int crt_n) ;

//pthread_t 	tid[NUM_THREADS];      /* array of thread IDs */
gint    real_curve_timer_id = 0;
short   CLR_GROUP[MAX_CURVE_NUM] = {RED1,CLD_GRY1,BLUE1,WHITE,GREEN,YELLOW} ;

/********************** end ********************************/


extern void 	DestroyDlg(GtkWidget *w);
extern void 	SgzyDevSelectCB (GtkWidget* w, gpointer data);
extern void	DrawSgzyCurve(int crt_n,GtkWidget* w,GdkGC* gc,SHORT now_i,SHORT num,int x_left,int y_top,int width,int height,float ys,float ye,char longname[DBNAME_LEN*2]);

 
void            real_draw_area_realize(GtkWidget *widget,gpointer data);
void            realCurvRedraw (int crt_n,GdkDrawable *drawable , GdkGC *gc ,int width ,int height);

void            UpdateRealDataTypeMenu(int crt_n) ;

const 		gchar *list_item_data_r = "list_item_row_no" ;
const		gchar *lst_item_data   = "lst_item_data"   ;

/****** **                static	variable    *******/
extern foreground[9][10];
short	FetchFloor();	
static	FILE	*fp;
static  short   trend_add_item ;
static  short   trend_del_item ;
static  int	zero_y , y_h;    /*  x` axis from left top of canvas    */

static  short   Interval ;
static  struct tm starwhen ;


int X_scale [MAX_CRT_NUM] = {0,0,0} ;

int trend_station_id, i;
int  select_point,select_points[MAX_WDQS_COUNT],sel_pt_num=0;
POINTER  curv_ptr[6]  ;
#ifdef  SUN
TRENDDATAHIS   trenddatahis;
TRENDDATA	RealCurvePt,tmpRealCurvePt ;
#endif

GtkWidget *CreateRealCurvToolbar (GtkWidget * parent);
static GtkWidget *CreateProgRealCurveShell (GtkWidget *parent , int  crt_n );

void DrawrcdLabel(int crt_n ,GdkDrawable *drawable, GdkGC *gc ,RCD_LABLE *rcd )
{
   
        rcd->bg	= canvas_bg_color;
	 
	char record[20]; 
        gdk_gc_set_function(gc, GDK_COPY);
        gdk_gc_set_foreground(gc,&pixels[crt_n][BLACK]);
	gdk_draw_rectangle ( drawable,gc , TRUE,rcd->rect.x,rcd->rect.y, rcd->rect.width , rcd->rect.height); 
	gdk_gc_set_foreground(gc,&pixels[crt_n][WHITE]);
        gdk_draw_rectangle ( drawable,gc , FALSE,rcd->rect.x,rcd->rect.y, rcd->rect.width , rcd->rect.height); 
        
        
        
        gdk_gc_set_foreground(gc,&pixels[crt_n][GREEN]);
          gdk_gc_set_foreground(gc,&pixels[crt_n][GREEN]);
        strcpy(record , "TIME:"); 
        gdk_draw_string( drawable, font_set[0], gc,  rcd->rect.x +15, rcd->rect.y +15 , record);
        sprintf(record , "%d%s%d:%d ", rcd->Time.tm_mday ,_CS_(OIX_CSTR_DAY), rcd->Time.tm_hour,rcd->Time.tm_min); 
        gdk_draw_string( drawable, font_set[0], gc,  rcd->rect.x +10, rcd->rect.y +30 , record);
        /*sprintf(record , "X :%d ",  rcd->val[0]); 
        gdk_draw_string( drawable, font_set[0], gc,  rcd->rect.x +10, rcd->rect.y +45 , record);
        
       */
        for (i = 0 ; i< MAX_CURVE_NUM; i++)
            {
                sprintf(record , "Y[%d] :%d\0",  i ,rcd->val[i]); 
                gdk_gc_set_foreground (gc, &pixels[ crt_n][CLR_GROUP[i]]); 
                gdk_draw_string( drawable, font_set[0], gc,  rcd->rect.x +10, rcd->rect.y + 50 + i *25 , record);
        }   
    
}
void DrawXLine(int crt_n ,GdkDrawable *drawable, GdkGC *gc ,int x , int h , short mod )
{  
    char ss[30];
   
    struct tm when ,when1;
    time_t  result, now ; 
 /*   time(&now);
    when1 = *localtime(&now) ;
    when1.tm_min  -=  15 ;
    when1.tm_isdst = -1 ;
 */
  /*  if (result = mktime(&when1) == (time_t)-1) return ;*/
    
    
   memcpy (&when1, &starwhen, sizeof(struct tm ));
    short  xi =1  ;
    gdk_gc_set_fill ( gc, GDK_SOLID );	
    if (!(mod %2)){
    	
    	gdk_gc_set_line_attributes(gc,0,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND) ;
  	
  
   
      
       MakeFormatTmR( &when1 ,ss , 60 * mod/2 ,A_SEC1 );  /* 60 :  900/15 = 60 */
       switch (Interval)
       {
            case 0 :  	/* 1 `s */
                 sprintf ( ss,"%02dm:%02ds",  when1.tm_min,  when1.tm_sec);
                 xi = 2 ;
                 break;
            case 1 :  	/* 1 `m */
                 sprintf ( ss,"%02dh%02dm",  when1.tm_hour,when1.tm_min);
                 xi = 2 ;
                 break;
            case 2 :  	/* 5 `m */
                 sprintf ( ss,"%02dD:%02dH",  when1.tm_mday,when1.tm_hour);
                 xi = 1 ;
                 break;
            case 3 :  	/* 15 `m */
                 sprintf ( ss,"%02dD:%02dH",  when1.tm_mday,when1.tm_hour);
                  
                 break;
            case 4 :  	/* 1 `h */
                 sprintf ( ss,"%02D:%02H",  when1.tm_mday,when1.tm_hour);
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


int Y_to_y(int *y)
{   
    
    return zero_y - *y ; 
     
}

void DrawYLine(int crt_n ,GdkDrawable *drawable ,GdkGC *gc,int y , int w , short mod )
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


void HisCurvXYLine(int crt_n ,GdkWindow *drawable ,GdkGC *gc,int width, int height ) 
{  
        
        float stepx ,stepy ; int i;
        int x, y ; 
        
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
	
        for (i = 0 ; i < MAX_CURVE_NUM; i ++)
            {
                if (i < 3)
                    y = 20 ;
                else
                    y = 40;
                    
             if (strcmp(rcdLabel[crt_n].dbLongName[i] , ""))   
                 {
               gdk_gc_set_foreground (gc, &pixels[ crt_n][CLR_GROUP[i]]);   
               gdk_draw_string( drawable, font_set[0], gc, (i % 3)* 250 + 100  ,  y ,rcdLabel[crt_n].dbLongName[i] );  
             }
        }
        
	for (i= 0 ; i <=30 ; i++)
	{
	       	
	       DrawXLine( crt_n ,drawable ,gc,  rcd.rect.x1  + i*stepx ,  rcd.rect.y2 - rcd.rect.y1,i );		
	}
	for (i= 0 ; i <=20 ; i++)
	{
	       	
	       DrawYLine( crt_n , drawable , gc , rcd.rect.y1  + i*stepy ,  rcd.rect.x2 - rcd.rect.x1,i );	
	       	
	}
	y_h = 10 * stepy ;  
	
	
    
}


 
void  realCurve_expose_events(	GtkWidget *widget,GdkEventExpose *event, gpointer data)
{     
        int crt_n = (gint) data ; 
       
     GdkRectangle rc ;
     rc.x = rc.y = 0 ;
     rc.width = widget->allocation.width ;
     rc.height = widget->allocation.height ;
     realCurvRedraw (  crt_n,off_draw_area[crt_n] , canvas_gc[crt_n] , rc.width , rc.height);
     gdk_draw_pixmap(widget->window, canvas_gc[crt_n]  ,off_draw_area[crt_n]  ,event->area.x ,event->area.y , 
			event->area.x ,event->area.y ,event->area.width, event->area.height );
        
       
        
}

int  get_data_from_real_curve(int inI , int curveI,struct tm *setwhen  ,int crt_n ,int nextpos  )
{
     struct tm when ;
     char ss[30]; 
     float value; 
     int tmp ; 
	memcpy (&when, &starwhen ,sizeof (struct tm));
	MakeFormatTmR(  &when ,ss , inI ,Interval );   
	memcpy (setwhen, &when ,sizeof (struct tm));

	tmp =(  tmp = (inI  + nextpos ))>900  ? tmp -900: tmp  ;

	if( RealCurvePt.ptr[curveI].data_type    ==  ANA_TYPE)
		value	= real_curve_pt->wd_dat.fvalue[curveI][tmp  ];
	else
		value	= real_curve_pt->wd_dat.status[curveI][tmp  ];

	return (int)( value ) ;

}


void update_real_rcd(GtkWidget *widget ,int crt_n )
{
    return ;
    GdkRectangle rcTmp ;
    memcpy(&rcTmp ,&rcdLabel[crt_n].rect, sizeof(GdkRectangle));
    rcTmp.x -=2 ;rcTmp.y -= 2 ;
    rcTmp.width +=4 ;rcTmp.height += 4 ;
    DrawrcdLabel( crt_n ,off_draw_area[crt_n], canvas_gc[crt_n] , &rcdLabel[crt_n] );
    gdk_window_invalidate_rect(widget->window ,&rcTmp,1);
}
 

int    motion_real_CB(GtkWidget  *widget,   GdkEventMotion  *event,
                                        gpointer     user_data)
{
int dx , dy ;
int crt_n = 0 ;
int i ,j ; 
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
/*
printf ("\n button is %d ,%d", (bevent)->button , (bevent)->state );

*/
        if (event->x >= coordin[crt_n].rc.x1  && event->x <= coordin[crt_n].rc.x1 + coordin[crt_n].xLength  )
            {
                 gdk_gc_set_function(canvas_gc[crt_n], GDK_XOR);
                 dx	= mycur.x2	- mycur.x1;
                 dy	= mycur.y2	- mycur.y1;
                 
               if (rcdLabel[crt_n].canMove)
                   {
                      /* gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_ON_OFF_DASH,GDK_CAP_ROUND,GDK_JOIN_MITER);  */
                       gdk_gc_set_foreground(canvas_gc[crt_n],&pixels[crt_n][WHITE]);
                       gdk_draw_rectangle (  widget->window,canvas_gc[crt_n] , FALSE,rcdLabel[crt_n].rect.x +dx -2 ,rcdLabel[crt_n].rect.y +dy -2, rcdLabel[crt_n].rect.width +4 , rcdLabel[crt_n].rect.height +4); 
        
               }else  /* move  x scale */
                   {
                       gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_MITER);
                       gdk_gc_set_foreground(canvas_gc[crt_n], &pixels[crt_n][WHITE]);
                       gdk_draw_line  ( widget->window,canvas_gc[crt_n] ,X_scale [crt_n] + dx   ,coordin[crt_n].rc.y1,X_scale [crt_n]+ dx ,coordin[crt_n].rc.y1+coordin[crt_n].yLength   ) ;	

               }
                 
                 
                mycur.x2 = event->x ; mycur.y2 = event->y ;
                dx	=mycur.x2	- mycur.x1;
                dy	=mycur.y2       - mycur.y1 ;

                
                 if (rcdLabel[crt_n].canMove)
                 {
                      /* gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_ON_OFF_DASH,GDK_CAP_ROUND,GDK_JOIN_MITER);*/
                     
                     
                       gdk_gc_set_foreground(canvas_gc[crt_n],&pixels[crt_n][WHITE]);
                       gdk_draw_rectangle (  widget->window,canvas_gc[crt_n] , FALSE,rcdLabel[crt_n].rect.x +dx -2 ,rcdLabel[crt_n].rect.y +dy -2, rcdLabel[crt_n].rect.width +4 , rcdLabel[crt_n].rect.height +4); 
                 }else
                     {
                       gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_MITER);
                       gdk_draw_line  ( widget->window,canvas_gc[crt_n] ,X_scale [crt_n] + dx ,coordin[crt_n].rc.y1,X_scale [crt_n] +dx ,coordin[crt_n].rc.y1+coordin[crt_n].yLength   ) ;	
                
                 }
                 
                i =   (int)((event->x - coordin[crt_n].rc.x1)  /  coordin[crt_n].xstep) ; 
                if  (i > 0 && i < 900)
                    { 
                                   
                         for (j = 0 ; j< RealCurvePt.next_pos; j++)
                             {
                             rcdLabel[crt_n].val[j] = get_data_from_real_curve(i  , j,&rcdLabel[crt_n].Time  ,crt_n ,real_curve_pt->nextpt  );           
                         }
                     
                        update_real_rcd( widget ,crt_n);
                }
 
        }

  return FALSE;
}

gint btn_release_CB( GtkWidget *widget,GdkEvent *event , gpointer *p)
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
                  
                   if  (rcdLabel[crt_n].canMove)
                       {
                        rcdLabel[crt_n].canMove = FALSE ; 
                        rcdLabel[crt_n].rect.x += dx ;
                        rcdLabel[crt_n].rect.y += dy ;
                         
 g_message("\n   Button release !!!!!!! ");                    
                    }else
                        {
                         X_scale [crt_n] += dx ;
                         if (( x  >=  coordin[crt_n].rc.x1) && ( x  <=  coordin[crt_n].rc.x1 + coordin[crt_n].xLength ) &&
		               ( y  >=  coordin[crt_n].rc.y1) && ( y  <= coordin[crt_n].rc.y1 + coordin[crt_n].yLength))
                         {
                             gdk_gc_set_line_attributes(canvas_gc[crt_n],0,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_MITER);
                             gdk_gc_set_foreground(canvas_gc[crt_n], &pixels[crt_n][WHITE]);
                             gdk_draw_line  ( widget->window,canvas_gc[crt_n] ,X_scale [crt_n]   ,coordin[crt_n].rc.y1,
                                                   X_scale [crt_n]   ,coordin[crt_n].rc.y1+coordin[crt_n].yLength   ) ;	
                         }
                    }
                  
                   realCurvRedraw (  crt_n, off_draw_area[crt_n] , canvas_gc[crt_n],rc.width ,rc.height);
                   gdk_window_invalidate_rect(widget->window ,&rc ,1);    
  return TRUE;
        
         
    }
              
              
              
 }
   
}


gint btn_press_CB( GtkWidget *widget,GdkEvent *event , gpointer *p)
{

    int x ,y ,dx, dy, i ,j; 
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
                
                 if ( ( x  >=  rcdLabel[crt_n].rect.x) && ( x  <=  rcdLabel[crt_n].rect.width + rcdLabel[crt_n].rect.x ) &&
		( y  >= rcdLabel[crt_n].rect.y ) && ( y  <= rcdLabel[crt_n].rect.y + rcdLabel[crt_n].rect.height))					 
       
                  {
                        rcdLabel[crt_n].canMove  = TRUE ;
                        
                   
                  }
                   else
                   {
                          rcdLabel[crt_n].canMove  = FALSE ; 
                          X_scale [crt_n] =  (bevent->x) ;
                          if (( x  >=  coordin[crt_n].rc.x1) && ( x  <=  coordin[crt_n].rc.x1 + coordin[crt_n].xLength ) &&
		               ( y  >=  coordin[crt_n].rc.y1) && ( y  <= coordin[crt_n].rc.y1 + coordin[crt_n].yLength))
                              {
                                  gdk_gc_set_function(canvas_gc[crt_n], GDK_XOR); 
                                  gdk_gc_set_foreground(canvas_gc[crt_n], &pixels[crt_n][WHITE]);
                                  gdk_draw_line  ( widget->window,canvas_gc[crt_n] ,X_scale [crt_n]   ,coordin[crt_n].rc.y1,
                                                   X_scale [crt_n] ,coordin[crt_n].rc.y1+coordin[crt_n].yLength   ) ;
                                  
                                   
                                  i =   (int)((x - coordin[crt_n].rc.x1)  /  coordin[crt_n].xstep) ; 
                                  if  (i > 0 && i < 900)
                                        { 
                                                 for (j = 0 ; j<  RealCurvePt.next_pos; j++)
                                                 {
                                                 rcdLabel[crt_n].val[j] = get_data_from_real_curve(i  , j,&rcdLabel[crt_n].Time  ,crt_n ,real_curve_pt->nextpt  );           
                                             }
                                        
                                            update_real_rcd( widget ,crt_n);
                                    }
                             
                          }
                                    
                  }
              
                  return TRUE;
        }
        
    }
        
        
}

 
  
 
 
 
void	SelRealValueListCB (GtkWidget* w, gpointer list)
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
			
		select_row=gtk_object_get_data(list_item,list_item_data_r );	
		
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


void	UpdateAnaValList ( int crt_n  )
{
	GtkWidget       *list_item;
	short		i,j;
	POINTER		ptr;
	DMS_COMMON	pdt;
//	char	        longname[DBNAME_LEN];
	char			LongNameStr1[128];

	int		db_num[MAX_CRT_NUM];
	gchar *pConvert=0;
       
	
	gtk_list_clear_items(list_ana_val,0,-1);
    trenddatahis.next_pos  = 0 ;
        
    
    ptr = real_curve_db_info[crt_n].select_pt ;
        
        
	db_num[crt_n] =  0   ;        
    if(-1==GetEntryNumById( real_curve_db_info[crt_n].select_pt.stn_id, 
	real_curve_db_info[crt_n].select_pt.dev_id, real_curve_db_info[crt_n].select_pt.data_type,&db_num[crt_n]))
		/*return*/  ;
	for (i=0; i< db_num[crt_n]; i++)
	{
		ptr.pt_id	=i ;
		
		
		if ( ReadEntryById ( &ptr, &pdt ) == -1)
		{
			printf("OIX: TrendName No this point in the database file !!!\n");
		}
		else
		{    

			
		//	GetGroupLongnameById( ptr->stn_id, ptr->dev_id, LongNameStr );
			GetEntryLongnameById (&ptr,LongNameStr1 );

			trenddatahis.ptr[trenddatahis.next_pos] = ptr ; 
			 
			trenddatahis.next_pos ++ ; 
			 
			list_item = gtk_list_item_new_with_label(pConvert=_toUtf8(LongNameStr1));
			G_SAFE_FREE(pConvert);
			
			gtk_container_add(GTK_CONTAINER(list_ana_val),list_item );
			gtk_widget_show(list_item);
			gtk_object_set_data(GTK_OBJECT(list_item),list_item_data_r,trenddatahis.next_pos);	
		}
	}
	
	printf("OIX:  total nextpos is %d  and dbnum is%d !!!\n" ,trenddatahis.next_pos ,  db_num);
	
}



void	UpdateAnaLstSel (int crt_n )
{
GtkWidget       *list_item;
DMS_COMMON  common_aa;
        short           i;
        POINTER         *ptr;
        DMS_COMMON      pdt;
   //     char            longname[DBNAME_LEN];  
		
		char	LongNameStr[128] ,LongNameStr1[128];
        GdkColor color ;           
	gtk_list_clear_items(GTK_LIST(sel_ana_val_list),0,-1);
	
        gdk_color_parse("red",&color);
        
         if ((fp = fopen(TREND_FILE, "rb")) == NULL) 
	{
         	printf("OIX:unable to open %s\n", TREND_FILE);
         	return;
        }
                
        if (fread(&RealCurvePt,sizeof(TRENDDATA),1,fp)!=1)
         	printf("in updatatrendlist fread error\n");
        fclose(fp);
#ifdef  SUN_OS
	Swap2Byte(&RealCurvePt.next_pos);
#endif
	if( RealCurvePt.next_pos > MAX_POINTER_NUM)
	{       
               gdk_beep();
		printf("Oix ::in gtk_real_curve.c UpdateAnaLstSel:  RealCurvePt.next_pos=%d > MAX_POINTER_NUM \n", RealCurvePt.next_pos);
		 RealCurvePt.next_pos= 0;   
		return;
	}
#ifdef  SUN_OS
	for( i=0; i<RealCurvePt.next_pos; i++)
		Swap2Byte(&RealCurvePt.ptr[i].pt_id);
#endif
        
        
        for (i=0; i<MAX_CURVE_NUM; i++)
            {
                strcpy(rcdLabel[crt_n].dbLongName[i],"");
        }
		
        for (i=0; i<RealCurvePt.next_pos; i++)
        {
         	ptr	= &RealCurvePt.ptr[i];
#ifdef  DEBUG
         	printf("in UpdateAnaLstSel: dev_id=%d, data_type=%d, pt_id = %d\n",
                                                ptr->dev_id, ptr->data_type, ptr->pt_id);
#endif
		
		if ( ReadEntryById ( ptr, &pdt )  == -1)
                {
                        printf("OIX: TrendList no this point in the database file !!!\n");
                }
		else
		{			
		 
                    
                       switch 	(RealCurvePt.ptr[i].data_type )
		{

			case	ANA_TYPE:
			case	OUTA_TYPE:
			case	PARAM_TYPE:
			      rcdLabel[crt_n].hi_range[i]  = pdt.var.anlg.eng_para.HI_RANGE ;
                              rcdLabel[crt_n].low_range[i]  = pdt.var.anlg.eng_para.LO_RANGE ;
				break;
			default:
				rcdLabel[crt_n].hi_range[trend_del_item]  = 9999.00 ;
				break;
		}
                       
                       

		GetGroupLongnameById( ptr->stn_id, ptr->dev_id, LongNameStr );
		GetEntryLongnameById (ptr,LongNameStr1 );
		strcat( LongNameStr, LongNameStr1 );
                    
		strcpy (rcdLabel[crt_n].dbLongName[i],LongNameStr);	

        printf("\n rcdLabel[crt_n].dbLongName[i] = %s", rcdLabel[crt_n].dbLongName[i] ); 
		
		list_item = gtk_list_item_new_with_label(pConvert=_toUtf8(LongNameStr1));
		G_SAFE_FREE(pConvert);
		gtk_widget_modify_bg(list_item, GTK_STATE_NORMAL,&color);
                    gtk_container_add(GTK_CONTAINER(sel_ana_val_list),list_item );			
		gtk_widget_show(list_item);
		gtk_object_set_data(GTK_OBJECT(list_item),list_item_data_r,i+1);
		}
        }

}

        
void	LstValAddCB (GtkWidget *w  )
{
	   DMS_COMMON  common_aa;
       int crt_n   ;
       GetCrtNumberOfWgt(w, &crt_n);
       if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
    
        int             i;
        
 	if ( RealCurvePt.next_pos  >=  MAX_CURVE_NUM)
	{
		
		gdk_beep();
		PopupMessageBox(real_curve_main_w[crt_n],"\nOIX: RealCurvePt.next_pos >= MAX_CURVE_NUM\n");
		return;
	}

        for(i=0; i<RealCurvePt.next_pos;i++)
        {
         	if((RealCurvePt.ptr[i].stn_id  == trenddatahis.ptr[trend_add_item-1].stn_id) &&
                (RealCurvePt.ptr[i].dev_id  == trenddatahis.ptr[trend_add_item-1].dev_id) &&
                (RealCurvePt.ptr[i].data_type  == trenddatahis.ptr[trend_add_item-1].data_type) && 
                (RealCurvePt.ptr[i].pt_id  == trenddatahis.ptr[trend_add_item-1].pt_id))
		{
                	gdk_beep();
         		return ;
		}
        }    
       
    	if ( trend_del_item == 0 )
                trend_del_item = RealCurvePt.next_pos;
        else
        {
                for (i=RealCurvePt.next_pos; i>trend_del_item; i--)
                        RealCurvePt.ptr[i] = RealCurvePt.ptr[i-1];
        }
        RealCurvePt.ptr[trend_del_item] = trenddatahis.ptr[trend_add_item-1] ;   
        RealCurvePt.next_pos++;
        
       
/**********************************************/
                if ((fp = fopen(TREND_FILE, "rb+")) == NULL)
                {
                        printf("TrendAdd unable to open file\n");
                        return;
                }

        #ifdef  SUN_OS   
                tmpRealCurvePt = RealCurvePt;
                for( i=0; i< tmpRealCurvePt.next_pos; i++)
                        Swap2Byte(&tmpRealCurvePt.ptr[i].pt_id);
                Swap2Byte(&tmpRealCurvePt.next_pos);
                if (fwrite (&tmpRealCurvePt, sizeof(TRENDDATA), 1, fp) != 1)
                        printf("OIX:in TrendAddCB write error\n");
        #else
                if (fwrite (&RealCurvePt, sizeof(TRENDDATA), 1, fp) != 1)
                        printf("OIX:in TrendAddCB write error\n");
        #endif

                fclose (fp);         
                
                
  /************************************************/            
                
END:
		UpdateAnaLstSel ( crt_n );
                  
  		trend_del_item  = RealCurvePt.next_pos; 
                real_curve_pt->flg 	= 1;               /**  ----set flag report WDQS_SHM_DATA_____**/
                /* realCurvRedrawE (  crt_n) ;   */


}


void	LstValRemoveCB ()
{
	int crt_n=0;
        int             i;

	if (trend_del_item ==0)
	{
		gdk_beep();
		return;
	}
 
	if ((fp = fopen(TREND_FILE, "rb+")) == NULL) 
	{
         	printf("unable to open %s\n", TREND_FILE);
         	return;
        }       
        
        gtk_list_clear_items(sel_ana_val_list,trend_del_item-1,trend_del_item);
        
        RealCurvePt.next_pos = RealCurvePt.next_pos-1;
        for (i=trend_del_item; i<RealCurvePt.next_pos+1; i++)
	        { RealCurvePt.ptr[i-1]	= RealCurvePt.ptr[i]; }
                
 
       #ifdef  SUN_OS   
                tmpRealCurvePt = RealCurvePt;
                for( i=0; i< tmpRealCurvePt.next_pos; i++)
                        Swap2Byte(&tmpRealCurvePt.ptr[i].pt_id);
                Swap2Byte(&tmpRealCurvePt.next_pos);
                if (fwrite (&tmpRealCurvePt, sizeof(TRENDDATA), 1, fp) != 1)
                        printf("OIX:in TrendAddCB write error\n");
        #else
                if (fwrite (&RealCurvePt, sizeof(TRENDDATA), 1, fp) != 1)
                        printf("OIX:in TrendAddCB write error\n");
        #endif

                fclose (fp);   
        
        
        
	UpdateAnaLstSel(crt_n);	 
	realCurvRedrawE (  crt_n) ; 
	
	trend_del_item	= 0;
        real_curve_pt->flg 	= 1;               /**  ----set flag report WDQS_SHM_DATA_____**/


}

void DrawTimeLabel(int crt_n )
{ 
     
     GtkWidget  *Sentry , *Eentry; 
     char ss[64];
     struct tm when1 ;  
     time_t   now ,result; 
     time(&now); 
     Sentry = g_object_get_data(real_curve_main_w[crt_n],"startWgt");
     Eentry = g_object_get_data(real_curve_main_w[crt_n],"endWgt");
     when1 = *localtime(&now);
     
     
     sprintf(ss  , "%4d-%2d-%2d %2d:%2d:%2d",when1.tm_year+1900,when1.tm_mon+1,when1.tm_mday,
                 when1.tm_hour,when1.tm_min,when1.tm_sec ) ;  
      if (Sentry)  gtk_label_set_text(Eentry , ss);
     when1.tm_min  -=  15 ;
     when1.tm_isdst = -1 ;
     if (result = mktime(&when1) == (time_t)-1) return ; 
     memcpy (&starwhen, &when1 ,sizeof (struct tm)); 
     
     
     
     sprintf(ss  , "%4d-%2d-%2d %2d:%2d:%2d",starwhen.tm_year+1900,starwhen.tm_mon+1,starwhen.tm_mday,
     starwhen.tm_hour,starwhen.tm_min,starwhen.tm_sec ) ;  
     if (Sentry)  gtk_label_set_text(Sentry , ss);
     
    
}



 realCurvRedrawE (int crt_n)
{
    
     GdkRectangle rc ;
     rc.x = rc.y = 0 ;
     rc.width = real_draw_area[crt_n]->allocation.width ;
     rc.height = real_draw_area[crt_n]->allocation.height ;
     realCurvRedraw (  crt_n,off_draw_area[crt_n] , canvas_gc[crt_n] , rc.width , rc.height);
     gdk_window_invalidate_rect(real_draw_area[crt_n]->window , &rc    ,TRUE);
   
}
void	realCurvRedraw (int crt_n,GdkDrawable *drawable , GdkGC *gc ,int width ,int height)
{
    	int 	i, i1, i2,k ,curid ,now_num;
    	POINTER         *ptr;
    	DMS_COMMON      pdt;
    	char            longname[DBNAME_LEN*2];
	ANLG_DATA	anlg_data;
    	
    	float   hi_range, lo_range;
    	float	xstep,value;
    	GdkPoint pt1, pt2  ,curve_points[WDQS_NUM] ;
    	int  xi ,yi;
    	int canvas_w, canvas_h ;
    	BACKRCD  rcdtmp ;  
        canvas_w = width  ;
        canvas_h =height ; 
        
        
        
  g_message("\n real curve redraw !!!!")   ;   
    	
    /*	printf (" canvas_w  is %d ,canvas_h  is %d\n" , canvas_w, canvas_h);  */
    	
    	if (drawable == NULL )
    	{
    	     printf("Oix:: realCurvRedraw Error  :draw_area is null!!! \n"); 
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
		
    HisCurvXYLine(crt_n ,drawable ,gc,coordin[crt_n].alloc.width,coordin[crt_n].alloc.height );	
        
     
    xstep = coordin[crt_n].xstep ;
    
	for(   curid = 0;   curid <  RealCurvePt.next_pos  ;curid++)       
	{   
		
		if (rcdLabel[crt_n].hi_range[curid]==0) rcdLabel[crt_n].hi_range[curid]  = 9999.00; 
		gdk_gc_set_foreground (gc, &pixels[ crt_n][CLR_GROUP[curid]]);  
        	yi= (int)( data_curve.data_curve[crt_n].seg[curid].data[0]/rcdLabel[crt_n].hi_range[curid] * y_h) ;
        
       		for (i = 0 ,  now_num = real_curve_pt->nextpt  ; i< WDQS_NUM ; i++ )
   		{     
                        if( RealCurvePt.ptr[curid].data_type    ==  ANA_TYPE)
				value	= real_curve_pt->wd_dat.fvalue[curid][now_num];
			else
				value	= real_curve_pt->wd_dat.status[curid][now_num];
                        
                        if (value >  rcdLabel[crt_n].hi_range[curid])
                            value = rcdLabel[crt_n].hi_range[curid];
                        if (value <   rcdLabel[crt_n].low_range[curid])
                             value = rcdLabel[crt_n].low_range[curid];
       			xi = (int) ((float)i * xstep) ;
           		yi = (int)( value/rcdLabel[crt_n].hi_range[curid] * (float)y_h) ;
			curve_points[i].x = xi + X_ORGIN ; 
                        curve_points[i].y =  Y_to_y( &yi) ; 
	        	now_num ++;
                        if (now_num > WDQS_NUM) now_num =0 ;
       	     		 	
      		}
                gdk_draw_lines (  drawable, gc,  curve_points , WDQS_NUM);
               	
	 }         
        /*  end for  each curid*/
        
           DrawrcdLabel( crt_n ,drawable, gc , &rcdLabel[crt_n] );
           DrawTimeLabel (crt_n ) ;   
}



void	DisplayRealCurve (int crt_n  )
{
        int             i;
         crt_n = 0 ;
  /*	      
        
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;
        
   
  	
	if ( RealCurvePt.next_pos == 0)
        {
                gdk_beep();
                PopupMessageBox(real_curve_main_w[crt_n],"OIX: RealCurvePt.next_pos == 0!");
                return;
        }
        
       if (real_curve_timer_id  !=0 )
		gtk_timeout_remove(real_curve_timer_id ); 
       real_curve_timer_id     =gtk_timeout_add (  1000, (GtkFunction)realCurvRedrawE,crt_n); 
 
       realCurvRedrawE(crt_n);
        printf("@@@@  in DisplayRealCurve\n");
  
  */
}
 

 

void AnaValueAddReal(GtkWidget *w, gpointer data )
{
	int crt_n,i;
	
	printf("####in AnaValueAddReal,sel_pt_num=%d,select_point=%d\n",sel_pt_num,select_point);
	
	/*GetCrtNumberOfWgt ( w, &crt_n );*/	
	
	if(sel_pt_num > CURVE_NUM)
	   {  
               gdk_beep(); 	
               PopupMessageBox(real_curve_main_w[crt_n],"???è¯?ï¼??????©ç????²çº¿ä¸???°å·²ç»?è¶?è¿?6ä¸?!");
        }
	else
	{
		sel_pt_num ++;			
		select_points[sel_pt_num-1]=select_point;
						
	}		   	
}
 



void FocusInOutInput(GtkWidget *text_w, GdkEventFocus *e ,gpointer  data  )
{
    
     
}

	
void SetOtherTm(GtkWidget *text_w, gpointer  data)
{
    
}

 

 
/**************************************************************************************************              
		    s--return time string ; add_num--want add num , type--kind         

 ************************************************************************************************* */
int MakeFormatTmR(struct tm  *Tm , char *s ,int add_num , enum T_Interval type )
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
        sprintf(s,"%4d-%2d-%2d %2d:%2d:%2d",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,Tm->tm_min,Tm->tm_sec ) ; 
        return 0  ;  /* success!!! */
     }
     else
     {   
     	sprintf(s, "%s","YYYY-MM-DD HH:MM:SS\n");
     	return -1  ;  /* error!!! */
     }
         
 
}


void real_draw_area_allocate(GtkWidget *widget,GtkAllocation *a ,gpointer data)
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
    
   realCurvRedraw (  crt_n,off_draw_area[crt_n]  , canvas_gc[crt_n] ,   a->width    ,a->height); 
   g_message("\n Allocation ! x:%d ,y:%d , width:%d height : %d \n ",a->x ,a->y ,a->width, a->height);
}

void real_draw_area_realize(GtkWidget *widget,gpointer data)
{
   g_message ( " real_draw_area_realize!!!  \n");
    int crt_n = (gint) data ;
    if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
    
   
    g_message ( " real_draw_area_realize!!!  \n");
    canvas_gc[crt_n] = gdk_gc_new(widget->window);
   /* draw_area = widget ;   */
    off_draw_area[crt_n] = gdk_pixmap_new(widget->window , 1200, 1024,-1 ); 
    gdk_window_set_background(widget->window, &pixels[crt_n][BLACK]);
    rcdLabel[crt_n].rect.x	= 100;
    rcdLabel[crt_n].rect.y	= 100;
    rcdLabel[crt_n].rect.width	= 100;
    rcdLabel[crt_n].rect.height  = 200;
    
 
}



void DestoryRealCurveShell(GtkWidget *widget,   gpointer data)
{     
      
      
    
      int crt_n = (gint) data ;
      if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
      int i ;  
      g_message("\n quit realcurve !!!!! \n");
      
      gtk_timeout_remove(real_curve_timer_id ); 
      real_curve_timer_id  = 0 ;
  
      gtk_widget_destroy(GTK_WIDGET(real_curve_main_w[crt_n])); 
      real_curve_main_w[crt_n] = (GtkWidget *)NULL ;
      return FALSE;
}

gint	RealDatatypePressProc (GtkWidget *w, gpointer data )
{
	int i;
	char cur_datatype_name[20];
	int	crt_n=0;
	
	GetCrtNumberOfWgt ( w, &crt_n );
  	if( crt_n < 0 )
  		return; 
  		
  	strcpy(cur_datatype_name,gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(data)->entry)));

	strcpy(cur_datatype_name,g_locale_from_utf8(cur_datatype_name,strlen(cur_datatype_name),NULL,NULL,NULL));
	if(strcmp(cur_datatype_name,"")==0)
		return;
  	for(i=0;i<sizeof(recordlongname)/sizeof(recordlongname[0]);i++)
  	{
  		if(strcmp(recordlongname[i],cur_datatype_name)==0)  
		{	
  			break;
		}
  	}

	real_dtype    		= i;
/*
 	UpdateTrendNameList ( crt_n, trend_stn,real_dev );	 
 */   	
	return TRUE;
}

void ProgRealCurve (GtkWidget *parent ,int  crt_n )
{
    if (  crt_n  >0)  
    {    
       
         gdk_beep(); 
         PopupMessageBox(parent , "OIX: Only open in 0 screen !");
         return ; 
    } 
    
    
      if (real_curve_main_w[crt_n]==(GtkWidget*)NULL)
    {
         real_curve_main_w[crt_n]  = CreateProgRealCurveShell ( parent, crt_n   ) ;
         gtk_window_set_position(real_curve_main_w[crt_n] ,GTK_WIN_POS_CENTER_ON_PARENT);

         gtk_widget_show(real_curve_main_w[crt_n] );
         
  	
         real_curve_pt	= LinktoShareMem(SHM_WDQS_KEY_CHAR );
         if( real_curve_pt == (void *)-1)
         {
                perror("OIX ::in ProgTrendSet ERROR: real_curve_pt= -1!!!"); 
                return;
         }	

         
         UpdateAnaValList ( crt_n ); 
         UpdateAnaLstSel ( crt_n ) ;
         
         if (real_curve_timer_id  !=0 )
		gtk_timeout_remove(real_curve_timer_id ); 
          real_curve_timer_id     =gtk_timeout_add (  1000, (GtkFunction)realCurvRedrawE,crt_n); 
 
       realCurvRedrawE(crt_n);
         trenddatahis.next_pos = 0; 
    }else
    {
        gtk_window_present(real_curve_main_w[crt_n] );
    }
}



static GtkWidget *CreateProgRealCurveShell (GtkWidget *parent , int  crt_n )
{
	GtkWidget  *vbox,*vbox1, *hbox,*hbox1,*button_add_point,*button_del_point;
	GtkWidget  *frame;
	GtkWidget  *toolbar,*toolbarDataSel,*ana_name_list_scrollw,*sel_ana_list_scrollw;
	GtkWidget  *table,*hrule, *vrule;
	GtkWidget  *windows ;
    SHORT	i   ;
	int	shmid2;

	gchar *pConvert=0;
       /*  gtk_set_locale();
       gtk_rc_add_default_file("./gtkrc.zh");  RM BY CHI*/

        windows=gtk_window_new(GTK_WINDOW_TOPLEVEL);
 
        gtk_window_set_default_size(GTK_WIDGET( windows),1224, 1000);

        gtk_window_set_title(GTK_WINDOW(windows), pConvert=_toUtf8("Real curve Window"));
		G_SAFE_FREE(pConvert);
        gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
  
        gtk_signal_connect(GTK_OBJECT(windows),
                           "destroy",
                           GTK_SIGNAL_FUNC( DestoryRealCurveShell),
                           crt_n);
 
        vbox=gtk_vbox_new(FALSE, 5);
        gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
        gtk_container_add(GTK_CONTAINER(windows), vbox);
        gtk_widget_show(vbox);
  
        toolbar =  CreateRealCurvToolbar(windows);
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
                           GTK_SIGNAL_FUNC(SelRealValueListCB), ANA_LIST); 
 
 
/********************* two button code here !! *********************************/
 		      
 	hbox1=gtk_hbox_new(FALSE, 15);
	gtk_widget_set_usize (GTK_WIDGET(hbox1),-1 , 30);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox1 ,FALSE, FALSE, 0);
    	gtk_widget_show(hbox1);
    	
        GtkWidget *image1 ;
	button_add_point = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ADD)); 
    G_SAFE_FREE(pConvert);
        
        
    	gtk_box_pack_start (GTK_BOX (hbox1), button_add_point, TRUE, TRUE, 0);
    	button_del_point = gtk_button_new_with_label(pConvert=_toUtf8EX( OIX_CSTR_DEL ));
		G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox1), button_del_point, TRUE, TRUE, 0);     
    	
    	gtk_signal_connect(GTK_OBJECT(button_add_point), "clicked",
			      GTK_SIGNAL_FUNC(LstValAddCB), button_add_point);
			      
   	gtk_signal_connect(GTK_OBJECT(button_del_point), "clicked",
			      GTK_SIGNAL_FUNC(LstValRemoveCB), button_del_point);			      
    	
   	gtk_widget_show(button_add_point);
    	gtk_widget_show(button_del_point);   	       
 
     /***************** list2 window code ***************************************/ 
   
        ana_name_list_scrollw=gtk_scrolled_window_new (NULL, NULL);      
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ana_name_list_scrollw),
                                        GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);


        gtk_box_pack_start(GTK_BOX(vbox1), ana_name_list_scrollw, TRUE,TRUE, 0);
        
     /**************** gtk_widget_show (ana_name_list_scrollw);  ****************/

        sel_ana_val_list = gtk_list_new ();    
        gtk_scrolled_window_add_with_viewport(GTK_CONTAINER(ana_name_list_scrollw),sel_ana_val_list );    
        gtk_widget_show (sel_ana_val_list);     

	gtk_signal_connect(GTK_OBJECT(sel_ana_val_list), "selection_changed",
		       GTK_SIGNAL_FUNC(SelRealValueListCB),SEL_ANA_LIST); 		        
	
 		      	    

    /***************** draw area code ***************************************/     
   
       frame = gtk_frame_new("curve frame");    
        gtk_box_pack_start(GTK_BOX(hbox ), frame, TRUE,TRUE, 0);
         gtk_widget_show(frame);


        real_draw_area[crt_n]= gtk_drawing_area_new();


       gtk_container_add( (frame),real_draw_area[crt_n] );

 
        gtk_widget_set_events( real_draw_area[crt_n],  GDK_POINTER_MOTION_MASK |
                                                GDK_BUTTON_PRESS_MASK  |
                                                GDK_BUTTON_RELEASE_MASK  |
                                     GDK_EXPOSURE_MASK );
  
        gtk_signal_connect (GTK_OBJECT (real_draw_area[crt_n]), "realize", 
                          GTK_SIGNAL_FUNC (real_draw_area_realize), crt_n);
        gtk_signal_connect (GTK_OBJECT (real_draw_area[crt_n]), "size_allocate", 
                          GTK_SIGNAL_FUNC (real_draw_area_allocate), crt_n);
 
        gtk_signal_connect (GTK_OBJECT (real_draw_area[crt_n]),"expose_event",
                            GTK_SIGNAL_FUNC(realCurve_expose_events), crt_n) ; 
 
 
        gtk_signal_connect(GTK_OBJECT (real_draw_area[crt_n]), "button_press_event",
                       GTK_SIGNAL_FUNC (btn_press_CB),  crt_n);
        gtk_signal_connect(GTK_OBJECT (real_draw_area[crt_n]), "button_release_event",
                       GTK_SIGNAL_FUNC (btn_release_CB),  crt_n);
        gtk_signal_connect (GTK_OBJECT (real_draw_area[crt_n]),"motion_notify_event",
   			GTK_SIGNAL_FUNC(motion_real_CB),crt_n) ;

         gtk_widget_realize(GTK_WIDGET(real_draw_area[crt_n])) ;
        gtk_widget_show (real_draw_area[crt_n]);
        gtk_widget_show_all(windows);	  
      


       return windows;
}
 

GtkWidget *cmbHisCurvDev;
GList *realCurv_stn_lst=NULL,*realCurv_dev_lst=NULL,*realCurv_datatype_lst=NULL;


void UpdateRealStnSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;

	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;

	real_curve_db_info[crt_n].select_pt.stn_id= (UCHAR)(int)data;
	UpdateRealDevMenu(crt_n );
	UpdateAnaValList ( crt_n );  
}


void UpdateRealDevSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;

	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;

	real_curve_db_info[crt_n].select_pt.dev_id= (UCHAR)(int)data;
	UpdateRealDataTypeMenu(crt_n);
	UpdateAnaValList (   crt_n );
}



void UpdateRealDataTypeSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;

	GetCrtNumberOfWgt(widget, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;

	real_curve_db_info[crt_n].select_pt.data_type= (UCHAR)(int)data;
	UpdateAnaValList ( crt_n );  
}




void UpdateRealDevMenu(int crt_n) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	UCHAR	stn_id, dev_id;
	int		i,grp_num;
 	GtkWidget *menu,*items;
	gchar *pConvert=0;
  
	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].dev_optionmenu));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].dev_optionmenu));

	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

	stn_id= real_curve_db_info[crt_n].select_pt.stn_id;
	if(-1==GetGroupNumById(stn_id, &grp_num))
		return;

	dev_id= 0;
 	for(i=1; i<=grp_num; i++) 
  	{  		
		if(-1==GetGroupCommentById(stn_id, (UCHAR)i, dev_comment))
			continue;
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(dev_comment));
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateRealDevSelect), (gpointer)i);
		if(!dev_id)
			dev_id= i;
	}

        gtk_option_menu_set_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].dev_optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(real_curve_db_info[crt_n].dev_optionmenu), 0);
	real_curve_db_info[crt_n].select_pt.dev_id= dev_id;
	gtk_widget_show_all(real_curve_db_info[crt_n].dev_optionmenu);
}



void UpdateRealDataTypeMenu(int crt_n) 
{
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	UCHAR	stn_id, dev_id, data_type;
	int		i,j,rec_num,entry_num;
 	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;

	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].datatype_optionmenu));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].datatype_optionmenu));

	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

	stn_id= real_curve_db_info[crt_n].select_pt.stn_id;
	dev_id= real_curve_db_info[crt_n].select_pt.dev_id;
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
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateRealDataTypeSelect), (gpointer)i);
		j++;
		if(!data_type)
		{
			data_type= i;
			active_index= 0;
		}
		if(i==real_curve_db_info[crt_n].select_pt.data_type)
		{
			data_type= i;
			active_index= menu_index;
		}
		menu_index++;
	}

        gtk_option_menu_set_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].datatype_optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(real_curve_db_info[crt_n].datatype_optionmenu), active_index);
	real_curve_db_info[crt_n].select_pt.data_type= data_type;
	gtk_widget_show_all(real_curve_db_info[crt_n].datatype_optionmenu);
}
/*

GtkWidget* CreateInterval_Menu(int crt_n) 
{
         char *interval[] = {OIX_CSTR_1_SEC , OIX_CSTR_1_MIN , OIX_CSTR_5_MIN , OIX_CSTR_15_MIN , OIX_CSTR_1_HOUR };   
	int		i;
 	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;

	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 150, -1);
	menu_index= active_index= 0;
 	for(i=0; i< MAX_INTERVAL_NUM  ; i++) 
  	{  		
 		 
		items = gtk_menu_item_new_with_label(_toUtf8(interval[i]));	 
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(ChangeInterReal), (gpointer)i);
  	}
	optionmenu = gtk_option_menu_new();
        gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	return optionmenu;
}


*/




GtkWidget* CreateRealStnMenu(int crt_n) 
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
		if(-1==GetStnIdByIndex(i, &stn_id))
			continue;
		
		if( GetStnCommentById((UCHAR)stn_id,(char*)( stn_comment))==-1 )
			continue;
		
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(stn_comment));
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateRealStnSelect), (gpointer)i);
		if(!stn_id)
		{
			stn_id= i;
			active_index= 0;
		}
		if(i==real_curve_db_info[crt_n].select_pt.stn_id)
		{
			stn_id= i;
			active_index= menu_index;
		}
		menu_index++;
  	}

	optionmenu = gtk_option_menu_new();
        gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	real_curve_db_info[crt_n].select_pt.stn_id= stn_id;
	real_curve_db_info[crt_n].stn_optionmenu= optionmenu;
	return optionmenu;
}




GtkWidget* CreateRealDevMenu(int crt_n) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	UCHAR	stn_id, dev_id;
	int		i,grp_num;
 	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
  
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

	stn_id= real_curve_db_info[crt_n].select_pt.stn_id;
	if(-1==GetGroupNumById(stn_id, &grp_num))
		return;

	dev_id= 0;
	menu_index= active_index= 0;
 	for(i=1; i<=grp_num; i++) 
  	{  		
		if(-1==GetGroupCommentById(stn_id, (UCHAR)i, dev_comment))
			continue;
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(dev_comment));
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateRealDevSelect), (gpointer)i);
		if(!dev_id)
		{
			dev_id= i;
			active_index= 0;
		}
		if(i==real_curve_db_info[crt_n].select_pt.dev_id)
		{
			dev_id= i;
			active_index= menu_index;
		}
		menu_index++;
	}

	optionmenu = gtk_option_menu_new();
        gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	(GTK_OPTION_MENU(optionmenu), active_index);
	real_curve_db_info[crt_n].select_pt.dev_id= dev_id;
	real_curve_db_info[crt_n].dev_optionmenu= optionmenu;
	return optionmenu;
}



GtkWidget* CreateRealDataTypeMenu(int crt_n) 
{
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	UCHAR	stn_id, dev_id, data_type;
	int		i,j,rec_num,entry_num;
 	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
  
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

	stn_id= real_curve_db_info[crt_n].select_pt.stn_id;
	dev_id= real_curve_db_info[crt_n].select_pt.dev_id;
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
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateRealDataTypeSelect), (gpointer)i);
		j++;
		if(!data_type)
		{
			data_type= i;
			active_index= 0;
		}
		if(i==real_curve_db_info[crt_n].select_pt.data_type)
		{
			data_type= i;
			active_index= menu_index;
		}
		menu_index++;
	}

	optionmenu = gtk_option_menu_new();
        gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	real_curve_db_info[crt_n].select_pt.data_type= data_type;
	real_curve_db_info[crt_n].datatype_optionmenu= optionmenu;
        gtk_widget_show(optionmenu);
	return optionmenu;
}




GtkWidget * CreateDataSelectToolbar (GtkWidget * parent, int crt_n)
{
  	GtkWidget *box;  	
	GtkWidget *toolbar; 
  	GtkWidget *menuStn ,*menuDev ,*menuDataType ; 
 	
  	toolbar = gtk_handle_box_new();

 	box = gtk_hbox_new (FALSE, 5);
 	gtk_container_add(GTK_CONTAINER(toolbar), box);
  	gtk_container_border_width(GTK_CONTAINER(box), 5);
        menuStn =  CreateRealStnMenu(crt_n) ;
        menuDev =  CreateRealDevMenu(crt_n) ;
        menuDataType = CreateRealDataTypeMenu(crt_n) ; 
        gtk_widget_show (menuStn);
        gtk_widget_show (menuDev);
        gtk_widget_show (menuDataType);
	gtk_box_pack_start(GTK_BOX(box), menuStn , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box),menuDev  , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), menuDataType , FALSE, FALSE, 0);


 /*   
	button_exit = gtk_button_new_with_label(_toUtf8EX(OIX_STR_EXIT)); 
	gtk_box_pack_start(GTK_BOX(box), button_exit, FALSE, FALSE, 0);   
	gtk_signal_connect(GTK_OBJECT(button_exit), "clicked", GTK_SIGNAL_FUNC(db_tab_quit), (gpointer)crt_n);   */
		      	      	    
  	return (toolbar);
}


GtkWidget *
CreateRealCurvToolbar (GtkWidget * parent)
{
  	int	crt_n,i ;
  	GtkWidget *box, *tempwid,*menuInterval;  	
	GtkWidget *vbox, *hbox;	
	GtkWidget *toolbar; 
  	GtkWidget *cmbInterval;    	
  	GtkWidget	*button_display,*button_exit;
         GtkWidget *menuStn ,*menuDev ,*menuDataType ; 
	 
        char timeVal[22];
        struct tm when ;
        time_t now1; 

		gchar *pConvert=0;
   
   	GetCrtNumberOfWgt (parent, &crt_n );
        if( crt_n < 0 )
                return;
         

  	toolbar = gtk_handle_box_new ();

 	box = gtk_hbox_new (FALSE, 4);
 	gtk_container_add (GTK_CONTAINER (toolbar), box);
  	gtk_container_border_width (GTK_CONTAINER (box), 5);
  	gtk_widget_show (box);
  
 
         
         
 
  	
 	     
  	
  /******************  display  button   *********************/
/*
    	button_display = gtk_button_new_with_label(_toUtf8EX(OIX_CSTR_DISPLAY));
    	gtk_box_pack_start (GTK_BOX (box), button_display, FALSE, FALSE, 0);    	
  
    
    	gtk_signal_connect(GTK_OBJECT(button_display), "clicked",
			      GTK_SIGNAL_FUNC(DisplayRealCurve),crt_n);    	
			      
    	gtk_widget_show(button_display);
   */	 
	button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX( OIX_CSTR_EXIT)) ;
	G_SAFE_FREE(pConvert);
    	gtk_box_pack_end (GTK_BOX (box), button_exit, FALSE, FALSE, 0); 
    	gtk_widget_show (button_exit);
    	  	     
      	gtk_signal_connect(GTK_OBJECT(button_exit),		       "clicked",
		       GTK_SIGNAL_FUNC(DestoryRealCurveShell),		        crt_n  );
     
        
      
 	
/***********************************************************************/

 	 
 	 
  	 
        menuStn =  CreateRealStnMenu(crt_n) ;
        menuDev =  CreateRealDevMenu(crt_n) ;
        menuDataType = CreateRealDataTypeMenu(crt_n) ; 
        /**/gtk_widget_show (menuStn);
        gtk_widget_show (menuDev);
        gtk_widget_show (menuDataType);
	gtk_box_pack_start(GTK_BOX(box), menuStn , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box),menuDev  , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), menuDataType , FALSE, FALSE, 0);
        
        
		         	    
  	return (toolbar);
}




#endif
