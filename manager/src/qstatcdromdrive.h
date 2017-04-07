#ifndef QSTATCDROMDRIVE_H
#define QSTATCDROMDRIVE_H

#include <QObject>
#include <QVector>

#include "qabstractstat.h"
#include "extern.h"

class QStatCDROMDrive : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatCDROMDrive(QObject *parent=0);
	~QStatCDROMDrive();

	enum {Type=WMI_TYPE_CDROM_DRIVE};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecCDROMDrive)

protected:
	virtual void beferQuery();
	virtual void query();

private:
	QVector<CDROMDrive>	m_vecCDROMDrive;
};

#endif // QSTATCDROMDRIVE_H
