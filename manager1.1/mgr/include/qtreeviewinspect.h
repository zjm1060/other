#ifndef QTREEVIEWINSPECT_H
#define QTREEVIEWINSPECT_H

#include <QTreeView>
#include <QRunnable>

#include "qutildef.h"
#include "qtreemodel.h"
#include "qtreeviewinspectsetting.h"
#include "qinspectdelegate.h"
#include "qclientmanager.h"
#include "qreceiverinspect.h"

class QTreeViewInspect : public QTreeView
{
	Q_OBJECT

public:
	QTreeViewInspect(QWidget *parent=0);
	~QTreeViewInspect();

public:
	enum TitleLabel
	{
		NameTitle,
		StatusTitle,
		TitleLabelEnd
	};

	enum ItemRoleType
	{
		PeerItem,
		GroupItem,
		ChildItem
	};

	enum ItemStatusType
	{
		Initialized,
		Connecting,
		Unconnected,
		Connected,
		Reconnecting,
		Collecting,		//正在收集...
		Verifying,		//正在验证...
		Skip,			//跳过
		Completed,		//收集参考源数据完成
		Failed,			//收集数据源失败
		Match,
		Mismatch
	};

	enum ItemCauseType
	{
		H9000NotRunning,		//H9000未运行
		DataSourceNoExisted,	//数据源不存在
		NoReferenceSource,		//无参考源
		ServiceQuit				//服务器退出
	};

	enum UserDataRole
	{
		ItemRole=Qt::UserRole,
		PeerInfoRole,
		ComparedRole,
		MsgParamRole,	//用于网络通信查询
		DataRole,		//用户数据
		StatusRole,		//巡检状态
		CauseRole		//故障原因
	};

public:
	void setInspectItems(const ServerInfo& info, const QTreeViewInspectSetting::InspectItem& items);

	void inspect(bool init=false);

	void pause(bool state);
	void cancel();

signals:
	void sendMessage(QString tip);
	void finished(int normal, int skip, int total);
	void progress(int value, int min, int max);
	void addItemsFinished();

protected:
	virtual void resizeEvent(QResizeEvent* event);

private slots:
	void addInspectItemsFinished(QTreeModel* model, int count);

	void connected(QClient* client);
	void unconnected(QClient* client);
	void disconnected(QClient* client);

	void cscsDataSourceLost();
	void cscsDataSourceNoExisted();
	void recvCscsStationEigenvalue(int stn, QByteArray buffer);
	void recvCscsGroupEigenvalue(int stn, int group, QByteArray buffer);
	void recvCscsRecordEigenvalue(int stn, int group, int datatype, QByteArray buffer);
	void recvCscsEigenvalue(QByteArray buffer);

private:
	void nextTask();
	void setData(const QModelIndex& index, const QVariant& value, int role);

	void restore(const QModelIndex& pindex=QModelIndex());
	int childrenCount(const QModelIndex& index);

private:
	QTreeModel*			m_pModel;
	QInspectDelegate*	m_pDelegate;

	QClientManager*		m_pClientManager;
	QReceiverInspect*	m_pReceiver;

	QModelIndex			m_index;
	QClient*			m_pClient;
	bool				m_bInpecting;
	bool				m_bReconnecting;

	int		m_nMsg;		//巡检组消息类型ID
	int		m_nAck;		//巡检项消息应答ID

	bool	m_bReference;	//参考源
	QMap<QByteArray, QByteArray>	m_data;	//巡检参考源数据

	int		m_nNumItems;	//所有巡检的项数
	int		m_nCurItem;		//当前巡检的项
	int		m_nNumNormal;	//巡检正常项数
	int		m_nNumSkip;		//跳过项数

	bool	m_bPause;
};

class QTreeViewInspectRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QTreeViewInspectRunnable(const ServerInfo& info, const QTreeViewInspectSetting::InspectItem& items, QObject* parent=0);
	virtual ~QTreeViewInspectRunnable();

signals:
	void finished(QTreeModel* model, int count);

public:
	virtual void run();

private:
	void addInspectGroup(const ServerInfo& info, const QTreeViewInspectSetting::InspectItem& items, bool compared=false);
	void addInspectItem(const QTreeViewInspectSetting::InspectItem& item, const QModelIndex& parent, bool isGroup, const QByteArray& buffer="");

private:
	ServerInfo	m_info;		//参考数据源主机信息
	QTreeViewInspectSetting::InspectItem	m_items;	//巡检项

	QTreeModel*	m_pModel;
	int			m_nNumItems;	//巡检项总数
};

#endif // QTREEVIEWINSPECT_H
