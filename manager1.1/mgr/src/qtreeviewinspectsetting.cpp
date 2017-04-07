#include "qtreeviewinspectsetting.h"

#include <QMouseEvent>

#include "common.h"
#include "qimessage.h"
#include "qviewcontroller.h"

#define HMAN_INSPECT_SETTING_FILE "ini/inspect.xml"

QTreeViewInspectSetting::QTreeViewInspectSetting(QWidget *parent)
	: QTreeView(parent)
	,m_pClient(NULL)
{
	qRegisterMetaType<QTreeViewInspectSetting::InspectItem>("QTreeViewInspectSetting::InspectItem");

	QSharedControllerPtr pController(new QViewController);
	m_pClientManager=new QClientManager(pController, this);
	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));
	connect(m_pClientManager, SIGNAL(connectFailed(QClient*)), this, SLOT(unconnected(QClient*)));
	connect(m_pClientManager, SIGNAL(disconnected(QClient*)), this, SLOT(disconnected(QClient*)));

	m_pReceiver=new QReceiverInspect;
	connect(m_pReceiver, SIGNAL(cscsDataSourceLost()), this, SLOT(cscsDataSourceLost()));
	connect(m_pReceiver, SIGNAL(recvCscsDatabase(CscsDatabase)), this, SLOT(recvCscsDatabase(CscsDatabase)));

	m_pDelegate=new QInspectSettingDelegate(this);
	setItemDelegate(m_pDelegate);

	m_pModel=new QTreeModel(TitleLabelEnd);
	setModel(m_pModel);

	setHeaderHidden(true);

	QReadInspectRecordFormXmlFile record(appendPath(getAppParentDir(), HMAN_INSPECT_SETTING_FILE));
	if (record.read())
	{
		m_record=record.result();
	}

	init();
}

QTreeViewInspectSetting::~QTreeViewInspectSetting()
{
	m_pClientManager->removeAll();
	SAFE_DELETE_CLASS(m_pClientManager);

	SAFE_DELETE_CLASS(m_pReceiver);

	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);
}

void QTreeViewInspectSetting::init()
{
	if (m_pModel == NULL)
	{
		return;
	}

	int nRow=0;

	//H9000实时数据库差异检查
	m_pModel->insertRow(nRow);
	QModelIndex pindex=m_pModel->index(nRow++, NameTitle);
	m_pModel->setData(pindex, tr("Check differences of H9000 real time database"), Qt::DisplayRole);
	m_pModel->setData(pindex, TitleItem, ItemRole);
	m_pModel->setData(pindex, MSG_CSCS, MsgParamRole);
	m_pModel->setData(pindex, MSG_CSCS, DataRole);

	refreshItemCheckedState(m_record);
}

void QTreeViewInspectSetting::writeItemCheckedState(QTreeViewInspectSetting::InspectItem& item, QReadInspectRecordFormXmlFile::InspectItem& record, 
													const QModelIndex & parent)
{
	if (m_pModel == NULL)
	{
		return;
	}

	int nRows=m_pModel->rowCount(parent);
	if (nRows > 0)
	{
		for (int nRow=0; nRow<nRows; nRow++)
		{
			QModelIndex cindex=m_pModel->index(nRow, NameTitle, parent);

			if (cindex.isValid())
			{
				QTreeViewInspectSetting::InspectItem citem;
				QReadInspectRecordFormXmlFile::InspectItem crecord;

				crecord.id=m_pModel->data(cindex, DataRole).toInt();
				crecord.checked=(m_pModel->data(cindex, CheckedRole).toInt() != Qt::Unchecked);

				if (crecord.checked)
				{
					citem.desc=m_pModel->data(cindex, Qt::DisplayRole).toString();
					citem.id=crecord.id;
					citem.msgParam=m_pModel->data(cindex, MsgParamRole).toInt();
				}

				writeItemCheckedState(citem, crecord, cindex);

				if (crecord.checked)
				{
					item.children.append(citem);
				}

				record.children.insert(crecord.id, crecord);
			}
		}
	}

	return;
}

QTreeViewInspectSetting::InspectItem QTreeViewInspectSetting::writeConfigure()
{
	QTreeViewInspectSetting::InspectItem items;

	writeItemCheckedState(items, m_record);

	QWriteInspectRecordToXmlFile record(appendPath(getAppParentDir(), HMAN_INSPECT_SETTING_FILE), m_record);
	record.write();

	return items;
}

