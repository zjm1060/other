#include "qframeinspect.h"

#include <QDir>
#include <QMessageBox>

#include "common.h"
#include "qservermanager.h"
#include "qimessage.h"
#include "qglobalinfo.h"

QFrameInspect::QFrameInspect(QWidget *parent)
	: QFrame(parent)
	,m_bPause(false)
{
	setupUi(this);

	m_pLinkBtnReinspect=new QLinkButton(tr("Reinspect"), this);
	horizontalLayoutReinspect->addWidget(m_pLinkBtnReinspect);

	progressBar->setRange(0, 10000);

	connect(pushButtonPause, SIGNAL(clicked()), this, SLOT(onClickedBtnPause()));
	connect(pushButtonCancel, SIGNAL(clicked()), this, SLOT(onClickedBtnCancel()));
	connect(pushButtonBack, SIGNAL(clicked()), this, SLOT(onClichedBtnBack()));
	connect(m_pLinkBtnReinspect, SIGNAL(clicked()),this, SLOT(onClickedBtnReinspect()));

	m_pTreeViewInspect=new QTreeViewInspect(this);
	horizontalLayoutInspect->addWidget(m_pTreeViewInspect);

	connect(m_pTreeViewInspect, SIGNAL(sendMessage(QString)), this, SLOT(message(QString)));
	connect(m_pTreeViewInspect, SIGNAL(finished(int, int, int)), this, SLOT(finished(int, int, int)));
	connect(m_pTreeViewInspect, SIGNAL(progress(int, int, int)), this, SLOT(setProgress(int, int, int)));
	connect(m_pTreeViewInspect, SIGNAL(addItemsFinished()), this, SLOT(addInspectItemsFinished()));
}

QFrameInspect::~QFrameInspect()
{
	
}

void QFrameInspect::message(QString tip)
{
	labelInfo->setText(tip);
}

void QFrameInspect::setProgress(int value, int min, int max)
{
	Q_ASSERT(min<max);
	progressBar->setValue(value*10000/(max-min));
}

void QFrameInspect::finished(int normal, int skip, int total)
{
	progressBar->hide();
	pushButtonPause->hide();
	pushButtonCancel->hide();
	labelTitle->show();
	labelIcon->show();
	m_pLinkBtnReinspect->show();

	if (normal == total)
	{
		labelIcon->setPixmap(findPixmap(":/images/tick1.png"));

		QPalette palette=labelTitle->palette();
		palette.setColor(QPalette::WindowText, QColor(0, 128, 0));
		labelTitle->setPalette(palette);

		labelTitle->setText(tr("Inspection finished and no problem found"));
	}
	else
	{
		labelIcon->setPixmap(findPixmap(":/images/alert.png"));

		QPalette palette=labelTitle->palette();
		palette.setColor(QPalette::WindowText, QColor(249, 158, 0));
		labelTitle->setPalette(palette);

		labelTitle->setText(tr("Inspection finished, and %1 problems found!").arg(total-normal-skip));
	}

	labelInfo->setText(tr("%1 detected, %2 problems, %3 unknown").arg(total).arg(total-normal-skip).arg(skip));

	pushButtonBack->show();
}

void QFrameInspect::onClickedBtnPause()
{
	m_bPause=!m_bPause;
	pushButtonPause->setText(m_bPause? tr("Continue"):tr("Pause"));

	m_pTreeViewInspect->pause(m_bPause);
}

void QFrameInspect::onClickedBtnCancel()
{
	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
		tr("Are you sure to cancel inspection?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

	if (button == QMessageBox::Yes)
	{
		m_pTreeViewInspect->cancel();

		emit goBack();
	}
}

void QFrameInspect::onClichedBtnBack()
{
	emit goBack();
}

void QFrameInspect::onClickedBtnReinspect()
{
	if (m_pLinkBtnReinspect != NULL)
	{
		inspect(true);
	}
}

void QFrameInspect::setInspectItems(const ServerInfo& info, const QTreeViewInspectSetting::InspectItem& items)
{
	if (m_pTreeViewInspect == NULL)
	{
		return;
	}

	m_pTreeViewInspect->setInspectItems(info, items);

	setInspectLayout();
	pushButtonPause->setEnabled(false);
	pushButtonCancel->setEnabled(false);
}

void QFrameInspect::setInspectLayout()
{
	labelIcon->hide();
	labelTitle->hide();

	progressBar->show();

	pushButtonPause->setText(tr("Pause"));
	pushButtonPause->show();

	pushButtonCancel->show();
	pushButtonBack->hide();
	m_pLinkBtnReinspect->hide();
}

void QFrameInspect::inspect(bool init)
{
	setInspectLayout();

	m_pTreeViewInspect->inspect(init);
}

void QFrameInspect::addInspectItemsFinished()
{
	pushButtonPause->setEnabled(true);
	pushButtonCancel->setEnabled(true);

	m_pTreeViewInspect->inspect();
}
