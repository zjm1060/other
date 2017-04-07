#ifndef QRECEIVERPARSER_H
#define QRECEIVERPARSER_H

#include <QObject>
#include <QMutex>
#include <QMap>

#include "qabstractreceiver.h"

class UTIL_EXPORT QReceiverParser : public QObject
{
	Q_OBJECT

public:
	~QReceiverParser();

	enum UserFlag
	{
		AllUser=(quint32)-1
	};

protected:
	QReceiverParser();

public:
	quint32 registerInterface(QAbstractReceiver* receiver);
	void unregisterInterface(quint32 index);

	QAbstractReceiver*	instanceInterface(quint32 index);
	QList<quint32> instanceInterfaces();

public:
	static QReceiverParser* instance();

private:
	QMap<quint32, QAbstractReceiver*> m_mapInterface;

private:
	static QMutex			m_mutex;
	static QReceiverParser*	m_pInstance;

	quint32	m_nIndex;	//下一个请求用户ID
};

#endif // QRECEIVERPARSER_H
