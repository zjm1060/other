#ifndef QMIBTOPOLOGY_H
#define QMIBTOPOLOGY_H

#include "qmibextern.h"
#include "qutildef.h"

class MIB_EXPORT QMibTolology : public QAbstractMib
{
public:
	QMibTolology();
	~QMibTolology();

public:
	enum {Type=UTIL_TYPE_TOPOLOGY};

public:
	virtual int type(){ return Type; }

protected:
	virtual void setResult(const QVariant& var);
	virtual QVariant getResult() CONVERT_TO_VARIANT(m_graph)

private:
	GraphTopo	m_graph;
};

#endif // QMIBTOPOLOGY_H
