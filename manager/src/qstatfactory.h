#ifndef QSTATFACTORY_H
#define QSTATFACTORY_H

#include <QMutex>
#include <QMap>

#include "qabstractstat.h"

class QStatFactory
{
public:
	virtual ~QStatFactory();

protected:
	QStatFactory();

public:
	static QStatFactory* instance();

public:
	QAbstractStat* statInstance(int type);
	
	void registerStat(QAbstractStat* statistics);
	QVariant exec(int type);

	int execMethod(int type, int method, const QStringList& arguments);

private:
	static QMutex			m_mutex;
	static QStatFactory*	m_pInstance;

	QMap<int, QAbstractStat*>	m_mapStatistics;
};

#endif // QSTATFACTORY_H
