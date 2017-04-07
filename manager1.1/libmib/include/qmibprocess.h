#ifndef QMIBPROCESS_H
#define QMIBPROCESS_H

#include "qmibextern.h"

class MIB_EXPORT QMibProcess : public QAbstractMib
{
public:
	QMibProcess();
	~QMibProcess();

public:
	enum {Type=UTIL_TYPE_PROCESS};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_vecProcess; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_vecProcess)

private:
	QVector<SystemProcess>	m_vecProcess;

	QMap<quint32, quint64>	m_mapTimeProcess;
	quint64	m_nTotalTimeProcess;
};

#endif // QMIBPROCESS_H
