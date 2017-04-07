#ifndef CHECKCALCU_H
#define CHECKCALCU_H
#include <QObject>
#include <QFile>
#include <QTextStream>
 #include <QStringList>
#include "common.h"
#include "calldblib.h"
#include "mdiMainWin.h"
//#define DEF_CALCUFILENAME "/home/ems/h9000/cfgmgr/def/CalcuFile.def"
//#define DEF_CALCUFILENAME1 "/home/ems/h9000/cfgmgr/def/CalcuFile1.def"
//#define DEF_CALCUERRNAME "/home/ems/h9000/cfgmgr/def/CalcuErr.log"
//#define DEF_CALCUFILEPATH "/home/ems/h9000/cfgmgr/def/"

#define  CALCUREGEXPSPLIT "\\s\|^BEGIN:$|^POINT:$|^INCLUDE$|^END$|^CLOSE$|^OPEN$|[!=\-\?\+\&\*()/c<]|^C$|^IF$"
#define  CALCUREGEXPKEY "^BEGIN:$|^POINT:$|^INCLUDE$|^END$|^CLOSE$|^OPEN$|^[0-9]+$|^L$|^IF$|^C$|~|\-|^TMP_FLOAT[0-9]+$|^TMP_STATUS[0-9]+$|^-?([1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*|0?\\.0+|0)$"

class  CheckCalcu:QObject
{
	Q_OBJECT
public:
	CheckCalcu(MdiMainWindow * parent,Calcucfg calcucfg);
	~CheckCalcu();

	QList<FilePoint> filenamelist[2] ;////////////////////此处用个数组
	QStringList getlinelist(QString strin,QRegExp rx);

	void checkAllCalcufile(); //检查所有计算文件
	int checkOneCalcufile(QString filename); //检查一个具体计算文件
	int checkPoint(QStringList listline,QString curfilename);  //检查一行list中的点 是否在库中存在
	QTextStream *getErrFileSteam(){return perrFileStream;}
private:
	void initClass();
MdiMainWindow * m_parent;
	QString strCalcuFile;//strInterLockFile;
	QTextStream fileStream;
	QFile *pCalcuFile;// *pInterlockFile;

	QFile *pCalcuErrFile;//*pInterLockErrFile;
	QTextStream *perrFileStream;
	Calcucfg m_calcucfg;

	int lineNum;

	Calldblib *m_pCalldblib;

};
#endif