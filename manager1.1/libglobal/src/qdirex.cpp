#include "qdirex.h"

#ifndef Q_OS_WIN

#include "common.h"

#include <sys/types.h>
#include <unistd.h>

QDirEx::QDirEx(const QString & path)
: QDir(path)
{

}

QDirEx::~QDirEx()
{

}

bool QDirEx::mkpath(const QString& dirPath) const
{
	QDir dir;
	if (dir.exists(dirPath))
	{
		return true;
	}

	QString strDirPath=dirPath;
	QString strTemp;
	while ((strTemp=getParentDir(strDirPath)) != strDirPath)
	{
		strDirPath=strTemp;
		if (dir.exists(strDirPath))
		{
			break;
		}
	}

	QFileInfo info(strDirPath);
	uint uid=info.ownerId();
	uint gid=info.groupId();

	QDir::mkpath(dirPath);

	QString strDir=dirPath;
	do 
	{
		chown(strDir.toLocal8Bit().data(), uid, gid);
	} while ((strDir=getParentDir(strDir)) != strDirPath);
}
#endif
