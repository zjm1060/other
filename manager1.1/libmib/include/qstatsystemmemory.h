#ifndef QSTATSYSTEMMEMORY_H
#define QSTATSYSTEMMEMORY_H

#include "qmibextern.h"

class MIB_EXPORT QStatSystemMemory : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatSystemMemory(QObject *parent=0);
	~QStatSystemMemory();

	enum {Type=UTIL_TYPE_SYSTEM_MEMORY};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_memory)

protected:
	virtual void query();

private:
	SystemMemory	m_memory;

	quint64		m_nUsagePeak;
};

#endif // QSTATSYSTEMMEMORY_H
