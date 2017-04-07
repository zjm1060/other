#include "qabstractfiletransfor.h"

QAbstractFileTransfor::QAbstractFileTransfor()
{

}

QAbstractFileTransfor::~QAbstractFileTransfor()
{
}

void QAbstractFileTransfor::recvRoot(const QString& root)
{
	Q_UNUSED(root);
}

void QAbstractFileTransfor::recvPath(int flag, const QVector<FileInfo>& files)
{
	Q_UNUSED(flag);
	Q_UNUSED(files);
}

void QAbstractFileTransfor::recvDrive(const QVector<FileInfo>& drives)
{
	Q_UNUSED(drives);
}
