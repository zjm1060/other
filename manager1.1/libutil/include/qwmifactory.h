#ifndef QWMIFACTORY_H
#define QWMIFACTORY_H

#include <QMutex>
#include <QMap>

#include "qabstractwmi.h"

class UTIL_EXPORT QWmiFactory
{
public:
	virtual ~QWmiFactory();

protected:
	QWmiFactory();

public:
	static QWmiFactory* instance();

public:
	QAbstractWmiPtr wmiInstance(int type);
	
	void registerWmi(QAbstractWmiPtr wmi);
	QVariant exec(int type);

	int execMethod(int type, int method, const QStringList& arguments);

public:
	void initialize();
	void release();

public:
	bool loadLibrary(const QString& filename, const char* symbol);

private:
	bool init();

private:
	bool	m_bInitialize;

	IWbemLocator*	m_pLoc;
	IWbemServices*	m_pSvc;

	QMap<int, QAbstractWmiPtr>	m_mapWmi;
};

#endif // QWMIFACTORY_H
