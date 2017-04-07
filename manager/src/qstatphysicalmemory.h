#ifndef QSTATPHYSICALMEMORY_H
#define QSTATPHYSICALMEMORY_H

#include <QObject>
#include <QVector>

#include "qabstractstat.h"
#include "extern.h"

#ifdef Q_OS_SOLARIS_SPARC_SUN4V
#include <picl.h>
#endif

class QStatPhysicalMemory : public QObject, public QAbstractStat
{
	Q_OBJECT

public:
	QStatPhysicalMemory(QObject *parent=0);
	~QStatPhysicalMemory();

	enum {Type=WMI_TYPE_PHYSICAL_MEMORY};

public:
	virtual int type(){ return Type; }
	virtual QVariant result() CONVERT_TO_VARIANT(m_vecMemory)

#ifdef Q_OS_SOLARIS
	void appendMemory(PhysicalMemory& memory);
#endif

protected:
	virtual void beferQuery();
	virtual void query();

#ifdef Q_OS_SOLARIS_SPARC_SUN4V
private:
	typedef struct _memory_module 
	{
		QString label;
		QString nac;

		QString modelName;
		QString mfgName;
		QString serialNumber;
		QString description;
		QString operationalStatus;
	}memory_module;

	typedef struct _memory_bank
	{
		int id;
		quint64 size;

		QVector<memory_module> modules;
	}memory_bank;

	typedef struct _memory_segment 
	{
		int interleaveFactor;
		quint64 size;

		QVector<memory_bank> banks;
	}memory_segment;

	typedef struct _memory_tree
	{
		QVector<memory_segment> segments;
	}memory_tree;

	typedef struct _socket_node 
	{
		QVector<struct _socket_node> children;

		char label[PICL_CLASSNAMELEN_MAX];

		memory_module* module;
	}socket_node;

private:
	int traversal_memory(int lvl, picl_nodehdl_t nodeh, QVector<memory_tree>& memories);
	int traversal_memory_segment(int lvl, picl_nodehdl_t nodeh, memory_tree& memory);
	int traversal_memory_bank(int lvl, picl_nodehdl_t nodeh, memory_segment& segment);
	int traversal_memory_module(int lvl, picl_nodehdl_t nodeh, memory_bank& bank);

	int traversal_memory_socket_node(int lvl, picl_nodehdl_t nodeh, const socket_node& node);

	int get_prop_value(const picl_prophdl_t& proph, const picl_propinfo_t& propinfo, QByteArray& buffer);
#endif

private:
	QVector<PhysicalMemory>	m_vecMemory;
};

#endif // QSTATPHYSICALMEMORY_H
