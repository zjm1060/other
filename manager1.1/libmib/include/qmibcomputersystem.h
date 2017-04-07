#ifndef QMIBCOMPUTERSYSTEM_H
#define QMIBCOMPUTERSYSTEM_H

#include "qmibextern.h"

class MIB_EXPORT QMibComputerSystem : public QAbstractMib
{
public:
	QMibComputerSystem();
	~QMibComputerSystem();

public:
	enum {Type=UTIL_TYPE_COMPUTER_SYSTEM};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_system; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_system)

private:
	ComputerSystem	m_system;
};

#endif // QMIBCOMPUTERSYSTEM_H
