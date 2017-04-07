/*-----------------------------------------------------------------------

  Upadate from X11 to GTK ;
  Created by chi.hailong
  Date : 2005.9.17
  Last Update data: 2005.9.22 
  UNIX 已经与WINDOWS版本同步!!! chihailong  2007.06.19 
  Last Update data: 2007.6.23
  Last SYNC hcl 2007.11.28
---------------------------------------------------------------------*/




#include	<gdk/gdk.h>

#include	"../inc/oix.h"
#include	"../inc/gtk_fill_bitmap.h"



/*#define	DEBUG*/
int Font_Size[]={6, 8,  8,13,14,18,26,30,36,46}; 
// int Font_Size[]={6, 9,  9,15,20,24,30,38,44,54};

#define _LeadByte 		0x80      /*  added by chi 05.03.08    */

#define _isleadbyte(_c)		( ( unsigned char) ( _c ) & _LeadByte )   /* added by chi 05.03.08  */
#define _floorInt(_c)		( ( int ) floor ( _c +.5 )   )   /* added by chi 08.01.10  */
extern SHM_RECORD	*shm_rec_addr;
extern GBRCD   ctrl_act_rcd_found[MAX_CRT_NUMBER];
extern GBRCD   *tag_rcd[MAX_CRT_NUMBER];
extern int _lstrlen(char *lpString);  /*  chi */

void	GetX_Coord_F (float h_lmt,float l_lmt,float v,float x1,float x2,float *y1,float *y2 );
void	GetX_Coord (float h_lmt,float l_lmt,float v,int x1,int x2,int *y1,int *y2 );
void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height );
void	GetTagPosition (GBRCD	*rcd, XRECT	*rect  );
void	ClearArea (int crt_n,  GdkDrawable	*drawable,int x1,int y1,int width,int height,int expose_mode ) ;
extern  char  OIX_USE_XFONT ; 
extern char   OIX_LIST_FONT[]; 
extern int	XtoSX (int crt_n,int x);
extern int	YtoSY (int crt_n,int y);
extern void    SetRcdColor (int crt_n,   GdkGC	*gc, BACKRCD 	*rcd);
extern int ColorId3to4(int id3);
extern void DrawTagImage(int crt_n,   GdkDrawable *drawable, GdkGC	*gc, int x,int y ,char *filename );
extern char *_toUtf8(char *c);
extern char *_toUtf8EX(char *c);
extern int getUnitConditionbyId(POINTER*pt);//机组运行工况
extern void PopupMessageBox (GtkWidget *parent,char msg[] );

//ipm 画字符串，字符串右侧如果有空格则，分辨率修改后显示重叠
//修改为OIX自动过滤右侧空格字符
void trimRightSpace(char * strr) //去掉右侧空格,制表符
{
	int i;
	int len = strlen(strr);
	for( i = len - 1; i>= 0;i--)
		if(strr[i] =='\t'||strr[i] ==' ')
			strr[i] = '\0';
		else
			break;
}

/*增加基础坐标系转换!!!*/
void oix_draw_lines(GdkDrawable *drawable, GdkGC *gc, GdkPoint *points, gint npoints,gint crt_n)
{
	int i ; 
	for (i = 0 ; i<npoints ; i++)
	{
		points[i].x    *=(int) xscale[crt_n]  ; 
		points[i].y    *=(int) yscale[crt_n]  ; 
	}
	gdk_draw_lines(drawable,  gc,  points,   npoints)  ; 
	
}
void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength ,int crt_n)  ; 
void  oix_draw_text(GdkDrawable * drawable ,GdkFont *font,GdkGC *gc, int x,int y,const char *text,int strlength ,gint crt_n)
{
	
	windk_draw_text(drawable ,   font, gc,   (int)(x ),  (int)(y ),     text,  strlength,crt_n) ; 
	
	
}

void oix_draw_line(GdkDrawable *drawable, GdkGC *gc, gint x1 , gint y1 , gint x2 , gint y2 ,gint crt_n )
{
	gdk_draw_line(drawable, gc,   (int)(x1 * xscale[crt_n] ),   (int)(y1 *yscale[crt_n]) ,   (int)(x2 * xscale[crt_n] ),   (int)(y2 *yscale[crt_n]));
	
}
void oix_draw_rectangle(GdkDrawable *drawable, GdkGC *gc, gboolean filled, gint x, gint y, gint width, gint height,int crt_n) 
{
	gdk_draw_rectangle( drawable,  gc,   filled,  (int)(x *xscale [crt_n]), (int) (y*yscale[crt_n]), (int) (width*xscale[crt_n]), (int) (height*yscale[crt_n])) ; 
}

