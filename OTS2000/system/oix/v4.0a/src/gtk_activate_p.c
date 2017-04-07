
/**************************************************
2007.06.11 : have been SYNC  by chichi;
LASTUPDATE : 2007.06.11  by chi.hailong;
***************************************************/

#include <gtk/gtk.h>
#include	"../inc/oix.h"


#include <stdio.h>


#ifdef  WINDOWS_OS

#include <windows.h>  
#include <string.h>
#include <winspool.h>
#include <commdlg.h>
#pragma	comment(lib, "user32.lib")
#pragma	comment(lib, "gdi32.lib")
#pragma	comment(lib, "comdlg32.lib")

#endif	


#include	"../inc/gtk_ipm_k_def.h"
#include	"../inc/gtk_widgets.h"

#include	"../inc/gtk_alarm_tab.h"
extern int  iSetDispEntry[MAX_CRT_NUMBER]  ; 
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void gtk_main_win_quit( );


#ifdef  WINDOWS_OS
extern void PrintWinPic (HDC prtdc,int crt_n,char * bmpfilename);
extern char* get_temp_file_name(int crt_n);
extern int   WinSaveJPGFile(HWND hWnd, char* filename,int print_mode) ;
#endif
/************************************************************************/
/*	add by hcl 2007.8.30												*/
/*	for win32 print                                                     */
/************************************************************************/

#include <math.h>
#include <pango/pangocairo.h>
#include "cairo.h"
#include <string.h>
#include <stdio.h>

typedef struct 
{
	gchar *Title[50];
	gchar *filename;
	
	gdouble font_size;
	
	gint lines_per_page;  
	gchar **lines;
	gint num_lines;
	gint num_pages;
} PrintData;
#define HEADER_HEIGHT (10*72/25.4)
#define HEADER_GAP  (40*72/25.4)	/* // 控制横线下第一行输入的位置*/
#define LEFT_START  50				/* // 设置左侧开始位置*/
#define LINESPACE   10				/*控制行间距的系数*/
#define FONT_SIZE   10.0
#define PER_PAGE     52             //每页打印52行 

int PrintTitleLocation[8]={PRINTTITLELOCATION_L0,PRINTTITLELOCATION_L1,PRINTTITLELOCATION_L2,
PRINTTITLELOCATION_L3,PRINTTITLELOCATION_L4,PRINTTITLELOCATION_L5,PRINTTITLELOCATION_L6,PRINT_RULER_LEN};

int PrintElementLocation[8] = {PRINTELEMENTLOCATION_L0,PRINTELEMENTLOCATION_L1,PRINTELEMENTLOCATION_L2,
PRINTELEMENTLOCATION_L3,PRINTELEMENTLOCATION_L4,PRINTELEMENTLOCATION_L5,PRINTELEMENTLOCATION_L6,PRINT_RULER_LEN};

char PrintLocationRuler[PRINT_RULER_LEN];


/***********************************************/

extern char * _CS_( char* indent   ) ;

/*  #define DEBUG  */

void GetSelectedPrinter(GtkWidget *w,gpointer data);
void DestroyPrtDlg(GtkWidget *w,gpointer data);
void	ProgHardCopy1(int crt_n ,int print_mode );
GtkWidget *PrinterSelDlg[MAX_CRT_NUMBER];
GtkWidget *LangSelDlg[MAX_CRT_NUMBER];
static char hard_copy_cmd_str[100],hard_copy_cmd_str1[100],hard_copy_cmd_str2[100];
static char  printer_name[20] ;
char *_toUtf8EX(char *c) ;
char *_toUtf8(char *c);
//HDC    prtdc;/*winprint pic*/

void DestroyDlg(GtkWidget *w)
{	
	gtk_widget_destroy(w);
}	

void	PopupMessageBox (GtkWidget *parent,char msg[] )
{
	GtkWidget *msg_dialog;
	gchar *pConvert=0;
	
	//msg_dialog = gtk_message_dialog_new(GTK_WINDOW(parent),/*GTK_DIALOG_MODAL| */GTK_DIALOG_DESTROY_WITH_PARENT,  
	//	GTK_MESSAGE_INFO,GTK_BUTTONS_OK ,
	//	_toUtf8(_CS_(msg)) 
	//	);
	
	msg_dialog = gtk_message_dialog_new(GTK_WINDOW(parent),/*GTK_DIALOG_MODAL| */GTK_DIALOG_DESTROY_WITH_PARENT,  
		GTK_MESSAGE_INFO,GTK_BUTTONS_NONE ,
	pConvert=_toUtf8(_CS_(msg)) 
		);
	G_SAFE_FREE(pConvert);//dzy1017
     gtk_dialog_add_button(GTK_DIALOG(msg_dialog),pConvert=_toUtf8EX(OIX_CSTR_OK),GTK_RESPONSE_OK);
	G_SAFE_FREE(pConvert);//dzy1017

	gdk_beep();
	/*gtk_dialog_run(GTK_DIALOG(msg_dialog));
	gtk_widget_destroy(msg_dialog);	*/
	gtk_window_set_position(GTK_WINDOW(msg_dialog),GTK_WIN_POS_CENTER) ; 
	gtk_widget_show(msg_dialog);
	g_signal_connect_swapped(msg_dialog,"response",G_CALLBACK(gtk_widget_destroy),msg_dialog);
}

