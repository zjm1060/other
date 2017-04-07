#ifndef QMIBSERVER_H
#define QMIBSERVER_H

#include "qmibextern.h"

class MIB_EXPORT QMibNetworkAdapter : public QAbstractMib
{
public:
	QMibNetworkAdapter();
	~QMibNetworkAdapter();

public:
	enum {Type=UTIL_TYPE_NETWORK_ADAPTER};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_vecAdapter; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_vecAdapter)

private:
	QVector<NetworkAdapter>	m_vecAdapter;
};

#endif // QMIBSERVER_H
