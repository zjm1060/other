#include "pointname.h"

CPointNameTable::CPointNameTable(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setModal(true);
	//InitTreeView();
	treePointName->header()->setHidden(1);
	editPointName->setReadOnly(TRUE);
	connect(treePointName,SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(ItemSelected(QTreeWidgetItem *)));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(Enter()));
}
void CPointNameTable::loadChinese()
{
	QTranslator translator;
	translator.load("Teachermanage_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CPointNameTable::loadEnglish()
{
	QTranslator translator;
	translator.load("Teachermanage_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CPointNameTable::InitTreeView()
{
	char	stn_comment[STATION_COMMENT_SIZE];
	char	dev_comment[GROUP_COMMENT_SIZE];
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	char	entry_longname[ENTRY_LONGNAME_SIZE];
	char	text[128];
	UCHAR	dev_id, data_type;
	GULONG		stn_num,grp_num,rec_num,entry_num;
	int		i,id;
	POINTER	pt;
	QVariant var;

	treePointName->clear();
	GetStationNum(&stn_num);
	for(i=1; i<=stn_num; i++) 
	{  		
		UCHAR stn_id;
		GetStnIdByIndex(i, &stn_id);

		if( GetStnCommentById(stn_id,(char*)( stn_comment))==-1 )
			continue;

		sprintf(text, "%d. %s", stn_id, stn_comment);
		QTreeWidgetItem *itemStn = new QTreeWidgetItem(treePointName, QStringList(QString::fromLocal8Bit(text)));
		if(-1==GetGroupNumById(stn_id, &grp_num))
			continue;

		for(dev_id=1; dev_id<=grp_num; dev_id++) 
		{  	
			int	j;
			if(-1==GetGroupCommentById(stn_id, dev_id, dev_comment))
				continue;

			sprintf(text, "%d. %s", dev_id, dev_comment);
			QTreeWidgetItem *itemDev = new QTreeWidgetItem(itemStn, QStringList(QString::fromLocal8Bit(text)));
			if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
				continue;

			for(data_type=1,j=0; j<rec_num; data_type++) 
			{
				if(data_type>=MAX_DATATYPE_NUM)
					break;
				if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
					continue;

				GetRecLongnameByType(data_type, datatype_longname);
				j++;

				sprintf(text, "%d. %s", data_type, datatype_longname);
				QTreeWidgetItem *itemDataType = new QTreeWidgetItem(itemDev, QStringList(QString::fromLocal8Bit(text)));
				pt.stn_id= stn_id;
				pt.dev_id= dev_id;
				pt.data_type= data_type;
				for(id=0; id<entry_num; id++)
				{
					pt.pt_id= id;
					if(-1==GetEntryLongnameById(&pt, entry_longname))
						continue;

					sprintf(text, "%d. %s", id, entry_longname);
					QTreeWidgetItem *itemEntry = new QTreeWidgetItem(itemDataType, QStringList(QString::fromLocal8Bit(text)));
					var.setValue(pt);
					itemEntry->setData(1,0,var);
				}
			}
		}
	}
}

void CPointNameTable::SetSelected(const QString &selected_pt_name)
{
	
}

void CPointNameTable::ItemSelected(QTreeWidgetItem *curItem)
{
	POINTER pt;
	QVariant var;	
	char ptName[POINT_NAME_SIZE];
	var = curItem->data(1,0);
	pt = var.value<POINTER>();
	if(-1!=GetPtNameById(&pt, ptName, NULL))
	{
		curPointName = QString::fromLocal8Bit(ptName);
		editPointName->setText(curPointName);
	}		
}

void CPointNameTable::Enter()
{
	if (curPointName.isEmpty())
		return;
	emit sig_curPointName(curPointName);
	QStringList list;
	list = curPointName.split(".");
	P_entry_name = list.at(3);
	emit sig_curEntryName(P_entry_name);
	this->close();
}


void CPointNameTable::DispTreeView(short stn,short dev,short data_type)
{
	treePointName->clear();
	char	stn_comment[STATION_COMMENT_SIZE];
	char	dev_comment[GROUP_COMMENT_SIZE];
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	char	entry_longname[ENTRY_LONGNAME_SIZE];
	char	text[128];
	UCHAR	dev_id;
	GULONG		rec_num,entry_num;
	int		i,j,id;
	POINTER	pt;
	QVariant var;
	UCHAR stn_id;
	GetStnIdByIndex(stn, &stn_id);
	if( GetStnCommentById(stn_id,(char*)( stn_comment))==-1 )
		return;
	sprintf(text, "%d. %s", stn_id, stn_comment);
	QTreeWidgetItem *itemStn = new QTreeWidgetItem(treePointName, QStringList(QString::fromLocal8Bit(text)));
	dev_id = UCHAR(dev);
	if(-1==GetGroupCommentById(stn_id, dev_id, dev_comment))
		return;
	sprintf(text, "%d. %s", dev_id, dev_comment);
	QTreeWidgetItem *itemDev = new QTreeWidgetItem(itemStn, QStringList(QString::fromLocal8Bit(text)));
	if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
		return;
	if(data_type>=MAX_DATATYPE_NUM)
		return;
	if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
		return;
	GetRecLongnameByType((UCHAR)data_type, datatype_longname);
	sprintf(text, "%d. %s", data_type, datatype_longname);
	QTreeWidgetItem *itemDataType = new QTreeWidgetItem(itemDev, QStringList(QString::fromLocal8Bit(text)));
	for(j=0; j<rec_num;j++) 
	{	
		pt.stn_id= stn_id;
		pt.dev_id= dev_id;
		pt.data_type= data_type;
		for(id=0; id<entry_num; id++)
		{
			pt.pt_id= id;
			if(-1==GetEntryLongnameById(&pt, entry_longname))
				continue;
			else
				//strcpy(text,entry_longname);
			sprintf(text, "%d. %s", id, entry_longname);
			QTreeWidgetItem *itemEntry = new QTreeWidgetItem(itemDataType, QStringList(QString::fromLocal8Bit(text)));
			var.setValue(pt);
			itemEntry->setData(1,0,var);
		}
	}
}