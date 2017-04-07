#ifndef QMIBLOGICALDISK_H
#define QMIBLOGICALDISK_H

#include "qmibextern.h"

class MIB_EXPORT QMibLogicalDisk : public QAbstractMib
{
public:
	QMibLogicalDisk();
	~QMibLogicalDisk();

public:
	enum {Type=UTIL_TYPE_LOGICAL_DISK};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_disks; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_disks)

private:
	QVector<LogicalDisk>	m_disks;
};

#endif // QMIBLOGICALDISK_H
