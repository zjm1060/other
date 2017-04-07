

#include 	<stdio.h>
#include 	<string.h>

#include	"resource.h"

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/string_pattern.h"
#include	"../inc/field_def.h"

#define		PTR_SRC_LEN		36

#define		STATION_NAME_LEN	12
#define		DEVICE_NAME_LEN		40

#define		DATA_TYPE_NAME_LEN	40

int	replace_cell_num();
int	replace_cell_off();
int	replace_color_grp();
int 	ReplaceDbPtr (NEW_LINKRCD *);
void ParsePtrSource (char *, char *, char *, char *, char *); 
void BuildPtrSource (char *, char *, char *, char *, char *); 
int 	ReplaceString ( PATTERN_INFO *, PATTERN_INFO *, char *, char *, int );
int 	ParsePattern ( char *, PATTERN_INFO *);
BOOL CALLBACK ReplaceDlgProc(HWND, UINT, WPARAM, LPARAM);
void ReplaceReference (HWND, char *, char *);
void ReplaceRptReqType (char *, char *, NEW_LINKRCD * );
void ReplaceRptPkVlyType (char *, char *, NEW_LINKRCD * );
void ReplaceRptTimeId (char *, char *, NEW_LINKRCD * );
void ReplaceCtrlMenu (char *src, char *dest,NEW_LINKRCD	* lnk_rcd ,int menu_id);
void FindReference (HWND hdlg, char *srch_pat, int ref_type) ; 
#define REF_BADGE		0
#define REF_STATION_NAME		1
#define	REF_DEVICE_NAME		2
#define REF_DATA_TYPE_NAME	3
#define REF_ENTRY_NAME		4
#define REF_CELL_NUM		5
#define REF_CELL_OFF		6
#define REF_COLOR_GRP		7
#define REF_TIME_ID			8
#define REF_CTRL_MENU		9  /*chichi*/
#define REF_DSP_NAME		10
#define REF_TEXT			11
#define MAX_REF_NUM			12
char *search_type[] = 
{
	"BADGE numbers",	//REF_BADGE
	"Station name" ,	//REF_STATION
	"Device names",	  	//REF_DEVICE_NAME
	"Record names",	  	//REF_DATA_TYPE_NAME
	"Entry names",	  	//REF_ENTRY_NAME
	"Req Types",	  	//REF_CELL_NUM
	"PkVly Types",	  	//REF_CELL_OFF
	"Color groups",	  	//REF_COLOR_GROUP
	"Req TIME_ID" ,		//REF_TIME_ID 
	"Ctrl Menu"    ,    //REF_CTRL_MENU
	"Dsp Name"	,		///REF_DSP_NAME
	"Text"  ///REF_TEXT
};

int	chg_cnt;
int	ref_type	= REF_BADGE;
int	rplc_error;
int	pat_error;
PATTERN_INFO	srch_info;
PATTERN_INFO	rplc_info;


void CheckReqType( req_name, req_type)
char    req_name[PTR_SRC_LEN];
short	*req_type;
{
	if( strcmp( req_name, "Dayly") == 0)
		{ *req_type = REQ_DAYLY;}
	else if ( strcmp( req_name, "Hourly") == 0)
		{ *req_type = REQ_HOURLY;} 
	else if ( strcmp( req_name, "Monthly") == 0)
		{ *req_type = REQ_MONTHLY;	} 
	else if ( strcmp( req_name, "Yearly") == 0)
		{ *req_type = REQ_YEARLY; }
}

void CheckPkVlyType( pkvly_name, pkvly_type)
char    pkvly_name[PTR_SRC_LEN];
short	*pkvly_type;
{
	if( strcmp( pkvly_name, "NULL") == 0)
		 *pkvly_type = NULL_PKVLY;
	else if ( strcmp( pkvly_name, "Peak") == 0)
		 *pkvly_type = PEAK; 
	else if ( strcmp( pkvly_name, "Ppeak") == 0)
		*pkvly_type = PPEAK;
	else if ( strcmp( pkvly_name, "Vally") == 0)
		 *pkvly_type = VALLY; 
	else if ( strcmp( pkvly_name, "OpnNum") == 0)
		 *pkvly_type = OPEN_NUM;	
	else if ( strcmp( pkvly_name, "ClsNum") == 0)
		 *pkvly_type = CLS_NUM;	
	else if ( strcmp( pkvly_name, "SgOpnNum") == 0)
		 *pkvly_type = SG_OPN_NUM;	
	else if ( strcmp( pkvly_name, "SgClsNum") == 0)
		 *pkvly_type = SG_CLS_NUM;	
	else if ( strcmp( pkvly_name, "TstOpnNum") == 0)
		 *pkvly_type = TST_OPN_NUM;	
	else if ( strcmp( pkvly_name, "TstClsNum") == 0)
		 *pkvly_type = TST_CLS_NUM;	
	else if ( strcmp( pkvly_name, "DayMax") == 0)
		 *pkvly_type = DAY_MAX;	
	else if ( strcmp( pkvly_name, "Averg") == 0)
                 *pkvly_type = AVERG;
	else if ( strcmp( pkvly_name, "Sum") == 0)
                 *pkvly_type = SUM ;
}


