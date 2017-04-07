#ifndef QDDBWAREHOUSE_H
#define QDDBWAREHOUSE_H

#include <QMutex>
#include <QMap>
#include <QHash>

#include "global.h"
#include "qsharedobject.h"
#include "qutildef.h"

class UTIL_EXPORT QDdbWarehouse : public QObject
{
	Q_OBJECT

public:
	~QDdbWarehouse();

protected:
	QDdbWarehouse(QObject *parent=0);

public:
	static QDdbWarehouse* instance();

public:
	bool updatePrivilegeData(const PrivilegeGroups& data);
	const PrivilegeGroups& privilegeData() const;

public:
	void clear();

	void insert(QObject* conn, quint32 usrID, const QString& uid);
	void remove(QObject* conn);

	bool contains(QObject* conn) const;

	QPair<quint32, QString>	connection(QObject* conn);
	QMap<QObject*, QPair<quint32, QString> > connections();

private:
	bool updatePrivilegeGroups(const QMap<QString, PrivilegeGroup>& groups);
	bool updatePrivilegeOperations(QMap<QString, PrivilegeGroup>& groups, const QList<PrivilegeOperation>& operations);

	void mergePrivilegeOperations(QList<PrivilegeOperation>& operations, const QList<PrivilegeOperation>& src);

private:
	QMutex m_mutex;

	QMap<QObject*, QPair<quint32, QString> >	m_mapConnection;	//所有客户端连接

	PrivilegeGroups		m_groups;		//权限信息
};


#endif // QDDBWAREHOUSE_H
