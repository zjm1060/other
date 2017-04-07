#include "sendmessage.h"
#include <QMessageBox>

CSendMessageTable::CSendMessageTable(QWidget* parent)
	:QWidget()//(parent,Qt::WindowMinMaxButtonsHint)
{
	setupUi(this);
	if (flag_langueage == 0)
	{
		loadChinese();
	}
	else if (flag_langueage == 1)
	{
		loadEnglish();
	}	
	dlg_created_flag[MSGDLG] = true;
	//this->setAttribute(Qt::WA_DeleteOnClose);
	//this->setModal(true);
	refresh = new QTimer(this);
	menuButtonSend = new QMenu(this);
	allUserModel = new QStandardItemModel;
	sendUserModel = new QStandardItemModel;
	allUserSortModel = new QSortFilterProxyModel;
	udpSendSocket = new QUdpSocket(this);
	udpRecvSocket = new QUdpSocket(this);
	faceImageWidget = new CFaceImageWidget(this);
	actionSend1 = new QAction(QString::fromLocal8Bit("按Enter键发送消息"), this);
	actionSend2 = new QAction(QString::fromLocal8Bit("按Ctrl+Enter键发送消息"), this);
	actionSend3 = new QAction(QString::fromLocal8Bit("消息发送后关闭窗口"), this);
	actionGroup = new QActionGroup(this);
	actionRefresh = new QAction(this);
	flagUnReadMsg = false;
	setControlAtt();
	displayAllUser();
	connect(buttonSend,SIGNAL(pressed()),this,SLOT(sendMessage()));
	connect(buttonHideAllUser,SIGNAL(clicked()),this,SLOT(changeAllUserShow()));
	connect(buttonMsgRec,SIGNAL(toggled(bool)),this,SLOT(changeMsgRecShow(bool)));
	connect(buttonAddUser,SIGNAL(clicked()),this,SLOT(addToSendUser()));
	connect(listAllUser,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(addToSendUser(const QModelIndex &)));
	connect(buttonAddUserAll,SIGNAL(clicked()),this,SLOT(addAllToSendUser()));
	connect(buttonDelUser,SIGNAL(clicked()),this,SLOT(delToSendUser()));
	connect(listSendUser,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(delToSendUser(const QModelIndex &)));
	connect(buttonDelUserAll,SIGNAL(clicked()),this,SLOT(delAllToSendUser()));
	connect(buttonFont,SIGNAL(clicked()),this,SLOT(setFont()));
	connect(buttonColor,SIGNAL(clicked()),this,SLOT(setColor()));
	connect(buttonImage,SIGNAL(toggled(bool)),this,SLOT(openFaceImageWidget(bool)));
	connect(faceImageWidget,SIGNAL(sigToggled(bool)),buttonImage,SLOT(setChecked(bool)));
	connect(buttonShake,SIGNAL(clicked()),this,SLOT(sendShake()));
	connect(udpRecvSocket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
	connect(refresh, SIGNAL(timeout()), this, SLOT(displayAllUser()));
	connect(actionRefresh, SIGNAL(triggered()), this, SLOT(displayAllUser()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(close()));
	buttonImage->hide();//功能未完全实现，先隐藏按钮
	buttonShake->hide();
	buttonColor->hide();
	buttonFont->hide();
	connect(comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_usefulExpression(int)));
}
CSendMessageTable::~CSendMessageTable()
{
	dlg_created_flag[MSGDLG] = false;
}
void CSendMessageTable::setControlAtt()
{
	isAllUserShow=true;
	editMsgRec->setReadOnly(TRUE);
	editMsgShow->setReadOnly(TRUE);
	buttonExit->setShortcut(tr("ALT+C"));
	actionRefresh->setShortcut(tr("F5"));
	listAllUser->setEditTriggers(QTableView::NoEditTriggers);
	listAllUser->setSelectionMode(QAbstractItemView::ExtendedSelection); 
	listSendUser->setEditTriggers(QTableView::NoEditTriggers);
	listSendUser->setSelectionMode(QAbstractItemView::ExtendedSelection); 
	editMsgRec->hide();
	listAllUser->setModel(allUserSortModel);
	listSendUser->setModel(sendUserModel);
	allUserSortModel->setSourceModel(allUserModel);
	allUserSortModel->sort(0,Qt::DescendingOrder);
	allUserSortModel->setSortRole(Qt::UserRole + 2);
	allUserSortModel->setDynamicSortFilter(true);
	selectionModelAllUser = listAllUser->selectionModel();
	selectionModelSendUser = listSendUser->selectionModel();
	udpRecvSocket->bind(OTS_UDP_PORT,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	menuButtonSend->addAction(actionSend1);
	menuButtonSend->addAction(actionSend2);
	menuButtonSend->addAction(actionSend3);
	actionSend1->setCheckable(true);
	actionSend2->setCheckable(true);
	actionSend3->setCheckable(true);
	actionSend1->setChecked(true);
	actionGroup->addAction(actionSend1);
	actionGroup->addAction(actionSend2);
	actionGroup->setExclusive(true);
	buttonSend->setMenu(menuButtonSend);
	buttonSend->setPopupMode(QToolButton::MenuButtonPopup);
	editMsgSend->installEventFilter(this);
}
void CSendMessageTable::loadChinese()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_zh_CN");
	}
	else
		translator.load("SimuTutor_zh_CN");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}

