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
#include <libperfstat.h>
#endif

#include "define.h"

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
	perfstat_cpu_total_t *cputotal;
	perfstat_cpu_t *cpu;
	perfstat_id_t *id;
	int ret;

	cputotal = new perfstat_cpu_total_t;
	memset(cputotal, 0, sizeof(perfstat_cpu_total_t));
	ret = perfstat_cpu_total(NULL, cputotal, sizeof(perfstat_cpu_total_t), 1);
	if (ret != -1)
	{
		PerfProcessorCore processor;
		CpuUsage old,now;

		processor.name=QString::fromLocal8Bit("_Total:");

		now.user=cputotal->user;
		now.wait=cputotal->wait;
		now.kernel=cputotal->sys;
		now.idle=cputotal->idle;

		if (!m_mapCpuUsage.contains(-1))
		{
			processor.percentProcessorTime=0.0;
		}
		else
		{
			old=m_mapCpuUsage[-1];

			processor.percentProcessorTime=
				(now.user-old.user)*100.0/(now.user-old.user+now.idle-old.idle+now.kernel-old.kernel+now.wait-old.wait);
		}

		m_mapCpuUsage.insert(-1, now);

		m_processor.core.append(processor);
	}

	/* check how many perfstat_cpu_t structures are available */
	int nNumProcessor =  perfstat_cpu(NULL, NULL, sizeof(perfstat_cpu_t), 0);
	if (nNumProcessor > 0)
	{
		/* Get the indivadul cpu information */
		id = new perfstat_id_t;
		strcpy(id->name, """");

		cpu = new perfstat_cpu_t[nNumProcessor];
		memset(cpu, 0, nNumProcessor*sizeof(perfstat_cpu_t));

		ret = perfstat_cpu(id, cpu, sizeof(perfstat_cpu_t), nNumProcessor);

		if (ret > 0)
		{
			PerfProcessorCore processor;
			CpuUsage old,now;

			for (int i=0; i<ret; i++)
			{
				processor.name=QString::fromLocal8Bit(cpu[i].name);

				now.user=cpu[i].user;
				now.wait=cpu[i].wait;
				now.kernel=cpu[i].sys;
				now.idle=cpu[i].idle;

				if (!m_mapCpuUsage.contains(i))
				{
					processor.percentProcessorTime=0.0;
				}
				else
				{
					old=m_mapCpuUsage[i];

					processor.percentProcessorTime=
						(now.user-old.user)*100.0/(now.user-old.user+now.idle-old.idle+now.kernel-old.kernel+now.wait-old.wait);
				}

				m_mapCpuUsage.insert(i, now);

				m_processor.core.append(processor);
			}
		}

		SAFE_DELETE_CLASS(cputotal);
		SAFE_DELETE_CLASS(id);
		SAFE_DELETE_ARRAY(cpu);
	}
#endif
}