int QTreeViewInspectSetting::refreshItemCheckedState(const QReadInspectRecordFormXmlFile::InspectItem& item, const QModelIndex& index)
{
	if (m_pModel == NULL)
	{
		return -1;
	}

	int nGroupChecked=-1;
	int nRowCount=m_pModel->rowCount(index);
	if (nRowCount > 0)
	{
		for (int i=0; i<nRowCount; i++)
		{
			QModelIndex cindex=m_pModel->index(i, NameTitle, index);

			if (cindex.isValid())
			{
				int id=m_pModel->data(cindex, DataRole).toInt();

				QReadInspectRecordFormXmlFile::InspectItem citem;
				if (item.children.contains(id))
				{
					citem=item.children[id];
				}
				else
				{
					citem.checked=true;
				}

				int nChecked=refreshItemCheckedState(citem, cindex);

	 			if (nGroupChecked == -1)
	 			{
	 				nGroupChecked=nChecked;
	 			}
	 			else
	 			{
					if (nChecked == Qt::PartiallyChecked)
					{
						nGroupChecked=Qt::PartiallyChecked;
					}
					else if (nChecked == Qt::Checked)
					{
						if (nGroupChecked == Qt::Unchecked)
						{
							nGroupChecked=Qt::PartiallyChecked;
						}
					}
	 				else if (nChecked == Qt::Unchecked)
	 				{
						if (nGroupChecked == Qt::Checked)
						{
							nGroupChecked=Qt::PartiallyChecked;
						}
	 				}
	 			}
			}
		}

		if (index.isValid())
		{
			m_pModel->setData(index, nGroupChecked, CheckedRole);
		}
	}
	else
	{
		if (index.isValid())
		{
			nGroupChecked= (item.checked? Qt::Checked:Qt::Unchecked);

			m_pModel->setData(index, nGroupChecked, CheckedRole);
		}
	}

	return nGroupChecked;
}

void QTreeViewInspectSetting::setClient(const ServerInfo& info)
{
	if (m_pModel == NULL)
	{
		return;
	}

	QClient* pClient=m_pClientManager->addClient(info);

	if (m_pClient != NULL && m_pClient != pClient)
	{
		m_pClientManager->remove(m_pClient);
		m_pClient=NULL;
	}

	emit enableInspect(false);

	int nRowCount=m_pModel->rowCount();
	for (int i=0; i<nRowCount; i++)
	{
		QModelIndex index=m_pModel->index(i, NameTitle);
		int nItemRole=m_pModel->data(index, ItemRole).toInt();
		if (nItemRole == TitleItem)
		{
			int nMsgParamRole=m_pModel->data(index, MsgParamRole).toInt();
			if (nMsgParamRole == MSG_CSCS)
			{
				m_pModel->setRowCount(0, index);

				m_pModel->setData(index, Connecting, StatusRole);
				update(index);

				break;
			}
		}
	}

	m_pClientManager->connectToHost(pClient);
}

void QTreeViewInspectSetting::setTitleItemStatus(int paramRole, StatusRoleType statusRole)
{
	int nRowCount=m_pModel->rowCount();
	for (int i=0; i<nRowCount; i++)
	{
		QModelIndex index=m_pModel->index(i, NameTitle);
		int nItemRole=m_pModel->data(index, ItemRole).toInt();
		if (nItemRole == TitleItem)
		{
			int nMsgParamRole=m_pModel->data(index, MsgParamRole).toInt();
			if (nMsgParamRole == paramRole)
			{
				m_pModel->setData(index, statusRole, StatusRole);
				update(index);

				break;
			}
		}
	}
}

void QTreeViewInspectSetting::connected(QClient* client)
{
	m_pClient=client;
	if (m_pClient == NULL)
	{
		return;
	}

	setTitleItemStatus(MSG_CSCS, DataLoading);

	m_pReceiver->queryDatabase(m_pClient);
}

void QTreeViewInspectSetting::unconnected(QClient* client)
{
	m_pClientManager->remove(client);

	setTitleItemStatus(MSG_CSCS, Unconnected);

	emit enableInspect(true);
}

void QTreeViewInspectSetting::disconnected(QClient* client)
{
	unconnected(client);
}

void QTreeViewInspectSetting::cscsDataSourceLost()
{
	setTitleItemStatus(MSG_CSCS, H9000NotRunning);

	emit enableInspect(true);
}

