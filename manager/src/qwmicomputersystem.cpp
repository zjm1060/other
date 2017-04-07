#include "qwmicomputersystem.h"

#include "define.h"

QWmiComputerSystem::QWmiComputerSystem(QObject *parent)
: QObject(parent)
{
}

QWmiComputerSystem::~QWmiComputerSystem()
{
}

void QWmiComputerSystem::beferQuery() 
{
	m_vecSystem.clear(); 
}

QStringList QWmiComputerSystem::query()
{
	QStringList listQuery;
	listQuery<<"SELECT Caption, Name, Domain, PartOfDomain, Workgroup FROM Win32_ComputerSystem";

	return listQuery;
}

void QWmiComputerSystem::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	HRESULT hr;

	ComputerSystem system;
	VARIANT var;

	system.name=findString(object, L"Name");

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
				system.workgroup=QString::fromLocal8Bit((char*)_bstr_t(V_BSTR(&var)));
			}
		}
	}
	VariantClear(&var);

	m_vecSystem.append(system);
}


