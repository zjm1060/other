#ifndef QMIBSYSTEM_H
#define QMIBSYSTEM_H

#include "qmibextern.h"

class MIB_EXPORT QMibSystem : public QAbstractMib
{
public:
	QMibSystem();
	~QMibSystem();

public:
	enum {Type=UTIL_TYPE_SYSTEM};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_system; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_system)

private:
	OperatingSystem	m_system;
};

#endif // QMIBSYSTEM_H
