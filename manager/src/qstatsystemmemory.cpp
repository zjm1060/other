#include "qstatsystemmemory.h"

#include "define.h"
#include "common.h"

#ifdef Q_OS_SOLARIS
#include <kstat.h>
#include <vm/anon.h>
#include <sys/swap.h>

#elif defined(Q_OS_AIX)
#include <libperfstat.h>
#endif

QStatSystemMemory::QStatSystemMemory(QObject *parent)
: QObject(parent),
m_nUsagePeak(0)
{
}

QStatSystemMemory::~QStatSystemMemory()
{
}

void QStatSystemMemory::query()
{
	m_memory.datetime=QDateTime::currentDateTime();

#ifdef Q_OS_SOLARIS
	kstat_ctl_t *kc = NULL;
	kstat_t *ksp;
	kstat_named_t *knp;

	int nFactor=(int)(sysconf(_SC_PAGESIZE) >> 10);

	quint64 nPhysicalPages=sysconf(_SC_PHYS_PAGES);
	m_memory.totalVisibleMemorySize=nPhysicalPages*nFactor;

	if((kc = kstat_open()) == NULL)
	{
		qDebug("QStatSystemMemory::kstat_open fail!\n");
		return;
	}

	ksp = kstat_lookup(kc, (char*)"unix", -1, (char*)"system_pages");
	if(ksp && kstat_read(kc, ksp, NULL) >= 0)
	{
		knp = (kstat_named_t *)kstat_data_lookup(ksp, (char*)"pagesfree");
		if(knp != NULL)
		{
			m_memory.freePhysicalMemory=knp->value.ui32*nFactor;
		}
	}

	struct anoninfo ai;
	if(swapctl(SC_AINFO, &ai) == -1)
	{
		qDebug("swapctl fail!\n");
		kstat_close(kc);
		return;
	}

	m_memory.totalVirtualMemorySize=ai.ani_max*nFactor;
	m_memory.freeVirtualMemory=(ai.ani_max - ai.ani_resv)*nFactor;
	m_memory.systemCache=ai.ani_resv*nFactor;

	m_memory.sizeUsageInPagingFiles=m_memory.systemCache+m_memory.totalVisibleMemorySize-m_memory.freePhysicalMemory;
	m_memory.sizeStoredInPagingFiles=m_memory.totalVisibleMemorySize+m_memory.totalVirtualMemorySize;

	if (m_nUsagePeak<m_memory.sizeUsageInPagingFiles)
	{
		m_nUsagePeak=m_memory.sizeUsageInPagingFiles;
	}
	m_memory.sizeUsageInPagingFilesPeak=m_nUsagePeak;

	m_memory.poolPaged=m_memory.poolNonpaged=0;

	ksp = kstat_lookup(kc, "vmem", -1, "kmem_oversize");
	if(ksp && kstat_read(kc, ksp, NULL) >= 0)
	{
		knp = (kstat_named_t *)kstat_data_lookup(ksp, "mem_total");
		if(knp != NULL)
		{
			m_memory.poolPaged=knp->value.ui64;
			m_memory.poolNonpaged=m_memory.poolPaged;
		}
	}

	quint64 nSlabCreate, nSlabDestroy, nSlabSize, nMemTotal;
	quint64 nBufSize, nBufAvail;
	for (ksp = kc->kc_chain; ksp; ksp = ksp->ks_next)
	{
		if (strcmp(ksp->ks_class, "kmem_cache") == 0 && kstat_read(kc, ksp, NULL) != -1)
		{
			knp = (kstat_named_t *)kstat_data_lookup(ksp, "slab_create");
			nSlabCreate=knp!=NULL? knp->value.ui64:0;
			
			knp = (kstat_named_t *)kstat_data_lookup(ksp, "slab_destroy");
			nSlabDestroy=knp!=NULL? knp->value.ui64:0;

			knp = (kstat_named_t *)kstat_data_lookup(ksp, "slab_size");
			nSlabSize=knp!=NULL? knp->value.ui64:0;

			knp = (kstat_named_t *)kstat_data_lookup(ksp, "buf_size");
			nBufSize=knp!=NULL? knp->value.ui64:0;

			knp = (kstat_named_t *)kstat_data_lookup(ksp, "buf_avail");
			nBufAvail=knp!=NULL? knp->value.ui64:0;

			nMemTotal=(nSlabCreate-nSlabDestroy)*nSlabSize;

			m_memory.poolPaged+=nMemTotal-nBufSize*nBufAvail;
			m_memory.poolNonpaged+=nMemTotal;
		}
	}

	m_memory.poolNonpaged-=m_memory.poolPaged;

	m_memory.poolNonpaged/=1024;
	m_memory.poolPaged/=10254;

	kstat_close(kc);

#elif defined(Q_OS_AIX)
	perfstat_memory_total_t minfo;
	int rc;
	rc = perfstat_memory_total(NULL, &minfo, sizeof(perfstat_memory_total_t), 1);    
	if (rc != 1)
	{
		qDebug("perfstat_memory_total error!!!");
		return;
	} 

	m_memory.totalVisibleMemorySize=minfo.real_total*4;
	m_memory.totalVirtualMemorySize=minfo.virt_total*4;

	m_memory.freePhysicalMemory=minfo.real_free*4;
	m_memory.freeVirtualMemory=(minfo.real_avail-minfo.real_free)*4;

	m_memory.systemCache=minfo.real_system*4;

	m_memory.sizeUsageInPagingFiles=m_memory.systemCache+m_memory.totalVisibleMemorySize-m_memory.freePhysicalMemory;
	m_memory.sizeStoredInPagingFiles=m_memory.totalVisibleMemorySize+m_memory.totalVirtualMemorySize;

	if (m_nUsagePeak<m_memory.sizeUsageInPagingFiles)
	{
		m_nUsagePeak=m_memory.sizeUsageInPagingFiles;
	}
	m_memory.sizeUsageInPagingFilesPeak=m_nUsagePeak;

	m_memory.poolPaged=(minfo.pgsp_total-minfo.pgsp_free-minfo.pgsp_rsvd)*4;
	m_memory.poolNonpaged=(minfo.pgsp_free+minfo.pgsp_rsvd)*4;
#endif
}
