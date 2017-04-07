#ifndef QMESSAGEPARSER_H
#define QMESSAGEPARSER_H

#include "global.h"

#include <QMutex>
#include <QMap>
#include <QFileInfo>

#include "qimessage.h"
#include "qutildef.h"

class UTIL_EXPORT QMessageParser
{
public:
	virtual ~QMessageParser();

protected:
	QMessageParser();

public:
	bool registerMessage(const QIMessagePtr& msg);
	QIMessagePtr messageInstance(int type);

public:
	bool loadLibrary(const QString& filename, const char* symbol);

public:
	static QMessageParser* instance();

private:
	QMap<int, QIMessagePtr > m_mapMessage;
};

#endif // QMESSAGEPARSER_H
