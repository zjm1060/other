
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/menu_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/layer_def.h"
#include	"../inc/dyn_form_def.h"
#include	"../inc/sub_dsp_def.h"
FILE	*gCurDspFilefp  =  NULL  ;
char   gCurDspFileName[256]  ; 
void BlankStripCopy (char *dst, char *src, int len);
void DynButtonInit ();
void EditButtonInit ();
void ClearGdbItemsTag ();
void SearchItems ( HWND, int, int, int, int, NEW_GDBUFF *,BOOL);

int	version4_flag= 1;

void SaveGdbaseData (fp)
FILE	*fp;
{
	int	i, layer, len, nextpos;
    char version[] = "v4.0a" ;

	fwrite(version,sizeof(char),6,fp) ;  //add a version tag before file;
	nextpos =v4_gdbase.nextpos;
	fwrite (&nextpos, 4, 1, fp);
	len	=sizeof(v4_gdbase.buffer[i]);
	for (i=0; i < nextpos; i++)
    {
		fwriteRcd (fp, &v4_gdbase.buffer[i]);
	}

	for (layer=0; layer < MAXLAYERS; layer++)
	{
		fwrite (&layerdb[layer].min_visible, 4 ,1, fp);
		fwrite (&layerdb[layer].max_visible, 4 ,1, fp);
	}

}

void SaveSubGdbaseData (fp)
FILE	*fp;
{
	int	i, layer, len, nextpos ;
     char version[] = "v4.0a" ;

	fwrite(version,sizeof(char),6,fp) ;  //add a version tag before file;

	SearchItems( ghWnd, SUB_DSP_x, SUB_DSP_y, 
		SUB_DSP_x+SUB_DSP_canvas_width, SUB_DSP_y+SUB_DSP_canvas_height,
		&v4_gdbase,FALSE);

	nextpos =gdbuff.nextpos;
	
	fwrite (&nextpos, 4, 1, fp);
	len	=sizeof(gdbuff.buffer[i]);

	for (i=0; i < nextpos; i++)
    {
		fwriteRcd (fp, &gdbuff.buffer[i]);
	}

	for (layer=0; layer < MAXLAYERS; layer++)
	{
		fwrite (&layerdb[layer].min_visible, 4 ,1, fp);
		fwrite (&layerdb[layer].max_visible, 4 ,1, fp);
	}
}




void	fwriteBckRcd (fp, rcd)
BACKRCD 	*rcd;
FILE	*fp;
{
		int	len;

        len     =sizeof (rcd->rect.x1);
        fwrite (&rcd->rect.x1, len, 1, fp);
        fwrite (&rcd->rect.y1, len, 1, fp);
        fwrite (&rcd->rect.x2, len, 1, fp);
        fwrite (&rcd->rect.y2, len, 1, fp);
        len     =sizeof (rcd->draw_type);
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

}

void	fwriteLnkRcd (fp, rcd)
NEW_LINKRCD	*rcd;
FILE    *fp;
{
	int	len;

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

	len     =sizeof (rcd->state);
	fwrite (&rcd->state, len, 1, fp);  //chichi  

	len     = sizeof (rcd->dyn.fmt_def.free_area);
	fwrite (rcd->dyn.fmt_def.free_area, len, 1, fp);

	len     =sizeof (rcd->cgrp.cgrp_id);
	fwrite (&rcd->cgrp.cgrp_id, len, 1, fp);
	fwrite (&rcd->cgrp.tag,  len, 1, fp);

}

void	fwriteRcd (fp, rcd)
NEW_GBRCD 	*rcd;
FILE    *fp;
{
	fwriteBckRcd (fp, &rcd->bck);
	fwriteLnkRcd (fp, &rcd->lnk);
}