void my_draw_lines(GdkDrawable  *drawable, GdkGC  *gc,GdkPoint *points,	 int npoints ,int HY, int LY   )
{
	int i ; 
    GdkPoint  ptTmp ,ptTmp1 ;
	/*	
	gdk_gc_set_foreground(gc, &pixels[0][GREEN4]); 
	gdk_draw_line(drawable , gc , 0  ,HY , 200  ,HY );
	gdk_draw_line(drawable , gc ,0  ,LY , 200  ,LY );
	*/
	
	for  (i = 1 ; i< npoints ; i++ )
	{
		
		if (points[i].y > LY  )
		{
			
			if (points[i-1].y <= LY &&  points[i-1].y > HY  )
			{   /*插入一个点*/
				ptTmp.x =  points[i-1].x  ;
				ptTmp.y =   LY  ;
				
				gdk_gc_set_foreground(gc, &pixels[0][GREEN4]);
                gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , ptTmp.x  ,ptTmp.y ); 
				
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,ptTmp.x  ,ptTmp.y  , points[i].x  ,points[i].y );
				
			}else if (points[i-1].y <= HY)
			{	/*插入二个点*/
				
				ptTmp1.x = ptTmp.x =  points[i-1].x  ;
				ptTmp.y =   LY  ;
				ptTmp1.y =  HY  ;
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , ptTmp1.x  ,ptTmp1.y ); 
				
				gdk_gc_set_foreground(gc, &pixels[0][GREEN4]);
                gdk_draw_line(drawable , gc ,ptTmp1.x  ,ptTmp1.y , ptTmp.x  ,ptTmp.y );
				
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,ptTmp.x  ,ptTmp.y , points[i].x  ,points[i].y );
			}
			else 
			{
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , points[i ].x  ,points[i ].y );
				printf ("points[i].y > HY\n\n");
			}
		}
		else if (points[i ].y < HY )
		{
			
			
			if (points[i-1].y >= HY && points[i-1].y <  LY  )
			{  /*插入一个点*/   
				ptTmp.x =  points[i].x  ;
				ptTmp.y =   HY  ;
				
				gdk_gc_set_foreground(gc, &pixels[0][GREEN4]);
                gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , ptTmp.x  ,ptTmp.y ); 
				
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,ptTmp.x  ,ptTmp.y, points[i].x  ,points[i].y );
				
			}
			else if (points[i-1].y > LY)
			{	/*插入二个点*/
				
				ptTmp1.x = ptTmp.x =  points[i-1].x  ;
				ptTmp.y =   HY  ;
				ptTmp1.y =  LY  ;
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , ptTmp1.x  ,ptTmp1.y ); 
				
				gdk_gc_set_foreground(gc, &pixels[0][GREEN4]);
                gdk_draw_line(drawable , gc ,ptTmp1.x  ,ptTmp1.y , ptTmp.x  ,ptTmp.y );
				
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,ptTmp.x  ,ptTmp.y , points[i].x  ,points[i].y );
				printf ("插入二个点\n\n");  
				
			}
			else 
			{
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
				gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , points[i].x  ,points[i].y );
				printf ("points[i].y < LY\n\n");
			}
		}
		else 
		{
			if (points[i-1].y <HY )
			{
				ptTmp.x =  points[i].x  ;
				ptTmp.y =   HY  ;
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , ptTmp.x  ,ptTmp.y ); 
				
				gdk_gc_set_foreground(gc, &pixels[0][GREEN4]);
                gdk_draw_line(drawable , gc ,ptTmp.x  ,ptTmp.y, points[i].x  ,points[i].y );
				
				
			}
			else if (points[i-1].y >LY)
			{
				ptTmp.x =  points[i].x  ;
				ptTmp.y =   LY  ;
				gdk_gc_set_foreground(gc, &pixels[0][RED1]);
                gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , ptTmp.x  ,ptTmp.y ); 
				
				gdk_gc_set_foreground(gc, &pixels[0][GREEN4]);
                gdk_draw_line(drawable , gc ,ptTmp.x  ,ptTmp.y, points[i].x  ,points[i].y );
			}
			else
			{
				
				gdk_gc_set_foreground(gc, &pixels[0][GREEN4]); 
				printf ("GREEN \n\n");
				gdk_draw_line(drawable , gc ,points[i-1].x  ,points[i-1].y , points[i ].x  ,points[i ].y );
			}
		}
		
		
		
		
		
		
		
	}
	
}
int _lstrlen(char *lpString)
{
	int n =0 ;
	while (*lpString)
	{
		if (_isleadbyte(*lpString))
			lpString +=2 ;
		else
			lpString ++ ;
		n++ ;	        	
	}      /* while */
	return n ; 
}



/*  added vertical print out  by chi 05.03.09  */

void    windk_get_text_height(char *fontdesc, int* cx,int *cy,char*text ) 
{
	
	
    PangoFontDescription *font_desc ;
    PangoLayout  *layout   ; 
    gchar  *sText ;	
    sText = _toUtf8(text) ;
	layout  =	gtk_widget_create_pango_layout(canvas[0],sText);
	font_desc = pango_font_description_from_string (fontdesc);   
    pango_layout_set_font_description(layout,font_desc);
	
	if (cx && cy) 
		pango_layout_get_pixel_size( layout, cx  , cy );
    g_object_unref(G_OBJECT(layout));
	G_SAFE_FREE(sText);//dzy1017
    pango_font_description_free(font_desc);		 	 
	
}
int windk_text_height    (GdkFont        *font,      gchar    *text,   gint txt_length ,char *fontdesc)
{
	//#if defined( WINDOWS_OS)
    int cx, cy ;
    windk_get_text_height( fontdesc,  &cx, &cy, text ) 	 ;
	return cy ; 
	//#elif defined( SUN_OS)
	//	return gdk_text_height( font ,text  ,  txt_length) ;
	//#endif 	 
	
}

void  pango_draw_text (GdkDrawable * drawable ,const char  *fontdescr ,GdkGC  *gc, int x,int y,const char   *text,int  strlength,int crt_n)
{
	PangoFontDescription *font_desc ;
    PangoLayout  *layout   ; 
	int fontsize = 12 ;
    gchar *sText =0 ;
	int cy,cx ;
    
#ifdef   _AIX
	PangoLayout  *layout1   ;
	int aix_cy;
#endif
	
   	//add by hcl
	//	char tmptext[256];	
	// strcpy(tmptext,text);
    strlength = strlen(text);
	sText= _toUtf8EX((gchar*)text /*tmptext*/);
    layout  =	gtk_widget_create_pango_layout(canvas[crt_n],sText);
	pango_layout_get_pixel_size( layout, &cx  , &cy );
	font_desc = pango_font_description_from_string (fontdescr/*fontdesc[0]*/);   
    pango_layout_set_font_description(layout,font_desc);
	
#ifdef   _AIX
	layout1  =	gtk_widget_create_pango_layout(canvas[crt_n],"A");
	pango_layout_get_pixel_size( layout1, NULL  , &aix_cy );
	cy=aix_cy;
#endif
    
	gdk_draw_layout( drawable,  gc, (int)(x*xscale[crt_n]),  (int)(y*yscale[crt_n])-cy, layout);
    g_object_unref(G_OBJECT(layout));
	G_SAFE_FREE(sText);//dzy1017
	pango_font_description_free(font_desc);	
	
}


//此函数决定 事件表，报警表，数据一览表 表头汉字显示  增加了字体的点阵大小
void    windk_draw_text_EX(GdkDrawable * drawable ,char  *fontdescr ,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n ) 
{
	
   	pango_draw_text (  drawable ,fontdescr , gc,   x,  y, text,   strlength,  crt_n) ;
   	
}

//此函数决定 数据一览表 表头汉字显示
void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n ) 
{
	
	if  (OIX_USE_XFONT)
	{
		strlength = strlen(text);
#ifdef SUN_OS  
		gdk_draw_text( drawable , font, gc,    (int)(x*xscale[crt_n]),  (int)(y*yscale[crt_n]),     text,   strlength) ;
#endif 
#ifdef WINDOWS_OS
		pango_draw_text (  drawable ,    OIX_LIST_FONT , gc,   x,  y, text,   strlength,  crt_n) ;
		// gdk_draw_text  在windows会崩溃;
#endif
	}else
	{  
		pango_draw_text (  drawable ,    OIX_LIST_FONT , gc,   x,  y, text,   strlength,  crt_n) ;		
	}
	
	
}

