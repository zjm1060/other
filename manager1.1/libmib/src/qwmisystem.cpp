#include "qwmisystem.h"

QWmiSystem::QWmiSystem(QObject *parent)
: QObject(parent)
{
}

QWmiSystem::~QWmiSystem()
{
}

void QWmiSystem::beferQuery() 
{
	m_vecSystem.clear(); 
}

QStringList QWmiSystem::query()
{
	QStringList listQuery;
	listQuery<<"SELECT BuildNumber, Caption, CSDVersion, Version, OSArchitecture FROM Win32_OperatingSystem";

	return listQuery;
}

void QWmiSystem::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	OperatingSystem system;

	system.buildNumber=findString(object, L"BuildNumber");
	system.caption=findString(object, L"Caption");
	system.CSDVersion=findString(object, L"CSDVersion");
	system.version=findString(object, L"Version");
	system.OSArchitecture=findString(object, L"OSArchitecture");

	m_vecSystem.append(system);
}

REGISTER_WMI_INSTANCE(QWmiSystem)