void	freadBckRcd (fp, rcd)
BACKRCD 	*rcd;
FILE    *fp;
{
	int	len;

        len     =sizeof (rcd->rect.x1);
        fread (&rcd->rect.x1, len, 1, fp);
        fread (&rcd->rect.y1, len, 1, fp);
        fread (&rcd->rect.x2, len, 1, fp);
        fread (&rcd->rect.y2, len, 1, fp);
        len     =sizeof (rcd->draw_type);
        fread (&rcd->draw_type, len, 1, fp);
        fread (&rcd->fill.fill_type, len, 1, fp);
if (rcd->fill.fill_type >= 8)
	printf ("rcd->fill.fill_type=%d  \n" , rcd->fill.fill_type ); 
        fread (&rcd->fg, len, 1, fp);
        fread (&rcd->bg, len, 1, fp);
        fread (&rcd->line_width, len, 1, fp);
        fread (&rcd->layer, len, 1, fp);
        fread (&rcd->plcnt, len, 1, fp);
        fread (&rcd->arc_type, len, 1, fp);
        fread (&rcd->ftset_id, len, 1, fp);
        len     =sizeof (rcd->data);
        fread (&rcd->data, len, 1, fp);
		
	/*added by wzg 20060417 for v3->v4*/
		if(version4_flag==0)
		{
			rcd->fg= ColorId3to4(rcd->fg);
			rcd->bg= ColorId3to4(rcd->bg);
		}
}

void	freadLnkRcd (fp, rcd)
LINKRCD 	*rcd;
FILE    *fp;
{
	int	len;

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
	fread (rcd->dbname, len, 1, fp);
	len     = sizeof (rcd->dyn.fmt_def.free_area);
	fread (rcd->dyn.fmt_def.free_area, len, 1, fp);

	len     =sizeof (rcd->cgrp.cgrp_id);
	fread (&rcd->cgrp.cgrp_id, len, 1, fp);
	fread (&rcd->cgrp.tag,  len, 1, fp);

}
//read 4.0a  rcd data from file
void	v4_freadLnkRcd (fp, rcd)   
NEW_LINKRCD 	*rcd;
FILE    *fp;
{
	int	len;

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

	len     =sizeof (rcd->dbname);   // 48   ;
	fread (rcd->dbname, len, 1, fp);

	len     =sizeof (rcd->state);   // 48   ;
	fread (&rcd->state, len, 1, fp);
	len     = sizeof (rcd->dyn.fmt_def.free_area);
	fread (rcd->dyn.fmt_def.free_area, len, 1, fp);

	len     =sizeof (rcd->cgrp.cgrp_id);
	fread (&rcd->cgrp.cgrp_id, len, 1, fp);
	fread (&rcd->cgrp.tag,  len, 1, fp);

}

void	freadRcd (fp, rcd) //old 3.0a
GBRCD 	*rcd;
FILE    *fp;
{
	freadBckRcd (fp, &(rcd->bck));
	freadLnkRcd (fp, &(rcd->lnk));
}

