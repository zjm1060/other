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
#include "qreceiverparser.h"

class QResourceManager : public QFrame, public QAbstractReceiver
{
	Q_OBJECT

	enum ButtonId
	{
		ButtonProcessor=0,
		ButtonMemory,
		ButtonDisk
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

public:
	virtual void recvProcessor(int flag, const QVector<ProcessorInfo>& processors);
	virtual void recvPerfProcessor(int flag, const PerfProcessor& processor);

	virtual void recvPhysicalMemroy(int flag, const QVector<PhysicalMemory>& memories);
	virtual void recvSystemMemory(int flag, const SystemMemory& memory);

	virtual void recvDiskDrive(int flag, const QVector<DiskDrive>& drives);
	virtual void recvCDROMDrive(int flag, const QVector<CDROMDrive>& drives);
	virtual void recvLogicalDisk(int flag, const QVector<LogicalDisk>& disc);

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
	quint32			m_nUsrID;			//ÓÃ»§ID

	QHBoxLayout*		m_layout;
	QButtonList*		m_pButtonList;

	QFrameProcessor*	m_pFrameProcessor;
	QFrameMemory*		m_pFrameMemory;
	QFrameDisk*			m_pFrameDisk;

	bool m_bQueryMemory;
	bool m_bPerfProcessor;
};

#endif // QRESOURCEMANAGER_H
