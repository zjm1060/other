#include "stdafx.h"
#include "SettingServ.h"
#include "RawSocketServerWorker.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <lm.h>
#include "rtdbapi.h"
#include <list>
#include <map>
#include <math.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

CWizThreadDispatcher	*g_Dispatcher_Student, *g_Dispatcher_Teacher;		//响应终端的各种请求
Worker		*g_Worker_Student, *g_Worker_Teacher;
HANDLE		g_hShutDownEvent, g_hTimerEvent;

FILE		*g_fErrLog;
time_t		g_Time, g_ResetTime = 0;
struct tm	g_Tm;	

BYTE		g_nTeacher;

UINT		sock_to_student_port,sock_from_student_port, g_nTeacherPort;	
SOCKET		sock_to_student=INVALID_SOCKET, sock_from_student=INVALID_SOCKET;  
SOCKADDR_IN saUdpServ_from_student, saUdpServ_to_student, saUdpCli_From, saUdpCli_To;	
int			g_nStudentConnected = 0, g_nTeacherConnected = 0, g_nWorkerNum;

map<string, int> DO_INDEX;
map<string, int> AO_INDEX;
map<string, int> DI_INDEX;
map<string, int> AI_INDEX;
map<string, int> EX_INDEX;
map<string, int> ZYCB_INDEX;
map<string, int> PSXF_INDEX;
map<string, int> ZLLN_INDEX;
map<string, int> GUADIXIAN_INDEX;

list<string> DO_LIST;
list<string> AO_LIST;
list<string> EX_LIST;
list<string> ZYCB_LIST;
list<string> ZLLN_LIST;
list<string> PSXF_LIST;

int			g_nRecNumND = 0;
int			POWER_NODE_STATE[POWER_NODE_NUM];
list<string> POWER_NODE_LIST[POWER_NODE_NUM];

map<string, int> DO_STATE;
map<string, int> DI_STATE;
map<string, int> EX_STATE;
map<string, int> ZYCB_STATE;
map<string, int> PSXF_STATE;
map<string, int> ZLLN_STATE;
map<string, float> AO_VALUE;
map<string, float> AI_VALUE;
map<string, int> GUADIXIAN_STATE;

list<string> AI_LIST;
list<string> DI_LIST;
list<string> PULS_LIST;
list<string> GUADIXIAN_LIST;

int			g_nCount=0;
bool		g_bConnectRTDBHost = false;

WORKER_INFO		g_StudentWorkerList[STUDENT_NUM];

int	ProcessMsgData( COMM_MSG *msg, int nIndex=-1 );

extern CSettingServ *g_pServ;

int init_DO()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int index;

	file_name = "./SCT_DO.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志

	DO_LIST.clear();
	DO_STATE.clear();

	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		return -1;
	}
	
	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);
		
		line_stream>>str;

		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			DO_LIST.push_back( cstr );
			DO_STATE[cstr] = -10000;

			index = GetIndex( "SCT","DO","ID", (char *)cstr.c_str() );

			if( index>=0 )
				DO_INDEX[cstr.c_str()] = index;
		}
	}
	
	// 关闭文件
	fin.close();

	return 0;
}

int init_AO()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int index;

	file_name = "./SCT_AO.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志
	
	AO_LIST.clear();
	AO_VALUE.clear();
	
	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		return -1;
	}
	
	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);
		
		line_stream>>str;
		
		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			AO_LIST.push_back( cstr );
			AO_VALUE[cstr] = -20000;
			
			index = GetIndex( "SCT","AO","ID", (char *)cstr.c_str() );

			if( index>=0 )
				AO_INDEX[cstr.c_str()] = index;
		}
	}
	
	// 关闭文件
	fin.close();

	return 0;
}

int init_GUADIXIAN()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int	index;

	file_name = "./SCT_GUADIXIAN.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志

	GUADIXIAN_LIST.clear();
	GUADIXIAN_STATE.clear();
	GUADIXIAN_INDEX.clear();

	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		// TODO 错误类型编码
		return -1;
	}

	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);

		line_stream>>str;

		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			GUADIXIAN_LIST.push_back( cstr );

			index = GetIndex( "SCT","GUADX","ID", (char *)cstr.c_str() );

			GUADIXIAN_STATE[cstr] = -10000;

			if( index>=0 )
				GUADIXIAN_INDEX[cstr.c_str()] = index;
		}
	}

	// 关闭文件
	fin.close();

	return 0;
}