BOOL CALLBACK ReplaceDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int		i;
	HWND	hlist;
	char srch_pat[DEVICE_NAME_LEN], rplc_pat[DEVICE_NAME_LEN];

    switch(wMsg)
    {
		case WM_INITDIALOG:
			chg_cnt 	= 0;
			rplc_error 	= 0;

			hlist = GetDlgItem(hDlg, IDC_REF_TYPE);
			for( i=0; i<MAX_REF_NUM; i++ )
				SendMessage(hlist, CB_ADDSTRING, 0, (LPARAM)search_type[i]);
			SendMessage(hlist, CB_SETCURSEL, ref_type, 0);

			CenterWindow(hDlg, ghWnd);
			return TRUE;

		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDC_REF_TYPE:
				switch( HIWORD(wParam) )
				{
				case CBN_SELCHANGE:
					hlist = GetDlgItem(hDlg, IDC_REF_TYPE);
					ref_type = SendMessage(hlist, CB_GETCURSEL,0,0);
					return TRUE;
				default:	
					break;
				}
				break;
			case IDOK:
				memset(srch_pat, 0, sizeof(srch_pat));
				memset(rplc_pat, 0, sizeof(rplc_pat));
/*_		Get search pattern text */
				GetDlgItemText(hDlg, IDC_REPLACE_SEARCH, srch_pat, sizeof(srch_pat));
				GetDlgItemText(hDlg, IDC_REPLACE_TEXT, rplc_pat, sizeof(rplc_pat));
				ReplaceReference (hDlg, srch_pat, rplc_pat);
			case IDCANCEL:
				EndDialog(hDlg, wParam);
				return TRUE;
			}
			break;

    }
    return FALSE;
}

/** REPLACE STRING  by zyp 2012.12.10*/
int ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr)
{
        int StringLen;
        char caNewString[64];
        char* FindPos;
        FindPos =(char *)strstr(sSrc, sMatchStr);
        if( (!FindPos) || (!sMatchStr) )
                return -1;

        while( FindPos )
        {
                memset(caNewString, 0, sizeof(caNewString));
                StringLen = FindPos - sSrc;
                strncpy(caNewString, sSrc, StringLen);
                strcat(caNewString, sReplaceStr);
                strcat(caNewString, FindPos + strlen(sMatchStr));
                strcpy(sSrc, caNewString);

                FindPos =(char *)strstr(sSrc, sMatchStr);
        }
        free(FindPos);
        return 0;
}


/*--------------------------------------------------------------------------------
!_		4.x		replace_reference - Find and Replace Database References
!_
!_		4.x.1	Module Function
!_				---------------
!_		This module searches the currenlty selected fields for specific device,
!_		record, entry, cell number, or cell offset references and replaces each
!_		instance with a new reference.  The module uses a dialog box to allow
!_		the user to specify the search and replace patterns and the reference
!-		reference type.  The search and replace strings can have imbedded 
!_		wild card characters (*).
!_		
!--------------------------------------------------------------------------------*/

