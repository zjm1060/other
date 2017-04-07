#include "qstatcomputersystem.h"

#include "define.h"
#include "common.h"

#ifndef Q_OS_WIN
#include <sys/utsname.h>
#include <sys/types.h>
#include <grp.h> 
#endif

#include "qstatfactory.h"

QStatComputerSystem::QStatComputerSystem(QObject *parent)
: QObject(parent)
{
}

QStatComputerSystem::~QStatComputerSystem()
{
}

void QStatComputerSystem::beferQuery() 
{
	
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

	system.numberOfProcessors=0;
	QAbstractStatPtr ptr=QStatFactory::instance()->statInstance(UTIL_TYPE_PROCESSOR);
	if (ptr != NULL)
	{
		QVariant var=ptr->exec();
		if (var.isValid())
		{
			QVector<ProcessorInfo> processors=var.value<QVector<ProcessorInfo> >();
			system.numberOfProcessors=processors.size();
			if (system.numberOfProcessors > 0)
			{
				system.processor=processors[0].name;
			}
		}
	}

	system.totalPhysicalMemory=0;
	ptr=QStatFactory::instance()->statInstance(UTIL_TYPE_PHYSICAL_MEMORY);
	if (ptr != NULL)
	{
		QVariant var=ptr->exec();
		if (var.isValid())
		{
			QVector<PhysicalMemory> memories=var.value<QVector<PhysicalMemory> >();
			for (int i=0; i<memories.size(); i++)
			{
				system.totalPhysicalMemory+=memories[i].capacity;
			}
		}
	}
#endif

	m_system=system;
}

REGISTER_STAT_INSTANCE(QStatComputerSystem)
