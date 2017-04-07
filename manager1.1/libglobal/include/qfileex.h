#ifndef QFILEEX_H
#define QFILEEX_H

#include <QFile>

#include "global.h"

class GLOBAL_EXPORT QFileEx : public QFile
{
public:
	QFileEx(const QString& name);
	QFileEx(QObject* parent);
	QFileEx(const QString& name, QObject* parent);

	~QFileEx();

public:
	virtual bool open(OpenMode mode);

public:
	static bool link(const QString& fileName, const QString& linkName);
	static bool copy(const QString& fileName, const QString& newName);
};

#endif // QFILEEX_H