int 	PopupMessageBoxRet (GtkWidget *parent,char msg[] )
{
	GtkWidget *msg_dialog;
	int result ; 
	gchar *pConvert=0;
//	msg_dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_DESTROY_WITH_PARENT,  
//		GTK_MESSAGE_INFO,GTK_BUTTONS_OK_CANCEL ,
//		_toUtf8(_CS_(msg)) 
//		);

	msg_dialog = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_DESTROY_WITH_PARENT,  
		GTK_MESSAGE_INFO,GTK_BUTTONS_NONE ,
		pConvert=_toUtf8(_CS_(msg)) 
		);
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_dialog_add_button(GTK_DIALOG(msg_dialog),pConvert=_toUtf8EX(OIX_CSTR_CANCEL),GTK_RESPONSE_CANCEL);
    G_SAFE_FREE(pConvert);//dzy1017
	gtk_dialog_add_button(GTK_DIALOG(msg_dialog),pConvert=_toUtf8EX(OIX_CSTR_OK),GTK_RESPONSE_OK);
	G_SAFE_FREE(pConvert);//dzy1017

	gtk_dialog_set_alternative_button_order (GTK_DIALOG (msg_dialog), GTK_RESPONSE_OK, GTK_RESPONSE_CANCEL, -1);

	gdk_beep();
	gtk_window_set_position(GTK_WINDOW(msg_dialog),GTK_WIN_POS_CENTER) ; 
	result = gtk_dialog_run (GTK_DIALOG (msg_dialog));
	gtk_widget_destroy (msg_dialog);
	if (result == GTK_RESPONSE_DELETE_EVENT )
		return  GTK_RESPONSE_CANCEL;      /*   GTK_RESPONSE_OK    or  GTK_RESPONSE_CANCEL  */
	else 
		return  result ;       
}



void	PopupCmdDownMessageBox (int crt_n,char msg[] )
{
	GtkWidget *msg_dialog;
	
//	msg_dialog = gtk_message_dialog_new(GTK_WINDOW(main_w[crt_n]),GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,  
//		GTK_MESSAGE_INFO,GTK_BUTTONS_OK|GTK_BUTTONS_CANCEL  ,
//		_toUtf8(msg) );
	gchar *pConvert=0;
	msg_dialog = gtk_message_dialog_new(GTK_WINDOW(main_w[crt_n]),GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,  
		GTK_MESSAGE_INFO,GTK_BUTTONS_OK|GTK_BUTTONS_CANCEL  ,
		pConvert=_toUtf8(msg) );
    G_SAFE_FREE(pConvert);//dzy1017

	gtk_dialog_set_alternative_button_order (GTK_DIALOG (msg_dialog), GTK_RESPONSE_OK, GTK_RESPONSE_CANCEL, -1);

    gdk_beep();
    gtk_dialog_run(GTK_DIALOG(msg_dialog));
	gtk_widget_destroy(msg_dialog);	
}


