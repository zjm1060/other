
 
#define	BITMAP_DEF_MODULE

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/fill_bitmap.h"

void ChangeItemsFillPattern ( int);


void CreateFillPalette ()
{
	int 		i;

	for (i = 0; i < MAX_FILL_PATTERN; i++)
	{
		fill_pixmenu[i]	= CreateBitmap(16,16,1,1,ipm_bitmaps[i]);

		if (!fill_pixmenu[i])
		{
			MessageBox(NULL, "Create fill pattern error", "IPM", MB_OK | MB_ICONHAND);
			ExitProc();
            PostQuitMessage (0) ;
            break;
		}
		
	}

}
void ChangeItemsFillBk (int  fill)
{
        int     i;

        for (i=0; i<v4_gdbase.nextpos; i++)
        {
            if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
            {
//				ClearTagItems ( &v4_gdbase.buffer[i].bck );
		        v4_gdbase.buffer[i].bck.fill.fillbkmode = fill;
            }
        }

		InvalidateRect(ghWnd, NULL, TRUE);

}
void     SetFillBk(int isTrans)
{
	 if (gdbuff.nextpos == 0)        /* no item selected */
	{
		cur.bck.fill.fillbkmode    = isTrans;
	}
    else            /* items selected, change item's fill_type */
    {
		ChangeItemsFillBk ( isTrans);
    }

}
void	SetFillPattern(num_fill_ptn)
int	num_fill_ptn;
{

    if (gdbuff.nextpos == 0)        /* no item selected */
	{
		cur.bck.fill.fill_type  = num_fill_ptn;
	}
    else            /* items selected, change item's fill_type */
    {
		ChangeItemsFillPattern ( num_fill_ptn);
    }
  
}


void ChangeItemsFillPattern ( fill)
int     fill;
{
        int     i;

        for (i=0; i<v4_gdbase.nextpos; i++)
        {
            if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
            {
//				ClearTagItems ( &v4_gdbase.buffer[i].bck );
		        v4_gdbase.buffer[i].bck.fill.fill_type	= fill;
            }
        }

		InvalidateRect(ghWnd, NULL, TRUE);

}

