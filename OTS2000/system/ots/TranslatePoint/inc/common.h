#ifndef _COMMON_H
#define _COMMON_H

#include <QtCore/QCoreApplication>
#include <QRegExp>
#include <windows.h>
#include "/home/ems/system/ots/ots_hdb/inc/ots.h"
#pragma warning( disable : 4996 )

enum    POINT_INFO_TYPE  
{ 
	TYPE_ID = 0,			//点号
	TYPE_NAME = 1,			//点逻辑名
	TYPE_LONGNAME = 2,		//点中文名
	TYPE_VALUE = 3,			//点值
	TYPE_STATE = 4			//点状态
};

bool CheckPointName(QString &text);
bool CheckPointId(QString &text);
void DelBlank(QString &strs);
void GetPointInfo(DMS_COMMON *db_cmm,char *info,int info_type);
static BOOL enumUserWindowsCB(HWND hwnd,LPARAM lParam);
HWND findDesktopIconWnd();

#endif