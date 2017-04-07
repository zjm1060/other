#ifndef QWMIPERFPROCESSOR_H
#define QWMIPERFPROCESSOR_H

#include <QObject>
#include <QVector>

#include "qabstractwmi.h"
#include "extern.h"
#include "define.h"

class QWmiPerfProcessor : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiPerfProcessor(QObject *parent=0);
	~QWmiPerfProcessor();

	enum {Type=WMI_TYPE_PERF_PROCESSOR};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_processor)

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	PerfProcessor	m_processor;
};

#endif // QWMIPERFPROCESSOR_H
