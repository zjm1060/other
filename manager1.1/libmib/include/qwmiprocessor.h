#ifndef QWMIPROCESSOR_H
#define QWMIPROCESSOR_H

#include <QObject>
#include <QVector>

#include "qmibextern.h"

class MIB_EXPORT QWmiProcessor : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiProcessor(QObject *parent=0);
	~QWmiProcessor();

	enum {Type=UTIL_TYPE_PROCESSOR};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecProcessor)

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	QVector<ProcessorInfo>	m_vecProcessor;
};

#endif // QWMIPROCESSOR_H
