#include "stdafx.h"
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

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

extern time_t		g_Time, g_ResetTime;
extern struct tm	g_Tm;	
extern FILE			*g_fErrLog;

extern HANDLE		g_hShutDownEvent, g_hTimerEvent;
extern CWizThreadDispatcher	*g_Dispatcher_Student;		//响应终端的各种请求
extern Worker		*g_Worker_Student;

extern BYTE			g_nTeacher;

extern SOCKET		sock_to_student,sock_from_student;  
extern SOCKADDR_IN	saUdpServ_from_student, saUdpServ_to_student, saUdpCli_To, saUdpCli_From;
extern UINT			sock_to_student_port,sock_from_student_port;	
extern UINT			sock_from_teacher_port, sock_to_teacher_port, g_nTeacherPort;

extern WORKER_INFO	g_StudentWorkerList[STUDENT_NUM];
extern bool			g_bConnectRTDBHost;


extern map<string, int> DO_STATE;
extern map<string, int> ZYCB_STATE;
extern map<string, int> PSXF_STATE;
extern map<string, int> ZLLN_STATE;
extern map<string, float> AO_VALUE;
extern map<string, int> EX_STATE;
extern map<string, int> GUADIXIAN_STATE;

extern list<string> DO_LIST;
extern list<string> AO_LIST;
extern list<string> AI_LIST;
extern list<string> DI_LIST;
extern list<string> EX_LIST;
extern list<string> PULS_LIST;
extern list<string> ZYCB_LIST;
extern list<string> PSXF_LIST;
extern list<string> ZLLN_LIST;
extern list<string> GUADIXIAN_LIST;

extern map<string, int> DO_INDEX;
extern map<string, int> AO_INDEX;
extern map<string, int> DI_INDEX;
extern map<string, int> AI_INDEX;
extern map<string, int> EX_INDEX;
extern map<string, int> ZYCB_INDEX;
extern map<string, int> PSXF_INDEX;
extern map<string, int> ZLLN_INDEX;
extern map<string, int> GUADIXIAN_INDEX;

void ResetStudent( int nWorkerIndex );

int	ProcessMsgData( COMM_MSG *msg, int nIndex )
{
	for(int i=0; i<STUDENT_NUM; i++ )
	{
		if( (nIndex>=0) && (nIndex!=i) )
			continue;

		if( g_StudentWorkerList[i].m_bConnected )
		{
			if( g_StudentWorkerList[i].mutex_msg_to_student_list.Lock() )
			{
				COMM_MSG *new_msg = new COMM_MSG;
				memset( new_msg, 0, sizeof(COMM_MSG) );
				memcpy( new_msg, msg, sizeof(COMM_MSG) );

				g_StudentWorkerList[i].comm_to_student_list.AddTail( new_msg );

				g_StudentWorkerList[i].mutex_msg_to_student_list.Unlock();
			}
		}
	}

	return 1;
}

//设备类型、设备编号、相位、状态、部件编号
int ProcessTTSEVT(int nDeviceType, char *code, int abc, int toStat, int nSubType)
{
	MODEL_MESSAGE	*pmsg = new MODEL_MESSAGE;

	if( NULL==pmsg )
		return 0;

	memset(pmsg, 0, sizeof(MODEL_MESSAGE));

	pmsg->student		= 255;				//255表示向所有学员机发送
	pmsg->weather		= -1;
	pmsg->wind			= -1;			
	pmsg->rank			= nDeviceType;
	pmsg->unit			= nSubType;
	pmsg->phase			= abc;
	pmsg->device_index	= toStat;
	pmsg->scale			= 0;
	pmsg->pos[0]		= 0;
	pmsg->pos[1]		= 0;
	pmsg->pos[2]		= 0;
	pmsg->pos[3]		= 0;
	sprintf(pmsg->model, "TTSEVT%s", code);	

	for(int i=0; i<STUDENT_NUM; i++ )
	{
		if( g_StudentWorkerList[i].m_bConnected )
		{
			MODEL_MESSAGE	*msg = new MODEL_MESSAGE;

			if( msg!=NULL && g_StudentWorkerList[i].mutex_msg_to_student_list.Lock() )
			{
				memcpy( msg, pmsg, sizeof(MODEL_MESSAGE) );

				g_StudentWorkerList[i].msg_to_student_list.AddTail( msg );

				g_StudentWorkerList[i].mutex_msg_to_student_list.Unlock();
			}
		}
	}

	delete pmsg;

	return 1;
}

int ProcessTeacherData( MODEL_MESSAGE *pMsg )
{
	for(int i=0; i<STUDENT_NUM; i++ )
	{
		if( (g_StudentWorkerList[i].m_bConnected) && 
			(pMsg->student==g_StudentWorkerList[i].m_nStudent || pMsg->student==255) )
		{
			MODEL_MESSAGE	*pmsg = new MODEL_MESSAGE;

			if( NULL==pmsg )
				return 0;

			memcpy( pmsg, pMsg, sizeof(MODEL_MESSAGE) );

			if( g_StudentWorkerList[i].mutex_msg_to_student_list.Lock() )
			{
				g_StudentWorkerList[i].msg_to_student_list.AddTail( pmsg );

				g_StudentWorkerList[i].mutex_msg_to_student_list.Unlock();
			}
		}
	}

	return 1;
}