void CSendMessageTable::loadEnglish()
{
	QTranslator translator;
	if (flag_simutrainee)
	{
		 translator.load("/home/ems/system/ots/SimuTrainee/SimuTrainee_en");
	}
	else
		translator.load("SimuTutor_en");
	qApp->installTranslator(&translator);
	retranslateUi(this);
}
bool CSendMessageTable::eventFilter(QObject *target, QEvent *event)
{
	if (target == editMsgSend)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Return && !(keyEvent->modifiers() & Qt::ControlModifier)) 
			{          
				if (actionSend1->isChecked())
				{
					sendMessage();
					return true;
				}
			}
			else if (keyEvent->key() == Qt::Key_Return && (keyEvent->modifiers() & Qt::ControlModifier))
			{
				if (actionSend2->isChecked())
				{
					sendMessage();
					return true;
				}
			}
			else if (keyEvent->key() == Qt::Key_S && (keyEvent->modifiers() & Qt::AltModifier))
			{
				sendMessage();
				return true;
			}
		}
	}
	return QWidget::eventFilter(target, event);  
}
void CSendMessageTable::showEvent ( QShowEvent * event )
{
	event->accept();
	//refresh->start(3000);
	displayAllUser();
	sendUserModel->clear();
	//userLoginStateThread = new UserLoginStateThread(this);
	//connect(userLoginStateThread,SIGNAL(sigUserStateChanged(int,short)),this,SLOT(userStateChanged(int,short)));
	//userLoginStateThread->start();
	selectionModelAllUser->clear();
	selectionModelSendUser->clear();
	if (!flagUnReadMsg)
		editMsgShow->clear();
	flagUnReadMsg = false;
	editMsgSend->clear();
	timerButtonChat->stop();
}
void  CSendMessageTable::setLanguage(int language)
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
	displayAllUser();
	sendUserModel->clear();
	selectionModelAllUser->clear();
	selectionModelSendUser->clear();
	if (!flagUnReadMsg)
		editMsgShow->clear();
	flagUnReadMsg = false;
	editMsgSend->clear();
	timerButtonChat->stop();
}
void CSendMessageTable::closeEvent(QCloseEvent *event)
{
	this->hide();
	event->accept();
	//refresh->stop();
	//userLoginStateThread->terminate();
	//userLoginStateThread->wait(); 
	//userLoginStateThread->quit();
	//delete userLoginStateThread;
}
void CSendMessageTable::hideEvent(QHideEvent *event)
{
	event->accept();
	//refresh->stop();
	//userLoginStateThread->terminate();
	//userLoginStateThread->wait(); 
	//userLoginStateThread->quit();
	//delete userLoginStateThread;
}
void CSendMessageTable::keyPressEvent(QKeyEvent *event)
{	
	if (event->key() == Qt::Key_F5)
	{
		displayAllUser();
	}
	QWidget::keyPressEvent(event);    
}
void CSendMessageTable::setUserId(int m_user_id)
{
	cur_user_id = m_user_id;
}
void CSendMessageTable::changeAllUserShow()
{
	if(groupAllUser->isHidden())
	{
		groupAllUser->show();
		isAllUserShow=true;
		buttonHideAllUser->setText(QString::fromLocal8Bit("隐藏所有用户列表"));
	}
	else
	{
		groupAllUser->hide();
		isAllUserShow=false;
		buttonHideAllUser->setText(QString::fromLocal8Bit("显示所有用户列表"));
	}
}
void CSendMessageTable::changeMsgRecShow(bool checked)
{
	if (checked)
	{
		editMsgRec->show();
		groupAllUser->hide();
		groupSendUser->hide();	
	}
	else
	{
		editMsgRec->hide();
		if (isAllUserShow)
			groupAllUser->show();
		groupSendUser->show();
	}
}
void CSendMessageTable::displayAllUser()
{
	DB_FMT data;
	InitDbData(&data,TABLE_USERINFO);
	if( SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		FreeDbData(&data,TABLE_USERINFO);
		qDebug("查询数据库失败!");
		return;
	}
	//allUserModel->clear();
	//allUserModel->setColumnCount(1);
	for (int i=0;i<data.rownum;i++)
	{
		if (allUserModel->findItems(QString::fromLocal8Bit(data_userinfo[i].longname)).size() != 0)
			continue;
		QStandardItem* item = new QStandardItem(QString::fromLocal8Bit(data_userinfo[i].longname)); 
		item->setData(QVariant(data_userinfo[i].user_id));
		item->setIcon(QIcon(QString::fromLocal8Bit(":/pic/images/user7.png")));
		allUserModel->appendRow(item);
	}
	for (int i=0;i<allUserModel->rowCount();i++)
	{
		QStandardItem* item = allUserModel->item(i);
		if (JudgeUserRole(item->data().toInt(),ROLE_STUDENT) == FALSE || JudgeLoginState(item->data().toInt()) == TRUE)
		{
			item->setData(QVariant(1),Qt::UserRole + 2);
			item->setEnabled(true);
		}
		else
		{
			item->setData(QVariant(0),Qt::UserRole + 2);
			item->setEnabled(false);
			if (sendUserModel->findItems(item->text()).size() != 0)
				sendUserModel->takeRow(sendUserModel->findItems(item->text()).at(0)->row());
		}
	}
	FreeDbData(&data,TABLE_USERINFO);
}
void CSendMessageTable::userStateChanged(int user_id,short state)
{
	for (int i=0;i<allUserModel->rowCount();i++)
	{
		QStandardItem* item = allUserModel->item(i);
		if (state)
		{
			item->setData(QVariant(1),Qt::UserRole + 2);
			item->setEnabled(true);
		}
		else
		{
			item->setData(QVariant(0),Qt::UserRole + 2);
			item->setEnabled(false);
		}
	}
}
void CSendMessageTable::addToSendUser()
{
	QModelIndexList indexes = selectionModelAllUser->selectedRows(0);
	if (indexes.size() == 0)
		return;
	for (int i=0;i<indexes.size();i++)
	{
		QStandardItem *itemUser = allUserModel->itemFromIndex(allUserSortModel->mapToSource(allUserSortModel->index(indexes.at(i).row(),0)))->clone();
		if (itemUser->data(Qt::UserRole + 2) == 1 && sendUserModel->findItems(itemUser->text()).size() == 0)
			sendUserModel->appendRow(itemUser);	
	}
}
void CSendMessageTable::addToSendUser(const QModelIndex &index)
{
	QStandardItem *itemUser = allUserModel->itemFromIndex(allUserSortModel->mapToSource(index))->clone();
	if (itemUser->data(Qt::UserRole + 2) == 1 && sendUserModel->findItems(itemUser->text()).size() == 0)
		sendUserModel->appendRow(itemUser);	
}
void CSendMessageTable::addAllToSendUser()
{
	for (int i=0;i<allUserModel->rowCount();i++)
	{
		QStandardItem *itemUser = allUserModel->itemFromIndex(allUserSortModel->mapToSource(allUserSortModel->index(i,0)))->clone();
		if (itemUser->data(Qt::UserRole + 2) == 1 && sendUserModel->findItems(itemUser->text()).size() == 0)
			sendUserModel->appendRow(itemUser);	
	}
}
void CSendMessageTable::delToSendUser()
{
	QModelIndexList indexes = selectionModelSendUser->selectedRows(0);
	if (indexes.size() == 0)
		return;
	for (int i=indexes.size()-1;i>=0;i--)
	{
		sendUserModel->takeRow(indexes.at(i).row());
	}
}
void CSendMessageTable::delToSendUser(const QModelIndex &index)
{
	sendUserModel->takeRow(index.row());
}
void CSendMessageTable::delAllToSendUser()
{
	for (int i=sendUserModel->rowCount()-1;i>=0;i--)
	{
		sendUserModel->takeRow(i);
	}
}
void CSendMessageTable::sendMessage()
{
	OTS_MSG msg;
	char send_user_name[20],send_time[22];
	if (editMsgSend->toPlainText().isEmpty())
	{
		QToolTip::showText(buttonSend->pos()+this->pos(),QString::fromLocal8Bit("发送内容不能为空，请重新输入!"));
		return;
	}
	GetCurTimeStr(send_time);
	msg.head.type = 0;
	msg.send_time = QString::fromLocal8Bit(send_time);
	msg.content = editMsgSend->toPlainText();
	GetUserLongName(cur_user_id,send_user_name);
	msg.send_user_name = QString::fromLocal8Bit(send_user_name);
	msg.color = cur_send_color;
	msg.font = cur_send_font;
	msg.send_host_name = QString::fromLocal8Bit(g_host_name);
	if (msg.content.size() > MAX_MSG_SIZE)
	{
		QString title;
		char data_time[22];
		GetCurTimeStr(data_time);
		title = QString::fromLocal8Bit(data_time);
		QFont ff(QString::fromLocal8Bit("宋体"), 10, QFont::Normal);
		QColor cc(0,0,0);
		setInsertTextColor(cc,editMsgShow);
		setInsertTextFont(ff,editMsgShow);
		editMsgShow->append(title);
		editMsgShow->append(QString::fromLocal8Bit("发送消息内容超长，请分条发送。"));
		editMsgShow->append("\n");
		return;
	}
	for (int i=0;i<sendUserModel->rowCount();i++)
	{
		QStandardItem *item = sendUserModel->itemFromIndex(sendUserModel->index(i,0));
		udpSend(item->data().toInt(),&msg);
	}
	appendMsgShow(&msg);
	appendMsgRec(&msg);
	if (actionSend3->isChecked())
		this->close();
}
void CSendMessageTable::sendShake()
{
	OTS_MSG msg;
	msg.head.type = 1;
	for (int i=0;i<sendUserModel->rowCount();i++)
	{
		QStandardItem *item = sendUserModel->itemFromIndex(sendUserModel->index(i,0));
		udpSend(item->data().toInt(),&msg);
	}
	shakeDialog();
}
void CSendMessageTable::udpSend(int user_id,OTS_MSG *msg)
{
	char ip_a[22],ip_b[22],cur_ip_a[22],cur_ip_b[22];
	GetUserIpAddr(user_id,ip_a,ip_b);
	GetUserIpAddr(cur_user_id,cur_ip_a,cur_ip_b);
	msg->head.length = sizeof(msg);
	msg->head.src_user_id = cur_user_id;
	msg->head.dest_user_id = user_id;
	msg->head.src_host_id = g_host_id;
	msg->head.dest_host_id = GetUserHostId(user_id);
	msg->head.src_ip = QString::fromLocal8Bit(cur_ip_a);
	msg->head.dest_ip = QString::fromLocal8Bit(ip_a);
	QByteArray datagram;
	QDataStream out(&datagram,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	out << msg->head.length << msg->head.src_ip << msg->head.dest_ip << msg->head.src_user_id << msg->head.dest_user_id
		<< msg->head.src_host_id << msg->head.dest_host_id << msg->head.type << msg->send_user_name << msg->send_host_name 
		<< msg->send_time << msg->font << msg->color << msg->content;
	msg->head.length = datagram.size();
	udpSendSocket->writeDatagram(datagram.data(),datagram.size(),QHostAddress::Broadcast,OTS_UDP_PORT);
	/*if (udpSendSocket->writeDatagram((char *)msg,msg->head.length,QHostAddress(ip_a),OTS_UDP_PORT) != -1)
	{
		qDebug("发送成功");
	}
	else
	{
		strcpy(msg->head.src_ip,cur_ip_a);
		strcpy(msg->head.dest_ip,ip_b);
		udpSendSocket->writeDatagram((char *)msg,msg->head.length,QHostAddress(ip_b),OTS_UDP_PORT);
	}*/
}
void CSendMessageTable::appendMsgShow(OTS_MSG *msg)
{
	QString title;
	title = QString::fromLocal8Bit("%1 (%2) %3").arg(msg->send_user_name,msg->send_host_name,msg->send_time);
	QFont ff(QString::fromLocal8Bit("宋体"), 10, QFont::Normal);
	QColor cc(0,0,0);
	setInsertTextColor(cc,editMsgShow);
	setInsertTextFont(ff,editMsgShow);
	editMsgShow->append(title);
	setInsertTextColor(msg->color,editMsgShow);
	setInsertTextFont(msg->font,editMsgShow);
	editMsgShow->append(msg->content);
	setInsertTextColor(cc,editMsgShow);
	setInsertTextFont(ff,editMsgShow);
	editMsgShow->append("\n");
	editMsgSend->clear();
	editMsgShow->ensureCursorVisible();
	if (this->isHidden())
		flagUnReadMsg = true;
}
void CSendMessageTable::appendMsgRec(OTS_MSG *msg) 
{
	QString title;
	title = QString::fromLocal8Bit("%1 (%2) %3").arg(msg->send_user_name,msg->send_host_name,msg->send_time);
	QFont ff(QString::fromLocal8Bit("宋体"), 10, QFont::Normal);
	QColor cc(0,0,0);
	setInsertTextColor(cc,editMsgRec);
	setInsertTextFont(ff,editMsgRec);
	editMsgRec->append(title);
	setInsertTextColor(msg->color,editMsgRec);
	setInsertTextFont(msg->font,editMsgRec);
	editMsgRec->append(msg->content);
	setInsertTextColor(cc,editMsgRec);
	setInsertTextFont(ff,editMsgRec);
	editMsgRec->append("\n");
	QDir dir;
	dir.setPath(QString::fromLocal8Bit("/home/ems/h9000/his/msgrec"));
	if (!dir.exists())
		dir.mkpath(QString::fromLocal8Bit("/home/ems/h9000/his/msgrec"));
	QString fileName = QDate::currentDate().toString("yyyyMMdd");
	QFile file(QString("/home/ems/h9000/his/msgrec/REC%1.txt").arg(fileName));
	if(file.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text))
	{
		QTextCodec* codec=QTextCodec::codecForName("UTF-8");
		QTextStream out (&file);
		out.setCodec(codec); 
		out <<title<<"\n"<<msg->content<<"\n\n";
		file.flush();
		file.close();
	}
}
void CSendMessageTable::readPendingDatagrams()
{
	while (udpRecvSocket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(udpRecvSocket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;
		udpRecvSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
		processTheDatagram(datagram);
	}
}
void CSendMessageTable::processTheDatagram(QByteArray datagram)
{
	OTS_MSG msg;
	QDataStream in(&datagram,QIODevice::ReadOnly);
	in.setVersion(QDataStream::Qt_4_5);
	in  >> msg.head.length >> msg.head.src_ip >> msg.head.dest_ip >> msg.head.src_user_id >> msg.head.dest_user_id
		>> msg.head.src_host_id >> msg.head.dest_host_id >> msg.head.type >> msg.send_user_name >> msg.send_host_name 
		>> msg.send_time >> msg.font >> msg.color >> msg.content;
	//memcpy((char *)&msg,datagram.data(),datagram.size());
	if (msg.head.dest_user_id != cur_user_id || msg.head.src_user_id == cur_user_id)
		return;
	emit sigRecvMsg();
	switch(msg.head.type)
	{
		case 0:
			appendMsgShow(&msg);
			appendMsgRec(&msg);
			break;
		case 1:
			shakeDialog();
			break;
		default:
			break;
	}
}
void CSendMessageTable::setInsertTextColor(const QColor &col,QTextEdit *textEdit)//把col颜色作为字体颜色
{
	QTextCharFormat fmt;//文本字符格式
	fmt.setForeground(col);// 前景色(即字体色)设为col色
	QTextCursor cursor = textEdit->textCursor();//获取文本光标
	cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
	textEdit->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
}
void CSendMessageTable::setInsertTextFont(const QFont &font,QTextEdit *textEdit)//把col颜色作为字体颜色
{
	QTextCharFormat fmt;//文本字符格式
	fmt.setFont(font);//字体
	QTextCursor cursor = textEdit->textCursor();//获取文本光标
	cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
	textEdit->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
}
void CSendMessageTable::setColor()
{
	QPalette palette = editMsgSend->palette();/*获取TextEdit中的调色板*/
	const QColor &color = QColorDialog::getColor(palette.color(QPalette::Text), this);
	if(color.isValid()) 
	{
		palette.setColor(QPalette::Text, color);/*设置颜色                 */
		editMsgSend->setPalette(palette);   /*应用到QTextEdit */
		cur_send_color = color;
	}
}
void CSendMessageTable::setFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok,editMsgSend->currentFont(),this);
	if (ok)
	{
		QTextCharFormat fmt;
		fmt.setFont(font);
		QTextCursor cursor = editMsgSend->textCursor();
		cursor.movePosition(QTextCursor::Start);
		editMsgSend->mergeCurrentCharFormat(fmt);
		cursor.movePosition(QTextCursor::End);
		editMsgSend->setCurrentFont(font);
		cur_send_font = font;
	}
} 