void	PopupPrinterSetDlg (GtkWidget *parent,int crt_n ,int prt_obj_type)
{     
	GtkWidget *dialog_vbox1;
	GtkWidget *vbox1;
	GtkWidget *prompt_lab;
	GtkWidget *radiobutton_prt[20];
	GSList *radiobutton_prt1_group = NULL;
	GtkWidget *dialog_action_area1;
	GtkWidget *cancelbutton1;
	GtkWidget *okbutton1;
	
	int i,prt_num;
	char  hcpy_printer[10][20];
	char  printer_longname[10][20];
	char  prt_index[10][20],prt_name[20];
	gchar *pConvert=0;
	if(prt_obj_type==HARD_COPY)
	{
		prt_num = GetHcopyPrinterNum();
        for(i=0;i<prt_num;i++)
        {                
            if(GetHcopyPrinter(i, prt_name)!=-1)
            {
				if (GetHostLongname(  prt_name, printer_longname[i])==-1)
					strcpy(prt_name,"Undefine!");
				strcpy (hcpy_printer[i] ,prt_name);
            }
        }
	}
	else
	{
		if(GetAlmPrinterNum()!=-1)
            prt_num = GetAlmPrinterNum();
		else
            return;
		for(i=0;i<prt_num;i++)
		{        
            if(GetAlmPrinter(i,prt_name)!=-1)
			{                    
				strcpy(hcpy_printer[i],prt_name);   
            }
		}    
	}
	
	PrinterSelDlg[crt_n] = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (PrinterSelDlg[crt_n]), pConvert=_toUtf8EX(OIX_CSTR_PRINTER_SET));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_window_set_type_hint (GTK_WINDOW (PrinterSelDlg[crt_n]), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_transient_for(GTK_WINDOW(PrinterSelDlg[crt_n]),GTK_WINDOW(parent));
	
	dialog_vbox1 = GTK_DIALOG (PrinterSelDlg[crt_n])->vbox;
	gtk_widget_show (dialog_vbox1);
	
	vbox1 = gtk_vbox_new (FALSE, 10);
	gtk_widget_show (vbox1);
	gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 10);
	
	prompt_lab = gtk_label_new (pConvert=_toUtf8(_CS_(MSG_SELECT_PRINTER)));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (prompt_lab);
	gtk_box_pack_start (GTK_BOX (vbox1), prompt_lab, FALSE, FALSE, 0);
	
	for(i=0;i<prt_num;i++)
	{
		radiobutton_prt[i] = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8(printer_longname[i]/*hcpy_printer[i]*/));
		G_SAFE_FREE(pConvert);//dzy1017		
		gtk_widget_show (radiobutton_prt[i]);
		gtk_box_pack_start (GTK_BOX (vbox1), radiobutton_prt[i], FALSE, FALSE, 0);
		gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_prt[i]), radiobutton_prt1_group);
		radiobutton_prt1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_prt[i]));
		sprintf(prt_index[i],"SelectedPrinter%d",i);
		g_object_set_data(G_OBJECT(PrinterSelDlg[crt_n]),prt_index[i],radiobutton_prt[i]);
		if (strcmp(printer_name , "")!=0 &&  strcmp(hcpy_printer[i],printer_name)==0 )
		{
			gtk_toggle_button_set_active    ((GtkToggleButton*)radiobutton_prt[i],TRUE);
		}
		
		
	}
	
	dialog_action_area1 = GTK_DIALOG (PrinterSelDlg[crt_n])->action_area;
	gtk_widget_show (dialog_action_area1);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);
	
	cancelbutton1 =gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);//dzy1017
	// gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (PrinterSelDlg[crt_n]), cancelbutton1, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);
	gtk_signal_connect (GTK_OBJECT (cancelbutton1), "clicked",
		GTK_SIGNAL_FUNC(DestroyPrtDlg),
		(gpointer)crt_n);
	
	okbutton1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);//dzy1017
		//gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (PrinterSelDlg[crt_n]), okbutton1, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);
	gtk_signal_connect (GTK_OBJECT (okbutton1), "clicked",
		GTK_SIGNAL_FUNC(GetSelectedPrinter),
		GINT_TO_POINTER (prt_obj_type));
	
	gtk_widget_show (PrinterSelDlg[crt_n]);
	return  ;//PrinterSelDlg[crt_n];
}

void DestroyPrtDlg(GtkWidget *w,gpointer data)
{
    int crt_n = (gint)data;
	gtk_widget_destroy(PrinterSelDlg[crt_n]);
	PrinterSelDlg[crt_n]=NULL;
}

