#include "qpanelfactory.h"

#include <QString>

QPanelFactory::QPanelFactory()
{
	
}

QPanelFactory::~QPanelFactory()
{
	m_mapProducts.clear();
}

QPanelFactory* QPanelFactory::instance()
{
	static QPanelFactory* pInstance=new QPanelFactory;

	return pInstance;
}

void QPanelFactory::registerProduct(const QString& name, const QAbstractPanelProducerPtr& ptr)
{
	Q_ASSERT_NO_NULL(ptr);

	if (!m_mapProducts.contains(name))
	{
		m_mapProducts.insert(name, ptr);
	}
}

QList<QAbstractPanel*>	QPanelFactory::produces(int belong)
{
	QList<QAbstractPanel*> panels;

	if (m_mapPanels.contains(belong))
	{
		panels=m_mapPanels[belong];
	}

	return panels;
}

void QPanelFactory::load()
{
	QMapIterator<QString, QAbstractPanelProducerPtr> it(m_mapProducts);
	while (it.hasNext())
	{
		it.next();

		QAbstractPanelProducerPtr ptr=it.value();
		QAbstractPanel* panel=ptr->produce();
		if (panel != NULL)
		{
			QList<QAbstractPanel*>& panels=m_mapPanels[panel->belongsTo()];
			int nIndex=0;
			foreach (QAbstractPanel* pPanel, panels)
			{
				if (pPanel->sequence() > panel->sequence())
				{
					break;
				}

				nIndex++;
			}

			panels.insert(nIndex, panel);
		}
	}
}
