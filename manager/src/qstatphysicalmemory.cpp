#include "qstatphysicalmemory.h"

#include "define.h"
#include "common.h"

#ifdef Q_OS_SOLARIS_X86
#include <smbios.h>
#include <alloca.h>
#include <ctype.h>

#elif defined(Q_OS_SOLARIS_SPARC_SUN4V)
#include <stdlib.h>
#include <libintl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/termios.h>

#define	ROOT_LEVEL		0

#endif

#ifdef	Q_OS_SOLARIS_X86
extern "C" int doMemoryDevs(smbios_hdl_t *shp, const smbios_struct_t *sp, void* arg);
#endif

QStatPhysicalMemory::QStatPhysicalMemory(QObject *parent)
: QObject(parent)
{
}

QStatPhysicalMemory::~QStatPhysicalMemory()
{
}

void QStatPhysicalMemory::beferQuery() 
{
	m_vecMemory.clear(); 
}

void QStatPhysicalMemory::query()
{
#ifdef Q_OS_SOLARIS_X86
	smbios_hdl_t *shp;
	int err;

	if ((shp = smbios_open(NULL, SMB_VERSION, 0, &err)) == NULL)
	{
		qDebug("failed to open SMBIOS: %s\n", smbios_errmsg(err));
		return;
	}

	smbios_iter(shp, doMemoryDevs, this);
	smbios_close(shp);

#elif defined(Q_OS_SOLARIS_SPARC_SUN4V)
	int err;
	picl_nodehdl_t	rooth;
	int done;

	QVector<memory_tree> memories;

	err = picl_initialize();
	if (err != PICL_SUCCESS)
	{
		return;
	}

	//获取内存大小、个数信息
	do 
	{
		done = 1;
		err = picl_get_root(&rooth);
		if (err != PICL_SUCCESS)
		{
			break;
		}

		err = traversal_memory(ROOT_LEVEL, rooth, memories);
		if ((err == PICL_STALEHANDLE) || (err == PICL_INVALIDHANDLE))
		{
			done = 0;
		}
	} while (!done);

	//获取内存厂商、版本等信息
	socket_node root;
	strcpy(root.label, "/");
	root.module=NULL;
	for (int i=0; i<memories.size(); i++)
	{
		const memory_tree& tree=memories[i];
		for (int j=0; j<tree.segments.size(); j++)
		{
			const memory_segment& segment=tree.segments[j];

			for (int k=0; k<segment.banks.size(); k++)
			{
				const memory_bank& bank=segment.banks[k];

				for (int m=0; m<bank.modules.size(); m++)
				{
					const memory_module& module=bank.modules[m];
					
					QStringList lstLabel=module.nac.split(QChar('/'), QString::SkipEmptyParts);
					socket_node* ptr=&root;
					foreach (QString label, lstLabel)
					{
						bool bFind=false;
						for (int n=0; n<ptr->children.size(); n++)
						{
							const socket_node& node=ptr->children[n];
							if (label == node.label)
							{
								ptr=(socket_node*)&ptr->children[n];
								bFind=true;

								break;
							}
						}
						
						if (!bFind)
						{
							socket_node node;
							strcpy(node.label, label.toAscii());
							node.module=NULL;
							ptr->children.append(node);

							ptr=(socket_node*)&ptr->children.last();
						}
					}

					ptr->module=(memory_module*)&bank.modules[m];
				}
			}
		}
	}

	do 
	{
		done = 1;
		err = picl_get_root(&rooth);
		if (err != PICL_SUCCESS)
		{
			break;
		}

		err = traversal_memory_socket_node(ROOT_LEVEL, rooth, root);
		if ((err == PICL_STALEHANDLE) || (err == PICL_INVALIDHANDLE))
		{
			done = 0;
		}
	} while (!done);

	picl_shutdown();

	for (int i=0; i<memories.size(); i++)
	{
		const memory_tree& tree=memories[i];
		for (int j=0; j<tree.segments.size(); j++)
		{
			const memory_segment& segment=tree.segments[j];

			for (int k=0; k<segment.banks.size(); k++)
			{
				const memory_bank& bank=segment.banks[k];
				if (bank.modules.size() == 0)
				{
					continue;
				}

				PhysicalMemory memory;
				memory.capacity=bank.size/bank.modules.size();
				for (int m=0; m<bank.modules.size(); m++)
				{
					const memory_module& module=bank.modules[m];
					memory.name=QString("Physical Memory %1").arg(m_vecMemory.size()+1);

					memory.description=module.mfgName+" "+module.modelName+" "+module.description;
					memory.manufacturer=module.mfgName;
					memory.memoryType=module.description;
					memory.serialNumber=module.serialNumber;
					memory.tag=module.nac;
					memory.dataWidth=-1;
					memory.speed=-1;
					memory.version=module.modelName;

					m_vecMemory.append(memory);
				}
			}
		}
	}

#elif defined(Q_OS_SOLARIS_SPARC_SUN4U)
	FILE *pp, *pSub;
	char buf[256]; 

	QString strContent;
	QStringList lstSubContent;

	if( (pp = popen("/usr/sbin/prtfru -c", "r")) == NULL )
	{ 
		printf("popen() error!\n"); 
		return;
	} 

	while(fgets(buf, sizeof buf, pp)) 
	{
		strContent=QString::fromLocal8Bit(buf);
		if (strContent.isEmpty())
		{
			continue;
		}

		lstSubContent=strContent.split(QChar(':'), QString::SkipEmptyParts);
		if (lstSubContent.size() != 2 || lstSubContent[0] != "SEGMENT" || lstSubContent[1] != "SP")
		{
			continue;
		}

		PhysicalMemory memory;

		while(fgets(buf, sizeof buf, pp))
		{
			strContent=QString::fromLocal8Bit(buf);
			if (strContent.isEmpty())
			{
				continue;
			}

			lstSubContent=strContent.split(QChar(':'), QString::SkipEmptyParts);
			if (lstSubContent.size() < 2)
			{
				continue;
			}

			if (lstSubContent[0].contains("_Capacity"))
			{
				memory.capacity=StringToSize(lstSubContent[1]);
			}
			else if (lstSubContent[0].contains("_Fundamental_Memory_Type"))
			{
				memory.memoryType=lstSubContent[1];
			}
			else if (lstSubContent[0].contains("_Module_Data_Width"))
			{
				memory.dataWidth=lstSubContent[1].toInt(NULL, 16);
			}
			else if (lstSubContent[0].contains("_Access_Time"))
			{
				memory.speed=lstSubContent[1].toUInt();
			}
			else if (lstSubContent[0].contains("Vendor_Name"))
			{
				memory.manufacturer=lstSubContent[1];
			}
			else if (lstSubContent[0].contains("_Manufacturer_Part_No"))
			{
				memory.tag=lstSubContent[1];
			}
			else if (lstSubContent[0].contains("_Assembly_Serial_No"))
			{
				memory.serialNumber=lstSubContent[1];
			}
			else if (lstSubContent[0].contains("/frutree/chassis"))
			{
				appendMemory(memory);

				break;
			}
		}
	}

	pclose(pp);

#elif defined(Q_OS_AIX)
	FILE *pp, *pSub;
	char buf[256]; 
	char cmd[64];

	QString strContent;
	QStringList lstSubContent;

	if( (pp = popen("/usr/sbin/lsdev -Cc memory", "r")) == NULL )
	{ 
		printf("popen() error!\n"); 
		return;
	} 

	PhysicalMemory memory;

	while(fgets(buf, sizeof buf, pp)) 
	{
		strContent=QString::fromLocal8Bit(buf);
		if (strContent.isEmpty())
		{
			continue;
		}

		lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if (lstSubContent[0].contains("cache") || lstSubContent.size() < 3)
		{
			continue;
		}

		memory.name=lstSubContent[0];
		memory.description=lstSubContent[2];

		sprintf(cmd, "/usr/sbin/lsattr -El %s", memory.name.toLocal8Bit().data());
		if( (pSub = popen(cmd, "r")) != NULL )
		{ 
			while(fgets(buf, sizeof buf, pSub))
			{
				strContent=QString::fromLocal8Bit(buf);
				if (strContent.isEmpty())
				{
					continue;
				}

				lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (lstSubContent[0] == "goodsize" && lstSubContent.size() >=2)
				{
					memory.capacity=lstSubContent[1].toULongLong()*1024*1024;
				}
			}

			pclose(pSub);
		}

		memory.dataWidth=-1;
		memory.speed=-1;

		m_vecMemory.append(memory);
	}

	pclose(pp);
#endif
}

