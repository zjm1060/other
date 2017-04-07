#ifndef QWMICOMPUTERSYSTEM_H
#define QWMICOMPUTERSYSTEM_H

#include <QObject>
#include <QVector>

#include "qmibextern.h"

class MIB_EXPORT QWmiComputerSystem : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiComputerSystem(QObject *parent=0);
	~QWmiComputerSystem();

	enum {Type=UTIL_TYPE_COMPUTER_SYSTEM};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_system)

protected:
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	ComputerSystem	m_system;
};

#endif // QWMICOMPUTERSYSTEM_H
