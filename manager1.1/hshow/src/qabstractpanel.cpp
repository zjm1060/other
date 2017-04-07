#include "qabstractpanel.h"

#include <QHBoxLayout>
#include <QAbstractButton>
#include <QToolButton>
#include <QEvent>
#include <QDesktopWidget>
#include <QApplication>

#include "define.h"
#include "qfloatingdockwidget.h"

QAbstractPanel::QAbstractPanel(QWidget* parent/* =0 */)
:QWidget(parent)
{
	m_pLayout=new QHBoxLayout;
	m_pLayout->setMargin(0);
	setLayout(m_pLayout);
}

QAbstractPanel::~QAbstractPanel()
{

}

void QAbstractPanel::addToolButton(QToolButton* button)
{
	Q_ASSERT_NO_NULL(button);
	Q_ASSERT_NO_NULL(m_pLayout);

	button->setAutoRaise(true);
	button->setToolButtonStyle(Qt::ToolButtonIconOnly);

	setDefaultIconSize(button);

	m_pLayout->addWidget(button);
}

void QAbstractPanel::setDefaultIconSize(QAbstractButton* button)
{
	Q_ASSERT_NO_NULL(button);

	button->setIconSize(QSize(32, 32));
}

void QAbstractPanel::showWidget(QObject* watched)
{
	Q_UNUSED(watched);
}

void QAbstractPanel::hideWidget(QObject* watched)
{
	Q_UNUSED(watched);
}

bool QAbstractPanel::eventFilter(QObject* watched, QEvent* event)
{
	switch (event->type())
	{
	case QEvent::Enter:
		{
			showWidget(watched);
		}
		break;
	case QEvent::Leave:
		{
			hideWidget(watched);
		}
		break;
	default:
		break;
	}

	return QWidget::eventFilter(watched, event);
}

QFloatingDockWidget* QAbstractPanel::createDockWidget(const QString& title, int width, int height)
{
	QFloatingDockWidget* pDockWidget=new QFloatingDockWidget;
	pDockWidget->setWindowTitle(title);
	pDockWidget->setContentsMargins(0, 0, 0, 0);
	pDockWidget->setMinimumSize(width,height);
	pDockWidget->setGeometry(0, 0, width, height);

	QPoint pt=QCursor::pos();
	QDesktopWidget* pDesktop=QApplication::desktop();
	QRect rect;
	if (pDesktop != NULL)
	{
		rect=pDesktop->availableGeometry(pt);
	}

	QRect r=pDockWidget->frameGeometry();

	int x=pt.x()-r.width()/2;
	if (x < rect.left())
	{
		x=rect.left();
	}

	if (x+r.width() > rect.right())
	{
		x=rect.right()-r.width();
	}

	int y=rect.top()+42;

	pDockWidget->move(x, y);

	return pDockWidget;
}
