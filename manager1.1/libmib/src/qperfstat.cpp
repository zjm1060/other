#include "qperfstat.h"

#ifdef Q_OS_AIX
#include <libperfstat.h>
#endif

#ifdef Q_OS_AIX
void getStatPerfProcessor(PerfProcessor& processor, QMap<int, QStatPerfProcessor::CpuUsage>& cpus)
{
	perfstat_cpu_total_t *cputotal;
	perfstat_cpu_t *cpu;
	perfstat_id_t *id;
	int ret;

	cputotal = new perfstat_cpu_total_t;
	memset(cputotal, 0, sizeof(perfstat_cpu_total_t));
	ret = perfstat_cpu_total(NULL, cputotal, sizeof(perfstat_cpu_total_t), 1);
	if (ret != -1)
	{
		PerfProcessorCore core;
		QStatPerfProcessor::CpuUsage old,now;

		core.name=QString::fromLocal8Bit("_Total:");

		now.user=cputotal->user;
		now.wait=cputotal->wait;
		now.kernel=cputotal->sys;
		now.idle=cputotal->idle;

		if (!cpus.contains(-1))
		{
			core.percentProcessorTime=0.0;
		}
		else
		{
			old=cpus[-1];

			core.percentProcessorTime=
				(now.user-old.user)*100.0/(now.user-old.user+now.idle-old.idle+now.kernel-old.kernel+now.wait-old.wait);
		}

		cpus.insert(-1, now);

		processor.core.append(core);
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
			PerfProcessorCore core;
			QStatPerfProcessor::CpuUsage old,now;

			for (int i=0; i<ret; i++)
			{
				core.name=QString::fromLocal8Bit(cpu[i].name);

				now.user=cpu[i].user;
				now.wait=cpu[i].wait;
				now.kernel=cpu[i].sys;
				now.idle=cpu[i].idle;

				if (!cpus.contains(i))
				{
					core.percentProcessorTime=0.0;
				}
				else
				{
					old=cpus[i];

					core.percentProcessorTime=
						(now.user-old.user)*100.0/(now.user-old.user+now.idle-old.idle+now.kernel-old.kernel+now.wait-old.wait);
				}

				cpus.insert(i, now);

				processor.core.append(core);
			}
		}

		SAFE_DELETE_CLASS(cputotal);
		SAFE_DELETE_CLASS(id);
		SAFE_DELETE_ARRAY(cpu);
	}
}

void getStatSystemMemory(SystemMemory& memory, quint64& peak)
{
	perfstat_memory_total_t minfo;
	int rc;
	rc = perfstat_memory_total(NULL, &minfo, sizeof(perfstat_memory_total_t), 1);    
	if (rc != 1)
	{
		qDebug("perfstat_memory_total error!!!");
		return;
	} 

	memory.totalVisibleMemorySize=minfo.real_total*4;
	memory.totalVirtualMemorySize=minfo.virt_total*4;

	memory.freePhysicalMemory=minfo.real_free*4;
	memory.freeVirtualMemory=(minfo.real_avail-minfo.real_free)*4;

	memory.systemCache=minfo.real_system*4;

	memory.sizeUsageInPagingFiles=memory.systemCache+memory.totalVisibleMemorySize-memory.freePhysicalMemory;
	memory.sizeStoredInPagingFiles=memory.totalVisibleMemorySize+memory.totalVirtualMemorySize;

	if (peak<memory.sizeUsageInPagingFiles)
	{
		peak=memory.sizeUsageInPagingFiles;
	}
	memory.sizeUsageInPagingFilesPeak=peak;

	memory.poolPaged=(minfo.pgsp_total-minfo.pgsp_free-minfo.pgsp_rsvd)*4;
	memory.poolNonpaged=(minfo.pgsp_free+minfo.pgsp_rsvd)*4;
}
#endif
