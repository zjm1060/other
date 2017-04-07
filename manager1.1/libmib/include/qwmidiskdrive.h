#ifndef QWMIDISKDRIVE_H
#define QWMIDISKDRIVE_H

#include <QObject>
#include <QVector>

#include "qmibextern.h"

class MIB_EXPORT QWmiDiskDrive : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiDiskDrive(QObject *parent=0);
	~QWmiDiskDrive();

	enum {Type=UTIL_TYPE_DISK_DRIVE};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecDiskDrive)

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	QVector<DiskDrive>	m_vecDiskDrive;
};

#endif // QWMIDISKDRIVE_H
