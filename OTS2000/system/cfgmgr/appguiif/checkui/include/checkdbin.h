#ifndef CHECKDBIN_H
#define CHECKDBIN_H
#include <QObject>
#include <QFile>
#include <QDialog>
 #include <QProgressDialog>
#include <QTextStream>
#include <QStringList>
#include "operatexml.h"
#include "common.h"
#include "mdiMainWin.h"
#include "callipm.h"

//#define DEF_DBINERRNAME "/home/ems/h9000/cfgmgr/def/DbinErr.log"
class  CheckDbin:QObject
{
	Q_OBJECT
public:
	CheckDbin(MdiMainWindow * parent,Dbincfg dbincfg);
	~CheckDbin();
	QList <Type_Name> fileTypeNamelist;

	void checkAllDbinfile(); //检查所有画面文件
private:

	void initClass();
	void getFileTypeNamelist();
	
	int checkOneDbinfile(QString filename,QString classfy=""); //检查一个画面文件

	QFile *pDbinErrFile;
	QTextStream *perrFileStream;

	QStringList fileNamelist;
	MdiMainWindow * m_parent;
	Dbincfg m_dbincfg;

	CallIPM *m_pCallIPM;
	
};
#endif