#ifdef Q_OS_SOLARIS
void QStatPhysicalMemory::appendMemory(PhysicalMemory& memory)
{
	memory.description=memory.name=QString("Physical Memory %1").arg(m_vecMemory.size()+1);

	m_vecMemory.append(memory);
}
#endif

#ifdef Q_OS_SOLARIS_X86
int doMemoryDevs(smbios_hdl_t *shp, const smbios_struct_t *sp, void* arg)
{
	if (arg == NULL)
	{
		return -1;
	}

	smbios_memdevice_t md;

	if (sp->smbstr_type == SMB_TYPE_MEMDEVICE && smbios_info_memdevice(shp, sp->smbstr_id, &md) != SMB_ERR)
	{
		if (md.smbmd_size > 0)
		{
			QStatPhysicalMemory* memories=(QStatPhysicalMemory*)arg;
			if (memories == NULL)
			{
				return -1;
			}

			PhysicalMemory memory;

 			memory.capacity=md.smbmd_size;
			memory.dataWidth=(quint16)md.smbmd_dwidth;

			memory.memoryType=QString::fromLocal8Bit(smbios_memdevice_type_desc(md.smbmd_type));
			memory.speed=md.smbmd_speed;

			memory.tag=QString::fromLocal8Bit(md.smbmd_dloc);

			memories->appendMemory(memory);
		}
	}

	return 0;
}

