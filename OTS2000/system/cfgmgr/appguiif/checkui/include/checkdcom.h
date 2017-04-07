#ifndef CHECKDCOM_H
#define CHECKDCOM_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include "common.h"
#include "mdiMainWin.h"
#include "calldblib.h"
//#define DEF_INTERLOCKFILENAME "/home/ems/h9000/cfgmgr/def/InterLockFile.def"
//#define DEF_INTERLOCKERRNAME "/home/ems/h9000/cfgmgr/def/InterLockErr.log"
//#define DEF_INTERLOCKFILEPATH "/home/ems/h9000/cfgmgr/def/"
#define  DCOMERREGEXPSPLIT "\\s"
#define  DCOMERREGEXPKEY "^BEGIN:$|^POINT:$|^INCLUDE$|^END$|^CLOSE$|^OPEN$|^NULL$"	////关键字不检查

class  CheckDcom:QObject
{
	Q_OBJECT
public:
	CheckDcom(MdiMainWindow * parent,Dcomcfg dcomcfg);
	~CheckDcom();

	QList<FilePoint> iccpfilelist ;
	QList<FilePoint> iec101filelist ;
	QList<FilePoint> iec104filelist ;
	QStringList getlinelist(QString strin,QRegExp rx);
	
	void checkAllDcomfile(); //检查所有点表文件
	int checkOneDcomfile(QString path,Dcomfile dcomfile); //检查一个具体点表文件
	int checkPoint(QStringList listline,QString curfilename);  //检查一行list中的点 是否在库中存在
	void getDcomfileInfo(QString filename,Dcomfile*dcomfile);
	void checkdbpoint(Dcomfile dcomfile,QStringList listline,QString curfilename); //检查逻辑名 
	void checkdbpoint_range(Dcomfile dcomfile,QStringList linelist,QString curfilename);//检查逻辑名&&上下量程
	void checkintlock(Dcomfile dcomfile,QStringList linelist,QString curfilename);//检查闭锁名
private:
	MdiMainWindow * m_parent;
	void initClass();
	void getFileNamelist(QString filetype,QString filepath,QList<FilePoint>*filenamelist);
	

	QString strDcomFile;
	QTextStream fileStream;
	QFile *pDcomFile;

	QFile *pDcomErrFile;
	QTextStream *perrFileStream;
	int lineNum;
	Dcomcfg m_dcomcfg;

	Calldblib *m_pCalldblib;
};
#endif