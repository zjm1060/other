#ifndef QDIREX_H
#define QDIREX_H

#include <QDir>

#ifdef Q_OS_WIN
#define QDirEx QDir
#else

#include "global.h"

class GLOBAL_EXPORT QDirEx : public QDir
{
public:
	QDirEx(const QString & path = QString());
	~QDirEx();

public:
	bool mkpath(const QString& dirPath) const;

private:

};
#endif

#endif // QDIREX_H
