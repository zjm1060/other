#include "qframenetwork.h"

QFrameNetwork::QFrameNetwork(QWidget *parent)
	: QFrame(parent)
{
	setupUi(this);

	QStringList iplabels;
	iplabels<<tr("IP Address")<<tr("Subnet Mask");
	m_pModelIPAddr=new QStandardItemModel(0, iplabels.size(), this);
	m_pModelIPAddr->setHorizontalHeaderLabels(iplabels);

	treeViewIPAddr->setRootIsDecorated(false);
	treeViewIPAddr->setAlternatingRowColors(true);
	treeViewIPAddr->setModel(m_pModelIPAddr);

	treeViewIPAddr->setColumnWidth(0, 150);

	QStringList gatewayLabels;
	gatewayLabels<<tr("Gateway")<<tr("Metric");
	m_pModelGateway=new QStandardItemModel(0, gatewayLabels.size(), this);
	m_pModelGateway->setHorizontalHeaderLabels(gatewayLabels);

	treeViewGateway->setRootIsDecorated(false);
	treeViewGateway->setAlternatingRowColors(true);
	treeViewGateway->setModel(m_pModelGateway);

	treeViewGateway->setColumnWidth(0, 150);

	QStringList dnsLabels;
	dnsLabels<<tr("DNS");
	m_pModelDNS=new QStandardItemModel(0, dnsLabels.size(), this);
	m_pModelDNS->setHorizontalHeaderLabels(dnsLabels);

	listViewDNS->setAlternatingRowColors(true);
	listViewDNS->setModel(m_pModelDNS);

	connect(comboBoxNIC, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
}

QFrameNetwork::~QFrameNetwork()
{

}

void QFrameNetwork::setNetworkAdapter(QVector<NetworkAdapter>& adapter)
{
	m_vecAdapter=adapter;

	showNetworkAdapter();
}

void QFrameNetwork::showNetworkAdapter()
{
	comboBoxNIC->clear();

	for (int i=0; i<m_vecAdapter.size(); i++)
	{
		comboBoxNIC->addItem(QIcon(":images/networkadapter.png"), m_vecAdapter[i].description, i);
	}
}

void QFrameNetwork::currentIndexChanged(int index)
{
	m_pModelIPAddr->setRowCount(0);
	m_pModelGateway->setRowCount(0);
	m_pModelDNS->setRowCount(0);

	if (index < 0)
	{
		return;
	}

	int nIndex=comboBoxNIC->itemData(index).toInt();
	if (nIndex >= m_vecAdapter.size())
	{
		return;
	}

	const NetworkAdapter& adapter=m_vecAdapter[nIndex];
	for (int i=0; i<adapter.IPAddress.size(); i++)
	{
		m_pModelIPAddr->insertRow(i);

		m_pModelIPAddr->itemFromIndex(m_pModelIPAddr->index(i, 0))->setEditable(false);
		m_pModelIPAddr->setData(m_pModelIPAddr->index(i, 0), adapter.IPAddress[i]);

		if (i<adapter.IPSubnet.size())
		{
			m_pModelIPAddr->itemFromIndex(m_pModelIPAddr->index(i, 1))->setEditable(false);
			m_pModelIPAddr->setData(m_pModelIPAddr->index(i, 1), adapter.IPSubnet[i]);
		}
	}

	for (int i=0; i<adapter.defaultIPGateway.size(); i++)
	{
		m_pModelGateway->insertRow(i);

		m_pModelGateway->itemFromIndex(m_pModelGateway->index(i, 0))->setEditable(false);
		m_pModelGateway->setData(m_pModelGateway->index(i, 0), adapter.defaultIPGateway[i]);

		if (i<adapter.gatewayCostMetric.size())
		{
			m_pModelGateway->itemFromIndex(m_pModelGateway->index(i, 1))->setEditable(false);
			m_pModelGateway->setData(m_pModelGateway->index(i, 1), adapter.gatewayCostMetric[i]);
		}
	}

	//添加DNS服务器列表
	for (int i=0; i<adapter.DNSServer.size(); i++)
	{
		m_pModelDNS->insertRow(i);

		m_pModelDNS->itemFromIndex(m_pModelDNS->index(i, 0))->setEditable(false);
		m_pModelDNS->setData(m_pModelDNS->index(i, 0), adapter.DNSServer[i]);
	}

	labelMAC->setText("MAC: "+adapter.MACAddress);
}
