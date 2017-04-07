/* 
 * File:   popupmenu.h
 * Author: CHI HAILONG
 *
 * Created on 2006年7月14日, 下午2:34
 * Should must be svae as  ***UTF8!!!!!
 * UNIX 已经与WINDOWS版本同步!!! chihailong  2007.10.23 
 */

#ifndef _POPUP_MENU_H
#define	_POPUP_MENU_H

#ifdef	__cplusplus
extern "C" {
#endif 
static GtkItemFactoryEntry menu_items[] = {
  { OIX_CSTR_MENU_CONSOLE,        NULL,        consoleOut,           0, "<Item>" },
  { OIX_CSTR_MENU_SEP1,     NULL,         NULL,           0, "<Separator>"  },
  { OIX_CSTR_MENU_DSP,         NULL,         NULL,           0, "<Branch>" },
  { OIX_CSTR_MENU_OPENDSP,     "<control>N", pic_proc,   0, "<Item>" },
  { OIX_CSTR_MENU_COMMONDIR,    "<control>O", pic_proc,   1, "<Item>" },
  { OIX_CSTR_MENU_SINGLELINE, NULL,         pic_proc,           2, "<Item>" },
  { OIX_CSTR_MENU_DSPSEP1,     NULL,         NULL,           0, "<Separator>" },
  { OIX_CSTR_MENU_EXIT,     "<control>Q", gtk_main_win_quit,  0, "<Item>" },
  { OIX_CSTR_MENU_ZOOMIN,       "<control>U",         zoom_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_ZOOMOUT,      "<control>D",         zoom_proc,           1, "<Item>" },
{ OIX_CSTR_MENU_PRINT,      "<control>P",         Print_proc,           1, "<Item>" },
  { OIX_CSTR_MENU_SEP1,     NULL,         NULL,           0, "<Separator>"  },
  { OIX_CSTR_MENU_PTINFO,     NULL,        ptInfo_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_PTEVENT,     NULL,        ptevent_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_PTCHECK,     NULL,        ptCheck_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_OBJ,     NULL,        ptObjInfo_proc,           0, "<Item>" },  
  { "OIX_CSTR_MENU_REAL_CURVE",     NULL,        ptrealcurve_proc_evt,           0, "<Item>" },
  { "OIX_CSTR_MENU_SWITCH_SCENE",     NULL,        ptSwitchScene_proc_evt,           0, "<Item>" },
  { OIX_CSTR_MENU_SEP1,     NULL,         NULL,           0, "<Separator>"  },
  { OIX_CSTR_MENU_ALMACK,        NULL,        ptAlarmAck,           0, "<Item>" },
  {OIX_CSTR_MENU_ONTAG,		NULL,			ptOnTag_proc,           0, "<Item>" },
  {OIX_CSTR_MENU_OFFTAG,		NULL,			ptOffTag_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_SEP1,     NULL,         NULL,           0, "<Separator>"  },
  { OIX_CSTR_MENU_HELP,         NULL,         NULL,           0, "<LastBranch>" },
  { OIX_CSTR_MENU_ABOUT,   NULL,         NULL,           0, "<Item>" },
};

static GtkItemFactoryEntry menu_items_obj_disable[] = {
  { OIX_CSTR_MENU_CONSOLE,        NULL,        consoleOut,           0, "<Item>" },
  { OIX_CSTR_MENU_SEP1,     NULL,         NULL,           0, "<Separator>"  },
  { OIX_CSTR_MENU_DSP,         NULL,         NULL,           0, "<Branch>" },
  { OIX_CSTR_MENU_OPENDSP,     "<control>N", pic_proc,   0, "<Item>" },
  { OIX_CSTR_MENU_COMMONDIR,    "<control>O", pic_proc,   1, "<Item>" },
  { OIX_CSTR_MENU_SINGLELINE, NULL,         pic_proc,           2, "<Item>" },
  {OIX_CSTR_MENU_DSPSEP1,     NULL,         NULL,           0, "<Separator>" },
  { OIX_CSTR_MENU_EXIT,     "<control>Q", gtk_main_win_quit,  0, "<Item>" },
  { OIX_CSTR_MENU_ZOOMIN,       "<control>U",         zoom_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_ZOOMOUT,      "<control>D",         zoom_proc,           1, "<Item>" },
  { OIX_CSTR_MENU_PRINT,      "<control>P",         Print_proc,           1, "<Item>" },
  { OIX_CSTR_MENU_SEP1,     NULL,         NULL,           0, "<Separator>"  },
  { OIX_CSTR_MENU_PTINFO,     NULL,        ptInfo_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_PTEVENT,     NULL,        ptevent_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_PTCHECK,     NULL,        ptCheck_proc,           0, "<Item>" },
  { "OIX_CSTR_MENU_REAL_CURVE",     NULL,        ptrealcurve_proc_evt,           0, "<Item>" },
  { "OIX_CSTR_MENU_SWITCH_SCENE",     NULL,        ptSwitchScene_proc_evt,           0, "<Item>" },
  { OIX_CSTR_MENU_SEP1,     NULL,         NULL,           0, "<Separator>"  },
  { OIX_CSTR_MENU_ALMACK,        NULL,        ptAlarmAck,           0, "<Item>" },
  {OIX_CSTR_MENU_ONTAG,		NULL,			ptOnTag_proc,           0, "<Item>" },
  {OIX_CSTR_MENU_OFFTAG,		NULL,			ptOffTag_proc,           0, "<Item>" },
  { OIX_CSTR_MENU_SEP1,     NULL,         NULL,           0, "<Separator>"  },
  { OIX_CSTR_MENU_HELP,         NULL,         NULL,           0, "<LastBranch>" },
  { OIX_CSTR_MENU_ABOUT,   NULL,         NULL,           0, "<Item>" },
};


static GtkItemFactoryEntry menu_items_evt[] = {
	{ OIX_CSTR_MENU_PTINFO,     NULL,        ptInfo_proc_evt,           0, "<Item>" },
	{ OIX_CSTR_MENU_OPENPTDSP,     NULL, ptPic_proc_evt,   0, "<Item>" },
	{ OIX_CSTR_MENU_RELATEPT,     NULL, ptrelate_proc_evt,   0, "<Item>" },
	{ "OIX_CSTR_MENU_REAL_CURVE",     NULL,        ptrealcurve_proc_evt,           0, "<Item>" },
};

#ifdef	__cplusplus
}
#endif

#endif	/* _1111_H */


 
