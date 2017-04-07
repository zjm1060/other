#ifndef QABSTRACTFILETRANSFOR_H
#define QABSTRACTFILETRANSFOR_H

#include "extern.h"

class QAbstractFileTransfor
{
public:
	QAbstractFileTransfor();
	virtual ~QAbstractFileTransfor();

public:
	virtual void recvRoot(const QString& root);
	virtual void recvPath(int flag, const QVector<FileInfo>& files);
	virtual void recvDrive(const QVector<FileInfo>& drives);
};

#endif // QABSTRACTFILETRANSFOR_H
