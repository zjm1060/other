#ifndef CHECKTEMPEIGEN_H
#define CHECKTEMPEIGEN_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include "common.h"
#include "mdiMainWin.h"
#include "calldblib.h"
//#define DEF_TEMPFILENAME "temp_eigenvalue.def"
//#define DEF_TEMPERRNAME "/home/ems/h9000/cfgmgr/def/TempErr.log"
//#define DEF_TEMPFILEPATH "/home/ems/h9000/cfgmgr/def/"

#define  TEMPERREGEXPSPLIT "\\s\|^BEGIN:$|^POINT:$|^INCLUDE$|^END$" //分割字符串
#define  TEMPERREGEXPKEY "^BEGIN:$|^POINT:$|^INCLUDE$|^END$|^[0-9]+$"	////关键字不检查

class  CheckTempeigen:QObject
{
	Q_OBJECT
public:
	CheckTempeigen(MdiMainWindow * parent,Tempcfg tempcfg);
	~CheckTempeigen();

	QList<FilePoint> filenamelist ;
	QStringList getlinelist(QString strin,QRegExp rx);
	
	void checkAllTempfile(); //检查所有温度
	int checkOneTempfile(QString filename); //检查一个具体温度文件
	int checkPoint(QStringList listline,QString curfilename,bool pointflag=false);  //检查一行list中的点 是否在库中存在
//	QTextStream getErrFileSteam(){return perrFileStream;}
	//QTextStream *perrFileStream;
private:
	MdiMainWindow * m_parent;
	void initClass();
	int loadTempFile(QString filepath,QList<FilePoint>*filenamelist);
	QString strTempFile;
	QTextStream fileStream;
	QFile *pTempFile;

	QFile *pTempErrFile;
	QTextStream *perrFileStream;
	int lineNum;
	Tempcfg m_tempcfg;

	Calldblib *m_pCalldblib;
};
#endif