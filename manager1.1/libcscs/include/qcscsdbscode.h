#ifndef QCSCSDBSCODE_H
#define QCSCSDBSCODE_H

#include <QByteArray>

#include "global.h"
#include "qimessage.h"
#include "qutildef.h"

class CSCS_EXPORT QCscsDbsCode
{
protected:
	QCscsDbsCode();

public:
	~QCscsDbsCode();

public:
	static QCscsDbsCode* instance(bool initIgnored=false);

public:
	CscsDatabase database();

	int code(QByteArray& buffer, CscsEigenvalueType type, ...);
	int command(const QString& cmd, const QVector<QString>& params);
};

#endif // QCSCSDBSCODE_H
