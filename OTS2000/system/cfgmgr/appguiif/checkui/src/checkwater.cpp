#include "checkwater.h"
#include <QDebug>
#include <QMessageBox>
CheckWaterRevert::CheckWaterRevert(MdiMainWindow * parent,Watercfg watercfg)
{
	m_parent=parent;
	m_watercfg=watercfg;
	initClass();
}
CheckWaterRevert::~CheckWaterRevert()
{

}
void CheckWaterRevert::initClass()
{
	lineNum=0;
	filenamelist.clear();
	m_pCalldblib=new Calldblib(DBPATH);
	if(-1==m_pCalldblib->MountDB())
	{
		qDebug()<<"Fail to MountDB!";
		QMessageBox::information(m_parent, tr("MountDB"),
			tr("Fail to MountDB!.\nplease check DATABASE.OUT and restart!"));
		exit(-1);
	} 


	loadTempFile(m_watercfg.waterfile,&filenamelist);
}
int CheckWaterRevert::loadTempFile(QString filepath,QList<FilePoint>*filenamelist)
{
	FilePoint tmpfile;

	tmpfile.filename=filepath;
	tmpfile.selectflag=0;
	tmpfile.validflag=1;



	filenamelist->append(tmpfile);

return 1;

}
void CheckWaterRevert::checkAllWaterfile() //检查所有闭锁文件
{
	if (QFile::exists(m_watercfg.errfile))
	{
		QFile(m_watercfg.errfile).remove();
	}

	pWaterErrFile=new QFile(m_watercfg.errfile);

	if (!pWaterErrFile->open(QFile::WriteOnly | QFile::Text))
	{
		qDebug()<<"open "<<m_watercfg.errfile<<" error";
		pWaterErrFile->close();
		return;
	}
	perrFileStream=new QTextStream(pWaterErrFile);

	QString filename;
	for(int i=0;i<filenamelist.count();i++)
	{
		if (filenamelist.at(i).selectflag)
		{
			filename=filenamelist.at(i).filename;
			checkOneWaterfile(filename);
		}

	}

	pWaterErrFile->close();
	
}
int CheckWaterRevert::checkOneWaterfile(QString filename) //检查一个具体闭锁文件
{
	filename=m_watercfg.filepath+"/"+filename;
	pWaterFile=new QFile(filename);
	if (!pWaterFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<filename<<" error";
		pWaterFile->close();
		return(-1);
	}

	bool interlockPoint=false; //当前行是否是 POINT：行
	QTextStream streamIn(pWaterFile);
	lineNum=0;
	while (!streamIn.atEnd()) {

		QString line = streamIn.readLine();

		if (line.contains("POINT:"))
		{
			interlockPoint=true;
		}
		else
		{
			interlockPoint=false;
		}
		QStringList linelist=getlinelist(line,QRegExp(WATERERREGEXPSPLIT));
		lineNum++;
		checkPoint(linelist, filename,interlockPoint);

	}
	
	pWaterFile->close();
	return 1;
}

QStringList CheckWaterRevert::getlinelist(QString strin,QRegExp rx)
{

	QStringList tmplist=strin.split( rx,QString::SkipEmptyParts);
	return	tmplist;

}

int CheckWaterRevert::checkPoint(QStringList listline,QString curfilename,bool pointflag)   //检查一行list中的点 是否在库中存在
{
	int check_pt_err=0;
	QString namestr;
	POINTER	point;
	int enorcn=0;
	for (int i=0;i<listline.count();i++)
	{
		namestr=listline.at(i);

		if (namestr.contains(QRegExp(WATERERREGEXPKEY))) //关键字不检查
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
			QString errstr=QString("WATER\t%1\t%2\t%3\tnot in db \n").arg(curfilename).arg(lineNum).arg(namestr);
			//qDebug()<<errstr;
			*perrFileStream<<errstr;
		}
	}

	return check_pt_err;
}

