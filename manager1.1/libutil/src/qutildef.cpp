#include "qutildef.h"

#include "common.h"
#include "qlogger.h"
#include "qimessage.h"

#include <QApplication>
#include <QStyle>
#include <QDir>
#include <QPixmapCache>

#include <math.h>

#ifdef Q_OS_SOLARIS
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <sys/sockio.h>

#define MAX_NUM_INTERFACES   16
#endif

bool operator==(const Server& source, const Server& target)
{
	return (source.addr==target.addr && source.netmask==target.netmask && source.port==target.port);
}

bool operator==(const ServerInfo& source, const ServerInfo& target)
{
	QList<QString> keys=source.servers.keys();
	foreach (QString key, keys)
	{
		if (key.isEmpty())
		{
			continue;
		}

		if (target.servers.contains(key))
		{
			if (key == "FF:FF:FF:FF:FF:FF" && source.servers[key] != target.servers[key])
			{
				return false;
			}

			return true;
		}
	}

	return false;
}

bool operator!=(const ServerInfo& source, const ServerInfo& target)
{
	if (source.hostName != target.hostName || source.usrName != target.usrName)
	{
		return true;
	}

	if (source.count != target.count || source.servers != target.servers)
	{
		return true;
	}

	return false;
}

QDataStream& operator<<(QDataStream& out, const ServerInfo& info)
{
	out<<info.hostName<<info.usrName<<info.home;

	QList<QString> keys=info.servers.keys();
	out<<keys.size();
	foreach (QString key, keys)
	{
		out<<key;

		const QVector<Server>& servers=info.servers[key];
		out<<servers.size();
		foreach (Server server, servers)
		{
			out<<server.addr<<server.netmask<<server.port;
		}
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, ServerInfo& info)
{
	int nNumKeys, nNumServers;

	in>>info.hostName>>info.usrName>>info.home>>nNumKeys;

	info.count=0;

	QString key;
	Server server;
	for (int i=0; i<nNumKeys; i++)
	{
		in>>key>>nNumServers;
		QVector<Server> servers;
		for (int j=0; j<nNumServers; j++)
		{
			in>>server.addr>>server.netmask>>server.port;

			servers.append(server);
		}

		info.servers.insert(key, servers);

		info.count+=nNumServers;
	}

	return in;
}

QDataStream& operator<<(QDataStream& out, const ProcessorInfo& processor)
{
	out<<processor.os;

	switch (processor.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			out<<processor.configManager<<processor.core<<processor.addressWidth<<processor.dataWidth
				<<processor.description<<processor.deviceID<<processor.externalClock<<processor.L2CacheSize
				<<processor.L2CacheSpeed<<processor.manufacturer<<processor.maxClockSpeed<<processor.name;
		}
		break;
	case OS_AIX:
		{
			out<<processor.status<<processor.core<<processor.addressWidth<<processor.dataWidth
				<<processor.description<<processor.deviceID<<processor.externalClock<<processor.L2CacheSize
				<<processor.L2CacheSpeed<<processor.manufacturer<<processor.maxClockSpeed<<processor.name;
		}
		break;
	default:
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Unknow OS!!!"));
		break;
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, ProcessorInfo& processor)
{
	in>>processor.os;

	switch (processor.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			in>>processor.configManager>>processor.core>>processor.addressWidth>>processor.dataWidth
				>>processor.description>>processor.deviceID>>processor.externalClock>>processor.L2CacheSize
				>>processor.L2CacheSpeed>>processor.manufacturer>>processor.maxClockSpeed>>processor.name;
		}
		break;
	case OS_AIX:
		{
			in>>processor.status>>processor.core>>processor.addressWidth>>processor.dataWidth
				>>processor.description>>processor.deviceID>>processor.externalClock>>processor.L2CacheSize
				>>processor.L2CacheSpeed>>processor.manufacturer>>processor.maxClockSpeed>>processor.name;
		}
		break;
	default:
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Unknow OS!!!"));
		break;
	}

	return in;
}

QDataStream& operator<<(QDataStream& out, const PerfProcessorCore& core)
{
	out<<core.name<<core.percentProcessorTime;

	return out;
}

QDataStream& operator>>(QDataStream& in, PerfProcessorCore& core)
{
	in>>core.name>>core.percentProcessorTime;

	return in;
}

QDataStream& operator<<(QDataStream& out, const PerfProcessor& perf)
{
	out<<perf.currentClockSpeed<<perf.currentVoltage<<perf.status<<perf.core;

	return out;
}

QDataStream& operator>>(QDataStream& in, PerfProcessor& perf)
{
	in>>perf.currentClockSpeed>>perf.currentVoltage>>perf.status>>perf.core;

	return in;
}

QDataStream& operator<<(QDataStream& out, const OperatingSystem& system)
{
	out<<system.caption<<system.version<<system.buildNumber<<system.CSDVersion<<system.OSArchitecture;

	return out;
}

QDataStream& operator>>(QDataStream& in, OperatingSystem& system)
{
	in>>system.caption>>system.version>>system.buildNumber>>system.CSDVersion>>system.OSArchitecture;

	return in;
}

QDataStream& operator<<(QDataStream& out, const ComputerSystem& system)
{
	out<<system.numberOfProcessors<<system.totalPhysicalMemory<<system.name<<system.workgroup<<system.processor;

	return out;
}

QDataStream& operator>>(QDataStream& in, ComputerSystem& system)
{
	in>>system.numberOfProcessors>>system.totalPhysicalMemory>>system.name>>system.workgroup>>system.processor;
	
	return in;
}

bool operator!=(const NetworkAdapter& source, const NetworkAdapter& target)
{
	if (source.MACAddress != target.MACAddress)
	{
		return true;
	}

	if (source.description != target.description)
	{
		return true;
	}

	if (source.IPAddress != target.IPAddress)
	{
		return true;
	}

	if (source.IPSubnet != target.IPSubnet)
	{
		return true;
	}

	if (source.gatewayCostMetric != target.gatewayCostMetric)
	{
		return true;
	}

	if (source.defaultIPGateway != target.defaultIPGateway)
	{
		return true;
	}

	if (source.DNSServer != target.DNSServer)
	{
		return true;
	}

	return false;
}

