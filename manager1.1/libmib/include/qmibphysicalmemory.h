#ifndef QMIBPHYSICALMEMORY_H
#define QMIBPHYSICALMEMORY_H

#include "qmibextern.h"

class MIB_EXPORT QMibPhysicalMemorhy : public QAbstractMib
{
public:
	QMibPhysicalMemorhy();
	~QMibPhysicalMemorhy();

public:
	enum {Type=UTIL_TYPE_PHYSICAL_MEMORY};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_memories; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_memories)

private:
	QVector<PhysicalMemory>	m_memories;
};

#endif // QMIBPHYSICALMEMORY_H