int ProcessTTSReset()
{
	MODEL_MESSAGE	*pmsg = new MODEL_MESSAGE;
	
	if( NULL==pmsg )
		return 0;

	memset(pmsg, 0, sizeof(MODEL_MESSAGE));

	pmsg->student		= 255;				//255表示向所有学员机发送
	pmsg->weather		= -1;
	pmsg->wind			= -1;			
	pmsg->rank			= 0;
	pmsg->unit			= 0;
	pmsg->phase			= 0;
	pmsg->device_index	= 0;
	pmsg->scale			= 0;
	pmsg->pos[0]		= 0;
	pmsg->pos[1]		= 0;
	pmsg->pos[2]		= 0;
	pmsg->pos[3]		= 0;
	sprintf(pmsg->model, "RESET_" );		
	
	for(int i=0; i<STUDENT_NUM; i++ )
	{
		if( g_StudentWorkerList[i].m_bConnected )
		{
			MODEL_MESSAGE	*msg = new MODEL_MESSAGE;

			if( msg!=NULL && g_StudentWorkerList[i].mutex_msg_to_student_list.Lock() )
			{
				memcpy( msg, pmsg, sizeof(MODEL_MESSAGE) );

				g_StudentWorkerList[i].msg_to_student_list.AddTail( msg );

				g_StudentWorkerList[i].mutex_msg_to_student_list.Unlock();
			}
		}
	}

	delete pmsg;

	return 1;
}

void WriteLog(char *valuename, char *operate)
{
	char  m_user[64];
	memset( m_user, 0, sizeof(m_user) );
	GetUserName(m_user);
	CString strUser( m_user );

	time_t  v;
	time( &v );
	int type = 31;

	if (GetFldValue("ps","timeint",(char*)&v))
	{
		v /= 1000;
		int hour, minute, second;
		hour = v/3600;
		minute = (v - hour*3600)/60;
		second = v - hour*3600 - minute*60;

		CTime t_now = CTime::GetCurrentTime();
		CString date=t_now.Format("%Y-%m-%d");
		CString time_int;
		time_int.Format( "%02d:%02d:%02d", hour, minute, second );

		int record=AddOneRecord("SCT","OPR");

		if (record>0)
		{
			UpdateFldValue(record,"SCT","OPR","date",(LPSTR)(LPCTSTR)date);
			UpdateFldValue(record,"SCT","OPR","time",(LPSTR)(LPCTSTR)time_int);
			UpdateFldValue(record,"SCT","OPR","type",(char *)&type);
			UpdateFldValue(record,"SCT","OPR","operator",(LPSTR)(LPCTSTR)strUser);
			UpdateFldValue(record,"SCT","OPR","varname",valuename);
			UpdateFldValue(record,"SCT","OPR","desc",operate);
		}
		else
			AfxMessageBox("SCT OPR 记录增加不成功");

	}
	else
		AfxMessageBox("时间得到不成功");
}

