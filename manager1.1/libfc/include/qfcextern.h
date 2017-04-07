#ifndef QFCEXTERN_H
#define QFCEXTERN_H

#include "global.h"

#include <QString>
#include <QVector>

#include "qutildef.h"
#include "qabstractfilecompare.h"

extern "C" FC_EXPORT void  CreateFileCompareInstance(QVector<QAbstractFileComparePtr>& instances);

class FC_EXPORT QFcRegisterInstances
{
public:
	QFcRegisterInstances(){};
	~QFcRegisterInstances(){};

public:
	static QFcRegisterInstances* instance();

public:
	void addInterface(const QAbstractFileComparePtr& ptr);
	const QVector<QAbstractFileComparePtr>& interfaces() const;

private:
	QVector<QAbstractFileComparePtr> m_instances;
};

template<class T>
class QFcRegisterInterface
{
public:
	QFcRegisterInterface()
	{
		QFcRegisterInstances::instance()->addInterface(QAbstractFileComparePtr(new T));
	}
};

#define REGISTER_FC_INSTANCE(name) static QFcRegisterInterface<name> static##name##FcInstance;


#endif // QFCEXTERN_H
