#include "qwmiprocessor.h"

QWmiProcessor::QWmiProcessor(QObject *parent)
: QObject(parent)
{
}

QWmiProcessor::~QWmiProcessor()
{
}

void QWmiProcessor::beferQuery() 
{
	m_vecProcessor.clear(); 
}

QStringList QWmiProcessor::query()
{
	QStringList listQuery;
	listQuery<<"SELECT * FROM Win32_Processor"; 

	return listQuery;
}

void QWmiProcessor::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	ProcessorInfo info;

	info.deviceID=findString(object, L"DeviceID");
	for (int i=0; i<m_vecProcessor.size(); i++)
	{
		if (m_vecProcessor[i].deviceID == info.deviceID)
		{
			return;
		}
	}

	info.os=OS_WINDOWS;

	info.addressWidth=findUInt16(object, L"AddressWidth");

	info.configManager=findUInt32(object, L"ConfigManagerErrorCode");
	info.dataWidth=findUInt16(object, L"DataWidth");
	info.description=findString(object, L"Description");
	info.externalClock=findUInt32(object, L"ExtClock");
	info.L2CacheSize=findUInt32(object, L"L2CacheSize")*1024;
	info.L2CacheSpeed=findUInt32(object, L"L2CacheSpeed");
	info.manufacturer=findString(object, L"Manufacturer");
	info.maxClockSpeed=findUInt32(object, L"MaxClockSpeed");
	info.name=findString(object, L"Name");

	m_vecProcessor.append(info);
}

REGISTER_WMI_INSTANCE(QWmiProcessor)
