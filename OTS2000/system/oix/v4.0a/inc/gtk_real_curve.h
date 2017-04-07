/*-----------------------------------------------------------------------
	
	 
	Created by chi.hailong  2007.8.8 	  
	ONLY use in UNIX
 
 
---------------------------------------------------------------------*/



#ifndef   _GTK_REAL_CURVE_H 
#define   _GTK_REAL_CURVE_H 


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



#endif 