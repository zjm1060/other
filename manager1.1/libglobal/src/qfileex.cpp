#include "qfileex.h"

#include <QFileInfo>

#ifndef Q_OS_WIN

#include <sys/types.h>
#include <unistd.h>

#endif

#include "common.h"
#include "qdirex.h"

QFileEx::QFileEx(const QString& name)
: QFile(name)
{

}

QFileEx::QFileEx(QObject* parent)
:QFile(parent)
{

}

QFileEx::QFileEx(const QString& name, QObject* parent)
:QFile(name, parent)
{

}

QFileEx::~QFileEx()
{

}

bool QFileEx::open(OpenMode mode)
{
	QFileInfo fileInfo(*this);

	QString strDirPath=getParentDir(fileInfo.absoluteFilePath());

	bool bExist=QFile::exists();
	if (!bExist)
	{
		QDirEx dir;
		if (!dir.exists(strDirPath))
		{
			dir.mkpath(strDirPath);
		}
	}
	
	bool bRet=QFile::open(mode);

#ifndef Q_OS_WIN
	if (bRet && ((mode&QIODevice::WriteOnly) != 0) && !bExist)
	{
		QFileInfo info(strDirPath);
		uint uid=info.ownerId();
		uint gid=info.groupId();

		QString strFileName=fileInfo.absoluteFilePath();

		chown(strFileName.toLocal8Bit().data(), uid, gid);
	}
#endif

	return bRet;
}

bool QFileEx::link(const QString& fileName, const QString& linkName)
{
	bool bExist=QFile::exists(fileName);
	if (!bExist)
	{
		QDirEx dir;
		dir.mkpath(getParentDir(fileName));
	}

	bool bRet=QFile::link(fileName, linkName);

#ifndef Q_OS_WIN 
	if (!bExist)
	{
		QFileInfo info(getParentDir(fileName));
		uint uid=info.ownerId();
		uint gid=info.groupId();

		chown(fileName.toLocal8Bit().data(), uid, gid);
	}
#endif

	return bRet;
}

bool QFileEx::copy(const QString& fileName, const QString& newName)
{
	bool bExist=QFile::exists(newName);
	if (!bExist)
	{
		QDirEx dir;
		dir.mkpath(getParentDir(newName));
	}
	else
	{
		QFile::remove(newName);
	}

	bool bRet=QFile::copy(fileName, newName);

#ifndef Q_OS_WIN 
	if (bRet)
	{
		QFileInfo info(getParentDir(newName));
		uint uid=info.ownerId();
		uint gid=info.groupId();

		chown(newName.toLocal8Bit().data(), uid, gid);
	}
#endif

	return bRet;
}
