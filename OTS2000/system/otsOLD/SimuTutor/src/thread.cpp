#include "thread.h"

UserLoginStateThread::UserLoginStateThread(QObject * parent)
:QThread(parent)
{
	udpSocket = new QUdpSocket(this);
	mapLoginUser.clear();
	udpSocket->bind(HEART_BEAT_PORT,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	connect(udpSocket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
}
void UserLoginStateThread::run()
{
	for (;;)
	{
		QMap<int,int>::const_iterator i = mapLoginUser.constBegin();
		while (i != mapLoginUser.constEnd()) 
		{
			if (i.value() > 5)
			{
				mapLoginUser.remove(i.key());
				emit sigUserStateChanged(i.key(),0);
			}
			else
			{
				mapLoginUser[i.key()] = i.value()+1;	
			}
			++i;
		}
		MySleep(1000);
	}
}
void UserLoginStateThread::readPendingDatagrams()
{
	while (udpSocket->hasPendingDatagrams())
	{
		QByteArray datagram;
		USER_INFO user_info;
		datagram.resize(udpSocket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;
		udpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
		memcpy((char *)&user_info,datagram.data(),datagram.size());
		if (mapLoginUser.contains(user_info.user_id))
		{
			mapLoginUser[user_info.user_id] = 0;
		}
		else
		{
			mapLoginUser.insert(user_info.user_id,user_info.timeout);
			emit sigUserStateChanged(user_info.user_id,1);
		}
	}
}
/**********************************************************************************************************************/
SaveSceneThread::SaveSceneThread(UCHAR stn_id,UCHAR dev_id,int scene_id,QObject * parent)
:QThread(parent),stn_id(stn_id),dev_id(dev_id),scene_id(scene_id)
{

}

void SaveSceneThread::run()
{
	UCHAR	data_type;
	GULONG	rec_num,entry_num;
	int		i,j,id;
	POINTER	pt;
	float	fvalue;
	USHORT	state;	
	char	point_name[POINT_NAME_SIZE];
	DMS_COMMON	common_aa;
	if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
		return;
	for(data_type=1,j=0; j<rec_num; data_type++) 
	{
		if(data_type>=MAX_DATATYPE_NUM)
			break;
		if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)data_type, &entry_num))
			continue;
		j++;
		pt.stn_id= stn_id;
		pt.dev_id= dev_id;
		pt.data_type= data_type;
		for(id=0; id<entry_num; id++)
		{
			pt.pt_id= id;	
			if(-1==GetPtNameById( &pt, point_name,NULL ))
			{
				printf("\nNo this point in database\n" );
				continue;
			}
			switch(data_type)
			{
				case ANA_TYPE:
					if(-1==GetAnaPtfValueById( &pt, &fvalue ))
					{
						printf("\nGet Value error\n" );
						continue;
					}
					addSceneData(point_name,0,fvalue);
					break;
				case DEVOBJ_TYPE:
					if(-1==GetObjPtStateById( &pt, &state ))
					{
						printf("\nGet State error\n" );
						continue;
					}
					addSceneData(point_name,state,0);
					break;
				default:
					if(-1==GetIndPtStateById( &pt, &state ))
					{
						printf("\nGet State error\n" );
						continue;
					}
					addSceneData(point_name,state,0);
					break;
			}
		}
	}
}

void SaveSceneThread::addSceneData(char *point_name,short status,float fvalue)
{
	DB_FMT data;
	mutex.lock();
	InitDbData(&data,TABLE_SCENE_DATA);
	//设置表中每个字段的flag
	data.flag.flag_scene_data.scene_id =TRUE;
	data.flag.flag_scene_data.logicalname=TRUE;
	data.flag.flag_scene_data.status=TRUE;
	data.flag.flag_scene_data.data=TRUE;

	//填充表中每个字段的值

	data.seg.seg_scene_data.scene_id = scene_id;
	strcpy(data.seg.seg_scene_data.logicalname,point_name);
	data.seg.seg_scene_data.status = status;
	data.seg.seg_scene_data.data = fvalue;

	//调用接口函数，参数为DB_FMT结构体
	if( InsertOneRec(&data) != TRUE)
	{
		//QMessageBox().critical(this,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("插入数据失败!"),QMessageBox::Ok);		
		HdbLogWrite(data.sql_str,data.sql_error);
		return;
	}
	mutex.unlock();
}


