#include "qframesystem.h"

#include "common.h"
#include "ui_system.h"

QFrameSystem::QFrameSystem(QWidget *parent)
	: QFrame(parent)
{
	m_ui=new Ui_FrameSystem;
	m_ui->setupUi(this);
}

QFrameSystem::~QFrameSystem()
{
	SAFE_DELETE_CLASS(m_ui);
}

void QFrameSystem::setOperatingSystem(const OperatingSystem& system)
{
	m_operatingSystem=system;

	showOperatingSystem();
}

void QFrameSystem::setComputerSystem(const ComputerSystem& system)
{
	m_computerSystem=system;
	showComputerSystem();
}

void QFrameSystem::showOperatingSystem()
{
	m_ui->lineEditOSName->setText(m_operatingSystem.caption);

	QString strVersion=m_operatingSystem.version;
	if (!m_operatingSystem.buildNumber.isEmpty())
	{
		strVersion+="(Build "+m_operatingSystem.buildNumber+")";
	}

	if (!m_operatingSystem.CSDVersion.isEmpty())
	{
		strVersion+=" "+m_operatingSystem.CSDVersion;
	}

	m_ui->lineEditOSVersion->setText(strVersion);

	if (!m_operatingSystem.OSArchitecture.isEmpty())
	{
		m_ui->lineEditSystemType->setText(m_operatingSystem.OSArchitecture+tr(" Operating System"));
	}

	if (m_operatingSystem.caption.contains("microsoft", Qt::CaseInsensitive))
	{
		if (m_operatingSystem.caption.contains("windows xp", Qt::CaseInsensitive))
		{
			m_ui->labelLogo->setPixmap(findPixmap(":/images/windowsxp.png"));
		}
		else
		{
			m_ui->labelLogo->setPixmap(findPixmap(":/images/windows7.png"));
		}
	}
	else if (m_operatingSystem.caption.contains("AIX", Qt::CaseInsensitive))
	{
		m_ui->labelLogo->setPixmap(findPixmap(":/images/aix.png"));
	}
	else if (m_operatingSystem.caption.contains("SunOS", Qt::CaseInsensitive))
	{
		m_ui->labelLogo->setPixmap(findPixmap(":/images/solaris.png"));
	}
}

void QFrameSystem::showComputerSystem()
{
	m_ui->lineEditProcessor->setText(m_computerSystem.processor);
	m_ui->lineEditNumProcessors->setText(QString("%1").arg(m_computerSystem.numberOfProcessors));
	m_ui->lineEditMemorySize->setText(sizeToString(m_computerSystem.totalPhysicalMemory));
	m_ui->lineEditComputerName->setText(m_computerSystem.name);
	m_ui->lineEditWorkgroup->setText(m_computerSystem.workgroup);
}
