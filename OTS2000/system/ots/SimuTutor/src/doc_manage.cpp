#include "doc_manage.h"

Doc_manage::Doc_manage(QWidget* parent)
	:QDialog(parent)
{
	setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	maxdepth = 0;
	is_root = true;
	new_doc = false;
	is_loadall = false;
	setControlAtt();
	docSourceModel = new QStandardItemModel;
	docModel = new QSortFilterProxyModel;
	docModel->setSourceModel(docSourceModel);
	docModel->setDynamicSortFilter(true);
	getDocType();
//	displayDoclist();
	connect(pushButton_adddoc,SIGNAL(clicked()),this,SLOT(on_pushButton_adddoc()));
	connect(pushButton_modifydoc,SIGNAL(clicked()),this,SLOT(on_pushButton_modifydoc()));
	connect(pushButton_deldoc,SIGNAL(clicked()),this,SLOT(on_pushButton_deldoc()));
	connect(pushButton_OK,SIGNAL(clicked()),this,SLOT(on_pushButton_OK()));
	connect(pushButton_CANCEL,SIGNAL(clicked()),this,SLOT(on_pushButton_CANCEL()));
	connect(pushButtonLoadAll,SIGNAL(clicked()),this,SLOT(on_pushButton_LoadAlldoc()));
	QDir Path;
	QString appPath = qApp->applicationDirPath();
	QString curDrive = appPath.mid(0,2);//得到当前程序的盘符 例如“C:”
	Path.setPath(curDrive+QString("\\home\\ems\\h9000\\ots"));//	QString strFile = Path.absoluteFilePath("oix.exe");
	QString strFile = Path.absoluteFilePath("doc");
	QString filePath = QString::fromLocal8Bit("/home/ems/h9000/ots/doc");
	QDir dir(strFile);
	if (!dir.exists())
	{
		PopupMessageBox(this,"找不到路径/home/ems/h9000/ots/doc",QMessageBox::Information);//qmessagebox 提示无文档
		return ;
	} 
	else
	{
		depth = 1;//记录文件深度
		maxdepth = depth;
		findFile(strFile);
		m_mymodel = new MytreeModel(file_list);
		treeView_docdir->setModel(m_mymodel);	//model里面什么都没有 会出现中断
		treeView_docdir->setSelectionMode(QAbstractItemView::ExtendedSelection); 
		connect(treeView_docdir,SIGNAL( doubleClicked ( const QModelIndex & )),this,SLOT( on_view_DBclicked ( const QModelIndex & )));
	}
}
void Doc_manage::showEvent(QShowEvent *event)
{
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	setdocModel();
	//comboFilterType->setCurrentIndex(-1);
	docModel->setFilterRegExp(QRegExp("/*"));
	event->accept();
}
void  Doc_manage::setLanguage(int language)
{	
	QTranslator translator;
	if (language == 0)
	{
		translator.load("SimuTutor_zh_CN");
	}
	else if (language == 1)
	{
		translator.load("SimuTutor_en");
	}
	qApp->installTranslator(&translator);
	retranslateUi(this);
		setdocModel();
}
void Doc_manage::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
/*	selecttasktable->loadChinese();*/
	//exceltable->loadChinese();
}
void Doc_manage::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
/*	selecttasktable->loadEnglish();*/
	//exceltable->loadEnglish();
}
void Doc_manage::displayDoclist()
{

}
void Doc_manage::setControlAtt()
{
	splitter->setChildrenCollapsible(true);
	int i = splitter->count();
	i = splitter->indexOf(groupBox_3);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	tableView->setEditTriggers(QTableView::NoEditTriggers);
	tableView->setSortingEnabled(true);
	tableView->horizontalHeader()->setStretchLastSection(true);
}
void Doc_manage::setdocModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_DOC);
	tableView->setModel(NULL);
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_DOC);
		qDebug("查询数据库失败!");
		//return;
	}
// 	if(data.rownum>3)
// 		progressDlg->setRange(0,data.rownum-1);
// 	else
// 		progressDlg->setRange(0,0);
	docSourceModel->clear();
	docSourceModel->setColumnCount(6);  
	docSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("文档 ID "));  
	docSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("文 档 名 称")); 
