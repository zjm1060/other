#ifndef QREADWRITEXMLFILE_H
#define QREADWRITEXMLFILE_H

#include "qabstractreadwritexmlfile.h"

typedef struct _panelActionConfigure
{
	QString name;
	QString command;	//Ö´ÐÐÃüÁî
}PanelActionConfigure;

typedef struct _panelMenuConfigure
{
	QString name;

	QMap<int, _panelMenuConfigure>	menus;	//×Ó²Ëµ¥
	QMap<int, PanelActionConfigure> actions;
}PanelMenuConfigure;

typedef struct _panelMenuBarConfigure 
{
	QVector<PanelMenuConfigure>	menus;
}PanelMenuBarConfigure;

class QReadPanelMenuConfigureFromXmlFile : public QAbstractReadXmlFile
{
public:
	QReadPanelMenuConfigureFromXmlFile(const QString& filename);
	~QReadPanelMenuConfigureFromXmlFile();

public:
	PanelMenuBarConfigure result();

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readMenuBarNode(QXmlStreamReader& reader);
	void readMenuNode(QXmlStreamReader& reader);
	void readChildNode(QXmlStreamReader& reader, PanelMenuConfigure& menu);

private:
	PanelMenuBarConfigure	m_results;
};

class QWritePanelMenuConfigureToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWritePanelMenuConfigureToXmlFile(const QString& filename, const PanelMenuBarConfigure& menubar);
	~QWritePanelMenuConfigureToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "menubar";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

private:
	void saveMenuNode(QDomDocument& doc, QDomElement& parent, const QMap<int, PanelMenuConfigure>& menus);
	void saveActionNode(QDomDocument& doc, QDomElement& parent, const QMap<int, PanelActionConfigure>& actions);

private:
	PanelMenuBarConfigure		m_menubar;
};

#endif // QREADWRITEXMLFILE_H
