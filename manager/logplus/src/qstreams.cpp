#include "qstreams.h"

QMutex QStreams::m_gMutex;
QSharedObjectPtr<QStreams> QStreams::m_pInstance;

QStreams::QStreams()
{
	m_streams.insert("stderr", new QTextStream(stderr, QIODevice::WriteOnly));
	m_streams.insert("stdout", new QTextStream(stdout, QIODevice::WriteOnly));
}

QStreams::~QStreams()
{
	foreach (QTextStream* stream, m_streams)
	{
		delete stream;
	}
}

QSharedObjectPtr<QStreams> QStreams::instance()
{
	if (m_pInstance.get() == 0)
	{
		m_gMutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=QSharedObjectPtr<QStreams>(new QStreams);
		}
		m_gMutex.unlock();
	}

	return m_pInstance;
}

QTextStream& QStreams::stream(const QString& name)
{
	Q_ASSERT(m_streams.contains(name));

	return *m_streams[name];
}

void QStreams::addStream(const QString& name, QIODevice* device)
{
	Q_ASSERT(name.isEmpty());
	Q_ASSERT_NO_NULL(device);
	Q_ASSERT(device->isWritable());

	if (!device->isOpen())
	{
		device->open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append);
	}

	if (m_streams.contains(name))
	{
		delete m_streams[name];
	}

	m_streams.insert(name, new QTextStream(device));
}
