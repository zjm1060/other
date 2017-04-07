#include "common.h"

#include <QCoreApplication>
#include <QProcess>
#include <QPainter>
#include <QRect>
#include <QSettings>
#include <QRegExp>
#include <QDir>
#include <QFile>
#include <QHostInfo>

#include <math.h>
#include <stdlib.h>

#ifdef Q_OS_WIN
#include   <windows.h>     
#include   <wincon.h>         
#include   <stdio.h>     
#include   <time.h>     

#define	SIZEOF_DEVICE_NAME	256

#elif defined(Q_OS_SOLARIS)
#include <sys/sockio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <stropts.h>
#include <strings.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>

#define	MAX_IFS	128

#elif defined(Q_OS_AIX)
#include 	<sys/socket.h>  /* for AF_INET        */
#include 	<netinet/in.h>  /* for struct in_addr */
#include 	<arpa/inet.h>   /* for inet_ntoa()    */
#include	<unistd.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<netdb.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>
#include	<net/if.h>
#include	<errno.h>

#define	MAX_IFS	16
#endif

QString getAppDir()
{
	QString strAppDir=QCoreApplication::applicationDirPath();

	if (strAppDir == ".")
	{
		QRegExp exp("^(PATH=)", Qt::CaseInsensitive);
		QRegExp splitExp("[^(:|;)]+");

		QString strAppName=QCoreApplication::applicationFilePath();
		int index=strAppName.lastIndexOf('/');
		if (index != -1)
		{
			strAppName=strAppName.right(strAppName.length()-index);
		}

		QStringList environment = QProcess::systemEnvironment();
		foreach (QString strEnv, environment)
		{
			index = exp.indexIn(strEnv);
			if (index != -1)
			{
				strEnv=strEnv.right(strEnv.length()-index-exp.matchedLength());

				index=0;
				QString strPath;
				while ((index=splitExp.indexIn(strEnv, index)) != -1)
				{
					strPath=splitExp.cap();
					if (!strPath.isEmpty() && QFile::exists(appendPath(strPath, strAppName)))
					{
						return strPath;
					}

					index+=splitExp.matchedLength();
				}
			}
		}
	}

	return strAppDir;
}

QString getAppParentDir()
{
	QString strParentDir=getAppDir();

	strParentDir=getParentDir(strParentDir);

	return strParentDir;
}

QString getParentDir(const QString& strDir)
{
	QString strParentDir=QDir::fromNativeSeparators(strDir);

	if (strParentDir.right(1) == "/")
	{
		strParentDir=strParentDir.left(strParentDir.size()-1);
	}

	int nIndex=strParentDir.lastIndexOf('/');
	if (nIndex != -1)
	{
		strParentDir=strParentDir.left(nIndex);
	}

	if (strParentDir.isEmpty() || strParentDir.right(1) == ":")
	{
		strParentDir+="/";
	}

	return strParentDir;
}

QString parseNameFromPath(const QString& strPath)
{
	QString str=strPath;
	int nIndex=str.lastIndexOf("/");
	if (nIndex != -1)
	{
		str=str.right(str.length()-nIndex-1);
	}

	return str;
}

QString fileSuffix(const QString& strFile)
{
	QString str=strFile;
	int nIndex=str.lastIndexOf(".");
	if (nIndex != -1)
	{
		str=strFile.right(str.length()-nIndex-1);
	}

	return str;
}

QString appendPath(const QString& dir, const QString& path)
{
	QString strDir=dir;
	if (dir.right(1) != "/" && dir.right(1) != "\\")
	{
		strDir+="/";
	}

	return strDir+path;
}

