#ifndef QINFOMANAGER_H
#define QINFOMANAGER_H

#include <QObject>
#include <QMutex>
#include <QColor>
#include <QList>
#include <QTimer>
#include <QPainter>

#include "qsharedobject.h"

class QInfoManager;
typedef QSharedObjectPtr<QInfoManager>	QInfoManagerPtr;

class QInfoManager : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QInfoManager(QObject *parent=0);
	~QInfoManager();

public:
	enum InfoType
	{
		Information=0,
		Warning,
		Error,
		Important,
		InfoTypeEnd
	};

	typedef struct _info
	{
		int type;
		QString text;

		float tick;
	}Info;

public:
	static QInfoManagerPtr instance();

public:
	void append(InfoType type, const QString& text);
	
	void paint(QPainter* painter, int left, int bottom);

private slots:
	void timeout();

signals:
	void refresh();
	void appendTip(QInfoManager::InfoType type, const QString text);

private:
	QMutex			m_mutex;
	QTimer			m_timer;

	QList<Info>		m_listInfos;
	QColor			m_color[InfoTypeEnd];

	int		m_nPointSize;
};

#endif // QINFOMANAGER_H