int init_ZYCB()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int	index;

	file_name = "./ps_zycb.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志
	
	ZYCB_LIST.clear();
	ZYCB_STATE.clear();
	ZYCB_INDEX.clear();

	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		// TODO 错误类型编码
		return -1;
	}
	
	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);
		
		line_stream>>str;
		
		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			ZYCB_LIST.push_back( cstr );

			index = GetIndex( "ps","zycb","id", (char *)cstr.c_str() );

			ZYCB_STATE[cstr] = -10000;

			if( index>=0 )
				ZYCB_INDEX[cstr.c_str()] = index;
		}
	}
	
	// 关闭文件
	fin.close();

	return 0;
}

int init_PSXF()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int	index;

	file_name = "./ps_xf.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志

	PSXF_LIST.clear();
	PSXF_STATE.clear();
	PSXF_INDEX.clear();

	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		// TODO 错误类型编码
		return -1;
	}

	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);

		line_stream>>str;

		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			PSXF_LIST.push_back( cstr );

			index = GetIndex( "ps","xf","id", (char *)cstr.c_str() );

			PSXF_STATE[cstr] = -10000;

			if( index>=0 )
				PSXF_INDEX[cstr.c_str()] = index;
		}
	}

	// 关闭文件
	fin.close();

	return 0;
}

int init_ZLLN()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int	index;

	file_name = "./ps_zlln.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志

	ZLLN_LIST.clear();
	ZLLN_STATE.clear();
	ZLLN_INDEX.clear();

	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		// TODO 错误类型编码
		return -1;
	}

	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);

		line_stream>>str;

		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			ZLLN_LIST.push_back( cstr );

			index = GetIndex( "ps","zlln","id", (char *)cstr.c_str() );

			ZLLN_STATE[cstr] = -10000;

			if( index>=0 )
				ZLLN_INDEX[cstr.c_str()] = index;
		}
	}

	// 关闭文件
	fin.close();

	return 0;
}

int init_AI()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int	index;

	file_name = "./SCT_AI.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志
	
	AI_LIST.clear();
	AI_VALUE.clear();
	
	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		// TODO 错误类型编码
		return -1;
	}
	
	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);
		
		line_stream>>str;
		
		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			AI_LIST.push_back( cstr );

			index = GetIndex( "SCT","AI","ID", (char *)cstr.c_str() );

			if( index>=0 )
				AI_INDEX[cstr.c_str()] = index;
		}
	}
	
	// 关闭文件
	fin.close();

	return 0;
}

int init_DI()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int	index;

	file_name = "./SCT_DI.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志
	
	DI_LIST.clear();
	DI_STATE.clear();
	
	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		// TODO 错误类型编码
		return -1;
	}
	
	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);
		
		line_stream>>str;
		
		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			DI_LIST.push_back( cstr );

			index = GetIndex( "SCT","DI","ID", (char *)cstr.c_str() );

			if( index>=0 )
				DI_INDEX[cstr.c_str()] = index;
		}
	}
	
	// 关闭文件
	fin.close();

	return 0;
}

int init_PULS()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;

	file_name = "./SCT_PULS.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志
	
	PULS_LIST.clear();

	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		// TODO 错误类型编码
		return -1;
	}
	
	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);
		
		line_stream>>str;
		
		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			PULS_LIST.push_back( cstr );
		}
	}
	
	// 关闭文件
	fin.close();

	return 0;
}

int init_EX()
{
	string str;
	string file_name;
	stringstream line_stream;
	ifstream fin;
	int index;

	file_name = "./SCT_EX.txt";
	fin.clear();	// 用同一文件指针多次打开文件时,需要先清空标志

	EX_LIST.clear();
	EX_STATE.clear();
	EX_INDEX.clear();

	fin.open(file_name.c_str(), ios::in);
	if(!fin.is_open()) {
		std::cerr<<"Failed to open "<<file_name<<endl;
		return -1;
	}
	
	// 遍历文件,读取数据
	while(getline(fin, str)) {
		line_stream.clear();
		line_stream.str(str);
		
		line_stream>>str;

		if( str.length()>1 )
		{
			string cstr( str.c_str() );
			EX_LIST.push_back( cstr );
			EX_STATE[cstr] = 0;

			index = GetIndex( "SCT","EX","ID", (char *)cstr.c_str() );

			if( index>=0 )
				EX_INDEX[cstr.c_str()] = index;
		}
	}
	
	// 关闭文件
	fin.close();

	return 0;
}

