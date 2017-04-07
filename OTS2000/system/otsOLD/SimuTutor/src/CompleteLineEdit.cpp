#include "CompleteLineEdit.h"
#include <QKeyEvent>
#include <QtGui/QListView>
#include <QtGui/QStringListModel>
#include <QDebug>

CompleteLineEdit::CompleteLineEdit(QStringList stringList, QWidget *parent)
: QLineEdit(parent)
, m_bEditFocus(true),m_stringList(stringList)
{

	m_stringListmodel = new QStringListModel(this);
	m_pFindWnd = new QListView(this);
	//m_pFindWnd->setWindowFlags(Qt::Popup);
	m_pFindWnd->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pFindWnd->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pFindWnd->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pFindWnd->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pFindWnd->setParent(0, Qt::Popup);
	m_pFindWnd->setFocusPolicy(Qt::NoFocus);
	m_pFindWnd->setFocusProxy(this);

	connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(textEditedSlot(const QString&)));
	QObject::connect(m_pFindWnd, SIGNAL(clicked(QModelIndex)),
		this, SLOT(clickedSlot(QModelIndex)));
	QObject::connect(this, SIGNAL(activated(QModelIndex)),
		m_pFindWnd, SLOT(hide()));

	this->installEventFilter(this);
	m_pFindWnd->installEventFilter(this);

}

CompleteLineEdit::~CompleteLineEdit()
{
	delete m_pFindWnd;
}

QStringList& CompleteLineEdit::stringList()
{
	return m_stringList;
}

void CompleteLineEdit::showFindWnd(const QString& text)
{
	//效率较低，需要优化
	QStringList sl;  
	foreach(QString word, m_stringList) {  
		if (word.contains(text)) {  
			sl << word; 
		}  
	}

	if (sl.size() == 0) 
	{
		hideFineWnd();
		return;
	}
	m_stringListmodel->setStringList(sl);  
	m_pFindWnd->setModel(m_stringListmodel);  

	//高度需要优化
	int num;
	sl.size()>7 ? num = 7 : num = sl.size();
	m_pFindWnd->resize(rect().width(), rect().height()*num);
	QPoint pTopleft = mapToGlobal(rect().bottomLeft());
	m_pFindWnd->move(pTopleft.x(), pTopleft.y());
	m_pFindWnd->show();
}

void CompleteLineEdit::textEditedSlot(const QString& text)
{
	QString strText = text.trimmed();
	if (!strText.isEmpty())
	{
		showFindWnd(strText);
	}
	else
	{
		hideFineWnd();
	}
}

void CompleteLineEdit::clickedSlot(QModelIndex modelIndex)
{
	setText(m_pFindWnd->model()->data(modelIndex).toString());
	hideFineWnd();
}

void CompleteLineEdit::hideFineWnd()
{
	m_pFindWnd->hide();
}

bool CompleteLineEdit::eventFilter(QObject *o, QEvent *e)
{
	if (m_bEditFocus && (o == this) && e->type() == QEvent::FocusOut) 
	{
		if (m_pFindWnd && m_pFindWnd->isVisible())
			return true;
	}

	if (o != m_pFindWnd)
		return __super::eventFilter(o, e);

	switch (e->type()) 
	{
	case QEvent::KeyPress: 
		{
			QKeyEvent *ke = static_cast<QKeyEvent *>(e);
			QModelIndex curIndex = m_pFindWnd->currentIndex();
			QModelIndexList selList = m_pFindWnd->selectionModel()->selectedIndexes();
			const int key = ke->key();

			if ((key == Qt::Key_Up || key == Qt::Key_Down) && selList.isEmpty() && curIndex.isValid() )
			{
				m_pFindWnd->setCurrentIndex(curIndex);
				return true;
			}

			switch (key) 
			{
			case Qt::Key_End:
			case Qt::Key_Home:
				if (ke->modifiers() & Qt::ControlModifier)
					return false;
				break;

			case Qt::Key_Up:
				if (!curIndex.isValid()) 
				{
					int rowCount = m_pFindWnd->model()->rowCount();
					QModelIndex lastIndex = m_pFindWnd->model()->index(rowCount - 1, m_pFindWnd->modelColumn());
					m_pFindWnd->setCurrentIndex(lastIndex);
					return true;
				} 
				else if (curIndex.row() == 0) 
				{
					return true;
				}
				return false;

			case Qt::Key_Down:
				if (!curIndex.isValid()) 
				{
					QModelIndex firstIndex = m_pFindWnd->model()->index(0, m_pFindWnd->modelColumn());
					m_pFindWnd->setCurrentIndex(firstIndex);
					return true;
				} 
				else if (curIndex.row() == m_pFindWnd->model()->rowCount() - 1) 
				{
					return true;
				}
				return false;
			}

			m_bEditFocus = false;
			this->event(ke);
			m_bEditFocus = true;
			if ( e->isAccepted() || !m_pFindWnd->isVisible()) {
				if (!this->hasFocus())
					hideFineWnd();
				if (e->isAccepted())
					return true;
			}

			switch (key) 
			{
			case Qt::Key_Return:
			case Qt::Key_Enter:
			case Qt::Key_Tab:
				hideFineWnd(); 
				if (curIndex.isValid()) 
				{  
					QString text = m_pFindWnd->currentIndex().data().toString();  
					setText(text);  
				}  
				break;

			case Qt::Key_F4:
				if (ke->modifiers() & Qt::AltModifier)
					hideFineWnd();
				break;

			case Qt::Key_Backtab:
			case Qt::Key_Escape:
				hideFineWnd();
				break;

			default:
				break;
			}

			return true;
		}
	case QEvent::MouseButtonPress: 
		if (!m_pFindWnd->underMouse()) 
		{
			hideFineWnd();
			return true;
		}
		return false;
	case QEvent::InputMethod:
	case QEvent::ShortcutOverride:
		QApplication::sendEvent(this, e);
		break;

	default:
		return false;
	}
	return false;
}
