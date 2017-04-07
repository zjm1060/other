
/*	Modification History							**

 *	updated by zyp on 2006.3.25 for v4.0db
 *	
 *	It provides *.dbin, *.rbin and *.sbin read-write routins.
    
 *	  WINDOWS HAVE SYN TO UNIX by hcl  2007.12.07 ;
 */

/*#define   DEBUG  */   
#include <stdio.h>
/*
*/

#include        "../inc/oix.h"
#ifdef SUN_OS
#include <sys/stat.h>
#endif

#include	"../inc/gtk_fill_bitmap.h"

FILE    *OpenFileX();
 
void	fwriteBckRcd (FILE	*fp, BACKRCD 	*rcd);
void	fwriteLnkRcd (FILE	*fp, LINKRCD	*rcd);
void	freadBckRcd (FILE	*fp, BACKRCD 	*rcd);
void	fwriteLnkRcd (FILE	*fp, LINKRCD	*rcd);
void	freadLnkRcd (FILE	*fp, LINKRCD 	*rcd);
void	freadSubBckRcd (int crt_n,FILE	*fp, BACKRCD 	*rcd,int sub_win_n);
extern void	CheckRectCoords (XRECT rect, int	*xtmp, int	*ytmp, int	*width, int	*height );



int sub_dsp_id=0;

#ifdef	SUN_OS 
extern  int fault_redisplay_on[MAX_CRT_NUM];
extern char fault_redisplay_dsp_name[MAX_FULL_FILE_NAME_LEN];
extern int fault_redisplay_crt_num;
#define SUB_WIN_X1 1280
#define SUB_WIN_Y1 1024
#endif

#ifdef WINDOWS_OS
/*extern*/  int fault_redisplay_on[MAX_CRT_NUM];
/*extern*/ char fault_redisplay_dsp_name[MAX_FULL_FILE_NAME_LEN];
/*extern*/ int fault_redisplay_crt_num;
#define SUB_WIN_X1 900
#define SUB_WIN_Y1 800
#endif

extern BOOL SoftCopyFlag;
 
void	fwriteRcd (FILE	*fp, GBRCD	*rcd)
{

	fwriteBckRcd (fp, &rcd->bck);
	fwriteLnkRcd (fp, &rcd->lnk);
}


void	fwriteBckRcd (FILE	*fp, BACKRCD 	*rcd)
{
	int	len;
#ifdef	SUN_OS
	BACKRCD	 sun_rcd;
	int     i;
	
	len	=sizeof (rcd->rect.x1);
	sun_rcd = *rcd;
	
#ifdef DEBUG
	printf("in write: before convert: x1=%d, y1=%d,x2=%d,y2=%d,draw_type=%d\n", sun_rcd.rect.x1,sun_rcd.rect.y1,
		sun_rcd.rect.x2,sun_rcd.rect.y2,sun_rcd.draw_type);
#endif
	Swap4Byte(&sun_rcd.rect.x1);
	Swap4Byte(&sun_rcd.rect.y1);
	Swap4Byte(&sun_rcd.rect.x2);
	Swap4Byte(&sun_rcd.rect.y2);

	fwrite (&sun_rcd.rect.x1, len, 1, fp);
	fwrite (&sun_rcd.rect.y1, len, 1, fp);
	fwrite (&sun_rcd.rect.x2, len, 1, fp);
	fwrite (&sun_rcd.rect.y2, len, 1, fp);
	
	len	=sizeof (rcd->draw_type);
	Swap2Byte(&sun_rcd.draw_type);
	Swap2Byte(&sun_rcd.fill.fill_type);
	Swap2Byte(&sun_rcd.fg);
	Swap2Byte(&sun_rcd.bg);
	Swap2Byte(&sun_rcd.line_width);
	Swap2Byte(&sun_rcd.layer);
	Swap2Byte(&sun_rcd.plcnt);
	Swap2Byte(&sun_rcd.arc_type);
	Swap2Byte(&sun_rcd.ftset_id);
	
#ifdef DEBUG
	printf(" in write: afetr convert: x1=%d, y1=%d,x2=%d,y2=%d, draw_type=%d\n", sun_rcd.rect.x1,sun_rcd.rect.y1,
		sun_rcd.rect.x2,sun_rcd.rect.y2, sun_rcd.draw_type);	
#endif
	fwrite (&sun_rcd.draw_type, len, 1, fp);
	fwrite (&sun_rcd.fill.fill_type, len, 1, fp);
	fwrite (&sun_rcd.fg, len, 1, fp);
	fwrite (&sun_rcd.bg, len, 1, fp);
	fwrite (&sun_rcd.line_width, len, 1, fp);
	fwrite (&sun_rcd.layer, len, 1, fp);
	fwrite (&sun_rcd.plcnt, len, 1, fp);
	fwrite (&sun_rcd.arc_type, len, 1, fp);
	fwrite (&sun_rcd.ftset_id, len, 1, fp);

	switch(rcd->draw_type)
	{
		case DrwT_PLINE:
		for(i=0;i<MAXPLCNT;i++)
		{
			Swap2Byte(&sun_rcd.data.pl[i].x );
			Swap2Byte(&sun_rcd.data.pl[i].y );

		}	
		break;

		case DrwT_ARC:
			Swap2Byte( &sun_rcd.data.arc.x );
			Swap2Byte( &sun_rcd.data.arc.y );
			Swap2Byte( &sun_rcd.data.arc.width );
			Swap2Byte( &sun_rcd.data.arc.height );
			Swap2Byte( &sun_rcd.data.arc.angle1 );
			Swap2Byte( &sun_rcd.data.arc.angle2 );

			break;

		case DrwT_SYMBOL:
/*printf(" in write beforconvert: sym_class=%d, sym_name=%d\n", sun_rcd.data.sym.class, sun_rcd.data.sym.name);*/

			Swap4Byte( &sun_rcd.data.sym.class );
			Swap4Byte( &sun_rcd.data.sym.name );
/*printf(" in write: after sym_class=%d, sym_name=%d\n", sun_rcd.data.sym.class, sun_rcd.data.sym.name);
*/
			break;
		default:
			break;
	}
	len     =sizeof (sun_rcd.data);
	fwrite (&sun_rcd.data, len, 1, fp);	
	
#else
	len	=sizeof (rcd->rect.x1);
	fwrite (&rcd->rect.x1, len, 1, fp);
	fwrite (&rcd->rect.y1, len, 1, fp);
	fwrite (&rcd->rect.x2, len, 1, fp);
	fwrite (&rcd->rect.y2, len, 1, fp);

	len	=sizeof (rcd->draw_type);
	fwrite (&rcd->draw_type, len, 1, fp);
	fwrite (&rcd->fill.fill_type, len, 1, fp);
	fwrite (&rcd->fg, len, 1, fp);
	fwrite (&rcd->bg, len, 1, fp);
	fwrite (&rcd->line_width, len, 1, fp);
	fwrite (&rcd->layer, len, 1, fp);
	fwrite (&rcd->plcnt, len, 1, fp);
	fwrite (&rcd->arc_type, len, 1, fp);
	fwrite (&rcd->ftset_id, len, 1, fp);
	
	len     =sizeof (rcd->data);
	fwrite (&rcd->data, len, 1, fp);
	
#endif
           
	rcd->tag	=0;
}

