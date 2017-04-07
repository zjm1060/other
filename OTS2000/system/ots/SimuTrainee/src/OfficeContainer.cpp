#include "OfficeContainer.h"

OfficeContainer::OfficeContainer(QWidget *parent /*= NULL*/):QWidget(parent)
{
	pFramerControl = new FramerControl(this);
	pFramerControl->SetMenubar(false);
	pFramerControl->SetTitlebar(false);
	ply = new QGridLayout;
	ply->addWidget(pFramerControl,0,0,1,1);
	ply->setContentsMargins(0,0,0,0);
	this->setLayout(ply);
	//dzy added 
	ptimer  = new QTimer(this);
	ptimer->setSingleShot(true);
	waitDialog = new QWidget(this,Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
//	pFramer = new QFrame;
// 	ply2 = new QGridLayout;
// 	ply2->addWidget(pFramerControl,0,0,1,1);
// 	ply2->setContentsMargins(0,0,0,0);
// 	ply2 = new QGridLayout;
// 	ply2->addWidget(waitDialog,0,0,1,1);
	//waitDialog->setAttribute(Qt::WA_TranslucentBackground, true);
	//waitDialog->setVisible(false);
	connect(ptimer,SIGNAL(timeout()),this,SLOT(close_wait()));
}

OfficeContainer::~OfficeContainer()
{
	pFramerControl->Close();
	delete pFramerControl;
}

void OfficeContainer::Open(const QVariant& Document)
{
	if(pFramerControl)
	{
		showwait();
		pFramerControl->Open(Document);
	}
}

void OfficeContainer::Open(const QVariant& Document, const QVariant& ReadOnly)
{
	if(pFramerControl)
	{
		showwait();
		pFramerControl->Close();
		pFramerControl->Open(Document,ReadOnly);
	}
}

void OfficeContainer::Close()
{
	if(pFramerControl)
	{
	//	this->setLayout(ply2);
		showtips();
		pFramerControl->Close();
	}
}

void OfficeContainer::Save()
{
	pFramerControl->Save();
}

void OfficeContainer::Save(const QVariant& SaveAsDocument)
{
	pFramerControl->Save(SaveAsDocument);
}

void OfficeContainer::Save(const QVariant& SaveAsDocument, const QVariant& OverwriteExisting)
{
	pFramerControl->Save(SaveAsDocument,OverwriteExisting);
}

QString OfficeContainer::DocumentFullName() const
{
	return pFramerControl->DocumentFullName();
}

QString OfficeContainer::DocumentName() const
{
	return pFramerControl->DocumentName();
}

bool OfficeContainer::IsDirty() const
{
	return pFramerControl->IsDirty();
}

bool OfficeContainer::IsReadOnly() const
{
	return pFramerControl->IsReadOnly();
}

void OfficeContainer::ShowDialog(DialogType type)
{
	pFramerControl->ShowDialog((DSOFramer::dsoShowDialogType)type);
}
void OfficeContainer::showwait()
{	
// 	
// 	waitDialog->setVisible(true);		
	ptimer->start(5000);
	QMovie *movie = new QMovie(waitDialog);
	movie->setFileName(QString::fromLocal8Bit(":/pic/images/wait.gif"));
	QLabel *label = new QLabel(waitDialog);
	QLabel *labelMovie = new QLabel(waitDialog);
	labelMovie->setMovie(movie);
	movie->setSpeed(80);
	movie->start();
	QFont f_label(QString::fromLocal8Bit("华文行楷"), 70, QFont::Bold);
	label->setText(QString::fromLocal8Bit("文档载入中，请耐心等待\n  首次打开文档载入较慢    "));
	QPalette p_label;
	p_label.setColor(QPalette::WindowText,Qt::blue);
	label->setPalette(p_label);
	label->setFont(f_label);
	QGridLayout *layout = new QGridLayout(waitDialog);
	layout->addWidget(label,0,0,Qt::AlignCenter);
	layout->addWidget(labelMovie,1,0,Qt::AlignCenter);
	QRect rect = QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(this));
	waitDialog->resize(rect.size());	
	waitDialog->show();
//	ply->addWidget(waitDialog,0,0,1,1);

}
void OfficeContainer::showtips()
{
// 	QLabel *label = new QLabel(pFramer);
// 	label->setText(QString::fromLocal8Bit("请选择左侧学习文档列表进行学习，\n      请耐心等待"));
}
void OfficeContainer::close_wait()
{
	this->setLayout(ply);	
	waitDialog->close();
}