#include "qfiletransferparser.h"

#include "define.h"

QMutex QFileTransferParser::m_mutex;
QFileTransferParser* QFileTransferParser::m_pInstance=0;
quint32 QFileTransferParser::m_nIndex=QFileTransferParser::UserInvaild;

QFileTransferParser::QFileTransferParser()
{

}

QFileTransferParser::~QFileTransferParser()
{
	m_pInstance=NULL;
}

QFileTransferParser* QFileTransferParser::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QFileTransferParser;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

quint32 QFileTransferParser::registerInterface(QAbstractFileTransfer* fileTransferInterface)
{
	Q_ASSERT_NO_NULL(fileTransferInterface);

	do 
	{
		if ((++m_nIndex) == UserInvaild)
		{
			m_nIndex=0;
		}
	} while (m_mapInterface.contains(m_nIndex));

	m_mapInterface.insert(m_nIndex, fileTransferInterface);

	return m_nIndex;
}

void QFileTransferParser::unregisterInterface(quint32 index)
{
	m_mapInterface.remove(index);
}

QAbstractFileTransfer*	QFileTransferParser::instanceInterface(quint32 index)
{
	return m_mapInterface.value(index, NULL);
}
