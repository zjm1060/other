

#include <stdio.h>
#include <string.h>

#include	"../inc/ipm.h"
#include	"../inc/tmplt.h"
#include	"../src/resource.h"


char	src_file_name[4][MAX_FILE_NAME_LEN];
FILE	*src_fid[4];
int	line_num[4];
int	src_level;
int	cur_line;
char	in_line[80];
char type_names[FLD_TYPE_CNT][FLD_TYPE_NAME_LEN] = 
			{ "OPT_MBR", "OPT_GRP", "BOOL","TEXT"};


void CheckFldType( char *, int *);


void ExitProc();
void print_loc();
void early_eof();
void error_exit(char *error_msg);
int get_next_line();
void FindTmpltBeginLine(char *);
void FindFldBeginLine(char *);
void FindItemBeginLine(char *);
extern  PRJDEF  prj ; 
char    curfile[30];

BOOL APIENTRY Prj_proc(HWND hDlg,UINT message, UINT wParam, LONG lParam)
{

	HWND  hcombo ;
	int i; 
	hcombo = GetDlgItem(hDlg,  IDC_CMB_PRJ  );
    switch (message){
        case WM_INITDIALOG:

				
				 for (i = 0 ; i< prj.nextprj; i++)
				 {
					 SendMessage(hcombo,CB_ADDSTRING,0,(LPARAM)(LPSTR) prj.project[i].Longname );
					 if (!strcmp(prj.project[i].name ,curfile))
						 prj.curprj =  i ;
				 }
				 SendMessage(hcombo,CB_SETCURSEL, prj.curprj,0 );   
				 return(TRUE);
        case WM_COMMAND:
         
            if (LOWORD(wParam) == ID_DOK){
                EndDialog(hDlg,1);
                return(TRUE);
			}else if (LOWORD(wParam) == ID_DCANCEL){
                EndDialog(hDlg,0);
                return(TRUE);
            }
            else if (HIWORD(wParam) == CBN_SELCHANGE )
			{
				 	prj.curprj  = SendMessage(hcombo,CB_GETCURSEL,0,0);
			}


            break;
    }
    return(FALSE);
        UNREFERENCED_PARAMETER(lParam);
}


//added by chi 

int    ReadPrjDef(PRJDEF *prj)
{
		char	*prj_file_name	= {"/home/ems/project/project.def"};
		char	*prj_file_ini	= {"/home/ems/project/project.ini"};
	    char 	str[300];
	
		int parm_cnt ,res; 
		FILE  *fp1 ,*fp2  ;
		char  *temp ;
		char  parm[4][30];
 
	 prj->nextprj = 0 ;
	 prj->curprj  = 0 ;
	/*	Open def file for reading */
	 if ( !(fp1 = fopen(prj_file_ini,"r")) )
 	{
 		sprintf(str, "Could not open file %s for reading\n",prj_file_ini);
        error_exit(str);
    }
    if ( fgets(str,sizeof(str),fp1) != NULL)
	{
		parm_cnt = sscanf(str,"%s %s %s %s",parm[0],parm[1], parm[2],parm[3]);
		strcpy(curfile,parm[3]);
	}


    if ( !(fp2 = fopen(prj_file_name,"r")) )
 	{
 		sprintf(str, "Could not open file %s for reading\n",prj_file_name);
        error_exit(str);
    }
    
	do 
		{
			temp = fgets(str,sizeof(str),fp2);
			if (strstr(temp,"PROJECT") != NULL) 
			{
			       parm_cnt = sscanf(str,"%s %s %s %s",parm[0],parm[1], parm[2],parm[3]);
                   strcpy(prj->project[prj->nextprj].name, parm[3]); 
				  
			}
			else if (strstr(temp,"LONGNAME" ) != NULL)  
			{
				  parm_cnt = sscanf(str,"%s %s %s",parm[0],parm[1], parm[2]);
				  strcpy(prj->project[prj->nextprj].Longname , parm[2]); 
				   prj->nextprj ++ ;
			}
			else if (strstr(temp,"END" ) != NULL)
				break;
		} while (temp != NULL);
        fclose(fp1) ;
		fclose(fp2)  ;
        res = DialogBox (ghInst, MAKEINTRESOURCE(DLG_PROJECT),
                              ghWnd,
                              (DLGPROC)Prj_proc) ;
		return  res ; 

}




