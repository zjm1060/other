#include "TcpSocket.h"

Connection::Connection(int socketDescriptor, QObject *parent)
	: QObject(parent),m_nSocketDescriptor(socketDescriptor)  
{  
	
}
Connection::~Connection()
{
	
}
void Connection::start()
{
	m_pTcpSocket = new QTcpSocket;  
	if (!m_pTcpSocket->setSocketDescriptor(m_nSocketDescriptor)) 
	{  
		emit error(m_pTcpSocket->error(),m_pTcpSocket->errorString()); 
		return;  
	}  
	qDebug() << QString("a new client has connected ! socket = %1").arg(m_nSocketDescriptor);
	connect(m_pTcpSocket, SIGNAL(readyRead()),this, SLOT(recvMsgData()));
	connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
}
void Connection::recvMsgData()  
{  
	while (m_pTcpSocket->bytesAvailable() < sizeof(COMM_MSG)) 
	{  
		if (!m_pTcpSocket->waitForReadyRead()) 
		{  
			emit error(m_pTcpSocket->error(),m_pTcpSocket->errorString());
			return;  
		}  
	}
	m_byteArray.clear();
	m_byteArray = m_pTcpSocket->read(sizeof(COMM_MSG));
	if (m_byteArray.size() == sizeof(COMM_MSG))
	{
		emit recvMsgData(m_byteArray);
	}
} 
void Connection::sendMsgData(QByteArray byteArray)  
{  
	m_pTcpSocket->write(byteArray);
	if(!m_pTcpSocket->waitForBytesWritten())
		emit error(m_pTcpSocket->error(),m_pTcpSocket->errorString());
} 
void Connection::disconnected()
{  
	if (m_pTcpSocket)
	{
		m_pTcpSocket->abort();
		m_pTcpSocket->deleteLater();
		emit deleteConnection(m_nSocketDescriptor);
		qDebug() << QString("a client has disconnected ! socket = %1").arg(m_nSocketDescriptor);
	}
} 

/*********************************************************************************************************************/
TcpServer::TcpServer(QObject *parent)  
	: QTcpServer(parent)
{  
	m_mapConnections.clear();
}  
TcpServer::~TcpServer()
{
	QMapIterator<int,QPair<QThread *,Connection *>> it(m_mapConnections);
	while (it.hasNext())
	{
		it.next();
		QThread *pThread = it.value().first;
		Connection *pConnection = it.value().second;
		if (pThread && pConnection)
		{
			pConnection->deleteLater();
			pThread->exit();
			pThread->wait();
		}
	}
	m_mapConnections.clear();
}
TcpServer* TcpServer::instance()
{
	static TcpServer* pInstance=new TcpServer;
	return pInstance;
}
void TcpServer::incomingConnection(int socketDescriptor)  
{  
	QThread *pThread = new QThread(this); 
	Connection *pConnection = new Connection(socketDescriptor,NULL); 
	m_mapConnections.insert(socketDescriptor,qMakePair(pThread,pConnection));
	pThread->start();  
	pConnection->moveToThread(pThread);
	connect(pThread, SIGNAL(started()), pConnection, SLOT(start())); 
	connect(pConnection,SIGNAL(deleteConnection(int)),this, SLOT(deleteConnection(int))); 
	connect(this,SIGNAL(sendMsgData(QByteArray)),pConnection,SLOT(sendMsgData(QByteArray)));
	connect(pConnection,SIGNAL(recvMsgData(QByteArray)),this,SLOT(ProcessRecvMsgData(QByteArray)));
}  
void TcpServer::ProcessSendMsgData(COMM_MSG *comm_msg)
{
	QByteArray byteArray((char *)comm_msg,sizeof(COMM_MSG));
	emit sendMsgData(byteArray);
}
void TcpServer::ProcessRecvMsgData(QByteArray byteArray)
{
	COMM_MSG *comm_msg = new COMM_MSG;
	memcpy(comm_msg,byteArray.data(),byteArray.size());
	DMS_COMMON	dbentry;
	char msg[SMS_MESSAGE_SIZE];
	switch(comm_msg->msg_type)
	{	
	case MSG_TYPE_DEVTAG:
	case MSG_TYPE_DEVCTL:
		{
			int state = comm_msg->state;

			if(BdCmdByNameStr(comm_msg->logicname, comm_msg->cmd_type, comm_msg->state,comm_msg->_value) != TRUE)
			{
				qDebug()<<"Err BdCmdByNameStr STATE "<<comm_msg->logicname;
			}
		}
		break;		

	case MSG_TYPE_DEVCHK:
		{
			if( -1==ReadEntryByNameStr(comm_msg->logicname,&dbentry) )
			{
				qDebug()<<"Err ReadEntryByNameStr STATE "<<comm_msg->logicname;
			}
			sprintf(msg,"check::%d.%d.%d.%d",dbentry.point.stn_id,dbentry.point.dev_id,dbentry.point.data_type,dbentry.point.pt_id);
			if(strlen(msg)>SMS_MESSAGE_SIZE)
				msg[SMS_MESSAGE_SIZE-1]='\0';
			BdSms(msg);
		}
		break;

	case MSG_TYPE_GRDWIR:
		{
			if( -1==ReadEntryByNameStr(comm_msg->logicname,&dbentry) )
			{
				qDebug()<<"Err ReadEntryByNameStr STATE "<<comm_msg->logicname;
			}
			sprintf(msg,"gdwire::%d.%d.%d.%d",dbentry.point.stn_id,dbentry.point.dev_id,dbentry.point.data_type,dbentry.point.pt_id);
			if(strlen(msg)>SMS_MESSAGE_SIZE)
				msg[SMS_MESSAGE_SIZE-1]='\0';
			BdSms(msg);
		}
	case MSG_TYPE_SETEVT:
		ProcessSendMsgData(comm_msg);
		break;
	default:
		break;
	}
}
void TcpServer::deleteConnection(int socketDescriptor)
{
	QThread *pThread = m_mapConnections[socketDescriptor].first;
	Connection *pConnection = m_mapConnections[socketDescriptor].second;
	if (pThread && pConnection)
	{
		m_mapConnections.remove(socketDescriptor);
		pConnection->deleteLater();
		pThread->exit(0);
		pThread->wait();
	}
}

