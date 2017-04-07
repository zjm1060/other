#include "qbuttonlist.h"

#include <QToolButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPainter>

#include "define.h"
#include "common.h"

QButtonList::QButtonList(QWidget *parent)
	: QFrame(parent)
{
	m_font.setPointSize(10);

	m_pGroup=new QButtonGroup(this);
	setExclusive(true);
	connect(m_pGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(buttonClicked(int)));

	m_pCheckGroup=new QButtonGroup(this);
	m_pCheckGroup->setExclusive(false);
	connect(m_pCheckGroup, SIGNAL(buttonClicked(int)), this, SLOT(stateChanged(int)));

	m_nHeight=parseConfig(getAppParentDir()+INI_FILE, "button/height", 35).toInt();
	m_nSpacing=parseConfig(getAppParentDir()+INI_FILE, "button/spacing", 15).toInt();
	m_nBorder=parseConfig(getAppParentDir()+INI_FILE, "button/border", 5).toInt();
}

QButtonList::~QButtonList()
{

}

void QButtonList::setExclusive(bool exclusive)
{
	if (m_pGroup != NULL)
	{
		m_pGroup->setExclusive(exclusive);
	}
}

void QButtonList::addButton(const QString& text, int id)
{
	QToolButton* pButton=new QToolButton(this);
	pButton->setText(text);
	pButton->setAutoRaise(true);

	addButton(pButton, id);
}

void QButtonList::addButton(QAbstractButton* button, int id)
{
	if (button == NULL)
	{
		return;
	}

	button->setFont(m_font);
	button->setAutoExclusive(true);
	button->setCheckable(true);

	m_pGroup->addButton(button, id);
	m_listButtons.append(button);
}

void QButtonList::addCheckableButton(const QString& text, int id)
{
	QFrame* pFrame=new QFrame(this);
	QHBoxLayout* pLayout=new QHBoxLayout(pFrame);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(1, 1, 1, 1);
	pFrame->setLayout(pLayout);

	QCheckBox* pCheckButton=new QCheckBox(pFrame);
	pLayout->addWidget(pCheckButton);
	m_pCheckGroup->addButton(pCheckButton, id);

	QToolButton* pButton=new QToolButton(pFrame);
	pButton->setText(text);
	pButton->setFont(m_font);
	pButton->setAutoRaise(true);
	pButton->setAutoExclusive(true);
	pButton->setCheckable(true);
	pButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//	pButton->setMinimumHeight(m_nHeight-8);

	pLayout->addWidget(pButton);

	m_pGroup->addButton(pButton, id);
	m_listButtons.append(pFrame);
}

int QButtonList::clickedButton() const
{
	return m_pGroup->checkedId();
}

QVector<int> QButtonList::checkedButtons() const
{
	QVector<int> ids;

	QList<QAbstractButton*> buttons=m_pCheckGroup->buttons();
	foreach (QAbstractButton* pButton, buttons)
	{
		if (pButton != NULL && pButton->isChecked())
		{
			ids.append(m_pCheckGroup->id(pButton));
		}
	}

	return ids;
}

bool QButtonList::hasChecked() const
{
	QList<QAbstractButton*> buttons=m_pCheckGroup->buttons();
	foreach (QAbstractButton* pButton, buttons)
	{
		if (pButton != NULL && pButton->isChecked())
		{
			return true;
		}
	}

	return false;
}

void QButtonList::click(int id)
{
	QAbstractButton* pButton=m_pGroup->button(id);
	Q_ASSERT_NO_NULL(pButton);

	pButton->click();
}

void QButtonList::stateChanged(int id)
{
	QAbstractButton* pButton=m_pGroup->button(id);
	Q_ASSERT_NO_NULL(pButton);

	emit buttonChecked(id, !pButton->isChecked());
	click(id);
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
	foreach (QWidget* pButton, m_listButtons)
	{
		pButton->setGeometry(m_nBorder, y, size.width()-2*m_nBorder, m_nHeight);

		y+=m_nHeight+m_nSpacing;
	}
}
