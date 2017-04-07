#ifndef QMESSAGEBOXLOGOUT_H
#define QMESSAGEBOXLOGOUT_H

#include <QMessageBox>
#include <QPushButton>
#include <QTimer>

class QMessageBoxLogout : public QMessageBox
{
	Q_OBJECT

public:
	QMessageBoxLogout(const QPixmap& icon, QWidget* parent = 0);
	~QMessageBoxLogout();

public slots:
	void timeout();

private:
	QTimer	m_timer;

	int	m_nCount;
	QPushButton*	m_pButtonLogout;
};

#endif // QMESSAGEBOXLOGOUT_H
