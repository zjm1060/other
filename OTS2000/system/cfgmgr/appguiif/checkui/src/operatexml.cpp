#include "operatexml.h"
#define CFGMGRXML "/home/ems/h9000/cfgmgr/cfgmgr.xml"
//#define CFGMGRXML "cfgmgr.xml"
COperateXml::COperateXml(MdiMainWindow * parent)
{
	
	fileName=CFGMGRXML;
	m_parent=parent;
}
COperateXml::~COperateXml()
{

}
bool COperateXml::openxml()
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(m_parent, tr("Open cfgmgr.xml"),
			tr("Cannot read file %1:\n%2.please check cfgmgr.xml and restart! ")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!m_domDocument.setContent(&file, true, &errorStr, &errorLine,
		&errorColumn)) {
			QMessageBox::information(m_parent, tr("Open cfgmgr.xml"),
				tr("cfgmgr.xml Parse error at line %1, column %2\nErr info:%3.\nplease check cfgmgr.xml and restart!")
				.arg(errorLine)
				.arg(errorColumn)
				.arg(errorStr));
			return false;
	}

	QDomElement root = m_domDocument.documentElement();

	if (root.tagName() != "cfgmgr") {
		QMessageBox::information(m_parent, tr("Open cfgmgr.xml"),
			tr("The file is not an cfgmgr file.please check cfgmgr.xml and restart!"));
		return false;
	} 



	return true;
}

QDomNode COperateXml::getElementbyStr(QString tagName)
{

	QDomElement docElem = m_domDocument.documentElement();
	QDomNode n = docElem.firstChild();

	while(!n.isNull())
	{
		if (n.isElement())
		{
			QDomElement e = n.toElement();
		
		QString	tagname = e.tagName();
		if (e.tagName()==tagName)
		{
			return n;
		}
			QDomNodeList list = e.childNodes();
			for(int i=0; i<list.count(); i++)
			{
				QDomNode node = list.at(i);
				if(node.isElement())
				{
					if (node.toElement().tagName()==tagName)
					{
						return node;
					}
					tagname = node.toElement().tagName();
				QString	text = node.toElement().text();
				}
			}
		}
		n = n.nextSibling();
	}


return docElem;
//	return NULL;
}
bool COperateXml::savexml()
{
	if (fileName.isEmpty())
		return false;

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		//QMessageBox::warning(this, tr("SAX Bookmarks"),
		//	tr("Cannot write file %1:\n%2.")
		//	.arg(fileName)
		//	.arg(file.errorString()));
		qDebug()<<"Cannot write file "<<fileName;
		return false;
	}
}