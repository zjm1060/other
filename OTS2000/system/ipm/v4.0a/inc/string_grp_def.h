
#define MAX_STR_GRPS		128	/* max string groups */
#define GRP_MAX_STRS		16	/* max strings in a string group */
#define STR_GRP_NAME_LEN	20
#define STR_LEN			16

typedef struct
{
	char		name[STR_GRP_NAME_LEN];
	char		str[STR_LEN][GRP_MAX_STRS];
	int		nextpos;
}	STR_GRP;

STR_GRP		strgdb[MAX_STR_GRPS];
int		strg_cnt;

int		str_grp_edit_mode;
#define		ADD_NEW_STR_GRP		0
#define		EDIT_EXISTING_STR_GRP	1
#define		DELETE_STR_GRP		2
#define		COPY_EXISTING_STR_GRP	3

#define	STGED_new	0
#define	STGED_save	1
#define	STGED_save_as	2
#define	STGED_delete	3
#define	STGED_save_file	4
#define	STGED_quit	5
#define	STGED_upload	6


//XmStringTable	str_grp_name_list;
HWND ghWndStrGrpEdit;
BOOL CALLBACK StringGroupEditHandler(HWND, UINT, WPARAM, LPARAM);

