#ifndef QSTATLOGICALDISK_H
#define QSTATLOGICALDISK_H

#include "qmibextern.h"

#ifdef Q_OS_SOLARIS
#include <sys/statvfs.h>
#include <sys/mnttab.h>
#endif

class MIB_EXPORT QStatLogicalDisk : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatLogicalDisk(QObject *parent=0);
	~QStatLogicalDisk();

	enum {Type=UTIL_TYPE_LOGICAL_DISK};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecLogicalDisk)

protected:
	virtual void beferQuery();
	virtual void query();

#ifdef Q_OS_SOLARIS
private:
	int dfreemnt(char *file, struct mnttab *mnt, LogicalDisk& disk);
	int chroot_stat(char *dir, int (*statfunc)(const char*, statvfs64_t*), statvfs64_t *statp, char **dirp);
	char* zap_chroot(char *path);
	char* pathsuffix(char *full, char *pref);
#endif

private:
	QVector<LogicalDisk>	m_vecLogicalDisk;

	char*	m_strChrootPath;
};

#endif // QWMILOGICALDISK_H
