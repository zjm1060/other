#ifndef QGROUPDEFFILECOMPARE_H
#define QGROUPDEFFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"
#include "qtreemodel.h"

class FC_EXPORT QGroupDefFileCompare : public QObject, public QAbstractFileCompare
{
	Q_OBJECT

public:
	QGroupDefFileCompare(QObject *parent=0);
	~QGroupDefFileCompare();

	enum {Type=COMPARE_GROUP_DEF_FILE};


public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);
	virtual void compareFile(FileCompareResult& source, FileCompareResult& target);
	virtual int writeData(QIODevice* device, FileCompareResult& result);
	virtual bool checkModified(FileCompareResult& source, FileCompareResult& target);

private:
	void compareGroup(const QVector<GROUP_ENTRY>& src, const QVector<GROUP_ENTRY>& tar, QTreeModel* source, QTreeModel* target,
		QModelIndex psrc, QModelIndex ptar);

private:
	enum TitleType
	{
		StationNameTitle,
		GroupIDTitle,
		DeviceNameTitle,
		RTUTypeTitle,
		LongNameTitle,
		CommentTitle,
		ConditionDeviceTitle,
		ConditionDeviceTypeTitle,
		ConditionDeviceIDTitle
	};
};

FC_EXPORT bool operator==(const GROUP_ENTRY& source, const GROUP_ENTRY& target);
FC_EXPORT bool operator==(const GROUP_ENTRY_EX& source, const GROUP_ENTRY_EX& target);

Q_DECLARE_METATYPE(QVector<GROUP_ENTRY_EX>)

#endif // QGROUPDEFFILECOMPARE_H
