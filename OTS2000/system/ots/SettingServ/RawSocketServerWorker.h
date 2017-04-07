
#ifndef __CWizRawSocketServerWorker_H
#define __CWizRawSocketServerWorker_H

#include "ThreadDispatcher.h"
#include "RawSocket.h"
/////////////////////////////////////////////////////////////////////////////
// class CWizRawSocketServerWorker
#include "ThreadDispatcher.h"
#include "critsect.h"

// Events to interrupt blocking socket functions.
__declspec( thread ) static const int HOOK_CANCEL_EVENTS = 2;
__declspec( thread ) static HANDLE sh_HookCanselEvents[HOOK_CANCEL_EVENTS] 
	= { NULL, NULL };
__declspec( thread ) static int nHookEventsInstalled = 0;


class CWizRawSocketListener : public CWizMultiThreadedWorker
{
public:
	// Exceptions hierarchy
	struct Xeption {}; // common
	struct XCannotCreate : public Xeption {};  // createsocket fail
	struct XCannotSetHook : public Xeption {}; // WSASetBlockingHook fail
	struct XCannotSetHookEvent : public Xeption {}; // SetEven fail
	struct XCannotListen : public Xeption {};		// Listen fail

// Constructors:
	CWizRawSocketListener (int nPort); // Constructor do almost nothing
// Destructor:
	virtual ~CWizRawSocketListener ();

public:
// Operations:
// Virtual operations:
	// Interface for CWizThreadDispatcher
	virtual void Prepare();
	virtual BOOL WaitForData(HANDLE hShutDownEvent);
	virtual BOOL TreatData(HANDLE hShutDownEvent, HANDLE hDataTakenEvent) = 0;
	virtual void CleanUp();
	// Pure virtual function - do something
	// with the socket connected to the client.
	// Should return TRUE if needs to continue I/O.
	virtual int ReadWrite(CWizReadWriteSocket& socket, int nWorkerIndex) = 0;
protected:

// Implementation:
// Virtual implementation:
protected:
// Data Members:
	CWizReadWriteSocket *m_pListenSocket;
	SOCKET				m_hAcceptedSocket;
	int			m_nPort;
};

class Worker : public CWizRawSocketListener
{
public:
	int		m_nStudent;
	int		m_nType;		//0--connected with student,1--connected with tts
	int		m_nSendSequence;	//记录本socket上次发送的广播包的序列号
	Worker(int nPort, int nType);
	~Worker();
	virtual BOOL TreatData(HANDLE hShutDownEvent, HANDLE hDataTakenEvent);
	virtual int	 ReadWrite(CWizReadWriteSocket& Socket, int nWorkerIndex);
};

typedef struct{
	int		m_nStudent;		//学员登录时输入的学号
	bool	m_bConnected;	//是否和学员建立了连接
	
	CMutex	mutex_msg_to_student_list;
	CPtrList msg_to_student_list;
	CPtrList comm_to_student_list;
}WORKER_INFO;

//*****************************************************************
inline BOOL SetHook();

struct EventInstall
{
	EventInstall (HANDLE h, int &n)
		: m_n (n)
	{
		ASSERT(nHookEventsInstalled <= m_n);
		
		if(nHookEventsInstalled > m_n)
			throw CWizRawSocketListener::XCannotSetHookEvent();
		
		sh_HookCanselEvents[m_n] = h;
		nHookEventsInstalled = m_n + 1;
		n++;
	}

	~EventInstall ()
	{
		ASSERT(nHookEventsInstalled == m_n + 1);
		sh_HookCanselEvents [m_n] = NULL;
		nHookEventsInstalled = m_n;
	}
	
	int m_n;
};
/////////////////////////////////////////////////////////////////////////////
#endif // __CWizRawSocketServerWorker_H