UINT Read_Reset_Thread( LPVOID lParam )
{	
	while (1)
	{
		if( NULL==g_hShutDownEvent )	//全局的“关闭程序”事件被删除，应该已经退出了？？
			return 0;

		const DWORD res = 
			::WaitForSingleObject(g_hShutDownEvent, 0);

		switch (res)
		{
		case WAIT_FAILED: //全局的“关闭程序”事件被删除，应该已经退出了？？
			return 0;

		case WAIT_OBJECT_0: //全局的“关闭程序”事件被触发，退出线程，准备关闭
			return 0;

		case WAIT_TIMEOUT: 
			{		
				if( g_nStudentConnected<=0 || (!g_bConnectRTDBHost))
					break;

				int nReset = 0;

				if (GetFldValue("SCT","autosnap",(char*)&nReset))
				{
					if( nReset && (g_Time-g_ResetTime)>2 )
					{
						cout<<"autosnap"<<endl;

						g_ResetTime = g_Time;

						COMM_MSG	*comm_msg;
						comm_msg = new COMM_MSG;
						memset( comm_msg, 0, sizeof(COMM_MSG) );
						comm_msg->student = 254;

						strcat( comm_msg->id, "RESET_" );

						ProcessMsgData( comm_msg );
					}
				}
			}
			break;

		default:
			break;
		}//switch (res)

		Sleep( 200 );
	} // while 1

	return 1;
}