//	docSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("大小"));
	docSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("修 改 日 期"));
	docSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("适用工种"));
	docSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("学习时长"));
	docSourceModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("学分"));
	for (int i=0;i<data.rownum;i++)
	{
		QStandardItem* item0 = new QStandardItem(QString::number(data_doc[i].doc_id));
//		item0->setData(QVariant(data_doc[i].name));
		item0->setData(QVariant(data_doc[i].doc_id));
		QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_doc[i].name)); 
		item1->setData(QVariant(data_doc[i].name));
		QStandardItem* item2 = new QStandardItem(QString(data_doc[i].update_time));  
		item2->setData(QVariant(data_doc[i].update_time));
		DB_FMT data2;
		InitDbData(&data2,TABLE_USER_TYPE);
		data2.cond_flag.flag_user_type.user_type_id = TRUE;
		data2.cond_seg.seg_user_type.user_type_id = data_doc[i].user_type_id;
		QStandardItem* item3 ;
		if( SelectRecs(&data2) != TRUE)
		{		
			HdbLogWrite(data2.sql_str,data.sql_error);
			FreeDbData(&data2,TABLE_USER_TYPE);
			qDebug("查询数据库失败!");
			item3 = new QStandardItem(QString::fromLocal8Bit("")); 
			//return;
		}
		else
			item3 = new QStandardItem(QString::fromLocal8Bit(data_user_type[0].user_type_name)); 
		item3->setData(QVariant(data_doc[i].user_type_id));
		QStandardItem* item4 = new QStandardItem(QString::number(data_doc[i].studytime));  
		item4->setData(QVariant(data_doc[i].studytime));
		QStandardItem* item5 = new QStandardItem(QString::number(data_doc[i].study_points));
		item5->setData(QVariant(data_doc[i].study_points));
		item5->setData(QVariant(data_doc[i].doctype_id),4);
		item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		item3->setTextAlignment(Qt::AlignCenter);
		item4->setTextAlignment(Qt::AlignCenter);
		item5->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2 << item3 << item4 << item5; 
		docSourceModel->appendRow(items); 
	}
	tableView->setModel(docSourceModel);
	//tableView->hideColumn(3);
	selectionModel = tableView->selectionModel();
	tableView->resizeColumnsToContents();
	FreeDbData(&data,TABLE_DOC);
}
void Doc_manage::getDocType()
{
	DB_FMT data;
	InitDbData(&data,TABLE_DOC_TYPE);
	//调用接口函数，参数为DB_FMT结构体
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_DOC_TYPE);
		return;
	}
	else
	{
// 		if(-1==GetStnIdByIndex(i, &stnid))
// 			continue;
		for(int i = 0 ;i <= GetMaxIdInHdb(TABLE_DOC_TYPE);i++)
		{
			if( data_doc_type[i].doctype_id != i)
			{
				i++;
				continue;///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////DZY
			}
			comboBox_doctype->addItem(QString(data_doc_type[i].name),QVariant(data_doc_type[i].doctype_id));	
		}
	}
}
void Doc_manage::on_view_DBclicked ( const QModelIndex & index )
{
	QString strfilepath = m_mymodel->data(m_mymodel->index(index.row(),0,index.parent()),UserRole_DIRINFO).toString();//DM
	QString strfilename = m_mymodel->data(m_mymodel->index(index.row(),0,index.parent()), Qt::DisplayRole).toString(); 
	QString isdir =  m_mymodel->data(m_mymodel->index(index.row(),0,index.parent()),UserRole_DIRINFO2).toString(); ////???????????中秋后解决
	if (!strfilepath.isEmpty()&&isdir=="notdir")
	{
		if(new_doc)
		{
			for (int i  = 0 ; i<docSourceModel->rowCount();i++)
			{
				if (docSourceModel->item(i,1)->data().toString() == strfilename)
				{
					PopupMessageBox(this,"该文档已存在在数据库中",0,false);
					return;
				}
			}
		}
		strcpy(m_doc.name,strfilename.toLocal8Bit().data());
		QString strfiletime = m_mymodel->data(m_mymodel->index(index.row(),2,index.parent()), Qt::DisplayRole).toString(); 
		strcpy(m_doc.update_time,strfiletime.toLocal8Bit().data());
		docname =strfilename;
		updatetime = strfiletime;
		label_docname->setText(strfilename);
		//PopupMessageBox(this,m_doc.name,QMessageBox::Information);
		//PopupMessageBox(this,m_doc.update_time,QMessageBox::Information);
		//m_doc.doc_app = 
	}
	else 
	{
		strcpy(m_doc.name,"notdir");
		//PopupMessageBox(this,"请选择正确的文档路径",QMessageBox::Information); 
		return ;
	}
}

