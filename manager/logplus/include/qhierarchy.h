#ifndef QHIERARCHY_H
#define QHIERARCHY_H

#include <QVector>
#include <QMap>

#include "qloglog.h"
#include "qlogger.h"
#include "qloggerfactory.h"

class QLoggerFactoryInterface;

class LOGPLUS_EXPORT QHierarchy : protected QLogLogUser
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

	virtual QLogger& loggerInstance(const QString& name);
	virtual QLogger& loggerInstance(const QString& name, QLoggerFactoryInterface& factory);

	virtual QLoggerList currentLoggers();

	virtual bool isDisabled(int level);

	virtual QLogger& root();

	virtual void resetConfiguration(); 

	virtual void setLoggerFactory(std::auto_ptr<QLoggerFactoryInterface> factory);
	virtual QLoggerFactoryInterface* loggerFactory() { return m_defaultFactory.get(); }

	virtual void shutdown();

private:
	typedef QVector<QLogger> QProvisionNode;
	typedef QMap<QString, QProvisionNode> QProvisionNodeMap;
	typedef QMapIterator<QString, QProvisionNode> QProvisionNodeMapIterator;
	typedef QMap<QString, QLogger> QLoggerMap;
	typedef QMapIterator<QString, QLogger>	QLoggerMapIterator;

	
	virtual QLogger& loggerImplementInstance(const QString& name, QLoggerFactoryInterface& factory);

	virtual void initializeLoggerList(QLoggerList& list) const;

	void updateParents(QLogger logger);
	void updateChildren(QProvisionNode& pn, QLogger logger);

public:
	static const LogLevel DISABLE_OFF;
	static const LogLevel DISABLE_OVERRIDE;

private:
	QMutex m_mutex;

	std::auto_ptr<QLoggerFactoryInterface> m_defaultFactory;
	QProvisionNodeMap m_provisionNodes;
	QLoggerMap m_loggerPtrs;
	QLogger m_root;

	int m_nDisableValue;

	bool m_bEmittedNoAppenderWarning;
	bool m_bEmittedNoResourceBundleWarning;

private:
	// Disallow copying of instances of this class
	QHierarchy(const QHierarchy&);
	QHierarchy& operator=(const QHierarchy&);

	friend class QLoggerImplement;
	friend class QHierarchyLocker;
};

#endif // QHIERARCHY_H
