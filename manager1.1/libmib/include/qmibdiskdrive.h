#ifndef QMIBDISKDRIVE_H
#define QMIBDISKDRIVE_H

#include "qmibextern.h"

class MIB_EXPORT QMibDiskDrive : public QAbstractMib
{
public:
	QMibDiskDrive();
	~QMibDiskDrive();

public:
	enum {Type=UTIL_TYPE_DISK_DRIVE};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_dirves; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_dirves)

private:
	QVector<DiskDrive>	m_dirves;
};

#endif // QMIBDISKDRIVE_H
