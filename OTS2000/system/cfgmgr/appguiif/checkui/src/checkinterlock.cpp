#include "checkinterlock.h"
#include <QDebug>
CheckInterlock::CheckInterlock(MdiMainWindow * parent,Interlockcfg interlockcfg)
{
	m_parent=parent;
	m_interlockcfg=interlockcfg;
	initClass();
}
CheckInterlock::~CheckInterlock()
{

}
void CheckInterlock::initClass()
{
	lineNum=0;
	filenamelist.clear();
	m_pCalldblib=new Calldblib(DBPATH);
	if(-1==m_pCalldblib->MountDB())
	{
		qDebug()<<"Fail to MountDB!";
		exit(-1);
	} 

	loadLock_CalcuFile(m_interlockcfg.interlockfile,&filenamelist);

}
void CheckInterlock::checkAllIntlkfile() //检查所有闭锁文件
{
	if (QFile::exists(m_interlockcfg.errfile))
	{
		QFile(m_interlockcfg.errfile).remove();
	}

	pInterLockErrFile=new QFile(m_interlockcfg.errfile);

	if (!pInterLockErrFile->open(QFile::WriteOnly | QFile::Text))
	{
		qDebug()<<"open "<<m_interlockcfg.errfile<<" error";
		pInterLockErrFile->close();
		return;
	}
	perrFileStream=new QTextStream(pInterLockErrFile);
	
	QString filename;
	for(int i=0;i<filenamelist.count();i++)
	{
		if (filenamelist.at(i).selectflag)
		{
			filename=filenamelist.at(i).filename;
			checkOneIntlkfile(filename);
		}

	}

	pInterLockErrFile->close();
	
}
int CheckInterlock::checkOneIntlkfile(QString filename) //检查一个具体闭锁文件
{
	filename=m_interlockcfg.filepath+"/"+filename;
	pInterlockFile=new QFile(filename);
	if (!pInterlockFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<filename<<" error";
		pInterlockFile->close();
		return(-1);
	}

	bool interlockPoint=false; //当前行是否是 POINT：行
	QTextStream streamIn(pInterlockFile);
	lineNum=0;
	while (!streamIn.atEnd()) {

		QString line = streamIn.readLine();
		lineNum++;
		if (line.contains("POINT:")) //POINT:行为闭锁名不检查
		{
			
			continue;
		}

		QStringList linelist=getlinelist(line,QRegExp(INTERREGEXPSPLIT));
		//lineNum++;
		checkPoint(linelist, filename);

	}
	
	pInterlockFile->close();
	return 1;
}

QStringList CheckInterlock::getlinelist(QString strin,QRegExp rx)
{

	QStringList tmplist=strin.split( rx,QString::SkipEmptyParts);	
	return	tmplist;

}

int CheckInterlock::checkPoint(QStringList listline,QString curfilename)   //检查一行list中的点 是否在库中存在
{
	int check_pt_err=0;
	QString namestr;
	POINTER	point;
	int enorcn=0;
	for (int i=0;i<listline.count();i++)
	{
		namestr=listline.at(i);

		if (namestr.contains(QRegExp(INTERREGEXPKEY))) //关键字不检查
		{
			continue;
		}

		enorcn=0;
		foreach(QChar charitem,namestr)
		{
			if (charitem.unicode()>0x7F)
			{
				enorcn=1;
				break;
			}
		}

		if (enorcn) //汉字不检查
		{
			continue;
		}

		if(-1==m_pCalldblib->qGetPtIdByNameStr(namestr.toLocal8Bit().data(), &point))
		{
			check_pt_err= 1;
			QString errstr=QString("Interlock\t%1\t%2\t%3\tnot in db \n").arg(curfilename).arg(lineNum).arg(namestr);
			//qDebug()<<errstr;
			*perrFileStream<<errstr;
		}
	}

	return check_pt_err;
}

