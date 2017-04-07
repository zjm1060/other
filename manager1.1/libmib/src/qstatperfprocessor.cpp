#include "qstatperfprocessor.h"

#ifdef Q_OS_SOLARIS
#include <kstat.h>
#include <sys/sysinfo.h>
#include <string.h>

#include <sys/types.h>
#include <sys/systeminfo.h>
#include <sys/utsname.h>
#include <sys/stat.h>

#elif defined(Q_OS_AIX)
#include "qperfstat.h"
#endif

#include "qlogger.h"

QStatPerfProcessor::QStatPerfProcessor(QObject *parent)
: QObject(parent)
{
}

QStatPerfProcessor::~QStatPerfProcessor()
{
}

void QStatPerfProcessor::init()
{
	m_mapCpuUsage.clear();
}

void QStatPerfProcessor::beferQuery()
{
	m_processor.core.clear();
}

void QStatPerfProcessor::query()
{
#ifdef Q_OS_SOLARIS
	kstat_ctl_t *kc;
	kstat_t *ksp;
	kstat_named_t *knp;

	if ((kc = kstat_open()) == NULL)
	{
		return;
	}

	for (ksp = kc->kc_chain; ksp!=NULL; ksp = ksp->ks_next)
	{
		if ((strcmp(ksp->ks_module, "cpu_info")) != 0) continue;
		if (kstat_read(kc, ksp, NULL) == -1)
		{
			continue;
		}

		knp=(kstat_named_t*)kstat_data_lookup(ksp, "current_clock_Hz");
		m_processor.currentClockSpeed=(quint32)(knp->value.ui32/1000000.0+0.5);

		m_processor.status=QString::fromLocal8Bit(knp->value.str.addr.__pad);
	}

	PerfProcessorCore processor;
	CpuUsage old,now;
	cpu_stat_t cs;
	for (ksp = kc->kc_chain; ksp!=NULL; ksp = ksp->ks_next)
	{
		if ((strcmp(ksp->ks_module, "cpu_stat")) != 0) continue;
		if (kstat_read(kc, ksp, &cs) == -1)
		{
			continue;
		}

		processor.name=QString("CPU%1").arg(ksp->ks_instance);

		now.user=cs.cpu_sysinfo.cpu[CPU_USER];
		now.wait=cs.cpu_sysinfo.cpu[CPU_WAIT];
		now.kernel=cs.cpu_sysinfo.cpu[CPU_KERNEL];
		now.idle=cs.cpu_sysinfo.cpu[CPU_IDLE];

		if (!m_mapCpuUsage.contains(ksp->ks_instance))
		{
			processor.percentProcessorTime=0.0;
		}
		else
		{
			old=m_mapCpuUsage[ksp->ks_instance];

			processor.percentProcessorTime=
				(now.user-old.user)*100.0/(now.user-old.user+now.idle-old.idle+now.kernel-old.kernel+now.wait-old.wait);
		}

		m_mapCpuUsage.insert(ksp->ks_instance, now);

		m_processor.core.append(processor);
	}

	kstat_close(kc);
#elif defined(Q_OS_AIX)
	getStatPerfProcessor(m_processor, m_mapCpuUsage);
#endif
}

REGISTER_STAT_INSTANCE(QStatPerfProcessor)
