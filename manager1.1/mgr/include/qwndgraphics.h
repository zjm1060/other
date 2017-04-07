#ifndef QWNDGRAPHICS_H
#define QWNDGRAPHICS_H

#include <QGraphicsView>
#include <QMap>

#include "qabstractreceiver.h"
#include "qwndgraphicsscene.h"
#include "qlogingraphicsscene.h"

class QWndGraphics : public QGraphicsView, public QAbstractReceiver
{
    Q_OBJECT

public:
    QWndGraphics(QWidget* parent=0);
	virtual ~QWndGraphics();

public:
	void setLocalServer(const ServerInfo& info);

public:
	void closeEvent();

	void openLoginWnd();

public:
	virtual void recvLocalServer(int flag, const ServerInfo& info);
	virtual void recvLocalServerQuit();
	virtual void recvInformStatusChanged(quint64 sign);
	virtual void recvTopology(int flag, const GraphTopo& graph);
	virtual void recvRemoteServerList(int flag, const QList<ServerInfo>& servers);

public slots:
	void refresh();

	void logged();

	void logout();
	void logout(const QString& user);
	void forceLogout(const QString user);

	void setCenterPos(QPointF pt);

protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

	virtual void wheelEvent(QWheelEvent* event);

private:
	quint32		m_nUsrID;		//用户ID,用于接收消息

	QWndGraphicsScene*		m_pScene;
	QLoginGraphicsScene*	m_pLoginPanel;

	bool		m_bLeftBtnPressed;
	QPoint		m_ptStart,m_ptEnd;
};

#endif	//QWNDGRAPHICS_H
