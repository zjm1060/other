
#include <windows.h>
#include <stdio.h>

#include	"resource.h"
#include	"../inc/draw_def.h"
#include	"../inc/ipm_k_def.h"
#include	"../inc/layer_def.h"
#include	"../inc/mode_def.h"
#include	"../inc/text_edit_def.h"
#include	"../inc/zoom_def.h"
#include	"../inc/color_def.h"
#include	"../inc/ipm.h"

void GridInit ();
void DrawInit ();
void EditButtonInit ();
void DynButtonInit ();
void SetCurrentLayerBtn (int);
int	GetColorId(COLORREF rgb);


void IpmInit ()
{
	int	i;

	edit_mode	= DISPLAY_EDIT;

	canvas_bg_color = GetColorId(RGB(90,90,90));
    IsLineDash =  FALSE ;

	/* initialize v4_gdbase -- graphics database. */
	v4_gdbase.nextpos		= 0;
	cur.bck.draw_type	= DrwT_LINE;  //FOM SELECTION
	cur.bck.layer		= 0;
	cur.bck.fill.fill_type	= UNFILLED;
	cur.bck.line_width	= 0;
	cur.bck.ftset_id	= DEFAULT_FONT;
	cur.bck.fg	= GetColorId(RGB(255,255,255));
	cur.bck.bg	= GetColorId(RGB(0,0,0));


	
	GridInit ();
	DrawInit ();
	ZoomInit ();

	EditButtonInit ();
	DynButtonInit ();
	SetCurrentLayerBtn(cur.bck.layer);
    SetPageRage (0 )  ; 
//	info.fb_choice	= k_fg;
	info.sym.class	= 0;
	info.sym.name	= 0;
	info.sym_grp	= 0;
	info.menu	= 0;
	info.clutter	=CLUTTER_ON;

	hide_all_btn_tag = VISIBLE;
	lock_all_btn_tag = UNLOCKED;

	for (i=0; i < MAXLAYERS; i++)
	{
		layerdb[i].min_visible	= (float)zoomdb.factor_min;
		layerdb[i].max_visible	= (float)zoomdb.factor_max;
	}

	CheckMenuItem(GetMenu( ghWnd ), ID_DISPLAY_DBNAME, MF_BYCOMMAND | MF_UNCHECKED);

}