void Doc_manage::findFile(const QString & path)
{
	if (is_root)
	{
		QFileInfo fi(path);
		QFileitem root_file;
		root_file.file = fi;
		root_file.depth = 0;
		file_list.append(root_file);
		is_root = false;
	}
	QDir dir(path);
	dir.setFilter(QDir::Dirs|QDir::Files);
	dir.setSorting(QDir::DirsFirst);
	QFileInfoList list = dir.entryInfoList();
	QStringList infolist = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
	int i = 0;
	do{
		QFileInfo fileInfo = list.at(i);
		if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")
		{
			i++;
			continue;
		}
		bool bisDir = fileInfo.isDir();
		if(bisDir)
		{
			QFileitem m_file;
			m_file.file = fileInfo;
			m_file.depth = depth;
			QString group = fileInfo.group();
			file_list.append(m_file);
			depth++;
			if (maxdepth<depth)
			{
				maxdepth = depth;
			}
			findFile(fileInfo.filePath());
		}
		else
		{     
			for(int m = 0; m <infolist.size(); m++)
			{
				QFileitem m_file;
				QString t1 = (fileInfo.absoluteFilePath()).remove(fileInfo.fileName()) ;
				QFileInfo file(t1+ infolist.at(m));
				m_file.file =file;
				m_file.depth = depth;
				file_list.append(m_file);
				if(is_loadall)
				{
					//dzy
					DB_FMT data;
					data.table_id = TABLE_DOC;
					InitDbData(&data,data.table_id);
					data.cond_flag.flag_doc.name =TRUE;
					strcpy(data.cond_seg.seg_doc.name,file.fileName().toLocal8Bit().data());
					if (SelectRecs(&data)==int(TRUE) && data.rownum > 0)
					{
						continue;
					}
					else if (data.rownum==0)
					{
						data.flag.flag_doc.doc_id = TRUE;
						data.flag.flag_doc.name = TRUE;
						data.flag.flag_doc.update_time = TRUE;
						data.seg.seg_doc.doc_id = GetMaxIdInHdb(TABLE_DOC)+1;
						strcpy(data.seg.seg_doc.name,file.fileName().toLocal8Bit().data());
						GetCurTimeStr(data.seg.seg_doc.update_time);
						InsertOneRec(&data);
					}
					//dzy
				}
			}
			depth--;			
			break;
		}
		i++;
		depth = 1;
	}while(i < list.size());
}
void Doc_manage::on_pushButton_adddoc()
{
	new_doc = true;
	frame->setEnabled(true);
	groupBox_3->setEnabled(false);
}
void Doc_manage::on_pushButton_modifydoc()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
//	QString filename(m_doc.name);
	if (indexes.size()== 0/*|| filename=="notdir"*/)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请正确选择要修改的文档!"),QMessageBox::Ok);
		return;
	}
	else
	{
		int row = indexes.at(0).row();
		QStandardItem *item = docSourceModel->itemFromIndex(docModel->mapToSource(docModel->index(row,0)));
		m_doc.doc_id = item->data().toInt();
		item = docSourceModel->itemFromIndex(docModel->mapToSource(docModel->index(row,1)));
		strcpy(m_doc.name,item->data().toString().toLocal8Bit().data());
		item = docSourceModel->itemFromIndex(docModel->mapToSource(docModel->index(row,2)));
		strcpy(m_doc.update_time,item->data().toString().toLocal8Bit().data());
		item = docSourceModel->itemFromIndex(docModel->mapToSource(docModel->index(row,3)));
		m_doc.user_type_id = item->data().toInt();
		item = docSourceModel->itemFromIndex(docModel->mapToSource(docModel->index(row,4)));
		m_doc.studytime = item->data().toInt();
		item = docSourceModel->itemFromIndex(docModel->mapToSource(docModel->index(row,5)));
		m_doc.study_points =  item->data().toInt();

		comboBox_usertype->setCurrentIndex(m_doc.user_type_id+1);
		doubleSpinBox_studytime->setValue(double(m_doc.studytime)/60.0);
		spinBox_studypoints->setValue(m_doc.study_points );
		label_docname->setText(QString::fromLocal8Bit(m_doc.name));
		frame->setEnabled(true);
		groupBox_docdir->setEnabled(false);
	}
}
void Doc_manage::on_pushButton_deldoc()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	//	QString filename(m_doc.name);
	if (indexes.size()== 0/*|| filename=="notdir"*/)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请正确选择要删除的文档!"),QMessageBox::Ok);
		return;
	}
	else
	{
		int row = indexes.at(0).row();
		QStandardItem *item = docSourceModel->itemFromIndex(docModel->mapToSource(docModel->index(row,0)));
		m_doc.doc_id = item->data().toInt();
		DB_FMT data;
		InitDbData(&data,TABLE_DOC);
		data.cond_flag.flag_doc.doc_id = TRUE;
		data.cond_seg.seg_doc.doc_id = m_doc.doc_id;
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_DOC);
			cleardocinfo();
			setdocModel();
			return;
		}	
		FreeDbData(&data,TABLE_DOC);
		cleardocinfo();
		setdocModel();
	}
}
void Doc_manage::on_pushButton_OK()
{
	if (m_doc.name == "NULL"|| m_doc.name =="notdir")
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请正确选择要修改的文档!"),QMessageBox::Ok);
		return;
	}
	if (new_doc)
	{		
		DB_FMT data;
		InitDbData(&data,TABLE_DOC);
		m_doc.doc_id = GetMaxIdInHdb(TABLE_DOC)+1;
		if (comboBox_doctype->currentIndex()>0)
		{
			m_doc.doctype_id = comboBox_doctype->itemData(comboBox_doctype->currentIndex()).toInt();
		}
		else
			m_doc.doctype_id = -1;
		if(comboBox_usertype->currentIndex() > 0 )
		{
			m_doc.user_type_id = comboBox_usertype->currentIndex() - 1;
		}
		else
			m_doc.user_type_id = -1;
		if (doubleSpinBox_studytime->value()>0)
		{
			m_doc.studytime = int(doubleSpinBox_studytime->value()*60);
		}
		else
			m_doc.studytime = 0;
		if (spinBox_studypoints->value()>0)
		{		
			m_doc.study_points = spinBox_studypoints->value();
		}
		else
			m_doc.study_points = 0;

		data.flag.flag_doc.doc_id = TRUE;
		data.flag.flag_doc.name = TRUE;
		data.flag.flag_doc.doctype_id = TRUE;
		data.flag.flag_doc.user_type_id = TRUE;
		data.flag.flag_doc.update_time = TRUE;
		data.flag.flag_doc.study_points = TRUE;
		data.flag.flag_doc.studytime = TRUE;
		data.seg.seg_doc.doc_id = m_doc.doc_id;
		//strcpy( data.seg.seg_doc.name , m_doc.name);
		strcpy( data.seg.seg_doc.name ,docname.toLocal8Bit().data());
		data.seg.seg_doc.doctype_id = m_doc.doctype_id;
		data.seg.seg_doc.user_type_id = m_doc.user_type_id;
		//PopupMessageBox(this, m_doc.update_time,QMessageBox::Information);
		//strcpy( data.seg.seg_doc.update_time , m_doc.update_time);
		strcpy( data.seg.seg_doc.update_time ,updatetime.toLocal8Bit().data());
		data.seg.seg_doc.study_points = m_doc.study_points;
		data.seg.seg_doc.studytime = m_doc.studytime;
		if( InsertOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_DOC);
			cleardocinfo();
			setdocModel();
			return;
		}
	}
	else
	{
		DB_FMT data;
		InitDbData(&data,TABLE_DOC);
		if (comboBox_doctype->currentIndex()>0)
		{
			m_doc.doctype_id = comboBox_doctype->itemData(comboBox_doctype->currentIndex()).toInt();
			data.flag.flag_doc.doctype_id = TRUE;
			data.seg.seg_doc.doctype_id = m_doc.doctype_id;
		}
		if(comboBox_usertype->currentIndex() > 0 )
		{
			m_doc.user_type_id = comboBox_usertype->currentIndex() - 1;
			data.flag.flag_doc.user_type_id = TRUE;
			data.seg.seg_doc.user_type_id = m_doc.user_type_id;
		}
		if (doubleSpinBox_studytime->value()>0)
		{
			m_doc.studytime = int(doubleSpinBox_studytime->value()*60);
			data.flag.flag_doc.studytime = TRUE;

			data.seg.seg_doc.studytime = m_doc.studytime;
		}
		if (spinBox_studypoints->value()>0)
		{		
			m_doc.study_points = spinBox_studypoints->value();
			data.flag.flag_doc.study_points = TRUE;
			data.seg.seg_doc.study_points = m_doc.study_points;
		}
		data.flag.flag_doc.update_time = TRUE;		
		strcpy( data.seg.seg_doc.update_time , m_doc.update_time);

		if( UpdateOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_DOC);
			cleardocinfo();
			setdocModel();
			return;
		}
	}
	cleardocinfo();
	setdocModel();
}
void Doc_manage::on_pushButton_LoadAlldoc()
{
	is_loadall = true;
	QString filePath = QString::fromLocal8Bit("/home/ems/h9000/ots/doc");
	findFile(filePath);
	setdocModel();
	is_loadall = false;
}
void Doc_manage::on_pushButton_CANCEL()
{
	cleardocinfo();
}
void Doc_manage::cleardocinfo()
{
	frame->setEnabled(false);
	label_docname->setText(QString::fromLocal8Bit("未选定文档（双击文档选中）"));
	groupBox_3->setEnabled(true);
	new_doc = false;
	m_doc.doc_id = -1;
	strcpy(m_doc.name,"NULL");
	comboBox_doctype->setCurrentIndex(0);
	comboBox_usertype->setCurrentIndex(0);
	doubleSpinBox_studytime->setValue(0.0);
	spinBox_studypoints->setValue(0);
}
MytreeModel::MytreeModel(QFileitemList &data,QObject *parent)
:QAbstractItemModel(parent)
{
	initModel();
	setupModelData(data);
}

