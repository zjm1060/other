#ifndef UIAPPINTERFACE_H
#define UIAPPINTERFACE_H
#include <QWidget>
#include "mdiMainWin.h"
#ifdef WIN32

#define SHIFT_MASK      0x01
#define CTRL_MASK       0x02
#define ALT_MASK        0x04
// Button values for mouse events
#define LEFT_BUTTON     0x01
#define RIGHT_BUTTON    0x02
#define MIDDLE_BUTTON   0x04

#  ifdef UIAPP_DLL
#  define UIAPP_EXPORT __declspec(dllexport)
#  else
#  define UIAPP_EXPORT __declspec(dllimport)
#  endif

#else

// Shift state values for mouse and keyboard events
#define SHIFT_MASK      0x01
#define CTRL_MASK       0x02
#define ALT_MASK        0x04
// Button values for mouse events
#define LEFT_BUTTON     0x01
#define RIGHT_BUTTON    0x02
#define MIDDLE_BUTTON   0x04

#define UIAPP_EXPORT 
#endif

typedef void* (*CreateAppInstanceProc)();
typedef void (*DeleteAppInstanceProc)( void* );

class QToolBar;
class QMainWindow;


class UIAPP_EXPORT UIAppInterface : public QWidget
{
    Q_OBJECT
public:
	UIAppInterface ();
	~ UIAppInterface ();
    virtual QWidget * createMainView(QMainWindow *parent)=0;
//	virtual void createAppMenuBar( QMainWindow* parent )=0;
//	virtual void createAppMenuBar( QMainWindow* parent ,QMdiArea* parentmdiarea)=0;
	virtual QToolBar* createAppToolBar( QMainWindow	* parent  )=0;
	virtual QToolBar* createAppToolBar2( QMainWindow	* parent )=0;

	//virtual QWidget * createMainView(MdiMainWindow *parent)=0;
	virtual void createAppMenuBar( MdiMainWindow* parent )=0;
	virtual QToolBar* createAppToolBar( MdiMainWindow	* parent  )=0;
	//virtual QToolBar* createAppToolBar2( MdiMainWindow	* parent )=0;
	virtual void exeAfterLoaded() = 0;
//	virtual void setDataStreamPara(RdbMixFieldStream *prdbmixfieldstream,int datawino)=0;
    virtual void installGraphInstance( void* ) = 0;
public slots:
	virtual void mouseDownOnGraph( Qt::MouseButtons  nButton, Qt::KeyboardModifiers nShiftState, int x, int y ) = 0;
	virtual void mouseUpOnGraph( Qt::MouseButtons  nButton, Qt::KeyboardModifiers nShiftState, int x, int y ) = 0;
	virtual void keyDownOnGraph( int& nChar, Qt::KeyboardModifiers nShiftState ) = 0;
	virtual void mouseDoubleClickedOnGraph(Qt::MouseButtons  nButton, Qt::KeyboardModifiers nShiftState, int x, int y )=0;
//    virtual void installPasswd(CPasswordClass *passwd)=0;
//	virtual void installRegman(char *pregman,char *pregmanpasswd)=0;
	
signals:
	void appStatusChanged(int type, void* userData);
};


#endif
