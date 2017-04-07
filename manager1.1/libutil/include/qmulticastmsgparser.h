#ifndef QMULTICASTMSGPARSER_H
#define QMULTICASTMSGPARSER_H

#include "global.h"

#include <QMutex>
#include <QMap>
#include <QFileInfo>

#include "qimulticastmsg.h"
#include "qutildef.h"

class UTIL_EXPORT QMulticastMsgParser
{
public:
	virtual ~QMulticastMsgParser();

protected:
	QMulticastMsgParser();

public:
	bool registerMessage(const QIMulticastMsgPtr& msg);
	QIMulticastMsgPtr messageInstance(int type);

public:
	bool loadLibrary(const QString& filename, const char* symbol);

public:
	static QMulticastMsgParser* instance();

private:
	QMap<int, QIMulticastMsgPtr > m_mapMessage;
};

#endif // QMULTICASTMSGPARSER_H
