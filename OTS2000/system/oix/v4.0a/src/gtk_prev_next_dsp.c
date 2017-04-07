/**************************************************** 
2005.11 Created by zyp for gtk
2006.7  Update  by chi 
	GetPrevNextDsp :
			0------get previous display
			1------get next display

WINDOWS HAVE SYN TO UNIX by chi.hailong  2007.08.05 ;   

************************************************************/

#include <gtk/gtk.h>
#include	"../inc/oix.h"

extern int	Max(int x,int y);
extern int OpenDspFile (int crt_n, char	*filename);

 
GList  *dsp_list[MAX_CRT_NUMBER];
typedef struct 
{
    char rem_dsp_name[MAX_DSP_CNT][DSP_FILE_SHORTNAME_LEN];
    int  cur_dsp_ptr; 
    int  nextpos ;
    int  start ; 
    int  notAdd  ;
    
} T_DSP_LIST ;

T_DSP_LIST recall_dsp[MAX_CRT_NUMBER];
void AppendDspData(  int crt_n ,  char*dsp_name)
{
    int i ; 
    if ( recall_dsp[crt_n].notAdd ) return ; 
    if (recall_dsp[crt_n].nextpos==0)
        {
             strcpy(recall_dsp[crt_n].rem_dsp_name[recall_dsp[crt_n].nextpos] ,dsp_name);
             recall_dsp[crt_n].nextpos= ((++ recall_dsp[crt_n].nextpos>= MAX_DSP_CNT)?MAX_DSP_CNT:recall_dsp[crt_n].nextpos);
    }else
    if (strcmp( recall_dsp[crt_n].rem_dsp_name[Max(recall_dsp[crt_n].nextpos-1,0)] ,dsp_name))
        {

		  if (recall_dsp[crt_n].cur_dsp_ptr != recall_dsp[crt_n].nextpos -1 ) /*added by chi*/
		  {
			  printf ("cur_dsp_ptr !=  nextpos -1  \n ");
			  for (i =MAX_DSP_CNT-1   ; i > recall_dsp[crt_n].cur_dsp_ptr ; i--)
						  strcpy( (char *)(&recall_dsp[crt_n].rem_dsp_name[i]),(char*)(&recall_dsp[crt_n].rem_dsp_name[i-1])); 

                      recall_dsp[crt_n].nextpos= ((++ recall_dsp[crt_n].nextpos>= MAX_DSP_CNT)?MAX_DSP_CNT:recall_dsp[crt_n].nextpos);   //added 
					  strcpy(recall_dsp[crt_n].rem_dsp_name[++recall_dsp[crt_n].cur_dsp_ptr] ,dsp_name); 
                      
					 // recall_dsp[crt_n].cur_dsp_ptr ++ ;
		  }
		  else
		  {
			  if (recall_dsp[crt_n].nextpos == MAX_DSP_CNT )
				  {
					  for (i =0  ; i< MAX_DSP_CNT -1; i++)
						  strcpy((char*)&recall_dsp[crt_n].rem_dsp_name[i],(char*)&recall_dsp[crt_n].rem_dsp_name[i+1]); 
                  
                  
					  strcpy(recall_dsp[crt_n].rem_dsp_name[MAX_DSP_CNT-1] ,dsp_name); 
			  }else
				 strcpy(recall_dsp[crt_n].rem_dsp_name[recall_dsp[crt_n].nextpos] ,dsp_name);

			  
			  recall_dsp[crt_n].nextpos= ((++ recall_dsp[crt_n].nextpos>= MAX_DSP_CNT)?MAX_DSP_CNT:recall_dsp[crt_n].nextpos);   //added 
			  recall_dsp[crt_n].cur_dsp_ptr = recall_dsp[crt_n].nextpos-1 ;   //added 
		  }
          
          /*printf ("dsp name  %s\n", dsp_name); */
         
       //   recall_dsp[crt_n].nextpos= ((++ recall_dsp[crt_n].nextpos>= MAX_DSP_CNT)?MAX_DSP_CNT:recall_dsp[crt_n].nextpos); 
          
                
    }
    
 //   recall_dsp[crt_n].cur_dsp_ptr = recall_dsp[crt_n].nextpos-1 ;
    
   
  /* printf ("Next pos : %d  ;;   cur_dsp_no = %d   \n",recall_dsp[crt_n].nextpos , recall_dsp[crt_n].cur_dsp_ptr);  
  for (i = 0 ; i < MAX_DSP_CNT ; i ++ ) 
      {
          printf ("file name :%s =========" , recall_dsp[crt_n].rem_dsp_name[i]);
  }
   printf ("\n " );  
   
   */

   /*
   for ( i = 0 ; i < MAX_DSP_CNT ; i++ )
      {
          printf (" #%d =  %s ,", i ,  recall_dsp[crt_n].rem_dsp_name[i])  ;
   	  
      }
   printf ("\n " );  */
   


}

void GetPrevDsp(GtkWidget *w,gpointer data)
{
	int crt_n =(gint)data;
	 
        recall_dsp[crt_n].cur_dsp_ptr--;
        if(recall_dsp[crt_n].cur_dsp_ptr < 0)
            {
               recall_dsp[crt_n].cur_dsp_ptr  = 0;
               gdk_beep();
        }
        recall_dsp[crt_n].notAdd   = TRUE ;
        if ( rem_dsp_name[crt_n][ recall_dsp[crt_n].cur_dsp_ptr/*cur_dsp_ptr[crt_n] */] )
        {
                OpenDspFile ( crt_n, recall_dsp[crt_n].rem_dsp_name[ recall_dsp[crt_n].cur_dsp_ptr]);
        }
        recall_dsp[crt_n].notAdd   = FALSE ; 
printf ("Next pos : %d  ;;   cur_dsp_no = %d   \n",recall_dsp[crt_n].nextpos , recall_dsp[crt_n].cur_dsp_ptr);  		 
	 
}

void GetNextDsp(GtkWidget *w,gpointer data)
{
	int crt_n =(gint)data;
	 
        recall_dsp[crt_n].cur_dsp_ptr++;
        if ( recall_dsp[crt_n].cur_dsp_ptr  >= recall_dsp[crt_n].nextpos )
            {
                recall_dsp[crt_n].cur_dsp_ptr	= recall_dsp[crt_n].nextpos-1;
                gdk_beep();
        }

        recall_dsp[crt_n].notAdd   = TRUE ;
        if ( rem_dsp_name[crt_n][ cur_dsp_ptr[crt_n] ] )
        {
                OpenDspFile ( crt_n, recall_dsp[crt_n].rem_dsp_name[ recall_dsp[crt_n].cur_dsp_ptr]);
        }
        
        recall_dsp[crt_n].notAdd   = FALSE ; 


        printf ("Next pos : %d  ;;   cur_dsp_no = %d   \n",recall_dsp[crt_n].nextpos , recall_dsp[crt_n].cur_dsp_ptr);  
}

