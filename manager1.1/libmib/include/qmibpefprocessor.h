#ifndef QMIBPERFPROCESSOR_H
#define QMIBPERFPROCESSOR_H

#include "qmibextern.h"

class MIB_EXPORT QMibPefProcessor : public QAbstractMib
{
public:
	QMibPefProcessor();
	~QMibPefProcessor();

public:
	enum {Type=UTIL_TYPE_PERF_PROCESSOR};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_perfProcessor; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_perfProcessor)

private:
	PerfProcessor	m_perfProcessor;
};

#endif // QMIBPERFPROCESSOR_H
