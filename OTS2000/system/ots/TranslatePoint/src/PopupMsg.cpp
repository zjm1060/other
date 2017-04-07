#include "inc/PopupMsg.h"
#include "inc/common.h"

CPopupMsg::CPopupMsg(QWidget* parent)
:QWidget(parent,Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint)
{	  
	setAttribute(Qt::WA_TranslucentBackground, true);
	setStyleSheet(QString::fromLocal8Bit("QWidget{color: rgb(255, 255, 255);font: 12pt 宋体;}"));
	refresh = new QTimer(this);
	autoHide = new QTimer(this);
	autoHide->setSingleShot(true);
	QVBoxLayout *layout = new QVBoxLayout(this);
	QHBoxLayout *hlayout1 = new QHBoxLayout();
	QHBoxLayout *hlayout2 = new QHBoxLayout();
	QHBoxLayout *hlayout3 = new QHBoxLayout();
	QHBoxLayout *hlayout4 = new QHBoxLayout();
	QLabel *label1 = new QLabel(QString::fromLocal8Bit("点号:"),this);
	QLabel *label2 = new QLabel(QString::fromLocal8Bit("点逻辑名:"),this);
	QLabel *label3 = new QLabel(QString::fromLocal8Bit("点中文名:"),this);
	QLabel *label4 = new QLabel(QString::fromLocal8Bit("实时值:"),this);
	label1_text = new QLabel(this);
	label2_text = new QLabel(this);
	label3_text = new QLabel(this);
	label4_text = new QLabel(this);
	hlayout1->addWidget(label1);
	hlayout1->addWidget(label1_text,Qt::AlignRight);
	hlayout2->addWidget(label2);
	hlayout2->addWidget(label2_text,Qt::AlignRight);
	hlayout3->addWidget(label3);
	hlayout3->addWidget(label3_text,Qt::AlignRight);
	hlayout4->addWidget(label4);
	hlayout4->addWidget(label4_text,Qt::AlignRight);
	layout->addLayout(hlayout1);
	layout->addLayout(hlayout2);
	layout->addLayout(hlayout3);
	layout->addLayout(hlayout4);
	connect(refresh, SIGNAL(timeout()), this, SLOT(updateMsg()));
	connect(autoHide, SIGNAL(timeout()), this, SLOT(hide()));
	//HWND desktopHwnd = findDesktopIconWnd();
	//if(desktopHwnd) 
		//SetParent(winId(), desktopHwnd);
}
CPopupMsg::~CPopupMsg()
{

}
void CPopupMsg::showEvent(QShowEvent *event)
{
	refresh->start(1000);
	event->accept();
}
void CPopupMsg::hideEvent(QHideEvent *event)
{
	if (refresh->isActive())
		refresh->stop();
	event->accept();
}
void CPopupMsg::leaveEvent(QEvent *event)
{
	autoHide->start(5000);
	event->accept();
}
void CPopupMsg::enterEvent(QEvent *event)
{
	autoHide->stop();
	event->accept();
}
void CPopupMsg::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) //点击左边鼠标
	{
		dragPosition = event->globalPos() - frameGeometry().topLeft();
		//globalPos()获取根窗口的相对路径，frameGeometry().topLeft()获取主窗口左上角的位置
		event->accept();   //鼠标事件被系统接收
	}
	if (event->button() == Qt::RightButton) 
	{
		this->hide();
	}
}
void CPopupMsg::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() == Qt::LeftButton) //当满足鼠标左键被点击时。
	{
		move(event->globalPos() - dragPosition);//移动窗口
		event->accept();
	}
}
void CPopupMsg::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.fillRect(this->rect(),QColor(0,0,0,150));//0xff
}

void CPopupMsg::updateMsg()
{
	char info_tmp[POINT_LONGNAME_SIZE];
	POINTER point;
	DMS_COMMON check_dms_cmm;
	char str[100];
	strcpy(str,m_selectedText.toLocal8Bit().data());
	if (GetPtIdByNameStr(str,&point)==-1)
	{
		label1_text->setText(QString::fromLocal8Bit("该点不存在"));
		label2_text->setText("");
		label3_text->setText("");
		label4_text->setText("");
		return;
	}
	if (ReadEntryById (&point,&check_dms_cmm) == -1)
	{		
		label1_text->setText(QString::fromLocal8Bit("该点不存在"));
		label2_text->setText("");
		label3_text->setText("");
		label4_text->setText("");
		qDebug();
		return;
	}
	GetPointInfo(&check_dms_cmm,info_tmp,TYPE_ID);
	label1_text->setText(QString::fromLocal8Bit(info_tmp));
	GetPointInfo(&check_dms_cmm,info_tmp,TYPE_NAME);
	label2_text->setText(QString::fromLocal8Bit(info_tmp));
	GetPointInfo(&check_dms_cmm,info_tmp,TYPE_LONGNAME);
	label3_text->setText(QString::fromLocal8Bit(info_tmp));
	GetPointInfo(&check_dms_cmm,info_tmp,TYPE_VALUE);
	label4_text->setText(QString::fromLocal8Bit(info_tmp));
}
void CPopupMsg::setSelectedText(const QString &selectedText)
{
	m_selectedText = selectedText;
}