//read 4.0a  rcd data from file  !!!
void	v4_freadRcd (fp, rcd)
NEW_GBRCD 	*rcd;
FILE    *fp;
{
	freadBckRcd (fp, &(rcd->bck));
	v4_freadLnkRcd (fp, &(rcd->lnk));
}
//add by chi   for 4.0a  file data!!!
void TransV3toV4file(GDBUFF*v3gdbase  , NEW_GDBUFF*v4gdbase )
{   
	int  i ; 
    char dbname1[60] ="IPM.";
    v4gdbase->nextpos =  v3gdbase->nextpos ;
	strcpy(v4gdbase->name , v3gdbase->name);
	 
	for( i = 0 ; i< v3gdbase->nextpos ; i++ )
	{
         memcpy( &v4gdbase->buffer[i].bck , &v3gdbase->buffer[i].bck , sizeof(BACKRCD) );
		 if (strcmp(v3gdbase->buffer[i].lnk.dbname,"")==0)
		 {
			strcpy(v4gdbase->buffer[i].lnk.dbname, v3gdbase->buffer[i].lnk.dbname)   ;
		 }
		 else
		 {
			    strcat(dbname1,v3gdbase->buffer[i].lnk.dbname);
				strcpy(v4gdbase->buffer[i].lnk.dbname,  dbname1) ;
				strcpy(dbname1,  "IPM.") ;
		 }
		
		 memcpy( &v4gdbase->buffer[i].lnk.act   , &v3gdbase->buffer[i].lnk.act , sizeof(ACTION) );
		 memcpy( &v4gdbase->buffer[i].lnk.cgrp   , &v3gdbase->buffer[i].lnk.cgrp , sizeof(DYN_COLOR_GRP) );
		 memcpy( &v4gdbase->buffer[i].lnk.dyn  , &v3gdbase->buffer[i].lnk.dyn , sizeof(DYNAMIC) );
	}
    return ;
}

   
void	ReadGdbaseData (fp)
FILE    *fp;
{
	int	i, layer, nextpos;
    char version[6] ;

   fread(version,1,6,fp);
   if (strcmp("v4.0a",version)==0)    //4.0a *.dbin file data formate
   {
	    version4_flag= 1;
		fread (&v4_gdbase.nextpos, 4, 1, fp);
		nextpos =v4_gdbase.nextpos;
		for (i=0; i < nextpos; i++)
		{
			v4_freadRcd (fp, &v4_gdbase.buffer[i]);
		}

		for (layer=0; layer < MAXLAYERS; layer++)
		{
			fread (&layerdb[layer].min_visible, 4 ,1, fp);
			fread (&layerdb[layer].max_visible, 4 ,1, fp);
		}
   } else {     //3.0a *.dbin file data formate 
		MessageBox(ghWnd,"文件的数据结构为v3.0a版，需要转换!\n注意!!!如果你保存，则这个文件将转存成新的格式。 ",
			                  "IPM:提示信息",MB_OK|MB_ICONINFORMATION) ;
	    version4_flag= 0;
		rewind( fp );   
		fread (&v3_gdbase.nextpos, 4, 1, fp);
		nextpos =v3_gdbase.nextpos;
		for (i=0; i < nextpos; i++)
		{
			freadRcd (fp, &v3_gdbase.buffer[i]);
		}
        TransV3toV4file(&v3_gdbase  , &v4_gdbase );
		for (layer=0; layer < MAXLAYERS; layer++)
		{
			fread (&layerdb[layer].min_visible, 4 ,1, fp);
			fread (&layerdb[layer].max_visible, 4 ,1, fp);
		}


   }  // endif ;


	 
   
	if (cur.bck.draw_type != DrwT_CUT)
	{
		DynButtonInit ();
		EditButtonInit ();
	}

}


//read 4.0a  v4_gdbase data from file
void	v4_ReadGdbaseData (fp)
FILE    *fp;
{
	int	i, layer, nextpos;


	fread (&v4_gdbase.nextpos, 4, 1, fp);
	nextpos =v4_gdbase.nextpos;
/*	len     =sizeof(v4_gdbase.buffer[i]);
*/	for (i=0; i < nextpos; i++)
	{
		v4_freadRcd (fp, &v4_gdbase.buffer[i]);
	}

	for (layer=0; layer < MAXLAYERS; layer++)
	{
		fread (&layerdb[layer].min_visible, 4 ,1, fp);
		fread (&layerdb[layer].max_visible, 4 ,1, fp);
	}

	if (cur.bck.draw_type != DrwT_CUT)
	{
		DynButtonInit ();
		EditButtonInit ();
	}

}

int 	ReadGdbaseDataFindFile (fp)
FILE    *fp;
{
	int	i, layer, nextpos;
    char version[6] ;
	
	fread(version,1,6,fp);
	if (strcmp("v4.0a",version)==0)    //4.0a *.dbin file data formate
	{
		version4_flag= 1;
		fread (&GDBASE_FILE.nextpos, 4, 1, fp);
		nextpos =GDBASE_FILE.nextpos;
		for (i=0; i < nextpos; i++)
		{
			v4_freadRcd (fp, &GDBASE_FILE.buffer[i]);
		}
				
	} else 
		return FALSE ; 
	return   TRUE ; 
}



