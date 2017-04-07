#include "qstreams.h"

#include <QStringList>

QMutex QExclusiveTextStream::m_mutex;
QExclusiveTextStream::QExclusiveTextStream(QIODevice* device)
:QTextStream(device)
{

}

QExclusiveTextStream::QExclusiveTextStream(FILE* fileHandle, QIODevice::OpenMode openMode)
:QTextStream(fileHandle, openMode)
{

}

QExclusiveTextStream::~QExclusiveTextStream()
{

}

void QExclusiveTextStream::lock()
{
	m_mutex.lock();
}

void QExclusiveTextStream::unlock()
{
	m_mutex.unlock();
}

QMutex QStreams::m_mutex;
QStreamsPtr QStreams::m_pInstance;

QStreams::QStreams()
{
	m_streams.insert(STD_ERR_STREAM, new QExclusiveTextStream(stderr, QIODevice::WriteOnly));
	m_streams.insert(STD_OUT_STREAM, new QExclusiveTextStream(stdout, QIODevice::WriteOnly));
}

QStreams::~QStreams()
{
	QStringList keys=m_streams.keys();
	foreach (QString key, keys)
	{
		removeStream(key);
	}
}

QStreamsPtr QStreams::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QStreams;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

bool QStreams::contains(const	QString& name)
{
	QMutexLocker locker(&m_mutex);

	return m_streams.contains(name);
}

QExclusiveTextStream& QStreams::stream(const QString& name)
{	
	if (!m_streams.contains(name))
	{
		return stream(STD_ERR_STREAM);
	}

	QMutexLocker locker(&m_mutex);

	return *m_streams[name];
}

bool QStreams::addStream(const QString& name, QIODevice* device)
{
	if (name.isEmpty() || device == NULL)
	{
		return false;
	}

	if (!device->isOpen())
	{
		if (!device->open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append))
		{
			return false;
		}
	}

	removeStream(name);

	QMutexLocker locker(&m_mutex);

	m_streams.insert(name, new QExclusiveTextStream(device));
	return true;
}

void QStreams::removeStream(const QString& name)
{
	if (!contains(name))
	{
		return;
	}

	QMutexLocker locker(&m_mutex);

	QExclusiveTextStream* stream=m_streams[name];
	Q_ASSERT_NO_NULL(stream);

	QIODevice* device=stream->device();
	if (device->isOpen())
	{
		device->close();
	}

	m_streams.remove(name);
	SAFE_DELETE_CLASS(stream);

	if (name != STD_ERR_STREAM && name != STD_OUT_STREAM)
	{
		SAFE_DELETE_CLASS(device);
	}
}