// 		PangoFontDescription *font_desc ;
// 		PangoLayout  *layout   ; 
// 		int fontsize = 10 ;
// 		gchar *sText =0 ;
// 		int cy,cx ;
// 		
// 		
// 		//add by hcl
// 		char tmptext[256];	
// 		strcpy(tmptext,text);
// 		
// 		sprintf(fontdesc[0] , "sans  %d " ,fontsize); /* added by zyp on 2008.7.10*/
// 		
// 		strlength = strlen(text);
// 		sText= _toUtf8EX(tmptext);
// 		layout  =	gtk_widget_create_pango_layout(canvas[crt_n],sText);
// 		pango_layout_get_pixel_size( layout, &cx  , &cy );
// 		font_desc = pango_font_description_from_string (fontdesc[0]);   
// 		pango_layout_set_font_description(layout,font_desc);
// 		gdk_draw_layout( drawable,  gc, (int)(x*xscale[crt_n]),  (int)(y*yscale[crt_n])-cy, layout);
// 		g_object_unref(G_OBJECT(layout));
// 		if (sText) g_free(sText);
// 		pango_font_description_free(font_desc);	


void	DrawRcd (int crt_n, GdkDrawable	*drawable, GdkGC *gc, BACKRCD *rcd ,BOOL isTrutype)
{
	int	fill_type;int xoffset = 0 ; 
	int	x, y, width, height,i,j,linewidth;
	BACKRCD	tmp;
	char *pChar ;
	char  sWBuff[4] ;
	char  sBuff[1]  ;   /*  chi  */
	int   cxChar, cyChar ;
	BOOL isDouble ;
    GdkPoint		gpl[MAXPLCNT];  
    char  cBuff[MAX_TEXT_LEN +5] ;
	char  cBuff_no_rightspace[MAX_TEXT_LEN +5] ;
    int	bsl_x , bsl_y ;     
    PangoFontDescription *font_desc ;
    PangoLayout  *layout   ;  
	int  font_size;char s[10];
	gchar *sText =0 ;
	tmp	=  * rcd;
	font_desc  = NULL;
	layout  = NULL ; 
	
	/*
	#ifdef  WINDOWS_OS
    char fontdesc[20]= "simhei ";   / *****simhei***** /
	#else    / *SUN_OS * /
    char fontdesc[20]=  "FZHeiTi   " ; 
	#endif
	*/
	
	
#ifdef  WINDOWS_OS
    strcpy((char*)fontdesc,_CS_("OIX_WIN_FONT_DESC")) ;
#else    /*SUN_OS */
    strcpy(fontdesc,_CS_("OIX_SUN_FONT_DESC")) ;
#endif
	
	
	
	if (drawable == bakgrd_pixmap[crt_n]   || drawable == pixmap[crt_n] ||   drawable == canvas[crt_n]->window)    
	{ 
		tmp.rect.x1	=   XtoSX ( crt_n, rcd->rect.x1); 
		tmp.rect.y1	=	YtoSY ( crt_n, rcd->rect.y1); 
		tmp.rect.x2	=   XtoSX ( crt_n, rcd->rect.x2); 
		tmp.rect.y2	=	YtoSY ( crt_n, rcd->rect.y2); 
	}
	
	fill_type	=rcd->fill.fill_type ;
	linewidth	= _linewidth( rcd->line_width );
	
	
	
	SetRcdColor ( crt_n,   gc, rcd );
    gdk_gc_set_function(gc, GDK_COPY);
	
	if (rcd->fill.fill_type  == UNFILLED || rcd->draw_type == DrwT_TEXT)
	{ 
		gdk_gc_set_fill ( gc, GDK_SOLID ); 
	}
	else
	{
		gdk_gc_set_fill( gc, GDK_OPAQUE_STIPPLED );
		gdk_gc_set_stipple(gc, fill_pixmenu[crt_n][rcd->fill.fill_type]);
	}	
	
	
	if (_islinedash( rcd->line_width ))
		
		gdk_gc_set_line_attributes(gc,linewidth,GDK_LINE_ON_OFF_DASH,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);
	else	
		gdk_gc_set_line_attributes(gc,linewidth,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND); 
	
	
	switch (rcd->draw_type )
	{
		
	case	DrwT_LINE:
		gdk_draw_line (  drawable, gc, (int)(tmp.rect.x1 * xscale[crt_n]),(int)(tmp.rect.y1 * yscale[crt_n]) , 
			(int) (tmp.rect.x2 * xscale[crt_n]), (int)(	tmp.rect.y2 * yscale[crt_n]));  
		break;
		
	case	DrwT_ARC:
		if (drawable == bakgrd_pixmap[crt_n] ||  drawable == canvas[crt_n]->window  || drawable == pixmap[crt_n] )    
		{
			tmp.data.arc.x   = XtoSX ( crt_n, tmp.data.arc.x);
			tmp.data.arc.y   = YtoSY ( crt_n, tmp.data.arc.y);
			tmp.data.arc.width	=(unsigned short) (tmp.data.arc.width*zoomdb[crt_n].factor);
			tmp.data.arc.height	= (unsigned short) (tmp.data.arc.height*zoomdb[crt_n].factor);
		}
		
		gdk_draw_arc ( drawable, gc,fill_type,(int)(tmp.data.arc.x * xscale[crt_n]) ,(int)(tmp.data.arc.y * yscale[crt_n]) ,
			(int)(tmp.data.arc.width* xscale[crt_n]),(int)(tmp.data.arc.height* yscale[crt_n]),tmp.data.arc.angle1,tmp.data.arc.angle2  );  
		
		
		break;
		
	case	DrwT_RECTANGLE:	   	
	  	CheckRectCoords (tmp.rect, &x, &y, &width, &height );
		 
    
	 	gdk_draw_rectangle ( drawable, gc,(fill_type != UNFILLED), (int)(x * xscale[crt_n]) ,(int)(y * yscale[crt_n]) , 
						_floorInt(width  * xscale[crt_n]  ), _floorInt(height  * yscale[crt_n] ) );
 	 
	

		break;
		
	case	DrwT_CIRCLE:
		CheckRectCoords (tmp.rect, &x, &y, &width, &height );
		
		gdk_draw_arc (drawable, gc,(fill_type != UNFILLED), (int)(x * xscale[crt_n]), (int)(y * yscale[crt_n]), (int)(width * xscale[crt_n]), (int)(height * yscale[crt_n]), ANGLE_0, ANGLE_360);
		
		break;
		
	case	DrwT_TEXT:
		{
			
			
			/* Modify by chi chi!!!  */
			
			bsl_x = rcd->rect.x1;
			bsl_y = rcd->rect.y1; 
			
			font_size  = Font_Size[tmp.ftset_id];
			font_size  = (int)( xscale[crt_n] *  font_size )   ; 
			if (drawable == bakgrd_pixmap[crt_n] ||  drawable == canvas[crt_n]->window  || drawable == pixmap[crt_n] )    
			{ 
				bsl_x	= XtoSX ( crt_n, bsl_x);
				bsl_y	= YtoSY ( crt_n, bsl_y);
				font_size = (int)((float)(Font_Size[tmp.ftset_id]* zoomdb[crt_n].factor) * xscale[crt_n]*1.2);	 // 0.9 -> 1.2 适应新字体 chichi	
			}

             sprintf(s,"   %d", font_size );  strcat((char*)fontdesc, s);   
						
 
			if (rcd->data.text.DirOut == ITEM_TAG_VERTI   )
			{
				/*  VERTICAL ALIGN !!!  */
				
                pChar = rcd->data.text.text ;
                sText = _toUtf8(rcd->data.text.text) ;
                layout  = gtk_widget_create_pango_layout(canvas[crt_n],sText);
                
                font_desc = pango_font_description_from_string ((char*)fontdesc);   
                pango_layout_set_font_description(layout,font_desc);
                pango_layout_get_pixel_size( layout, &cxChar ,&cyChar ); 

				/*      pango_font_description_free(font_desc);  */
                g_object_unref(G_OBJECT(layout));  
				G_SAFE_FREE(sText);//dzy1017

                x = tmp.rect.x1 ; y =tmp.rect.y1  ;
                layout = NULL ;
				
                for ( i = 0; i<_lstrlen(rcd->data.text.text);i++)
                {
					memset(sWBuff,0,4) ;
					if( _isleadbyte(  (*pChar)))
					{
						strncpy(sWBuff ,pChar,2); 
						sText = _toUtf8(sWBuff);
						layout  = gtk_widget_create_pango_layout(canvas[crt_n],sText);
						pango_layout_set_font_description(layout,font_desc);
						gdk_draw_layout( drawable,  gc, (int)(x * xscale[crt_n]) ,(int)(y * yscale[crt_n]), layout);
						g_object_unref(layout); 
						G_SAFE_FREE(sText) ;
						pChar +=2  ;
						
					}
					else 
					{
						strncpy(sBuff ,pChar,1);
						sText = _toUtf8(sBuff) ;
						layout  = gtk_widget_create_pango_layout(canvas[crt_n],sText);
						pango_layout_set_font_description(layout,font_desc);
						gdk_draw_layout( drawable,  gc,(int)(x * xscale[crt_n]) ,(int)(y * yscale[crt_n]), layout);            
						g_object_unref(layout);
						G_SAFE_FREE(sText);//dzy1017
						pChar ++  ;
						
					}
					
					y += cyChar ;
					
				}
				
				pango_font_description_free(font_desc); 
				
			}else			/*horizontal  ALIGN!!!*/
			{
#ifdef WIN32            
				memset (cBuff,0 ,sizeof(cBuff));
				isDouble= FALSE;
				
				for (  i=0 ,j= 0;rcd->data.text.text[i] != '\0';i++)
				{
					if((int) rcd->data.text.text[i]==32)   /*space  ascii code*/
					{
						if(isDouble)
						{
							isDouble=FALSE;
							continue;
						}
						isDouble=TRUE;
					}
					else isDouble = FALSE;
					cBuff[j++] = rcd->data.text.text[i];						 
				}    
				//去掉右侧空格字符串
				trimRightSpace(cBuff);

				sText = _toUtf8(cBuff) ;

				layout  = gtk_widget_create_pango_layout(canvas[crt_n],sText);  
#else
				sText = _toUtf8(rcd->data.text.text) ;
				layout  = gtk_widget_create_pango_layout(canvas[crt_n],sText);
#endif
                 	font_desc = pango_font_description_from_string ((char*)fontdesc);   
                 	pango_layout_set_font_description(layout,font_desc);
					pango_layout_get_pixel_size( layout, &cxChar ,&cyChar );
					CheckRectCoords (tmp.rect, &x, &y, &width, &height );

					if  (cxChar > width*xscale[crt_n])
						xoffset  =(int) (cxChar-width*xscale[crt_n]) /2 ;
					else 
						xoffset   = 0 ;  	
                	gdk_draw_layout( drawable,  gc,(int)(( bsl_x /*-xoffset*/) * xscale[crt_n]-xoffset) ,(int)(bsl_y * yscale[crt_n]) , layout);
                    g_object_unref(G_OBJECT(layout));
					G_SAFE_FREE(sText);//dzy1017
                    pango_font_description_free(font_desc);		 	            
                     
             }	

		}
		break;
		
	case	DrwT_PLINE:
		{
			int	i; 		 
			for (i=0; i< tmp.plcnt; i++)
			{
				if (drawable == bakgrd_pixmap[crt_n]  ||   drawable == canvas[crt_n]->window  || drawable == pixmap[crt_n] )  
				{
					gpl[i].x = XtoSX ( crt_n, tmp.data.pl[i].x);      
					gpl[i].y = YtoSY ( crt_n, tmp.data.pl[i].y);
				}
				else
				{
					gpl[i].x =   tmp.data.pl[i].x ;      
					gpl[i].y =   tmp.data.pl[i].y ;
				}
				
				gpl[i].x= (long)(gpl[i].x * xscale[crt_n]);
				gpl[i].y= (long)(gpl[i].y * yscale[crt_n]);
				
			}
			
			if (fill_type == UNFILLED)
			{
				gdk_draw_lines ( drawable, gc, gpl, tmp.plcnt);
			}
			else
			{
				gdk_draw_polygon (  drawable, gc, TRUE ,gpl, tmp.plcnt);
			}
		}
		break;
	default:	;
	}
}
/**     draw bargraph  limit line . chichi */
void	DrawDynBarLmtRcd (int crt_n, GdkDrawable *drawable, GBRCD	*rcd,  BAR_GRAPH_LMT *lmt ) 
{
	
	int	fg_sav;
	BACKRCD	dyn_rcd;
	int	x, y, y1, y2, width, height;
	
	CheckRectCoords (rcd->bck.rect, &x, &y, &width, &height );
	rcd->bck.rect.x1 = x; 
	rcd->bck.rect.y1 = y; 
	rcd->bck.rect.x2 = x+width; 
	rcd->bck.rect.y2 = y+height;
    
	dyn_rcd 	= rcd->bck;
	dyn_rcd.draw_type 	=  DrwT_LINE;
	fg_sav 		= dyn_rcd.fg;
	dyn_rcd.fg 	= BLACK;
	if (!(lmt->hi_limit1 == 9999.00 || lmt->hi_limit1 == -9999.00))
	{
		GetX_Coord ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
			lmt->hi_limit1, rcd->bck.rect.y1, rcd->bck.rect.y2, &y1, &y2 );
		dyn_rcd.fg 	= ColorId3to4(RED1) ;
		dyn_rcd.rect.y1 	= y1;
		dyn_rcd.rect.y2 	= y1;
		gdk_gc_set_foreground ( info[crt_n].gc, &pixels[crt_n][BLUE4]);
		gdk_draw_string (drawable,font_set[0],info[crt_n].gc , rcd->bck.rect.x1 , y1-5 ,"H1");
		DrawRcd ( crt_n,  drawable, info[crt_n].gc, &dyn_rcd,FALSE );
	}
	if (!(lmt->low_limit1 == 9999.00 || lmt->low_limit1 == -9999.00))
	{
		GetX_Coord ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
			lmt->low_limit1, rcd->bck.rect.y1, rcd->bck.rect.y2, &y1, &y2 );
		dyn_rcd.fg 	=  ColorId3to4(GREEN);
		dyn_rcd.rect.y1 	= y1;
		dyn_rcd.rect.y2 	= y1;
		gdk_gc_set_foreground ( info[crt_n].gc, &pixels[crt_n][BLUE4]);
		gdk_draw_string (drawable,font_set[0],info[crt_n].gc , rcd->bck.rect.x1 , y1-5 ,"L1");
		DrawRcd ( crt_n,  drawable, info[crt_n].gc, &dyn_rcd,FALSE );
	}
	
	
}