UINT Read_AODO_Thread( LPVOID lParam )
{	
	while (1)
	{
		if( NULL==g_hShutDownEvent )	//全局的“关闭程序”事件被删除，应该已经退出了？？
			return 0;
		
		const DWORD res = 
			::WaitForSingleObject(g_hShutDownEvent, 0);
		
		switch (res)
		{
			case WAIT_FAILED: //全局的“关闭程序”事件被删除，应该已经退出了？？
				return 0;
				
			case WAIT_OBJECT_0: //全局的“关闭程序”事件被触发，退出线程，准备关闭
				return 0;
				
			case WAIT_TIMEOUT: 
			{		
				if( !g_bConnectRTDBHost )
				{
					if( !ConnectHost() )
					{
						cerr << _T("Fatal Error: ConnectHost failed") << endl;

						g_bConnectRTDBHost = false;		

						Sleep( 1000);
						break;
					}
					else
					{
						//////////////////////////////////////////////////////////////////////
						//读取验电的全部点表
						g_nRecNumND = GetRecNum( "ps", "nd" );
						int i;
						int	rec_num;

						rec_num = GetRecNum( "ps", "cb" );
						for( i=0; i<rec_num; i++ )
						{
							char	szID[256];
							int		ind1, ind2;

							memset( szID, 0, sizeof(szID) );
							if( !GetInFldValue( i, "ps","cb", "id", szID ) )
							{
								cerr<<"Err GetFldValue pscb-id "<<i<<endl;
								continue;
							}

							if( !GetInFldValue( i, "ps","cb", "ind1", (char *)&ind1 ) )
							{
								cerr<<"Err GetFldValue pscb-ind1 "<<i<<endl;
								continue;
							}

							if( !GetInFldValue( i, "ps","cb", "ind2", (char *)&ind2 ) )
							{
								cerr<<"Err GetFldValue pscb-ind2 "<<i<<endl;
								continue;
							}

							if( ind1>=0 && ind1<POWER_NODE_NUM && strlen(szID)>0 )
							{
								string str( szID );
								str += "_ind1";

								POWER_NODE_LIST[ind1].push_back( str );
							}

							if( ind2>=0 && ind2<POWER_NODE_NUM && strlen(szID)>0 )
							{
								string str( szID );
								str += "_ind2";

								POWER_NODE_LIST[ind2].push_back( str );
							}
						}

						rec_num = GetRecNum( "ps", "gnsw" );
						for( i=0; i<rec_num; i++ )
						{
							char	szID[256];
							int		ind;

							memset( szID, 0, sizeof(szID) );
							if( !GetInFldValue( i, "ps","gnsw", "id", szID ) )
							{
								cerr<<"Err GetFldValue gnsw-id "<<i<<endl;
								continue;
							}

							if( !GetInFldValue( i, "ps","gnsw", "ind", (char *)&ind ) )
							{
								cerr<<"Err GetFldValue gnsw-ind "<<i<<endl;
								continue;
							}

							if( ind>=0 && ind<POWER_NODE_NUM && strlen(szID)>0 )
							{
								string str( szID );
								str += "_ind";

								POWER_NODE_LIST[ind].push_back( str );
							}
						}

						init_AO();	
						init_DO();	
						init_EX();
						/*init_AI();
						init_DI();						
						init_PULS();
						init_ZYCB();
						init_ZLLN();
						init_GUADIXIAN();
						init_PSXF();*/

						g_bConnectRTDBHost = true;

					}//if( !ConnectHost() )
					
				}//if( !g_bConnectRTDBHost )	

				if( g_nStudentConnected<=0 || (!g_bConnectRTDBHost))
					break;

				list<string>::iterator itor;
				int state;
				float value;
				for( itor=DO_LIST.begin(); itor!=DO_LIST.end(); itor++ )
				{
					//if( !GetFldValue("SCT","DO","ID",(*itor).c_str(),"STATE",(char *)&state) )
					if( !GetFldValue( DO_INDEX[(*itor).c_str()], "SCT","DO", "STATE",(char *)&state) )
						cerr<<"Err GetFldValue STATE "<<(*itor).c_str()<<endl;
					else
					if( state!=DO_STATE[(*itor)] )
					{
						if(DO_STATE[(*itor)] > -10000 )
						{
							COMM_MSG	*comm_msg;
							comm_msg = new COMM_MSG;
							memset( comm_msg, 0, sizeof(COMM_MSG) );
							comm_msg->student = 254;

							strcat( comm_msg->id, (*itor).c_str() );
							comm_msg->state = state;
							comm_msg->value = state;

							ProcessMsgData( comm_msg );

							delete comm_msg;

							cout<<(*itor).c_str()<<endl;
						}

						DO_STATE[(*itor)] = state;
					}

					g_nCount++;

					if( (g_nCount%500)==0 )
						Sleep( 10 );
				}			

				for( itor=AO_LIST.begin(); itor!=AO_LIST.end(); itor++ )
				{
					//if( !GetFldValue("SCT","AO","ID",(*itor).c_str(),"VALUE",(char *)&value) )
					if( !GetFldValue( AO_INDEX[(*itor).c_str()], "SCT","AO","VALUE",(char *)&value) )
						cerr<<"Err GetFldValue VALUE "<<(*itor).c_str()<<endl;
					else
					{
						if( ( fabs(AO_VALUE[(*itor)])<=FINIT && fabs(value)>FINIT )
							||( fabs(AO_VALUE[(*itor)])>FINIT && 
							fabs((value-AO_VALUE[(*itor)])/AO_VALUE[(*itor)])>0.01 ) )
						{
							if( AO_VALUE[(*itor)] > -10000 )
							{
								COMM_MSG	*comm_msg;
								comm_msg = new COMM_MSG;
								memset( comm_msg, 0, sizeof(COMM_MSG) );
								comm_msg->student = 254;

								strcat( comm_msg->id, (*itor).c_str() );
								comm_msg->state = value;
								comm_msg->value = value;

								ProcessMsgData( comm_msg );

								delete comm_msg;

								cout<<(*itor).c_str()<<endl;
							}

							AO_VALUE[(*itor)] = value;
						}
					}
						
					g_nCount++;

					if( (g_nCount%500)==0 )
						Sleep( 10 );
				}			

				for( itor=EX_LIST.begin(); itor!=EX_LIST.end(); itor++ )
				{
					if( !GetFldValue( EX_INDEX[(*itor).c_str()], "SCT","EX","VALUE",(char *)&state) )
						cerr<<"Err GetFldValue STATE "<<(*itor).c_str()<<endl;
					else
					if( state!=EX_STATE[(*itor)] )
					{
						COMM_MSG	*comm_msg;
						comm_msg = new COMM_MSG;
						memset( comm_msg, 0, sizeof(COMM_MSG) );
						comm_msg->student = 254;
						
						strcat( comm_msg->id, (*itor).c_str() );
						comm_msg->state = state;
						comm_msg->value = state;

						ProcessMsgData( comm_msg );

						delete comm_msg;

						EX_STATE[(*itor)] = state;

						cout<<(*itor).c_str()<<endl;
						InsertErrLog((char *)((*itor).c_str()), EVENTLOG_ERROR_TYPE);
					}

					g_nCount++;

					if( (g_nCount%500)==0 )
						Sleep( 10 );
				}			

				/*for( itor=ZYCB_LIST.begin(); itor!=ZYCB_LIST.end(); itor++ )
				{
					if( !GetInFldValue( ZYCB_INDEX[(*itor).c_str()], "ps","zycb","iopen",(char *)&state) )
						cerr<<"Err GetFldValue ZYCB iopen "<<(*itor).c_str()<<endl;
					else
					{
						if( state )
							state = 0;
						else
							state = 1;

						if( state!=ZYCB_STATE[(*itor)] )
						{
							COMM_MSG	*comm_msg;
							comm_msg = new COMM_MSG;
							memset( comm_msg, 0, sizeof(COMM_MSG) );
							comm_msg->student = 254;
							
							strcat( comm_msg->id, (*itor).c_str() );
							comm_msg->state = state;
							comm_msg->value = state;

							ProcessMsgData( comm_msg );

							delete comm_msg;

							ZYCB_STATE[(*itor)] = state;

							cout<<(*itor).c_str()<<endl;
						}

						g_nCount++;

						if( (g_nCount%500)==0 )
							Sleep( 10 );
					}
				}			

				for( itor=PSXF_LIST.begin(); itor!=PSXF_LIST.end(); itor++ )
				{
					if( !GetInFldValue( PSXF_INDEX[(*itor).c_str()], "ps","xf","iclose1",(char *)&state) )
						cerr<<"Err GetFldValue PSXF iclose1 "<<(*itor).c_str()<<endl;
					else
					{
						if( state!=PSXF_STATE[(*itor)] )
						{
							COMM_MSG	*comm_msg;
							comm_msg = new COMM_MSG;
							memset( comm_msg, 0, sizeof(COMM_MSG) );
							comm_msg->student = 254;

							strcat( comm_msg->id, (*itor).c_str() );
							comm_msg->state = state;
							comm_msg->value = state;

							ProcessMsgData( comm_msg );

							delete comm_msg;

							PSXF_STATE[(*itor)] = state;

							cout<<(*itor).c_str()<<endl;
						}

						g_nCount++;

						if( (g_nCount%500)==0 )
							Sleep( 10 );
					}
				}			

				for( itor=ZLLN_LIST.begin(); itor!=ZLLN_LIST.end(); itor++ )
				{
					if( !GetFldValue( ZLLN_INDEX[(*itor).c_str()], "ps","zlln","iopen",(char *)&state) )
						cerr<<"Err GetFldValue ZLLN iopen "<<(*itor).c_str()<<endl;
					else
					{
						if( state )
							state = 0;
						else
							state = 1;

						if( state!=ZLLN_STATE[(*itor)] )
						{
							COMM_MSG	*comm_msg;
							comm_msg = new COMM_MSG;
							memset( comm_msg, 0, sizeof(COMM_MSG) );
							comm_msg->student = 254;

							strcat( comm_msg->id, (*itor).c_str() );
							comm_msg->state = state;
							comm_msg->value = state;

							ProcessMsgData( comm_msg );

							delete comm_msg;

							ZLLN_STATE[(*itor)] = state;

							cout<<(*itor).c_str()<<endl;
						}

						g_nCount++;

						if( (g_nCount%500)==0 )
							Sleep( 10 );
					}
				}			

				for( itor=GUADIXIAN_LIST.begin(); itor!=GUADIXIAN_LIST.end(); itor++ )
				{
					if( !GetFldValue( GUADIXIAN_INDEX[(*itor).c_str()], "SCT","GUADX","STATE",(char *)&state) )
						cerr<<"Err GetFldValue GUADX STATE "<<(*itor).c_str()<<endl;
					else
					{
						if( state!=GUADIXIAN_STATE[(*itor)] )
						{
							COMM_MSG	*comm_msg;
							comm_msg = new COMM_MSG;
							memset( comm_msg, 0, sizeof(COMM_MSG) );
							comm_msg->student = 254;

							strcat( comm_msg->id, (*itor).c_str() );
							comm_msg->state = state;
							comm_msg->value = state;

							ProcessMsgData( comm_msg );

							delete comm_msg;

							GUADIXIAN_STATE[(*itor)] = state;

							cout<<(*itor).c_str()<<endl;
						}

						g_nCount++;

						if( (g_nCount%500)==0 )
							Sleep( 10 );
					}
				}			

				for( int i=1; i<=g_nRecNumND; i++ )
				{
					if( !GetOutFldValue( i, "ps","nd","idead",(char *)&state) )
						cerr<<"Err GetFldValue psnd STATE "<<(*itor).c_str()<<endl;
					else
					if( state!=POWER_NODE_STATE[i] )
					{
						for( itor=POWER_NODE_LIST[i].begin(); itor!=POWER_NODE_LIST[i].end(); itor++ )
						{
							COMM_MSG	*comm_msg;
							comm_msg = new COMM_MSG;
							memset( comm_msg, 0, sizeof(COMM_MSG) );
							comm_msg->student = 254;
							
							strcat( comm_msg->id, (*itor).c_str() );

							if( 0==state )
							{
								comm_msg->state = 1;
								comm_msg->value = 1;
							}
							else
							if( 1==state )
							{
								comm_msg->state = 0;
								comm_msg->value = 0;
							}

							ProcessMsgData( comm_msg );

							delete comm_msg;

							cout<<(*itor).c_str()<<endl;
						}

						POWER_NODE_STATE[i] = state;
					}

					g_nCount++;

					if( (g_nCount%500)==0 )
						Sleep( 10 );
				}	*/
			}
			break;
				
			default:
				break;
		}//switch (res)

		Sleep( 10 );
	} // while 1

	return 1;
}

