#ifndef QMIBFACTORY_H
#define QMIBFACTORY_H

#include <QMutex>
#include <QMap>
#include <QSignalMapper>
#include <QMutex>

#include "qabstractmib.h"
#include "qutildef.h"

class UTIL_EXPORT QMibFactory : public QObject
{
	Q_OBJECT

public:
	virtual ~QMibFactory();

protected:
	QMibFactory();

public:
	static QMibFactory* instance();

public:
	bool loadLibrary(const QString& filename, const char* symbol);

public:
	QAbstractMibPtr product(int type);
	const QMap<int, QAbstractMibPtr>& products() const;
	
	void registerProduct(const QAbstractMibPtr& ptr);

	quint64 signs();
	void clear();

public slots:
	void changed(int type);

private:
	QMap<int, QAbstractMibPtr>	m_mapProducts;

	QMutex	m_mutex;
	bool	m_bChanged[UTIL_TYPE_END];

	QSignalMapper*	m_pSignalMapper;
};

#endif // QMIBFACTORY_H
