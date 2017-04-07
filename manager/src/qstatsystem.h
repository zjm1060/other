#ifndef QSTATSYSTEM_H
#define QSTATSYSTEM_H

#include <QObject>
#include <QVector>

#include "qabstractstat.h"
#include "extern.h"

class QStatSystem : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatSystem(QObject *parent=0);
	~QStatSystem();

	enum {Type=WMI_TYPE_SYSTEM};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecSystem)

protected:
	virtual void beferQuery();
	virtual void query();

private:
	QVector<OperatingSystem>	m_vecSystem;
};

#endif // QSTATSYSTEM_H
