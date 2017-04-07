/************************************************************************* 
* File:   gtk_draw_image.c
* Author:  chihai  long 
* 2007.11.23 日  与WINDOWS版本 同步 by chi ;
* Created on 2006年6月16日, 上午10:11
Windows Have  SYN to  UNIX  by chi 07.11.30 ;
*************************************************************************/



/********************************************************************************************************		
*	history : The  old version (v3.0a) display a 8-bits color picture of .gif			*
*	update  : The  latest version display a 24 true color picture of (.tif )TIFF .Here what		*
*		must be declare is the TIFF file must be 24 ,namely bitspersample is 8 and 		*	
*		samplesperpixel is 3  ;									*
*	author  : Writed by   chi.hl  2005.08.05							*
*		 											*
*													*
*********************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <gdk/gdk.h>
#define		GIF_DEF
#define	GIF_NORMAL	0
#define	GIF_ERR		1

#include	"../inc/oix.h"


extern void	DrawRcd (int crt_n, GdkDrawable	*drawable, GdkGC *gc, BACKRCD *rcd ,BOOL isTrutype);
extern int ColorId3to4(int id3); 
extern void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height );
extern int	XtoSX (int crt_n,int x);
extern int	YtoSY (int crt_n,int y);
extern void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n) ;
extern int	LoadGIF (int crt_n, char *fname ,GdkPixbuf **pic ,GdkRectangle *rc );
int load_pixbuf_from_file   (const gchar *filename, GdkPixbuf **pic,GdkRectangle *rc );





/*#define		min(a,b)		(((a) < (b)) ? (a) : (b)) */
#define		BYTEN			4

/*******************************************/

int   width,height, *raster;

int i,j ;
static int byteOrder,rshift,gshift,bshift;

int mask_bits,mask_width,mask_height;
GtkWidget *window,*image, *vbox;
GdkImage *gimage;
GdkVisual *visual;
GdkBitmap *mask; 

/*******************************************/
//extern void  gdk_draw_pixbuf();
void GifInit (int crt_n )
{
/*	expand	= 1;  	

  visual = gtk_widget_get_visual(main_w[crt_n]);
  byteOrder = visual->byte_order;
  gshift =8 ;
  
	if(byteOrder == LSBFirst){
	rshift = 0;   
	bshift =16;      
	}else{
	rshift =16 ;
	bshift =0;
}*/
}

void DrawTagImage(int crt_n,   GdkDrawable *drawable, GdkGC	*gc, int x,int y ,char *filename )
{
	GdkRectangle rc ;
	GdkPixbuf *PixImage = NULL;
	BACKRCD	 tmp ; 
	char FullFileName[80]  = "" ;
	int rowstride ; 
	if ( LoadGIF ( crt_n, filename , &PixImage,&rc ) == GIF_ERR ) 
	{
        tmp.rect.x1	=  x ;
        tmp.rect.y1	= y  ;
        tmp.rect.x2	=  x + 20 ;
        tmp.rect.y2	=  y + 20 ;
		tmp.draw_type = DrwT_RECTANGLE;
		tmp.fill.fill_type = UNFILLED;
		tmp.line_width =1 ;
		tmp.fg =  ColorId3to4(8);  /* GREEN */
		DrawRcd(crt_n,drawable,gc,&tmp,FALSE); 
		sprintf(FullFileName, "%s: not found!!!", filename) ;
		windk_draw_text( drawable, font_set[0], gc,x,y, 
			FullFileName,strlen( FullFileName),crt_n); 		
		return;      /*draw a rect frame if pic not exits  */
	}
	rowstride = gdk_pixbuf_get_rowstride(PixImage);
	
	gdk_draw_pixbuf( drawable, gc,
		PixImage,0,0,x, y, rc.width ,  rc.height ,
		GDK_RGB_DITHER_NORMAL, 0 , 0 ); 
	
	zoomdb[crt_n].zoom_lock = 1; 
	gdk_pixbuf_unref(PixImage);   
	PixImage = NULL ;  
	return  ;
    
}
static GdkPixbuf* ResizeGif (GdkPixbuf* src,  int     w,  int     h,
		   int     new_w,     int     new_h)
{

	GdkPixbuf *dest;
	GdkPixbuf *tmp;
	int size; 
	
	if (src == NULL)
		return NULL;
	if (0&&w != h)
    {     
		size = MAX (w, h);
		tmp = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, size, size);
		if (0&&tmp != NULL)
		{
			gdk_pixbuf_fill (tmp, 0);
			gdk_pixbuf_copy_area (src, 0, 0, w, h,
				tmp,
				0   /* (size - w) / 2 */, 0/*(size - h) / 2 */   );
			g_object_unref (src);
			src = tmp;
		}
    }
	if (w != new_w || h != new_h)
    {
		dest = gdk_pixbuf_scale_simple (src, new_w, new_h, GDK_INTERP_BILINEAR /*GDK_INTERP_BILINEAR*/);
		g_object_unref (G_OBJECT (src));
    }
	else
    {
		dest = src;
		
    }
	return dest;
}





