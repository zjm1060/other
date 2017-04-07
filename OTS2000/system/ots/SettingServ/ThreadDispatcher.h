#include "RawSocket.h"

#ifndef __CWizThreadDispatcher_H
#define __CWizThreadDispatcher_H

class CWizMultiThreadedWorker
{
	public:
		CWizMultiThreadedWorker() {}
		virtual ~CWizMultiThreadedWorker() {}

		virtual void Prepare     () = 0;
		virtual BOOL WaitForData (HANDLE hShutDownEvent) = 0;
		virtual BOOL TreatData   (HANDLE hShutDownEvent, HANDLE hDataTakenEvent) = 0;
		virtual void CleanUp     () =  0;

	class Stack
		{
		public:
			Stack(CWizMultiThreadedWorker& rW)
				: m_rW(rW)
				{ m_rW.Prepare(); }
			~Stack()
				{ m_rW.CleanUp(); }
		private:
			CWizMultiThreadedWorker& m_rW;
		};
};
/////////////////////////////////////////////////////////////////////////////
// class CWizThreadDispatcher
class CWizThreadDispatcher
{
public:
// Constructors:
	CWizThreadDispatcher (CWizMultiThreadedWorker &rWorker, int MaxThreads = 5);
// Destructor:
	virtual ~CWizThreadDispatcher ();

public:
// Operations:
	void	Start();					// Starts Dispatching
	void	Stop(BOOL bWait = TRUE);	// Signals to stop and (?) waits for all threads to exit
										// posts Message to pWnd and exits
	void	WaitShutDown();				// Waits for all threads to exit after signaling to stop

public:
// Classes 
	// Exceptions the class can throw: (+CMemoryException)
	struct Xeption {};
	struct XCannotCreate : public Xeption {};
	struct XWaitFailed : public Xeption {};
	// Event wrapper
	class Event
	{
	public:
		Event(BOOL bManualReset);
		~Event();
		void	Set();
		HANDLE	m_h;
	};
protected:
	class WorkerThread
	{
	public:
		WorkerThread(CWizMultiThreadedWorker &rWorker, 
				HANDLE hDataReadyEvent,
				HANDLE hStartedTreatEvent,
				HANDLE hShutDownEvent);
		friend UINT AFX_CDECL CWizThreadDispatcher_WorkerThread_Fun(LPVOID pParam);
	private:
		UINT Run();

		enum { DataWaitHCount = 2 };
		CWizMultiThreadedWorker&	m_rWorker;
		HANDLE	m_hDataWait [DataWaitHCount];
		HANDLE	m_hStartedTreatEvent;
	};
// Implementation:
	friend UINT AFX_CDECL CWizThreadDispatcherFun(LPVOID pParam);
	friend UINT AFX_CDECL CWizThreadDispatcher_WorkerThread_Fun(LPVOID pParam);

// Virtual implementation:
	virtual UINT  Run();
// Data members
	enum { StartedTreatmentEventsCount = 2};
public:
	int							m_nMaxThreads;
	CWizMultiThreadedWorker&	m_rWorker;
	Event						m_ShutDownEvent;
	Event						m_HasDataEvent;
	Event						m_StartedDataTreatEvent;
	HANDLE						m_ahStartedTreatmentEvents[StartedTreatmentEventsCount];
	HANDLE*						m_ahWorkersHandles;
	HANDLE						m_hThisThread;
};

/////////////////////////////////////////////////////////////////////////////
inline CWizThreadDispatcher::Event::Event(BOOL bManualReset)
	: m_h (::CreateEvent(NULL, bManualReset, FALSE, NULL))
{
	if (m_h == NULL)
		throw CWizThreadDispatcher::XCannotCreate();
}

inline CWizThreadDispatcher::Event::~Event()
{
	if (m_h != NULL)
		VERIFY(::CloseHandle(m_h));
}

inline void CWizThreadDispatcher::Event::Set()
{
	ASSERT(m_h != NULL);
	SetEvent(m_h); // May fail because is set already
}

///////////////////////////////////////////////////////////////////
#endif // __CWizThreadDispatcher_H


