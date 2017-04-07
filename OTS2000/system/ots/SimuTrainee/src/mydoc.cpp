#include "mydoc.h"

Mydoc::Mydoc(QWidget* parent)
:QWidget(parent)
{
	setupUi(this);
	dlg_created_flag[MYDOCDLG] = true;
	this->setAttribute(Qt::WA_DeleteOnClose);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	pOfficeContainer	= new OfficeContainer(this);
	QVBoxLayout *vly0010 = new QVBoxLayout(widget);
	vly0010->addWidget(pOfficeContainer,1);
	widget->setLayout(vly0010);

	m_Timer = new QTimer;
	timecount = 0 ;
	lcdNumber_time->display("00:00:00");


   QWidget::installEventFilter(this);//为这个窗口安装过滤器 
	flag_windowact = true;
	m_Timer_mousestop = new QTimer;
	cout_mouseunmoved = 0;			
	m_Timer_mousestop->start(1000);

	groupbox_w = groupBox->width();
	maxdepth = 0;
	is_root = true;
	QDir Path;
	QString appPath = qApp->applicationDirPath();
	QString curDrive = appPath.mid(0,2);//得到当前程序的盘符 例如“C:”
	Path.setPath(curDrive+QString("\\home\\ems\\h9000\\ots"));//	QString strFile = Path.absoluteFilePath("oix.exe");
	QString strFile = Path.absoluteFilePath("doc");
	QString filePath = QString::fromLocal8Bit("/home/ems/h9000/ots/doc");
	QDir dir(strFile);

	initdata();
	if (!dir.exists())
	{
		PopupMessageBox(this,"找不到文档打开路径：/home/ems/h9000/ots/doc",QMessageBox::Information);//qmessagebox 提示无文档
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
		treeView_docdir->setColumnHidden(1,true);
		treeView_docdir->setColumnHidden(2,true);
		treeView_docdir->resizeColumnToContents(0);
		connect(treeView_docdir,SIGNAL( doubleClicked ( const QModelIndex & )),this,SLOT( on_view_doubleClicked ( const QModelIndex & )));
		connect(pushButtonStudy,SIGNAL( clicked ()),this,SLOT( on_pushButtonStudy_Clicked ()));
		connect(pushButton_MystudyRec,SIGNAL(clicked()),this,SLOT( on_pushButton_MystudyRec_Clicked ()));
		connect(pushButton_finishdoc,SIGNAL(clicked()),this,SLOT( on_pushButton_finishdoc_Clicked ()));	
		connect(m_Timer,SIGNAL(timeout()),this,SLOT( timeplus()));
		connect(m_Timer_mousestop,SIGNAL(timeout()),this,SLOT( timeplus_mouse()));
		connect(pushButton_collapse,SIGNAL(clicked()),this,SLOT(on_collapse()));
// 	treeView_docdir->expand(index);
// 	treeView_docdir->scrollTo(index); 
//	treeView_docdir->resizeColumnToContents(0);
//	treeView_docdir->setRootIsDecorated(false);
	}
}	
Mydoc::~Mydoc()
{
	dlg_created_flag[MYDOCDLG] = false;
}
void Mydoc::initdata()
{
	//init data:
	timecount = 0;
	m_study_plan.doc_id = -1;
	m_study_plan.plan_id = -1;
	m_study_plan.studytime = 0;
	m_doc.doc_id = -1;
	label_docname->setText("");
}
void Mydoc::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTrainee_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void Mydoc::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTrainee_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void  Mydoc::setLanguage(int language)
{	
	QTranslator translator;
	if (language == 0)
	{
		translator.load("SimuTrainee_zh_CN");
	}
	else if (language == 1)
	{
		translator.load("SimuTrainee_en");
	}
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void Mydoc::showEvent(QShowEvent *event)
{
// 	if (flag_langueage == 0)
// 	{
// 		loadChinese();
// 	}
// 	else if (flag_langueage == 1)
// 	{
// 		loadEnglish();
// 	}
	event->accept();
}

bool Mydoc::eventFilter(QObject *watched, QEvent *event) 
{ 
	if( watched == this ) 
	{ 
		//窗口停用，变为不活动的窗口 
		if(QEvent::WindowDeactivate == event->type()) 
		{ 
			pausetimer();  
			return true ; 
		} 
		else if (QEvent::MouseMove == event->type()||QEvent::WindowActivate == event->type()||QEvent::KeyPress == event->type()||QEvent::HoverEnter== event->type()||QEvent::FocusIn== event->type())
		{	
			cout_mouseunmoved = 0;
			continuetimer();			
			return true ; 
		}
// 		else if(QEvent::WindowActivate == event->type()) 
// 		{ 
// 			continuetimer();
// 			return true ; 
// 		} 
		else 
		{
			return false ; 
		}
	} 
	return false ; 
} 
void  Mydoc::pausetimer()
{
	if (m_doc.doctype_id == 1)
	{
		flag_windowact = false;
	}

}
void  Mydoc::continuetimer()
{
	flag_windowact = true;
}
void Mydoc::on_view_doubleClicked ( const QModelIndex & index )
{
	QString strfilepath = m_mymodel->data(m_mymodel->index(index.row(),0,index.parent()),UserRole_DIRINFO).toString();//DM
	QString strfilename = m_mymodel->data(m_mymodel->index(index.row(),0,index.parent()), Qt::DisplayRole).toString(); 
	QString isdir =  m_mymodel->data(m_mymodel->index(index.row(),2,index.parent()),UserRole_DIRINFO).toString(); 
	if (!strfilepath.isEmpty()&&isdir=="notdir")
	{
// 		pushButton_collapse->setText(">");		
// 		groupBox->setMaximumWidth(0);
// 		timecount = 0 ;	
// 		QVariant v(strfilepath);
// 		QVariant r("ReadOnly");
// 		pOfficeContainer->Open(v/*,r*/);
// // 		QString strfilepath2 = m_mymodel->data(m_mymodel->index(index.row()+2,0,index.parent()),UserRole_DIRINFO).toString();//DM
// // 		QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(strfilepath2).absoluteFilePath())); 
// 		label_docname->setText(strfilename);
// 		cout_mouseunmoved = 0;	
// 		m_Timer->start(1000);

	}
	else 
	{
		//PopupMessageBox(this,"请选择正确的文档路径",QMessageBox::Information); 
		return ;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_DOC);	
	data.cond_flag.flag_doc.name=TRUE;
	strcpy(data.cond_seg.seg_doc.name,strfilename.toLocal8Bit().data());
	if (SelectRecs(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_DOC);
		PopupMessageBox(this,"当前文档不在文档库中，无学分累积",QMessageBox::Information); 
		//qDebug("chaxu数据失败!");
		//return;
	}
	else
	{
		m_doc = data_doc[0];
	}
	DB_FMT data2;
	InitDbData(&data2,TABLE_STUDY_PLAN);	
	data2.cond_flag.flag_study_plan.doc_id=TRUE;
	data2.cond_flag.flag_study_plan.doc_id=TRUE;
	strcpy(data2.cond_seg.seg_doc.name,strfilename.toLocal8Bit().data());
	if (SelectRecs(&data2)!=TRUE)
	{
		HdbLogWrite(data2.sql_str,data.sql_error);
		FreeDbData(&data2,TABLE_STUDY_PLAN);
		PopupMessageBox(this,"当前文档不在文档计划中，无学分累积",QMessageBox::Information); 
		//qDebug("chaxu数据失败!");
	//	return;
	}
	else
	{
		m_study_plan = data_study_plan[0];
	}

 		timecount = 0 ;	
		if(m_doc.doctype_id==1)
		{
			QVariant v(strfilepath);
			QVariant r("ReadOnly");
			pOfficeContainer->Open(v/*,r*/); 
			pushButton_collapse->setText(">");		
			groupBox->setMaximumWidth(0);
		}
		if (m_doc.doctype_id==2 ||m_doc.doctype_id==3)
		{
			QString strfilepath2 = m_mymodel->data(m_mymodel->index(index.row(),0,index.parent()),UserRole_DIRINFO).toString();//DM
	 		QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(strfilepath2).absoluteFilePath())); 
		}
 		label_docname->setText(strfilename);
 		cout_mouseunmoved = 0;	
		m_Timer->start(1000);
}
void Mydoc::on_pushButtonStudy_Clicked ()
{
	QModelIndex   index = treeView_docdir->currentIndex ();
	QString strfilepath = m_mymodel->data(m_mymodel->index(index.row(),0,index.parent()),UserRole_DIRINFO).toString();//DM
	QString strfilename = m_mymodel->data(m_mymodel->index(index.row(),0,index.parent()), Qt::DisplayRole).toString(); 
	QString isdir =  m_mymodel->data(m_mymodel->index(index.row(),2,index.parent()),UserRole_DIRINFO).toString(); 
	if (!strfilepath.isEmpty()&&isdir=="notdir")
	{
		pushButton_collapse->setText(">");		
		groupBox->setMaximumWidth(0);
		timecount = 0 ;	
		QVariant v(strfilepath);
		QVariant r("ReadOnly");
		pOfficeContainer->Open(v,r);
		label_docname->setText(strfilename);
		m_Timer->start(1000);
	}
	else 
	{
		PopupMessageBox(this,"请选择正确的文档路径",QMessageBox::Information); 
		return ;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_DOC);	
	data.cond_flag.flag_doc.name=TRUE;
	strcpy(data.cond_seg.seg_doc.name,strfilename.toLocal8Bit().data());
	if (SelectRecs(&data)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_DOC);
		PopupMessageBox(this,"当前文档不在文档库中，无学分累积",QMessageBox::Information); 
		//qDebug("chaxu数据失败!");
		return;
	}
	else
	{
		m_doc = data_doc[0];
	}
	DB_FMT data2;
	InitDbData(&data2,TABLE_STUDY_PLAN);	
	data2.cond_flag.flag_study_plan.doc_id=TRUE;
	data2.cond_flag.flag_study_plan.doc_id=TRUE;
	strcpy(data2.cond_seg.seg_doc.name,strfilename.toLocal8Bit().data());
	if (SelectRecs(&data2)!=TRUE)
	{
		HdbLogWrite(data2.sql_str,data.sql_error);
		FreeDbData(&data2,TABLE_STUDY_PLAN);
		PopupMessageBox(this,"当前文档不在文档计划中，无学分累积",QMessageBox::Information); 
		//qDebug("chaxu数据失败!");
		return;
	}
	else
	{
		m_study_plan = data_study_plan[0];
	}
}
void Mydoc::on_pushButton_MystudyRec_Clicked ()
{
	emit sig_myrec();
}
void Mydoc::on_pushButton_finishdoc_Clicked ()
{
	if (m_doc.doctype_id == 1)
	{
		pOfficeContainer->Close();
	}
	m_Timer->stop();
	pushButton_collapse->setText(">");
	on_collapse();
	 writerec();
	 initdata();
}
void Mydoc:: writerec()
{
	if (m_study_plan.plan_id>=0)
	{
		int old_points,new_points=0;
		DB_FMT data;
		InitDbData(&data,TABLE_STUDY_RECORD);	
		data.cond_flag.flag_study_record.user_id=TRUE;
		data.cond_seg.seg_study_record.user_id=login_user_id;
		if( SelectRecs(&data) != TRUE)
		{	
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_STUDY_RECORD);
			qDebug("查询数据失败!");
			//return;
		}
		else if (data.rownum>0)
		{
			old_points = data_study_record[0].points;
		}
		else
		{
			old_points = 0;
		}
		if (m_study_plan.studytime>0)
		{
			new_points =(timecount*(m_study_plan.study_points))/(m_study_plan.studytime);
		}
		data.flag.flag_study_record.user_id=TRUE;
		data.flag.flag_study_record.host_id=TRUE;
		data.flag.flag_study_record.date_time=TRUE;
		data.flag.flag_study_record.type=TRUE;
		data.flag.flag_study_record.plan_id=TRUE;
		data.flag.flag_study_record.points=TRUE;
		data.seg.seg_study_record.user_id=login_user_id;
		data.seg.seg_study_record.host_id=g_host_id;
		GetCurTimeStr(data.seg.seg_study_record.date_time);
		//login_time = QString::fromLocal8Bit(data.seg.seg_study_record.date_time);
		data.seg.seg_study_record.type=1;
		data.seg.seg_study_record.plan_id=m_study_plan.plan_id;
		if (old_points + new_points > m_study_plan.study_points)
		{
			data.seg.seg_study_record.points = m_study_plan.study_points;
		}
		else
			data.seg.seg_study_record.points =  old_points + new_points ;
		if( InsertOneRec(&data) != TRUE)
		{	
			HdbLogWrite(data.sql_str,data.sql_error);
			FreeDbData(&data,TABLE_STUDY_RECORD);
			qDebug("插入数据失败!");
			//return;
		}
		InitDbData(&data,TABLE_STUDY_RECORD);
	}
}
void Mydoc:: timeplus()
{
	if (flag_windowact)
	{
		timecount++ ;
	}
	int h,m,t;
	h = timecount/3600;
	m = timecount%3600/60;
	t = timecount%60;
	QTime ti(h,m,t,0);
	lcdNumber_time->display(ti.toString("hh:mm:ss"));
}
void Mydoc:: timeplus_mouse()
{
	cout_mouseunmoved++ ;
	if (cout_mouseunmoved>30)
	{
		flag_windowact = false;
	}
	else
		flag_windowact = true;		
}
void Mydoc::findFile(const QString & path)
{
	treeView_docdir->setHeaderHidden(true);
	//treeView_docdir->setColumnWidth(0,200);

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
				//dzy
				DB_FMT data;
				data.table_id = TABLE_DOC;
				InitDbData(&data,data.table_id);
				data.cond_flag.flag_doc.name =TRUE;
				strcpy(data.cond_seg.seg_doc.name,file.fileName().toLocal8Bit().data());
				if (SelectRecs(&data)==int(TRUE) && data.rownum > 0)
				{
					m_file.file =file;
					m_file.depth = depth;
					file_list.append(m_file);
				}
				//dzy
// 				m_file.file =file;
// 				m_file.depth = depth;
// 				file_list.append(m_file);
			}
			depth--;			
			break;
		}
		i++;
		depth = 1;
	}while(i < list.size());
}
void Mydoc:: on_collapse()
{
	if (pushButton_collapse->text() == "<")
	{
		pushButton_collapse->setText(">");
		//int i = splitter->indexOf(groupBox);
		groupBox->setMaximumWidth(0);
		//groupBox->resize(0,groupBox->height());
		//frame->resize(frame->width()+groupbox_w,groupBox->height());
		//.gridLayout_3-
		//setupUi(this);
		//splitter->childAt(i);
		//splitter->move(-groupBox->width(),0);
		//splitter->moveSplitter(10,i);
	}
	else if(pushButton_collapse->text() == ">")
	{
		pushButton_collapse->setText("<");
		groupBox->setMaximumWidth(83500);
		//int i = splitter->indexOf(groupBox);
		//groupBox->resize(groupbox_w,groupBox->height());
		//frame->resize(frame->width()-groupbox_w,groupBox->height());
		//3splitter->childAt(i);
		//splitter->move(-groupBox->width(),0);
		//groupBox->width();
		//splitter->moveSplitter(100,i);
	}

}
//============================================================
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

	if (role== UserRole_DIRINFO||role== UserRole_Studyinfo)
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
	this->setHeaderData(0, Qt::Horizontal, tr("姓名"));
	this->setHeaderData(1, Qt::Horizontal, tr("性别"));
	this->setHeaderData(2, Qt::Horizontal, tr("身高"));
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
	return 3;
}
//! [5]

//! [6]
QString TreeItem::data(int column,int role) const
{
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
		case 2:
			return 	 itemData.file.lastModified().toString("yyyy-MM-dd hh:mm:ss");  
			break;
		default:
			break;
		}
	}
	if (role == UserRole_DIRINFO)
	{
		bool isdir = itemData.file.isDir();
		switch (column)
		{
		case  0:
			return itemData.file.absoluteFilePath();
			//return itemData.file.absoluteFilePath();
			break;
		case  1:
			return 	 itemData.file.fileName();
			break;
		case 2:
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
