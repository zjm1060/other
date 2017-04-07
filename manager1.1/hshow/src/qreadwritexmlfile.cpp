#include "qreadwritexmlfile.h"

#include "common.h"
#include "qutildef.h"

QReadPanelMenuConfigureFromXmlFile::QReadPanelMenuConfigureFromXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{

}

QReadPanelMenuConfigureFromXmlFile::~QReadPanelMenuConfigureFromXmlFile()
{

}

PanelMenuBarConfigure QReadPanelMenuConfigureFromXmlFile::result()
{
	return m_results;
}

void QReadPanelMenuConfigureFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "menubar", readMenuBarNode);
}

void QReadPanelMenuConfigureFromXmlFile::readMenuBarNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "menu", readMenuNode);
}

void QReadPanelMenuConfigureFromXmlFile::readMenuNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "menu");

	PanelMenuConfigure menu;

	QXmlStreamAttributes attributes=reader.attributes();
	if (attributes.hasAttribute("name"))
	{
		menu.name=attributes.value("name").toString();

		readAllNode(reader, readChildNode, menu);

		m_results.menus.append(menu);
	}
	else
	{
		readUnknownElement(reader);
	}
}

void QReadPanelMenuConfigureFromXmlFile::readChildNode(QXmlStreamReader& reader, PanelMenuConfigure& menu)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (reader.name() == "menu")
	{
		if (attributes.hasAttribute("id") && attributes.hasAttribute("name"))
		{
			int nID=attributes.value("id").toString().toInt();

			PanelMenuConfigure subMenu;
			subMenu.name=attributes.value("name").toString();

			readAllNode(reader, readChildNode, subMenu);

			menu.menus.insert(nID, subMenu);
		}
		else
		{
			readUnknownElement(reader);
		}
	}
	else if (reader.name() == "action")
	{
		if (attributes.hasAttribute("id") && attributes.hasAttribute("name"))
		{
			int nID=attributes.value("id").toString().toInt();

			PanelActionConfigure action;
			action.name=attributes.value("name").toString();
			if (attributes.hasAttribute("command"))
			{
				action.command=attributes.value("command").toString();
			}

			readItemFinished(reader);

			menu.actions.insert(nID, action);
		}
	}
	else
	{
		readUnknownElement(reader);
	}
}

QWritePanelMenuConfigureToXmlFile::QWritePanelMenuConfigureToXmlFile(const QString& filename, const PanelMenuBarConfigure& menubar)
:QAbstractWriteXmlFile(filename)
,m_menubar(menubar)
{

}

QWritePanelMenuConfigureToXmlFile::~QWritePanelMenuConfigureToXmlFile()
{

}

void QWritePanelMenuConfigureToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	for (int i=0; i<m_menubar.menus.size(); i++)
	{
		const PanelMenuConfigure& menu=m_menubar.menus[i];
		if (menu.name.isEmpty())
		{
			continue;
		}

		QDomElement child=doc.createElement("menu");

		child.setAttribute("name", menu.name);

		saveMenuNode(doc, child, menu.menus);
		saveActionNode(doc, child, menu.actions);

		parent.appendChild(child);
	}
}

void QWritePanelMenuConfigureToXmlFile::saveMenuNode(QDomDocument& doc, QDomElement& parent, const QMap<int, PanelMenuConfigure>& menus)
{
	QMapIterator<int, PanelMenuConfigure> it(menus);
	while (it.hasNext())
	{
		const PanelMenuConfigure& menu=it.value();

		QDomElement child=doc.createElement("menu");

		child.setAttribute("id", it.key());
		child.setAttribute("name", menu.name);

		saveMenuNode(doc, child, menu.menus);
		saveActionNode(doc, child, menu.actions);

		parent.appendChild(child);
	}
}

void QWritePanelMenuConfigureToXmlFile::saveActionNode(QDomDocument& doc, QDomElement& parent, const QMap<int, PanelActionConfigure>& actions)
{
	QMapIterator<int, PanelActionConfigure> it(actions);
	while (it.hasNext())
	{
		const PanelActionConfigure& action=it.value();
		if (action.name.isEmpty())
		{
			continue;
		}

		QDomElement child=doc.createElement("action");

		child.setAttribute("id", it.key());
		child.setAttribute("name", action.name);
		child.setAttribute("command", action.command);

		parent.appendChild(child);
	}
}