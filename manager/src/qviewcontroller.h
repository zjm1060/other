#ifndef QVIEWCONTROLLER_H
#define QVIEWCONTROLLER_H

#include <QVector>
#include <QByteArray>

#include "qfilecontroller.h"
#include "extern.h"
#include "qresourcemanager.h"
#include "qmessagefiletransfer.h"

class QWndManager;
class QViewController : public QFileController
{
	Q_OBJECT

	friend class QWndManager;

public:
	QViewController(QWndManager* wnd, QObject *parent=0);
	~QViewController();

public:
	Q_INVOKABLE void showProcessor(int flag, QVector<ProcessorInfo> processors);
	Q_INVOKABLE void showPerfProcessor(int flag, PerfProcessor processor);

	Q_INVOKABLE void showPhysicalMemroy(int flag, QVector<PhysicalMemory> vecMemory);
	Q_INVOKABLE void showSystemMemory(int flag, SystemMemory memory);

	Q_INVOKABLE void showDiskDrive(int flag, QVector<DiskDrive> diskDrives);
	Q_INVOKABLE void showCDROMDrive(int flag, QVector<CDROMDrive> drives);
	Q_INVOKABLE void showLogicalDisk(int flag, QVector<LogicalDisk> disk);

	Q_INVOKABLE void showOperatingSystem(int flag, QVector<OperatingSystem> vecSystem);
	Q_INVOKABLE void showComputerSystem(int flag, QVector<ComputerSystem> vecSystem);

	Q_INVOKABLE void showNetworkAdapter(int flag, QVector<NetworkAdapter> vecAdapter);
	Q_INVOKABLE void showSystemProcess(int flag, QVector<SystemProcess> vecProcess);
	Q_INVOKABLE void showCreateProcess(QString strCommand, int result);
	Q_INVOKABLE void showTerminateProcess(quint32 nProcessId, int result);

private:
	QWndManager*	m_pWnd;
};

#endif // QVIEWCONTROLLER_H
