#include "qwmiperfprocessor.h"

QWmiPerfProcessor::QWmiPerfProcessor(QObject *parent)
: QObject(parent)
{
}

QWmiPerfProcessor::~QWmiPerfProcessor()
{
}

void QWmiPerfProcessor::beferQuery()
{
	m_processor.core.clear();
}

QStringList QWmiPerfProcessor::query()
{
	QStringList listQuery;
	listQuery<<"SELECT CurrentClockSpeed, CurrentVoltage, LoadPercentage FROM Win32_Processor"
		<<"SELECT * FROM Win32_PerfFormattedData_PerfOS_Processor";

	return listQuery;
}

void QWmiPerfProcessor::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);

	Q_ASSERT_NO_NULL(object);

	switch (index)
	{
	case 0:
		{
			m_processor.currentClockSpeed=findUInt32(object, L"CurrentClockSpeed");
			
			qint16 currentClockSpeed=findUInt16(object, L"CurrentVoltage");
			if (currentClockSpeed != -1)
			{
				m_processor.currentVoltage=(currentClockSpeed&0x7f)/10.0;
			}
			else
			{
				m_processor.currentClockSpeed=-1;
			}

			m_processor.status=findString(object, L"Status");
		}
		break;
	case 1:
		{
			PerfProcessorCore processor;

			processor.name=findString(object, L"Name");
			if (processor.name != "_Total")
			{
				processor.name="CPU"+processor.name;
			}
			processor.percentProcessorTime=findUInt64(object, L"PercentProcessorTime");

			m_processor.core.append(processor);
		}
		break;
	default:
		break;
	}
}

REGISTER_WMI_INSTANCE(QWmiPerfProcessor)
