

int	canvas_width, canvas_height;
int   cxClient, cyClient ; //added by chi for scroll bar! ;


#define	DrwT_LINE		1      
#define DrwT_LINE_DASH  11   // added by chi to draw line of dash '--------'       
#define	DrwT_CIRCLE		2
#define	DrwT_RECTANGLE		3
#define	DrwT_SELECTION		4
#define	DrwT_TEXT			5
#define	DrwT_ARC			6
#define	DrwT_PLINE			7
#define	DrwT_SYMBOL			8
#define	DrwT_GIF			9
#define DrwT_MODIFY_TEXT	15
#define DrwT_CUT			91
#define DrwT_PASTE			92
#define DrwT_RESHAPE		93
#define DrwT_RESIZE		95
#define DrwT_MOVE		96
#define DrwT_COPY		97
#define DrwT_SELECTED		98
#define	DrwT_ZOOM		100

#define ANGLE_0         0
#define ANGLE_90        (90*64)
#define ANGLE_180       (180*64)
#define ANGLE_270       (270*64)
#define ANGLE_360       (360*64)

#define k_arc_1         1
#define k_arc_2         2
#define k_arc_3         3
#define k_arc_4         4
#define k_arc_5         5
#define k_arc_6         6
#define k_arc_7         7
#define k_arc_8         8
#define HORZ_AXIS       1
#define VERT_AXIS       2




#define	k_drag_items	1
#define	k_paste_items	2

#define	k_mv_items	1
#define	k_cp_items	2

#define	GDBASE_MODIFIED		1
#define	GDBASE_UNMODIFIED	0

#define k_fg	0
#define k_bg	1

#define DRAWING_OFF     1
#define DRAWING_ON      2
#define	DRAWING_TEXT	3

#define	ITEM_TAG_OFF	0
#define	ITEM_TAG_ON	    1
#define	ITEM_TAG_ON_P	2  // primary RCD of selected cluster  added by chi 05.02.01
#define ITEM_TAG_VERTI  10  //vertical print out ;             added by chi 05.03.01

#define	CLUTTER_OFF	0
#define	CLUTTER_ON	1

#define	LAYER_INVISIBLE	0
#define	LAYER_VISIBLE	1

#define	HDL_OFFSET	4	
#define	UL_HANDLE	61
#define	UR_HANDLE	62
#define	BL_HANDLE	63
#define	BR_HANDLE	64
#define	L_LINE_HANDLE	65
#define	T_LINE_HANDLE	66
#define	R_LINE_HANDLE	67
#define	B_LINE_HANDLE	68
#define	WRONG_HANDLE	69


#define	click_pl_offset		10

int	drawing_mode;
int	arc_mode;
int	resize_mode;

#define	k_no_expose		0
#define	k_expose  		1

#define	UNFILLED	0
#define FILLED		4

#define FIRST_DRAW	0    //use to display gif pic ;
#define RE_DRAW		1
 

#define _LineTypeBit  0x80   // added chi 05.03.17
#define _LineWidthBit  0x0f   // added chi 05.03.17
  
#define _islinedash( _c)  ( (unsigned char)(_c) & _LineTypeBit)   //added chi 05.03.17

#define _setlinedash( _c)  ( (unsigned char)(_c) |= _LineTypeBit)   //added chi 05.03.17
#define _rsetlinedash( _c)  ( (unsigned char)(_c) &= _LineWidthBit)   //added chi 05.03.17

#define _linewidth( _c)  ( (unsigned char)(_c) &  _LineWidthBit)   //added chi 05.03.17


/*
#ifdef	DEF_MODULE
char *	draw_tools[]=	{
	"line",
	"circle",
	"rect",
	"selection",
	"text",
	"arc",
	"pline",
	"symbol",
	"gif"
	};
#else
extern	char *draw_tools[];
#endif
*/



