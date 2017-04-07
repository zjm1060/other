#include "mdichild.h"

MdiChild::MdiChild()
{
	setAttribute(Qt::WA_DeleteOnClose);
	isUntitled = true;
	closeable = true ; 
	stackedWidget = new QStackedWidget(this);
	setWidget(stackedWidget);
	menu = new QMenu(this);
	setSystemMenu(menu);
   // QWidget::installEventFilter(this);//为这个窗口安装过滤器 
	flag_style = 0;
	//setonStyleSheet();
	//QPalette palette;
	//palette.setBrush(this->backgroundRole(), QBrush(QPixmap("./pic/images/background/hostpagebackround.png")));
	//this->setPalette(palette);
	//this->setAutoFillBackground(true);
	//this->setStyleSheet("background-image: url(:/pic/images/background/hostpagebackround.png);");
	connect(menu,SIGNAL(aboutToShow()),this,SLOT(setMenu()));
}

MdiChild::~MdiChild()
{
	menu->~QMenu();
	stackedWidget->~QStackedWidget();
}
void  MdiChild::closeEvent(QCloseEvent *event)
{
	 if (dlg_created_flag[TASKDLG] == true&&shm_ots_addr->data_task.task_status == 2)
	{
		PopupMessageBox(this,"当前模式不可关闭相关窗口",QMessageBox::Information,false);
		event->ignore();
	}
	else
		event->accept();
	return;
}
void MdiChild::switchWindow()
{
	int index = stackedWidget->currentIndex();
	index == 0 ? index = 1 : index = 0;
	stackedWidget->setCurrentIndex(index);
}
QString MdiChild::currentFileBaseName()
{
	return QFileInfo(m_sCurFile).baseName();
}

void MdiChild::setMenu()
{
	menu->clear();
	menu->addAction(QString::fromLocal8Bit("关闭"),this,SLOT(close()));
	menu->addSeparator();
	QAction *actionSwitch = menu->addAction(QString::fromLocal8Bit("切换窗口"),this,SLOT(switchWindow()));
}
void MdiChild::mouseDoubleClickEvent ( QMouseEvent * event )  
{
	if(event->type() ==QMouseEvent::MouseButtonDblClick )  
	{
		int my = this->pos().ry();
		int mmy = event->globalY ();

		if(mmy -my<=100)
		{	
			this->close();
		}
	}
}
// void  MdiChild::setonStyleSheet()
// {
// 	if (flag_style == 0)
// 	{
// 	//	setStyleSheet("");
// 		QFile file(":/pic/qss/mdidialog.qss");
// 		file.open(QFile::ReadOnly);
// 		QString styleSheet = QLatin1String(file.readAll());
// 		//qApp->setStyleSheet(styleSheet);
// 		this->setStyleSheet(styleSheet);	
// 	}
// }


MdiArea::MdiArea()
{
 	setAttribute(Qt::WA_DeleteOnClose);
}

MdiArea::~MdiArea()
{
 
}
void MdiArea::mouseDoubleClickEvent ( QMouseEvent * event )  
{
	if(event->type() ==QMouseEvent::MouseButtonDblClick )  
	{
		int my = this->pos().ry();
		if (event->globalY ()-my<=10)
		{	
			closeActiveSubWindow ()  ;
		}
	}
}
