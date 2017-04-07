#ifndef SimuComm_H
#define SimuComm_H
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

int		BdCmdByNameStr(char *ptNameStr, unsigned char type_id, USHORT cmd_stat); //, float);

#endif