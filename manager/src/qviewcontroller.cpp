#include "qviewcontroller.h"
#include "qwndmanager.h"
#include "qmessagesystem.h"
#include "qmessagecomputersystem.h"
#include "qmessagenetworkadapter.h"
#include "qmessageprocess.h"
#include "qmessageprocessor.h"
#include "qmessagedisk.h"
#include "qmessagememory.h"

#ifdef Q_OS_WIN
#include "qabstractwmi.h"
#endif

QViewController::QViewController(QWndManager* wnd, QObject *parent)
	: QFileController(parent)
{
	m_pWnd=wnd;
}

QViewController::~QViewController()
{

}

void QViewController::showProcessor(int flag, QVector<ProcessorInfo> processors)
{
	if (flag == QMessageProcessor::QuerySucceed)
	{
		m_pWnd->resourceManager()->setProcessors(processors);
	}
}

void QViewController::showPerfProcessor(int flag, PerfProcessor processor)
{
	if (flag == QMessageProcessor::QuerySucceed)
	{
		m_pWnd->resourceManager()->setPerfProcessor(processor);
	}
}

void QViewController::showPhysicalMemroy(int flag, QVector<PhysicalMemory> vecMemory)
{
	if (flag == QMessageMemory::QuerySucceed)
	{
		m_pWnd->resourceManager()->setPhysicalMemory(vecMemory);
	}
}

void QViewController::showSystemMemory(int flag, SystemMemory memory)
{
	if (flag == QMessageMemory::QuerySucceed)
	{
		m_pWnd->resourceManager()->setSystemMemory(memory);
	}
}

void QViewController::showDiskDrive(int flag, QVector<DiskDrive> diskDrives)
{
	if (flag == QMessageDisk::QuerySucceed)
	{
		m_pWnd->resourceManager()->setDiskDrive(diskDrives);
	}
}

void QViewController::showCDROMDrive(int flag, QVector<CDROMDrive> drives)
{
	if (flag == QMessageDisk::QuerySucceed)
	{
		m_pWnd->resourceManager()->setCDROMDrive(drives);
	}
}

void QViewController::showLogicalDisk(int flag, QVector<LogicalDisk> disk)
{
	if (flag == QMessageDisk::QuerySucceed)
	{
		m_pWnd->resourceManager()->setLogicalDisk(disk);
	}
}

void QViewController::showOperatingSystem(int flag, QVector<OperatingSystem> vecSystem)
{
	if (flag == QMessageSystem::QuerySucceed)
	{
		m_pWnd->systemManager()->setOperatingSystem(vecSystem);
	}
}

void QViewController::showComputerSystem(int flag, QVector<ComputerSystem> vecSystem)
{
	if (flag == QMessageComputerSystem::QuerySucceed)
	{
		m_pWnd->systemManager()->setComputerSystem(vecSystem);
	}
}

void QViewController::showNetworkAdapter(int flag, QVector<NetworkAdapter> vecAdapter)
{
	if (flag == QMessageNetworkAdapter::QuerySucceed)
	{
		m_pWnd->systemManager()->setNetworkAdapter(vecAdapter);
	}
}

void QViewController::showSystemProcess(int flag, QVector<SystemProcess> vecProcess)
{
	if (flag == QMessageProcess::QuerySucceed)
	{
		m_pWnd->processManager()->setSystemProcess(vecProcess);
	}
}

void QViewController::showCreateProcess(QString strCommand, int result)
{
	//Êä³öµ½×´Ì¬À¸
}

void QViewController::showTerminateProcess(quint32 nProcessId, int result)
{
#ifdef Q_OS_WIN
	if (result != QAbstractWmi::ExecMethodSucceed)
	{
		//Êä³öµ½×´Ì¬À¸
	}
#endif
}
