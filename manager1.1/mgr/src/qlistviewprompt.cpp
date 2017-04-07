#include "qlistviewprompt.h"

QListViewPrompt::QListViewPrompt(QWidget *parent)
	: QListView(parent)
{
	m_pDelegate=new QTipsDelegate(this);
	setItemDelegate(m_pDelegate);

	m_pModelTips=new QStandardItemModel(0, 1, this);
	setModel(m_pModelTips);
}

QListViewPrompt::~QListViewPrompt()
{
	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModelTips);
}

void QListViewPrompt::clear()
{
	if (m_pModelTips != NULL)
	{
		m_pModelTips->setRowCount(0);
	}
}

void QListViewPrompt::appendTip(int flag, const QString& tip)
{
	if (m_pModelTips == NULL)
	{
		return;
	}

	int nRow=m_pModelTips->rowCount();
	m_pModelTips->insertRow(nRow);

	m_pModelTips->itemFromIndex(m_pModelTips->index(nRow, 0))->setEditable(false);
	m_pModelTips->setData(m_pModelTips->index(nRow, 0), flag, Qt::UserRole);
	m_pModelTips->setData(m_pModelTips->index(nRow, 0), tip);

	QItemSelectionModel* pModel=selectionModel();
	if (pModel != NULL && !pModel->hasSelection())
	{
		scrollTo(m_pModelTips->index(nRow, 0));
	}
}
