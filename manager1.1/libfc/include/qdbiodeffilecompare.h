#ifndef QDBIODEFFILECOMPARE_H
#define QDBIODEFFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"
#include "qtreemodel.h"

class FC_EXPORT QDbioDefFileCompare : public QObject, public QAbstractFileCompare
{
	Q_OBJECT

public:
	QDbioDefFileCompare(QObject *parent=0);
	~QDbioDefFileCompare();

	enum {Type=COMPARE_DBIO_DEF_FILE};

	typedef struct
	{
		QString name;
		QString status;
	}DBIO_IOGROUP_STATUS_ENTRY;

	typedef struct 
	{
		int id;

		QString comment;
		QString exComment;

		QVector<DBIO_IOGROUP_STATUS_ENTRY> status;
	}DBIO_IOGROUP_DEF_ENTRY;

	typedef struct
	{
		QString grpName;

		QVector<DBIO_IOGROUP_DEF_ENTRY> iogrps;
	}DBIO_GROUP_DEF_ENTRY;

	typedef struct
	{
		QString stnName;

		QVector<DBIO_GROUP_DEF_ENTRY> groups;
	}DBIO_DEF_ENTRY;

public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);
	virtual void compareFile(FileCompareResult& source, FileCompareResult& target);
	virtual bool checkModified(FileCompareResult& source, FileCompareResult& target);
	virtual int writeData(QIODevice* device, FileCompareResult& result);

private:
	void compareGroups(const QVector<DBIO_GROUP_DEF_ENTRY>& sdata, const QVector<DBIO_GROUP_DEF_ENTRY>& tdata, 
		QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex);
	void compareIOGroups(const QVector<DBIO_IOGROUP_DEF_ENTRY>& sdata, const QVector<DBIO_IOGROUP_DEF_ENTRY>& tdata, 
		QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex);
	void compareStates(const QVector<DBIO_IOGROUP_STATUS_ENTRY>& sdata, const QVector<DBIO_IOGROUP_STATUS_ENTRY>& tdata,
		QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex);

	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget,
		const QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY>& data, bool append=true);
	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
		const QVector<QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY>& data, QModelIndex sindex, QModelIndex tindex, bool append=true);
	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
		const QVector<QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY>& data, QModelIndex sindex, QModelIndex tindex, bool append=true);
	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
		const QVector<QDbioDefFileCompare::DBIO_IOGROUP_STATUS_ENTRY>& data, QModelIndex sindex, QModelIndex tindex, bool append=true);

	void fill(int num, QAbstractItemModel* model, const QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY>& data);
	void fill(int num, QAbstractItemModel* model, const QVector<QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY>& data, QModelIndex pindex);
	void fill(int num, QAbstractItemModel* model, const QVector<QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY>& data, QModelIndex pindex);
	void fill(int num, QAbstractItemModel* model, const QVector<QDbioDefFileCompare::DBIO_IOGROUP_STATUS_ENTRY>& data, QModelIndex pindex);

private:
	void fillBlank(QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex,
		const DBIO_GROUP_DEF_ENTRY& group, bool blank);
	void fillBlank(QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex,
		const DBIO_IOGROUP_DEF_ENTRY& iogroup, bool blank);
	void fillBlank(QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex,
		const DBIO_IOGROUP_STATUS_ENTRY& status, bool blank);

private:
	enum TitleType
	{
		StationNameTitle,
		GroupNameTitle,
		IOGroupIDTitle,
		CommentTitle,
		NumberTitle,
		StatusTitle
	};

	QVector<DBIO_DEF_ENTRY> parseIOGroupDefFile(QTextStream& stream);
};

FC_EXPORT bool operator==(const QDbioDefFileCompare::DBIO_IOGROUP_STATUS_ENTRY& source, const QDbioDefFileCompare::DBIO_IOGROUP_STATUS_ENTRY& target);
FC_EXPORT bool operator==(const QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY& source, const QDbioDefFileCompare::DBIO_IOGROUP_DEF_ENTRY& target);
FC_EXPORT bool operator==(const QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY& source, const QDbioDefFileCompare::DBIO_GROUP_DEF_ENTRY& target);
FC_EXPORT bool operator==(const QDbioDefFileCompare::DBIO_DEF_ENTRY& source, const QDbioDefFileCompare::DBIO_DEF_ENTRY& target);

Q_DECLARE_METATYPE(QVector<QDbioDefFileCompare::DBIO_DEF_ENTRY>)

#endif // QDBIODEFFILECOMPARE_H