FILE    *FileOpen (full_name)
char	*full_name;
{
	struct	stat	statb;
	FILE    *fp;
	char	str[150];
//MessageBox(ghWnd, full_name, NULL, MB_OK | MB_ICONINFORMATION);  // chi		
    /* make sure the file is a regular text file and open it */
	if (stat(full_name, &statb) == -1 ||
        	(statb.st_mode & S_IFMT) != S_IFREG ||
       		!(fp = fopen(full_name, "rb")))
	{
       	if ((statb.st_mode & S_IFMT) == S_IFREG) 
       	{
			sprintf (str, "IPM: in FileOpen: file not opened.%s", full_name);
			MessageBox(ghWnd, str, NULL, MB_OK | MB_ICONINFORMATION);
		}
		else 
		{
			sprintf (str, "IPM: in FileOpen: not a regular file or file not exist. %s", full_name);
			MessageBox(ghWnd, str, NULL, MB_OK | MB_ICONINFORMATION);
		}
		return	NULL;
	}
	else
	{
		return (fp);
	}
}


void	OpenDspFile (filename)
char	*filename;
{
	FILE	*fp;
	char	full_name[MAX_FILE_NAME_LEN]={""};
	char	part_name[50]={""};
	


	if( strrchr(filename, '\\') != NULL )
	{
		strcpy(full_name, filename);
		strcpy(filename, strrchr(full_name, '\\')+1  );
	}

	if( full_name[0] == '\0' ) 
	{
		strcpy (part_name, "/DSP/");
		strcat (part_name, filename);
		FormFullName (home, full_name, part_name);
	}
	
    strcpy(gCurDspFileName  ,full_name ); 
	fp  =	FileOpen( full_name); 
	if ( fp == NULL)
	{
		MessageBox(ghWnd, "Open File Error", NULL, MB_OK | MB_ICONINFORMATION);
		return;
	}

	ReadGdbaseData (fp);

	/* reset gdbuff count if DrwType != DrwT_CUT. */
	if (cur.bck.draw_type != DrwT_CUT)
	{
		gdbuff.nextpos	=0;
		ClearGdbItemsTag ();
	}
	if( edit_mode != SUB_DSP_EDIT)
		edit_mode	=DISPLAY_EDIT;

	fclose (fp);

	InvalidateRect(ghWnd, NULL, TRUE);

}

//aded by chi 

void GetFilePartName(const char *FullName, char *PartName)
{
	char filename[MAX_GIF_FILE_NAME_LEN] ;
	//PartName = "\0" ; 
	if( strrchr(FullName, '\\') != NULL )		
		strcpy(filename, strrchr(FullName, '\\')+1);
	strcpy(PartName,filename);
}


