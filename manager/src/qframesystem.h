#ifndef QFRAMESYSTEM_H
#define QFRAMESYSTEM_H

#include <QFrame>
#include <QResizeEvent>

#include "ui_system.h"
#include "extern.h"

using namespace Ui;

class QFrameSystem : public QFrame, public Ui::FrameSystem
{
	Q_OBJECT

public:
	QFrameSystem(QWidget *parent);
	~QFrameSystem();

public:
	void setOperatingSystem(QVector<OperatingSystem>& system);
	void setComputerSystem(QVector<ComputerSystem>& system);

private:
	void showOperatingSystem();
	void showComputerSystem();

private:
	QVector<OperatingSystem>		m_vecOperatingSystem;
	QVector<ComputerSystem>			m_vecComputerSystem;
};

#endif // QFRAMESYSTEM_H