/* vibration area     added by chichi draw   !!!  */
void	DrawDynVibraAreaRcd (int crt_n, GdkDrawable *drawable, GBRCD	*rcd, float upRange ,float lowRange ) 
{
	
	int	fg_sav;
	BACKRCD	dyn_rcd;
	int	x, y, y1, y2, width, height;
	
	CheckRectCoords (rcd->bck.rect, &x, &y, &width, &height );
	rcd->bck.rect.x1 = x; 
	rcd->bck.rect.y1 = y; 
	rcd->bck.rect.x2 = x+width; 
	rcd->bck.rect.y2 = y+height;
    
	dyn_rcd 	= rcd->bck;
	fg_sav 		= dyn_rcd.fg;
	dyn_rcd.fill.fill_type 	= rcd->bck.fill.fill_type /*FILLED  */;
	dyn_rcd.fg 	= BLACK;
	
	GetX_Coord ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
		upRange, rcd->bck.rect.y1, rcd->bck.rect.y2, &y1, &y2 );
	
	dyn_rcd.fg 	= fg_sav;
	
	dyn_rcd.rect.y1 = y1;
	GetX_Coord ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
		lowRange, rcd->bck.rect.y1, rcd->bck.rect.y2, &y1, &y2 );
	
	dyn_rcd.rect.y2 	= y1;
	DrawRcd ( crt_n,  drawable, info[crt_n].gc, &dyn_rcd,FALSE );
	
}

