/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Cate : 2005.9.17 
	LAST UPDATA by hcl 2007.12.09
	SYNC by hcl 2007.12.09
     

---------------------------------------------------------------------*/



#include <stdio.h>


#include        "../inc/oix.h"

extern void	DrawRcd (int crt_n, GdkDrawable	*drawable, GdkGC *gc, BACKRCD *rcd ,BOOL isTrutype);
extern void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height );
extern void	freadBckRcd (FILE	*fp, BACKRCD 	*rcd);


/*	#define   DEBUG	  */ 

void	DrawSymbol (int crt_n,  GdkDrawable	*drawable,  GdkGC *gc, BACKRCD	*rec,int fb,BOOL isDYN, BOOL disable_flag )
{
	int	i, xtmp, ytmp, width, height;
	int	blink_color	= rec->fg;
	SYMBOL	sym;
	BACKRCD	rcd;
	
	rcd	= *rec;
	
	if (0)
	{
		if ( rcd.bg != BLACK)
			rcd.fg = rcd.bg;
		else
			rcd.fg	= canvas_bg_color;


		rcd.draw_type	= DrwT_RECTANGLE;
		rcd.fill.fill_type	= FILLED; 
		 
		DrawRcd ( crt_n,  drawable, gc, &rcd,FALSE);
	}   /*  end if  DYN  */	
	
	CheckRectCoords (rec->rect, &xtmp, &ytmp, &width, &height);   
	
#ifdef 	DEBUG
	printf(" 		return from CheckRectCoords\n");
	printf(" 		width=  %d,  height=  %d\n",width,height);
	printf(" 		draw_type=  %d,  tag=  %d\n",rec->draw_type,rec->tag);
	printf(" 		fill_type=  %d,  fg=  %d\n",rec->fill_type,rec->fg);
	printf(" 		line_width=  %d,  layer=  %d\n",rec->line_width,rec->layer);
	printf(" 		plcnt=  %d,  arc_type=  %d\n",rec->plcnt,rec->arc_type);
	printf(" 		ftset_id=  %d\n",rec->ftset_id);
#endif

	if( MAX_SYMBOL_CLASS <= rec->data.sym.class )               /* added by wzg 2003.5.6  */
	{
		printf("!!! Error: class=  %d", rec->data.sym.class);
		return;
	}
	if( MAX_SYMBOLS <= rec->data.sym.name )
	{
		printf("!!! Error: name=  %d",rec->data.sym.name);
		return;
	}	
		
	sym	= symdb[rec->data.sym.class].symbol[rec->data.sym.name];
	for (i=0; i<sym.nextpos; i++)
	{
		rcd	=sym.objects[i];

		switch	(rcd.draw_type)
		{
		case	DrwT_ARC:
			rcd.data.arc.x		=rcd.data.arc.x*width/SYMBOL_PIX+xtmp;
			rcd.data.arc.y		=rcd.data.arc.y*height/SYMBOL_PIX+ytmp;
			rcd.data.arc.width	=rcd.data.arc.width*width/SYMBOL_PIX;
			rcd.data.arc.height	=rcd.data.arc.height*height/SYMBOL_PIX;
			break;

		case	DrwT_PLINE:
			{
				int	k;

				for (k=0; k < rcd.plcnt; k++)
				{
					rcd.data.pl[k].x 	=rcd.data.pl[k].x*width/SYMBOL_PIX+xtmp;
					rcd.data.pl[k].y	=rcd.data.pl[k].y*height/SYMBOL_PIX+ytmp;
				}
			}
			break;

		case	DrwT_LINE:
		case	DrwT_CIRCLE:
		case	DrwT_RECTANGLE:
			rcd.rect.x1	=rcd.rect.x1*width/SYMBOL_PIX+xtmp;
			rcd.rect.y1	=rcd.rect.y1*height/SYMBOL_PIX+ytmp;
			rcd.rect.x2	=rcd.rect.x2*width/SYMBOL_PIX+xtmp;
			rcd.rect.y2	=rcd.rect.y2*height/SYMBOL_PIX+ytmp;
			
			break;

		default: 
			break;

		}

/*		added by wdk 99.05.03						*/
		if ( fb == FOREGROUND && rcd.fg == BLACK4 )
		{
			rcd.fg	= blink_color;
		}

		if(disable_flag)
			rcd.fg= rec->fg;
		DrawRcd ( crt_n, drawable, gc, &rcd,FALSE);
	}	
}


void	LoadSymbolLibrary ()
{
#ifdef SUN_OS
        int tmp;
#endif
	int	len;
	int	id_cl, id_sym, id_obj;
	FILE	*fp;
	char version[6] ;
	char	symbol_part_name[100]; 
    strcpy(symbol_part_name,FILE_SYMBOL_LIBRARY);
	fp = fopen(symbol_part_name, "rb");
	if (fp == NULL)
	{
		 printf (" OIX:open SYMBOL_LIBRARY.DBIN file error.\n");
		 exit (-1);
	 
	}



  

   fread(version,1,6,fp);
   if (strcmp("v4.0a",version)!=0)    /*4.0a *.dbin file data formate  */
   		rewind(fp);


	len	=4;
	fread (&symdb_class_cnt, len, 1, fp);
	

#ifdef SUN_OS
printf("SUN_OS is DEFINED !!!\n");
        Swap4Byte(&symdb_class_cnt);
#endif	

printf("symdb_class_cnt=%d\n",symdb_class_cnt);

	for (id_cl=0; id_cl<symdb_class_cnt; id_cl++)
	{
		len	=4;
		fread (&symdb[id_cl].nextpos, len, 1, fp);
#ifdef SUN_OS
		Swap4Byte(&symdb[id_cl].nextpos);
#endif

		
		len	=sizeof(symdb[id_cl].name);
		fread (symdb[id_cl].name, len, 1, fp);
                if ( len > SYMBOL_CLASS_NAME_LEN )    /* add by zqh, 00,3,24		*/
                {
                        printf(" SYMBOL_CLASS_NAME is too long: %s\n", symdb[id_cl].name );
                        symdb[id_cl].name[SYMBOL_CLASS_NAME_LEN-1] = 0;
                }
		for (id_sym=0; id_sym<symdb[id_cl].nextpos; id_sym++)
		{
			len	=4;
			fread (&symdb[id_cl].symbol[id_sym].nextpos, len, 1, fp);
#ifdef SUN_OS
        		Swap4Byte(&symdb[id_cl].symbol[id_sym].nextpos);
#endif
			for (id_obj=0; id_obj<symdb[id_cl].symbol[id_sym].nextpos; id_obj++)
			{
				freadBckRcd (fp, &symdb[id_cl].symbol[id_sym].objects[id_obj]);
			}
		}
	}

	fclose (fp);
#ifdef SUN_OS
	printf ("OIX: LoadSymbolLibrary ok\n");	
#endif

}




