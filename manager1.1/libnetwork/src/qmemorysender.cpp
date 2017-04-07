#include "qmemorysender.h"

QMutex QMemorySender::g_mutex;
QMemorySender* QMemorySender::g_pInstance=0;

QMemorySender::QMemorySender(QObject *parent)
	: QAbstractSender(1000, parent)
{
}

QMemorySender::~QMemorySender()
{

}

QMemorySender* QMemorySender::instance()
{
	if (g_pInstance == 0)
	{
		g_mutex.lock();
		if (g_pInstance == 0)
		{
			g_pInstance=new QMemorySender;
		}
		g_mutex.unlock();
	}

	return g_pInstance;
}
