#include "qbuttonlist.h"

#include <QPainter>

#include "define.h"
#include "common.h"

QButtonList::QButtonList(QWidget *parent)
	: QFrame(parent)
{
	m_font.setPointSize(10);

	m_pGroup=new QButtonGroup(this);
	m_pGroup->setExclusive(true);

	m_nHeight=parseConfig(getAppParentDir()+INI_FILE, "button/height", 35).toInt();
	m_nSpacing=parseConfig(getAppParentDir()+INI_FILE, "button/spacing", 15).toInt();
	m_nBorder=parseConfig(getAppParentDir()+INI_FILE, "button/border", 5).toInt();

	connect(m_pGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(buttonClicked(int)));
}

QButtonList::~QButtonList()
{

}

void QButtonList::addButton(const QString& text, int id)
{
	QToolButton* pButton=new QToolButton(this);
	pButton->setText(text);
	pButton->setFont(m_font);
	pButton->setAutoRaise(true);
	pButton->setAutoExclusive(true);
	pButton->setCheckable(true);

	m_pGroup->addButton(pButton, id);
	m_listButtons.append(pButton);
}

int QButtonList::clickedButton()
{
	return m_pGroup->checkedId();
}

void QButtonList::click(int id)
{
	QAbstractButton* pButton=m_pGroup->button(id);
	Q_ASSERT_NO_NULL(pButton);

	pButton->click();
}

void QButtonList::paintEvent(QPaintEvent* event)
{
	int nCount=m_listButtons.count();
	if (nCount == 0)
	{
		return;
	}

	QPainter painter(this);
	int nWidth=contentsRect().width();


	QLinearGradient gradient(0.0, 0.0, nWidth, 0.0);
	gradient.setColorAt(0.0, palette().background().color());
	gradient.setColorAt(1.0, QColor(60, 150, 255));

	QBrush brush(gradient);
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);

	int y=m_nHeight+m_nSpacing;
	for (int i=0; i<nCount-1; i++)
	{
		painter.drawRect(0, y+m_nSpacing/2, nWidth, 1);

		y+=m_nHeight+m_nSpacing;
	}

	QFrame::paintEvent(event);
}

void QButtonList::resizeEvent(QResizeEvent* event)
{
	QSize size=event->size();

	int y=m_nSpacing;
	foreach (QToolButton* pButton, m_listButtons)
	{
		pButton->setGeometry(m_nBorder, y, size.width()-2*m_nBorder, m_nHeight);

		y+=m_nHeight+m_nSpacing;
	}
}
