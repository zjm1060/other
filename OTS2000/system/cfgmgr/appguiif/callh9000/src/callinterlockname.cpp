#include "callinterlockname.h"
#include <QDebug>
CallInterlockname::CallInterlockname(QString pathname)
{	
	m_pathname=pathname;
	initClass();
}
CallInterlockname::~CallInterlockname()
{

}
int CallInterlockname::initClass()
{
	m_pCalldblib=new Calldblib(DBPATH);

	if(-1==m_pCalldblib->MountDB())
	{
		qDebug()<<"Fail to MountDB!";
		exit(-1);
	}

	pt_num = -1;
	gt_num = gt_num_rec = -1;
	et_num = et_num_rec = 0;
	layer_num = 0;
	f = 0;
	line_num = 0;

	def_filename = m_pathname;
	gen_filename=QString("%1/%2")
		.arg(APP_DEFINE_PATH).arg(INTERLOCK_DATA_FILE);

	err_filename=QString("%1/%2")
		.arg(APP_DEFINE_PATH).arg(ERRFILE);

	if (-1==openerr_file(ferr,err_filename))
	{
		qDebug()<<"open "<<err_filename<<" error"; 
	}

	if (-1==open_deffile(fpdef,def_filename))
	{
		qDebug()<<"open "<<def_filename<<" error"; 
	}


	return 1;





}
int CallInterlockname::openerr_file(QFile* pf,QString filepath)
{
	pf= new QFile(m_pathname);
	if (!pf->open(QIODevice::ReadWrite | QIODevice::Text))
	{
		qDebug()<<"open "<<filepath<<" error";
		pf->close();
		m_pCalldblib->UnMountDB();
		exit(-1);
	}
	err_fileStream.setDevice(pf);

}

int CallInterlockname::open_deffile(QFile* pf,QString filepath)
{
	pf= new QFile(m_pathname);
	if (!pf->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<filepath<<" error";
		pf->close();
		m_pCalldblib->UnMountDB();
		return(-1);
	}

	def_fileStream.setDevice(pf);

	if (DefFgets_Line()==-1)
	{
		printf ("the file is empty! exit now!\n");
		fpdef->close();
		m_pCalldblib->UnMountDB();
		return(-1);
	}
	if (filehead!=def_fileline)
	{
		printf ("The file %s miss BEGIN: the first line is %s Exit now!\n",def_filename,def_fileline);
		fpdef->close();
		m_pCalldblib->UnMountDB();
		exit(-1);
	}
}
int CallInterlockname::DefFgets_Line()
{	
	while(1)
	{
		line_num++;

		def_fileline=def_fileStream.readLine();
		if (def_fileline.count()==0)//(fgets(filebuf,LINE_BUF_CHAR,fpdef)==NULL)
		{
			if (def_fileStream.atEnd())//(feof (fpdef))
			{
				f++;
				printf ("file %s exit without END\n",def_filename);
				//					fprintf (ferr,"file %s exit without END\n",def_filename);
				QString str=QString("file %s exit without END")
					.arg(def_filename);
				err_fileStream<<str;
				return -1;
			}
			else
			{
				f++;
				//					fprintf (ferr,"Error! %3d line: is a null line.\n",line_num);
				QString str=QString("Error! %3d line: is a null line.")
					.arg(line_num);
				err_fileStream<<str;
				continue;
			}
		}
		n=0;
		return n;
	}
}

QStringList CallInterlockname::getlinelist(QString strin,QRegExp rx)
{


	qDebug()<< strin.split( rx,QString::SkipEmptyParts);
	return	strin.split( rx,QString::SkipEmptyParts);

}

int CallInterlockname::checkPoint(QFile* pf)   
{
	QTextStream in(pf);
	while (!in.atEnd()) {
		QString line = in.readLine();
//		process_line(line);
	}
	return 0;
}