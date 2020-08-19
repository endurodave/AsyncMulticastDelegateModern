#ifndef _THREAD_STD_H
#define _THREAD_STD_H

#include "DelegateOpt.h"
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

	virtual void DispatchDelegate(std::shared_ptr<DelegateLib::DelegateMsgBase> msg);

private:
	WorkerThread(const WorkerThread&) = delete;
	WorkerThread& operator=(const WorkerThread&) = delete;

	/// Entry point for the thread
	void Process();

	std::unique_ptr<std::thread> m_thread;
	std::queue<std::shared_ptr<ThreadMsg>> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	const CHAR* THREAD_NAME;
};

#endif 

