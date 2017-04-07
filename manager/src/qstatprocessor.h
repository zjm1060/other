#ifndef QSTATPROCESSOR_H
#define QSTATPROCESSOR_H

#include <QObject>
#include <QVector>

#include <QByteArray>

#include "qabstractstat.h"
#include "extern.h"

#ifdef Q_OS_SOLARIS
#include <picl.h>
#endif

class QStatProcessor : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatProcessor(QObject *parent=0);
	~QStatProcessor();

	enum {Type=WMI_TYPE_PROCESSOR};

#ifdef Q_OS_AIX
public:
	static int processors;	//处理器个数
#endif

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecProcessor)

protected:
	virtual void beferQuery();
	virtual void query();

#ifdef Q_OS_SOLARIS
private:
	quint16 findAddrWidth();		//获取地址宽度
	quint16 bitness(const char *isaname);

	void findProcessorProp(const char* className, ProcessorInfo& processor, int instance);
	int traversal_tree_by_class(int lvl, picl_nodehdl_t nodeh, char *piclclass, int instance, ProcessorInfo& processor);
	int traversal_proplist(int lvl, picl_nodehdl_t nodeh, int instance, ProcessorInfo& processor);
	int get_prop_value(const picl_prophdl_t& proph, const picl_propinfo_t& propinfo, QByteArray& buffer);
#endif

private:
	QVector<ProcessorInfo>	m_vecProcessor;
};

#endif // QSTATPROCESSOR_H
