#ifndef QMIBCDROMDRIVE_H
#define QMIBCDROMDRIVE_H

#include "qmibextern.h"

class MIB_EXPORT QMibCDRomDrive : public QAbstractMib
{
public:
	QMibCDRomDrive();
	~QMibCDRomDrive();

public:
	enum {Type=UTIL_TYPE_CDROM_DRIVE};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual void setResult(const QByteArray& buffer);

	virtual void writeStream(QDataStream& stream) { stream<<m_dirves; }

	virtual QVariant getResult() CONVERT_TO_VARIANT(m_dirves)

private:
	QVector<CDROMDrive>	m_dirves;
};

#endif // QMibCDRomDrive