#elif defined(Q_OS_SOLARIS_SPARC_SUN4V)
int QStatPhysicalMemory::traversal_memory(int lvl, picl_nodehdl_t nodeh, QVector<memory_tree>& memories)
{
	int err;

	char piclclass[PICL_CLASSNAMELEN_MAX];
	strlcpy(piclclass, "memory", PICL_CLASSNAMELEN_MAX);

	char classval[PICL_PROPNAMELEN_MAX];

	memory_tree memory;

	picl_nodehdl_t	chdh;

	err = picl_get_propval_by_name(nodeh, PICL_PROP_CLASSNAME, classval, sizeof(classval));

	if (strcasecmp(piclclass, classval) == 0)
	{
		++lvl;

		err = traversal_memory_segment(lvl, nodeh, memory);

		if (err == PICL_SUCCESS)
		{
			memories.append(memory);
		}

		return err;
	}

	for (err = picl_get_propval_by_name(nodeh, PICL_PROP_CHILD, &chdh, sizeof (picl_nodehdl_t)); err != PICL_PROPNOTFOUND;
		err = picl_get_propval_by_name(chdh, PICL_PROP_PEER, &chdh,sizeof (picl_nodehdl_t)))
	{

		if (err != PICL_SUCCESS) 
		{
			return (err);
		}

		err = traversal_memory(lvl, chdh, memories);

		if (err != PICL_SUCCESS)
		{
			return (err);
		}
	}

	return (PICL_SUCCESS);
}

int QStatPhysicalMemory::traversal_memory_segment(int lvl, picl_nodehdl_t nodeh, memory_tree& memory)
{
	char piclclass[PICL_CLASSNAMELEN_MAX];
	strlcpy(piclclass, "memory-segment", PICL_CLASSNAMELEN_MAX);

	char classval[PICL_PROPNAMELEN_MAX];
	int err;

	picl_nodehdl_t	chdh;

	memory_segment segment;

	/*
	* First get the class name of the node to compare with piclclass
	*/
	err = picl_get_propval_by_name(nodeh, PICL_PROP_CLASSNAME, classval, sizeof(classval));

	if (strcasecmp(piclclass, classval) == 0) 
	{
		picl_prophdl_t	proph;
		picl_propinfo_t	propinfo;

		QByteArray buffer;

		for (err = picl_get_first_prop(nodeh, &proph); err == PICL_SUCCESS; err = picl_get_next_prop(proph, &proph))
		{
			err = picl_get_propinfo(proph, &propinfo);
			if (err != PICL_SUCCESS) 
			{
				return (err);
			}

			if (strcasecmp(propinfo.name, "InterleaveFactor") == 0)
			{
				if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
				{
					return err;
				}

				segment.interleaveFactor=buffer.toUInt();
			}
			else if (strcasecmp(propinfo.name, "Size") == 0)
			{
				if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
				{
					return err;
				}

				segment.size=buffer.toULongLong();
			}
		}

		++lvl;

		err = traversal_memory_bank(lvl, nodeh, segment);
		if (err == PICL_SUCCESS)
		{
			memory.segments.append(segment);
		}

		return err;
	}

	for (err = picl_get_propval_by_name(nodeh, PICL_PROP_CHILD, &chdh, sizeof (picl_nodehdl_t)); err != PICL_PROPNOTFOUND;
		err = picl_get_propval_by_name(chdh, PICL_PROP_PEER, &chdh,sizeof (picl_nodehdl_t)))
	{

		if (err != PICL_SUCCESS) 
		{
			return (err);
		}

		err = traversal_memory_segment(lvl, chdh, memory);

		if (err != PICL_SUCCESS)
		{
			return (err);
		}
	}

	memory.segments.append(segment);

	return (PICL_SUCCESS);
}