void SaveSceneRunnable::run()
{
	int i=0;
	int		DataType,	PointId,	Number;
	char		lname[100];
	POINTER		point;
	POINT_NAME	pt_name;	
	int 		Start,End;
	pt_num = 0;
	if( dev_id == 0 )
	{
		Start = 1;
		End = g_dev_num[stn_id];
	}
	else
	{
		Start = dev_id;
		End = dev_id;
	}

	for(dev_id=Start;dev_id<=End;dev_id++)
	{
		for(DataType=1;DataType<=MAX_DATATYPE_NUM;DataType++)
		{								
			if( DataType==ANA_TYPE || DataType==PARAM_TYPE || DataType==IMP_TYPE || DataType==SOE_TYPE || 
				DataType==POL_TYPE || DataType==SYSMSG_TYPE || DataType==DEVOBJ_TYPE || DataType==OUTS_TYPE )
			{	
				point.stn_id	  =	stn_id;	 		
				point.dev_id      =	dev_id;
				point.data_type   =	DataType;
				point.pt_id    =	0;
				Number = GetPointNumber ( &point );
				for(PointId=0;PointId<Number;PointId++)
				{
					point.stn_id	  =	stn_id;
					point.dev_id      =	dev_id;
					point.data_type   =	DataType;
					point.pt_id    =	PointId;

					if(GetPtNameById( &point, lname, &pt_name ) ==-1)
					{
						continue;
					}

					strcpy(scn_data[i].LogicalName,lname); 

					if( (++i)<MAX_PT_NUM ) continue; 
					else return;
				}
			}
		}
	}
	pt_num = i;
	insertSceneData();
}

int	SaveSceneRunnable::insertSceneData()
{
	unsigned int i;
	DB_DATA		db_data;
	//统一函数参数数据体
	DB_FMT data;
	char	log_str[256];
	for(i=0;i<pt_num;i++)
	{
		if(ReadDBByName( scn_data[i].LogicalName, &db_data )==-1)
		{	
			printf("%s invalid!",scn_data[i].LogicalName);
			return FALSE;
		}
		scn_data[i].status = db_data.status;
		scn_data[i].value = db_data.fvalue;
	}
	data.table_id=TABLE_SCENE_DATA;
	InitDbData(&data,data.table_id);
	//将需要访问的字段flag置为1
	data.flag.flag_scene_data.scene_id =TRUE;
	data.flag.flag_scene_data.logicalname=TRUE;
	data.flag.flag_scene_data.status=TRUE;
	data.flag.flag_scene_data.data=TRUE;
	data.seg.seg_scene_data.scene_id = scene_id;
	for(i=0;i<pt_num;i++)
	{
		mutex.lock();
		//填充表中每个字段的值
		data.seg.seg_scene_data.status =scn_data[i].status;
		data.seg.seg_scene_data.data =scn_data[i].value;
		strcpy(data.seg.seg_scene_data.logicalname ,scn_data[i].LogicalName);

		//调用接口函数，参数为DB_FMT结构体
		if( InsertOneRec(&data) != TRUE)
		{
			HdbLogWrite(data.sql_str,data.sql_error);
			printf("插入数据失败\n");
			return FALSE;
		}
		else
		{
			printf("插入数据成功\n");		
		}
		mutex.unlock();
	}
	printf("InsertSceneData successfully, num = %d\n",pt_num);
	return TRUE;
}