/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2005.9.22
  SYN by hcl 2007.12.10

---------------------------------------------------------------------*/
#ifndef _GTK_DRAW_DEF_H_
#define _GTK_DRAW_DEF_H_


/*XGCValues	gcv;*/

GdkPixmap		*pixmap[MAX_CRT_NUMBER],*pixmap2[MAX_CRT_NUMBER];	
GdkPixmap		*sub_pixmap[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER],*bkgrd_sub_pixmap[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
GdkPixmap		*bakgrd_pixmap[MAX_CRT_NUMBER];	
GdkPixmap		*view_pixmap[MAX_CRT_NUMBER];

/*dimensions of drawing area(pixmap)*/

int	canvas_width, canvas_height; 
typedef struct 
{
    float hi_limit1;
    float hi_limit2;
    float low_limit1;
    float low_limit2 ;
} BAR_GRAPH_LMT ;


#define	DrwT_LINE		1
#define	DrwT_LINE_DASH		11        /* added by chi to display dash line   */
#define	DrwT_CIRCLE		2
#define	DrwT_RECTANGLE		3
#define	DrwT_SELECTION		4
#define	DrwT_TEXT		5
#define	DrwT_ARC		6
#define	DrwT_PLINE		7
#define	DrwT_SYMBOL		8
#define	DrwT_GIF		9
#define DrwT_MODIFY_TEXT	15
#define DrwT_RESHAPE		93
#define DrwT_RESIZE		95
#define DrwT_MOVE		96
#define DrwT_COPY		97
#define DrwT_SELECTED		98

#define ANGLE_0         0
#define ANGLE_90        (90*64)
#define ANGLE_180       (180*64)
#define ANGLE_270       (270*64)
#define ANGLE_360       (360*64)


#define k_fg	0
#define k_bg	1


#define	ITEM_TAG_OFF	0
#define	ITEM_TAG_ON	1
#define ITEM_TAG_VERTI   10   /*   used to print out verticallly chi */

#define	k_no_expose		0
#define	k_expose  		1

#define	UNFILLED	0
#define FILLED		4	


#define _LineTypeBit  0x80      /*   added chi  05.3.17   */
#define _LineWidthBit  0x0f  

#define _islinedash(_c)	((unsigned char)(_c) & _LineTypeBit) 

#define _setlinedash(_c) ((unsigned char)(_c) |= _LineTypeBit)
#define _rsetlinedash(_c) ((unsigned char)(_c) &= _LineWidthBit)
#define _linewidth(_c) ((unsigned char)(_c) &  _LineWidthBit)

#endif


