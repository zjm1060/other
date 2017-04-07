#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QMainWindow>

#include "checkui.h"
#include "testui.h"
#include "checkWin.h"


extern "C" {
	
	CHECKUIAPP_EXPORT CheckUI* CHECKAppInterface;
	
	CHECKUIAPP_EXPORT void* CreateCHECKAppInterface()
	{
		CheckUI* instance = new CheckUI();
		
		return instance;
	}
	
	CHECKUIAPP_EXPORT void DeleteCHECKAppInterface(void* ptr)
	{
		CheckUI* instance = (CheckUI*)ptr;
		if( instance )	delete instance;
	}
	
} /* end of extern "C" */

CheckUI::CheckUI()
: UIAppInterface()
{




}

CheckUI::~CheckUI()
{
	

}
QWidget * CheckUI::createMainView(QMainWindow *parent)
{
	return 0;
	
	
}

 QToolBar* CheckUI::createAppToolBar( QMainWindow	* parent  )
{
	return NULL;
}
void CheckUI::createAppMenuBar( MdiMainWindow* parent )
{
	if( !parent ) return;
	m_parent = parent;

	m_parentMdiarea=m_parent->m_mdiArea;

	//check
	m_CheckEntrance = new QAction(QIcon(":/images/checkdiff.png"), tr("&CHECK  "), this);
	m_CheckEntrance->setShortcut(tr("Ctrl+O"));
	m_CheckEntrance->setStatusTip(tr("Check Point in DB"));
	connect(m_CheckEntrance, SIGNAL(triggered()), this, SLOT(slotCheckEntrance()));

	m_checkMenu=m_parent->menuBar()->addMenu(tr("Check &Diff"));

	m_checkMenu->addAction(m_CheckEntrance);

}
QToolBar* CheckUI::createAppToolBar( MdiMainWindow* parent  )
{
	if( !parent ) return NULL;
	m_parent = parent;
	
    m_appTools= new QToolBar(tr("CheckUI ToolBar"),parent);
    parent->addToolBar(m_appTools);
	m_appTools->addAction(m_CheckEntrance);
		
	return m_appTools;
}


QToolBar* CheckUI::createAppToolBar2( QMainWindow* parent )
{
	return NULL;
}

void CheckUI::slotCheckEntrance()
{

	//Ctestwdg* graphicsview=new Ctestwdg();
	//m_parentMdiarea->addSubWindow(graphicsview);
	//graphicsview->showMaximized();

	CcheckWin * temp=new CcheckWin(m_parent);
	m_parentMdiarea->addSubWindow(temp);
	temp->showMaximized();

	m_CheckEntrance->setEnabled(false);

}
void CheckUI::mouseDownOnGraph( Qt::MouseButtons nButton, Qt::KeyboardModifiers nShiftState, int x, int y )
{


}

void CheckUI::mouseUpOnGraph( Qt::MouseButtons nButton, Qt::KeyboardModifiers nShiftState, int x, int y )
{
}

void CheckUI::keyDownOnGraph( int& nChar, Qt::KeyboardModifiers nShiftState )
{


}
void CheckUI::mouseDoubleClickedOnGraph(Qt::MouseButtons nButton, Qt::KeyboardModifiers nShiftState, int x, int y )
{


}

void CheckUI::exeAfterLoaded()
{	

}


void CheckUI::installGraphInstance( void* graphObj )
{

}
