/**
*	@Desc	通用函数或宏定义
*	@Author	zlb
**/

#ifndef COMMON_H
#define COMMON_H

#include "global.h"

#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QColor>
#include <QVariant>

#include <memory>

#define INVALID_SIZE_MARK	(quint64)-1

GLOBAL_EXPORT QString getAppDir();
GLOBAL_EXPORT QString getAppParentDir();
GLOBAL_EXPORT QString getParentDir(const QString& strDir);
GLOBAL_EXPORT bool isCommonDriveMode(const QString& source, const QString& target);
GLOBAL_EXPORT void convertToStdDir(QString& dir);
GLOBAL_EXPORT QString parseNameFromPath(const QString& strPath);
GLOBAL_EXPORT QString fileSuffix(const QString& strFile);
GLOBAL_EXPORT QString appendPath(const QString& dir, const QString& path);
GLOBAL_EXPORT QString getEnvPath(const QString& name);
GLOBAL_EXPORT QString getEnv(const QString& name);
GLOBAL_EXPORT QString getMacAddress(const QString& name);
GLOBAL_EXPORT quint64 macStringToUint64(const QString mac);
GLOBAL_EXPORT QString uint64ToMacString(quint64 uid);
GLOBAL_EXPORT QString convertToStdMAC(const QString& mac);

GLOBAL_EXPORT QString formatNumberString(const QString& str);
GLOBAL_EXPORT QString sizeToString(quint64 size);
GLOBAL_EXPORT quint64 StringToSize(QString size);

GLOBAL_EXPORT QVariant parseConfig(const QString& filename, const QString& key, const QVariant& defaultValue);
GLOBAL_EXPORT void codeConfig(const QString& filename, const QString& key, const QVariant& value);

GLOBAL_EXPORT bool hasInvalidChar(const QString& dir, QChar* ch);		//判断目录字符串中包含非法字符，若包含，返回第一个出现的字符
GLOBAL_EXPORT bool isUrl(const QString& text);

GLOBAL_EXPORT bool isTextFormat(const QString& file);
GLOBAL_EXPORT bool isRestrictFile(const QString& filename);

GLOBAL_EXPORT QColor parseConfigColor(const QString& filename, const QString& key, const QColor& defaultColor);

GLOBAL_EXPORT QIcon addColorToIcon(const QString& strIcon, const QColor& color);
GLOBAL_EXPORT QIcon addIconToIcon(const QIcon& icon, const QIcon& targetIcon, Qt::Corner corner);
GLOBAL_EXPORT QPixmap addPixmapToPixmap(const QPixmap& pixmap, const QPixmap& targetPixmap, Qt::Corner corner);
GLOBAL_EXPORT QPixmap findPixmap(const QString& key);

GLOBAL_EXPORT int	QColor2ARGB(const QColor& color);
GLOBAL_EXPORT QColor ARGB2QColor(int nColor);

GLOBAL_EXPORT QString addSerialNumberToFileName(const QString& filename, int number);
GLOBAL_EXPORT QString addSerialStringToFileName(const QString& filename, const QString& str);

GLOBAL_EXPORT QString qFormatString(const char* format, ...);

GLOBAL_EXPORT int parseOption(int argc, char **argv, char *ostr);

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <windows.h>
#include <time.h>

typedef DWORD pid_t;
typedef int socklen_t;

GLOBAL_EXPORT pid_t getpid();
GLOBAL_EXPORT int gettimeofday(struct timeval *tp, void *tzp);
GLOBAL_EXPORT QString getHostByAddr(const QString& addr);
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#endif

#ifdef Q_OS_SOLARIS
#include <unistd.h>
#include <sys/sockio.h>
#endif

GLOBAL_EXPORT QString getUserName();
GLOBAL_EXPORT QString getHostName();

#endif // COMMON_H
