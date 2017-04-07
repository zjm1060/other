#ifndef QABSTRACTMONITOR_H
#define QABSTRACTMONITOR_H

#include <QObject>
#include <QMutex>
#include <QMap>

#include "qutildef.h"

class QAbstractMonitor : public QObject
{
	Q_OBJECT

public:
	QAbstractMonitor(const QString& filename, QObject* watched, QObject *parent=0);
	~QAbstractMonitor();

public:
	void load();

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event);

	virtual bool loadFile()=0;

protected:
	QMutex	m_mutex;

	QString		m_strFileName;	//文件名称
	FileInfo	m_file;			//文件信息

	bool		m_bFirst;		//是否首次监视

	QObject*	m_pWatched;		//被监视
};


#endif // QABSTRACTMONITOR_H