CSettingServ::CSettingServ()
	: CNTService(TEXT("SettingServ"))
{
	m_dwControlsAccepted = 0;
	m_dwControlsAccepted |= SERVICE_ACCEPT_STOP;

	g_fErrLog = NULL;

	g_fErrLog = fopen("SettingServ.log","wt");

	time(&g_Time);
	struct tm *pTm = localtime(&g_Time);
	memcpy(&g_Tm, pTm, sizeof(struct tm));

	for( int i=0; i<POWER_NODE_NUM; i++ )
	{
		POWER_NODE_STATE[i] = -1;
	}

	loadconfig();
}

void CSettingServ::Run(DWORD dwArgc, LPTSTR * ppszArgv) 
{

#if 1
	HWND hwnd=GetConsoleWindow();
	ShowWindow(hwnd,SW_HIDE);
#endif

	// report to the SCM that we're about to start
	ReportStatus(SERVICE_START_PENDING);

	if (!AfxSocketInit())
	{
		InsertErrLog("SOCKETS_INIT_FAILED",EVENTLOG_ERROR_TYPE);
	}

	g_hShutDownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hTimerEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	ReportStatus(SERVICE_START_PENDING);

	AfxBeginThread(TimerThread,NULL);

	////////////////////////////////////////////////////////
	g_Worker_Student = new Worker( g_nTeacherPort, CONNECT_WITH_STUDENT );

	g_Dispatcher_Student = new CWizThreadDispatcher( *g_Worker_Student, g_nWorkerNum );
	g_Dispatcher_Student->Start();

	////////////////////////////////////////////////////////////
	g_Worker_Teacher= new Worker( g_nTeacherPort - g_nTeacher, CONNECT_WITH_TEACHER );
	
	g_Dispatcher_Teacher = new CWizThreadDispatcher( *g_Worker_Teacher, 5 );
	g_Dispatcher_Teacher->Start();
	
	AfxBeginThread( Read_AODO_Thread, NULL, THREAD_PRIORITY_HIGHEST );

	AfxBeginThread( Read_Reset_Thread, NULL, THREAD_PRIORITY_HIGHEST );

	// report SERVICE_RUNNING immediately before you enter the main-loop
	// DON'T FORGET THIS!
	ReportStatus(SERVICE_RUNNING);

	while(::WaitForSingleObject(g_hShutDownEvent, 0) != WAIT_OBJECT_0 ) 
	{
		SetEvent(g_hTimerEvent);
		::Sleep( 1000 );
	}

	if( g_hShutDownEvent )
	{
		::SetEvent(g_hShutDownEvent);

		CloseHandle(g_hShutDownEvent);
	}

	g_Dispatcher_Student->Stop(FALSE);
	
	if(g_Dispatcher_Student!=NULL)
	{
		delete g_Dispatcher_Student;
		g_Dispatcher_Student = NULL;
	}

	g_Dispatcher_Teacher->Stop(FALSE);
	
	if(g_Dispatcher_Teacher!=NULL)
	{
		delete g_Dispatcher_Teacher;
		g_Dispatcher_Teacher = NULL;
	}

	if(g_Worker_Student!=NULL)
	{
		delete g_Worker_Student;
		g_Worker_Student = NULL;
	}

	if(g_Worker_Teacher!=NULL)
	{
		delete g_Worker_Teacher;
		g_Worker_Teacher = NULL;
	}

	if( g_hTimerEvent )
	{
		CloseHandle(g_hTimerEvent);
	}

	if(g_fErrLog!=NULL)
	{
		fclose(g_fErrLog);
		g_fErrLog = NULL;
	}

	DisConnectHost();
}