void ReplaceReference (HWND hdlg, char *srch_pat, char *rplc_pat)
{
	int	i , menu_id;
	char	pat_name[10];
	char tmp_name[MAX_TEXT_LEN +5];
/*_		Parse the pattern (ParsePattern) */
/*_		IF search pattern is valid */
		if (!(pat_error = ParsePattern (srch_pat, &srch_info))) 
		{
				
			if (!(pat_error = ParsePattern (rplc_pat, &rplc_info))) 
			{
/*_				Verify that the two patterns are compatible */
/*
				if (pattern_stars[srch_info.type]
					 < pattern_stars[rplc_info.type]) 
				{
					pat_error = STAR_MATCH_ERR;
				}
*/
			}
			else 
			{
				strcpy(pat_name, "Replace");
			}
		}
		else 
		{
			strcpy(pat_name, "Search");
		}

		if (!pat_error) 
		{
/*_			Set up to undo the replacement (setup_undo) */
/*			setup_undo(USR_ACT_RPLC);
*/printf("ref_type=%d\n",ref_type);
			switch (ref_type) 
			{
			case 	REF_BADGE:
			case	REF_STATION_NAME:
			case 	REF_DEVICE_NAME:
			case 	REF_DATA_TYPE_NAME:
			case 	REF_ENTRY_NAME:
				for (i=0; i<v4_gdbase.nextpos; i++)
				{
					NEW_GBRCD	*rcd;

					rcd	= &v4_gdbase.buffer[i];
					if (( rcd->bck.tag == ITEM_TAG_ON )   &&   ((rcd->lnk.dyn.dyn_tag == DYN_TAG_ON )||
						(rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd->lnk.cgrp.cgrp_id !=0)))
						
					{
						ReplaceDbPtr ( &rcd->lnk );
					}
				}
				break;

			case 	REF_CTRL_MENU:     /*added by chichi*/
		 
				for (i=0; i<v4_gdbase.nextpos; i++)
				{
					NEW_GBRCD	*rcd;
					
					rcd	= &v4_gdbase.buffer[i];
					if ( (rcd->lnk.dyn.dyn_tag == DYN_TAG_ON ||
						(rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON  ))
						&& rcd->bck.tag == ITEM_TAG_ON )
					{
                           
					    	menu_id  =  SearchMenuId(rplc_pat) ;

							if (menu_id == -1)
							{
								MessageBox(ghWnd,"The Menuname your want replace is not in menuDB!!","Searche Error ",
												MB_OK | MB_ICONWARNING) ;
								return ;
							}
    
	
							 if (strcmp(rcd->lnk.act.fmt_def.menu.name,"" )==0)
								continue ;
	 	 
				 		    ReplaceCtrlMenu ( srch_pat, rplc_pat, &rcd->lnk ,menu_id );
					}
				
				}
				break;
			case 	REF_DSP_NAME:     /*added by chichi*/
		 
				for (i=0; i<v4_gdbase.nextpos; i++)
				{
					NEW_GBRCD	*rcd;
					
					rcd	= &v4_gdbase.buffer[i];
					if ( rcd->bck.tag == ITEM_TAG_ON )
					{
						    //wzg if (strcmp(rcd->lnk.act.fmt_def.menu.name,"" )==0)
							if (strcmp(rcd->lnk.act.fmt_def.dsp.name,"" )==0)
								continue ;
                            if (strcmp(rcd->lnk.act.fmt_def.dsp.name ,srch_pat   ) ==0  )
							{
								strcpy( rcd->lnk.act.fmt_def.dsp.name,rplc_pat  );
								chg_cnt++;
								
							}
    
	 
							
	 	 
				 		    //ReplaceCtrlMenu ( srch_pat, rplc_pat, &rcd->lnk ,menu_id );
					}
				
				}
				break;

			case 	REF_TEXT:     /*added by zyp*/
		 
				for (i=0; i<v4_gdbase.nextpos; i++)
				{
					NEW_GBRCD	*rcd;
					
					rcd	= &v4_gdbase.buffer[i];
					if ( rcd->bck.draw_type == DrwT_TEXT )
					{						    
							if (strcmp(rcd->bck.data.text.text,"" )==0)
								continue ;
							else
								printf("text=%s,srch_pat=%s\n",rcd->bck.data.text.text,srch_pat);
                            if (strstr(rcd->bck.data.text.text ,srch_pat   ) !=0  )
							{
								// ReplaceString (&srch_pat, &rplc_pat, rcd->bck.data.text.text, tmp_name, MAX_TEXT_LEN +5);
								ReplaceStr(rcd->bck.data.text.text,srch_pat,rplc_pat);
								//strcpy( rcd->bck.data.text.text,tmp_name  );
								chg_cnt++;
								
							}				 		    
					}
				
				}
				break;

			case	REF_CELL_NUM:			/* for rpt table require_type   */
				for (i=0; i<v4_gdbase.nextpos; i++)
				{
					NEW_GBRCD	*rcd;
					
					rcd	= &v4_gdbase.buffer[i];
					if ( (rcd->lnk.dyn.dyn_tag == DYN_TAG_ON ||
						(rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd->lnk.cgrp.cgrp_id !=0))
						&& rcd->bck.tag == ITEM_TAG_ON )
					{
						ReplaceRptReqType ( srch_pat, rplc_pat, &rcd->lnk );
					}
				
				}
				break;

			case	REF_CELL_OFF:
				for (i=0; i<v4_gdbase.nextpos; i++)
				{
					NEW_GBRCD	*rcd;
					
					rcd	= &v4_gdbase.buffer[i];
					if ( (rcd->lnk.dyn.dyn_tag == DYN_TAG_ON ||
						(rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd->lnk.cgrp.cgrp_id !=0))
						&& rcd->bck.tag == ITEM_TAG_ON )
					{
						ReplaceRptPkVlyType ( srch_pat, rplc_pat, &rcd->lnk );
					}
				}
				break;

			case	REF_TIME_ID:
				for (i=0; i<v4_gdbase.nextpos; i++)
				{
					NEW_GBRCD	*rcd;
					
					rcd	= &v4_gdbase.buffer[i];
					if ( (rcd->lnk.dyn.dyn_tag == DYN_TAG_ON ||
						(rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd->lnk.cgrp.cgrp_id !=0))
						&& rcd->bck.tag == ITEM_TAG_ON )
					{
						ReplaceRptTimeId ( srch_pat, rplc_pat, &rcd->lnk );
						
					}
				}
				break;

			default:
				break;

			}

/*_			IF replacement error occurred */
			if (rplc_error) 
			{
				MessageBox (hdlg, "Replacement error on some fields: ", "IPM", MB_OK);
			}

			if (chg_cnt) 
			{
/*_				Mark drawing changed (drawing_change) */
		/*		drawing_change ();
		*/			
				char notify_msg[200];

				sprintf (notify_msg, "%d reference%s replaced.", chg_cnt, (chg_cnt > 1) ? "s":"");
				MessageBox(ghWnd, notify_msg, "IPM: Notify", MB_OK);
			}
			else 
			{
				MessageBox (hdlg, "No matching references found.", "IPM", MB_OK);
			}

		}
		else 
		{
			MessageBox (hdlg, "Search pattern unvalid.", "IPM", MB_OK);
		}

}



