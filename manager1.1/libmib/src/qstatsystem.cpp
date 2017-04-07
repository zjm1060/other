#include "qstatsystem.h"

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

	quint32 bits;
	char *isa;
	if ((isa = getsysinfo(SI_ARCHITECTURE_64)) == NULL)
	{
		isa=getsysinfo(SI_ARCHITECTURE_32);
	}
	
	if (isa != NULL)
	{
		bits = bitness(isa);
		if (bits != 0)
		{
			system.OSArchitecture=QString("%1-bits").arg(bits);
		}

		free(isa);
	}
	
#elif defined(Q_OS_AIX)
	system.caption=QString::fromLocal8Bit(os.sysname);
	system.version=QString::fromLocal8Bit(os.version)+"."+QString::fromLocal8Bit(os.release);
#endif

	m_vecSystem.append(system);
}

#ifdef Q_OS_SOLARIS
quint32 QStatSystem::bitness(const char *isaname)
{
 	if (strcmp(isaname, "sparc") == 0 || strcmp(isaname, "i386") == 0)
 		return (32);
 
 	if (strcmp(isaname, "sparcv9") == 0 || strcmp(isaname, "amd64") == 0)
 		return (64);
 
 	return (0);
}

char* QStatSystem::getsysinfo(int cmd)
{
	char *buf;
	size_t bufsize = 20;	/* wild guess */
	long ret;

	if ((buf = (char*)malloc(bufsize)) == NULL)
	{
		return (NULL);
	}

	do
	{
		ret = sysinfo(cmd, buf, bufsize);
		if (ret == -1)
			return (NULL);
		if (ret > bufsize)
		{
			bufsize = ret;
			buf = (char*)realloc(buf, bufsize);
		}
		else
		{
			break;
		}
	} while (buf != NULL);

	return (buf);
}
#endif


REGISTER_STAT_INSTANCE(QStatSystem)
