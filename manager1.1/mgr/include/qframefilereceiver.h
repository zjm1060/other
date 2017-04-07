#ifndef QFRAMEFILERECEIVER_H
#define QFRAMEFILERECEIVER_H

#include <QFrame>
#include <QPushButton>
#include <QCheckBox>
#include <QMap>

#include "qlinkbutton.h"
#include "qlistviewfilereceiver.h"

class QFrameFileReceiver : public QFrame
{
	Q_OBJECT

public:
	QFrameFileReceiver(const QString& record, const QString& subPath, QWidget *parent=0);
	~QFrameFileReceiver();

public:
	void refreshWorkspace();

	bool hasSelected() const;
	int nextSelected(int row, ServerInfo& info) const;

	bool isAddedItem(int row) const;
	QString savePath(int row) const;

signals:
	void selectedChanged();

	void sendTip(int type, QString tip);

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event);

private slots:
	void readWorkspace();
	void currentTextChanged();

	void configureChanged(const QMap<QString, QString>& configure);

	void stateChanged(int state);

	void onClickedBtnWorkspace();

	void onClickedBtnSelectAll();
	void onClickedBtnSelectNone();

private:
	void writeConfigure();

	QMap<QString, QVector<QString> > readConfigServers(const QString& filename) const;

	void setMenuState(bool refresh);

private:
	QLinkButton*	m_pButtonWorkspace;				//同步接收方工作目录
	QListViewFileReceiver*	m_pListViewReveiver;	//同步接收方文件列表

//	QCheckBox*		m_pButtonShowAll;
	QLinkButton*	m_pButtonSelectAll;
	QLinkButton*	m_pButtonSelectNone;
	QPushButton*	m_pButtonRefresh;

	QString			m_strWorkspace;

	QMonitorConfigureFile*	m_pMonitor;
	QMap<QString, QString>	m_configure;	//接收端配置信息
	QString			m_strSubPath;

	QMap<QString, QVector<QString> >	m_servers;	// 读取的配置主机列表

	bool	m_bRefreshing;
};

#endif // QFRAMEFILERECEIVER_H
