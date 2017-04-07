/*******************************************
2007.12.06 : have been SYNC  by hcl; 
******************************************/

#include	<sys/types.h>
#include 	<sys/stat.h>
#include	<stdio.h>
 
#include	<gtk/gtk.h>
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"


#define    DEBUG  

static	int	nextpos;
static	int	dyn_nextpos;
static	int	real_bkgdpict_nextpos;

void	ReadBkgdPictData (int crt_n, FILE	*fp);
void	ReadFrgdPictData (int crt_n, FILE	*fp );
extern FILE	*OpenFileX();
extern BOOL SoftCopyFlag;/*true softcopy button useless,false softcopy button usefull*/
extern void PopFileSelectionDlg(GtkWidget *parent ,int crt_n,char *title,char *dir);
extern void	freadBckRcd (FILE	*fp, BACKRCD 	*rcd);
extern void	freadLnkRcd (FILE	*fp, LINKRCD 	*rcd);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );

void FetchSoftCopy (int crt_n )
{
	file_mode[crt_n]	= SFTCP_FILE;	
	PopFileSelectionDlg( main_w[crt_n] ,crt_n,_CS_(OIX_STR_SFTCOPY_FILE_SELECT_BOX),FILE_SOFTCP_PIC);
}


void	ReadSftCpData (int crt_n, FILE	*fp)
{	
	int	len;
	FILE	*bkgd_fp;
	char	sft_bkgd_file[MAX_FULL_FILE_NAME_LEN];

	len = sizeof(cur_dsp_name[crt_n]);
        
	fread ( sft_bkgd_file, len, 1, fp);
        printf("sft_bkgd_file=%s\n",sft_bkgd_file);
	bkgd_fp = OpenFileX( sft_bkgd_file); 
    /*add by hcl, change SoftCopyFlag*/
	/*if filename is sdbin make softcopy button useless */			
	SoftCopyFlag=TRUE;

	if ( bkgd_fp == NULL)
            {
                printf("***bkgd_fp == NULL\n");
		return;
        }
		
	real_bkgdpict_nextpos =0;
	ReadBkgdPictData ( crt_n, bkgd_fp);
	ReadFrgdPictData ( crt_n, fp );
	fclose ( bkgd_fp );	
}		

void	ReadBkgdPictData (int crt_n, FILE	*fp)
{
	int	i, layer;
	GBRCD   rcd;
	char	version[6];

	fread(version,1,6,fp);/* added by zyp on 2006.5.8 */

	fread (&gdbase[crt_n].nextpos, 4, 1, fp);

#ifdef SUN_OS
	Swap4Byte(&gdbase[crt_n].nextpos);
#endif
	nextpos =gdbase[crt_n].nextpos;

#ifdef	DEBUG
	printf(" in ReadBkgdPictData: nextpos =%d\n", nextpos);
#endif
	for (i=0; i < nextpos; i++)
	{
                freadBckRcd (fp, &rcd.bck);
               
                freadLnkRcd (fp, &rcd.lnk);
		if( rcd.lnk.dyn.dyn_tag == DYN_TAG_ON  
			|| (rcd.lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd.lnk.cgrp.cgrp_id !=0)
			/*|| (rcd.lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON && rcd.lnk.cgrp.cgrp_id !=0)*/ )
		{
                    /*gdbase[crt_n].buffer[i] = rcd;
                    printf("gdbase[%d].buffer[%d].lnk.dbname=%s,gdbase[%d].buffer[%d].lnk.dyn.dyn_tag=%d,cgrp.tag=%d\n",crt_n,i,
                                gdbase[crt_n].buffer[i].lnk.dbname,crt_n,i,gdbase[crt_n].buffer[i].lnk.dyn.dyn_tag,rcd.lnk.cgrp.tag);
			*/continue;
		}
		else
		{	
			rcd.lnk.act.ctrl_tag   = CTRL_TAG_OFF;
			rcd.lnk.act.ctrl_type  = CTRL_REQ_NULL ;
			gdbase[crt_n].buffer[real_bkgdpict_nextpos] = rcd;
                        
			real_bkgdpict_nextpos ++;

			if( real_bkgdpict_nextpos > MAXOBJECTS)
			{
				PopupMessageBox ( main_w[crt_n], MSG_SOFTCOPY_BUFFER_OVERFLOW);  
				break;
			}
		}
	}
	gdbase[crt_n].nextpos = real_bkgdpict_nextpos;
	
	for (layer=0; layer < MAXLAYERS; layer++)
        {
                fread (&gdbase[crt_n].layer[layer].min_visible, 4 ,1, fp);
                fread (&gdbase[crt_n].layer[layer].max_visible, 4 ,1, fp);
#ifdef SUN_OS
		Swap4Byte(&gdbase[crt_n].layer[layer].min_visible);
		Swap4Byte(&gdbase[crt_n].layer[layer].max_visible);
#endif
        }
	fclose( fp);/* Modified by zyp on 2001.1.9     */
}

void	ReadFrgdPictData (int crt_n, FILE	*fp )
{
	int	i;
	
	long	offset;
GBRCD   rcd;

	offset = (long) sizeof(cur_dsp_name[crt_n]);
	
	fseek( fp , offset, 0);
	fread (&dyn_nextpos, 4, 1, fp);
#ifdef SUN_OS
	Swap4Byte(&dyn_nextpos);
#endif

#ifdef	DEBUG
	printf("$$$$ begin in ReadFrgdPictData: dyn_nextpos =%d,real_bkgdpict_nextpos=%d\n", dyn_nextpos,real_bkgdpict_nextpos);
#endif
	for(i=0; i < dyn_nextpos; i++)
	{
		if( real_bkgdpict_nextpos > MAXOBJECTS-1 )
		{
			PopupMessageBox ( main_w[crt_n], MSG_SOFTCOPY_BUFFER_OVERFLOW);  
			break;
		}
	/*freadBckRcd ( fp, &gdbase[crt_n].buffer[real_bkgdpict_nextpos].bck);
                
		freadLnkRcd ( fp, &gdbase[crt_n].buffer[real_bkgdpict_nextpos].lnk);*/
        freadBckRcd ( fp, &rcd.bck);
                
		freadLnkRcd ( fp, &rcd.lnk);
        gdbase[crt_n].buffer[real_bkgdpict_nextpos] = rcd;

       /*printf("###gdbase[%d].buffer[%d].lnk.dbname=%s,gdbase[%d].buffer[%d].lnk.dyn.dyn_tag=%d,cgrp.tag=%d\n",
		crt_n,real_bkgdpict_nextpos,rcd.lnk.dbname,crt_n,real_bkgdpict_nextpos,rcd.lnk.dyn.dyn_tag,
		rcd.lnk.cgrp.tag);
	*/		
		real_bkgdpict_nextpos++;
	}
	fclose( fp);

	gdbase[crt_n].nextpos = real_bkgdpict_nextpos ;
#ifdef	DEBUG
	printf(" in ReadFrgdPictData:: nextpos =%d\n", gdbase[crt_n].nextpos);
#endif
}