int GetRcdColor(int crt_n,GBRCD	*rcd)
{
	char db_name[100];
	DMS_COMMON      dms_cmm;
	int anlgvalue;
	int fgcolor;
	POINTER pt ;
//	char str[24];
	int UnitConditon;
	int colorgroup;
	int i;
	strcpy(db_name,rcd->lnk.dbname);
	
	if ( ReadEntryByNameStr ( db_name,  &dms_cmm )== -1)
	{
		char		info[50];
		strcpy(info,OIX_STR_DB_NO_POINT);
		strcat(info,db_name );
		PopupMessageBox ( main_w[crt_n], info);
		return -1;              
	}
	switch (rcd->lnk.dyn.data_class)
	{
		
	case  0://NULL 
		fgcolor=rcd->bck.fg;
		return fgcolor;
		break;
	case 1://ANA_LIMIT
		anlgvalue=dms_cmm.var.anlg.value.data/1000;
		
		if (anlgvalue>=dms_cmm.var.anlg.lmt_para.HI_ALARM)// 越上上限
		{
			return cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[1].c_normal;
		}else if (anlgvalue>=dms_cmm.var.anlg.lmt_para.HI_WARN&&anlgvalue<dms_cmm.var.anlg.lmt_para.HI_ALARM)// 越上限
		{
			return  cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[2].c_normal;
		}else if (anlgvalue<dms_cmm.var.anlg.lmt_para.HI_WARN&&anlgvalue>dms_cmm.var.anlg.lmt_para.LO_WARN)//正常 上限 下限之间
		{
			return cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[0].c_normal;
			
		}else if (anlgvalue>dms_cmm.var.anlg.lmt_para.LO_ALARM&&anlgvalue<=dms_cmm.var.anlg.lmt_para.LO_WARN)// 越下限 
		{
			return cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[3].c_normal;
		}
		else if (anlgvalue<=dms_cmm.var.anlg.lmt_para.LO_ALARM)// 越下下限
		{
			return cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[4].c_normal;
		}
		break;
		
	case  2://Unit_STATUS 
		{
			if (GetPtIdByNameStr(rcd->lnk.dbname,&pt) == -1)
			{
				printf ( "OIX:in DrawDynamicBarChart  %s data type found in database.\n", rcd->lnk.dbname );
				fgcolor=rcd->bck.fg;
				return -1;
			}
			
			UnitConditon=getUnitConditionbyId(&pt);//机组运行工况
			
			colorgroup=-1;
			
			for (i=0; i<cgdb_cnt; i++)
			{
				if (strcmp("UNIT_CONDITION", cgdb[i].name) == 0)
				{
					colorgroup=i;
					break;
				}
			}
			if (colorgroup!=-1)
			{
				
				switch (UnitConditon)
				{
				case  0://未定义区
					{
						return cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[0].c_normal;	
					}
					
				case  1://禁止区
					{
						return cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[1].c_normal;	
					}
				case  2://正常
					{
						return cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[2].c_normal;	
					}
				case  3://限制区
					{
						return cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[3].c_normal;	
					}
					
				}	
			}else
			{
				return rcd->bck.fg;
			}
			
			break;
		}
	default:
		fgcolor=rcd->bck.fg;
		return fgcolor;
	}

		return rcd->bck.fg;
}
void	DrawDynBarRcd (int crt_n, GdkDrawable *drawable, GBRCD	*rcd, float rt_value ) 
{
	
	int	fg_sav;
	BACKRCD	dyn_rcd;
	int	x, y, m1,m2, width, height;
	
	
	// rcd->lnk.dyn.fmt_def.bar_chart.direction  ;    
	CheckRectCoords (rcd->bck.rect, &x, &y, &width, &height );
	rcd->bck.rect.x1 = x; 
	rcd->bck.rect.y1 = y; 
	rcd->bck.rect.x2 = x+width; 
	rcd->bck.rect.y2 = y+height;
	
	dyn_rcd 	= rcd->bck;
	fg_sav 		= dyn_rcd.fg;
	dyn_rcd.fill.fill_type 	= FILLED;
	dyn_rcd.fg 	= BLACK;
	
	
	switch (rcd->lnk.dyn.fmt_def.bar_chart.direction)
	{
		
	case   0:  //UP	 
		GetX_Coord ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
			rt_value, rcd->bck.rect.y1, rcd->bck.rect.y2, &m1, &m2 );
		dyn_rcd.rect.y1 	= m1;
		dyn_rcd.rect.y2 	= m2;
		break ; 
	case 1:  //Down
		GetX_Coord ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
			rt_value, rcd->bck.rect.y2, rcd->bck.rect.y1, &m1, &m2 );
		dyn_rcd.rect.y2 	= m1;
		dyn_rcd.rect.y1 	= MIN(rcd->bck.rect.y1, rcd->bck.rect.y2);
		break; 
	case 3: //left 
		GetX_Coord ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
			rt_value, rcd->bck.rect.x2, rcd->bck.rect.x1, &m1, &m2 );
		dyn_rcd.rect.x1 	= m1;
		dyn_rcd.rect.x2 	= MIN(rcd->bck.rect.x1, rcd->bck.rect.x2);
		break; 
	case 2: //right 
		GetX_Coord ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
			rt_value, rcd->bck.rect.x1, rcd->bck.rect.x2, &m1, &m2 );
		dyn_rcd.rect.x2 	= m1;
		dyn_rcd.rect.x1 	= MAX(rcd->bck.rect.x1, rcd->bck.rect.x2);
		break;
	}
	dyn_rcd.fg 	= GetRcdColor(crt_n,rcd);//fg_sav;
	DrawRcd ( crt_n,  drawable, info[crt_n].gc, &dyn_rcd,FALSE );
	
}

