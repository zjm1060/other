#include "LocalOperate.h"

QHash<QString,QString> mapOptImage;
CLocalOprate::CLocalOprate(QWidget* parent)
:QWidget(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	dlg_created_flag[LOCALOPRDLG] = true;
	setWindowTitle(QString::fromLocal8Bit("其它运行操作"));

	m_opt_type = -1;
	m_opt_id = -1;
	m_opt_name.clear();
	initMapOptImage();
	lableReport= new QLabel(QString::fromLocal8Bit("汇报内容"));
	editReport = new QTextEdit;
	editReport->setMinimumHeight(30);
	lableReport->setVisible(false);
	editReport->setVisible(false);
	setupUi();
	this->setObjectName("localopr");
	connect(tabWidget,SIGNAL(currentChanged ( int )),this,SLOT(on_currentChanged ( int ) ) );
//	setModal(true);
}
CLocalOprate::~CLocalOprate()
{
	dlg_created_flag[LOCALOPRDLG] = false;
	mapOptImage.clear();
}
void CLocalOprate::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QBrush(Qt::white));
}
void CLocalOprate::setupUi()
{
	tabWidget = new QTabWidget;
	prevWidget = new QWidget;
	labelPixmap = new QLabel;
	labelName = new QLabel;
	buttonEnter = new QToolButton;
	buttonFilter = new QToolButton;
	buttonExit = new QPushButton(QString::fromLocal8Bit("    退    出  "));

// 	dbopr = new QSqlDatabase;
// 	dbopr->setDatabaseName("dblocalpr");
// 	if (ConnectHDB(*dbopr,"dblocalpr")!=TRUE)
// 	{
// 		return;
// 	}
///HN2014 mysql test
	int server_port;
	if (strcmp(g_ots_hdb_type,"ORACLE")==0)
	{		
		server_port = 1521;
		dbopr =  QSqlDatabase::addDatabase("QOCI","dblocalpr"); 
	}
	else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
	{
		server_port = 3306;
		dbopr =  QSqlDatabase::addDatabase("QMYSQL","dblocalpr"); 
	}
//	dbopr = QSqlDatabase::addDatabase("QMYSQL","dblocalpr"); 
	dbopr.setHostName(g_ots_hdb_ip);   
	dbopr.setDatabaseName(g_ots_hdb_sid);   
	dbopr.setUserName("ots");   
	dbopr.setPassword("ots");   
	dbopr.setPort(server_port);   
	if (dbopr.open())   
	{
		if (strcmp(g_ots_hdb_type,"MYSQL")==0)
			dbopr.exec(QString::fromLocal8Bit("SET NAMES 'utf8'")); 
	}
	else
	{
		return;
	}
	
	DB_FMT data;
	InitDbData(&data,TABLE_OPR_TYPE);
	if( SelectRecs(&data,dbopr) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error,dbopr);
		//FreeDbData(&data,TABLE_OPR_TYPE);
		qDebug("查询TABLE_OPR_TYPE失败!");
	}
	else
	{
		for(int i=0;i<data.rownum;i++)
		{
			QScrollArea *scrollArea = new QScrollArea;
			scrollArea->setFrameShadow(QFrame::Raised);
			QWidget *scrollAreaWidgetContents = new QWidget();
			scrollArea->setWidgetResizable(true);
			scrollArea->setWidget(scrollAreaWidgetContents);
			tabWidget->addTab(scrollArea,QString::fromLocal8Bit(data_opr_type[i].name));
			scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
			scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			scrollArea->verticalScrollBar()->setSingleStep(61);
			scrollArea->verticalScrollBar()->setPageStep(61*2);
			QGridLayout *gridLayout = new QGridLayout(scrollAreaWidgetContents);
			//scrollAreaWidgetContents->setMaximumWidth(135*3);
			gridLayout->setContentsMargins(5,5,5,5);
			DB_FMT data_opt;
			InitDbData(&data_opt,TABLE_LOCAL_OPR);
			data_opt.cond_flag.flag_local_opr.opr_type = TRUE;
			data_opt.cond_seg.seg_local_opr.opr_type = data_opr_type[i].opr_type;
			if( SelectRecs(&data_opt,dbopr) != TRUE)
			{
				HdbLogWrite(data_opt.sql_str,data_opt.sql_error,dbopr);
				qDebug("查询TABLE_LOCAL_OPR失败!");
			}
				//MySleep(480);//dzyadded;
			for(int j=0;j<data_opt.rownum;j++)
			{
				COptWidget *optWidget = new COptWidget;
				optWidget->setOptType(data_local_opr[j].opr_type);
				optWidget->setOptId(data_local_opr[j].opr_id);
				optWidget->setText(data_local_opr[j].name);

				LocalOperateHash.insert(QString::fromLocal8Bit(data_local_opr[j].name),qMakePair(data_local_opr[j].opr_type,data_local_opr[j].opr_id));
				if (mapOptImage.count(data_local_opr[j].name) != 0)
					optWidget->setPixmap(QPixmap(mapOptImage.value(data_local_opr[j].name)));
				//optWidget->setPixmapSize(128,128,Qt::KeepAspectRatio);
				optWidget->setPixmapSize(128,128,Qt::IgnoreAspectRatio);
				gridLayout->addWidget(optWidget,j/3,j%3);
				connect(scrollArea->verticalScrollBar(),SIGNAL(valueChanged(int)),optWidget,SLOT(update()));
				connect(optWidget,SIGNAL(optSelected(int,int,const QString &)),this,SLOT(optPreview(int,int,const QString &)));
			}
			FreeDbData(&data,TABLE_OPR_TYPE);
		}
	}
	tabWidget->setMinimumHeight(485);
	prevWidget->setMinimumSize(350,350);
	QVBoxLayout *layoutPixmap = new QVBoxLayout(prevWidget);
	layoutPixmap->setContentsMargins(0,0,0,0);
	spacer1 =  new QSpacerItem(10, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);
	layoutPixmap->addItem(spacer1);
	layoutPixmap->addWidget(labelName);
	layoutPixmap->addWidget(labelPixmap);
	spacer2 =  new QSpacerItem(10, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);
	layoutPixmap->addItem(spacer2);
	labelPixmap->setPixmap(prevPixmap);
	labelPixmap->setAlignment(Qt::AlignCenter); 
	labelName->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	labelName->adjustSize();
	labelName->setWordWrap(true);
	labelName->setScaledContents(true);
	labelName->setStyleSheet(QString::fromLocal8Bit("QLabel{color: rgb(255, 0, 0);font: 18pt 宋体;}"));
	buttonEnter->setText(QString::fromLocal8Bit("          执   行   此   操   作     "));
	buttonEnter->setAutoRaise(true);
	buttonEnter->setStyleSheet(QString::fromLocal8Bit("QToolButton{color: rgb(0, 0, 255);font: 16pt 宋体;}"));
	QIcon icon;
	buttonExit->setMaximumWidth(200);
	icon.addFile(QString::fromLocal8Bit(":/pic/images/search.png"), QSize(), QIcon::Normal, QIcon::Off);
	buttonFilter->setIcon(icon);
	buttonFilter->setAutoRaise(true);

	QHashIterator<QString,QPair<short,int>> i(LocalOperateHash);
	while (i.hasNext()) 
	{
		i.next();
		LocalOperateList.append(i.key());
	}