char	*GetPtrSource (rcd)
NEW_LINKRCD	*rcd;
{

	return	(char *)rcd->dbname;

}


int	PutPtrSource (rcd, ptr)
NEW_LINKRCD	*rcd;
char	*ptr;
{

	strcpy (rcd->dbname, ptr);

	return	0;
}

void ReplaceCtrlMenu (char *src, char *dest,NEW_LINKRCD	* lnk_rcd ,int menuid)
{
 
	if(strcmp( lnk_rcd->act.fmt_def.menu.name , src)==0 )
	{
		strcpy(lnk_rcd->act.fmt_def.menu.name,dest  );
	    lnk_rcd->act.fmt_def.menu.id =  menuid ;
		chg_cnt++;
	}
}


void ReplaceRptReqType (src_patn, dest_patn, lnk_rcd )
char	src_patn[PTR_SRC_LEN];
char	dest_patn[PTR_SRC_LEN];
NEW_LINKRCD	*lnk_rcd;
{
	short	req_type;
	short	new_req_type;	
	
	CheckReqType ( src_patn, &req_type);
	CheckReqType ( dest_patn, &new_req_type);
	if( lnk_rcd->dyn.fmt_def.rpt_num.req_type == req_type)
	{
		lnk_rcd->dyn.fmt_def.rpt_num.req_type =  new_req_type;
		chg_cnt++;
	}
}

void ReplaceRptPkVlyType (src_patn, dest_patn, lnk_rcd )
char	src_patn[PTR_SRC_LEN];
char	dest_patn[PTR_SRC_LEN];
NEW_LINKRCD	*lnk_rcd;
{
	short	pkvly_type;
	short	new_pkvly_type;	
	
	CheckPkVlyType ( src_patn, &pkvly_type);
	CheckPkVlyType ( dest_patn, &new_pkvly_type);

	if( lnk_rcd->dyn.fmt_def.rpt_num.special_type == pkvly_type)
	{
		lnk_rcd->dyn.fmt_def.rpt_num.special_type =  new_pkvly_type;
		chg_cnt++;
	}
}

void ReplaceRptTimeId (src_patn, dest_patn, lnk_rcd )
char	src_patn[PTR_SRC_LEN];
char	dest_patn[PTR_SRC_LEN];
NEW_LINKRCD	*lnk_rcd;
{
	short	src_time;
	short	dest_time;
	
	src_time = atoi( src_patn );
	dest_time = atoi( dest_patn );
	
	if( lnk_rcd->dyn.fmt_def.rpt_num.hour_id == src_time)
	{
		if( lnk_rcd->dyn.fmt_def.rpt_num.req_type == REQ_HOURLY )
		{
			lnk_rcd->dyn.fmt_def.rpt_num.hour_id = dest_time;
			chg_cnt++;
		}
		else if ( lnk_rcd->dyn.fmt_def.rpt_num.req_type == REQ_DAYLY ||
				lnk_rcd->dyn.fmt_def.rpt_num.req_type == REQ_MONTHLY ||
				lnk_rcd->dyn.fmt_def.rpt_num.req_type == REQ_YEARLY )
		{
			lnk_rcd->dyn.fmt_def.rpt_num.date_id = dest_time;
			chg_cnt++;
		}
		else if ( lnk_rcd->dyn.fmt_def.rpt_num.req_type == REQ_MONTHLY ) 
		{
			lnk_rcd->dyn.fmt_def.rpt_num.month_id = dest_time;
			chg_cnt++;
		}
	}
}


