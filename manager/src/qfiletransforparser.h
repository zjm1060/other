#ifndef QFILETRANSFORPARSER_H
#define QFILETRANSFORPARSER_H

#include <QObject>
#include <QMutex>
#include <QMap>

#include "qabstractfiletransfor.h"

class QFileTransforParser : public QObject
{
	Q_OBJECT

public:
	~QFileTransforParser();

protected:
	QFileTransforParser();

public:
	quint32 registerInterface(QAbstractFileTransfor* fileTransforInterface);
	void unregisterInterface(quint32 index);
	QAbstractFileTransfor*	instanceInterface(quint32 index);

public:
	static QFileTransforParser* instance();

private:
	QMap<quint32, QAbstractFileTransfor*> m_mapInterface;

private:
	static QMutex			m_mutex;
	static QFileTransforParser*	m_pInstance;

	static quint32	m_nIndex;	//下一个请求用户ID
};

#endif // QFILETRANSFORPARSER_H