void  InitPrinterName()
{    
	int prt_num ; char prt_name1[20]; 
	prt_num = GetHcopyPrinterNum(); 
	   memset(prt_name1,20, 0 );
	   if (prt_num >0 )
           if(GetHcopyPrinter(0, prt_name1)!=-1)
		   {
               strcpy (printer_name ,prt_name1 ) ;
			   printf("\nOIX::init printer name is :%s \n" ,printer_name);
		   }
		   
		   
}
void GetSelectedPrinter(GtkWidget *w,gpointer data)
{
    int i,isSelectedPrinter,prt_num,prt_obj_type,prt_type=0;
    char  hcpy_printer[10][20];
    char    prt_name[20],  prt_index[10][20];
    GtkWidget *radiobutton_prt[20];
    char    command_buf_ps[ MAX_FILE_NAME_LEN * 10 ]  ; 
    int crt_n;
    
    prt_obj_type = (gint)data;
    memset(command_buf_ps,0,MAX_FILE_NAME_LEN * 10 ) ;
    GetCrtNumberOfWgt ( GTK_WIDGET(w), &crt_n );
	if( crt_n < 0 )
		return; 
    gtk_widget_hide(PrinterSelDlg[crt_n]);
    printf("here is GetSelectedPrinter\n");
    if(prt_obj_type == HARD_COPY)
	{
        prt_num = GetHcopyPrinterNum();
        for(i=0;i<prt_num;i++)
        {                
            if(GetHcopyPrinter(i, prt_name)!=-1)
            {
                strcpy(hcpy_printer[i],prt_name);
                sprintf(prt_index[i],"SelectedPrinter%d",i);
                printf("prt_index[%d]=%s\n",i,prt_index[i]);
                radiobutton_prt[i]= g_object_get_data(G_OBJECT(PrinterSelDlg[crt_n]),prt_index[i]);
				isSelectedPrinter = 0 ;
                isSelectedPrinter = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_prt[i])); 
				
                if(isSelectedPrinter==1)
				{   memset(printer_name,20, 0);
				strcpy (printer_name ,hcpy_printer[i] ) ;
				break;
                }
            }
        }
    }
    else
    {
		if(GetAlmPrinterNum()!=-1)
            prt_num = GetAlmPrinterNum();
		else
            return;
		for(i=0;i<prt_num;i++)
		{        
            if(GetAlmPrinter(i,prt_name)!=-1)
			{                    
                strcpy(hcpy_printer[i],prt_name);
                sprintf(prt_index[i],"SelectedPrinter%d",i);
                printf("prt_index[%d]=%s\n",i,prt_index[i]);
                radiobutton_prt[i]= g_object_get_data(G_OBJECT(PrinterSelDlg[crt_n]),prt_index[i]);
                isSelectedPrinter = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_prt[i]));  
				memset(printer_name,20, 0);
				isSelectedPrinter = 0 ;
				strcpy (printer_name ,hcpy_printer[i] ) ;
                if(isSelectedPrinter==1)
                    break;  
            }
		}    
	}   
	
    /*GetPrinterTypeByName(hcpy_printer[i],&prt_type);*/
	printf ("OIX::Curent Pintr is %s \n" , printer_name) ;	
	gtk_widget_destroy(PrinterSelDlg[crt_n]);
	
}


extern print_delay_time;
void	ProgHardCopy(int crt_n ,int print_mode) //add by hcl print mode
{    
    
	if(print_delay_time!=0)
	{	
		gtk_timeout_remove(print_delay_time);
	    print_delay_time=0;
	}
		/*    if(strcmp(hard_copy_cmd_str,"")==0)  */
	ProgHardCopy1( crt_n ,print_mode);
    /*    else
	sprintf(hard_copy_cmd_str,"%s%d%s",hard_copy_cmd_str1,crt_n,hard_copy_cmd_str2);*/
#ifdef  SUN_OS
	printf("OIX will print &&&&&&& hard_copy_cmd_str=%s\n",hard_copy_cmd_str);  
	system(hard_copy_cmd_str);
#endif
}

void	ProgHardCopy1(int crt_n ,int print_mode )
{
	char hostname[20]/*prt_name[20]*/;
	int /*i,*/prt_num;        	  
	
	/*	char	temp[200];*/
	char    command_buf_ps[MAX_FILE_NAME_LEN * 10 ];     
	
	memset(command_buf_ps,0,MAX_FILE_NAME_LEN * 10);
	
	gethostname(hostname,sizeof(hostname));    
	
	prt_num = GetHcopyPrinterNum();
	/*      for(i=0;i<prt_num;i++)
	{                
	if(GetHcopyPrinter(i, printer_name)!=-1)
	break;
	}
	
	*/
    
#ifdef  SUN_OS
	sprintf ( command_buf_ps, "/usr/openwin/bin/xdpr   -d%s -landscape -density 140",  printer_name);
	
	if(IsColorPrinter(printer_name))
		strcat ( command_buf_ps, "  -device pjetxl -noposition  ");
	else
		strcat ( command_buf_ps, "  -device ps -gray 2  -rv");/* Modified by zyp on 2006.7.14 */
	
	/*system ( command_buf_ps ); /* output to printer*/  
	strcpy(hard_copy_cmd_str,command_buf_ps);
	printf("command_buf_ps=%s\n",command_buf_ps);
#endif 
	
#ifdef  UNIX
	
	if(IsColorPrinter(printer_name))
		strcpy ( command_buf_ps, "/usr/bin/X11/dxprint -c hdcp_tmp.ps" );
	else
		strcpy ( command_buf_ps, "/usr/bin/X11/dxprint -x15 -y70 -w1245 -h950 hdcp_tmp.ps" );
	
	system ( command_buf_ps );
	
	/*	sprintf(temp,"lpr -P %s hdcp_tmp.ps",UNIX_HARD_COPY);  */
	
	system ( "rm hdcp_tmp.ps" );	
#endif
	
#ifdef WINDOWS_OS
	
    {	HWND	hWnd;				
	    PRINTDLG  pd;

		HDC    prtdc;
		char temp_print_name[200];
		char cmdbuff[200];

	
	
	hWnd= GetActiveWindow();
	memset ((void *) &pd, 0, sizeof(PRINTDLG));
	
	pd.lStructSize = sizeof(PRINTDLG);
	pd.hwndOwner   = hWnd;
	pd.hDevMode = (HANDLE)NULL;
	pd.hDevNames = (HANDLE)NULL;
	pd.Flags = PD_RETURNDC | PD_NOPAGENUMS | PD_NOSELECTION | PD_PRINTSETUP;
	pd.nFromPage = 0;
	pd.nToPage = 0;
	pd.nMinPage = 0;
	pd.nMaxPage = 0;
	pd.nCopies = 1;//0 By zyp on 2002.10.16  
	pd.hInstance   = NULL;
	
    strcpy( temp_print_name,get_temp_file_name( crt_n));
	WinSaveJPGFile(HWND_DESKTOP, temp_print_name,print_mode);

	if (PrintDlg(&pd) == TRUE)
	{
		prtdc = pd.hDC;
		PrintWinPic(prtdc,crt_n,temp_print_name);//By zyp on 2002.10.16
		//DeleteDC(prtdc);
	}
	//delete temp print file
	sprintf(cmdbuff, "%s %s", "del ",temp_print_name); 
   	system(cmdbuff);				
	}
				
				
	
	
#endif
	
}


