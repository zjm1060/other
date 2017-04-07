#include "qstatlogicaldisk.h"

#include "define.h"

QStatLogicalDisk::QStatLogicalDisk(QObject *parent)
: QObject(parent)
{
}

QStatLogicalDisk::~QStatLogicalDisk()
{
}

void QStatLogicalDisk::beferQuery() 
{
	m_vecLogicalDisk.clear(); 
}

void QStatLogicalDisk::query()
{
#ifdef Q_OS_SOLARIS
	struct mnttab mnt;

	m_strChrootPath = getenv("CHROOT");
	if (m_strChrootPath != NULL && strcmp(m_strChrootPath, "/") == 0)
	{
		m_strChrootPath = NULL;
	}

	FILE *mtabp;

	if ((mtabp = fopen(MNTTAB, "r")) == NULL)
	{
		qDebug("df: %s", MNTTAB);
		return;
	}
	
	char* strType="ufs";
	while (getmntent(mtabp, &mnt) == 0)
	{
		if (strcmp(strType, mnt.mnt_fstype) != 0)
		{
			continue;
		}

		LogicalDisk disk;

		if (dfreemnt(mnt.mnt_mountp, &mnt, disk) == -1)
		{
			continue;
		}

		m_vecLogicalDisk.append(disk);
	}

	fclose(mtabp);

#elif defined(Q_OS_AIX)
	FILE *pp;
	char buf[256];

	QString strContent;
	QStringList lstSubContent;

	if( (pp = popen("/usr/bin/df -P", "r")) == NULL )
	{ 
		printf("popen() error!\n"); 
		return;
	} 

	LogicalDisk disk;

	int nBlockSize=512;
	while(fgets(buf, sizeof buf, pp)) 
	{
		strContent=QString::fromLocal8Bit(buf).trimmed();
		if (strContent.isEmpty())
		{
			continue;
		}

		lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if (!lstSubContent[0].startsWith(QChar('/')) && lstSubContent.size() >=2)
		{
			bool bOk;
			int size=lstSubContent[1].toUInt(&bOk);
			if (bOk)
			{
				nBlockSize=size;
			}

			continue;
		}
		else if (lstSubContent.size() >=6)
		{
			if (lstSubContent[1] == "-")
			{
				continue;
			}

			disk.fileSystem=lstSubContent[0];
			disk.size=lstSubContent[1].toULongLong()*nBlockSize;
			disk.freeSpace=lstSubContent[3].toULongLong()*nBlockSize;
			disk.description=lstSubContent[5];

			disk.driveType=3;
			disk.driveTypeDesc=tr("Local Disk");
		}

		m_vecLogicalDisk.append(disk);
	}

	pclose(pp);
#endif
}

#ifdef Q_OS_SOLARIS
int QStatLogicalDisk::dfreemnt(char *file, struct mnttab *mnt, LogicalDisk& disk)
{
	struct statvfs64 fs;

	if (statvfs64(file, &fs) < 0 && chroot_stat(file, statvfs64, &fs, &file) < 0)
	{
		qDebug("df: %s", file);
		return -1;
	}

	if (fs.f_blocks == 0)
	{
		return -1;
	}

	disk.fileSystem=QString::fromLocal8Bit(mnt->mnt_special);
	disk.driveType=3;
	disk.driveTypeDesc=tr("Local Disk");

	fsblkcnt64_t avail;

	avail = fs.f_bavail;
	if ((long long)avail < 0)
		avail = 0;

	disk.size=quint64((fs.f_blocks-fs.f_bfree+fs.f_bavail)*fs.f_frsize);
	disk.freeSpace=(quint64)(avail*fs.f_frsize);

	disk.description=QString::fromLocal8Bit(mnt->mnt_mountp);

	return 0;
}

int QStatLogicalDisk::chroot_stat(char *dir, int (*statfunc)(const char*, statvfs64_t*), statvfs64_t *statp, char **dirp)
{
	if ((dir = zap_chroot(dir)) == NULL)
		return (-1);
	if (dirp)
		*dirp = dir;
	return (*statfunc)(dir, statp);
}

char* QStatLogicalDisk::zap_chroot(char *path)
{
	return (pathsuffix(path, m_strChrootPath));
}

char* QStatLogicalDisk::pathsuffix(char *full, char *pref)
{
	int preflen;

	if (full == NULL || pref == NULL)
	{
		return (NULL);
	}

	preflen = strlen(pref);
	if (strncmp(pref, full, preflen) != 0)
	{
		return (NULL);
	}

	/*
	* pref is a substring of full.  To be a subpath, it cannot cover a
	* partial component of full.  The last clause of the test handles the
	* special case of the root.
	*/
	if (full[preflen] != '\0' && full[preflen] != '/' && preflen > 1)
		return (NULL);

	if (preflen == 1 && full[0] == '/')
		return (full);
	else
		return (full + preflen);
}

#endif
