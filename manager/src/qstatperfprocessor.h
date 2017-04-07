#ifndef QSTATPERFPROCESSOR_H
#define QSTATPERFPROCESSOR_H

#include <QObject>
#include <QMap>

#include "qabstractstat.h"
#include "extern.h"

class QStatPerfProcessor : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatPerfProcessor(QObject *parent=0);
	~QStatPerfProcessor();

	enum {Type=WMI_TYPE_PERF_PROCESSOR};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_processor)

protected:
	virtual void beferQuery();
	virtual void query();

	virtual void init();

private:
	PerfProcessor	m_processor;

private:
	typedef struct _cpuUsage 
	{
		quint32 user;
		quint32 wait;
		quint32 kernel;
		quint32 idle;
	}CpuUsage;

	QMap<int, CpuUsage>	m_mapCpuUsage;
};

#endif // QSTATPERFPROCESSOR_H