void QTreeViewInspectSetting::recvCscsDatabase(CscsDatabase db)
{
	QModelIndex pindex;

	if (m_pModel != NULL)
	{
		int nRowCount=m_pModel->rowCount();
		for (int i=0; i<nRowCount; i++)
		{
			QModelIndex index=m_pModel->index(i, NameTitle);
			int nItemRole=m_pModel->data(index, ItemRole).toInt();
			if (nItemRole == TitleItem)
			{
				int nMsgParamRole=m_pModel->data(index, MsgParamRole).toInt();
				if (nMsgParamRole == MSG_CSCS)
				{
					pindex=index;
					break;
				}
			}
		}

		if (pindex.isValid())
		{
			for (int stn=0; stn<db.stations.size(); stn++)
			{
				const CscsStation& station=db.stations[stn];

			 	m_pModel->insertRow(stn, pindex);
			 	QModelIndex index=m_pModel->index(stn, NameTitle, pindex);
			 	m_pModel->setData(index, station.longname, Qt::DisplayRole);
			 	m_pModel->setData(index, ChildItem, ItemRole);
			 	m_pModel->setData(index, QueryCscsStationEigenvalue, MsgParamRole);
				m_pModel->setData(index, (int)station.stnId, DataRole);
				m_pModel->setData(index, true, InspectRole);

				for (int grp=0; grp<station.groups.size(); grp++)
				{
					const CscsGroup& group=station.groups[grp];

					m_pModel->insertRow(grp, index);
					QModelIndex cindex=m_pModel->index(grp, NameTitle, index);
					m_pModel->setData(cindex, group.longname, Qt::DisplayRole);
					m_pModel->setData(cindex, ChildItem, ItemRole);
					m_pModel->setData(cindex, QueryCscsGroupEigenvalue, MsgParamRole);
					m_pModel->setData(cindex, (int)group.grpId, DataRole);
					m_pModel->setData(cindex, true, InspectRole);

					for (int type=0; type<group.types.size(); type++)
					{
						const CscsDataType& dataType=group.types[type];

						m_pModel->insertRow(type, cindex);
						QModelIndex ccindex=m_pModel->index(type, NameTitle, cindex);
						m_pModel->setData(ccindex, dataType.longname, Qt::DisplayRole);
						m_pModel->setData(ccindex, ChildItem, ItemRole);
						m_pModel->setData(ccindex, QueryCscsDataTypeEigenvalue, MsgParamRole);
						m_pModel->setData(ccindex, (int)dataType.dataType, DataRole);
						m_pModel->setData(ccindex, true, InspectRole);
					}
				}
			}

			QReadInspectRecordFormXmlFile::InspectItem item;
			if (m_record.children.contains(MSG_CSCS))
			{
				item=m_record.children[MSG_CSCS];
			}
			else
			{
				item.checked=true;
			}

			refreshItemCheckedState(item, pindex);
			setParentItemCheckState(pindex);
		}
	}

	expand(pindex);

	setTitleItemStatus(MSG_CSCS, DataLoaded);

	emit enableInspect(true);
}

void QTreeViewInspectSetting::setItemCheckState(int row, int state, const QModelIndex& parent)
{
	Q_ASSERT(row < m_pModel->rowCount(parent));

	setChildItemCheckState(row, state, parent);
	setParentItemCheckState(parent);
}

void QTreeViewInspectSetting::setChildItemCheckState(int row, int state, const QModelIndex& parent)
{
	QModelIndex pindex=m_pModel->index(row, NameTitle, parent);

	m_pModel->setData(pindex, state, CheckedRole);

	int nNumRow=m_pModel->rowCount(pindex);
	for (int nRow=0; nRow<nNumRow; nRow++)
	{
		setChildItemCheckState(nRow, state, pindex);
	}
}

void QTreeViewInspectSetting::setParentItemCheckState(const QModelIndex& parent)
{
	if (parent == QModelIndex())
	{
		return;
	}

	int nNumRow=m_pModel->rowCount(parent);
	int nChecked=-1;

	for (int nRow=0; nRow<nNumRow; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, NameTitle, parent);
		int nState=m_pModel->data(index, CheckedRole).toInt();

		if (nState == Qt::PartiallyChecked)
		{
			nChecked=Qt::PartiallyChecked;
			break;
		}
		else
		{
			if (nChecked == -1)
			{
				nChecked=nState;
			}
			else
			{
				if (nChecked != nState)
				{
					nChecked=Qt::PartiallyChecked;
					break;
				}
			}
		}
	}

	if (nChecked != -1)
	{
		m_pModel->setData(m_pModel->index(parent.row(), NameTitle, parent.parent()), nChecked, CheckedRole);
	}

	setParentItemCheckState(parent.parent());
}

