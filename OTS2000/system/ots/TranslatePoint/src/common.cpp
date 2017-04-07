#include "inc/common.h"

static BOOL enumUserWindowsCB(HWND hwnd,LPARAM lParam)
{
	long wflags = GetWindowLong(hwnd, GWL_STYLE);
	if(!(wflags & WS_VISIBLE)) return TRUE;
	HWND sndWnd;
	if( !(sndWnd=FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL)) ) return TRUE;
	HWND targetWnd;
	if( !(targetWnd=FindWindowEx(sndWnd, NULL, L"SysListView32", L"FolderView")) ) return TRUE;
	HWND* resultHwnd = (HWND*)lParam;
	*resultHwnd = targetWnd;
	return FALSE;
}
HWND findDesktopIconWnd()
{
	HWND resultHwnd = NULL;
	EnumWindows((WNDENUMPROC)enumUserWindowsCB, (LPARAM)&resultHwnd);
	return resultHwnd;
}

bool CheckPointName(QString &text)
{	
	text = text.trimmed();
	QString pattern("^\\w+\\.\\w+\\.\\w+\\.\\w+$");
	QRegExp rx(pattern);
	bool match = rx.exactMatch(text);
	return match;
}
bool CheckPointId(QString &text)
{
	text = text.trimmed();
	QString pattern("^\\d+\\.\\d+\\.\\d+\\.\\d+$");
	QRegExp rx(pattern);
	bool match = rx.exactMatch(text);
	return match;
}
void GetPointInfo(DMS_COMMON *db_cmm,char *info,int info_type)
{
	char tmp_state[50];
	switch(info_type)
	{
		case TYPE_ID:
			sprintf(info,"%d.%d.%d.%d",db_cmm->point.stn_id,db_cmm->point.dev_id,db_cmm->point.data_type,db_cmm->point.pt_id);
			break;
		case TYPE_NAME:
			GetPtNameById(&db_cmm->point,info,NULL);
			break;
		case TYPE_LONGNAME:
			GetEntryLongnameById(&db_cmm->point,info);
			break;
		case TYPE_VALUE:
			switch(db_cmm->point.data_type)
			{
				case	ANA_TYPE:
				case	PARAM_TYPE:
				case	OUTA_TYPE: 
				case	CALC_TYPE:
					sprintf(info, "%.3f", db_cmm->var.anlg.fValue); 
					break;
				case	POL_TYPE:
				case	SOE_TYPE:
				case	SYSMSG_TYPE:
				case    OUTS_TYPE:
					GetIndPtRtStateStrById(&db_cmm->point, info);
					sprintf(tmp_state,"(%d)",db_cmm->var.ind.status.rt_state);
					strcat(info,tmp_state);
					break;
				case    IMP_TYPE:
					sprintf(info, "%u", db_cmm->var.imp.entry.rt_value); 
					break;
				case	CHARA_TYPE:
					sprintf(info, "%d", db_cmm->var.chara.pt_num); 	
					break;
				case	CURVE_TYPE:
					sprintf(info,"%.0f ", db_cmm->var.curve.data.value[0]);
					break;
				case    DEVOBJ_TYPE:
					sprintf(info,"%d",db_cmm->var.devobj.status.opr);
					break;
				case TIME_TYPE:
					sprintf(info,"%.4f",db_cmm->var.time.coords.x);
					break;
				default:
					break;
			}
		default:
			break;
	}
}
void DelBlank(QString &strs)
{
	int len;
	BOOL isFinish = TRUE;
	if (!strs.isEmpty())
		if (strs.at(0).isSpace())
		{
			strs.remove(0,1);
			isFinish = FALSE;
		}
	len = strs.size();
	if (!strs.isEmpty())
		if (strs.at(--len).isSpace())
		{
			strs.remove(len,1);  
			isFinish = FALSE;
		}
	if (isFinish)
		return;
	DelBlank(strs);
}