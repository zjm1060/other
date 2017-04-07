#ifndef QMIBSRC_H
#define QMIBSRC_H

#include <QObject>

#include "qinterprocessclient.h"

class QMibSrc : public QObject
{
	Q_OBJECT

public:
	QMibSrc(QObject *parent=0);
	~QMibSrc();

public:
	void startMib();

private slots:
	void collected(int type);

	void connected();
	void clientQuit();
	
private:
	void init();

private:
	QInterProcessClient*		m_pClient;
};

#endif // QMIBSRC_H
