#ifndef QMIBSYSTEMMEMORY_H
#define QMIBSYSTEMMEMORY_H

#include "qmibextern.h"

class MIB_EXPORT QMibSystemMemory : public QAbstractMib
{
public:
	QMibSystemMemory();
	~QMibSystemMemory();

public:
	enum {Type=UTIL_TYPE_SYSTEM_MEMORY};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_memory; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_memory)

private:
	SystemMemory	m_memory;
};

#endif // QMIBSYSTEMMEMORY_H
