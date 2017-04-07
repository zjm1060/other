#ifndef QSTATCOMPUTERSYSTEM_H
#define QSTATCOMPUTERSYSTEM_H

#include <QObject>
#include <QVector>

#include "qabstractstat.h"
#include "extern.h"

class QStatComputerSystem : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatComputerSystem(QObject *parent=0);
	~QStatComputerSystem();

	enum {Type=WMI_TYPE_COMPUTER_SYSTEM};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecSystem)

protected:
	virtual void beferQuery();
	virtual void query();

private:
	QVector<ComputerSystem>	m_vecSystem;
};

#endif // QSTATCOMPUTERSYSTEM_H
