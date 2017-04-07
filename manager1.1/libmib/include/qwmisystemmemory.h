#ifndef QWMISYSTEMMEMORY_H
#define QWMISYSTEMMEMORY_H

#include <QObject>
#include <QVector>

#include "qmibextern.h"

class MIB_EXPORT QWmiSystemMemory : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiSystemMemory(QObject *parent=0);
	~QWmiSystemMemory();

	enum {Type=UTIL_TYPE_SYSTEM_MEMORY};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_memory)

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	SystemMemory	m_memory;
};

#endif // QWMISYSTEMMEMORY_H
