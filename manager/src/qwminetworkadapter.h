#ifndef QWMINETWORKADAPTER_H
#define QWMINETWORKADAPTER_H

#include <QObject>
#include <QVector>

#include "qabstractwmi.h"
#include "extern.h"
#include "define.h"

class QWmiNetworkAdapter : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiNetworkAdapter(QObject *parent=0);
	~QWmiNetworkAdapter();

	enum {Type=WMI_TYPE_NETWORK_ADAPTER};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecAdapter)

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	QVector<NetworkAdapter>	m_vecAdapter;
};

#endif // QWMINETWORKADAPTER_H
