#ifndef QMIBPROCESSOR_H
#define QMIBPROCESSOR_H

#include "qmibextern.h"

class MIB_EXPORT QMibProcessor : public QAbstractMib
{
public:
	QMibProcessor();
	~QMibProcessor();

public:
	enum {Type=UTIL_TYPE_PROCESSOR};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_processors; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_processors)

private:
	QVector<ProcessorInfo>	m_processors;
};

#endif // QMIBPROCESSOR_H