bool operator==(const NetworkAdapter& source, const NetworkAdapter& target)
{
	if (source.MACAddress != target.MACAddress)
	{
		return false;
	}

	if (source.description != target.description)
	{
		return false;
	}

	if (source.IPAddress != target.IPAddress)
	{
		return false;
	}

	if (source.IPSubnet != target.IPSubnet)
	{
		return false;
	}

	if (source.gatewayCostMetric != target.gatewayCostMetric)
	{
		return false;
	}

	if (source.defaultIPGateway != target.defaultIPGateway)
	{
		return false;
	}

	if (source.DNSServer != target.DNSServer)
	{
		return false;
	}

	return true;
}

QDataStream& operator<<(QDataStream& out, const NetworkAdapter& adapter)
{
	out<<adapter.description<<adapter.defaultIPGateway<<adapter.DNSServer<<adapter.gatewayCostMetric
		<<adapter.IPAddress<<adapter.IPSubnet<<adapter.MACAddress;

	return out;
}

QDataStream& operator>>(QDataStream& in, NetworkAdapter& adapter)
{
	in>>adapter.description>>adapter.defaultIPGateway>>adapter.DNSServer>>adapter.gatewayCostMetric
		>>adapter.IPAddress>>adapter.IPSubnet>>adapter.MACAddress;

	return in;
}