static 	int 	ReplaceDbPtr (rcd)
NEW_LINKRCD	*rcd;
{
	char	*ptr_src;
	char    stn_name[STATION_NAME_LEN +1];    
	char	dev_name[DEVICE_NAME_LEN+1];
	char	rec_name[DATA_TYPE_NAME_LEN+1];
	char	ent_name[DBNAME_LEN+1];
	char	tmp_name[DBNAME_LEN];
//	char	tmp_badge[PTR_SRC_LEN];
	char	new_ptr[PTR_SRC_LEN];
	int	stat;

	stat = 0;

	ptr_src 	= GetPtrSource (rcd);

/*_	IF field contains a TREN pointer */
	if (ptr_src && ptr_src[0]) 
	{
	    if(ref_type == REF_BADGE) 
		{
		} 
		else 
		{
/*_			Extract the device, record, and entry names (ParsePtrSource) */
			ParsePtrSource (ptr_src, stn_name, dev_name, rec_name, ent_name);

/*_			Perform the pattern replacement on the selected part (ReplaceString) */	
			switch	(ref_type) 
			{


			case 	REF_STATION_NAME:	
				stat 	= ReplaceString (&srch_info, &rplc_info, stn_name, tmp_name, STATION_NAME_LEN);
				if (stat > 0)
					strcpy (stn_name, tmp_name);
				break;



			case 	REF_DEVICE_NAME:	
				stat 	= ReplaceString (&srch_info, &rplc_info, dev_name, tmp_name, DEVICE_NAME_LEN);
				if (stat > 0)
					strcpy (dev_name, tmp_name);
				break;

			case 	REF_DATA_TYPE_NAME:	
				stat 	= ReplaceString (&srch_info, &rplc_info, rec_name, tmp_name, DATA_TYPE_NAME_LEN);
				if (stat > 0)
					strcpy (rec_name, tmp_name);
				break;

			case 	REF_ENTRY_NAME:	
				stat 	= ReplaceString (&srch_info, &rplc_info, ent_name, tmp_name, DBNAME_LEN);
				if (stat > 0)
					strcpy (ent_name, tmp_name);
				break;
			}

/*_			IF name changed */
			if (stat > 0) 
			{
/*_				Build new pointer from device, record, entry names (BuildPtrSource) */ 
				BuildPtrSource (stn_name,dev_name, rec_name, ent_name, new_ptr);

/*_				Store new pointer with field (put_ptr_src) */
				if ( PutPtrSource (rcd, new_ptr) == 0) 
				{
					chg_cnt++;
				}
				else 
				{
/*					stat 	= MEM_CHG_ERR;
	*/			}
			}
		}
	}	

/*_	IF name not changed */
	if (stat <= 0) 
	{
/*_		Reset the field selection (set_select) */
/*		set_select (fld, fld_type, 0);
	*/		
/*_		IF error occured */
		if (stat < 0) 
		{
/*_			Set the replacement error flag */
			rplc_error 	= stat;
		}
	}	

	return 0;
}



/*--------------------------------------------------------------------------------
!_		4.x		ParsePtrSource -	Parse Pointer Source String
!_
!_		4.x.1	Module Function
!_				---------------
!_		This module breaks a TREN pointer string into the device, record, and
!_		entry name components.
!_
!--------------------------------------------------------------------------------*/

void ParsePtrSource (ptr_src,stn_s, dev_s, rec_s, ent_s) 
char 	*ptr_src;
char	*stn_s;
char 	*dev_s;
char 	*rec_s;
char 	*ent_s;
{

int	char_cnt;

/*_	Initialize device, record, and entry names to empty strings */
	dev_s[0] = rec_s[0] = ent_s[0] = 0;

/*_	IF pointer is not blank */
	if (*ptr_src != ' ') 
	{

/*_		Extract station name from string (start to .) */
		char_cnt 	= 0;
		while (*ptr_src != '.' && *ptr_src != ' ' && char_cnt < DEVICE_NAME_LEN) 
		{
			*stn_s++ 	= *ptr_src++;
			char_cnt++;
		}
		*stn_s 	= 0;

/*_		Find device of record name */		
		while (*ptr_src != '.') 
		{
			if (*ptr_src == ' ') return;
			ptr_src++;
		}
		ptr_src++;


		char_cnt 	= 0;
		while (*ptr_src != '.' && *ptr_src != ' ' && char_cnt < DATA_TYPE_NAME_LEN) 
		{
			*dev_s++ 	= *ptr_src++;
			char_cnt++;
		}
		*dev_s 	= 0;
		
/*_		Find start of record name */		
		while (*ptr_src != '.') 
		{
			if (*ptr_src == ' ') return;
			ptr_src++;
		}
		ptr_src++;


		char_cnt 	= 0;
		while (*ptr_src != '.' && *ptr_src != ' ' && char_cnt < DATA_TYPE_NAME_LEN) 
		{
			*rec_s++ 	= *ptr_src++;
			char_cnt++;
		}
		*rec_s 	= 0;
		
/*_		Find start of entry name */		
		while (*ptr_src != '.'  &&  *ptr_src != 0  ) //gtk ...
		{
			if (*ptr_src == ' ') return;
			ptr_src++;
		}
		ptr_src++;


		char_cnt 	= 0;
		while (*ptr_src != 0 && char_cnt < LONGNAME_SIZE) 
		{
			*ent_s++ 	= *ptr_src++;
			char_cnt++;
		}
		*ent_s 	= 0;	
	}
}


/*--------------------------------------------------------------------------------
!_		4.x		BuildPtrSource - Build Pointer Source String 
!_
!_		4.x.1	Module Function
!_				---------------
!_		This module constructs a TREN pointer string from the device, record, and
!_		entry name components.
!_
!--------------------------------------------------------------------------------*/

