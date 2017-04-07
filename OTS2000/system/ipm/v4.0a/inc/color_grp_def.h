#define MAX_COLOR_GRPS		64	/* max COLOR groups */
#define GRP_MAX_COLORS		16	/* max COLORS in a string group */
#define COLOR_GRP_NAME_LEN	20
#define CGRP_CELL_NAME_LEN	20
#define COLOR_GRP_WIDTH        	32
#define COLOR_GRP_HEIGHT       	32

#define	MAX_COLOR_PAIR		64

#define	CGED_new        	0
#define CGED_edit       	1
#define CGED_save       	2
#define CGED_save_as    	3
#define CGED_save_file  	4
#define CGED_quit       	5

typedef	struct
{
	int	clr0;
	int	clr1;
}	COLOR_PAIR;


#ifdef	COLOR_GROUP_DEF
COLOR_PAIR	clr_pair[MAX_COLOR_PAIR]	={
	BLACK1, BLACK2, 	BLACK1, BLACK3, 	BLACK1,	BLACK4, 	BLACK2,	BLACK4,
	GRAY1,	GRAY2, 		GRAY1,	GRAY3, 		GRAY1,	GRAY4, 		GRAY2,	GRAY4,
	GREEN1,	GREEN2, 	GREEN1,	GREEN3, 	GREEN1,	GREEN4, 	GREEN2,	GREEN4,
	L_GRN1,	L_GRN2, 	L_GRN1,	L_GRN3, 	L_GRN1,	L_GRN4, 	L_GRN2,	L_GRN4,
	BLUE1,	BLUE2, 		BLUE1,	BLUE3, 		BLUE1,	BLUE4, 		BLUE2,	BLUE4,
	L_BLUE1,L_BLUE2, 	L_BLUE1,L_BLUE3, 	L_BLUE1,L_BLUE4, 	L_BLUE2,L_BLUE4,
	RED1,	RED2, 		RED1,	RED3, 		RED1,	RED4, 		RED2,	RED4,
	L_RED1,	L_RED2, 	L_RED1,	L_RED3, 	L_RED1,	L_RED4, 	L_RED2,	L_RED4,
	YLW1,	YLW2, 		YLW1,	YLW3, 		YLW1,	YLW4, 		YLW2,	YLW4,
	L_YLW1,	L_YLW2, 	L_YLW1,	L_YLW3,	 	L_YLW1,	L_YLW4, 	L_YLW2,	L_YLW4,
	PINK1,	PINK2, 		PINK1,	PINK3, 		PINK1,	PINK4, 		PINK2,	PINK4,
	PURPLE1,PURPLE2, 	PURPLE1,PURPLE3, 	PURPLE1,PURPLE4, 	PURPLE2,PURPLE4,
	CHOCLT1,CHOCLT2, 	CHOCLT1,CHOCLT3, 	CHOCLT1,CHOCLT4, 	CHOCLT2,CHOCLT4,
	L_CHOCLT1,L_CHOCLT2, 	L_CHOCLT1,L_CHOCLT3, 	L_CHOCLT1,L_CHOCLT4, 	L_CHOCLT2,L_CHOCLT4,
	SEA_BLUE1,SEA_BLUE2, 	SEA_BLUE1,SEA_BLUE3, 	SEA_BLUE1,SEA_BLUE4, 	SEA_BLUE2,SEA_BLUE4,
	LEM_CHFN1,LEM_CHFN2, 	LEM_CHFN1,LEM_CHFN3, 	LEM_CHFN1,LEM_CHFN4, 	LEM_CHFN2,LEM_CHFN4
};
#else
extern	COLOR_PAIR	clr_pair[MAX_COLOR_PAIR];
#endif


typedef struct
{
	int			name;
	int			c_normal;
	int			flash;
}	CGRP_CELL;

typedef struct
{
	char		name[COLOR_GRP_NAME_LEN];
	CGRP_CELL	cgrp_cell[GRP_MAX_COLORS];
}	COLOR_GRP;

CGRP_CELL		cgrp_cell;
COLOR_GRP		cgdb[MAX_COLOR_GRPS];
COLOR_GRP    		cg_buff;
int			cgdb_cnt;


//XmStringTable   cgrp_name_list;

int     CGED_canvas_width, CGED_canvas_height;
int     CGED_xn, CGED_yn;
int     CGED_clr_width, CGED_clr_height;
int     cgrp_edit_mode;

#define ADD_NEW_COLOR_GRP              0
#define EDIT_EXISTING_COLOR_GRP        1
#define DELETE_COLOR_GRP               2
#define COPY_EXISTING_COLOR_GRP        3

#define C_DYN_SOLID_ON	1	
#define C_DYN_SOLID_OFF	0	
#define C_DYN_FLASH_ON	1
#define C_DYN_FLASH_OFF 0	

int	c_solid_type;
int	c_flash_type;

HWND ghWndCGrpEdit;
BOOL CALLBACK ColorGroupEditHandler(HWND, UINT, WPARAM, LPARAM);