void	DrawDynLine (int crt_n,GBRCD	*rcd, float	curve[] ) 
{
	
	int	fg_sav;
	BACKRCD	dyn_rcd;
	int	x, y, width, height;
	int	y1, y2;				  
	int	tx, offset = rcd->lnk.dyn.f_by_off;
	int	 j2, j;
	
	CheckRectCoords (rcd->bck.rect, &x, &y, &width, &height );
	rcd->bck.rect.x1 = x; rcd->bck.rect.y1 = y; rcd->bck.rect.x2 = x+width; rcd->bck.rect.y2 = y+height;
	
	dyn_rcd 	= rcd->bck;
	fg_sav 		= dyn_rcd.fg;
	dyn_rcd.fill.fill_type 	= FILLED;
	dyn_rcd.fg 	= BLACK;
	DrawRcd ( crt_n,   canvas[crt_n]->window, info[crt_n].gc, &dyn_rcd ,FALSE);
	
	dyn_rcd 	= rcd->bck;
	dyn_rcd.fg 	= fg_sav;
	dyn_rcd.fill.fill_type 	= UNFILLED;
	dyn_rcd.draw_type 	= DrwT_PLINE;
	
	if ( offset != 0)
	{
		tx	= ( dyn_rcd.rect.x2 - dyn_rcd.rect.x1 )/offset;
	}
	else
	{
		printf ( "in OIX: offset is 0, float under floating. \n" );
		return;
	}
	
	j2	= offset/(MAXPLCNT-1)+1;
	for ( j=0; j<j2; j++)
	{
		int	i, i1, i2;
		
		dyn_rcd.plcnt	= 0;
		
		i1	= j*MAXPLCNT;
		i2	= MIN ( (j+1)*MAXPLCNT, offset );
		
		if ( j == j2-1 )
			i2	= i2+j2-1;
		
		for ( i= i1; i < i2; i++)
		{
			GetX_Coord ( rcd->lnk.dyn.fmt_def.curve.hi_limit, rcd->lnk.dyn.fmt_def.curve.low_limit, 
				curve[i-j], rcd->bck.rect.y1, rcd->bck.rect.y2, &y1, &y2 );
			dyn_rcd.plcnt++;
			dyn_rcd.data.pl[i-i1].y	= y1;
			dyn_rcd.data.pl[i-i1].x	= dyn_rcd.rect.x1+tx*(i-j);
		}
		DrawRcd ( crt_n,   canvas[crt_n]->window, info[crt_n].gc, &dyn_rcd ,FALSE);
	}
	
}

char *bitmapfile[4]={"tag1.bmp","tag2.bmp","tag3.bmp","tag4.bmp"};
void	DrawTag ( int crt_n, GdkDrawable  *drawable, GdkGC *gc, GBRCD	*rcd   )
{
	BACKRCD tmp;
	XRECT	rect;
	USHORT 	tag =0 ;
	int	x,y;
	
	if ( rcd->lnk.dyn.tagi_loc != TAG_NONE )
	{
		tmp             = rcd->bck;
		GetTagPosition ( rcd, &rect );
		x=rect.x1;
		y=rect.y1;
		
		/*		x =  MIN(	rcd->bck.rect.x1 ,	rcd->bck.rect.x2 ) ;
		y  = MIN(	rcd->bck.rect.y1 ,	rcd->bck.rect.y2 ) ; test*/ 		
		
		if (-1 ==GetPtTagStateByNameStr(rcd->lnk.dbname,&tag ))
			return ;         	               
		
#ifdef	DEBUG  
		
		printf ( "in DrawTag: TagPosition=: %d,   tag= %d\n", 
			rcd->lnk.dyn.tagi_loc,   tag );
#endif		 
		
		switch  ( tag )
		{
		case    0:
			BITMAP_TAG_OFF = 0;
			ClearArea ( crt_n,  drawable, x,y,   80, 80 ,  k_no_expose ) ;
			
			break;
			
		case    1:
			DrawTagImage(crt_n,  drawable , gc , x,y, bitmapfile[1] );
			Tag_bit = 1;
			break;
			
		case    2:
			DrawTagImage(crt_n,  drawable , gc , x,y, bitmapfile[2] );
			break;
			
		case    3:
			DrawTagImage(crt_n,  drawable , gc , x,y, bitmapfile[3] );
			break;
			
		default:
			;
		}
		
	}	
}
#define	TAG_SIZE	64   // 挂牌图片尺寸
void	GetTagPosition (GBRCD	*rcd, XRECT	*rect  )
{
	int	x_mean, y_mean;
	x_mean	= ( rcd->bck.rect.x1 + rcd->bck.rect.x2 )/2;
	y_mean	= ( rcd->bck.rect.y1 + rcd->bck.rect.y2 )/2;
	
	switch	( rcd->lnk.dyn.tagi_loc )
	{
	case	TAG_UP:
		rect->y1	= MIN(rcd->bck.rect.y1,rcd->bck.rect.y2) -   TAG_SIZE;
		rect->y2	= rect->y1 -2;
		rect->x1	= MIN(rcd->bck.rect.x1,rcd->bck.rect.x2)	;		  // x_mean - TAG_SIZE/2;
		rect->x2	= x_mean + TAG_SIZE/2;
		break;
		
	case	TAG_DOWN:
		rect->y1	= MAX(rcd->bck.rect.y1,rcd->bck.rect.y2 )	;
		rect->y2	= rect->y1 + TAG_SIZE;
		rect->x1	= MIN(rcd->bck.rect.x1,rcd->bck.rect.x2)	;		  // x_mean - TAG_SIZE/2;
		rect->x2	= x_mean + TAG_SIZE/2;
		break;
		
	case	TAG_LEFT:
	case	TAG_UP_LEFT:
	case	TAG_DOWN_LEFT:
		rect->x2	= rcd->bck.rect.x1 -2;
		rect->x1	= rect->x2 - TAG_SIZE;
		rect->y1	= y_mean - TAG_SIZE/2;
		rect->y2	= y_mean + TAG_SIZE/2;
		break;
		
	case	TAG_RIGHT:
	case	TAG_UP_RIGHT:
	case	TAG_DOWN_RIGHT:
		rect->x1	= MIN(rcd->bck.rect.x2,rcd->bck.rect.x1)+2;
		rect->x2	= rect->x1 + TAG_SIZE;
		rect->y1	= y_mean - TAG_SIZE/2;
		rect->y2	= y_mean + TAG_SIZE/2;
		break;
		
	default:	/* the default position is TAG_UP_LEFT: added by wgp 2000.2.16 */
		if( rcd->lnk.dyn.tagi_loc != TAG_NONE )
		{
			rect->x2	= rcd->bck.rect.x1-2 ;
			rect->x1	= rect->x2 - TAG_SIZE;
			rect->y1	= y_mean - TAG_SIZE/2;
			rect->y2	= y_mean + TAG_SIZE/2;
		}
		break;
	}
	
	
}