void CSendMessageTable::insertImage() //文本编辑框中插入图片
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open File"),"pic/",\
		tr("Images (*.png *.jpg)"));
	QUrl Uri ( QString ( "file://%1" ).arg ( file ) );
	QImage image = QImageReader ( file ).read();
	QTextDocument * textDocument = editMsgSend->document();
	textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
	QTextCursor cursor = editMsgSend->textCursor();
	QTextImageFormat imageFormat;
	imageFormat.setWidth( image.width() );
	imageFormat.setHeight( image.height() );
	imageFormat.setName( Uri.toString() );
	cursor.insertImage(imageFormat);
}

void CSendMessageTable::shakeDialog()
{
	this->show();
	this->activateWindow();
	//QPoint p = getPos(this);//获取当前窗口坐标
	QPoint p = this->pos();
	int x = p.x();
	int y = p.y();
	int i = 0;
	for(i=0;i<20;i++)//改变窗口位置实现震动效果
	{
		this->move(x+5,y+5);
		Sleep(20);
		this->move(x,y);
		Sleep(20);
		this->move(x-5,y-5);
		Sleep(20);
		this->move(x,y);
	}
	//this->move(x-1,y-22);
}

QPoint CSendMessageTable::getPos(QWidget* widget)//获取窗体位置坐标
{
	if (NULL == widget)
	{
		return QPoint(-1,-1);
	}
	float px = 0;
	float py = 0;
	QWidget *parent = widget;
	QWidget *preParent= NULL;
	do
	{
		QPoint p = parent->pos();
		px += p.x();
		py += p.y();
		preParent = parent ;
		parent = parent->parentWidget();
	} while (NULL != parent);
	QSize size = preParent->frameSize();
	QSize size2 = preParent->size();
	px = px + (size.width() - size2.width())/2;
	py = py + (size.height() - size2.height() - (size.width() - size2.width())/2);
	QPoint pr(px, py);
	return pr;
}
void CSendMessageTable::openFaceImageWidget(bool checked)
{
	if (checked)
	{
		QPoint pos;
		faceImageWidget->show();
		pos = buttonImage->pos() + this->pos();
		pos.setX(pos.x()-faceImageWidget->width()/2);
		pos.setY(pos.y()-faceImageWidget->height());
		faceImageWidget->move(pos);
	}
	else
	{
		faceImageWidget->hide();
	}
}

