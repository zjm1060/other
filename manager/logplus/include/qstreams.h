#ifndef QSTREAMS_H
#define QSTREAMS_H

#include "qsharedobject.h"

#include <QMap>
#include <QTextStream>

class LOGPLUS_EXPORT QStreams : public QSharedObject
{
private:
	QStreams();

public:
	virtual ~QStreams();

public:
	static QSharedObjectPtr<QStreams> instance();

public:
	QTextStream& stream(const QString& name);

	void addStream(const QString& name, QIODevice* device);

private:
	static QMutex m_gMutex;
	static QSharedObjectPtr<QStreams> m_pInstance;

private:
	QMap<QString, QTextStream*>	m_streams;
};

#endif // QSTREAMS_H
