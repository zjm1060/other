#include "qmibtopology.h"

QMibTolology::QMibTolology()
:QAbstractMib(-1)
{
	setModel(ManualModel);
}

QMibTolology::~QMibTolology()
{

}

void QMibTolology::setResult(const QVariant& var)
{
	if (var.isValid())
	{
		m_graph=var.value<GraphTopo>();
		
		emit changed();
	}
}

REGISTER_MIB_INSTANCE(QMibTolology)
