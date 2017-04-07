#ifndef QFILECOMPAREFACTORY_H
#define QFILECOMPAREFACTORY_H

#include <QMutex>
#include <QMap>

#include "qabstractfilecompare.h"
#include "qutildef.h"

enum FileModifiedStatus
{
	Modified,		//已修改的
	Added,			//添加的
	Deleted,		//删除的
	Unmodified,		//未修改的
	NonVersioned,	//已存在，但被忽略的
	ModifiedStatusEnd
};

enum FileUpdateFlag
{
	Modify=Modified,	//修改
	Add=Added,			//添加
	Latest=ModifiedStatusEnd,			//文件已是最新的
	Delete,				//删除
	UpdateFlagEnd
};

typedef struct _diffFile 
{
	QString path;	//文件相对路径名
	quint64 size;	//文件大小
	int type;		//文件类型
	QDateTime modified;		//修改日期
	int status;		//文件修改标识
	int version;	//文件修改版本
}DiffFile;

class UTIL_EXPORT QFileCompareFactory : public QObject
{
	Q_OBJECT

public:
	virtual ~QFileCompareFactory();

	typedef struct _fileCompareResult
	{
		QAbstractFileCompare::FileCompareResult source;
		QAbstractFileCompare::FileCompareResult target;
	}FileCompareResult;

protected:
	QFileCompareFactory();

public:
	static QFileCompareFactory* instance();

public:
	FileCompareResult compare(const QString& source, const QString& target);
	QAbstractFileCompare::ErrorCode write(QAbstractFileCompare::FileCompareResult& source, QAbstractFileCompare::FileCompareResult& target, 
		QPair<QModelIndex, QModelIndex>& error);

	bool isModified(const QString& source, const QString& target);
	bool isHeaderHidden(const FileCompareResult& result) const;

public:
	bool loadLibrary(const QString& filename, const char* symbol);

public:
	void registerProduct(const QAbstractFileComparePtr& ptr);

private:
	QAbstractFileComparePtr	m_pChains;

	QMutex	m_mutex;
};

Q_DECLARE_METATYPE(QFileCompareFactory::FileCompareResult)
Q_DECLARE_METATYPE(DiffFile)

#endif // QFILECOMPAREFACTORY_H