void CSendMessageTable::on_usefulExpression(int index)//插入常用语
{
	if (index<=0)
	{
		return;
	}
	else
	{
		QString str = comboBox->itemText(index);
		editMsgSend->setText(editMsgSend->toPlainText()+str);
		MySleep(500);
		comboBox->setCurrentIndex(0);
	}
}

/*
void QChatDialog::OnCbxFontIndexChanged(const QString&amp; text)
{
tstring name = text.toStdString();
std::fill(m_imFmt.m_fontName, m_imFmt.m_fontName + sizeof(m_imFmt.m_fontName), 0);
std::copy(name.begin(), name.end(), m_imFmt.m_fontName);

QTextCharFormat fmt = m_txtSend-&gt;currentCharFormat();
fmt.setFontFamily(text);
QTextCursor cursor = m_txtSend-&gt;textCursor();
cursor.clearSelection();
cursor.select(QTextCursor::Document);
cursor.setCharFormat(fmt);
m_txtSend-&gt;setTextCursor(cursor);
cursor.setPosition(cursor.selectionEnd());
m_txtSend-&gt;setTextCursor(cursor);
}
void QChatDialog::OnCbxFontSizeIndexChanged(const QString&amp; text)
{
	m_imFmt.m_fontSize = text.toInt();

	QTextCharFormat fmt = m_txtSend-&gt;currentCharFormat();
	fmt.setFontPointSize(m_imFmt.m_fontSize);

	QTextCursor cursor = m_txtSend-&gt;textCursor();
	cursor.clearSelection();
	cursor.select(QTextCursor::Document);
	cursor.setCharFormat(fmt);
	m_txtSend-&gt;setTextCursor(cursor);

	cursor.setPosition(cursor.selectionEnd());
	m_txtSend-&gt;setTextCursor(cursor);
}

void QChatDialog::OnFontBoldClicked(bool checked)
{
	m_imFmt.m_bold = checked;

	QTextCharFormat fmt = m_txtSend-&gt;currentCharFormat();
	fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);

	QTextCursor cursor = m_txtSend-&gt;textCursor();
	cursor.clearSelection();
	cursor.select(QTextCursor::Document);
	cursor.setCharFormat(fmt);
	m_txtSend-&gt;setTextCursor(cursor);

	cursor.setPosition(cursor.selectionEnd());
	m_txtSend-&gt;setTextCursor(cursor);
}

void QChatDialog::OnFontItalicClicked(bool checked)
{
	m_imFmt.m_italic = checked;

	QTextCharFormat fmt = m_txtSend-&gt;currentCharFormat();
	fmt.setFontItalic(checked);

	QTextCursor cursor = m_txtSend-&gt;textCursor();
	cursor.clearSelection();
	cursor.select(QTextCursor::Document);
	cursor.setCharFormat(fmt);
	m_txtSend-&gt;setTextCursor(cursor);

	cursor.setPosition(cursor.selectionEnd());
	m_txtSend-&gt;setTextCursor(cursor);
}

void QChatDialog::OnFontUnderlineClicked(bool checked)
{
	m_imFmt.m_underline = checked;

	QTextCharFormat fmt = m_txtSend-&gt;currentCharFormat();
	fmt.setFontUnderline(checked);

	QTextCursor cursor = m_txtSend-&gt;textCursor();
	cursor.clearSelection();
	cursor.select(QTextCursor::Document);
	cursor.setCharFormat(fmt);
	m_txtSend-&gt;setTextCursor(cursor);

	cursor.setPosition(cursor.selectionEnd());
	m_txtSend-&gt;setTextCursor(cursor);
}
*/
/****************************************************************************************************************/
CFaceImageWidget::CFaceImageWidget(QWidget* parent)
:QWidget(parent,Qt::Popup)
{
	scrollArea = new QScrollArea;
	characterWidget = new CharacterWidget;
	scrollArea->setWidget(characterWidget);
	scrollArea->setFrameShadow(QFrame::Raised);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->verticalScrollBar()->setSingleStep(32);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(scrollArea, 1);
	layout->addSpacing(4);
	setLayout(layout);
}
void CFaceImageWidget::hideEvent(QHideEvent *event)
{
	emit sigToggled(false);
	event->accept();
}
void CFaceImageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QBrush(Qt::white));
}
/*****************************************************************************************************************/
CharacterWidget::CharacterWidget(QWidget *parent)
: QWidget(parent)
{
	faceSquareSize = 32;
	columns = 14;
	lastKey = -1;
	setMouseTracking(true);
	scrollBarWidth = 30;
	facePrevWidget = new QWidget(this,Qt::ToolTip);
}
QSize CharacterWidget::sizeHint() const
{
	return QSize(columns*faceSquareSize+scrollBarWidth, (500/columns)*faceSquareSize);
}
void CharacterWidget::hideEvent(QHideEvent *event)
{
	facePrevWidget->hide();
	event->accept();
}
void CharacterWidget::mouseMoveEvent(QMouseEvent *event)
{
	uint key;
	QPoint widgetPosition = mapFromGlobal(event->globalPos());
	if (widgetPosition.x()/faceSquareSize >= columns)
	{
		key = -1;
		return;
	}
	else
	{
		key = ((widgetPosition.y())/faceSquareSize)*columns + widgetPosition.x()/faceSquareSize;
	}
	QVBoxLayout *layout = new QVBoxLayout(facePrevWidget);
	QLabel *label = new QLabel(facePrevWidget);
	facePrevMovie = new QMovie(facePrevWidget);
	QPalette pat;
	pat.setBrush(QPalette::Background,QBrush(Qt::white));
	facePrevWidget->setPalette(pat);
	facePrevWidget->resize(faceSquareSize-1,faceSquareSize-1);
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(0);
	layout->addWidget(label);
	label->setAlignment(Qt::AlignCenter);
	label->setMovie(facePrevMovie);
	facePrevMovie->setSpeed(80);
	facePrevMovie->setFileName(QString(":/pic/images/QQ/1.gif"));
	facePrevMovie->start();
	facePrevWidget->show();
	facePrevWidget->move(mapToGlobal(QPoint(key%columns*faceSquareSize+1,key/columns*faceSquareSize+1)));
	QToolTip::showText(event->globalPos(),QString::number(key), facePrevWidget);
}

void CharacterWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		lastKey = (event->y()/faceSquareSize)*columns + event->x()/faceSquareSize;
		if (QChar(lastKey).category() != QChar::NoCategory)
			emit characterSelected(QString(QChar(lastKey)));
		update();
	}
	else
		QWidget::mousePressEvent(event);
}

void CharacterWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QBrush(Qt::white));

	QRect redrawRect = event->rect();
	int beginRow = redrawRect.top()/faceSquareSize;
	int endRow = redrawRect.bottom()/faceSquareSize;
	int beginColumn = redrawRect.left()/faceSquareSize;
	int endColumn = redrawRect.right()/faceSquareSize;

	painter.setPen(QPen(Qt::gray));
	for (int row = beginRow; row <= endRow; ++row) {
		for (int column = beginColumn; column <= endColumn; ++column) {
			painter.drawRect(column*faceSquareSize, row*faceSquareSize, faceSquareSize, faceSquareSize);
		}
	}
	for (int row = beginRow; row <= endRow; ++row) {

		for (int column = beginColumn; column <= endColumn; ++column) {

			int key = row*columns + column;
			//painter.setClipRect(column*faceSquareSize, row*faceSquareSize, faceSquareSize, faceSquareSize);

			if (key == lastKey)
			{
				painter.setPen(QPen(Qt::red));
				painter.drawRect(column*faceSquareSize, row*faceSquareSize, faceSquareSize, faceSquareSize);
			}
			QPixmap pixmap;
			pixmap.load(QString::fromLocal8Bit(":/pic/images/QQ/%1.gif").arg(key+1));
			painter.drawPixmap(column*faceSquareSize + faceSquareSize/2 - pixmap.width()/2,
				row*faceSquareSize + faceSquareSize/2 - pixmap.height()/2,pixmap);
		}
	}
}
void CharacterWidget::leaveEvent(QEvent *event)
{
	facePrevWidget->hide();
}