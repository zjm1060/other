
#ifndef ReportGen_H
#define ReportGen_H

#include <QtGui>
#include <QDomDocument>

//! [0]
class ReportGen
{
public:
//! [1]
//    ReportGen();
//! [1]
	QBuffer content;
	void genReport(QWidget *window, QBuffer *reportTemplate, QBuffer *reportData );

private:

};
//! [0]

#endif
