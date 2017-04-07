
#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"

void EditButtonInit();
void ClearTagItems (HWND, BACKRCD *);

void ClearGdbItemsTag ()
{
	int	i;
	int	nextpos;

	gdbuff.nextpos	= 0;
	nextpos		= v4_gdbase.nextpos;

	for (i=0; i < nextpos; i++)
	{
		v4_gdbase.buffer[i].bck.tag =ITEM_TAG_OFF;
	}	
}

void DeleteGdbItems (void)
{
	int	i, new_ic;

	new_ic	=0;

	for (i=0; i < v4_gdbase.nextpos; i++)
	{
		if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_OFF)
		{
			v4_gdbase.buffer[new_ic] = v4_gdbase.buffer[i];
			new_ic++;
		}
		else if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON)
		{
//			ClearTagItems (&v4_gdbase.buffer[i].bck );
		}
	}	

	ClearGdbItemsTag ();
	EditButtonInit ();
	v4_gdbase.nextpos	= new_ic;
	cur.bck.draw_type	= DrwT_SELECTION;
	InvalidateRect(ghWnd, NULL, TRUE);
}

