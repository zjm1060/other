#include "SimuService.h"

SimuService::SimuService(QObject* parent)
: QObject(parent)
{
	//qDebug() << "SimuService = " <<QThread::currentThreadId();
	readPointThread = new ReadPointThread;
	readMsgThread = new ReadMsgThread;
	connect(TcpServer::instance(), SIGNAL(newConnection()), readPointThread, SLOT(setInitValue()));
}

void SimuService::Run() 
{
	while (!TcpServer::instance()->isListening() && !TcpServer::instance()->listen(QHostAddress::Any,PORT))
	{    
		qDebug() << QString("The following error occurred: %1.").arg(TcpServer::instance()->errorString());
		MySleep(1000);
	}
	readPointThread->start();
	readMsgThread->start();
}

void SimuService::Stop() 
{
	readPointThread->exit();
	readMsgThread->exit();
	qApp->quit();
	exit(0);
}
void ReadPointThread::run()
{
	if (initPoint() == false)	return;
	m_bInitValue = false;
	while(true)
	{
		if (m_bInitValue)	
		{
			for (int i=0;i<POINT_LIST.count();i++)
			{
				POINT_MAP[POINT_LIST.at(i)].state = -1;
				POINT_MAP[POINT_LIST.at(i)]._value = -1;
				POINT_MAP[POINT_LIST.at(i)].tag_state = -1;
			}
			m_bInitValue = false;
		}
		int	rtstate, tag_state;
		float value;
		USHORT	state;
		DMS_COMMON	dbentry;
		for (int i=0;i<POINT_LIST.count();i++)
		{
			memset( &dbentry, 0, sizeof(dbentry) );	

			if( -1==ReadEntryByNameStr(POINT_LIST.at(i).toLocal8Bit().data(),&dbentry) )
			{
				qDebug() <<"Err ReadEntryByNameStr "<<POINT_LIST.at(i);
				continue;
			}
			switch(dbentry.point.data_type)
			{
			case	ANA_TYPE:
			case	PARAM_TYPE:
				value=(float)dbentry.var.anlg.value.data/1000.0;
				if(POINT_MAP[POINT_LIST.at(i)]._value == -1 ||
					fabs(value-POINT_MAP[POINT_LIST.at(i)]._value)>POINT_MAP[POINT_LIST.at(i)].step)
				{
					COMM_MSG	*comm_msg;
					comm_msg = new COMM_MSG;
					memset( comm_msg, 0, sizeof(COMM_MSG) );
					comm_msg->msg_type = MSG_TYPE_ANACHG;
					strcpy( comm_msg->logicname, POINT_LIST.at(i).toLocal8Bit().data());
					comm_msg->_value = value;

					TcpServer::instance()->ProcessSendMsgData( comm_msg );

					delete comm_msg;

					qDebug() << QString("POINT_ANA(%1) %2  old=%3  new=%4")
						.arg(i)
						.arg(POINT_LIST.at(i))
						.arg(POINT_MAP[POINT_LIST.at(i)]._value)
						.arg(value);
					POINT_MAP[POINT_LIST.at(i)]._value = value;
				}
				break;
			case	POL_TYPE:
			case	SOE_TYPE:
			case	SYSMSG_TYPE:
				rtstate =dbentry.var.ind.status.rt_state;
				if(POINT_MAP[POINT_LIST.at(i)].state == -1 || rtstate!=POINT_MAP[POINT_LIST.at(i)].state)
				{
					COMM_MSG	*comm_msg;
					comm_msg = new COMM_MSG;
					memset( comm_msg, 0, sizeof(COMM_MSG) );

					comm_msg->msg_type = MSG_TYPE_INDCHG;
					strcpy( comm_msg->logicname, POINT_LIST.at(i).toLocal8Bit().data());
					comm_msg->state = rtstate;

					TcpServer::instance()->ProcessSendMsgData( comm_msg );

					delete comm_msg;

					qDebug() << QString("POINT_IND(%1) %2  old=%3  new=%4")
						.arg(i)
						.arg(POINT_LIST.at(i))
						.arg(POINT_MAP[POINT_LIST.at(i)].state)
						.arg(rtstate);
					POINT_MAP[POINT_LIST.at(i)].state = rtstate;
				}
				break;
			case	DEVOBJ_TYPE:
				GetObjPtStateByNameStr(POINT_LIST.at(i).toLocal8Bit().data(), &state );
				rtstate =state;
				if(POINT_MAP[POINT_LIST.at(i)].state == -1 || rtstate!=POINT_MAP[POINT_LIST.at(i)].state)
				{
					COMM_MSG	*comm_msg;
					comm_msg = new COMM_MSG;
					memset( comm_msg, 0, sizeof(COMM_MSG) );

					comm_msg->msg_type = MSG_TYPE_INDCHG;
					strcpy( comm_msg->logicname, POINT_LIST.at(i).toLocal8Bit().data());
					comm_msg->state = rtstate;

					TcpServer::instance()->ProcessSendMsgData( comm_msg );

					delete comm_msg;

					qDebug() << QString("POINT_IND(%1) %2  old=%3  new=%4")
						.arg(i)
						.arg(POINT_LIST.at(i))
						.arg(POINT_MAP[POINT_LIST.at(i)].state)
						.arg(rtstate);
					POINT_MAP[POINT_LIST.at(i)].state = rtstate;
				}
				break;
			default:
				break;
			}//end switch
			tag_state = dbentry.fixed.iost.tagState;
			if(tag_state>1)
				tag_state=1;/*0-ÕªÅÆ   1-¹ÒÅÆ£¬OIXÉÏÉèÖÃ¹ÒÅÆ×´Ì¬Èç¹û´óÓÚ1£¬°´1´¦Àí*/
			if( POINT_MAP[POINT_LIST.at(i)].tag_state == -1 || tag_state!=POINT_MAP[POINT_LIST.at(i)].tag_state)
			{
				COMM_MSG	*comm_msg;
				comm_msg = new COMM_MSG;
				memset( comm_msg, 0, sizeof(COMM_MSG) );

				comm_msg->msg_type = MSG_TYPE_DEVTAG;
				strcpy( comm_msg->logicname, POINT_LIST.at(i).toLocal8Bit().data());
				comm_msg->state = tag_state;
				short sta_ioattr = dbentry.fixed.ioattr.procType ;
				if(sta_ioattr==1||sta_ioattr==4)//1£ºBREAKER   4£ºSWITCH
				{
					if(dbentry.var.ind.status.rt_state==0)//·ÖÕ¢Ì¬
						comm_msg->tag_type = 2;////½ûÖ¹ºÏÕ¢
					else if(dbentry.var.ind.status.rt_state==1)//ºÏÕ¢Ì¬
						comm_msg->tag_type = 3;////½ûÖ¹·ÖÕ¢
					else
						comm_msg->tag_type = 1;////½ûÖ¹²Ù×÷
				}
				else
					comm_msg->tag_type = 1;////½ûÖ¹²Ù×÷

				TcpServer::instance()->ProcessSendMsgData( comm_msg );

				delete comm_msg;

				qDebug() << QString("POINT_TAG(%1) %2  old=%3  new=%4")
					.arg(i)
					.arg(POINT_LIST.at(i))
					.arg(POINT_MAP[POINT_LIST.at(i)].tag_state)
					.arg(tag_state);

				POINT_MAP[POINT_LIST.at(i)].tag_state = tag_state;
			}
		}//end for
		MySleep(10);
	}//end while
}
bool ReadPointThread::initPoint()
{
	POINT_LIST.clear();
	POINT_MAP.clear();
	QFile file_ind("/home/ems/h9000/ots/IND_TABLE.txt");
	QFile file_ana("/home/ems/h9000/ots/ANA_TABLE.txt");
	if (file_ind.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		int index;
		QString point_name;
		QTextStream in(&file_ind);
		do 
		{
			in>>index>>point_name;
			if( point_name.length()>1 )
			{
				POINT_LIST.append(point_name);
				POINT_INFO point_info;
				point_info.state = -1;
				point_info._value = -1;
				point_info.step = -1;
				point_info.tag_state = -1;
				POINT_MAP.insert(point_name,point_info);
			}
		} while (!in.readLine().isNull());
	}
	else
	{
		qDebug() <<"Failed to open " << file_ind.fileName();
		return false;
	}
	if (file_ana.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		int index;
		float step;
		QString point_name;
		QTextStream in(&file_ana);
		do 
		{
			in>>index>>point_name>>step;
			if( point_name.length()>1 )
			{
				POINT_LIST.append(point_name);
				POINT_INFO point_info;
				point_info.state = -1;
				point_info._value = -1;
				point_info.step = step;
				point_info.tag_state = -1;
				POINT_MAP.insert(point_name,point_info);
			}
		} while (!in.readLine().isNull());
	}
	else
	{
		qDebug() <<"Failed to open " << file_ana.fileName();
		return false;
	}
	return true;
}