//#if defined(WINDOWS_OS)

/*****************************************************************************/
/*  	Function :begin_print() ,draw_page() ,end_print()  ,PrintListWin32() */
/*	    is for print on WIN_OS										       	 */
/*      add by hcl 2007-9-4                                                  */
/************************************************************************/
static void begin_print (GtkPrintOperation *operation, 
						 GtkPrintContext   *context,
						 gpointer           user_data)
{
	PrintData *data = (PrintData *)user_data;
	char *contents;
	int i;
	double height;
	
	height = gtk_print_context_get_height (context) - HEADER_HEIGHT - HEADER_GAP;
	
	data->lines_per_page =PER_PAGE;// (int)(floor (height / ((data->font_size)*6)));
	
	g_file_get_contents (data->filename, &contents, NULL, NULL);
	
	data->lines = g_strsplit (contents, "\n", 0);
	g_free (contents);
	
	i = 0;
	while (data->lines[i] != NULL)
		i++;
	
	data->num_lines = i;
	data->num_pages = (data->num_lines - 1) / data->lines_per_page + 1;
	gtk_print_operation_set_n_pages (operation, data->num_pages);
}

static void draw_page (GtkPrintOperation *operation,
					   GtkPrintContext   *context,
					   gint               page_nr,
					   gpointer           user_data)
{
	PrintData *data = (PrintData *)user_data;
	cairo_t *cr;
	PangoLayout *layout;
	gdouble width, text_height;
	gint line, i, layout_height;
	PangoFontDescription *desc;
	gchar *page_str;
	gchar *pConvert=0;
	
	int j;
	double x;
	char element[100];
	char *mydateline;

	struct   tm   *local;  
	time_t   t;
	double height;
	
	cr = gtk_print_context_get_cairo_context (context);
	width = gtk_print_context_get_width (context)-LEFT_START; /*// 使横线到左右两边距离一致	*/
	height = gtk_print_context_get_height (context) - HEADER_HEIGHT - HEADER_GAP;
	
	cairo_rectangle (cr, 0, 100, width, HEADER_HEIGHT/2);/*//控制上面横线的位置 和大小*/
	
	cairo_set_source_rgb (cr, 0.8, 0.8, 0.8);
	cairo_fill_preserve (cr);
	
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, 1);
	cairo_stroke (cr);
	
	layout = gtk_print_context_create_pango_layout (context);
	
	desc = pango_font_description_from_string ("sans 14");
	pango_layout_set_font_description (layout, desc);
	pango_font_description_free (desc);
	
	pango_layout_set_text (layout, (char *)data->Title , -1); /*//设置标题*/
	pango_layout_set_width (layout, (int)(width));
	pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);
	
	pango_layout_get_size (layout, NULL, &layout_height);
	text_height = (gdouble)layout_height / PANGO_SCALE;
	
	cairo_move_to (cr, width / 2,  (HEADER_HEIGHT - text_height) / 2);
	pango_cairo_show_layout (cr, layout);
	
	page_str = g_strdup_printf ("%d/%d", page_nr + 1, data->num_pages);
	pango_layout_set_text (layout, page_str, -1);
	g_free (page_str);
	pango_layout_set_alignment (layout, PANGO_ALIGN_RIGHT);
	
	cairo_move_to (cr, width - 2, (HEADER_HEIGHT - text_height) / 2);
	pango_cairo_show_layout (cr, layout);
	
	g_object_unref (layout);
	
	layout = gtk_print_context_create_pango_layout (context);
	
	desc = pango_font_description_from_string ("mono");
	pango_font_description_set_size (desc, (int)(data->font_size * PANGO_SCALE));
	pango_layout_set_font_description (layout, desc);
	pango_font_description_free (desc);
	
	cairo_move_to (cr, LEFT_START, HEADER_HEIGHT + HEADER_GAP);
	line = page_nr * data->lines_per_page;
	for (i = 0; i < data->lines_per_page && line < (data->num_lines)-1; i++) 
    {

		mydateline=data->lines[line];

		if(i==0)//title
		{
			for(j=0;j<=PRINTELEMENT;j++)
			{
				
				strncpy(element, mydateline+PrintTitleLocation[j],PrintTitleLocation[j+1]-PrintTitleLocation[j]);
				element[PrintTitleLocation[j+1]-PrintTitleLocation[j]-1] = '\0';
				
				pango_layout_set_text (layout, pConvert=_toUtf8(element), -1);
				G_SAFE_FREE(pConvert);//dzy1017
				pango_cairo_show_layout (cr, layout);
				
				x=width*(PrintTitleLocation[j+1]-PrintTitleLocation[j])/PRINT_RULER_LEN;			
			
				cairo_rel_move_to (cr,x, 0);	  
			}		  
			cairo_move_to (cr, 2,(height/data->lines_per_page)+HEADER_HEIGHT + HEADER_GAP);
			
		}
		else //body
		{
			for(j=0;j<=PRINTELEMENT;j++)
			{
				strncpy(element, mydateline+PrintElementLocation[j],PrintElementLocation[j+1]-PrintElementLocation[j]);
				element[PrintElementLocation[j+1]-PrintElementLocation[j]-1] = '\0';
								
				pango_layout_set_text (layout, pConvert=_toUtf8(element), -1);
				G_SAFE_FREE(pConvert);//dzy1017
				pango_cairo_show_layout (cr, layout);

				strcpy(element,"");
				
				/*确定每一行Y的坐标 现对坐标*/
				cairo_rel_move_to (cr,width*(PrintElementLocation[j+1]-PrintElementLocation[j])/PRINT_RULER_LEN, 0);	  
				
			}
			/*绝对坐标*/
			//cairo_move_to (cr, 0,(data->font_size*LINESPACE)*(i+1)+HEADER_HEIGHT + HEADER_GAP);
				
			cairo_move_to (cr, 0,(height/data->lines_per_page)*(i+1) +HEADER_HEIGHT + HEADER_GAP);
			
		}
		
		line++;
	}
	
	t=time(NULL);  
	local=localtime(&t);  
	/*printf("local   time   and   date:%s\n",asctime(local));  */ 
	
	pango_layout_set_text (layout, pConvert=_toUtf8(asctime(local)), -1);
	G_SAFE_FREE(pConvert);//dzy1017
	cairo_move_to (cr, width*2/3,(height/data->lines_per_page)*(i)+HEADER_HEIGHT + HEADER_GAP+200);
	
	pango_cairo_show_layout (cr, layout);


	g_object_unref (layout);
}