int QStatPhysicalMemory::traversal_memory_bank(int lvl, picl_nodehdl_t nodeh, memory_segment& segment)
{
	char piclclass[PICL_CLASSNAMELEN_MAX];
	strlcpy(piclclass, "memory-bank", PICL_CLASSNAMELEN_MAX);

	char classval[PICL_PROPNAMELEN_MAX];
	int err;

	picl_nodehdl_t	chdh;

	memory_bank bank;

	/*
	* First get the class name of the node to compare with piclclass
	*/
	err = picl_get_propval_by_name(nodeh, PICL_PROP_CLASSNAME, classval, sizeof(classval));

	if (strcasecmp(piclclass, classval) == 0) 
	{
		picl_prophdl_t	proph;
		picl_propinfo_t	propinfo;

		QByteArray buffer;

		for (err = picl_get_first_prop(nodeh, &proph); err == PICL_SUCCESS; err = picl_get_next_prop(proph, &proph))
		{
			err = picl_get_propinfo(proph, &propinfo);
			if (err != PICL_SUCCESS) 
			{
				return (err);
			}

			if (strcasecmp(propinfo.name, "ID") == 0)
			{
				if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
				{
					return err;
				}

				bank.id=buffer.toUInt();
			}
			else if (strcasecmp(propinfo.name, "Size") == 0)
			{
				if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
				{
					return err;
				}

				bank.size=buffer.toULongLong();
			}
		}

		++lvl;

		err = traversal_memory_module(lvl, nodeh, bank);
		if (err == PICL_SUCCESS)
		{
			segment.banks.append(bank);
		}

		return err;
	}

	for (err = picl_get_propval_by_name(nodeh, PICL_PROP_CHILD, &chdh, sizeof (picl_nodehdl_t)); err != PICL_PROPNOTFOUND;
		err = picl_get_propval_by_name(chdh, PICL_PROP_PEER, &chdh,sizeof (picl_nodehdl_t)))
	{

		if (err != PICL_SUCCESS) 
		{
			return (err);
		}

		err = traversal_memory_bank(lvl, chdh, segment);

		if (err != PICL_SUCCESS)
		{
			return (err);
		}
	}

	segment.banks.append(bank);

	return (PICL_SUCCESS);
}

int QStatPhysicalMemory::traversal_memory_module(int lvl, picl_nodehdl_t nodeh, memory_bank& bank)
{
	char piclclass[PICL_CLASSNAMELEN_MAX];
	strlcpy(piclclass, "memory-module", PICL_CLASSNAMELEN_MAX);
	
	char classval[PICL_PROPNAMELEN_MAX];
	int err;

	picl_nodehdl_t	chdh;

	memory_module module;

	/*
	* First get the class name of the node to compare with piclclass
	*/
	err = picl_get_propval_by_name(nodeh, PICL_PROP_CLASSNAME, classval, sizeof(classval));

	if (strcasecmp(piclclass, classval) == 0) 
	{
		picl_prophdl_t	proph;
		picl_propinfo_t	propinfo;

		QByteArray buffer;

		for (err = picl_get_first_prop(nodeh, &proph); err == PICL_SUCCESS; err = picl_get_next_prop(proph, &proph))
		{
			err = picl_get_propinfo(proph, &propinfo);
			if (err != PICL_SUCCESS) 
			{
				return (err);
			}

			if (strcasecmp(propinfo.name, "Label") == 0)
			{
				if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
				{
					return err;
				}

				module.label=buffer;
			}
			else if (strcasecmp(propinfo.name, "nac") == 0)
			{
				if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
				{
					return err;
				}

				module.nac=buffer;
			}
		}

		bank.modules.append(module);

		++lvl;
	}

	for (err = picl_get_propval_by_name(nodeh, PICL_PROP_CHILD, &chdh, sizeof (picl_nodehdl_t)); err != PICL_PROPNOTFOUND;
		err = picl_get_propval_by_name(chdh, PICL_PROP_PEER, &chdh,sizeof (picl_nodehdl_t)))
	{

		if (err != PICL_SUCCESS) 
		{
			return (err);
		}

		err = traversal_memory_module(lvl, chdh, bank);

		if (err != PICL_SUCCESS)
		{
			return (err);
		}
	}

	return (PICL_SUCCESS);
}

