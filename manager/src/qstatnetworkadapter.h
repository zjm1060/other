#ifndef QSTATNETWORKADAPTER_H
#define QSTATNETWORKADAPTER_H

#include <QObject>
#include <QVector>

#include "qabstractstat.h"
#include "extern.h"

class QStatNetworkAdapter : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatNetworkAdapter(QObject *parent=0);
	~QStatNetworkAdapter();

	enum {Type=WMI_TYPE_NETWORK_ADAPTER};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecAdapter)

protected:
	virtual void beferQuery();
	virtual void query();

private:
	QVector<NetworkAdapter>	m_vecAdapter;
};

#endif // QSTATNETWORKADAPTER_H