void	DrawDynPQ_Dot (int crt_n, GdkDrawable *drawable , GBRCD	*rcd,float	curve[] )
{	
	BACKRCD	dyn_rcd;
	int	x, y, width, height;
	/* added by wgp 99.6.16 */
	int	y1, y2;
	int	delta_xy	= 5;
	
	CheckRectCoords (rcd->bck.rect, &x, &y, &width, &height );
	rcd->bck.rect.x1 = x; rcd->bck.rect.y1 = y; rcd->bck.rect.x2 = x+width; rcd->bck.rect.y2 = y+height;
	dyn_rcd 	= rcd->bck;
	dyn_rcd.bg 	= BLACK;
	
	dyn_rcd.fill.fill_type 	= FILLED;
	dyn_rcd.fill.fill_type 	= BLACK;
//	DrawRcd ( crt_n,  canvas[crt_n]->window, info[crt_n].gc, &dyn_rcd ,FALSE);
	
	dyn_rcd.fg 	= ColorId3to4(RED1);
	dyn_rcd.fill.fill_type 	= FILLED;
	dyn_rcd.draw_type 	= DrwT_RECTANGLE;
	/* modified by wgp 99.6.16 */
	
	GetX_Coord ( rcd->lnk.dyn.fmt_def.curve.hi_limit, rcd->lnk.dyn.fmt_def.curve.low_limit, 
		curve[0], rcd->bck.rect.y1, rcd->bck.rect.y2, &y , &y1);
	GetX_Coord ( rcd->lnk.dyn.fmt_def.curve.hi_limit_2, rcd->lnk.dyn.fmt_def.curve.low_limit_2, 
		curve[1], rcd->bck.rect.x2, rcd->bck.rect.x1, &x , &y2);
	dyn_rcd.rect.y1 	= y - delta_xy;
	dyn_rcd.rect.x1 	= x - delta_xy;
	dyn_rcd.rect.y2 	= y + delta_xy;
	dyn_rcd.rect.x2 	= x + delta_xy;
	DrawRcd ( crt_n,   drawable, info[crt_n].gc, &dyn_rcd ,FALSE);
}

/* modified by wgp , where y2 is added for bar_chart .99.6.16 */

void	GetX_Coord (float h_lmt,float l_lmt,float v,int x1,int x2,int *y1,int *y2 )
{
	/*int	delt_data;  by zyp on 2004.10.28 */
	float   delt_data;
	float	fact;
	
	delt_data	= h_lmt - l_lmt ;
	
	/*if ( abs (delt_data) <= 0.00001 )*/
	if ( fabs (delt_data) <= 0.00001 )
	{
		printf ( "Float underflow!!!\n" );
		fact	= 0.0;
	}
	else
	{
		fact	= (float)(x1-x2)/delt_data;
	}
	
	if ( v < l_lmt )
		v	= l_lmt;
	else if ( v > h_lmt )
		v	= h_lmt;
	
	if ( v < l_lmt || v > h_lmt )
	{
		printf ( "Paramter exceeds limit\n" );
	}
	else if ( v <= h_lmt && v >= l_lmt ) 
	{
		if ( l_lmt >= 0)
		{
			*y1 	= (int ) (x2 + ( v - l_lmt )*fact );
			*y2 	= x2; 				/*Modified by zyp.      change '+' to '-'*/
		}
		else if ( l_lmt < 0)
		{
			/* modified by wgp 99.6.16 ,then modified by zyp on 1999.8.17 to draw the PQ dot*/
			/*	*y1     = (int ) (x1- h_lmt*fact );   */
			
			if(v>=0)
				*y1		= (int)(x1-(h_lmt-v)*fact);  
			else if(v<0)
				*y1     =(int)(x2-(l_lmt-v)*fact);			
			
			*y2     = (int ) ( *y1 -  v *fact );		/*Modified by zyp.      change '+' to '-'*/	
		}				
	}
}



/* modified by CHI , where y2 is added for dial meter float  06.16 */

void	GetX_Coord_F (float h_lmt,float l_lmt,float v,float x1,float x2,float *y1,float *y2 )
{
	
	float   delt_data;
	float	fact;
	
	delt_data	= h_lmt - l_lmt ;
	
	/*if ( abs (delt_data) <= 0.00001 )*/
	if ( fabs (delt_data) <= 0.00001 )
	{
		printf ( "Float underflow!!!\n" );
		fact	= 0.0;
	}
	else
	{
		fact	= (float)(x1-x2)/delt_data;
	}
	
	if ( v < l_lmt )
		v	= l_lmt; 
	else if ( v > h_lmt )
		v	= h_lmt;
	
	if ( v < l_lmt || v > h_lmt )
	{
		printf ( "Paramter exceeds limit\n" );
	}
	else if ( v <= h_lmt && v >= l_lmt ) 
	{
		if ( l_lmt >= 0)
		{
			*y1 	= (float ) (x2 + ( v - l_lmt )*fact );
			*y2 	= x2; 				/*Modified by zyp.      change '+' to '-'*/
		}
		else if ( l_lmt < 0)
		{
			/* modified by wgp 99.6.16 ,then modified by zyp on 1999.8.17 to draw the PQ dot*/
			/*	*y1     = (int ) (x1- h_lmt*fact );   */
			
			if(v>=0)
				*y1	= (float)(x1-(h_lmt-v)*fact);  
			else if(v<0)
				*y1     =(float)(x2-(l_lmt-v)*fact);			
			
			*y2     = (float ) ( *y1 -  v *fact );		/*Modified by zyp.      change '+' to '-'*/	
		}				
	}
}




