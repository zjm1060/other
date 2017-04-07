#ifndef QCMDDEFFILECOMPARE_H
#define QCMDDEFFILECOMPARE_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "qfcextern.h"
#include "qmsgdeffilecompare.h"

class FC_EXPORT QCmdDefFileCompare : public QMsgDefFileCompare
{
	Q_OBJECT

public:
	QCmdDefFileCompare(QObject *parent=0);
	~QCmdDefFileCompare();

	enum {Type=COMPARE_CMD_MSG_FILE};

	typedef MSG_DEF_ENTRY CMD_DEF_ENTRY;
	typedef MSG_GROUP_DEF_ENTRY CMD_GROUP_DEF_ENTRY;
	typedef MSG_STATE_DEF_ENTRY CMD_STATE_DEF_ENTRY;

public:
	virtual int type() const	{return Type;}
	virtual bool match(const QString& filename) const;

protected:
	virtual void parseFile(QFile& file, FileCompareResult& result);

private:
	QVector<CMD_DEF_ENTRY> parseCmdDefFile(QTextStream& stream);
};

#endif // QCMDDEFFILECOMPARE_H
