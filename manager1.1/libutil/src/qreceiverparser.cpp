#include "qreceiverparser.h"

#include "define.h"

QMutex QReceiverParser::m_mutex;
QReceiverParser* QReceiverParser::m_pInstance=0;

QReceiverParser::QReceiverParser()
:m_nIndex(AllUser)
{

}

QReceiverParser::~QReceiverParser()
{
	m_pInstance=NULL;
}

QReceiverParser* QReceiverParser::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QReceiverParser;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

quint32 QReceiverParser::registerInterface(QAbstractReceiver* receiver)
{
	Q_ASSERT_NO_NULL(receiver);

	do 
	{
		if ((++m_nIndex) == AllUser)
		{
			m_nIndex=0;
		}
	} while (m_mapInterface.contains(m_nIndex));

	m_mapInterface.insert(m_nIndex, receiver);

	return m_nIndex;
}

void QReceiverParser::unregisterInterface(quint32 index)
{
	m_mapInterface.remove(index);
}

QAbstractReceiver*	QReceiverParser::instanceInterface(quint32 index)
{
	return m_mapInterface.value(index, NULL);
}

QList<quint32> QReceiverParser::instanceInterfaces()
{
	return m_mapInterface.keys();
}
