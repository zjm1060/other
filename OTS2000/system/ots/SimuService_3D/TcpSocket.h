#ifndef TcpSocket_H
#define TcpSocket_H

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QThread>
#include <QtNetwork>
#include <QMetaType>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket> 
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QMetaType>
#include <QDebug>
#include <QMutex>
#include "/home/ems/system/ots/ots_hdb/inc/ots.h"

#define FINIT	1e-6
#define PORT	10001	

typedef enum{
	MSG_TYPE_INDCHG = 1,	//信号变位（单向数据，后台—>三维）
	MSG_TYPE_ANACHG,		//测量值变化（单向数据，后台—>三维）
	MSG_TYPE_DEVCTL,		//设备控制（单向数据，三维—>后台）
	MSG_TYPE_DEVMAP,		//设备导航（单向数据，后台—>三维）
	MSG_TYPE_DEVTAG,		//设备挂牌（双向数据）
	MSG_TYPE_DEVCHK,		//设备检查（单向数据，三维—>后台）
	MSG_TYPE_GRDWIR,		//挂接地线（单向数据，三维—>后台）
	MSG_TYPE_MSG,			//字符串消息（双向数据）//数据库中无关联点都用此类
	MSG_TYPE_SETEVT,		//设置异常（双向数据）
}MSG_TYPE;

typedef struct{
	MSG_TYPE	msg_type;					//传送数据类型
	char		logicname[POINT_NAME_SIZE];		//点逻辑名
	int			state;							//状态
	int			cmd_type;						//命令类型
	int			tag_type;						//挂牌类型
	float			_value;					
	char		opr_msg[POINT_LONGNAME_SIZE];			
}COMM_MSG;
Q_DECLARE_METATYPE(COMM_MSG);

typedef struct  _point_info
{
	POINTER	point;
	int		state;
	float		_value;
	float		step;
	int		tag_state;
}POINT_INFO;
Q_DECLARE_METATYPE(POINT_INFO);

int		BdCmdByNameStr(char *ptNameStr, unsigned char type_id, USHORT cmd_stat, float);

class Connection : public QObject
{
	Q_OBJECT  
public:  
	explicit Connection(int socketDescriptor, QObject *parent); 
	~Connection();
signals:  
	void error(QTcpSocket::SocketError socketError,const QString &errorString); 
	void recvMsgData(QByteArray byteArray);
	void deleteConnection(int socketDescriptor);
public slots: 
	void start();
	void sendMsgData(QByteArray byteArray);
	void recvMsgData();
	void disconnected();
private:
	QTcpSocket *m_pTcpSocket;
	QByteArray m_byteArray;
	int m_nSocketDescriptor;
};


class TcpServer : public QTcpServer
{  
	Q_OBJECT  
public:  
	explicit TcpServer(QObject *parent = 0);
	~TcpServer();
public:
	static TcpServer* instance();
	void ProcessSendMsgData(COMM_MSG *comm_msg);
private slots:  
	void ProcessRecvMsgData(QByteArray byteArray);
	void deleteConnection(int socketDescriptor);
signals:
	void sendMsgData(QByteArray byteArray);
protected:  
	void incomingConnection(int socketDescriptor);
private:
	QMap<int,QPair<QThread *,Connection *>> m_mapConnections;
}; 



#endif