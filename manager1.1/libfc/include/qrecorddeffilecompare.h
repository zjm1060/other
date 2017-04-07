#ifndef QRECORDDEFFILECOMPARE_H
#define QRECORDDEFFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"
#include "qtreemodel.h"

class FC_EXPORT QRecordDefFileCompare : public QObject, public QAbstractFileCompare
{
	Q_OBJECT

public:
	QRecordDefFileCompare(QObject *parent=0);
	~QRecordDefFileCompare();

	enum {Type=COMPARE_RECORD_DEF_FILE};

public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);
	virtual void compareFile(FileCompareResult& source, FileCompareResult& target);
	virtual int writeData(QIODevice* device, FileCompareResult& result);
	virtual bool checkModified(FileCompareResult& source, FileCompareResult& target);

private:
	void compareGroup(const QVector<RECORD_GROUP_ENTRY>& src, const QVector<RECORD_GROUP_ENTRY>& tar, QTreeModel* source, QTreeModel* target,
		QModelIndex psrc, QModelIndex ptar);
	void compareRecords(const QVector<QString>& src, const QVector<QString>& tar, QTreeModel* source, QTreeModel* target,
		QModelIndex psrc, QModelIndex ptar);

private:
	enum TitleType
	{
		StationNameTitle,
		DeviceNameTitle,
		IncludeFileTitle
	};
};

FC_EXPORT bool operator==(const RECORD_GROUP_ENTRY& source, const RECORD_GROUP_ENTRY& target);
FC_EXPORT bool operator==(const RECORD_STATION_ENTRY& source, const RECORD_STATION_ENTRY& target);

Q_DECLARE_METATYPE(QVector<RECORD_STATION_ENTRY>)

#endif // QRECORDDEFFILECOMPARE_H