void	fwriteLnkRcd (FILE	*fp, LINKRCD	*rcd)
{
	int	len;

#ifdef	SUN_OS
	LINKRCD	sun_rcd;

	sun_rcd = *rcd;
	len	=sizeof (rcd->act.ctrl_type);
	
	Swap4Byte(&sun_rcd.act.ctrl_type);
	Swap4Byte(&sun_rcd.act.ctrl_tag);
	
	fwrite (&sun_rcd.act.ctrl_type, len, 1, fp);
	fwrite (&sun_rcd.act.ctrl_tag, len, 1, fp);
	
	switch ( rcd->act.ctrl_type )
	{
		case	CTRL_REQ_PROG:
			Swap4Byte( &sun_rcd.act.fmt_def.prog.id);
			break;
			
		case	CTRL_REQ_MENU:
			Swap4Byte( &sun_rcd.act.fmt_def.menu.id);
			
			break;
		default:
			break;

	}
	len 	= sizeof (rcd->act.fmt_def.free_area);
/*!!!! modify by wzg 2003.7.17*/
/*	fwrite (rcd->act.fmt_def.free_area, len, 1, fp);*/
	fwrite (sun_rcd.act.fmt_def.free_area, len, 1, fp);
	

	len	=sizeof (rcd->dyn.dyn_type);
	Swap4Byte(&sun_rcd.dyn.dyn_type);
	Swap4Byte(&sun_rcd.dyn.dyn_tag);
	Swap4Byte(&sun_rcd.dyn.dio_type);
	Swap4Byte(&sun_rcd.dyn.f_by_off);
	Swap4Byte(&sun_rcd.dyn.qi_loc);
	Swap4Byte(&sun_rcd.dyn.tagi_loc);
	Swap4Byte(&sun_rcd.dyn.data_class);
	
	
	fwrite (&sun_rcd.dyn.dyn_type, len, 1, fp);
	fwrite (&sun_rcd.dyn.dyn_tag, len, 1, fp);
	fwrite (&sun_rcd.dyn.dio_type, len, 1, fp);
	fwrite (&sun_rcd.dyn.f_by_off, len, 1, fp);
	fwrite (&sun_rcd.dyn.qi_loc, len, 1, fp);
	fwrite (&sun_rcd.dyn.tagi_loc, len, 1, fp);
	fwrite (&sun_rcd.dyn.data_class,  len, 1, fp);

	len	=sizeof (rcd->dbname);
       /* if(strcmp(sun_rcd.dbname,"")!=0)
            printf("sun_rcd.dbname=%s,len=%d\n",sun_rcd.dbname,len);*/
	fwrite (sun_rcd.dbname, len, 1, fp);
        
	 len     =sizeof (rcd->state);
	fwrite (&sun_rcd.state, len, 1, fp);
	
	switch( rcd->dyn.dyn_type )
	{
		case	DF_DYN_COLOR:
			break;
			
		case	DF_SYMBOL_GRP:
			Swap4Byte(&sun_rcd.dyn.fmt_def.sym.sym_grp);
			break;
			
		case	DF_BAR_CHART:                
			Swap4Byte( &sun_rcd.dyn.fmt_def.bar_chart.hi_limit );
			Swap4Byte( &sun_rcd.dyn.fmt_def.bar_chart.low_limit );
			Swap4Byte( &sun_rcd.dyn.fmt_def.bar_chart.hi_limit_2 );
			Swap4Byte( &sun_rcd.dyn.fmt_def.bar_chart.low_limit_2 );
			Swap4Byte( &sun_rcd.dyn.fmt_def.bar_chart.direction );
			break;
		
		case	DF_HIST_BAR:
			Swap4Byte( &sun_rcd.dyn.fmt_def.hist_bar.hi_limit );
			Swap4Byte( &sun_rcd.dyn.fmt_def.hist_bar.low_limit );
			Swap2Byte( &sun_rcd.dyn.fmt_def.hist_bar.hour_id );
			Swap4Byte( &sun_rcd.dyn.fmt_def.hist_bar.hi_limit_2 );
			Swap4Byte( &sun_rcd.dyn.fmt_def.hist_bar.low_limit_2 );
			break;
				
		
		case	DF_LINE_PLOT:
		case	DF_PQ_CHART:
			Swap4Byte( &sun_rcd.dyn.fmt_def.curve.hi_limit );
			Swap4Byte( &sun_rcd.dyn.fmt_def.curve.low_limit );
			Swap4Byte( &sun_rcd.dyn.fmt_def.curve.hi_limit_2 );
			Swap4Byte( &sun_rcd.dyn.fmt_def.curve.low_limit_2 );
			Swap4Byte( &sun_rcd.dyn.fmt_def.curve.direction );
			break;
			
		case	DF_ANA_STATUS:
			Swap4Byte( &sun_rcd.dyn.fmt_def.ana_status.sym_grp);
			Swap2Byte ( &sun_rcd.dyn.fmt_def.ana_status.limit_type);
			break;
			
		/*case	DF_CASCAD_CLR:
			Swap4Byte( &sun_rcd.dyn.fmt_def.cascad_clr.tag);
			Swap4Byte( &sun_rcd.dyn.fmt_def.cascad_clr.cgrp_id);
			Swap2Byte( &sun_rcd.dyn.fmt_def.cascad_clr.req_type);
			break;*/

                case    DF_CURVE_PT:
                        Swap4Byte( &sun_rcd.dyn.fmt_def.curve_point.tag);
			Swap4Byte( &sun_rcd.dyn.fmt_def.curve_point.point_id);
			break;
                        
                case    DF_CUR_LOAD:
                        Swap4Byte( &sun_rcd.dyn.fmt_def.load_curve.tag);
						Swap4Byte( &sun_rcd.dyn.fmt_def.load_curve.point_id);
                        Swap4Byte( &sun_rcd.dyn.fmt_def.load_curve.hi_limit);
                        Swap4Byte( &sun_rcd.dyn.fmt_def.load_curve.low_limit);
			break;
                        
                 case    DF_TEMP_CHAR:
                        Swap4Byte( &sun_rcd.dyn.fmt_def.temp_chara.tag);
			Swap4Byte( &sun_rcd.dyn.fmt_def.temp_chara.temp_chara_id);       
                        break;
                        
		case	DF_NUMERIC:
			Swap4Byte( &sun_rcd.dyn.fmt_def.num.hi_limit );
			Swap4Byte( &sun_rcd.dyn.fmt_def.num.low_limit );
			Swap4Byte( &sun_rcd.dyn.fmt_def.num.length );
			Swap4Byte( &sun_rcd.dyn.fmt_def.num.sign_typ );
			Swap4Byte( &sun_rcd.dyn.fmt_def.num.dec_pt );
			break;	
			
		case	DF_STRING_GRP:		
			Swap4Byte( &sun_rcd.dyn.fmt_def.str.str_grp);
			break;
			
			
	/*	case 	DF_ANAIMATE_CLR:
			Swap4Byte( &sun_rcd.dyn.fmt_def.animate_clr.tag);
			Swap4Byte( &sun_rcd.dyn.fmt_def.animate_clr.cgrp_id);
			Swap2Byte( &sun_rcd.dyn.fmt_def.animate_clr.update_time);	
			break;*/
			
		case	DF_RPT_NUMERIC:	
			Swap2Byte( &sun_rcd.dyn.fmt_def.rpt_num.length );
			Swap2Byte( &sun_rcd.dyn.fmt_def.rpt_num.sign_type );
			Swap2Byte( &sun_rcd.dyn.fmt_def.rpt_num.dec_pt);
			Swap2Byte( &sun_rcd.dyn.fmt_def.rpt_num.req_type );
			Swap2Byte( &sun_rcd.dyn.fmt_def.rpt_num.hour_id );
			Swap2Byte( &sun_rcd.dyn.fmt_def.rpt_num.date_id );
			Swap2Byte( &sun_rcd.dyn.fmt_def.rpt_num.month_id );
			Swap2Byte( &sun_rcd.dyn.fmt_def.rpt_num.special_type );
			break;
		default:
			break;
	}
	len	= sizeof (rcd->dyn.fmt_def.free_area);      /*  ????? must swap  before fwrite !!! */
/*!!!! modify by wzg 2003.7.17*/
/*	fwrite (rcd->dyn.fmt_def.free_area, len, 1, fp);*/
	fwrite (sun_rcd.dyn.fmt_def.free_area, len, 1, fp);
	
	len	=sizeof (rcd->cgrp.cgrp_id);
	Swap4Byte(&sun_rcd.cgrp.cgrp_id);
	Swap4Byte(&sun_rcd.cgrp.tag);
	
	fwrite (&sun_rcd.cgrp.cgrp_id, len, 1, fp);
	fwrite (&sun_rcd.cgrp.tag,	len, 1, fp);

#else
	len     =sizeof (rcd->act.ctrl_type);
	fwrite (&rcd->act.ctrl_type, len, 1, fp);
	fwrite (&rcd->act.ctrl_tag, len, 1, fp);
	len     = sizeof (rcd->act.fmt_def.free_area);
	fwrite (rcd->act.fmt_def.free_area, len, 1, fp);

	len     =sizeof (rcd->dyn.dyn_type);
	fwrite (&rcd->dyn.dyn_type, len, 1, fp);
	fwrite (&rcd->dyn.dyn_tag, len, 1, fp);
	fwrite (&rcd->dyn.dio_type, len, 1, fp);
	fwrite (&rcd->dyn.f_by_off, len, 1, fp);
	fwrite (&rcd->dyn.qi_loc, len, 1, fp);
	fwrite (&rcd->dyn.tagi_loc, len, 1, fp);
	fwrite (&rcd->dyn.data_class, len, 1, fp);

	len     =sizeof (rcd->dbname);
	fwrite (rcd->dbname, len, 1, fp);

	 len     =sizeof (rcd->state); /* added by zyp on 2007.10*/
	fwrite (&rcd->state, len, 1, fp);

	len     = sizeof (rcd->dyn.fmt_def.free_area);
	fwrite (rcd->dyn.fmt_def.free_area, len, 1, fp);

	len     =sizeof (rcd->cgrp.cgrp_id);
	fwrite (&rcd->cgrp.cgrp_id, len, 1, fp);
	fwrite (&rcd->cgrp.tag,  len, 1, fp);
#endif

}


