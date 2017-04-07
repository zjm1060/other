#include "setpointmask.h"

Q_DECLARE_METATYPE(POINTER);
CSetPointMaskTable::CSetPointMaskTable(QWidget* parent)
	:QDialog(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	pointMode = comboPointMode->currentIndex();
	pointShowType = comboPointShowType->currentIndex();
	pointSourceModel = new QStandardItemModel;
	pointModel = new QSortFilterProxyModel;
	pointModel->setDynamicSortFilter(true);
	pointModel->setSourceModel(pointSourceModel);
	listView->setModel(pointModel);
	selectionModel = listView->selectionModel();
// 	QCompleter *completer = new QCompleter(this);
// 	completer->setModel(pointModel);
// 	editPointName->setCompleter(completer);
	buttonEnter->setFocus();
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(saveAllChange()));
	connect(buttonCancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(buttonSelectAll,SIGNAL(clicked()),this,SLOT(selectAll()));
	connect(buttonSelectOff,SIGNAL(clicked()),this,SLOT(selectOff()));
	connect(buttonSelectInvert,SIGNAL(clicked()),this,SLOT(selectInvert()));
	connect(comboPointMode,SIGNAL(currentIndexChanged(int)),this,SLOT(pointModeChange(int)));
	connect(comboPointShowType,SIGNAL(currentIndexChanged(int)),this,SLOT(pointShowTypeChange(int)));
	connect(editPointName,SIGNAL(textChanged(const QString &)),this,SLOT(filterChanged(const QString &))); 
	connect(listView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(pointClicked(const QModelIndex &))); 
	connect(treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
	connect(treeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem *,QTreeWidgetItem *)),this,SLOT(displayPoint(QTreeWidgetItem *,QTreeWidgetItem *)));
}
void CSetPointMaskTable::showEvent(QShowEvent *event)
{
	displayStnDevTree();
	event->accept();
}
void  CSetPointMaskTable::setLanguage(int language)
{	
	QTranslator translator;
	if(flag_simutrainee)
	{
		if (language == 0)
		{
			translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
		}
		else if (language == 1)
		{
			translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_en");
		}
	}
	else
	{
		if (language == 0)
		{
			translator.load("SimuTutor_zh_CN");
		}
		else if (language == 1)
		{
			translator.load("SimuTutor_en");
		}
	}	
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CSetPointMaskTable::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CSetPointMaskTable::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CSetPointMaskTable::displayStnDevTree()
{
	short qlt=0;
	POINTER	point;
	GULONG	stn_num,grp_num,rec_num,entry_num;
	UCHAR	stn_id,dev_id,data_type;
	char	stn_comment[STATION_COMMENT_SIZE];
	char	dev_comment[GROUP_COMMENT_SIZE];
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	char	text[128];
	treeWidget->clear();
	pointSourceModel->clear();
	hashPoint.clear();
	GetStationNum(&stn_num);
	for(int i=1; i<=stn_num; i++) 
	{  		
		GetStnIdByIndex(i, &stn_id);
		
		if (pointMode == 0)
		{
			if( GetStnCommentById(stn_id,(char*)( stn_comment))==-1 )
				continue;
		}
		else
		{
			if( GetStnNameById(stn_id,(char*)( stn_comment))==-1 )
				continue;
		}

		sprintf(text, "%d. %s", stn_id, stn_comment);
		QTreeWidgetItem *itemStn = new QTreeWidgetItem(treeWidget, QStringList(QString::fromLocal8Bit(text)));
		if (pointShowType == 2)	itemStn->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		else	itemStn->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		itemStn->setCheckState (0, Qt::Unchecked);
		itemStn->setData(ITEM_TYPE,Qt::UserRole,"stn");
		itemStn->setData(ITEM_STN_ID,Qt::UserRole,stn_id);
		if(-1==GetGroupNumById(stn_id, &grp_num))
			continue;

		for(dev_id=1; dev_id<=grp_num; dev_id++) 
		{  
			if (pointMode == 0)
			{
				if( GetGroupCommentById(stn_id, dev_id, dev_comment)==-1 )
					continue;
			}
			else
			{
				if( GetGroupNameById(stn_id, dev_id, dev_comment)==-1 )
					continue;
			}
			sprintf(text, "%d. %s", dev_id, dev_comment);
			QTreeWidgetItem *itemDev = new QTreeWidgetItem(itemStn, QStringList(QString::fromLocal8Bit(text)));
			if (pointShowType == 1)	itemDev->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			else	itemDev->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			itemDev->setCheckState (0, Qt::Unchecked);
			itemDev->setData(ITEM_TYPE,Qt::UserRole,"dev");
			itemDev->setData(ITEM_STN_ID,Qt::UserRole,stn_id);
			itemDev->setData(ITEM_DEV_ID,Qt::UserRole,dev_id);
			if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
				continue;
			for(int j=0,data_type=1; j<=rec_num; data_type++) 
			{
				if(data_type>=MAX_DATATYPE_NUM)
					break;
				if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
					continue;
				if (pointMode == 0)
				{
					if( GetRecLongnameByType(data_type, datatype_longname)==-1 )
						continue;
				}
				else
				{
					if( GetRecNameByType(data_type, datatype_longname)==-1 )
						continue;
				}
				j++;
				sprintf(text, "%d. %s", data_type, datatype_longname);
				QTreeWidgetItem *itemDataType = new QTreeWidgetItem(itemDev, QStringList(QString::fromLocal8Bit(text)));
				if (pointShowType == 0)	itemDataType->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				else	itemDataType->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				itemDataType->setCheckState (0, Qt::Unchecked);
				itemDataType->setData(ITEM_TYPE,Qt::UserRole,"data_type");
				itemDataType->setData(ITEM_STN_ID,Qt::UserRole,stn_id);
				itemDataType->setData(ITEM_DEV_ID,Qt::UserRole,dev_id);
				itemDataType->setData(ITEM_DATA_TYPE,Qt::UserRole,data_type);
				point.stn_id= stn_id;
				point.dev_id= dev_id;
				point.data_type= data_type;
				int selectedCount = 0;
				for(int id=0; id<entry_num; id++)
				{
					point.pt_id= id;
					QString ptStr = QString("%1.%2.%3.%4").arg(point.stn_id).arg(point.dev_id).arg(point.data_type).arg(point.pt_id);
					GetPtQlt(point,&qlt);
					if (qlt == 0)
					{
						hashPoint.insert(ptStr,qMakePair(short(0),short(0)));
					}
					else
					{
						hashPoint.insert(ptStr,qMakePair(short(1),short(1)));
						selectedCount++;
					}
				}
				if (selectedCount <= 0)
				{     //未选中状态         
					itemDataType->setCheckState(0, Qt::Unchecked);     
				}
				else if(selectedCount > 0 && selectedCount < entry_num)
				{     //部分选中状态         
					itemDataType->setCheckState(0, Qt::PartiallyChecked);     
				}
				else if(selectedCount == entry_num)
				{     //选中状态     
					itemDataType->setCheckState(0, Qt::Checked); 
				}// 
			}
		}
	}
}
void CSetPointMaskTable::displayPoint(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	POINTER	pt;
	short qlt = 0;
	UCHAR	stn_id,dev_id,data_type;
	GULONG	stn_num,grp_num,rec_num,entry_num;
	char	entry_longname[ENTRY_LONGNAME_SIZE];
	pointSourceModel->clear();
	if (current == NULL) return;
	if (current->data(ITEM_TYPE,Qt::UserRole).toString() == "stn")
	{
		if (pointShowType != 2) return; 
		stn_id = current->data(ITEM_STN_ID,Qt::UserRole).toInt();
		if(-1==GetGroupNumById(stn_id, &grp_num))
			return;
		for(dev_id=1; dev_id<=grp_num; dev_id++) 
		{
			if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
				continue;
			for(int j=0,data_type=1; j<=rec_num; data_type++) 
			{
				if(data_type>=MAX_DATATYPE_NUM)
					break;
				if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
					continue;
				pt.stn_id= stn_id;
				pt.dev_id= dev_id;
				pt.data_type= data_type;
				for(int id=0; id<entry_num; id++)
				{
					QVariant var;
					pt.pt_id= id;
					if (pointMode == 0)
					{
						if( GetEntryLongnameById(&pt, entry_longname)==-1 )
							continue;
					}
					else
					{
						if( GetEntryNameById(&pt, entry_longname)==-1 )
							continue;
					}
					QStandardItem* item = new QStandardItem;
					item->setText(QString::fromLocal8Bit(entry_longname)); 
					item->setCheckable(true);
					var.setValue(pt);
					item->setData(var,Qt::UserRole);
					pointSourceModel->appendRow(item);
				}
			}
		}
	}
	else if (current->data(ITEM_TYPE,Qt::UserRole).toString() == "dev")
	{
		if (pointShowType != 1) return; 
		stn_id = current->data(ITEM_STN_ID,Qt::UserRole).toInt();
		dev_id = current->data(ITEM_DEV_ID,Qt::UserRole).toInt();
		if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
			return;
		for(int j=0,data_type=1; j<=rec_num; data_type++) 
		{
			if(data_type>=MAX_DATATYPE_NUM)
				break;
			if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
				continue;
			pt.stn_id= stn_id;
			pt.dev_id= dev_id;
			pt.data_type= data_type;
			for(int id=0; id<entry_num; id++)
			{
				QVariant var;
				pt.pt_id= id;
				if (pointMode == 0)
				{
					if( GetEntryLongnameById(&pt, entry_longname)==-1 )
						continue;
				}
				else
				{
					if( GetEntryNameById(&pt, entry_longname)==-1 )
						continue;
				}
				QStandardItem* item = new QStandardItem;
				item->setText(QString::fromLocal8Bit(entry_longname)); 
				item->setCheckable(true);
				var.setValue(pt);
				item->setData(var,Qt::UserRole);
				pointSourceModel->appendRow(item);
			} 
		}
	}
	else if (current->data(ITEM_TYPE,Qt::UserRole).toString() == "data_type")
	{
		if (pointShowType != 0) return; 
		stn_id = current->data(ITEM_STN_ID,Qt::UserRole).toInt();
		dev_id = current->data(ITEM_DEV_ID,Qt::UserRole).toInt();
		data_type = current->data(ITEM_DATA_TYPE,Qt::UserRole).toInt();
		if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
			return;
		pt.stn_id= stn_id;
		pt.dev_id= dev_id;
		pt.data_type= data_type;
		for(int id=0; id<entry_num; id++)
		{
			QVariant var;
			pt.pt_id= id;
			if (pointMode == 0)
			{
				if( GetEntryLongnameById(&pt, entry_longname)==-1 )
					continue;
			}
			else
			{
				if( GetEntryNameById(&pt, entry_longname)==-1 )
					continue;
			}
			QStandardItem* item = new QStandardItem;
			item->setText(QString::fromLocal8Bit(entry_longname));
			item->setCheckable(true);
			var.setValue(pt);
			item->setData(var,Qt::UserRole);
			pointSourceModel->appendRow(item);
		}
	}
	for (int i=0;i<pointSourceModel->rowCount();i++)
	{
		POINTER	point = pointSourceModel->item(i,0)->data(Qt::UserRole).value<POINTER>();
		QString ptStr = QString("%1.%2.%3.%4").arg(point.stn_id).arg(point.dev_id).arg(point.data_type).arg(point.pt_id);
		hashPoint.value(ptStr).second == 0 ? pointSourceModel->item(i,0)->setCheckState(Qt::Unchecked) : pointSourceModel->item(i,0)->setCheckState(Qt::Checked);
	}
}
void CSetPointMaskTable::treeItemChanged(QTreeWidgetItem *item, int column)
{ 
	if(Qt::Checked == item->checkState(0))
	{  
		int count = item->childCount();     
		if (count > 0)
		{         
			for (int i = 0; i < count; i++)
			{
				item->child(i)->setCheckState(0, Qt::Checked);         
			}     
		}
		else
		{
			updateParentItem(item);
			selectAll();
		}
	}
	else if(Qt::Unchecked == item->checkState(0))
	{   
		int count = item->childCount();
		if (count > 0)
		{         
			for (int i = 0; i < count; i++)
			{        
				item->child(i)->setCheckState(0, Qt::Unchecked);
			}     
		}
		else
		{
			updateParentItem(item);
			selectOff();
		}
	}
}
void CSetPointMaskTable::updateParentItem(QTreeWidgetItem* item)
{    
	QTreeWidgetItem *parent = item->parent();     
	if (parent == NULL)
	{        
		return;     
	}     //选中的子节点个数     
	int selectedCount = 0;     
	int childCount = parent->childCount();     
	for (int i = 0; i < childCount; i++)
	{         
		QTreeWidgetItem *childItem = parent->child(i);         
		if (childItem->checkState(0) == Qt::Checked || childItem->checkState(0) == Qt::PartiallyChecked)
		{             
			selectedCount++;         
		}     
	}     
	if (selectedCount <= 0)
	{     //未选中状态         
		parent->setCheckState(0, Qt::Unchecked);     
	}
	else if(selectedCount > 0 && selectedCount < childCount)
	{     //部分选中状态         
		parent->setCheckState(0, Qt::PartiallyChecked);     
	}
	else if(selectedCount == childCount)
	{     //选中状态     
		parent->setCheckState(0, Qt::Checked); 
	}//    
	//changeFromUser = true;
}
void CSetPointMaskTable::updateParentItem()
{
	int selectedCount = 0;   
	int childCount = pointSourceModel->rowCount(); 
	if (pointSourceModel->rowCount() == 0)	return;
	for (int i=0;i<childCount;i++)
	{
		if (pointSourceModel->item(i,0)->checkState() == Qt::Checked || pointSourceModel->item(i,0)->checkState() == Qt::PartiallyChecked)
		{
			selectedCount++;
		}
	}
	if (selectedCount <= 0)
	{     //未选中状态         
		treeWidget->currentItem()->setCheckState(0, Qt::Unchecked);     
	}
	else if(selectedCount > 0 && selectedCount < childCount)
	{     //部分选中状态         
		treeWidget->currentItem()->setCheckState(0, Qt::PartiallyChecked);     
	}
	else if(selectedCount == childCount)
	{     //选中状态     
		treeWidget->currentItem()->setCheckState(0, Qt::Checked); 
	}//  
}
void CSetPointMaskTable::pointModeChange(int index)
{
	pointMode = index;
	displayStnDevTree();
}
void CSetPointMaskTable::pointShowTypeChange(int index)
{
	pointShowType = index;
	treeWidget->clearSelection();
	pointSourceModel->clear();
}
void CSetPointMaskTable::filterChanged(const QString &text)
{
	QRegExp regExp(text, Qt::CaseInsensitive); 
	pointModel->setFilterRegExp(regExp); 
}
void CSetPointMaskTable::pointClicked(const QModelIndex & index)
{	
	POINTER point = index.data(Qt::UserRole).value<POINTER>();
	QString ptStr = QString("%1.%2.%3.%4").arg(point.stn_id).arg(point.dev_id).arg(point.data_type).arg(point.pt_id);
	pointSourceModel->item(index.row(),0)->checkState() == Qt::Unchecked ? hashPoint[ptStr].second = 0 : hashPoint[ptStr].second = 1;
	updateParentItem();
}
void CSetPointMaskTable::selectAll()
{
	for (int i=0;i<pointSourceModel->rowCount();i++)
	{
		pointSourceModel->item(i,0)->setCheckState(Qt::Checked);
		POINTER	point = pointSourceModel->item(i,0)->data(Qt::UserRole).value<POINTER>();
		QString ptStr = QString("%1.%2.%3.%4").arg(point.stn_id).arg(point.dev_id).arg(point.data_type).arg(point.pt_id);
		hashPoint[ptStr].second = 1;
		treeWidget->currentItem()->setCheckState(0, Qt::Checked);
	}
}
void CSetPointMaskTable::selectOff()
{
	for (int i=0;i<pointSourceModel->rowCount();i++)
	{
		pointSourceModel->item(i,0)->setCheckState(Qt::Unchecked);
		POINTER	point = pointSourceModel->item(i,0)->data(Qt::UserRole).value<POINTER>();
		QString ptStr = QString("%1.%2.%3.%4").arg(point.stn_id).arg(point.dev_id).arg(point.data_type).arg(point.pt_id);
		hashPoint[ptStr].second = 0;
		treeWidget->currentItem()->setCheckState(0, Qt::Unchecked);
	}
}
void CSetPointMaskTable::selectInvert()
{
	for (int i=0;i<pointSourceModel->rowCount();i++)
	{
		if (pointSourceModel->item(i,0)->checkState() == Qt::Checked)
		{
			pointSourceModel->item(i,0)->setCheckState(Qt::Unchecked);
			POINTER	point = pointSourceModel->item(i,0)->data(Qt::UserRole).value<POINTER>();
			QString ptStr = QString("%1.%2.%3.%4").arg(point.stn_id).arg(point.dev_id).arg(point.data_type).arg(point.pt_id);
			hashPoint[ptStr].second = 0;
		}
		else
		{
			pointSourceModel->item(i,0)->setCheckState(Qt::Checked);
			POINTER	point = pointSourceModel->item(i,0)->data(Qt::UserRole).value<POINTER>();
			QString ptStr = QString("%1.%2.%3.%4").arg(point.stn_id).arg(point.dev_id).arg(point.data_type).arg(point.pt_id);
			hashPoint[ptStr].second = 1;
		}
	}
	updateParentItem();
}
void CSetPointMaskTable::saveAllChange()
{
	int chg_num = 0;
	POINTER	point;
	QHash<QString,QPair<short, short>>::const_iterator i = hashPoint.constBegin();
	while (i != hashPoint.constEnd()) 
	{
		if (i.value().first != i.value().second)
		{
			point.stn_id = i.key().split(".",QString::SkipEmptyParts).at(0).toInt();
			point.dev_id = i.key().split(".",QString::SkipEmptyParts).at(1).toInt();
			point.data_type = i.key().split(".",QString::SkipEmptyParts).at(2).toInt();
			point.pt_id = i.key().split(".",QString::SkipEmptyParts).at(3).toInt();
			BdcastQlt(point,i.value().second);
			if (chg_num++ >= 100)
			{
				chg_num = 0;
				MySleep(200);
			}
		}
		++i;
	}
	accept();
}
