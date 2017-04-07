#include "qwminetworkadapter.h"

QWmiNetworkAdapter::QWmiNetworkAdapter(QObject *parent)
: QObject(parent)
{
}

QWmiNetworkAdapter::~QWmiNetworkAdapter()
{
}

void QWmiNetworkAdapter::beferQuery() 
{
	m_vecAdapter.clear(); 
}

QStringList QWmiNetworkAdapter::query()
{
	QStringList listQuery;

	listQuery<<"SELECT DefaultIPGateway, DHCPEnabled, Description, DNSServerSearchOrder, GatewayCostMetric, IPAddress, IPSubnet, MACAddress	\
			   FROM Win32_NetworkAdapterConfiguration";

	return listQuery;
}

void QWmiNetworkAdapter::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	NetworkAdapter adapter;

	adapter.MACAddress=findString(object, L"MACAddress");
	adapter.description=findString(object, L"Description");

	adapter.defaultIPGateway=findStringArray(object, L"DefaultIPGateway");
	adapter.DNSServer=findStringArray(object, L"DNSServerSearchOrder");
	adapter.gatewayCostMetric=findUInt16Array(object, L"GatewayCostMetric");
	adapter.IPSubnet=findStringArray(object, L"IPSubnet");
	adapter.IPAddress=findStringArray(object, L"IPAddress");

	int nIndex=-1;
	for (int i=0; i<m_vecAdapter.size(); i++)
	{
		if (adapter.MACAddress == m_vecAdapter[i].MACAddress)
		{
			nIndex=i;
			break;
		}
	}

	if (nIndex == -1)
	{
		m_vecAdapter.append(adapter);
	}
	else
	{
		bool bDHCPEnabled=findBoolean(object, L"DHCPEnabled");
		if (!bDHCPEnabled)
		{
			m_vecAdapter[nIndex]=adapter;
		}
	}
}

REGISTER_WMI_INSTANCE(QWmiNetworkAdapter)
