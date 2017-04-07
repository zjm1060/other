#include "qprocessorsender.h"

QMutex QProcessorSender::m_mutex;
QProcessorSender* QProcessorSender::m_pInstance=0;

QProcessorSender::QProcessorSender(QObject *parent)
	: QAbstractSender(1000, parent)
{

}

QProcessorSender::~QProcessorSender()
{

}

QProcessorSender* QProcessorSender::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QProcessorSender;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}
