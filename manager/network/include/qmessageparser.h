#ifndef QMESSAGEPARSER_H
#define QMESSAGEPARSER_H

#include "global.h"
#include "qimessage.h"

#include <QMutex>
#include <QMap>

class NETWORK_EXPORT QMessageParser
{
public:
	virtual ~QMessageParser();

protected:
	QMessageParser();

public:
	bool messageRegister(QIMessage* msg);
	QIMessage* messageInstance(int type);

public:
	static QMessageParser* instance();

private:
	QMap<int, QIMessage*> m_mapMessage;

private:
	static QMutex			m_mutex;
	static QMessageParser*	m_pInstance;
};

#endif // QMESSAGEPARSER_H
