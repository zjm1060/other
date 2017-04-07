#ifndef QWMILOGICALDISK_H
#define QWMILOGICALDISK_H

#include <QObject>
#include <QVector>

#include "qmibextern.h"

class MIB_EXPORT QWmiLogicalDisk : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiLogicalDisk(QObject *parent=0);
	~QWmiLogicalDisk();

	enum {Type=UTIL_TYPE_LOGICAL_DISK};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecLogicalDisk)

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	QVector<LogicalDisk>	m_vecLogicalDisk;
};

#endif // QWMILOGICALDISK_H
