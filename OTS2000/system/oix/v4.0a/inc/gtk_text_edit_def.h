/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Cate : 2005.9.17 
	SYN  : 2008.4.21
     

---------------------------------------------------------------------*/




#ifndef _GTK_TEXT_EDIT_DEF_H_ 
#define _GTK_TEXT_EDIT_DEF_H_



#include<gdk/gdk.h>

#define	DEFAULT_FONT		0

#ifdef SUN
#define DEFAULT_LOCALE    "zh_CN.GBK" 
#else
#define	DEFAULT_LOCALE	  "zh_CN.dechanzi"
#endif

#define MAX_BYTES_I18N_CHAR     4
#define	INSERT			0
/*#define DELETE			1  */
#define FINISH			2

#define	MAX_FONTSET		10

GdkFont* font_set[MAX_FONTSET]; 
 
int	key_mode;

int     text_buff_len;
char    *buff_string;

int	modify_x, modify_y,  modify_loop;
int     start_x, start_y;
int	sys_font_width;
int	sys_font_height;



long    im_ev_mask;


#ifdef	DEF_MODULE 

char    *font_name[]    = {
              "-dt-kai-medium-r-normal--16-140-75-75-m-140-gbk-0,-b&h-lucidatypewriter-medium-r-normal-asian-14-140-75-75-m-70-iso8859-1 ",

                "-dt-kai-medium-r-normal--16-140-75-75-m-140-gbk-0,-adobe-times-medium-r-normal--17-120-100-100-p-84-iso8859-1",
                   
                "-dt-kai-medium-r-normal--18-160-75-75-m-160-gbk-0,-adobe-times-medium-r-normal--20-140-100-100-p-96-iso8859-1",
                  
                "-dt-kai-medium-r-normal--22-200-75-75-m-200-gbk-0, -adobe-times-medium-r-normal--25-180-100-100-p-125-iso8859-1",
                    
                "-dt-kai-medium-r-normal--26-240-75-75-m-240-gbk-0,-adobe-times-medium-r-normal--34-240-100-100-p-170-iso8859-1" ,

              "-dt-kai-medium-r-normal--16-140-75-75-m-140-gbk-0,-b&h-lucidatypewriter-medium-r-normal-asian-14-140-75-75-m-70-iso8859-1 ",

                "-dt-kai-medium-r-normal--16-140-75-75-m-140-gbk-0,-adobe-times-medium-r-normal--17-120-100-100-p-84-iso8859-1",
                   
                "-dt-kai-medium-r-normal--18-160-75-75-m-160-gbk-0,-adobe-times-medium-r-normal--20-140-100-100-p-96-iso8859-1",
                  
                "-dt-kai-medium-r-normal--22-200-75-75-m-200-gbk-0, -adobe-times-medium-r-normal--25-180-100-100-p-125-iso8859-1",
                    
                "-dt-kai-medium-r-normal--26-240-75-75-m-240-gbk-0,-adobe-times-medium-r-normal--34-240-100-100-p-170-iso8859-1"
                };





#else
extern	char	*font_name[];
#endif  



#endif