void ReadMsgThread::run()
{
	char msg[SMS_MESSAGE_SIZE];
	short rcv_pnt,rcv_end;
	char *pt_id=NULL;
	//	char *entryLongname;
	char	str[POINT_NAME_SIZE];
	POINTER pt;
	rcv_end = shm_rec_addr->head.header_ptr;
	for(;;)
	{
		while( rcv_end!=shm_rec_addr->head.header_ptr )
		{
			rcv_pnt = rcv_end;
			rcv_end++;
			if(rcv_end >= MAX_SHM_RECORD_NUM)
				rcv_end = 0;

			strcpy(msg,shm_rec_addr->rec[rcv_pnt].sms_rec.message);
			if (strstr(msg,"switch:"))
			{
				pt_id = strtok(msg,":");
				pt_id = strtok(NULL,":");
				GetPtIdFromStr(pt_id,&pt);
				//printf("msg=%s\n",pt_id);
				GetPtNameById(&pt, str, NULL );

				COMM_MSG	*comm_msg;
				comm_msg = new COMM_MSG;
				memset( comm_msg, 0, sizeof(COMM_MSG) );
				comm_msg->msg_type =MSG_TYPE_DEVMAP;
				strcpy( comm_msg->logicname, str );
				TcpServer::instance()->ProcessSendMsgData( comm_msg );
				delete comm_msg;
				printf("DEVMAP pt_name=%s(%s)\n",str,pt_id);
			}
		}
		MySleep(100);
	}
}