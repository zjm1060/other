#ifndef QWMISYSTEM_H
#define QWMISYSTEM_H

#include <QObject>
#include <QVector>

#include "qmibextern.h"

class MIB_EXPORT QWmiSystem : public QObject, public QAbstractWmi
{
	Q_OBJECT

public:
	QWmiSystem(QObject *parent=0);
	~QWmiSystem();

	enum {Type=UTIL_TYPE_SYSTEM};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecSystem);

protected:
	virtual void beferQuery();
	virtual QStringList query();
	virtual void record(IWbemClassObject* object, IWbemServices* service, int index);

private:
	QVector<OperatingSystem>	m_vecSystem;
};

#endif // QWMISYSTEM_H