//	editNameFilter= new CompleteLineEdit(LocalOperateList,this);
// 	editNameFilter->setMinimumWidth(600);
// 	editNameFilter->setMaximumWidth(600);

	//completer = new QCompleter(LocalOperateList,this);
	//completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
	//editNameFilter->setCompleter(completer);
	
	QHBoxLayout *layoutFilter = new QHBoxLayout;
	QSpacerItem *horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
// 	layoutFilter->setSpacing(0);
// 	layoutFilter->addWidget(editNameFilter);
// 	layoutFilter->addWidget(buttonFilter);
 	layoutFilter->addItem(horizontalSpacer);
	layoutFilter->addWidget(buttonExit);
	QHBoxLayout *layoutRpt = new QHBoxLayout;//dzy
// 	layoutRpt->setSpacing(0);
// 	layoutRpt->addWidget(lableReport);
	lableReport->setStyleSheet(QString::fromLocal8Bit("QLabel{color: rgb(0, 0, 255);font: 16pt 宋体;}"));
//	editReport->setMinimumWidth(1000);
	editReport->setMaximumHeight(60);
//	layoutRpt->addWidget(editReport);
	QGridLayout *layout = new QGridLayout(this);
//	layout->setSizeConstraint(QLayout::SetFixedSize);
	layout->addWidget(prevWidget,0,0);
	layout->addWidget(tabWidget,0,1);

	layout->addWidget(lableReport,1,0,Qt::AlignCenter);	
	layout->addWidget(editReport,1,1/*,Qt::AlignRight*/);	
	layout->addWidget(buttonEnter,2,0,Qt::AlignCenter);	
	layout->addWidget(buttonExit,2,1,Qt::AlignRight);
