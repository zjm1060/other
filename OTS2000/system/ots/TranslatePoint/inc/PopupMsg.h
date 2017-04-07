#ifndef _POPUPMSG_H
#define _POPUPMSG_H
#include <QtGui>
#include <QApplication>

class CPopupMsg : public QWidget
{
	Q_OBJECT
public:
	CPopupMsg(QWidget *parent=0);
	~CPopupMsg();
	void setSelectedText(const QString &selectedText);
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void paintEvent(QPaintEvent *event);
	void leaveEvent(QEvent *event);
	void enterEvent(QEvent *event);
private:
	QLabel *label1_text,*label2_text,*label3_text,*label4_text;
	QString m_selectedText;
	QPoint dragPosition;
	QTimer *refresh,*autoHide;
public slots:
	void updateMsg();
};

#endif