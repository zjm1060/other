/***********************************************
   SYN by hcl 2007.12.12
**********************************************/
#ifndef  _ZOOM_DEF_H
#define  _ZOOM_DEF_H
typedef struct
{
	float	factor;
	int	factor_max;
	int	factor_min;
	float	step;
        int     zoom_lock ;   /* lock while load dbin include picture */
	int	sx_orig, sy_orig;
	int	canvas_w, canvas_h;
}       ZOOMDB;


#endif 

#ifndef _ZOOMDB_
#define _ZOOMDB_
ZOOMDB	zoomdb[MAX_CRT_NUMBER];
#else
extern ZOOMDB	zoomdb[MAX_CRT_NUMBER]; 
#endif