static void end_print (GtkPrintOperation *operation, 
					   GtkPrintContext   *context,
					   gpointer           user_data)
{
	PrintData *data = (PrintData *)user_data;
	
	g_free (data->filename);
	g_strfreev (data->lines);
	g_free (data);
}



char* GetPrintTitle(char *output_file,char * strTitle)
{
	//char strTitle[100];
	char *ptr;
	int len;
#ifdef  SUN_OS
	char  c = '/';
#endif
#ifdef WINDOWS_OS
	char c = '\\';
#endif
	
	strcpy(strTitle, output_file);
	ptr = strrchr(output_file, c);
	len=strlen(output_file);

	strncpy(strTitle, ptr+1,len-(ptr-output_file) );
	
	strTitle[strlen(strTitle)]='\0';
		
	return (strTitle);
}

void PrintListWin32 (GtkWidget *do_widget,char *output_file)
{
	GtkPrintOperation *operation;
	PrintData *data;
	GError *error = NULL;
	char strTitle[100];

	operation = gtk_print_operation_new ();
	data = g_new0 (PrintData, 1);
	data->filename =output_file;

	GetPrintTitle(output_file,strTitle);
      strcpy((char*)data->Title ,strTitle);

	data->font_size = FONT_SIZE;
	
	g_signal_connect (G_OBJECT (operation), "begin-print", 
		G_CALLBACK (begin_print), data);
	g_signal_connect (G_OBJECT (operation), "draw-page", 
		G_CALLBACK (draw_page), data);
	g_signal_connect (G_OBJECT (operation), "end-print", 
		G_CALLBACK (end_print), data);
	
	gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, GTK_WINDOW (do_widget), &error);
	
	g_object_unref (operation);
	
	if (error)
    {
		GtkWidget *dialog;
		
		dialog = gtk_message_dialog_new (GTK_WINDOW (do_widget),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_ERROR,
			GTK_BUTTONS_CLOSE,
			"%s", error->message);
		g_error_free (error);
		
		g_signal_connect (dialog, "response",
			G_CALLBACK (gtk_widget_destroy), NULL);
		
		gtk_widget_show (dialog);      
    }
		
}
//#endif 
#if defined(SUN_OS)
void PrintListUnix(char *output_file)
{
	char    List_printer_name[20],prt_cmd_str[100];
	int i,printer_num;

	sprintf(prt_cmd_str,"pr -s %s|mp -PS -n -ll |lp   -o nobanner -P A4 -d %s",output_file,printer_name);
	// 	sprintf(prt_cmd_str,"pr -s %s|mp -PS -n -ll |lp -y magnify=100  -o nobanner -P A4 -o length=21c -o width=29.7c -d %s",output_file,printer_name);
	printf("prt_cmd_str=%s\n",prt_cmd_str);
    system(prt_cmd_str);

	pthread_exit(0);

}
#endif 

