#include "qstatdiskdrive.h"

#include "define.h"

#ifdef Q_OS_SOLARIS
#include <kstat.h>

#elif defined(Q_OS_AIX)
#include <libperfstat.h>
#endif

QStatDiskDrive::QStatDiskDrive(QObject *parent)
: QObject(parent)
{
}

QStatDiskDrive::~QStatDiskDrive()
{
}

void QStatDiskDrive::beferQuery() 
{
	m_vecDiskDrive.clear();

#ifdef Q_OS_SOLARIS
	m_mapDisk.clear();
#endif
}

void QStatDiskDrive::query()
{
#ifdef Q_OS_SOLARIS
	kstat_ctl_t *kc;
	kstat_t *ksp;
	kstat_named_t *knp;

	if ((kc = kstat_open()) == NULL)
	{
		return;
	}

	for (ksp = kc->kc_chain; ksp!=NULL; ksp = ksp->ks_next)
	{
		if ((strcmp(ksp->ks_class, "partition")) != 0 && (strcmp(ksp->ks_class, "device_error")) != 0) continue;
		if (kstat_read(kc, ksp, NULL) == -1)
		{
			continue;
		}

		if ((strcmp(ksp->ks_class, "partition")) == 0)
		{
			QString strName=QString("%1%2").arg(ksp->ks_module).arg(ksp->ks_instance);
			if (!m_mapDisk.contains(strName))
			{
				insertDisk(strName);
			}

			m_mapDisk[strName].partitions++;
		}
		else if ((strcmp(ksp->ks_class, "device_error")) == 0)
		{
			QString strName=QString::fromLocal8Bit(ksp->ks_name);
			QStringList listSplit=strName.split(QChar(','), QString::SkipEmptyParts);
			if (listSplit.isEmpty())
			{
				continue;
			}

			if (!m_mapDisk.contains(listSplit[0]))
			{
				insertDisk(listSplit[0]);
			}

			DiskDrive& disk=m_mapDisk[listSplit[0]];

			knp=(kstat_named_t*)kstat_data_lookup(ksp, "Size");
			if (knp != NULL)
			{
				disk.size=knp->value.ui64;
			}

			knp=(kstat_named_t*)kstat_data_lookup(ksp, "Device Not Ready");
			if (knp != NULL)
			{
				if (knp->value.ui32 == 0)
				{
					disk.configManager=0;
				}
				else
				{
					disk.configManager=11;
				}
			}

			knp=(kstat_named_t*)kstat_data_lookup(ksp, "Vendor");
			if (knp != NULL)
			{
				disk.manufacturer=QString::fromLocal8Bit(knp->value.str.addr.__pad).trimmed();
			}

			knp=(kstat_named_t*)kstat_data_lookup(ksp, "Product");
			if (knp != NULL)
			{
				disk.model=QString::fromLocal8Bit(knp->value.str.addr.__pad).trimmed();

				QRegExp exp("(Revision)$", Qt::CaseInsensitive);
				int index = exp.indexIn(disk.model);
				if (index != -1)
				{
					disk.model=disk.model.left(index);
					knp=(kstat_named_t*)kstat_data_lookup(ksp, "Revision");
					if (knp != NULL)
					{
						disk.model+=QString::fromLocal8Bit(knp->value.str.addr.__pad);
						disk.model.trimmed();
					}
				}
			}
			else
			{
				knp=(kstat_named_t*)kstat_data_lookup(ksp, "Model");
				if (knp != NULL)
				{
					disk.model=QString::fromLocal8Bit(knp->value.str.addr.__pad).trimmed();
				}
			}

			disk.description=tr("Disk Drive");

			disk.mediaType=tr("Fixedhard disk media");
		}
	}

	QMapIterator<QString, DiskDrive> it(m_mapDisk);
	while (it.hasNext())
	{
		it.next();

		DiskDrive disk=it.value();

		disk.os=OS_SOLARIS;

		if (disk.model.contains("CD", Qt::CaseInsensitive) || disk.model.contains("DVD", Qt::CaseInsensitive))
		{
			continue;
		}
		
		if (disk.partitions == 0)
		{
			disk.partitions=1;
		}

		if (!disk.model.isEmpty())
		{
			if (!disk.manufacturer.isEmpty())
			{
				disk.model=disk.manufacturer+" "+disk.model;
			}
		}
		else
		{
			disk.model=tr("Disk Device");
		}

		if (disk.manufacturer.isEmpty())
		{
			disk.manufacturer=tr("(Standard disk drive)");
		}

		m_vecDiskDrive.append(disk);
	}

	kstat_close(kc);

#elif defined(Q_OS_AIX)
// 	int i, ret, tot;
// 
// 	perfstat_disk_t *statp;
// 	perfstat_id_t first;
// 
// 	/* check how many perfstat_disk_t structures are available */
// 	tot =  perfstat_disk(NULL, NULL, sizeof(perfstat_disk_t), 0);
// 
// 	/* check for error */
// 	if (tot < 0)
// 	{
// 		qDebug("QStatDiskDrive: perfstat_disk error!!");
// 		return;
// 	}
// 	else if (tot == 0)
// 	{
// 		qDebug("QStatDiskDrive: No disks found in the system!!");
// 		return;
// 	}
// 
// 	/* allocate enough memory for all the structures */
// 	statp = new perfstat_disk_t[tot];
// 
// 	/* set name to first interface */
// 	strcpy(first.name, FIRST_DISK);
// 
// 	/* ask to get all the structures available in one call */
// 	/* return code is number of structures returned */
// 	ret = perfstat_disk(&first, statp, sizeof(perfstat_disk_t), tot);
// 
// 	/* check for error */
// 	if (ret <= 0)
// 	{
// 		qDebug("QStatDiskDrive: perfstat_disk error!!");
// 		return;
// 	}
// 
// 	/* print statistics for each of the disks */
// 	for (i = 0; i < ret; i++)
// 	{
// 		DiskDrive disk;
// 
// 		disk.description=QString::fromLocal8Bit(statp[i].description);
// 		if (disk.description.contains("CD", Qt::CaseInsensitive) || disk.description.contains("DVD", Qt::CaseInsensitive))
// 		{
// 			continue;
// 		}
// 
// 		disk.model=QString::fromLocal8Bit(statp[i].name);
// 
// 		disk.configManager=0;
// 		disk.deviceID=QString::fromLocal8Bit(statp[i].vgname);
// 		disk.mediaType=QString::fromLocal8Bit(statp[i].adapter);
// 		disk.partitions=1;
// 		disk.size=statp[i].size*1024*1024;
// 
// 		disk.numberOfMediaSupported=0;
// 
// 		disk.SCSIBus=-1;
// 		disk.SCSILogicalUnit=-1;
// 		disk.SCSITargetId=-1;
// 		disk.SCSIPort=-1;
// 		
// 		if (disk.model.isEmpty())
// 		{
// 			disk.model=tr("Disk Device");
// 		}
// 
// 		if (disk.manufacturer.isEmpty())
// 		{
// 			disk.manufacturer=tr("(Standard disk drive)");
// 		}
// 
// 		m_vecDiskDrive.append(disk);
// 	}

	FILE *pp, *pSub;
	char buf[256]; 
	char* ptr;
	int i, j, index;
	int nLen;
	char cmd[64];

	QString strContent;
	QStringList lstSubContent;

	if( (pp = popen("/usr/sbin/lsdev -Cc pdisk", "r")) == NULL )
	{ 
		printf("popen() error!\n"); 
		return;
	} 

	DiskDrive disk;
	disk.os=OS_AIX;

	while(fgets(buf, sizeof buf, pp)) 
	{
		ptr=buf;
		i=j=0;
		nLen=strlen(buf);
		index=0;

		if (nLen == 0)
		{
			continue;
		}

		while (j<=nLen && index<3)
		{
			if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || j == nLen)
			{
				if (j > i)
				{
					switch (index)
					{
					case 0:
						{
							disk.model=QString::fromLocal8Bit(buf+i, j-i);
						}
						break;
					case 1:
						{
							disk.status=QString::fromLocal8Bit(buf+i, j-i);
						}
						break;
					case 2:
						{
							disk.description=QString::fromLocal8Bit(buf+j+1, nLen-j-2);
						}
						break;
					default:
						break;
					}

					index++;
				}

				i=j+1;
			}

			ptr++;
			j++;
		}

		sprintf(cmd, "/usr/sbin/lsattr -El %s", disk.model.toLocal8Bit().data());
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
				if (lstSubContent[0] == "serial_number" && lstSubContent.size() >=2)
				{
					disk.serialNumber=lstSubContent[1];
				}
				else if (lstSubContent[0] == "size_in_mb")
				{
					disk.size=lstSubContent[1].toULongLong()*1024*1024;
				}
				else if (lstSubContent[0] == "unique_id" && lstSubContent.size() >=2)
				{
					disk.deviceID=lstSubContent[1];
				}
				else if (lstSubContent[0] == "vend_prod_id" && lstSubContent.size() >=3)
				{
					disk.manufacturer=lstSubContent[1]+" "+lstSubContent[2];
				}
			}

			pclose(pSub);

			//»ñÈ¡Î»ÖÃ
			sprintf(cmd, "/usr/sbin/lscfg -l %s", disk.model.toLocal8Bit().data());
			if( (pSub = popen(cmd, "r")) != NULL )
			{ 
				while(fgets(buf, sizeof buf, pSub))
				{
					strContent=QString::fromLocal8Bit(buf).trimmed();
					if (strContent.isEmpty())
					{
						continue;
					}

					lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
					if (lstSubContent[0] == disk.model && lstSubContent.size() >=2)
					{
						disk.position=lstSubContent[1];
					}
				}

				pclose(pSub);
			} 
		}

		m_vecDiskDrive.append(disk);
	}

	pclose(pp);

#endif
}

#ifdef Q_OS_SOLARIS
void QStatDiskDrive::insertDisk(const QString& name)
{
	DiskDrive disk;
	disk.configManager=11;
	disk.partitions=0;
	disk.size=0;
	disk.numberOfMediaSupported=0;
	disk.SCSIBus=-1;
	disk.SCSILogicalUnit=-1;
	disk.SCSITargetId=-1;
	disk.SCSIPort=-1;

	disk.deviceID=name;

	m_mapDisk.insert(name, disk);
}
#endif

