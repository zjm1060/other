#ifndef QPANELFILESYSTEM_H
#define QPANELFILESYSTEM_H

#include "qabstractpanel.h"
#include "qabstractreceiver.h"

QT_BEGIN_NAMESPACE
class QToolButton;
class QDockWidget;
QT_END_NAMESPACE

class QFrameFileSystemPanel;

class QPanelFileSystem : public QAbstractPanel, public QAbstractReceiver
{
	Q_OBJECT

public:
	QPanelFileSystem(QWidget *parent=0);
	~QPanelFileSystem();

public:
	virtual int belongsTo()	{ return PanelSystem; }
	virtual int sequence()	{ return FileSystemPanel; }

public slots:
	void onClickedBtn();

protected:
	virtual void recvLogicalDisk(int flag, const QVector<LogicalDisk>& disc);

protected:
	virtual void showWidget(QObject* watched);
	virtual void hideWidget(QObject* watched);

private:
	void queryLogicalDisk();

private:
	QToolButton*	m_pButton;

	QFloatingDockWidget*	m_pDockWidget;
	QFrameFileSystemPanel*	m_pPanel;

	quint32		m_nUsrID;
};

#endif // QPANELFILESYSTEM_H