void PrintList(GtkWidget*parent,int crt_n,int prt_obj_type ,char *output_file)
{ 

 /*   
	// sprintf(prt_cmd_str,"pr -s %s|mp -PS|lp -o nobanner -o nofilebreak -o length=21c -o width=29.7c -d %s",output_file,List_printer_name);

 #if defined(SUN_OS)
 	char    List_printer_name[20],prt_cmd_str[100];
 	int i,printer_num;
// 	printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
// 	system("echo $LANG");
sprintf(prt_cmd_str,"pr -s %s|mp -PS -n -ll |lp   -o nobanner -P A4 -d %s",output_file,printer_name);
// 	sprintf(prt_cmd_str,"pr -s %s|mp -PS -n -ll |lp -y magnify=100  -o nobanner -P A4 -o length=21c -o width=29.7c -d %s",output_file,printer_name);
     printf("prt_cmd_str=%s\n",prt_cmd_str);
     system(prt_cmd_str);
 	
 #endif 
*/	
	/*******************************************************/
	/************add by hcl for WINDOWS Print*****************/
//#if defined(WINDOWS_OS)
//	PrintListWin32(parent,output_file);
//#endif 

//////////////////////////////////////////////////////////////////////////////////
#ifdef  SUN_OS
	pthread_t tid1,tid2;
	int error;
#endif

	
#ifdef  SUN_OS
	error = pthread_create(&tid1, NULL, PrintListUnix, (char*)(output_file ));
	if(error!=-1)
	{
		printf("Create PrintListUnix Success!\n");
		
	}
	else
	{
		printf("Create PrintListUnix failed!!! error=%d\n",error);
		
	}
#endif 
	
#ifdef  WINDOWS_OS
	PrintListWin32(parent,output_file);
#endif
}


char *_toUtf8(char *c)
{
//	return (g_locale_to_utf8(c,-1,0,0,0)); 

	return g_convert(c, strlen(c), "UTF-8", "GBK", NULL, NULL, NULL);
}

char *_toUtf8EX(char *c)
{
 //  char title[1024] ;

//	return (g_locale_to_utf8(_CS_(c),-1,0,0,0));
	return _toUtf8(_CS_(c));
}

char *_fromUtf8(char *c)
{
//	return (g_locale_from_utf8(c,-1,0,0,0)); 

	return g_convert(c, strlen(c), "GBK", "UTF-8", NULL, NULL, NULL);
}

void GetSelectedLang(GtkWidget *w,gpointer data);


void DestroyLangDlg(GtkWidget *w,gpointer data)
{
    int crt_n = (gint)data;
	gtk_widget_destroy(LangSelDlg[crt_n]);
	LangSelDlg[crt_n]=NULL;
}


char  *SetLangName[] ={OIX_STR_CHINESE,OIX_STR_FORGEIN};

