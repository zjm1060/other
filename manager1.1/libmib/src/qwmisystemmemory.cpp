#include "qwmisystemmemory.h"

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

QWmiSystemMemory::QWmiSystemMemory(QObject *parent)
: QObject(parent)
{
}

QWmiSystemMemory::~QWmiSystemMemory()
{
}

void QWmiSystemMemory::beferQuery()
{
	m_memory.datetime=QDateTime::currentDateTime();

	PERFORMANCE_INFORMATION info;
	GetPerformanceInfo(&info, sizeof(info));

	m_memory.systemCache=(quint64)info.SystemCache*info.PageSize/1024;
	m_memory.sizeUsageInPagingFilesPeak=(quint64)info.CommitPeak*info.PageSize/1024;
}

QStringList QWmiSystemMemory::query()
{
	QStringList listQuery;
	listQuery<<"SELECT FreePhysicalMemory, FreeSpaceInPagingFiles, FreeVirtualMemory, SizeStoredInPagingFiles,		\
			   TotalSwapSpaceSize, TotalVirtualMemorySize, TotalVisibleMemorySize FROM Win32_OperatingSystem"
			 <<"SELECT * FROM Win32_PerfFormattedData_PerfOS_Memory";

	return listQuery;
}

void QWmiSystemMemory::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);

	Q_ASSERT_NO_NULL(object);

	switch (index)
	{
	case 0:
		{
			m_memory.freePhysicalMemory=findUInt64(object, L"FreePhysicalMemory");
			m_memory.freeSpaceInPagingFiles=findUInt64(object, L"FreeSpaceInPagingFiles");
			m_memory.freeVirtualMemory=findUInt64(object, L"FreeVirtualMemory");
			m_memory.sizeStoredInPagingFiles=findUInt64(object, L"SizeStoredInPagingFiles");
			m_memory.totalVirtualMemorySize=findUInt64(object, L"TotalVirtualMemorySize");
			m_memory.totalVisibleMemorySize=findUInt64(object, L"TotalVisibleMemorySize");

			if (m_memory.sizeStoredInPagingFiles != -1 && m_memory.freeSpaceInPagingFiles != -1)
			{
				m_memory.sizeUsageInPagingFiles=m_memory.sizeStoredInPagingFiles-m_memory.freeSpaceInPagingFiles;
			}
			else
			{
				m_memory.sizeUsageInPagingFiles=-1;
			}
		}
		break;
	case 1:
		{
			m_memory.poolNonpaged=findUInt64(object, L"PoolNonpagedBytes");
			if (m_memory.poolNonpaged != -1)
			{
				m_memory.poolNonpaged/=1024;
			}

			m_memory.poolPaged=findUInt64(object, L"PoolPagedBytes");
			if (m_memory.poolPaged != -1)
			{
				m_memory.poolPaged/=1024;
			}
		}
		break;
	default:
		break;
	}
}

REGISTER_WMI_INSTANCE(QWmiSystemMemory)
