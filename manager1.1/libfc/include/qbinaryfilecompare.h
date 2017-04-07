#ifndef QBINARYFILECOMPARE_H
#define QBINARYFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"

class FC_EXPORT QBinaryFileCompare : public QObject, public QAbstractFileCompare
{
	Q_OBJECT

public:
	QBinaryFileCompare(QObject *parent=0);
	~QBinaryFileCompare();

	enum {Type=COMPARE_BINARY_FILE};

public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

public:
	virtual bool isHeaderHidden() const;

	virtual int write(FileCompareResult& source, FileCompareResult& target, QPair<QModelIndex, QModelIndex>& error);

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);
	virtual void compareFile(FileCompareResult& source, FileCompareResult& target);
	virtual int writeData(QIODevice* device, FileCompareResult& result);
	virtual bool checkModified(FileCompareResult& source, FileCompareResult& target);

private:
	bool compareDiff(QAbstractItemModel* source, int srow, QAbstractItemModel* target, int trow);

private:
	enum TitleType
	{
		LineTitle,
		TitleTypeEnd
	};
};

#endif // QBINARYFILECOMPARE_H
