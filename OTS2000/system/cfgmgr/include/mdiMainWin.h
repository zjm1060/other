/****************************************************************************
**

****************************************************************************/

#ifndef MDIMAINWINDOW_H
#define MDIMAINWINDOW_H

#include <QMainWindow>

class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
QT_END_NAMESPACE

class MdiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MdiMainWindow();

protected:
 //   void closeEvent(QCloseEvent *event);

private slots:
    
  //  MdiChild *createMdiChild();
  //  void setActiveSubWindow(QWidget *window);

public:
  //  MdiChild *activeMdiChild();
   // QMdiSubWindow *findMdiChild(const QString &fileName);

 //   QMdiArea *mdiArea;
 //   QSignalMapper *windowMapper;

	QMdiArea *m_mdiArea;
	QSignalMapper *m_windowMapper;

};

#endif
