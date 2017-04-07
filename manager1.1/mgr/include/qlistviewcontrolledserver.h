#ifndef QLISTVIEWCONTROLLEDSERVER_H
#define QLISTVIEWCONTROLLEDSERVER_H

#include <QStandardItemModel>
#include <QMouseEvent>
#include <QPair>

#include "qlistviewhorizontalicon.h"
#include "qutildef.h"
#include "qreadwritexmlfile.h"
#include "qfilecomparefactory.h"

class QListViewControlledServer : public QListViewHorizontalIcon
{
	Q_OBJECT

public:
	QListViewControlledServer(QWidget *parent=0);
	~QListViewControlledServer();

public:
	enum UserDataRole
	{
		ItemUserDataRole=Qt::UserRole,		//主机信息
		ItemUserDataStatusRole,				//主机是否掉线
		ItemUserDataSelectRole				//主机是否被选中
	};

public:
	void refreshServerList(const QList<QPair<ServerInfo, int> >& servers);

	ServerInfo serverInfoFromIndex(const QModelIndex& index) const;

	bool selectedServer(ServerInfo& info) const;

public:
	void setServerItemSelectStatus(const QModelIndex& index, bool select);
	int serverStatus(const QModelIndex& index) const;

signals:
	void sendTips(int flag, const QString& message);
	void currentChanged(const QModelIndex& index);

protected:
	virtual void mouseMoveEvent(QMouseEvent* event);

protected:
	virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous);

private slots:
	void entered(const QModelIndex& index);

private:
	void updateIcon(const QModelIndex& index);

private:
	QStandardItemModel*			m_pModel;

	QModelIndex		m_indexSelected;

private:
	enum IconType
	{
		HostIcon,
		HostQuitIcon,
		CheckIcon,
		IconTypeEnd
	};

	QPixmap		m_pixmap[IconTypeEnd];
};

#endif // QLISTVIEWCONTROLLEDSERVER_H
