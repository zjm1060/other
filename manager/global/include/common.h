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

GLOBAL_EXPORT QString getAppDir();
GLOBAL_EXPORT QString getAppParentDir();
GLOBAL_EXPORT QString getParentDir(const QString& strDir);
GLOBAL_EXPORT QString parseNameFromPath(const QString& strPath);
GLOBAL_EXPORT QString fileSuffix(const QString& strFile);
GLOBAL_EXPORT QString appendPath(const QString& dir, const QString& path);
GLOBAL_EXPORT QString getEnvPath(const QString& name);
GLOBAL_EXPORT QString getMacAddress();

GLOBAL_EXPORT QString formatNumberString(const QString& str);
GLOBAL_EXPORT QString sizeToString(quint64 size);
GLOBAL_EXPORT quint64 StringToSize(QString size);

GLOBAL_EXPORT QVariant parseConfig(const QString& filename, const QString& key, const QVariant& defaultValue);
GLOBAL_EXPORT void codeConfig(const QString& filename, const QString& key, const QVariant& value);

GLOBAL_EXPORT bool hasInvalidChar(const QString& dir, QChar* ch);		//判断目录字符串中包含非法字符，若包含，返回第一个出现的字符

GLOBAL_EXPORT bool isTextFormat(const QString& file);

GLOBAL_EXPORT QColor parseConfigColor(const QString& filename, const QString& key, const QColor& defaultColor);

GLOBAL_EXPORT QIcon addColorToIcon(const QString& strIcon, const QColor& color);
GLOBAL_EXPORT QIcon addIconToIcon(const QIcon& icon, const QIcon& targetIcon, int flag);
GLOBAL_EXPORT QPixmap addPixmapToPixmap(const QPixmap& pixmap, const QPixmap& targetPixmap, int flag);

GLOBAL_EXPORT int	QColor2ARGB(const QColor& color);
GLOBAL_EXPORT QColor ARGB2QColor(int nColor);

GLOBAL_EXPORT QString addSerialNumberToFileName(const QString& filename, int number);
GLOBAL_EXPORT QString addSerialStringToFileName(const QString& filename, const QString& str);

GLOBAL_EXPORT QString qFormatString(const char* format, ...);

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
// #include <net/if.h>
#endif

#ifdef Q_OS_SOLARIS
#include <unistd.h>
#include <sys/sockio.h>
#endif

GLOBAL_EXPORT QString getHostName();
GLOBAL_EXPORT QString getUserName();

#define INI_FILE	"/ini/manager.ini"

#define TEMP_FILE_SUFFIX	".syn"

#define TRANSFER_SETTING_FILE	"/ini/transfer.ini"

#define H9000_DEF_PATH			"def"
#define H9000_DSP_PATH			"dsp"
#define H9000_DATABASE_PATH		"gen"
#define H9000_INTERLOCK_PATH	"def"
#define H9000_CALC_PATH			"def"
#define H9000_VOICE_PATH		"wav"

#endif // COMMON_H
