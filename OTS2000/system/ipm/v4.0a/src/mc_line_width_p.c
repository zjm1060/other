
#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/toolkit.h"

void ChangeItemsLineWidth ( int);


void    SetLineWidth (line_width)
int     line_width;
{

	if (gdbuff.nextpos == 0)        /* no item selected */
	{  
	   cur.bck.line_width  =  line_width;
	}
    else            /* items selected, change item's line_width */
    {
        ChangeItemsLineWidth ( line_width);
    }
  
} 


void ChangeItemsLineWidth ( line_width)
int     line_width;
{
        int     i;

        for (i=0; i<v4_gdbase.nextpos; i++)
        {
        	if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
            {
//				ClearTagItems ( &v4_gdbase.buffer[i].bck);
 				v4_gdbase.buffer[i].bck.line_width= line_width;
            }
        }

		InvalidateRect(ghWnd, NULL, TRUE);

}




