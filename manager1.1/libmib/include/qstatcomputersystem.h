#ifndef QSTATCOMPUTERSYSTEM_H
#define QSTATCOMPUTERSYSTEM_H

#include "qmibextern.h"

class MIB_EXPORT QStatComputerSystem : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatComputerSystem(QObject *parent=0);
	~QStatComputerSystem();

	enum {Type=UTIL_TYPE_COMPUTER_SYSTEM};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_system)

protected:
	virtual void beferQuery();
	virtual void query();

private:
	ComputerSystem	m_system;
};

#endif // QSTATCOMPUTERSYSTEM_H
