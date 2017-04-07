#ifndef QHIERARCHY_H
#define QHIERARCHY_H

#include <QVector>
#include <QMap>

#include "qloglog.h"
#include "qlogger.h"
#include "qloggerfactory.h"

class QLoggerFactoryInterface;

class LOGPLUS_EXPORT QHierarchy :public QSharedObject, protected QLogLogUser
{
public:
	QHierarchy();
	~QHierarchy();

public:
	virtual void clear();

	virtual bool exists(const QString& name);


	virtual void disable(const QString& logLevel);
	virtual void disable(LogLevel logLevel);
	virtual void disableAll();
	virtual void disableDebug();
	virtual void disableInfo();

	virtual void enableAll();

	virtual QLoggerPtr loggerInstance(const QString& name);
	virtual QLoggerPtr loggerInstance(const QString& name, const QLoggerFactoryInterfacePtr& factory);

	virtual QLoggerPtrList currentLoggers();

	virtual bool isDisabled(int level);

	virtual QLoggerPtr& root();

	virtual void resetConfiguration(); 

	virtual void setLoggerFactory(const QLoggerFactoryInterfacePtr& factory);
	virtual const QLoggerFactoryInterfacePtr& loggerFactory() { return m_pDefaultFactory; }

	virtual void shutdown();

private:
	typedef QVector<QLoggerPtr> QProvisionNode;
	typedef QMap<QString, QProvisionNode > QProvisionNodeMap;
	typedef QMapIterator<QString, QProvisionNode > QProvisionNodeMapIterator;
	typedef QMap<QString, QLoggerPtr > QLoggerPtrMap;
	typedef QMapIterator<QString, QLoggerPtr >	QLoggerPtrMapIterator;

	
	virtual QLoggerPtr& loggerImplementInstance(const QString& name, const QLoggerFactoryInterfacePtr& factory);

	virtual void initializeLoggerList(QLoggerPtrList& list) const;

	void updateParents(const QLoggerPtr& logger);
	void updateChildren(QProvisionNode& pn, const QLoggerPtr& logger);

public:
	static const LogLevel DISABLE_OFF;
	static const LogLevel DISABLE_OVERRIDE;

private:
	QMutex m_mutex;

	QLoggerFactoryInterfacePtr m_pDefaultFactory;
	QProvisionNodeMap m_provisionNodes;
	QLoggerPtrMap m_loggerPtrs;
	QLoggerPtr m_pRoot;

	int m_nDisableValue;

	bool m_bEmittedNoAppenderWarning;
	bool m_bEmittedNoResourceBundleWarning;

private:
	// Disallow copying of instances of this class
	QHierarchy(const QHierarchy&);
	QHierarchy& operator=(const QHierarchy&);

	friend class QLoggerImplement;
};

#endif // QHIERARCHY_H