void	freadRcd (FILE	*fp, GBRCD 	*rcd)
{

	freadBckRcd (fp, &rcd->bck);
	freadLnkRcd (fp, &rcd->lnk);
}

void	freadSubRcd (int crt_n,FILE	*fp, GBRCD 	*rcd,int sub_win_n)
{

	freadSubBckRcd (crt_n,fp, &rcd->bck,sub_win_n);
	freadLnkRcd (fp, &rcd->lnk);
}


void	freadBckRcd (FILE	*fp, BACKRCD 	*rcd)
{
	int	len;
#ifdef	SUN_OS
	int i;
#endif	

        len     =sizeof (rcd->rect.x1);
        fread (&rcd->rect.x1, len, 1, fp);
        fread (&rcd->rect.y1, len, 1, fp);
        fread (&rcd->rect.x2, len, 1, fp);
        fread (&rcd->rect.y2, len, 1, fp);
       
        len     =sizeof (rcd->draw_type);
        fread (&rcd->draw_type, len, 1, fp);
        fread (&rcd->fill.fill_type, len, 1, fp);


        fread (&rcd->fg, len, 1, fp);
        fread (&rcd->bg, len, 1, fp);
        fread (&rcd->line_width, len, 1, fp);
        fread (&rcd->layer, len, 1, fp);
        fread (&rcd->plcnt, len, 1, fp);
        fread (&rcd->arc_type, len, 1, fp);
        fread (&rcd->ftset_id, len, 1, fp);


#ifdef 	SUN_OS
 	Swap4Byte(&rcd->rect.x1);
	Swap4Byte(&rcd->rect.y1);
	Swap4Byte(&rcd->rect.x2);
	Swap4Byte(&rcd->rect.y2);
      
	Swap2Byte(&rcd->draw_type);
	//modified by wzg 20101224 如果定义fill_type为char类型，
	//则不需要交换，并且兼容IPM原来的short类型；
	//如果定义fill_type为short类型，则需要交换
#ifndef FILL_TYPE_ONE_CHAR
	Swap2Byte(&rcd->fill.fill_type);
#endif
	Swap2Byte(&rcd->fg);
	Swap2Byte(&rcd->bg);
	Swap2Byte(&rcd->line_width);
	Swap2Byte(&rcd->layer);
	Swap2Byte(&rcd->plcnt);
	Swap2Byte(&rcd->arc_type);
	Swap2Byte(&rcd->ftset_id);   
	
#endif

	if (rcd->fill.fill_type >= MAX_FILL_PATTERN)
	{
		printf("OIX: rcd->fill.fill_type ERROR: %d\n" , rcd->fill.fill_type );
		rcd->fill.fill_type= 0;
	}

/*printf("in read: after convert: x1=%d, y1=%d,x2=%d,y2=%d,draw_type=%d,fill_type=%d,ftset_id=%d\n", rcd->rect.x1,rcd->rect.y1,
		rcd->rect.x2,rcd->rect.y2,rcd->draw_type,rcd->fill_type,rcd->ftset_id);*/
	len     =sizeof (rcd->data);
        fread (&rcd->data, len, 1, fp);
        
#ifdef SUN_OS
	switch(rcd->draw_type)
	{
		case DrwT_PLINE:
		for(i=0;i<MAXPLCNT;i++)
		{
			Swap2Byte(&rcd->data.pl[i].x );
			Swap2Byte(&rcd->data.pl[i].y );
		}	
		break;

		case DrwT_ARC:
			Swap2Byte( &rcd->data.arc.x );
			Swap2Byte( &rcd->data.arc.y );
			Swap2Byte( &rcd->data.arc.width );
			Swap2Byte( &rcd->data.arc.height );
			Swap2Byte( &rcd->data.arc.angle1 );
			Swap2Byte( &rcd->data.arc.angle2 );
/*printf("arc.x=%d,y=%d,width=%d,height=%d,angle1=%d,angle2=%d\n",rcd->data.arc.x,rcd->data.arc.y,rcd->data.arc.width,rcd->data.arc.height,rcd->data.arc.angle1,rcd->data.arc.angle2);*/
		break;

		case DrwT_SYMBOL:
/*printf("111:before  class=%d,rcd->data.sym.name=%d\n",rcd->data.sym.class,rcd->data.sym.name);*/
			Swap4Byte( &rcd->data.sym.class );
			Swap4Byte( &rcd->data.sym.name );
/*printf("111:after  class=%d,rcd->data.sym.name=%d\n",rcd->data.sym.class,rcd->data.sym.name);*/
		break;

		default:		
		break;
	}
#endif       
 	rcd->tag	=0;
}