//	layout->addLayout(layoutRpt,2,1,1,2,Qt::AlignCenter);//dzy
	


	//layout->addLayout(layoutFilter,2,0,1,2,Qt::AlignCenter);//dzy removed 20141226
//	editNameFilter->setVisible(false);
	setLayout(layout);
	this->setStyleSheet(QString::fromLocal8Bit("font: 16pt 宋体;"));
	connect(buttonEnter,SIGNAL(clicked()),this,SLOT(optRun()));
	connect(buttonExit,SIGNAL(clicked()),this,SLOT(onclose()));
	connect(buttonFilter,SIGNAL(clicked()),this,SLOT(optFilter()));
}
void CLocalOprate::optPreview(int opt_type,int opt_id,const QString &opt_name)
{
	m_opt_type = opt_type;
	m_opt_id = opt_id;
	m_opt_name = opt_name;
	labelName->setText(m_opt_name);
	if (mapOptImage.count(m_opt_name) == 0)
	{
		prevPixmap.load(mapOptImage.value("default"));
		labelPixmap->setPixmap(prevPixmap.scaled(350,350,Qt::KeepAspectRatio));
	}
	else
	{
		prevPixmap.load(mapOptImage.value(m_opt_name));
		labelPixmap->setPixmap(prevPixmap.scaled(350,350,Qt::KeepAspectRatio));	
	}
}
void CLocalOprate::optRun()
{
	if (m_opt_id == -1)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("请选择要执行的操作!"),QMessageBox::Ok);
		return;
	}
	if(buttonEnter->text() == QString::fromLocal8Bit("          执  行  汇  报           ")&&editReport->toPlainText()!="")
	{
		m_opt_name = m_opt_name +QString::fromLocal8Bit(" 汇报内容: ")+editReport->toPlainText();
	}
	if (m_opt_name.length()>100)
	{
		QMessageBox().warning(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("汇报内容超出长度，请简要陈述"),QMessageBox::Ok);
		return;
	}
	DB_FMT data;
	InitDbData(&data,TABLE_RECORD);
	data.flag.flag_record.content = TRUE;
	data.flag.flag_record.host_id = TRUE;
	data.flag.flag_record.user_id = TRUE;
	data.flag.flag_record.groups_id = TRUE;
	data.flag.flag_record.datetime = TRUE;
	data.flag.flag_record.ms = TRUE;
	data.flag.flag_record.type = TRUE;
	data.flag.flag_record.dev_id = TRUE;
	data.flag.flag_record.station_id = TRUE;
	data.flag.flag_record.task_id = TRUE;
	data.flag.flag_record.send_task_id = TRUE;
	data.flag.flag_record.logicalname = TRUE;
	strcpy(data.seg.seg_record.content,m_opt_name.toLocal8Bit().data());
	data.seg.seg_record.host_id = g_host_id;
	data.seg.seg_record.user_id = login_user_id;
	data.seg.seg_record.groups_id = shm_ots_addr->groups_id;
	DATE_TIME tm;
	GetSysTime(&tm);
	GetCurTimeStr(data.seg.seg_record.datetime);
	data.seg.seg_record.ms = tm.ms;
	data.seg.seg_record.type = 3;// 现地操作
	data.seg.seg_record.dev_id = 0;
	data.seg.seg_record.station_id = 0;
	if (shm_ots_addr->data_task.task_status == 2)
	{
		data.seg.seg_record.task_id = shm_ots_addr->data_task.task_id;
		data.seg.seg_record.send_task_id = shm_ots_addr->data_task.send_task_id;
	}
	else
	{
		data.seg.seg_record.task_id = 0;
		data.seg.seg_record.send_task_id = 0;
	}
	QString logicalname = QString("%1.%2").arg(m_opt_type).arg(m_opt_id);
	strcpy(data.seg.seg_record.logicalname,logicalname.toLocal8Bit().data());
	if( InsertOneRec(&data,dbopr) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error,dbopr);
		FreeDbData(&data,TABLE_RECORD);
		qDebug("插入数据失败!");
		return;
	}
