#include "checkdcom.h"
#include <QDebug>
 #include <QDir>
#include <QMessageBox>
CheckDcom::CheckDcom(MdiMainWindow * parent,Dcomcfg dcomcfg)
{
	m_parent=parent;
	m_dcomcfg=dcomcfg;
	initClass();
}
CheckDcom::~CheckDcom()
{
	m_pCalldblib->qInterLockFree();
}
void CheckDcom::initClass()
{
	lineNum=0;
	iccpfilelist.clear();
	iec101filelist.clear();
	iec104filelist.clear();
	m_pCalldblib=new Calldblib(DBPATH);
	if(-1==m_pCalldblib->MountDB())
	{
		qDebug()<<"Fail to MountDB!";
		QMessageBox::information(m_parent, tr("MountDB"),
			tr("Fail to MountDB!.\nplease check DATABASE.OUT and restart!"));
	
		exit(-1);
	} 
	if(-1==m_pCalldblib->qInterLockFileLoad())
	{
		
		qDebug()<<"Fail to load InterLockFile";
		QMessageBox::information(m_parent, tr("Load InterLockFile"),
			tr("Fail to load InterLockFile!.\nplease check InterLockGen.dat and restart!"));
		//exit(-1);

	}
for (int i=0;i<m_dcomcfg.dcompathlist.count();i++)
{
	if (m_dcomcfg.dcompathlist.at(i).filetype.operator==("iccp"))
	{
		getFileNamelist(m_dcomcfg.dcompathlist.at(i).keyword,m_dcomcfg.dcompathlist.at(i).filepath,&iccpfilelist);
	}else if (m_dcomcfg.dcompathlist.at(i).filetype.operator==("iec101"))
	{
		getFileNamelist(m_dcomcfg.dcompathlist.at(i).keyword,m_dcomcfg.dcompathlist.at(i).filepath,&iec101filelist);
	}else if (m_dcomcfg.dcompathlist.at(i).filetype.operator==("iec104"))
	{
		getFileNamelist(m_dcomcfg.dcompathlist.at(i).keyword,m_dcomcfg.dcompathlist.at(i).filepath,&iec104filelist);
	}
}


}
void CheckDcom::getFileNamelist(QString filetype,QString filepath,QList<FilePoint>*filenamelist)
{
	QDir currentDir = QDir(filepath.trimmed());
	QStringList files;
	FilePoint tpmfilePoint;
	QString filename= QString("*%1*.def").arg(filetype);

		files = currentDir.entryList(QStringList(filename),
			QDir::Files | QDir::NoSymLinks);

		for (int j=0;j<files.count();j++)
		{
			tpmfilePoint.selectflag=0;
			tpmfilePoint.validflag=0;

			tpmfilePoint.filename=files.at(j);
		
		filenamelist->append(tpmfilePoint);		
		}

}
void  CheckDcom::getDcomfileInfo(QString filename,Dcomfile*dcomfile)
{
	for (int i=0;i<m_dcomcfg.dcomfilelist.count();i++)
	{
		if (filename==m_dcomcfg.dcomfilelist.at(i).filename)
		{
			 *dcomfile=m_dcomcfg.dcomfilelist.at(i);
			 return;
		}
	}
	

}
void CheckDcom::checkAllDcomfile() //检查所有点表文件
{
	if (QFile::exists(m_dcomcfg.errfile))
	{
		QFile(m_dcomcfg.errfile).remove();
	}

	pDcomErrFile=new QFile(m_dcomcfg.errfile);

	if (!pDcomErrFile->open(QFile::WriteOnly | QFile::Text))
	{
		qDebug()<<"open "<<m_dcomcfg.errfile<<" error";
		pDcomErrFile->close();
		return;
	}
	perrFileStream=new QTextStream(pDcomErrFile);
	
	QString filename;
	Dcomfile tmpdcomfile;

	for (int i=0;i<m_dcomcfg.dcompathlist.count();i++)
	{
		if (m_dcomcfg.dcompathlist.at(i).filetype.operator==("iccp"))
		{			
			for(int j=0;j<iccpfilelist.count();j++)
			{
				if (iccpfilelist.at(j).selectflag)
				{
					filename=iccpfilelist.at(j).filename;
					getDcomfileInfo( filename,&tmpdcomfile);
					checkOneDcomfile(m_dcomcfg.dcompathlist.at(i).filepath,tmpdcomfile);
				}

			}

		}else if (m_dcomcfg.dcompathlist.at(i).filetype.operator==("iec101"))
		{			
			for(int j=0;j<iec101filelist.count();j++)
			{
				if (iec101filelist.at(j).selectflag)
				{
					filename=iec101filelist.at(j).filename;
					getDcomfileInfo( filename,&tmpdcomfile);
					checkOneDcomfile(m_dcomcfg.dcompathlist.at(i).filepath,tmpdcomfile);
				}

			}
		}else if (m_dcomcfg.dcompathlist.at(i).filetype.operator==("iec104"))
		{		
			for(int j=0;j<iec104filelist.count();j++)
			{
				if (iec104filelist.at(j).selectflag)
				{
					filename=iec104filelist.at(j).filename;
					getDcomfileInfo( filename,&tmpdcomfile);
					checkOneDcomfile(m_dcomcfg.dcompathlist.at(i).filepath,tmpdcomfile);
				}

			}
		}
	}

	pDcomErrFile->close();
	
}
int CheckDcom::checkOneDcomfile(QString path,Dcomfile dcomfile) //检查一个具体点表文件
{

	QString filename=path+"/"+dcomfile.filename;
	pDcomFile=new QFile(filename);
	if (!pDcomFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<filename<<" error";
		pDcomFile->close();
		return(-1);
	}

	
	QTextStream streamIn(pDcomFile);
	lineNum=0;
	while (!streamIn.atEnd()) {

		QString line = streamIn.readLine();
		QStringList linelist=getlinelist(line,QRegExp(DCOMERREGEXPSPLIT));
		lineNum++;

		if (line.contains("#")||linelist.count()==0) //#注释行不检查
		{

			continue;
		}
		if (dcomfile.checkrange)
		{
			checkdbpoint_range(dcomfile,linelist,filename);
		}
		else{
			checkdbpoint(dcomfile,linelist,filename);//检查数据库点
		}	

		if (dcomfile.checkinterlock)
		{
			checkintlock(dcomfile,linelist,filename);//检查闭锁名
		}
		
	}
	
	pDcomFile->close();
	return 1;
}
void CheckDcom::checkdbpoint(Dcomfile dcomfile,QStringList linelist,QString curfilename) //检查逻辑名
{
	POINTER	point;
	QString namestr;
	
	namestr=linelist.at(dcomfile.nameindex);

	if (namestr.contains(QRegExp(DCOMERREGEXPKEY)))
	{
		return;
	}
	if(-1==m_pCalldblib->qGetPtIdByNameStr(namestr.toLocal8Bit().data(), &point))
	{
		QString errstr=QString("DCOM\t%1\t%2\t%3\tnot in db \n").arg(curfilename).arg(lineNum).arg(namestr);
		//qDebug()<<errstr;
		*perrFileStream<<errstr;
	}


}
void CheckDcom::checkdbpoint_range(Dcomfile dcomfile,QStringList linelist,QString curfilename) //检查逻辑名&&上下量程
{
	POINTER	point;
	
	QString namestr;
	namestr=linelist.at(dcomfile.nameindex);
	if (namestr.contains(QRegExp(DCOMERREGEXPKEY)))
	{
		return;
	}

	if(-1==m_pCalldblib->qGetPtIdByNameStr(namestr.toLocal8Bit().data(), &point))
	{
		QString errstr=QString("DCOM\t%1\t%2\t%3\tnot in db \n").arg(curfilename).arg(lineNum).arg(namestr);
		//qDebug()<<errstr;
		*perrFileStream<<errstr;
		
		return;
	}

	if (dcomfile.checkrange)
	{

		ANA_ENG_PARA *eng_para=(ANA_ENG_PARA *)malloc(sizeof(ANA_ENG_PARA));
		LMT_PARA *lmt_para=(LMT_PARA *)malloc(sizeof(LMT_PARA)) ;
		float high_range;
		float low_range;
		QString highstr=linelist.at(dcomfile.HI_RANGEindex);
		QString lowstr=linelist.at(dcomfile.LO_RANGEindex);
		high_range=highstr.toFloat();
		low_range=lowstr.toFloat();

		m_pCalldblib->qGetAnaPtEngLmtParaByNameStr(namestr.toLocal8Bit().data(),eng_para,lmt_para);	
		
		if (high_range-eng_para->HI_RANGE)
		{
			QString errstr=QString("DCOM\t%1\t%2\t%3\t ERR HI_RANGE=%4\n").arg(curfilename).arg(lineNum).arg(namestr).arg(eng_para->HI_RANGE);
			//qDebug()<<errstr;
			*perrFileStream<<errstr;

		}


		if (low_range-eng_para->LO_RANGE)
		{
			QString errstr=QString("DCOM\t%1\t%2\t%3\t ERR LO_RANGE ERR=%4\n").arg(curfilename).arg(lineNum).arg(namestr).arg(eng_para->LO_RANGE);
			//qDebug()<<errstr;
			*perrFileStream<<errstr;

		}

		if(eng_para) free(eng_para);
		if(lmt_para) free(lmt_para);

	}

}
void CheckDcom::checkintlock(Dcomfile dcomfile,QStringList linelist,QString curfilename)//检查闭锁名
{
	QString interlockname=linelist.at(dcomfile.interlockIndex);

	if (interlockname.contains(QRegExp(DCOMERREGEXPKEY)))
	{
		return;
	}

	if (-1==m_pCalldblib->qInterLock_Judge(interlockname.toLocal8Bit().data()))
	{
		QString errstr=QString("DCOM\t%1\t%2\t%3\t Interlockname ERR\n").arg(curfilename).arg(lineNum).arg(interlockname);
		//qDebug()<<errstr;
		*perrFileStream<<errstr;

	}
}
QStringList CheckDcom::getlinelist(QString strin,QRegExp rx)
{

	QStringList tmplist=strin.split( rx,QString::SkipEmptyParts);
	return	tmplist;

}

int CheckDcom::checkPoint(QStringList listline,QString curfilename)   //检查一行list中的点 是否在库中存在
{
	int check_pt_err=0;
	QString namestr;
	POINTER	point;
	int enorcn=0;
	for (int i=0;i<listline.count();i++)
	{
		namestr=listline.at(i);

		if (namestr.contains(QRegExp(DCOMERREGEXPKEY))) //关键字不检查
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

