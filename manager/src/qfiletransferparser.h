#ifndef QFILETRANSFERPARSER_H
#define QFILETRANSFERPARSER_H

#include <QObject>
#include <QMutex>
#include <QMap>

#include "qabstractfiletransfer.h"

class QFileTransferParser : public QObject
{
	Q_OBJECT

public:
	~QFileTransferParser();

	enum UserFlag
	{
		UserInvaild=-1
	};

protected:
	QFileTransferParser();

public:
	quint32 registerInterface(QAbstractFileTransfer* fileTransferInterface);
	void unregisterInterface(quint32 index);
	QAbstractFileTransfer*	instanceInterface(quint32 index);

public:
	static QFileTransferParser* instance();

private:
	QMap<quint32, QAbstractFileTransfer*> m_mapInterface;

private:
	static QMutex			m_mutex;
	static QFileTransferParser*	m_pInstance;

	static quint32	m_nIndex;	//下一个请求用户ID
};

#endif // QFILETRANSFERPARSER_H