void	freadSubBckRcd (int crt_n,FILE	*fp, BACKRCD 	*rcd,int sub_win_n)
{
	int	len;
#ifdef	SUN_OS
	int i;
#endif	

        len     =sizeof (rcd->rect.x1);
        fread (&rcd->rect.x1, len, 1, fp);
        fread (&rcd->rect.y1, len, 1, fp);
        fread (&rcd->rect.x2, len, 1, fp);
        fread (&rcd->rect.y2, len, 1, fp);
       
        len     =sizeof (rcd->draw_type);
        fread (&rcd->draw_type, len, 1, fp);
        fread (&rcd->fill.fill_type, len, 1, fp);
        fread (&rcd->fg, len, 1, fp);
        fread (&rcd->bg, len, 1, fp);
        fread (&rcd->line_width, len, 1, fp);
        fread (&rcd->layer, len, 1, fp);
        fread (&rcd->plcnt, len, 1, fp);
        fread (&rcd->arc_type, len, 1, fp);
        fread (&rcd->ftset_id, len, 1, fp);
 

#ifdef 	SUN_OS
 	Swap4Byte(&rcd->rect.x1);
	Swap4Byte(&rcd->rect.y1);
	Swap4Byte(&rcd->rect.x2);
	Swap4Byte(&rcd->rect.y2);
       /* added by zyp on 200607*/
       /* if(sub_dsp_id==SUB_DSP_FILE && rcd->rect.x1>0 &&rcd->rect.y1>0)*/
        
        /* ended 200607*/
	Swap2Byte(&rcd->draw_type);
	//modified by wzg 20101224 如果定义fill_type为char类型，
	//则不需要交换，并且兼容IPM原来的short类型；
	//如果定义fill_type为short类型，则需要交换
#ifndef FILL_TYPE_ONE_CHAR
	Swap2Byte(&rcd->fill.fill_type);
#endif
	Swap2Byte(&rcd->fg);
	Swap2Byte(&rcd->bg);
	Swap2Byte(&rcd->line_width);
	Swap2Byte(&rcd->layer);
	Swap2Byte(&rcd->plcnt);
	Swap2Byte(&rcd->arc_type);
	Swap2Byte(&rcd->ftset_id);   
	
#endif
 
	if (rcd->fill.fill_type >= MAX_FILL_PATTERN)
	{
		printf("OIX: rcd->fill.fill_type ERROR: %d\n" , rcd->fill.fill_type );
		rcd->fill.fill_type= 0;
	}

    sub_win[sub_win_n][crt_n].x1 = MIN(sub_win[sub_win_n][crt_n].x1,rcd->rect.x1);
    sub_win[sub_win_n][crt_n].y1 = MIN(sub_win[sub_win_n][crt_n].y1,rcd->rect.y1);
    sub_win[sub_win_n][crt_n].x2 = MAX(sub_win[sub_win_n][crt_n].x2,rcd->rect.x2);
    sub_win[sub_win_n][crt_n].y2 = MAX(sub_win[sub_win_n][crt_n].y2,rcd->rect.y2);	
            /*printf("!!!sub_win.x1=%d\n",sub_win[sub_win_n].x1);*/
 
/*printf("in read: after convert: x1=%d, y1=%d,x2=%d,y2=%d,draw_type=%d,fill_type=%d,ftset_id=%d\n", rcd->rect.x1,rcd->rect.y1,
		rcd->rect.x2,rcd->rect.y2,rcd->draw_type,rcd->fill_type,rcd->ftset_id);*/
	len     =sizeof (rcd->data);
    fread (&rcd->data, len, 1, fp);
        
#ifdef SUN_OS
	switch(rcd->draw_type)
	{
		case DrwT_PLINE:
		for(i=0;i<MAXPLCNT;i++)
		{
			Swap2Byte(&rcd->data.pl[i].x );
			Swap2Byte(&rcd->data.pl[i].y );
		}	
		break;

		case DrwT_ARC:
			Swap2Byte( &rcd->data.arc.x );
			Swap2Byte( &rcd->data.arc.y );
			Swap2Byte( &rcd->data.arc.width );
			Swap2Byte( &rcd->data.arc.height );
			Swap2Byte( &rcd->data.arc.angle1 );
			Swap2Byte( &rcd->data.arc.angle2 );
/*printf("arc.x=%d,y=%d,width=%d,height=%d,angle1=%d,angle2=%d\n",rcd->data.arc.x,rcd->data.arc.y,rcd->data.arc.width,rcd->data.arc.height,rcd->data.arc.angle1,rcd->data.arc.angle2);*/
		break;

		case DrwT_SYMBOL:
/*printf("111:before  class=%d,rcd->data.sym.name=%d\n",rcd->data.sym.class,rcd->data.sym.name);*/
			Swap4Byte( &rcd->data.sym.class );
			Swap4Byte( &rcd->data.sym.name );
/*printf("111:after  class=%d,rcd->data.sym.name=%d\n",rcd->data.sym.class,rcd->data.sym.name);*/
		break;

		default:		
		break;
	}
#endif       
 	rcd->tag	=0;
}


