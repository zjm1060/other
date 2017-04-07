#ifndef QFRAMENETWORK_H
#define QFRAMENETWORK_H

#include <QFrame>
#include <QStandardItemModel>

#include "ui_network.h"
#include "qutildef.h"

using namespace Ui;

class QFrameNetwork : public QFrame, public Ui::FrameNetwork
{
	Q_OBJECT

public:
	QFrameNetwork(QWidget *parent);
	~QFrameNetwork();

public:
	void setNetworkAdapter(const QVector<NetworkAdapter>& adapter);

private slots:
	void currentIndexChanged(int index);

private:
	void showNetworkAdapter();

private:
	QVector<NetworkAdapter>		m_vecAdapter;

private:
	QStandardItemModel*		m_pModelIPAddr;
	QStandardItemModel*		m_pModelGateway;
	QStandardItemModel*		m_pModelDNS;
};

#endif // QFRAMENETWORK_H