// 	if (m_opt_name.size()>26)//dzy
// 	{	
// 			QStringList m_slist;
// 			m_slist = m_opt_name.split(QString::fromLocal8Bit(" 汇报内容: "));
// 			if (m_slist.length()>0)
// 			{
// 				m_opt_name = m_slist.at(0);
// 				if (m_opt_name.size()>26)
// 				{
// 					m_opt_name = QString::fromLocal8Bit("执行其他操作");
// 				}
// 			}
// 			else
// 				m_opt_name = QString::fromLocal8Bit("执行其他操作");
// 	}
	//WriteRec(m_opt_name.toLocal8Bit().data());  //DZY MODIFIED  FOR HUNAN140705
//	QTimer timer;
	QMessageBox *newmsgbox = new QMessageBox(QMessageBox::Information,QString::fromLocal8Bit("提示!"),QString::fromLocal8Bit("执行%1成功!").arg(m_opt_name),QMessageBox::Ok,this);
//connect(&timer,SIGNAL(timeout()),newmsgbox,SLOT(close()));
	newmsgbox->setModal ( false );
	newmsgbox->show();
	editReport->clear();
	m_opt_name.clear();
	FreeDbData(&data,TABLE_RECORD);
}
void CLocalOprate::optFilter()
{
	if (LocalOperateHash.contains(editNameFilter->text()))
	{
		int opr_type = LocalOperateHash.value(editNameFilter->text()).first;
		int opr_id = LocalOperateHash.value(editNameFilter->text()).second;
		QString opr_name = editNameFilter->text();
		optPreview(opr_type,opr_id,opr_name);
	}
}
void CLocalOprate::initMapOptImage()
{
	QDir dir;
	dir.setPath(QString::fromLocal8Bit("/home/ems/h9000/ots/images/opt"));
	if (!dir.exists())
		dir.setPath(QString::fromLocal8Bit("/home/ems/h9000/bin/images/opt"));
	dir.setFilter(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Hidden);
	QStringList filters;
	filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.gif";
	QFileInfoList listImage = dir.entryInfoList(filters);
	mapOptImage.clear();
	for (int i=0;i<listImage.size();++i)
	{
		mapOptImage.insert(listImage.at(i).baseName(),listImage.at(i).absoluteFilePath());
	}
}

