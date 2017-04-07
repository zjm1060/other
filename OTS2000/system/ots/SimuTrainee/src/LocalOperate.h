#ifndef _LOCALOPRATE_H
#define _LOCALOPRATE_H
#include <QtGui>
#include "Login.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
#include "/home/ems/system/ots/SimuTutor/src/CompleteLineEdit.h"
#define squareSize	64
#define prevSquareSize	256
#define PIXMAP_SIZE	64
extern QHash<QString,QString> mapOptImage;

class COptPrevWidget : public QWidget
{
	Q_OBJECT
public:
	COptPrevWidget(QWidget *parent = 0);
	void setPrevPixmap(const QPixmap &pixmap);
	void setPrevMovie(QMovie *movie);
	void setPixmapScale(int width,int heigth,Qt::AspectRatioMode mode);
	void setPixmapScale(const QSize &size,Qt::AspectRatioMode mode);
private:
	QMovie *prevMovie;
	QPixmap prevPixmap;
	QLabel *label;
	QSize pixmapSize;
};
class COptSelectWidget : public QWidget
{
	Q_OBJECT
public:
	COptSelectWidget(QWidget *parent = 0);
	QSize sizeHint() const;
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void leaveEvent(QEvent *event);
private:
	QStringList listOpt;
	int oldKey,curKey;
	COptPrevWidget *optPrevWidget;
	QPixmap pixmap_no;
	QPixmap pixmap_show;

public slots:
	void addOpt(int,const QString &name);
	void delOpt(const QString &name);
	QStringList getAllOpt() const;
};
class COptImageWidget : public QWidget
{
	Q_OBJECT
public:
	COptImageWidget(QWidget *parent = 0);
	QSize sizeHint() const;
	void setOptList(QStringList m_listOpt);
signals:
	void optSelected(const QString &optName);
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void hideEvent(QHideEvent *event);
	void leaveEvent(QEvent *event);
private:
	COptPrevWidget *optPrevWidget;
	QStringList listOpt;
	int scrollBarWidth;
	int columns;
	int lastKey;
	int oldKey,curKey;
	QPixmap pixmap_no;
	QPixmap pixmap_show;
};

class COptWidget : public QWidget
{
	Q_OBJECT
public:
	COptWidget(QWidget *parent = 0);
	void setOptType(int opt_type);
	void setOptId(int opt_id);
	void setPixmap(const QPixmap &pixmap);
	void setText(const QString &text);
	void setPixmapSize(int width,int heigth,Qt::AspectRatioMode mode);
	void setPixmapSize(const QSize &size,Qt::AspectRatioMode mode);
	int getOptId();
	int getOptType();
	QPixmap getPixmap() const;
	QString getText() const;
	QSize getSize() const;
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
signals:
	void optSelected(int opt_type,int opt_id,const QString &opt_name);
private:
	int m_opt_type;
	int m_opt_id;
	QPixmap m_pixmap;
	QLabel *m_labelText,*m_labelPixmap;
	QSize m_pixmapSize;

};

class CLocalOprate : public QWidget
{
	Q_OBJECT
public:
	CLocalOprate(QWidget *parent = 0);
	~CLocalOprate();
	QTabWidget *tabWidget;
	QLabel *lableReport;//dzy
	QTextEdit *editReport;
private:
	void setupUi();
	void initMapOptImage();

	QWidget *prevWidget;
	QPixmap prevPixmap;
	QLabel *labelPixmap;
	QLabel *labelName;
	QToolButton *buttonEnter;
	CompleteLineEdit *editNameFilter;
	QToolButton *buttonFilter;
	QPushButton *buttonExit;
	QCompleter *completer;
	QStringList LocalOperateList;
	QSpacerItem *spacer1, *spacer2, *spacer3;
	QHash<QString,QPair<short,int>> LocalOperateHash;
	int m_opt_type;
	int m_opt_id;
	QString m_opt_name;
	QSqlDatabase  dbopr;//dzy added for mutithread to mysql 140707 
protected:
	void paintEvent(QPaintEvent *event);
private slots:
	void optPreview(int opt_type,int opt_id,const QString &opt_name);
	void optRun();
	void optFilter();
	void onclose();
	void on_currentChanged ( int index ) ;
signals:
	void closedlg();
};

#endif