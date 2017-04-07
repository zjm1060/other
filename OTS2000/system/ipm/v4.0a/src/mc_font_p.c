
#define		DEF_MODULE

#include	"../inc/ipm.h"
#include	"../inc/draw_def.h"
#include	"../inc/text_edit_def.h"

void ChangeFont (int);




void	CreateLogFont()
{
	int		font_id;
	
	for(font_id=0; font_id<MAX_FONTSET; font_id++)
	{
		font_set[font_id] = CreateFont(font_size[font_id].height, 0,
										0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
										OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY,
										VARIABLE_PITCH | FF_DONTCARE, "RS_Song");	//RS_Song  RS_FangSong RS_Kai ËÎÌו


		if( font_set[font_id] == NULL) 
		{
			MessageBox(NULL, " Create font set error", "IPM:", MB_OK | MB_ICONHAND);
			ExitProc();
            PostQuitMessage (0) ;
            break;
		}
	}
}


void	SetFont (ftset_id)
int	ftset_id;
{

	if (gdbuff.nextpos == 0)  
	{
		cur.bck.ftset_id    =ftset_id;
	}
	else            /* items selected, change item's font_set. */
	{
		ChangeFont (ftset_id);
	}

	return;
}

void ChangeFont (ftset_id)
int     ftset_id;
{
	int     i;
	int cyChar,cxChar,iLen ;
	TEXTMETRIC tm;
	SIZE	size;
    HDC	hdc;
    HFONT	hOldFont;
////////////////////////////chi////////////////////////////////////////////////////
    hdc =GetDC(ghWnd)   ;//chi

	for (i=0; i<v4_gdbase.nextpos; i++)
	{
		if (v4_gdbase.buffer[i].bck.tag == ITEM_TAG_ON 
			&& v4_gdbase.buffer[i].bck.draw_type == DrwT_TEXT )
		{

//			GetBaselineCoordFromRcd ( &v4_gdbase.buffer[i].bck, &bsl_x, &bsl_y);
			v4_gdbase.buffer[i].bck.ftset_id = ftset_id;

            hOldFont = 	SelectObject(hdc, font_set[ftset_id]);
	
			if (v4_gdbase.buffer[i].bck.data.text.DirOut == ITEM_TAG_VERTI )
			{
                  //GetRcdVerticHW(  ghWnd, &v4_gdbase.buffer[i], bsl_x, bsl_y) ;
                  GetTextMetrics (hdc, &tm) ;    
                  cyChar = tm.tmHeight + tm.tmExternalLeading ;
                  cxChar = tm.tmHeight ;
				  iLen = _lstrlen(  v4_gdbase.buffer[i].bck.data.text.text );
				  v4_gdbase.buffer[i].bck.rect.x2 = v4_gdbase.buffer[i].bck.rect.x1+cyChar;
        	      v4_gdbase.buffer[i].bck.rect.y2 = v4_gdbase.buffer[i].bck.rect.y1+cyChar * iLen ;
			}
			else
			{
					GetTextExtentPoint( hdc, v4_gdbase.buffer[i].bck.data.text.text, strlen( v4_gdbase.buffer[i].bck.data.text.text),  &size);  //added chi
					SelectObject(hdc, hOldFont);
        			v4_gdbase.buffer[i].bck.rect.x2 = v4_gdbase.buffer[i].bck.rect.x1+size.cx;
        			v4_gdbase.buffer[i].bck.rect.y2 = v4_gdbase.buffer[i].bck.rect.y1+size.cy;
			}

///////////////////////////////////////////////////////////////////////////////////


		}
	}

	InvalidateRect(ghWnd, NULL, TRUE);
}