void	freadLnkRcd (FILE	*fp, LINKRCD 	*rcd)
{
#ifdef SUN_OS
	int	i;
#endif
	int	len;
    DMS_COMMON dms_cmm;
	int DataType;

	len     =sizeof (rcd->act.ctrl_type);
	fread (&rcd->act.ctrl_type, len, 1, fp);
	fread (&rcd->act.ctrl_tag, len, 1, fp);
	len     = sizeof (rcd->act.fmt_def.free_area);
	fread (rcd->act.fmt_def.free_area, len, 1, fp);

	len     =sizeof (rcd->dyn.dyn_type);
	fread (&rcd->dyn.dyn_type, len, 1, fp);
	fread (&rcd->dyn.dyn_tag, len, 1, fp);
	fread (&rcd->dyn.dio_type, len, 1, fp);
	fread (&rcd->dyn.f_by_off, len, 1, fp);
	fread (&rcd->dyn.qi_loc, len, 1, fp);
	fread (&rcd->dyn.tagi_loc, len, 1, fp);
	fread (&rcd->dyn.data_class, len, 1, fp);

	len     =sizeof (rcd->dbname);
       /* if(strcmp(rcd->dbname,"")!=0)
            printf("in readlnk: dbname=%s,len=%d\n",rcd->dbname,len);*/
	fread (rcd->dbname, len, 1, fp);
	//added by yyp 2010.11.1
	if(strcmp(rcd->dbname,"")!=0)
	{
		//		printf("in readlnk: dbname=%s,len=%d\n",rcd->dbname,len);
		if(ReadEntryByNameStr(rcd->dbname,&dms_cmm)!=-1)
		{
			DataType = dms_cmm.point.data_type;
			if( DataType==SOE_TYPE || DataType==POL_TYPE || DataType==SYSMSG_TYPE 
				|| DataType==DEVOBJ_TYPE || DataType==OUTS_TYPE)
			{
				strcpy(shm_ots_addr->data_point.yxname[shm_ots_addr->data_point.yxnum],rcd->dbname);
				shm_ots_addr->data_point.yxnum ++;				
			}
			else if(DataType==ANA_TYPE || DataType==PARAM_TYPE || DataType==IMP_TYPE)
			{
				strcpy(shm_ots_addr->data_point.ycname[shm_ots_addr->data_point.ycnum],rcd->dbname);
				shm_ots_addr->data_point.ycnum ++;					
			}
		}
	}
    
    len     =sizeof (rcd->state);
	fread (&rcd->state, len, 1, fp);

	if(fault_redisplay_on[fault_redisplay_crt_num]==1&&strcmp(fault_redisplay_dsp_name,cur_dsp_name[fault_redisplay_crt_num])==0)
	{
            /*printf("fault_redisplay_crt_num=%d,fault_redisplay_dsp_name=%s,cur_dsp_name=%s\n",
                fault_redisplay_crt_num,fault_redisplay_dsp_name,cur_dsp_name[fault_redisplay_crt_num]);*/
		if(strcmp(rcd->dbname,"")!=0)
		{
			if(strstr(rcd->dbname,"ANA")!=NULL)
			{
                            if(ReadEntryByNameStr(rcd->dbname,&dms_cmm)!=-1)
                                {
				strcpy(shm_hdb_addr->data_revert.yc_data[shm_hdb_addr->data_revert.ycnum].logicalname,rcd->dbname);
				shm_hdb_addr->data_revert.ycnum ++;	
                            }
				
			}		
			else 
			{
                            if(ReadEntryByNameStr(rcd->dbname,&dms_cmm)!=-1)
                                {
				strcpy(shm_hdb_addr->data_revert.yx_data[shm_hdb_addr->data_revert.yxnum].logicalname,rcd->dbname);
				shm_hdb_addr->data_revert.yxnum ++;
                               /* printf("db_name=%s,yxnum=%d\n",rcd->dbname,shm_hdb_addr->data_revert.yxnum);*/
                            }
			}		
		}		
	}
	
	len     = sizeof (rcd->dyn.fmt_def.free_area);
	fread (rcd->dyn.fmt_def.free_area, len, 1, fp);

	len     =sizeof (rcd->cgrp.cgrp_id);
	fread (&rcd->cgrp.cgrp_id, len, 1, fp);
	fread (&rcd->cgrp.tag,  len, 1, fp);
	
#ifdef SUN_OS	
	Swap4Byte( &rcd->act.ctrl_type );	

	Swap4Byte( &rcd->act.ctrl_tag );

	switch ( rcd->act.ctrl_type )
	{
		case	CTRL_REQ_PROG:
			Swap4Byte( &rcd->act.fmt_def.prog.id);
			break;
			
		case	CTRL_REQ_MENU:
			Swap4Byte( &rcd->act.fmt_def.menu.id);			
			break;
		default:
			break;

	}
	
	Swap4Byte( &rcd->dyn.dyn_type );
	Swap4Byte( &rcd->dyn.dyn_tag );
	Swap4Byte( &rcd->dyn.dio_type );
	Swap4Byte( &rcd->dyn.f_by_off );
	Swap4Byte( &rcd->dyn.qi_loc );
	Swap4Byte( &rcd->dyn.tagi_loc );
	Swap4Byte( &rcd->dyn.data_class );

	switch( rcd->dyn.dyn_type )
	{
		case	DF_DYN_COLOR:
			break;
                       
                case	32:  /*  DF_CURVE_PT: chichi  */
                        Swap4Byte(&rcd->dyn.fmt_def.curve_point.point_id);
                        Swap4Byte(&rcd->dyn.fmt_def.curve_point.tag);
			break;
                case    DF_TEMP_CHAR:
                        Swap4Byte(&rcd->dyn.fmt_def.temp_chara.temp_chara_id);
                        Swap4Byte(&rcd->dyn.fmt_def.temp_chara.tag);
                        Swap4Byte(&rcd->dyn.fmt_def.temp_chara.length);  /* chichi*/
                        break;
		case	 DF_CUR_LOAD: /*    chichi  */
                        Swap4Byte(&rcd->dyn.fmt_def.load_curve.point_id);
                        Swap4Byte(&rcd->dyn.fmt_def.load_curve.tag);
                        Swap4Byte(&rcd->dyn.fmt_def.load_curve.hi_limit);
                        Swap4Byte(&rcd->dyn.fmt_def.load_curve.low_limit);
			break;	
		case	DF_SYMBOL_GRP:
			Swap4Byte(&rcd->dyn.fmt_def.sym.sym_grp);
			break;
		case	 DF_VIBRA_AREA: /*    34 chichi  */
                        Swap4Byte( &rcd->dyn.fmt_def.vibra_area.hi_limit );
			Swap4Byte( &rcd->dyn.fmt_def.vibra_area.low_limit );
			Swap4Byte( &rcd->dyn.fmt_def.vibra_area.hi_limit_2 );
			Swap4Byte( &rcd->dyn.fmt_def.vibra_area.low_limit_2 );
                        Swap4Byte( &rcd->dyn.fmt_def.vibra_area.low_limit_2 );
                        Swap4Byte( &rcd->dyn.fmt_def.vibra_area.vibra_serial );
			break;	
                        
		case	DF_BAR_CHART:
                case    DF_DIAL_METER:
			Swap4Byte( &rcd->dyn.fmt_def.bar_chart.hi_limit );
			Swap4Byte( &rcd->dyn.fmt_def.bar_chart.low_limit );
			Swap4Byte( &rcd->dyn.fmt_def.bar_chart.hi_limit_2 );
			Swap4Byte( &rcd->dyn.fmt_def.bar_chart.low_limit_2 );
			Swap4Byte( &rcd->dyn.fmt_def.bar_chart.direction );
			break;
		
		case	DF_HIST_BAR:
			Swap4Byte( &rcd->dyn.fmt_def.hist_bar.hi_limit );
			Swap4Byte( &rcd->dyn.fmt_def.hist_bar.low_limit );
			Swap2Byte( &rcd->dyn.fmt_def.hist_bar.hour_id );
			Swap4Byte( &rcd->dyn.fmt_def.hist_bar.hi_limit_2 );
			Swap4Byte( &rcd->dyn.fmt_def.hist_bar.low_limit_2 );
			break;
				
		
		case	DF_LINE_PLOT:
		case	DF_PQ_CHART:
			Swap4Byte( &rcd->dyn.fmt_def.curve.hi_limit );
			Swap4Byte( &rcd->dyn.fmt_def.curve.low_limit );
			Swap4Byte( &rcd->dyn.fmt_def.curve.hi_limit_2 );
			Swap4Byte( &rcd->dyn.fmt_def.curve.low_limit_2 );
			Swap4Byte( &rcd->dyn.fmt_def.curve.direction );
			break;
			
		case	DF_ANA_STATUS:
			Swap4Byte( &rcd->dyn.fmt_def.ana_status.sym_grp);
			Swap2Byte ( &rcd->dyn.fmt_def.ana_status.limit_type);
			break;
			
/*		case	DF_CASCAD_CLR:
			Swap4Byte( &rcd->dyn.fmt_def.cascad_clr.tag);
			Swap4Byte( &rcd->dyn.fmt_def.cascad_clr.cgrp_id);
			Swap2Byte( &rcd->dyn.fmt_def.cascad_clr.req_type);
			break;
*/
			
/*		case	DF_NUMERIC:   merge with DF_RPT_NUMERIC  by chichi
			Swap4Byte( &rcd->dyn.fmt_def.num.hi_limit );
			Swap4Byte( &rcd->dyn.fmt_def.num.low_limit );
			Swap4Byte( &rcd->dyn.fmt_def.num.length );
			Swap4Byte( &rcd->dyn.fmt_def.num.sign_typ );
			Swap4Byte( &rcd->dyn.fmt_def.num.dec_pt );
			break;	
*/			
		case	DF_STRING_GRP:		
			Swap4Byte( &rcd->dyn.fmt_def.str.str_grp);
			break;
			
			
/*		case 	DF_ANAIMATE_CLR:
			Swap4Byte( &rcd->dyn.fmt_def.animate_clr.tag);
			Swap4Byte( &rcd->dyn.fmt_def.animate_clr.cgrp_id);
			Swap2Byte( &rcd->dyn.fmt_def.animate_clr.update_time);	
			break;
*/		case	DF_NUMERIC:	
		case	DF_RPT_NUMERIC:	
case     DF_IMP_SET:
			Swap2Byte( &rcd->dyn.fmt_def.rpt_num.length );
			Swap2Byte( &rcd->dyn.fmt_def.rpt_num.sign_type );
			Swap2Byte( &rcd->dyn.fmt_def.rpt_num.dec_pt);
			Swap2Byte( &rcd->dyn.fmt_def.rpt_num.req_type );
			Swap2Byte( &rcd->dyn.fmt_def.rpt_num.hour_id );
			Swap2Byte( &rcd->dyn.fmt_def.rpt_num.date_id );
			Swap2Byte( &rcd->dyn.fmt_def.rpt_num.month_id );
			Swap2Byte( &rcd->dyn.fmt_def.rpt_num.special_type );
			break;
		default:
			break;
	}
	
	Swap4Byte( &rcd->cgrp.cgrp_id );
	Swap4Byte( &rcd->cgrp.tag);

#endif
}

