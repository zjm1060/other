#ifndef QRESOURCEMANAGER_H
#define QRESOURCEMANAGER_H

#include <QFrame>
#include <QHBoxLayout>
#include <QVector>

#include "qbuttonlist.h"
#include "qclient.h"
#include "qframeprocessor.h"
#include "qframememory.h"
#include "qframedisk.h"

class QResourceManager : public QFrame
{
	Q_OBJECT

	enum ButtonId
	{
		ButtonProcessor=0,
		ButtonMemory,
		ButtonDisk,
		ButtonLog
	};

public:
	QResourceManager(QClient* client, QWidget *parent=0);
	~QResourceManager();

public:
	void setProcessors(const QVector<ProcessorInfo>& processors);
	void setPerfProcessor(const PerfProcessor& processor);
	void setPhysicalMemory(const QVector<PhysicalMemory>& memory);
	void setSystemMemory(const SystemMemory& memory);
	void setDiskDrive(const QVector<DiskDrive>& diskDrives);
	void setCDROMDrive(const QVector<CDROMDrive>& drives);
	void setLogicalDisk(const QVector<LogicalDisk>& disk);

public:
	void reconnected(bool connected);

	void sendQueryMessage();
	void sendStopMessage();

private:
	void queryProcessors();
	void queryPhysicalMemory();
	void queryDrive();

private:
	void querySystemMemory();
	void stopSystemMemory();

	void queryPerfProcessor();
	void stopPerfProcessor();

private slots:
	void buttonClicked(int id);

private:
	QClient*		m_pClient;

	QHBoxLayout*		m_layout;
	QButtonList*		m_pButtonList;

	QFrameProcessor*	m_pFrameProcessor;
	QFrameMemory*		m_pFrameMemory;
	QFrameDisk*			m_pFrameDisk;

	bool m_bQueryMemory;
	bool m_bPerfProcessor;
};

#endif // QRESOURCEMANAGER_H