/****************************************************************************
*
*    FUNCTION: OpenNewFile(HWND, int)
*
*    PURPOSE:  Invokes common dialog function to open a file and opens it.
*
*    COMMENTS:
*
*        This function initializes the OPENFILENAME structure and calls
*        the GetOpenFileName() common dialog function.
*
*    RETURN VALUES:
*        TRUE - The file was opened successfully and read into the buffer.
*        FALSE - No files were opened.
*
****************************************************************************/
BOOL OpenNewFile( HWND hWnd, char*openFile,int file_mode)
{

	OPENFILENAME OpenFileName;
	CHAR         DirName[256]      = "\0";
	CHAR         FileName[256]      = "\0";
	CHAR         FileTitle[256]	= "\0";
	CHAR		 DlgTitle[100]	= "\0";
	CHAR		 DefExt[10]		= "\0";
	DWORD		 nFilterIndex;
    //CHAR Filter[80];
// Filter specification for the OPENFILENAME struct
// This is portable for i386 and MIPS
// Leaving out the \0 terminator will cause improper DWORD alignment
// and cause a failure under MIPS
   
   strcpy( FileName, "");
   strcpy( FileTitle, "");
   switch	( file_mode )
   {
   		case	DSP_FILE:
   			strcpy( DlgTitle, "文件选择窗口");
   			strcpy( DefExt, "*.dbin");
			strcpy( DirName, "\\home\\ems\\h9000\\dsp");
			OpenFileName.lpstrFilter= "DSP Files (*.dbin)\0*.dbin\0GIF Files (*.gif)\0*.gif\0All Files (*.*)\0*.*\0";
			nFilterIndex = 1L;
			break;
		case	SYM_FILE:
   			strcpy( DlgTitle, "符号文件选择窗口");
   			strcpy( DefExt, "*.DBIN");
			OpenFileName.lpstrFilter= "SYM LIB Files (*.DBIN)\0*.DBIN\0All Files (*.*)\0*.*\0";
			strcpy( DirName, "\\home\\ems\\h9000\\dsp");
			nFilterIndex = 1L;
			break;
		case	GIF_FILE:
   			strcpy( DlgTitle, "扫描照片文件选择窗口");
   			strcpy( DefExt, "*.bmp");
			OpenFileName.lpstrFilter= "bitmaps Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
			strcpy( DirName, "\\home\\ems\\h9000\\dsp\\gif");
			nFilterIndex = 1L;
			break;
		default:
			strcpy( DlgTitle, "文件选择窗口");
   			strcpy( DefExt, "*.*");
			strcpy( DirName, NULL);
			OpenFileName.lpstrFilter= "DSP Files (*.dbin)\0*.dbin\0GIF Files (*.gif)\0*.gif\0SGZY Files (*.SGZY)\0*.SGZY\0SFTCP Files (*.sfbin)\0*.sfbin\0All Files (*.*)\0*.*\0";
			nFilterIndex = 5L;
			break;

   }

   OpenFileName.lStructSize       = sizeof(OPENFILENAME);
   OpenFileName.hwndOwner         = ghWnd;
   OpenFileName.hInstance         = NULL;
//   OpenFileName.lpstrFilter       = Filter;
   OpenFileName.lpstrCustomFilter = (LPSTR) NULL;
   OpenFileName.nMaxCustFilter    = 0L;
   OpenFileName.nFilterIndex      = nFilterIndex;
   OpenFileName.lpstrFile         = FileName;
   OpenFileName.nMaxFile          = sizeof(FileName);
   OpenFileName.lpstrFileTitle    = FileTitle;
   OpenFileName.nMaxFileTitle     = sizeof(FileTitle);
   OpenFileName.lpstrInitialDir   = DirName;
   OpenFileName.lpstrTitle        = DlgTitle;
   OpenFileName.Flags			  = OFN_SHOWHELP | OFN_PATHMUSTEXIST |
                					OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
   OpenFileName.nFileOffset       = 0;
   OpenFileName.nFileExtension    = 0;
   OpenFileName.lpstrDefExt       = DefExt;
   OpenFileName.lCustData         = 0;

   if (GetOpenFileName(&OpenFileName))
   {
		switch	( file_mode )
		{
			case	DSP_FILE:
			case SYM_FILE:
				BlankStripCopy ( openFile, OpenFileName.lpstrFile, MAX_FILE_NAME_LEN);
				OpenDspFile(OpenFileName.lpstrFile);
	  			// reset the title in the title bar to reflect the new open file
      			//SetWindowText( hWnd, OpenFileName.lpstrFile );
				break;


			case	GIF_FILE:
				{
					BlankStripCopy ( openFile, OpenFileName.lpstrFile, MAX_FILE_NAME_LEN);
					//strcpy(openFile,OpenFileName.lpstrFilter ) ;
					//system (cmd_buff);
				}
				break;


			default:
				break;
		}

    }
    else
    {
      ProcessCDError(CommDlgExtendedError(), ghWnd );
      return FALSE;
    }

    return TRUE;
}