void	ReadGdbaseData ( GDBUFF	*gdb, FILE	*fp)
{
	int	i, layer, nextpos, tmp_cnt=0;
	GBRCD	tmp;
	char	version[6];

	fread(version,1,6,fp);

	if(strcmp(version,"v4.0a")==0)
		printf("this is new file\n");
	else
	{
		printf("this is old file\n");
		return;
	}
	fread (&gdb->nextpos, 4, 1, fp);
#ifdef SUN_OS
	Swap4Byte(&gdb->nextpos);
#endif

#ifdef DEBUG
	printf("in ReadGdbaseData: nextpos=%d\n", gdb->nextpos);
#endif
	if ( gdb->nextpos > MAXOBJECTS )
	{
		printf ( "DSP exceeds MAXOBJECTS! nextpos= %d\n", gdb->nextpos );
		tmp_cnt	= gdb->nextpos;
		gdb->nextpos	= MAXOBJECTS-1;
	}
	
	nextpos = gdb->nextpos;

	if(fault_redisplay_on[fault_redisplay_crt_num]==1)
	{	
		shm_hdb_addr->data_revert.ycnum =0;		
		shm_hdb_addr->data_revert.yxnum =0;	
	}	
	shm_ots_addr->data_point.ycnum =0;
	shm_ots_addr->data_point.yxnum =0;
	for (i=0; i < nextpos; i++)	
		freadRcd (fp, &gdb->buffer[i]);
	
	for (i=nextpos; i < tmp_cnt; i++)	
		freadRcd (fp, &tmp );		
/*printf("after freadRcd::shm_hdb_addr->data_revert.queryflag=%d\n",shm_hdb_addr->data_revert.queryflag);*/
	for (layer=0; layer < MAXLAYERS; layer++)
	{
		fread (&gdb->layer[layer].min_visible, 4 ,1, fp);
		fread (&gdb->layer[layer].max_visible, 4 ,1, fp);
#ifdef SUN_OS
		Swap4Byte(&gdb->layer[layer].min_visible);
		Swap4Byte(&gdb->layer[layer].max_visible);
#endif
	}
}

