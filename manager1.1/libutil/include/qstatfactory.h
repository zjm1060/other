#ifndef QSTATFACTORY_H
#define QSTATFACTORY_H

#include <QMutex>
#include <QMap>

#include "qabstractstat.h"

class UTIL_EXPORT QStatFactory
{
public:
	virtual ~QStatFactory();

protected:
	QStatFactory();

public:
	static QStatFactory* instance();

public:
	bool loadLibrary(const QString& filename, const char* symbol);

public:
	QAbstractStatPtr statInstance(int type);
	
	void registerStat(QAbstractStatPtr& statistics);
	QVariant exec(int type);

	int execMethod(int type, int method, const QStringList& arguments);

private:
	QMap<int, QAbstractStatPtr>	m_mapStatistics;
};

#endif // QSTATFACTORY_H