/****************************************************************************
*
*    FUNCTION: SaveToFile( HWND )
*
*    PURPOSE:  Saves the current buffer to the current file.
*
*    RETURN VALUES:
*        TRUE - The file was saved successfully.
*        FALSE - The buffer was not saved to a file.
*
****************************************************************************/
BOOL SaveToFile( HWND hWnd )
{
	FILE	*fp;
   	CHAR buf[256];

	fp = fopen(cur_dsp_file, "wb");
    if (fp == NULL)
    {
      sprintf( buf, "Could not save file: %s", cur_dsp_file );
      MessageBox( hWnd, buf, "IPM", MB_OK );
      return FALSE;
    }
   	switch	( edit_mode )
   	{
   		case	DISPLAY_EDIT:
    		SaveGdbaseData (fp);
			break;
		case	SUB_DSP_EDIT:
            SaveSubGdbaseData (fp);
			break;
	}
    fclose (fp);

   	return TRUE;
}

/****************************************************************************
*
*    FUNCTION: SaveAs(HWND)
*
*    PURPOSE:  Invokes the common dialog function to save the current
*              buffer to a file.
*
*    RETURN VALUES:
*        TRUE - The file was saved successfully.
*        FALSE - The buffer was not saved to a file.
*
****************************************************************************/
BOOL SaveAs( HWND hWnd )
{

	OPENFILENAME OpenFileName;
	CHAR         DirName[256]      = "\0";
	CHAR         FileName[256]      = "\0";
	CHAR         FileTitle[256]	= "\0";
//	CHAR		 Filter[256]	= "\0";
	CHAR		 DefExt[10]		= "\0";
	DWORD		 nFilterIndex;
	CHAR		 DlgTitle[100]	= "\0";

	CHAR Filter[] = "DSP Files (*.dbin)\0*.dbin\0DRBIN Files (*.drbin)\0*.drbin\0MRBIN Files (*.mrbin)\0*.mrbin\0SUB DSP Files(*.sbin)\0*.sbin\0All Files (*.*)\0*.*\0";

   strcpy( FileName, "");
   strcpy( FileTitle, "");

   strcpy( DirName, "\\home\\ems\\h9000\\dsp");
   strcpy( DlgTitle, "Save As 文件选择窗口");

   switch	( edit_mode )
   {
   		case	DISPLAY_EDIT:
   			strcpy( DefExt, "*.dbin");
			nFilterIndex = 1L;
			break;
		case	SUB_DSP_EDIT:
   			strcpy( DefExt, "*.sbin");
			nFilterIndex = 3L;
			break;
		default:
   			strcpy( DefExt, "*.*");
			strcpy( DirName, NULL);
			nFilterIndex = 4L;
			break;
   }


   OpenFileName.lStructSize       = sizeof(OPENFILENAME);
   OpenFileName.hwndOwner         = ghWnd;
   OpenFileName.hInstance         = NULL;
   OpenFileName.lpstrFilter       = Filter;
   OpenFileName.lpstrCustomFilter = (LPSTR) NULL;
   OpenFileName.nMaxCustFilter    = 0L;
   OpenFileName.nFilterIndex      = nFilterIndex;
   OpenFileName.lpstrFile         = FileName;
   OpenFileName.nMaxFile          = sizeof(FileName);
   OpenFileName.lpstrFileTitle    = FileTitle;
   OpenFileName.nMaxFileTitle     = sizeof(FileTitle);
   OpenFileName.lpstrInitialDir   = DirName;
   OpenFileName.lpstrTitle        = "Save File As";
   OpenFileName.Flags			  = OFN_SHOWHELP | OFN_HIDEREADONLY;
   OpenFileName.nFileOffset       = 0;
   OpenFileName.nFileExtension    = 0;
   OpenFileName.lpstrDefExt       = DefExt;
   OpenFileName.lCustData         = 0;

   if ( GetSaveFileName( &OpenFileName ))
   {
		BlankStripCopy ( cur_dsp_file, OpenFileName.lpstrFile, MAX_FILE_NAME_LEN);
		SaveToFile( hWnd );
// reset the title in the title bar to reflect the new open file
//      SetWindowText( hWnd, OpenFileName.lpstrFile );
   }
   else
   {
        ProcessCDError(CommDlgExtendedError(), hWnd );
        return FALSE;
   }

   return (TRUE);
}



