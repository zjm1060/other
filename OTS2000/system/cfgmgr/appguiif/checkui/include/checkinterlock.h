#ifndef CHECKINTERLOCK_H
#define CHECKINTERLOCK_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include "common.h"
#include "callinterlockname.h"
#include "mdiMainWin.h"
#include "calldblib.h"
//#define DEF_INTERLOCKFILENAME "/home/ems/h9000/cfgmgr/def/InterLockFile.def"
//#define DEF_INTERLOCKERRNAME "/home/ems/h9000/cfgmgr/def/InterLockErr.log"
//#define DEF_INTERLOCKFILEPATH "/home/ems/h9000/cfgmgr/def/"

#define  INTERREGEXPSPLIT "\\s\|^BEGIN:$|^POINT:$|^INCLUDE$|^END$|^CLOSE$|^OPEN$|[=-\?\+\&()/</>]" //分割字符串
#define  INTERREGEXPKEY "^BEGIN:$|^POINT:$|^INCLUDE$|^END$|^CLOSE$|^OPEN$|^[0-9]+$"	////关键字不检查

class  CheckInterlock:QObject
{
	Q_OBJECT
public:
	CheckInterlock(MdiMainWindow * parent,Interlockcfg interlockcfg);
	~CheckInterlock();

	QList<FilePoint> filenamelist ;
	QStringList getlinelist(QString strin,QRegExp rx);
	
	void checkAllIntlkfile(); //检查所有闭锁文件
	int checkOneIntlkfile(QString filename); //检查一个具体闭锁文件
	int checkPoint(QStringList listline,QString curfilename);  //检查一行list中的点 是否在库中存在
//	QTextStream getErrFileSteam(){return perrFileStream;}
	//QTextStream *perrFileStream;
private:
	MdiMainWindow * m_parent;
	void initClass();

	QString strInterLockFile;
	QTextStream fileStream;
	QFile *pInterlockFile;

	QFile *pInterLockErrFile;
	QTextStream *perrFileStream;
	int lineNum;
	Interlockcfg m_interlockcfg;

	Calldblib *m_pCalldblib;
};
#endif