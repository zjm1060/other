

#define	DEFAULT_FONT		2
#define	DEFAULT_LOCALE		"zh_CN.dechanzi"
#define MAX_BYTES_I18N_CHAR     4
#define	INSERT			0
//#define DELETE			1
#define FINISH			2

#define	MAX_FONTSET		10
HFONT	font_set[MAX_FONTSET];
HFONT	prt_font_set[MAX_FONTSET];	 //used by printer

int	key_mode;
char    text_buff[MAX_TEXT_LEN+5];
int     text_buff_len;
char    *buff_string;

int	modify_x, modify_y,  modify_loop;
int     start_x, start_y;
int	sys_font_width;
int	sys_font_height;

long    im_ev_mask;

typedef	struct
{
//	int	width;
	int	height;
}	FontSize;

#ifdef	DEF_MODULE
FontSize	font_size[MAX_FONTSET] = {
		{ 10},
		{ 15},
		{ 20},
		{ 25},
		{ 30},
		{ 36},
		{ 45},
		{ 55},
		{ 65},
		{ 75}   //34
};	// RS_Song	 Chinese WindowNT 1280*1024



char *font_string[MAX_FONTSET] = {
		"F*10",
		"F*15",
		"F*20",
		"F*25",
		"F*30",
		"F*36",
		"F*45",
		"F*55",
		"F*65",
		"F*75",
 
};



/*

FontSize	font_size[MAX_FONTSET] = {
		{8, 13},
		{13, 13},
		{16, 16},
		{19, 19},
		{26, 26}	  
};   // RS_Song		LiFangHanZi  1024*768
FontSize	font_size[MAX_FONTSET] = {
		{8, 16},
		{16, 16},
		{16, 16},
		{24, 24},
		{34, 34}	  
};   // RS_Song	   LiFangHanZi	  1280*1024
FontSize	font_size[MAX_FONTSET] = {
		{8, 13},
		{13, 13},
		{14, 14},
		{19, 19},
		{26, 26}
}; */ 	 //	ËÎÌו	LiFangHanZi



#else
extern	FontSize	font_size[MAX_FONTSET];
extern	char *font_string[MAX_FONTSET];
#endif

