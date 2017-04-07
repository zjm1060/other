/* 
updated by zyp on 2006.3
SYN by hcl 2007.12.12 
*/



#define	MAX_OPR_COND_OBJECTS	200

#define	SBO_YES			1
#define	SBO_NO			0	
#define	SBO_RESULT_OK		1
#define	SBO_RESULT_FALSE	0
#define	NO_SBO_RESULT		-1	
#define	SBO_NAME_LEN	56


int     sub_dsp_box_width,sub_win_width[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
int     sub_dsp_box_height,sub_win_height[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
XRECT    sub_win[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
POINTER obj_pnt_tab_ptr[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
/*int     sub_win_no;*/
char    sub_dsp_name[MAX_FILE_NAME_LEN];
typedef	struct
{
	GBRCD	buffer[MAX_OPR_COND_OBJECTS];
	int	nextpos;
}	GCOND_BUFF;

GCOND_BUFF	gcondb[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
GtkWidget	*sub_dsp_box[MAX_CRT_NUMBER],*sub_dsp_canvas[MAX_CRT_NUMBER];
GtkWidget       *sub_win_shell[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER],*sub_win_canvas[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER];
char    sub_win_title[MAX_SUB_WIN_NUM][MAX_CRT_NUMBER][50];

short	sbo_result[MAX_CRT_NUMBER];
char	sbo_pnt_name[MAX_CRT_NUMBER][SBO_NAME_LEN];
char	sbo_longname[MAX_CRT_NUMBER][SBO_NAME_LEN];

