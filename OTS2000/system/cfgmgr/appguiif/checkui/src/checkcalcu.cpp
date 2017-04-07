#include "checkcalcu.h"
#include <QDebug>
#include <QMessageBox>
CheckCalcu::CheckCalcu(MdiMainWindow * parent,Calcucfg calcucfg)
{
	m_parent=parent;
	m_calcucfg=calcucfg;

	initClass();
}
CheckCalcu::~CheckCalcu()
{

}

void CheckCalcu::initClass()
{
	lineNum=0;

	m_pCalldblib=new Calldblib(DBPATH);
	if(-1==m_pCalldblib->MountDB())
	{
		qDebug()<<"Fail to MountDB!";
		QMessageBox::information(m_parent, tr("MountDB"),
			tr("Fail to MountDB!.\nplease check DATABASE.OUT and restart!"));
		exit(-1);
	}

	filenamelist[0].clear();
	loadLock_CalcuFile(m_calcucfg.calcufile,&filenamelist[0]);

	filenamelist[1].clear();
	loadLock_CalcuFile(m_calcucfg.calcufile1,&filenamelist[1]);

}
void CheckCalcu::checkAllCalcufile() //检查所有计算文件
{

	if (QFile::exists(m_calcucfg.errfile))
	{
		QFile(m_calcucfg.errfile).remove();
	}

	pCalcuErrFile=new QFile(m_calcucfg.errfile);
	if (!pCalcuErrFile->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<m_calcucfg.errfile<<" error";
		pCalcuErrFile->close();
		return;
	}
	perrFileStream=new QTextStream(pCalcuErrFile);

	QString filename;
	for (int j=0;j<2;j++)
	{

		for(int i=0;i<filenamelist[j].count();i++)
		{
			if (filenamelist[j].at(i).selectflag)
			{
				filename=filenamelist[j].at(i).filename;
				checkOneCalcufile(filename);
			}

		}
	}
	pCalcuErrFile->close();

}
int CheckCalcu::checkOneCalcufile(QString filename) //检查一个具体计算文件
{
	filename=m_calcucfg.filepath+"/"+filename;
	pCalcuFile=new QFile(filename);
	if (!pCalcuFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<filename<<" error";
		pCalcuFile->close();
		return(-1);
	}

	QTextStream streamIn(pCalcuFile);
	lineNum=0;
	while (!streamIn.atEnd()) {
		
		QString line = streamIn.readLine();
		QStringList linelist=getlinelist(line,QRegExp(CALCUREGEXPSPLIT));
		lineNum++;
		checkPoint(linelist, filename);
		
	}
	
	pCalcuFile->close();
	return 1;
}

QStringList CheckCalcu::getlinelist(QString strin,QRegExp rx)
{
	QStringList tmplist=strin.split( rx,QString::SkipEmptyParts);
	return	tmplist;
}

int CheckCalcu::checkPoint(QStringList listline,QString curfilename)   //检查一行list中的点 是否在库中存在
{
	int check_pt_err=0;
	QString namestr;
	POINTER	point;
	int enorcn=0;
	unsigned int 	chan_fail;
	float	fValue;
	for (int i=0;i<listline.count();i++)
	{
		namestr=listline.at(i);

		if (namestr.contains(QRegExp(CALCUREGEXPKEY))) //关键字不检查
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

		if(-1==m_pCalldblib->qGetPtDataByStr(namestr.toLocal8Bit().data(),&fValue, &chan_fail, &point))
		{
			check_pt_err= 1;
			QString errstr=QString("Calcu\t%1\t%2\t%3\tnot in db \n").arg(curfilename).arg(lineNum).arg(namestr);
			*perrFileStream<<errstr;
		}
	}

	return check_pt_err;
}