MytreeModel::~MytreeModel()
{
	delete rootItem;
}

QModelIndex MytreeModel::index(int row, int column,  const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}
QModelIndex MytreeModel::parent(const QModelIndex &child)const
{
	if (!child.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(child.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}
int MytreeModel::rowCount(const QModelIndex &parent ) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}
int MytreeModel::columnCount(const QModelIndex &parent ) const 
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}
Qt::ItemFlags MytreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant MytreeModel::data(const QModelIndex &index, int role) const 
{
	if (!index.isValid())
		return QVariant();

	if (role== UserRole_DIRINFO||role== UserRole_DIRINFO2||role== UserRole_Studyinfo||role == UserRole_Headerdata)
	{
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		return item->data(index.column(),role);
	}
	else if (role != Qt::DisplayRole)
		return QVariant();
	else {
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		return item->data(index.column(),role);
	}

}
// QFileInfoList Mydoc::GetFileList(QString path)
// {
// 	QDir dir(path);
// 	QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
// 	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
// 
// 	for(int i = 0; i != folder_list.size(); i++)
// 	{
// 		QString name = folder_list.at(i).absoluteFilePath();
// 		QFileInfoList child_file_list = GetFileList(name);
// 		file_list.append(child_file_list);
// 	}
// 
// 	return file_list;
// }