int QStatPhysicalMemory::traversal_memory_socket_node(int lvl, picl_nodehdl_t nodeh, const socket_node& node)
{
	int err;

	char classval[PICL_PROPNAMELEN_MAX];

	picl_nodehdl_t	chdh;

	err = picl_get_propval_by_name(nodeh, PICL_PROP_NAME, classval, sizeof(classval));

	foreach (socket_node temp, node.children)
	{
		if (strcasecmp(temp.label, classval) == 0)
		{
			++lvl;

			if (temp.children.isEmpty())
			{
				picl_prophdl_t	proph;
				picl_propinfo_t	propinfo;

				QByteArray buffer;

				printf("temp.label=%s\n", temp.label);

				for (err = picl_get_first_prop(nodeh, &proph); err == PICL_SUCCESS; err = picl_get_next_prop(proph, &proph))
				{
					err = picl_get_propinfo(proph, &propinfo);
					if (err != PICL_SUCCESS) 
					{
						return (err);
					}

					if (strcasecmp(propinfo.name, "ModelName") == 0)
					{
						if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
						{
							return err;
						}

						temp.module->modelName=buffer;
					}
					else if (strcasecmp(propinfo.name, "MfgName") == 0)
					{
						if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
						{
							return err;
						}

						temp.module->mfgName=buffer;
					}
					else if (strcasecmp(propinfo.name, "SerialNumber") == 0)
					{
						if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
						{
							return err;
						}

						temp.module->serialNumber=buffer;
					}
					else if (strcasecmp(propinfo.name, "Description") == 0)
					{
						if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
						{
							return err;
						}

						temp.module->description=buffer;
					}
					else if (strcasecmp(propinfo.name, "OperationalStatus") == 0)
					{
						if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
						{
							return err;
						}

						temp.module->operationalStatus=buffer;
					}
				}

				return PICL_SUCCESS;
			}
			else
			{
				return traversal_memory_socket_node(lvl, nodeh, temp);
			}
		}
	}

	for (err = picl_get_propval_by_name(nodeh, PICL_PROP_CHILD, &chdh, sizeof (picl_nodehdl_t)); err != PICL_PROPNOTFOUND;
		err = picl_get_propval_by_name(chdh, PICL_PROP_PEER, &chdh,sizeof (picl_nodehdl_t)))
	{

		if (err != PICL_SUCCESS) 
		{
			return (err);
		}

		err = traversal_memory_socket_node(lvl, chdh, node);

		if (err != PICL_SUCCESS)
		{
			return (err);
		}
	}

	return (PICL_SUCCESS);
}

int QStatPhysicalMemory::get_prop_value(const picl_prophdl_t& proph, const picl_propinfo_t& propinfo, QByteArray& buffer)
{
	int err;

	if (!(propinfo.accessmode & PICL_READ)) 
	{
		return (PICL_SUCCESS);
	}

	if (propinfo.type == PICL_PTYPE_VOID)
	{
		return (PICL_SUCCESS);
	}

	buffer.resize(propinfo.size);

	err = picl_get_propval(proph, buffer.data(), propinfo.size);
	if ((err == PICL_INVALIDHANDLE) || (err == PICL_STALEHANDLE) || (err == PICL_NORESPONSE))
	{
		return (err);
	}
	else if (err != PICL_SUCCESS) 
	{
		return (PICL_SUCCESS);
	}

	QString strValue;
	switch (propinfo.type)
	{
	case PICL_PTYPE_CHARSTRING:
		{
			strValue=buffer;
		}
		break;
	case PICL_PTYPE_INT:
		switch (propinfo.size)
		{
		case sizeof (int8_t):
			strValue=QString::number(*(int8_t *)buffer.data());
			break;
		case sizeof (int16_t):
			strValue=QString::number(*(int16_t *)buffer.data());
			break;
		case sizeof (int32_t):
			strValue=QString::number(*(int32_t *)buffer.data());
			break;
		case sizeof (int64_t):
			strValue=QString::number((qlonglong)*(int64_t *)buffer.data());
			break;
		default:
			return (PICL_FAILURE);
		}
		break;
	case PICL_PTYPE_UNSIGNED_INT:
		switch (propinfo.size)
		{
		case sizeof (uint8_t):
			strValue=QString::number(*(uint8_t *)buffer.data());
			break;
		case sizeof (uint16_t):
			strValue=QString::number(*(uint16_t *)buffer.data());
			break;
		case sizeof (uint32_t):
			strValue=QString::number(*(uint32_t *)buffer.data());
			break;
		case sizeof (uint64_t):
			strValue=QString::number((qulonglong)*(uint64_t *)buffer.data());
			break;
		default:
			return (PICL_FAILURE);
		}
		break;
	}

	buffer=strValue.toAscii();

	return -1;
}
#endif

