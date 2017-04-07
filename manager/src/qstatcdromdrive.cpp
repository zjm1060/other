#include "qstatcdromdrive.h"

#include "define.h"

#ifdef Q_OS_SOLARIS
#include <kstat.h>
#endif

QStatCDROMDrive::QStatCDROMDrive(QObject *parent)
: QObject(parent)
{
}

QStatCDROMDrive::~QStatCDROMDrive()
{
}

void QStatCDROMDrive::beferQuery() 
{
	m_vecCDROMDrive.clear(); 
}

void QStatCDROMDrive::query()
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
		if ((strcmp(ksp->ks_class, "device_error")) != 0) continue;
		if (kstat_read(kc, ksp, NULL) == -1)
		{
			continue;
		}

		CDROMDrive disk;

		disk.os=OS_SOLARIS;
		
		disk.configManager=11;
		disk.size=0;
		disk.numberOfMediaSupported=0;
		disk.SCSIBus=-1;
		disk.SCSILogicalUnit=-1;
		disk.SCSITargetId=-1;
		disk.SCSIPort=-1;
		disk.transferRate=-1;

		knp=(kstat_named_t*)kstat_data_lookup(ksp, "Product");
		if (knp != NULL)
		{
			disk.name=QString::fromLocal8Bit(knp->value.str.addr.__pad).trimmed();

			QRegExp exp("(Revision)$", Qt::CaseInsensitive);
			int index = exp.indexIn(disk.name);
			if (index != -1)
			{
				disk.name=disk.name.left(index);
				knp=(kstat_named_t*)kstat_data_lookup(ksp, "Revision");
				if (knp != NULL)
				{
					disk.name+=QString::fromLocal8Bit(knp->value.str.addr.__pad);
					disk.name.trimmed();
				}
			}
		}
		else
		{
			knp=(kstat_named_t*)kstat_data_lookup(ksp, "Model");
			if (knp != NULL)
			{
				disk.name=QString::fromLocal8Bit(knp->value.str.addr.__pad).trimmed();
			}
		}

		if (!disk.name.contains("CD", Qt::CaseInsensitive) && !disk.name.contains("DVD", Qt::CaseInsensitive))
		{
			continue;
		}

		QString strName=QString::fromLocal8Bit(ksp->ks_name);
		QStringList listSplit=strName.split(QChar(','), QString::SkipEmptyParts);
		if (!listSplit.isEmpty())
		{
			disk.deviceID=listSplit[0];
		}

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

		knp=(kstat_named_t*)kstat_data_lookup(ksp, "Serial No");
		if (knp != NULL)
		{
			disk.volumeSerialNumber=QString::fromLocal8Bit(knp->value.str.addr.__pad).trimmed();
		}

		disk.description=tr("CD-ROM Drive");

		disk.mediaType=tr("CD-ROM");

		if (!disk.name.isEmpty())
		{
			if (!disk.manufacturer.isEmpty())
			{
				disk.name=disk.manufacturer+" "+disk.name;
			}
		}
		else
		{
			disk.name=tr("CD-ROM Drive");
		}

		if (disk.manufacturer.isEmpty())
		{
			disk.manufacturer=tr("(Standard CD-ROM drive)");
		}

		m_vecCDROMDrive.append(disk);
	}

	kstat_close(kc);

#elif defined(Q_OS_AIX)
	FILE *pp, *pSub;
	char buf[256]; 
	char* ptr;
	int i, j, index;
	int nLen;
	char cmd[64];

	QString strContent;
	QStringList lstSubContent;

	if( (pp = popen("/usr/sbin/lsdev -Cc cdrom", "r")) == NULL )
	{ 
		printf("popen() error!\n"); 
		return;
	} 

	CDROMDrive disk;
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

		disk.transferRate=-1;

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
							disk.name=QString::fromLocal8Bit(buf+i, j-i);
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

		sprintf(cmd, "/usr/sbin/lsattr -El %s", disk.name.toLocal8Bit().data());
		if( (pSub = popen(cmd, "r")) != NULL )
		{ 
			while(fgets(buf, sizeof buf, pSub))
			{
				strContent=QString::fromLocal8Bit(buf);
				lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (lstSubContent[0] == "max_transfer"  && lstSubContent.size() >=2)
				{
					disk.transferRate=lstSubContent[1].toUInt(NULL, 16)/1024;
				}
			}

			pclose(pSub);

			//»ñÈ¡Î»ÖÃ
			sprintf(cmd, "/usr/sbin/lscfg -vl %s", disk.name.toLocal8Bit().data());
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
					if (lstSubContent[0] == disk.name && lstSubContent.size() >=2)
					{
						disk.position=lstSubContent[1];
					}
					else if (lstSubContent[0] == "Manufacturer" && lstSubContent.size() >=2)
					{
						disk.manufacturer=lstSubContent[1];
					}
					else if (lstSubContent[0] == "Machine" && lstSubContent.size() >=5)
					{
						disk.volumeName=lstSubContent[4];
					}
					else if (lstSubContent[0] == "Device"  && lstSubContent.size() >=3)
					{
						disk.deviceID=lstSubContent[2];
					}
 				}

				pclose(pSub);
			} 
		}

		m_vecCDROMDrive.append(disk);
	}

	pclose(pp);
#endif
}