void  CLocalOprate::onclose()
{
	emit closedlg();
	//this->close();
}
void  CLocalOprate::on_currentChanged ( int index )
{
	if (index!=4)
	{
		buttonEnter->setText(QString::fromLocal8Bit("执  行  此  操  作"));
		lableReport->setVisible(false);
		editReport->setVisible(false);
		return;
	}
	else
	{
		buttonEnter->setText(QString::fromLocal8Bit("          执  行  汇  报           "));
		lableReport->setVisible ( true );
		editReport->clear();
		editReport->setVisible ( true );
	}
}
/*******************************************************COptWidget**********************************************************/
COptWidget::COptWidget(QWidget *parent)
: QWidget(parent)
{
	m_opt_id = -1;
	m_pixmapSize.setWidth(128);
	m_pixmapSize.setHeight(128);
	QVBoxLayout *layout = new QVBoxLayout(this);
	QHBoxLayout *layout_h = new QHBoxLayout(this);
	m_labelText = new QLabel(this);
	m_labelPixmap = new QLabel(this);
	m_pixmap.load(mapOptImage.value("default"));
	m_labelText->setText(QString::fromLocal8Bit("示例"));
	m_labelText->adjustSize();
	m_labelText->setWordWrap(true);
	m_labelText->setScaledContents(true);
	m_labelText->setStyleSheet("QLabel{background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.077, fx:0.5, fy:0.5, stop:0.835227 rgba(192, 238, 239, 255), stop:0.857955 rgba(0, 169, 255, 147), stop:0.931818 rgba(221, 221, 221, 245));;color:black}");
	m_labelText->setMinimumHeight(15*3);
	m_labelText->setMaximumHeight(15*3);
	m_labelPixmap->setMinimumSize(128,128);
	m_labelPixmap->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	setPixmapSize(m_pixmapSize,Qt::KeepAspectRatio);
	layout_h->setSpacing(20);
	layout->setContentsMargins(1,1,1,1);
	layout->setSpacing(0);
	m_labelPixmap->setAlignment(Qt::AlignCenter);
	m_labelText->setAlignment(Qt::AlignCenter);
	layout_h->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	layout->addWidget(m_labelText);
	layout->addWidget(m_labelPixmap);
}
void COptWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QBrush(Qt::white));
	painter.setPen(QPen(Qt::gray));
	painter.drawRect(event->rect().x(),event->rect().y(),event->rect().width()-1,event->rect().height()-1);
}

void COptWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) 
	{
		emit optSelected(m_opt_type,m_opt_id,m_labelText->text());
	}
	else
		QWidget::mousePressEvent(event);
}
void COptWidget::setText(const QString &text)
{
	m_labelText->setText(text);
}
void COptWidget::setPixmap(const QPixmap &pixmap)
{
	m_pixmap = pixmap;
	m_labelPixmap->setPixmap(m_pixmap);
}
void COptWidget::setOptType(int opt_type)
{
	m_opt_type = opt_type;
}
void COptWidget::setOptId(int opt_id)
{
	m_opt_id = opt_id;
}
void COptWidget::setPixmapSize(int width,int heigth,Qt::AspectRatioMode mode)
{
	m_pixmapSize.setWidth(width);
	m_pixmapSize.setHeight(heigth);
	setPixmap(m_pixmap.scaled(width,heigth,mode));
}
void COptWidget::setPixmapSize(const QSize &size,Qt::AspectRatioMode mode)
{
	m_pixmapSize = size;
	setPixmap(m_pixmap.scaled(size,mode));
}
QString COptWidget::getText() const
{
	return m_labelText->text();
}
QPixmap COptWidget::getPixmap() const
{
	return m_pixmap;
}
QSize COptWidget::getSize() const
{
	return m_pixmapSize;
}
int COptWidget::getOptType()
{
	return m_opt_type;
}
int COptWidget::getOptId()
{
	return m_opt_id;
}
/*******************************************************COptImageWidget*******************************************************/
COptImageWidget::COptImageWidget(QWidget *parent)
: QWidget(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);	//dzy 140706
	columns = 7;
	oldKey = curKey = lastKey = -1;
	scrollBarWidth = 30;
	pixmap_no.load(QString::fromLocal8Bit(":/pic/images/opt/无图片.jpg"));
	optPrevWidget= new COptPrevWidget(this);
	setMouseTracking(true);
	listOpt.clear();
}
QSize COptImageWidget::sizeHint() const
{
	return QSize(columns*squareSize+scrollBarWidth, (listOpt.size()%columns ? (listOpt.size()+columns)/columns : listOpt.size()/columns)*squareSize+1);
}
void COptImageWidget::hideEvent(QHideEvent *event)
{
	optPrevWidget->hide();
	event->accept();
}
void COptImageWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPoint widgetPosition = mapFromGlobal(event->globalPos());
	if (widgetPosition.x()/squareSize >= columns)
	{
		curKey = -1;
		optPrevWidget->hide();
		return;
	}
	else
	{
		curKey = ((widgetPosition.y())/squareSize)*columns + widgetPosition.x()/squareSize;
	}
	if (curKey != oldKey)
	{
		lastKey = (event->y()/squareSize)*columns + event->x()/squareSize;
		update();
		if (curKey < listOpt.size())
		{
			if (mapOptImage.count(listOpt.at(curKey)) == 0)
			{
				optPrevWidget->setPrevPixmap(QPixmap(QString::fromLocal8Bit(":/pic/images/opt/无图片.jpg")));
				optPrevWidget->setPixmapScale(prevSquareSize,prevSquareSize,Qt::KeepAspectRatio);
			}
			else
			{
				optPrevWidget->setPrevPixmap(QPixmap(mapOptImage.value(listOpt.at(curKey))));
				optPrevWidget->setPixmapScale(prevSquareSize,prevSquareSize,Qt::KeepAspectRatio);
			}
			optPrevWidget->show();
			optPrevWidget->move(QCursor::pos()+QPoint(10,10));
		}
		else
			optPrevWidget->hide();
		oldKey = curKey;
	}
	else
		optPrevWidget->move(QCursor::pos()+QPoint(10,10));
	event->accept();
}

void COptImageWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) 
	{
		lastKey = (event->y()/squareSize)*columns + event->x()/squareSize;
		if (lastKey<listOpt.size())
			emit optSelected(listOpt.at(lastKey));
	}
	else
		QWidget::mousePressEvent(event);
}

void COptImageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QBrush(Qt::white));

	int width = 10;
	QRect redrawRect = event->rect();
	int beginRow = redrawRect.top()/squareSize;
	int endRow = redrawRect.bottom()/squareSize;
	int beginColumn = redrawRect.left()/squareSize;
	int endColumn = (redrawRect.right()-scrollBarWidth)/squareSize;

	QPen pen;
	//pen.setWidth(10);
	//pen.setColor(this->palette().color(QPalette::Window));
	//pen.setColor(Qt::blue);
	painter.setPen(QPen(Qt::gray));
	//painter.setPen(pen);
	for (int row = beginRow; row <= endRow; ++row) 
	{
		for (int column = beginColumn; column <= endColumn; ++column) 
		{
			painter.drawRect(column*squareSize, row*squareSize, squareSize, squareSize);
		}
	}

	for (int row = beginRow; row <= endRow; ++row) 
	{
		for (int column = beginColumn; column <= endColumn; ++column) 
		{
			int key = row*columns + column;
			if (key == lastKey)
			{
				painter.setPen(QPen(Qt::red));
				painter.drawRect(column*squareSize, row*squareSize, squareSize, squareSize);
			}
			
			if (key < listOpt.size())
			{
				if (mapOptImage.count(listOpt.at(key)) == 0)
					pixmap_show = pixmap_no.scaled(squareSize-2,squareSize-2,Qt::KeepAspectRatio);
				else
					pixmap_show = QPixmap(mapOptImage.value(listOpt.at(key))).scaled(squareSize-2,squareSize-2,Qt::KeepAspectRatio);
				painter.drawPixmap(column*squareSize + 1 + squareSize/2 - pixmap_show.width()/2,
					row*squareSize + squareSize/2 - pixmap_show.height()/2,pixmap_show);
			}
		}
	}
}
void COptImageWidget::leaveEvent(QEvent *event)
{
	optPrevWidget->hide();
}
void COptImageWidget::setOptList(QStringList m_listOpt)
{
	listOpt = m_listOpt;
}
/****************************************************************COptSelectWidget***************************************************************************/
COptSelectWidget::COptSelectWidget(QWidget *parent)
: QWidget(parent)
{
	oldKey = curKey = -1;
	pixmap_no.load(QString::fromLocal8Bit(":/pic/images/opt/无图片.jpg"));
	optPrevWidget= new COptPrevWidget(this);
	setMouseTracking(true);
}
QSize COptSelectWidget::sizeHint() const
{
	return QSize(squareSize*20+1,squareSize+1);
}
void COptSelectWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPoint widgetPosition = mapFromGlobal(event->globalPos());
	if (widgetPosition.y()/squareSize >= 1)
	{
		curKey = -1;
		optPrevWidget->hide();
		return;
	}
	else
	{
		curKey = widgetPosition.x()/squareSize;
	}
	if (curKey != oldKey)
	{
		if (curKey < listOpt.size())
		{
			if (mapOptImage.count(listOpt.at(curKey)) == 0)
			{
				optPrevWidget->setPrevPixmap(QPixmap(QString::fromLocal8Bit(":/pic/images/opt/无图片.jpg")));
				optPrevWidget->setPixmapScale(prevSquareSize,prevSquareSize,Qt::KeepAspectRatio);
			}
			else
			{
				optPrevWidget->setPrevPixmap(QPixmap(mapOptImage.value(listOpt.at(curKey))));
				optPrevWidget->setPixmapScale(prevSquareSize,prevSquareSize,Qt::KeepAspectRatio);
			}
			optPrevWidget->show();
			optPrevWidget->move(QCursor::pos()+QPoint(10,10));
		}
		else
			optPrevWidget->hide();
		oldKey = curKey;
	}
	else
		optPrevWidget->move(QCursor::pos()+QPoint(10,10));
	event->accept();
}

void COptSelectWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QBrush(Qt::white));

	QRect redrawRect = event->rect();
	int num = listOpt.size();

	painter.setPen(QPen(Qt::gray));
	for (int key=0; key<num; key++) 
	{
		painter.drawRect(key*squareSize, 0, squareSize, squareSize);
	}
	
	for (int key=0; key<num; key++) 
	{
		if (mapOptImage.count(listOpt.at(key)) == 0)
			pixmap_show = pixmap_no.scaled(squareSize-2,squareSize-2,Qt::KeepAspectRatio);
		else
			pixmap_show = QPixmap(mapOptImage.value(listOpt.at(key))).scaled(squareSize-2,squareSize-2,Qt::KeepAspectRatio);
		painter.drawPixmap(key*squareSize + 1 + squareSize/2 - pixmap_show.width()/2,
			1 + squareSize/2 - pixmap_show.height()/2,pixmap_show);
	}
}
void COptSelectWidget::leaveEvent(QEvent *event)
{
	optPrevWidget->hide();
}
void COptSelectWidget::addOpt(int id,const QString &name)
{
	sizeHint();
	if (!listOpt.contains(name))
	{
		listOpt.append(name);
		update();
	}
}
void COptSelectWidget::delOpt(const QString &name)
{
	listOpt.removeOne(name);
	update();
}
QStringList COptSelectWidget::getAllOpt() const
{
	return listOpt;
}
/****************************************************************COptPrevWidget**************************************************************************/
COptPrevWidget::COptPrevWidget(QWidget *parent)
: QWidget(parent,Qt::ToolTip)
{
	setAttribute(Qt::WA_DeleteOnClose);	//dzy 140706
	QVBoxLayout *layout = new QVBoxLayout(this);
	label = new QLabel(this);
	prevPixmap.load(QString::fromLocal8Bit(":/pic/images/opt/无图片.jpg"));
	QPalette pat;
	pat.setBrush(QPalette::Background,QBrush(Qt::gray));
	setPalette(pat);
	setMinimumSize(prevSquareSize,prevSquareSize);
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(0);
	layout->addWidget(label);
	label->setAlignment(Qt::AlignCenter); 
	label->setPalette(pat);
	setPixmapScale(256,256,Qt::KeepAspectRatio);
}
void COptPrevWidget::setPrevPixmap(const QPixmap &pixmap)
{
	prevPixmap = pixmap;
	label->setPixmap(prevPixmap);
}
void COptPrevWidget::setPrevMovie(QMovie *movie)
{

}
void COptPrevWidget::setPixmapScale(int width,int heigth,Qt::AspectRatioMode mode)
{
	pixmapSize.setWidth(width);
	pixmapSize.setHeight(heigth);
	label->setPixmap(prevPixmap.scaled(width,heigth,mode));
}
void COptPrevWidget::setPixmapScale(const QSize &size,Qt::AspectRatioMode mode)
{
	pixmapSize = size;
	label->setPixmap(prevPixmap.scaled(size,mode));
}