void BuildPtrSource (stn_s,dev_s,  rec_s, ent_s, ptr_src) /* stn.dev.data_type.ptr_id*/
char	*stn_s;
char	*dev_s;
char	*rec_s;
char	*ent_s;
char	*ptr_src;
{
   	while (*stn_s)
		*ptr_src++ 	= *stn_s++;
	*ptr_src++ 	= '.';


	while (*dev_s)
		*ptr_src++ 	= *dev_s++;
	*ptr_src++ 	= '.';

	while (*rec_s)
		*ptr_src++ 	= *rec_s++;
	*ptr_src++ 	= '.';

	while (*ent_s)
		*ptr_src++ 	= *ent_s++;

	*ptr_src++ 	= '\0';	

}



#define 	MATCH 		1
#define 	NO_MATCH 	0
#define 	OKAY 		0

/*--------------------------------------------------------------------------------
!_		4.x		ReplaceString - Do Pattern String Replacement
!_
!_		4.x.1	Module Function
!_				---------------
!_		This module performs a match and replace operation on a string.  First
!_		the module tests whether the source string matches a match pattern.  If
!_		it does, the module translates the string using the replacement pattern.
!_
!_		Examples:
!_			Source string:		ABCDEF		ABCDEF		ABCDEF
!_			Match pattern: 		ABC*		*CD*		A*F
!_			Replace pattern:  	XYZ*		*X*		F*A
!_			Translated string:	XYZDEF		ABXEF		FBCDEA
!_
!--------------------------------------------------------------------------------*/

