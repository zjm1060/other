#include "qdlgcreategroup.h"

#include "common.h"

QDlgCreateGroup::QDlgCreateGroup(const QPixmap& icon, QWidget *parent/* =0 */)
:QDialog(parent)
{
	m_ui=new Ui::DialogCreateGroup;
	m_ui->setupUi(this);

	m_ui->labelIcon->setPixmap(icon);

	m_ui->labelInfoIcon->setPixmap(findPixmap(":/images/error.png"));
	m_ui->labelInfoIcon->hide();

	connect(m_ui->lineEditGroup, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
}

QDlgCreateGroup::~QDlgCreateGroup()
{

}

const QString QDlgCreateGroup::group() const
{
	return m_ui->lineEditGroup->text();
}

void QDlgCreateGroup::textChanged(const QString& text)
{
	Q_UNUSED(text);

	m_ui->pushButtonOk->setEnabled(!text.isEmpty());
}