GBRCD	*GetActCtrlItemRcd (int crt_n, int x1,int y1,int x2,int y2, GDBUFF	*gdb )
{
	int	i;
	int	point_offset	=2;
	int	ctrl_act_found;
	int	xtmp, ytmp;
	int	width, height;
	GBRCD	*rcd;
	
	ctrl_act_found	=ITEM_TAG_OFF;
	
	if (( abs(x1-x2) <= point_offset || abs(y1-y2) <= point_offset ))
	{
		for (i=gdb->nextpos-1 ; i >= 0; i--)
		{
			rcd	=&gdb->buffer[i];
			if (zoomdb[crt_n].factor >= gdbase[crt_n].layer[rcd->bck.layer].min_visible &&
				zoomdb[crt_n].factor <= gdbase[crt_n].layer[rcd->bck.layer].max_visible )
			{
				if (rcd->lnk.act.ctrl_type != CTRL_REQ_NULL || 
					rcd->lnk.act.ctrl_tag == CTRL_TAG_ON)	
				{
					switch (rcd->bck.draw_type )
					{
					case	DrwT_TEXT:
					case	DrwT_LINE:
					case	DrwT_ARC:
					case	DrwT_RECTANGLE:
					case	DrwT_CIRCLE:
					case	DrwT_SYMBOL:
					case	DrwT_PLINE:
						CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height );
						if ( (x1 >= xtmp) && (x1 <= xtmp+width) &&
							(y1 >= ytmp) && (y1 <= ytmp+height ))
						{
							ctrl_act_found	=ITEM_TAG_ON;
						}
						break;
						
					default:
						printf ("OIX: GetActCtrlItemRcd: DrwType Error.\n" );
						break;
					}
				}
			}
			if ( ctrl_act_found == ITEM_TAG_ON) break;
		}
	}
	
	if ( ctrl_act_found == ITEM_TAG_ON) 
	{
		ctrl_act_rcd_found[crt_n] = gdb->buffer[i];
		printf("$$$$$$ bck.tag=%d\n",gdb->buffer[i].bck.tag);
		tag_rcd[crt_n] = &gdb->buffer[i];
		printf("111111$$$$$$ bck.tag=%d\n",gdb->buffer[i].bck.tag);
		return &(ctrl_act_rcd_found[crt_n]);
		/*		return	rcd;								*/
	}
	else
	{
#ifdef	DEBUG
		printf ( "OIX:  rcd is not found \n" );
#endif
		memset( &ctrl_act_rcd_found[crt_n], 0, sizeof(GBRCD) );
		tag_rcd[crt_n] = (GBRCD *)NULL;
		return	(GBRCD *)NULL;
	}
}

char* GetActCtrlItemDbname (int crt_n, int x1,int y1,int x2,int y2 )
{
	int	i;
	int	point_offset	=2;	
	int	xtmp, ytmp;
	int	width, height;
	GDBUFF	*layer;
	GBRCD	*rcd;
	layer = &gdbase[crt_n];
	if (( abs(x1-x2) <= point_offset || abs(y1-y2) <= point_offset ))
	{
		for (i=gdbase[crt_n].nextpos-1 ; i >= 0; i--)
		{
			rcd	=&layer->buffer[i];
            if (zoomdb[crt_n].factor >= gdbase[crt_n].layer[rcd->bck.layer].min_visible &&
				zoomdb[crt_n].factor <= gdbase[crt_n].layer[rcd->bck.layer].max_visible )
			{				
				switch (rcd->bck.draw_type )
				{
				case	DrwT_TEXT:
				case	DrwT_LINE:
				case	DrwT_ARC:
				case	DrwT_RECTANGLE:
				case	DrwT_CIRCLE:
				case	DrwT_SYMBOL:
				case	DrwT_GIF:
				case	DrwT_PLINE:                                           
					CheckRectCoords (rcd->bck.rect, &xtmp, &ytmp, &width, &height );						
					if ( (x1 >= xtmp) && (x1 <= xtmp+width) &&
						(y1 >= ytmp) && (y1 <= ytmp+height ))
					{
						if(rcd->lnk.dyn.dyn_tag == DYN_TAG_ON)
						{
							printf("111:x1=%d,rcd->lnk.dbname=%s\n",
								rcd->bck.rect.x1,rcd->lnk.dbname);
							return rcd->lnk.dbname;
						}
					}                                               
					break;
					
				default:
					printf ("OIX: GetActCtrlItemRcd: DrwType Error.\n" );
					return "";
					break;
				}				
			}			
		}
		
	}
	return   "" ;
}

void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height )
{
#ifdef  DEBUG
	printf(" in CheckRectCoords: x1=%d , x2=%d , y1=%d , y2=%d \n",rect.x1,rect.x2,rect.y1,rect.y2);
#endif
	*xtmp	= rect.x1;
	*width	= rect.x2 - rect.x1;
	if (*width < 0)
	{
		*width   = - *width;
		*xtmp   = rect.x1 - *width;
	}
	
	*ytmp   = rect.y1;
	*height	= rect.y2 - rect.y1;
	if ( *height < 0)
	{
		*height  = - *height;
		*ytmp   = rect.y1 - *height;
	}
	
}



/*************************************
gtk--ClearArea;
**************************************/

void	ClearArea (int crt_n,  GdkDrawable	*drawable,int x1,int y1,int width,int height,int expose_mode )
{
	static	BACKRCD	rcdtmp;  
	
	
	if ( (drawable == pixmap[crt_n]) || (drawable == (GdkDrawable *)sub_pixmap[crt_n]) || 
		(drawable == bakgrd_pixmap[crt_n])|| (drawable == (GdkDrawable *)view_pixmap[crt_n]))
	{
		if (drawable == view_pixmap[crt_n])
			rcdtmp.fg  =  ColorId3to4(BLUE1) ; 	
		else
			rcdtmp.fg	= ColorId3to4(canvas_bg_color); /*  update 160 to 32768 kind colors  */
		rcdtmp.rect.x1	= x1;
		rcdtmp.rect.y1	= y1;
		rcdtmp.rect.x2	= rcdtmp.rect.x1+  (int)(width/xscale[crt_n]);
		rcdtmp.rect.y2	= rcdtmp.rect.y1+  (int) (height/yscale[crt_n]);
		rcdtmp.fill.fill_type	= FILLED;
		rcdtmp.draw_type		= DrwT_RECTANGLE;
		
		DrawRcd ( crt_n,   drawable, info[crt_n].gc, &rcdtmp,FALSE );
	}
	else
	{	gdk_gc_set_exposures(info[crt_n].gc,expose_mode);
	gdk_window_clear_area (drawable, x1, y1, width, height) ;
	/* XClearArea( disp, drawable, x1, y1, width, height, expose_mode );  */
	}
	
}
























