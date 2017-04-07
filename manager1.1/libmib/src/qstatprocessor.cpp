#include "qstatprocessor.h"

#ifdef Q_OS_SOLARIS
#include <kstat.h>
#include <sys/sysinfo.h>
#include <string.h>

#include <sys/types.h>
#include <sys/systeminfo.h>
#include <sys/utsname.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <libintl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/termios.h>

#define	ROOT_LEVEL		0
#define	MATCHING_CLASSVAL(x, y)	((x == NULL) || (strcasecmp(x, y) == 0))

#elif defined(Q_OS_AIX)
#include <fcntl.h>
#include <sys/systemcfg.h>
#include <nlist.h>

#endif

#ifdef Q_OS_AIX
int QStatProcessor::processors=0;
#endif

QStatProcessor::QStatProcessor(QObject *parent)
: QObject(parent)
{
}

QStatProcessor::~QStatProcessor()
{
}

void QStatProcessor::beferQuery() 
{
	m_vecProcessor.clear(); 
}

void QStatProcessor::query()
{
#ifdef Q_OS_SOLARIS
	kstat_ctl_t *kc;
	kstat_t *ksp;
	kstat_named_t *knp;

	if ((kc = kstat_open()) == NULL)
	{
		return;
	}

	ProcessorInfo info;
	info.os=OS_SOLARIS;

	int nInstance;

	for (ksp = kc->kc_chain; ksp!=NULL; ksp = ksp->ks_next)
	{
		if ((strcmp(ksp->ks_module, "cpu_info")) != 0) continue;
		if (kstat_read(kc, ksp, NULL) == -1)
		{
			continue;
		}

		nInstance=ksp->ks_instance;

		knp=(kstat_named_t*)kstat_data_lookup(ksp, "chip_id");

		info.deviceID=QString(tr("proc")+"%1").arg(knp->value.ui32);
		info.core=1;

		bool bFind=false;
		for (int i=0; i<m_vecProcessor.size(); i++)
		{
			if (m_vecProcessor[i].deviceID == info.deviceID)
			{
				bFind=true;
				m_vecProcessor[i].core++;

				break;
			}
		}

		if (bFind)
		{
			continue;
		}

		knp=(kstat_named_t*)kstat_data_lookup(ksp, "state");
		QString strStatus=QString::fromLocal8Bit(knp->value.str.addr.__pad);
		if (strStatus == "on-line")
		{
			info.configManager=0;
		}
		else
		{
			info.configManager=11;
		}
		
		knp=(kstat_named_t*)kstat_data_lookup(ksp, "brand");
		if (knp != NULL)
		{
			info.name=QString::fromLocal8Bit(knp->value.str.addr.ptr);
		}

		knp=(kstat_named_t*)kstat_data_lookup(ksp, "vendor_id");
		if (knp != NULL)
		{
			info.manufacturer=QString::fromLocal8Bit(knp->value.str.addr.ptr);
		}

		knp=(kstat_named_t*)kstat_data_lookup(ksp, "clock_MHz");
		info.maxClockSpeed=knp->value.ui32;

		knp=(kstat_named_t*)kstat_data_lookup(ksp, "implementation");
		info.description=QString::fromLocal8Bit(knp->value.str.addr.ptr);

		info.dataWidth=info.addressWidth=findAddrWidth();

		info.L2CacheSize=-1;
		info.L2CacheSpeed=-1;
		info.externalClock=-1;

		findProcessorProp("cpu", info, nInstance);

		m_vecProcessor.append(info);
	}

	kstat_close(kc);

#elif defined(Q_OS_AIX)

	struct nlist info;

	int fd;
	int arch, imp;

	char name[]="_system_configuration";

	info.n_name = name;

	fd = open("/dev/kmem",O_RDONLY);
	knlist(&info,1,sizeof(struct nlist));
	lseek(fd,info.n_value,0);
	read(fd,&_system_configuration,sizeof(_system_configuration));

	ProcessorInfo processor;
	processor.os=OS_AIX;

	processor.addressWidth=(quint16)_system_configuration.width;
	processor.dataWidth=_system_configuration.kernel&(1<<31) == 0? 32:64;
	processor.L2CacheSize=(quint32)_system_configuration.L2_cache_size;
	processor.L2CacheSpeed=-1;

	arch=_system_configuration.architecture;
	imp = _system_configuration.implementation;
	switch (arch)
	{
	case POWER_RS:
		{
			processor.name="POWER_RS";
			processor.description="Power Classic architecture";
		}
		break;
	case POWER_PC:
		{
			processor.name="POWER_PC";
			processor.description="Power PC architecture";

			switch (imp)
			{
			case POWER_RS1:
				{
					processor.name+="_POWER_RS1";
					processor.description+=",RS1 class CPU";
				}
				break;
			case POWER_RSC:
				{
					processor.name+="_POWER_RSC";
					processor.description+=",RSC class CPU";
				}
				break;
			case POWER_RS2:
				{
					processor.name+="_POWER_RS2";
					processor.description+=",RS2 class CPU";
				}
				break;
			case POWER_601:
				{
					processor.name+="_POWER601";
					processor.description+=",601 class CPU";
				}
				break;
			case POWER_603:
				{
					processor.name+="_POWER603";
					processor.description+=",603 class CPU";
				}
				break;
			case POWER_604:
				{
					processor.name="_POWER604";
					processor.description+=",604 class CPU";
				}
				break;
			case POWER_620:
				{
					processor.name+="_POWER620";
					processor.description+=",620 class CPU";
				}
				break;
			case POWER_630:
				{
					processor.name+="_POWER630";
					processor.description+=",630 class CPU";
				}
				break;
			case POWER_A35:
				{
					processor.name="_POWER_A35";
					processor.description+=",A35 class CPU";
				}
				break;
			case POWER_RS64II:
				{
					processor.name+="_POWER_RS64II";
					processor.description+=",RS64-II class CPU";
				}
				break;
			case POWER_RS64III:
				{
					processor.name+="_POWER_RS64III";
					processor.description+=",RS64-III class CPU";
				}
				break;
			case POWER_4:
				{
					processor.name+="_POWER4";
					processor.description+=",4 class CPU";
				}
				break;
			case POWER_MPC7450:
				{
					processor.name+="_POWER_MPC7450";
					processor.description+=",MPC7450 class CPU";
				}
				break;
			case POWER_5:
				{
					processor.name+="_POWER5";
					processor.description+=",5 class CPU";
				}
				break;
			case POWER_6:
				{
					processor.name+="_POWER6";
					processor.description+=",6 class CPU";
				}
				break;
			case POWER_7:
				{
					processor.name+="_POWER7";
					processor.description+=",7 class CPU";
				}
				break;
			default:
				break;
			}
		}
		break;
	case IA64:
		{
			processor.name="Intel IA64";
			processor.description="Intel IA64 architecture";

			switch (imp)
			{
			case IA64_M1:
				{
					processor.name+=" M1";
					processor.description+=",IA64 M1 class CPU (Itanium)";
				}
				break;
			case IA64_M2:
				{
					processor.name+=" M2";
					processor.description+=",IA64 M2 class CPU";
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	processor.maxClockSpeed=-1;
	processor.externalClock=-1;

	int nNum=_system_configuration.ncpus;
	processor.core=1;
	if ((_system_configuration.smt_status & 0x7) == 7 && _system_configuration.smt_threads>0)
	{
		nNum=(nNum+_system_configuration.smt_threads-1)/_system_configuration.smt_threads;
		processor.core=_system_configuration.smt_threads;
	}

// 	perfstat_cpu_total_t *cputotal;
// 	int ret;
// 
// 	cputotal = new perfstat_cpu_total_t;
// 	memset(cputotal, 0, sizeof(perfstat_cpu_total_t));
// 	ret = perfstat_cpu_total(NULL, cputotal, sizeof(perfstat_cpu_total_t), 1);
// 	if (ret != -1)
// 	{
// 		processor.maxClockSpeed=cputotal->processorHZ/1000;
// 	}
// 
// 	for (int i=0; i<nNum; i++)
// 	{
// 		processor.deviceID=QString("proc%1").arg(i*processor.core);
// 
// 		m_vecProcessor.append(processor);
// 	}
// 
// 	SAFE_DELETE_CLASS(cputotal);
// 
// 	close(fd);

	FILE *pp, *pSub;
	char buf[256]; 
	char cmd[64];

	QString strContent;
	QStringList lstSubContent;

	if( (pp = popen("/usr/sbin/lsdev -Cc processor", "r")) == NULL )
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

		lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if (lstSubContent.size() < 2)
		{
			continue;
		}

		processor.deviceID=lstSubContent[0];
		processor.status=lstSubContent[1];

		sprintf(cmd, "/usr/sbin/lsattr -El %s", processor.deviceID.toLocal8Bit().data());
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
				if (lstSubContent[0] == "frequency" && lstSubContent.size() >=2)
				{
					processor.maxClockSpeed=lstSubContent[1].toULongLong()/1000000;
				}
			}

			pclose(pSub);
		}

		m_vecProcessor.append(processor);
	}

	pclose(pp);

	processors=m_vecProcessor.size();

#endif
}

#ifdef Q_OS_SOLARIS
quint16 QStatProcessor::findAddrWidth()
{
	char buffer[20];

	int size=sysinfo(SI_ARCHITECTURE_64, buffer, _countof(buffer));
	if (size == -1)
	{
		size=sysinfo(SI_ARCHITECTURE_32, buffer, _countof(buffer));
	}

	if (size != -1)
	{
		return bitness(buffer);
	}
	
	return -1;
}

quint16 QStatProcessor::bitness(const char *isaname)
{
	if (strcmp(isaname, "sparc") == 0 || strcmp(isaname, "i386") == 0)
		return (32);
	 
	if (strcmp(isaname, "sparcv9") == 0 || strcmp(isaname, "amd64") == 0)
		return (64);
	
	return (-1);
}

void QStatProcessor::findProcessorProp(const char* className, ProcessorInfo& processor, int instance)
{
	int err;
	picl_nodehdl_t	rooth;
	int done;
	char piclclass[PICL_CLASSNAMELEN_MAX];
	strlcpy(piclclass, className, PICL_CLASSNAMELEN_MAX);

	err = picl_initialize();
	if (err != PICL_SUCCESS)
	{
		return;
	}

	do 
	{
		done = 1;
		err = picl_get_root(&rooth);
	 	if (err != PICL_SUCCESS)
		{
 			break;
	 	}
		
 		err = traversal_tree_by_class(ROOT_LEVEL, rooth, piclclass, instance, processor);
 		if ((err == PICL_STALEHANDLE) || (err == PICL_INVALIDHANDLE))
		{
 			done = 0;
		}
	} while (!done);
		 
	picl_shutdown();
}

int QStatProcessor::traversal_tree_by_class(int lvl, picl_nodehdl_t nodeh, char *piclclass, int instance, ProcessorInfo& processor)
{
	picl_nodehdl_t	chdh;
	char classval[PICL_PROPNAMELEN_MAX];
	int err;
	 
	/*
	* First get the class name of the node to compare with piclclass
	*/
	err = picl_get_propval_by_name(nodeh, PICL_PROP_CLASSNAME, classval, sizeof(classval));
 	if (err != PICL_SUCCESS) 
	{
 		return (err);
 	}
		 
 	if (MATCHING_CLASSVAL(piclclass, classval)) 
	{		 
		err = traversal_proplist(lvl, nodeh, instance, processor);
		if (err != PICL_SUCCESS)
		{
			return (err);
		}

	 	++lvl;
	}
	 
	for (err = picl_get_propval_by_name(nodeh, PICL_PROP_CHILD, &chdh, sizeof (picl_nodehdl_t)); err != PICL_PROPNOTFOUND;
		err = picl_get_propval_by_name(chdh, PICL_PROP_PEER, &chdh,sizeof (picl_nodehdl_t)))
	{
	 
		if (err != PICL_SUCCESS) 
		{
			return (err);
		}
	 
 		err = traversal_tree_by_class(lvl, chdh, piclclass, instance, processor);

		if (err != PICL_SUCCESS)
		{
			return (err);
		}
	}

 	return (PICL_SUCCESS);
}

int QStatProcessor::traversal_proplist(int lvl, picl_nodehdl_t nodeh, int instance, ProcessorInfo& processor)
{
	int err;
 	picl_prophdl_t	proph;
 	picl_propinfo_t	propinfo;

	Q_UNUSED(lvl);

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

			if (buffer.toInt() != instance)
			{
				return PICL_SUCCESS; 
			}
		}

		if (strcasecmp(propinfo.name, "shared-l2-tlb-4k-size") == 0)
		{
			if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
			{
				return err;
			}

			processor.L2CacheSize=buffer.toUInt()*4*1024;
		}
		else if (strcasecmp(propinfo.name, "l2-cache-size") == 0)
		{
			if ((err=get_prop_value(proph, propinfo, buffer)) != -1)
			{
				return err;
			}

			processor.L2CacheSize=buffer.toUInt();
		}
	}

	return (PICL_SUCCESS);
}

int QStatProcessor::get_prop_value(const picl_prophdl_t& proph, const picl_propinfo_t& propinfo, QByteArray& buffer)
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
			strValue=QString::number(*(int64_t *)buffer.data());
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
			strValue=QString::number(*(uint64_t *)buffer.data());
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

REGISTER_STAT_INSTANCE(QStatProcessor)