void MytreeModel::initModel()
{
	// 	QStringList filters;
	// 	filters << "*.doc" << "*.docx" << "*.ppt";
	// 	m_dirmodel->setNameFilters(filters);
//	this->setColumnCount(3);  
// 	this->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("文 件 名"),0);
// 	this->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("大 小"),0);
// 	this->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("日 期"),0);
	//
}
// void  MytreeModel::clear()
// {
// 	QAbstractItemModel::clear();
// }

void MytreeModel::setupModelData(const QFileitemList &lines)
{
	QList<TreeItem*> parents;
	QList<int> indentations;

	rootItem = new TreeItem(lines.at(0));

	parents << rootItem;
	indentations << 0;

	int number = 0;

	while (number < lines.count()) {//逐行 
		int position = 0;	//POSITION 记录缩进
		while (position < lines[number].depth&&maxdepth>0) 
		{//逐项
			position++;
		}
		QString lineData = lines[number].file.fileName();
		if (!lineData.isEmpty()) {
			if (position > indentations.last()) {
				if (parents.last()->childCount() > 0) {
					parents << parents.last()->child(parents.last()->childCount()-1);
					indentations << position;
				}
			} else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}
			parents.last()->appendChild(new TreeItem(lines[number], parents.last()));
		}
		number++;
	}
}

//! [0]
TreeItem::TreeItem(const QFileitem &data, TreeItem *parent)
{
	parentItem = parent;
	itemData.file = data.file;
	itemData.depth = data.depth;
}
//! [0]

//! [1]
TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}
//! [1]

//! [2]
void TreeItem::appendChild(TreeItem *item)
{
	childItems.append(item);
}
//! [2]

//! [3]
TreeItem *TreeItem::child(int row)
{
	return childItems.value(row);
}
//! [3]

//! [4]
int TreeItem::childCount() const
{
	return childItems.count();
}
//! [4]

//! [5]
int TreeItem::columnCount() const
{
	//return itemData.count();
	return 2;
}
//! [5]

//! [6]
QString TreeItem::data(int column,int role) const
{
	if(role == UserRole_Headerdata)
	{
		switch (column)
		{
		case  0:
			return 	QString::fromLocal8Bit("文 件 名");
			break;
		case  1:
			return	QString::fromLocal8Bit("大 小");
			break;
// 		case 2:
// 			return	QString::fromLocal8Bit("日 期");
// 			break;
		default:
			break;
		}
	}
	if(role == Qt::DisplayRole)
	{
		int i = itemData.file.size()/1024;
		QString m_size;
		m_size.setNum(i);
		switch (column)
		{
		case  0:
			return 	 itemData.file.fileName();
			//return itemData.file.absoluteFilePath();
			break;
		case  1:
			return 	m_size+"kb" ;
			break;
// 		case 2:
// 			return 	 itemData.file.lastModified().toString("yyyy-MM-dd hh:mm:ss");  
// 			break;
		default:
			break;
		}
	}
	if (role == UserRole_DIRINFO)
	{
		//bool isdir = itemData.file.isDir();
		switch (column)
		{
		case  0:
			return itemData.file.absoluteFilePath();
			//return itemData.file.absoluteFilePath();
			break;
		case  1:
			return 	 itemData.file.fileName();
			break;
// 		case 2:
// 			if ( isdir == true)
// 			{
// 				return 	 QString("isdir");
// 			}
// 			else
// 				return  	 QString("notdir");			
// 			break;
		default:
			break;
		}
	}
		if (role == UserRole_DIRINFO2)
		{
			bool isdir = itemData.file.isDir();
			switch (column)
			{
				case 0:
					if ( isdir == true)
					{
						return 	 QString("isdir");
					}
					else
						return  	 QString("notdir");			
					break;
				default:
					break;
			}
	}
	//	return itemData.value(column);
}
//! [6]