QString getEnvPath(const QString& name)
{
	QRegExp exp("^(PATH=)", Qt::CaseInsensitive);
	QRegExp splitExp("[^(:|;)]+");

	QStringList environment = QProcess::systemEnvironment();
	foreach (QString strEnv, environment)
	{
		int index = exp.indexIn(strEnv);
		if (index != -1)
		{
			strEnv=strEnv.right(strEnv.length()-index-exp.matchedLength());

			index=0;
			QString strPath;
			while ((index=splitExp.indexIn(strEnv, index)) != -1)
			{
				strPath=splitExp.cap();
				if (!strPath.isEmpty() && strPath.contains(name, Qt::CaseInsensitive))
				{
					return strPath;
				}

				index+=splitExp.matchedLength();
			}
		}
	}

	return QString();
}

QIcon addColorToIcon(const QString& strIcon, const QColor& color)
{
	QPixmap image(strIcon);

	QPixmap pixmap(image.width(), image.height());
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);

	QRect target(0, 0, pixmap.width(), pixmap.height());
	QRect source(0, 0, image.width(), image.height());
	painter.drawPixmap(target, image, source);

	painter.fillRect(QRect(3, pixmap.height()-6, pixmap.width()-6, 5), color);
	
	return QIcon(pixmap);
}

QIcon addIconToIcon(const QIcon& icon, const QIcon& targetIcon, int flag)
{
	if (icon.isNull() || targetIcon.isNull())
	{
		return targetIcon;
	}

	QPixmap pixmap=targetIcon.pixmap(QSize(16, 16), QIcon::Normal, QIcon::On);
	QPixmap image=icon.pixmap(QSize(10, 10), QIcon::Normal, QIcon::On);

	QPainter painter(&pixmap);

	QRect target;
	switch (flag)
	{
	case 0:
		target=QRect(pixmap.width()-image.width(), pixmap.height()-image.height(), image.width(), image.height());
		break;
	case 1:
		target=QRect((pixmap.width()-image.width())/2, (pixmap.height()-image.height())/2, image.width(), image.height());
		break;
	default:
		break;
	}
	
	QRect source(0, 0, image.width(), image.height());
	painter.drawPixmap(target, image, source);

	return QIcon(pixmap);
}

QPixmap addPixmapToPixmap(const QPixmap& pixmap, const QPixmap& targetPixmap, int flag)
{
	if (pixmap.isNull() || targetPixmap.isNull())
	{
		return targetPixmap;
	}

	QPixmap image=targetPixmap;

	QPainter painter(&image);

	QRect target;
	switch (flag)
	{
	case 0:
		target=QRect(image.width()-pixmap.width(), image.height()-pixmap.height(), pixmap.width(), pixmap.height());
		break;
	case 1:
		target=QRect((image.width()-pixmap.width())/2, (image.height()-pixmap.height())/2, pixmap.width(), pixmap.height());
		break;
	default:
		break;
	}

	QRect source(0, 0, pixmap.width(), pixmap.height());
	painter.drawPixmap(target, pixmap, source);

	return image;
}

int	QColor2ARGB(const QColor& color)
{
	int nColor=0;
	nColor=(color.alpha()<<24)|(color.red()<<16)|(color.green()<<8)|color.blue();

	return nColor;
}

QColor ARGB2QColor(int nColor)
{
	int a, r, g, b;
	a=(nColor&0xff000000)>>24;
	r=(nColor&0x00ff0000)>>16;
	g=(nColor&0x0000ff00)>>8;
	b=nColor&0x000000ff;

	return QColor(r, g, b, a);
}

QVariant parseConfig(const QString& filename, const QString& key, const QVariant& defaultValue)
{
	QSettings settings(filename, QSettings::IniFormat);

	return settings.value(key, defaultValue);
}

void codeConfig(const QString& filename, const QString& key, const QVariant& value)
{
	QSettings settings(filename, QSettings::IniFormat);

	settings.setValue(key, value);
}

QColor parseConfigColor(const QString& filename, const QString& key, const QColor& defaultColor)
{
	QColor color;
	QString strColor=parseConfig(filename, key, "").toString();
	QStringList listColorRgb=strColor.split(',', QString::SkipEmptyParts);
	int r,g,b;
	if (listColorRgb.size() == 3)
	{
		r=listColorRgb[0].toInt();
		g=listColorRgb[1].toInt();
		b=listColorRgb[2].toInt();

		color=QColor(r, g, b);
	}
	else
	{
		color=defaultColor;
	}

	return color;
}

