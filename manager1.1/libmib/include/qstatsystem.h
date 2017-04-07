#ifndef QSTATSYSTEM_H
#define QSTATSYSTEM_H

#include "qmibextern.h"

class MIB_EXPORT QStatSystem : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatSystem(QObject *parent=0);
	~QStatSystem();

	enum {Type=UTIL_TYPE_SYSTEM};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecSystem)

protected:
	virtual void beferQuery();
	virtual void query();

#ifdef Q_OS_SOLARIS
private:
	char* getsysinfo(int cmd);
	quint32 bitness(const char *isaname);
#endif

private:
	QVector<OperatingSystem>	m_vecSystem;
};

#endif // QSTATSYSTEM_H