//! [7]
TreeItem *TreeItem::parent()
{
	return parentItem;
}
//! [7]

//! [8]
int TreeItem::row() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}
//! [8]


Study_plan::Study_plan(QWidget* parent)
:QDialog(parent)
{
	setupUi(this);
 	this->setAttribute(Qt::WA_DeleteOnClose);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	flag_add =flag_modify = false;
 	setControlAtt();
 	planSourceModel = new QStandardItemModel;
 	planModel = new QSortFilterProxyModel;
 	planModel->setSourceModel(planSourceModel);
 	planModel->setDynamicSortFilter(true);
	init_doc_comobox();
	clearplaninfo();
	setplanModel();
// 	displayDoclist();
	connect(pushButtonAdd,SIGNAL(clicked()),this,SLOT(on_pushButton_addplan()));
	connect(pushButtonModify,SIGNAL(clicked()),this,SLOT(on_pushButton_modifyplan()));
	connect(pushButtonDel,SIGNAL(clicked()),this,SLOT(on_pushButton_delplan()));
	connect(pushButton_OK,SIGNAL(clicked()),this,SLOT(on_pushButton_OK()));
	connect(pushButton_EXIT,SIGNAL(clicked()),this,SLOT(on_pushButton_EXIT()));
}
void Study_plan::showEvent(QShowEvent *event)
{
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}
	event->accept();
}
void Study_plan::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void Study_plan::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void Study_plan::setControlAtt()
{
// 	splitter->setChildrenCollapsible(true);
// 	int i = splitter->count();
// 	i = splitter->indexOf(groupBox_3);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	tableView->setEditTriggers(QTableView::NoEditTriggers);
	tableView->setSortingEnabled(true);
	tableView->horizontalHeader()->setStretchLastSection(true);
}
void Study_plan::init_doc_comobox()
{
	DB_FMT data;
	InitDbData(&data,TABLE_DOC);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	comboBoxDoc->clear();
	if(data.rownum<=0)
		return;
	comboBoxDoc->insertItem(0,QString::fromLocal8Bit("请选择"));
	for (int i=1;i<data.rownum+1;i++)
	{
		comboBoxDoc->insertItem(i,QString::fromLocal8Bit(data_doc[i].name));
		comboBoxDoc->setItemData(i,QVariant(data_doc[i].doc_id));
	}
}
void  Study_plan::clearplaninfo()
{
	lineEdit_planname->setText("");
	comboBoxDoc->setCurrentIndex(0);
	comboBox_usertype->setCurrentIndex(0);
	doubleSpinBox_studytime->setValue(0);
	spinBox_studypoints->setValue(0);
	flag_add = flag_modify = false;
	tableView->setEnabled(true);
}
void Study_plan::setplanModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_STUDY_PLAN);
	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	tableView->setModel(NULL);	
	if(data.rownum<=0)
		return;

	if( SelectRecs(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("查询数据库失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_STUDY_PLAN);
		qDebug("查询数据库失败!");
		//return;
	}
	planSourceModel->clear();
	planSourceModel->setColumnCount(6);  
	planSourceModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("计划ID "));  
	planSourceModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("计 划 名 称")); 
	planSourceModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("适用工种"));
	//planSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("学习类型"));
	planSourceModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("学习内容"));
	planSourceModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("学习时长(分钟)"));
	planSourceModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("学分"));
	for (int i=0;i<data.rownum;i++)
	{
		QStandardItem* item0 = new QStandardItem(QString::number(data_study_plan[i].plan_id));
		//		item0->setData(QVariant(data_doc[i].name));
		item0->setData(QVariant(data_study_plan[i].plan_id));
		QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit(data_study_plan[i].name)); 
		item1->setData(QVariant(data_study_plan[i].name));
		DB_FMT data2;
		InitDbData(&data2,TABLE_USER_TYPE);
		data2.cond_flag.flag_user_type.user_type_id = TRUE;
		data2.cond_seg.seg_user_type.user_type_id = data_study_plan[i].user_type_id;
		QStandardItem* item2 ;
		if( SelectRecs(&data2) != TRUE)
		{		
			HdbLogWrite(data2.sql_str,data.sql_error);
			FreeDbData(&data2,TABLE_USER_TYPE);
			qDebug("查询数据库失败!");
			item2 = new QStandardItem(QString::fromLocal8Bit("未查到相关用户类")); 
			//return;
		}
		else
			item2 = new QStandardItem(QString::fromLocal8Bit(data_user_type[0].user_type_name)); 
		item2->setData(QVariant(data_user_type[0].user_type_id));
		InitDbData(&data2,TABLE_DOC);
		data2.cond_flag.flag_doc.doc_id = TRUE;
		data2.cond_seg.seg_doc.doc_id = data_study_plan[i].doc_id;
		QStandardItem* item3 ;
		if( SelectRecs(&data2) != TRUE)
		{		
			HdbLogWrite(data2.sql_str,data.sql_error);
			FreeDbData(&data2,TABLE_DOC);
			qDebug("查询数据库失败!");
			item3 = new QStandardItem(QString::fromLocal8Bit("未查到相关文档")); 
			//return;
		}
		else
			item3 = new QStandardItem(QString::fromLocal8Bit(data_doc[0].name)); 
		item3->setData(QVariant(data_doc[0].doc_id));
		QStandardItem* item4 = new QStandardItem(QString::number(data_study_plan[i].studytime));
		item4->setData(QVariant(data_study_plan[i].studytime));
		QStandardItem* item5 = new QStandardItem(QString::number(data_study_plan[i].study_points));
		item5->setData(QVariant(data_study_plan[i].study_points));
		//item5->setData(QVariant(data_study_plan[i].doctype_id),4);
		item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		item3->setTextAlignment(Qt::AlignCenter);
		item4->setTextAlignment(Qt::AlignCenter);
		item5->setTextAlignment(Qt::AlignCenter);
		QList<QStandardItem*> items;  
		items << item0 << item1 << item2 << item3 << item4 << item5; 
		planSourceModel->appendRow(items); 
	}
	tableView->setModel(planSourceModel);
	//tableView->hideColumn(3);
	selectionModel = tableView->selectionModel();
	tableView->resizeColumnsToContents();
	tableView->horizontalHeader()->setStretchLastSection(true);
	FreeDbData(&data,TABLE_DOC);
}

