#ifndef QTEXTFILECOMPARE_H
#define QTEXTFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"

class FC_EXPORT QTextFileCompare : public QObject, public QAbstractFileCompare
{
	Q_OBJECT

public:
	QTextFileCompare(QObject *parent=0);
	~QTextFileCompare();

	enum {Type=COMPARE_TEXT_FILE};

public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

public:
	virtual bool isHeaderHidden() const;

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);
	virtual void compareFile(FileCompareResult& source, FileCompareResult& target);
	virtual int writeData(QIODevice* device, FileCompareResult& result);
	virtual bool checkModified(FileCompareResult& source, FileCompareResult& target);

private:
	void diff(QAbstractItemModel* source, QAbstractItemModel* target, int& curIntexTarget,int endIndexTarget,
		const QVector<QString>& data, bool append=true);

	void fill(int num, QAbstractItemModel* model);

	int setRowData(QAbstractItemModel* model, const QString& data, int rowRole);

	bool compareDiff(QAbstractItemModel* source, int srow, QAbstractItemModel* target, int trow);

private:
	enum TitleType
	{
		LineTitle,
		TitleTypeEnd
	};
};

Q_DECLARE_METATYPE(QVector<QString>)

#endif // QTEXTFILECOMPARE_H
