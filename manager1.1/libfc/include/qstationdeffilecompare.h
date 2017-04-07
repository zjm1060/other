#ifndef QSTATIONDEFFILECOMPARE_H
#define QSTATIONDEFFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"

class FC_EXPORT QStationDefFileCompare : public QObject, public QAbstractFileCompare
{
	Q_OBJECT

public:
	QStationDefFileCompare(QObject *parent=0);
	~QStationDefFileCompare();

	enum {Type=COMPARE_STATION_DEF_FILE};

public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);
	virtual void compareFile(FileCompareResult& source, FileCompareResult& target);
	virtual int writeData(QIODevice* device, FileCompareResult& result);
	virtual bool checkModified(FileCompareResult& source, FileCompareResult& target);

private:
	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget,
		const QVector<STATION_ENTRY>& data, bool append=true);

	void fill(int num, QAbstractItemModel* model);

	int setData(QAbstractItemModel* model, const STATION_ENTRY& data, int rowRole);
	void setItemData(QAbstractItemModel* model, int row, int column, const QVariant& var, int itemRole);

	bool compareDiff(QAbstractItemModel* source, int srow, QAbstractItemModel* target, int trow);

private:
	enum TitleType
	{
		StationIDTitle,
		StationNameTitle,
		NumOfGroupsTitle,
		LongNameTitle,
		CommentTitle,
		TitleTypeEnd
	};
};

FC_EXPORT bool operator==(const STATION_ENTRY& source, const STATION_ENTRY& target);

Q_DECLARE_METATYPE(QVector<STATION_ENTRY>)

#endif // QSTATIONDEFFILECOMPARE_H