int 	ReplaceString ( mat_pat, rep_pat, src, dest, max_len )
PATTERN_INFO 	*mat_pat;
PATTERN_INFO 	*rep_pat;
char 	*src;
char 	*dest;
int 	max_len;
{
   	int 	return_code;
   	int 	search_off;
   	int 	len_1;
   	char 	*ls_str;
   	int 	ls_len;
   	char 	*rs_str;
   	int 	rs_len;

	switch ( mat_pat->type ) 
	{
	case 	NO_STAR:
/*_		IF match left string not equal to source string */
		if ( strcmp ( mat_pat->left_str, src ) ) 
		{
       			return_code 	= NO_MATCH;
      	}
		else 
		{
/*_			Set left and right strings to NULL */
         		ls_str 		= NULL;
         		ls_len 		= 0;
         		rs_str 		= NULL;
         		rs_len 		= 0;
         		return_code 	= MATCH;
      	}
      	break;

	case 	RIGHT_STAR:
/*_		IF match left string not equal to source left string */
      	if ( strncmp ( mat_pat->left_str, src, mat_pat->left_len)) 
		{
         		return_code 	= NO_MATCH;
      	}
      	else 
		{
/*_			Set left string to match left string */
         		ls_str 		= src+mat_pat->left_len;
         		ls_len 		= strlen(ls_str);

/*_			Set right string to NULL */
         		rs_str 		= NULL;
         		rs_len 		= 0;

         		return_code 	= MATCH;
      	}
      	break;

	case 	LEFT_STAR:
      		len_1 	= strlen(src);
/*_		IF match right string matches right end of source string */
      		if ( len_1 >= mat_pat->right_len && 
           		strcmp( mat_pat->right_str, src + len_1 - mat_pat->right_len) == 0 ) 
		{
/*_			Set left string to unmatched portion of source string */
         		ls_str 	= src;
        		ls_len 	= len_1 - mat_pat->right_len;

/*_			Set right string to NULL */
         		rs_str 	= NULL;
         		rs_len 	= 0;
         		return_code 	= MATCH;
      		}
      		else 
		{
         		return_code 	= NO_MATCH;
      		}
      		break;

   	case 	LEFT_RIGHT_STAR:
      		len_1 	= strlen(src);

      		return_code 	= NO_MATCH;

/*_		Search source string for occurrence of match left string */
      		for ( search_off=0; len_1 >= mat_pat->left_len+search_off; ++search_off ) 
			{
/*_			IF occurrence found */
         		if ( strncmp ( src+search_off, mat_pat->left_str, mat_pat->left_len ) == 0)
				{
/*_				Set left string to portion of source left of match */
            			ls_str 	= src;
            			ls_len 	= search_off;

/*_				Set right string to portion of source right of match */
 	           		rs_str 	= src + search_off + mat_pat->left_len;
        	    		rs_len 	= strlen(rs_str);
            			search_off 	= len_1;
 	           		return_code 	= MATCH;
         		} 
      		}
      		break;

   	case 	MID_STAR:
      		len_1 	= strlen(src);

/*_		IF match right string matches right of source string */
      		if ( len_1 >= mat_pat->right_len + mat_pat->left_len && 
           		strcmp( mat_pat->right_str, 
       		    	src + len_1 - mat_pat->right_len) == 0 ) 
			{
         		return_code 	= MATCH;
      		}
      		else 
			{
         		return_code 	= NO_MATCH;
      		}

/*_		IF right match found */
      		if ( return_code == MATCH ) 
			{
/*_			IF match left string doesn't match left of source string */
         		if ( strncmp ( mat_pat->left_str, src, mat_pat->left_len)) 
				{
            			return_code 	= NO_MATCH;
         		}
         		else 
				{
/*_				Set left string to unmatched center of source string */
            			ls_str 	= src+mat_pat->left_len;
            			ls_len 	= len_1 - mat_pat->left_len - mat_pat->right_len;

/*_				Set right string to NULL */
        	    		rs_str 	= NULL;
            			rs_len 	= 0;
         		}
      		}
      		break;
     
   		default:
/* 			return_code 	= SOFTWARE_ERR;
     */ 		return_code 	= 0;
   	}

   	if ( return_code == MATCH ) 
	{
   	   	switch ( rep_pat->type ) 
		{
   		case 	NO_STAR:
/*_			Set output string to left replace string */
         		if ( rep_pat->left_len <= max_len ) 
				{
           		 	strcpy (dest, rep_pat->left_str);
         		}
         		else 
				{
/*
           			return_code = RPLC_SIZE_ERR;
         */		}
         		break;

      		case 	RIGHT_STAR:
/*_			Set output string to left replace string + left string */ 
         		if ( rep_pat->left_len+ls_len <= max_len ) 
				{
           			strncpy (dest,rep_pat->left_str, rep_pat->left_len);
           			strncpy (dest+rep_pat->left_len, ls_str, ls_len);
           		 	*(dest+rep_pat->left_len+ls_len) 	= '\0';
         		}
         		else 
				{
        /*    			return_code 	= RPLC_SIZE_ERR;
         */		}
         		break;

      		case 	LEFT_STAR:
/*_			Set output string to left string + right replace string */
         		if ( rep_pat->right_len+ls_len <= max_len ) 
				{ 
            			strncpy (dest, ls_str, ls_len);
            			strcpy (dest+ls_len, rep_pat->right_str); 
         		}
         		else 
				{
           /* 			return_code 	= RPLC_SIZE_ERR;
         */		}
         		break;

      		case 	LEFT_RIGHT_STAR:
/*_			Set output string to left string */
/*_				 + left replace string + right string */
         		if ( ls_len+rep_pat->left_len+rs_len <= max_len ) 
				{
 	           		strncpy (dest, ls_str, ls_len);
        	    		strncpy (dest+ls_len, rep_pat->left_str, rep_pat->left_len);
            			strcpy (dest+ls_len+rep_pat->left_len, rs_str); 
         		}
         		else 
				{
          /*  			return_code 	= RPLC_SIZE_ERR;
         */		}
         		break;

      		case 	MID_STAR:
/*_			Set output string to left replace string */
/*_				+ left string + right replace string */
         		if ( rep_pat->left_len+ls_len+rep_pat->right_len <= max_len ) 
				{
            			strncpy (dest, rep_pat->left_str, rep_pat->left_len); 
            			strncpy (dest+rep_pat->left_len, ls_str, ls_len);
            			strcpy (dest+rep_pat->left_len+ls_len, rep_pat->right_str);
         		}
         		else 
				{
        /*    			return_code 	= RPLC_SIZE_ERR;
         */		}
         		break;

      		default:
        /* 		return_code 	= SOFTWARE_ERR;
*/
				;
      		}
	}

   	if ( return_code == NO_MATCH ) 
	{
/*_		Set output string to source string */
      		if ( strlen(src) <= (unsigned) max_len ) 
			{
         		strcpy (dest, src);
      		}
      		else 
			{
        /* 		return_code 	= RPLC_SIZE_ERR;
      	*/	}
   	}
  
   	return 	return_code;
}


/*--------------------------------------------------------------------------------
!_		4.x		MatchString - Test String Against Pattern
!_
!_		4.x.1	Module Function
!_				---------------
!_		This module compares a string against a pattern specification, which may
!_		contain wildcard characters (*).  The module returns a status indicating
!_		whether the string matches the pattern.
!_
!_		Examples:
!_			String		ABCDEF	ABCDEF	ABCDEF	ABCDEF	ABCDEF	ABCDEF
!_			Pattern		ABCDEF	A*F		ABC*	*A		*C*		*AF*
!_			Match		Yes		Yes		Yes		No		Yes		No
!_
!--------------------------------------------------------------------------------*/

