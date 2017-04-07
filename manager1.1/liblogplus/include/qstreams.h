#ifndef QSTREAMS_H
#define QSTREAMS_H

#include "qsharedobject.h"

#include <QMap>
#include <QTextStream>

#define STD_ERR_STREAM	"stderr"
#define STD_OUT_STREAM	"stdout"

class LOGPLUS_EXPORT QExclusiveTextStream : public QTextStream
{
public:
	QExclusiveTextStream(){};
	QExclusiveTextStream(QIODevice* device);
	QExclusiveTextStream(FILE* fileHandle, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
	virtual ~QExclusiveTextStream();

public:
	void lock();
	void unlock();

private:
	static QMutex m_mutex;
};

class QStreams;
typedef QSharedObjectPtr<QStreams> QStreamsPtr;

class LOGPLUS_EXPORT QStreams : public QSharedObject
{
private:
	QStreams();

public:
	virtual ~QStreams();

public:
	static QStreamsPtr instance();

public:
	bool contains(const	QString& name);

	bool addStream(const QString& name, QIODevice* device);
	QExclusiveTextStream& stream(const QString& name);

	void removeStream(const QString& name);

private:
	static QMutex m_mutex;
	static QStreamsPtr m_pInstance;

private:
	QMap<QString, QExclusiveTextStream*>	m_streams;
};

#endif // QSTREAMS_H
