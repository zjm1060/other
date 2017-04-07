#ifndef QFRAMESYSTEM_H
#define QFRAMESYSTEM_H

#include <QFrame>

#include "libctlglobal.h"
#include "qutildef.h"

class Ui_FrameSystem;

class CTL_EXPORT QFrameSystem : public QFrame 
{
	Q_OBJECT

public:
	QFrameSystem(QWidget *parent=0);
	~QFrameSystem();

public:
	void setOperatingSystem(const OperatingSystem& system);
	void setComputerSystem(const ComputerSystem& system);

private:
	void showOperatingSystem();
	void showComputerSystem();

private:
	Ui_FrameSystem*		m_ui;

	OperatingSystem		m_operatingSystem;
	ComputerSystem		m_computerSystem;
};

#endif // QFRAMESYSTEM_H