QString formatNumberString(const QString& str)
{
	QString strNumber=str;

	int nNum=str.size();
	int nIndex=nNum%3;
	if (nIndex == 0)
	{
		nIndex=3;
	}
	nNum-=nIndex;
	while (nNum > 0)
	{
		strNumber.insert(nIndex, ',');

		nNum-=3;
		nIndex+=4;
	}

	return strNumber;
}

QString sizeToString(quint64 size)
{
	QString strSize;

	if (size == -1)
	{
		strSize=QObject::tr("Unknown");
	}
	else
	{
		qreal dSize=(qreal)size;
		if (dSize/(1024*1024*1024) > 1.0)
		{
			strSize=QString::number(dSize/(1024*1024*1024), 'f', 2)+" GB";
		}
		else if (dSize/(1024*1024) > 1.0)
		{
			strSize=QString::number(ceil(dSize/(1024*1024)))+" MB";
		}
		else if (dSize/1024 > 1.0)
		{
			strSize=QString::number(ceil(dSize/1024))+" KB";
		}
		else
		{
			strSize=QString::number(ceil(dSize))+" B";
		}
	}

	return strSize;
}

quint64 StringToSize(QString size)
{
	quint64 nSize=-1;
	quint64 nFactor=1;

	QStringList strSub=size.split(QRegExp("(GB|MB|KB|B)$"), QString::SkipEmptyParts);
	if (strSub.size() > 1)
	{
		if (strSub[1] == "GB")
		{
			nFactor*=1024*1024*1024;
		}
		else if (strSub[1] == "MB")
		{
			nFactor*=1024*1024;
		}
		else if (strSub[1] == "KB")
		{
			nFactor*=1024;
		}

		nSize=(quint64)(strSub[0].toDouble()*nFactor);
	}

	return nSize;
}

bool hasInvalidChar(const QString& dir, QChar* ch)
{
	QChar c;
	QRegExp exp("[\\*|\\?|\"|\\<|\\>|\\||:|\\\\]");

	int nPos=exp.indexIn(dir);
	if (nPos != -1)
	{
		c=dir.at(nPos);
		if (c == ':' && nPos == 1)
		{
			nPos+=exp.matchedLength();
			nPos=exp.indexIn(dir, nPos);
			if (nPos != -1)
			{
				if (ch != NULL)
				{
					*ch=dir.at(nPos);
				}

				return true;
			}
			return false;
		}

		return true;
	}

	return false;
}