int 	MatchString ( pat_info, str )
PATTERN_INFO 	*pat_info;
char 		*str;
{
	int 	return_code;
   	int 	len_str; 
   	int 	search_off;

	return_code = NO_MATCH;

	switch ( pat_info->type ) 
	{
	case 	NO_STAR:
/*_		IF source string equal to pattern string */
      		if ( strcmp( pat_info->left_str, str ) == 0 ) 
		{
         		return_code = MATCH;
      		}
      		break;

   	case 	RIGHT_STAR:
/*_		IF pattern left string matches left of source string */
      		if ( strncmp( pat_info->left_str, str, pat_info->left_len) == 0) 
		{
         		return_code = MATCH;
      		}
      		break;

   	case 	LEFT_STAR:
      		len_str = strlen(str);
/*_		IF pattern right string matches right of source string */
      		if ( len_str >= pat_info->right_len && 
           		strcmp( pat_info->right_str, 
       			str + len_str - pat_info->right_len) == 0 ) 
		{
         		return_code = MATCH;
      		}
      		break;

   	case 	LEFT_RIGHT_STAR:
      		len_str = strlen(str);
/*_		Search source string for occurrence of pattern left string */
      		for ( search_off=0; len_str >= pat_info->left_len+search_off; ++search_off ) 
		{
/*_			IF occurrence found */
         		if ( strncmp ( str+search_off, pat_info->left_str, pat_info->left_len ) == 0) 
			{
            			search_off = len_str;
       		     		return_code = MATCH;
         		} 
      		}
      		break;

   	case 	MID_STAR:
      		len_str = strlen(str);
/*_		IF source length >= length of right plus left pattern strings */
/*_			AND pattern right string matches right of source string */
/*_			AND pattern left string matches left of source string */
      		if ( len_str >= pat_info->right_len + pat_info->left_len && 
           		strcmp( pat_info->right_str, str + len_str - pat_info->right_len) == 0 &&
         		strncmp( pat_info->left_str, str, pat_info->left_len) == 0) 
		{
         		return_code = MATCH;
      		}
      		break;
      
   		default:
/*
      		return_code = SOFTWARE_ERR;
   */
		;
	}

   	return return_code;
}
#define  MAX_FIND_NUM   2 
char *find_type[MAX_FIND_NUM] = 
{
		"DataBaseName",	//REF_BADGE
		"DspFileName" ,	//REF_STATION

};

int  iFindType =0  ; 
 
BOOL CALLBACK FindDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
	int		i;
	HWND	hlist;
	 
	char srch_str[256] ;	
    switch(wMsg)
    {
	case WM_INITDIALOG:
		chg_cnt 	= 0;
		rplc_error 	= 0;
		
		hlist = GetDlgItem(hDlg, IDC_FIND_TYPE);
		for( i=0; i<MAX_FIND_NUM; i++ )
			SendMessage(hlist, CB_ADDSTRING, 0, (LPARAM)find_type[i]);
		SendMessage(hlist, CB_SETCURSEL, iFindType, 0);
		
		CenterWindow(hDlg, ghWnd);
		return TRUE;
		
	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDC_FIND_TYPE:
			switch( HIWORD(wParam) )
			{
			case CBN_SELCHANGE:
				hlist = GetDlgItem(hDlg, IDC_FIND_TYPE);
				iFindType = SendMessage(hlist, CB_GETCURSEL,0,0);
				return TRUE;
			default:	
				break;
			}
			break;
			case IDOK:
				memset(srch_str, 0, sizeof(srch_str));
				/*_		Get search pattern text */
				GetDlgItemText(hDlg, IDC_FIND, srch_str, sizeof(srch_str));
			 	FindReference (hDlg, srch_str, iFindType);
			case IDCANCEL:
				EndDialog(hDlg, wParam);
				return TRUE;
		}
		break;
		
    }
    return FALSE;
}




void FindReference (HWND hdlg, char *srch_pat, int ref_type)
{
	int	i , menu_id;
	char	pat_name[10];
	char    msgInfo[256] ;
    int  find_num ; 
	find_num = 0 ; 
	switch (ref_type) 
	{
	case    0  :    //	DB_ENTRY_NAME:
		for (i=0; i<v4_gdbase.nextpos; i++)
		{
			NEW_GBRCD	*rcd;
			
			rcd	= &v4_gdbase.buffer[i];
			if (  strcmp (rcd->lnk.dbname,"")!=0  )
				
			{
				if (strstr(rcd->lnk.dbname,srch_pat))
				{
					rcd->bck.tag  =    ITEM_TAG_ON;			
					DrawHandle (ghWnd, &rcd->bck ,3);
					find_num++  ; 
				}
			 
			}
		}
		break;
		
	case  1 :  //	DSP_FILE_NAME:     /*added by chichi*/
		
		for (i=0; i<v4_gdbase.nextpos; i++)
		{
			NEW_GBRCD	*rcd;
			
			rcd	= &v4_gdbase.buffer[i];
			
			//wzg if (strcmp(rcd->lnk.act.fmt_def.menu.name,"" )==0)
			if (strcmp(rcd->lnk.act.fmt_def.dsp.name,"" )==0)
				continue ;
			if (strcmp(rcd->lnk.act.fmt_def.dsp.name ,srch_pat   ) ==0  )
			{
				rcd->bck.tag  =    ITEM_TAG_ON;			
				DrawHandle (ghWnd, &rcd->bck ,3);
				find_num++  ;  

			}
								
		}
		break;
		
	default:
		break;
		
	}
	sprintf(msgInfo,"本次查询共计找到了%d个内容!!!" , find_num) ; 
	MessageBox (ghWnd, msgInfo, "IPM", MB_OK); 
	
}