void CSettingServ::Stop() 
{
	ReportStatus(SERVICE_STOP_PENDING, 3000);

	if( g_hShutDownEvent )
		::SetEvent(g_hShutDownEvent);
}

void loadconfig()
{
	char szIniFile[256], station[256], str2[256];
	char szInfo[1024];

	memset(szInfo,0,sizeof(szInfo));
	memset(station,0,sizeof(station));
	memset(szIniFile,0,sizeof(szIniFile));	

	if(GetModuleFileName(NULL,szIniFile,sizeof(szIniFile)))
	{
		CString str;
		str.Format("%s",szIniFile);
		int nPos = str.Find('.');
		sprintf(szIniFile,"%s.ini",str.Left(nPos));

		GetPrivateProfileString(
			"System","ThreadsNum","2",szInfo,sizeof(szInfo),szIniFile);
		g_nWorkerNum = atoi(szInfo);	

		GetPrivateProfileString(
			"System","Station","any",station,sizeof(station),szIniFile);

		GetPrivateProfileString(
			"STATION","Station_Num","0",szInfo,sizeof(szInfo),szIniFile);
		int n = atoi(szInfo);

		for( g_nTeacher=1; g_nTeacher<=n; g_nTeacher++ )
		{
			memset( str2, 0, sizeof(str2) );
			sprintf( str2, "Station%d", g_nTeacher );

			GetPrivateProfileString(
				"STATION",str2,"",szInfo,sizeof(szInfo),szIniFile);

			if( _stricmp( szInfo, station )==0 )
				break;
		}

		GetPrivateProfileString(
			"NET","base_port","10000",szInfo,sizeof(szInfo),szIniFile);
		g_nTeacherPort = atoi(szInfo) + g_nTeacher;

		memset(szInfo,0,sizeof(szInfo));
		GetPrivateProfileString(
			"DB", "DB_ADDR", "127.0.0.1", szInfo,sizeof(szInfo), szIniFile);

		//if( !ConnectHostByName(szInfo) )

		memset(szInfo,0,sizeof(szInfo));
		GetPrivateProfileString(
			"DB", "DB_DIR", "", szInfo,sizeof(szInfo), szIniFile);

		if( strlen(szInfo)>0 )
		{
			SHELLEXECUTEINFO	sInfo;
			memset(&sInfo, 0, sizeof(sInfo));
			sInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			sInfo.cbSize = sizeof(sInfo);
			sInfo.hwnd = NULL;
			sInfo.lpVerb = "open";
			sInfo.lpFile = "rtdbsrv.exe";
			sInfo.lpParameters = "";	
			sInfo.lpDirectory = szInfo;
			sInfo.nShow = SW_SHOWNORMAL;

			ShellExecuteEx(&sInfo);

			Sleep( 1000 );
		}		
	}
}

void InsertErrLog(char *err,WORD nType)
{		
	fprintf(g_fErrLog,"%d时%d分%d秒 %s\n",g_Tm.tm_hour, g_Tm.tm_min, g_Tm.tm_sec, err);
	fflush(g_fErrLog);

	//g_pServ->AddToMessageLog(err,nType);
}