void Study_plan::on_pushButton_addplan()
{
	clearplaninfo();
	flag_add = true;
	flag_modify = false;
}
void Study_plan::on_pushButton_modifyplan()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	if (indexes.size()== 0/*|| filename=="notdir"*/)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请正确选择要修改的计划!"),QMessageBox::Ok);
		return;
	}
	else
	{
		flag_add = false;
		flag_modify = true;
		tableView->setEnabled(false);
		int row = indexes.at(0).row();
		QStandardItem *item = planSourceModel->itemFromIndex(planModel->mapToSource(planModel->index(row,0)));
		m_plan.plan_id = item->data().toInt();
		item = planSourceModel->itemFromIndex(planModel->mapToSource(planModel->index(row,1)));
		strcpy(m_plan.name,item->data().toString().toLocal8Bit().data());
		lineEdit_planname->setText(QString::fromLocal8Bit(m_plan.name));
		item = planSourceModel->itemFromIndex(planModel->mapToSource(planModel->index(row,2)));
		m_plan.user_type_id =  item->data().toInt();
		comboBox_usertype->setCurrentIndex(m_plan.user_type_id+1);
		item = planSourceModel->itemFromIndex(planModel->mapToSource(planModel->index(row,3)));
		m_plan.doc_id = item->data().toInt();
		comboBoxDoc->setCurrentIndex(0);
		for(int d_i = 0;d_i<comboBoxDoc->count();d_i++)
		{
			if (comboBoxDoc->itemData(d_i).toInt()==m_plan.doc_id )
			{
				comboBoxDoc->setCurrentIndex(d_i);
			}
		}
  		item = planSourceModel->itemFromIndex(planModel->mapToSource(planModel->index(row,4)));
		m_plan.studytime = item->data().toInt();
		doubleSpinBox_studytime->setValue(double(m_plan.studytime)/60.0);
		item = planSourceModel->itemFromIndex(planModel->mapToSource(planModel->index(row,5)));
		m_plan.study_points = item->data().toInt();
		spinBox_studypoints->setValue(m_plan.study_points );
	}
}
void Study_plan::on_pushButton_delplan()
{
	QModelIndexList indexes = selectionModel->selectedRows(0);
	//	QString filename(m_doc.name);
	if (indexes.size()== 0/*|| filename=="notdir"*/)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请正确选择要删除的计划!"),QMessageBox::Ok);
		return;
	}
	else
	{
		int row = indexes.at(0).row();
		QStandardItem *item = planSourceModel->itemFromIndex(planModel->mapToSource(planModel->index(row,0)));
		m_plan.plan_id = item->data().toInt();
		DB_FMT data;
		InitDbData(&data,TABLE_STUDY_PLAN);
		data.cond_flag.flag_study_plan.plan_id = TRUE;
		data.cond_seg.seg_study_plan.plan_id = m_plan.plan_id;
		if( DeleteOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_STUDY_PLAN);
			//cleardocinfo();
			setplanModel();
			return;
		}	
		FreeDbData(&data,TABLE_STUDY_PLAN);
		clearplaninfo();
		setplanModel();
	}
}
void Study_plan::on_pushButton_OK()
{
	strcpy(m_plan.name,lineEdit_planname->text().toLocal8Bit().data());
	if (flag_modify/*&&m_plan.name == "NULL"*/)
	{
		QModelIndexList indexes = selectionModel->selectedRows(0);
		if (indexes.size()== 0/*|| filename=="notdir"*/)
		{
			QMessageBox().warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请正确选择要修改的计划!"),QMessageBox::Ok);
			return;
		}
	}
	if (flag_add)
	{		
		DB_FMT data;
		InitDbData(&data,TABLE_STUDY_PLAN);
		m_plan.plan_id = GetMaxIdInHdb(TABLE_STUDY_PLAN)+1;
		if(comboBoxDoc->currentIndex() > 0 )
		{
			m_plan.doc_id = comboBoxDoc->itemData(comboBoxDoc->currentIndex()).toInt();
		}
		else
			m_plan.doc_id = -1;
		if(comboBox_usertype->currentIndex() > 0 )
		{
			m_plan.user_type_id = comboBox_usertype->currentIndex() - 1;
		}
		else
			m_plan.user_type_id = -1;
		if (doubleSpinBox_studytime->value()>0)
		{
			m_plan.studytime = int(doubleSpinBox_studytime->value()*60);
		}
		else
			m_plan.studytime = 0;
		if (spinBox_studypoints->value()>0)
		{		
			m_plan.study_points = spinBox_studypoints->value();
		}
		else
			m_plan.study_points = 0;

		data.flag.flag_study_plan.plan_id = TRUE;
		data.flag.flag_study_plan.name = TRUE;
		data.flag.flag_study_plan.doc_id = TRUE;
		data.flag.flag_study_plan.user_type_id = TRUE;
		data.flag.flag_study_plan.study_points = TRUE;
		data.flag.flag_study_plan.studytime = TRUE;
		data.seg.seg_study_plan.plan_id =  m_plan.plan_id;
		data.seg.seg_study_plan.doc_id = m_plan.doc_id;
		//strcpy( data.seg.seg_doc.name , m_doc.name);
		strcpy( data.seg.seg_study_plan.name ,m_plan.name);
		data.seg.seg_study_plan.user_type_id = m_plan.user_type_id;
		data.seg.seg_study_plan.study_points = m_plan.study_points;
		data.seg.seg_study_plan.studytime = m_plan.studytime;
		if( InsertOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_STUDY_PLAN);
			clearplaninfo();
			setplanModel();
			return;
		}
	}
	else if(flag_modify)
	{
		QModelIndexList indexes = selectionModel->selectedRows(0);
		int row = indexes.at(0).row();
		QStandardItem *item = planSourceModel->itemFromIndex(planModel->mapToSource(planModel->index(row,0)));
		m_plan.plan_id = item->data().toInt();

		DB_FMT data;
		InitDbData(&data,TABLE_STUDY_PLAN);
		if(comboBoxDoc->currentIndex() > 0 )
		{
			m_plan.doc_id = comboBoxDoc->itemData(comboBoxDoc->currentIndex()).toInt();
		}
		else
			m_plan.doc_id = -1;
		if(comboBox_usertype->currentIndex() > 0 )
		{
			m_plan.user_type_id = comboBox_usertype->currentIndex() - 1;
		}

		if (doubleSpinBox_studytime->value()>0)
		{
			m_plan.studytime = int(doubleSpinBox_studytime->value()*60);
		}
		else
			m_plan.studytime = 0;
		if (spinBox_studypoints->value()>0)
		{		
			m_plan.study_points = spinBox_studypoints->value();
		}
		else
			m_plan.study_points = 0;

		data.cond_flag.flag_study_plan.plan_id = TRUE;
		data.cond_seg.seg_study_plan.plan_id = m_plan.plan_id;
		data.flag.flag_study_plan.name = TRUE;
		data.flag.flag_study_plan.doc_id = TRUE;
		data.flag.flag_study_plan.user_type_id = TRUE;
		data.flag.flag_study_plan.study_points = TRUE;
		data.flag.flag_study_plan.studytime = TRUE;
		data.seg.seg_study_plan.doc_id = m_plan.doc_id;
		//strcpy( data.seg.seg_doc.name , m_doc.name);
		strcpy( data.seg.seg_study_plan.name ,m_plan.name);
		data.seg.seg_study_plan.user_type_id = m_plan.user_type_id;
		data.seg.seg_study_plan.study_points = m_plan.study_points;
		data.seg.seg_study_plan.studytime = m_plan.studytime;
		if( UpdateOneRec(&data) != TRUE)
		{
			//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_STUDY_PLAN);
			clearplaninfo();
			setplanModel();
			return;
		}
 	}
	clearplaninfo();
	setplanModel();
}

void Study_plan::on_pushButton_EXIT()
{
	this->close();
}