void DrawGif (int crt_n,   GdkDrawable *drawable, GdkGC	*gc, BACKRCD	*rcd )
{
	int width, height, xtmp, ytmp;
	GdkPixbuf *PixImage,*destImage ;
	GdkRectangle rc ;
	int rowstride ; 
	
	BACKRCD	tmp;
    char FullFileName[80]  = "" ;
	CheckRectCoords ( rcd->rect, &xtmp, &ytmp, &width, &height );
	tmp	= *rcd;
    PixImage = NULL; 
	
	
	xtmp = 	XtoSX ( crt_n,  xtmp) ;
	ytmp =  YtoSY ( crt_n,  ytmp) ;
	tmp.rect.x1	=    xtmp     ;
    tmp.rect.y1	=   ytmp        ;
    tmp.rect.x2	=   xtmp+width ;
    tmp.rect.y2	=   ytmp+height ;    
	
	sprintf(FullFileName, OIX_PIC_PATH);     /*  chi */
	
	
	strcat(FullFileName,tmp.data.fname); 
	if ( LoadGIF ( crt_n, tmp.data.fname, &PixImage,&rc ) == GIF_ERR ) 
	{
		tmp.draw_type = DrwT_RECTANGLE;
		tmp.fill.fill_type  = UNFILLED;
		tmp.line_width =1 ;
		tmp.fg =  ColorId3to4( 8 );  /* GREEN */
		DrawRcd(crt_n,drawable,gc,&tmp,FALSE); 
		strcat(FullFileName,":not found!!!") ;
		
		windk_draw_text( drawable, font_set[0], gc, xtmp +10,ytmp +20, 
			FullFileName,strlen( FullFileName),crt_n); 		
		return;      /*draw a rect frame if pic not exits  */
	}
	rowstride = gdk_pixbuf_get_rowstride(PixImage);
    width =(int)(width *xscale[crt_n] *zoomdb[crt_n].factor)  /*abs(tmp.rect.x2 -tmp.rect.x1) */;
	height=(int)(height *yscale[crt_n] *zoomdb[crt_n].factor) /*abs(tmp.rect.y2 -tmp.rect.y1) */;
    destImage =  ResizeGif ( PixImage,rc.width ,rc.height, width , height );  /* modified by  chi 07.29*/
	
	gdk_draw_pixbuf( drawable, gc,
destImage,0,0, (int)(tmp.rect.x1*xscale[crt_n]), (int)(tmp.rect.y1*yscale[crt_n]),   MIN((int)(STND_WIDTH *xscale[crt_n] *zoomdb[crt_n].factor),width)   ,  MIN((int)(STND_HEIGHT *yscale[crt_n] *zoomdb[crt_n].factor),height)   ,
		GDK_RGB_DITHER_NORMAL, 0 , 0 ); 
    zoomdb[crt_n].zoom_lock = 0; 
	gdk_pixbuf_unref(PixImage);  
	gdk_pixbuf_unref(destImage); 
	destImage = PixImage = NULL ;  
	return  ;
}




int	LoadGIF (int crt_n, char *fname ,GdkPixbuf **pic ,GdkRectangle *rc )
{
	 	 
	char FullFileName[80]  = "" ,  Name[80] = "" ;
	sprintf(FullFileName, OIX_PIC_PATH);     /*  chi */      
	strcat(FullFileName,fname);     	
	return  load_pixbuf_from_file (FullFileName,pic, rc );
	
	
}
#ifdef BUFSIZE1
#undef BUFSIZE1
#endif 
#define BUFSIZE1 1024
int load_pixbuf_from_file   (const gchar *filename, GdkPixbuf **pic,GdkRectangle *rc )
{
	GError *error = NULL;
	GdkPixbufLoader *loader;
	FILE *file;
	guchar buf[BUFSIZE1];
	int num_read;
	
	error = NULL;
	if (filename == NULL)
    {
		printf("LoadGif: %s file not found.\n",filename);
		return (GIF_ERR);              
    }
	
	/* *pic = gdk_pixbuf_new_from_file(filename,  &error); */
    /*************************************************************************/
		
	file = fopen (filename, "rb");
	if (!file) {
		g_message ("Could not open the file\n");
		return (GIF_ERR);
	}	
	loader = gdk_pixbuf_loader_new ();	
	do {
		num_read = fread (buf, 1, BUFSIZE1, file);
		if (num_read < BUFSIZE1)
			if (ferror (file)) {
				printf ("OIX::Error when reading the file\n");
				return (GIF_ERR);
			}
			if (!gdk_pixbuf_loader_write (loader, buf, num_read, &error)) 
			{
				GtkWidget *dialog;			
				dialog = gtk_message_dialog_new (GTK_WINDOW (window),
					GTK_DIALOG_DESTROY_WITH_PARENT,	GTK_MESSAGE_ERROR,
					GTK_BUTTONS_CLOSE,	"Failed to load image: %s",
					error->message);
				printf ("\nFailed to load image: %s \n " , error->message )  ; 			
				g_error_free (error);				
				g_signal_connect (dialog, "response",
					G_CALLBACK (gtk_widget_destroy), NULL);		
				gtk_widget_show (dialog);			  			
				return GIF_ERR; /* error */
		}	
	} while (num_read > 0);
	
	fclose (file);
	gdk_pixbuf_loader_close (loader, NULL);
	
	*pic = gdk_pixbuf_loader_get_pixbuf (loader);
	if (!*pic) {
		g_message ("Could not create the pixbuf\n");
		return (GIF_ERR);
	}	
	/*	gdk_pixbuf_ref (*pic);  make sure we keep a reference to it */
//	gtk_object_destroy (GTK_OBJECT (loader));
	
    /***************************************************************************/
	
	if (*pic == NULL)
    {
		
		g_message("\n OIX:  GTK   MISSING IMAGE  \n"  );
		g_error_free(error);
		return (GIF_ERR);  
    }
    g_object_ref(*pic);
	
    rc->width = gdk_pixbuf_get_width(*pic);
    rc->height = gdk_pixbuf_get_height(*pic);
	
    /* g_object_unref (pic);  */
	
	return (GIF_NORMAL);
	
	
}

