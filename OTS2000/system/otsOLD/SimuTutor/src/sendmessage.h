#ifndef _SENDMESSAGE_H_
#define _SENDMESSAGE_H_

#include <QUdpSocket>

#include "ui_sendmessage.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "qcommon.h"
#include "thread.h"
#define MAX_MSG_SIZE	10240

class CharacterWidget : public QWidget
{
	Q_OBJECT
public:
	CharacterWidget(QWidget *parent = 0);
	QSize sizeHint() const;
signals:
	void characterSelected(const QString &character);
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void hideEvent(QHideEvent *event);
	void leaveEvent(QEvent *event);
private:
	QMap<int,QString> mapImage;
	QFont displayFont;
	int scrollBarWidth;
	int columns;
	int lastKey;
	int faceSquareSize;
	QWidget *facePrevWidget;
	QMovie *facePrevMovie;
};
class CFaceImageWidget : public QWidget
{
	Q_OBJECT
public:
	CFaceImageWidget(QWidget *parent=0);
protected:
	void hideEvent(QHideEvent *event);
	void paintEvent(QPaintEvent *event);
signals:
	void sigToggled(bool);
private:
	CharacterWidget *characterWidget;
	QScrollArea *scrollArea;
};

class CSendMessageTable : public QDialog,public Ui_CSendMessageTable
{
	Q_OBJECT
	public:
		CSendMessageTable(QWidget *parent=0);
		void setControlAtt();
		void loadChinese();
		void loadEnglish();
		void setUserId(int);
	protected:
		bool eventFilter(QObject *, QEvent *);
		void showEvent(QShowEvent *);
		void closeEvent(QCloseEvent *);
		void hideEvent(QHideEvent *);
		void keyPressEvent(QKeyEvent *);
	Q_SIGNALS:
		void sigRecvMsg();
	private:
		QAction *actionRefresh;
		QAction *actionSend1,*actionSend2,*actionSend3;
		QActionGroup *actionGroup;
		QMenu *menuButtonSend;
		bool flagUnReadMsg;
		bool isAllUserShow;
		QTimer *refresh;
		QUdpSocket *udpSendSocket,*udpRecvSocket;
		int cur_user_id;
		QStandardItemModel *allUserModel,*sendUserModel;
		QSortFilterProxyModel *allUserSortModel;
		QItemSelectionModel *selectionModelAllUser,*selectionModelSendUser;
		QItemSelection selectionRange;
		QFont cur_send_font;
		QColor cur_send_color;
		UserLoginStateThread *userLoginStateThread;
		CFaceImageWidget *faceImageWidget;
		QPoint getPos(QWidget* widget);
		void udpSend(int user_id,OTS_MSG *msg);
		void processTheDatagram(QByteArray);
		void shakeDialog();
		void setInsertTextColor(const QColor &col,QTextEdit *textEdit);
		void setInsertTextFont(const QFont &font,QTextEdit *textEdit);
	private slots:
		void changeAllUserShow();
		void changeMsgRecShow(bool);
		void sendMessage();
		void sendShake();
		void appendMsgRec(OTS_MSG *msg);
		void appendMsgShow(OTS_MSG *msg);
		void displayAllUser();
		void addToSendUser();
		void addToSendUser(const QModelIndex &);
		void addAllToSendUser();
		void delToSendUser();
		void delToSendUser(const QModelIndex &);
		void delAllToSendUser();
		void readPendingDatagrams();
		void insertImage();
		void setColor();
		void setFont();
		void userStateChanged(int user_id,short state);
		void openFaceImageWidget(bool);
};
#endif