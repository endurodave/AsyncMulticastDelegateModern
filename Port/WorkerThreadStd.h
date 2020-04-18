#ifndef _THREAD_STD_H
#define _THREAD_STD_H

#include "DelegateOpt.h"
#if USE_STD_THREADS

#include "DelegateThread.h"
#include "DataTypes.h"
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

class ThreadMsg;

class WorkerThread : public DelegateLib::DelegateThread
{
public:
	/// Constructor
	WorkerThread(const CHAR* threadName);

	/// Destructor
	~WorkerThread();

	/// Called once to create the worker thread
	/// @return TRUE if thread is created. FALSE otherise. 
	BOOL CreateThread();

	/// Called once a program exit to exit the worker thread
	void ExitThread();

	/// Get the ID of this thread instance
	std::thread::id GetThreadId();

	/// Get the ID of the currently executing thread
	static std::thread::id GetCurrentThreadId();

	virtual void DispatchDelegate(DelegateLib::DelegateMsgBase* msg);

private:
	WorkerThread(const WorkerThread&);
	WorkerThread& operator=(const WorkerThread&);

	/// Entry point for the thread
	void Process();

	std::thread* m_thread;
	std::queue<ThreadMsg*> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	const CHAR* THREAD_NAME;
};

#endif 

#endif
