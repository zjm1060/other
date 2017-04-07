#include "common.h"
#include <QFile>
#include <QTextStream>
int loadLock_CalcuFile(QString filepath,QList<FilePoint>*filenamelist)
{
	FilePoint tmpfile;
	QFile InterLockFile(filepath);//InterLockFile= QFile(filepath);
	if (!InterLockFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return -1;

	QTextStream in(&InterLockFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.contains("BEGIN:",Qt::CaseInsensitive)||line.contains("END",Qt::CaseInsensitive))
		{
			continue; 
		}else
		{
			line.trimmed();
			line.remove(QChar(' '), Qt::CaseInsensitive);
			line.remove(QChar(TAB), Qt::CaseInsensitive);
			line.remove(QChar(LF), Qt::CaseInsensitive);
			line.remove(QChar(CR), Qt::CaseInsensitive);
			line.remove("include", Qt::CaseInsensitive);
			if (line.count()==0)
			{
				continue;
			}
			if (line.contains("#",Qt::CaseInsensitive))
			{
				line.remove("#", Qt::CaseInsensitive);
				tmpfile.filename=line;
				tmpfile.validflag=0;
			}else
			{
				tmpfile.filename=line;
				tmpfile.validflag=1;

			}
			tmpfile.selectflag=0;
			//qDebug()<<"tmpfile.filename=="<<tmpfile.filename;
			filenamelist->append(tmpfile);

		}

	}
	return 1;
}
QList<FilePoint>loadLock_CalcuFile(QString filepath)
{
	QList<FilePoint> tfilenamelist;
	FilePoint tmpfile;
	QFile InterLockFile(filepath);//InterLockFile= QFile(filepath);
	if (!InterLockFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return tfilenamelist;

	QTextStream in(&InterLockFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.contains("BEGIN:",Qt::CaseInsensitive)||line.contains("END",Qt::CaseInsensitive))
		{
			continue; 
		}else
		{
			line.trimmed();
			line.remove(QChar(' '), Qt::CaseInsensitive);
			line.remove(QChar(TAB), Qt::CaseInsensitive);
			line.remove(QChar(LF), Qt::CaseInsensitive);
			line.remove(QChar(CR), Qt::CaseInsensitive);
			line.remove("include", Qt::CaseInsensitive);
			if (line.count()==0)
			{
				continue;
			}
			if (line.contains("#",Qt::CaseInsensitive))
			{
				line.remove("#", Qt::CaseInsensitive);
				tmpfile.filename=line;
				tmpfile.validflag=false;
			}else
			{
				tmpfile.filename=line;
				tmpfile.validflag=true;

			}
			tmpfile.selectflag=0;
			//qDebug()<<"tmpfile.filename=="<<tmpfile.filename;
			tfilenamelist.append(tmpfile);

		}

	}

	return tfilenamelist;
}

