#ifndef QSTATDISKDRIVE_H
#define QSTATDISKDRIVE_H

#ifndef Q_OS_WIN
#include <QMap>
#endif

#include "qmibextern.h"

class MIB_EXPORT QStatDiskDrive : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatDiskDrive(QObject *parent=0);
	~QStatDiskDrive();

	enum {Type=UTIL_TYPE_DISK_DRIVE};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecDiskDrive)

protected:
	virtual void beferQuery();
	virtual void query();

#ifdef Q_OS_SOLARIS
private:
	QMap<QString, DiskDrive>	m_mapDisk;

private:
	void insertDisk(const QString& name);
#endif

private:
	QVector<DiskDrive>	m_vecDiskDrive;
};

#endif // QSTATDISKDRIVE_H

