#include "qwmicomputersystem.h"

QWmiComputerSystem::QWmiComputerSystem(QObject *parent)
: QObject(parent)
{
}

QWmiComputerSystem::~QWmiComputerSystem()
{
}

QStringList QWmiComputerSystem::query()
{
	QStringList listQuery;
	listQuery<<"SELECT Caption, Name, Domain, PartOfDomain, Workgroup, NumberOfProcessors, TotalPhysicalMemory FROM Win32_ComputerSystem"
		<<"SELECT Name FROM Win32_Processor";

	return listQuery;
}

void QWmiComputerSystem::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	HRESULT hr;

	switch (index)
	{
	case 0:
		{
			VARIANT var;

			m_system.name=findString(object, L"Name");

			VariantInit(&var);
			hr = object->Get(L"PartOfDomain", 0, &var, 0, 0);
			if (!FAILED(hr))
			{
				bool bPartOfDomain=true;

				if (var.vt != VT_NULL)
				{
					bPartOfDomain=V_BOOL(&var);
				}
				VariantClear(&var);

				VariantInit(&var);
				if (bPartOfDomain)
				{
					hr = object->Get(L"Workgroup", 0, &var, 0, 0);
				}
				else
				{
					hr = object->Get(L"Domain", 0, &var, 0, 0);
				}

				if (!FAILED(hr))
				{
					if (var.vt != VT_NULL)
					{
						m_system.workgroup=QString::fromLocal8Bit((char*)_bstr_t(V_BSTR(&var)));
					}
				}
			}
			VariantClear(&var);

			m_system.numberOfProcessors=findUInt32(object, L"NumberOfProcessors");
			m_system.totalPhysicalMemory=findUInt64(object, L"TotalPhysicalMemory");
		}
		break;
	case 1:
		{
			m_system.processor=findString(object, L"Name");
		}
		break;
	}
}

REGISTER_WMI_INSTANCE(QWmiComputerSystem)
