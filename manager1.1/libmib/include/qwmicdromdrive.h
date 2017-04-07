#ifndef QWMICDROMDRIVE_H
#define QWMICDROMDRIVE_H

#include "qmibextern.h"

class MIB_EXPORT QWmiCDROMDrive : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiCDROMDrive(QObject *parent=0);
	~QWmiCDROMDrive();

	enum {Type=UTIL_TYPE_CDROM_DRIVE};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecCDROMDrive)

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	QVector<CDROMDrive>	m_vecCDROMDrive;
};

#endif // QWMICDROMDRIVE_H
