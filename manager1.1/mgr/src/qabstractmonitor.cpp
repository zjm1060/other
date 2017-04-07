#include "qabstractmonitor.h"

#include <QEvent>
#include <QFileInfo>

QAbstractMonitor::QAbstractMonitor(const QString& filename, QObject* watched, QObject *parent)
	: QObject(parent)
	,m_strFileName(filename)
	,m_pWatched(watched)
	,m_bFirst(true)
{
	if (watched != NULL)
	{
		watched->installEventFilter(this);
	}
}

QAbstractMonitor::~QAbstractMonitor()
{
	if (m_pWatched != NULL)
	{
		m_pWatched->removeEventFilter(this);
	}
}

bool QAbstractMonitor::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::Show || event->type() == QEvent::ShowToParent)
	{
		load();
	}

	return QObject::eventFilter(watched, event);
}

void QAbstractMonitor::load()
{
	if (!m_bFirst && (m_strFileName.isEmpty() || m_file == entryInfo(QFileInfo(m_strFileName), false)))
	{
		return;
	}

	if (loadFile())
	{
		m_file=entryInfo(QFileInfo(m_strFileName), false);
	}
}

