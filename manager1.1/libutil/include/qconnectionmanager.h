#ifndef QCONNECTIONMANAGER_H
#define QCONNECTIONMANAGER_H

#include <QMutex>
#include <QMap>
#include <QHash>

#include "global.h"
#include "qsharedobject.h"

class UTIL_EXPORT QConnectionManager : public QObject
{
	Q_OBJECT

public:
	~QConnectionManager();

protected:
	QConnectionManager(QObject *parent=0);

public:
	typedef struct _connectionParam
	{
		QObjectPtr	connection;			//连接
		bool		highRate;			//是否高速传输
	}ConnectionParam;

public:
	static QConnectionManager* instance();

public:
	bool isEmpty(QObject* server);

	void insert(QObject* server, QObject* key, QSharedObject* connection, bool highRate);
	void remove(QObject* server, QObject* connection);

	bool contains(QObject* server, QObject* connection);

	QList<ConnectionParam>	connections(QObject* server);
	QObjectPtr connection(QObject* key);
	bool canHighRate(QObject* key);

	void insertLoginUser(QObject* sender, const QString& loginName);
	void removeLoginUser(QObject* sender);

	QMap<QObject*, QString > loginUsers();

private:
	QMutex m_mutex;

	QMap<QObject*, QHash<QObject*, ConnectionParam> >	m_mapConnections;	//所有客户端连接,key为服务器

	QMap<QObject*, QString>		m_loginUsers;		//所有登录的用户
};


#endif // QCONNECTIONMANAGER_H
