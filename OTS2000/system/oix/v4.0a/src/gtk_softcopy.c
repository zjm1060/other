
/*************************************
2005.11.	updated by zyp by gtk.
have been SYNC  by hcl;
LAST UPATE : 2007.12.05 by hcl ;
*************************************/


#include	<stdio.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"


extern void	Redraw (GtkWidget*);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );

extern void	fwriteBckRcd (FILE	*fp, BACKRCD 	*rcd);
extern void	fwriteLnkRcd (FILE	*fp, LINKRCD	*rcd);
extern void	fwriteRcd (FILE	*fp, GBRCD	*rcd);

#if defined( WINDOWS_OS)
extern int   WinSaveJPGFile(HWND hWnd, char* filename,int print_mode) ;
#endif

static	int	nextpos;
static	int	dyn_nextpos;
void	CpGdbaseData (int crt_n, FILE	*fp );
void	fcpRcd (FILE    *fp,GBRCD rcd);
void	ProcessDynPartFile ( FILE	*fp, GBRCD	*rcd);

void	ProgSoftCopy (int crt_n )
{	
	char	sft_cp_fn[MAX_FILE_NAME_LEN*2]		= {""};
	char	sft_cp_fn_tmp[MAX_FILE_NAME_LEN*2]	= {""};
	char    tmp[MAX_FILE_NAME_LEN*2]	= {""};
	char	part_sft_cp_fn[MAX_FILE_NAME_LEN*2]	= {""};
	char	delt_part[10]				= {""};
	FILE	*fp;
	int	len, len_1, len_2;
	struct tm *sftcp_time;
	time_t t;
	long	tloc;
	char str_time[30];
	
	char hostname[20]; 
	char    command_buf_ps[MAX_FILE_NAME_LEN * 10 ]     = { "" };
	char    command_buf_ps1[MAX_FILE_NAME_LEN * 10 ]     = { "" };
	
//	int first_pos;
	char gifname[50];
	char jpgpath[100];
	char jpgpathname[100];
	
	
	char	*line ={"_"};
	
	char path[100];
	strcpy(path,PATH_SOFTCP_PIC);
	gethostname(hostname,sizeof(hostname));
	t	 = time (&tloc);
	sftcp_time = localtime (&t);
	
	sprintf(str_time_eg, "%04d%02d%02d",sftcp_time->tm_year + 1900,sftcp_time->tm_mon+1,sftcp_time->tm_mday);
	sprintf(str_time,"_%02d%02d%02d_",sftcp_time->tm_hour,sftcp_time->tm_min,sftcp_time->tm_sec);
	strcat(str_time_eg,str_time);
	
	if(strcmp(cur_dsp_name[crt_n],"")==0)
	{
		PopupMessageBox(main_w[crt_n],MSG_OPEN_DIS);
		return;
	}        
	strcpy (sft_cp_fn_tmp, strrchr(cur_dsp_name[crt_n], PATH_PIPE));	
	len_1 	= strlen(sft_cp_fn_tmp);
	
	strcpy (delt_part, strrchr(sft_cp_fn_tmp, '.'));	
	len_2 	= strlen(delt_part);
	len 	= len_1 - len_2;
	strncpy (sft_cp_fn, sft_cp_fn_tmp+1, len-1); 		
	
	strcpy(tmp,str_time_eg);
	strcat(tmp,sft_cp_fn);
	
	
	strcpy (part_sft_cp_fn, path);
	
	strcat(part_sft_cp_fn, tmp);
	strcat (part_sft_cp_fn, ".sfbin");
	
	printf("111:;in ProgSoftCopy: part_sft_cp_fn=%s,tmp=%s,sft_cp_fn=%s\n",part_sft_cp_fn,tmp,sft_cp_fn);
	/****************rm copy  *.sbfin file	function ****************/
	fp      = fopen (part_sft_cp_fn , "w+b"); 
	if (fp == NULL)
	{
		printf ("OIX:open file_name=%s error.\n", part_sft_cp_fn);
		return;
	}		
	
	CpGdbaseData ( crt_n, fp );
	/**/
	Redraw (canvas[crt_n] );
	/* free all allocated space and we're outta here. */
	fclose( fp);  
	
	/* added by zyp on 2006.10.11 to get the giffile*/   
	
	
#if defined( SUN_OS)
	strcpy(gifname,tmp);        
	/*       sprintf(command_buf_ps,"/usr/dt/bin/sdtimage -snapshot -screen -display %s:0.%d -out JPEG:/home/ems/h9000/his/sftcp/%s.jpg -f",
	hostname,crt_n,gifname);  -region 1280x1024+0+0 */   
	sprintf(jpgpathname, "JPEG:%sJPG\\%s.jpg",path,gifname);            
	sprintf(command_buf_ps,"/usr/dt/bin/sdtimage -snapshot -screen -display %s:0.%d  -out %s -f",hostname,crt_n, jpgpathname);       
	/*sprintf(command_buf_ps,"/usr/dt/bin/sdtimage/* -snapshot -region 1280x1024+0+0  -out JPEG:/home/ems/h9000/his/sftcp/%s.jpg -f", gifname);*/
	system ( command_buf_ps );
	
	printf("command_buf_ps=%s\n",command_buf_ps);
#elif defined( WINDOWS_OS)
	
	strcpy(gifname,tmp);        
	sprintf(jpgpathname, "%sJPG\\%s.jpg",path,gifname);    
	sprintf(jpgpath,"%sJPG",path);
	mkdir(jpgpath); 
	/*小系统生成JPG代码*/
	WinSaveJPGFile(HWND_DESKTOP, jpgpathname,SOFTCPY_MODE);
	
#endif
	
} 