void	ReadTmpltFile() 
{
	int	i;
	int	tmplt_cnt;
	int	tmplt_size;
	char	tmplt_name[TMPLT_NAME_LEN];
	char	fld_type_name[FLD_TYPE_NAME_LEN];
	char	fld_name[FLD_NAME_LEN];
	char	item_name[FLD_NAME_LEN], item_val_str[2];
	int	parm_cnt, min_cnt, fld_cnt , item_cnt;
	int	item_val;
	int	format_num;
	int	x, y, width, height, dflt_value;
	int	grp_type, fld_type, text_req_type;
	TMPLT	cur;
	char	*tmplt_file_name	= {"/tmplt/ipm.tpl"};
	char 	str[300];

/*_	Create source and binary file names */
	FormFullName(home, src_file_name[0], tmplt_file_name ); 

/*_	Open source file for reading */
    if ( !(src_fid[0] = fopen(src_file_name[0],"r")) )
 	{
 		sprintf(str, "Could not open file %s for reading\n",src_file_name[0]);
        error_exit(str);
    }
    	
	tmplt_cnt = 0;
	cur_line = 0;
			
/*_	DO until last source line reached */
	while ( get_next_line() ) 
	{
			
/*_		IF maximum number of templates already compiled */
		if (tmplt_cnt == MAX_TMPLTS) 
		{
	
/*_			Exit with error */
			print_loc();
        	strcpy(str, "Maximum number of templates (50) exceeded.");
			error_exit(str);
		}
/*_		ENDIF maximum number of templates already compiled */

/*_		Read template name, format, fld_cnt, net_fld_ptr */
		parm_cnt = sscanf(in_line,"%s %d %d %d",tmplt_name,&format_num, &width,&height);
		if (parm_cnt < 3) {
			print_loc();
			error_exit("Missing fld in template definition");
		}
		strcpy(cur.tmplt_name, tmplt_name);
		cur.format = format_num;
		cur.fld_cnt	= fld_cnt;
		cur.width	= width;
		cur.height	= height;

/*_		Check for openning bracket of template definition */
		FindTmpltBeginLine(tmplt_name);
		
/*_		Initialize fld count to zero */
		fld_cnt = 0;

		if ( !get_next_line() ) early_eof();
/*_		IF not closing bracket of template def */
		while ( !strchr(in_line,'}') ) 
		{
				
/*_			IF not closing bracket of templt def */
/*_			Scan in common fld data */
			if (!in_line[0]) 
				continue ;  
			parm_cnt = sscanf(in_line,"%s %s %d %d %d",fld_type_name,fld_name,&x,&y,&width);
			
/*_			Try to match fld type name */
			for (i=0; i < FLD_TYPE_CNT; i++)
 			{
				if ( strncmp(fld_type_name,type_names[i],FLD_TYPE_NAME_LEN) == 0 ) 
				{
					fld_type = i+1;
					break;
				}
			}
			if ( i == FLD_TYPE_CNT ) 
			{
				print_loc();
				sprintf(str, "Unknown fld type (%s) in template %s\n",fld_type_name,tmplt_name);
				error_exit(str);
			}
			min_cnt = 5;
			if (parm_cnt < min_cnt) 
			{
				 print_loc();
				 sprintf(str, "Missing data in fld %s of template %s\n",fld_name,tmplt_name);
				 error_exit(str);
			}

			strcpy(cur.fld_buff[fld_cnt].fld_type_name, fld_type_name);
			strcpy(cur.fld_buff[fld_cnt].fld_name, fld_name);
			cur.fld_buff[fld_cnt].x = x;
			cur.fld_buff[fld_cnt].y = y;
			cur.fld_buff[fld_cnt].width = width;	

			FindFldBeginLine(fld_name);
			while ( !strchr(in_line,')') ) 
			{
				if ( !get_next_line() ) early_eof();
/*_				DO CASE fld type */
				switch(fld_type) 
				{
				
				case FLD_TYPE_OPT_GRP:			
					parm_cnt = sscanf(in_line,"%d ", &grp_type);
					if (parm_cnt <1 ) 
					{
						print_loc();
						sprintf(str, "Missing group ID in fld %s of template %s\n",fld_name,tmplt_name);
						error_exit(str);
					}

					cur.fld_buff[fld_cnt].fld_def.opt_grp_fld.grp_type = grp_type;
					if ( !get_next_line() ) early_eof();
				break;			

/*				case FLD_TYPE_BOOL:
					parm_cnt = sscanf(in_line,"%s %s %d %d %d %d %d",fld_type_name,fld_name,&size,
										&x,&y,&width,&dflt_value);
				break;
*/
				case FLD_TYPE_OPT_MBR:				
					item_cnt =0;
					parm_cnt = sscanf(in_line,"%d ", &dflt_value); 
					cur.fld_buff[fld_cnt].fld_def.opt_mbr_fld.dflt_value =dflt_value;

/*_					Read fld definition line */
					FindItemBeginLine(fld_name);		
/*_					DO while not closing bracket of equate list */
					if ( !get_next_line() ) early_eof();
					while ( !strchr(in_line,']') ) 
					{
/*_						Scan in equate item */
						parm_cnt = sscanf(in_line,"%s %s", item_name, item_val_str);
				
						if (parm_cnt < 2) 
						{
							print_loc();
							sprintf(str, "Incomplete item in fld %s\n",fld_name);
							error_exit(str);
						}

						item_val = atoi( item_val_str );
						
/*_						Add item to fld def */
						strcpy(cur.fld_buff[fld_cnt].fld_def
								.opt_mbr_fld.list_name[item_cnt], item_name);
						cur.fld_buff[fld_cnt].fld_def
								.opt_mbr_fld.list_value[item_cnt]= item_val;
						item_cnt++;
						if ( !get_next_line() ) early_eof();
					}
/*_					ENDDO while not close of opt_mbr list']' */
					cur.fld_buff[fld_cnt].fld_def.opt_mbr_fld.list_cnt = item_cnt;

/*_					Must have at least one item in equate list */
					if (item_cnt == 0) 
					{
						print_loc();
						sprintf(str, "Empty item list in fld %s\n",fld_name);
						error_exit(str);
					}
					if ( !get_next_line() ) early_eof();
				break;

				case FLD_TYPE_TEXT:				
					parm_cnt = sscanf(in_line,"%d " ,&text_req_type);
					cur.fld_buff[fld_cnt].fld_def.text_fld.req_type= text_req_type;
					if ( !get_next_line() ) early_eof();
				break;
				
				}
			}
		fld_cnt++;
/*_		Read fld definition line */
		if ( !get_next_line() ) early_eof();
		}
		
/*_	Put fld count in template def */
	cur.fld_cnt = fld_cnt;
	tmplt_size =sizeof(cur); 
	tmplt_buff.tmplt[tmplt_cnt]= cur;
//	BlankFillCopy (tmplt_nm_ptrs[tmplt_cnt].name, cur.tmplt_name, TMPLT_NAME_LEN);
	strcpy(tmplt_nm_ptrs[tmplt_cnt].name, cur.tmplt_name);
	tmplt_nm_ptrs[tmplt_cnt].format_num = cur.format;
	tmplt_cnt++;
	tmplt_buff.nextpos= tmplt_cnt;
	}
	
	if (!tmplt_cnt) 
	{
		error_exit("No template definitions found.");
	}
	tmplt_def_cnt = tmplt_cnt;
	fclose(src_fid[0]);
}
	
