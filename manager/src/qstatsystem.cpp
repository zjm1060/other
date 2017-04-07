#include "qstatsystem.h"

#include "define.h"

#include <stdlib.h>

#ifndef Q_OS_WIN
// #include <kstat.h>
// #include <time.h>
#include <sys/utsname.h>
#endif

#ifdef Q_OS_SOLARIS
#include <sys/systeminfo.h>
#endif

QStatSystem::QStatSystem(QObject *parent)
: QObject(parent)
{
}

QStatSystem::~QStatSystem()
{
}

void QStatSystem::beferQuery() 
{
	m_vecSystem.clear(); 
}

void QStatSystem::query()
{
	OperatingSystem system;

#ifndef Q_OS_WIN
	struct utsname os;

	if((uname(&os)) < 0)
	{
		return;
	}
#endif

#ifdef Q_OS_SOLARIS
	char procbuf[SYS_NMLN];

	system.caption=QString::fromLocal8Bit(os.sysname)+" "+QString::fromLocal8Bit(os.release)+" "+QString::fromLocal8Bit(os.machine);
	if (sysinfo(SI_ARCHITECTURE, procbuf, sizeof (procbuf)) != -1)
	{
		system.caption+=" "+QString::fromLocal8Bit(procbuf);
	}

	if (sysinfo(SI_PLATFORM, procbuf, sizeof (procbuf)) != -1)
	{
		system.caption+=" "+QString::fromLocal8Bit(procbuf);
	}

	system.version=QString::fromLocal8Bit(os.version);
	system.CSDVersion==QString::fromLocal8Bit(os.release);
#elif defined(Q_OS_AIX)
	system.caption=QString::fromLocal8Bit(os.sysname);
	system.version=QString::fromLocal8Bit(os.version)+"."+QString::fromLocal8Bit(os.release);
#endif

	m_vecSystem.append(system);
}