void	CpGdbaseData (int crt_n, FILE	*fp )
{
	int     i,  len;
	long	offset;
	int     tmp_nextpos;
	
	offset =(long) sizeof (cur_dsp_name[crt_n]);
	len = sizeof (cur_dsp_name[crt_n]);
	fwrite ( cur_dsp_name[crt_n], len, 1, fp); 
	
	nextpos=  gdbase[crt_n].nextpos;
	
	fwrite (&nextpos, 4, 1, fp);
	
	dyn_nextpos = 0;
	
	for (i=0; i < nextpos; i++)	
	{
		fcpRcd (fp, gdbase[crt_n].buffer[i]);
	}
	
	fseek(fp, offset, 0);
	tmp_nextpos= dyn_nextpos;
	
#ifdef SUN_OS
	printf("in CpGdbaseData: before swap:: dyn_nextpos=%d \n",dyn_nextpos);
	Swap4Byte(&tmp_nextpos);
#endif
	fwrite (&tmp_nextpos, 4, 1, fp);
	fseek(fp, 0L, 2);	
}

void	fcpRcd (FILE    *fp,GBRCD rcd)
{
	if( (rcd.lnk.dyn.dyn_type !=0 && rcd.lnk.dyn.dyn_tag == DYN_TAG_ON)
		|| (rcd.lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd.lnk.cgrp.cgrp_id != 0) 
		/*|| (rcd.lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON && rcd.lnk.cgrp.cgrp_id != 0) */)
	{
		ProcessDynPartFile ( fp, &rcd);	
		fwriteRcd ( fp, &rcd);
		dyn_nextpos ++;
	}
}



void	ProcessDynPartFile ( FILE	*fp, GBRCD	*rcd)
{	
	//int	free_area[MAX_DYN_AREA_NUM];
	int	dyn_type;
	//int	len;
	float	rt_value;
	GBRCD	add_rcd;
	int	high, low;
	int	y1, y2, y, y0;
	int	delt_data;
	//short	pointer_loc;
	DMS_COMMON	dms_cmm;
	//int		data_type;
	char            FormatS[20];
	
	/*printf("in ProcessDynPartFile:	rcd->lnk.dbname=%s\n",rcd->lnk.dbname);*/
	if ( ReadEntryByNameStr (rcd->lnk.dbname, &dms_cmm ) == -1)
	{
		printf("OIX: in ProcessDynPartFile: %s is not found in database.\n", rcd->lnk.dbname);
		return;
	}
	
	dyn_type 	= rcd->lnk.dyn.dyn_type;
	rcd->lnk.dyn.dyn_type 	= 0;
	rcd->lnk.dyn.dyn_tag 	= DYN_TAG_OFF;		
	rcd->lnk.cgrp.tag 	= DYN_CGRP_TAG_OFF;
	rcd->lnk.cgrp.cgrp_id 	= 0;
	rcd->lnk.act.ctrl_tag	= CTRL_TAG_OFF;
	rcd->lnk.act.ctrl_type	= 0; 	
	
	switch( dyn_type)
	{
	case	DF_BAR_CHART:
		rt_value 	= dms_cmm.var.anlg.fValue;/*GetAnaValue ( &dms_cmm.entry.anlg.variable.value ); */	
		add_rcd 	= *rcd;
		add_rcd.bck.fill.fill_type 	= FILLED;
		
		high 	=(int)( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit);
		low 	= (int)(rcd->lnk.dyn.fmt_def.bar_chart.low_limit);
		delt_data 	= high - low ;
		y1 	= rcd->bck.rect.y1;
		y2 	= rcd->bck.rect.y2;
		if( rt_value <= high && rt_value >= low)
		{
			if( low >= 0)
			{
				y 	=(int)( y2+ (rt_value - low)*(y1-y2)/delt_data);
				add_rcd.bck.rect.y1 	= y;	
			}
			else if ( low < 0)
			{
				y0 	= y1- high*(y1-y2)/(high-low);
				y  	=(int)( y0 + rt_value * (y1-y2)/delt_data);
				add_rcd.bck.rect.y1 	= y0;
				add_rcd.bck.rect.y2 	= y;
			}
			
		}
		fwriteBckRcd (fp, &add_rcd.bck);
		fwriteLnkRcd (fp, &add_rcd.lnk);
		dyn_nextpos ++;
		break;	
		
	case	DF_NUMERIC:	
		rt_value 	= dms_cmm.var.anlg.fValue;/*GetAnaValue ( &dms_cmm.entry.anlg.variable.value ); */	
		
		sprintf (FormatS,"%%*.%df\0"  ,rcd->lnk.dyn.fmt_def.rpt_num.dec_pt);
		sprintf ( rcd->bck.data.text.text,(char*)FormatS, strlen( rcd->bck.data.text.text),  rt_value );
		/*sprintf ( rcd->bck.data.text.text,"%*%df\0", strlen(rcd->bck.data.text.text), rt_value);*/
		break;	
		
	default:
		break;
	}
	
}



