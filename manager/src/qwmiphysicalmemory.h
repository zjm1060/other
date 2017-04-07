#ifndef QWMIPHYSICALMEMORY_H
#define QWMIPHYSICALMEMORY_H

#include <QObject>
#include <QVector>

#include "qabstractwmi.h"
#include "extern.h"
#include "define.h"

class QWmiPhysicalMemory : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiPhysicalMemory(QObject *parent=0);
	~QWmiPhysicalMemory();

	enum {Type=WMI_TYPE_PHYSICAL_MEMORY};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecMemory);

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	QVector<PhysicalMemory>	m_vecMemory;
};

#endif // QWMIPHYSICALMEMORY_H
