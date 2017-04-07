#include "qprocesssender.h"

QMutex QProcessSender::m_mutex;
QProcessSender* QProcessSender::m_pInstance=0;

#define PROCESS_SENDER_TIME_OUT		1000

QProcessSender::QProcessSender(QObject *parent)
	: QAbstractSender(PROCESS_SENDER_TIME_OUT, parent)
{

}

QProcessSender::~QProcessSender()
{

}

QProcessSender* QProcessSender::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QProcessSender;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