int BdCmdByNameStr(char *ptNameStr, unsigned char type_id, USHORT cmd_stat, float fvalue)
//int BdCmdByNameStr(char *ptNameStr, unsigned char type_id, USHORT cmd_stat)
{
	DMS_COMMON	dms_comm;
	LAN_HEAD	lanhead;
	COMMAND_MSG	cmd_buf;
	struct	tm	tmpt;
	time_t		tim;
	char			pt[100];
	char			buf[10];

	if(ReadEntryByNameStr(ptNameStr, &dms_comm) ==-1) return FALSE;

	cmd_buf.type_id = type_id;
	cmd_buf.stn_id = dms_comm.point.stn_id;
	cmd_buf.dev_id = dms_comm.point.dev_id;
	cmd_buf.data_type = dms_comm.point.data_type;
	cmd_buf.point_id = dms_comm.point.pt_id;

	if(type_id== DPS_IOST_SET)
	{	cmd_buf.status.opr=IOST_SET_TAG_STATE;
	cmd_buf.status.state= cmd_stat;
	}
	else
	{	
		cmd_buf.status.opr=cmd_stat;
		if (type_id==C_GATE &&cmd_stat==15)  /*闸门、阀开度控制命令*/
			cmd_buf.status.state=0;
		else
			cmd_buf.status.state=1;
		strcpy(pt,ptNameStr);
		strcat(pt,"_3D_");
		sprintf(buf,"%d",cmd_buf.status.opr);
		strcat(pt,buf);
		if(InterLock_Judge(pt)!=1)
		{
			printf("闭锁条件不满足\n");
			return FALSE;
		}
/*		printf("控制命令%s下达\n",pt);*/
	}

	cmd_buf.status.result=0;
	cmd_buf.status.src=4;/**命令来源，如"三维系统"*/
	if (type_id==C_GATE &&cmd_stat==15)  /*闸门、阀开度控制命令*/
		cmd_buf.data.iValue=(int)(fvalue*1000);
	else
		cmd_buf.data.iValue=0;
	if(type_id!= DPS_IOST_SET)
	{
		if (type_id==C_GATE &&cmd_stat==15)
			printf("调节命令%s下达:%f\n",ptNameStr,fvalue);
		/*else if(type_id!= C_SYS_MODE_SET)
			printf("控制命令%s下达:%d\n",ptNameStr,cmd_stat);*/
		else
			printf("操作命令%s下达:%d\n",ptNameStr,cmd_stat);
	}
	else
		printf("摘牌/挂牌%s下达:%d\n",ptNameStr,cmd_stat);
	cmd_buf.ctrl_addr=dms_comm.fixed.ctrl_addr;
	GetLocalHostId(&cmd_buf.host_id);
	cmd_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	cmd_buf.second=tmpt.tm_sec;
	cmd_buf.minute=tmpt.tm_min;
	cmd_buf.hour=tmpt.tm_hour;
	cmd_buf.day=tmpt.tm_mday;
	cmd_buf.month=tmpt.tm_mon+1;
	cmd_buf.year=tmpt.tm_year+1900;
	strcpy(cmd_buf.usr_login_name,"3D");
	GetSrcId(&cmd_buf.host_id);
	lanhead.length=sizeof(COMMAND_MSG);
	if( type_id==C_HOST_SYS ||type_id==C_SYS_MODE_SET||type_id==C_NET_SWITCH || type_id== DPS_IOST_SET)
	{
		lanhead.dest_id	= 0;
		lanhead.dest_stn	= 0;
		lanhead.dp_type	= COMM_RECORD_TIME;
	}
	else
	{
		lanhead.dest_id 	= cmd_buf.dev_id;
		lanhead.dest_stn	= cmd_buf.stn_id;
		lanhead.dp_type	=COMM_SIM; //COMM_RECORD_TIME;//COMM_SIM;
	}	
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);

	return TRUE;
}
