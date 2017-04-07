#include "privilegemanager.h"

CPrivilegeMananger::CPrivilegeMananger(QWidget* parent)
	:QDialog(parent)
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
	popMenu = new QMenu(this);
	actionPrivCopy = new QAction(QString::fromLocal8Bit("复制权限"), popMenu);
	actionPrivPaste = new QAction(QString::fromLocal8Bit("粘贴权限"), popMenu);
	actionPrivCopy->setShortcut(QKeySequence(QKeySequence::Copy));
	actionPrivPaste->setShortcut(QKeySequence(QKeySequence::Paste));
	userSourceModel = new QStandardItemModel;
	userModel = new QSortFilterProxyModel;
	userModel->setDynamicSortFilter(true);
	userModel->setSourceModel(userSourceModel);
	listUser->setModel(userModel);
	selectionModel = listUser->selectionModel();
	QCompleter *completer = new QCompleter(this);
	completer->setModel(userModel);
	editName->setCompleter(completer);
	listUser->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(selectionModel,SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)),this,SLOT(showUserPriv(const QModelIndex &)));
	connect(listPrivilege,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(setPrivMask(QListWidgetItem *)));
	connect(buttonSave,SIGNAL(clicked()),this,SLOT(saveAllPriv()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(close()));
	connect(editName,SIGNAL(textChanged(const QString &)),this,SLOT(userFilterChanged(const QString &))); 
	connect(comboMode,SIGNAL(currentIndexChanged(int)),this,SLOT(privModeChanged(int))); 
	connect(buttonSelectAll,SIGNAL(clicked()),this,SLOT(selectAll()));
	connect(buttonSelectOff,SIGNAL(clicked()),this,SLOT(selectOff()));
	connect(buttonSelectInvert,SIGNAL(clicked()),this,SLOT(selectInvert()));
	connect(actionPrivCopy,SIGNAL(triggered()),this,SLOT(privCopy()));
	connect(actionPrivPaste,SIGNAL(triggered()),this,SLOT(privPaste()));
	connect(listUser,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(popupUserMenu(const QPoint &)));
}
void CPrivilegeMananger::showEvent(QShowEvent *event)
{
	InitPrivilege();
	memcpy(priv_mask_tmp,priv_mask,sizeof(priv_mask));
	memset(priv_copy.priv_mask_copy,0,sizeof(priv_copy.priv_mask_copy));
	actionPrivPaste->setEnabled(false);
	setUserSourceModel();
	event->accept();
}
void  CPrivilegeMananger::setLanguage(int language)
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
	InitPrivilege();
	memcpy(priv_mask_tmp,priv_mask,sizeof(priv_mask));
	memset(priv_copy.priv_mask_copy,0,sizeof(priv_copy.priv_mask_copy));
	actionPrivPaste->setEnabled(false);
	setUserSourceModel();
}
void CPrivilegeMananger::keyPressEvent(QKeyEvent *event)
{	
	if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier))
	{
		privCopy();
	}
	else if (event->key() == Qt::Key_V && (event->modifiers() & Qt::ControlModifier))
	{
		if (actionPrivPaste->isEnabled())
			privPaste();
	}
	QDialog::keyPressEvent(event);    
}
void CPrivilegeMananger::loadChinese()
{
	QTranslator translator;
	translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CPrivilegeMananger::loadEnglish()
{
	QTranslator translator;
	translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
void CPrivilegeMananger::popupUserMenu(const QPoint &pos)
{
	popMenu->addAction(actionPrivCopy);
	popMenu->addAction(actionPrivPaste);
	popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}//设置右键弹出菜单
void CPrivilegeMananger::setUserSourceModel()
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	if( SelectRecs(&data) != TRUE)
	{		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	userSourceModel->clear();
	for(int i=0;i<data.rownum;i++)	
	{
		QStandardItem* item = new QStandardItem;
		if (data_userinfo[i].role == ROLE_SYSTEM)
		{
			item->setText(QString::fromLocal8Bit("(系统)%1").arg(data_userinfo[i].longname));
			item->setIcon(QIcon(QString::fromLocal8Bit(":/pic/images/user3.png")));
		}
		else if (data_userinfo[i].role == ROLE_TEACHER)
		{
			item->setText(QString::fromLocal8Bit("(教员)%1").arg(data_userinfo[i].longname)); 
			item->setIcon(QIcon(QString::fromLocal8Bit(":/pic/images/user6.png")));
		}
		else if (data_userinfo[i].role == ROLE_STUDENT)
		{
			item->setText(QString::fromLocal8Bit("(学员)%1").arg(data_userinfo[i].longname)); 
			item->setIcon(QIcon(QString::fromLocal8Bit(":/pic/images/user7.png")));
		}
		item->setData(QVariant(data_userinfo[i].user_id),Qt::UserRole);
		userSourceModel->appendRow(item); 
	}
	listUser->setCurrentIndex(userModel->index(0,0));
}
void CPrivilegeMananger::userFilterChanged(const QString &text)
{
	QRegExp regExp(text, Qt::CaseInsensitive); 
	userModel->setFilterRegExp(regExp); 
}
void CPrivilegeMananger::privModeChanged(int mode)
{
	showUserPriv(selectionModel->currentIndex());
}
void CPrivilegeMananger::setPrivMask(QListWidgetItem *item)
{
	int user_id,row,mode;
	row = listPrivilege->row(item);
	user_id = userModel->data(selectionModel->currentIndex(),Qt::UserRole).toInt();
	if (JudgeUserRole(user_id,ROLE_STUDENT)==TRUE)
		mode = comboMode->currentIndex();
	else
		mode = 0;
	if (item->checkState() == Qt::Checked)
	{
		priv_mask_tmp[user_id][mode] = priv_mask_tmp[user_id][mode] | (1<<row);
	}
	else
	{
		priv_mask_tmp[user_id][mode] = priv_mask_tmp[user_id][mode] & (~(1<<row));
	}
}

void CPrivilegeMananger::showUserPriv(const QModelIndex &index)
{
	if (!index.isValid()) return;
	int mode = 0;
	int user_id = userModel->data(index,Qt::UserRole).toInt();
	listPrivilege->clear();
	if (JudgeUserRole(user_id,ROLE_SYSTEM)==TRUE)
	{
		comboMode->hide();
		for (int i=0;i<PrivTeacher.count();i++)
		{
			QListWidgetItem *privItem = new QListWidgetItem;
			privItem->setText(PrivTeacher[i]);
			if (priv_mask_tmp[user_id][mode] & 1<<i)
				privItem->setCheckState(Qt::Checked);
			else
				privItem->setCheckState(Qt::Unchecked);
			listPrivilege->addItem(privItem);
			privItem->setFlags(Qt::ItemIsEnabled);
		}
	}
	else if (JudgeUserRole(user_id,ROLE_TEACHER)==TRUE)
	{
		comboMode->hide();
		for (int i=0;i<PrivTeacher.count();i++)
		{
			QListWidgetItem *privItem = new QListWidgetItem;
			privItem->setText(PrivTeacher[i]);
			if (priv_mask_tmp[user_id][mode] & 1<<i)
				privItem->setCheckState(Qt::Checked);
			else
				privItem->setCheckState(Qt::Unchecked);
			listPrivilege->addItem(privItem);
		}
	}
	else if (JudgeUserRole(user_id,ROLE_STUDENT)==TRUE)
	{
		comboMode->show();
		mode = comboMode->currentIndex();
		for (int i=0;i<PrivStudent.count();i++)
		{
			QListWidgetItem *privItem = new QListWidgetItem;
			privItem->setText(PrivStudent[i]);
			if (priv_mask_tmp[user_id][mode] & 1<<i)
				privItem->setCheckState(Qt::Checked);
			else
				privItem->setCheckState(Qt::Unchecked);
			listPrivilege->addItem(privItem);
		}
	}
}
void CPrivilegeMananger::selectAll()
{
	for (int i=0;i<listPrivilege->count();i++)
	{
		listPrivilege->item(i)->setCheckState(Qt::Checked);
	}
}
void CPrivilegeMananger::selectOff()
{
	for (int i=0;i<listPrivilege->count();i++)
	{
		listPrivilege->item(i)->setCheckState(Qt::Unchecked);
	}
}
void CPrivilegeMananger::privCopy()
{
	int user_id = userModel->data(selectionModel->currentIndex(),Qt::UserRole).toInt();
	priv_copy.user_id = user_id;
	if (JudgeUserRole(user_id,ROLE_STUDENT)==TRUE)
		priv_copy.role = ROLE_STUDENT;
	else if (JudgeUserRole(user_id,ROLE_TEACHER)==TRUE)
		priv_copy.role = ROLE_TEACHER;
	else
		priv_copy.role = ROLE_SYSTEM;
	for (int mode=0;mode<MAX_PRIV_MODE_NUM;mode++)
	{
		priv_copy.priv_mask_copy[mode] = priv_mask_tmp[user_id][mode];
	}
	actionPrivPaste->setEnabled(true);
}
void CPrivilegeMananger::privPaste()
{
	int user_id = userModel->data(selectionModel->currentIndex(),Qt::UserRole).toInt();
	if (priv_copy.user_id == user_id)	return;
	if (JudgeUserRole(user_id,priv_copy.role)==FALSE)
	{
		PopupMessageBox(this,"复制权限的用户和粘贴用户权限的用户角色不同,不能粘贴!",QMessageBox::Warning);
		return;
	}
	for (int mode=0;mode<MAX_PRIV_MODE_NUM;mode++)
	{
		priv_mask_tmp[user_id][mode] = priv_copy.priv_mask_copy[mode];
	}
	showUserPriv(selectionModel->currentIndex());
}
void CPrivilegeMananger::selectInvert()
{
	for (int i=0;i<listPrivilege->count();i++)
	{
		listPrivilege->item(i)->checkState() == Qt::Checked ? listPrivilege->item(i)->setCheckState(Qt::Unchecked) : listPrivilege->item(i)->setCheckState(Qt::Checked);
	}
}
void CPrivilegeMananger::saveAllPriv()
{
	DB_FMT data;
	bool chg_flag = false;
	char privilege[50];
	InitDbData(&data,TABLE_USERINFO);
	for (int i=0;i<MAX_USER_NUM;i++)
	{
		chg_flag = false;
		for (int mode=0;mode<MAX_PRIV_MODE_NUM;mode++)
		{
			sprintf(privilege+mode*8,"%08X",priv_mask_tmp[i][mode]);
			if (priv_mask[i][mode] != priv_mask_tmp[i][mode])
			{
				priv_mask[i][mode] = priv_mask_tmp[i][mode];
				chg_flag = true;
			}
		}
		if (chg_flag)
		{
			data.flag.flag_userinfo.privilege = TRUE;
			strcpy(data.seg.seg_userinfo.privilege,privilege);

			data.cond_flag.flag_userinfo.user_id = TRUE;
			data.cond_seg.seg_userinfo.user_id = i;
			if( UpdateOneRec(&data) != TRUE)
			{
				HdbLogWrite(data.sql_str,data.sql_error);
				continue;
			}
		}
	}
	emit sigPrivChanged(loginUser->login_user_id);
	this->close();
}
