#include "qfiletransforparser.h"

#include "define.h"

QMutex QFileTransforParser::m_mutex;
QFileTransforParser* QFileTransforParser::m_pInstance=0;
quint32 QFileTransforParser::m_nIndex=-1;

QFileTransforParser::QFileTransforParser()
{

}

QFileTransforParser::~QFileTransforParser()
{
	m_pInstance=NULL;
}

QFileTransforParser* QFileTransforParser::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QFileTransforParser;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

quint32 QFileTransforParser::registerInterface(QAbstractFileTransfor* fileTransforInterface)
{
	Q_ASSERT_NO_NULL(fileTransforInterface);

	do 
	{
		if ((++m_nIndex) == -1)
		{
			m_nIndex=0;
		}
	} while (m_mapInterface.contains(m_nIndex));

	m_mapInterface.insert(m_nIndex, fileTransforInterface);

	return m_nIndex;
}

void QFileTransforParser::unregisterInterface(quint32 index)
{
	m_mapInterface.remove(index);
}

QAbstractFileTransfor*	QFileTransforParser::instanceInterface(quint32 index)
{
	return m_mapInterface.value(index, NULL);
}