bool isTextFormat(const QString& file)
{
	QString strSuffix=parseConfig(getAppParentDir()+INI_FILE, "format/text", "").toString();

	if (!strSuffix.isEmpty())
	{
		QStringList listSuffix=strSuffix.split(',', QString::SkipEmptyParts, Qt::CaseInsensitive);

		QString strExp="(";

		QStringListIterator it(listSuffix);

		strExp+="\\"+it.next();

		while (it.hasNext())
		{
			strExp+="|\\"+it.next();
		}

		strExp+=")$";

		QRegExp exp(strExp, Qt::CaseInsensitive);
		int nPos=exp.indexIn(file);
		if (nPos != -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

#ifdef Q_OS_WIN
pid_t getpid()
{
	return GetCurrentProcessId();
}
#endif

QString getUserName()
{
	QString strHostName;
	QStringList envVariables;
	envVariables<<"USERDOMAIN.*"<<"DOMAINNAME.*"<<"HOSTNAME.*"<<"USERNAME.*"<<"USER.*";

	QStringList environment = QProcess::systemEnvironment();
	foreach (QString string, envVariables)
	{
		int index = environment.indexOf(QRegExp(string));
		if (index != -1)
		{
			QStringList stringList = environment.at(index).split("=");
			if (stringList.size() == 2)
			{
				strHostName = stringList.at(1).toUtf8();
				break;
			}
		}
	}

	if (strHostName.isEmpty())
	{
		strHostName= "unknown";
	}

	return strHostName;
}

QString getHostName()
{
	return QHostInfo::localHostName();
}

/**
*	获取主机名，要查询DNS
*	该函数比较耗时
**/
#ifdef Q_OS_WIN
QString getHostByAddr(const QString& addr)
{
	QString strHostName;

	struct hostent *hptr;
	struct in_addr hipaddr;

	if(_inet_aton(addr.toLocal8Bit().data(), &hipaddr))
	{
		if( (hptr = gethostbyaddr((char*)&hipaddr, 4, AF_INET) ) != NULL)
		{
			strHostName=QString::fromLocal8Bit(hptr->h_name);
		}
	}

	return strHostName;
}
#endif

QString addSerialNumberToFileName(const QString& filename, int number)
{
	return addSerialStringToFileName(filename, QString::number(number));
}

QString addSerialStringToFileName(const QString& filename, const QString& str)
{
	QString strFileName=filename;

	int index=strFileName.lastIndexOf(QChar('.'));
	if (index != -1)
	{
		if (index == 0 || strFileName.at(index-1) == QChar('/'))
		{
			strFileName.insert(index, QString("%1").arg(str));
		}
		else
		{
			strFileName.insert(index, QString("_%1").arg(str));
		}
	}
	else
	{
		strFileName.append(QString("_%1").arg(str));
	}

	return strFileName;
}

#ifdef Q_OS_WIN
int gettimeofday(struct timeval *tp, void *tzp)
{
	Q_UNUSED(tzp);

	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year     = wtm.wYear - 1900;
	tm.tm_mon     = wtm.wMonth - 1;
	tm.tm_mday     = wtm.wDay;
	tm.tm_hour     = wtm.wHour;
	tm.tm_min     = wtm.wMinute;
	tm.tm_sec     = wtm.wSecond;
	tm. tm_isdst    = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;

	return (0);
}
#endif

QString qFormatString(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	QString s;
	s = s.vsprintf(format, ap);
	va_end(ap);

	return s;
}

#ifdef Q_OS_WIN
bool list_netcard_mac(unsigned char *netcard_name, char *mac_addr)
{
	HANDLE hDev;
	char	name[256];

	sprintf_s(name, sizeof(name), "\\\\.\\%s", netcard_name);	
	hDev= CreateFileA(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	
	if(hDev != INVALID_HANDLE_VALUE)
	{
		char	buf[32];
		int		size;
		int		inbuf= 0x01010101;//OID_802_3_PERMANENT_ADDRESS
		int		dwIoControlCode= 0x00170002;//IOCTL_NDIS_QUERY_STATS

		memset(buf, 0, sizeof(buf));
		if(DeviceIoControl(hDev, dwIoControlCode, &inbuf, 4, buf, sizeof(buf), (LPDWORD)&size, NULL))
		{
			sprintf_s(mac_addr, 20, "%02X:%02X:%02X:%02X:%02X:%02X", 
				*(unsigned char*)buf, *(unsigned char*)(buf+1), *(unsigned char*)(buf+2), 
				*(unsigned char*)(buf+3), *(unsigned char*)(buf+4), *(unsigned char*)(buf+5));
			CloseHandle(hDev);
			return true;
		}

		CloseHandle(hDev);
	}
	return false;
}
#endif

QString getMacAddress()
{
	QString strMacAddress;
	char	mac[20];

#ifdef Q_OS_WIN
	char key_name[ SIZEOF_DEVICE_NAME];
	unsigned char device_info[ SIZEOF_DEVICE_NAME];
	FILETIME file_time;

	HKEY hkey,hsub;
	int index;
	DWORD size;

	index = 0 ;

	if( RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards",
		0, KEY_READ, &hkey) == ERROR_SUCCESS)
	{
		size = SIZEOF_DEVICE_NAME;

		while( RegEnumKeyExA(hkey,index,key_name,&size,NULL,NULL,NULL,&file_time) == ERROR_SUCCESS)
		{
			if(RegOpenKeyExA(hkey,key_name, 0, KEY_READ, &hsub) == ERROR_SUCCESS)
			{
				size= SIZEOF_DEVICE_NAME;
				if(RegQueryValueExA(hsub, "ServiceName", NULL, NULL, device_info, &size)==ERROR_SUCCESS)
				{
					if(list_netcard_mac(device_info, mac))
					{
						strMacAddress=QString::fromLocal8Bit(mac);
						RegCloseKey( hsub);
						break;
					}
				}
				RegCloseKey( hsub);
			}

			index++;

			size = SIZEOF_DEVICE_NAME;
		}

		RegCloseKey( hkey);
	}

#elif defined(Q_OS_SOLARIS)
	int sock;
	struct ifconf ifc;
	struct ifreq *ifr, *ifend;
	struct ifreq ifr_temp;
	struct ifreq ifs[MAX_IFS];
	struct arpreq arp;

	char* buf;

	sock = socket(AF_INET, SOCK_DGRAM, 0);	

	ifc.ifc_len = sizeof(ifs);
	ifc.ifc_req = ifs;
	if (ioctl(sock, SIOCGIFCONF, &ifc) < 0)
	{
		close(sock);
		return strMacAddress;
	}	

	ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
	for (ifr = ifc.ifc_req; ifr < ifend; ifr++)
	{
		if (ifr->ifr_addr.sa_family == AF_INET)
		{
			memset(&ifr_temp, 0, sizeof(ifr_temp));

			strncpy(ifr_temp.ifr_name, ifr->ifr_name, sizeof(ifr_temp.ifr_name));

			if (ioctl (sock, SIOCGIFADDR, &ifr_temp) < 0)
			{
				continue;
			}

			arp.arp_pa.sa_family = AF_INET;
			arp.arp_ha.sa_family = AF_INET;
			((struct sockaddr_in*)&arp.arp_pa)->sin_addr.s_addr=((struct sockaddr_in *)&ifr_temp.ifr_addr)->sin_addr.s_addr;
			if (ioctl (sock, SIOCGARP, &arp) < 0)
			{
				continue;
			}

			buf=(char*)arp.arp_ha.sa_data;
			sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", 
				*(unsigned char*)buf, *(unsigned char*)(buf+1), *(unsigned char*)(buf+2), 
				*(unsigned char*)(buf+3), *(unsigned char*)(buf+4), *(unsigned char*)(buf+5));

			strMacAddress=QString::fromLocal8Bit(mac);

			break;
		}
	}

	close(sock);

#elif defined(Q_OS_AIX)
	int sock,len;
	struct ifconf ifc;
	struct ifreq *ifr;
	char buff[8192];

	char* buf;

	sock = socket(AF_INET, SOCK_DGRAM, 0);	

	ifc.ifc_len = sizeof(buff);
	ifc.ifc_req = (ifreq*)buff;
	if (ioctl(sock, SIOCGIFCONF, &ifc) < 0)
	{
		close(sock);
		return strMacAddress;
	}	

	ifr = ifc.ifc_req;
	len = ifc.ifc_len;

	while(len>0)
	{
		unsigned int inc= ifr->ifr_addr.sa_len;

		if (ifr->ifr_addr.sa_family == AF_LINK)
		{
			buf=(char*)&(ifr->ifr_addr.sa_data[9]);
			sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", 
				*(unsigned char*)buf, *(unsigned char*)(buf+1), *(unsigned char*)(buf+2), 
				*(unsigned char*)(buf+3), *(unsigned char*)(buf+4), *(unsigned char*)(buf+5));

			strMacAddress=QString::fromLocal8Bit(mac);

			break;	
		}

		if (inc < sizeof(ifr->ifr_addr))
			inc = sizeof(ifr->ifr_addr);
		inc += IFNAMSIZ;

		ifr = (struct ifreq*) (((char*) ifr) + inc);
		len -= inc;		

	}

	close(sock);
#endif

	return strMacAddress;
}
