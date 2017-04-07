#ifndef QLISTVIEWSYNCFILESERVER_H
#define QLISTVIEWSYNCFILESERVER_H

#include <QStandardItemModel>
#include <QMouseEvent>
#include <QSet>

#include "qlistviewhorizontalicon.h"
#include "qutildef.h"
#include "qreadwritexmlfile.h"

class QListViewSyncFileServer : public QListViewHorizontalIcon
{
	Q_OBJECT

public:
	QListViewSyncFileServer(const QString& record, QWidget *parent=0);
	~QListViewSyncFileServer();

public:
	enum ItemRoleType
	{
		OriginItem,
		AddedItem,
		AddOp
	};

	enum UserData
	{
		ItemRole=Qt::UserRole,
		UserDataRole,
		StatusRole,
		SelectedRole
	};

public:
	void refreshServerList(const QList<QPair<ServerInfo, int> >& servers);

	void saveServers();

public:
	bool hasSelected() const;
	int nextSelected(int row, ServerInfo& info) const;

	bool isAddedItem(int row) const;

	void setServerItemsEnabled(bool enabled);

	bool hasMantualServer() const;

	QModelIndex findItem(const ServerInfo& info);

signals:
	void sendTips(int flag, const QString& message);

	void selectedChanged();

protected:
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);

private slots:
	void onClickListViewItem(const QModelIndex& index);
	void changed(const QList<Server>& servers);

	void entered(const QModelIndex& index);

private:
	void addServerItemToServerView();
	bool findServer(const Server& server);
	void addMantualServerItemToServerView(int row, const Server& server);
	void setServerItemSelectStatus(const QModelIndex& index, bool select, int tick=0);

	void setServerItemsRemoveable(bool removable);
	void setServerItemRemovable(const QModelIndex& index, bool removable);

	bool isSelectedItem(const ServerInfo& info);

private:
	QStandardItemModel*			m_pModel;

	QMonitorServersFile*	m_pMonitor;
	QList<Server>			m_servers;		//添加的接收方服务器列表

	QString				m_strRecord;	//记录选中主机的标识
	QSet<QString>		m_selections;	//上次被选中的

	bool	m_bInit;	//初始化

private:
	QPixmap		m_pixHost;
	QPixmap		m_pixHostQuit;
	QPixmap		m_pixCheck;
	QPixmap		m_pixRemove;

	bool		m_bRemovable;
	bool		m_bEnabled;
};

class QReadSelectionsFromXmlFile : public QAbstractReadXmlFile
{
public:
	QReadSelectionsFromXmlFile(const QString& filename, const QString& type);
	~QReadSelectionsFromXmlFile();

public:
	QSet<QString>	result()
	{
		return m_results;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readCommitNode(QXmlStreamReader& reader);
	void readTypeNode(QXmlStreamReader& reader);
	void readSelectionsNode(QXmlStreamReader& reader);
	void readSelectionNode(QXmlStreamReader& reader);

private:
	QString		m_strType;

	QSet<QString>	m_results;
};

class QWriteSelectionsToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWriteSelectionsToXmlFile(const QString& filename, const QString& type, const QVector<QString>& selections);
	~QWriteSelectionsToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "commit";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

private:
	QDomElement findChildElement(QDomDocument& doc, QDomElement& parent, const QString& tag);

private:
	QString				m_strType;
	QVector<QString>	m_selections;
};

#endif // QLISTVIEWSYNCFILESERVER_H
