#ifndef QWMIFACTORY_H
#define QWMIFACTORY_H

#include <QMutex>
#include <QMap>

#include "qabstractwmi.h"

class QWmiFactory
{
public:
	virtual ~QWmiFactory();

protected:
	QWmiFactory();

public:
	static QWmiFactory* instance();

public:
	QAbstractWmi* wmiInstance(int type);
	
	void registerWmi(QAbstractWmi* wmi);
	QVariant exec(int type);

	int execMethod(int type, int method, const QStringList& arguments);

private:
	bool init();
	void release();

private:
	static QMutex			m_mutex;
	static QWmiFactory*		m_pInstance;

	bool	m_bInitilize;

	IWbemLocator*	m_pLoc;
	IWbemServices*	m_pSvc;

	QMap<int, QAbstractWmi*>	m_mapWmi;
};

#endif // QWMIFACTORY_H