/****************************************************************************
*
*    FUNCTION: ProcessCDError(DWORD)
*
*    PURPOSE:  Processes errors from the common dialog functions.
*
*    COMMENTS:
*
*        This function is called whenever a common dialog function
*        fails.  The CommonDialogExtendedError() value is passed to
*        the function which maps the error value to a string table.
*        The string is loaded and displayed for the user.
*
*    RETURN VALUES:
*        void.
*
****************************************************************************/
#include	"../inc/cderr.h"
void ProcessCDError(DWORD dwErrorCode, HWND hWnd)
{
   WORD  wStringID;
   CHAR  buf[256];

   switch(dwErrorCode)
      {
         case CDERR_DIALOGFAILURE:   wStringID=IDS_DIALOGFAILURE;   break;
         case CDERR_STRUCTSIZE:      wStringID=IDS_STRUCTSIZE;      break;
         case CDERR_INITIALIZATION:  wStringID=IDS_INITIALIZATION;  break;
         case CDERR_NOTEMPLATE:      wStringID=IDS_NOTEMPLATE;      break;
         case CDERR_NOHINSTANCE:     wStringID=IDS_NOHINSTANCE;     break;
         case CDERR_LOADSTRFAILURE:  wStringID=IDS_LOADSTRFAILURE;  break;
         case CDERR_FINDRESFAILURE:  wStringID=IDS_FINDRESFAILURE;  break;
         case CDERR_LOADRESFAILURE:  wStringID=IDS_LOADRESFAILURE;  break;
         case CDERR_LOCKRESFAILURE:  wStringID=IDS_LOCKRESFAILURE;  break;
         case CDERR_MEMALLOCFAILURE: wStringID=IDS_MEMALLOCFAILURE; break;
         case CDERR_MEMLOCKFAILURE:  wStringID=IDS_MEMLOCKFAILURE;  break;
         case CDERR_NOHOOK:          wStringID=IDS_NOHOOK;          break;
         case PDERR_SETUPFAILURE:    wStringID=IDS_SETUPFAILURE;    break;
         case PDERR_PARSEFAILURE:    wStringID=IDS_PARSEFAILURE;    break;
         case PDERR_RETDEFFAILURE:   wStringID=IDS_RETDEFFAILURE;   break;
         case PDERR_LOADDRVFAILURE:  wStringID=IDS_LOADDRVFAILURE;  break;
         case PDERR_GETDEVMODEFAIL:  wStringID=IDS_GETDEVMODEFAIL;  break;
         case PDERR_INITFAILURE:     wStringID=IDS_INITFAILURE;     break;
         case PDERR_NODEVICES:       wStringID=IDS_NODEVICES;       break;
         case PDERR_NODEFAULTPRN:    wStringID=IDS_NODEFAULTPRN;    break;
         case PDERR_DNDMMISMATCH:    wStringID=IDS_DNDMMISMATCH;    break;
         case PDERR_CREATEICFAILURE: wStringID=IDS_CREATEICFAILURE; break;
         case PDERR_PRINTERNOTFOUND: wStringID=IDS_PRINTERNOTFOUND; break;
         case CFERR_NOFONTS:         wStringID=IDS_NOFONTS;         break;
         case FNERR_SUBCLASSFAILURE: wStringID=IDS_SUBCLASSFAILURE; break;
         case FNERR_INVALIDFILENAME: wStringID=IDS_INVALIDFILENAME; break;
         case FNERR_BUFFERTOOSMALL:  wStringID=IDS_BUFFERTOOSMALL;  break;

         case 0:   //User may have hit CANCEL or we got a *very* random error
            return;

         default:
            wStringID=IDS_UNKNOWNERROR;
      }

   LoadString(NULL, wStringID, buf, sizeof(buf));
   MessageBox(hWnd, buf, NULL, MB_OK);
   return;
}