void 	OpenRptDspFile (int crt_n, char    *filename)
{
        FILE    *fp;

        if (!filename || !*filename)
        {
/*                if (filename) XtFree (filename);
*/
                return;
        }

        fp=     OpenFileX(filename);
        if ( fp == NULL)
        {
                printf ("OIX: open file error.\n"); return;
        }
        ReadGdbaseData ( &gdbase[crt_n], fp);
        fclose (fp);
}

FILE    *OpenFileX (char    *full_name)
{
#ifdef SUN_OS
	struct  stat    statb;
#endif
	FILE *fp;


#ifdef  DEBUG
        printf ("OIX/OpenFile: full_name is: %s \n", full_name );
#endif
        /*add by hcl, change SoftCopyFlag*/
	/*if filename is sdbin make softcopy button useless */
	
	if(strstr(full_name,".sfbin")!=NULL)
		{
			
			SoftCopyFlag=TRUE;
		}
		else
		{
			SoftCopyFlag=FALSE;		
		}
		
		
		
		
		/* make sure the file is a regular text file and open it */
#ifdef SUN_OS
	if (stat(full_name, &statb) == -1 ||
		(statb.st_mode & S_IFMT) != S_IFREG ||
		!(fp = fopen(full_name, "rb")))
	{

		printf ("OIX: in OpenFileX: file not opened.%s\n", full_name);
		if ((statb.st_mode & S_IFMT) == S_IFREG)
			perror (full_name);              /* send to stderr why we can't read it*/
		else
/*			sprintf (stderr, "in OpenFileX: %s: not a regular file\n", full_name);
*/
			printf ("in OpenFileX: %s: not a regular file\n", full_name);
		return  NULL;
	}
	else
	{
		return (fp);
	}
#endif

#ifdef WINDOWS_OS
		if (/*stat(full_name, &statb) == -1 ||
		(statb.st_mode & S_IFMT) != S_IFREG ||*/
		!(fp = fopen(full_name, "rb")))
	{

		printf ("OIX: in OpenFile: file not opened.%s\n", full_name);
/*if ((statb.st_mode & S_IFMT) == S_IFREG)
			perror (full_name);              
		else*/
			printf ("in OpenFile: %s: not a regular file\n", full_name);
		return  NULL;
	}
	else
	{
		return (fp);
	}
#endif
	
}


