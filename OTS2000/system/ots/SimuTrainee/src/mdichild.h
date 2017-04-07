#ifndef MDICHILD_H
#define MDICHILD_H
#include <QtGui>
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"

class MdiChild : public QMdiSubWindow
{
	Q_OBJECT
public:
	MdiChild();
	int flag_style;
	virtual ~MdiChild();
	QString currentFile() { return m_sCurFile; }
	QString currentDataFile() {return m_sCurDataFile;}
	QString currentFileBaseName();
	//void setonStyleSheet();
	//void mouseDoubleClickEvent ( QMouseEvent * event ) ;
	// virtual void mouseDoubleClickEvent(QMouseEvent *);//重载鼠标双击事件函数 从QWidget继承 
	bool closeable;
public slots:

protected:
	void closeEvent(QCloseEvent *event);
	void mouseDoubleClickEvent ( QMouseEvent * event ) ; 
private:
	//QString strippedName(const QString &fullFileName);
	//void setCurrentFile(const QString &fileName);
// 	void readSequenceText();
// 	S_KEY_WORD keyWord(const QString &str);
// 	S_SEQ_TYPE seqType(const QString &str);
// 	S_EQUA_TYPE equaType(const QString &str);
//	int pointValue(const QString &str);
private slots:
	void switchWindow();
	void setMenu();
//	void on_btndoubleclicked();
private:
	bool isUntitled;
	QString m_sCurFile;
	QString m_sCurDataFile;
	QStackedWidget *stackedWidget;
	QMenu *menu;
};
class MdiArea : public QMdiArea
{
	Q_OBJECT
public:
	MdiArea();
	virtual ~MdiArea();	
	//void mouseDoubleClickEvent ( QMouseEvent * event ) ;
	void mouseDoubleClickEvent(QMouseEvent *);//重载鼠标双击事件函数 从QWidget继承 
public slots:

private:

private slots:


};
#endif