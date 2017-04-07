#ifndef QPANELFACTORY_H
#define QPANELFACTORY_H

#include <QObject>
#include <QMap>
#include <QList>

class QString;

#include "qutildef.h"
#include "qabstractpanel.h"

typedef QAbstractClassProducer<QAbstractPanel> QAbstractPanelProducer;
typedef QSharedObjectPtr<QAbstractPanelProducer> QAbstractPanelProducerPtr;

class QPanelFactory : public QObject
{
	Q_OBJECT

public:
	virtual ~QPanelFactory();

protected:
	QPanelFactory();

public:
	static QPanelFactory* instance();

public:
	void load();

public:
	void registerProduct(const QString& name, const QAbstractPanelProducerPtr& ptr);
	QList<QAbstractPanel*>	produces(int belong);

private:
	QMap<QString, QAbstractPanelProducerPtr>	m_mapProducts;

	QMap<int, QList<QAbstractPanel*> >	m_mapPanels;
};

template<typename T, class C>
class QRegisterAbstractPanel
{
public:
	QRegisterAbstractPanel(T t)
	{
		QPanelFactory::instance()->registerProduct(t, QAbstractPanelProducerPtr(new C));
	}
};

#define REGISTER_PANEL_CLASS(name)	\
	static QRegisterAbstractPanel<QString, QClassProducer<name, QAbstractPanelProducer> > static##name##Instance(#name);

#endif // QPANELFACTORY_H
