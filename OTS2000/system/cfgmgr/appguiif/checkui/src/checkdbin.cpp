#include "checkdbin.h"

CheckDbin::CheckDbin(MdiMainWindow * parent,Dbincfg dbincfg)
{
	m_parent=parent;
	m_dbincfg=dbincfg;
	initClass();
}
CheckDbin::~CheckDbin()
{
	if (pDbinErrFile) 
	{
		delete pDbinErrFile;
		pDbinErrFile = 0;             
	}

	if (perrFileStream) 
	{
		delete perrFileStream;
		perrFileStream = 0;             
	}

	
		if (m_pCallIPM) 
		{
			delete m_pCallIPM;
			m_pCallIPM = 0;             
		}
	
}
void CheckDbin::initClass()
{
	m_pCallIPM=new CallIPM();
	pDbinErrFile=NULL;
	perrFileStream=NULL;
	getFileTypeNamelist();
}

void CheckDbin::getFileTypeNamelist()
{
	QDir currentDir = QDir(m_dbincfg.filepath.trimmed());
	QStringList files;
	Type_Name tmptypeName;
	FilePoint tpmfilePoint;
	QString filename;
	for(int i=0;i<m_dbincfg.filetype.count();i++)
	{
		filename=m_dbincfg.filetype.at(i).trimmed()+"*";

		files = currentDir.entryList(QStringList(filename),
			QDir::Files | QDir::NoSymLinks);

		tmptypeName.filetype=m_dbincfg.filetype.at(i).trimmed();
		tmptypeName.filenamelist=files;
		for (int j=0;j<files.count();j++)
		{
			
			tpmfilePoint.selectflag=0;
			tpmfilePoint.validflag=0;

			tmptypeName.filepointnamelist.append(tpmfilePoint);

		}
		fileTypeNamelist.append(tmptypeName);		

	}


}

void CheckDbin::checkAllDbinfile() //检查所有画面文件
{

	QProgressDialog progressDialog(m_parent);
	progressDialog.setCancelButtonText(tr("&Cancel"));
	progressDialog.setRange(0, 100);
	progressDialog.setWindowTitle(tr("Check Dbin Files"));
	qApp->processEvents();

	if (QFile::exists(m_dbincfg.errfile))
	{
		QFile(m_dbincfg.errfile).remove();
	}

	pDbinErrFile=new QFile(m_dbincfg.errfile);
	if (!pDbinErrFile->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<m_dbincfg.errfile<<" error";
		pDbinErrFile->close();
		return;
	}
	perrFileStream=new QTextStream(pDbinErrFile);

	
	QString filename;
	int num=0;
	for (int j=0;j<fileTypeNamelist.count();j++)
	{
		for (int k=0;k<fileTypeNamelist.at(j).filenamelist.count();k++)
		{
			if (fileTypeNamelist.at(j).filepointnamelist.at(k).selectflag==1)
			{
				num++;
				progressDialog.setValue(num);
				//qDebug()<<"fileTypeNamelist.at(j).filenamelist.at(k).filename==j,k"<<j<<k<<fileTypeNamelist.at(j).filenamelist.at(k)<<fileTypeNamelist.at(j).filepointnamelist.at(k).selectflag;
				QString filapahtName=m_dbincfg.filepath.trimmed()+"/"+fileTypeNamelist.at(j).filenamelist.at(k);
				checkOneDbinfile(filapahtName,fileTypeNamelist.at(j).filetype);

			}

		}

	}
	pDbinErrFile->close();

}
int CheckDbin::checkOneDbinfile(QString filename,QString classfy) //检查一个具体画面文                                                                      s件
{
	
	
	m_pCallIPM->openDbin(filename);


	if (m_pCallIPM->Openfile_DynamicsTest()==-1)
	{
		QString errstr=QString("DBIN\t%1\t%2\t%3\thave point not in db \n").arg(filename).arg("").arg(classfy);
		//qDebug()<<errstr;
		*perrFileStream<<errstr;

	}
	
	
return 1;
}