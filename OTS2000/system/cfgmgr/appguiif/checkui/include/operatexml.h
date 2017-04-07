#ifndef OPERATEXML_H
#define OPERATEXML_H
#include <QObject>
#include <QFile>
#include <QtXml>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include "mdiMainWin.h"

class  COperateXml:QObject
{
	Q_OBJECT
public:
	COperateXml(MdiMainWindow * parent);
	~COperateXml();

	bool openxml();
	bool savexml();
	QDomNode getElementbyStr(QString tagName);//输入tagName 活动tagName所对应的Node
private:
	QString fileName;
	QDomDocument m_domDocument;
	MdiMainWindow * m_parent;

};
#endif