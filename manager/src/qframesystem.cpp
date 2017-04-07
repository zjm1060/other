#include "qframesystem.h"

QFrameSystem::QFrameSystem(QWidget *parent)
	: QFrame(parent)
{
	setupUi(this);
}

QFrameSystem::~QFrameSystem()
{

}

void QFrameSystem::setOperatingSystem(QVector<OperatingSystem>& system)
{
	m_vecOperatingSystem=system;

	showOperatingSystem();
}

void QFrameSystem::setComputerSystem(QVector<ComputerSystem>& system)
{
	m_vecComputerSystem=system;
	showComputerSystem();
}

void QFrameSystem::showOperatingSystem()
{
	if (m_vecOperatingSystem.size() == 0)
	{
		return;
	}

	QString strOperatingSystem=m_vecOperatingSystem[0].caption+"\n"
		+tr("Version: ")+m_vecOperatingSystem[0].version;
	if (!m_vecOperatingSystem[0].buildNumber.isEmpty())
	{
		strOperatingSystem+="(Build "+m_vecOperatingSystem[0].buildNumber+")";
	}

	if (!m_vecOperatingSystem[0].CSDVersion.isEmpty())
	{
		strOperatingSystem+="\n"+m_vecOperatingSystem[0].CSDVersion;
	}

	labelOperatingSystem->setText(strOperatingSystem);
	if (m_vecOperatingSystem[0].caption.contains("microsoft", Qt::CaseInsensitive))
	{
		if (m_vecOperatingSystem[0].caption.contains("windows xp", Qt::CaseInsensitive))
		{
			labelLogo->setPixmap(QPixmap(":/images/windowsxp.png"));
		}
		else
		{
			labelLogo->setPixmap(QPixmap(":/images/windows7.png"));
		}
	}
	else if (m_vecOperatingSystem[0].caption.contains("AIX", Qt::CaseInsensitive))
	{
		labelLogo->setPixmap(QPixmap(":/images/aix.png"));
	}
	else if (m_vecOperatingSystem[0].caption.contains("SunOS", Qt::CaseInsensitive))
	{
		labelLogo->setPixmap(QPixmap(":/images/solaris.png"));
	}
}

void QFrameSystem::showComputerSystem()
{
	if (m_vecComputerSystem.size() == 0)
	{
		return;
	}

	lineEditComputerName->setText(m_vecComputerSystem[0].name);
	lineEditWorkgroup->setText(m_vecComputerSystem[0].workgroup);
}