int get_next_line()
{
	char	*temp;
	char	*open_q,*close_q;
	FILE	*inc_fid;
	char	str[300];

	do 
	{
		do 
		{
			temp = fgets(in_line,sizeof(in_line),src_fid[src_level]);
			if (temp == NULL) 
			{
				fclose(src_fid[src_level--]);

				if (src_level < 0)
					return 0;

				cur_line = line_num[src_level];
			}
			else 
			{
				cur_line++;
			}
		} while (temp == NULL);

		if (in_line[0] == '#') 
		{
			if (memcmp(in_line,"#include",8) == 0) 
			{
				open_q = strchr(in_line,'\"');
				close_q = strrchr(in_line,'\"');

				if (open_q != NULL && close_q - open_q > 1) 
				{
					*close_q = 0;

   					if ( (inc_fid = fopen(&open_q[1],"r")) ) 
					{
						line_num[src_level] = cur_line;
						cur_line = 0;
						src_fid[++src_level] = inc_fid;
						strcpy(src_file_name[src_level],&open_q[1]);
					}
					else 
					{
						print_loc();
    					sprintf(str, "Could not open include file %s for reading.",&open_q[1]);
						MessageBox(NULL, str, NULL, MB_OK);
					}
				}
				else 
				{
					print_loc();
					MessageBox(NULL, "Invalid file reference in #include statement.", NULL, MB_OK);
				}
			}
			else 
			{
				print_loc();
				sprintf(str, "Unknown compiler command: %s\n",in_line);
				MessageBox(NULL, str, NULL, MB_OK);
			}
		}

	} while (in_line[0] == '/' || in_line[0] == '\n' || in_line[0] == '#');
	
	return 1;
}

