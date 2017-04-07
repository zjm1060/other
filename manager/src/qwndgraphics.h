#ifndef QWNDGRAPHICS_H
#define QWNDGRAPHICS_H

#include <QGraphicsView>
#include <QMap>

#include "qabstractgraphicsitem.h"
#include "qservermanager.h"

class QWndGraphics : public QGraphicsView
{
    Q_OBJECT

public:
    QWndGraphics(QWidget* parent=0);
	virtual ~QWndGraphics();

public:
	bool addServer(const QServerManager::ServerInfo& info);		//返回false，发现新网络，需要对网络进行重新探测
	void removeServer(const QServerManager::ServerInfo& info);

	void reconnect();

public slots:
	void setRouters(QVector<QPair<quint32, int> > routers);
	void setAlives(quint32 route, quint32 gateway, QVector<quint32> alives);
	void checkAlives();
	void disconnected();

	void closeEvent();

private:
	QAbstractGraphicsItem* findRouter(quint32 router, QAbstractGraphicsItem* item);

private:
	QGraphicsScene* m_pScene;

	QAbstractGraphicsItem* m_pAbstractItem;
};

#endif	//QWNDGRAPHICS_H
