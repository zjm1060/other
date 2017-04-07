#ifndef QDEAFACTORY_H
#define QDEAFACTORY_H

#include <QMap>

#include "global.h"
#include "qidea.h"

class UTIL_EXPORT QDEAFactory
{
public:
	virtual ~QDEAFactory();

protected:
	QDEAFactory();

public:
	bool registerInstance(const QIDEAPtr& msg);
	QIDEAPtr dea(int type);
	QIDEAPtr dea(const QString& desc);

public:
	bool loadLibrary(const QString& filename, const char* symbol);

public:
	static QDEAFactory* instance();

private:
	QMap<int, QIDEAPtr > m_mapInstances;
};

#endif // QDEAFACTORY_H
