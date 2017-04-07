#ifndef QIDEA_H
#define QIDEA_H

#include <QVector>
#include <QString>

#include "global.h"
#include "qsharedobject.h"

enum DEA
{
	PlainText=-1,	//明文
	DES=0,
	TripleDES
};

class UTIL_EXPORT QIDEA : public QSharedObject
{
public:
	QIDEA(){};
	virtual ~QIDEA(){};

public:
	enum {ENCRYPT,DECRYPT};

public:
	virtual int type() const=0;
	virtual const QString desc() const=0;		//类型名称描述

	virtual QByteArray encrypt(const QByteArray& data)=0;	//加密
	virtual QByteArray decrypt(const QByteArray& data)=0;
};

typedef QSharedObjectPtr<QIDEA> QIDEAPtr;

typedef void (*CreateDEAInstanceFunc)(QVector<QIDEAPtr>&);		//数据加密算法动态库必须提供的接口

#endif // QIDEA_H
