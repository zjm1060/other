/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2006.4.8 
  SYN by hcl 2007.12.12
 

---------------------------------------------------------------------*/



#ifndef   _GTK_SCALER_H 
#define   _GTK_SCALER_H 

 int  LOCAL_WIDTH  ;	 
 int  LOCAL_HEIGHT	;   

 float	xscale[MAX_CRT_NUMBER], yscale[MAX_CRT_NUMBER];	
 char fontdesc[MAX_CRT_NUMBER][20] ;  //font_size


 #define	POINT_OFFSET  2	// mouse drag distance (pixel)



#endif