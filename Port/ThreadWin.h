#ifndef _THREAD_WIN_H
#define _THREAD_WIN_H

#if USE_WIN32_THREADS

#include "DelegateLib.h"
#include "DataTypes.h"

// See http://www.codeproject.com/Articles/1095196/Win32-Thread-Wrapper-with-Synchronized-Start

/// @brief ThreadWin is class to ease the creation of worker threads. The underlying 
///	threads are created with the Win32 CreateThread() API.
///
/// @details Inherit from ThreadWin and override Process() in the derived class.
/// Process() is the thread entry function. 
///
/// In addition to encapsulating thread creation and destruction, ThreadWin has 
/// these main features.
///
/// 1) Synchronized start - all the threads can be created, then block waiting for
/// StartAllThreads() to release all simultaneously. 
///
/// 2) Forced queue creation - the thread message queue is created before the thread
/// is run. If a thread is created and a message posted too early before the queue 
/// is created, the message is lost. This feature solves that problem. 
///
/// 3) Inherit from DelegateThread and implement DispatchCallback() to post callback 
/// messages into the thread queue. 
///
/// 4) Thread exit - an orderly exit and cleanup of the thread. 
class ThreadWin : public DelegateLib::DelegateThread
{
public:
	/// Constructor
	/// @param[in] threadName - a user defined thread name.	
	/// @param[in] syncStart - TRUE to synchronize the threads to start at the same  
	///		time after StartAllThreads() is called. FALSE thread starts immediately. 
	ThreadWin (const CHAR* threadName, BOOL syncStart = TRUE);

	/// Destructor
	virtual ~ThreadWin ();

	/// Called once to create the worker thread
	/// @return TRUE if thread is created. FALSE otherise. 
	BOOL CreateThread();

	/// Called once a program exit to exit the worker thread
	void ExitThread();

	/// Get the status of the thread creation.
	/// @return Returns TRUE if thread is already created; FALSE otherwise.
	bool IsCreated() { return (m_hThread != INVALID_HANDLE_VALUE); }

	/// Get the Windows thread handle
	/// @return The thread handle.
	HANDLE GetThreadHandle() {	return m_hThread; }

	/// Get the Windows thread ID
	/// @return The thread ID.
	DWORD GetThreadId() { return m_threadId; }

	/// Get the current executing thread ID
	/// @return The thread ID of the currently executing thread.
	static DWORD GetCurrentThreadId() { return ::GetCurrentThreadId(); }

	/// Get the thread name.
	/// @return The thread name.
	const CHAR* GetThreadName() { return THREAD_NAME; }

	/// Post a message with optional dynamic data to the thread message queue.
	/// @param[in] msg - a message ID with a value greater than WM_USER.
	/// @param[in] data - optional data pointer. The caller typically 
	///		creates the data pointer on the heap, and the thread deletes
	///		the data when the message is handled. 
	void PostThreadMessage(UINT msg, void* data = NULL);

	/// Releases all waiting threads to allow a synchronized thread start. 
	static void StartAllThreads();

protected:
	/// Entry point for the thread. Override the function in the derived class. 
	virtual unsigned long Process (void* parameter) = 0;	

private:
	ThreadWin(const ThreadWin&);
	ThreadWin& operator=(const ThreadWin&);

	/// @see DelegateThread::DispatchDelegate
	virtual void DispatchDelegate(DelegateLib::DelegateMsgBase* msg);

	/// The thread start routine. 
	/// @param[in] threadParam - the thread data passed into the function. 
	static int RunProcess (void* threadParam);

	/// A thread name. 
	const CHAR* THREAD_NAME;

	/// A maximum thread timeout constant in mS.
	static const DWORD MAX_WAIT_TIME = 60000;

	/// The worker thread ID.
	DWORD m_threadId;

	/// The handle to the worker thread.
	HANDLE m_hThread;

	/// TRUE if thread uses a synchronized start.
	const BOOL SYNC_START; 

	/// A synchronization handle to start all threads. 
	static HANDLE m_hStartAllThreads;

	/// Pointer to the instance that created the m_hStartAllThreads handle.
	static ThreadWin* m_allocatedStartAllThreads;

	/// A handle to signal that the thread is created and waiting to start
	HANDLE m_hThreadStarted;

	/// A handle to signal that the thread process function has exited.
	HANDLE m_hThreadExited;
};

#endif

#endif 