int  ProcessStudentData( COMM_MSG *pMsg, int nWorkerIndex )
{	
	list<string>::iterator itor;
	string strID( pMsg->id );

	if( strID.length()>0 )
	{
		if( strcmp(pMsg->id, "RESET_")==0 )
		{
			ResetStudent( nWorkerIndex );

			return 0;
		}

		/*itor = find( DI_LIST.begin(), DI_LIST.end(), strID );

		if( itor!=DI_LIST.end() )
		{
			int state = pMsg->state;

			//if( !UpdateFldValue("SCT","DI","ID",pMsg->id,"STATE",(char *)&state) )
			if( !UpdateFldValue( DI_INDEX[pMsg->id], "SCT","DI","STATE",(char *)&state) )
			{
				cerr<<"Err UpdateFldValue STATE "<<pMsg->id<<endl;
			}
		}*/
		itor = find( EX_LIST.begin(), EX_LIST.end(), strID );

		if( itor!=EX_LIST.end() )
		{
			int state = pMsg->state;
			CString str;
			//if( !UpdateFldValue("SCT","DI","ID",pMsg->id,"STATE",(char *)&state) )
			if( !UpdateFldValue( EX_INDEX[pMsg->id], "SCT","EX","VALUE",(char *)&state) )
			{				
				str.Format("Err UpdateFldValue STATE, EX-%s", pMsg->id );
				InsertErrLog((char *)(LPCTSTR)str, EVENTLOG_ERROR_TYPE);
			}
			else
			{
				CString str2="", strName("SETVALUE");
				float	f2=0, fParam=(float)state;
				int		i1=0, i2=0;
				
				str = "";
				str.Format( "%s", pMsg->id );
				/*int nPos = str.ReverseFind( '_' );
				if( nPos>0 )
				{
					str2 = str.Right( str.GetLength()-nPos-1 );
					sscanf( (LPCTSTR)str2, "%f", &fParam );
					str = str.Left( nPos );	
					str2 = ""; 
				}*/

				int record=AddOneRecord("SCT","COMMAND");

				if (record>0)
				{
					UpdateFldValue(record,"SCT","COMMAND","NAME",(LPSTR)(LPCTSTR)strName);
					UpdateFldValue(record,"SCT","COMMAND","CPARA",(LPSTR)(LPCTSTR)str);
					UpdateFldValue(record,"SCT","COMMAND","FPARA",(char *)&fParam);
					UpdateFldValue(record,"SCT","COMMAND","IPARA",(char *)&i1);
					UpdateFldValue(record,"SCT","COMMAND","CPARA2",(LPSTR)(LPCTSTR)str2);
					UpdateFldValue(record,"SCT","COMMAND","FPARA2",(char *)&f2);
					UpdateFldValue(record,"SCT","COMMAND","IPARA2",(char *)&i2);

					str2.Format("Student->Setting, EX-%s VALUE-%2.0f", str, fParam );
					InsertErrLog((char *)(LPCTSTR)str2, EVENTLOG_ERROR_TYPE);
				}
			}
		}
		/*else
		{
			itor = find( AI_LIST.begin(), AI_LIST.end(), strID );
			
			if( itor!=AI_LIST.end() )
			{
				float value = pMsg->value;
				//if( !UpdateFldValue("SCT","AI","ID",pMsg->id,"VALUE",(char *)&value) )
				if( !UpdateFldValue( AI_INDEX[pMsg->id], "SCT","AI","VALUE",(char *)&value) )
				{
					cerr<<"Err UpdateFldValue VALUE "<<pMsg->id<<endl;
				}
			}
			else
			{
				itor = find( ZYCB_LIST.begin(), ZYCB_LIST.end(), strID );
			
				if( itor!=ZYCB_LIST.end() )
				{
					int iopen;

					if( pMsg->state )
						iopen = 0;
					else
						iopen = 1;
					
					if( !UpdateInFldValue( ZYCB_INDEX[pMsg->id], "ps","zycb","iopen",(char *)&iopen) )
					{
						cerr<<"Err UpdateFldValue ZYCB "<<pMsg->id<<endl;
					}
				}
				else
				{
					itor = find( ZLLN_LIST.begin(), ZLLN_LIST.end(), strID );

					if( itor!=ZLLN_LIST.end() )
					{
						int iopen;

						if( pMsg->state )
							iopen = 0;
						else
							iopen = 1;

						if( !UpdateInFldValue( ZLLN_INDEX[pMsg->id], "ps","zlln","iopen",(char *)&iopen) )
						{
							cerr<<"Err UpdateFldValue ZLLN "<<pMsg->id<<endl;
						}
					}
					else
					{
						itor = find( GUADIXIAN_LIST.begin(), GUADIXIAN_LIST.end(), strID );

						if( itor!=GUADIXIAN_LIST.end() )
						{
							int state = pMsg->state;

							if( !UpdateFldValue( GUADIXIAN_INDEX[pMsg->id], "SCT","GUADX","STATE",(char *)&state) )
							{
								cerr<<"Err UpdateFldValue STATE "<<pMsg->id<<endl;
							}
						}
						else
						{		
							itor = find( PSXF_LIST.begin(), PSXF_LIST.end(), strID );

							if( itor!=PSXF_LIST.end() )
							{
								int state = pMsg->state;

								if( !UpdateInFldValue( ZYCB_INDEX[pMsg->id], "ps","xf","iclose1",(char *)&state) )
								{
									cerr<<"Err UpdateFldValue PSXF "<<pMsg->id<<endl;
								}
							}
						}						
					}
				}				
			}
		}*/
	}

	if( strlen(pMsg->opr_msg)>0 )
	{
		WriteLog( pMsg->id, pMsg->opr_msg );
	}

	return 0;
}

UINT TimerThread(LPVOID lParam)
{
	HANDLE	hDataWait[2];

	hDataWait[0] = g_hShutDownEvent;
	hDataWait[1] = g_hTimerEvent;

	while (1)
	{
		const DWORD res = 
			::WaitForMultipleObjects(2, hDataWait, FALSE, INFINITE);

		switch (res)
		{
			case WAIT_FAILED: // something wrong!
				return 1;
			
			case WAIT_OBJECT_0: // Shut down!
				return 0;
			
			case (WAIT_OBJECT_0 + 1): // timer is signaled
			{
				struct tm	*pTm;
				time(&g_Time);
				pTm = localtime(&g_Time);
				memcpy(&g_Tm, pTm, sizeof(struct tm));

				ResetEvent(g_hTimerEvent);
			}
			break;
			
			default:
				ASSERT(0);
				break;
		}
	} // while 1
	return 0;
}
