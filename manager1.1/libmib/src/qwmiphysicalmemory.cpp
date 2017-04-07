#include "qwmiphysicalmemory.h"

QWmiPhysicalMemory::QWmiPhysicalMemory(QObject *parent)
: QObject(parent)
{
}

QWmiPhysicalMemory::~QWmiPhysicalMemory()
{
}

void QWmiPhysicalMemory::beferQuery() 
{
	m_vecMemory.clear(); 
}

QStringList QWmiPhysicalMemory::query()
{
	QStringList listQuery;
	listQuery<<"SELECT * FROM Win32_PhysicalMemory";

	return listQuery;
}

void QWmiPhysicalMemory::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	PhysicalMemory memory;

	memory.capacity=findUInt64(object, L"Capacity");
	memory.dataWidth=findUInt16(object, L"DataWidth");
	memory.description=findString(object, L"Description");
	memory.manufacturer=findString(object, L"Manufacturer");

	quint16 memType=findUInt16(object, L"MemoryType");
	switch (memType)
	{
	case 2:
		memory.memoryType="DRAM";
		break;
	case 3:
		memory.memoryType="Synchronous DRAM";
		break;
	case 4:
		memory.memoryType="Cache DRAM";
		break;
	case 5:
		memory.memoryType="EDO";
		break;
	case 6:
		memory.memoryType="EDRAM";
		break;
	case 7:
		memory.memoryType="VRAM";
		break;
	case 8:
		memory.memoryType="SRAM";
		break;
	case 9:
		memory.memoryType="RAM";
		break;
	case 10:
		memory.memoryType="ROM";
		break;
	case 11:
		memory.memoryType="Flash";
		break;
	case 12:
		memory.memoryType="EEPROM";
		break;
	case 13:
		memory.memoryType="FEPROM";
		break;
	case 14:
		memory.memoryType="EPROM";
		break;
	case 15:
		memory.memoryType="CDRAM";
		break;
	case 16:
		memory.memoryType="3DRAM";
		break;
	case 17:
		memory.memoryType="SDRAM";
		break;
	case 18:
		memory.memoryType="SGRAM";
		break;
	case 19:
		memory.memoryType="RDRAM";
		break;
	case 20:
		memory.memoryType="DDR";
		break;
	default:
		memory.memoryType=tr("Unknown");
		break;
	}

	memory.name=findString(object, L"Name");
	memory.serialNumber=findString(object, L"SerialNumber");
	memory.speed=findUInt32(object, L"Speed");
	memory.tag=findString(object, L"Tag");
	memory.version=findString(object, L"Version");

	m_vecMemory.append(memory);
}

REGISTER_WMI_INSTANCE(QWmiPhysicalMemory)
