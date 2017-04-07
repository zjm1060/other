#ifndef QABSTRACTPANEL_H
#define QABSTRACTPANEL_H

#include <QWidget>

class QHBoxLayout;
class QAbstractButton;
class QFloatingDockWidget;
class QToolButton;

enum PanelGroup
{
	PanelMenu,
	PanelApp,
	PanelSystem,
	PanelEnd
};

enum PanelSequence
{
	ComputerPanel,
	TrashPanel,
	MachinePanel,
	ProcessPanel,
	FileSystemPanel,
	DataTimePanel
};

class QAbstractPanel : public QWidget
{
	Q_OBJECT

public:
	QAbstractPanel(QWidget* parent=0);
	virtual ~QAbstractPanel();

public:
	virtual int belongsTo()=0;	//分组号
	virtual int sequence()=0;	//组内序列号

protected:
	virtual void showWidget(QObject* watched);
	virtual void hideWidget(QObject* watched);

	virtual bool eventFilter(QObject* watched, QEvent* event);

protected:
	void addToolButton(QToolButton* button);
	void setDefaultIconSize(QAbstractButton* button);

	QFloatingDockWidget* createDockWidget(const QString& title, int width, int height);

protected:
	QHBoxLayout*	m_pLayout;

};

#endif // QABSTRACTPANEL_H