void QTreeViewInspectSetting::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		QModelIndex index=indexAt(event->pos());

		if (index.column() == NameTitle)
		{
			QRect rect=visualRect(index);

			int nCheckWidth=style()->pixelMetric(QStyle::PM_ButtonIconSize, 0, this);

			rect=QRect(rect.left()+4, rect.top(), nCheckWidth, rect.height());
			if (rect.contains(event->pos()))
			{
				int nState =m_pModel->data(m_pModel->index(index.row(), NameTitle, index.parent()), CheckedRole).toInt();
				if (nState == Qt::PartiallyChecked || nState == Qt::Checked)
				{
					nState=Qt::Unchecked;
				}
				else
				{
					nState=Qt::Checked;
				}

				setItemCheckState(index.row(), nState, index.parent());
				setExpanded(index, true);

				return;
			}
		}
	}

	QTreeView::mousePressEvent(event);
}

void QTreeViewInspectSetting::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_F5:
		{
			emit refreshKeyPress();

			event->ignore();
		}
		return;
	default:
		break;
	}

	QTreeView::keyPressEvent(event);
}

QReadInspectRecordFormXmlFile::QReadInspectRecordFormXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{
}

QReadInspectRecordFormXmlFile::~QReadInspectRecordFormXmlFile()
{

}

void QReadInspectRecordFormXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "Inspection", readInspectionNode);
}

void QReadInspectRecordFormXmlFile::readInspectionNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "Inspect", readInspectNode);
}

void QReadInspectRecordFormXmlFile::readInspectNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("id"))
	{
		int id=attributes.value("id").toString().toInt();

		readNextNodeEx(reader, "Item", readItemNode, m_record.children[id].children);
	}
	else
	{
		readItemFinished(reader);
	}
}

void QReadInspectRecordFormXmlFile::readItemNode(QXmlStreamReader& reader, QMap<int, InspectItem>& items)
{
	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("id"))
	{
		int id=attributes.value("id").toString().toInt();
		InspectItem item;
		item.id=id;
		if (attributes.hasAttribute("checked"))
		{
			bool bChecked=(attributes.value("checked").toString() == "true");
			item.checked=bChecked;
		}

		readNextNodeEx(reader, "Item", readItemNode, item.children);

		items.insert(id, item);
	}
	else
	{
		readItemFinished(reader);
	}
}

QWriteInspectRecordToXmlFile::QWriteInspectRecordToXmlFile(const QString& filename, const QReadInspectRecordFormXmlFile::InspectItem& records)
:QAbstractWriteXmlFile(filename)
,m_records(records)
{

}

QWriteInspectRecordToXmlFile::~QWriteInspectRecordToXmlFile()
{

}

void QWriteInspectRecordToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	QMapIterator<int, QReadInspectRecordFormXmlFile::InspectItem> it(m_records.children);
	while (it.hasNext())
	{
		it.next();

		const QReadInspectRecordFormXmlFile::InspectItem& item=it.value();

		QDomElement child=doc.createElement("Inspect");
		child.setAttribute("id", item.id);
		child.setAttribute("checked", item.checked? "true":"false");

		parent.appendChild(child);

		saveRecords(doc, child, item);
	}
}

void QWriteInspectRecordToXmlFile::saveRecords(QDomDocument& doc, QDomElement& node, const QReadInspectRecordFormXmlFile::InspectItem& items)
{
	if (!items.children.isEmpty())
	{
		QMapIterator<int, QReadInspectRecordFormXmlFile::InspectItem> it(items.children);
		while (it.hasNext())
		{
			it.next();

			const QReadInspectRecordFormXmlFile::InspectItem& item=it.value();

			QDomElement child=doc.createElement("Item");
			child.setAttribute("id", item.id);
			child.setAttribute("checked", item.checked? "true":"false");

			node.appendChild(child);

			saveRecords(doc, child, item);
		}
	}
}
