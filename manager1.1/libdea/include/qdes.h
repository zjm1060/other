#ifndef QDES_H
#define QDES_H

#include <QByteArray>

#include "qabstractdes.h"

class DEA_EXPORT QDES : public QAbstractDES
{
public:
	QDES();
	~QDES();

public:
	enum {Type=DES};

public:
	virtual int type() const	{return Type;}
	virtual const QString desc() const {return "DES";}

	virtual QByteArray encrypt(const QByteArray& data);	//º”√‹
	virtual QByteArray decrypt(const QByteArray& data);

private:
	QByteArray run(const QByteArray& data, int type);
};

#endif // QDES_H
