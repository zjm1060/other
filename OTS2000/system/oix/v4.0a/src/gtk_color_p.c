


/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2005.9.15
     updated by zyp on 2006.3 for v4.0db
	Modified format of  fuctions define
    HAVE  SYNC   BY CHI .2008.04
---------------------------------------------------------------------*/

typedef unsigned int    COLORREF_X;



typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;


#define RGBX(r,g,b)          ((COLORREF_X)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

#define	COLOR_DEF_MODULE
#include "../inc/oix.h"

int	ReadColor(int crt_n);
extern void	Redraw (GtkWidget *w);
extern char *_toUtf8EX(char *c);

int   GetColor(short id , GdkColor *clr )
{
       /* UCHAR red ,green,blue ;*/
        clr->red    = ((id&0x1f)<<3) *256;
        clr->green = (((id>>5)&0x1f)<<3)*256; 
        clr->blue = (((id>>10)&0x1f)<<3 ) *256; 
        
        clr->pixel   =  (gulong) (clr->red * 65535 +  clr->green *256 + clr->blue);
        
         if (gdk_color_alloc(gtk_widget_get_colormap(canvas[0]),clr) != TRUE)
            {           
                        g_message("allocate color (%d) : is failure!!!", id);
		 	return -1 ;
            }
        return   0 ;
}


int    gdk_rgb(int red  ,int green, int  blue , GdkColor *clr  )
{
        
        clr->red    = red *256 ;
        clr->green =  green *256 ; 
        clr->blue  =   blue *256 ; 
        
        clr->pixel   =  (gulong) (clr->red * 65535 +  clr->green *256 + clr->blue);
        
         if (gdk_color_alloc(gtk_widget_get_colormap(canvas[0]),clr) != TRUE)
            {           
                        g_message("OIX:allocate color failure!!!");
		 	return -1 ;
            }
        return  0 ;
}




void    SetRcdColor (int crt_n,   GdkGC	*gc, BACKRCD 	*rcd)	
{
	GdkColor  bg ,fg;
	if (GetColor(rcd->bg , &bg )== -1) return ;
	if (GetColor(rcd->fg , &fg )== -1) return ;
   
	gdk_gc_set_background ( gc, &bg);
 	gdk_gc_set_foreground ( gc, &fg);
        

}



void    SetRcdColor_old (int crt_n,   GdkGC	*gc, BACKRCD 	*rcd)	
{
	gdk_gc_set_background ( gc, &pixels[crt_n][rcd->bg]);
 	gdk_gc_set_foreground ( gc, &pixels[crt_n][rcd->fg]);  
}

int	ReadColor(int crt_n )
{
	
	/* test by zyp  on 2005.7.27 */	 
	
	int depth,j;
	static char *class[]={"StaticGray","GrayScale","StaticColor","PseudoColor","TrueColor","DirectColor"};
	GdkVisual *my_visual;
	GdkVisualType visual_type;
	/*Status result;*/
	/*GdkColor *clr ;*/
	 
	j = 5 ;
	visual_type = gdk_visual_get_best_type() ;
	my_visual = gdk_visual_get_system()	;
	
	depth =  my_visual->depth ;
	
	printf("OIX:depth of cur scr is %d\n",depth);
	
	printf("OIXfound a %s class visual at cur depth\n",class[visual_type]);	


	
	for(j=0;j<MAXCOLORS;j++)
	{					
		pixels[crt_n][j].red  	= rgb_value[j].rred	;
		pixels[crt_n][j].green  = rgb_value[j].ggreen	;
		pixels[crt_n][j].blue  	= rgb_value[j].bblue	;
		
		pixels[crt_n][j].pixel	=  (gulong) (pixels[crt_n][j].red * 65535 +  pixels[crt_n][j].green *256 + pixels[crt_n][j].blue);
        	
		 if (gdk_color_alloc(gtk_widget_get_colormap(canvas[0]),&pixels[crt_n][j]) != TRUE)
		 	{g_message("allocate color (%d) : is failure!!!" , j);
		 	return -1 ;}
	}

	return 0 ;   /*  we success..... */
}


/*wzg20081022*/
int GetColorIdFromGdkColor(GdkColor clr)
{
	return ((clr.red>>11) | ((clr.green>>11)<<5) | ((clr.blue>>11)<<10)) + MAXCOLORS;
}

void	SetCanvasBgColor ( int crt_n  )
{
	GtkWidget *colorseldlg = NULL;
	GtkColorSelection *colorsel;
	GdkColor color;
	gint response;
	static GdkColor	canvas_bg_color_real;		
	gchar *pConvert=0;		
	/* 创建颜色选择对话框 */
	colorseldlg = gtk_color_selection_dialog_new (pConvert=_toUtf8EX(OIX_CSTR_BACK_COR_SEL));//"OIX_CSTR_BACK_COR_SEL" 背景颜色选择
	G_SAFE_FREE(pConvert);//dzy1017
	if(colorseldlg==NULL)
		return;
	
	/* 获取颜色选择构件 */
	colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (colorseldlg)->colorsel);
	if(colorsel==NULL)
	{
		gtk_widget_destroy (colorseldlg);
		return;
	}
	
	if(canvas_bg_color_real.pixel==0)
		canvas_bg_color_real= pixels[crt_n][canvas_bg_color];

	gtk_color_selection_set_current_color(colorsel, &canvas_bg_color_real);
	gtk_color_selection_set_has_palette(colorsel, TRUE);
	
	/* 显示对话框 */
	response = gtk_dialog_run(GTK_DIALOG (colorseldlg));
	if (response == GTK_RESPONSE_OK)
	{
		gtk_color_selection_get_current_color(colorsel, &color);
		canvas_bg_color_real= color;
//		printf("nred= %d, green= %d, blue= %d", color.red/256, color.green/256, color.blue/256);
		canvas_bg_color= GetColorIdFromGdkColor(color);
		gdk_window_set_background(canvas[crt_n]->window, &color);
		Redraw(canvas[crt_n]);
	}
	gtk_widget_destroy(colorseldlg);

/*
	switch	(canvas_bg_color)
	{
	case	BLACK:
		canvas_bg_color	= LEM_CHFN4;
		break;

	case	LEM_CHFN4:
		canvas_bg_color	= BLUE4;
		break;

	case	BLUE4:
		canvas_bg_color	= BLACK2;
		break;

	case	BLACK2:
		canvas_bg_color	= WHITE;
		break;

	case	WHITE:
		canvas_bg_color	= BLACK;
		break;

	default:
		canvas_bg_color	= BLACK2;
		break;
	}
	
	gdk_window_set_background ( canvas[crt_n]->window, &pixels[crt_n][canvas_bg_color]); 
	Redraw (canvas[crt_n]);
*/
}

int GetColorId(COLORREF_X rgb)
{
	return  ((rgb&0xF8)>>3) | ((rgb&0xF800)>>6) | ((rgb&0xF80000)>>9);
}

/*wzg20081022*/
int ColorId3to4(int id3)
{
	if(id3<MAXCOLORS)
		return GetColorId(RGBX(rgb_value[id3].rred, rgb_value[id3].ggreen, rgb_value[id3].bblue));
	else
		return id3-MAXCOLORS;
}









