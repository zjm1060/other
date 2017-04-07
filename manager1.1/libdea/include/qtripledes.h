#ifndef QTRIPLEDES_H
#define QTRIPLEDES_H

#include <QByteArray>
#include <QString>

#include "qabstractdes.h"

class DEA_EXPORT QTripleDES : public QAbstractDES
{
public:
	QTripleDES();
	~QTripleDES();

public:
	enum {Type=TripleDES};

public:
	virtual int type() const	{return Type;}
	virtual const QString desc() const {return "TripleDES";}

	virtual QByteArray encrypt(const QByteArray& data);	//º”√‹
	virtual QByteArray decrypt(const QByteArray& data);

private:
	QByteArray run(const QByteArray& data, int type);
};

#endif // QTRIPLEDES_H