void	ReadSubGdbaseData (int crt_n, GCOND_BUFF	*gcon, FILE		*fp,int sub_win_no)
{
	int	i, nextpos;
	char	version[6];

	fread(version,1,6,fp);

	if(strcmp(version,"v4.0a")==0)
		printf("this is new file\n");
	else
	{
		printf("this is old file\n");
		return;
	}
	fread (&gcon->nextpos, 4, 1, fp);
#ifdef SUN_OS
        Swap4Byte(&gcon->nextpos);
	printf("SUN_OS:in ReadSubGdbaseData: nextpos=%d\n", gcon->nextpos);
#endif
	
	if ( gcon->nextpos > MAX_OPR_COND_OBJECTS )
	{
		gcon->nextpos	= MAX_OPR_COND_OBJECTS;
		printf ( "SubDspBuffer Overflow!!!\n");
	}
		nextpos = gcon->nextpos;
		sub_win_width[sub_win_no][crt_n]=0;
        sub_win_height[sub_win_no][crt_n]=0;
        sub_win[sub_win_no][crt_n].x1=SUB_WIN_X1;		/*1280*/
        sub_win[sub_win_no][crt_n].y1=SUB_WIN_Y1;		/*1024*/
        sub_win[sub_win_no][crt_n].x2=0;
        sub_win[sub_win_no][crt_n].y2=0;
        sub_dsp_id = SUB_DSP_FILE;
	for (i=0; i < nextpos; i++)
	{
		freadSubRcd (crt_n,fp, &gcon->buffer[i],sub_win_no);  
               /* printf("##ctrl_type=%d,ctrl_tag=%d\n",gcon->buffer[i].lnk.act.ctrl_type,gcon->buffer[i].lnk.act.ctrl_tag);*/
	}
     /*   printf("###x1=%d ,y1=%d,x2=%d,y2=%d\n",sub_win[sub_win_no][crt_n].x1,sub_win[sub_win_no][crt_n].y1,sub_win[sub_win_no][crt_n].x2,sub_win[sub_win_no][crt_n].y2);
      */ 
	CheckRectCoords (sub_win[sub_win_no][crt_n], &sub_win[sub_win_no][crt_n].x1, &sub_win[sub_win_no][crt_n].y1,&sub_win_width[sub_win_no][crt_n],&sub_win_height[sub_win_no][crt_n] );
        
}




