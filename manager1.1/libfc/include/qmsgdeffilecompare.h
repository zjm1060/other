#ifndef QMSGDEFFILECOMPARE_H
#define QMSGDEFFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"
#include "qtreemodel.h"

class FC_EXPORT QMsgDefFileCompare : public QObject, public QAbstractFileCompare
{
	Q_OBJECT

public:
	QMsgDefFileCompare(QObject *parent=0);
	~QMsgDefFileCompare();

	enum {Type=COMPARE_MSG_DEF_FILE};

	typedef struct 
	{
		int index;
		QString def;
	}MSG_STATE_DEF_ENTRY;

	typedef struct
	{
		QString grpName;

		QVector<MSG_STATE_DEF_ENTRY> states;
	}MSG_GROUP_DEF_ENTRY;

	typedef struct
	{
		QString typeName;

		QVector<MSG_GROUP_DEF_ENTRY> groups;
	}MSG_DEF_ENTRY;

public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);
	virtual void compareFile(FileCompareResult& source, FileCompareResult& target);
	virtual int writeData(QIODevice* device, FileCompareResult& result);
	virtual bool checkModified(FileCompareResult& source, FileCompareResult& target);

private:
	void compareGroups(const QVector<MSG_GROUP_DEF_ENTRY>& sdata, const QVector<MSG_GROUP_DEF_ENTRY>& tdata, 
		QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex);
	void compareStates(const QVector<MSG_STATE_DEF_ENTRY>& sdata, const QVector<MSG_STATE_DEF_ENTRY>& tdata,
		QAbstractItemModel* source, QAbstractItemModel* target, QModelIndex sindex, QModelIndex tindex);

	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,
		int endIndexTarget, const QVector<QMsgDefFileCompare::MSG_DEF_ENTRY>& data, bool append=true);
	void fill(int num, QAbstractItemModel* model, const QVector<QMsgDefFileCompare::MSG_DEF_ENTRY>& data);

	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget, 
		const QVector<QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY>& data, QModelIndex sindex, QModelIndex tindex, bool append=true);
	void fill(int num, QAbstractItemModel* model, const QVector<QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY>& data, QModelIndex pindex);

	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget,
		const QVector<MSG_STATE_DEF_ENTRY>& data, const QModelIndex& parent, bool append=true);
	void fill(int num, QAbstractItemModel* model, const QModelIndex& parent);
	bool compareDiff(QAbstractItemModel* source, int srow, const QModelIndex& sparent, QAbstractItemModel* target, int trow, const QModelIndex& tparent);

	int setData(QAbstractItemModel* model, const MSG_STATE_DEF_ENTRY& data, const QModelIndex& parent, int rowRole);
	void setItemData(QAbstractItemModel* model, int row, int column, const QModelIndex& parent, const QVariant& var, int itemRole);

	QModelIndex setData(QAbstractItemModel* model, int column, const QVariant& data, const QModelIndex& parent=QModelIndex());

private:
	enum TitleType
	{
		TypeTitle,
		GroupNameTitle,
		StatusTitle,
		MeaningTitle,
		TitleTypeEnd
	};

	QVector<MSG_DEF_ENTRY> parseMsgDefFile(QTextStream& stream);
};

FC_EXPORT bool operator==(const QMsgDefFileCompare::MSG_STATE_DEF_ENTRY& source, const QMsgDefFileCompare::MSG_STATE_DEF_ENTRY& target);
FC_EXPORT bool operator==(const QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY& source, const QMsgDefFileCompare::MSG_GROUP_DEF_ENTRY& target);
FC_EXPORT bool operator==(const QMsgDefFileCompare::MSG_DEF_ENTRY& source, const QMsgDefFileCompare::MSG_DEF_ENTRY& target);

Q_DECLARE_METATYPE(QVector<QMsgDefFileCompare::MSG_DEF_ENTRY>)

#endif // QMSGDEFFILECOMPARE_H
