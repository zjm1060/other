#ifndef QFRAMEDISK_H
#define QFRAMEDISK_H

#include <QFrame>
#include <QVector>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QFileIconProvider>

#include "ui_disk.h"
#include "qutildef.h"
#include "qdiskdelegate.h"

using namespace Ui;

class QFrameDisk : public QFrame, public Ui::FrameDisk
{
	Q_OBJECT

public:
	QFrameDisk(QWidget *parent=0);
	~QFrameDisk();

public:
	void setDiskDrive(const QVector<DiskDrive>& diskDrives);
	void setCDROMDrive(const QVector<CDROMDrive>& drives);
	void setLogicalDisk(const QVector<LogicalDisk>& disk);

	void clear();

private slots:
	void currentIndexChanged(int index);

private:
	void showDiskDrive();
	void showCDROMDrive();
	void showLogicalDisk();

private:
	enum DriveType
	{
		DriveTypeDiskDrive,
		DriveTypeCDROMDrive
	};

private:
	QVector<DiskDrive>		m_vecDiskDrive;
	QVector<CDROMDrive>		m_vecCDROMDrive;
	QVector<LogicalDisk>	m_vecLogicalDisk;

	QStandardItemModel*		m_pModelDrive;

	QStandardItemModel*		m_pModelComputer;
	QSortFilterProxyModel*	m_pSortModel;
	QDiskDelegate*			m_pDelegate;

	QFileIconProvider		m_iconProvider;

	bool	m_bClear;
};

#endif // QFRAMEDISK_H