void print_loc()
{
	char str[300];
	sprintf(str, "%s : line %d - ",src_file_name[src_level],cur_line);
	MessageBox(NULL, str, NULL, MB_OK);
}

void early_eof()
{
	MessageBox(NULL, "Unexpected end of file", NULL, MB_OK);
	ExitProc();
    PostQuitMessage (0) ;
}

void error_exit(char *error_msg)
{ 
	MessageBox(NULL, error_msg, NULL, MB_OK);
	ExitProc();
    PostQuitMessage (0) ;
}

void FindTmpltBeginLine(tmplt_name)
char	*tmplt_name;
{	
	char str[300];

	if ( !get_next_line() ) early_eof();
	if ( !strchr(in_line,'{') ) 
	{
		print_loc();
		sprintf(str, "Missing open bracket in template %s.",tmplt_name);
		error_exit(str);
	}
}		


void FindFldBeginLine(fld_name)
char	*fld_name;
{	
	char str[300];

	if ( !get_next_line() ) early_eof();
	if ( !strchr(in_line,'(') ) 
	{
		print_loc();
		printf(str, "Missing open bracket in field %s.",fld_name);
		error_exit(str);
	}
}		

void FindItemBeginLine(fld_name)
char	*fld_name;
{	
	char str[300];

	if ( !get_next_line() ) early_eof();
	if ( !strchr(in_line,'[') ) 
	{
		print_loc();
		sprintf(str, "Missing open bracket in field %s.",fld_name);
		error_exit(str);
	}
}		


void WriteTmpltFile()
{
	int     i;
	int     nextpos;
	TMPLT   cur;
	int     fld_type;
	char 	str[300];

	nextpos = tmplt_buff.nextpos;
	cur = tmplt_buff.tmplt[1];
	printf("tmplt_name=%s\n ", cur.tmplt_name);
	printf("tmplt.format=%d, tmplt.fld_cnt=%d\n ",cur.format, cur.fld_cnt);
	printf("tmplt.width= %d, tmplt.height= %d \n",cur.width, cur.height);
	for(i=0; i<cur.fld_cnt; i++)
	{

/*              printf("fld_type_name=%s, fld_name=%s\n ",cur.fld_buff[i].fld_type_name,
                        cur.fld_buff[i].fld_name);
                printf("fld_x=%d, fld_y=%d, fld_width=%d\n",
                        cur.fld_buff[i].x, cur.fld_buff[i].y, cur.fld_buff[i].width);
*/      CheckFldType(cur.fld_buff[i].fld_type_name, &fld_type) ;
		switch  (fld_type)
		{
		case    FLD_TYPE_TEXT:
				sprintf(str, "req_type =%d\n", 
					cur.fld_buff[i].fld_def.text_fld.req_type);
				MessageBox(NULL, str, NULL, MB_OK);
			break;
		default:
			break;
		}
	}
}

