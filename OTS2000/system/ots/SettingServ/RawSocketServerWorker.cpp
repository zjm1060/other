#include "stdafx.h"
#include "RawSocketServerWorker.h"
#include "SimComm_Teacher.h"
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

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern WORKER_INFO		g_StudentWorkerList[STUDENT_NUM];
extern int				g_nStudentConnected, g_nTeacherConnected;

extern HANDLE		g_hShutDownEvent;
extern list<string> DO_LIST;
extern map<string, int> DO_STATE;

extern list<string> AO_LIST;
extern map<string, float> AO_VALUE;

extern list<string> AI_LIST;
extern list<string> DI_LIST;
extern list<string> EX_LIST;
extern map<string, int> DO_INDEX;
extern map<string, int> AO_INDEX;
extern map<string, int> DI_INDEX;
extern map<string, int> AI_INDEX;
extern map<string, int> EX_STATE;

void InsertErrLog(char *err,WORD nType);
int	ProcessMsgData( COMM_MSG *msg, int nIndex=-1 );

///////////////////////////////////////////////////////////////////
inline void ThrowIfNull(void* p)
{
	if (p == NULL)
		AfxThrowMemoryException();
}

// Function called from Windows Sockets blocking hook function
inline BOOL TestCancelEvents (DWORD TimeOut = 0)
{

#ifdef _DEBUG
	ASSERT(nHookEventsInstalled <= HOOK_CANCEL_EVENTS);
	for(INDEX i = 0; i < nHookEventsInstalled; i++)
		ASSERT(sh_HookCanselEvents[i] != NULL);
#endif
	// Tests events - if one signaled, should cancel blocking function.
	return (::WaitForMultipleObjects(nHookEventsInstalled, sh_HookCanselEvents, FALSE, TimeOut) != WAIT_TIMEOUT);
}
//*****************************************************************
// Windows Sockets blocking hook function
BOOL WINAPI BlockingHook(void) 
{
	// As simple as...
	if (::TestCancelEvents(0))
		WSACancelBlockingCall();
	return 0;
}
//*****************************************************************
// Function installes blocking hook handler
inline BOOL SetHook()
{
	static BOOL bInstalled = FALSE;
	// If not installed yet
	if (!bInstalled)
	{
		// Try to install hook even if some
		// blocking Windows Sockets operation is in progress.
		while (::WSASetBlockingHook(BlockingHook) == NULL)
		{
			if (::WSAGetLastError() != WSAEINPROGRESS)
			{
				return FALSE;
			}
			if(::TestCancelEvents(100))
				return FALSE;
		}
		bInstalled = TRUE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////
// class CWizRawSocketListener
///////////////////////////////////////////////////////////////////
// Default Constructor										
CWizRawSocketListener::CWizRawSocketListener(int nPort)
	: m_nPort         (nPort)
{	
	m_pListenSocket = NULL;
}																

// Destructor												
CWizRawSocketListener::~CWizRawSocketListener()				
{															
	if (m_pListenSocket != NULL)							
	{													
		//ASSERT(0);										
		delete m_pListenSocket;	
		m_pListenSocket = NULL;
	}

}	

// Method called from dispath thread.
void CWizRawSocketListener::Prepare()
{
	// Maybe install hook
	if(!SetHook())
		throw CWizRawSocketListener::XCannotSetHook();
	
	ASSERT(nHookEventsInstalled == 0);
	
	// Create listening socket
	if (m_pListenSocket != NULL)
	{
		m_pListenSocket->Close();

		delete m_pListenSocket;

		m_pListenSocket = NULL;
	}
	
	m_pListenSocket = new CWizReadWriteSocket(m_nPort,SOCK_STREAM);

	ThrowIfNull(m_pListenSocket);
}

// Method called from dispath thread.
void CWizRawSocketListener::CleanUp()
{
	if(m_pListenSocket)
	{
		m_pListenSocket->Close();

		delete m_pListenSocket;

		m_pListenSocket = NULL;
	}
}

// Method called from dispath thread.
BOOL CWizRawSocketListener::WaitForData(HANDLE hShutDownEvent)
{
	// Install shutdown event.
	int n = 0;
	SOCKADDR	sa;

	EventInstall ei(hShutDownEvent, n);
	
	// Maybe set hook.
	if(!SetHook())
		throw CWizRawSocketListener::XCannotSetHook();
	
	while (1)
	{
		// Shutdown?
		if (::WaitForSingleObject(hShutDownEvent,0) != WAIT_TIMEOUT)
			return FALSE;

		// Get accepted socket.
		SOCKET h = m_pListenSocket->Accept(&sa);			

		if (h != INVALID_SOCKET)
		{
			m_hAcceptedSocket = h;
			return TRUE;		
		}
	} // while 1
	
	return TRUE;
}

//*****************************************************************
Worker::Worker(int nPort, int nType)
		: CWizRawSocketListener(nPort)
{
	m_nType = nType;
}

Worker::~Worker()
{
}

void ResetStudent( int nWorkerIndex )
{
	list<string>::iterator itor;
	int state;
	float value;

	if(g_StudentWorkerList[nWorkerIndex].mutex_msg_to_student_list.Lock())
	{
		while(!g_StudentWorkerList[nWorkerIndex].msg_to_student_list.IsEmpty())
		{
			MODEL_MESSAGE *pmsg = (MODEL_MESSAGE *)g_StudentWorkerList[nWorkerIndex].msg_to_student_list.RemoveHead();
					
			delete pmsg;
		}

		g_StudentWorkerList[nWorkerIndex].mutex_msg_to_student_list.Unlock();
	}	
	
	for( itor=DO_LIST.begin(); itor!=DO_LIST.end(); itor++ )
	{
		//if( !GetFldValue("SCT","DO","ID",(*itor).c_str(),"STATE",(char *)&state) )
		if( !GetFldValue( DO_INDEX[(*itor).c_str()], "SCT","DO", "STATE",(char *)&state) )
			cerr<<"Err GetFldValue STATE "<<(*itor).c_str()<<endl;
		else
		{
			DO_STATE[(*itor)] = state;

			COMM_MSG	*comm_msg;
			comm_msg = new COMM_MSG;
			memset( comm_msg, 0, sizeof(COMM_MSG) );
			comm_msg->student = 254;
			strcat( comm_msg->id, (*itor).c_str() );
			comm_msg->state = state;
			comm_msg->value = state;

			ProcessMsgData( comm_msg, nWorkerIndex );

			delete comm_msg;
		}
	}

	for( itor=AO_LIST.begin(); itor!=AO_LIST.end(); itor++ )
	{
		//if( !GetFldValue("SCT","AO","ID",(*itor).c_str(),"VALUE",(char *)&value) )
		if( !GetFldValue( AO_INDEX[(*itor).c_str()], "SCT","AO","VALUE",(char *)&value) )
			cerr<<"Err GetFldValue VALUE "<<(*itor).c_str()<<endl;
		else
		{
			AO_VALUE[(*itor)] = value;

			COMM_MSG	*comm_msg;
			comm_msg = new COMM_MSG;
			memset( comm_msg, 0, sizeof(COMM_MSG) );
			comm_msg->student = 254;
			strcat( comm_msg->id, (*itor).c_str() );
			comm_msg->state = value;
			comm_msg->value = value;

			ProcessMsgData( comm_msg, nWorkerIndex );

			delete comm_msg;
		}
	}	
}

BOOL Worker::TreatData(HANDLE hShutDownEvent, HANDLE hDataTakenEvent)
{
	int	nWorkerIndex=0, n=0;
	int err = 0;
	
	// Install shutdown event.
	EventInstall ei( hShutDownEvent, n );
	
	// Maybe set hook.
	if(!SetHook())
		throw CWizRawSocketListener::XCannotSetHook();

	if( CONNECT_WITH_TEACHER==m_nType )
	{
		g_nTeacherConnected++;
	}
	else
	{
		g_nStudentConnected++;

		//找到一个空闲的WORKER_INFO结构来保存本次连接的信息
		for( nWorkerIndex=0; nWorkerIndex<STUDENT_NUM; nWorkerIndex++ )
		{
			if( !g_StudentWorkerList[nWorkerIndex].m_bConnected )
				break;
		}

		if( nWorkerIndex>=STUDENT_NUM )
		{
			cerr<<"Too Much Students Connections!"<<endl;
			return -4;
		}
		else
		{
			cout<<"Student "<<nWorkerIndex<<" Connected!"<<endl;
		}

		if(g_StudentWorkerList[nWorkerIndex].mutex_msg_to_student_list.Lock())
		{
			g_StudentWorkerList[nWorkerIndex].m_bConnected = true;
			
			g_StudentWorkerList[nWorkerIndex].m_nStudent = -1;

			g_StudentWorkerList[nWorkerIndex].mutex_msg_to_student_list.Unlock();	
		}
		
		ResetStudent( nWorkerIndex );
	}

	CWizReadWriteSocket ClientSide( m_hAcceptedSocket );

	// Signal dispather to continue waiting.
	::SetEvent(hDataTakenEvent);

	u_long fionbio = 1;
	ioctlsocket( m_hAcceptedSocket , FIONBIO, &fionbio);

	// Exchange with client. 
	for( ; err>=0 ; )
	{
		// Shutdown?
		if(::WaitForSingleObject(hShutDownEvent,0)==WAIT_OBJECT_0)
		{
			ClientSide.Close();
			return FALSE;
		}

		err = ReadWrite( ClientSide, nWorkerIndex );
	}

	switch(err)
	{
	case -1:
		InsertErrLog("Error Receive Student Data", EVENTLOG_ERROR_TYPE);
		break;
	case -2:
		InsertErrLog("Error Send Data to Student", EVENTLOG_ERROR_TYPE);
		break;
	case -3:
		InsertErrLog("Error Receive Teacher Data", EVENTLOG_ERROR_TYPE);
		break;
	default:
		break;
	}

	ClientSide.Close();

	if( CONNECT_WITH_TEACHER==m_nType )
	{
		g_nTeacherConnected--;
	}
	else
	{
		g_nStudentConnected--;

		if(g_StudentWorkerList[nWorkerIndex].mutex_msg_to_student_list.Lock())
		{
			while(!g_StudentWorkerList[nWorkerIndex].msg_to_student_list.IsEmpty())
			{
				MODEL_MESSAGE *pmsg = (MODEL_MESSAGE *)g_StudentWorkerList[nWorkerIndex].msg_to_student_list.RemoveHead();
						
				delete pmsg;
			}

			g_StudentWorkerList[nWorkerIndex].m_nStudent = -1;

			g_StudentWorkerList[nWorkerIndex].m_bConnected = false;

			g_StudentWorkerList[nWorkerIndex].mutex_msg_to_student_list.Unlock();
		}	
		
		cout<<"Student "<<nWorkerIndex<<" DisConnected!"<<endl;
	}

	return TRUE;
}

int Worker::ReadWrite(CWizReadWriteSocket& Socket, int nWorkerIndex)
{
	int result_length=0, err=0;

	if( CONNECT_WITH_STUDENT==m_nType )
	{
		POSITION		pos,pos2;

		//如果有数据要发，就先发数据
		if(!g_StudentWorkerList[nWorkerIndex].comm_to_student_list.IsEmpty())
		{
			int num=0;

			for( pos = g_StudentWorkerList[nWorkerIndex].comm_to_student_list.GetHeadPosition(); pos != NULL; num++ )
			{
				pos2 = pos;
				
				COMM_MSG	*pmsg;

				pmsg = (COMM_MSG *)g_StudentWorkerList[nWorkerIndex].comm_to_student_list.GetNext(pos);			
		
				err = Socket.Write( (char *)pmsg, sizeof(COMM_MSG), 1 );
					
				if( sizeof(COMM_MSG)==err )
				{
					if(g_StudentWorkerList[nWorkerIndex].mutex_msg_to_student_list.Lock())
					{
						g_StudentWorkerList[nWorkerIndex].comm_to_student_list.RemoveAt(pos2);
						
						g_StudentWorkerList[nWorkerIndex].mutex_msg_to_student_list.Unlock();	
					}

					delete pmsg;		
				}
				else
				{
					return -2;
				}
			}

			cout<<"Send "<<num<<endl;
		}

		///////////////////////////////////////////////////////
		//收学员机发来的数据
		COMM_MSG msg;	

		memset( &msg, 0, sizeof(COMM_MSG));

		u_long fionbio = 1;
		ioctlsocket( Socket.m_hSocket, FIONBIO, &fionbio);

		result_length = Socket.Recv((char *)&msg, sizeof(COMM_MSG), 0 );

		if( sizeof(COMM_MSG) == result_length )
		{
			g_StudentWorkerList[nWorkerIndex].m_nStudent = msg.student;

			CString str;
			str.Format("Student->Setting, MODEL-%s", msg.id );
			InsertErrLog((char *)(LPCTSTR)str, EVENTLOG_ERROR_TYPE);

			ProcessStudentData( &msg, nWorkerIndex );
		}
		else	//超时是-1,断开是0,出错是-2
		if( 0==result_length || -2==result_length )
		{					
			/*INT iError = ::WSAGetLastError();
			if( iError == WSAENETRESET || iError == WSAECONNABORTED ||
				iError == WSAECONNRESET || iError == WSAEINVAL || iError == WSAECONNRESET )*/
			{
				return -1;
			}
		}
	}
	else
	if( CONNECT_WITH_TEACHER==m_nType )
	{	
		///////////////////////////////////////////////////////////////////
		//收数据
		MODEL_MESSAGE	msg_from_teacher;
		memset(&msg_from_teacher, 0, sizeof(msg_from_teacher));

		u_long fionbio = 1;
		ioctlsocket( Socket.m_hSocket, FIONBIO, &fionbio);

		err = Socket.Recv( (char *)&msg_from_teacher, sizeof(msg_from_teacher), 0 );
		
		if(err == sizeof(MODEL_MESSAGE) )
		{
			CString str;
			str.Format("Teacher->Setting, MODEL-%s", (char *)(msg_from_teacher.model));
			InsertErrLog((char *)(LPCTSTR)str, EVENTLOG_ERROR_TYPE);

			ProcessTeacherData( &msg_from_teacher );	

			//教师机每次设置异常都重新连接，每次处理完就退出
			return 0;
		}
		else	//超时是-1,断开是0,出错是-2
		if( 0==err || -2==err )
		{					
			/*INT iError = ::WSAGetLastError();
			if( iError == WSAENETRESET || iError == WSAECONNABORTED ||
				iError == WSAECONNRESET || iError == WSAEINVAL || iError == WSAECONNRESET )*/
			{
				return -3;
			}
		}
	}

	Sleep(10);

	return 0;
}