#if defined(Q_OS_AIX) || defined(Q_OS_SOLARIS)
QVector<NetworkAdapter> findNetworkAdapter()
{
	QVector<NetworkAdapter> adapters;

#ifdef Q_OS_SOLARIS
	int fd, interface, retn = 0;

	struct ifreq buf[MAX_NUM_INTERFACES]; 
	struct ifconf ifc;
	struct arpreq arp;

	char* ptr;
	char mac[32];

	fd=socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		return adapters;
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;
	if (!ioctl(fd, SIOCGIFCONF, (char*)&ifc))
	{
		interface=ifc.ifc_len/sizeof(struct ifreq);

		while (interface-- > 0)
		{
			NetworkAdapter adapter;

			adapter.description=QString::fromLocal8Bit(buf[interface].ifr_name);

			if (!(ioctl(fd, SIOCGIFFLAGS, (char*)&buf[interface])))
			{
				if (buf[interface].ifr_flags & IFF_PROMISC)
				{
					retn++;
				}
			}
			else
			{
				continue;
			}

			if (!(ioctl(fd, SIOCGIFADDR, (char*)&buf[interface])))
			{
				adapter.IPAddress.append(
					QString::fromLocal8Bit(inet_ntoa(((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr)));
			}

			arp.arp_pa.sa_family = AF_INET;
			arp.arp_ha.sa_family = AF_INET;
			((struct sockaddr_in*)&arp.arp_pa)->sin_addr.s_addr=((struct sockaddr_in *)&buf[interface].ifr_addr)->sin_addr.s_addr;
			if (ioctl (fd, SIOCGARP, &arp) >= 0)
			{
				ptr=(char*)arp.arp_ha.sa_data;
				sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", 
					*(unsigned char*)ptr, *(unsigned char*)(ptr+1), *(unsigned char*)(ptr+2), 
					*(unsigned char*)(ptr+3), *(unsigned char*)(ptr+4), *(unsigned char*)(ptr+5));

				adapter.MACAddress=QString::fromLocal8Bit(mac);
			}

			if (!(ioctl(fd, SIOCGIFNETMASK, (char*)&buf[interface])))
			{
				adapter.IPSubnet.append(
					QString::fromLocal8Bit(inet_ntoa(((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr)));
			}

			adapters.append(adapter);
		}
	}

	close (fd);

#elif defined(Q_OS_AIX)

	FILE *pp, *pSub;
	char buf[256]; 
	char cmd[64];

	QString strContent;
	QStringList lstSubContent;
	QStringList lstAddr;

	if( (pp = popen("/usr/sbin/lsdev -Cc if", "r")) == NULL )
	{ 
		printf("popen() error!\n"); 
		return adapters;
	} 

	while(fgets(buf, sizeof buf, pp)) 
	{
		strContent=QString::fromLocal8Bit(buf);
		if (strContent.isEmpty())
		{
			continue;
		}

		lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if (lstSubContent.size() == 0)
		{
			continue;
		}

		NetworkAdapter adapter;

		adapter.description=lstSubContent[0];

		sprintf(cmd, "/usr/sbin/lsattr -El %s", adapter.description.toLocal8Bit().data());
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
				if (lstSubContent[0] == "alias4" && lstSubContent.size() >=2)
				{
					if (lstSubContent[1].contains(QRegExp("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
					{
						lstAddr=lstSubContent[1].split(QChar(','));
						adapter.IPAddress.append(lstAddr[0]);
						if (lstAddr.size() == 2)
						{
							adapter.IPSubnet.append(lstAddr[1]);
						}
						else
						{
							adapter.IPSubnet.append("");
						}
					}
				}
				else if (lstSubContent[0] == "netaddr" && lstSubContent.size() >=2)
				{
					if (lstSubContent[1].contains(QRegExp("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
					{
						adapter.IPAddress.append(lstSubContent[1]);
					}
				}
				else if (lstSubContent[0] == "netmask" && lstSubContent.size() >=2)
				{
					if (lstSubContent[1].contains(QRegExp("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
					{
						adapter.IPSubnet.append(lstSubContent[1]);
					}
				}
			}

			pclose(pSub);
		}

		int gateway=-1;
		int cost=-1;
		int nameserver=-1;
		sprintf(cmd, "/usr/sbin/mktcpip -S %s", adapter.description.toLocal8Bit().data());
		if( (pSub = popen(cmd, "r")) != NULL )
		{
			while(fgets(buf, sizeof buf, pSub))
			{
				strContent=QString::fromLocal8Bit(buf);
				if (strContent.isEmpty())
				{
					continue;
				}

				lstSubContent=strContent.split(":");
				if (strContent.contains("gateway") || strContent.contains("nameserv"))
				{
					for (int i=0; i<lstSubContent.size(); i++)
					{
						if (lstSubContent[i] == "gateway")
						{
							gateway=i;
						}
						else if (lstSubContent[i] == "cost")
						{
							cost=i;
						}
						else if (lstSubContent[i] == "nameserv")
						{
							nameserver=i;
						}
					}
				}
				else
				{
					if (gateway != -1 && lstSubContent.size() > gateway)
					{
						if (!lstSubContent[gateway].isEmpty())
						{
							adapter.defaultIPGateway.append(lstSubContent[gateway]);
						}
					}

					if (cost != -1 && lstSubContent.size() > cost)
					{
						if (!lstSubContent[cost].isEmpty())
						{
							adapter.gatewayCostMetric.append(lstSubContent[cost].toUShort());
						}
					}

					if (nameserver != -1 && lstSubContent.size() > nameserver)
					{
						if (!lstSubContent[nameserver].isEmpty())
						{
							adapter.DNSServer.append(lstSubContent[nameserver]);
						}
					}
				}
			}

			pclose(pSub);
		}

		if (adapter.description.startsWith("en") || adapter.description.startsWith("et"))
		{
			int num=adapter.description.right(adapter.description.length()-2).toInt();

			bool bFind=false;
			for (int i=0; i<adapters.size(); i++)
			{
				if (adapters[i].description == QString("en%1").arg(num) || 
					adapters[i].description == QString("et%1").arg(num))
				{
					bFind=true;

					adapter.MACAddress=adapters[i].MACAddress;
					break;
				}
			}

			if (!bFind)
			{
				sprintf(cmd, "/usr/sbin/lscfg -vl ent%d", num);
				if( (pSub = popen(cmd, "r")) != NULL )
				{
					while(fgets(buf, sizeof buf, pSub))
					{
						strContent=QString::fromLocal8Bit(buf).trimmed();
						if (strContent.isEmpty())
						{
							continue;
						}

						strContent.replace(QRegExp("[.]{2,}"), " ");
						lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
						if (lstSubContent[0] == "Network" && lstSubContent[1] == "Address" && lstSubContent.size() >=3)
						{
							if (!lstSubContent[2].isEmpty())
							{
								adapter.MACAddress=lstSubContent[2].left(2);
								for (int i=2; i<lstSubContent[2].size(); i+=2)
								{
									adapter.MACAddress+=":"+lstSubContent[2].mid(i, 2);
								}
							}
						}
					}

					pclose(pSub);
				}
			}
		}

		if (adapter.MACAddress.isEmpty())
		{
			continue;
		}

		adapters.append(adapter);
	}

	pclose(pp);
#endif

	return adapters;
}
#endif

QDataStream& operator<<(QDataStream& out, const SystemProcess& process)
{
	out<<process.usrName<<process.executablePath<<process.name<<process.processId
		<<process.sessionId<<process.workingSetSize<<process.percentProcessorTime;

	return out;
}

QDataStream& operator>>(QDataStream& in, SystemProcess& process)
{
	in>>process.usrName>>process.executablePath>>process.name>>process.processId
		>>process.sessionId>>process.workingSetSize>>process.percentProcessorTime;

	return in;
}

QDataStream& operator<<(QDataStream& out, const FileInfo& info)
{
	out<<info.type<<info.name<<info.size<<info.isReadable<<info.isWritable<<info.isExecutable<<info.lastModified;

	return out;
}

QDataStream& operator>>(QDataStream& in, FileInfo& info)
{
	in>>info.type>>info.name>>info.size>>info.isReadable>>info.isWritable>>info.isExecutable>>info.lastModified;

	return in;
}

bool operator==(const FileInfo& source, const FileInfo& target)
{
	if (source.name == target.name && source.size == target.size && source.lastModified == target.lastModified)
	{
		return true;
	}

	return false;
}

bool operator!=(const FileInfo& source, const FileInfo& target)
{
	return !(source == target);
}

QDataStream& operator<<(QDataStream& out, const DiskDrive& disk)
{
	out<<disk.os;

	switch (disk.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			out<<disk.configManager<<disk.description<<disk.deviceID<<disk.interfaceType
				<<disk.manufacturer<<disk.partitions<<disk.size<<disk.mediaType
				<<disk.model<<disk.numberOfMediaSupported<<disk.SCSIBus<<disk.SCSILogicalUnit
				<<disk.SCSITargetId<<disk.SCSIPort;
		}
		break;
	case OS_AIX:
		{
			out<<disk.status<<disk.serialNumber<<disk.position<<disk.description<<disk.deviceID<<disk.interfaceType
				<<disk.manufacturer<<disk.size<<disk.model;
		}
		break;
	default:
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Unknow OS!!!"));
		break;
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, DiskDrive& disk)
{
	in>>disk.os;

	switch (disk.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			in>>disk.configManager>>disk.description>>disk.deviceID>>disk.interfaceType
				>>disk.manufacturer>>disk.partitions>>disk.size>>disk.mediaType
				>>disk.model>>disk.numberOfMediaSupported>>disk.SCSIBus>>disk.SCSILogicalUnit
				>>disk.SCSITargetId>>disk.SCSIPort;
		}
		break;
	case OS_AIX:
		{
			in>>disk.status>>disk.serialNumber>>disk.position>>disk.description>>disk.deviceID>>disk.interfaceType
				>>disk.manufacturer>>disk.size>>disk.model;
		}
		break;
	default:
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Unknow OS!!!"));
		break;
	}

	return in;
}

QDataStream& operator<<(QDataStream& out, const CDROMDrive& disk)
{
	out<<disk.os;

	switch (disk.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			out<<disk.configManager<<disk.description<<disk.deviceID<<disk.drive
				<<disk.manufacturer<<disk.mediaType<<disk.numberOfMediaSupported
				<<disk.SCSIBus<<disk.SCSILogicalUnit<<disk.SCSITargetId<<disk.SCSIPort
				<<disk.size<<disk.transferRate<<disk.volumeName<<disk.volumeSerialNumber<<disk.name;
		}
		break;
	case OS_AIX:
		{
			out<<disk.status<<disk.position<<disk.description<<disk.deviceID<<disk.manufacturer<<disk.transferRate
				<<disk.volumeName<<disk.volumeSerialNumber<<disk.name;
		}
		break;
	default:
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Unknow OS!!!"));
		break;
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, CDROMDrive& disk)
{
	in>>disk.os;

	switch (disk.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			in>>disk.configManager>>disk.description>>disk.deviceID>>disk.drive
				>>disk.manufacturer>>disk.mediaType>>disk.numberOfMediaSupported
				>>disk.SCSIBus>>disk.SCSILogicalUnit>>disk.SCSITargetId>>disk.SCSIPort
				>>disk.size>>disk.transferRate>>disk.volumeName>>disk.volumeSerialNumber>>disk.name;
		}
		break;
	case OS_AIX:
		{
			in>>disk.status>>disk.position>>disk.description>>disk.deviceID>>disk.manufacturer>>disk.transferRate
				>>disk.volumeName>>disk.volumeSerialNumber>>disk.name;
		}
		break;
	default:
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Unknow OS!!!"));
		break;
	}

	return in;
}

QDataStream& operator<<(QDataStream& out, const LogicalDisk& disk)
{
	out<<disk.description<<disk.deviceID<<disk.driveType<<disk.driveTypeDesc
		<<disk.fileSystem<<disk.freeSpace<<disk.size;

	return out;
}

QDataStream& operator>>(QDataStream& in, LogicalDisk& disk)
{
	in>>disk.description>>disk.deviceID>>disk.driveType>>disk.driveTypeDesc
		>>disk.fileSystem>>disk.freeSpace>>disk.size;

	return in;
}

QDataStream& operator<<(QDataStream& out, const PhysicalMemory& memory)
{
	out<<memory.capacity<<memory.dataWidth<<memory.description<<memory.manufacturer
		<<memory.memoryType<<memory.name<<memory.serialNumber<<memory.speed
		<<memory.tag<<memory.version;

	return out;
}

QDataStream& operator>>(QDataStream& in, PhysicalMemory& memory)
{
	in>>memory.capacity>>memory.dataWidth>>memory.description>>memory.manufacturer
		>>memory.memoryType>>memory.name>>memory.serialNumber>>memory.speed
		>>memory.tag>>memory.version;

	return in;
}

QDataStream& operator<<(QDataStream& out, const SystemMemory& memory)
{
	out<<memory.datetime<<memory.freePhysicalMemory<<memory.freeVirtualMemory<<memory.totalVisibleMemorySize
		<<memory.totalVirtualMemorySize<<memory.freeSpaceInPagingFiles<<memory.sizeStoredInPagingFiles
		<<memory.sizeUsageInPagingFiles<<memory.sizeUsageInPagingFilesPeak
		<<memory.systemCache<<memory.poolNonpaged<<memory.poolPaged;

	return out;
}

QDataStream& operator>>(QDataStream& in, SystemMemory& memory)
{
	in>>memory.datetime>>memory.freePhysicalMemory>>memory.freeVirtualMemory>>memory.totalVisibleMemorySize
		>>memory.totalVirtualMemorySize>>memory.freeSpaceInPagingFiles>>memory.sizeStoredInPagingFiles
		>>memory.sizeUsageInPagingFiles>>memory.sizeUsageInPagingFilesPeak
		>>memory.systemCache>>memory.poolNonpaged>>memory.poolPaged;

	return in;
}

QDataStream& operator<<(QDataStream& out, const GraphNodeInterface& face)
{
	out<<face.ifIndex<<face.desc<<face.type<<face.speed<<face.physAddress<<face.operStatus<<face.addresses;

	return out;
}

QDataStream& operator>>(QDataStream& in, GraphNodeInterface& face)
{
	in>>face.ifIndex>>face.desc>>face.type>>face.speed>>face.physAddress>>face.operStatus>>face.addresses;

	return in;
}

QDataStream& operator<<(QDataStream& out, const GraphTopoNode& node)
{
	out<<node.uid<<node.type<<node.name<<node.oid<<node.desc<<node.services<<node.ifNumber<<node.ipForwarding
		<<node.interfaces<<node.peers;

	return out;
}

QDataStream& operator>>(QDataStream& in, GraphTopoNode& node)
{
	in>>node.uid>>node.type>>node.name>>node.oid>>node.desc>>node.services>>node.ifNumber>>node.ipForwarding
		>>node.interfaces>>node.peers;

	return in;
}

QDataStream& operator<<(QDataStream& out, const GraphTopo& graph)
{
	out<<graph.root<<graph.nodes;

	return out;
}

QDataStream& operator>>(QDataStream& in, GraphTopo& graph)
{
	in>>graph.root>>graph.nodes;

	return in;
}

FileInfo entryInfo(const QFileInfo& info, bool isDrive)
{
	FileInfo fileInfo;

	fileInfo.type=isDrive? Drive:entryInfoFileType(info);
	fileInfo.name=isDrive? info.filePath():info.fileName();
	fileInfo.size=info.isFile()? info.size():0;
	fileInfo.isReadable=info.isReadable();
	fileInfo.isWritable=info.isWritable();
	fileInfo.isExecutable=info.isExecutable();
	fileInfo.lastModified=info.lastModified();

	return fileInfo;
}

int entryInfoFileType(const QFileInfo& info)
{
	int nFileType=File;;

	if (info.isDir())
	{
		if (!info.isHidden())
		{
			nFileType=Dir;
		}
		else
		{
			nFileType=HideDir;
		}
	}
	else if (info.isFile())
	{
		if (!info.isHidden())
		{
			nFileType=File;
		}
		else
		{
			nFileType=HideFile;
		}
	}

	if (info.isSymLink())
	{
		if (!info.isHidden())
		{
			nFileType|=SymLink;
		}
		else
		{
			nFileType|=HideSymLink;
		}
	}

	return nFileType;
}

QString fileType(const QFileInfo& info)
{
	QString strType;
	if (info.isSymLink())
	{
		strType=QObject::tr("Symbol Link");
	}
	else if (info.isDir())
	{
		strType=QObject::tr("File Folder");
	}
	else if (info.isExecutable())
	{
		strType=QObject::tr("Executable");
	}
	else
	{
		strType=info.suffix()+" "+QObject::tr("File");
	}

	return strType;
}

QString fileType(const FileInfo& info)
{
	QString strType;

	switch (info.type)
	{
	case Drive:
		{
			strType=QObject::tr("Drive");
		}
		break;
	case Dir:
	case HideDir:
		{
			strType=QObject::tr("File Folder");
		}
		break;
	case File:
	case HideFile:
		{
			if (info.isExecutable)
			{
				strType=QObject::tr("Executable");
			}
			else
			{
				strType=fileSuffix(info.name)+" "+QObject::tr("File");
			}
		}
		break;
	case SymLinkDir:
	case SymLinkHideDir:
	case SymLinkFile:
	case SymLinkHideFile:
		{
			strType=QObject::tr("Symbol Link");
		}
		break;
	default:
		break;
	}

	return strType;
}

bool isFile(int type)
{
	if (type == File || type == HideFile || (type & SymLink) != 0 )
	{
		return true;
	}

	return false;
}

bool isDir(int type)
{
	if (type == Dir || type == HideDir)
	{
		return true;
	}

	return false;
}

bool deleteDir(const QString& path)
{
	bool bSuccess=true;

	QDir dir(path);
	if (dir.exists())
	{
		QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot);
		foreach (QFileInfo info, list)
		{
			if (info.isSymLink() || info.isFile())
			{
				bSuccess&=QFile::remove(info.absoluteFilePath());
			}
			else if (info.isDir())
			{
				bSuccess&=deleteDir(info.absoluteFilePath());
			}
		}

		bSuccess&=dir.rmdir(path);
	}

	return bSuccess;
}

void entryInfoList(const QFileInfoList& list, QVector<FileInfo>& files, bool isDrive)
{
	foreach (QFileInfo info, list)
	{
		files.append(entryInfo(info, isDrive));
	}
}

#define FILE_PACKET_SIZE	(16*1024)
#define FILE_PACKET_HIGH_SIZE	(100*1024*1024)

int readContents(QFile* file, QByteArray& buffer, bool highRate)
{
	Q_ASSERT_NO_NULL(file);

	int nStatus=QueryFileReadFail;

	int nPacketSize=(highRate? FILE_PACKET_HIGH_SIZE:FILE_PACKET_SIZE);

	buffer.resize(nPacketSize);
	int nSize=file->read(buffer.data(), nPacketSize);
	if (nSize != -1)
	{
		if (nSize != nPacketSize)
		{
			buffer.resize(nSize);
		}

		if (nSize == 0)
		{
			nStatus=QueryFileReadFinish;
		}
		else
		{
			nStatus=QueryFileReadSuccess;
		}
	}
	else
	{
		buffer.clear();
	}

	return nStatus;
}

QIcon fileIcon(int type)
{
	QStyle *style = QApplication::style();
	QIcon icon;

	switch (type)
	{
	case Drive:
		{
			icon=style->standardIcon(QStyle::SP_DriveHDIcon);
		}
		break;
	case Dir:
	case HideDir:
		{
			icon=style->standardIcon(QStyle::SP_DirIcon);
		}
		break;
	case File:
	case HideFile:
		{
			icon=style->standardIcon(QStyle::SP_FileIcon);
		}
		break;
	case SymLinkDir:
	case SymLinkHideDir:
		{
			icon=style->standardIcon(QStyle::SP_DirLinkIcon);
		}
		break;
	case SymLinkFile:
	case SymLinkHideFile:
		{
			icon=style->standardIcon(QStyle::SP_FileLinkIcon);
		}
		break;
	case CDDVDDrive:
		{
			icon=style->standardIcon(QStyle::SP_DriveDVDIcon);
		}
		break;
	default:
		break;;
	}

	return icon;
}

#ifdef Q_WS_WIN
#ifndef SHGFI_ADDOVERLAYS
#define SHGFI_ADDOVERLAYS 0x000000020
#endif
#endif

#ifdef Q_WS_WIN
#ifndef Q_OS_WINCE

QImage qt_fromWinHBITMAP(HDC hdc, HBITMAP bitmap, int w, int h)
{
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = w;
	bmi.bmiHeader.biHeight      = -h;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage   = w * h * 4;

	QImage image(w, h, QImage::Format_ARGB32_Premultiplied);
	if (image.isNull())
		return image;

	// Get bitmap bits
	uchar *data = (uchar *) qMalloc(bmi.bmiHeader.biSizeImage);

	if (GetDIBits(hdc, bitmap, 0, h, data, &bmi, DIB_RGB_COLORS)) {
		// Create image and copy data into image.
		for (int y=0; y<h; ++y) {
			void *dest = (void *) image.scanLine(y);
			void *src = data + y * image.bytesPerLine();
			memcpy(dest, src, image.bytesPerLine());
		}
	} else {
		qWarning("qt_fromWinHBITMAP(), failed to get bitmap bits");
	}
	qFree(data);

	return image;
}

QPixmap convertHIconToPixmap( const HICON icon)
{
	bool foundAlpha = false;
	HDC screenDevice = GetDC(0);
	HDC hdc = CreateCompatibleDC(screenDevice);
	ReleaseDC(0, screenDevice);

	ICONINFO iconinfo;
	bool result = GetIconInfo(icon, &iconinfo); //x and y Hotspot describes the icon center
	if (!result)
		qWarning("convertHIconToPixmap(), failed to GetIconInfo()");

	int w = iconinfo.xHotspot * 2;
	int h = iconinfo.yHotspot * 2;

	BITMAPINFOHEADER bitmapInfo;
	bitmapInfo.biSize        = sizeof(BITMAPINFOHEADER);
	bitmapInfo.biWidth       = w;
	bitmapInfo.biHeight      = h;
	bitmapInfo.biPlanes      = 1;
	bitmapInfo.biBitCount    = 32;
	bitmapInfo.biCompression = BI_RGB;
	bitmapInfo.biSizeImage   = 0;
	bitmapInfo.biXPelsPerMeter = 0;
	bitmapInfo.biYPelsPerMeter = 0;
	bitmapInfo.biClrUsed       = 0;
	bitmapInfo.biClrImportant  = 0;
	DWORD* bits;

	HBITMAP winBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bitmapInfo, DIB_RGB_COLORS, (VOID**)&bits, NULL, 0);
	HGDIOBJ oldhdc = (HBITMAP)SelectObject(hdc, winBitmap);
	DrawIconEx( hdc, 0, 0, icon, iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 0, 0, DI_NORMAL);
	QImage image = qt_fromWinHBITMAP(hdc, winBitmap, w, h);

	for (int y = 0 ; y < h && !foundAlpha ; y++) {
		QRgb *scanLine= reinterpret_cast<QRgb *>(image.scanLine(y));
		for (int x = 0; x < w ; x++) {
			if (qAlpha(scanLine[x]) != 0) {
				foundAlpha = true;
				break;
			}
		}
	}
	if (!foundAlpha) {
		//If no alpha was found, we use the mask to set alpha values
		DrawIconEx( hdc, 0, 0, icon, w, h, 0, 0, DI_MASK);
		QImage mask = qt_fromWinHBITMAP(hdc, winBitmap, w, h);

		for (int y = 0 ; y < h ; y++){
			QRgb *scanlineImage = reinterpret_cast<QRgb *>(image.scanLine(y));
			QRgb *scanlineMask = mask.isNull() ? 0 : reinterpret_cast<QRgb *>(mask.scanLine(y));
			for (int x = 0; x < w ; x++){
				if (scanlineMask && qRed(scanlineMask[x]) != 0)
					scanlineImage[x] = 0; //mask out this pixel
				else
					scanlineImage[x] |= 0xff000000; // set the alpha channel to 255
			}
		}
	}
	//dispose resources created by iconinfo call
	DeleteObject(iconinfo.hbmMask);
	DeleteObject(iconinfo.hbmColor);

	SelectObject(hdc, oldhdc); //restore state
	DeleteObject(winBitmap);
	DeleteDC(hdc);
	return QPixmap::fromImage(image);
}
#else //ifndef Q_OS_WINCE
QPixmap convertHIconToPixmap( const HICON icon, bool large)
{
	HDC screenDevice = GetDC(0);
	HDC hdc = CreateCompatibleDC(screenDevice);
	ReleaseDC(0, screenDevice);

	int size = large ? 64 : 32;

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth       = size;
	bmi.bmiHeader.biHeight      = -size;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage   = size*size*4;

	uchar* bits;

	HBITMAP winBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**) &bits, 0, 0);
	if (winBitmap )
		memset(bits, 0xff, size*size*4);
	if (!winBitmap) {
		qWarning("convertHIconToPixmap(), failed to CreateDIBSection()");
		return QPixmap();
	}

	HGDIOBJ oldhdc = (HBITMAP)SelectObject(hdc, winBitmap);
	if (!DrawIconEx( hdc, 0, 0, icon, size, size, 0, 0, DI_NORMAL))
		qWarning("convertHIconToPixmap(), failed to DrawIcon()");

	uint mask = 0xff000000;
	// Create image and copy data into image.
	QImage image(size, size, QImage::Format_ARGB32);

	if (!image.isNull()) { // failed to alloc?
		int bytes_per_line = size * sizeof(QRgb);
		for (int y=0; y<size; ++y) {
			QRgb *dest = (QRgb *) image.scanLine(y);
			const QRgb *src = (const QRgb *) (bits + y * bytes_per_line);
			for (int x=0; x<size; ++x) {
				dest[x] = src[x];
			}
		}
	}
	if (!DrawIconEx( hdc, 0, 0, icon, size, size, 0, 0, DI_MASK))
		qWarning("convertHIconToPixmap(), failed to DrawIcon()");
	if (!image.isNull()) { // failed to alloc?
		int bytes_per_line = size * sizeof(QRgb);
		for (int y=0; y<size; ++y) {
			QRgb *dest = (QRgb *) image.scanLine(y);
			const QRgb *src = (const QRgb *) (bits + y * bytes_per_line);
			for (int x=0; x<size; ++x) {
				if (!src[x])
					dest[x] = dest[x] | mask;
			}
		}
	}
	SelectObject(hdc, oldhdc); //restore state
	DeleteObject(winBitmap);
	DeleteDC(hdc);
	return QPixmap::fromImage(image);
}
#endif //ifndef Q_OS_WINCE
#endif

#ifdef Q_OS_WIN
QIcon getWinIcon(const FileInfo& fileInfo)
{
	QIcon icon;

	if (fileInfo.name == "..")
	{
		return icon;
	}
	
	QString key;

	QString strSuffix = fileSuffix(fileInfo.name).toUpper();
	strSuffix.prepend( QLatin1String(".") );

	if ((fileInfo.type==File || fileInfo.type==HideFile) && !fileInfo.isExecutable && 
		(fileInfo.type&SymLink) == 0 && (fileInfo.type&HideSymLink) == 0)
		key = QLatin1String("qt_") + strSuffix;

	QPixmap pixmap;
	if (!key.isEmpty())
	{
		QPixmapCache::find(key, pixmap);
	}

	if (!pixmap.isNull())
	{
		icon.addPixmap(pixmap);
		if (QPixmapCache::find(key + QLatin1Char('l'), pixmap))
			icon.addPixmap(pixmap);
		return icon;
	}

	if (fileInfo.type != File && fileInfo.type != HideFile)
	{
		return fileIcon(fileInfo.type);
	}

	/* We don't use the variable, but by storing it statically, we
	* ensure CoInitialize is only called once. */
	static HRESULT comInit = CoInitialize(NULL);
	Q_UNUSED(comInit);

	SHFILEINFO info;
	unsigned long val = 0;

	//Get the small icon
#ifndef Q_OS_WINCE
	val = SHGetFileInfo((const WCHAR *)strSuffix.utf16(), FILE_ATTRIBUTE_NORMAL, &info,
		sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_SYSICONINDEX|SHGFI_ADDOVERLAYS);
#else
	val = SHGetFileInfo((const WCHAR *)strSuffix.utf16(), FILE_ATTRIBUTE_NORMAL, &info,
		sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES|SHGFI_SMALLICON|SHGFI_SYSICONINDEX);
#endif

	if (val)
	{
#ifndef Q_OS_WINCE
		pixmap = convertHIconToPixmap(info.hIcon);
#else
		pixmap = convertHIconToPixmap(ImageList_GetIcon((HIMAGELIST) val, info.iIcon, ILD_NORMAL));
#endif
		if (!pixmap.isNull())
		{
			icon.addPixmap(pixmap);
			if (!key.isEmpty())
				QPixmapCache::insert(key, pixmap);
		}
		else
		{
			qWarning("QFileIconProviderPrivate::getWinIcon() no small icon found");
		}

		DestroyIcon(info.hIcon);
	}

	//Get the big icon
#ifndef Q_OS_WINCE
	val = SHGetFileInfo((const WCHAR *)strSuffix.utf16(), FILE_ATTRIBUTE_NORMAL, &info,
		sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_LARGEICON|SHGFI_SYSICONINDEX|SHGFI_ADDOVERLAYS);
#else
	val = SHGetFileInfo((const WCHAR *)strSuffix.utf16(), FILE_ATTRIBUTE_NORMAL, &info,
		sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES|SHGFI_LARGEICON|SHGFI_SYSICONINDEX);
#endif

	if (val)
	{
#ifndef Q_OS_WINCE
		pixmap = convertHIconToPixmap(info.hIcon);
#else
		pixmap = convertHIconToPixmap(ImageList_GetIcon((HIMAGELIST) val, info.iIcon, ILD_NORMAL), true);
#endif
		if (!pixmap.isNull())
		{
			icon.addPixmap(pixmap);
			if (!key.isEmpty())
				QPixmapCache::insert(key + QLatin1Char('l'), pixmap);
		}
		else
		{
			qWarning("QFileIconProviderPrivate::getWinIcon() no large icon found");
		}
		DestroyIcon(info.hIcon);
	}
	return icon.isNull()? fileIcon(fileInfo.type):icon;
}
#endif

QIcon fileIcon(const FileInfo& info)
{
	if (info.name == "..")
	{
		QStyle *style = QApplication::style();
		if (style != NULL)
		{
			return style->standardIcon(QStyle::SP_FileDialogToParent);
		}
	}

#ifdef Q_WS_WIN
	QIcon icon = getWinIcon(info);
	if (!icon.isNull())
		return icon;
#endif
	
	return fileIcon(info.type);
}

QIcon fileIcon(const QString& filename, int type)
{
	FileInfo info;
	info.name=filename;
	info.type=type;

	return fileIcon(info);
}

QString fileSize(const FileInfo& info)
{
	if (info.type == File || info.type == HideFile || (info.type & (SymLink|HideSymLink)) != 0)
	{
		fileSize(info.size);
	}

	return "";
}

QString fileSize(quint64 size)
{
	quint64 nSize=(quint64)ceil(size/1024.0);

	return formatNumberString(QString::number(nSize))+" KB";
}

quint64 fileSizeForNumber(const FileInfo& info)
{
	if (info.type == File || info.type == HideFile || (info.type & (SymLink|HideSymLink)) != 0)
	{
		return info.size;
	}

	return INVALID_SIZE_MARK;
}

bool operator==(const TransferFileItem& source, const TransferFileItem& target)
{
	if (source.name != target.name || source.type != target.type)
	{
		return false;
	}

	return true;
}

bool operator!=(const TransferFileItem& source, const TransferFileItem& target)
{
	return !(source == target);
}

QDataStream& operator>>(QDataStream& in, PrivilegeUser& user)
{
	in>>user.datetime>>user.name>>user.passwd>>user.privileges;

	return in;
}

QDataStream& operator<<(QDataStream& out, const PrivilegeUser& user)
{
	out<<user.datetime<<user.name<<user.passwd<<user.privileges;

	return out;
}

bool checkPrivileges(const QMap<int, quint32>& privileges, int type, int flag)
{
	if (privileges.contains(type) && (privileges[type]&flag) != 0)
	{
		return true;
	}

	return false;
}

QDataStream& operator>>(QDataStream& in, PrivilegeGroup& group)
{
	in>>group.datetime>>group.name>>group.usrs;

	return in;
}

QDataStream& operator<<(QDataStream& out, const PrivilegeGroup& group)
{
	out<<group.datetime<<group.name<<group.usrs;

	return out;
}

bool operator==(const PrivilegeGroup& source, const PrivilegeGroup& target)
{
	if (source.datetime == target.datetime && source.name == target.name)
	{
		return true;
	}

	return false;
}

bool operator!=(const PrivilegeGroup& source, const PrivilegeGroup& target)
{
	return !(source==target);
}

QDataStream& operator>>(QDataStream& in, PrivilegeOperation& operation)
{
	in>>operation.uid>>operation.datetime>>operation.operation;

	return in;
}

QDataStream& operator<<(QDataStream& out, const PrivilegeOperation& operation)
{
	out<<operation.uid<<operation.datetime<<operation.operation;

	return out;
}

QDataStream& operator>>(QDataStream& in, PrivilegeGroups& groups)
{
	in>>groups.groups>>groups.operations;

	return in;
}

QDataStream& operator<<(QDataStream& out, const PrivilegeGroups& groups)
{
	out<<groups.groups<<groups.operations;

	return out;
}

QDataStream& operator>>(QDataStream& in, CscsDatabase& data)
{
	in>>data.stations;

	return in;
}

QDataStream& operator<<(QDataStream& out, const CscsDatabase& data)
{
	out<<data.stations;

	return out;
}

QDataStream& operator>>(QDataStream& in, CscsStation& data)
{
	in>>data.stnId>>data.longname>>data.groups;

	return in;
}

QDataStream& operator<<(QDataStream& out, const CscsStation& data)
{
	out<<data.stnId<<data.longname<<data.groups;

	return out;
}

QDataStream& operator>>(QDataStream& in, CscsGroup& data)
{
	in>>data.grpId>>data.longname>>data.types;

	return in;
}

QDataStream& operator<<(QDataStream& out, const CscsGroup& data)
{
	out<<data.grpId<<data.longname<<data.types;

	return out;
}

QDataStream& operator>>(QDataStream& in, CscsDataType& data)
{
	in>>data.dataType>>data.longname;

	return in;
}

QDataStream& operator<<(QDataStream& out, const CscsDataType& data)
{
	out<<data.dataType<<data.longname;

	return out;
}

QDataStream& operator<<(QDataStream& out, const SyncMonitorFile& file)
{
	out<<file.path<<file.enabled<<file.file;

	return out;
}

QDataStream& operator>>(QDataStream& in, SyncMonitorFile& file)
{
	in>>file.path>>file.enabled>>file.file;

	return in;
}

QDataStream& operator<<(QDataStream& out, const SyncProgram& program)
{
	out<<program.path<<program.program<<program.enabled<<program.running<<program.terminal<<program.files;

	return out;
}

QDataStream& operator>>(QDataStream& in, SyncProgram& program)
{
	in>>program.path>>program.program>>program.enabled>>program.running>>program.terminal>>program.files;

	return in;
}

QDataStream& operator<<(QDataStream& out, const SyncCommand& command)
{
	out<<command.command<<command.enabled<<command.params<<command.files;

	return out;
}

QDataStream& operator>>(QDataStream& in, SyncCommand& command)
{
	in>>command.command>>command.enabled>>command.params>>command.files;

	return in;
}

QDataStream& operator<<(QDataStream& out, const SyncCommands& commands)
{
	out<<commands.programs<<commands.commands;

	return out;
}

QDataStream& operator>>(QDataStream& in, SyncCommands& commands)
{
	in>>commands.programs>>commands.commands;

	return in;
}

QString configManagerErrorCode(quint32 configManager)
{
	QString strCode;

	switch (configManager)
	{
	case 1:
		strCode=QObject::tr("Device is not configured correctly.");
		break;
	case 2:
		strCode=QObject::tr("Windows cannot load the driver for this device.");
		break;
	case 3:
		strCode=QObject::tr("Driver for this device might be corrupted, or the system may be low on memory or other resources.");
		break;
	case 4:
		strCode=QObject::tr("Device is not working properly. One of its drivers or the registry might be corrupted.");
		break;
	case 5:
		strCode=QObject::tr("Driver for the device requires a resource that Windows cannot manage.");
		break;
	case 6:
		strCode=QObject::tr("Boot configuration for the device conflicts with other devices.");
		break;
	case 7:
		strCode=QObject::tr("Cannot filter.");
		break;
	case 8:
		strCode=QObject::tr("Driver loader for the device is missing.");
		break;
	case 9:
		strCode=QObject::tr("Device is not working properly; the controlling firmware is incorrectly reporting the resources for the device.");
		break;
	case 10:
		strCode=QObject::tr("Device cannot start.");
		break;
	case 11:
		strCode=QObject::tr("Device failed.");
		break;
	case 12:
		strCode=QObject::tr("Device cannot find enough free resources to use.");
		break;
	case 13:
		strCode=QObject::tr("Windows cannot verify the device's resources.");
		break;
	case 14:
		strCode=QObject::tr("Device cannot work properly until the computer is restarted.");
		break;
	case 15:
		strCode=QObject::tr("Device is not working properly due to a possible re-enumeration problem.");
		break;
	case 16:
		strCode=QObject::tr("Windows cannot identify all of the resources that the device uses.");
		break;
	case 17:
		strCode=QObject::tr("Device is requesting an unknown resource type.");
		break;
	case 18:
		strCode=QObject::tr("Device drivers need to be reinstalled.");
		break;
	case 19:
		strCode=QObject::tr("Failure using the VxD loader.");
		break;
	case 20:
		strCode=QObject::tr("Registry might be corrupted.");
		break;
	case 21:
		strCode=QObject::tr("System failure. If changing the device driver is ineffective, see the hardware documentation. Windows is removing the device.");
		break;
	case 22:
		strCode=QObject::tr("Device is disabled.");
		break;
	case 23:
		strCode=QObject::tr("System failure. If changing the device driver is ineffective, see the hardware documentation.");
		break;
	case 24:
		strCode=QObject::tr("Device is not present, not working properly, or does not have all of its drivers installed.");
		break;
	case 25:
		strCode=QObject::tr("Windows is still setting up the device.");
		break;
	case 26:
		strCode=QObject::tr("Windows is still setting up the device.");
		break;
	case 27:
		strCode=QObject::tr("Device does not have valid log configuration.");
		break;
	case 28:
		strCode=QObject::tr("Device drivers are not installed.");
		break;
	case 29:
		strCode=QObject::tr("Device is disabled; the device firmware did not provide the required resources.");
		break;
	case 30:
		strCode=QObject::tr("Device is using an IRQ resource that another device is using.");
		break;
	case 31:
		strCode=QObject::tr("Device is not working properly; Windows cannot load the required device drivers.");
		break;
	default:
		strCode=QObject::tr("Device is working properly.");
		break;
	}

	return strCode;
}

