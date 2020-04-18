#include "DelegateOpt.h"
#if USE_WIN32_THREADS

#include "ThreadWin.h"
#include "UserMsgs.h"
#include "ThreadMsg.h"
#include "Fault.h"

HANDLE ThreadWin::m_hStartAllThreads = INVALID_HANDLE_VALUE;
ThreadWin* ThreadWin::m_allocatedStartAllThreads = NULL;

struct ThreadParam 
{
	ThreadWin* pThread;
};

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
ThreadWin::ThreadWin (const CHAR* threadName, BOOL syncStart) :	
	THREAD_NAME(threadName),
	SYNC_START(syncStart),
	m_hThreadStarted(INVALID_HANDLE_VALUE),
	m_hThreadExited(INVALID_HANDLE_VALUE),
	m_hThread(INVALID_HANDLE_VALUE),
	m_threadId(0)
{
	if (m_hStartAllThreads == INVALID_HANDLE_VALUE)
	{
		m_hStartAllThreads = CreateEvent( NULL, TRUE, FALSE, TEXT("StartAllThreadsEvent"));
		m_allocatedStartAllThreads = this;
	}
}

//----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------
ThreadWin::~ThreadWin()
{
	if (m_allocatedStartAllThreads == this && m_hStartAllThreads != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hStartAllThreads);
		m_hStartAllThreads = INVALID_HANDLE_VALUE;
	}

	if (m_hThread != INVALID_HANDLE_VALUE) 
		ExitThread();
}

//----------------------------------------------------------------------------
// PostThreadMessage
//----------------------------------------------------------------------------
void ThreadWin::PostThreadMessage(UINT msg, void* data)
{
	BOOL success = ::PostThreadMessage(GetThreadId(), msg, (WPARAM)data, 0);
	ASSERT_TRUE(success != 0);
}

//----------------------------------------------------------------------------
// DispatchDelegate
//----------------------------------------------------------------------------
void ThreadWin::DispatchDelegate(DelegateLib::DelegateMsgBase* msg)
{
	// Create a new ThreadMsg
	ThreadMsg* threadMsg = new ThreadMsg(WM_DISPATCH_DELEGATE, msg);

	// Post the message to the this thread's message queue
	PostThreadMessage(WM_DISPATCH_DELEGATE, threadMsg);
}

//----------------------------------------------------------------------------
// CreateThread
//----------------------------------------------------------------------------
BOOL ThreadWin::CreateThread()
{
	// Is the thread already created?
	if (!IsCreated ()) 
	{
		m_hThreadStarted = CreateEvent(NULL, TRUE, FALSE, TEXT("ThreadCreatedEvent"));	

		// Create the worker thread
		ThreadParam threadParam;
		threadParam.pThread = this;
		m_hThread = ::CreateThread (NULL, 0, (unsigned long (__stdcall *)(void *))RunProcess, (void *)(&threadParam), 0, &m_threadId);
		ASSERT_TRUE(m_hThread != NULL);

		// Block the thread until thread is fully initialized including message queue
		DWORD err = WaitForSingleObject(m_hThreadStarted, MAX_WAIT_TIME);	
		ASSERT_TRUE(err == WAIT_OBJECT_0);

		CloseHandle(m_hThreadStarted);
		m_hThreadStarted = INVALID_HANDLE_VALUE;

		return m_hThread ? TRUE : FALSE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
// ExitThread
//----------------------------------------------------------------------------
void ThreadWin::ExitThread()
{
	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		m_hThreadExited = CreateEvent(NULL, TRUE, FALSE, TEXT("ThreadExitedEvent"));	

		PostThreadMessage(WM_EXIT_THREAD);

		// Wait here for the thread to exit
		if (::WaitForSingleObject (m_hThreadExited, MAX_WAIT_TIME) == WAIT_TIMEOUT)
			::TerminateThread (m_hThread, 1);

		::CloseHandle (m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;

		::CloseHandle (m_hThreadExited);
		m_hThreadExited = INVALID_HANDLE_VALUE;
	}
}

//----------------------------------------------------------------------------
// RunProcess
//----------------------------------------------------------------------------
int ThreadWin::RunProcess(void* threadParam)
{
	// Extract the ThreadWin pointer from ThreadParam.
	ThreadWin* thread;
	thread = (ThreadWin*)(static_cast<ThreadParam*>(threadParam))->pThread;

	// Force the system to create the message queue before setting the event below.
	// This prevents a situation where another thread calls PostThreadMessage to post
	// a message before this thread message queue is created.
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	// Thread now fully initialized. Set the thread started event.
	BOOL err = SetEvent(thread->m_hThreadStarted);
	ASSERT_TRUE(err != 0);

	// Using a synchronized start?
	if (thread->SYNC_START == TRUE)
	{
		// Block the thread here until all other threads are ready. A call to 
		// StartAllThreads() releases all the threads at the same time.
		DWORD err = WaitForSingleObject(m_hStartAllThreads, MAX_WAIT_TIME);
		ASSERT_TRUE(err == WAIT_OBJECT_0);
	}

	// Call the derived class Process() function to implement the thread loop.
	int retVal = thread->Process(NULL);

	// Thread loop exited. Set exit event. 
	err = SetEvent(thread->m_hThreadExited);
	ASSERT_TRUE(err != 0);	

	return retVal;
}

//----------------------------------------------------------------------------
// StartAllThreads
//----------------------------------------------------------------------------
void ThreadWin::StartAllThreads()
{
	BOOL err = SetEvent(m_hStartAllThreads);
	ASSERT_TRUE(err != 0);
}

#endif

