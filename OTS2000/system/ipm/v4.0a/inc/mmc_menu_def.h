

#define	MAX_MENU_OBJECTS	48	/* max draws in one control menu */
#define	MAX_MENUS		128	/* max control menus */


typedef struct
{
	char		name[MENU_NAME_LEN];
	GBRCD		rcd[MAX_MENU_OBJECTS];
	int		x1, y1, x2, y2;
	int		tag;
	int		nextpos;
}	MENU;

typedef struct
{
 	char		name[MENU_NAME_LEN];
	NEW_GBRCD		rcd[MAX_MENU_OBJECTS];
	int		x1, y1, x2, y2;
	int		tag;
	int		nextpos;
}	NEW_MENU;

typedef struct
{
	MENU		menu[MAX_MENUS];
	int		tag;
	int		nextpos;
}	MDBASE;

typedef struct
{
	NEW_MENU		menu[MAX_MENUS];
	int		tag;
	int		nextpos;
}	NEW_MDBASE;


MDBASE		menudb;
NEW_MDBASE	new_menudb;



GBRCD	*ctrl_rcd[MAX_CRT_NUMBER];
GBRCD   ctrl_act_rcd_found[MAX_CRT_NUMBER];
GBRCD	ctrl_rcd_parent[MAX_CRT_NUMBER];
GBRCD	ctrl_rcd_menu_p[MAX_CRT_NUMBER];
GBRCD	*tag_rcd[MAX_CRT_NUMBER];
int	ctrl_menu_id[MAX_CRT_NUMBER];

int	ctrl_menu_popup;		/*	set ON if any one crt has ctrl menu	*/
					/*	avoiding simultaneous control on multi-crt	*/

GBRCD	*ptr_tab_rcd[MAX_CRT_NUMBER];	/*       this is only for paramter table   */
int	ptr_tab_rcd_tag[MAX_CRT_NUMBER];
#define	PTR_TAB_RCD_PROHIBIT	1
#define	PTR_TAB_RCD_AVAILABLE	0


short	rpt_tag[MAX_CRT_NUMBER];
#define	RPT_TAG_ON		1
#define	RPT_TAG_OFF		0

