#ifndef QPLUGINMANAGER_H
#define QPLUGINMANAGER_H

#include <QObject>
#include <QSet>

class QPluginManager : public QObject
{
	Q_OBJECT

public:
	~QPluginManager();

protected:
	QPluginManager(QObject *parent=0);

public:
	static QPluginManager* instance();

public:
	void activate(const QString& module);

private:
	QSet<qint64>	m_pids;
};

#endif // QPLUGINMANAGER_H
