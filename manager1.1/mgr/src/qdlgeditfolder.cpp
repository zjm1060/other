#include "qdlgeditfolder.h"

#include <QPixmap>
#include <QStringList>
#include <QDataStream>
#include <QDir>

#include "common.h"

QDlgEditFolder::QDlgEditFolder(const QPixmap& icon, const QString& folder, QWidget *parent/* =0 */)
	: QDialog(parent)
{
	setupUi(this);

	labelIcon->setPixmap(icon);

	labelInfoIcon->setPixmap(findPixmap(":/images/error.png"));
	labelInfoIcon->hide();

	lineEditFolder->setText(folder);

	connect(lineEditFolder, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));

 	lineEditFolder->installEventFilter(this);
}

QDlgEditFolder::~QDlgEditFolder()
{
}

QString QDlgEditFolder::text()
{
	return lineEditFolder->text();
}

bool QDlgEditFolder::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == lineEditFolder)
	{
		if (event->type() == QEvent::FocusIn)
		{
			textChanged(lineEditFolder->text());

			pushButtonOk->setDefault(true);
		}
	}
	
	return QDialog::eventFilter(obj, event);
}

void QDlgEditFolder::textChanged(const QString& text)
{
	QString strText=text.trimmed();
	if (!strText.isEmpty())
	{
		QChar ch;
		if (hasInvalidChar(strText, &ch))
		{
			labelInfoIcon->show();
			labelInfo->setText(tr("%1 is an invalid character in your target folder.").arg(ch));
			pushButtonOk->setEnabled(false);

			return;
		}
	}
	
	labelInfoIcon->hide();
	labelInfo->setText(tr("Enter your target folder."));
	pushButtonOk->setEnabled(true);
}

