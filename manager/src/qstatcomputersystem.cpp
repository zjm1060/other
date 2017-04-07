#include "qstatcomputersystem.h"

#include "define.h"
#include "common.h"

#include <stdlib.h>

#ifndef Q_OS_WIN
#include <sys/utsname.h>
#include <sys/types.h>
#include <grp.h> 
#endif

QStatComputerSystem::QStatComputerSystem(QObject *parent)
: QObject(parent)
{
}

QStatComputerSystem::~QStatComputerSystem()
{
}

void QStatComputerSystem::beferQuery() 
{
	m_vecSystem.clear(); 
}

void QStatComputerSystem::query()
{
	ComputerSystem system;

#ifndef Q_OS_WIN
	struct utsname os;

	if((uname(&os)) < 0)
	{
		return;
	}

	system.name=QString::fromLocal8Bit(os.nodename);

	struct group *grp;
	if ((grp = getgrgid(getgid())) != NULL)
	{
		system.workgroup=QString::fromLocal8Bit(grp->gr_name);
	}
	else
	{
		system.workgroup=QString("%8d").arg(getgid());
	}
#endif

	m_vecSystem.append(system);
}