void	PopupLanguageSetDlg (GtkWidget *parent,int crt_n ,int prt_obj_type)
{     
	GtkWidget *dialog_vbox1;
	GtkWidget *vbox1;
	GtkWidget *prompt_lab;
	GtkWidget *radiobutton_prt[20];
	GSList *radiobutton_prt1_group = NULL;
	GtkWidget *dialog_action_area1;
	GtkWidget *cancelbutton1;
	GtkWidget *okbutton1;
	gchar *pConvert=0;
	int i;
//	int prt_num ,res;
//	char  hcpy_printer[10][20];
	char  prt_index[10][20];
//	char prt_name[20];

	
	LangSelDlg[crt_n] = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (LangSelDlg[crt_n]), pConvert=_toUtf8EX(OIX_STR_LANG_SET));
    G_SAFE_FREE(pConvert);//dzy1017
	gtk_window_set_type_hint (GTK_WINDOW (LangSelDlg[crt_n]), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_transient_for(GTK_WINDOW(LangSelDlg[crt_n]),GTK_WINDOW(parent));
	
	dialog_vbox1 = GTK_DIALOG (LangSelDlg[crt_n])->vbox;
	gtk_widget_show (dialog_vbox1);
	
	vbox1 = gtk_vbox_new (FALSE, 10);
	gtk_widget_show (vbox1);
	gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 10);
	
	prompt_lab = gtk_label_new (pConvert=_toUtf8(_CS_(OIX_MSG_CHANGE_LANG)));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_widget_show (prompt_lab);
	gtk_box_pack_start (GTK_BOX (vbox1), prompt_lab, FALSE, FALSE, 0);
	
	for(i=0;i<2;i++)
	{
		radiobutton_prt[i] = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8EX(SetLangName[i]));
		G_SAFE_FREE(pConvert);//dzy1017
		gtk_widget_show (radiobutton_prt[i]);
		gtk_box_pack_start (GTK_BOX (vbox1), radiobutton_prt[i], FALSE, FALSE, 0);
		gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_prt[i]), radiobutton_prt1_group);
		radiobutton_prt1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_prt[i]));
		sprintf(prt_index[i],"SelectedLang%d",i);
		g_object_set_data(G_OBJECT(LangSelDlg[crt_n]),prt_index[i],radiobutton_prt[i]);

	}
	gtk_toggle_button_set_active    ((GtkToggleButton*)radiobutton_prt[GetExLangState()],TRUE);
	
	dialog_action_area1 = GTK_DIALOG (LangSelDlg[crt_n])->action_area;
	gtk_widget_show (dialog_action_area1);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);
	
	cancelbutton1 =gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);//dzy1017
	//gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (LangSelDlg[crt_n]), cancelbutton1, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);
	gtk_signal_connect (GTK_OBJECT (cancelbutton1), "clicked",
		GTK_SIGNAL_FUNC(DestroyLangDlg),
		GINT_TO_POINTER(crt_n));
	
	okbutton1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);//dzy1017
	//gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (LangSelDlg[crt_n]), okbutton1, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);
	gtk_signal_connect (GTK_OBJECT (okbutton1), "clicked",
		GTK_SIGNAL_FUNC(GetSelectedLang),
		GINT_TO_POINTER (prt_obj_type));
	
	gtk_widget_show (LangSelDlg[crt_n]);
	return  ;
}


void GetSelectedLang(GtkWidget *w,gpointer data)
{
    int i,isSelected;

    char     prt_index[10][20];
    GtkWidget *radiobutton_prt[20];
    int crt_n;
    GetCrtNumberOfWgt ( GTK_WIDGET(w), &crt_n );
	if( crt_n < 0 )
		return; 

		for(i=0;i<2;i++)
		{                                   
			sprintf(prt_index[i],"SelectedLang%d",i);
			radiobutton_prt[i]= g_object_get_data(G_OBJECT(LangSelDlg[crt_n]),prt_index[i]);
			isSelected = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_prt[i]));  
			if(isSelected)
				break ;
							
		}  
//		printf("I =%d ",i) ;
       if (GetExLangState()!=i)
	   {
		    if (PopupMessageBoxRet (main_w[crt_n],_CS_(OIX_MSG_WILL_REBOOT) ) !=GTK_RESPONSE_CANCEL)
			{ 
				   SetExLangState(i);  
			       gtk_main_win_quit();  	
			}
			
	   }  
	    DestroyLangDlg(NULL,GINT_TO_POINTER(crt_n));
	
}


void  ProgDispEntryName (GtkWidget*parent,int crt_n)
{
    if (iSetDispEntry[crt_n] == 1)
		iSetDispEntry[crt_n] =0 ; 
	else
		iSetDispEntry[crt_n] =1  ;
	
}

